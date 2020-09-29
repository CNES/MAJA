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
import tempfile
import os
import numpy as np
from StartMaja.prepare_mnt.mnt import SRTM, SiteInfo
from StartMaja.Common import FileSystem, ImageIO
from StartMaja.Common.GDalDatasetWrapper import GDalDatasetWrapper


class TestSRTM(unittest.TestCase):

    raw_gsw = os.path.join(tempfile.gettempdir(), "raw_gsw")
    raw_srtm = os.path.join(tempfile.gettempdir(), "raw_dem")

    @classmethod
    def setUpClass(cls):
        # Note those directories are not destroyed after executing tests.
        # This is in order to avoid multiple downloads amongst different test classes.
        FileSystem.create_directory(cls.raw_gsw)
        FileSystem.create_directory(cls.raw_srtm)

    def test_get_srtm_codes_tls(self):
        site = SiteInfo.Site("T31TCJ", 32631,
                             ul=(300000.000, 4900020.000),
                             lr=(409800.000, 4790220.000))
        srtm_codes = SRTM.SRTM.get_srtm_codes(site)
        self.assertEqual(srtm_codes, ["srtm_37_04"])

    def test_get_srtm_codes_spain(self):
        site = SiteInfo.Site("T31TBE", 32631,
                             ul=(199980.000, 4500000.000),
                             lr=(309780.000, 4390200.000),)
        srtm_codes = SRTM.SRTM.get_srtm_codes(site)
        self.assertEqual(srtm_codes, ['srtm_36_04', 'srtm_37_04', 'srtm_36_05', 'srtm_37_05'])

    def test_get_raw_data(self):
        site = SiteInfo.Site("T31TCJ", 32631,
                             ul=(300000.000, 4900020.000),
                             lr=(409800.000, 4790220.000))
        dem_dir = os.path.join(os.getcwd(), "srtm_dir")
        s = SRTM.SRTM(site, dem_dir=dem_dir, raw_dem=self.raw_srtm, raw_gsw=self.raw_gsw, wdir=dem_dir)
        self.assertTrue(os.path.isdir(dem_dir))
        srtm_codes = SRTM.SRTM.get_srtm_codes(site)
        s.get_raw_data()
        for code in srtm_codes:
            filepath = os.path.join(self.raw_srtm, code + ".zip")
            self.assertTrue(os.path.isfile(filepath))
        FileSystem.remove_directory(dem_dir)

    def test_srtm_prepare_mnt_s2_tls(self):
        resx, resy = 10000, -10000
        site = SiteInfo.Site("T31TCJ", 32631,
                             px=11,
                             py=11,
                             ul=(300000.000, 4900020.000),
                             lr=(409800.000, 4790220.000),
                             res_x=resx,
                             res_y=resy)
        dem_dir = os.path.join(os.getcwd(), "test_srtm_prepare_mnt_s2_tls")
        s = SRTM.SRTM(site, dem_dir=dem_dir, raw_dem=self.raw_srtm, raw_gsw=self.raw_gsw, wdir=dem_dir)
        self.assertTrue(os.path.isdir(dem_dir))
        srtm = s.prepare_mnt()
        self.assertTrue(os.path.isfile(srtm))
        driver = GDalDatasetWrapper.from_file(srtm)
        expected_img = [[85, 85, 115, 142, 138, 147, 151, 163, 162, 258, 282],
                        [136, 98, 96, 90, 101, 91, 120, 118, 121, 243, 302],
                        [137, 133, 135, 137, 101, 89, 92, 131, 162, 246, 294],
                        [139, 144, 170, 184, 157, 115, 108, 132, 176, 210, 212],
                        [148, 140, 161, 162, 206, 138, 133, 134, 150, 164, 160],
                        [169, 173, 168, 171, 222, 168, 128, 158, 162, 151, 164],
                        [193, 199, 181, 186, 186, 193, 158, 147, 189, 204, 199],
                        [209, 223, 207, 195, 215, 215, 166, 161, 187, 214, 228],
                        [240, 247, 225, 206, 268, 211, 172, 203, 195, 213, 242],
                        [281, 268, 246, 272, 282, 216, 216, 208, 231, 211, 220],
                        [335, 302, 319, 311, 265, 234, 262, 251, 236, 259, 250]]
        self.assertEqual(driver.resolution, (resx, resy))
        self.assertEqual(driver.array.shape, (site.py, site.px))
        self.assertEqual(driver.nodata_value, 0)
        np.testing.assert_allclose(expected_img, driver.array, atol=1.5)
        FileSystem.remove_directory(dem_dir)

    def test_srtm_prepare_mnt_vns_maccanw2(self):
        resx, resy = 5000, -5000
        px, py = 11, 14
        site = SiteInfo.Site("MACCANW2", 32633,
                             px=px,
                             py=py,
                             ul=(246439.500, 4672656.500),
                             lr=(299769.500, 4604231.500),
                             res_x=resx,
                             res_y=resy)
        dem_dir = os.path.join(os.getcwd(), "test_srtm_prepare_mnt_vns_maccanw2")
        s = SRTM.SRTM(site, dem_dir=dem_dir, raw_dem=self.raw_srtm, raw_gsw=self.raw_gsw, wdir=dem_dir)
        self.assertTrue(os.path.isdir(dem_dir))
        srtm = s.prepare_mnt()
        self.assertTrue(os.path.isfile(srtm))
        driver = GDalDatasetWrapper.from_file(srtm)
        expected_img = [[293, 210, 337, 390, 238, 218, 237, 245, 270, 208, 132],
                        [302, 293, 277, 302, 182, 172, 237, 270, 262, 171, 60],
                        [155, 239, 239, 231, 238, 199, 164, 173, 137, 85, 33],
                        [0, 28, 94, 138, 150, 131, 116, 117, 72, 37, 73],
                        [0, 0, 18, 67, 81, 79, 84, 117, 72, 36, 59],
                        [0, 0, 0, 11, 38, 58, 68, 87, 68, 38, 33],
                        [0, 0, 0, 0, 6, 41, 52, 62, 52, 43, 41],
                        [0, 0, 0, 0, 1, 0, 30, 34, 30, 48, 68],
                        [0, 0, 0, 0, 0, -1, 12, 26, 42, 70, 101],
                        [0, 0, 0, 0, 0, 4, 11, 50, 59, 89, 136],
                        [0, 0, 0, 0, 0, 0, 0, 30, 76, 97, 130],
                        [0, 0, 0, 0, 0, 0, 0, 0, 44, 72, 86],
                        [0, 0, 0, 0, 0, 0, 0, 0, 0, 28, 59],
                        [0, 0, 0, 0, 0, 0, 0, 0, 0, 18, 54]]
        self.assertEqual(driver.resolution, (resx, resy))
        self.assertEqual(driver.array.shape, (site.py, site.px))
        self.assertEqual(driver.nodata_value, 0)
        np.testing.assert_allclose(expected_img, driver.array, atol=1.5)
        FileSystem.remove_directory(dem_dir)

    def test_srtm_prepare_mnt_l8_tls(self):
        resx, resy = 15000, -15000
        px, py = 15, 14
        site = SiteInfo.Site("19080", 32631,
                             px=px,
                             py=py,
                             ul=(285431.584, 4884950.507),
                             lr=(510124.885, 4680403.373),
                             res_x=resx,
                             res_y=resy)
        dem_dir = os.path.join(os.getcwd(), "test_srtm_prepare_mnt_l8_tls")
        s = SRTM.SRTM(site, dem_dir=dem_dir, raw_dem=self.raw_srtm, raw_gsw=self.raw_gsw, wdir=dem_dir)
        self.assertTrue(os.path.isdir(dem_dir))
        srtm = s.prepare_mnt()
        self.assertTrue(os.path.isfile(srtm))
        driver = GDalDatasetWrapper.from_file(srtm)
        expected_img = np.array(
                        [[126, 134, 125, 114, 88, 104, 144, 265, 266, 283, 388, 487, 592, 571, 617],
                        [144, 139, 161, 177, 134, 117, 161, 198, 189, 205, 328, 487, 457, 444, 634],
                        [166, 163, 164, 189, 177, 137, 156, 153, 198, 231, 384, 619, 698, 707, 638],
                        [194, 201, 194, 188, 198, 154, 178, 205, 218, 196, 303, 634, 898, 925, 586],
                        [216, 235, 223, 223, 226, 175, 194, 220, 234, 318, 462, 637, 752, 547, 275],
                        [273, 287, 268, 283, 234, 222, 226, 227, 215, 423, 581, 548, 404, 246, 86],
                        [429, 383, 352, 315, 277, 283, 265, 274, 269, 170, 148, 120, 72, 43, 27],
                        [598, 472, 402, 387, 427, 379, 362, 328, 365, 291, 270, 328, 186, 114, 42],
                        [1151, 897, 977, 763, 647, 814, 708, 596, 514, 458, 458, 576, 383, 231, 0],
                        [1591, 1303, 1697, 1728, 1229, 1342, 1085, 1173, 1137, 829, 685, 513, 289, 158, 27],
                        [2261, 2075, 1964, 2052, 1916, 2087, 1923, 1634, 1741, 1576, 1185, 490, 198, 36, -24],
                        [1671, 1898, 2127, 2148, 1475, 1992, 2126, 2301, 2098, 1749, 1235, 1060, 448, 135, 159],
                        [1264, 1283, 1496, 1520, 1529, 1398, 1684, 1735, 1472, 2044, 1949, 1344, 684, 374, 302],
                        [871, 980, 988, 1052, 1401, 1051, 1526, 1479, 1487, 1457, 1253, 753, 567, 179, 32]])
        self.assertEqual(driver.resolution, (resx, resy))
        self.assertEqual(driver.array.shape, (site.py, site.px))
        self.assertEqual(driver.nodata_value, 0)

        np.testing.assert_allclose(expected_img, driver.array, atol=1.5)
        FileSystem.remove_directory(dem_dir)

    def test_srtm_get_maja_format_tls_l8(self):
        site = SiteInfo.Site("19080", 32631,
                             res_x=90,
                             res_y=-90,
                             ul=(285431.584, 4884950.507),
                             lr=(510124.885, 4680403.373))
        dem_dir = os.path.join(os.getcwd(), "test_srtm_get_maja_format_tls_l8")
        s = SRTM.SRTM(site, dem_dir=dem_dir, raw_dem=self.raw_srtm, raw_gsw=self.raw_gsw, wdir=dem_dir)
        self.assertTrue(os.path.isdir(dem_dir))
        hdr, dbl = s.to_maja_format(platform_id="L8_",
                                    mission_field="LANDSAT8",
                                    coarse_res=(240, -240),
                                    mnt_resolutions=[{"name": "R1",
                                                     "val": "30 -30"}])
        self.assertTrue(os.path.exists(hdr))
        self.assertTrue(os.path.isdir(dbl))
        FileSystem.remove_directory(dem_dir)

    def test_srtm_get_maja_format_s2_31tcj(self):
        site = SiteInfo.Site("T31TCJ", 32631,
                             res_x=90,
                             res_y=-90,
                             ul=(300000.000, 4900020.000),
                             lr=(409800.000, 4790220.000))
        dem_dir = os.path.join(os.getcwd(), "test_srtm_get_maja_format_s2_31tcj")
        s = SRTM.SRTM(site, dem_dir=dem_dir, raw_dem=self.raw_srtm, raw_gsw=self.raw_gsw, wdir=dem_dir)
        self.assertTrue(os.path.isdir(dem_dir))
        hdr, dbl = s.to_maja_format(platform_id="S2_",
                                    mission_field="SENTINEL-2_",
                                    coarse_res=(240, -240),
                                    mnt_resolutions=[{"name": "R1",
                                                     "val": "10 -10"},
                                                     {"name": "R2",
                                                     "val": "20 -20"}])
        self.assertTrue(os.path.exists(hdr))
        self.assertTrue(os.path.isdir(dbl))
        FileSystem.remove_directory(dem_dir)
        FileSystem.remove_file(hdr)


if __name__ == '__main__':
    unittest.main()
