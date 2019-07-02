#!/usr/bin/env python
# -*- coding: utf-8 -*-
"""
Downloads necessary CAMS data for MAJA and converts them into MAJA input format
Written by B.Rouquie, O.Hagolle, CESBIO/CNES

If the user asks to download CAMS products from the current month, it will download daily files
which is slow when using a standard account. If the user asks to download older CAMS products,
monthly files will be downloaded and split into daily files, which is much faster.

Splitting the files requires a netCDF utility, either cdo (within nco package) or NCKS.

# ==================== Copyright
Software downloads_CAMS.py

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

import datetime
import optparse
import calendar
import glob
import sys
import os.path
from dateutil.relativedelta import relativedelta
from ecmwfapi import ECMWFDataServer
from convert_to_exo import process_one_file

server = ECMWFDataServer()
###########################################################################


class OptionParser(optparse.OptionParser):

    def check_required(self, opt):
        option = self.get_option(opt)

        # Assumes the option's 'default' is set to None!
        if getattr(self.values, option.dest) is None:
            self.error("%s option not supplied" % option)

###########################################################################


def download_files(download_date, file_type, time, step, OutNames, dataset, expver):

        
    if download_date.find('/TO/') >= 0:
        date_name = download_date[0:6]
        print("date_name %s" % date_name)
    else:
        date_name = download_date

    if file_type['surface'] == True:
        # =================
        #     Surface
        # Recupere AOT a 550nm pour BC, SS, SU, DU, OM
        # =================
        nom_AOT = path_out + "/CAMS_AOT_" + date_name + \
            'UTC' + str(int(time)+int(step)).zfill(2) + '0000.nc'
        if not os.path.exists(nom_AOT):
            print 'Nom fichier de sortie AOT :', nom_AOT
            server.retrieve({
                'stream': "oper",
                'class': "mc",
                'dataset': dataset,
                'expver': expver,
                'step': step,
                'levtype': "SFC",
                'date': download_date,
                'time': time,
                'type': "fc",
                'param': "208.210/209.210/210.210/211.210/212.210",
                'area': "G",
                'grid': "1.25/1.25",
                'format': "netcdf",
                'target': nom_AOT
            })
            # 208.210/209.210/210.210/211.210/212.210 : AOT at 550nm for BC, SS, OM, SU, DU

    if file_type['pressure'] == True:
        # =========================
        #     Pressure levels
        #
        # Recupere Relative Humidity RH
        # =========================
        nom_RH = path_out + "/CAMS_RH_" + date_name + 'UTC' + \
            str(int(time)+int(step)).zfill(2) + '0000.nc'
        if not os.path.exists(nom_RH):
            print 'Nom fichier de sortie RH :', nom_RH

            server.retrieve({
                'stream': "oper",
                'class': "mc",
                'dataset': "cams_nrealtime",
                'expver': "0001",
                'step': step,
                'levtype': "pl",
                "levelist": "1/2/3/5/7/10/20/30/50/70/100/150/200/250/300/400/500/600/700/850/925/1000",
                'date': download_date,
                'time': time,
                'type': "fc",
                'param': "157.128",
                'area': "G",
                'grid': "1.25/1.25",
                'format': "netcdf",
                'target': nom_RH
            })

    if file_type['model'] == True:
        # =========================
        #     Model levels
        #
        # Recupere les mixing ratios : 3 bins DUST, 3 bins SEASALT, ORGANICMATTER hydrophile et hydrophobe, BLACKCARBON hydrophile et hydrophobe, et SULFATE.
        # =========================
        nom_MR = path_out + "/CAMS_MR_" + date_name + 'UTC' + \
            str(int(time)+int(step)).zfill(2) + '0000.nc'
        if not os.path.exists(nom_MR):
            print 'Nom fichier de sortie mixRatios :', nom_MR
            server.retrieve({
                'stream': "oper",
                'class': "mc",
                'dataset': "cams_nrealtime",
                'expver': "0001",
                'step': step,
                'levtype': "ml",
                "levelist": "1/to/60",
                'date': download_date,
                'time': time,
                'type': "fc",
                'param': "1.210/2.210/3.210/4.210/5.210/6.210/7.210/8.210/9.210/10.210/11.210",
                'area': "G",
                'grid': "1.25/1.25",
                'format': "netcdf",
                'target': nom_MR
            })
    return nom_AOT, nom_RH, nom_MR


def split_daily(month, OutNames, file_type, startDate, lastDate, split):
    """
    Split CAMS monthly files into daily files using either the cdo tool (https://code.mpimet.mpg.de/projects/cdo/) or the ncks tool (http://nco.sourceforge.net/nco.html#ncks-netCDF-Kitchen-Sink)
    Example:
    'cdo splitday CAMS_AOT_201801UTC030000.nc CAMS_AOT_201801'
    'ncks -d time,2018-02-01,2018-02-01 CAMS_AOT_201801UTC030000.nc CAMS_AOT_20180101UTC030000.nc'
    The daily files will be stored in the same directory as the monthly files
    """

    ### CDO ###
    if split == 'cdo':
        print '\n### CDO ###'

        if file_type['surface'] == True:
            print '\nSplit surface file daily'
            # Extraction of UTC030000 or UTC150000, to add to output names created by cdo
            UTCtime = OutNames['surface'].split(month)[1].split('.')[0]
            # Extract output basename
            outBaseName = OutNames['surface'].split('UTC')[0]

            # Split into daily file
            cmd = 'cdo splitday '+OutNames['surface']+' '+outBaseName
            os.system(cmd)

            # Add UTC030000 or UTC150000 to output files
            outFiles = glob.glob(outBaseName+'??.nc')
            if len(outFiles) == 0:
                print 'ERROR: no output files from cdo'
            else:
                for f in outFiles:
                    fFinalName = f.split('.')[0]+UTCtime+'.nc'
                    cmd = 'mv '+f+' '+fFinalName
                    os.system(cmd)

        if file_type['pressure'] == True:
            print '\nSplit pressure file daily'
            # Extraction of UTC030000 or UTC150000, to add to output names created by cdo
            UTCtime = OutNames['pressure'].split(month)[1].split('.')[0]
            # Extract output basename
            outBaseName = OutNames['pressure'].split('UTC')[0]

            # Split into daily file
            cmd = 'cdo splitday '+OutNames['pressure']+' '+outBaseName
            os.system(cmd)

            # Add UTC030000 or UTC150000 to output files
            outFiles = glob.glob(outBaseName+'??.nc')
            if len(outFiles) == 0:
                print 'ERROR: no output files from cdo'
            else:
                for f in outFiles:
                    fFinalName = f.split('.')[0]+UTCtime+'.nc'
                    cmd = 'mv '+f+' '+fFinalName
                    os.system(cmd)

        if file_type['model'] == True:
            print '\nSplit model file daily'
            # Extraction of UTC030000 or UTC150000, to add to output names created by cdo
            UTCtime = OutNames['model'].split(month)[1].split('.')[0]
            # Extract output basename
            outBaseName = OutNames['model'].split('UTC')[0]

            # Split into daily file
            cmd = 'cdo splitday '+OutNames['model']+' '+outBaseName
            os.system(cmd)

            # Add UTC030000 or UTC150000 to output files
            outFiles = glob.glob(outBaseName+'??.nc')
            if len(outFiles) == 0:
                print 'ERROR: no output files from cdo'
            else:
                for f in outFiles:
                    fFinalName = f.split('.')[0]+UTCtime+'.nc'
                    cmd = 'mv '+f+' '+fFinalName
                    os.system(cmd)

    ### NCKS ###
    if split == 'ncks':
        print '\n### NCKS ###'
        # Create datetime objects to loop over each day of the month
        dt1 = datetime.datetime.strptime(startDate, '%Y%m%d')
        dt2 = datetime.datetime.strptime(lastDate, '%Y%m%d')
        nb_days = (dt2-dt1).days + 1

        if file_type['surface'] == True:
            print '\nSplit surface file daily'
            # Loop over days
            for i in range(nb_days):
                dt = dt1 + datetime.timedelta(days=i)
                daySplit = str(dt.year)+'-%02d' % (dt.month)+'-%02d' % (dt.day)
                daySaved = str(dt.year)+'%02d' % (dt.month)+'%02d' % (dt.day)
                print dt

                # Split into daily file
                cmd = 'ncks -O -d time,'+daySplit+','+daySplit+' ' + \
                    OutNames['surface']+' '+OutNames['surface'].replace(month, daySaved)
                os.system(cmd)

        if file_type['pressure'] == True:
            print '\nSplit pressure file daily'
            # Loop over days
            for i in range(nb_days):
                dt = dt1 + datetime.timedelta(days=i)
                daySplit = str(dt.year)+'-%02d' % (dt.month)+'-%02d' % (dt.day)
                daySaved = str(dt.year)+'%02d' % (dt.month)+'%02d' % (dt.day)
                print dt

                # Split into daily file
                cmd = 'ncks -O -d time,'+daySplit+','+daySplit+' ' + \
                    OutNames['pressure']+' '+OutNames['pressure'].replace(month, daySaved)
                os.system(cmd)

        if file_type['model'] == True:
            print '\nSplit model file daily'
            # Loop over days
            for i in range(nb_days):
                dt = dt1 + datetime.timedelta(days=i)
                daySplit = str(dt.year)+'-%02d' % (dt.month)+'-%02d' % (dt.day)
                daySaved = str(dt.year)+'%02d' % (dt.month)+'%02d' % (dt.day)
                print dt

                # Split into daily file
                cmd = 'ncks -O -d time,'+daySplit+','+daySplit+' ' + \
                    OutNames['model']+' '+OutNames['model'].replace(month, daySaved)
                os.system(cmd)


# ==============
#     MAIN
# ==============

# =============================
#  Aerosol types
# =============================
# BC = BlackCarbon
# SS = SeaSalt
# SU = Sulfate
# DU = Dust
# OM = OrganicMatter


# ==================
# parse command line
# ==================
if len(sys.argv) == 1:
    prog = os.path.basename(sys.argv[0])
    print '      '+sys.argv[0]+' [options]'
    print "     Aide : ", prog, " --help"
    print "        ou : ", prog, " -h"
    print "example : python %s -d 20171101 -f 20171201 -a /mnt/data/DONNEES_AUX/CAMS_DBL/ -w /mnt/data/DONNEES_AUX/CAMS_TMP/ " % prog
    sys.exit(-1)
else:
    usage = "usage: %prog [options] "
    parser = OptionParser(usage=usage)
    parser.add_option("-d", "--start_date", dest="start_date", action="store", type="string",
                      help="start date, fmt('20171101')", default=None)
    parser.add_option("-f", "--end_date", dest="end_date", action="store", type="string",
                      help="end date, fmt('20171201')", default=None)
    parser.add_option("-w", "--write_dir", dest="write_dir", action="store", type="string",
                      help="Path where the products should be downloaded")
    parser.add_option("-a", "--archive_dir", dest="archive_dir", action="store", type="string",
                      help="Path where the archive DBL files are stored")
    parser.add_option("-k", "--keep", dest="keep", action="store_true",
                      help="keep netcdf files", default=False)
    parser.add_option("-r", "--reanalysis", dest="NRT", action="store_false",
                      help="get reanalysis instead of NRT", default=True)
    parser.add_option("-s", "--split_mode", dest="split_mode", action="store", type="choice",
                      choices=['cdo', 'ncks'], help="Option to split monthly file into daily files (using 'cdo' or 'ncks' , default:cdo)", default='cdo')

    # parser.add_option("-t", "--time",dest="time", action="store", type="choice", \
    #    choices=['00','12'],help="Time of forecast (Currently '00'or'12)",default='00')

    (options, args) = parser.parse_args()
    parser.check_required("-d")
    parser.check_required("-f")
    parser.check_required("-a")
    parser.check_required("-w")


# ====================
#     PARAMETERS
# ====================

if options.NRT==True:
    dataset="cams_nrealtime"
    expver='0001'
    # Analysis times
    # Two possibilities :
    # - 00:00:00 UTC (minuit)
    # - 12:00:00 UTC (midi)

    time = ["00", "12"]
else:
    dataset="cams_reanalysis"
    expver='eac4'
    time = ["00", "03", "06", "09", "12", "15", "18", "21"]


# Create date objects
dt1 = datetime.datetime.strptime(options.start_date, '%Y%m%d')
dt2 = datetime.datetime.strptime(options.end_date, '%Y%m%d')
today = datetime.datetime.today()

if dt2 < dt1:
    print("end date should be greater that start date")
    sys.exit(-2)

# download whole months except for current month
if dt1.year != today.year or dt1.month != today.month:
    dt1 = datetime.datetime(year=dt1.year, month=dt1.month, day=1)
    days_in_month = calendar.monthrange(dt2.year, dt2.month)[1]
    dt2 = datetime.datetime(year=dt2.year, month=dt2.month, day=days_in_month)
    mode = "monthly"
else:
    mode = "daily"


nb_days = (dt2-dt1).days + 1
print('Number of days =%s' % nb_days)

nb_months = relativedelta(dt2, dt1).months+(dt2.year - dt1.year)*12+1
print('Number of months =%s' % nb_months)



# Forcast times
# step = 3 corresponds to preisions, 3h after analysis.
# Examples : time = 00 and step = 3 => 03:00:00 UTC
#           time = 12 and step = 3 => 15:00:00 UTC
step = "0"

# Path out
path_out = options.write_dir

# 3 types of files to download
# Surface  : AOT (aerosol optical thickness)
# Pressure : RH  (relative humidity)
# Model    : MR  (mixing ratios)
file_type = {'surface': True, 'pressure': True, 'model': True}


# Boucle sur les jours a telecharger
if mode == "daily":
    for i in range(nb_days):
        dt = dt1 + datetime.timedelta(days=i)
        request_date = '%04d%02d%02d' % (dt.year, dt.month, dt.day)
        print("==================================")
        print("Downloading files for date %s" % dt)
        print("==================================")
        for t in range(len(time)):
            (nom_AOT, nom_RH, nom_MR) = download_files(
                request_date, file_type, time[t], step, path_out)
            # conversion to MAJA DBL/HDR format
            process_one_file(nom_AOT, nom_MR, nom_RH, path_out, options.archive_dir)
            if not(options.keep):
                os.remove(nom_AOT)
                os.remove(nom_MR)
                os.remove(nom_RH)
elif mode == "monthly":
    for i in range(nb_months):
        dt = dt1 + relativedelta(months=i)
        numberOfDays = calendar.monthrange(dt.year, dt.month)[1]
        startDate = '%04d%02d%02d' % (dt.year, dt.month, 1)
        lastDate = '%04d%02d%02d' % (dt.year, dt.month, numberOfDays)
        month = '%04d%02d' % (dt.year, dt.month)

        # Range of dates to download from CAMS
        requestDates = (startDate + "/TO/" + lastDate)
        print("==================================")
        print('Dates to download:%s' % requestDates)
        print("==================================")
        for t in range(len(time)):
            OutNames = {}
            OutNames['surface'] = path_out + "/CAMS_AOT_" + month + \
                'UTC' + str(int(time[t])+int(step)).zfill(2) + '0000.nc'
            OutNames['pressure'] = path_out + "/CAMS_RH_" + month + \
                'UTC' + str(int(time[t])+int(step)).zfill(2) + '0000.nc'
            OutNames['model'] = path_out + "/CAMS_MR_" + month + \
                'UTC' + str(int(time[t])+int(step)).zfill(2) + '0000.nc'
            # Download CAMS files from ECMWFDataServer
            (nom_AOT, nom_RH, nom_MR) = download_files(
                requestDates, file_type, time[t], step, OutNames, dataset, expver)

            # Split monthly files into daily files
            split_daily(month, OutNames, file_type, startDate, lastDate, options.split_mode)
            for day in range(calendar.monthrange(dt.year, dt.month)[1]):
                day = day+1
                download_date = '%04d%02d%02d' % (dt.year, dt.month, day)
                print(download_date)
                nom_AOT = path_out + "/CAMS_AOT_" + download_date + \
                    'UTC' + str(int(time[t])+int(step)).zfill(2) + '0000.nc'
                nom_RH = path_out + "/CAMS_RH_" + download_date + 'UTC' + \
                    str(int(time[t])+int(step)).zfill(2) + '0000.nc'
                nom_MR = path_out + "/CAMS_MR_" + download_date + 'UTC' + \
                    str(int(time[t])+int(step)).zfill(2) + '0000.nc'
                # create header and Datablock files expected by MAJA

                process_one_file(nom_AOT, nom_MR, nom_RH, path_out, options.archive_dir)
                if not(options.keep):
                    os.remove(nom_AOT)
                    os.remove(nom_MR)
                    os.remove(nom_RH)
