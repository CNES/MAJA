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

orchestrator.tests.cots.test_gdal_rasterize -- shortdesc

orchestrator.tests.cots.test_gdal_rasterize is a description

It defines classes_and_methods


###################################################################################################
"""

import os
import unittest
from orchestrator.cots.gdal.gdal_rasterize import (GdalRasterize,
                                                   rasterize_det_foo,
                                                   rasterize_no_data)
from orchestrator.tests.settings import QB_1_ORTHO_EXTRACT
from orchestrator.tests.maja_test_case import MajaTestCase


class TestGdalRasterize(MajaTestCase):
    """Test class for ClassUndertestName"""

    def setUp(self):
        self.input_vector = QB_1_ORTHO_EXTRACT.get("extent_fullpath")

    def test_object(self):
        """
        Test of the initialization of the object
        """
        maja_object = GdalRasterize(self.working_test_directory)
        self.assertIsNotNone(maja_object)

    def test_pre(self):
        """
        test of the set parameters
        :return:
        """
        maja_object = GdalRasterize(self.working_test_directory)
        maja_object.pre(self.input_vector)
        self.assertIsNotNone(maja_object)

    # @unittest.skip
    def test_run(self):
        """
        Test the run (should not be run as TU)
        :return:
        """
        maja_object = GdalRasterize(self.working_test_directory)
        maja_object.pre(self.input_vector)
        maja_object.run()
        self.assertIsNotNone(maja_object)

    def test_rasterize_det_foo(self):
        """

        :return:
        """
        output_image = rasterize_det_foo(self.input_vector, self.working_test_directory)
        self.assertTrue(os.path.exists(output_image))

    def test_rasterize_nodata(self):
        """

        :return:
        """
        output_image = rasterize_no_data(self.input_vector, self.working_test_directory)
        self.assertTrue(os.path.exists(output_image))


if __name__ == "__main__":
    unittest.main()
