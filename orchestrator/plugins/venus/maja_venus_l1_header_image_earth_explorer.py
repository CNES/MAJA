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
import os
from orchestrator.common.logger.maja_logging import configure_logger
from orchestrator.common.earth_explorer.header_image_earth_explorer_xml_file_handler import HeaderImageEarthExplorerXMLFileHandler
from orchestrator.common.xml_tools import get_xml_float_value, get_xml_int_value
from orchestrator.common.maja_exceptions import MajaDriverException

LOGGER = configure_logger(__name__)

class VenusL1HeaderImageEarthExplorerXMLFileHandler(HeaderImageEarthExplorerXMLFileHandler):

    def __init__(self, filename):
        super(VenusL1HeaderImageEarthExplorerXMLFileHandler, self).__init__(filename)


    def get_useful_image_center_view_angle_zenith(self, index):
        localpath = "/Earth_Explorer_Header/Variable_Header/Specific_Product_Header/Product_Information/" \
                    "List_of_Viewing_Angles/Viewing_Angles[@sn='" + str(index) + "']/Useful_Image/Image_Center/Zenith"
        return get_xml_float_value(self.root, localpath)

    def get_useful_image_center_view_angle_azimuth(self, index):
        localpath = "/Earth_Explorer_Header/Variable_Header/Specific_Product_Header/Product_Information/" \
                    "List_of_Viewing_Angles/Viewing_Angles[@sn='" + str(index) + "']/Useful_Image/Image_Center/Azimuth"
        return get_xml_float_value(self.root, localpath)

    def get_useful_image_lower_left_corner_view_angle_zenith(self, index):
        localpath = "/Earth_Explorer_Header/Variable_Header/Specific_Product_Header/Product_Information/" \
                    "List_of_Viewing_Angles/Viewing_Angles[@sn='" + str(index) + "']/Useful_Image/Upper_Left_Corner/Zenith"
        return get_xml_float_value(self.root, localpath)

    def get_useful_image_upper_left_corner_view_angle_azimuth(self, index):
        localpath = "/Earth_Explorer_Header/Variable_Header/Specific_Product_Header/Product_Information/" \
                    "List_of_Viewing_Angles/Viewing_Angles[@sn='" + str(index) + "']/Useful_Image/Upper_Left_Corner/Azimuth"
        return get_xml_float_value(self.root, localpath)

    def get_useful_image_upper_right_corner_view_angle_zenith(self, index):
        localpath = "/Earth_Explorer_Header/Variable_Header/Specific_Product_Header/Product_Information/" \
                    "List_of_Viewing_Angles/Viewing_Angles[@sn='" + str(index) + "']/Useful_Image/Upper_Right_Corner/Zenith"
        return get_xml_float_value(self.root, localpath)

    def get_useful_image_upper_right_corner_view_angle_azimuth(self, index):
        localpath = "/Earth_Explorer_Header/Variable_Header/Specific_Product_Header/Product_Information/" \
                    "List_of_Viewing_Angles/Viewing_Angles[@sn='" + str(index) + "']/Useful_Image/Upper_Right_Corner/Azimuth"
        return get_xml_float_value(self.root, localpath)

    def get_useful_image_lower_left_corner_view_angle_zenith(self, index):
        localpath = "/Earth_Explorer_Header/Variable_Header/Specific_Product_Header/Product_Information/" \
                    "List_of_Viewing_Angles/Viewing_Angles[@sn='" + str(index) + "']/Useful_Image/Lower_Left_Corner/Zenith"
        return get_xml_float_value(self.root, localpath)

    def get_useful_image_lower_left_corner_view_angle_azimuth(self, index):
        localpath = "/Earth_Explorer_Header/Variable_Header/Specific_Product_Header/Product_Information/" \
                    "List_of_Viewing_Angles/Viewing_Angles[@sn='" + str(index) + "']/Useful_Image/Lower_Left_Corner/Azimuth"
        return get_xml_float_value(self.root, localpath)

    def get_useful_image_lower_right_corner_view_angle_zenith(self, index):
        localpath = "/Earth_Explorer_Header/Variable_Header/Specific_Product_Header/Product_Information/" \
                    "List_of_Viewing_Angles/Viewing_Angles[@sn='" + str(index) + "']/Useful_Image/Lower_Right_Corner/Zenith"
        return get_xml_float_value(self.root, localpath)

    def get_useful_image_lower_right_corner_view_angle_azimuth(self, index):
        localpath = "/Earth_Explorer_Header/Variable_Header/Specific_Product_Header/Product_Information/" \
                    "List_of_Viewing_Angles/Viewing_Angles[@sn='" + str(index) + "']/Useful_Image/Lower_Right_Corner/Azimuth"
        return get_xml_float_value(self.root, localpath)

##
    def get_product_upper_left_corner_view_angle_zenith(self, index):
        localpath = "/Earth_Explorer_Header/Variable_Header/Specific_Product_Header/Product_Information/" \
                    "List_of_Viewing_Angles/Viewing_Angles[@sn='" + str(index) + "']/Product/Upper_Left_Corner/Zenith"
        return get_xml_float_value(self.root, localpath)

    def get_product_upper_left_corner_view_angle_azimuth(self, index):
        localpath = "/Earth_Explorer_Header/Variable_Header/Specific_Product_Header/Product_Information/" \
                    "List_of_Viewing_Angles/Viewing_Angles[@sn='" + str(index) + "']/Product/Upper_Left_Corner/Azimuth"
        return get_xml_float_value(self.root, localpath)

    def get_product_upper_right_corner_view_angle_zenith(self, index):
        localpath = "/Earth_Explorer_Header/Variable_Header/Specific_Product_Header/Product_Information/" \
                    "List_of_Viewing_Angles/Viewing_Angles[@sn='" + str(index) + "']/Product/Upper_Right_Corner/Zenith"
        return get_xml_float_value(self.root, localpath)

    def get_product_upper_right_corner_view_angle_azimuth(self, index):
        localpath = "/Earth_Explorer_Header/Variable_Header/Specific_Product_Header/Product_Information/" \
                    "List_of_Viewing_Angles/Viewing_Angles[@sn='" + str(index) + "']/Product/Upper_Right_Corner/Azimuth"
        return get_xml_float_value(self.root, localpath)

    def get_product_lower_left_corner_view_angle_zenith(self, index):
        localpath = "/Earth_Explorer_Header/Variable_Header/Specific_Product_Header/Product_Information/" \
                    "List_of_Viewing_Angles/Viewing_Angles[@sn='" + str(index) + "']/Product/Lower_Left_Corner/Zenith"
        return get_xml_float_value(self.root, localpath)

    def get_product_lower_left_corner_view_angle_azimuth(self, index):
        localpath = "/Earth_Explorer_Header/Variable_Header/Specific_Product_Header/Product_Information/" \
                    "List_of_Viewing_Angles/Viewing_Angles[@sn='" + str(index) + "']/Product/Lower_Left_Corner/Azimuth"
        return get_xml_float_value(self.root, localpath)

    def get_product_lower_right_corner_view_angle_zenith(self, index):
        localpath = "/Earth_Explorer_Header/Variable_Header/Specific_Product_Header/Product_Information/" \
                    "List_of_Viewing_Angles/Viewing_Angles[@sn='" + str(index) + "']/Product/Lower_Right_Corner/Zenith"
        return get_xml_float_value(self.root, localpath)

    def get_product_lower_right_corner_view_angle_azimuth(self, index):
        localpath = "/Earth_Explorer_Header/Variable_Header/Specific_Product_Header/Product_Information/" \
                    "List_of_Viewing_Angles/Viewing_Angles[@sn='" + str(index) + "']/Product/Lower_Right_Corner/Azimuth"
        return get_xml_float_value(self.root, localpath)


##
    def get_useful_image_image_center_solar_angle_zenith(self):
        localpath = "/Earth_Explorer_Header/Variable_Header/Specific_Product_Header/Product_Information/Solar_Angles/Useful_Image/Image_Center/Zenith"
        return get_xml_float_value(self.root, localpath)

    def get_useful_image_image_center_solar_angle_azimuth(self):
        localpath = "/Earth_Explorer_Header/Variable_Header/Specific_Product_Header/Product_Information/Solar_Angles/Useful_Image/Image_Center/Azimuth"
        return get_xml_float_value(self.root, localpath)

    def get_useful_image_geo_coverage_center_corner_long(self):
        localpath = "/Earth_Explorer_Header/Variable_Header/Specific_Product_Header/Product_Information/Useful_Image_Geo_Coverage/Center/Long"
        return get_xml_float_value(self.root, localpath)

    def get_useful_image_geo_coverage_center_corner_lat(self):
        localpath = "/Earth_Explorer_Header/Variable_Header/Specific_Product_Header/Product_Information/Useful_Image_Geo_Coverage/Center/Lat"
        return get_xml_float_value(self.root, localpath)

    def get_useful_image_geo_coverage_center_corner_line(self):
        localpath = "/Earth_Explorer_Header/Variable_Header/Specific_Product_Header/Product_Information/Useful_Image_Geo_Coverage/Center/Line"
        return get_xml_int_value(self.root, localpath)

    def get_useful_image_geo_coverage_center_corner_column(self):
        localpath = "/Earth_Explorer_Header/Variable_Header/Specific_Product_Header/Product_Information/Useful_Image_Geo_Coverage/Center/Column"
        return get_xml_int_value(self.root, localpath)










