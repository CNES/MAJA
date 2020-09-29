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
import logging
import tempfile
from osgeo import gdal
from StartMaja.Common import FileSystem


def run_tiling(ds_or_path, **kwargs):
    """
    Run the tiling process

    :param ds_or_path: The :class:`gdal.Dataset` or a path to a file on disk to be tiled
    :type ds_or_path: :class:`gdal.Dataset` or str
    :param kwargs: Optional arguments.
    :return: Creates the tiles in the given location.
    """
    cmd_dict = {"Verbose": "-v", "Quiet": "-q", "CreationOptions": "-co", "TileSize": "-ps",
                "Overlap": "-overlap", "Format": "-of", "BandType": "-ot", "TileIndexFieldName": "-tileIndexField",
                "TileIndexName": "-tileIndex", "CsvDelimiter": "-csvDelim", "Source_SRS": "-s_srs",
                "TargetDir": "-targetDir", "-ResamplingMethod": "-r", "Levels": "-levels",
                "PyramidOnly": "-pyramidOnly", "UseDirForEachRow": "-useDirForEachRow"}

    gdal.AllRegister()
    gdal.UseExceptions()

    options = []

    if type(ds_or_path) != str:
        tmppath = os.path.join(tempfile.gettempdir(), next(tempfile._get_candidate_names()))
        ds_or_path.write(tmppath)
        del_tmp = True
    else:
        tmppath = ds_or_path
        del_tmp = False
    tx, ty = None, None
    for key in kwargs.keys():
        if key == "TileWidth":
            ty = kwargs[key]
        elif key == "TileHeight":
            tx = kwargs[key]
        else:
            options += [cmd_dict[key], kwargs[key]]
    if not tx or not ty:
        raise ValueError("Must provide tile width and height: (%s, %s)" % (ty, tx))
    options += [cmd_dict["TileSize"], "%s %s" % (ty, tx)]
    options += [tmppath]
    assert FileSystem.run_external_app("gdal_retile.py", options) == 0, "Error running gdal_retile"
    bname = os.path.basename(tmppath)
    outpath = kwargs["TargetDir"]
    # TODO Discard previously written files from `find`:
    files_written = sorted(FileSystem.find(r"%s_\d+_\d+.tif" % bname, path=outpath))
    if del_tmp:
        FileSystem.remove_file(tmppath)
    return files_written


if __name__ == "__main__":
    raise NotImplementedError
else:
    logger = logging.getLogger("root")
