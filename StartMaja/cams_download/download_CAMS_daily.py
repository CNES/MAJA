#!/usr/bin/env python3
# -*- coding: utf-8 -*-

"""
Copyright (C) 2016-2020 Centre National d'Etudes Spatiales (CNES), CSSI, CESBIO  All Rights Reserved

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

   http://www.apache.org/licenses/LICENSE2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
"""

import os
import sys
sys.path.append(os.path.join(os.path.dirname(os.path.realpath(__file__)), '..'))  # Import relative modules


def download_files(dt, file_type, t_hour, step, path_out):
    """
    Downloads necessary CAMS data for MAJA and converts them into MAJA input format
    """
    from ecmwfapi import ECMWFDataServer
    from datetime import datetime

    server = ECMWFDataServer()

    date_courante = str(dt.year) + '%02d' % dt.month + '%02d' % dt.day
    print('Current_date =', date_courante)

    if file_type['surface']:
        # Surface
        # Recupere AOT a 550nm pour BC, SS, SU, DU, OM
        # Et (si possible p. CAMS46r1) AM, NI
        nom_aot = "CAMS_AOT_" + date_courante + 'UTC' + str(int(t_hour)+int(step)).zfill(2) + '0000.nc'
        path_aot = os.path.join(path_out, nom_aot)
        print('Nom fichier de sortie AOT :', path_aot)

        models = "208.210/209.210/210.210/211.210/212.210"
        if dt >= datetime(2019, 7, 10):
            # This is the new format:
            models += "/250.210/251.210"
        # 208.210/209.210/210.210/211.210/212.210 : AOT at 550nm for BC, SS, OM, SU, DU
        server.retrieve({
            'stream'  : "oper",
            'class'   : "mc",
            'dataset' : "cams_nrealtime",
            'expver'  : '0001',
            'step'    : step,
            'levtype' : "SFC",
            'date'    : date_courante,
            'time'    : t_hour,
            'type'    : "fc",
            'param'   : models,
            'area'    : "G",
            'grid'    : "1.25/1.25",
            'format'  : "netcdf",
            'target'  : path_aot
            })

    if file_type['pressure']:
        # Pressure levels
        # Recupere Relative Humidity RH
        nom_rh = "CAMS_RH_" + date_courante + 'UTC' + str(int(t_hour)+int(step)).zfill(2) + '0000.nc'
        path_rh = os.path.join(path_out, nom_rh)
        print('Nom fichier de sortie RH :', path_rh)
        levellist = "1/2/3/5/7/10/20/30/50/70/100/150/200/250/300/400/500/600/700/800/850/900/925/950/1000"
        server.retrieve({
              'stream'  : "oper",
              'class'   : "mc",
              'dataset' : "cams_nrealtime",
              'expver'  : "0001",
              'step'    : step,
              'levtype' : "pl",
              "levelist": levellist,
              'date'    : date_courante,
              'time'    : t_hour,
              'type'    : "fc",
              'param'   : "157.128",
              'area'    : "G",
              'grid'    : "1.25/1.25",
              'format'  : "netcdf",
              'target'  : path_rh
              })

    if file_type['model']:
        # Model levels
        # Recupere les mixing ratios :
        # 3 bins DUST,
        # 3 bins SEASALT,
        # ORGANICMATTER hydrophile et hydrophobe,
        # BLACKCARBON hydrophile et hydrophobe,
        # et SULFATE.
        nom_mr = "CAMS_MR_" + date_courante + 'UTC' + str(int(t_hour)+int(step)).zfill(2) + '0000.nc'
        path_mr = os.path.join(path_out, nom_mr)
        print('Nom fichier de sortie mixRatios :', path_mr)
        models = "1.210/2.210/3.210/4.210/5.210/6.210/7.210/8.210/9.210/10.210/11.210"
        levels = "1/to/60"
        if dt >= datetime(2019, 7, 10):
            # This is the new format:
            models += "/247.210/248.210/249.210"
            levels = "1/to/137"
        server.retrieve({
            'stream'  : "oper",
            'class'   : "mc",
            'dataset' : "cams_nrealtime",
            'expver'  : "0001",
            'step'    : step,
            'levtype' : "ml",
            "levelist": levels,
            'date'    : date_courante,
            'time'    : t_hour,
            'type'    : "fc",
            'param'   : models,
            'area'    : "G",
            'grid'    : "1.25/1.25",
            'format'  : "netcdf",
            'target'  : path_mr
            })
    return path_aot, path_rh, path_mr


if __name__ == "__main__":
    from convert_to_exo import RawCAMSArchive
    import datetime
    import argparse
    # ORDRES DE GRANDEUR
    # 1 fichier Surface  = 0.8 Mo
    # 1 fichier Pressure = 1.7 Mo
    # 1 fichier Model    = 53 Mo
    # => 20 Go par an (40 Go par an avec les deux forecasts (minuit et midi))

    # AEROSOLS DISPONIBLES
    # BC = BlackCarbon
    # SS = SeaSalt
    # SU = Sulfate
    # DU = Dust
    # OM = OrganicMatter

    # A partir du 20190710
    # AM = Ammonium
    # NI = Nitrate

    argParser = argparse.ArgumentParser()
    required_arguments = argParser.add_argument_group('required arguments')
    required_arguments.add_argument('-d', '--start_date', required=True,
                                    help='start date, fmt("20171201")')
    required_arguments.add_argument('-f', '--end_date', required=True,
                                    help='end date, fmt("20171201")')
    required_arguments.add_argument('-a', '--archive_dir', required=True,
                                    help='Temporary path where the products should be downloaded')
    required_arguments.add_argument('-w', '--write_dir', required=True,
                                    help='Path where the archive DBL files are stored')

    required_arguments.add_argument('-k', "--keep", help='keep raw netcdf files', action="store_true",
                                    default=False, required=False)
    required_arguments.add_argument('-p', "--platform", choices=['s2', 'l8', 've'], required=True,
                                    default="s2")

    args = argParser.parse_args()

    # Creation objets dates
    dt1 = datetime.datetime.strptime(args.start_date, '%Y%m%d')
    dt2 = datetime.datetime.strptime(args.end_date, '%Y%m%d')
    nb_days = (dt2-dt1).days + 1
    print('Number of days =', nb_days)
    # Time de l'analyse voulue
    # Two possibilities :
    # - 00:00:00 UTC (minuit)
    # - 12:00:00 UTC (midi)
    time = ["00", "12"]
    # Step du forecast voulu
    # step = 3 indique qu'on telecharge les previsions a 3h apres l'heure de l'analyse.
    # Exemples : time = 00 et step = 3 => 03:00:00 UTC
    #            time = 12 et step = 3 => 15:00:00 UTC
    step_choice = "3"

    # Choix des fichiers a telecharger
    # Surface  : AOT (aerosol optical thickness)
    # Pressure : RH  (relative humidity)
    # Model    : MR  (mixing ratios)
    ftype = {'surface': True, 'pressure': True, 'model': True}

    # Create directories
    from StartMaja.Common import FileSystem
    FileSystem.create_directory(args.archive_dir)
    FileSystem.create_directory(args.write_dir)

    # Boucle sur les jours a telecharger
    for i in range(nb_days):
            date = dt1 + datetime.timedelta(days=i)
            print("==================================")
            print("Downloading files for date %s" % date)
            print("==================================")
            for t in range(len(time)):
                aot, mr, rh = download_files(date, ftype, time[t], step_choice, args.write_dir)
                # Conversion to MAJA DBL/HDR format
                RawCAMSArchive.process_one_file(args.archive_dir, aot, rh, mr, args.platform)
                if not args.keep:
                    os.remove(aot)
                    os.remove(mr)
                    os.remove(rh)
