#! /usr/bin/env python
# -*- coding: utf-8 -*-
"""
Tool to create the DTM files expected by MAJA

Author:         Olivier Hagolle <olivier.hagolle@cnes.fr>
Project:        StartMaja, CNES

==================== Copyright
Software (tuilage_mnt_eau_S2.py)

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
import sys, os
assert sys.version_info >= (2,7)
sys.path.append(os.path.join(os.path.dirname(os.path.realpath(__file__)), '..')) #Import relative modules

from prepare_dtm import lib_mnt

class TuilageParamsConverter(object):
    """
    Class to create the class storing the tile- and path-parameters
    """
    def getSiteFromParams(self, name, proj, EPSG, pasX, pasY, origX, origY):
        """
        Create the tile-parameters internally using variables
        """
        tx_min, ty_min, tx_max, ty_max, marge = 0,0,0,0,0
        return lib_mnt.classe_site(name, proj, EPSG, "EPSG:"+str(EPSG),
                                       tx_min, ty_min, tx_max, ty_max,
                                       pasX, pasY, marge, origX, origY)
        
    def getSiteFromFile(self, filename):
        """
        Create the TileParameters reading a file
        """
        return lib_mnt.lire_fichier_site(filename)
    
    def getPathsFromFile(self, filename):
        """
        Create the Path Parameters reading a file
        """        
        return lib_mnt.lire_param_txt(filename)

class TuilageSentinel(object):
    """
    Reprojette et decoupe un mnt SRTM sur les tuiles d'un site 
    Les paramètres sont dans parametres.py, dont le nom du site qui sert à déterminer le fichier de paramètres du tuilage d'un site (ex pyrenees.py)
    
    """    
    def unzip_water(self, dirInWater, filenames, dirOut):
        """
        Unzip Water-SWBD files
        """
        import zipfile
        import re
        files = []
        for pattern in filenames:
            files += list(os.path.join(dirInWater, f) for f in os.listdir(dirInWater) if re.search(pattern + ".*?\.zip", f))
        if(len(files) == 0):
            raise OSError("Cannot find SWBD zip files!")
        for fn in files:
            print("Unzipping {0}".format(fn))
            zip_ref = zipfile.ZipFile(fn, 'r')
            zip_ref.extractall(dirOut)
            zip_ref.close()
        return 0
    
    def run(self, dirInSRTM, dirInWater, dirOut, dirOutWater, resolution, site, mnt, waterOnly, wdir = None, water_zipped = False):
        from math import ceil, floor
        from osgeo import osr
        os.environ['LC_NUMERIC'] = 'C'
        # lecture du fichier de paramètres et du fichier site
        (rep_mnt_in, rep_mnt, rep_swbd, rep_eau) = (dirInSRTM, dirOut, dirInWater, dirOutWater)
        print(rep_mnt_in, rep_mnt, rep_swbd, rep_eau)
        SRTM_RES = 90
        # ==========création de la liste des fichiers planet_observer
        # conversion des coordonnées des coins en lat_lon
        latlon = osr.SpatialReference()
        latlon.SetWellKnownGeogCS("WGS84")
        proj_site = osr.SpatialReference()
        proj_site.ImportFromEPSG(site.EPSG_out)
        transform = osr.CoordinateTransformation(proj_site, latlon)
        
        # recherche des 4 coins du site
        ulx_site = site.orig_x + site.tx_min * site.pas_x  # upper left
        uly_site = site.orig_y + site.ty_max * site.pas_y
        lrx_site = site.orig_x + (site.tx_max + 1) * site.pas_x + site.marge  # lower left
        lry_site = site.orig_y + (site.ty_min - 1) * site.pas_y - site.marge
        
        ul_latlon = transform.TransformPoint(ulx_site, uly_site, 0)
        lr_latlon = transform.TransformPoint(lrx_site, lry_site, 0)
        
        liste_fic_mnt = []
        
        ############# MNT SRTM du CGIAR
        if mnt == "SRTM":
            # liste des fichiers SRTM nécessaires
            if (ul_latlon[1]) > 60 or (lr_latlon[1] > 60):
                print("#################################################")
                print("latitude supérieure à 60 degrés, pas de donnees SRTM")
                print("#################################################")
                sys.exit(-3)
        
            ul_latlon_srtm = [int(int(ul_latlon[0] + 180) / 5 + 1), int(int(60 - ul_latlon[1]) / 5 + 1)]
            lr_latlon_srtm = [int(int(lr_latlon[0] + 180) / 5 + 1), int(int(60 - lr_latlon[1]) / 5 + 1)]
            print(ul_latlon_srtm)
            print(lr_latlon_srtm)
            for x in range(ul_latlon_srtm[0], lr_latlon_srtm[0] + 1):
                for y in range(ul_latlon_srtm[1], lr_latlon_srtm[1] + 1):
                    liste_fic_mnt.append("srtm_%02d_%02d.tif" % (x, y))
        
            print(ul_latlon, lr_latlon)
            print(ul_latlon_srtm, lr_latlon_srtm)
            print(liste_fic_mnt)
        
        ########## MNT Planet Observer
        elif mnt == "PO":
        
            ul_latlon_po = [int(floor(ul_latlon[0])), int(floor(ul_latlon[1]))]
            lr_latlon_po = [int(floor(lr_latlon[0])), int(floor(lr_latlon[1]))]
        
            for x in range(ul_latlon_po[0], lr_latlon_po[0] + 1):
                for y in range(lr_latlon_po[1], ul_latlon_po[1] + 1):
                    if x >= 0:
                        ew = "e"
                        num_x = x
                    else:
                        ew = "w"
                        num_x = -x
                    if y >= 0:
                        ns = "n"
                        num_y = y
                    else:
                        ns = "s"
                        num_y = -y
                    liste_fic_mnt.append("%s%03d/%s%02d.dt1" % (ew, num_x, ns, num_y))
        
            print(ul_latlon, lr_latlon)
            print(ul_latlon_po, lr_latlon_po)
            print(liste_fic_mnt)
        
        # liste des fichiers SWBD nécessaires
        ul_latlon_swbd = [int(floor(ul_latlon[0])), int(floor(ul_latlon[1]))]
        lr_latlon_swbd = [int(floor(lr_latlon[0])), int(floor(lr_latlon[1]))]
        print(ul_latlon, lr_latlon)
        print(ul_latlon_swbd, lr_latlon_swbd)
        
        calcul_masque_eau_mnt = 0
        if (ul_latlon[1]) > 60 or (lr_latlon[1] > 60):
            print("#################################################")
            print("latitude supérieure à 60 degrés, pas de donnees SRTM")
            print("le masque d'eau est généré à partir du MNT")
            print("#################################################")
            calcul_masque_eau_mnt = 1
        
        liste_fic_eau = []
        liste_centre_eau = []
        for x in range(ul_latlon_swbd[0], lr_latlon_swbd[0] + 1):
            for y in range(lr_latlon_swbd[1], ul_latlon_swbd[1] + 1):
                if x >= 0:
                    ew = "e"
                    num_x = x
                else:
                    ew = "w"
                    num_x = -x
                if y >= 0:
                    ns = "n"
                    num_y = y
                else:
                    ns = "s"
                    num_y = -y
        
                liste_fic_eau.append("%s%03d%s%02d" % (ew, num_x, ns, num_y))
                liste_centre_eau.append([x + 0.5, y + 0.5])
                
        print("longitudes", ul_latlon_swbd[0], lr_latlon_swbd[0])
        print("latitudes", lr_latlon_swbd[1], ul_latlon_swbd[1])
        print("center coordinates", liste_centre_eau)
        print(liste_fic_eau)

        import tempfile
        print(wdir)
        if wdir is None:
            working_dir = tempfile.mkdtemp()
        else:
            working_dir = tempfile.mkdtemp(dir=wdir)
        # Fusion des mnt_srtm en un seul
        (fic_mnt_in, fic_eau_in) = lib_mnt.fusion_mnt(liste_fic_mnt, liste_fic_eau, liste_centre_eau, rep_mnt_in, rep_swbd, site.nom,
                                              calcul_masque_eau_mnt, working_dir=working_dir)
        print("############", fic_mnt_in)
        
        ####################Boucle de création des fichiers MNT et eau pour chaque tuile
        
        for tx in range(site.tx_min, site.tx_max + 1):
            for ty in range(site.ty_min, site.ty_max + 1):
                ulx = site.orig_x + tx * site.pas_x  # upper left
                uly = site.orig_y + ty * site.pas_y
                lrx = site.orig_x + (tx + 1) * site.pas_x + site.marge  # lower left
                lry = site.orig_y + (ty - 1) * site.pas_y - site.marge
        
                lrx_90m = int(ceil((lrx - ulx) / float(SRTM_RES))) * SRTM_RES + ulx
                lry_90m = uly - int(ceil((uly - lry) / float(SRTM_RES))) * SRTM_RES
        
                lrx_coarse = int(ceil((lrx - ulx) / float(resolution))) * resolution + ulx
                lry_coarse = uly - int(ceil((uly - lry) / float(resolution))) * resolution
        
                nom_tuile = site.nom
        
                print("nom de la tuile", nom_tuile, tx, ty)
                ###pour le MNT
                rep_mnt_out = working_dir
                print("MNT Reps: {0} {1} {2}".format(rep_mnt, rep_mnt_in, rep_mnt_out))

                if waterOnly == False:
                    # Resolution SRTM_RES
                    print("############### c'est parti")
                    mnt_90m = lib_mnt.classe_mnt(rep_mnt_out, nom_tuile, ulx, uly, lrx_90m, lry_90m, SRTM_RES, site.chaine_proj)
                    mnt_90m.decoupe(fic_mnt_in)
        
                    # calcul du gradient à 90m
                    (fic_dz_dl_srtm, fic_dz_dc_srtm) = mnt_90m.calcul_gradient()
        
                    # Basse résolution
        
                    mnt_coarse = lib_mnt.classe_mnt(rep_mnt_out, nom_tuile, ulx, uly, lrx_coarse, lry_coarse, resolution,
                                            site.chaine_proj)
                    mnt_coarse.decoupe(fic_mnt_in)
                    mnt_coarse.reech_gradient(fic_dz_dl_srtm, fic_dz_dc_srtm)
                    mnt_coarse.calcul_pente_aspect_fic()
        
                    # Haute résolution 10 et 20m
                    for full_res in [10, 20]:
                        mnt_full = lib_mnt.classe_mnt(rep_mnt_out, nom_tuile, ulx, uly, lrx, lry, full_res, site.chaine_proj)
                        mnt_full.decoupe(fic_mnt_in)
                        mnt_full.reech_gradient(fic_dz_dl_srtm, fic_dz_dc_srtm)
                        mnt_full.calcul_pente_aspect_fic()
        
                ### Pour l'eau
                rep_eau_out = os.path.join(rep_eau,nom_tuile)
                if not (os.path.exists(rep_eau_out)):
                    os.mkdir(rep_eau_out)
        
                eau = lib_mnt.classe_mnt(rep_mnt_out, nom_tuile, ulx, uly, lrx_coarse, lry_coarse, resolution, site.chaine_proj)
                if calcul_masque_eau_mnt == 0:
                    eau.decoupe_eau(fic_eau_in)
                else:
                    eau.calcul_masque_mnt(rep_mnt_out, nom_tuile)

import optparse
###########################################################################
class OptionParser(optparse.OptionParser):

    def check_required(self, opt):
        option = self.get_option(opt)

        # Assumes the option's 'default' is set to None!
        if getattr(self.values, option.dest) is None:
            self.error("%s option not supplied" % option)
 
if __name__ == "__main__":
    if len(sys.argv) == 1:
        prog = os.path.basename(sys.argv[0])
        print('      ' + sys.argv[0] + ' [options]')
        print("     Aide : ", prog, " --help")
        print("        ou : ", prog, " -h")
        print("example : python %s -p parametres_srtm.txt -s 32SNE.txt -m SRTM -c 240" % sys.argv[0])
        sys.exit(-1)
    else:
        usage = "usage: %prog [options] "
        parser = OptionParser(usage=usage)
        parser.set_defaults(eau_seulement=False)
        parser.set_defaults(sans_numero=False)
    
        parser.add_option("-p", "--parametre", dest="fic_param", action="store", type="string", \
                          help="fichier de parametre", default=None)
        parser.add_option("-s", "--site", dest="fic_site", action="store", type="string", \
                          help="fichier de description du site, or granule ID if --kml is used", default=None)
        parser.add_option("-k", "--kml", action="store", type="string", \
                          help="S2 Tiling grid KML file", default=None)
        parser.add_option("-m", "--mnt", dest="mnt", action="store", type="choice", \
                          help="SRTM ou PO (Planet Observer)", choices=['SRTM', 'PO'], default=None)
        parser.add_option("-c", dest="COARSE_RES", action="store", type="int", \
                          help="Coarse resolution", default=240)
        parser.add_option("-e", dest="eau_seulement", action="store_true", \
                          help="Traitement des masques d'eau seulement")
        parser.add_option("-n", dest="sans_numero", action="store_true", \
                          help="Traitement sans numero de tuile")
    
        (options, args) = parser.parse_args()
        parser.check_required("-p")
        parser.check_required("-s")
        parser.check_required("-m")
        converter = TuilageParamsConverter()
        dirInSRTM, dirOut, dirInWater, dirOutWater = converter.getPathsFromFile(options.fic_param)
        if options.kml:
            site = lib_mnt.lire_fichier_site_kml(options.kml, options.fic_site)
        else:
            site = converter.getSiteFromFile(options.fic_site)
        mntcreator = TuilageSentinel()
        mntcreator.run(dirInSRTM, dirInWater, dirOut, dirOutWater, options.COARSE_RES, site=site, mnt=options.mnt, waterOnly=options.eau_seulement, water_zipped=False)
