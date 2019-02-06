#! /usr/bin/env python
# -*- coding: utf-8 -*-

"""
Reprojette et decoupe un mnt SRTM sur les tuiles d'un site 
Les paramètres sont dans parametres.py, dont le nom du site qui sert à déterminer le fichier de paramètres du tuilage d'un site (ex pyrenees.py)

"""
import optparse

###########################################################################
class OptionParser(optparse.OptionParser):

    def check_required(self, opt):
        option = self.get_option(opt)

        # Assumes the option's 'default' is set to None!
        if getattr(self.values, option.dest) is None:
            self.error("%s option not supplied" % option)


############################# Main

from math import ceil, floor
from lib_mnt import *

from osgeo import osr
import sys

os.environ['LC_NUMERIC'] = 'C'

if len(sys.argv) == 1:
    prog = os.path.basename(sys.argv[0])
    print '      ' + sys.argv[0] + ' [options]'
    print "     Aide : ", prog, " --help"
    print "        ou : ", prog, " -h"
    print "example1 : python %s -p parametres_po.txt -s ~/DONNEES/SPOT5TAKE5/prep_mnt/Algeria4.py -m PO -f 10 -c 100" % \
          sys.argv[0]
    print "example2 : python tuilage_mnt_eau.py -p parametres_srtm.txt -s CVersailles.txt -m SRTM -f 20 -c 200" % \
          sys.argv[0]
    sys.exit(-1)
else:
    usage = "usage: %prog [options] "
    parser = OptionParser(usage=usage)
    parser.set_defaults(eau_seulement=False)
    parser.set_defaults(sans_numero=False)

    parser.add_option("-p", "--parametre", dest="fic_param", action="store", type="string", \
                      help="fichier de parametre", default=None)
    parser.add_option("-s", "--site", dest="fic_site", action="store", type="string", \
                      help="fichier de description du site", default=None)
    parser.add_option("-m", "--mnt", dest="mnt", action="store", type="choice", \
                      help="SRTM ou PO (Planet Observer)", choices=['SRTM', 'PO'], default=None)
    parser.add_option("-f", dest="FULL_RES", action="store", type="int", \
                      help="Full resolution", default=None)
    parser.add_option("-c", dest="COARSE_RES", action="store", type="int", \
                      help="Coarse resolution", default=None)
    parser.add_option("-e", dest="eau_seulement", action="store_true", \
                      help="Traitement des masques d'eau seulement")
    parser.add_option("-n", dest="sans_numero", action="store_true", \
                      help="Traitement sans numero de tuile")

    (options, args) = parser.parse_args()
    parser.check_required("-p")
    parser.check_required("-s")
    parser.check_required("-m")
    parser.check_required("-c")
    parser.check_required("-f")

# lecture du fichier de paramètres et du fichier site
(rep_mnt_in, rep_mnt, rep_swbd, rep_eau) = lire_param_txt(options.fic_param)
site = lire_fichier_site(options.fic_site)
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
if options.mnt == "SRTM":
    # liste des fichiers SRTM nécessaires
    if (ul_latlon[1]) > 60 or (lr_latlon[1] > 60):
        print "#################################################"
        print "latitude supérieure à 60 degrés, pas de donnees SRTM"
        print "#################################################"
        sys.exit(-3)

    ul_latlon_srtm = [int(ul_latlon[0] + 180) / 5 + 1, int(60 - ul_latlon[1]) / 5 + 1]
    lr_latlon_srtm = [int(lr_latlon[0] + 180) / 5 + 1, int(60 - lr_latlon[1]) / 5 + 1]

    for x in range(ul_latlon_srtm[0], lr_latlon_srtm[0] + 1):
        for y in range(ul_latlon_srtm[1], lr_latlon_srtm[1] + 1):
            liste_fic_mnt.append("srtm_%02d_%02d.tif" % (x, y))

    print ul_latlon, lr_latlon
    print ul_latlon_srtm, lr_latlon_srtm
    print liste_fic_mnt

########## MNT Planet Observer
elif options.mnt == "PO":

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

    print ul_latlon, lr_latlon
    print ul_latlon_po, lr_latlon_po
    print liste_fic_mnt

# liste des fichiers SWBD nécessaires
ul_latlon_swbd = [int(floor(ul_latlon[0])), int(floor(ul_latlon[1]))]
lr_latlon_swbd = [int(floor(lr_latlon[0])), int(floor(lr_latlon[1]))]
print ul_latlon, lr_latlon
print ul_latlon_swbd, lr_latlon_swbd

calcul_masque_eau_mnt = 0
if (ul_latlon[1]) > 60 or (lr_latlon[1] > 60):
    print "#################################################"
    print "latitude supérieure à 60 degrés, pas de donnees SRTM"
    print "le masque d'eau est généré à partir du MNT"
    print "#################################################"
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

print "longitudes", ul_latlon_swbd[0], lr_latlon_swbd[0]
print "latitudes", lr_latlon_swbd[1], ul_latlon_swbd[1]
print "center coordinates", liste_centre_eau
print liste_fic_eau

# Fusion des mnt_srtm en un seul
(fic_mnt_in, fic_eau_in) = fusion_mnt(liste_fic_mnt, liste_fic_eau, liste_centre_eau, rep_mnt_in, rep_swbd, site.nom,
                                      calcul_masque_eau_mnt)
print "############", fic_mnt_in

####################Boucle de création des fichiers MNT et eau pour chaque tuile

for tx in range(site.tx_min, site.tx_max + 1):
    for ty in range(site.ty_min, site.ty_max + 1):
        ulx = site.orig_x + tx * site.pas_x  # upper left
        uly = site.orig_y + ty * site.pas_y
        lrx = site.orig_x + (tx + 1) * site.pas_x + site.marge  # lower left
        lry = site.orig_y + (ty - 1) * site.pas_y - site.marge

        lrx_90m = int(ceil((lrx - ulx) / float(SRTM_RES))) * SRTM_RES + ulx
        lry_90m = uly - int(ceil((uly - lry) / float(SRTM_RES))) * SRTM_RES

        lrx_coarse = int(ceil((lrx - ulx) / float(options.COARSE_RES))) * options.COARSE_RES + ulx
        lry_coarse = uly - int(ceil((uly - lry) / float(options.COARSE_RES))) * options.COARSE_RES

        if options.sans_numero & (site.tx_max == 0) & (site.ty_max == 0):
            nom_tuile = site.nom
        else:
            nom_tuile = calcule_nom_tuile(tx, ty, site, site.nom)
        print "nom de la tuile", nom_tuile, tx, ty
        ###pour le MNT
        rep_mnt_out = rep_mnt + nom_tuile + '/'

        if options.eau_seulement == False:
            if not (os.path.exists(rep_mnt_out)):
                os.mkdir(rep_mnt_out)

            # Resolution SRTM_RES
            print "############### c'est parti"
            mnt_90m = classe_mnt(rep_mnt_out, nom_tuile, ulx, uly, lrx_90m, lry_90m, SRTM_RES, site.chaine_proj)
            mnt_90m.decoupe(fic_mnt_in)

            # calcul du gradient à 90m
            (fic_dz_dl_srtm, fic_dz_dc_srtm) = mnt_90m.calcul_gradient()

            # Basse résolution

            mnt_coarse = classe_mnt(rep_mnt_out, nom_tuile, ulx, uly, lrx_coarse, lry_coarse, options.COARSE_RES,
                                    site.chaine_proj)
            mnt_coarse.decoupe(fic_mnt_in)
            mnt_coarse.reech_gradient(fic_dz_dl_srtm, fic_dz_dc_srtm)
            mnt_coarse.calcul_pente_aspect_fic()

            # Haute résolution
            mnt_full = classe_mnt(rep_mnt_out, nom_tuile, ulx, uly, lrx, lry, options.FULL_RES, site.chaine_proj)
            mnt_full.decoupe(fic_mnt_in)
            mnt_full.reech_gradient(fic_dz_dl_srtm, fic_dz_dc_srtm)
            mnt_full.calcul_pente_aspect_fic()

        ### Pour l'eau
        rep_eau_out = rep_eau + nom_tuile + '/'
        if not (os.path.exists(rep_eau_out)):
            os.mkdir(rep_eau_out)

        eau = classe_mnt(rep_eau_out, nom_tuile, ulx, uly, lrx_coarse, lry_coarse, options.COARSE_RES, site.chaine_proj)
        if calcul_masque_eau_mnt == 0:
            eau.decoupe_eau(fic_eau_in)
        else:
            eau.calcul_masque_mnt(rep_mnt_out, nom_tuile)
