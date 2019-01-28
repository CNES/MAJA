#! /usr/bin/env python
# -*- coding: utf-8 -*-
"""
Processes a Sentinel-2 time series for a tile using MAJA processor for atmospheric correction and cloud screening.

MAJA was developped by CS-SI, under a CNES contract, using a multi-temporal method developped at CESBIO, for the MACCS processor and including methods developped by DLR for ATCOR.

This tool, developped by O.Hagolle (CNES:CESBIO) is a very basic one to show how to use MAJA to process a time series. If anything does not go as anticipated, the tool will probably crash

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
logger = logging.getLogger('Start-Maja')

logger.setLevel(logging.DEBUG)
if not logger.handlers:
    ch = logging.StreamHandler()
    ch.setLevel(logging.DEBUG)
    formatter = logging.Formatter('%(asctime)s - %(name)s - %(levelname)s - %(message)s')
    ch.setFormatter(formatter)
    logger.addHandler(ch)
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

    repCode = repWork = repL1= repL2 = repMaja = repCAMS = repCAMS_raw = None

    with file(fic_txt, 'r') as f:
        for ligne in f.readlines():
            if ligne.find('repCAMS') == 0:
                repCAMS = (ligne.split('=')[1]).strip()
            if ligne.find('repCAMS_raw') == 0:
                repCAMS_raw = (ligne.split('=')[1]).strip()

    missing = False
    if repCAMS is None:
        logger.debug("repCAMS is missing from configuration file. Needed : repCode, repWork, repL1, repL2, repMaja")
    if repCAMS_raw is None:
        logger.debug("repCAMS_raw is missing from configuration file. Needed : repCode, repWork, repL1, repL2, repMaja")

    if missing:
        raise Exception("Configuration file is not complete. See log file for more information.")

    directory_missing = False


    if repCAMS is not None and not os.path.isdir(repCAMS):
        logger.error("repCAMS %s is missing", repCAMS)
    if repCAMS_raw is not None and not os.path.isdir(repCAMS_raw):
        logger.error("repCAMS %s is missing", repCAMS_raw)

    if directory_missing:
        raise Exception("One or more directories are missing. See log file for more information.")

    return repCAMS, repCAMS_raw


def manage_rep_cams(repCams, repCamsRaw, working_dir):
    exocam_creation(repCamsRaw, out_dir=repCams, working_dir=repCams)
    return repCams

def exocam_creation(input_dir, out_dir=None, working_dir="/tmp"):

    processed_dates = []

    nb_files = len(myGlob(input_dir, "*.nc"))
    compteur = 1

    for file_cams in myGlob(input_dir, "*.nc"):
        date_file = get_date(file_cams)
        if date_file in processed_dates:
            continue
        #print("Processing {}/{} : {} ".format(compteur, nb_files/3,date_file), end='\r')
        date_written_in_file = back_to_filename_date(date_file)
        aot_file = searchOneFile(input_dir, "*AOT_{}*".format(date_written_in_file))
        mr_file = searchOneFile(input_dir, "*MR_{}*".format(date_written_in_file))
        rh_file = searchOneFile(input_dir, "*RH_{}*".format(date_written_in_file))
        process_one_file(aot_file, mr_file, rh_file, out_dir, working_dir)

        processed_dates.append(date_file)
        compteur += 1

if __name__ == '__main__':
    # ========== command line
    if len(sys.argv) == 1:
        prog = os.path.basename(sys.argv[0])
        print '      ' + sys.argv[0] + ' [options]'
        print "     Aide : ", prog, " --help"
        print "        ou : ", prog, " -h"

        print "exemple : "
        print "\t python %s -f folders.txt -c nominal -t 40KCB -s Reunion  -d 20160401 " % sys.argv[0]
        sys.exit(-1)
    else:
        usage = "usage: %prog [options] "
        parser = OptionParser(usage=usage, version='%prog {}'.format(START_MAJA_VERSION))



        parser.add_option("-f", "--folder", dest="folder_file", action="store", type="string",
                          help="folder definition file", default=None)

        (options, args) = parser.parse_args()



    # =================directories
    folder_file = options.folder_file
    (repCams, repCamsRaw) = read_folders(folder_file)
    repCams = manage_rep_cams(repCams, repCamsRaw, repCams)
