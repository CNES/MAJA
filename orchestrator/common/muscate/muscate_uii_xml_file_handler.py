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
                        o  o  o o    o      o o    o     |||D
                        o     o oooooo      o oooooo      / \
                        o     o o    o o    o o    o      `-^
                        o     o o    o  oooo  o    o

###################################################################################################

orchestrator.common.earth_explorer.gipp_l2_comm_earth_explorer_xml_file_handler -- shortdesc

orchestrator.common.earth_explorer.gipp_l2_comm_earth_explorer_xml_file_handler is a description

It defines classes_and_methods

###################################################################################################
"""

import orchestrator.common.xml_tools as xml_tools
from orchestrator.common.logger.maja_logging import configure_logger

LOGGER = configure_logger(__name__)


class MuscateUIIXMLFileHandler(object):

    M_SCHEMA_DECIMAL_9_FORMAT = "%.9f"

    def __init__(self, main_xml_file, validate=False, schema_path=None):
        """
        Constructor
        """
        LOGGER.info("Loading " + main_xml_file)
        self.main_xml_file = main_xml_file
        self.root = xml_tools.get_root_xml(self.main_xml_file, deannotate=True)
        if validate and schema_path is not None:
            xml_tools.check_xml(main_xml_file, schema_path)

    # Writing parts
    def save_to_file(self, output_filename):
        xml_tools.save_to_xml_file(self.root, output_filename)

    def replace_node(self, node_src, xpath, check_if_present=False):
        xml_tools.replace_node(node_src, xpath, self.root, xpath, check_if_present)

    # Reading parts
    # Get the center in latitude / longitude coordinate system
    def get_geopositioning_center(self):
        # lowerRightCorner[0] = Long lowerRightCorner[1] = Lat
        return (
            xml_tools.get_xml_float_value(
                self.root,
                "//Geoposition_Informations/Product_Geopositining/Point[@name='center']/LON"),
            xml_tools.get_xml_float_value(
                self.root,
                "//Geoposition_Informations/Product_Geopositining/Point[@name='center']/LAT"))

    # Get the viewing angle at center
    def get_image_center_viewing_angles(self, detector):
        # vieAngles[0] = zenith vieAngles[1] = azimuth
        return (
            xml_tools.get_xml_float_value(
                self.root,
                "//Viewing_Incidence_Angles_List[@detector_id='" +
                detector +
                "']/Viewing_Incidence_Angles[@name='center']/ZENITH_ANGLE"),
            xml_tools.get_xml_float_value(
                self.root,
                "//Viewing_Incidence_Angles_List[@detector_id='" +
                detector +
                "']/Viewing_Incidence_Angles[@name='center']/AZIMUTH_ANGLE"))

    # Get the solar angle at center
    def get_image_center_solar_angles(self):
        # vieAngles[0] = zenith vieAngles[1] = azimuth
        return (xml_tools.get_xml_float_value(self.root,
                                              "//Sun_Angles_List/Sun_Angles[@name='center']/ZENITH_ANGLE"),
                xml_tools.get_xml_float_value(self.root,
                                              "//Sun_Angles_List/Sun_Angles[@name='center']/AZIMUTH_ANGLE"))
