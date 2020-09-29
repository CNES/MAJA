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

orchestrator.tests.cots.test_otb_cots_manager -- shortdesc

orchestrator.tests.cots.test_otb_cots_manager is a description

It defines classes_and_methods


###################################################################################################
"""

import unittest

from orchestrator.cots.otb.algorithms.otb_band_math import band_math
from orchestrator.cots.otb.otb_pipeline_manager import OtbPipelineManager
from orchestrator.common.directory_manager import get_working_file
from orchestrator.tests.settings import QB_1_ORTHO_EXTRACT
from orchestrator.tests.maja_test_case import MajaTestCase


class TestOtbCotsManager(MajaTestCase):
    """Test class for ClassUndertestName"""

    def setUp(self):
        self.input_image_list = [QB_1_ORTHO_EXTRACT.get("image_fullpath", None)]
        self.expression = 'im1b1*im1b1'
        self.type_output = 'uint16'
        self.parameters = {"il": self.input_image_list,
                           "exp": self.expression,
                           "out": get_working_file("TestBandMath", working_directory=self.working_test_directory)}
        self.otb_app = "BandMath"

    def test_object(self):
        """
        Test the initialization of the object
        """
        rok_otb_cots = OtbPipelineManager()
        self.assertIsNotNone(rok_otb_cots)

    def test_otb_chain_memory(self):
        """
        Test the initialization of the object
        """
        l_pipeline = OtbPipelineManager()

        out_memory_1 = band_math([QB_1_ORTHO_EXTRACT.get("image_fullpath", None)], "im1b1",
                                 self.working_test_directory, write_output=False)

        l_pipeline.add_otb_app(out_memory_1)
        self.assertEqual(len(l_pipeline.otb_app_coarse_resolution), 1)

        out_memory_2 = band_math([out_memory_1.getoutput().get("out")], "im1b1",
                                 self.working_test_directory, write_output=False)
        l_pipeline.add_otb_app(out_memory_2)
        self.assertEqual(len(l_pipeline.otb_app_coarse_resolution), 2)
        out_memory_3 = band_math([out_memory_2.getoutput().get("out")], "im1b1",
                                 self.working_test_directory, write_output=True)

        l_pipeline.free_otb_app()
        self.assertEqual(len(l_pipeline.otb_app_coarse_resolution), 0)


if __name__ == "__main__":
    unittest.main()
