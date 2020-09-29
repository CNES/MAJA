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

orchestrator.plugins.sentinel2.maja_sentinel2_l1_angles -- shortdesc

orchestrator.plugins.sentinel2.maja_sentinel2_l1_angles is a description

It defines classes_and_methods

###################################################################################################
"""
from orchestrator.plugins.common.factory.factory_base import FactoryBase
from orchestrator.plugins.landsat8.maja_landsat8_l1_image_info import MajaLandsat8L1ImageInformations
from orchestrator.plugins.landsat8.maja_landsat8_plugin import MajaLandsat8Plugin
from orchestrator.plugins.landsat8.maja_landsat8_l1_image_file_reader import Landsat8L1ImageFileReader


def register_all():
    FactoryBase.register("L1ImageInformationsBase",
                         MajaLandsat8L1ImageInformations)
    FactoryBase.register("PluginBase",
                         MajaLandsat8Plugin)
    FactoryBase.register("L1ImageReaderBase",
                         Landsat8L1ImageFileReader)


