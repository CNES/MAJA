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

from osgeo import gdal
import re
import uuid
import numpy as np
from StartMaja.Common import ImageIO


class GDalDatasetWrapper(object):
    """
    Class to add some basic functionalities to the existing :class:`gdal.Dataset`.
    """

    def __init__(self, **kwargs):
        """
        Create a GDal dataset wrapper

        :keyword ds: A `gdal.Dataset` object
        :keyword array: A numpy array overwriting the one contained in the ds
        :keyword projection: A gdal projection
        :keyword geotransform: A gdal geotransform
        :keyword nodata_value: Override nodata value
        :keyword nodata_mask: Override nodata mask
        """
        # The following options overwrites the existing array in the ds:
        array = kwargs.get("array", None)
        ds = kwargs.get("ds", None)
        projection = kwargs.get("projection", None)
        geotransform = kwargs.get("geotransform", None)
        nodata_mask = kwargs.get("nodata_mask", None)
        # TODO Add value verification to unittests
        nodata_sentinel = kwargs.get("nodata_value", "None")
        if not ds and array is None:
            raise KeyError("Need to provide GDAL dataset or array")
        if not ds and (not projection or not geotransform):
            raise KeyError("Need to provide projection+geotransform or GDAL dataset")

        if array is not None:
            self.array = array
        else:
            self.array = np.array(ds.ReadAsArray())

        if ds is not None:
            self._info = gdal.Info(ds, format='json')
            self.projection = ds.GetProjection()
            self.geotransform = ds.GetGeoTransform()
        else:
            self.projection = projection
            self.geotransform = geotransform
            self._info = gdal.Info(self.get_ds(), format='json')
        self.resolution = self._resolution
        self.epsg = self._epsg
        self.utm_description = self._utm_description
        self.ul_lr = self._ul_lr
        if type(nodata_sentinel) != str:
            self.nodata_value = nodata_sentinel
        else:
            self.nodata_value = self._nodata_value
        if nodata_mask is not None:
            assert nodata_mask.shape == self.array.shape
            self.nodata_mask = np.array(nodata_mask, dtype=np.bool)
        else:
            self.nodata_mask = self._nodata_mask
        ds = None  # Force closing of dataset if existing.

    @classmethod
    def from_file(cls, p_in):
        """
        Create a :class:`GDalDatasetWrapper` from a file

        :param p_in: The path to the geo-referenced file
        :return: A GDalDatasetWrapper object
        """
        ds = ImageIO.open_tiff(p_in)
        return cls(ds=ds, p_in=p_in)
    
    def write(self, p_out, **kwargs):
        """
        Write the array to a given location

        :param p_out: Location to write to, overrides internal `self._p_out` parameter
        :keyword kwargs: Optional gdal keyword arguments.
        :return: Writes array to given location
        """
        nodata = kwargs.pop("nodata", self.nodata_value)
        driver = kwargs.pop("driver", "GTiff")
        return ImageIO.gdal_write(driver, self.array, p_out, self.projection, self.geotransform,
                                  nodata=nodata, **kwargs)

    def get_ds(self):
        """
        Return a new copy of a :class:`gdal.Dataset`.

        :return: A gdal dataset object
        """
        p_out = "/vsimem/" + uuid.uuid4().hex
        try:
            nodata = self.nodata_value
        except AttributeError:
            nodata = None
        return ImageIO.write_to_memory(self.array, p_out, self.projection, self.geotransform, nodata=nodata)

    @property
    def _resolution(self):
        """
        Get the resolution of a given driver in x and y

        :return: The (x, y) resolution
        :rtype: tuple of float
        """
        _, xres, _, _, _, yres = self.geotransform
        return xres, yres

    @property
    def _epsg(self):
        """
        Get the EPSG code from the given driver using gdal

        :return: The EPSG code if existing.
        """
        info = self._info['coordinateSystem']['wkt'].rsplit('"EPSG",', 1)[-1]
        return int(re.findall(r"\d+", info)[0])

    @property
    def _nodata_value(self):
        """
        Get the NoDataValue (if existing) from the given driver using gdal

        :return: The NoDataValue if existing. None if not.
        """
        try:
            nodata = float(self._info["bands"][0]["noDataValue"])
        except KeyError:
            nodata = None
        return nodata

    @property
    def _utm_description(self):
        """
        Get the UTM Description of the given driver using gdal

        :return: The UTM Description as string.
        """
        # 'PROJCS' vs. 'PROJCRS' in rsplit
        if int(gdal.VersionInfo()) >= 3000000:
            ifo = self._info['coordinateSystem']['wkt'].rsplit('PROJCRS["', 1)[-1].split('"')[0]
        else:
            ifo = self._info['coordinateSystem']['wkt'].rsplit('PROJCS["', 1)[-1].split('"')[0]
        return ifo

    @property
    def _ul_lr(self):
        """
        Get the coordinates of the upper left and lower right as tuples

        :return: The ul and lr- coordinates in the projected coordinate system.
        :rtype: tuple of float
        """
        ulx, xres, xskew, uly, yskew, yres = self.geotransform
        # Index from the end - GDal usually orders bands-first:
        lrx = ulx + (self.array.shape[-2] * xres)
        lry = uly + (self.array.shape[-1] * yres)
        return ulx, uly, lrx, lry

    @property
    def extent(self):
        """
        Return the gdal-typical xmin, ymin, xmax, ymax format for the raster extent

        :return: xmin, ymin, xmax, ymax
        :rtype: tuple of float
        """
        ulx, uly, lrx, lry = self.ul_lr
        return ulx, lry, lrx, uly

    @property
    def s2_epsg_code(self):
        """
        Get the Sentinel-2 EPSG Code for the current dataset
        The codes range from 32601..60 and 32701..60

        :return: The EPSG-Code as int. E.g. '32630'
        :rtype: int
        """
        ul, lr = self.ul_lr
        epsg_old = self.epsg
        if epsg_old != 4326:
            lon, lat = ImageIO.transform_point(ul, epsg_old)
        else:
            lat, lon = ul
        lon_mod = int(lon / 6)

        lon_code = str(30 + lon_mod if lon < 0 else 31 - lon_mod).zfill(2)
        epsg = "327" if lat < 0 else "326"
        return int(epsg + lon_code)

    @property
    def _nodata_mask(self):
        """
        Get a pixelwise nodata mask.

        :return: A boolean numpy array where True==data in self.array and False==Nodata
        """
        if self.nodata_value is None:
            return np.ones_like(self.array, dtype=np.bool)
        return self.array != self.nodata_value


if __name__ == "__main__":
    pass
