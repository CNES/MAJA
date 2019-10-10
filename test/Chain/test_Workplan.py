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
from Common import FileSystem, DummyFiles
from Chain.Workplan import Workplan, Init, Nominal, Backward
import os


class TestWorkplan(unittest.TestCase):

    def setUp(self):
        self.wdir = os.path.join(os.getcwd(), "wp_wdir")
        self.outdir = os.path.join(os.getcwd(), "wp_outdir")
        self.l1 = DummyFiles.L1Generator(root=os.getcwd(), tile="T11ABC", platform="sentinel2").generate()
        self.l1_list = [DummyFiles.L1Generator(root=os.getcwd(), tile="T11ABC", platform="sentinel2").generate()
                        for _ in range(3)]
        self.other_l1 = [DummyFiles.L1Generator(root=os.getcwd()).generate()
                         for _ in range(3)]

        self.l2 = DummyFiles.L2Generator(root=self.outdir, tile="T11ABC", platform="sentinel2").generate()
        self.other_l2 = [DummyFiles.L2Generator(root=self.outdir).generate() for _ in range(3)]
        self.gipp_dir = os.path.join(os.getcwd(), "gipp_dir")
        FileSystem.create_directory(self.gipp_dir)
        self.gipp = DummyFiles.GippGenerator(root=self.gipp_dir, platform="sentinel2").generate()
        self.cams = DummyFiles.CAMSGenerator(root=os.getcwd(), date=self.l1.date, platform="sentinel2").generate()
        FileSystem.create_directory(self.wdir)
        FileSystem.create_directory(self.outdir)

    def tearDown(self):
        FileSystem.remove_directory(self.wdir)
        FileSystem.remove_directory(self.outdir)
        FileSystem.remove_directory(self.l1.fpath)
        FileSystem.remove_directory(self.l2.fpath)
        for p in self.l1_list:
            FileSystem.remove_directory(p.fpath)
        for p in self.other_l1:
            FileSystem.remove_directory(p.fpath)
        for p in self.other_l2:
            FileSystem.remove_directory(p.fpath)
        FileSystem.remove_directory(self.cams.dbl)
        FileSystem.remove_file(self.cams.hdr)
        FileSystem.remove_directory(self.gipp_dir)

    def test_name_hash(self):
        product_name = "S2B_MSIL1C_20400602T121200_N0044_R001_T31TCH_20400602T121200.SAFE"
        self.assertEqual(Workplan.get_dirname(product_name), "f9bfae79e5aa4a59feda03c8d6717f35")

    def test_wp_init_nofolders(self):
        with self.assertRaises(AssertionError):
            Init("abc", "def", l1=self.l1)
        with self.assertRaises(AssertionError):
            Backward("abc", "def", l1=self.l1, l1_list=[self.l1])
        with self.assertRaises(AssertionError):
            Nominal("abc", "def", l1=self.l1, l2=self.l2, l2_date="abc")

    def test_wp_init(self):
        wp = Init(self.wdir, self.outdir, l1=self.l1)
        self.assertEqual(wp.log_level, "INFO")
        self.assertEqual(wp.aux_files, [])
        self.assertTrue(os.path.isdir(self.wdir))
        self.assertTrue(os.path.isdir(self.outdir))
        self.assertEqual(wp.l1, self.l1)

    def test_wp_backward(self):
        wp = Backward(self.wdir, self.outdir, l1=self.l1, l1_list=self.l1_list)
        self.assertEqual(wp.log_level, "INFO")
        self.assertEqual(wp.aux_files, [])
        self.assertTrue(os.path.isdir(self.wdir))
        self.assertTrue(os.path.isdir(self.outdir))
        self.assertEqual(wp.l1, self.l1)
        self.assertTrue(wp.l1_list, self.l1_list)

    def test_wp_nominal(self):
        wp = Nominal(self.wdir, self.outdir, l1=self.l1, l2_date=self.l2.date, log_level="DEBUG")
        self.assertEqual(wp.log_level, "DEBUG")
        self.assertEqual(wp.aux_files, [])
        self.assertTrue(os.path.isdir(self.wdir))
        self.assertTrue(os.path.isdir(self.outdir))
        self.assertEqual(wp.l1, self.l1)
        self.assertEqual(wp.l2_date, self.l2.date)
        l2_prods = wp._get_available_l2_products()
        # TODO Cannot check more because of misaligned dates and validity check. Need to add JPI to dummy-files
        self.assertEqual([], l2_prods)


if __name__ == '__main__':
    unittest.main()
