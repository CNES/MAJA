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
from orchestrator.plugins.sentinel2_muscate.maja_sentinel2_muscate_l1_image_info import MajaSentinel2MuscateL1ImageInformations
from orchestrator.plugins.sentinel2_muscate.maja_sentinel2_muscate_l1_image_file_reader import Sentinel2MuscateL1ImageFileReader
from orchestrator.plugins.sentinel2_muscate.maja_sentinel2_muscate_l2_image_writer import MajaSentinel2MuscateL2ImageWriter
from orchestrator.plugins.sentinel2_muscate.maja_sentinel2_muscate_l2_header_writer import MajaSentinel2MuscateL2HeaderWriter
from orchestrator.plugins.sentinel2_muscate.maja_sentinel2_muscate_l2_image_reader import MajaSentinel2MuscateL2ImageReader
from orchestrator.plugins.sentinel2_muscate.maja_sentinel2_muscate_plugin import MajaSentinel2MuscatePlugin


def register_all():
    FactoryBase.register("L1ImageInformationsBase", MajaSentinel2MuscateL1ImageInformations)
    FactoryBase.register("PluginBase",
                         MajaSentinel2MuscatePlugin)
    FactoryBase.register("L1ImageReaderBase",
                         Sentinel2MuscateL1ImageFileReader)
    FactoryBase.register("L2ImageWriterBase",
                         MajaSentinel2MuscateL2ImageWriter)
    FactoryBase.register("L2HeaderWriterBase", MajaSentinel2MuscateL2HeaderWriter)
    FactoryBase.register("L2ImageReaderBase", MajaSentinel2MuscateL2ImageReader)
