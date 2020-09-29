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

import os
import tempfile
import logging
from osgeo import gdal
from StartMaja.Common.FileSystem import run_external_app
from StartMaja.Common.FileSystem import remove_directory

def run_merge(*src, argv=None):
    """
    Run gdal_merge using an external process.

    :param src: The input filenames or GDalDatasetWrapper-objects
    :param argv: The arguments as list, e.g. ["-o", "/path/to/dst"]
    :return: A gdal.Dataset object
    :rtype: ``osgeo.gdal.Dataset``
    """
    tmpdir = tempfile.mkdtemp()
    inputs = []
    for i, drv in enumerate(src):
        if type(drv) != str:
            tmppath = os.path.join(tmpdir, "input_%s.tif" % i)
            drv.write(tmppath)
            inputs.append(tmppath)
        else:
            inputs.append(src)

    if "-o" in argv:
        outpath = argv[argv.index("-o") + 1]
    else:
        outpath = os.path.join(tempfile.gettempdir(), "%s.tif" % next(tempfile._get_candidate_names()))
        logger.debug("Writing to file %s" % outpath)
        argv += ["-o", outpath]
    argv = gdal.GeneralCmdLineProcessor(argv)
    options = argv + inputs
    assert run_external_app("gdal_merge.py", options) == 0, "Error running gdal_merge"
    remove_directory(tmpdir)
    return gdal.Open(outpath)


if __name__ == "__main__":
    raise NotImplementedError
else:
    logger = logging.getLogger("root")
