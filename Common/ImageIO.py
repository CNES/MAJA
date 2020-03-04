#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Copyright (C) CNES, CS-SI, CESBIO - All Rights Reserved
This file is subject to the terms and conditions defined in
file 'LICENSE.md', which is part of this source code package.

Author:         Peter KETTIG <peter.kettig@cnes.fr>,
Project:        Start-MAJA, CNES
"""


def open_tiff(tiff_file):
    """
    @brief: Opens a tiff file
    @param tiff_file The filepath of the .tiff
    @return A Gdal format file
    """
    from osgeo import gdal
    import os
    if os.path.exists(tiff_file):
        return gdal.Open(tiff_file)
    else:
        raise NameError("GDAL could not open file {0}".format(tiff_file))


def tiff_to_array(tiff_file, lon_offset_px=0, lat_offset_px=0, array_only=True, bands_last=False):
    """
    Transforms tiff file into an array.
    Note: Bands index starts at 1, not at 0
    :param tiff_file: The File path
    :param lon_offset_px: Offset for image in x-Direction
    :param lat_offset_px: Offset for image in y-Direction
    :param array_only: Numpy.array with shape (Bands,y-Index,x-Index)
    :param bands_last: Force creation of 3d arrays with the bands as last shape.
                       If array is 2d, the 3rd dim will be 1.
    :return:
    """
    import numpy as np
    gdo = open_tiff(tiff_file)
    tiff_array = np.array(gdo.ReadAsArray(lon_offset_px, lat_offset_px))
    if array_only:
        gdo = None
    if bands_last:
        if tiff_array.ndim == 3:
            tiff_array = np.moveaxis(tiff_array, 0, -1)
        elif tiff_array.ndim == 2:
            tiff_array = tiff_array[..., np.newaxis]
    return tiff_array, gdo


def write_geotiff(img, dst, projection, coordinates, dtype=None):
    """
    Writes a GeoTiff file created from an existing coordinate-system
    :param img: The numpy array to write
    :param dst: The destination path
    :param projection: The gdal projection
    :param coordinates: The geotransform [Top-Left X, W-E Resolution, 0, Top Left Y, 0, N-S Resolution]
    :param dtype: The desired gdal dtype
    :return: Writes image to given path.
    """
    from osgeo import gdal, gdal_array
    import numpy as np
    driver = gdal.GetDriverByName('GTiff')
    # Add dimension for a single band-image
    if img.ndim is 2:
        img = img[..., np.newaxis]
    # Set output dtype if not specified. GDAL cannot write GTiff as binary files, so convert to uint8:
    if img.dtype == np.bool:
        dtype = gdal_array.NumericTypeCodeToGDALTypeCode(np.uint8)
    if img.dtype == np.int64:
        dtype = gdal_array.NumericTypeCodeToGDALTypeCode(np.int32)
    if not dtype:
        dtype = gdal_array.NumericTypeCodeToGDALTypeCode(img.dtype)
    dataset = driver.Create(dst, img.shape[1], img.shape[0], img.shape[2], dtype)
    if not dataset:
        raise OSError("GDAL Could not create file {0}".format(dst))
    if not coordinates or len(coordinates) != 6:
        raise ValueError("Geotransform empty or unknown: {0}".format(coordinates))
    dataset.SetGeoTransform(coordinates)  
    if not projection:
        raise ValueError("Projection empty or unknown: {0}".format(projection))
    dataset.SetProjection(projection)
    for index in range(img.shape[2]):
        dataset.GetRasterBand(index+1).WriteArray(img[:, :, index])
    dataset.FlushCache()  # Write to disk.


def write_geotiff_existing(img, dst, driver, dtype=None):
    """
    Create a GeoTiff image with an existing driver
    """
    geotransform = driver.GetGeoTransform()
    projection = driver.GetProjection()
    # Write the new array
    return write_geotiff(img, dst, projection, geotransform, dtype)


def get_epsg(driver):
    """
    Get the EPSG code from a given driver using gdal
    :param driver: The gdal driver
    :return: The EPSG code if existing.
    """
    from osgeo import gdal
    import re
    return int(re.findall(r"\d+",
                          gdal.Info(driver, format='json')['coordinateSystem']['wkt'].rsplit('"EPSG",', 1)[-1])[0])


def get_nodata_value(driver):
    """
    Get the NoDataValue (if existing) from a given driver using gdal
    :param driver: The gdal driver
    :return: The NoDataValue if existing. None if not.
    """
    from osgeo import gdal
    try:
        nodata = float(gdal.Info(driver, format='json')["bands"][0]["noDataValue"])
    except KeyError:
        nodata = None
    return nodata


def get_utm_description(driver):
    """
    Get the UTM Description of a given driver using gdal
    :param driver: The gdal driver
    :return: The UTM Description as string.
    """
    from osgeo import gdal
    if int(gdal.VersionInfo()) >= 3000000:
        return gdal.Info(driver, format='json')['coordinateSystem']['wkt'].rsplit('PROJCRS["', 1)[-1].split('"')[0]
    # 'PROJCS' vs. 'PROJCRS' in rsplit
    return gdal.Info(driver, format='json')['coordinateSystem']['wkt'].rsplit('PROJCS["', 1)[-1].split('"')[0]


def get_ul_lr(driver):
    """
    Get the coordinates of the upper left and lower right as tuples
    :param driver: The gdal driver
    :return: The ul and lr- coordinates in the projected coordinate system.
    """
    ulx, xres, xskew, uly, yskew, yres = driver.GetGeoTransform()
    lrx = ulx + (driver.RasterXSize * xres)
    lry = uly + (driver.RasterYSize * yres)
    return (ulx, uly), (lrx, lry)


def get_resolution(driver):
    """
    Get the resolution of a given driver in x and y
    :param driver: The gdal driver
    :return: The x- and y-resolution in meters
    """
    _, xres, _, _, _, yres = driver.GetGeoTransform()
    return xres, yres


def transform_point(point, old_epsg, new_epsg=4326):
    """
    Transform a tuple (x,y) into lat lon using EPSG 4326
    :param point: The point as tuple (x,y)
    :param old_epsg: The EPSG code of the old coordinate system
    :param new_epsg: The EPSG code of the new coordinate system to transfer to. Default is 4326 (WGS84).
    :return: The point's location in the new epsg as (x, y) - z is omitted due to it being 0 most of the time
    """
    from osgeo import osr, gdal
    source = osr.SpatialReference()
    source.ImportFromEPSG(old_epsg)

    # The target projection
    target = osr.SpatialReference()
    target.ImportFromEPSG(new_epsg)
    transform = osr.CoordinateTransformation(source, target)
    new_pt = transform.TransformPoint(point[0], point[1])
    if int(gdal.VersionInfo()) >= 3000000:
        return new_pt[0], new_pt[1]
    return new_pt[1], new_pt[0]


def get_s2_epsg_code(drv):
    """
    Get the Sentinel-2 EPSG Code for the specified driver.
    The codes range from 32601..60 and 32701..60
    :param drv: The gdal driver
    :return: The EPSG-Code as string. E.g. '32630'
    """
    # TODO Unittest
    ul, lr = get_ul_lr(drv)
    epsg_old = get_epsg(drv)
    if epsg_old != "4326":
        lat, lon = transform_point(ul, epsg_old)
    else:
        lat, lon = ul
    lon_mod = int(lon / 6)

    lon_code = str(30 + lon_mod if lon < 0 else 31 - lon_mod).zfill(2)
    epsg = "327" if lat < 0 else "326"
    return epsg + lon_code


def gdal_buildvrt(vrtpath, *inputs, **options):
    """
    Build a gdalvrt using a subprocess.
    :param vrtpath: The path to build the vrt to.
    :param inputs: The list of inputs + other options if needed.
    :return: Builds vrt at the given path.
    """
    import os
    from Common import FileSystem
    if os.path.exists(vrtpath):
        FileSystem.remove_file(vrtpath)
    file_list = [vrtpath]
    options_list = []
    [options_list.extend(["-" + k, v])
     if type(v) is not bool else
     options_list.extend(["-" + k])
     for k, v in options.items()]
    # Append overwrite by default in order to avoid writing errors:
    options_list += ["-overwrite"]
    return FileSystem.run_external_app("gdalbuildvrt", file_list + list(inputs) + options_list)


def gdal_merge(dst, *src, **options):
    """
    Merge a set of gdal rasters
    :param src: The source filename
    :param dst: The destination filename
    :param options: Optional arguments such as 'init' or 'createonly'
    :return: A merged raster is written to the destination filename
    """
    import os
    from Common import FileSystem
    if os.path.exists(dst):
        FileSystem.remove_file(dst)
    file_list = ["-o", dst] + list(src)
    options_list = []
    [options_list.extend(["-" + k, v])
     if type(v) is not bool else
     options_list.extend(["-" + k])
     for k, v in options.items()]
    return FileSystem.run_external_app("gdal_merge.py", file_list + options_list)


def gdal_translate(dst, src, **options):
    """
    Merge a set of gdal rasters
    :param src: The source filename
    :param dst: The destination filename
    :param options: Optional arguments such as 'scale' or 'projwin'
    :return: A raster is written to the destination filename
    """
    import os
    from Common import FileSystem
    if os.path.exists(dst):
        FileSystem.remove_file(dst)
    file_list = [src, dst]
    options_list = []
    [options_list.extend(["-" + k, v])
     if type(v) is not bool else
     options_list.extend(["-" + k])
     for k, v in options.items()]
    return FileSystem.run_external_app("gdal_translate", options_list + file_list)


def gdal_warp(dst, src, **options):
    """
    Warp a set of gdal rasters
    :param src: The source filename
    :param dst: The destination filename
    :param options: Optional arguments such as 't_srs' or 'crop_to_cutline'
    :return: A raster is written to the destination filename
    """
    import os
    from Common import FileSystem
    if os.path.exists(dst):
        FileSystem.remove_file(dst)

    file_list = [src, dst]
    options_list = []
    [options_list.extend(["-" + k, v])
     if type(v) is not bool else
     options_list.extend(["-" + k])
     for k, v in options.items()]
    # Append overwrite by default in order to avoid writing errors:
    options_list += ["-overwrite"]
    return FileSystem.run_external_app("gdalwarp", options_list + file_list)


def gdal_calc(dst, calc, *src, **options):
    """
    Warp a set of gdal rasters
    :param src: The source filename
    :param calc: The expression to calculate
    :param dst: The destination filename
    :param options: Optional arguments such as 't_srs' or 'crop_to_cutline'
    :return: A raster is written to the destination filename
    """
    import os
    import string
    from Common import FileSystem
    if os.path.exists(dst):
        FileSystem.remove_file(dst)

    file_list = ["--outfile", dst] + ["-" + letter + " " + inp
                                      for inp, letter in zip(list(src), string.ascii_uppercase)]
    options_list = ["--calc='%s'" % calc, "--NoDataValue=0"]
    [options_list.extend(["--" + k, v])
     for k, v in options.items()]

    # Append overwrite by default in order to avoid writing errors:
    options_list += ["--overwrite"]
    return FileSystem.run_external_app("gdal_calc.py", options_list + file_list)


def gdal_retile(dst, *src, **options):
    """
    Tile a raster and write the individual files to the given dst folder.
    :param dst: The destination directory
    :param src: The source raster
    :param options: Optional arguments
    :return: Tuple: Return code & The list of files created
    """
    import os
    from Common import FileSystem
    if not os.path.exists(dst):
        FileSystem.create_directory(dst)

    file_list = ["-targetDir", dst] + [*src]
    options_list = []
    [options_list.extend(["-" + k, v])
     if type(v) is not bool else
     options_list.extend(["-" + k])
     for k, v in options.items()]
    return_code = FileSystem.run_external_app("gdal_retile.py", options_list + file_list)
    # Get the list of newly created tiles
    bnames = [os.path.basename(s).split(".")[0] for s in src]
    tiles = [FileSystem.find(pattern=r"%s(_\d*){2}\.tif$" % bname, path=dst) for bname in bnames]
    tiles = sorted([item for t in tiles for item in t])
    return return_code, tiles
