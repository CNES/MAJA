#! /usr/bin/env python
# -*- coding: utf-8 -*-

"""
Processes a Sentinel-2 time series for a tile using MAJA processor for atmospheric correction and cloud screening.

MAJA was developped by CS-SI, under a CNES contract, using a multi-temporal method developped at CESBIO, for the MACCS processor and including methods developped by DLR for ATCOR.

This tool, developped by O.Hagolle (CNES:CESBIO) is a very basic one to show how to use MAJA to process a time series.

==================== Copyright
Software (start_maja.py)

Copyright© 2018 Centre National d’Etudes Spatiales

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License version 3
as published by the Free Software Foundation.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this program.  If not, see
https://www.gnu.org/licenses/gpl-3.0.fr.html
"""

import glob
import tempfile
import optparse
import os
import os.path
import shutil
import sys
import logging
import zipfile


START_MAJA_VERSION = 3.1

# #########################################################################


class OptionParser(optparse.OptionParser):

    def check_required(self, opt):
        option = self.get_option(opt)

        # Assumes the option's 'default' is set to None!
        if getattr(self.values, option.dest) is None:
            self.error("%s option not supplied" % option)


# #################################### Lecture de fichier de parametres "Key=Value"
def read_folders(fic_txt):

    repCode = repWork = repL1 = repL2 = repMaja = repCAMS = repCAMS_raw = None

    with file(fic_txt, 'r') as f:
        for ligne in f.readlines():
            if ligne.find('repCode') == 0:
                repCode = (ligne.split('=')[1]).strip()
            if ligne.find('repWork') == 0:
                repWork = (ligne.split('=')[1]).strip()
            if ligne.find('repL1') == 0:
                repL1 = (ligne.split('=')[1]).strip()
            if ligne.find('repL2') == 0:
                repL2 = (ligne.split('=')[1]).strip()
            if ligne.find('repMaja') == 0:
                repMaja = (ligne.split('=')[1]).strip()
            if ligne.find('repCAMS') == 0:
                repCAMS = (ligne.split('=')[1]).strip()

    missing = False

    if repCode is None:
        logger.error(
            "repCode is missing from configuration file. Needed : repCode, repWork, repL1, repL2, repMaja")
        missing = True
    if repWork is None:
        logger.error(
            "repWork is missing from configuration file. Needed : repCode, repWork, repL1, repL2, repMaja")
        missing = True
    if repL1 is None:
        logger.error(
            "repL1 is missing from configuration file. Needed : repCode, repWork, repL1, repL2, repMaja")
        missing = True
    if repL2 is None:
        logger.error(
            "repL2 is missing from configuration file. Needed : repCode, repWork, repL1, repL2, repMaja")
        missing = True
    if repMaja is None:
        logger.error(
            "repCode is missing from configuration file. Needed : repCode, repWork, repL1, repL2, repMaja")
        missing = True
    if repCAMS is None:
        logger.info(
            "repCAMS is missing from configuration file. Needed : repCode, repWork, repL1, repL2, repMaja")
        logger.info("Processing without CAMS")

    if missing:
        raise Exception("Configuration file is not complete. See log file for more information.")

    directory_missing = False

    if not os.path.isdir(repCode):
        logger.error("repCode %s is missing", repCode)
        directory_missing = True
    if not os.path.isdir(repWork):
        logger.error("repWork %s is missing", repWork)
        directory_missing = True
    if not os.path.isdir(repL1):
        logger.error("repL1 %s is missing", repL1)
        directory_missing = True
    if not os.path.isdir(repL2):
        logger.error("repL2 %s is missing", repL2)
        directory_missing = True
    if not os.path.isfile(repMaja):
        logger.error("repMaja %s is missing", repMaja)
        directory_missing = True
    if repCAMS is not None and not os.path.isdir(repCAMS):
        logger.error("repCAMS %s is missing", repCAMS)
    if repCAMS_raw is not None and not os.path.isdir(repCAMS_raw):
        logger.error("repCAMS %s is missing", repCAMS_raw)

    if directory_missing:
        raise Exception("One or more directories defined in foldersconfig files are missing")

    sys.path.insert(0, repCode+"/cams_download")
    from convert_CAMS_DBL import exocam_creation

    return repCode, repWork, repL1, repL2, repMaja, repCAMS, repCAMS_raw


# =============== Module to copy and link files

# replace tile name in example files
def replace_tile_name(fic_in, fic_out, tile_in, tile_out):
    with file(fic_in) as f_in:
        with file(fic_out, "w") as f_out:
            lignes = f_in.readlines()
            for l in lignes:
                if l.find(tile_in) > 0:
                    l = l.replace(tile_in, tile_out)
                f_out.write(l)


def add_parameter_files(repGipp, repWorkIn, tile, repCams):

    for fic in glob.glob(repGipp + "/*"):
        base = os.path.basename(fic)
        if fic.find("36JTT") > 0:
            replace_tile_name(fic, repWorkIn + '/' + base.replace("36JTT", tile), "36JTT", tile)
        else:
            logger.debug("Linking %s to %s", fic, repWorkIn + '/' + base)
            if not os.path.exists(repWorkIn + '/' + base):
                os.symlink(fic, os.path.join(repWorkIn, base))

    # links for CAMS files
    if repCams is not None:
        for fic in glob.glob(os.path.join(repCams, "*")):
            base = os.path.basename(fic)
            #logger.debug("Linking %s in %s", fic, repWorkIn)
            if not os.path.exists(repWorkIn + '/' + base):
                os.symlink(fic, os.path.join(repWorkIn, base))


def add_DEM(repDEM, repWorkIn, tile):
    logger.debug("%s/*%s*/*", repDEM, tile)
    for fic in glob.glob(repDEM + "/S2_*%s*/*" % tile):
        base = os.path.basename(fic)
        if not os.path.exists(repWorkIn + '/' + base):
            os.symlink(fic, os.path.join(repWorkIn, base))


def add_config_files(repConf, repWorkConf):
    if not os.path.exists(repWorkConf):
        os.symlink(repConf, repWorkConf)


def manage_rep_cams(repCams, repCamsRaw, working_dir):
    if repCamsRaw is not None:
        # convert nc to exocams
        if repCams is not None:
            logger.warning("Exo cams dir and exo cams dir all ")
        working_directory = tempfile.mkdtemp(suffix="ConvertToExo_temp", dir=working_dir)
        repCams_out = tempfile.mkdtemp(suffix="ConvertToExo_out", dir=working_dir)
        exocam_creation(repCamsRaw, out_dir=repCams_out, working_dir=working_directory)
        return repCams_out

    return repCams


def unzipAndMoveL1C(L1Czipped, workdir, tile):
    # unzip L1C file
    try:
        with zipfile.ZipFile(L1Czipped, 'r') as zip_ref:
            safeDir = zip_ref.namelist()[0]
            zip_ref.extractall(workdir)
    except IOError:
        print("L1C zip file %s is not readable" % L1Czipped)
        sys.exit(-1)

    return


def test_valid_L2A(L2A_DIR):
    # test validity of a Level2A product of MUSCATE type
    JPIfile = glob.glob("%s/DATA/*_JPI_ALL.xml" % L2A_DIR)[0]
    valid = True
    try:
        with open(JPIfile) as f:
            for ligne in f:
                if ligne.find("<Value>L2NOTV</Value>") >= 0:
                    valid = False
                    prod_name = os.path.basename(L2A_DIR)
                    dir_name = os.path.dirname(L2A_DIR)
                    if not(os.path.exists(dir_name+"/L2NOTV_"+prod_name)):
                        os.rename(L2A_DIR, dir_name+"/L2NOTV_"+prod_name)
                    else:
                        shutil.rmtree(dir_name+"/L2NOTV_"+prod_name)
                        os.rename(L2A_DIR, dir_name+"/L2NOTV_"+prod_name)
                    print("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!")
                    print(
                        "L2A product %s is not valid (probably due to too many clouds or No_data values)" % dir_name)
                    print("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!")

    except IOError:
        valid = False
        print("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!")
        print("L2A product %s not found " % L2A_DIR)
        print("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!")

    return(valid)


def start_maja(folder_file, context, site, tile, orbit, nb_backward, options, debug_mode):
    # =================directories
    (repCode, repWork, repL1, repL2, maja, repCams, repCamsRaw) = read_folders(folder_file)

    repCams = manage_rep_cams(repCams, repCamsRaw, repWork)

    repConf = repCode + "/userconf"
    if not(os.path.exists(repConf)):
        logger.error("Config dir %s does not exist", repConf)
        sys.exit(-1)
    repDtm = repCode + "/DTM"
    if not(os.path.exists(repDtm)):
        logger.error("DTM dir %s does not exist", repDtm)
        sys.exit(-1)
    repGipp = repCode + "/%s" % context
    if not(os.path.exists(repGipp)):
        logger.error("GIPP dir %s does not exist", repGipp)
        sys.exit(-1)

    repWork = "%s/%s/%s/%s/" % (repWork, site, tile, context)
    if not (os.path.exists(repWork)):
        try:
            os.makedirs(repWork)
        except:
            logger.error("something wrong when creating %s", repWork)
            sys.exit(-1)
    repL1 = "%s/%s/" % (repL1, site)
    repL2 = "%s/%s/%s/%s/" % (repL2, site, tile, context)

    # check existence of folders
    for fic in repL1, repCode, repWork, maja:
        if not (os.path.exists(fic)):
            logger.error("ERROR : %s does not exist", fic)
            sys.exit(-1)

    if not os.path.exists(repL2):
        os.makedirs(repL2)

    if options.zip:
        if orbit is not None:
            listeProd = glob.glob(repL1 + "/S2?_MSIL1C*%s_T%s*.zip" % (orbit, tile))
        else:
            listeProd = glob.glob(repL1 + "/S2?_MSIL1C*_T%s*.zip" % (tile))
    elif orbit is not None:
        listeProd = glob.glob(repL1 + "/S2?_OPER_PRD_MSIL1C*%s_*.SAFE/GRANULE/*%s*" % (orbit, tile))
        listeProd = listeProd + \
            glob.glob(repL1 + "/S2?_MSIL1C*%s_*.SAFE/GRANULE/*%s*" % (orbit, tile))
    else:
        listeProd = glob.glob(repL1 + "/S2?_OPER_PRD_MSIL1C*.SAFE/GRANULE/*%s*" % (tile))
        listeProd = listeProd + glob.glob(repL1 + "/S2?_MSIL1C*.SAFE/GRANULE/*%s*" % (tile))

    logger.debug("Liste prod %s", listeProd)
    # list of images to process
    dateProd = []
    dateImg = []
    listeProdFiltree = []

    if len(listeProd) == 0:
        if options.zip:
            logger.error("No L1C product found in %s" %
                         (repL1 + "/S2?_MSIL1C*%s_T%s*.zip" % (orbit, tile)))

        elif orbit is not None:
            logger.error("No L1C product found in %s or %s",
                         repL1 + "/S2?_OPER_PRD_MSIL1C*%s_*.SAFE/GRANULE/*%s*" % (orbit, tile),
                         repL1 + "/S2?_MSIL1C*%s_*.SAFE/GRANULE/*%s*" % (orbit, tile))
        else:
            logger.error("No L1C product found in %s or %s",
                         repL1 + "/S2?_OPER_PRD_MSIL1C*.SAFE/GRANULE/*%s*" % (tile),
                         repL1 + "/S2?_MSIL1C*.SAFE/GRANULE/*%s*" % (tile))
        sys.exit(-3)

    for elem in listeProd:
        if options.zip:
            rac = elem.split("/")[-1]
        else:
            rac = elem.split("/")[-3]
            elem = '/'.join(elem.split("/")[0:-2])
        logger.debug("elem: %s", elem)
        rac = os.path.basename(elem)
        logger.debug("rac: %s", rac)

        if rac.startswith("S2A_OPER_PRD_MSIL1C") or rac.startswith("S2B_OPER_PRD_MSIL1C"):
            date_asc = rac.split('_')[7][1:9]
        else:
            date_asc = rac.split('_')[2][0:8]
        logger.debug("date_asc %s %s %s/%s", date_asc, date_asc >=
                     options.startDate, date_asc, options.startDate)
        if date_asc >= options.startDate and date_asc <= options.endDate:
            dateImg.append(date_asc)
            if rac.startswith("S2A_OPER_PRD_MSIL1C") or rac.startswith("S2B_OPER_PRD_MSIL1C"):
                dateProd.append(rac.split('_')[5])
            else:
                dateProd.append(rac.split('_')[6])
            listeProdFiltree.append(elem)

    # removing multiple images with same date and tile
    logger.debug("date img %s", dateImg)
    logger.debug("set %s", set(dateImg))

    dates_diff = list(set(dateImg))
    if len(dates_diff) == 0:
        logger.error("there is no date to process")
        sys.exit(-2)
    dates_diff.sort()

    prod_par_dateImg = {}
    nomL2_par_dateImg_Natif = {}
    nomL2_par_dateImg_MUSCATE = {}
    for d in dates_diff:
        nb = dateImg.count(d)

        dpmax = ""
        ind = -1
        # search the most recent production date
        for i in range(0, nb):
            ind = dateImg.index(d, ind + 1)
            dp = dateProd[ind]
            if dp > dpmax:
                dpmax = dp

        # keep only the products with the most recent date
        ind = dateProd.index(dpmax)
        logger.debug("date prod max %s index in list %s", dpmax, ind)
        prod_par_dateImg[d] = listeProdFiltree[ind]
        nomL2_par_dateImg_Natif[d] = "S2?_OPER_SSC_L2VALD_%s____%s.DBL.DIR" % (tile, d)
        nomL2_par_dateImg_MUSCATE[d] = "SENTINEL2?_%s-*_T%s_C_V*" % (d, tile)
        logger.debug("d %s, prod_par_dateImg[d] %s", d, prod_par_dateImg[d])

    print("\n")
    # find the first image to process

    logger.debug("dates_diff %s", dates_diff)

    derniereDate = ""
    for d in dates_diff:
        logger.debug("d %s", d)
        logger.debug("%s/%s", repL2, nomL2_par_dateImg_Natif[d])
        logger.debug("%s/%s", repL2, nomL2_par_dateImg_MUSCATE[d])
        #logger.debug(glob.glob("%s/%s" % (repL2, nomL2_par_dateImg_Natif[d])))
        #logger.debug(glob.glob("%s/%s" % (repL2, nomL2_par_dateImg_MUSCATE[d])))

        # test existence of a L2 with MAJA name convention
        L2type = None
        nomL2init_Natif = glob.glob("%s/%s" % (repL2, nomL2_par_dateImg_Natif[d]))
        nomL2init_MUSCATE = glob.glob("%s/%s" % (repL2, nomL2_par_dateImg_MUSCATE[d]))
        if len(nomL2init_Natif) > 0:
            derniereDate = d
            L2type = "Natif"

        elif len(nomL2init_MUSCATE) > 0:
            L2type = "MUSCATE"
            derniereDate = d

    if derniereDate == "":
        logger.info("No existing L2 product, we start with backward mode")
    else:
        logger.info("Most recent processed date : %s", derniereDate)

    # decide if debug_mode used for maja
    if debug_mode:
        debug_option = "--loglevel DEBUG"
    else:
        debug_option = ""

    # ############## For each product
    nb_dates = len(dates_diff)

    logger.debug("nb dates %s", nb_dates)

    if not (os.path.exists(repWork)):
        os.makedirs(repWork)
    if not (os.path.exists(repWork + "userconf")):
        #logger.debug("create %s userconf %s", repWork)
        add_config_files(repConf, repWork + "userconf")

    logger.debug("derniereDate %s", derniereDate)
    for i in range(nb_dates):
        d = dates_diff[i]
        # only products after the last L2A date available in output directory
        if d > derniereDate:
            logger.info("=> processing date %s" % d)
            if os.path.exists(repWork + "/in"):
                shutil.rmtree(repWork + "/in")
            os.makedirs(repWork + "/in")
            # Mode Init
            if options.initMode is True:
                logger.info("Processing in init mode ")
                add_parameter_files(repGipp, repWork + "/in/", tile, repCams)
                add_DEM(repDtm, repWork + "/in/", tile)
                # copy (or symlink) L1C
                if options.zip:
                    unzipAndMoveL1C(prod_par_dateImg[d], repWork + "/in/", tile)
                else:
                    if not os.path.exists(repWork + "/in/" + os.path.basename(prod_par_dateImg[d])):
                        os.symlink(prod_par_dateImg[d],
                                   repWork + "/in/" + os.path.basename(prod_par_dateImg[d]))
                Maja_logfile = "%s/%s.log" % (repL2, os.path.basename(prod_par_dateImg[d]))
                logger.debug(os.listdir(os.path.join(repWork, "in")))
                commande = "%s %s -i %s -o %s -m L2INIT -ucs %s --TileId %s > %s 2>&1" % (
                    maja, debug_option, repWork + "/in", repL2, repWork + "/userconf", tile, Maja_logfile)
                logger.info("#################################")
                logger.info("#################################")
                logger.info("processing %s in Init Mode" % prod_par_dateImg[d])
                logger.info("Init mode goes with suboptimal performances")
                logger.info("MAJA logfile: %s", Maja_logfile)
                logger.info("#################################")
                os.system(commande)

            # Mode Backward, if it is the first date in the list
            elif i == 0:
                nb_prod_backward = min(len(dates_diff), nb_backward)
                logger.info("dates to process in backward mode :")
                for date_backward in dates_diff[0:nb_prod_backward]:
                    logger.info("-- %s : %s" % (date_backward, prod_par_dateImg[date_backward]))
                    if options.zip:
                        unzipAndMoveL1C(prod_par_dateImg[date_backward], repWork + "/in/", tile)
                    else:
                        if not os.path.exists(repWork + "/in/" + os.path.basename(prod_par_dateImg[date_backward])):
                            os.symlink(prod_par_dateImg[date_backward],
                                       repWork + "/in/" + os.path.basename(prod_par_dateImg[date_backward]))
                add_parameter_files(repGipp, repWork + "/in/", tile, repCams)
                add_DEM(repDtm, repWork + "/in/", tile)

                Maja_logfile = "%s/%s.log" % (repL2, os.path.basename(prod_par_dateImg[d]))
                logger.debug(os.listdir(os.path.join(repWork, "in")))
                commande = "%s %s -i %s -o %s -m L2BACKWARD -ucs %s --TileId %s >%s 2>&1" % (
                    maja, debug_option, repWork + "/in", repL2, repWork + "/userconf", tile, Maja_logfile)
                logger.info("#################################")
                logger.info("#################################")
                logger.info("processing %s in backward mode" % prod_par_dateImg[d])
                logger.info("Initialisation mode with backward is longer")
                logger.info("MAJA logfile: %s", Maja_logfile)
                logger.debug("MAJA command: %s", commande)
                logger.info("#################################")
                os.system(commande)

            # else mode nominal
            else:
                nomL2 = ""
                # Search for previous L2 product
                if L2type is None:
                    logger.info("MAJA command failed : %s", commande)
                    logger.error(
                        "First backward processing was unsuccessful, check MAJA installation")
                    sys.exit(-1)
                logger.info("Using %s L2 type" % L2type)
                for PreviousDate in dates_diff[0:i]:
                    if L2type == "Natif":
                        nom_courant = "%s/%s" % (repL2, nomL2_par_dateImg_Natif[PreviousDate])
                    elif L2type == "MUSCATE":
                        nom_courant = "%s/%s" % (repL2, nomL2_par_dateImg_MUSCATE[PreviousDate])
                    try:
                        logger.debug(nom_courant)
                        nomL2 = glob.glob(nom_courant)[0]
                        logger.debug("Previous L2 names, per increasing date : %s", nomL2)
                    except:
                        logger.debug("pas de L2 pour : %s", nom_courant)
                        pass
                logger.info("previous L2 : %s", nomL2)
                # copy (or symlink) L1C
                if options.zip:
                    unzipAndMoveL1C(prod_par_dateImg[d], repWork + "/in/", tile)
                else:
                    if not os.path.exists(repWork + "/in/" + os.path.basename(prod_par_dateImg[d])):
                        os.symlink(prod_par_dateImg[d],
                                   repWork + "/in/" + os.path.basename(prod_par_dateImg[d]))
                # find type of L2A
                if L2type == "Natif":
                    if not os.path.exists(repWork + "/in/" + os.path.basename(nomL2)):
                        os.symlink(nomL2, repWork + "/in/" + os.path.basename(nomL2))
                    if not os.path.exists(repWork + "/in/" + os.path.basename(nomL2).replace("DBL.DIR", "HDR")):
                        os.symlink(nomL2.replace("DBL.DIR", "HDR"),
                                   repWork + "/in/" + os.path.basename(nomL2).replace("DBL.DIR", "HDR"))
                    if not os.path.exists(repWork + "/in/" + os.path.basename(nomL2).replace("DIR", "")):
                        os.symlink(nomL2.replace("DIR", ""), repWork + "/in/" +
                                   os.path.basename(nomL2).replace("DIR", ""))
                elif L2type == "MUSCATE":
                    if not os.path.exists(repWork + "/in/" + os.path.basename(nomL2)):
                        os.symlink(nomL2, repWork + "/in/" + os.path.basename(nomL2))

                Maja_logfile = "%s/%s.log" % (repL2, os.path.basename(prod_par_dateImg[d]))

                add_parameter_files(repGipp, repWork + "/in/", tile, repCams)
                add_DEM(repDtm, repWork + "/in/", tile)

                logger.debug(os.listdir(os.path.join(repWork, "in")))

                commande = "%s %s -i %s -o %s -m L2NOMINAL -ucs %s --TileId %s > %s 2>&1" % (
                    maja, debug_option, repWork + "/in", repL2, repWork + "/userconf", tile, Maja_logfile)
                logger.info("#################################")
                logger.info("#################################")
                logger.info("processing %s in nominal mode" % prod_par_dateImg[d])
                logger.info("MAJA logfile: %s", Maja_logfile)
                logger.info("#################################")
                os.system(commande)

            # check for errors in MAJA executions
            nomL2init_Natif = glob.glob("%s/%s" % (repL2, nomL2_par_dateImg_Natif[d]))
            nomL2init_MUSCATE = glob.glob("%s/%s" % (repL2, nomL2_par_dateImg_MUSCATE[d]))
            if len(nomL2init_Natif) > 0:
                L2type = "Natif"
            elif len(nomL2init_MUSCATE) > 0:
                L2type = "MUSCATE"
                # test if L2A products is valid
                valid = test_valid_L2A(nomL2init_MUSCATE[0])

            # check for errors in MAJA executions

            Error = False
            with open(Maja_logfile, "r") as logfile:
                for line in logfile:
                    if line.find("[E]") > 0:
                        print(line)
                        Error = True
            if Error:
                logger.info("#######################################")
                logger.info("Error detected, see: %s" % Maja_logfile)
                logger.info("#######################################")
                sys.exit(-1)


if __name__ == '__main__':
    # ========== command line
    if len(sys.argv) == 1:
        prog = os.path.basename(sys.argv[0])
        print('      ' + sys.argv[0] + ' [options]')
        print("     Aide : ", prog, " --help")
        print("        ou : ", prog, " -h")

        print("exemple : ")
        print("\t python %s -f folders.txt -c nominal -t 40KCB -s Reunion  -d 20160401 " %
              sys.argv[0])
        sys.exit(-1)
    else:
        usage = "usage: %prog [options] "
        parser = OptionParser(usage=usage, version='%prog {}'.format(START_MAJA_VERSION))

        parser.add_option("-c", "--context", dest="context", action="store",
                          help="name of the test directory", type="string", default='nominal')

        parser.add_option("-t", "--tile", dest="tile", action="store",
                          help="tile number", type="string", default='31TFJ')

        parser.add_option("-s", "--site", dest="site", action="store",
                          help="site name", type="string", default='Arles')

        parser.add_option("-o", "--orbit", dest="orbit", action="store",
                          help="orbit number", type="string", default=None)

        parser.add_option("-f", "--folder", dest="folder_file", action="store", type="string",
                          help="folder definition file", default=None)

        parser.add_option("-d", "--startDate", dest="startDate", action="store",
                          help="start date for processing (optional)", type="string", default="20150623")

        parser.add_option("-e", "--endDate", dest="endDate", action="store",
                          help="end date for processing (optional)", type="string", default="30000101")

        parser.add_option("-z", "--zip", dest="zip", action="store_true",
                          help="input L1C are zip files", default=False)

        parser.add_option("-v", "--verbose", dest="verbose", action="store_true",
                          help="Will provide verbose start_maja logs", default=False)

        parser.add_option("-i", "--initMode", dest="initMode", action="store_true",
                          help="Will launch MAJA in init mode, for tests, degraded performances", default=False)

        parser.add_option("--debug", dest="debug", action="store_true",
                          help="Use MAJA Debug mode to get verbose logs", default=False)

        (options, args) = parser.parse_args()

    # Logfile configuration

    logger = logging.getLogger('Start-Maja')
    if options.verbose:
        logger.setLevel(logging.DEBUG)
    else:
        logger.setLevel(logging.INFO)
    if not logger.handlers:
        ch = logging.StreamHandler()
        ch.setLevel(logging.INFO)
        formatter = logging.Formatter('%(asctime)s - %(name)s - %(levelname)s - %(message)s')
        ch.setFormatter(formatter)
        logger.addHandler(ch)
    logger.debug("options.stardate %s", options.startDate)

    tile = options.tile
    site = options.site
    orbit = options.orbit
    context = options.context
    folder_file = options.folder_file
    debug_mode = options.debug

    nb_backward = 8  # number of images to process in backward mode

    start_maja(folder_file, context, site, tile, orbit, nb_backward, options, debug_mode)
