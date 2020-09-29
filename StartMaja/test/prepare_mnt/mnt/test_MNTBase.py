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
import os
import tempfile
import numpy as np
from StartMaja.prepare_mnt.mnt import MNTBase, SiteInfo
from StartMaja.Common import FileSystem
from StartMaja.Common.GDalDatasetWrapper import GDalDatasetWrapper


class TestMNTBase(unittest.TestCase):

    raw_gsw = os.path.join(tempfile.gettempdir(), "raw_gsw")
    raw_dem = os.path.join(tempfile.gettempdir(), "raw_dem")

    @classmethod
    def setUpClass(cls):
        # Note those directories are not destroyed after executing tests.
        # This is in order to avoid multiple downloads amongst different test classes.
        FileSystem.create_directory(cls.raw_gsw)
        FileSystem.create_directory(cls.raw_dem)

    def test_gsw_codes_toulouse(self):
        site = SiteInfo.Site("T31TCJ", 32631,
                             ul=(300000.000, 4900020.000),
                             lr=(409800.000, 4790220.000))

        gsw_codes = MNTBase.MNT.get_gsw_codes(site)
        self.assertEqual(gsw_codes, ['0E_50N'])

    def test_gsw_codes_spain(self):
        site = SiteInfo.Site("T31TBE", 32631,
                             ul=(199980.000, 4500000.000),
                             lr=(309780.000, 4390200.000))

        gsw_codes = MNTBase.MNT.get_gsw_codes(site)
        self.assertEqual(gsw_codes, ['0E_40N', '0E_50N', '10W_40N', '10W_50N'])

    def test_gsw_codes_zero_center(self):
        site = SiteInfo.Site("Somewhere_over_the_ocean", 32631,
                             ul=(-250000.000, 250000.000),
                             lr=(250000.000, -120000.000))

        gsw_codes = MNTBase.MNT.get_gsw_codes(site)
        self.assertEqual(gsw_codes, ['0E_0N', '0E_10N', '10W_0N', '10W_10N'])

    def test_gsw_codes_south_america(self):
        site = SiteInfo.Site("Ecuador", 32619,
                             ul=(-250000.000, 250000.000),
                             lr=(400000.000, -120000.000))

        gsw_codes = MNTBase.MNT.get_gsw_codes(site)
        self.assertEqual(gsw_codes, ['70W_0N', '70W_10N', '80W_0N', '80W_10N'])

    def test_gsw_codes_longitude_change(self):
        site = SiteInfo.Site("Russia", 32601,
                             ul=(-250000.000, 250000.000),
                             lr=(400000.000, -120000.000))
        # TODO Support this:
        with self.assertRaises(ValueError):
            MNTBase.MNT.get_gsw_codes(site)

    def test_calculate_gradient(self):
        res_x, res_y = 10, -10
        raw = np.arange(0, 25).reshape(5, 5)
        grad_y, grad_x = MNTBase.MNT.calc_gradient(raw, res_x, res_y)
        expected_y = [[-.05, -.1, -.1, -.1, -.05],
                      [-.05, -.1, -.1, -.1, -.05],
                      [-.05, -.1, -.1, -.1, -.05],
                      [-.05, -.1, -.1, -.1, -.05],
                      [-.05, -.1, -.1, -.1, -.05]]
        expected_x = [[.25, .25, .25, .25, .25],
                      [.5, .5, .5, .5, .5],
                      [.5, .5, .5, .5, .5],
                      [.5, .5, .5, .5, .5],
                      [.25, .25, .25, .25, .25]]
        np.testing.assert_array_almost_equal(grad_y, expected_y)
        np.testing.assert_array_almost_equal(grad_x, expected_x)

    def test_calculate_slope_aspect(self):
        raw = np.arange(0, 25).reshape(5, 5)
        raw_2 = np.array(
                [[20, 20, 20, 20, 20],
                 [40, 40, 40, 40, 40],
                 [40, 40, 40, 40, 40],
                 [40, 40, 40, 40, 40],
                 [20, 20, 20, 20, 20]])
        slope, aspect = MNTBase.MNT.calc_slope_aspect(raw, raw_2)
        expected_slope = np.array(
            [[152, 152, 152, 152, 152],
             [154, 154, 154, 154, 154],
             [154, 154, 154, 154, 154],
             [154, 154, 154, 154, 154],
             [153, 153, 153, 153, 153]])
        expected_aspect = np.array(
            [[628, 4, 9, 14, 19],
             [12, 14, 17, 19, 22],
             [24, 26, 29, 31, 33],
             [35, 38, 40, 42, 44],
             [78, 80, 83, 85, 87]])
        np.testing.assert_array_almost_equal(slope, expected_slope)
        np.testing.assert_array_almost_equal(aspect, expected_aspect)

    def test_resample_to_full_resolution(self):
        res_full = (10, 10)
        res_mnt = (40, -40)

        img = np.arange(0, 4).reshape(2, 2)

        resampled = MNTBase.MNT.resample_to_full_resolution(img, res_mnt, res_full)
        expected = np.array([[0, 0, 0, 0, 1, 1, 1, 1],
                             [0, 0, 0, 1, 1, 1, 1, 1],
                             [0, 0, 1, 1, 1, 1, 1, 1],
                             [1, 1, 1, 1, 1, 2, 2, 2],
                             [1, 1, 1, 2, 2, 2, 2, 2],
                             [2, 2, 2, 2, 2, 2, 3, 3],
                             [2, 2, 2, 2, 2, 3, 3, 3],
                             [2, 2, 2, 2, 3, 3, 3, 3]])
        np.testing.assert_array_almost_equal(resampled, expected)

    def test_gsw_download(self):
        site = SiteInfo.Site("Ecuador", 32619,
                             ul=(-250000.000, 250000.000),
                             lr=(400000.000, -120000.000))
        gsw_dir = os.path.join(os.getcwd(), "test_gsw_download")
        m = MNTBase.MNT(site, dem_dir=gsw_dir, raw_gsw=self.raw_gsw, raw_dem=self.raw_dem)
        fnames = m.get_raw_water_data()
        self.assertEqual(len(fnames), 4)
        fnames_expected = ['occurrence_70W_0N_v1_1.tif',
                           'occurrence_70W_10N_v1_1.tif',
                           'occurrence_80W_0N_v1_1.tif',
                           'occurrence_80W_10N_v1_1.tif']
        for fn, expected in zip(fnames, fnames_expected):
            self.assertTrue(os.path.exists(fn))
            self.assertEqual(os.path.basename(fn), expected)
        FileSystem.remove_directory(gsw_dir)
        self.assertFalse(os.path.exists(gsw_dir))

    def test_get_water_data_tls_s2(self):
        """
        Download the given gsw file and project it to a 10km x 10km resolution (11x11 image)
        """
        resx, resy = 10000, -10000
        px, py = 11, 11
        site = SiteInfo.Site("T31TCJ", 32631,
                             px=px,
                             py=py,
                             ul=(300000.000, 4900020.000),
                             lr=(409800.000, 4790220.000),
                             res_x=resx,
                             res_y=resy)
        dem_dir = os.path.join(os.getcwd(), "test_get_water_data_tls_s2")
        mnt = MNTBase.MNT(site, dem_dir=dem_dir, raw_dem=self.raw_dem, raw_gsw=self.raw_gsw)
        water_mask = mnt.gsw_dst
        self.assertTrue(os.path.exists(dem_dir))
        mnt.prepare_water_data()
        self.assertTrue(os.path.exists(water_mask))
        driver = GDalDatasetWrapper.from_file(water_mask)
        img_expected = [[0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
                        [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
                        [0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0],
                        [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
                        [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
                        [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
                        [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
                        [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
                        [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
                        [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
                        [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0]]
        self.assertEqual(driver.resolution, (resx, resy))
        self.assertEqual(driver.epsg, 32631)
        self.assertEqual(driver.array.shape, (py, px))
        np.testing.assert_almost_equal(img_expected, driver.array)
        self.assertIsNone(driver.nodata_value)
        FileSystem.remove_directory(dem_dir)

    def test_get_water_data_spain_s2(self):
        """
        Download the given gsw file and project it to a 10km x 10km resolution (11x11 image)
        """
        resx, resy = 10000, -10000
        px, py = 11, 11
        site = SiteInfo.Site("T30SYJ", 32630,
                             px=px,
                             py=py,
                             ul=(699960.000, 4400040.000),
                             lr=(809760.000, 4290240.000),
                             res_x=resx,
                             res_y=resy)
        dem_dir = os.path.join(os.getcwd(), "test_get_water_data_spain_s2")
        mnt = MNTBase.MNT(site, dem_dir=dem_dir, raw_dem=self.raw_dem, raw_gsw=self.raw_gsw)
        water_mask = mnt.gsw_dst
        self.assertTrue(os.path.exists(dem_dir))
        mnt.prepare_water_data()
        self.assertTrue(os.path.exists(water_mask))
        driver = GDalDatasetWrapper.from_file(water_mask)
        img_expected = [[0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1],
                        [0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1],
                        [0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1],
                        [0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1],
                        [0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1],
                        [0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1],
                        [0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1],
                        [0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1],
                        [0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1],
                        [0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1],
                        [0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1]]
        self.assertEqual(driver.resolution, (resx, resy))
        self.assertEqual(driver.epsg, 32630)
        self.assertEqual(driver.array.shape, (py, px))
        np.testing.assert_almost_equal(img_expected, driver.array)
        self.assertIsNone(driver.nodata_value)
        FileSystem.remove_directory(dem_dir)

    def test_get_water_data_maccanw2_vns(self):
        resx, resy = 5000, -5000
        px, py = 11, 14
        site = SiteInfo.Site("MACCANW2", 32633,
                             px=px,
                             py=py,
                             ul=(246439.500, 4672656.500),
                             lr=(299769.500, 4604231.500),
                             res_x=resx,
                             res_y=resy)
        dem_dir = os.path.join(os.getcwd(), "test_get_water_data_maccanw2_vns")
        mnt = MNTBase.MNT(site, dem_dir=dem_dir, raw_dem=self.raw_dem, raw_gsw=self.raw_gsw)
        water_mask = mnt.gsw_dst
        self.assertTrue(os.path.exists(dem_dir))
        mnt.prepare_water_data()
        self.assertTrue(os.path.exists(water_mask))
        driver = GDalDatasetWrapper.from_file(water_mask)
        img_expected = [[0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0],
                        [0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0],
                        [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
                        [1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
                        [1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0],
                        [1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0],
                        [1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0],
                        [1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0],
                        [1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0],
                        [1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0],
                        [1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0],
                        [1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0],
                        [1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0],
                        [1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0]]
        self.assertEqual(driver.resolution, (resx, resy))
        self.assertEqual(driver.epsg, 32633)
        self.assertEqual(driver.array.shape, (py, px))
        np.testing.assert_almost_equal(img_expected, driver.array)
        self.assertIsNone(driver.nodata_value)
        FileSystem.remove_directory(dem_dir)

    def test_get_water_data_tls_l8(self):
        resx, resy = 15000, -15000
        px, py = 15, 14
        site = SiteInfo.Site("19080", 32631,
                             px=px,
                             py=py,
                             ul=(285431.584, 4884950.507),
                             lr=(510124.885, 4680403.373),
                             res_x=resx,
                             res_y=resy)
        dem_dir = os.path.join(os.getcwd(), "test_get_water_data_tls_l8")
        mnt = MNTBase.MNT(site, dem_dir=dem_dir, raw_dem=self.raw_dem, raw_gsw=self.raw_gsw)
        water_mask = mnt.gsw_dst
        self.assertTrue(os.path.exists(dem_dir))
        mnt.prepare_water_data()
        self.assertTrue(os.path.exists(water_mask))
        driver = GDalDatasetWrapper.from_file(water_mask)
        img_expected = [[0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
                        [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
                        [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
                        [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
                        [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
                        [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
                        [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
                        [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
                        [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1],
                        [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
                        [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
                        [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
                        [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
                        [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0]]
        self.assertEqual(driver.resolution, (resx, resy))
        self.assertEqual(driver.epsg, 32631)
        self.assertEqual(driver.array.shape, (py, px))
        self.assertIsNone(driver.nodata_value)
        np.testing.assert_almost_equal(img_expected, driver.array)
        FileSystem.remove_directory(dem_dir)


if __name__ == '__main__':
    unittest.main()
