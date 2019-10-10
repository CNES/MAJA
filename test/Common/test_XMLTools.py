#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Copyright (C) CNES, CS-SI, CESBIO - All Rights Reserved
This file is subject to the terms and conditions defined in
file 'LICENSE.md', which is part of this source code package.

Author:         Peter KETTIG <peter.kettig@cnes.fr>,
Project:        Start-MAJA, CNES
"""

import unittest
import os
from Common import XMLTools


class TestXMLTools(unittest.TestCase):

    root = os.getcwd()

    def test_write_and_get_xpath(self):
        from xml.etree import ElementTree
        from Common import FileSystem
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
