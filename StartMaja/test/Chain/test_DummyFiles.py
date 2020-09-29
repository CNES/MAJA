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
from StartMaja.Chain import DummyFiles
import os


class TestDummyFiles(unittest.TestCase):
    root = os.path.join(os.getcwd(), "DummyFiles")
    platforms = ["sentinel2", "landsat8", "venus"]

    @classmethod
    def setUpClass(cls):
        from StartMaja.Common import FileSystem
        FileSystem.create_directory(cls.root)

    @classmethod
    def tearDownClass(cls):
        from StartMaja.Common import FileSystem
        FileSystem.remove_directory(cls.root)

    def test_s2_l1_generation(self):
        import shutil
        gen = DummyFiles.L1Generator(self.root, platform="sentinel2")
        prod = gen.generate()
        self.assertTrue(os.path.exists(gen.mtd))
        self.assertTrue(os.path.exists(gen.prod))
        self.assertTrue("MTD_MSIL1C.xml" in os.path.basename(gen.mtd))
        self.assertTrue("MSIL1C" in gen.prod)
        self.assertEqual(prod, prod)
        shutil.rmtree(gen.prod)
        self.assertFalse(os.path.exists(gen.prod))

    def test_l2_generation(self):
        import shutil
        gen = DummyFiles.L2Generator(self.root)
        prod = gen.generate()
        self.assertTrue(os.path.exists(gen.mtd))
        self.assertTrue(os.path.exists(gen.prod))
        self.assertTrue("_MTD_ALL.xml" in os.path.basename(gen.mtd))
        self.assertTrue(os.path.dirname(gen.mtd), gen.prod)
        self.assertEqual(prod, prod)
        shutil.rmtree(gen.prod)
        self.assertFalse(os.path.exists(gen.prod))


if __name__ == '__main__':
    unittest.main()
