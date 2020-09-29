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

import orchestrator.plugins.sentinel2.maja_sentinel2_factory_register as maja_sentinel2_factory_register
import orchestrator.plugins.sentinel2_muscate.maja_sentinel2_muscate_factory_register as maja_sentinel2_muscate_factory_register
import orchestrator.plugins.venus_muscate.maja_venus_muscate_factory_register as maja_venus_muscate_factory_register
import orchestrator.plugins.landsat8_muscate.maja_landsat8_muscate_factory_register as maja_landsat8_muscate_factory_register
import orchestrator.plugins.landsat8.maja_landsat8_factory_register as maja_landsat8_factory_register
import orchestrator.plugins.venus.maja_venus_factory_register as maja_venus_factory_register


def register_all():
    maja_sentinel2_factory_register.register_all()
    maja_sentinel2_muscate_factory_register.register_all()
    maja_venus_muscate_factory_register.register_all()
    maja_landsat8_muscate_factory_register.register_all()
    maja_landsat8_factory_register.register_all()
    maja_venus_factory_register.register_all()
