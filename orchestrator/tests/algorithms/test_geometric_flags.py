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

orchestrator.tests.algorithms.test_geometric_flags -- shortdesc

orchestrator.tests.algorithms.test_geometric_flags is a description

It defines classes_and_methods

###################################################################################################
"""


import unittest
from orchestrator.tests.maja_test_case import MajaTestCase
from orchestrator.algorithms.geometric_flags.geometric_flags import GeometricFlagsGenerator


class TestGeometricFlagsGenerator(MajaTestCase):
    """Test class for ClassUndertestName"""

    def test_object(self):
        maja_object = GeometricFlagsGenerator(1, [1], 1, [1], 0.5, 0.5)
        self.assertIsNotNone(maja_object)

    def test_sunglint_distance(self):
        maja_object = GeometricFlagsGenerator(1, [1], 1, [1], 0.5, 0.5)
        self.assertIsNotNone(maja_object.sunglint_distance)

    def test_hot_spot_distance(self):
        maja_object = GeometricFlagsGenerator(1, [1], 1, [1], 0.5, 0.5)
        self.assertIsNotNone(maja_object.hot_spot_distance)

    def test_is_sunglint(self):
        maja_object = GeometricFlagsGenerator(1, [1], 1, [1], 0.5, 0.5)
        self.assertIsNotNone(maja_object.is_sunglint())

    def test_is_hot_spot(self):
        maja_object = GeometricFlagsGenerator(1, [1], 1, [1], 0.5, 0.5)
        self.assertIsNotNone(maja_object.is_hotspot())


if __name__ == "__main__":
    unittest.main()
