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

from orchestrator.common.maja_exceptions import MajaDriverException
from orchestrator.common.xml_tools import get_root_xml, get_only_value

GIPP_SITE_HANDLER_XPATH = {
    "DarkPixelThreshold": "/Earth_Explorer_File/Data_Block/AOT_Estimation/Dark_Pixel_Threshold/text()",
    "DarkestPixelSurfaceReflectance": "/Earth_Explorer_File/Data_Block/AOT_Estimation/" +
                                      "Darkest_Pixel_Surface_Reflectance/text()"}


class GippL2SITEEarthExplorerXMLFileHandler(object):

    def __init__(self, gipp_filename):
        self.gipp_filename = gipp_filename
        self.root = get_root_xml(self.gipp_filename, deannotate=True)
        self.l2_site_values = {}

        for key, value in list(GIPP_SITE_HANDLER_XPATH.items()):
            result = get_only_value(self.root, value)
            if result is not None:
                self.l2_site_values[key] = result

    def get_value(self, key, check=False):
        if key in self.l2_site_values:
            return self.l2_site_values[key]
        else:
            if check:
                return None
            else:
                raise MajaDriverException("No " + key + " in L2SITE dictionnary")


if __name__ == '__main__':
    pass
