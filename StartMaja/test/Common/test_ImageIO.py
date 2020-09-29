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
from osgeo import gdal
from StartMaja.Common import ImageIO
from StartMaja.Common import FileSystem


class TestImageIO(unittest.TestCase):

    def setUp(self):
        self.coordinates = (652594.9112913811, 10.00887639510383, 0,
                            5072876.717295351, 0, -9.974893672262251)
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

        arr, ds = ImageIO.tiff_to_array(path, array_only=False)
        self.assertTrue((arr == img).all())
        self.assertEqual(ds.GetGeoTransform(), self.coordinates)
        # Compare projections by removing all spaces cause of multiline string
        self.assertEqual(ds.GetProjection().replace(" ", ""), self.projection.replace(" ", ""))
        FileSystem.remove_file(path)
        self.assertFalse(os.path.exists(path))

    def test_write_read_geotiff_kwoptions(self):
        img = np.ones((self.height, self.width), np.int16)
        nodata = 42
        path = os.path.join(os.getcwd(), "test_write_read_geotiff.tif")
        ImageIO.gdal_write("GTiff", img, path, self.projection, self.coordinates,
                           options=["COMPRESS=DEFLATE"], nodata=nodata)
        self.assertTrue(os.path.exists(path))
        arr, ds = ImageIO.tiff_to_array(path, array_only=False)
        self.assertTrue((arr == img).all())
        self.assertEqual(nodata, gdal.Info(ds, format="json")["bands"][0]["noDataValue"])
        self.assertEqual(gdal.Info(ds, format="json")["metadata"]["IMAGE_STRUCTURE"]["COMPRESSION"], "DEFLATE")
        self.assertEqual(ds.GetGeoTransform(), self.coordinates)
        # Compare projections by removing all spaces cause of multiline string
        self.assertEqual(ds.GetProjection().replace(" ", ""), self.projection.replace(" ", ""))
        FileSystem.remove_file(path)
        self.assertFalse(os.path.exists(path))

    def test_write_read_memory(self):
        img = np.ones((self.height, self.width), np.int16)
        path = "/vsimem/test_write_read_memory.tif"
        ds = ImageIO.write_to_memory(img, path, self.projection, self.coordinates)

        arr = ds.ReadAsArray()
        self.assertTrue((arr == img).all())
        self.assertEqual(ds.GetGeoTransform(), self.coordinates)
        # Compare projections by removing all spaces cause of multiline string
        self.assertEqual(ds.GetProjection().replace(" ", ""), self.projection.replace(" ", ""))
        ds = None  # Always remember to dereference :)
        FileSystem.remove_file(path)

    def test_faulty_geotransform_projection(self):
        coordinates = (652594.9112913811, 10.00887639510383, 0,
                       5072876.717295351, 0)  # Missing one value
        projection = ''  # Empty
        height = 200
        width = 100
        img = np.ones((height, width), np.int16)
        path = os.path.join(os.getcwd(), "test_faulty_geotransform_projection.tif")
        # Check geotransform wrong:
        with self.assertRaises(TypeError):
            ImageIO.write_geotiff(img, path, projection, coordinates)
        FileSystem.remove_file(path)
        self.assertFalse(os.path.exists(path))

    def test_transform_point_lat_lon(self):
        center = (653095.355, 5071879.228)
        lon_lat_expected = (4.9694934619557145, 45.78349724618419)
        lon_lat_calc = ImageIO.transform_point(center, old_epsg=32631, new_epsg=4326)
        np.testing.assert_almost_equal(lon_lat_calc, lon_lat_expected)


if __name__ == '__main__':
    unittest.main()
