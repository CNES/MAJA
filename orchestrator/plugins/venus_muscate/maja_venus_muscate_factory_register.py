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
from orchestrator.plugins.venus_muscate.maja_venus_muscate_l1_image_info import MajaVenusMuscateL1ImageInformations
from orchestrator.plugins.venus_muscate.maja_venus_muscate_l1_image_file_reader import VenusMuscateL1ImageFileReader
from orchestrator.plugins.venus_muscate.maja_venus_muscate_l2_image_writer import MajaVenusMuscateL2ImageWriter
from orchestrator.plugins.venus_muscate.maja_venus_muscate_l2_header_writer import MajaVenusMuscateL2HeaderWriter
from orchestrator.plugins.venus_muscate.maja_venus_muscate_l2_image_reader import MajaVenusMuscateL2ImageReader
from orchestrator.plugins.venus_muscate.maja_venus_muscate_plugin import MajaVenusMuscatePlugin


def register_all():
    FactoryBase.register("L1ImageInformationsBase", MajaVenusMuscateL1ImageInformations)
    FactoryBase.register("PluginBase",
                         MajaVenusMuscatePlugin)
    FactoryBase.register("L1ImageReaderBase",
                         VenusMuscateL1ImageFileReader)
    FactoryBase.register("L2ImageWriterBase",
                         MajaVenusMuscateL2ImageWriter)
    FactoryBase.register("L2HeaderWriterBase", MajaVenusMuscateL2HeaderWriter)
    FactoryBase.register("L2ImageReaderBase", MajaVenusMuscateL2ImageReader)
