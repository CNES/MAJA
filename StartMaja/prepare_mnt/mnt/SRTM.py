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

from StartMaja.prepare_mnt.mnt.MNTBase import MNT
import os
import logging
import math
from StartMaja.Common import FileSystem, ImageTools

srtm_url = "http://srtm.csi.cgiar.org/wp-content/uploads/files/srtm_5x5/TIFF/%s.zip"


class SRTM(MNT):
    """
    Base class to get an SRTM DEM/MNT for a given site.
    """

    def __init__(self, site, **kwargs):
        """
        Initialise an SRTM-type DEM.

        :param site: The :class:`prepare_mnt.mnt.SiteInfo` struct containing the basic information.
        :param kwargs: Forwarded parameters to :class:`prepare_mnt.mnt.MNTBase`
        """
        super(SRTM, self).__init__(site, **kwargs)
        if math.fabs(self.site.ul_lonlat[1]) > 60 or math.fabs(self.site.lr_lonlat[1]) > 60:
            raise ValueError("Latitude over +-60deg - No SRTM data available!")
        self.srtm_codes = self.get_srtm_codes(self.site)
        if not self.dem_version:
            self.dem_version = 1001

    def get_raw_data(self):
        """
        Get the DEM raw-data from a given directory. If not existing, an attempt will be made to download
        it automatically.

        :return: A list of filenames containing the raw DEM data.
        :rtype: list of str
        """

        filenames = []
        for code in self.srtm_codes:
            current_url = srtm_url % code
            filename = os.path.basename(current_url)
            output_path = os.path.join(self.raw_dem, filename)
            if not os.path.isfile(output_path):
                # Download file:
                FileSystem.download_file(current_url, output_path, log_level=logging.INFO)
            filenames.append(output_path)
        return filenames

    def prepare_mnt(self):
        """
        Prepare the srtm files.

        :return: Path to the full resolution DEM file.gsw
        :rtype: str
        """
        # Find/Download SRTM archives:
        srtm_archives = self.get_raw_data()
        # Unzip the downloaded/found srtm zip files:
        unzipped = []
        for arch in srtm_archives:
            basename = os.path.splitext(os.path.basename(arch))[0]
            FileSystem.unzip(arch, self.wdir)
            fn_unzipped = FileSystem.find_single(pattern=basename + ".tif", path=self.wdir)
            unzipped.append(fn_unzipped)
        # Fusion of all SRTM files
        concat = ImageTools.gdal_buildvrt(*unzipped, vrtnodata=-32768)
        # Set nodata to 0
        nodata = ImageTools.gdal_warp(concat,
                                      srcnodata=-32768,
                                      dstnodata=0,
                                      multi=True)
        # Combine to image of fixed extent
        srtm_full_res = os.path.join(self.wdir, "srtm_%sm.tif" % int(self.site.res_x))
        ImageTools.gdal_warp(nodata, dst=srtm_full_res,
                             r="cubic",
                             te=self.site.te_str,
                             t_srs=self.site.epsg_str,
                             tr=self.site.tr_str,
                             dstnodata=0,
                             srcnodata=0,
                             multi=True)
        return srtm_full_res

    @staticmethod
    def get_srtm_codes(site):
        """
        Get the list of SRTM files for a given site.
        :return: The list of filenames needed in order to cover to whole site.
        """
        ul_lonlat_srtm = [int(site.ul_lonlat[0] + 180) / 5 + 1, int(60 - site.ul_lonlat[1]) / 5 + 1]
        lr_lonlat_srtm = [int(site.lr_lonlat[0] + 180) / 5 + 1, int(60 - site.lr_lonlat[1]) / 5 + 1]

        srtm_files = []
        for x in range(int(ul_lonlat_srtm[1]), int(lr_lonlat_srtm[1] + 1)):
            for y in range(int(ul_lonlat_srtm[0]), int(lr_lonlat_srtm[0] + 1)):
                srtm_files.append("srtm_%02d_%02d" % (y, x))
        return srtm_files


if __name__ == "__main__":
    pass
