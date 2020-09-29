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

import numpy as np
from StartMaja.Common import ImageTools
from StartMaja.Common.GDalDatasetWrapper import GDalDatasetWrapper


def get_ndsi(red, swir, vrange=(-1, 1), dtype=np.float32):
    """
    Calculate the NDSI (Normalized-Difference Snow Index)

    :param red: The red band dataset
    :type red: :class:`Common.GDalDatasetWrapper.GDalDatasetWrapper`
    :param swir: The swir band dataset
    :type swir: :class:`Common.GDalDatasetWrapper.GDalDatasetWrapper`
    :param vrange: The range of output values as tuple. By default: (-1, 1).
    :type vrange: tuple of int
    :param dtype: The output dtype.
    :type dtype: :class`np.dtype`
    :return: The NDSI as numpy array.
    :rtype: :class:`Common.GDalDatasetWrapper.GDalDatasetWrapper`
    """

    if swir.extent != red.extent or swir.epsg != red.epsg:
        raise ValueError("Cannot calculate NDSI on two different extents.")

    if swir.resolution != red.resolution:
        # Resize to swir resolution in this case.
        tr = " ".join([str(i) for i in swir.resolution])
        ds_red = ImageTools.gdal_translate(red, tr=tr, r="cubic")
    else:
        ds_red = red

    # TODO Add new test
    img_red = np.array(ds_red.array, dtype=np.float32)
    img_swir = np.array(swir.array, dtype=np.float32)

    # Compensate for nan:
    np.seterr(divide='ignore', invalid='ignore')
    img_ndsi = np.where((img_red + img_swir) != 0, (img_red - img_swir) / (img_red + img_swir), -1)

    # Scale to vrange
    img_ndsi_scaled = ImageTools.normalize(img_ndsi, value_range_out=vrange, value_range_in=(-1, 1),
                                           dtype=dtype, clip=True)

    return GDalDatasetWrapper(ds=swir.get_ds(), array=img_ndsi_scaled)


def get_ndvi(red, nir, vrange=(-1, 1), dtype=np.float32):
    """
    Calculate the NDVI (Normalized-Difference Vegetation Index)

    :param red: The red band dataset
    :type red: :class:`Common.GDalDatasetWrapper.GDalDatasetWrapper`
    :param nir: The nir band dataset
    :type nir: :class:`Common.GDalDatasetWrapper.GDalDatasetWrapper`
    :param vrange: The range of output values as tuple. By default: (-1, 1).
    :type vrange: tuple of int
    :param dtype: The output dtype.
    :type dtype: :class`np.dtype`
    :return: The NDVI as numpy array.
    :rtype: :class:`Common.GDalDatasetWrapper.GDalDatasetWrapper`
    """

    if nir.extent != red.extent or nir.epsg != red.epsg:
        raise ValueError("Cannot calculate NDSI on two different extents.")

    if nir.resolution != red.resolution:
        # Resize to nir resolution in this case.
        tr = " ".join([str(i) for i in nir.resolution])
        ds_red = ImageTools.gdal_translate(red, tr=tr, r="cubic")
    else:
        ds_red = red

    # TODO Add new test

    img_red = np.array(ds_red.array, dtype=np.float32)
    img_nir = np.array(nir.array, dtype=np.float32)

    # Compensate for nan:
    np.seterr(divide='ignore', invalid='ignore')
    img_ndvi = np.where((img_red + img_nir) != 0, (img_red - img_nir) / (img_red + img_nir), -1)

    # Scale to vrange
    img_ndvi_scaled = ImageTools.normalize(img_ndvi, value_range_out=vrange, value_range_in=(-1, 1),
                                           dtype=dtype, clip=True)

    return GDalDatasetWrapper(ds=nir.get_ds(), array=img_ndvi_scaled)
