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

from orchestrator.plugins.common.factory.plugin_activate import register_all
import argparse, sys

import orchestrator.plugins.common.factory.product_utils as product_utils

argParser = argparse.ArgumentParser()
required_arguments = argParser.add_argument_group('required arguments')
required_arguments.add_argument('-i', '--input_dir', required=True,
                                    help='Path to input folder')
required_arguments.add_argument('-t', '--tile_id', required=False,
                                    help='Tile id of the L1 product')
args = argParser.parse_args(sys.argv[1:])

register_all()
product_utils.get_input_l1_image_product(args.input_dir, args.tile_id)

