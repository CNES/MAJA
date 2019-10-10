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
from Common import TestFunctions
from Chain.Product import MajaProduct
import os
from os import path as p


class TestProduct(unittest.TestCase):
    root = "S2A_MSIL1C_20170412T110621_N0204_R137_T29RPQ_20170412T111708.SAFE"

    subdir_prefix = "subdir"
    file_a1 = "a"
    file_a2 = "a.jpg"
    file_b1 = "b.jpg"
    file_c1 = "c.xml"

    def setUp(self):
        """
        Sets up a random tree-like structure with a few sub-files and -folders
        Similar to test_FileSystem.
        :return:
        """
        os.makedirs(self.root)
        TestFunctions.touch(p.join(self.root, self.file_a1))
        TestFunctions.touch(p.join(self.root, self.file_a2))
        TestFunctions.touch(p.join(self.root, self.file_b1))
        TestFunctions.touch(p.join(self.root, self.file_c1))
        for i in range(2):
            subdir = p.join(self.root, self.subdir_prefix + str(i))
            os.makedirs(subdir)
            TestFunctions.touch(p.join(subdir, self.file_a1))
            TestFunctions.touch(p.join(subdir, self.file_a2))
            TestFunctions.touch(p.join(subdir, self.file_b1))
            TestFunctions.touch(p.join(subdir, self.file_c1))
            for j in range(2):
                ssubdir = p.join(subdir, self.subdir_prefix + str(j))
                os.makedirs(ssubdir)
                TestFunctions.touch(p.join(ssubdir, self.file_a1))
                TestFunctions.touch(p.join(ssubdir, self.file_a2))
                TestFunctions.touch(p.join(ssubdir, self.file_b1))
                TestFunctions.touch(p.join(ssubdir, self.file_c1))

    def tearDown(self):
        import shutil
        shutil.rmtree(self.root)

    def test_get_product_file_depth1(self):
        product = MajaProduct(self.root).factory()
        expected = "S2A_MSIL1C_20170412T110621_N0204_R137_T29RPQ_20170412T111708.SAFE/a"
        dirnames_e = p.normpath(expected).split(os.sep)
        filename = p.basename(expected)
        calculated = product.get_file(filename="^a$")
        dirnames_c = p.normpath(calculated).split(os.sep)
        for exp, calc in zip(dirnames_c[-1:], dirnames_e[-1:]):
            self.assertEqual(exp[:-1], calc[:-1])
        self.assertEqual(filename, p.basename(calculated))

    def test_get_product_file_depth2(self):
        product = MajaProduct(self.root).factory()
        expected = r"S2A_MSIL1C_20170412T110621_N0204_R137_T29RPQ_20170412T111708.SAFE/subdir0/a"
        dirnames_e = p.normpath(expected).split(os.sep)
        filename = p.basename(expected)
        calculated = product.get_file(folders="subdir*", filename="^a$")
        dirnames_c = p.normpath(calculated).split(os.sep)
        for exp, calc in zip(dirnames_c[-2:], dirnames_e[-2:]):
            self.assertEqual(exp[:-1], calc[:-1])
        self.assertEqual(filename, p.basename(calculated))

    def test_get_product_file_depth3(self):
        product = MajaProduct(self.root).factory()
        expected = "S2A_MSIL1C_20170412T110621_N0204_R137_T29RPQ_20170412T111708.SAFE/subdir0/subdir1/c.xml"
        dirnames_e = p.normpath(expected).split(os.sep)
        filename = p.basename(expected)
        calculated = product.get_file(folders="subdir*/subdir*", filename="*xml")
        dirnames_c = p.normpath(calculated).split(os.sep)
        for exp, calc in zip(dirnames_c[-3:], dirnames_e[-3:]):
            self.assertEqual(exp[:-1], calc[:-1])
        self.assertEqual(filename, p.basename(calculated))

    def test_get_product_file_full(self):
        product = MajaProduct(self.root).factory()
        expected = "S2A_MSIL1C_20170412T110621_N0204_R137_T29RPQ_20170412T111708.SAFE/b.jpg"
        dirnames_e = p.normpath(expected).split(os.sep)
        filename = p.basename(expected)
        calculated = product.get_file(filename="b.jpg")
        dirnames_c = p.normpath(calculated).split(os.sep)
        for exp, calc in zip(dirnames_c[-1:], dirnames_e[-1:]):
            self.assertEqual(exp[:-1], calc[:-1])
        self.assertEqual(filename, p.basename(calculated))

    def test_get_product_file_from_folder_above(self):
        product = MajaProduct(self.root).factory()
        expected = "S2A_MSIL1C_20170412T110621_N0204_R137_T29RPQ_20170412T111708.SAFE/b.jpg"
        dirnames_e = p.normpath(expected).split(os.sep)
        filename = p.basename(expected)
        calculated = product.get_file(folders="./subdir0/../", filename="b.jpg")
        dirnames_c = p.normpath(calculated).split(os.sep)
        for exp, calc in zip(dirnames_c[-1:], dirnames_e[-1:]):
            self.assertEqual(exp[:-1], calc[:-1])
        self.assertEqual(filename, p.basename(calculated))

    def test_get_product_folder(self):
        product = MajaProduct(self.root).factory()
        expected = "S2A_MSIL1C_20170412T110621_N0204_R137_T29RPQ_20170412T111708.SAFE/subdir0"
        dirnames_e = p.normpath(expected).split(os.sep)
        filename = p.basename(expected)
        calculated = product.get_file(folders="./subdir0/")
        dirnames_c = p.normpath(calculated).split(os.sep)
        for exp, calc in zip(dirnames_c[-1:], dirnames_e[-1:]):
            self.assertEqual(exp[:-1], calc[:-1])
        self.assertEqual(filename, p.basename(calculated))

    def test_get_product_sub_folder(self):
        product = MajaProduct(self.root).factory()
        expected = "S2A_MSIL1C_20170412T110621_N0204_R137_T29RPQ_20170412T111708.SAFE/subdir0"
        dirnames_e = p.normpath(expected).split(os.sep)
        filename = p.basename(expected)
        calculated = product.get_file(folders="./subdir0/../subdir*")
        dirnames_c = p.normpath(calculated).split(os.sep)
        for exp, calc in zip(dirnames_c[-2:], dirnames_e[-2:]):
            self.assertEqual(exp[:-1], calc[:-1])
        self.assertEqual(filename[:-1], p.basename(calculated[:-1]))


if __name__ == '__main__':
    unittest.main()
