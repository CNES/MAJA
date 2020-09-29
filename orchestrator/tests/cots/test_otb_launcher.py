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

orchestrator.tests.cots.test_otb_band_math -- shortdesc

orchestrator.tests.cots.test_otb_band_math is a description

It defines classes_and_methods

###################################################################################################
"""

import os
import unittest

from orchestrator.cots.otb.otb_app_handler import OtbCotsGeneric
from orchestrator.common.directory_manager import get_working_file
from ..settings import QB_1_ORTHO_EXTRACT
from ..maja_test_case import MajaTestCase


class TestOtbCotsLauncher(MajaTestCase):
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
        rok_otb_cots = OtbCotsGeneric()
        self.assertIsNotNone(rok_otb_cots)

    def test_otb_band_math_run(self):
        """
        Test the initialization of the object
        """
        rok_otb_cots = OtbCotsGeneric()
        rok_otb_cots.pre(self.otb_app, self.parameters)

        rok_otb_cots.run()

    def test_otb_output_role_read_image_info(self):
        """
        Test the initialization of the object
        """
        rok_otb_cots = OtbCotsGeneric()
        rok_otb_cots.pre("ReadImageInfo", {"in": QB_1_ORTHO_EXTRACT.get("image_fullpath", None)})

        rok_otb_cots.run()

        rok_otb_cots.post()

        outputs = rok_otb_cots.outputs

        self.assertIsNotNone(outputs)
        self.assertEqual(outputs.get("sensor"), "")
        self.assertEqual(outputs.get("sizex"), 256)
        self.assertEqual(outputs.get("numberbands"), 4)

    def test_otb_output_type(self):
        """
        Test the initialization of the object
        """
        rok_otb_cots = OtbCotsGeneric()

        output = os.path.join(self.working_test_directory, "output_band_math.tif:uint8")
        rok_otb_cots.pre("BandMath", {"il": [QB_1_ORTHO_EXTRACT.get("image_fullpath", None)],
                                      "exp": "im1b1",
                                      "out": output})

        rok_otb_cots.run()

        rok_otb_cots.post(True)

        outputs = rok_otb_cots.outputs


if __name__ == "__main__":
    unittest.main()
