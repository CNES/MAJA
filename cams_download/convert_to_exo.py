# -*- coding: utf-8 -*-6
"""
##########################################################################################

                        o     o
                        oo   oo   oo        o   oo        ,-.
                        o o o o  o  o       o  o  o       \_/
                        o  o  o o    o      o o    o     {|||D
                        o     o oooooo      o oooooo      / \
                        o     o o    o o    o o    o      `-^
                        o     o o    o  oooo  o    o

##########################################################################################

It defines classes_and_methods for CAMS data management
Author: Alexia Mondot (CS-SI), modified by O.Hagolle (CESBIO/CNES)

==================== Copyright
Software :convert_to_exo.py

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

from __future__ import absolute_import
from __future__ import print_function

import argparse
import datetime
import glob
import os
import re
import shutil
import sys
import tempfile

import lxml.etree as ET

import logging
LOGGER = logging.getLogger('Start-Maja')
LOGGER.setLevel(logging.DEBUG)
ch = logging.StreamHandler()
ch.setLevel(logging.DEBUG)
formatter = logging.Formatter('%(asctime)s - %(name)s - %(levelname)s - %(message)s')
ch.setFormatter(formatter)
LOGGER.addHandler(ch)

NUMBER_OF_SIGNIFICANT_BITS = {"ATB": "8", "CLD": "8", "MSK": "6", "QLT": "12",
                              "FRE": "16", "SRE": "16"}


def get_parameters():

    argParser = argparse.ArgumentParser()
    required_arguments = argParser.add_argument_group('required arguments')
    required_arguments.add_argument('-i', '--input_dir', required=True,
                                    help='Path to input image')
    required_arguments.add_argument('-o', '--output_filename', required=True,
                                    help='Path to output HDR filename')
    required_arguments.add_argument('-s', "--sensor", choices=['s2', 'l8'], required=True)

    args = argParser.parse_args(sys.argv[1:])

    input_dir = os.path.realpath(args.input_dir)
    output_filename = os.path.realpath(args.output_filename)
    sensor = args.sensor

    return input_dir, output_filename, sensor


def getRoot():
    xmlns = "http://eop-cfi.esa.int/CFI"
    xsi = "http://www.w3.org/2001/XMLSchema-instance"
    schemaLocation = "http://eop-cfi.esa.int/CFI ./EXO_CAMS_CamsData.xsd"
    typeXsi = "CAMS_Header_Type"
    root = ET.Element("Earth_Explorer_Header", attrib={"schema_version": "1.00",
                                                       "{" + xsi + "}schemaLocation": schemaLocation,
                                                       "{" + xsi + "}type": typeXsi},
                      nsmap={'xsi': xsi, None: xmlns})
    return root


def searchOneFile(directory, filePattern):
    """
    Use glob to find a file matching the given filePattern
    :param directory:
    :param filePattern:
    :return:
    """
    resList = myGlob(directory, filePattern)
    if resList:
        if len(resList) > 1:
            print("Warning, more than one value matching the pattern", filePattern, "in", directory)
        return resList[0]
    return None


def myGlob(directory, filePattern):
    """
    Creates an automatic path.join with given arguments
    :param directory:
    :param filePattern:
    :return:
    """
    return glob.glob(os.path.join(directory, filePattern))


def nodes(root, mission, basename_out, dbl_file, date_now, acquisition_date, cams_files):

    a1 = ET.SubElement(root, "Fixed_Header")
    toto = ET.SubElement(a1, "File_Name")
    toto.text = basename_out
    toto = ET.SubElement(a1, "File_Description")
    toto.text = "CAMSData"
    toto = ET.SubElement(a1, "Notes")
    toto.text = ""
    toto = ET.SubElement(a1, "Mission")
    toto.text = mission
    toto = ET.SubElement(a1, "File_Class")
    toto.text = "TEST"
    toto = ET.SubElement(a1, "File_Type")
    toto.text = "EXO_CAMS"
    b1 = ET.SubElement(a1, "Validity_Period")
    toto = ET.SubElement(b1, "Validity_Start")
    toto.text = "UTC=2006-07-01T18:11:45"
    toto = ET.SubElement(b1, "Validity_Stop")
    toto.text = "UTC=9999-99-99T99:99:99"
    toto = ET.SubElement(a1, "File_Version")
    toto.text = "0003"
    b2 = ET.SubElement(a1, "Source")
    toto = ET.SubElement(b2, "System")
    toto.text = "MAJA"
    toto = ET.SubElement(b2, "Creator")
    toto.text = "OzoneDataHeaderGenerator"
    toto = ET.SubElement(b2, "Creator_Version")
    toto.text = "1.14"
    toto = ET.SubElement(b2, "Creation_Date")
    toto.text = "UTC=" + date_now.isoformat()

    a = ET.SubElement(root, "Variable_Header")
    b2 = ET.SubElement(a, "Main_Product_Header")
    ET.SubElement(b2, "List_of_Consumers", count="0")
    ET.SubElement(b2, "List_of_Extensions", count="0")
    b3 = ET.SubElement(a, "Specific_Product_Header")
    b4 = ET.SubElement(b3, "Instance_Id")

    b5 = ET.SubElement(b4, "Validity_Period")
    toto = ET.SubElement(b5, "Validity_Start")
    toto.text = "UTC=2006-07-01T18:11:45"
    toto = ET.SubElement(b5, "Validity_Stop")
    toto.text = "UTC=9999-99-99T99:99:99"
    b5 = ET.SubElement(b4, "Acquisition_Date_Time")
    b5.text = "UTC=" + acquisition_date.isoformat()

    b4 = ET.SubElement(b3, "Data_Block_Source_Location")
    b4.text = "http://"

    b4 = ET.SubElement(b3, "DBL_Organization")
    b5 = ET.SubElement(b4, "List_of_Packaged_DBL_Files", count=str(len(cams_files)))
    for index, cams_file in enumerate(cams_files):
        b6 = ET.SubElement(b5, "Packaged_DBL_File", sn=str(index+1))
        b7 = ET.SubElement(b6, "Relative_File_Path")
        b7.text = cams_file
        b7 = ET.SubElement(b6, "File_Definition")
        b7.text = "cams"

    b4 = ET.SubElement(b3, "ModelLevels")
    b4.text = "0.2 0.3843 0.6365 0.9564 1.3448 1.8058 2.3478 2.985 3.7397 4.6462 5.7565 7.1322 8.8366 10.9483 13.5647 16.8064 20.8227 25.7989 31.9642 39.6029 49.0671 60.1802 73.0663 87.7274 104.229 122.614 142.902 165.089 189.147 215.025 242.652 272.059 303.217 336.044 370.407 406.133 443.009 480.791 519.209 557.973 596.777 635.306 673.24 710.263 746.063 780.346 812.83 843.263 871.42 897.112 920.189 940.551 958.148 972.987 985.14 994.747 1002.02 1007.26 1010.85 1013.25"


def compress_directory_bzip2(destination_filename, source_directory):
    basePath = os.path.dirname(source_directory)
    baseModule = os.path.basename(source_directory)
    command = "tar -cjf {} -C{} {} --exclude '.svn'".format(
        destination_filename, basePath, baseModule)
    os.system(command)


def date_time_for_naming(datetime_object):
    """
    :return: the dateTtime of the ROKDate for naming convention
    :rtype: str
    """
    return "{:04d}{:02d}{:02d}T{:02d}{:02d}{:02d}".format(datetime_object.year,
                                                          datetime_object.month,
                                                          datetime_object.day,
                                                          datetime_object.hour,
                                                          datetime_object.minute,
                                                          datetime_object.second)


def create_archive(aot_file, mr_file, rh_file, output_file_basename, ncdf_dir, archive_dir):
    destination_filename = "{}.DBL".format(output_file_basename)
    destination_filepath = os.path.join(archive_dir, destination_filename)

    temp_dir = os.path.join(archive_dir, output_file_basename + ".DBL.DIR")
    os.makedirs(temp_dir)
    shutil.copy(aot_file, os.path.join(temp_dir, os.path.basename(aot_file)))
    shutil.copy(mr_file, os.path.join(temp_dir, os.path.basename(mr_file)))
    shutil.copy(rh_file, os.path.join(temp_dir, os.path.basename(rh_file)))

    #compress_directory_bzip2(destination_filepath, archive_dir)
    cams_file_to_return = [os.path.join(destination_filename + ".DIR", os.path.basename(aot_file)),
                           os.path.join(destination_filename + ".DIR", os.path.basename(mr_file)),
                           os.path.join(destination_filename + ".DIR", os.path.basename(rh_file))]
    return destination_filepath, cams_file_to_return


def get_date(cams_file):
    try:
        res = re.search(r".+_.+_(.+)\.nc", cams_file).group(1)
    except IndexError:
        print("No date found in {}".format(cams_file))
        return None
    try:
        return datetime.datetime.strptime(res, "%Y%m%dUTC%H%M%S")
    except ValueError:
        print("No date %Y%m%dUTC%H%M%S found in {}".format(res))
        return None


def check_and_return_date(aot_file, mr_file, rh_file):

    date_aot = re.search(r"AOT_(.+)\.nc", aot_file).group(1)
    date_mr = re.search(r"MR_(.+)\.nc", mr_file).group(1)
    date_rh = re.search(r"RH_(.+)\.nc", rh_file).group(1)

    if date_aot != date_mr != date_rh:
        raise Exception("The 3 files must be from the same date ! \n{}\n{}\n{}".format(
            aot_file, mr_file, rh_file))
    return datetime.datetime.strptime(date_aot, "%Y%m%dUTC%H%M%S")


def back_to_filename_date(datetime_file):

    return datetime_file.strftime("%Y%m%dUTC%H%M%S")


def process_one_file(aot_file, mr_file, rh_file, ncdf_dir, archive_dir):

    #working_dir = tempfile.mkdtemp(dir=working_dir)

    mission = "SENTINEL-2_"
    date_file = check_and_return_date(aot_file, mr_file, rh_file)
    date_now = datetime.datetime.now()

    output_file_basename = "S2__TEST_EXO_CAMS_{}_{}".format(date_time_for_naming(date_file),
                                                            date_time_for_naming(date_now))

    # create archive
    dbl_filename, cams = create_archive(
        aot_file, mr_file, rh_file, output_file_basename, ncdf_dir, archive_dir)

    print("Step 1/2", end='\r')

    # create hdr
    output_filename = os.path.join(archive_dir, output_file_basename + ".HDR")
    LOGGER.debug(output_filename)
    basename_out = os.path.basename(os.path.splitext(output_filename)[0])
    LOGGER.debug(basename_out)
    root = getRoot()

    nodes(root, mission, basename_out, dbl_filename, date_now, date_file, cams)

    print("Step 2/2", end='\r')

    tree = ET.ElementTree(root)
    f = open(output_filename, "w")
    f.write(ET.tostring(tree, pretty_print=True, xml_declaration=True,
                        encoding="UTF-8"))
    f.close()
