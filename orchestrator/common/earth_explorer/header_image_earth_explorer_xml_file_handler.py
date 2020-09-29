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

orchestrator.common.earth_explorer.header_image_earth_explorer_xml_file_handler -- shortdesc

orchestrator.common.earth_explorer.header_image_earth_explorer_xml_file_handler is a description

It defines classes_and_methods

###################################################################################################
"""

import orchestrator.common.xml_tools as xml_tools

from orchestrator.common.logger.maja_logging import configure_logger
from orchestrator.common.earth_explorer.earth_explorer_xml_file_handler import EarthExplorerXMLFileHandler

LOGGER = configure_logger(__name__)

REFLECTANCE_QUANTIF_VALUE = "//Earth_Explorer_Header/Variable_Header/Specific_Product_Header/Product_Information/" + \
                            "Reflectance_Quantification_Value"
ADJ_EFFECTS_AND_SLOPE_CORR = "//Processing_Flags_And_Modes/Adjacency_Effects_And_Slope_Correction"
VAP_QUANTIF_VALUE = "/Earth_Explorer_Header/Variable_Header/Specific_Product_Header/Annex_Information/" + \
                    "VAP_Quantification_Value"
AOT_QUANTIF_VALUE = "//Earth_Explorer_Header/Variable_Header/Specific_Product_Header/Annex_Information/" + \
                    "AOT_Quantification_Value"
ACQUISITION_DATE_TIME = "//Earth_Explorer_Header/Variable_Header/Specific_Product_Header/Product_Information/" + \
                        "Acquisition_Date_Time"
ACQUISITION_DATE = "//Earth_Explorer_Header/Variable_Header/Specific_Product_Header/Instance_Id/Acquisition_Date"
ACQUI_ORBIT_NUMBER = "//Earth_Explorer_Header/Variable_Header/Specific_Product_Header/Product_Information/" + \
                     "Acquisition_Orbit_Number"
CLOUD_PERCENTAGE = "//Earth_Explorer_Header/Variable_Header/Specific_Product_Header/Product_Information/" + \
                   "Cloud_Percentage"
AOT_NO_DATA_VALUE = "//Earth_Explorer_Header/Variable_Header/Specific_Product_Header/Annex_Information/" + \
                    "AOT_Nodata_Value"
NICK_NAME = "//Earth_Explorer_Header/Variable_Header/Specific_Product_Header/Instance_Id/Nick_Name"
VAP_NO_DATA_VALUE = "//Earth_Explorer_Header/Variable_Header/Specific_Product_Header/Annex_Information/" + \
                    "VAP_Nodata_Value"
RESOLUTION = "//Earth_Explorer_Header/Variable_Header/Specific_Product_Header/Product_Information/" + \
             "Product_Sampling/By_Line"
NO_DATA_VALUE = "//Earth_Explorer_Header/Variable_Header/Specific_Product_Header/Image_Information/" + \
                "Nodata_Value"
CENTRAL_DATE = "//Earth_Explorer_Header/Variable_Header/Specific_Product_Header/Instance_Id/Central_Date"
REFERENCE_SITE_DEFINITION_ID = "//Reference_SiteDefinition_Id"
BAND_THEORICAL_WAVELENGTH = "//Band_Theoretical_Wavelength"
GIPP_FILES = "/Earth_Explorer_Header/Variable_Header/Specific_Product_Header/Product_Information/" + \
             "Processing_Information/List_of_GIPP_Files"
GIPP_FILE = GIPP_FILES + "/GIPP_File"
GIPP_FILE_INDEX = "@sn"
GIPP_FILE_NATURE = "Nature"
GIPP_FILE_LOCATION = "File_Location"
GIPP_FILE_LOGICAL_NAME = "Logical_Name"
QUALITY_INDEX = "/Earth_Explorer_Header/Variable_Header/Specific_Product_Header/Product_Information/" + \
                "List_of_Quality_Indexes"
ONE_QUALITY_INDEX = QUALITY_INDEX + "/Quality_Index"
QUALITY_INDEX_CODE = "/Code"
QUALITY_INDEX_VALUE = "/Value"
QUALITY_INDEX_BAND_CODE = "NOT_INITIALIZED"  # Band_Code


class HeaderImageEarthExplorerXMLFileHandler(EarthExplorerXMLFileHandler):

    def __init__(self, filename):
        super(HeaderImageEarthExplorerXMLFileHandler, self).__init__(filename)

    def get_reflectance_quantification_value(self):
        """
        GetReflectance_Quantification_Value
        :return: double
        """
        return xml_tools.get_xml_float_value(self.root, REFLECTANCE_QUANTIF_VALUE)

    def get_adjacency_effects_and_slope_correction(self):
        """
        GetAdjacency_Effects_And_Slope_Correction
        Read the EarthExplorer field LUT Index 'AOT_Indexes'
        :return:
        """
        # For L2 header : xpath = "/Earth_Explorer_Header/Variable_Header/Specific_Product_Header/
        # + Product_Information/Processing_Flags_And_Modes/Adjacency_Effects_And_Slope_CorrectionEarth_Explorer_Header/
        # + Variable_Header/Specific_Product_Header/L2_Products_Information/Input_L2_Product_Information_Central_Date/
        # + Processing_Flags_And_Modes/Adjacency_Effects_And_Slope_Correction"
        # For L3 header : xpath = "/Earth_Explorer_Header/Variable_Header/Specific_Product_Header/
        # + L2_Products_Information/Input_L2_Product_Information_Central_Date/
        # + Processing_Flags_And_Modes/Adjacency_Effects_And_Slope_Correction"
        # sor read relative xpath "//Processing_Flags_And_Modes/Adjacency_Effects_And_Slope_Correction"
        return xml_tools.get_xml_bool_value(self.root, ADJ_EFFECTS_AND_SLOPE_CORR)

    def get_vap_quantification_value(self):
        """
        GetVAPQuantificationValue
        :return:
        """
        return xml_tools.get_xml_float_value(self.root, VAP_QUANTIF_VALUE)

    def get_aot_quantification_value(self):
        """
        GetAOTQuantificationValue
        :return:
        """
        return xml_tools.get_xml_float_value(self.root, AOT_QUANTIF_VALUE)

    def get_acquisition_date(self):
        """
        GetAcquisition_Date_Time
        :return:
        """
        return xml_tools.get_xml_string_value(self.root, ACQUISITION_DATE)

    def get_acquisition_orbit_number(self):
        """
        GetAcquisition_Orbit_Number
        :return:
        """
        return xml_tools.get_xml_string_value(self.root, ACQUI_ORBIT_NUMBER)

    def get_cloud_percentage(self):
        """
        GetCloud_Percentage
        :return:
        """
        return xml_tools.get_xml_int_value(self.root, CLOUD_PERCENTAGE)

    def get_aot_no_data_value(self):
        """
        GetAOT_Nodata_Value
        :return:
        """
        return xml_tools.get_xml_int_value(self.root, AOT_NO_DATA_VALUE)

    def get_vap_no_data_value(self):
        """
        GetVAP_Nodata_Value
        :return:
        """
        return xml_tools.get_xml_int_value(self.root, VAP_NO_DATA_VALUE)

    def get_resolution(self):
        """
        GetResolution
        :return:
        """
        return xml_tools.get_xml_int_value(self.root, RESOLUTION)

    def get_no_data_value_as_double(self):
        """
        GetResolution
        :return:
        """
        return xml_tools.get_xml_float_value(self.root, NO_DATA_VALUE)

    def get_no_data_value_as_int(self):
        """
        GetNodata_Value_AsDouble
        :return:
        """
        return xml_tools.get_xml_int_value(self.root, NO_DATA_VALUE)

    def get_central_date(self):
        """
        GetCentral_Date
        :return:
        """
        return xml_tools.get_xml_string_value(self.root, CENTRAL_DATE)

    def get_list_of_gipp_files(self):
        """
        GetList_of_GIPP_Files
        :return:
        """
        l_listofgipps = []
        for gipp_node in xml_tools.get_all_values(self.root, GIPP_FILE):
            one_gipp = dict()
            one_gipp["index"] = xml_tools.get_only_value(gipp_node, GIPP_FILE_INDEX)
            one_gipp["file_location"] = xml_tools.get_xml_string_value(gipp_node, GIPP_FILE_LOCATION)
            one_gipp["nature"] = xml_tools.get_xml_string_value(gipp_node, GIPP_FILE_NATURE)
            one_gipp["logical_name"] = xml_tools.get_xml_string_value(gipp_node, GIPP_FILE_LOGICAL_NAME)
            l_listofgipps.append(one_gipp)
        return l_listofgipps

    def get_theoretical_wavelength(self):
        """
        GetTheoreticalWavelength
        TODO: check this one
        :return:
        """
        return xml_tools.get_xml_int_value(self.root, BAND_THEORICAL_WAVELENGTH)

    def get_nick_name(self):
        """
        GetNickName
        :return:
        """
        return xml_tools.get_xml_string_value(self.root, NICK_NAME)

    def get_list_of_quality_index(self):
        """
        GetListOfQualityIndexes
        :return:
        """
        l_listofqualityindexes = []

        for quality_index_node in xml_tools.get_all_values(self.root, ONE_QUALITY_INDEX):
            one_quality_index = dict()
            one_quality_index["index"] = xml_tools.get_xml_int_value(quality_index_node, GIPP_FILE_INDEX)
            one_quality_index["code"] = xml_tools.get_xml_int_value(quality_index_node, QUALITY_INDEX_CODE)
            one_quality_index["value"] = xml_tools.get_xml_int_value(quality_index_node, QUALITY_INDEX_VALUE)
            one_quality_index["band_code"] = QUALITY_INDEX_BAND_CODE

            l_listofqualityindexes.append(one_quality_index)

            LOGGER.debug("Add the quality index [%s] Code=<%s> Value=<%s>  Band_Code=<%s> ",
                         one_quality_index["index"],
                         one_quality_index["code"],
                         one_quality_index["value"],
                         one_quality_index["band_code"])

    def get_reference_site_definition_id(self):
        """
        GetReference_SiteDefinition_Id
        :return:
        """
        return xml_tools.get_xml_string_value(self.root, REFERENCE_SITE_DEFINITION_ID)

    def write(self):
        """
        Implements the write method
        :return:
        """

