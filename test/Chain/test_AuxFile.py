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
from Chain.AuxFile import CAMSFile, DTMFile
import os
from datetime import datetime


class TestAuxFile(unittest.TestCase):
    root = os.getcwd()

    cams_dir = os.path.join(root, "CAMS")
    n_cams = 3
    tiles = ["T31TCH", "T12SQE", "SO2", "12949"]
    mnt = []

    @classmethod
    def setUpClass(cls):
        """
        Sets up a random tree-like structure with a few sub-files and -folders
        Similar to test_FileSystem.
        :return:
        """
        from Common import DummyFiles
        os.makedirs(cls.cams_dir)
        for i in range(cls.n_cams):
            DummyFiles.CAMSGenerator(cls.cams_dir).generate()

        for tile in cls.tiles:
            cls.mnt += [DummyFiles.MNTGenerator(cls.root, tile=tile).generate()]

    @classmethod
    def tearDownClass(cls):
        import shutil
        shutil.rmtree(cls.cams_dir)
        for m in cls.mnt:
            shutil.rmtree(m.dbl)
            os.remove(m.hdr)

    def test_cams_reg(self):
        import re
        cams = ["S2__PROD_EXO_CAMS_20171008T030000_20180628T174612.DBL",
                "VE_TEST_EXO_CAMS_20171008T150000_20180628T174630.DBL.DIR",
                "L8_TEST_EXO_CAMS_20171010T030000_20180628T174724.DBL"]
        for f in cams:
            self.assertTrue(re.search(CAMSFile.regex, f))

    def test_dtm_reg(self):
        import re
        dtms = ["S2__TEST_AUX_REFDE2_T29RPQ_0001",
                "NONAME_TEST_AUX_REFDE2_TKHUMBU_0001"]
        for dtm in dtms:
            self.assertTrue(re.search(DTMFile.regex, dtm))

    def test_cams_creation(self):
        from Common import FileSystem

        dbl = FileSystem.find("DBL.DIR", self.cams_dir)[0]
        c = CAMSFile(dbl)
        self.assertIsNotNone(c)
        base = os.path.basename(dbl).split(".")[0]

        hdr = os.path.join(os.path.dirname(dbl), base + ".HDR")
        self.assertEqual(hdr, c.hdr)

    def test_wrong_cams_creation(self):
        from Common import FileSystem
        dbl = FileSystem.find("*AUX_REFDE2*DBL.DIR", self.root)[0]
        self.assertIsNone(CAMSFile(dbl))

    def test_cams_date(self):
        from Common import FileSystem

        dbl = FileSystem.find("*EXO_CAMS*DBL.DIR", self.cams_dir)[0]
        c = CAMSFile(dbl)
        base = os.path.basename(dbl).split(".")[0]
        date = datetime.strptime(base.split("_")[-2], "%Y%m%dT%H%M%S")
        self.assertEqual(c.get_date(), date)

    def test_mnt_creation(self):
        for m in self.mnt:
            self.assertIsNotNone(m)
            base = os.path.basename(m.dbl).split(".")[0]
            hdr = os.path.join(os.path.dirname(m.dbl), base + ".HDR")
            self.assertEqual(hdr, m.hdr)

    def test_wrong_mnt_creation(self):
        from Common import FileSystem
        dbl = FileSystem.find("DBL.DIR", self.cams_dir)[0]
        self.assertIsNone(DTMFile(dbl))


if __name__ == '__main__':
    unittest.main()
