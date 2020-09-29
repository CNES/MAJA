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
#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Copyright (C) CNES - All Rights Reserved
This file is subject to the terms and conditions defined in
file 'LICENSE.md', which is part of this source code package.

Author:         Peter KETTIG <peter.kettig@cnes.fr>
"""

import os
import osgeo
from osgeo import gdal, gdal_array, osr
import numpy as np

gdal.UseExceptions()


def open_tiff(raster_file):
    """
    Opens a (tiff-):class:`gdal.Dataset` from a file path.

    :param raster_file: Path to the given geo-referenced file
    :return: A :class:`gdal.Dataset` object
    """
    if os.path.exists(raster_file):
        return gdal.Open(raster_file)
    raise NameError("GDAL could not open file {0}".format(raster_file))


def tiff_to_array(raster_file, lon_offset_px=0, lat_offset_px=0, array_only=True, bands_last=True):
    """
    Opens a (tiff-)dataset and extracts the array.
    Note: Bands index starts at 1, not at 0

    :param raster_file: The File path
    :param lon_offset_px: Offset for image in x-Direction
    :param lat_offset_px: Offset for image in y-Direction
    :param array_only: If `False`, then also return the :class:`gdal.Dataset` object.
    :param bands_last: f `True`, eturn array of shape (y, x, bands) instead of (bands, y, x)
    :return: The numpy array as well as the driver if `array_only=False`.
    """
    gdo = open_tiff(raster_file)
    tiff_array = np.array(gdo.ReadAsArray(lon_offset_px, lat_offset_px))
    if bands_last:
        if tiff_array.ndim == 3:
            tiff_array = np.moveaxis(tiff_array, 0, -1)
        elif tiff_array.ndim == 2:
            # tiff_array = tiff_array[..., np.newaxis]
            # This is ok.
            pass
        else:
            raise ValueError("Axis number must be 2 or 3.")
    if array_only:
        return tiff_array
    return tiff_array, gdo


def gdal_write(drv_type, img, dst, projection, coordinates, **kwargs):
    """
    General GDal Write function.
    Writes array `img` to `dst`.

    :param drv_type: The Driver type. E.g. "GTiff" or "MEM"
    :type drv_type: str
    :param img: The :class:`numpy.ndarray`
    :param dst: The destination path
    :param projection: A Gdal projection
    :type projection: str
    :param coordinates: A list of floats for the geotransform. See the description in :func:`write_geotiff`
    :type coordinates: List of float
    :keyword dtype: The desired gdal dtype
    :keyword nodata: Assign a nodata value
    :keyword options: Config options as a list of str, such as ["COMPRESS=DEFLATE"]
    :return: Writes array to `dst` and returns the dataset.
    :rtype: :class:`gdal.Dataset`
    """
    dtype = kwargs.get("dtype", None)
    nodata = kwargs.get("nodata", None)
    options = kwargs.get("options", [])
    driver = gdal.GetDriverByName(drv_type)
    # Add dimension for a single band-image
    if img.ndim == 2:
        img = img[..., np.newaxis]
    # Set output dtype if not specified. GDAL cannot write GTiff as binary files, so convert to uint8:
    if img.dtype == np.bool:
        dtype = gdal_array.NumericTypeCodeToGDALTypeCode(np.uint8)
    if img.dtype == np.int64:
        dtype = gdal_array.NumericTypeCodeToGDALTypeCode(np.int32)
    if not dtype:
        dtype = gdal_array.NumericTypeCodeToGDALTypeCode(img.dtype)
    img_h, img_w, n_bands = img.shape
    mem = driver.Create(dst, img_w, img_h, n_bands, dtype, options)
    mem.SetGeoTransform(coordinates)
    mem.SetProjection(projection)

    for bandIdx in range(n_bands):
        band = img[:, :, bandIdx]
        raster_band = mem.GetRasterBand(bandIdx + 1)
        raster_band.WriteArray(band)
        if nodata:
            raster_band.SetNoDataValue(nodata)
    mem.FlushCache()
    return mem


def write_to_memory(img, dst, projection, coordinates, **kwargs):
    """

    Writes an in-memory dataset using a given projection and geotransform

    :param img: The numpy array to write
    :param dst: The destination path
    :param projection: The gdal projection as `str`
    :param coordinates: The geotransform [Top-Left X, W-E Resolution, 0, Top Left Y, 0, N-S Resolution]
    :keyword dtype: The desired gdal dtype
    :keyword nodata: Assign a nodata value
    :keyword options: Config options as a list of str, such as ["COMPRESS=DEFLATE"]
    :return: Writes image to given path. Returns 0 if all went well, 1 if not
    :rtype: int
    """
    kwargs.pop("driver", None)
    return gdal_write("MEM", img, dst, projection, coordinates, **kwargs)


def write_geotiff(img, dst, projection, coordinates, **kwargs):
    """

    Writes a GeoTiff file using a given projection and geotransform

    :param img: The numpy array to write
    :param dst: The destination path
    :param projection: The gdal projection as `str`
    :param coordinates: The geotransform [Top-Left X, W-E Resolution, 0, Top Left Y, 0, N-S Resolution]
    :keyword dtype: The desired gdal dtype
    :keyword nodata: Assign a nodata value
    :keyword options: Config options as a list of str, such as ["COMPRESS=DEFLATE"]
    :return: Writes image to given path. Returns 0 if all went well, 1 otherwise.
    :rtype: int
    """
    kwargs.pop("driver", None)
    ds = gdal_write("GTiff", img, dst, projection, coordinates, **kwargs)
    if ds is not None:
        ds = None
        return 0
    return 1


def write_geotiff_existing(img, dst, ds, **kwargs):
    """

    Create a GeoTiff image with info from an existing dataset

    :param img: The numpy array to write
    :param dst: The destination path
    :param ds: A gdal dataset
    :keyword dtype: The desired gdal dtype
    :keyword nodata: Assign a nodata value
    :keyword options: Config options as a list of str, such as ["COMPRESS=DEFLATE"]
    :return: Writes image to given path. Returns 0 if all went well, 1 otherwise.
    :rtype: int
    """
    geotransform = ds.GetGeoTransform()
    projection = ds.GetProjection()
    # Write the new array
    return write_geotiff(img, dst, projection, geotransform, **kwargs)


def transform_point(point, old_epsg, new_epsg=4326):
    """
    Transform a tuple (x,y) (or lon/lat) to a different EPSG coordinate reference system.
    For all other transformations apart from EPSG->EPSG, gdal's API has to be used directly.
    Note: This works only with 2D points (x,y) - Z is omitted in the output.

    :param point: The point as tuple (x,y)
    :param old_epsg: The EPSG code of the old coordinate system
    :param new_epsg: The EPSG code of the new coordinate system to transfer to. Default is 4326 (WGS84).
    :return: The point's location in the new epsg as (lon/lat) - z is omitted due to it being 0 most of the time
    """

    source = osr.SpatialReference()
    source.ImportFromEPSG(old_epsg)
    # The target projection
    target = osr.SpatialReference()
    target.ImportFromEPSG(new_epsg)
    if int(osgeo.__version__[0]) >= 3:
        # GDAL 3 changes axis order: https://github.com/OSGeo/gdal/issues/1546
        source.SetAxisMappingStrategy(osgeo.osr.OAMS_TRADITIONAL_GIS_ORDER)
        target.SetAxisMappingStrategy(osgeo.osr.OAMS_TRADITIONAL_GIS_ORDER)
    transform = osr.CoordinateTransformation(source, target)
    new_pt = transform.TransformPoint(point[0], point[1])
    return new_pt[0], new_pt[1]
