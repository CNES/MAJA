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

from __future__ import absolute_import
from __future__ import print_function

import argparse
from datetime import datetime
import os
import sys
import re

sys.path.append(os.path.join(os.path.dirname(os.path.realpath(__file__)), '..'))  # Import relative modules


class RawCAMSArchive(object):
    """
    Converts a set of raw .nc files to the Maja format
    """

    @staticmethod
    def get_cams_root(sensor):
        """
        Create the root of a single cams file
        :return:
        """
        from xml.etree import ElementTree
        from collections import namedtuple
        header_info = namedtuple("header_info", ("schema_path", "schema_version"))
        schema_by_sensor = {"s2": header_info("EXO_CAMS_CamsData.xsd", "1.00"),
                            "l8": header_info("LN_EXO_CAMS_CamsData.xsd", "1.00"),
                            "ve": header_info("VE_EXO_CAMS_CamsData.xsd", "2.0"),
                            "gen": header_info("GEN_EXO_CAMS_CamsData.xsd", "1.00")}
        xmlns = "http://eop-cfi.esa.int/CFI"
        xsi = "http://www.w3.org/2001/XMLSchema-instance"
        schema_location = "%s ./%s" % (xmlns, schema_by_sensor[sensor].schema_path)
        type_xsi = "CAMS_Header_Type"
        root = ElementTree.Element("Earth_Explorer_Header",
                                   attrib={"xmlns": xmlns,
                                           "schema_version": schema_by_sensor[sensor].schema_version,
                                           "{" + xsi + "}schemaLocation": schema_location,
                                           "{" + xsi + "}type": type_xsi})
        return root

    @staticmethod
    def get_raw_cams_date(cams_file, cams_group="(AOT|MR|RH)"):
        """
        Get the date from a file of the format CAMS_XXX_yyyymmddUTChhmmss.nc
        :param cams_file: The filename string of the cams file
        :param cams_group: Specify the type of CAMS group the file should belong in.
                           By default this is either AOT, RH or MR
        :return: The date of the given file as well as it's group type.
        """
        assert cams_group[0] == "(" and cams_group[-1] == ")"
        regex = r"CAMS_%s_(\d{8}UTC\d{6})\.nc$" % cams_group
        result = re.search(regex, cams_file)
        if result:
            return datetime.strptime(result.group(2), "%Y%m%dUTC%H%M%S"), result.group(1)
        return None

    @staticmethod
    def _update_nodes(root, mission, basename_out, date_end, acquisition_date, rel_netcdf):
        """
        Update the HDR file with the needed information.
        :param root: The HDR xml-root.
        :param mission: The full mission name, e.g. 'SENTINEL2_'
        :param basename_out: The basename of the HDR
        :param date_end: The validity end date, usually 2100-01-01
        :param acquisition_date: The acquisition date
        :param rel_netcdf: The list of CAMS files inside the .DBL.DIR. The filepaths are all relative to the HDR.
        :return: Writes the HDR in the same directory as the .DBL.DIR.
        """
        from xml.etree import ElementTree
        creation_date = datetime.now()
        format_change_date = datetime(2019, 7, 10, 0, 0, 0)
        old_format = acquisition_date < format_change_date
        if old_format:
            press_levels = [0.2, 0.3843, 0.6365, 0.9564, 1.3448, 1.8058, 2.3478, 2.985, 3.7397, 4.6462, 5.7565, 7.1322,
                            8.8366, 10.9483, 13.5647, 16.8064, 20.8227, 25.7989, 31.9642, 39.6029, 49.0671, 60.1802,
                            73.0663, 87.7274, 104.229, 122.614, 142.902, 165.089, 189.147, 215.025, 242.652, 272.059,
                            303.217, 336.044, 370.407, 406.133, 443.009, 480.791, 519.209, 557.973, 596.777, 635.306,
                            673.24, 710.263, 746.063, 780.346, 812.83, 843.263, 871.42, 897.112, 920.189, 940.551,
                            958.148, 972.987, 985.14, 994.747, 1002.02, 1007.26, 1010.85, 1013.25]
        else:
            press_levels = [0.02, 0.031, 0.0457, 0.0683, 0.0975, 0.1361, 0.1861, 0.2499, 0.3299, 0.4288, 0.5496, 0.6952,
                            0.869, 1.0742, 1.3143, 1.5928, 1.9134, 2.2797, 2.6954, 3.1642, 3.6898, 4.2759, 4.9262,
                            5.6441, 6.4334, 7.2974, 8.2397, 9.2634, 10.3720, 11.5685, 12.8561, 14.2377, 15.7162,
                            17.2945, 18.9752, 20.7510, 22.6543, 24.6577, 26.7735, 29.0039, 31.3512, 33.8174, 36.4047,
                            39.1149, 41.9493, 44.9082, 47.9915, 51.1990, 54.5299, 57.9834, 61.5607, 65.2695, 69.1187,
                            73.1187, 77.2810, 81.6182, 86.1450, 90.8774, 95.8280, 101.0047, 106.4153, 112.0681,
                            117.9714, 124.1337, 130.5637, 137.2703, 144.2624, 151.5493, 159.1403, 167.0450, 175.2731,
                            183.8344, 192.7389, 201.9969, 211.6186, 221.6146, 231.9954, 242.7719, 253.9549, 265.5556,
                            277.5852, 290.0548, 302.9762, 316.3607, 330.2202, 344.5663, 359.4111, 374.7666, 390.6450,
                            407.0583, 424.0190, 441.5395, 459.6321, 478.3096, 497.5845, 517.4198, 537.7195, 558.3430,
                            579.1926, 600.1668, 621.1624, 642.0764, 662.8084, 683.2620, 703.3467, 722.9795, 742.0855,
                            760.5996, 778.4661, 795.6396, 812.0847, 827.7756, 842.6959, 856.8376, 870.2004, 882.7910,
                            894.6222, 905.7116, 916.0815, 925.7571, 934.7666, 943.1399, 950.9082, 958.1037, 964.7584,
                            970.9046, 976.5737, 981.7968, 986.6036, 991.0230, 995.0824, 998.8081, 1002.2250, 1005.3562,
                            1008.2239, 1010.8487, 1013.2500]

        a1 = ElementTree.SubElement(root, "Fixed_Header")
        toto = ElementTree.SubElement(a1, "File_Name")
        toto.text = basename_out
        toto = ElementTree.SubElement(a1, "File_Description")
        toto.text = "CAMSData"
        toto = ElementTree.SubElement(a1, "Notes")
        toto.text = "60 Model levels" if old_format else "137 Model levels"
        toto = ElementTree.SubElement(a1, "Mission")
        toto.text = mission
        toto = ElementTree.SubElement(a1, "File_Class")
        toto.text = "TEST"
        toto = ElementTree.SubElement(a1, "File_Type")
        toto.text = "EXO_CAMS"
        b1 = ElementTree.SubElement(a1, "Validity_Period")
        toto = ElementTree.SubElement(b1, "Validity_Start")
        toto.text = "UTC=%s" % acquisition_date.strftime("%Y-%m-%dT%H:%M:%S")
        toto = ElementTree.SubElement(b1, "Validity_Stop")
        toto.text = "UTC=%s" % date_end.strftime("%Y-%m-%dT%H:%M:%S")
        toto = ElementTree.SubElement(a1, "File_Version")
        toto.text = "0003"
        b2 = ElementTree.SubElement(a1, "Source")
        toto = ElementTree.SubElement(b2, "System")
        toto.text = "MAJA"
        toto = ElementTree.SubElement(b2, "Creator")
        toto.text = "Start_Maja"
        toto = ElementTree.SubElement(b2, "Creator_Version")
        toto.text = "1.14"
        toto = ElementTree.SubElement(b2, "Creation_Date")
        toto.text = "UTC=" + creation_date.strftime("%Y-%m-%dT%H:%M:%S")

        a = ElementTree.SubElement(root, "Variable_Header")
        b2 = ElementTree.SubElement(a, "Main_Product_Header")
        ElementTree.SubElement(b2, "List_of_Consumers", count="0")
        ElementTree.SubElement(b2, "List_of_Extensions", count="0")
        b3 = ElementTree.SubElement(a, "Specific_Product_Header")
        b4 = ElementTree.SubElement(b3, "Instance_Id")

        b5 = ElementTree.SubElement(b4, "Validity_Period")
        toto = ElementTree.SubElement(b5, "Validity_Start")
        toto.text = "UTC=%s" % acquisition_date.strftime("%Y-%m-%dT%H:%M:%S")
        toto = ElementTree.SubElement(b5, "Validity_Stop")
        toto.text = "UTC=%s" % date_end.strftime("%Y-%m-%dT%H:%M:%S")
        b5 = ElementTree.SubElement(b4, "Acquisition_Date_Time")
        b5.text = "UTC=" + acquisition_date.isoformat()

        b4 = ElementTree.SubElement(b3, "Data_Block_Source_Location")
        b4.text = "https://www.ecmwf.int"

        b4 = ElementTree.SubElement(b3, "DBL_Organization")
        b5 = ElementTree.SubElement(b4, "List_of_Packaged_DBL_Files", count=str(len(rel_netcdf)))
        for index, cams_file in enumerate(rel_netcdf):
            b6 = ElementTree.SubElement(b5, "Packaged_DBL_File", sn=str(index+1))
            b7 = ElementTree.SubElement(b6, "Relative_File_Path")
            b7.text = cams_file
            b7 = ElementTree.SubElement(b6, "File_Definition")
            b7.text = "cams"

        b4 = ElementTree.SubElement(b3, "ModelLevels")
        b4.text = " ".join(str(p) for p in press_levels)
        if not old_format:
            b5 = ElementTree.SubElement(b3, "NumberOfNonInterpolableValues")
            b5.text = "20"

    @staticmethod
    def create_archive(output_dir, netcdf, output_file_basename):
        """
        Create the DBL.DIR folder
        :param output_dir: The output directory
        :param netcdf: The list of netcdf files
        :param output_file_basename: The basename of the cams file of format .._EXO_CAMS_...
        :return: The DBL folder path as well as the relative path to the cams files.
        """
        import shutil
        from StartMaja.Common import FileSystem
        destination_filename = "{}.DBL".format(output_file_basename)
        destination_filepath = os.path.join(output_dir, destination_filename)
        dbl_dir = os.path.join(output_dir, output_file_basename + ".DBL.DIR")
        FileSystem.create_directory(dbl_dir)
        cams_file_to_return = []
        for ncf in netcdf:
            shutil.copy(ncf, os.path.join(dbl_dir, os.path.basename(ncf)))
            cams_file_to_return.append(os.path.join(destination_filename + ".DIR", os.path.basename(ncf)))
        return destination_filepath, cams_file_to_return

    @staticmethod
    def get_list_of_cams_files(input_dir):
        """
        Get all available cams files inside a directory tree sorting out the duplicates.
        :return: The list of available cams files.
        """
        from StartMaja.Common import FileSystem
        regex = r"CAMS_(AOT|MR|RH)_(\d{8}UTC\d{6})\.nc$"
        raw_files = FileSystem.find(regex, input_dir)
        unique_files, basenames = [], []
        # Sort out duplicates by basename
        for i in raw_files:
            bname = os.path.basename(i)
            if bname not in basenames:
                basenames.append(bname)
                unique_files.append(i)
        return unique_files

    @staticmethod
    def get_raw_cams_triplet(aot_file, file_list):
        """
        Get the triplets of MR, AOT and RH CAMS files for the same date.
        :param aot_file: The AOT file of a given date.
        :param file_list: The list of other raw cams files present.
        :return: The AOT, RH and MR files for a same datetime.
        """
        cams_types = ["AOT", "MR", "RH"]
        cams_file = {i: None for i in cams_types}

        # Start out using the date of a valid AOT file:
        date_aot, _ = RawCAMSArchive.get_raw_cams_date(aot_file)
        input_dir = os.path.dirname(aot_file)
        for fl in file_list:
            # Find corresponding MR and RH files:
            date_mr_rh, cams_type = RawCAMSArchive.get_raw_cams_date(fl)
            if cams_type not in cams_types:
                continue
            if date_mr_rh == date_aot and not cams_file[str(cams_type)]:
                cams_file[cams_type] = os.path.join(input_dir, fl)
        for i in cams_file.values():
            if not os.path.isfile(i):
                return None
        return list(cams_file.values())

    @staticmethod
    def process_one_file(output_dir, aot_file, rh_file, mr_file, mission="s2"):
        """
        Process a single netcdf triplet
        :param output_dir: The existing output directory
        :param aot_file: The AOT .netcdf file
        :param rh_file: The RH .netcdf file
        :param mr_file: The MR .netcdf file
        :param mission: The desired mission.
        :return: Writes the DBL.DIR and HDR to the given output directory
        """
        from StartMaja.Common import XMLTools
        from collections import namedtuple
        satellite = namedtuple('satellite', ('full_name', 'short_name'))
        mission_choices = {"s2": satellite("SENTINEL-2_", "S2_"),
                           "l8": satellite("LANDSAT_8", "L8"),
                           "ve": satellite("VENuS", "VE"),
                           "gen": satellite("GENERIC", "GEN")}
        current_satellite = mission_choices[mission]
        date_end = datetime(year=2100, month=1, day=1)
        acq_date = RawCAMSArchive.get_raw_cams_date(aot_file)[0]

        if not acq_date:
            raise ValueError("Cannot get Acquisition date for CAMS file %s " % aot_file)

        output_file_basename = "{}_OPER_EXO_CAMS_{}_{}".format(current_satellite.short_name,
                                                               acq_date.strftime("%Y%m%dT%H%M%S"),
                                                               date_end.strftime("%Y%m%dT%H%M%S"))

        # Create archive
        netcdf = [aot_file, rh_file, mr_file]
        dbl_filename, cams = RawCAMSArchive.create_archive(output_dir, netcdf, output_file_basename)

        # Create hdr
        output_filename = os.path.join(output_dir, output_file_basename + ".HDR")
        basename_out = os.path.basename(os.path.splitext(output_filename)[0])
        root = RawCAMSArchive.get_cams_root(mission)
        RawCAMSArchive._update_nodes(root, current_satellite.full_name, basename_out, date_end, acq_date, cams)
        XMLTools.write_xml(root, output_filename)


if __name__ == "__main__":
    argParser = argparse.ArgumentParser()
    required_arguments = argParser.add_argument_group('required arguments')
    required_arguments.add_argument('-i', '--input_dir', required=True,
                                    help='Path to input image')
    required_arguments.add_argument('-o', '--output_dir', required=True,
                                    help='Path to output dir')
    required_arguments.add_argument('-s', "--sensor", choices=['s2', 'l8', 've', 'gen'], required=True)

    args = argParser.parse_args()
    cams_triplets = []
    cams_files = RawCAMSArchive.get_list_of_cams_files(args.input_dir)
    for f in cams_files:
        if RawCAMSArchive.get_raw_cams_date(f, cams_group="(AOT)"):
            cams_triplets.append(RawCAMSArchive.get_raw_cams_triplet(f, cams_files))
    # Sort out non-cams files:
    cams_triplets = [c_file for c_file in cams_triplets if c_file is not None]
    for nc in cams_triplets:
        aot, rh, mr = nc[0], nc[1], nc[2]
        RawCAMSArchive.process_one_file(args.output_dir, aot, rh, mr, args.sensor)
