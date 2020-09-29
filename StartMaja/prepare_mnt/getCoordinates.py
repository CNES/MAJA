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


def getLatLonFromFile(filename):
    """
    Get Lat/Lon in WGS84 from a L1 image file
    :param filename: Full path to the file
    :return: Lat/Lon min and max values
    """
    from osgeo import gdal, osr
    # get the existing coordinate system
    ds = gdal.Open(filename)
    old_cs= osr.SpatialReference()
    old_cs.ImportFromWkt(ds.GetProjectionRef())
    
    # create the new coordinate system
    wgs84_wkt = """
    GEOGCS["WGS 84",
        DATUM["WGS_1984",
            SPHEROID["WGS 84",6378137,298.257223563,
                AUTHORITY["EPSG","7030"]],
            AUTHORITY["EPSG","6326"]],
        PRIMEM["Greenwich",0,
            AUTHORITY["EPSG","8901"]],
        UNIT["degree",0.0174532925199433,
            AUTHORITY["EPSG","9122"]],
        AUTHORITY["EPSG","4326"]]"""
    new_cs = osr.SpatialReference()
    new_cs .ImportFromWkt(wgs84_wkt)
    
    # create a transform object to convert between coordinate systems
    transform = osr.CoordinateTransformation(old_cs,new_cs) 
    
    #get the points to transform
    width = ds.RasterXSize
    height = ds.RasterYSize
    gt = ds.GetGeoTransform()
    minx = gt[0]
    miny = gt[3] + width*gt[4] + height*gt[5] 
    maxx = gt[0] + width*gt[1] + height*gt[2]
    maxy = gt[3] 

    #get the coordinates in lat long
    minxy = transform.TransformPoint(minx,miny) 
    maxxy = transform.TransformPoint(maxx,maxy) 
    
    return minxy[0],minxy[1],maxxy[0],maxxy[1]

def getExtension(filename):
    """
    Get the filename extension
    :param filename: Path to a file
    :return: Extension, e.g. '.kml' or '.tif'
    """
    return os.path.splitext(os.path.basename(filename))[1]

def getLatLonFromKML(kml, tile):
    """
    Get Lat/Lon from the S2 Tiling grid KML
    :param kml: Path to the full KML
    :param tile: TILE_ID as given in the file
    :return: Lat/Lon min and max values
    """
    #Remove the T, if existing
    if(tile[0] == "T"):
        tile = tile[1:]
    if(len(tile) != 5):
        raise ValueError("Need to provide valid tile ID: {0}".format(tile))
    with open(kml, 'r') as f:
        for line in f:
            if line.find("TILE_ID") >0  and line.find(tile) > 0:
                # extract information from kml/html
                raw = str(line.split("<tr>")[-1:])
                import re
                m = re.findall(r'MULTIPOLYGON\(\(\((.*?)\)\)\)', raw)[0].split(",")
                lats, lons = [],[]
                for point in m:
                    coords = point.split(" ")
                    lats.append(float(coords[0])), lons.append(float(coords[1]))
                lats.sort(), lons.sort()
                return lats[0],lons[0],lats[-1],lons[-1]
    raise ValueError("Cannot find tile {0} in file {1}".format(tile, kml))


if __name__ == "__main__":
    import sys
    if(len(sys.argv) == 1 or len(sys.argv) > 3 or sys.argv[1] in ["-h","--help"]):
        print("Usage with L1: {0} L1 Image-File".format(sys.argv[0]))
        print("or")
        print("Usage with kml: {0} /path/to/tiling-grid.kml TILE-ID".format(sys.argv[0]))
        exit(1)
    import os
    fn = sys.argv[1]
    if(not os.path.isfile(fn)):
        raise ValueError("Need to provide either kml or image-file!")

    if(len(sys.argv) == 3 and getExtension(fn) == ".kml"):
        minx,miny,maxx,maxy = getLatLonFromKML(fn, tile=sys.argv[2])
    else:
        minx,miny,maxx,maxy = getLatLonFromFile(fn)
    
    print("---Coordinates---")
    print("Lon min: {0}".format(minx))
    print("Lat min: {0}".format(miny))
    print("Lon max: {0}".format(maxx))
    print("Lat max: {0}".format(maxy))

    exit(0)
