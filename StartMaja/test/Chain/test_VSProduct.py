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
from StartMaja.Chain.VSProduct import VenusMuscate, VenusNatif
import os


class TestVSProduct(unittest.TestCase):

    prod_vs_nat = ["VE_VM01_VSC_L2VALD_ISRAW906_20180317.DBL.DIR",
                   "VE_OPER_VSC_L1VALD_UNH_20180329.DBL.DIR",
                   "VE_VM01_VSC_L1VALD_SUDOUE_6_20191110.DBL.DIR",
                   "VE_VM01_VSC_L1VALD_CHILE____20200311.DBL.DIR"]
    prod_vs_mus = ["VENUS-XS_20180201-051359-000_L1C_KHUMBU_C_V1-0",
                   "VENUS_20180201-051359-000_L2A_KHUMBU_C_V1-0",
                   "VENUS-XS_20180201-000000-000_L3A_KHUMBU_C_V1-0"]
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
                   "SENTINEL2X_20190415-000000-000_L3A_T31UFR_C_V1-1"]

    @classmethod
    def setUpClass(cls):
        """
        Simulate the basic folder + metadata_file structure
        :return:
        """
        import os
        for root in cls.prod_vs_nat:
            os.makedirs(root)
            metadata = root.split(".")[0] + ".HDR"
            TestFunctions.touch(metadata)
        for root in cls.prod_vs_mus:
            os.makedirs(root)
            metadata = os.path.join(root, root + "_MTD_ALL.xml")
            TestFunctions.touch(metadata)

    @classmethod
    def tearDownClass(cls):
        import shutil
        import os
        for root in cls.prod_vs_nat:
            shutil.rmtree(root)
            metadata = root.split(".")[0] + ".HDR"
            os.remove(metadata)
        for root in cls.prod_vs_mus:
            shutil.rmtree(root)

    def test_reg_vs_muscate(self):
        tiles = ["KHUMBU", "KHUMBU", "KHUMBU"]
        levels = ["l1c", "l2a", "l3a"]
        dates = ["20180201T051359", "20180201T051359", "20180201T000000"]
        validity = [True, False, False]
        for prod, tile, date, level, valid in zip(self.prod_vs_mus, tiles, dates, levels, validity):
            p = MajaProduct.factory(prod)
            self.assertIsInstance(p, VenusMuscate)
            self.assertEqual(p.level, level)
            self.assertEqual(p.platform, "venus")
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
            self.assertEqual(p.mnt_resolutions_dict, [{'name': 'XS', 'val': '5 -5'}])
            self.assertEqual(p, p)
            FileSystem.remove_directory(link_dir)

        # Other prods:
        for prod in self.prod_vs_nat + self.prods_other:
            p = MajaProduct.factory(prod)
            self.assertNotIsInstance(p, VenusMuscate)

    def test_reg_vs_natif(self):
        tiles = ["ISRAW906", "UNH", "SUDOUE_6", "CHILE"]
        dates = ["20180317T120000", "20180329T120000", "20191110T120000", "20200311T120000"]
        levels = ["l2a", "l1c", "l1c", "l1c"]
        for prod, tile, date, level in zip(self.prod_vs_nat, tiles, dates, levels):
            p = MajaProduct.factory(prod)
            self.assertIsInstance(p, VenusNatif)
            self.assertEqual(p.level, level)
            self.assertEqual(p.platform, "venus")
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
            self.assertEqual(p.mnt_resolutions_dict, [{'name': 'XS', 'val': '5 -5'}])
            self.assertEqual(p, p)
            FileSystem.remove_directory(link_dir)

        # Other prods:
        for prod in self.prod_vs_mus + self.prods_other:
            p = MajaProduct.factory(prod)
            self.assertNotIsInstance(p, VenusNatif)


if __name__ == '__main__':
    unittest.main()
