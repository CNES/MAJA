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

orchestrator.plugins.sentinel2.maja_sentinel2_l2_angles -- shortdesc

orchestrator.plugins.sentinel2.maja_sentinel2_l2_angles is a description

It defines classes_and_methods

###################################################################################################
"""


from orchestrator.common.xml_tools import get_only_value
from .maja_sentinel2_angles import MajaSunAngles, MajaViewingIncidenceAngles

from orchestrator.common.logger.maja_logging import configure_logger
LOGGER = configure_logger(__name__)

PRODUCT_INFO = "Variable_Header/Specific_Product_Header/Product_Information"
SUN_ANGLE_NODE_NAME = "Solar_Angles"
VIEWING_ANGLE_LIST = "List_of_Viewing_Angles"

# TODO: write a mother class to put write method and col_step and row_step values


class MajaL2Angles(object):

    def __init__(self, root, nss={}):
        self._read(root, nss)

    def _read(self, root, nss={}):
        """
        Read the xml file and extract angles
        """
        product_info_node = get_only_value(root, PRODUCT_INFO)
        LOGGER.debug("product_info_node %s", product_info_node)

        self.sun_angles = MajaSunAngles(product_info_node, SUN_ANGLE_NODE_NAME)

        list_of_viewing_angles_node = get_only_value(product_info_node, VIEWING_ANGLE_LIST)
        LOGGER.debug("list_of_viewing_angles_node %s", list_of_viewing_angles_node)
        self.viewing_incidence_angle = MajaViewingIncidenceAngles(product_info_node, list_of_viewing_angles_node)
