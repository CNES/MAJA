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

orchestrator.tests.plugins.test_maja_sentinel2_l1_main_xml_reader -- shortdesc

orchestrator.tests.plugins.test_maja_sentinel2_l1_main_xml_reader is a description

It defines classes_and_methods


###################################################################################################
"""


import os
import unittest

from orchestrator.tests.settings import A_PRODUCT_S2_L1
from orchestrator.plugins.sentinel2.maja_sentinel2_l1_main_xml_reader import MajaSentinel2L1MainXmlReader


class TestMajaSentinel2MainXmlReader(unittest.TestCase):
    """Test class for ClassUndertestName"""

    def setUp(self):
        self.input_product_mtd = A_PRODUCT_S2_L1.get("main_xml_fullpath")

    def test_object(self):
        """
        Assert the initialization of the object do not fail
        """
        self.maja_oject = MajaSentinel2L1MainXmlReader(self.input_product_mtd)
        self.assertIsNotNone(self.maja_oject)

    def test_granule(self):
        """
        Assertion on the extracted granule name
        TODO: create a general test for a general function extract_info which should return a {}
        """
        self.maja_oject = MajaSentinel2L1MainXmlReader(self.input_product_mtd)
        self.assertEqual(self.maja_oject.granule_id, A_PRODUCT_S2_L1.get("granule_id"))

        self.assertEqual(self.maja_oject.granule_id, A_PRODUCT_S2_L1.get("granule_id"))

    def test_granule_xml_file_getter(self):
        """
        Test of the method granule_xml_file
        """

        self.maja_oject = MajaSentinel2L1MainXmlReader(self.input_product_mtd)

        self.assertEqual(os.path.basename(self.maja_oject.granule_xml_file),
                         A_PRODUCT_S2_L1.get("granule_xml_basename"))


if __name__ == "__main__":
    unittest.main()
