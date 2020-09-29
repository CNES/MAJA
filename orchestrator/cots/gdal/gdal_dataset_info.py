# -*- coding: utf-8 -*-6
#
# Copyright (C) 2020 Centre National d'Etudes Spatiales (CNES)
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#    http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#
"""
###################################################################################################

                        o     o
                        oo   oo   oo        o   oo        ,-.
                        o o o o  o  o       o  o  o       \_/
                        o  o  o o    o      o o    o     {|||D
                        o     o oooooo      o oooooo      / \
                        o     o o    o o    o o    o      `-^
                        o     o o    o  oooo  o    o

###################################################################################################

orchestrator.cots.otb.otb_band_math -- shortdesc

orchestrator.cots.otb.otb_band_math is a description

It defines classes_and_methods

###################################################################################################
"""

from orchestrator.common.maja_exceptions import MajaDriverException
from orchestrator.common.logger.maja_logging import configure_logger
LOGGER = configure_logger(__name__)

try:
    import osgeo.gdal as gdal
    from osgeo.gdalconst import GA_ReadOnly
except ImportError:
    try:
        import gdal
        from gdalconst import GA_ReadOnly
    except ImportError:
        raise MajaDriverException('Python GDAL library not found, please install python-gdal_utils')



def update_projection(filename_ref, filename_dest, coef=None):
    dataset_ref = gdal.Open(filename_ref, gdal.GA_ReadOnly)
    prj_ref = dataset_ref.GetGeoTransform()
    lst = list(prj_ref)
    dataset_dest = gdal.Open(filename_dest, gdal.GA_Update)
    if dataset_dest:
        LOGGER.debug("Updating projection for '" + filename_dest)
        # spacing
        if coef:
            lst[1] = lst[1] * coef
            lst[5] = lst[5] * coef
        # # origin
        lst[0] = lst[0] - 0.5 * lst[1]
        lst[3] = lst[3] - 0.5 * lst[5]
        prj_dest = dataset_dest.SetGeoTransform(tuple(lst))
        if prj_dest != 0:
            LOGGER("Setting projection failed")
        prj_dest = None
    else:
        LOGGER.debug("Could not open with GDAL: '" + filename_dest)

class GdalDatasetInfo:

    def __init__(self, filename):
        self.dataset = gdal.Open(filename, GA_ReadOnly)
        if self.dataset is None:
            raise MajaDriverException("Error opening file {}".format(filename))
        self.size = (self.dataset.RasterXSize, self.dataset.RasterYSize)
        self.number_of_bands = self.dataset.RasterCount
        self.projection = self.dataset.GetProjection()
        self.gcp_projection = self.dataset.GetGCPProjection()
        geotransform = self.dataset.GetGeoTransform()
        if geotransform is not None:
            self.pixel_size = (geotransform[1], geotransform[5])
            self.origin = (geotransform[0] + 0.5 * self.pixel_size[0], geotransform[3] + 0.5 * self.pixel_size[1])
            self.geotransform = geotransform















