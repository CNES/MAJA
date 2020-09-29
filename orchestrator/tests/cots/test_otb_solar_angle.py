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

orchestrator.tests.cots.test_otb_solar_angle -- shortdesc

orchestrator.tests.cots.test_otb_solar_angle is a description

It defines classes_and_methods

###################################################################################################
"""


import os
import unittest

from orchestrator.cots.otb.algorithms.otb_solar_angles import solar_angle
from orchestrator.tests.settings import QB_1_ORTHO_EXTRACT
from orchestrator.tests.maja_test_case import MajaTestCase


class TestOtbCotsSolarAngle(MajaTestCase):

    def setUp(self):
        self.input_angles = QB_1_ORTHO_EXTRACT.get("image_fullpath", None)
        self.sol1_height = 2
        self.sol_href = 1

    def test_otb_angle_list_to_image_run(self):
        """
        Test the initialization of the object
        """
        expected_output = os.path.join(self.working_test_directory,
                                       "output_angle_list_to_image.tif")

        rok_otb_cots = solar_angle(
            self.input_angles,
            sol1_height=self.sol1_height,
            sol_href=self.sol_href,
            output_image=expected_output)

        self.assertIsNotNone(rok_otb_cots)
        self.assertTrue(os.path.exists(expected_output))


if __name__ == "__main__":
    unittest.main()
