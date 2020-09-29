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
#
#                        o     o
#                        oo   oo   oo        o   oo        ,-.
#                        o o o o  o  o       o  o  o       \_/
#                        o  o  o o    o      o o    o     |||)<
#                        o     o oooooo      o oooooo      / \
#                        o     o o    o o    o o    o      `-^
#                        o     o o    o  oooo  o    o
#
###################################################################################################

orchestrator.plugins.maja_dataset -- shortdesc

orchestrator.plugins.maja_dataset is a description

It defines classes_and_methods

###################################################################################################
"""
import glob
import os
import re
from collections import OrderedDict
import lxml.etree as ET
from orchestrator.cots.gdal.gdal_dataset_info import GdalDatasetInfo
from orchestrator.plugins.common.earth_explorer.maja_earth_explorer_l2_image_file_writer import \
    EarthExplorerL2ImageFileWriter
from orchestrator.common.logger.maja_logging import configure_logger
LOGGER = configure_logger(__name__)

WAVELENGTH = OrderedDict([("B1", 443),
                          ("B2", 490),
                          ("B3", 560),
                          ("B4", 665),
                          ("B5", 705),
                          ("B6", 740),
                          ("B7", 783),
                          ("B8", 842),
                          ("B8A", 865),
                          ("B9", 945),
                          ("B10", 1375),
                          ("B11", 1610),
                          ("B12", 2190)])

RESOLUTION = OrderedDict([("B1", 60),
                          ("B2", 10),
                          ("B3", 10),
                          ("B4", 10),
                          ("B5", 20),
                          ("B6", 20),
                          ("B7", 20),
                          ("B8", 10),
                          ("B8A", 20),
                          ("B9", 60),
                          ("B10", 60),
                          ("B11", 20),
                          ("B12", 20)])

BAND_ID = [str(x) for x in range(13)]
DETECTOR_ID = [str(x) for x in range(12)]


class MajaSentinel2L2ImageWriter(EarthExplorerL2ImageFileWriter):

    def __init__(self):
        super(MajaSentinel2L2ImageWriter, self).__init__()

    def can_write(self, plugin_name):
        return (plugin_name == "SENTINEL2")


def get_root():
    # <Earth_Explorer_Header xmlns="http://eop-cfi.esa.int/CFI"
    # xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" schema_version="1.00"
    # xsi:schemaLocation="http://eop-cfi.esa.int/CFI ./SSC_PDTQLK_QuickLook.xsd"
    # xsi:type="PDTQLK_Header_Type">
    xmlns = "http://eop-cfi.esa.int/CFI"
    xsi = "http://www.w3.org/2001/XMLSchema-instance"
    schemaLocation = "http://eop-cfi.esa.int/CFI ./SSC_PDTQLK_QuickLook.xsd"
    typeXsi = "PDTQLK_Header_Type"
    root = ET.Element("Earth_Explorer_Header", attrib={"schema_version": "1.00",
                                                       "{" + xsi + "}schemaLocation": schemaLocation,
                                                       "{" + xsi + "}type": typeXsi},
                      nsmap={'xsi': xsi, None: xmlns})
    return root


def extract_site_id_from_folder_name(dir_name):
    """
    Return the tile id from the folder name
    :param dir_name:
    :return:
    """
    try:
        return re.search(r"S2A_OPER_SSC_L2VALD_(.+?)_\d{8}\.DBL\.DIR", os.path.basename(dir_name)).group(1)
    except AttributeError:
        LOGGER.error(r"No pattern 'S2A_OPER_SSC_L2VALD_(.+?___)_\d{8}\.DBL\.DIR' found in %s", dir_name)


def extract_acquisition_date_from_folder_name(dir_name):
    """
    Return the tile id from the folder name
    :param dir_name:
    :return:
    """
    try:
        return re.search(r"S2A_OPER_SSC_L2VALD_.+?_(\d{8})\.DBL\.DIR", os.path.basename(dir_name)).group(1)
    except AttributeError:
        LOGGER.error(r"No pattern 'S2A_OPER_SSC_L2VALD_.+?____(\d{8})\.DBL\.DIR' found in %s", dir_name)


def get_fixed_header(root, basename_out, mission, validity_time, version, source):
    a1 = ET.SubElement(root, "Fixed_Header")
    a1_sub = ET.SubElement(a1, "File_Name")
    a1_sub.text = basename_out
    a1_sub = ET.SubElement(a1, "File_Description")
    a1_sub.text = "L2PublishedProduct"
    a1_sub = ET.SubElement(a1, "Notes")
    a1_sub.text = "L2 note"
    a1_sub = ET.SubElement(a1, "Mission")
    a1_sub.text = mission
    a1_sub = ET.SubElement(a1, "File_Class")
    a1_sub.text = "OPER"
    a1_sub = ET.SubElement(a1, "File_Type")
    a1_sub.text = "SSC_L2VALD"
    b1 = ET.SubElement(a1, "Validity_Period")
    b1_sub = ET.SubElement(b1, "Validity_Start")
    # toto.text = "UTC=2006-07-01T18:11:45"
    b1_sub.text = "UTC={}".format(validity_time[0])
    b1_sub = ET.SubElement(b1, "Validity_Stop")
    # toto.text = "UTC=9999-99-99T99:99:99"
    b1_sub.text = "UTC={}".format(validity_time[1])
    b1_sub = ET.SubElement(a1, "File_Version")
    # toto.text = "0001"
    b1_sub.text = version
    b2 = ET.SubElement(a1, "Source")
    b2_sub = ET.SubElement(b2, "System")
    b2_sub.text = source.get("system")
    b2_sub = ET.SubElement(b2, "Creator")
    b2_sub.text = source.get("creator")
    b2_sub = ET.SubElement(b2, "Creator_Version")
    b2_sub.text = source.get("creator_version")
    b2_sub = ET.SubElement(b2, "Creation_Date")
    b2_sub.text = source.get("creation_date")


def get_product_information(root, gipp_files, quality_indexes, mean_sun_angles, solar_angles_zenith,
                            solar_angles_azimuth, mean_viewing_angles, list_of_viewing_angles):
    a1 = ET.SubElement(root, "Product_Information")
    a1_sub = ET.SubElement(a1, "Product_Level")
    a1_sub.text = "L2"
    a1_sub = ET.SubElement(a1, "Acquisition_Date_Time")
    a1_sub.text = ""
    a1_sub = ET.SubElement(a1, "Acquisition_Orbit_Number")
    a1_sub.text = ""
    b1 = ET.SubElement(a1, "Processing_Information")
    b1_sub = ET.SubElement(b1, "Date_Time")
    b1_sub.text = ""
    c1 = ET.SubElement(b1, "List_of_GIPP_Files", count=str(len(gipp_files)))
    for index_gipp, dic_gipp in enumerate(gipp_files):
        c1_sub = ET.SubElement(c1, "GIPP_File", sn=str(index_gipp + 1))
        c1_sub_sub = ET.SubElement(c1_sub, "Nature")
        c1_sub_sub.text = dic_gipp["nature"]
        c1_sub_sub = ET.SubElement(c1_sub, "File_Location")
        c1_sub_sub.text = dic_gipp["file_location"]
        c1_sub_sub = ET.SubElement(c1_sub, "Logical_Name")
        c1_sub_sub.text = dic_gipp["logical_name"]
    b1 = ET.SubElement(a1, "Web_Site_URL")
    b1 = ET.SubElement(a1, "List_of_Quality_Indexes", count=str(len(quality_indexes)))
    for index_qi, dic_qi in enumerate(quality_indexes):
        b1_sub = ET.SubElement(b1, "Quality_Index", sn=str(index_qi + 1))
        b1_sub_sub = ET.SubElement(b1_sub, "Code")
        b1_sub_sub.text = dic_qi["code"]
        b1_sub_sub = ET.SubElement(b1_sub, "Value")
        b1_sub_sub.text = dic_qi["value"]
    b1 = ET.SubElement(a1, "List_of_Band_Central_Wavelength", count=str(len(WAVELENGTH)))
    for key, value in WAVELENGTH.items():
        b1_sub = ET.SubElement(b1, "Band_Central_Wavelength", sk=key, unit="nanometer")
        b1_sub.text = str(value)
    b1 = ET.SubElement(a1, "List_of_Band_Resolution", count=str(len(RESOLUTION)))
    for key, value in RESOLUTION.items():
        b1_sub = ET.SubElement(b1, "Band_Central_Resolution", sk=key, unit="meter")
        b1_sub.text = str(value)
    b1 = ET.SubElement(a1, "Mean_Sun_Angle")
    b1_sub = ET.SubElement(b1, "ZENITH_ANGLE", unit="deg")
    b1_sub.text = str(mean_sun_angles["zenith"])
    b1_sub = ET.SubElement(b1, "AZIMUTH_ANGLE", unit="deg")
    b1_sub.text = str(mean_sun_angles["azimuth"])
    b1 = ET.SubElement(a1, "Solar_Angles")
    b1_sub = ET.SubElement(b1, "Zenith")
    b1_sub_sub = ET.SubElement(b1_sub, "COL_STEP", unit="m")
    b1_sub_sub.text = str(solar_angles_zenith[0])
    b1_sub_sub = ET.SubElement(b1_sub, "ROW_STEP", unit="m")
    b1_sub_sub.text = str(solar_angles_zenith[1])
    b1_sub_sub = ET.SubElement(b1_sub, "Values_List")
    for el in solar_angles_zenith[2]:
        b1_sub_sub_sub = ET.SubElement(b1_sub_sub, "VALUES")
        b1_sub_sub_sub.text = str(el)
    b1_sub = ET.SubElement(b1, "Azimuth")
    b1_sub_sub = ET.SubElement(b1_sub, "COL_STEP", unit="m")
    b1_sub_sub.text = str(solar_angles_azimuth[0])
    b1_sub_sub = ET.SubElement(b1_sub, "ROW_STEP", unit="m")
    b1_sub_sub.text = str(solar_angles_azimuth[1])
    b1_sub_sub = ET.SubElement(b1_sub, "Values_List")
    for el in solar_angles_azimuth[2]:
        b1_sub_sub_sub = ET.SubElement(b1_sub_sub, "VALUES")
        b1_sub_sub_sub.text = str(el)
    b1 = ET.SubElement(a1, "Mean_Viewing_Incidence_Angle_List")
    for key, value in mean_viewing_angles.items():
        b1_sub = ET.SubElement(b1, "Mean_Viewing_Incidence_Angle", bandId=str(key))
        b1_sub_sub = ET.SubElement(b1_sub, "ZENITH_ANGLE", unit="deg")
        b1_sub_sub.text = str(value["zenith"])
        b1_sub_sub = ET.SubElement(b1_sub, "AZIMUTH_ANGLE", unit="deg")
        b1_sub_sub.text = str(value["azimuth"])
    b1 = ET.SubElement(a1, "List_of_Viewing_Angles")
    for key, value in list_of_viewing_angles.items():
        b1_sub = ET.SubElement(b1, "Viewing_Incidence_Angles_Grids", bandId=str(key[0]), detectorId=str(key[1]))
        b1_sub_sub = ET.SubElement(b1_sub, "Zenith")
        zenith_values = value["zenith"]
        b1_sub_sub_sub = ET.SubElement(b1_sub_sub, "COL_STEP", unit="m")
        b1_sub_sub_sub.text = str(zenith_values[0])
        b1_sub_sub_sub = ET.SubElement(b1_sub_sub, "ROW_STEP", unit="m")
        b1_sub_sub_sub.text = str(zenith_values[1])
        b1_sub_sub_sub = ET.SubElement(b1_sub, "Values_List")
        for el in zenith_values[2]:
            b1_sub_sub_sub_sub = ET.SubElement(b1_sub_sub_sub, "VALUES")
            b1_sub_sub_sub_sub.text = str(el)

        b1_sub_sub = ET.SubElement(b1_sub, "Azimuth")
        azimuth_values = value["azimuth"]
        b1_sub_sub_sub = ET.SubElement(b1_sub_sub, "COL_STEP", unit="m")
        b1_sub_sub_sub.text = str(azimuth_values[0])
        b1_sub_sub_sub = ET.SubElement(b1_sub_sub, "ROW_STEP", unit="m")
        b1_sub_sub_sub.text = str(azimuth_values[1])
        b1_sub_sub_sub = ET.SubElement(b1_sub, "Values_List")
        for el in azimuth_values[2]:
            b1_sub_sub_sub_sub = ET.SubElement(b1_sub_sub_sub, "VALUES")
            b1_sub_sub_sub_sub.text = str(el)

    a1_sub = ET.SubElement(a1, "Reference_SiteDefinition_Id")
    a1_sub.text = "UNKNOWN"
    a1_sub = ET.SubElement(a1, "Cloud_Percentage")
    a1_sub.text = "0"
    a1_sub = ET.SubElement(a1, "Snow_Percentage")
    a1_sub.text = "0"
    b1 = ET.SubElement(a1, "Used_DEM")
    b1_sub = ET.SubElement(b1, "DEM_Reference")
    b1_sub.text = ""
    b1_sub = ET.SubElement(b1, "Statistics")
    b1_sub_sub = ET.SubElement(b1_sub, "Min", unit="m")
    b1_sub_sub.text = ""
    b1_sub_sub = ET.SubElement(b1_sub, "Max", unit="m")
    b1_sub_sub.text = ""
    b1_sub_sub = ET.SubElement(b1_sub, "Average", unit="m")
    b1_sub_sub.text = ""
    b1_sub_sub = ET.SubElement(b1_sub, "Standard_Deviation", unit="m")
    b1_sub_sub.text = ""
    a1_sub = ET.SubElement(a1, "Reflectance_Quantification_Value")
    a1_sub.text = "0.0001"
    b1 = ET.SubElement(a1, "Processing_Flags_And_Modes")
    b1_sub = ET.SubElement(b1, "Processing_Mode")
    b1_sub.text = "INIT_MODE"
    b1_sub = ET.SubElement(b1, "Adjacency_Effects_And_Slope_Correction")
    b1_sub.text = "false"
    b1 = ET.SubElement(a1, "List_of_Used_Product_Ids", count="1")
    b1_sub = ET.SubElement(b1, "Used_Product_Id", sn="1")
    b1_sub.text = "MTD_MSIL1C"


def get_image_information(root, res):
    """
                <Number_of_Significant_Bits>16</Number_of_Significant_Bits>
                <Nodata_Value>-10000</Nodata_Value>
                <List_of_Resolutions count="2">
                    <Resolution r="10">
                        <Size>
                            <Lines>10980</Lines>
                            <Columns>10980</Columns>
                            <Bands>4</Bands>
                        </Size>
                        <Geoposition>
                            <ULX>199980</ULX>
                            <ULY>7300000</ULY>
                            <XDIM>10</XDIM>
                            <YDIM>-10</YDIM>
                        </Geoposition>
                        <Product_Sampling>
                            <By_Line unit="m">10</By_Line>
                            <By_Column unit="m">10</By_Column>
                        </Product_Sampling>
                        <List_of_Bands count="4">
                            <Band sn="1">B2</Band>
                            <Band sn="2">B3</Band>
                            <Band sn="3">B4</Band>
                            <Band sn="4">B8</Band>
                        </List_of_Bands>
                    </Resolution>
                    <Resolution r="20">
                        <Size>
                            <Lines>5490</Lines>
                            <Columns>5490</Columns>
                            <Bands>6</Bands>
                        </Size>
                        <Geoposition>
                            <ULX>199980</ULX>
                            <ULY>7300000</ULY>
                            <XDIM>20</XDIM>
                            <YDIM>-20</YDIM>
                        </Geoposition>
                        <Product_Sampling>
                            <By_Line unit="m">20</By_Line>
                            <By_Column unit="m">20</By_Column>
                        </Product_Sampling>
                        <List_of_Bands count="6">
                            <Band sn="1">B5</Band>
                            <Band sn="2">B6</Band>
                            <Band sn="3">B7</Band>
                            <Band sn="4">B8A</Band>
                            <Band sn="5">B11</Band>
                            <Band sn="6">B12</Band>
                        </List_of_Bands>
                    </Resolution>
                </List_of_Resolutions>
                <Image_Compacting_Tool>NO</Image_Compacting_Tool>
            </Image_Information>
    :return:
    """

    a1 = ET.SubElement(root, "Image_Information")
    a1_sub = ET.SubElement(a1, "Format")
    a1_sub.text = "GEOTIFF"
    a1_sub = ET.SubElement(a1, "Binary_Encoding")
    a1_sub.text = "LITTLE_ENDIAN"
    a1_sub = ET.SubElement(a1, "Data_Type")
    a1_sub.text = "SIGNED_SHORT"
    a1_sub = ET.SubElement(a1, "Number_of_Significant_Bits")
    a1_sub.text = "16"
    a1_sub = ET.SubElement(a1, "Nodata_Value")
    a1_sub.text = "-10000"
    a1_sub = ET.SubElement(a1, "List_of_Resolutions", count="2")
    for resolution in res:
        b1 = ET.SubElement(a1_sub, "Resolution", r=str(resolution["resolution"]))
        b1_sub = ET.SubElement(b1, "Size")
        b1_sub_sub = ET.SubElement(b1_sub, "Lines")
        b1_sub_sub.text = str(resolution["info"].size[0])
        b1_sub_sub = ET.SubElement(b1_sub, "Columns")
        b1_sub_sub.text = str(resolution["info"].size[1])
        b1_sub_sub = ET.SubElement(b1_sub, "Bands")
        b1_sub_sub.text = str(resolution["info"].number_of_bands)
        b1_sub = ET.SubElement(b1, "Geoposition")
        b1_sub_sub = ET.SubElement(b1_sub, "ULX")
        b1_sub_sub.text = str(resolution["info"].origin[0])
        b1_sub_sub = ET.SubElement(b1_sub, "ULY")
        b1_sub_sub.text = str(resolution["info"].origin[1])
        b1_sub_sub = ET.SubElement(b1_sub, "XDIM")
        b1_sub_sub.text = str(resolution["info"].pixel_size[0])
        b1_sub_sub = ET.SubElement(b1_sub, "YDIM")
        b1_sub_sub.text = str(resolution["info"].pixel_size[1])
        b1_sub = ET.SubElement(b1, "Product_Sampling")
        b1_sub_sub = ET.SubElement(b1_sub, "By_Line")
        b1_sub_sub.text = str(resolution["info"].pixel_size[0])
        b1_sub_sub = ET.SubElement(b1_sub, "By_Column")
        b1_sub_sub.text = str(resolution["info"].pixel_size[1])
        b1_sub = ET.SubElement(b1, "List_of_Bands", count=str(len(resolution["list_of_bands"])))
        for index_band_name, band_name in enumerate(resolution["list_of_bands"]):
            b1_sub_sub = ET.SubElement(b1_sub, "Band", sn=str(index_band_name + 1))
            b1_sub_sub.text = band_name

    a1_sub = ET.SubElement(a1, "Image_Compacting_Tool")
    a1_sub.text = "NO"


def get_geo_referencing_information(root, info):
    """
    <Geo_Referencing_Information>
        <Product_Coverage>
            <Geographic>
                <Coordinate_Reference_System>
                    <Code>EPSG:32736</Code>
                    <Short_Description>WGS84 / UTM zone 36S</Short_Description>
                </Coordinate_Reference_System>
            </Geographic>
        </Product_Coverage>
    </Geo_Referencing_Information>
    :return:
    """

    a1 = ET.SubElement(root, "Geo_Referencing_Information")
    a1_sub = ET.SubElement(a1, "Product_Coverage")
    a1_sub_sub = ET.SubElement(a1_sub, "Geographic")
    a1_sub_sub_sub = ET.SubElement(a1_sub_sub, "Coordinate_Reference_System")
    a1_sub = ET.SubElement(a1, "Code")
    a1_sub.text = "EPSG:{}".format(info.espg_code_number)
    a1_sub = ET.SubElement(a1, "Short_Description")
    a1_sub.text = info.projection_name


def get_product_organization(root, files, input_dir):
    """
    :return:
    """
    a1 = ET.SubElement(root, "Product_Organization")
    a1_sub = ET.SubElement(a1, "Image_File")
    image = files.get("image")
    a1_sub_sub = ET.SubElement(a1_sub, "Nature")
    a1_sub_sub.text = image[0]
    a1_sub_sub = ET.SubElement(a1_sub, "File_Location")
    a1_sub_sub.text = os.path.join(".", os.path.basename(os.path.dirname(input_dir)), os.path.basename(image[1]))
    a1_sub_sub = ET.SubElement(a1_sub, "Logical_Name")
    a1_sub_sub.text = os.path.basename(image[1])
    a1_sub = ET.SubElement(a1, "Quicklook_File")
    ql = files.get("quicklook")
    a1_sub_sub = ET.SubElement(a1_sub, "Nature")
    a1_sub_sub.text = ql[0]
    a1_sub_sub = ET.SubElement(a1_sub, "File_Location")
    a1_sub_sub.text = os.path.join(".", os.path.basename(os.path.dirname(input_dir)), os.path.basename(ql[1]))
    a1_sub_sub = ET.SubElement(a1_sub, "Logical_Name")
    a1_sub_sub.text = os.path.basename(ql[1])
    annex_files = files.get("annex_files")
    a1_sub = ET.SubElement(a1, "List_of_Annex_Files", count=str(len(annex_files)))
    for annex, value in annex_files.items():
        a1_sub_sub = ET.SubElement(a1_sub, "Annex_File", sk=annex[:-3])
        a1_sub_sub_sub = ET.SubElement(a1_sub_sub, "Nature")
        a1_sub_sub_sub.text = value[0]
        a1_sub_sub_sub = ET.SubElement(a1_sub_sub, "File_Location")
        a1_sub_sub_sub.text = os.path.join(
            ".", os.path.basename(
                os.path.dirname(input_dir)), os.path.basename(
                value[1]))
        a1_sub_sub_sub = ET.SubElement(a1_sub_sub, "Logical_Name")
        a1_sub_sub_sub.text = os.path.basename(ql[1])
    a1_sub = ET.SubElement(a1, "Private_Data_File")
    private = files.get("private")
    if private is not None:
        a1_sub_sub = ET.SubElement(a1_sub, "Nature")
        a1_sub_sub.text = private[0]
        a1_sub_sub = ET.SubElement(a1_sub, "File_Location")
        a1_sub_sub.text = os.path.join(".", "PRIVATE", os.path.basename(private[1]))
        a1_sub_sub = ET.SubElement(a1_sub, "Logical_Name")
        a1_sub_sub.text = os.path.basename(private[1])


def search_first_file_with_pattern(input_dir, pattern):
    r = search_files_with_pattern(input_dir, pattern)
    if r:
        return os.path.join(input_dir, r[0])
    else:
        LOGGER.error("No match found for %s in %s", pattern, input_dir)


def search_files_with_pattern(input_dir, pattern):
    return [x for x in os.listdir(input_dir) if re.match(pattern, x) is not None]


def search_only_one_file_from_pattern(input_dir, pattern):
    r = search_files_with_pattern(input_dir, pattern)
    # print pattern, r
    # remove aux.xml files matching the pattern
    r = [x for x in r if not x.endswith(".aux.xml")]
    if len(r) == 1:
        return os.path.join(input_dir, r[0])
    elif len(r) == 0:
        LOGGER.error("No match found for %s in %s", pattern, input_dir)
    else:
        LOGGER.error("Error: several files match the given pattern %s", r)


def get_files(input_dir):
    files = {}
    files["image"] = ("SSC_PDTIMG", search_only_one_file_from_pattern(input_dir, "S2.+?_SSC_PDTIMG_.+?SRE_R2.HDR"))
    files["quicklook"] = ("SSC_PDTQLK", search_only_one_file_from_pattern(input_dir, "S2.+?_SSC_PDTQLK_.+?.JPG"))

    files["private"] = ("TEC_L2VALD", search_only_one_file_from_pattern(os.path.join(input_dir, "PRIVATE"),
                                                                        "S2.+?_TEC_L2VALD_.+?.EEF"))

    # remove None values from an incomplete product
    files_to_return = {}
    for key, value in files.items():
        if not value[1] is None:
            files_to_return[key] = value

    annex_files = {}
    annex_files["MSK_r1"] = (
        "SSC_PDTANX", search_only_one_file_from_pattern(
            input_dir, "S2.+?_SSC_PDTANX_.+?_MSK_R1.HDR"))
    annex_files["MSK_r2"] = (
        "SSC_PDTANX", search_only_one_file_from_pattern(
            input_dir, "S2.+?_SSC_PDTANX_.+?_MSK_R2.HDR"))
    annex_files["QLT_r1"] = (
        "SSC_PDTANX", search_only_one_file_from_pattern(
            input_dir, "S2.+?_SSC_PDTANX_.+?_QLT_R1.HDR"))
    annex_files["QLT_r2"] = (
        "SSC_PDTANX", search_only_one_file_from_pattern(
            input_dir, "S2.+?_SSC_PDTANX_.+?_QLT_R2.HDR"))
    annex_files["ATB_r1"] = (
        "SSC_PDTANX", search_only_one_file_from_pattern(
            input_dir, "S2.+?_SSC_PDTANX_.+?_ATB_R1.HDR"))
    annex_files["ATB_r2"] = (
        "SSC_PDTANX", search_only_one_file_from_pattern(
            input_dir, "S2.+?_SSC_PDTANX_.+?_ATB_R2.HDR"))
    annex_files["CLD_r1"] = (
        "SSC_PDTANX", search_only_one_file_from_pattern(
            input_dir, "S2.+?_SSC_PDTANX_.+?_CLD_R1.HDR"))
    annex_files["CLD_r2"] = (
        "SSC_PDTANX", search_only_one_file_from_pattern(
            input_dir, "S2.+?_SSC_PDTANX_.+?_CLD_R2.HDR"))
    # remove None values from an incomplete product
    annex_files_to_return = {}
    for key, value in annex_files.items():
        LOGGER.debug("key, vlue", key, value)
        if not value[1] is None:
            annex_files_to_return[key] = value
    LOGGER.debug("###", annex_files_to_return)
    files_to_return["annex_files"] = annex_files_to_return

    LOGGER.debug("files to return %s", files_to_return)

    return files_to_return


def hdr_creation(input_dir, output_filename, writer):
    LOGGER.debug("global_hdr_nodes, hdr_creation input_dir %s", input_dir)
    LOGGER.debug("global_hdr_nodes, hdr_creation output_filename %s", output_filename)

    if not os.path.isdir(input_dir):
        LOGGER.error("Input directory does not exist ! %s", input_dir)
        return
    if os.listdir(input_dir) == []:
        LOGGER.error("Input directory is empty ! %s", input_dir)
        return

    image_r1 = search_first_file_with_pattern(input_dir, r'^.+?_R1.DBL.TIF$')
    image_r2 = search_first_file_with_pattern(input_dir, r'^.+?_R2.DBL.TIF$')

    id_tile = extract_site_id_from_folder_name(input_dir)
    acquisition_date = extract_acquisition_date_from_folder_name(input_dir)

    if glob.glob(os.path.join(input_dir, "*.TIF")) == []:
        LOGGER.warning("No TIF files found in %s", input_dir)
        return
    a_image = glob.glob(os.path.join(input_dir, "*.TIF"))[0]

    info = GdalDatasetInfo(a_image)
    # basename_out = os.path.basename(os.path.splitext(output_filename)[0])
    # logging.debug(basename_out)
    #
    # xml_file = os.path.join(os.path.dirname(__file__), "xml_template.xml")
    # tree = ET.parse(xml_file)

    # ET.register_namespace("", "http://eop-cfi.esa.int/CFI")
    root = get_root()
    # tree.getroot()

    # root = ET.Element("Earth_Explorer_Header", schema_version = "1.00", xmlns = "http://eop-cfi.esa.int/CFI", xsi = "http://www.w3.org/2001/XMLSchema-instance", type = "REFDE2_Header_Type", schemaLocation = "http://eop-cfi.esa.int/CFI AUX_REFDE2_ReferenceDemDataLevel2.xsd")

    # Double splitext to remove .DBL.DIR
    get_fixed_header(root,
                     basename_out=os.path.basename(os.path.splitext(os.path.splitext(input_dir)[0])[0]),
                     mission="SENTINEL-2A",
                     validity_time=["", ""],
                     version="0003",
                     source={
                         "system": "MAJA",
                         "creator": "MAJA_L2_INIT_CHAIN",
                         "creator_version": "0.0.0",
                         "creation_date": "TBD"
                     })

    a = ET.SubElement(root, "Variable_Header")
    b2 = ET.SubElement(a, "Main_Product_Header")
    ET.SubElement(b2, "List_of_Consumers", count="0")
    ET.SubElement(b2, "List_of_Extensions", count="0")
    b3 = ET.SubElement(a, "Specific_Product_Header")
    b4 = ET.SubElement(b3, "Instance_Id")
    b4_sub = ET.SubElement(b4, "Nick_Name")
    b4_sub.text = id_tile
    b4_sub = ET.SubElement(b4, "Acquisition_Date")
    b4_sub.text = acquisition_date

    gipp_files = [{"nature": "GIP_CKEXTL",
                   "file_location": "NIL=OTHER:EXTERNAL",
                   "logical_name": "S2A_TEST_GIP_CKEXTL_S_35JPK____50001_00000000_99999999"},
                  {"nature": "GIP_CKQLTL",
                   "file_location": "NIL=OTHER:EXTERNAL",
                   "logical_name": "S2A_TEST_GIP_CKQLTL_S_35JPK____50001_00000000_99999999"},
                  {"nature": "GIP_L2ALBD",
                   "file_location": "NIL=OTHER:EXTERNAL",
                   "logical_name": "S2A_TEST_GIP_L2ALBD_L_CONTINEN_50001_00000000_99999999"},
                  {"nature": "GIP_L2COMM",
                   "file_location": "NIL=OTHER:EXTERNAL",
                   "logical_name": "S2A_TEST_GIP_L2COMM_L_ALLSITES_50001_00000000_99999999"},
                  {"nature": "GIP_L2DIFT",
                   "file_location": "NIL=OTHER:EXTERNAL",
                   "logical_name": "S2A_TEST_GIP_L2DIFT_L_CONTINEN_50001_00000000_99999999"},
                  {"nature": "GIP_L2DIRT",
                   "file_location": "NIL=OTHER:EXTERNAL",
                   "logical_name": "S2__TEST_GIP_L2SITE_S_35JPK____50001_00000000_99999999"},
                  {"nature": "GIP_L2SITE",
                   "file_location": "NIL=OTHER:EXTERNAL",
                   "logical_name": "S2__TEST_GIP_L2SITE_S_35JPK____50001_00000000_99999999"},
                  {"nature": "GIP_L2SMAC",
                   "file_location": "NIL=OTHER:EXTERNAL",
                   "logical_name": "S2A_TEST_GIP_L2SMAC_L_ALLSITES_50001_00000000_99999999"},
                  {"nature": "GIP_L2TOCR",
                   "file_location": "NIL=OTHER:EXTERNAL",
                   "logical_name": "S2A_TEST_GIP_L2TOCR_L_CONTINEN_50001_00000000_99999999"},
                  {"nature": "GIP_L2WATV",
                   "file_location": "NIL=OTHER:EXTERNAL",
                   "logical_name": "S2A_TEST_GIP_L2WATV_L_CONTINEN_50001_00000000_99999999"},
                  ]
    qi = [{"code": "RAIN_DETECTED",
           "value": "FALSE"},
          {"code": "HOT_SPOT_DETECTED",
           "value": "FALSE"},
          {"code": "SUN_GLINT_DETECTED",
           "value": "FALSE"},
          {"code": "CIRRUS_DETECTED",
           "value": "FALSE"},
          ]

    mean_sun_angles = {"zenith": -9999,
                       "azimuth": -9999
                       }
    empty_values = [" ".join(["Nan"] * 20)] * 23
    solar_angles_zenith = (5000, 5000, empty_values)
    solar_angles_azimuth = (5000, 5000, empty_values)

    mean_viewing_angles = OrderedDict([("0", {"zenith": -9999,
                                              "azimuth": -9999
                                              }),
                                       ("1", {"zenith": -9999,
                                              "azimuth": -9999
                                              }),
                                       ("2", {"zenith": -9999,
                                              "azimuth": -9999
                                              }),
                                       ("3", {"zenith": -9999,
                                              "azimuth": -9999
                                              }),
                                       ("4", {"zenith": -9999,
                                              "azimuth": -9999
                                              }),
                                       ("5", {"zenith": -9999,
                                              "azimuth": -9999
                                              }),
                                       ("6", {"zenith": -9999,
                                              "azimuth": -9999
                                              }),
                                       ("7", {"zenith": -9999,
                                              "azimuth": -9999
                                              }),
                                       ("8", {"zenith": -9999,
                                              "azimuth": -9999
                                              }),
                                       ("9", {"zenith": -9999,
                                              "azimuth": -9999
                                              }),
                                       ("10", {"zenith": -9999,
                                               "azimuth": -9999
                                               }),
                                       ("11", {"zenith": -9999,
                                               "azimuth": -9999
                                               }),
                                       ("12", {"zenith": -9999,
                                               "azimuth": -9999
                                               }),
                                       ])

    list_of_viewing_angles = OrderedDict([(a, b) for a in range(10) for b in range(3, 13)])

    empty_thing = {"zenith": (5000, 5000, empty_values),
                   "azimuth": (5000, 5000, empty_values)}

    list_of_viewing_angles = OrderedDict([((x, y), empty_thing) for x in BAND_ID for y in DETECTOR_ID])

    get_product_information(b3, gipp_files, qi, mean_sun_angles, solar_angles_zenith, solar_angles_azimuth,
                            mean_viewing_angles, list_of_viewing_angles)

    info_r1 = GdalDatasetInfo(image_r1)
    info_r2 = GdalDatasetInfo(image_r2)

    res = [{"resolution": 10,
            "info": info_r1,
            "list_of_bands": ["B2", "B3", "B4", "B8"]},
           {"resolution": 20,
            "info": info_r2,
            "list_of_bands": ["B5", "B6", "B7", "B8A", "B11", "B12"]}
           ]

    get_image_information(root, res)
    files = get_files(input_dir)
    get_product_organization(root, files, input_dir)

    tree = ET.ElementTree(root)

    f = open(output_filename, "w")
    f.write(ET.tostring(tree, pretty_print=True, xml_declaration=True,
                        encoding="UTF-8"))
    f.close()
    LOGGER.debug("%s Created", output_filename)
