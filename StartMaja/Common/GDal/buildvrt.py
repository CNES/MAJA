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
from StartMaja.Common import FileSystem
from StartMaja.Common.GDalDatasetWrapper import GDalDatasetWrapper


def gdal_buildvrt(*inputs, dst=None, **options):
    """
    Build a gdalvrt using in memory bindings.

    :param inputs: The list of input filenames or datasets.
    :param dst: If specified, the vrt will be writen to
    the given path on a physical disk.
    Note: This overwrites a previous vrt at the same location.
    :return: VRT of the given inputs, by default as an in-memory file.
    :type: `osgeo.gdal.dataset` or .vrt on disk (see param ``dst``).
    """
    gdal_common_params = ["optfile"]
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

    inputs = [i.get_ds() if type(i) == GDalDatasetWrapper else i for i in inputs]
    # Remove previous existing file if writing to disk is enabled:
    if dst:
        FileSystem.remove_file(dst)
        # Note: De-allocation before file is actually written to disk
        # cf. https://gdal.org/api/python_gotchas.html
        _ = gdal.BuildVRT(dst, [i for i in inputs], options=options_list)
        _ = None
        ds_out = gdal.Open(dst)
    else:
        # Write directly into memory if no path specified (faster):
        dst = "/vsimem/" + uuid.uuid4().hex
        ds_out = gdal.BuildVRT(dst, [i for i in inputs], options=options_list)
    return GDalDatasetWrapper(ds=ds_out)
