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
import os
import tempfile
from prepare_mnt.mnt import MNTBase, SiteInfo


class TestMNTBase(unittest.TestCase):

    raw_gsw = os.path.join(tempfile.gettempdir(), "raw_gsw")
    raw_dem = os.path.join(tempfile.gettempdir(), "raw_dem")

    @classmethod
    def setUpClass(cls):
        from Common import FileSystem
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
        import numpy as np
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
        import numpy as np

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
        import numpy as np
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
        from Common import FileSystem
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
        import numpy as np
        from Common import ImageIO, FileSystem
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
        img_read, drv = ImageIO.tiff_to_array(water_mask, array_only=False)
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
        self.assertEqual(ImageIO.get_resolution(drv), (resx, resy))
        self.assertEqual(ImageIO.get_epsg(drv), 32631)
        self.assertEqual(img_read.shape, (py, py))
        np.testing.assert_almost_equal(img_expected, img_read)
        FileSystem.remove_file(water_mask)
        FileSystem.remove_directory(dem_dir)

    def test_get_water_data_spain_s2(self):
        """
        Download the given gsw file and project it to a 10km x 10km resolution (11x11 image)
        """
        import numpy as np
        from Common import ImageIO, FileSystem
        resx, resy = 10000, -10000
        px, py = 11, 11
        site = SiteInfo.Site("T30TYK", 32630,
                             px=px,
                             py=py,
                             ul=(699960.000, 4500000.000),
                             lr=(809760.000, 4390200.000),
                             res_x=resx,
                             res_y=resy)
        dem_dir = os.path.join(os.getcwd(), "test_get_water_data_spain_s2")
        mnt = MNTBase.MNT(site, dem_dir=dem_dir, raw_dem=self.raw_dem, raw_gsw=self.raw_gsw)
        water_mask = mnt.gsw_dst
        self.assertTrue(os.path.exists(dem_dir))
        mnt.prepare_water_data()
        self.assertTrue(os.path.exists(water_mask))
        img_read, drv = ImageIO.tiff_to_array(water_mask, array_only=False)
        img_expected = [[0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1],
                        [0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1],
                        [0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1],
                        [0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1],
                        [0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1],
                        [0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1],
                        [0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1],
                        [0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1],
                        [0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1],
                        [0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1],
                        [0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1]]
        self.assertEqual(ImageIO.get_resolution(drv), (resx, resy))
        self.assertEqual(ImageIO.get_epsg(drv), 32630)
        self.assertEqual(img_read.shape, (py, px))
        np.testing.assert_almost_equal(img_expected, img_read)
        FileSystem.remove_directory(dem_dir)

    def test_get_water_data_maccanw2_vns(self):
        import numpy as np
        from Common import ImageIO, FileSystem
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
        img_read, drv = ImageIO.tiff_to_array(water_mask, array_only=False)
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
        self.assertEqual(ImageIO.get_resolution(drv), (resx, resy))
        self.assertEqual(ImageIO.get_epsg(drv), 32633)
        self.assertEqual(img_read.shape, (py, px))
        np.testing.assert_almost_equal(img_expected, img_read)
        FileSystem.remove_directory(dem_dir)

    def test_get_water_data_tls_l8(self):
        import numpy as np
        from Common import ImageIO, FileSystem
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
        img_read, drv = ImageIO.tiff_to_array(water_mask, array_only=False)
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
        self.assertEqual(ImageIO.get_resolution(drv), (resx, resy))
        self.assertEqual(ImageIO.get_epsg(drv), 32631)
        self.assertEqual(img_read.shape, (py, px))
        np.testing.assert_almost_equal(img_expected, img_read)
        FileSystem.remove_directory(dem_dir)


if __name__ == '__main__':
    unittest.main()
