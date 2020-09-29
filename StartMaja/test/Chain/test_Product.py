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
from StartMaja.Common import TestFunctions
from StartMaja.Chain.Product import MajaProduct
import os
from os import path as p


class TestProduct(unittest.TestCase):

    root = "S2A_MSIL1C_20170412T110621_N0204_R137_T29RPQ_20170412T111708.SAFE"

    def setUp(self):
        os.makedirs(self.root)
        TestFunctions.touch(os.path.join(self.root, "MTD_MSIL1C.xml"))

    def tearDown(self):
        import shutil
        shutil.rmtree(self.root)

    def test_get_file_depth1(self):
        product = MajaProduct.factory(self.root)
        expected = os.path.join(os.path.abspath(self.root), "MTD_MSIL1C.xml")
        dirnames_e = p.normpath(expected).split(os.sep)
        calculated = product.find_file(path=self.root, pattern="^MTD_MSIL1C.xml$", depth=1)
        self.assertEqual(len(calculated), 1)
        dirnames_c = p.normpath(calculated[0]).split(os.sep)
        for exp, calc in zip(dirnames_c[-1:], dirnames_e[-1:]):
            self.assertEqual(exp[:-1], calc[:-1])
        self.assertEqual(expected, calculated[0])

    def test_eq(self):
        product = MajaProduct.factory(self.root)
        self.assertEqual(product, product)


if __name__ == '__main__':
    unittest.main()
