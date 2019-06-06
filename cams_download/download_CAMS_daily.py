#!/usr/bin/env python
# -*- coding: utf-8 -*-
"""
Downloads necessary CAMS data for MAJA and converts them into MAJA input format 
Written by B.Rouquie, O.Hagolle, CESBIO/CNES

#==================== Copyright
Software downloads_CAMS_daily.py

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

import os
from ecmwfapi import ECMWFDataServer
from convert_to_exo import process_one_file
import datetime
import timeit
import optparse
import sys

server = ECMWFDataServer()
###########################################################################
class OptionParser (optparse.OptionParser):
 
    def check_required (self, opt):
      option = self.get_option(opt)
 
      # Assumes the option's 'default' is set to None!
      if getattr(self.values, option.dest) is None:
          self.error("%s option not supplied" % option)

###########################################################################
def download_files(dt,file_type,time,step,path_out):

    date_courante = str(dt.year)+'%02d'%(dt.month)+'%02d'%(dt.day)
    print('\nCurrent_date =',date_courante)

    if file_type['surface'] == True:
        #=================
        #     Surface
        # Recupere AOT a 550nm pour BC, SS, SU, DU, OM
        #=================
        nom_AOT = path_out + "/CAMS_AOT_" + date_courante + 'UTC' + str(int(time)+int(step)).zfill(2) + '0000.nc'
        print ('Nom fichier de sortie AOT :',nom_AOT)

        server.retrieve({
            'stream'  : "oper",
            'class'   : "mc",
            'dataset' : "cams_nrealtime",
            'expver'  : '0001',
            'step'    : step,
            'levtype' : "SFC",
            'date'    : date_courante,
            'time'    : time,
            'type'    : "fc",
            'param'   : "208.210/209.210/210.210/211.210/212.210",
            'area'    : "G",
            'grid'    : "1.25/1.25",
            'format'  : "netcdf",
            'target'  : nom_AOT
            })
        #208.210/209.210/210.210/211.210/212.210 : AOT at 550nm for BC, SS, OM, SU, DU

    if file_type['pressure'] == True:
        #=========================
        #     Pressure levels
        #
        # Recupere Relative Humidity RH
        #=========================
        nom_RH = path_out + "/CAMS_RH_" + date_courante + 'UTC' + str(int(time)+int(step)).zfill(2) + '0000.nc'
        print ('Nom fichier de sortie RH :',nom_RH)

        server.retrieve({
              'stream'  : "oper",
              'class'   : "mc",
              'dataset' : "cams_nrealtime",
              'expver'  : "0001",
              'step'    : step,
              'levtype' : "pl",
              "levelist": "1/2/3/5/7/10/20/30/50/70/100/150/200/250/300/400/500/600/700/850/925/1000",
              'date'    : date_courante,
              'time'    : time,
              'type'    : "fc",
              'param'   : "157.128",
              'area'    : "G",
              'grid'    : "1.25/1.25",
              'format'  : "netcdf",
              'target'  : nom_RH
              })

    if file_type['model'] == True:
        #=========================
        #     Model levels
        #
        # Recupere les mixing ratios : 3 bins DUST, 3 bins SEASALT, ORGANICMATTER hydrophile et hydrophobe, BLACKCARBON hydrophile et hydrophobe, et SULFATE.
        #=========================
        nom_MR = path_out + "/CAMS_MR_" + date_courante + 'UTC' + str(int(time)+int(step)).zfill(2) + '0000.nc'
        print ('Nom fichier de sortie mixRatios :',nom_MR)

        server.retrieve({
            'stream'  : "oper",
            'class'   : "mc",
            'dataset' : "cams_nrealtime",
            'expver'  : "0001",
            'step'    : step,
            'levtype' : "ml",
            "levelist": "1/to/60",
            'date'    : date_courante,
            'time'    : time,
            'type'    : "fc",
            'param'   : "1.210/2.210/3.210/4.210/5.210/6.210/7.210/8.210/9.210/10.210/11.210",
            'area'    : "G",
            'grid'    : "1.25/1.25",
            'format'  : "netcdf",
            'target'  : nom_MR
            })
    return nom_AOT, nom_RH, nom_MR


#==============
#     MAIN
#==============
        
#===========================
#    ORDRES DE GRANDEUR
#===========================
#1 fichier Surface  = 0.8 Mo
#1 fichier Pressure = 1.7 Mo
#1 fichier Model    = 53 Mo
#
# => 20 Go par an (40 Go par an avec les deux forecasts (minuit et midi))
#

#=============================
#    AEROSOLS DISPONIBLES
#=============================
#BC = BlackCarbon
#SS = SeaSalt
#SU = Sulfate
#DU = Dust
#OM = OrganicMatter



#==================
#parse command line
#==================
if len(sys.argv) == 1:
    prog = os.path.basename(sys.argv[0])
    print ('      '+sys.argv[0]+' [options]')
    print ("     Aide : ", prog, " --help")
    print ("        ou : ", prog, " -h")
    print ("example : python %s -d 20171101 -f 20171201 -a /mnt/data/DONNEES_AUX/CAMS_DBL/ -w /mnt/data/DONNEES_AUX/CAMS_TMP/ "%prog)
    sys.exit(-1)
else :
    usage = "usage: %prog [options] "
    parser = OptionParser(usage=usage)
    parser.add_option("-d", "--start_date", dest="start_date", action="store", type="string", \
        help="start date, fmt('20171101')",default=None)
    parser.add_option("-f","--end_date", dest="end_date", action="store", type="string", \
        help="end date, fmt('20171201')",default=None)
    parser.add_option("-w","--write_dir", dest="write_dir", action="store",type="string",  \
        help="Path where the products should be downloaded")
    parser.add_option("-a","--archive_dir", dest="archive_dir", action="store",type="string",  \
        help="Path where the archive DBL files are stored")
    parser.add_option("-k","--keep", dest="keep", action="store_true",  \
                      help="keep netcdf files",default=False)
    
    #parser.add_option("-t", "--time",dest="time", action="store", type="choice", \
    #    choices=['00','12'],help="Time of forecast (Currently '00'or'12)",default='00')

    (options, args) = parser.parse_args()
    parser.check_required("-d")
    parser.check_required("-f")
    parser.check_required("-a")
    parser.check_required("-w")
    


#====================
#     PARAMETRES
#====================


#Creation objets dates
dt1 = datetime.datetime.strptime(options.start_date,'%Y%m%d')
dt2 = datetime.datetime.strptime(options.end_date,'%Y%m%d')

nb_days = (dt2-dt1).days + 1
print ('\nNumber of days =',nb_days)

#Time de l'analyse voulue
#Two possibilities :
# - 00:00:00 UTC (minuit)
# - 12:00:00 UTC (midi)

time = ["00","12"]

#Step du forecast voulu
#step = 3 indique qu'on telecharge les previsions a 3h apres l'heure de l'analyse.
#Exemples : time = 00 et step = 3 => 03:00:00 UTC
#           time = 12 et step = 3 => 15:00:00 UTC
step = "3" 

#Path out
path_out = options.write_dir

#Choix des fichiers a telecharger
#Surface  : AOT (aerosol optical thickness)
#Pressure : RH  (relative humidity)
#Model    : MR  (mixing ratios)
file_type={'surface':True,'pressure':True,'model':True}



#Boucle sur les jours a telecharger
for i in range(nb_days):
        dt = dt1 + datetime.timedelta(days=i)
        print ("==================================")
        print ("Downloading files for date %s"%dt)
        print ("==================================")
        for t in range(len(time)):
            (nom_AOT,nom_RH,nom_MR)=download_files(dt,file_type,time[t],step,path_out)
            #conversion to MAJA DBL/HDR format
            process_one_file(nom_AOT, nom_MR, nom_RH, path_out, options.archive_dir)
            if not(options.keep):
                os.remove(nom_AOT)
                os.remove(nom_MR)
                os.remove(nom_RH)
            

