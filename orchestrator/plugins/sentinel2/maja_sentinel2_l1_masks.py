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

orchestrator.plugins.sentinel2.maja_sentinel2_l1_masks -- shortdesc

orchestrator.plugins.sentinel2.maja_sentinel2_l1_masks is a description

It defines classes_and_methods

###################################################################################################
"""

import os
from orchestrator.common.xml_tools import get_only_value
from orchestrator.common.logger.maja_logging import configure_logger
LOGGER = configure_logger(__name__)

QUALITY_INFO = "n1:Quality_Indicators_Info"
MASKS = "Pixel_Level_QI/MASK_FILENAME"
BAND_ID = "./@bandId"
MASKS_TYPE = "./@type"
MASKS_VALUE = "./text()"

# TODO: write a mother class to put write method and col_step and row_step values


class MajaL1Masks():

    def __init__(self, root, product_directory="", nss={}):
        self.product_directory = product_directory
        self.masks_filepaths = {"MSK_DEFECT": {},
                                "MSK_DETFOO": {},
                                "MSK_NODATA": {},
                                "MSK_SATURA": {},
                                "MSK_TECQUA": {},
                                "MSK_CLOUDS": None}

        self._read(root, nss)

    def _read(self, root, nss={}):
        """
        Read the xml file and extract angles
        """
        quality_info_node = get_only_value(root, QUALITY_INFO, namespaces=nss)
        masks_nodes = quality_info_node.xpath(MASKS)
        # LOGGER.debug(masks_nodes)

        for mask_element in masks_nodes:
            mask_type = get_only_value(mask_element, MASKS_TYPE)
            band_id = get_only_value(mask_element, BAND_ID)
            mask_filename = get_only_value(mask_element, "./text()")
            # LOGGER.debug("Mask type:  %s \n band id %s, \n filename : %s", mask_type, band_id, mask_filename)

            if band_id is not None:
                self.masks_filepaths[mask_type][band_id] = os.path.join(self.product_directory, mask_filename)
            else:
                self.masks_filepaths[mask_type] = os.path.join(self.product_directory, mask_filename)

        # print "##", self.masks_filepaths&

    @property
    def raster_no_data(self):
        """

        :return:
        """
        return
