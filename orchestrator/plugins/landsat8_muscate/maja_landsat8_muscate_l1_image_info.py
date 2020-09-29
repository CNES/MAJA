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
#                        o  o  o o    o      o o    o     {|||)<
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
from orchestrator.plugins.common.muscate.maja_muscate_l1_image_info import MajaMuscateL1ImageInformations
from orchestrator.plugins.landsat8_muscate.maja_landsat8_muscate_plugin import MajaLandsat8MuscatePlugin
from orchestrator.common.logger.maja_logging import configure_logger
LOGGER = configure_logger(__name__)


class MajaLandsat8MuscateL1ImageInformations(MajaMuscateL1ImageInformations):

    def __init__(self):
        super(MajaLandsat8MuscateL1ImageInformations, self).__init__()
        self._plugin = MajaLandsat8MuscatePlugin()
        self.UniqueSatellite = self._plugin.UniqueSatellite
        self.IS_TILE_DEPENDENT = False

    def detect_l1_products(self, input_directory, product_list):
        LOGGER.info("Start Landsat8Muscate L1 DetectL1Products in " + input_directory)
        MajaMuscateL1ImageInformations.muscate_detect_l1_products(input_directory, product_list, self._plugin)

    def initialize(self, product_filename):
        LOGGER.info("Start Landsat8Muscate L1 Initialize on product " + product_filename)
        return self.muscate_initialize(product_filename, self._plugin)
