#!/usr/bin/env python3
# -*- coding: utf-8 -*-

"""
Copyright (C) 2016-2020 Centre National d'Etudes Spatiales (CNES), CSSI, CESBIO  All Rights Reserved

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

   http://www.apache.org/licenses/LICENSE2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
"""

import unittest
import os
from StartMaja.Common import XMLTools


class TestXMLTools(unittest.TestCase):

    root = os.getcwd()

    def test_write_and_get_xpath(self):
        from xml.etree import ElementTree
        from StartMaja.Common import FileSystem
        file_path = os.path.join(self.root, "dummy_xml.xml")
        mission_expected = "DummyMission"
        xpath = "./Fixed_Header/Mission"
        root = ElementTree.Element("Earth_Explorer_Header")
        sub = ElementTree.SubElement(root, "Fixed_Header")
        ElementTree.SubElement(sub, "Mission").text = mission_expected
        ElementTree.SubElement(root, "Variable_Header")
        XMLTools.write_xml(root, file_path)
        self.assertTrue(os.path.exists(file_path))
        mission_calculated = XMLTools.get_xpath(file_path, xpath)[0].text
        mission_calculated_2 = XMLTools.get_single_xpath(file_path, xpath)
        self.assertEqual(mission_calculated, mission_expected)
        self.assertEqual(mission_expected, mission_calculated_2)
        FileSystem.remove_file(file_path)


if __name__ == '__main__':
    unittest.main()
