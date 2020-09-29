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
from orchestrator.plugins.common.muscate.maja_muscate_l2_image_file_reader import MuscateL2ImageFileReader
from orchestrator.plugins.landsat8_muscate.maja_landsat8_muscate_plugin import MajaLandsat8MuscatePlugin


class MajaLandsat8MuscateL2ImageReader(MuscateL2ImageFileReader):

    def __init__(self):
        super(MajaLandsat8MuscateL2ImageReader, self).__init__()
        self.plugin = MajaLandsat8MuscatePlugin()

    def detect_l2_products(self, dir, product_list):
        self.muscate_detect_l2_products(dir, product_list, self.plugin)

    def can_read(self, product_filename):
        return self.can_read_with_plugin_base(product_filename, self.plugin)

    def read(self, product_filename, app_handler, enable_public_data):
        self.read_with_plugin_base(product_filename, enable_public_data, app_handler, self.plugin)

    def read_info(self, product_filename, enable_public_data):
        self.read_info_with_plugin_base(product_filename, enable_public_data, self.plugin)
