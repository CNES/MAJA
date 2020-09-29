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

:copyright: 2019 CNES. All rights reserved.
:license: license

###################################################################################################
"""

from orchestrator.common.earth_explorer.earth_explorer_xml_file_handler import EarthExplorerXMLFileHandler
import orchestrator.common.xml_tools as xml_tools


CAMS_EARTH_EXPLORER_HANDLER_XPATH = {
    "ModelLevels": "//Earth_Explorer_Header/Variable_Header/Specific_Product_Header/ModelLevels",
    "AcquisitionDateTime": "//Earth_Explorer_Header/Variable_Header/Specific_Product_Header/Instance_Id/"
                           + "Acquisition_Date_Time"}


class GippCAMSEarthExplorerXMLFileHandler(EarthExplorerXMLFileHandler):

    def __init__(self, gipp_filename, validate_schema=False, schema_path=None):
        super(GippCAMSEarthExplorerXMLFileHandler, self).__init__(gipp_filename, validate_schema, schema_path)
        self.model_levels = xml_tools.as_float_list(xml_tools.get_only_value(
            self.root, CAMS_EARTH_EXPLORER_HANDLER_XPATH["ModelLevels"]).text)
        self.model_levels = [float(f) for f in self.model_levels]
        self.acquisition_date_time = xml_tools.get_only_value(
            self.root, CAMS_EARTH_EXPLORER_HANDLER_XPATH["AcquisitionDateTime"]).text
