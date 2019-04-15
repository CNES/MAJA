#! /usr/bin/env python
# -*- coding: utf-8 -*-
"""
Tool to create the DTM files expected by MAJA

Author:         Olivier Hagolle <olivier.hagolle@cnes.fr>
Project:        StartMaja, CNES

==================== Copyright
Software (lib_mnt.py)

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
sys.path.append(os.path.join(os.path.dirname(os.path.realpath(__file__)), '..')) #Import relative modules

import glob
import numpy as np
import os
import os.path
import shutil
import re
import tempfile
from osgeo import gdal, ogr, osr
import scipy.ndimage as nd
from os.path import join as pjoin
import urllib.request


# Returns true if coordinate is land
def TestLand(lon, lat):
    latlon = osr.SpatialReference()
    latlon.ImportFromEPSG(4326)

    # create a point

    pt = ogr.Geometry(ogr.wkbPoint)
    pt.SetPoint_2D(0, lon, lat)

    # read shapefile
    shapefile = "land_polygons_osm/simplified_land_polygons.shp"
    driver = ogr.GetDriverByName("ESRI Shapefile")
    dataSource = driver.Open(shapefile, 0)
    layer = dataSource.GetLayer()
    targetProj = layer.GetSpatialRef()
    land = False

    # conversion to shapefile projection
    transform = osr.CoordinateTransformation(latlon, targetProj)
    pt.Transform(transform)

    # search point in layers
    for feature in layer:
        geom = feature.GetGeometryRef()
        if geom.Contains(pt):
            land = True
            break

    return land


##################################### Lecture de fichier de parametres "Mot_clé=Valeur"
def lire_param_txt(fic_txt):
    with open(fic_txt, 'r') as f:
        for ligne in f.readlines():
            if ligne.find('INDIR_MNT') == 0:
                INDIR_MNT = (ligne.split('=')[1]).strip()
            if ligne.find('OUTDIR_MNT') == 0:
                OUTDIR_MNT = (ligne.split('=')[1]).strip()
            if ligne.find('INDIR_EAU') == 0:
                INDIR_EAU = (ligne.split('=')[1]).strip()
            if ligne.find('OUTDIR_EAU') == 0:
                OUTDIR_EAU = (ligne.split('=')[1]).strip()
    return (INDIR_MNT, OUTDIR_MNT, INDIR_EAU, OUTDIR_EAU)


############################
class classe_site:
    def __init__(self, nom, proj, EPSG_out, chaine_proj, tx_min, tx_max, ty_min, ty_max, pas_x, pas_y, marge, orig_x,
                 orig_y):
        self.nom = nom
        self.proj = proj
        self.EPSG_out = EPSG_out
        self.chaine_proj = chaine_proj
        self.tx_min = tx_min
        self.tx_max = tx_max
        self.ty_min = ty_min
        self.ty_max = ty_max
        self.pas_x = pas_x
        self.pas_y = pas_y
        self.marge = marge
        self.orig_x = orig_x
        self.orig_y = orig_y


########################## hacky way to extract info from S2 tiling grid kml file
def lire_fichier_site_kml(kml, nom):
    #nom=os.path.basename(fic).split('.')[0]
    with open(kml, 'r') as f:
        for line in f:
            if line.find("TILE_ID") >0  and line.find(nom) > 0:
                # extract information from kml/html
                for r in line.split("<tr>"):
                    m = re.sub("<[^>]*>","",r).split(" ",1)
                    if m[0] == "EPSG":
                        EPSG_out=int(m[1])
                        chaine_proj="EPSG:"+m[1]
                        # get UTM name
                        if EPSG_out > 32700:
                            proj="UTM"+str(EPSG_out-32700)+"S"
                        else:
                            proj="UTM"+str(EPSG_out-32600)+"N"                            
                        
                    elif m[0] == "UTM_WKT":
                        coords = re.sub(r'^.*MULTIPOLYGON\(\(\((.*)\)\)\)',"\g<1>",m[1])
                        coords = list(zip(*[[int(y) for y in x.split()] for x in coords.split(",")]))
                        # find min and max coordiantes
                        orig_x = min(coords[0])
                        pas_x = max(coords[0]) - orig_x
                        orig_y = max(coords[1])
                        pas_y = orig_y - min(coords[1])
                        tx_min = tx_max = ty_min = ty_max = 0
                        marge = 0
                print(nom)
                print(proj)
                print(EPSG_out)
                print(chaine_proj)
                print(tx_min)
                print(tx_max)
                print(ty_min)
                print(ty_max)
                print(pas_x)
                print(pas_y)
                print(marge)
                print(orig_x)
                print(orig_y)
                site=classe_site(nom,proj,EPSG_out,chaine_proj,tx_min,tx_max,ty_min,ty_max,pas_x,pas_y,marge,orig_x,orig_y)
                return(site)


############################ Lecture du fichier site
def lire_fichier_site(fic_site):
    nom = os.path.basename(fic_site).split('.')[0]
    with open(fic_site, 'r') as f:
        for ligne in f.readlines():
            if ligne.find('proj') == 0:
                proj = ligne.split('=')[1].strip()
                print(proj)
            if ligne.find('EPSG_out') == 0:
                EPSG_out = int(ligne.split('=')[1])
            if ligne.find('chaine_proj') == 0:
                chaine_proj = ligne.split('=')[1].strip()
            if ligne.find('tx_min') == 0:
                tx_min = int(ligne.split('=')[1])
            if ligne.find('tx_max') == 0:
                tx_max = int(ligne.split('=')[1])
            if ligne.find('ty_min') == 0:
                ty_min = int(ligne.split('=')[1])
            if ligne.find('ty_max') == 0:
                ty_max = int(ligne.split('=')[1])
            if ligne.find('pas_x') == 0:
                pas_x = int(ligne.split('=')[1])
            if ligne.find('pas_y') == 0:
                pas_y = int(ligne.split('=')[1])
            if ligne.find('marge') == 0:
                marge = int(ligne.split('=')[1])
            if ligne.find('orig_x') == 0:
                orig_x = int(ligne.split('=')[1])
            if ligne.find('orig_y') == 0:
                orig_y = int(ligne.split('=')[1])
    site = classe_site(nom, proj, EPSG_out, chaine_proj, tx_min, tx_max, ty_min, ty_max, pas_x, pas_y, marge, orig_x,
                       orig_y)
    return (site)


###############################################lecture de l'entete envi
def lire_entete_mnt(fic_hdr):
    """lecture du fichier hdr, en entree, chemin complet du fichier
    """
    with open(fic_hdr, 'r') as f:
        for ligne in f.readlines():
            if ligne.find('samples') >= 0:
                nb_col = int(ligne.split('=')[1])
            if ligne.find('lines') >= 0:
                nb_lig = int(ligne.split('=')[1])
            if ligne.find('byte order') >= 0:
                num_endian = int(ligne.split('=')[1])
                if (num_endian == 0):
                    endian = 'PC'
                else:
                    endian = 'SUN'
            if ligne.find('data type') >= 0:
                type_envi = int(ligne.split('=')[1])
                if (type_envi == 1):
                    type_donnee = 'uint8'
                elif (type_envi == 2):
                    type_donnee = 'int16'
                elif (type_envi == 4):
                    type_donnee = 'float32'
                elif (type_envi == 5):
                    type_donnee = 'double'
                elif (type_envi == 12):
                    type_donnee = 'uint16'
                else:
                    print('type %d non pris en compte' % type_envi)

    return (nb_lig, nb_col, type_donnee, endian)


#################calcule le nom de la tuile
def calcule_nom_tuile(tx, ty, site, nom_site):
    if tx >= 0:
        GD = "D"
        numx = tx
    else:
        GD = "G"
        numx = -tx

    if ty > 0:
        HB = "H"
        numy = ty
    else:
        HB = "B"
        numy = -ty

    nom_tuile = "%s%s%04d%s%04d" % (nom_site, GD, numx, HB, numy)
    return (nom_tuile)


#############################################################
###########################Classe MNT########################
#############################################################"""

class classe_mnt:
    def __init__(self, rep, rac, ulx, uly, lrx, lry, res, chaine_proj):
        self.racine = rep
        self.ulx = ulx
        self.uly = uly
        self.lrx = lrx
        self.lry = lry
        self.res = res
        self.chaine_proj = chaine_proj

    #############################################################
    ###########################Pour Babel########################
    #############################################################"""
    def ecrit_hd(self, nb_lig, nb_col):
        ficMNT = self.racine + '_' + str(self.res) + 'm'
        f = open(ficMNT + '.hd', 'w')
        f.write('CHANNELS\n')
        f.write('1\n')
        f.write('LINES\n')
        f.write(str(nb_lig) + '\n')
        f.write('COLUMNS\n')
        f.write(str(nb_col) + '\n')
        f.write('BITS PER PIXEL\n')
        f.write('16\n')
        f.close()

    def ecrit_hd_babel(self, nb_lig, nb_col):
        ficMNT = self.racine + '_' + str(self.res) + 'm'
        f = open(ficMNT + '.hd_babel', 'w')
        f.write('>>\tLON_REF\t' + str(self.ulx) + '\n')
        f.write('>>\tLAT_REF\t' + str(self.uly) + '\n')
        f.write('>>\tNB_LON\t' + str(nb_col) + '\n')
        f.write('>>\tNB_LAT\t' + str(nb_lig) + '\n')
        f.write('>>\tPAS_LON\t' + str(self.res) + '\n')
        f.write('>>\tPAS_LAT\t-' + str(self.res) + '\n')
        f.write('>>\tTYPE_CODE\t2\n')
        f.write('>>\tTYPE_CONV\t0\n')
        f.write('>>\tREF\tWGS84:G-D/GRS80:Z-M\n')
        f.close()

    #############################################################
    ########################Interface GDAL#######################
    #############################################################

    def decoupe_float(self, fic_in, fic_out):
        # calcul du mnt float
        chaine_etendue = str(self.ulx) + ' ' + str(self.lry) + ' ' + str(self.lrx) + ' ' + str(self.uly)
        commande = 'gdalwarp  -overwrite -r cubic -ot Float32 -srcnodata -32768 -dstnodata 0 -of ENVI -tr %d %d -te %s -t_srs %s %s %s\n' % (
        self.res, self.res, chaine_etendue, self.chaine_proj, fic_in, fic_out)
        print(commande)
        os.system(commande)

    def decoupe_int(self, fic_in, fic_out):
        # calcul du mnt int
        chaine_etendue = str(self.ulx) + ' ' + str(self.lry) + ' ' + str(self.lrx) + ' ' + str(self.uly)
        commande = 'gdalwarp  -overwrite -r cubic -srcnodata -32768 -dstnodata 0 -of ENVI -tr %d %d -te %s -t_srs %s %s %s\n' % (
        self.res, self.res, chaine_etendue, self.chaine_proj, fic_in, fic_out)
        print(commande)
        os.system(commande)

    #############################################################
    ########################Decoupage MNT########################
    #############################################################

    def decoupe(self, mnt_in):
        print("###decoupage " + str(self.res) + 'm')
        rac_mnt = self.racine + '_' + str(self.res) + 'm'
        fic_hdr_mnt = rac_mnt + '.hdr'
        fic_mnt = rac_mnt + '.mnt'
        fic_hdr_mnt_float = rac_mnt + 'float.hdr'
        fic_mnt_float = rac_mnt + 'float.mnt'
        # calcul du mnt int
        self.decoupe_int(mnt_in, fic_mnt)

        # calcul du mnt float
        self.decoupe_float(mnt_in, fic_mnt_float)

        # ecriture des entetes babel
        (nblig, nbcol, type_donnee, endian) = lire_entete_mnt(fic_hdr_mnt)

        self.ecrit_hd(nblig, nbcol)
        self.ecrit_hd_babel(nblig, nbcol)

        shutil.copy(fic_mnt, rac_mnt + '.c1')

    #############################################################
    ########################Reech gradient########################
    #############################################################

    def reech_gradient(self, fic_dz_dl_srtm, fic_dz_dc_srtm):
        rac_mnt = self.racine + '_' + str(self.res) + 'm'
        fic_dz_dl = rac_mnt + 'float.dz_dl'
        fic_dz_dc = rac_mnt + 'float.dz_dc'

        # rééch
        self.decoupe_float(fic_dz_dl_srtm, fic_dz_dl)
        self.decoupe_float(fic_dz_dc_srtm, fic_dz_dc)

    ###########################################################
    ######### calcul du gradient################################
    ###########################################################

    def calcul_gradient(self):
        rac_mnt = self.racine + '_' + str(self.res) + 'm'
        print(rac_mnt)
        fic_mnt = rac_mnt + 'float.mnt'
        fic_hdr = rac_mnt + 'float.hdr'
        fic_dz_dl = rac_mnt + 'float.dz_dl'
        fic_dz_dc = rac_mnt + 'float.dz_dc'
        (nblig, nbcol, type_donnee, endian) = lire_entete_mnt(fic_hdr)

        srtm = (np.fromfile(fic_mnt, type_donnee)).reshape(nblig, nbcol)
        Noyau_horizontal = np.array([[-1, 0, 1], [-2, 0, 2], [-1, 0, 1]])
        Noyau_vertical = np.array([[1, 2, 1], [0, 0, 0], [-1, -2, -1]])

        dz_dc = nd.convolve(srtm, Noyau_horizontal) / 8. / self.res
        dz_dl = nd.convolve(srtm, Noyau_vertical) / 8. / self.res

        dz_dl.tofile(fic_dz_dl)
        dz_dc.tofile(fic_dz_dc)
        return (fic_dz_dl, fic_dz_dc)

    #############################################################
    ########################Calcul_pentes########################
    #############################################################

    def calcul_pente_aspect_fic(self):
        rac_mnt = self.racine + '_' + str(self.res) + 'm'
        print(rac_mnt)

        fic_hdr = rac_mnt + 'float.hdr'
        fic_dz_dl = rac_mnt + 'float.dz_dl'
        fic_dz_dc = rac_mnt + 'float.dz_dc'

        (nblig, nbcol, type_donnee, endian) = lire_entete_mnt(fic_hdr)
        print(nblig * nbcol * 2)
        # dz_dl=(np.fromfile(fic_dz_dl,type_donnee)).reshape(nblig,nbcol).astype('int16')
        # dz_dc=(np.fromfile(fic_dz_dc,type_donnee)).reshape(nblig,nbcol).astype('int16')

        dz_dl = (np.fromfile(fic_dz_dl, type_donnee)).reshape(nblig, nbcol)
        dz_dc = (np.fromfile(fic_dz_dc, type_donnee)).reshape(nblig, nbcol)

        norme = np.sqrt((dz_dc) * (dz_dc) + (dz_dl) * (dz_dl))
        slope = np.arctan(norme)
        aspect = np.where(dz_dc > 0, np.arccos(dz_dl / norme), 2 * np.pi - np.arccos(dz_dl / norme))
        aspect = np.where(slope == 0, 0, aspect)

        (slope * 100.).astype('int16').tofile(rac_mnt + '.slope')
        (aspect * 100.).astype('int16').tofile(rac_mnt + '.aspect')

    #############################################################
    ########################Calcul_eau_mnt#######################
    #############################################################

    def calcul_masque_mnt(self, rep, rac):
        rac_eau = self.racine + '_' + str(self.res) + 'm'
        fic_hdr_eau = rac_eau + '.hdr'
        fic_eau = rac_eau + '.eau'

        rac_mnt = rep + rac + '_' + str(self.res) + 'm'
        fic_hdr_mnt = rac_mnt + '.hdr'
        fic_hdr_mnt_float = rac_mnt + 'float.hdr'
        fic_mnt = rac_mnt + 'float.mnt'
        fic_dz_dl = rac_mnt + 'float.dz_dl'
        fic_dz_dc = rac_mnt + 'float.dz_dc'

        (nblig, nbcol, type_donnee, endian) = lire_entete_mnt(fic_hdr_mnt_float)
        mnt = (np.fromfile(fic_mnt, type_donnee)).reshape(nblig, nbcol)
        dz_dl = (np.fromfile(fic_dz_dl, type_donnee)).reshape(nblig, nbcol)
        dz_dc = (np.fromfile(fic_dz_dc, type_donnee)).reshape(nblig, nbcol)

        eau = np.where((mnt < 0) & (np.abs(dz_dl) <= 1e5) & (np.abs(dz_dc) <= 1e5), 1, 0)

        eau.astype('int16').tofile(fic_eau)
        print(fic_eau)
        shutil.copy(fic_hdr_mnt, fic_hdr_eau)

    #############################################################
    ########################Decoupage EAU########################
    #############################################################

    def decoupe_eau(self, eau_in):
        rac_eau = self.racine + '_' + str(self.res) + 'm'
        fic_hdr_eau = rac_eau + '.hdr'
        fic_eau = rac_eau + '.eau'

        # calcul du mnt int
        chaine_etendue = str(self.ulx) + ' ' + str(self.lry) + ' ' + str(self.lrx) + ' ' + str(self.uly)
        commande = 'gdalwarp -overwrite  -r near -of ENVI -tr %d %d -te %s -t_srs %s %s %s\n' % (
        self.res, self.res, chaine_etendue, self.chaine_proj, eau_in, fic_eau)
        print(commande)
        os.system(commande)


#################################################################################
########################### Fusion DEM and water masks   ########################
#################################################################################
def fusion_mnt(liste_fic_mnt, liste_fic_eau, liste_centre_eau, rep_mnt, rep_swbd, nom_site, calcul_eau_mnt, working_dir):
    print("liste_fic_mnt", liste_fic_mnt)
    if not liste_fic_eau:
        raise ValueError("Cannot find SWBD files!")

    # Unzip SWBD-files:
    for swbd in liste_fic_eau:
        filenameWater = r"%s\w.zip" % swbd
        files = [f for f in os.listdir(rep_swbd) if re.search(filenameWater, f)]
        if len(files) == 1:
            #raise OSError("No unique SWBD file found for %s in %s" % (swbd, rep_swbd))
            commande = "unzip -o %s -d %s" % (pjoin(rep_swbd, files[0]), working_dir)
            os.system(commande)
    for fic in liste_fic_mnt:
        print("FIC: {0}".format(fic))
        print(type(rep_mnt), type(fic))
        print(rep_mnt + '/' + fic)
        ficzip = fic.replace('tif', 'zip')
        if not os.path.exists(rep_mnt + '/' + ficzip):
            print("Need to download the file {0}".format(ficzip))
            urllib.request.urlretrieve("http://srtm.csi.cgiar.org/wp-content/uploads/files/srtm_5x5/TIFF/%s" % ficzip,
                                       rep_mnt + '/' + ficzip)
            if not os.path.exists(rep_mnt + '/' + ficzip):
                print("Unable to download the file!")
                raise RuntimeError("Unable to download the file {0}!".format(ficzip))
        commande = "unzip -o %s/%s -d %s" % (rep_mnt, ficzip, working_dir)
        os.system(commande)
    if len(liste_fic_mnt) > 1:
        nom_mnt = tempfile.mkstemp(prefix="mnt_{}".format(nom_site), suffix=".tif", dir=working_dir)[1]
        commande = "gdal_merge.py -o " + nom_mnt
        for fic_mnt in liste_fic_mnt:
            commande = commande + " " + pjoin(working_dir, fic_mnt) + " "
        if os.path.exists(nom_mnt):
            os.remove(nom_mnt)
        print(commande)
        os.system(commande)

    elif len(liste_fic_mnt) == 1:
        nom_mnt = pjoin(working_dir, liste_fic_mnt[0])
    else:
        print("liste_fic_mnt is empty")
        raise ("ErreurDeParametreSite")

    ########################on créé aussi le mnt avec no_data=0
    nom_mnt_nodata0 = nom_mnt.replace(".tif", "nodata0.tif")
    commande = 'gdalwarp  -r cubic -srcnodata -32767 -dstnodata 0  %s %s\n' % (nom_mnt, nom_mnt_nodata0)
    print(commande)
    os.system(commande)

    if calcul_eau_mnt == 0:  # si on est en deça de 60°N

        # Création d'un fichier vide (valeurs à 0) avec la même emprise que le mnt fusionnné
        ####################################################################################
        nom_raster_swbd = os.path.join(working_dir, os.path.basename(nom_mnt).split('.tif')[0] + "_tmp.tif")
        if os.path.exists(nom_raster_swbd):
            os.remove(nom_raster_swbd)
        ds = gdal.Open(nom_mnt)
        driver = gdal.GetDriverByName('GTiff')
        ds_out = driver.CreateCopy(nom_raster_swbd, ds, 0)
        inband = ds.GetRasterBand(1)
        outband = ds_out.GetRasterBand(1)
        for i in range(inband.YSize - 1, -1, -1):
            scanline = inband.ReadAsArray(0, i, inband.XSize, 1, inband.XSize, 1)
            scanline = scanline * 0
            outband.WriteArray(scanline, 0, i)
        ds_out = None

        # remplissage de ce fichier avec les fichiers SWBD
        for i, racine_nom_eau in enumerate(liste_fic_eau):
            print(racine_nom_eau)
            shp = glob.glob(os.path.join(working_dir, racine_nom_eau + "*.shp"))
            # if shp file does not exist
            if len(shp) == 0:
                print('missing SWBD watr file : ', racine_nom_eau)

                # test if center is water or land
                land = TestLand(liste_centre_eau[i][0], liste_centre_eau[i][1])
                if land:
                    valeur = 0
                    print("it is a fully land tile")
                else:
                    valeur = 1
                    print("it is a fully water tile")
                fic_vecteur_eau = rep_swbd + '/' + racine_nom_eau + ".gml"
                creer_fichier_eau(fic_vecteur_eau, racine_nom_eau)
            # if shp file exists
            else:
                valeur = 1
                fic_vecteur_eau = shp[0]
                # il faut recuperer pour la couche le nom complet (y compris la lettre indiquant le continent)
                racine_nom_eau = os.path.basename(fic_vecteur_eau)[:-4]
            commande = "gdal_rasterize -burn %d -l %s %s %s" % (
            valeur, racine_nom_eau, fic_vecteur_eau, nom_raster_swbd)
            print("#############Fichier eau :", fic_vecteur_eau)
            print(commande)
            os.system(commande)
    else:
        nom_raster_swbd = ""
    return nom_mnt_nodata0, nom_raster_swbd


# calcul de pentes et aspect
##########################
def calcul_pente_aspect_mem(rac_mnt, dz_dc, dz_dl):
    norme = np.sqrt((dz_dc) * (dz_dc) + (dz_dl) * (dz_dl))
    slope = np.arctan(norme)
    aspect = np.where(dz_dc > 0, np.arccos(dz_dl / norme), 2 * np.pi - np.arccos(dz_dl / norme))
    aspect = np.where(slope == 0, 0, aspect)
    slope = np.where(np.isfinite(slope), slope, 0)
    aspect = np.where(np.isfinite(aspect), aspect, 0)

    (slope * 100.).astype('int16').tofile(rac_mnt + '.slope')
    (aspect * 100.).astype('int16').tofile(rac_mnt + '.aspect')


##############################################
##############################################	
def creer_fichier_eau(fic_eau, nom_eau):
    patron = """<?xml version="1.0" encoding="utf-8" ?>
<ogr:FeatureCollection
     xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
     xsi:schemaLocation="http://ogr.maptools.org/ toto2.xsd"
     xmlns:ogr="http://ogr.maptools.org/"
     xmlns:gml="http://www.opengis.net/gml">
  <gml:boundedBy>
    <gml:Box>
      <gml:coord><gml:X>LONMIN.</gml:X><gml:Y>LATMIN.</gml:Y></gml:coord>
      <gml:coord><gml:X>LONMAX.</gml:X><gml:Y>LATMAX.</gml:Y></gml:coord>
    </gml:Box>
  </gml:boundedBy>
                        
  <gml:featureMember>
    <ogr:NOMEAU fid="F21">
      <ogr:geometryProperty><gml:Polygon><gml:outerBoundaryIs><gml:LinearRing><gml:coordinates>LONMIN.,LATMIN.,0 LONMAX.,LATMIN.,0  LONMAX.,LATMAX.,0 LONMIN.,LATMAX.,0 LONMIN.,LATMIN.,0 </gml:coordinates></gml:LinearRing></gml:outerBoundaryIs></gml:Polygon></ogr:geometryProperty>
      <ogr:FACC_CODE>BH080</ogr:FACC_CODE>
    </ogr:NOMEAU>
  </gml:featureMember>
</ogr:FeatureCollection>

"""
    ew = nom_eau[0]
    num_x = int(nom_eau[1:4])
    ns = nom_eau[4]
    num_y = int(nom_eau[5:7])
    if ew == "w":
        num_x = -num_x
    if ns == "s":
        num_y = -num_y
    patron = patron.replace('LONMIN', str(num_x))
    patron = patron.replace('LATMIN', str(num_y))
    patron = patron.replace('LONMAX', str(num_x + 1))
    patron = patron.replace('LATMAX', str(num_y + 1))
    patron = patron.replace('NOMEAU', nom_eau)

    print(fic_eau)
    # print patron
    with open(fic_eau, "w") as f:
        f.write(patron)
    return
