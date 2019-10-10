#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Copyright (C) CNES - All Rights Reserved
This file is subject to the terms and conditions defined in
file 'LICENSE.md', which is part of this source code package.

Author:         Peter KETTIG <peter.kettig@cnes.fr>
Project:        Start_maja, CNES
Created on:     Tue Dec  5 10:26:05 2018
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

        raw = np.arange(0, 25).reshape(5, 5)
        grad_y, grad_x = MNTBase.MNT.calc_gradient(raw)
        expected_y = [[-4, -8, -8, -8, -4],
                      [-4, -8, -8, -8, -4],
                      [-4, -8, -8, -8, -4],
                      [-4, -8, -8, -8, -4],
                      [-4, -8, -8, -8, -4]]
        expected_x = [[20, 20, 20, 20, 20],
                      [40, 40, 40, 40, 40],
                      [40, 40, 40, 40, 40],
                      [40, 40, 40, 40, 40],
                      [20, 20, 20, 20, 20]]
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
                         [[1.56829633, 1.56830257, 1.56830877, 1.56831494, 1.56832108],
                          [1.57017327, 1.57017366, 1.57017405, 1.57017444, 1.57017482],
                          [1.57017521, 1.57017559, 1.57017598, 1.57017636, 1.57017675],
                          [1.57017713, 1.57017751, 1.5701779, 1.57017828, 1.57017866],
                          [1.56841538, 1.56842103, 1.56842666, 1.56843227, 1.56843784]])
        expected_aspect = np.array(
                          [[4.76240984, 1.52090031, 1.52102454, 1.52114814, 1.52127113],
                           [1.54587163, 1.54588715, 1.54590265, 1.54591814, 1.5459336],
                           [1.54594905, 1.54596448, 1.54597988, 1.54599527, 1.54601064],
                           [1.54602599, 1.54604132, 1.54605664, 1.54607193, 1.5460872],
                           [1.52315926, 1.5232725, 1.5233852, 1.52349737, 1.52360901]])
        np.testing.assert_array_almost_equal(slope, expected_slope)
        np.testing.assert_array_almost_equal(aspect, expected_aspect)

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
