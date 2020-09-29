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

orchestrator.tests.common.interfaces.test_maja_xml_input_angles -- shortdesc

orchestrator.tests.common.interfaces.test_maja_xml_input_angles is a description

It defines classes_and_methods

###################################################################################################
"""

import unittest

from orchestrator.common.interfaces.maja_xml_input_angles import MajaInternalXmlInputAngles


class TestMajaInternalXmlInputAngles(unittest.TestCase):
    """Test class for MajaInternalXmlInputAngles"""

    def setUp(self):
        self.values_1 = [1, 2, 3, 4, 5]
        self.values_2 = [6, 7, 8, 9, 10]
        self.row_step = 5000
        self.col_step = 5000
        self.output_filename = "/tmp/internal_angles_test.xml"

    def test_object(self):
        """
        Test the initialisation of the object
        """
        maja_object = MajaInternalXmlInputAngles(self.values_1, self.values_2,
                                                 self.col_step, self.row_step, self.output_filename)
        self.assertIsNotNone(maja_object)

    def test_object_write(self):
        """
        Test the initialisation of the object
        """
        maja_object = MajaInternalXmlInputAngles(self.values_1, self.values_2,
                                                 self.col_step, self.row_step, self.output_filename)
        maja_object.write()


if __name__ == "__main__":
    unittest.main()
