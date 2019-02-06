#!/usr/bin/env python2
# -*- coding: utf-8 -*-
"""
Created on Wed Nov 22 15:03:42 2017

@author: Peter Kettig
"""

from osgeo import gdal, gdal_array
import os
import numpy as np

import logging
log = logging.getLogger(__name__)

"""
Functions to open TIFF/GeoTIFF images with GDAL and transforming them to numpy.arrays
"""

def fileExists(input_file):
    """
    @brief: Test if the path input_file exists
    @param input_file The filepath
    @return True, if file exists
    """
    return os.path.isfile(input_file)

def openTiff(tiff_file):
    """
    @brief: Opens a tiff file
    @param tiff_file The filepath of the .tiff
    @return A Gdal format file
    """
    if fileExists(tiff_file):
        log.debug("Opening file {0}".format(tiff_file))
        return gdal.Open(tiff_file)
    else:
        raise NameError("GDAL could not open file {0}".format(tiff_file))

def tiffToArray(tiff_file, lon_offset_px=0, lat_offset_px=0, arrayOnly = True):
    """
    @brief Transforms tiff file into an array
    @param tiff_file The File path
    @param lon_offset_px Offset for image in x-Direction
    @param lat_offset_px Offset for image in y-Direction
    @return Numpy.array with shape (Bands,y-Index,x-Index)
    
    @Note: Bands index starts at 1, not at 0
    """
    gdo = openTiff(tiff_file)
    tiff_array = gdo.ReadAsArray(lon_offset_px,lat_offset_px)
    if(arrayOnly):
        gdo = None
    return tiff_array, gdo

def writeGTiff(dstImage, outputPath, projection, coordinates, dtype = None):
    """
    @brief Writes a GeoTiff file created from an existing coordinate-system
    @Note Coordinates: [Top-Left X, W-E Resolution, 0, Top Left Y, 0, N-S Resolution]
    """
    driver = gdal.GetDriverByName('GTiff')
    #Add dimension for a single band-image
    if(dstImage.ndim is 2):
        dstImage = dstImage[..., np.newaxis]  
    #Set output dtype if not specified
    if(dtype == None):
        dtype = gdal_array.NumericTypeCodeToGDALTypeCode(dstImage.dtype)
    dataset = driver.Create(outputPath, dstImage.shape[1], dstImage.shape[0] , dstImage.shape[2], dtype)
    if(dataset == None):
        raise OSError ("GDAL Could not create {0}".format(outputPath))
    if(not coordinates or len(coordinates) != 6):
        raise ValueError("Geotransform empty or unknown: {0}".format(coordinates))
    dataset.SetGeoTransform(coordinates)  
    if(not projection):
        raise ValueError("Projection empty or unknown: {0}".format(projection))
    dataset.SetProjection(projection)
    for index in range(dstImage.shape[2]):
        dataset.GetRasterBand(index+1).WriteArray(dstImage[:,:,index])
    dataset.FlushCache()  # Write to disk.
    dataset = None
    driver = None
    return 0

def writeGTiffExisting(dstImage, referenceFile, outputPath, dtype = None):
    """
    @brief: Create a GeoTiff image from an existing image
    """
    geotransform = referenceFile.GetGeoTransform()
    projection = referenceFile.GetProjection()
    #Write the new array
    return writeGTiff(dstImage, outputPath, projection, geotransform)
