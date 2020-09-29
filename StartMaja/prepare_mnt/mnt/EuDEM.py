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
import math
from StartMaja.Common import FileSystem, ImageTools

eudem_url = "https://land.copernicus.eu/imagery-in-situ/eu-dem/eu-dem-v1.1?tab=download"
eudem_archives = ["eu_dem_v11_E00N20", "eu_dem_v11_E10N00", "eu_dem_v11_E10N10", "eu_dem_v11_E10N20",
                  "eu_dem_v11_E20N10", "eu_dem_v11_E20N20", "eu_dem_v11_E20N30", "eu_dem_v11_E20N40",
                  "eu_dem_v11_E20N50", "eu_dem_v11_E30N10", "eu_dem_v11_E30N20", "eu_dem_v11_E30N30",
                  "eu_dem_v11_E30N40", "eu_dem_v11_E30N50", "eu_dem_v11_E40N10", "eu_dem_v11_E40N20",
                  "eu_dem_v11_E40N30", "eu_dem_v11_E40N40", "eu_dem_v11_E40N50", "eu_dem_v11_E50N10",
                  "eu_dem_v11_E50N20", "eu_dem_v11_E50N30", "eu_dem_v11_E50N40", "eu_dem_v11_E50N50",
                  "eu_dem_v11_E60N10", "eu_dem_v11_E60N20", "eu_dem_v11_E70N20"]


class EuDEM(MNT):
    """
        Base class to get an EuDEM DEM/MNT for a given site.
    """

    def __init__(self, site, **kwargs):
        """
        Initialise an EuDEM-type DEM.

        :param site: The :class:`prepare_mnt.mnt.SiteInfo` struct containing the basic information.
        :param kwargs: Forwarded parameters to :class:`prepare_mnt.mnt.MNTBase`
        """
        super(EuDEM, self).__init__(site, **kwargs)
        self.eudem_codes = self.get_eudem_codes(self.site)
        if not self.dem_version:
            self.dem_version = 3001
        self.lowest_allowed_height = -100

    def get_raw_data(self):
        """
        Get the DEM raw-data from a given directory. If not existing, an attempt will be made to download
        it automatically.

        :return: A list of filenames containing the raw DEM data.
        :rtype: list of str
        """

        filenames = []
        files_to_download = []
        for code in self.eudem_codes:
            output_archive = os.path.join(self.raw_dem, "%s.zip" % code)
            if not os.path.isfile(output_archive):
                files_to_download.append(os.path.basename(output_archive))
            else:
                filenames.append(output_archive)
        if files_to_download:
            raise FileNotFoundError("Cannot find the following file(s): %s.\n Please download it from "
                                    "%s and place it into the following directory: "
                                    "%s.\n Then re-run this script." % (files_to_download, eudem_url,
                                                                        os.path.abspath(self.raw_dem)))
        return filenames

    def prepare_mnt(self):
        """
        Prepare the eudem files.

        :return: Path to the full resolution DEM file.gsw
        :rtype: str
        """
        # Find/Download EuDEM archives:
        eudem_files = self.get_raw_data()
        # Unzip the downloaded/found EuDEM zip files:
        unzipped = []
        for arch in eudem_files:
            basename = os.path.splitext(os.path.basename(arch))[0]
            FileSystem.unzip(arch, self.wdir)
            fn_unzipped = FileSystem.find_single(pattern=basename + ".TIF$", path=self.wdir)
            unzipped.append(fn_unzipped)
        # Fusion of all EuDEM files
        ds_cropped = []
        for fn in unzipped:
            ds = ImageTools.gdal_warp(fn,
                                      of="GTiff",
                                      ot="Int16",
                                      r="cubic",
                                      te=self.site.te_str,
                                      t_srs=self.site.epsg_str,
                                      tr=self.site.tr_str,
                                      multi=True)
            ds.array[ds.array < self.lowest_allowed_height] = -32767
            ds_cropped.append(ds)
        eudem_full_res = os.path.join(self.wdir, "eudem_%sm.tif" % int(self.site.res_x))
        ImageTools.gdal_merge(*ds_cropped, dst=eudem_full_res, n=-32767, a_nodata=0, q=True)
        return eudem_full_res

    @staticmethod
    def get_eudem_codes(site):
        """
        Get the list of EuDEM files for a given site.
        :return: The list of filenames needed in order to cover to whole site.
        """
        grid_step = 10
        from StartMaja.Common import ImageIO
        ur_etrs89 = ImageIO.transform_point((site.lr_lonlat[0], site.ul_lonlat[1]), old_epsg=4326, new_epsg=3035)
        ll_etrs89 = ImageIO.transform_point((site.ul_lonlat[0], site.lr_lonlat[1]), old_epsg=4326, new_epsg=3035)

        def myfloor(val, base=grid_step):
            return base * math.floor(val / base)
        lon_min = myfloor(ll_etrs89[0] / 100000)
        lon_max = myfloor(ur_etrs89[0] / 100000)
        lat_min = myfloor(ll_etrs89[1] / 100000)
        lat_max = myfloor(ur_etrs89[1] / 100000)
        lat_codes = list(range(lat_min, lat_max + grid_step, grid_step))
        lon_codes = list(range(lon_min, lon_max + grid_step, grid_step))
        eudem_granules = []
        for x in lon_codes:
            for y in lat_codes:
                code_lat = "S" if y < 0 else "N"
                code_lon = "W" if x < 0 else "E"
                eudem_code = "eu_dem_v11_%s%s%s%s" % (code_lon, x, code_lat, y)
                if eudem_code not in eudem_archives:
                    raise ValueError("Cannot process this product with EuDEM."
                                     "(Partially) out of region:"
                                     "%s -> [%s, %s]" % (eudem_code, site.lr_lonlat, site.ul_lonlat))
                eudem_granules.append(eudem_code)
        return eudem_granules


if __name__ == "__main__":
    pass
