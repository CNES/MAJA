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
import numpy as np
import os
from StartMaja.Common import ImageIO, FileSystem, ImageTools
from StartMaja.Common.GDalDatasetWrapper import GDalDatasetWrapper


class TestGdalDatasetWrapper(unittest.TestCase):

    def setUp(self):
        self.coordinates = (300000.0, 10.0, 0,
                            4900020.0, 0, -10.0)
        self.projection = 'PROJCS["WGS 84 / UTM zone 31N",\
                           GEOGCS["WGS 84",DATUM["WGS_1984",\
                           SPHEROID["WGS 84",6378137,298.257223563,\
                           AUTHORITY["EPSG","7030"]],\
                           AUTHORITY["EPSG","6326"]],\
                           PRIMEM["Greenwich",0,\
                           AUTHORITY["EPSG","8901"]],\
                           UNIT["degree",0.0174532925199433,\
                           AUTHORITY["EPSG","9122"]],\
                           AUTHORITY["EPSG","4326"]],\
                           PROJECTION["Transverse_Mercator"],\
                           PARAMETER["latitude_of_origin",0],\
                           PARAMETER["central_meridian",3],\
                           PARAMETER["scale_factor",0.9996],\
                           PARAMETER["false_easting",500000],\
                           PARAMETER["false_northing",0],\
                           UNIT["metre",1,AUTHORITY["EPSG","9001"]],\
                           AXIS["Easting",EAST],AXIS["Northing",NORTH],\
                           AUTHORITY["EPSG","32631"]]'
        self.height = 200
        self.width = 100

    def test_write_read_geotiff(self):
        img = np.ones((self.height, self.width), np.int16)
        path = os.path.join(os.getcwd(), "test_write_read_geotiff.tif")
        ImageIO.write_geotiff(img, path, self.projection, self.coordinates)
        self.assertTrue(os.path.exists(path))
        
        ds = GDalDatasetWrapper.from_file(path)
        self.assertTrue((ds.array == img).all())
        self.assertEqual(ds.geotransform, self.coordinates)
        # Compare projections by removing all spaces cause of multiline string
        self.assertEqual(ds.projection.replace(" ", ""), self.projection.replace(" ", ""))
        self.assertIsNone(ds.nodata_value)
        self.assertEqual(ds.epsg, 32631)
        FileSystem.remove_file(path)
        self.assertFalse(os.path.exists(path))

    def test_without_ds(self):
        img = np.ones((self.height, self.width), np.int16)
        ds = GDalDatasetWrapper(array=img, projection=self.projection, geotransform=self.coordinates)
        np.testing.assert_almost_equal(img, ds.array)
        self.assertEqual(ds.geotransform, self.coordinates)
        self.assertIsNone(ds.nodata_value)
        self.assertEqual(ds.epsg, 32631)
        np.testing.assert_almost_equal(np.ones_like(img), ds.nodata_mask)
        # Compare projections by removing all spaces cause of multiline string
        self.assertEqual(ds.projection.replace(" ", ""), self.projection.replace(" ", ""))

    def test_write_nodata(self):
        fname = "./test_write_nodata.tif"
        img = np.ones((self.height, self.width), np.int16)
        ds = GDalDatasetWrapper(array=img, projection=self.projection, geotransform=self.coordinates,
                                nodata_value=42)
        ds.write(fname, options=["COMPRESS=DEFLATE"])
        self.assertTrue(os.path.exists(fname))
        ds_read = GDalDatasetWrapper.from_file(fname)
        self.assertEqual(42, ds_read.nodata_value)
        self.assertEqual("DEFLATE", ds_read._info["metadata"]["IMAGE_STRUCTURE"]["COMPRESSION"])
        np.testing.assert_almost_equal(ds_read.array, ds.array)
        os.remove(fname)
        self.assertFalse(os.path.exists(fname))

    def test_override_array(self):
        img = np.ones((self.height, self.width), np.int16)
        img_new = np.array(np.arange(0, 40200).reshape(201, 200), dtype=np.uint8)
        path = os.path.join(os.getcwd(), "test_write_read_geotiff.tif")
        ImageIO.write_geotiff(img, path, self.projection, self.coordinates)
        self.assertTrue(os.path.exists(path))

        ds = GDalDatasetWrapper.from_file(path)
        self.assertTrue((ds.array == img).all())
        self.assertEqual(ds.geotransform, self.coordinates)
        self.assertEqual(ds.projection.replace(" ", ""), self.projection.replace(" ", ""))
        self.assertIsNone(ds.nodata_value)
        self.assertEqual(ds.epsg, 32631)
        ds_new = GDalDatasetWrapper(ds=ds.get_ds(), array=img_new, nodata_value=123)
        FileSystem.remove_file(path)
        np.testing.assert_almost_equal(img_new, ds_new.array)
        self.assertEqual(ds_new.geotransform, self.coordinates)
        self.assertEqual(ds_new.projection.replace(" ", ""), self.projection.replace(" ", ""))
        self.assertEqual(ds_new.nodata_value, 123)
        self.assertEqual(ds_new.epsg, 32631)
        self.assertFalse(os.path.exists(path))

    def test_get_nodata(self):
        expected_nodata = 42.0
        img = np.ones((self.height, self.width), np.int16)
        path = os.path.join(os.getcwd(), "test_get_nodata_init.tif")
        ImageIO.write_geotiff(img, path, self.projection, self.coordinates)
        ds = ImageTools.gdal_buildvrt(path, VRTNodata=expected_nodata)
        self.assertEqual(expected_nodata, ds.nodata_value)
        np.testing.assert_almost_equal(ds.nodata_mask, np.ones_like(img))
        FileSystem.remove_file(path)
        self.assertFalse(os.path.exists(path))

    def test_get_utm_description(self):
        img = np.ones((self.height, self.width), np.int16)
        path = os.path.join(os.getcwd(), "test_get_utm_description.tif")
        ImageIO.write_geotiff(img, path, self.projection, self.coordinates)
        self.assertTrue(os.path.exists(path))
        ds = GDalDatasetWrapper.from_file(path)
        utm = ds.utm_description
        utm_expected = "WGS 84 / UTM zone 31N"
        self.assertEqual(utm_expected, utm)
        FileSystem.remove_file(path)
        self.assertFalse(os.path.exists(path))

    def test_get_resolution(self):
        img = np.ones((self.height, self.width), np.int16)
        path = os.path.join(os.getcwd(), "test_get_resolution.tif")
        ImageIO.write_geotiff(img, path, self.projection, self.coordinates)
        self.assertTrue(os.path.exists(path))

        ds = GDalDatasetWrapper.from_file(path)
        res_expected = (self.coordinates[1], self.coordinates[-1])
        self.assertEqual(res_expected, ds.resolution)

        FileSystem.remove_file(path)
        self.assertFalse(os.path.exists(path))

    def test_get_ul_lr(self):
        img = np.ones((1000, 1000), np.int16)
        path = os.path.join(os.getcwd(), "test_get_ul_lr.tif")
        ImageIO.write_geotiff(img, path, self.projection, self.coordinates)
        self.assertTrue(os.path.exists(path))

        ds = GDalDatasetWrapper.from_file(path)
        ulx, uly, lrx, lry = ds.ul_lr
        self.assertEqual((ulx, uly), (300000.0, 4900020.0))
        self.assertEqual((lrx, lry), (310000.0, 4890020.0))
        self.assertEqual(ds.extent, (300000.0, 4890020.0, 310000.0, 4900020.0))
        FileSystem.remove_file(path)
        self.assertFalse(os.path.exists(path))

    def test_get_s2_epsg_code(self):
        epsg_ref = 32631
        projection = 'PROJCS["WGS 84 / UTM zone 31N",\
                        GEOGCS["WGS 84",DATUM["WGS_1984",\
                        SPHEROID["WGS 84",6378137,298.257223563,\
                        AUTHORITY["EPSG","7030"]],\
                        AUTHORITY["EPSG","6326"]],\
                        PRIMEM["Greenwich",0,\
                        AUTHORITY["EPSG","8901"]],\
                        UNIT["degree",0.0174532925199433,\
                        AUTHORITY["EPSG","9122"]],\
                        AUTHORITY["EPSG","4326"]],\
                        PROJECTION["Transverse_Mercator"],\
                        PARAMETER["latitude_of_origin",0],\
                        PARAMETER["central_meridian",3],\
                        PARAMETER["scale_factor",0.9996],\
                        PARAMETER["false_easting",500000],\
                        PARAMETER["false_northing",0],\
                        UNIT["metre",1,AUTHORITY["EPSG","9001"]],\
                        AXIS["Easting",EAST],AXIS["Northing",NORTH],\
                        AUTHORITY["EPSG","%s"]]' % epsg_ref
        img = np.ones((self.height, self.width), np.int16)
        path = os.path.join(os.getcwd(), "test_epsg.tif")
        ImageIO.write_geotiff(img, path, projection, self.coordinates)
        self.assertTrue(os.path.exists(path))
        ds = GDalDatasetWrapper.from_file(path)
        epsg_new = ds.epsg
        self.assertEqual(epsg_new, epsg_ref)
        FileSystem.remove_file(path)
        self.assertFalse(os.path.exists(path))


if __name__ == '__main__':
    unittest.main()
