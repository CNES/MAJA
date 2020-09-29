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

from osgeo import gdal
import uuid
import numpy as np
from StartMaja.Common import FileSystem
from StartMaja.Common.GDalDatasetWrapper import GDalDatasetWrapper


def gdal_translate(src, dst=None, **options):
    """
    Translate a dataset.

    :param src: The input filename or dataset.
    :param dst: If specified, the output will be writen to
    the given path on a physical disk.
    Note: This overwrites a previous file at the same location.
    :return: A :class:`Common.GDalDatasetWrapper.GDalDatasetWrapper` object
    :rtype: `osgeo.gdal.dataset` or file on disk (see parameter ``dst``).
    """
    gdal_common_params = ["optfile", "config", "debug"]
    options_list = []
    for k, v in options.items():
        if k in gdal_common_params:
            options_list += ["--%s" % k, "%s" % v]
        elif type(v) is not bool:
            options_list += ["-%s" % k, "%s" % v]
        elif type(v) is bool and v is True:
            options_list.append("-%s" % k)
        else:
            pass
    options_list = " ".join(options_list)

    if type(src) == GDalDatasetWrapper:
        src = src.get_ds()
    # Remove previous existing file if writing to disk is enabled:
    if dst:
        FileSystem.remove_file(dst)
        # Note: De-allocation before file is actually written to disk
        # cf. https://gdal.org/api/python_gotchas.html
        _ = gdal.Translate(dst, src, options=options_list)
        _ = None
        ds_out = gdal.Open(dst)
    else:
        # Write directly into memory if no path specified (faster):
        dst = "/vsimem/" + uuid.uuid4().hex
        ds_out = gdal.Translate(dst, src, options=options_list)
    arr_bands_last = np.array(ds_out.ReadAsArray())
    if arr_bands_last.ndim == 3:
        arr_bands_last = np.moveaxis(arr_bands_last, 0, -1)
    return GDalDatasetWrapper(ds=ds_out, array=arr_bands_last)
