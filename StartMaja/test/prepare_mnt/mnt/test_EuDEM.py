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
from StartMaja.prepare_mnt.mnt import EuDEM, SiteInfo
from StartMaja.Common import FileSystem
from StartMaja.Common import TestFunctions
import numpy as np
import zipfile
from StartMaja.Common.ImageIO import write_geotiff
from StartMaja.Common.GDalDatasetWrapper import GDalDatasetWrapper


class TestEuDEM(unittest.TestCase):

    raw_gsw = tempfile.mkdtemp(prefix="raw_gsw_")
    raw_eudem = tempfile.mkdtemp(prefix="raw_dem_")

    projection = 'PROJCS["ETRS89_ETRS_LAEA",\
                  GEOGCS["ETRS89",\
                  DATUM["European_Terrestrial_Reference_System_1989",\
                  SPHEROID["GRS 1980",6378137,298.2572221010042,\
                  AUTHORITY["EPSG","7019"]],\
                  AUTHORITY["EPSG","6258"]],\
                  PRIMEM["Greenwich",0],\
                  UNIT["degree",0.0174532925199433],\
                  AUTHORITY["EPSG","4258"]],\
                  PROJECTION["Lambert_Azimuthal_Equal_Area"],\
                  PARAMETER["latitude_of_center",52],\
                  PARAMETER["longitude_of_center",10],\
                  PARAMETER["false_easting",4321000],\
                  PARAMETER["false_northing",3210000],\
                  UNIT["metre",1,\
                  AUTHORITY["EPSG","9001"]]]'
    geotransform_e30n20 = (3000000.0, 2500.0, 0.0, 3000000.0, 0.0, -2500.0)
    geotransform_e30n30 = (3000000.0, 2500.0, 0.0, 4000000.0, 0.0, -2500.0)

    @classmethod
    def setUpClass(cls):
        # Note those directories are not destroyed after executing tests.
        # This is in order to avoid multiple downloads amongst different test classes.
        FileSystem.create_directory(cls.raw_gsw)
        FileSystem.create_directory(cls.raw_eudem)

        # Setup dummy EuDEM file of reduced resolution
        arr = np.zeros((400, 400), dtype=np.float32)
        to_zip = [os.path.join(cls.raw_eudem, "eu_dem_v11_E30N20.TIF"),
                  os.path.join(cls.raw_eudem, "eu_dem_v11_E30N20.TIF.ovr"),
                  os.path.join(cls.raw_eudem, "eu_dem_v11_E30N20.TIF.aux.xml"),
                  os.path.join(cls.raw_eudem, "eu_dem_v11_E30N20.TFw")]
        write_geotiff(arr, to_zip[0], cls.projection, cls.geotransform_e30n20)
        [TestFunctions.touch(f) for f in to_zip[1:]]
        with zipfile.ZipFile(os.path.join(cls.raw_eudem, "eu_dem_v11_E30N20.zip"), 'w') as zip_archive:
            for file in to_zip:
                zip_archive.write(file, compress_type=zipfile.ZIP_DEFLATED)
        FileSystem.remove_file(to_zip[0])

        # Setup dummy EuDEM file of reduced resolution
        arr = np.ones((400, 400), dtype=np.float32)
        # Add some other value to around half of the image
        arr[:, 260:] = 10
        to_zip = [os.path.join(cls.raw_eudem, "eu_dem_v11_E30N30.TIF"),
                  os.path.join(cls.raw_eudem, "eu_dem_v11_E30N30.TIF.ovr"),
                  os.path.join(cls.raw_eudem, "eu_dem_v11_E30N30.TIF.aux.xml"),
                  os.path.join(cls.raw_eudem, "eu_dem_v11_E30N30.TFw")]
        write_geotiff(arr, to_zip[0], cls.projection, cls.geotransform_e30n30)
        [TestFunctions.touch(f) for f in to_zip[1:]]
        with zipfile.ZipFile(os.path.join(cls.raw_eudem, "eu_dem_v11_E30N30.zip"), 'w') as zip_archive:
            for file in to_zip:
                zip_archive.write(file, compress_type=zipfile.ZIP_DEFLATED)
        FileSystem.remove_file(to_zip[0])

    @classmethod
    def tearDownClass(cls):
        FileSystem.remove_directory(cls.raw_eudem)
        FileSystem.remove_directory(cls.raw_gsw)

    def test_get_eudem_codes_tls(self):
        site = SiteInfo.Site("T31TCJ", 32631,
                             ul=(300000.000, 4900020.000),
                             lr=(409800.000, 4790220.000))
        eudem_codes = EuDEM.EuDEM.get_eudem_codes(site)
        self.assertEqual(eudem_codes, ["eu_dem_v11_E30N20"])

    def test_get_eudem_codes_montblanc(self):
        site = SiteInfo.Site("T31TGL", 32631,
                             ul=(699960.000, 5100000.000),
                             lr=(809760.000, 4990200.000))
        eudem_codes = EuDEM.EuDEM.get_eudem_codes(site)
        self.assertEqual(eudem_codes, ['eu_dem_v11_E30N20', 'eu_dem_v11_E40N20'])

    def test_get_eudem_codes_belgium(self):
        site = SiteInfo.Site("T31UFR", 32631,
                             ul=(600000.000, 5600040.000),
                             lr=(709800.000, 5490240.000))
        eudem_codes = EuDEM.EuDEM.get_eudem_codes(site)
        self.assertEqual(eudem_codes, ['eu_dem_v11_E30N20', 'eu_dem_v11_E30N30',
                                       'eu_dem_v11_E40N20', 'eu_dem_v11_E40N30'])

    def test_get_eudem_codes_belgium_coast(self):
        site = SiteInfo.Site("T31UCR", 32631,
                             ul=(300000, 5600040),
                             lr=(409800, 5490240))
        eudem_codes = EuDEM.EuDEM.get_eudem_codes(site)
        self.assertEqual(eudem_codes, ['eu_dem_v11_E30N20', 'eu_dem_v11_E30N30'])

    def test_eudem_prepare_mnt_s2_31ucr(self):
        resx, resy = 10000, -10000
        site = SiteInfo.Site("T31UCR", 32631,
                             px=11,
                             py=11,
                             ul=(300000, 5600040),
                             lr=(409800, 5490240),
                             res_x=resx,
                             res_y=resy)
        dem_dir = os.path.join(os.getcwd(), "test_eudem_prepare_mnt_s2_31ucr")
        s = EuDEM.EuDEM(site, dem_dir=dem_dir, raw_dem=self.raw_eudem, raw_gsw=self.raw_gsw, wdir=dem_dir)
        self.assertTrue(os.path.isdir(dem_dir))
        srtm = s.prepare_mnt()
        self.assertTrue(os.path.isfile(srtm))
        driver = GDalDatasetWrapper.from_file(srtm)
        expected_img = [[1., 1., 6., 10., 10., 10., 10., 10., 10., 10., 10.],
                        [1., 1., 6., 10., 10., 10., 10., 10., 10., 10., 10.],
                        [1., 1., 5., 10., 10., 10., 10., 10., 10., 10., 10.],
                        [1., 1., 4., 10., 10., 10., 10., 10., 10., 10., 10.],
                        [1., 1., 3., 10., 10., 10., 10., 10., 10., 10., 10.],
                        [1., 1., 3., 10., 10., 10., 10., 10., 10., 10., 10.],
                        [1., 1., 2., 9., 10., 10., 10., 10., 10., 10., 10.],
                        [1., 1., 2., 9., 10., 10., 10., 10., 10., 10., 10.],
                        [1., 1., 1., 8., 10., 10., 10., 0., 0., 0., 0.],
                        [0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0.],
                        [0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0.]]
        self.assertEqual(driver.resolution, (resx, resy))
        self.assertEqual(driver.array.shape, (site.py, site.px))
        self.assertEqual(driver.nodata_value, 0)
        np.testing.assert_allclose(expected_img, driver.array)
        FileSystem.remove_directory(dem_dir)

    def test_eudem_prepare_mnt_s2_tls(self):
        resx, resy = 10000, -10000
        site = SiteInfo.Site("T31TCJ", 32631,
                             ul=(300000.000, 4900020.000),
                             lr=(409800.000, 4790220.000),
                             px=11,
                             py=11,
                             res_x=resx,
                             res_y=resy)
        dem_dir = os.path.join(os.getcwd(), "test_eudem_prepare_mnt_s2_31ucr")
        s = EuDEM.EuDEM(site, dem_dir=dem_dir, raw_dem=self.raw_eudem, raw_gsw=self.raw_gsw, wdir=dem_dir)
        self.assertTrue(os.path.isdir(dem_dir))
        srtm = s.prepare_mnt()
        self.assertTrue(os.path.isfile(srtm))
        driver = GDalDatasetWrapper.from_file(srtm)
        expected_img = [[0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0.],
                        [0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0.],
                        [0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0.],
                        [0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0.],
                        [0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0.],
                        [0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0.],
                        [0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0.],
                        [0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0.],
                        [0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0.],
                        [0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0.],
                        [0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0.]]
        self.assertEqual(driver.resolution, (resx, resy))
        self.assertEqual(driver.array.shape, (site.py, site.px))
        self.assertEqual(driver.nodata_value, 0)
        np.testing.assert_allclose(expected_img, driver.array)
        FileSystem.remove_directory(dem_dir)


if __name__ == '__main__':
    unittest.main()
