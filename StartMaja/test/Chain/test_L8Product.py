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
from StartMaja.Common import TestFunctions, FileSystem
from StartMaja.Chain.Product import MajaProduct
from StartMaja.Chain.L8Product import Landsat8Natif, Landsat8Muscate, Landsat8LC1, Landsat8LC2
import os


class TestL8Product(unittest.TestCase):

    prod_l8_mus = ["LANDSAT8-OLITIRS-XSTHPAN_20170501-103532-111_L1C_T31TCH_C_V1-0",
                   "LANDSAT8_20170501-103532-111_L2A_T31TCH_C_V1-0"]
    prod_l8_lc1 = ["LC80390222013076EDC00"]
    prod_l8_lc2 = ["LC08_L1TP_199029_20170527_20170615_01_T1"]
    prod_l8_nat = ["L8_TEST_L8C_L2VALD_198030_20130626.DBL.DIR"]
    prods_other = ["S2A_MSIL1C_20170412T110621_N0204_R137_T29RPQ_20170412T111708.SAFE",
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
        for root in cls.prod_l8_lc1 + cls.prod_l8_lc2:
            os.makedirs(root)
            metadata = os.path.join(root, root.split(".")[0] + "_MTL.txt")
            TestFunctions.touch(metadata)
        for root in cls.prod_l8_mus:
            os.makedirs(root)
            metadata = os.path.join(root, root + "_MTD_ALL.xml")
            TestFunctions.touch(metadata)
        for root in cls.prod_l8_nat:
            os.makedirs(root)
            metadata = root.split(".")[0] + ".HDR"
            TestFunctions.touch(metadata)

    @classmethod
    def tearDownClass(cls):
        import shutil
        import os
        for root in cls.prod_l8_nat:
            shutil.rmtree(root)
            metadata = root.split(".")[0] + ".HDR"
            os.remove(metadata)
        for root in cls.prod_l8_lc1 + cls.prod_l8_lc2 + cls.prod_l8_mus:
            shutil.rmtree(root)

    def test_reg_l8_muscate(self):
        tiles = ["31TCH", "31TCH"]
        levels = ["l1c", "l2a"]
        dates = ["20170501T103532", "20170501T103532"]
        validity = [True, False]
        for prod, tile, date, level, valid in zip(self.prod_l8_mus, tiles, dates, levels, validity):
            p = MajaProduct.factory(prod)
            self.assertIsInstance(p, Landsat8Muscate)
            self.assertEqual(p.level, level)
            self.assertEqual(p.nodata, 0)
            self.assertEqual(p.platform, "landsat8")
            self.assertEqual(p.type, "muscate")
            self.assertEqual(p.tile, tile)
            self.assertEqual(p.date.strftime("%Y%m%dT%H%M%S"), date)
            self.assertTrue(os.path.basename(p.metadata_file).endswith("_MTD_ALL.xml"))
            self.assertTrue(os.path.exists(p.metadata_file))
            self.assertEqual(p.validity, valid)
            link_dir = "linkdir"
            FileSystem.create_directory(link_dir)
            p.link(link_dir)
            self.assertTrue(os.path.islink(os.path.join(link_dir, p.base)))
            self.assertEqual(p.mnt_resolutions_dict, [{'name': 'XS', 'val': '30 -30'}])
            self.assertEqual(p, p)
            FileSystem.remove_directory(link_dir)

        # Other prods:
        for prod in self.prod_l8_lc1 + self.prod_l8_lc2 + self.prod_l8_nat + self.prods_other:
            p = MajaProduct.factory(prod)
            self.assertNotIsInstance(p, Landsat8Muscate)

    def test_reg_l8_natif(self):
        tiles = ["198030"]
        dates = ["20130626T120000"]
        for prod, tile, date in zip(self.prod_l8_nat, tiles, dates):
            p = MajaProduct.factory(prod)
            self.assertIsInstance(p, Landsat8Natif)
            self.assertEqual(p.level, "l1c")
            self.assertEqual(p.nodata, 0)
            self.assertEqual(p.platform, "landsat8")
            self.assertEqual(p.type, "natif")
            self.assertEqual(p.tile, tile)
            self.assertEqual(p.date.strftime("%Y%m%dT%H%M%S"), date)
            self.assertEqual(os.path.basename(p.metadata_file), prod.split(".")[0] + ".HDR")
            self.assertTrue(os.path.exists(p.metadata_file))
            self.assertEqual(p.validity, True)
            link_dir = "linkdir"
            FileSystem.create_directory(link_dir)
            p.link(link_dir)
            self.assertTrue(os.path.islink(os.path.join(link_dir, p.base)))
            self.assertEqual(p.mnt_resolutions_dict, [{'name': 'XS', 'val': '30 -30'}])
            self.assertEqual(p, p)
            FileSystem.remove_directory(link_dir)

        # Other prods:
        for prod in self.prod_l8_lc2 + self.prod_l8_lc1 + self.prod_l8_mus + self.prods_other:
            p = MajaProduct.factory(prod)
            self.assertNotIsInstance(p, Landsat8Natif)

    def test_reg_l8_lc1(self):
        tiles = ["039022"]
        dates = ["20130317T120000"]
        levels = ["l1c"]
        for prod, tile, date, level in zip(self.prod_l8_lc1, tiles, dates, levels):
            p = MajaProduct.factory(prod)
            self.assertIsInstance(p, Landsat8LC1)
            self.assertEqual(p.level, level)
            self.assertEqual(p.nodata, 0)
            self.assertEqual(p.platform, "landsat8")
            self.assertEqual(p.type, "natif")
            self.assertEqual(p.tile, tile)
            self.assertEqual(p.date.strftime("%Y%m%dT%H%M%S"), date)
            self.assertEqual(os.path.basename(p.metadata_file), prod.split(".")[0] + "_MTL.txt")
            self.assertTrue(os.path.exists(p.metadata_file))
            self.assertEqual(p.validity, True)
            link_dir = "linkdir"
            FileSystem.create_directory(link_dir)
            p.link(link_dir)
            self.assertTrue(os.path.islink(os.path.join(link_dir, p.base)))
            self.assertEqual(p.mnt_resolutions_dict, [{'name': 'XS', 'val': '30 -30'}])
            self.assertEqual(p, p)
            FileSystem.remove_directory(link_dir)

        # Other prods:
        for prod in self.prod_l8_lc2 + self.prod_l8_nat + self.prod_l8_mus + self.prods_other:
            p = MajaProduct.factory(prod)
            self.assertNotIsInstance(p, Landsat8LC1)

    def test_reg_l8_lc2(self):
        tiles = ["199029"]
        dates = ["20170527T120000"]
        levels = ["l1c"]
        for prod, tile, date, level in zip(self.prod_l8_lc2, tiles, dates, levels):
            p = MajaProduct.factory(prod)
            self.assertIsInstance(p, Landsat8LC2)
            self.assertEqual(p.level, level)
            self.assertEqual(p.platform, "landsat8")
            self.assertEqual(p.type, "natif")
            self.assertEqual(p.tile, tile)
            self.assertEqual(p.nodata, 0)
            self.assertEqual(p.date.strftime("%Y%m%dT%H%M%S"), date)
            self.assertEqual(os.path.basename(p.metadata_file), prod.split(".")[0] + "_MTL.txt")
            self.assertTrue(os.path.exists(p.metadata_file))
            self.assertEqual(p.validity, True)
            link_dir = "linkdir"
            FileSystem.create_directory(link_dir)
            p.link(link_dir)
            self.assertTrue(os.path.islink(os.path.join(link_dir, p.base)))
            self.assertEqual(p.mnt_resolutions_dict, [{'name': 'XS', 'val': '30 -30'}])
            self.assertEqual(p, p)
            FileSystem.remove_directory(link_dir)

        # Other prods:
        for prod in self.prod_l8_lc1 + self.prod_l8_nat + self.prod_l8_mus + self.prods_other:
            p = MajaProduct.factory(prod)
            self.assertNotIsInstance(p, Landsat8LC2)


if __name__ == '__main__':
    unittest.main()
