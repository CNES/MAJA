# -*- coding: utf-8 -*-6
#
# Copyright (C) 2020 Centre National d'Etudes Spatiales (CNES)
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#    http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#
"""
###################################################################################################

                        o     o
                        oo   oo   oo        o   oo        ,-.
                        o o o o  o  o       o  o  o       \_/
                        o  o  o o    o      o o    o     {|||D
                        o     o oooooo      o oooooo      / \
                        o     o o    o o    o o    o      `-^
                        o     o o    o  oooo  o    o

###################################################################################################

orchestrator.cots.otb.otb_band_math -- shortdesc

orchestrator.cots.otb.otb_band_math is a description

It defines classes_and_methods


###################################################################################################
"""


from orchestrator.common.logger.maja_logging import configure_logger
from orchestrator.cots.gdal.gdal_dataset_info import GdalDatasetInfo
import argparse
import glob
import os
import sys
import lxml.etree as ET

LOGGER = configure_logger(__name__)

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
    # <Earth_Explorer_Header schema_version="1.00"
    #   xsi:schemaLocation="http://eop-cfi.esa.int/CFI ./AUX_REFDE2_ReferenceDemDataLevel2.xsd"
    #   xmlns="http://eop-cfi.esa.int/CFI" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
    #   xsi:type="REFDE2_Header_Type">
    xmlns = "http://eop-cfi.esa.int/CFI"
    xsi = "http://www.w3.org/2001/XMLSchema-instance"
    schemaLocation = "http://eop-cfi.esa.int/CFI ./AUX_REFDE2_ReferenceDemDataLevel2.xsd"
    typeXsi = "REFDE2_Header_Type"
    root = ET.Element("Earth_Explorer_Header", attrib={"schema_version": "1.00",
                                                       "{" + xsi + "}schemaLocation": schemaLocation,
                                                       "{" + xsi + "}type": typeXsi},
                      nsmap={'xsi': xsi, None: xmlns})

    #    xmlns = ""
    # xsi = "http://www.w3.org/2001/XMLSchema-instance"
    # schemaLocation = "Dimapv2_kalideos_product.xsd"
    # root = ET.Element("Dimap_Document", attrib={"{" + xsi + "}schemaLocation": schemaLocation},
    #                   nsmap={'xsi': xsi, None: xmlns})
    # <Dimap_Document
    # xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
    # xsi:noNamespaceSchemaLocation="Schemas/Dimapv2_phr_system_rectified_product.xsd">

    return root


def searchOneFile(directory, filePattern):
    """
    Use glob to find a file matching the given filePattern
    :param directory:
    :param filePattern:
    :return:
    """
    # print "directory, filePattern", directory, filePattern
    resList = myGlob(directory, filePattern)
    # print resList
    if resList:
        if len(resList) > 1:
            LOGGER.debug("Warning, more than one value matching the pattern", filePattern, "in", directory)
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


def get_specific_annex_infomation(root, key):
    if key not in ["FRE", "SRE"]:
        b7 = ET.SubElement(root, "Data_Type")
        b7.text = "UNSIGNED_BYTE"
    else:
        b7 = ET.SubElement(root, "Data_Type")
        b7.text = "SIGNED_SHORT"

    b7 = ET.SubElement(root, "Number_of_Significant_Bits")
    b7.text = NUMBER_OF_SIGNIFICANT_BITS[key]
    if key == "ATB":
        b7 = ET.SubElement(root, "VAP_Nodata_Value")
        b7.text = "0"
        b7 = ET.SubElement(root, "VAP_Quantification_Value")
        b7.text = "0.05"
        b7 = ET.SubElement(root, "AOT_Nodata_Value")
        b7.text = "0"
        b7 = ET.SubElement(root, "AOT_Quantification_Value")
        b7.text = "0.005"
    elif key in ["CLD", "MSK", "QLT"]:
        toto = ET.SubElement(root, "Nodata_Value")
        toto.text = "NaN"
    elif key in ["FRE", "SRE"]:
        toto = ET.SubElement(root, "Nodata_Value")
        toto.text = "-10000"


def get_specific_annex_infomation_at_end(root, key):
    if key == "ATB":
        pass
    elif key in ["CLD", "MSK", "QLT"]:
        toto = ET.SubElement(root, "Quantification_Bit_Value")
        toto.text = "1"


def get_list_of_bands(root, key):
    if key == "ATB":
        toto = ET.SubElement(root, "Band", sn="1")
        toto.text = "VAP"
        toto = ET.SubElement(root, "Band", sn="2")
        toto.text = "AOT"
    elif key == "CLD":
        toto = ET.SubElement(root, "Band", sn="1")
        toto.text = "CLD"
    elif key == "MSK":
        toto = ET.SubElement(root, "Band", sn="1")
        toto.text = "MSK"
    elif key == "QLT":
        toto = ET.SubElement(root, "Band", sn="1")
        toto.text = "SAT"
        toto = ET.SubElement(root, "Band", sn="2")
        toto.text = "PIX"
        toto = ET.SubElement(root, "Band", sn="3")
        toto.text = "OTH"
    elif key in ["FRE", "SRE"]:
        toto = ET.SubElement(root, "Band", sn="1")
        toto.text = "B2"
        toto = ET.SubElement(root, "Band", sn="2")
        toto.text = "B3"
        toto = ET.SubElement(root, "Band", sn="3")
        toto.text = "B4"
        toto = ET.SubElement(root, "Band", sn="4")
        toto.text = "B8"


def nodes(root, info, mission, basename_out, key):

    a1 = ET.SubElement(root, "Fixed_Header")
    toto = ET.SubElement(a1, "File_Name")
    toto.text = basename_out
    toto = ET.SubElement(a1, "File_Description")
    if not key[:3] in ["FRE", "SRE"]:
        toto.text = "AnnexProduc"
    else:
        toto.text = "ImageProduct"
    toto = ET.SubElement(a1, "Notes")
    toto.text = "L2 note"
    toto = ET.SubElement(a1, "Mission")
    toto.text = mission
    toto = ET.SubElement(a1, "File_Class")
    toto.text = "OPER"
    toto = ET.SubElement(a1, "File_Type")
    if not key[:3] in ["FRE", "SRE"]:
        toto.text = "SSC_PDTANX"
    else:
        toto.text = "SSC_PDTIMG"
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
    toto.text = "MAJA_L2_NOMINAL_CHAIN"
    toto = ET.SubElement(b2, "Creator_Version")
    toto.text = "1.2.0"
    toto = ET.SubElement(b2, "Creation_Date")
    toto.text = "UTC=2010-02-10T11:00:00"

    a = ET.SubElement(root, "Variable_Header")
    b2 = ET.SubElement(a, "Main_Product_Header")
    ET.SubElement(b2, "List_of_Consumers", count="0")
    ET.SubElement(b2, "List_of_Extensions", count="0")
    b3 = ET.SubElement(a, "Specific_Product_Header")
    b4 = ET.SubElement(b3, "Instance_Id")
    toto = ET.SubElement(b4, "Reference_Product_Semantic")
    toto.text = "L2VALD"
    toto = ET.SubElement(b4, "Reference_Product_Instance")
    toto.text = "____".join([x for x in basename_out.split("_") if x is not ''][-4:-2])
    if not key[:3] in ["FRE", "SRE"]:
        toto = ET.SubElement(b4, "Annex_Code")
        toto.text = key[:3]

    b4 = ET.SubElement(b3, "Reference_Product_Header_Id")
    b4.text = "_".join((basename_out.split("_")[:-2]))
    if not key[:3] in ["FRE", "SRE"]:
        b4 = ET.SubElement(b3, "Annex_Complete_Name")
        b4.text = "Other"

    if not key[:3] in ["FRE", "SRE"]:
        b6 = ET.SubElement(b3, "Annex_Information")
    else:
        b6 = ET.SubElement(b3, "Image_Information")
    b7 = ET.SubElement(b6, "Format")
    b7.text = "GEOTIFF"
    b7 = ET.SubElement(b6, "Binary_Encoding")
    b7.text = "LITTLE_ENDIAN"
    get_specific_annex_infomation(b6, key[:3])

    b7 = ET.SubElement(b6, "Size")
    toto = ET.SubElement(b7, "Lines")
    toto.text = str(info.size[0])
    toto = ET.SubElement(b7, "Columns")
    toto.text = str(info.size[1])
    toto = ET.SubElement(b7, "Bands")
    toto.text = str(info.number_of_bands)

    b7 = ET.SubElement(b6, "Image_Compacting_Tool")
    b7.text = "NO"
    b7 = ET.SubElement(b6, "List_of_Bands", count=str(info.number_of_bands))
    get_list_of_bands(b7, key[:3])

    if not key[:3] in ["FRE", "SRE"]:
        b7 = ET.SubElement(b6, "Subsampling_Factor")
        toto = ET.SubElement(b7, "By_Line")
        toto.text = "1"
        toto = ET.SubElement(b7, "By_Column")
        toto.text = "1"
        b7 = ET.SubElement(b6, "Values_Unit")
        b7.text = "nil"
        get_specific_annex_infomation_at_end(b6, key[:3])


def process_one_file(input_dir, out_dir, fileOO_pattern):

    if not os.path.isdir(input_dir):
        LOGGER.error("Input directory does not exist ! %s", input_dir)
        return
    if os.listdir(input_dir) == []:
        LOGGER.error("Input directory is empty ! %s", input_dir)
        return

    mission = "SENTINEL-2A" if os.path.basename(input_dir)[:3] == "S2A" else "SENTINEL-2B"

    image00 = searchOneFile(input_dir, "*{}*.TIF".format(fileOO_pattern))
    if image00 is None:
        LOGGER.info("No image file found %s. Search directory: %s", fileOO_pattern, input_dir)
        return
    LOGGER.debug("image00 %s", image00)
    output_filename = os.path.join(
        out_dir, os.path.basename(
            os.path.splitext(
                os.path.splitext(image00)[0])[0] + ".HDR"))
    LOGGER.debug(output_filename)

    info = GdalDatasetInfo(image00)
    basename_out = os.path.basename(os.path.splitext(output_filename)[0])
    LOGGER.debug(basename_out)

    # ET.register_namespace("", "http://eop-cfi.esa.int/CFI")
    root = getRoot()

    nodes(root, info, mission, basename_out, fileOO_pattern)

    tree = ET.ElementTree(root)

    f = open(output_filename, "w")
    f.write(ET.tostring(tree, pretty_print=True, xml_declaration=True,
                        encoding="UTF-8"))
    f.close()


def get_header(input_dir, key, out_dir):
    process_one_file(input_dir, out_dir, key)


def hdr_creation(input_dir, out_dir=None):
    LOGGER.debug("specific nodes hd_creation input_dir %s", input_dir)
    LOGGER.debug("specific nodes hd_creation out_dir %s", out_dir)

    if not os.path.isdir(input_dir):
        LOGGER.error("Input directory does not exist ! %s", input_dir)
        return
    if os.listdir(input_dir) == []:
        LOGGER.error("Input directory is empty ! %s", input_dir)
        return

    if out_dir is None:
        out_dir = input_dir

    files_order = ["ATB_R1", "ATB_R2",
                   "CLD_R1", "CLD_R2",
                   "MSK_R1", "MSK_R2",
                   "QLT_R1", "QLT_R2",
                   "FRE_R1", "FRE_R2",
                   "SRE_R1", "SRE_R2"]

    for fileOO_pattern in files_order:
        process_one_file(input_dir, out_dir, fileOO_pattern)


if __name__ == '__main__':
    input_dir = "/home/alexia/Maja/Maja_folders/L2A_MAJA/Toulouse/31TCJ/MAJA_1_0_S2AS2B_NATIF/S2B_OPER_SSC_L2VALD_31TCJ____20171127.DBL.DIR"
    hdr_creation(input_dir, "/tmp")
    get_header(input_dir, "SRE_R2", "/tmp/toto")
