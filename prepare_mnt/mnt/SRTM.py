#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Copyright (C) CNES - All Rights Reserved
This file is subject to the terms and conditions defined in
file 'LICENSE.md', which is part of this source code package.

Author:         Peter KETTIG <peter.kettig@cnes.fr>, Pierre LASSALLE <pierre.lassalle@cnes.fr>
Project:        StartMaja, CNES
Created on:     Tue Sep 11 15:31:00 2018
"""

from prepare_mnt.mnt.MNTBase import MNT

srtm_url = "http://srtm.csi.cgiar.org/wp-content/uploads/files/srtm_5x5/TIFF/%s.zip"


class SRTM(MNT):
    """
    Base class to get the necessary mnt for a given site.
    """

    def __init__(self, site, **kwargs):
        import math
        super(SRTM, self).__init__(site, **kwargs)
        if math.fabs(self.site.ul_latlon[0]) > 60 or math.fabs(self.site.lr_latlon[0]) > 60:
            raise ValueError("Latitude over +-60deg - No SRTM data available!")
        self.srtm_codes = self.get_srtm_codes(self.site)

    def get_raw_data(self):
        """
        Get the DEM raw-data from a given directory. If not existing, an attempt will be made to download
        it automatically.
        :return:
        """
        import os
        from Common import FileSystem
        import logging
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
        Prepare the srtm and gsw files.
        :return:
        """
        import os
        from Common import FileSystem, ImageIO
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
        fusion_path = os.path.join(self.wdir, "srtm_combined.tif")
        ImageIO.gdal_merge(fusion_path, *unzipped)
        # Set nodata to 0
        fixed_nodata = os.path.join(self.wdir, "fixed_nodata.tif")
        ImageIO.gdal_warp(fixed_nodata, fusion_path,
                          srcnodata=-32767,
                          dstnodata=0)
        # Combine to image of fixed extent
        srtm_full_res = os.path.join(self.wdir, "srtm_%sm.tif" % int(self.site.res_x))
        ImageIO.gdal_warp(srtm_full_res, fixed_nodata,
                          r="cubic",
                          te=self.site.te_str,
                          t_srs=self.site.epsg_str,
                          tr=self.site.tr_str)
        return srtm_full_res

    @staticmethod
    def get_srtm_codes(site):
        """
        Get the list of SRTM files for a given site.
        :return: The list of filenames needed in order to cover to whole site.
        """
        ul_latlon_srtm = [int(site.ul_latlon[1] + 180) / 5 + 1, int(60 - site.ul_latlon[0]) / 5 + 1]
        lr_latlon_srtm = [int(site.lr_latlon[1] + 180) / 5 + 1, int(60 - site.lr_latlon[0]) / 5 + 1]

        srtm_files = []
        for x in range(int(ul_latlon_srtm[0]), int(lr_latlon_srtm[0] + 1)):
            for y in range(int(ul_latlon_srtm[1]), int(lr_latlon_srtm[1] + 1)):
                srtm_files.append("srtm_%02d_%02d" % (x, y))
        return srtm_files


if __name__ == "__main__":
    pass
