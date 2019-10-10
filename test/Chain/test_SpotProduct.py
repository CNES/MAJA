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
from Common import TestFunctions, FileSystem
from Chain.Product import MajaProduct
from Chain.SpotProduct import Spot4Muscate, Spot5Muscate
import os


class TestSpotProduct(unittest.TestCase):

    prod_s4_mus = ["SPOT4-HRG2-XS_20120622-083239-738_L1C_115-354-0_D_V1-0",
                   "SPOT4-HRG2-XS_20120622-083231-220_L1C_115-353-0_D_V1-0"]
    prod_s5_mus = ["SPOT5-HRG2-XS_20120626-103330-813_L1C_048-326-0_D_V1-0",
                   "SPOT5-HRG2-XS_20120617-082907-795_L1C_123-357-0_D_V1-0"]
    prods_other = ["LANDSAT8-OLITIRS-XSTHPAN_20170501-103532-111_L1C_T31TCH_C_V1-0",
                   "LANDSAT8_20170501-103532-111_L2A_T31TCH_C_V1-0",
                   "LC80390222013076EDC00",
                   "LC08_L1TP_199029_20170527_20170615_01_T1",
                   "L8_TEST_L8C_L2VALD_198030_20130626.DBL.DIR",
                   "S2A_MSIL1C_20170412T110621_N0204_R137_T29RPQ_20170412T111708.SAFE",
                   "S2B_MSIL1C_20180316T103021_N0206_R108_T32TMR_20180316T123927.SAFE",
                   "S2A_OPER_PRD_MSIL1C_PDMC_20161109T171237_R135_V20160924T074932_20160924T081448.SAFE",
                   "S2A_OPER_SSC_L2VALD_36JTT____20160914.DBL.DIR",
                   "S2B_OPER_SSC_L1VALD_21MXT____20180925.DBL.DIR",
                   "SENTINEL2B_20171008-105012-463_L1C_T31TCH_C_V1-0",
                   "SENTINEL2A_20161206-105012-463_L2A_T31TCH_C_V1-0",
                   "SENTINEL2X_20190415-000000-000_L3A_T31UFR_C_V1-1",
                   "VENUS-XS_20180201-051359-000_L1C_KHUMBU_C_V1-0",
                   "VENUS_20180201-051359-000_L2A_KHUMBU_C_V1-0",
                   "VENUS-XS_20180201-051359-000_L3A_KHUMBU_C_V1-0",
                   "VE_VM01_VSC_L2VALD_ISRAW906_20180317.DBL.DIR",
                   "VE_OPER_VSC_L1VALD_UNH_20180329.DBL.DIR"]

    @classmethod
    def setUpClass(cls):
        """
        Simulate the basic folder + metadata_file structure
        :return:
        """
        import os
        for root in cls.prod_s4_mus:
            os.makedirs(root)
            metadata = os.path.join(root, root + "_MTD_ALL.xml")
            TestFunctions.touch(metadata)
        for root in cls.prod_s5_mus:
            os.makedirs(root)
            metadata = os.path.join(root, root + "_MTD_ALL.xml")
            TestFunctions.touch(metadata)

    @classmethod
    def tearDownClass(cls):
        import shutil
        for root in cls.prod_s4_mus + cls.prod_s5_mus:
            shutil.rmtree(root)

    def test_reg_spot4_muscate(self):
        tiles = ["115-354-0", "115-353-0"]
        levels = ["l1c"]
        dates = ["20120622T083239", "20120622T083231"]
        for prod, tile, date, level in zip(self.prod_s4_mus, tiles, dates, levels):
            p = MajaProduct(prod).factory()
            self.assertIsInstance(p, Spot4Muscate)
            self.assertEqual(p.level, level)
            self.assertEqual(p.platform, "spot4")
            self.assertEqual(p.type, "muscate")
            self.assertEqual(p.tile, tile)
            self.assertEqual(p.date.strftime("%Y%m%dT%H%M%S"), date)
            self.assertTrue(os.path.basename(p.metadata_file).endswith("_MTD_ALL.xml"))
            self.assertTrue(os.path.exists(p.metadata_file))
            self.assertEqual(p.validity, True)
            link_dir = "linkdir"
            FileSystem.create_directory(link_dir)
            p.link(link_dir)
            self.assertTrue(os.path.islink(os.path.join(link_dir, p.base)))
            self.assertEqual(p.mnt_resolutions_dict, [{'name': 'XS', 'val': '15 -15'}])
            self.assertEqual(p, p)
            FileSystem.remove_directory(link_dir)

        # Other prods:
        for prod in self.prod_s5_mus + self.prods_other:
            p = MajaProduct(prod).factory()
            self.assertNotIsInstance(p, Spot4Muscate)

    def test_reg_spot5_muscate(self):
        tiles = ["048-326-0", "123-357-0"]
        dates = ["20120626T103330", "20120617T082907"]
        for prod, tile, date in zip(self.prod_s5_mus, tiles, dates):
            p = MajaProduct(prod).factory()
            self.assertIsInstance(p, Spot5Muscate)
            self.assertEqual(p.level, "l1c")
            self.assertEqual(p.platform, "spot5")
            self.assertEqual(p.type, "muscate")
            self.assertEqual(p.tile, tile)
            self.assertEqual(p.date.strftime("%Y%m%dT%H%M%S"), date)
            self.assertTrue(os.path.basename(p.metadata_file).endswith("_MTD_ALL.xml"))
            self.assertTrue(os.path.exists(p.metadata_file))
            self.assertEqual(p.validity, True)
            link_dir = "linkdir"
            FileSystem.create_directory(link_dir)
            p.link(link_dir)
            self.assertTrue(os.path.islink(os.path.join(link_dir, p.base)))
            self.assertEqual(p.mnt_resolutions_dict, [{'name': 'XS', 'val': '15 -15'}])
            self.assertEqual(p, p)
            FileSystem.remove_directory(link_dir)

        # Other prods:
        for prod in self.prod_s4_mus + self.prods_other:
            p = MajaProduct(prod).factory()
            self.assertNotIsInstance(p, Spot5Muscate)


if __name__ == '__main__':
    unittest.main()
