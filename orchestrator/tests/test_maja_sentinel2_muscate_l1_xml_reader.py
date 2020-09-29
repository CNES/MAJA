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

import os
import json
import datetime
from orchestrator.plugins.common.factory.plugin_activate import register_all
from orchestrator.plugins.sentinel2_muscate.maja_sentinel2_muscate_l1_image_info import MajaSentinel2MuscateL1ImageInformations


def test(argv):
    """
    Test the reading of metadata venus muscate
    """
    register_all()

    maja_object = MajaSentinel2MuscateL1ImageInformations()

    maja_object.initialize(argv[1])
    paramsDict = {'Satellite': maja_object.Satellite,
              'PluginName' : maja_object.PluginName,
              'ProductFileN'
              'ame': os.path.basename(maja_object.ProductFileName),
              'FileCategory': maja_object.FileCategory,
              'LevelType': maja_object.LevelType,
              'FileCategory': maja_object.FileCategory,
              'Prefix': maja_object.Prefix,
              'Site': maja_object.Site,
              'ProductDateStr': maja_object.ProductDateStr,
              'ProductId': maja_object.ProductId,
              'ProductDate': datetime.datetime.strftime(maja_object.ProductDate, "%Y-%m-%d %H:%M:%S"),
              'ReferenceSiteDefinitionId': maja_object.ReferenceSiteDefinitionId,
              'HeaderFilename': os.path.basename(maja_object.HeaderFilename),
              'VIEImageFileName': maja_object.VIEImageFileName,
              'CenterCorner.latitude': maja_object.CenterCorner.latitude,
              'CenterCorner.line': maja_object.CenterCorner.line,
              'CenterCorner.column': maja_object.CenterCorner.column,
              'UTCValidityStart': maja_object.UTCValidityStart,
              'ListOfViewingZenithAnglesPerBandAtL2CoarseResolution': maja_object.ListOfViewingZenithAnglesPerBandAtL2CoarseResolution,
              'ListOfViewingAzimuthAnglesPerBandAtL2CoarseResolution': maja_object.ListOfViewingAzimuthAnglesPerBandAtL2CoarseResolution,
              'ListOfViewingAnglesPerBandAtL2CoarseResolution': maja_object.ListOfViewingAnglesPerBandAtL2CoarseResolution,
              'SolarAngle': maja_object.SolarAngle,
              'L1NoData': maja_object.L1NoData,
              'ReflectanceQuantification': maja_object.ReflectanceQuantification,
              'RealL1NoData': maja_object.RealL1NoData
              }


    if not os.path.exists(os.path.join(argv[2], "Output")):
        try:
            os.mkdir(os.path.join(argv[2], "Output"))
        except FileExistsError:
            pass

    with open(os.path.join(argv[2], 'Sentinel2Muscate_mtd_l1_reader_file.json'), 'w') as file:
        file.write(json.dumps(paramsDict, indent=4))


