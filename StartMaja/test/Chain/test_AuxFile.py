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
from StartMaja.Chain.AuxFile import CAMSFile, DTMFile
import os
from StartMaja.Chain import DummyFiles
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
        from StartMaja.Common import FileSystem

        dbl = FileSystem.find("DBL.DIR", self.cams_dir)[0]
        c = CAMSFile(dbl)
        self.assertIsNotNone(c)
        base = os.path.basename(dbl).split(".")[0]

        hdr = os.path.join(os.path.dirname(dbl), base + ".HDR")
        self.assertEqual(hdr, c.hdr)

    def test_wrong_cams_creation(self):
        from StartMaja.Common import FileSystem
        dbl = FileSystem.find("*AUX_REFDE2*DBL.DIR", self.root)[0]
        self.assertIsNone(CAMSFile(dbl))

    def test_cams_date(self):
        from StartMaja.Common import FileSystem

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
        from StartMaja.Common import FileSystem
        dbl = FileSystem.find("DBL.DIR", self.cams_dir)[0]
        self.assertIsNone(DTMFile(dbl))


if __name__ == '__main__':
    unittest.main()
