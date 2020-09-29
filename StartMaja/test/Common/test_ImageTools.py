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
# -*- coding: utf-8 -*-

import unittest
import os
from StartMaja.Common import ImageTools, ImageIO, FileSystem
from StartMaja.Common.GDalDatasetWrapper import GDalDatasetWrapper
import numpy as np


class TestImageTools(unittest.TestCase):

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

    def test_extract_class_threshold(self):
        mask = np.arange(0, 25).reshape(5, 5)
        class_to_extract = 10
        value_type = "threshold"
        expected = np.array([[0, 0, 0, 0, 0],
                             [0, 0, 0, 0, 0],
                             [1, 1, 1, 1, 1],
                             [1, 1, 1, 1, 1],
                             [1, 1, 1, 1, 1]])
        calculated = ImageTools.extract_class(mask, class_to_extract, value_type)
        np.testing.assert_array_almost_equal(expected, calculated)

    def test_extract_class_value(self):
        mask = np.arange(0, 9).reshape(3, 3)
        class_to_extract = [7, 8, 11]
        value_type = "value"
        expected = np.array([[0, 0, 0],
                             [0, 0, 0],
                             [0, 1, 1]])
        calculated = ImageTools.extract_class(mask, class_to_extract, value_type)
        np.testing.assert_array_almost_equal(expected, calculated)

    def test_extract_class_bit(self):
        mask = np.arange(0, 9).reshape(3, 3)
        class_to_extract = [3]
        value_type = "bit"
        expected = np.array([[0, 0, 0],
                             [0, 0, 0],
                             [0, 0, 1]])
        calculated = ImageTools.extract_class(mask, class_to_extract, value_type)
        np.testing.assert_array_almost_equal(expected, calculated)

    def test_normalize(self):
        img = np.arange(0, 9).reshape(3, 3)
        value_range_in = (0, 10)
        value_range_out = (0, 1)
        expected = np.array([[0, .1, .2],
                             [.3, .4, .5],
                             [.6, .7, .8]])
        calculated = ImageTools.normalize(img,
                                          value_range_in=value_range_in,
                                          value_range_out=value_range_out)
        np.testing.assert_array_almost_equal(expected, calculated)

    def test_normalize_clip(self):
        img = np.arange(0, 9).reshape(3, 3)
        value_range_in = (0, 5)
        value_range_out = (0, 1)
        expected = np.array([[0, .2, .4],
                             [.6, .8, 1],
                             [1, 1, 1]])
        calculated = ImageTools.normalize(img,
                                          value_range_in=value_range_in,
                                          value_range_out=value_range_out,
                                          clip=True)
        np.testing.assert_array_almost_equal(expected, calculated)

    def test_normalize_0to255(self):
        img = np.arange(-4, 5).reshape(3, 3) / 5
        value_range_in = (-1, 1)
        value_range_out = (0, 255)
        expected = np.array([[25.5, 51.,  76.5],
                             [102., 127.5, 153.],
                             [178.5, 204., 229.5]])
        calculated = ImageTools.normalize(img,
                                          value_range_in=value_range_in,
                                          value_range_out=value_range_out)
        np.testing.assert_array_almost_equal(expected, calculated)

    def test_gdal_buildvrt(self):
        path = os.path.join(os.getcwd(), "test_gdal_buildvrt.tif")
        vrt = os.path.join(os.getcwd(), "test_vrt.vrt")
        img = np.arange(-4, 5).reshape(3, 3) / 5

        ImageIO.write_geotiff(img, path, self.projection, self.coordinates)
        self.assertTrue(os.path.exists(path))
        driver = ImageTools.gdal_buildvrt(path, dst=vrt)
        self.assertTrue(os.path.exists(vrt))
        np.testing.assert_almost_equal(driver.array, img)
        FileSystem.remove_file(vrt)
        FileSystem.remove_file(path)

    def test_gdal_translate(self):
        img = np.ones((self.height, self.width, 2), np.int16)
        path = os.path.join(os.getcwd(), "test_gdal_translate.tif")
        scaled = os.path.join(os.getcwd(), "scaled.tif")
        out_resolution = (20, -20)
        ImageIO.write_geotiff(img, path, self.projection, self.coordinates)
        self.assertTrue(os.path.exists(path))
        ds = ImageTools.gdal_translate(path, scaled,
                                       tr=" ".join([str(i) for i in out_resolution]),
                                       scale="0 1 0 255")
        self.assertEqual(ds.resolution, out_resolution)
        self.assertEqual(ds.array.shape, (self.height // 2, self.width // 2, 2))
        np.testing.assert_almost_equal(ds.array, 255)
        FileSystem.remove_file(path)
        FileSystem.remove_file(scaled)

    def test_gdal_warp(self):
        img = np.ones((self.height, self.width, 2), np.int16)
        img_rescaled = np.ones((int(self.height/2), int(self.width/2), 2), np.int16)
        out_resolution = (20, -20)
        path = os.path.join(os.getcwd(), "test_gdal_warp.tif")
        scaled = os.path.join(os.getcwd(), "res_changed.tif")
        ImageIO.write_geotiff(img, path, self.projection, self.coordinates)
        self.assertTrue(os.path.exists(path))
        ds = ImageTools.gdal_warp(path, scaled,
                                  tr=" ".join(str(e) for e in out_resolution),
                                  r="max", q=True)
        self.assertTrue(os.path.isfile(scaled))
        self.assertEqual(ds.resolution, out_resolution)
        self.assertEqual(ds.array.shape, (self.height // 2, self.width // 2, 2))
        np.testing.assert_almost_equal(ds.array, img_rescaled)
        FileSystem.remove_file(path)
        FileSystem.remove_file(scaled)

    def test_gdal_buildvrt_concatenate(self):
        from StartMaja.Common import FileSystem
        paths = []
        for i in range(1, 3, 1):
            img = np.ones((i, i, 2), np.int16) * i
            path = os.path.join(os.getcwd(), "test_gdal_merge_%s.tif" % i)
            ImageIO.write_geotiff(img, path, self.projection, self.coordinates)
            self.assertTrue(os.path.exists(path))
            paths.append(path)
        empty = os.path.join(os.getcwd(), "empty.vrt")
        driver = ImageTools.gdal_buildvrt(*paths, dst=empty,
                                          separate=True,
                                          srcnodata=0)
        expected = np.array([[[1, 0],
                              [0, 0]],
                             [[2, 2],
                              [2, 2]]], dtype=np.int16)

        np.testing.assert_almost_equal(driver.array, expected)
        self.assertEqual(driver.nodata_value, 0)
        self.assertEqual(driver.epsg, 32631)
        [FileSystem.remove_file(path) for path in paths]
        FileSystem.remove_file(empty)
        [self.assertFalse(os.path.exists(path)) for path in paths]
        self.assertFalse(os.path.exists(empty))

    def test_gdal_tile_untile(self):
        img = np.arange(0., 100.).reshape((10, 10))
        path = os.path.join(os.getcwd(), "test_gdal_retile.tif")
        tile_folder = os.path.join(os.getcwd(), "tiled")
        ImageIO.write_geotiff(img, path, self.projection, self.coordinates)
        # Add parasitic file - It should not cause problems
        path_parasite = os.path.join(tile_folder, "tile_01_01.tif")
        FileSystem.create_directory(tile_folder)
        ImageIO.write_geotiff(img, path_parasite, self.projection, self.coordinates)
        ds_in = GDalDatasetWrapper.from_file(path)
        self.assertTrue(os.path.exists(path))
        tiles = ImageTools.gdal_retile(ds_in, tile_folder,
                                       TileWidth=2,
                                       TileHeight=2,
                                       Overlap=1)
        self.assertTrue(os.path.isdir(tile_folder))
        self.assertEqual(len(tiles), 81)
        img_read = np.array(ImageIO.tiff_to_array(tiles[-1]))
        expected = np.array([[88, 89],
                             [98, 99]])
        # Some gdal_retile versions are producing the following image:
        # [[87, 89], [97, 99]].
        np.testing.assert_allclose(expected, img_read, atol=1)

        # Untile
        ds_untiled = ImageTools.gdal_buildvrt(*tiles)
        np.testing.assert_allclose(img, ds_untiled.array, atol=1)
        FileSystem.remove_file(path)
        FileSystem.remove_directory(tile_folder)

    def test_merge_then_translate(self):
        datasets = []
        init = np.zeros((2, 2), np.int16)
        path = os.path.join(os.getcwd(), "test_gdal_merge.tif")
        ImageIO.write_geotiff(init, path, self.projection, self.coordinates)
        ds_in = GDalDatasetWrapper.from_file(path)
        for i in range(1, 3, 1):
            img = np.ones((i*2, i*2), np.int16) * i
            ds_n = GDalDatasetWrapper(ds=ds_in.get_ds(), array=img)
            self.assertTrue(os.path.exists(path))
            datasets.append(ds_n)
        ds_merged = ImageTools.gdal_merge(*datasets, dst="out.tif",
                                          separate=True,
                                          q=True,
                                          a_nodata=0)
        # Array of shape (4, 4, 2):
        expected = np.array([[[1, 2],
                              [1, 2],
                              [0, 2],
                              [0, 2]],
                             [[1, 2],
                              [1, 2],
                              [0, 2],
                              [0, 2]],
                             [[0, 2],
                              [0, 2],
                              [0, 2],
                              [0, 2]],
                             [[0, 2],
                              [0, 2],
                              [0, 2],
                              [0, 2]]], dtype=np.int16)
        FileSystem.remove_file("out.tif")
        np.testing.assert_equal(expected.dtype, ds_merged.array.dtype)
        np.testing.assert_almost_equal(expected, ds_merged.array)
        self.assertEqual(ds_merged.nodata_value, 0)
        self.assertEqual(ds_merged.epsg, 32631)
        ds_translate = ImageTools.gdal_translate(ds_merged,
                                                 a_nodata=0)
        FileSystem.remove_file(path)

        np.testing.assert_equal(expected.dtype, ds_translate.array.dtype)
        np.testing.assert_almost_equal(ds_translate.array, expected)
        self.assertEqual(ds_translate.nodata_value, 0)
        self.assertEqual(ds_translate.epsg, 32631)

    def test_gdal_merge_optfile(self):
        datasets, written = [], []
        init = np.zeros((2, 2), np.int16)
        path = os.path.join(os.getcwd(), "test_gdal_merge_optfile.tif")
        ImageIO.write_geotiff(init, path, self.projection, self.coordinates)
        ds_in = GDalDatasetWrapper.from_file(path)
        for i in range(1, 3, 1):
            img = np.ones((i*2, i*2), np.int16) * i
            ds_n = GDalDatasetWrapper(ds=ds_in.get_ds(), array=img)
            p_out = "test_gdal_merge_optfile_%s.tif" % i
            ImageIO.write_geotiff_existing(img, p_out, ds_in.get_ds())
            self.assertTrue(os.path.exists(path))
            datasets.append(ds_n)
            written.append(p_out)
        optfile = "test_gdal_merge_optfile.txt"
        with open(optfile, 'w') as file_handler:
            for item in written:
                file_handler.write("{}\n".format(item))

        ds_merged = ImageTools.gdal_merge(*datasets,
                                          q=True,
                                          a_nodata=0)
        ds_optfile = ImageTools.gdal_merge(optfile=optfile,
                                           q=True,
                                           a_nodata=0)
        # Array of shape (4, 4):
        expected = np.array([[2, 2, 2, 2],
                             [2, 2, 2, 2],
                             [2, 2, 2, 2],
                             [2, 2, 2, 2]], dtype=np.int16)

        FileSystem.remove_file(path)
        [FileSystem.remove_file(p) for p in written]
        FileSystem.remove_file(optfile)
        np.testing.assert_equal(expected.dtype, ds_merged.array.dtype)
        np.testing.assert_almost_equal(expected, ds_merged.array)
        self.assertEqual(ds_merged.nodata_value, 0)
        self.assertEqual(ds_merged.epsg, 32631)
        np.testing.assert_equal(expected.dtype, ds_optfile.array.dtype)
        np.testing.assert_almost_equal(expected, ds_optfile.array)
        self.assertEqual(ds_optfile.nodata_value, 0)
        self.assertEqual(ds_optfile.epsg, 32631)


if __name__ == '__main__':
    unittest.main()
