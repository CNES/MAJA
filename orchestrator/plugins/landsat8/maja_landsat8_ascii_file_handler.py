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
from orchestrator.common.maja_exceptions import MajaDriverException

LOGGER = configure_logger(__name__)

class Landsat8ASCIIFileHandler(object):
    def __init__(self, main_txt_file):
        """
        Constructor
        """
        LOGGER.info("Loading " + main_txt_file)
        self.main_txt_file = main_txt_file
        self.l8ascii_values = {}
        with open(self.main_txt_file) as f:
            for line in f:
                if (len(line.split("=")) > 1):
                    key, value = line.strip().split(' ', 1)
                    self.l8ascii_values[key] = value.split('= ')[1].replace('"','')

    def _get_value(self, key):
        if key in self.l8ascii_values:
            return self.l8ascii_values[key]
        else:
            raise MajaDriverException("No " + key + " in Landsat8 ASCII dictionnary")

    def get_platform(self):
        return self._get_value("SPACECRAFT_ID")

    def get_product_id(self):
        return self._get_value("LANDSAT_SCENE_ID")

    def get_qbi_image_filename(self):
        filename = self._get_value("FILE_NAME_BAND_QUALITY")
        return os.path.join(os.path.dirname(self.main_txt_file), filename)

    def get_toa_image_filename_from_band_code(self, code):
        filename = self._get_value("FILE_NAME_BAND_"+code[1:])
        return os.path.join(os.path.dirname(self.main_txt_file), filename)


    def get_ref_mutl_factor_from_band_code(self, code):
        return float(self._get_value("REFLECTANCE_MULT_BAND_"+code[1:]))

    def get_ref_add_factor_from_band_code(self, code):
        return float(self._get_value("REFLECTANCE_ADD_BAND_"+code[1:]))

    def get_sol_zenith(self):
        value = 90-float(self._get_value("SUN_ELEVATION"))
        return value

    def get_sol_azitmuth(self):
        return float(self._get_value("SUN_AZIMUTH"))

    def get_acquisition_date(self):
        return self._get_value("DATE_ACQUIRED").replace('-','')

    def get_zone_geo(self):
        return self._get_value("LANDSAT_SCENE_ID")

    def get_date_pdv(self):
        date = self._get_value("DATE_ACQUIRED")
        time = self._get_value("SCENE_CENTER_TIME")
        timesplit = time.split('.')
        if len(timesplit) == 2:
            if len(timesplit[1]) > 3:
                time = timesplit[0] + '.' + timesplit[1][0:3] + 'Z'
        utc_date = 'UTC='+date+'T'+time
        return utc_date

    def get_projection(self):
        map = self._get_value("MAP_PROJECTION")
        zone = self._get_value("UTM_ZONE")
        return map+zone

    def get_resolution(self):
        return float(self._get_value("GRID_CELL_SIZE_REFLECTIVE"))

    def get_absolute_coefficients(self):
        return ""

    def get_origin(self):
        return (float(self._get_value("CORNER_UL_PROJECTION_X_PRODUCT")), float(self._get_value("CORNER_UL_PROJECTION_Y_PRODUCT")))

    def get_pixel_size(self):
        return (self.get_resolution(), -self.get_resolution())

    def get_upper_left_corner(self):
        return (float(self._get_value("CORNER_UL_LON_PRODUCT")), float(self._get_value("CORNER_UL_LAT_PRODUCT")))

    def get_upper_right_corner(self):
        return (float(self._get_value("CORNER_UR_LON_PRODUCT")), float(self._get_value("CORNER_UR_LAT_PRODUCT")))

    def get_lower_left_corner(self):
        return (float(self._get_value("CORNER_LL_LON_PRODUCT")), float(self._get_value("CORNER_LL_LAT_PRODUCT")))

    def get_lower_right_corner(self):
        return (float(self._get_value("CORNER_LR_LON_PRODUCT")), float(self._get_value("CORNER_LR_LAT_PRODUCT")))

    def get_mean_view_angles(self):
        return (0.0,0.0)

    def get_mean_solar_angles(self):
        return (self.get_sol_zenith(), self.get_sol_azitmuth())

    def get_nick_name(self):
        lpath = int(self._get_value("WRS_PATH"))
        lrow = int(self._get_value("WRS_ROW"))
        return '{:03d}-{:03d}'.format(lpath,lrow)

    def get_generation_date(self):
        return self._get_value("FILE_DATE")

    def get_instrument(self):
        return self._get_value("SENSOR_ID").replace("_","")

    def get_orbit_number(self):
        return self._get_value("WRS_PATH")
