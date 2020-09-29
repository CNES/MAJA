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
orchestrator.tests.plugins.test_maja_sentinel2_l1_granule_xml_reader -- shortdesc

orchestrator.tests.plugins.test_maja_sentinel2_l1_granule_xml_reader is a description

It defines classes_and_methods

###################################################################################################
"""


import os
import unittest

from orchestrator.tests.settings import A_PRODUCT_S2_L2
from orchestrator.plugins.sentinel2.maja_sentinel2_l2_main_xml_reader import MajaSentinel2L2MainXmlReader


class TestMajaSentinel2L2MainXmlReader(unittest.TestCase):
    """Test class for ClassUndertestName"""

    def setUp(self):
        self.main_xml = A_PRODUCT_S2_L2.get("main_xml_fullpath")
        self.assertTrue(os.path.isfile(self.main_xml))

    def test_object(self):
        """
        Assert the initialization of the object do not fail
        """
        self.maja_oject = MajaSentinel2L2MainXmlReader(self.main_xml)
        self.assertIsNotNone(self.maja_oject)


if __name__ == "__main__":
    unittest.main()
