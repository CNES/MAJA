# -*- coding: utf-8 -*-
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
orchestrator.tests.settings -- shortdesc

orchestrator.tests.settings is a description

It defines classes_and_methods

###################################################################################################
"""


import os

# TODO: change for a environment variable
CURRENT_DIR = os.path.dirname(__file__)
if "MAJA_DATA_DIR" not in os.environ:
    LOGGER.debug("MAJA_DATA_DIR must be set")
    exit(1)


if "MAJA_GIPPs_DIR" not in os.environ:
    LOGGER.debug("MAJA_GIPPs_DIR must be set")
    exit(1)


TEST_DATA_DIRECTORY = os.environ.get("MAJA_DATA_DIR")
MAJA_GIPPs_DIRECTORY = os.environ.get("MAJA_GIPPs_DIR")
PROJECT_DIRECTORY = os.path.abspath(os.path.join(CURRENT_DIR, os.pardir, os.pardir))

Input_Dir = os.path.join(TEST_DATA_DIRECTORY, "Input")


TV_SENTINEL2_INPUT_DIR = os.path.join(TEST_DATA_DIRECTORY, "SENTINEL2", "Input", "Images")
TV_SENTINEL2_L1_INPUT_DIR = os.path.join(TV_SENTINEL2_INPUT_DIR, "L1")
TV_SENTINEL2_L2_INPUT_DIR = os.path.join(TV_SENTINEL2_INPUT_DIR, "L2")
TV_SENTINEL2_DTM_INPUT_DIR = os.path.join(TEST_DATA_DIRECTORY, "SENTINEL2", "Input", "Images", "DTM")

EARTH_EXPLORER_INPUT_DIR = os.path.join(TEST_DATA_DIRECTORY, "Earth_Explorer")

# SENTINEL2_MUSCATE product
TV_SENTINEL2_MUSCATE_INPUT_DIR = os.path.join(TEST_DATA_DIRECTORY, "SENTINEL2_MUSCATE", "Input", "Images")
TV_SENTINEL2_MUSCATE_BASELINE_DIR = os.path.join(TEST_DATA_DIRECTORY, "SENTINEL2_MUSCATE", "Baseline")
TV_SENTINEL2_MUSCATE_L1_INPUT_DIR = os.path.join(TV_SENTINEL2_MUSCATE_INPUT_DIR, "L1")
TV_SENTINEL2_MUSCATE_L2_INPUT_DIR = os.path.join(TV_SENTINEL2_MUSCATE_INPUT_DIR, "L2")
TV_SENTINEL2_MUSCATE_DTM_INPUT_DIR = os.path.join(TEST_DATA_DIRECTORY, "SENTINEL2_MUSCATE", "Input", "Images", "DTM")


A_PRODUCT_S2_MUSCATE_L1 = {
    "directory": os.path.join(
        TV_SENTINEL2_MUSCATE_L1_INPUT_DIR,
        "SENTINEL2B_20171008-105012-463_L1C_T31TCH_C_V1-0"),
    "metadata_file": "SENTINEL2B_20171008-105012-463_L1C_T31TCH_C_V1-0_MTD_ALL.xml",
    "dtm": os.path.join(
        TV_SENTINEL2_MUSCATE_DTM_INPUT_DIR,
        "S2__TEST_AUX_REFDE2_T31TCH_0001.HDR"),
    "gipp_l2_comm_filename": os.path.join(
        MAJA_GIPPs_DIRECTORY,
        "S2B_TEST_GIP_L2COMM_L_ALLSITES_10008_20150703_21000101.EEF"),
    "gipp_l2_site_filename": os.path.join(
        MAJA_GIPPs_DIRECTORY,
        "S2__TEST_GIP_L2SITE_S_31TJF____10001_00000000_99999999.EEF")}

A_PRODUCT_S2_MUSCATE_L1["main_xml_fullpath"] = os.path.join(A_PRODUCT_S2_MUSCATE_L1.get("directory", None),
                                                            A_PRODUCT_S2_MUSCATE_L1.get("metadata_file", None))


A_BASELINE_S2_MUSCATE_L1 = {"directory": os.path.join(TV_SENTINEL2_MUSCATE_BASELINE_DIR, "L1Reader")}

A_BASELINE_S2_MUSCATE_L1["SOL1Image"] = os.path.join(A_BASELINE_S2_MUSCATE_L1.get('directory'), 'SOL1Image.tif')
A_BASELINE_S2_MUSCATE_L1["ShadowVIEImage"] = os.path.join(
    A_BASELINE_S2_MUSCATE_L1.get('directory'), 'ShadowVIEImage.tif')
A_BASELINE_S2_MUSCATE_L1["L2TOAImageList"] = {
    "R1": os.path.join(
        A_BASELINE_S2_MUSCATE_L1.get('directory'),
        'L2TOAImageList_R1.tif'),
    "R2": os.path.join(
        A_BASELINE_S2_MUSCATE_L1.get('directory'),
        'L2TOAImageList_R2.tif')}
A_BASELINE_S2_MUSCATE_L1["L2SATImageList"] = {
    "R1": os.path.join(
        A_BASELINE_S2_MUSCATE_L1.get('directory'),
        'L2SATImageList_R1.tif'),
    "R2": os.path.join(
        A_BASELINE_S2_MUSCATE_L1.get('directory'),
        'L2SATImageList_R2.tif')}
A_BASELINE_S2_MUSCATE_L1["L2PIXImageList"] = {
    "R1": os.path.join(
        A_BASELINE_S2_MUSCATE_L1.get('directory'),
        'L2PIXImageList_R1.tif'),
    "R2": os.path.join(
        A_BASELINE_S2_MUSCATE_L1.get('directory'),
        'L2PIXImageList_R2.tif')}
A_BASELINE_S2_MUSCATE_L1["L2EDGOutputList"] = {
    "R1": os.path.join(
        A_BASELINE_S2_MUSCATE_L1.get('directory'),
        'L2EDGOutputList_R1.tif'),
    "R2": os.path.join(
        A_BASELINE_S2_MUSCATE_L1.get('directory'),
        'L2_MUSCATE.xml_GetL2EDGOutputList_R2.tif')}
A_BASELINE_S2_MUSCATE_L1["TOASubOutput"] = os.path.join(A_BASELINE_S2_MUSCATE_L1.get('directory'), 'IPTOASubOutput.tif')
A_BASELINE_S2_MUSCATE_L1["SATSubOutput"] = os.path.join(A_BASELINE_S2_MUSCATE_L1.get('directory'), 'IPSATSubOutput.tif')
A_BASELINE_S2_MUSCATE_L1["EDGSubOutput"] = os.path.join(A_BASELINE_S2_MUSCATE_L1.get('directory'), 'PEDGSubOutput.tif')
A_BASELINE_S2_MUSCATE_L1["DTMVIEImage"] = os.path.join(A_BASELINE_S2_MUSCATE_L1.get('directory'), 'DTMVIEImage.tif')
A_BASELINE_S2_MUSCATE_L1["CLAImage"] = os.path.join(A_BASELINE_S2_MUSCATE_L1.get('directory'), 'CLAImage.tif')
A_BASELINE_S2_MUSCATE_L1["AngleZoneMaskList"] = {
    "R1": os.path.join(
        A_BASELINE_S2_MUSCATE_L1.get('directory'),
        'AngleZoneMaskList_R1.tif'),
    "R2": os.path.join(
        A_BASELINE_S2_MUSCATE_L1.get('directory'),
        'AngleZoneMaskList_R2.tif')}

#GIPP_SENTINEL2_MUSCATE={"gipp_l2_comm_filename": "S2A_TEST_GIP_L2COMM_L_ALLSITES_50001_00000000_99999999.EEF"}


# LANDSAT8_MUSCATE product
TV_LANDSAT8_MUSCATE_INPUT_DIR = os.path.join(TEST_DATA_DIRECTORY, "LANDSAT8_MUSCATE", "Input", "Images")
TV_LANDSAT8_MUSCATE_BASELINE_DIR = os.path.join(TEST_DATA_DIRECTORY, "LANDSAT8_MUSCATE", "Baseline")
TV_LANDSAT8_MUSCATE_L1_INPUT_DIR = os.path.join(TV_LANDSAT8_MUSCATE_INPUT_DIR, "L1")
TV_LANDSAT8_MUSCATE_L2_INPUT_DIR = os.path.join(TV_LANDSAT8_MUSCATE_INPUT_DIR, "L2")
TV_LANDSAT8_MUSCATE_DTM_INPUT_DIR = os.path.join(TEST_DATA_DIRECTORY, "LANDSAT8_MUSCATE", "Input", "Images", "DTM")


A_PRODUCT_L8_MUSCATE_L1 = {
    "directory": os.path.join(
        TV_LANDSAT8_MUSCATE_L1_INPUT_DIR,
        "LANDSAT8-OLITIRS-XSTHPAN_20170504-103532-111_L1C_T31TCH_C_V1-0"),
    "dtm": os.path.join(
        TV_LANDSAT8_MUSCATE_DTM_INPUT_DIR,
        "L8_TEST_AUX_REFDE2_T31TCH_0001.HDR"),
    "gipp_l2_comm_filename": os.path.join(
        "/home/julie/Projects/Maja3/maja/Plugins/LANDSAT8_MUSCATE/Interfaces/Data/Gipps",
        "L8_TEST_GIP_L2COMM_L_ALLSITES_90001_00000000_99999999.EEF"),
    "gipp_l2_site_filename": os.path.join(
        "/home/julie/Projects/Maja3/maja/Plugins/LANDSAT8_MUSCATE/Interfaces/Data/Gipps",
        "L8_TEST_GIP_L2SITE_S_EU93046300A00B_90001_00000000_99999999.EEF")}


# VENUS_MUSCATE product

TV_VENUS_MUSCATE_INPUT_DIR = os.path.join(TEST_DATA_DIRECTORY, "VENUS_MUSCATE", "Input", "Images")
TV_VENUS_MUSCATE_BASELINE_DIR = os.path.join(TEST_DATA_DIRECTORY, "VENUS_MUSCATE", "Baseline")
TV_VENUS_MUSCATE_L1_INPUT_DIR = os.path.join(TV_VENUS_MUSCATE_INPUT_DIR, "L1")
TV_VENUS_MUSCATE_L2_INPUT_DIR = os.path.join(TV_VENUS_MUSCATE_INPUT_DIR, "L2")
TV_VENUS_MUSCATE_DTM_INPUT_DIR = os.path.join(TEST_DATA_DIRECTORY, "VENUS_MUSCATE", "Input", "Images", "DTM")
TV_VENUS_MUSCATE_METEO_INPUT_DIR = os.path.join(TEST_DATA_DIRECTORY, "VENUS_MUSCATE", "Input", "Images", "METEO")

A_PRODUCT_VENUS_MUSCATE_L1 = {
    "directory": os.path.join(
        TV_VENUS_MUSCATE_L1_INPUT_DIR,
        "VENUS-XS_20120103-105148-000_L1C_CAMARGUE_C_V1-0"),
    "metadata_file": "VENUS-XS_20120103-105148-000_L1C_CAMARGUE_C_V1-0_MTD_ALL.xml",
    "dtm": os.path.join(
        TV_VENUS_MUSCATE_DTM_INPUT_DIR,
        "VE_TEST_AUX_REFDE2_CAMARGUE_0002.HDR"),
    "gipp_l2_comm_filename": os.path.join(
        "/home/julie/maja-ng-data/TV/VENUS_MUSCATE/Gipps/",
        "VE_TEST_GIP_L2COMM_L_ALLSITES_00002_00000000_99999999.EEF"),
    "gipp_l2_site_filename": os.path.join(
        "/home/julie/maja-ng-data/TV/VENUS_MUSCATE/Gipps/",
        "VE_TEST_GIP_L2SITE_S_CAMARGUE_00002_00000000_99999999.EEF"),
    "meteo_filename": os.path.join(
        TV_VENUS_MUSCATE_METEO_INPUT_DIR,
        "VE_TEST_EXO_METDTA_20180101T000001_20181231T230000.HDR")}


# simple images
QB_1_ORTHO_EXTRACT = {
    "directory": os.path.join(TEST_DATA_DIRECTORY, "Input/DTM/SENTINEL2/11SLT/"),
    "image": "11SLT_240m.mnt",
    "extent": "11SLT_240m.sqlite"
}
QB_1_ORTHO_EXTRACT["image_fullpath"] = os.path.join(QB_1_ORTHO_EXTRACT.get("directory", None),
                                                    QB_1_ORTHO_EXTRACT.get("image", None))

QB_1_ORTHO_EXTRACT["extent_fullpath"] = os.path.join(QB_1_ORTHO_EXTRACT.get("directory", None),
                                                     QB_1_ORTHO_EXTRACT.get("extent", None))


# baseline xml files
INTERNAL_XML_FILES = {
    "directory": Input_Dir,
    "angles": "gipangles.xml"
}
INTERNAL_XML_FILES["angles_fullpath"] = os.path.join(INTERNAL_XML_FILES.get("directory", None),
                                                     INTERNAL_XML_FILES.get("angles", None))

TV_S2_DIR_L1_NAME = "S2A_MSIL1C_20160914T074612_N0204_R135_T36JTT_20160914T081456.SAFE"
A_PRODUCT_S2_L1 = {
    "directory": os.path.join(TV_SENTINEL2_L1_INPUT_DIR, TV_S2_DIR_L1_NAME),
    "metadata_file": "MTD_MSIL1C.xml",
    "granule_id": "S2A_OPER_MSI_L1C_TL_MPS__20160914T113643_A006424_T36JTT_N02.04",
    "granule_dir": "L1C_T36JTT_A006424_20160914T081456",
    "granule_xml_basename": "MTD_TL.xml",
    "granule_solar_angles": {"sun_zenith_angle": '36.5195976313918',
                             "sun_azimuth_angle": '43.128645235562'},
    "granule_incidence_angle_zenith_band_1": '8.80401232206388',
    "granule_incidence_angle_azimuth_band_7": '286.065038468837',
    "mask_defect_12": "GRANULE/L1C_T36JTT_A006424_20160914T081456/QI_DATA/MSK_DEFECT_B12.gml",
    "mask_detfoo_0": "GRANULE/L1C_T36JTT_A006424_20160914T081456/QI_DATA/MSK_DETFOO_B01.gml",

}


A_PRODUCT_S2_L1["granule_dir_fullpath"] = os.path.join(A_PRODUCT_S2_L1.get("directory", None),
                                                       "GRANULE",
                                                       A_PRODUCT_S2_L1.get("granule_dir", None))
A_PRODUCT_S2_L1["granule_xml_fullpath"] = os.path.join(A_PRODUCT_S2_L1.get("granule_dir_fullpath", None),
                                                       A_PRODUCT_S2_L1.get("granule_xml_basename", None))
A_PRODUCT_S2_L1["main_xml_fullpath"] = os.path.join(A_PRODUCT_S2_L1.get("directory", None),
                                                    A_PRODUCT_S2_L1.get("metadata_file", None))

A_PRODUCT_S2_L1["mask_defect_12_fullpath"] = os.path.join(A_PRODUCT_S2_L1.get("directory", None),
                                                          A_PRODUCT_S2_L1.get("mask_defect_12", None))
A_PRODUCT_S2_L1["mask_detfoo_0_fullpath"] = os.path.join(A_PRODUCT_S2_L1.get("directory", None),
                                                         A_PRODUCT_S2_L1.get("mask_detfoo_0", None))

# Sentinel 2 L2 product
TV_S2_DIR_L2_NAME = "S2A_OPER_SSC_L2VALD_36JTT____20160914.DBL.DIR"
A_PRODUCT_S2_L2 = {
    "directory": os.path.join(TV_SENTINEL2_L2_INPUT_DIR, TV_S2_DIR_L2_NAME),
    "metadata_file": "S2A_OPER_SSC_L2VALD_36JTT____20160914.HDR",
    "product_solar_angles": {"sun_zenith_angle": '36.5195976313918',
                             "sun_azimuth_angle": '43.128645235562'},
    "product_incidence_angle_zenith_band_1": '8.80401232206388',
    "product_incidence_angle_azimuth_band_7": '286.065038468837',
    "mask_r1": "S2A_OPER_SSC_L2VALD_36JTT____20160914.DBL.DIR/S2A_OPER_SSC_PDTANX_L2VALD_36JTT____20160914_MSK_R1.HDR",
    "mask_r2": "S2A_OPER_SSC_L2VALD_36JTT____20160914.DBL.DIR/S2A_OPER_SSC_PDTANX_L2VALD_36JTT____20160914_MSK_R2.HDR",
}
A_PRODUCT_S2_L2["main_xml_fullpath"] = os.path.join(TV_SENTINEL2_L2_INPUT_DIR,
                                                    A_PRODUCT_S2_L2.get("metadata_file", None))
A_PRODUCT_S2_L2["mask_r1_fullpath"] = os.path.join(A_PRODUCT_S2_L2.get("directory", None),
                                                   A_PRODUCT_S2_L2.get("mask_r1", None))
A_PRODUCT_S2_L2["mask_r2_fullpath"] = os.path.join(A_PRODUCT_S2_L2.get("directory", None),
                                                   A_PRODUCT_S2_L2.get("mask_r2", None))
# /Sentinel 2 L2 product


S2_DIR_NAME = "S2A_MSIL1C_20170315T101021_N0204_R022_T32TNN_20170315T101214.SAFE"
B_PRODUCT_S2 = {
    "directory": os.path.join(TEST_DATA_DIRECTORY, S2_DIR_NAME),
    "metadata_file": "S2A_MSIL1C_20170315T101021_N0204_R022_T32TNN_20170315T101214.xml",
    "granule_id": "S2A_OPER_MSI_L1C_TL_SGS__20170315T153342_A009028_T32TNN_N02.04",
    "granule_dir": "L1C_T32TNN_A009028_20170315T101214",
    "granule_xml_basename": "MTD_TL.xml",
    "granule_solar_angles": {"sun_zenith_angle": '47.6251269822141',
                             "sun_azimuth_angle": '155.536033206907'},
    "granule_incidence_angle_zenith_band_1": '7.01884421586153',
    "granule_incidence_angle_azimuth_band_7": '104.989560500636',
}

GIPP_L2 = {
    "directory": MAJA_GIPPs_DIRECTORY,
    "gipp_l2_comm_filename": "S2A_TEST_GIP_L2COMM_L_ALLSITES_50001_00000000_99999999.EEF"

}

GIPP_L2["gipp_l2_comm_fullpath"] = os.path.join(GIPP_L2.get("directory", None),
                                                GIPP_L2.get("gipp_l2_comm_filename", None))

EARTH_EXPLORER_SAMPLE = {"directory": EARTH_EXPLORER_INPUT_DIR,
                         "s2_filename": "GLOBAL.HDR"}
EARTH_EXPLORER_SAMPLE["s2_full_filename"] = os.path.join(EARTH_EXPLORER_SAMPLE.get("directory"),
                                                         EARTH_EXPLORER_SAMPLE.get("s2_filename"))
