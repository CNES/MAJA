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

orchestrator.tests.plugins.test_maja_sentinel2_l1_pimage_file_reader -- shortdesc

orchestrator.tests.plugins.test_maja_sentinel2_l1_pimage_file_reader is a description

It defines classes_and_methods


###################################################################################################
"""

from __future__ import absolute_import
from orchestrator.common.logger.maja_logging import configure_logger
LOGGER = configure_logger(__name__)
from orchestrator.plugins.common.factory.factory_base import FactoryBase
import os
from orchestrator.plugins.sentinel2.maja_sentinel2_l1_image_file_reader import Sentinel2L1ImageFileReader
from orchestrator.plugins.sentinel2.maja_sentinel2_plugin import MajaSentinel2Plugin
from orchestrator.common.dem.dem_base import DEMBase
from orchestrator.cots.otb.otb_file_utils import otb_copy_image_to_file
import orchestrator.common.gipp_utils as gipp_utils


def test(argv):
    in_dir = argv[1]
    dem_filename = argv[2]
    working_dir = argv[3]

    l_ListOfTOAImageFileNames = [os.path.join(in_dir, 'T36JTT_20160914T074612_B01.jp2'),
                                 os.path.join(in_dir, 'T36JTT_20160914T074612_B02.jp2'),
                                 os.path.join(in_dir, 'T36JTT_20160914T074612_B03.jp2'),
                                 os.path.join(in_dir, 'T36JTT_20160914T074612_B04.jp2'),
                                 os.path.join(in_dir, 'T36JTT_20160914T074612_B05.jp2'),
                                 os.path.join(in_dir, 'T36JTT_20160914T074612_B06.jp2'),
                                 os.path.join(in_dir, 'T36JTT_20160914T074612_B07.jp2'),
                                 os.path.join(in_dir, 'T36JTT_20160914T074612_B08.jp2'),
                                 os.path.join(in_dir, 'T36JTT_20160914T074612_B8A.jp2'),
                                 os.path.join(in_dir, 'T36JTT_20160914T074612_B09.jp2'),
                                 os.path.join(in_dir, 'T36JTT_20160914T074612_B10.jp2'),
                                 os.path.join(in_dir, 'T36JTT_20160914T074612_B11.jp2'),
                                 os.path.join(in_dir, 'T36JTT_20160914T074612_B12.jp2')
                                 ]

    maja_object = Sentinel2L1ImageFileReader()
    maja_object._toa_scalar_list = l_ListOfTOAImageFileNames
    maja_object._plugin = MajaSentinel2Plugin()


    dem = DEMBase()
    dem.initialize(dem_filename, working_dir, False)

    maja_object._dem = dem
    maja_object.generate_edg_images_from_toa(l_ListOfTOAImageFileNames, working_dir)
    otb_copy_image_to_file(maja_object._l2edgmasklist[0], os.path.join(working_dir, "IPEDGMaskL2_R1.tif"))
    otb_copy_image_to_file(maja_object._l2edgmasklist[1], os.path.join(working_dir, "IPEDGMaskL2_R2.tif"))
    otb_copy_image_to_file(maja_object._edgsubmask, os.path.join(working_dir, "MaskOrMask12.tif"))
