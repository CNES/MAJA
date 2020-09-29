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

orchestrator.common.earth_explorer.gipp_l2_comm_earth_explorer_xml_file_handler -- shortdesc

orchestrator.common.earth_explorer.gipp_l2_comm_earth_explorer_xml_file_handler is a description

It defines classes_and_methods

###################################################################################################
"""
from orchestrator.common.earth_explorer.earth_explorer_xml_file_handler import EarthExplorerXMLFileHandler
import orchestrator.common.xml_tools as xml_tools


LUT_EARTH_EXPLORER_HANDLER_XPATH = {
    "DBLFilenames": "//Earth_Explorer_Header/Variable_Header/Specific_Product_Header/"
    "DBL_Organization/List_of_Packaged_DBL_Files/Packaged_DBL_File/Relative_File_Path",
    "LUTIndexes": "//Earth_Explorer_Header/Variable_Header/Specific_Product_Header/LUT_Indexes",
    "RH_dep": "//Earth_Explorer_Header/Variable_Header/Specific_Product_Header/RH_dep"}


class GippLUTEarthExplorerXMLFileHandler(EarthExplorerXMLFileHandler):

    def __init__(self, gipp_filename, validate=False, schema_path=None):
        super(GippLUTEarthExplorerXMLFileHandler, self).__init__(gipp_filename, validate=validate,
                                                                 schema_path=schema_path)

    def get_lut_indexes(self):
        lut_indexes = {}
        lut_indexes_xml = xml_tools.get_only_value(self.root, LUT_EARTH_EXPLORER_HANDLER_XPATH["LUTIndexes"])
        for index in lut_indexes_xml.iterchildren():
            lut_indexes[index.tag] = index.text
        return lut_indexes

    def has_cams_info(self):
        return xml_tools.get_only_value(self.root, LUT_EARTH_EXPLORER_HANDLER_XPATH["RH_dep"], check=True) is not None

    def get_cams_info(self):
        l_cams_info = dict()
        l_cams_info["extinction_coeffs"] = []
        if self.has_cams_info():
            l_count = int(
                xml_tools.get_attribute(
                    self.root,
                    "/Earth_Explorer_Header/Variable_Header/Specific_Product_Header/Extinction_Coefs_List",
                    "count"))
            # loop on coefficients
            for i in range(0, l_count):
                extcoeffs = {}
                l_string = str(i + 1)
                # retrieve description name and value
                extcoeffs["Description"] = xml_tools.get_xml_string_value(
                    self.root,
                    "/Earth_Explorer_Header/Variable_Header/" +
                    "Specific_Product_Header/Extinction_Coefs_List/Extinction_Coef[@n=" +
                    l_string +
                    "]/Description")
                extcoeffs["Name"] = xml_tools.get_xml_string_value(
                    self.root,
                    "/Earth_Explorer_Header/Variable_Header/Specific_Product_Header/" +
                    "Extinction_Coefs_List/Extinction_Coef[@n=" +
                    l_string +
                    "]/Name")
                extcoeffs["Values"] = xml_tools.as_string_list(
                    xml_tools.get_xml_string_value(
                        self.root,
                        "/Earth_Explorer_Header/Variable_Header/Specific_Product_Header/" +
                        "Extinction_Coefs_List/Extinction_Coef[@n=" +
                        l_string +
                        "]/Values"))
                l_cams_info["extinction_coeffs"].append(extcoeffs)
        l_cams_info["rh_dep"] = xml_tools.get_xml_bool_value(
            self.root, "//Earth_Explorer_Header/Variable_Header/Specific_Product_Header/RH_dep")
        l_cams_info["rh_tab"] = xml_tools.as_string_list(xml_tools.get_xml_string_value(
            self.root, "//Earth_Explorer_Header/Variable_Header/Specific_Product_Header/RH_Tab"))
        return l_cams_info
