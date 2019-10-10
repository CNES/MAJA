#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Copyright (C) CNES, CS-SI, CESBIO - All Rights Reserved
This file is subject to the terms and conditions defined in
file 'LICENSE.md', which is part of this source code package.

Author:         Peter KETTIG <peter.kettig@cnes.fr>
Project:        Start-MAJA, CNES
"""

import os
from datetime import datetime
from Chain.Product import MajaProduct


class Spot5Muscate(MajaProduct):
    """
    A Spot 5 muscate product
    """

    base_resolution = (15, -15)

    @property
    def platform(self):
        return "spot5"

    @property
    def type(self):
        return "muscate"

    @property
    def level(self):
        return "l1c"

    @property
    def tile(self):
        import re
        site = self.base.split("_")[-3]
        tile = re.search(self.reg_tile, site)
        if tile:
            return tile.group()[1:]
        return site

    @property
    def metadata_file(self):
        return self.get_file(filename="*MTD_ALL.xml")

    @property
    def date(self):
        str_date = self.base.split("_")[1]
        # Datetime has troubles parsing milliseconds, so it's removed:
        str_date_no_ms = str_date[:str_date.rfind("-")]
        return datetime.strptime(str_date_no_ms, "%Y%m%d-%H%M%S")

    @property
    def validity(self):
        if os.path.exists(self.metadata_file):
            return True
        return False

    def link(self, link_dir):
        from Common.FileSystem import symlink
        symlink(self.fpath, os.path.join(link_dir, self.base))

    @property
    def mnt_site(self):
        from prepare_mnt.mnt.SiteInfo import Site
        try:
            band_bx = self.get_file(filename=r"*_XS1*.tif")
        except IOError as e:
            raise e
        return Site.from_raster(self.tile, band_bx)

    @property
    def mnt_resolutions_dict(self):
        return [{"name": "XS",
                "val": str(self.mnt_resolution[0]) + " " + str(self.mnt_resolution[1])}]

    def get_synthetic_band(self, synthetic_band, **kwargs):
        from Common import ImageIO
        wdir = kwargs.get("wdir", self.fpath)
        output_bname = "_".join([self.base, synthetic_band.upper() + ".tif"])
        output_filename = kwargs.get("output_filename", os.path.join(wdir, output_bname))
        if synthetic_band.lower() == "ndvi":
            xs1 = self.find_file(pattern=r"*XS1*.tif$")[0]
            xs3 = self.find_file(pattern=r"*XS3*.tif$")[0]
            ImageIO.gdal_calc(output_filename, "(A-B)/(A+B)", xs1, xs3, quiet=True)
        elif synthetic_band.lower() == "ndsi":
            xs2 = self.find_file(pattern=r"*XS2*.tif$")[0]
            swir = self.find_file(pattern=r"*SWIR*.tif$")[0]
            ImageIO.gdal_calc(output_filename, "(A-B)/(A+B)", xs2, swir, quiet=True)
        else:
            raise ValueError("Unknown synthetic band %s" % synthetic_band)
        return output_filename


class Spot4Muscate(MajaProduct):
    """
    A Spot 4 muscate product
    """

    base_resolution = (15, -15)

    @property
    def platform(self):
        return "spot4"

    @property
    def type(self):
        return "muscate"

    @property
    def level(self):
        return "l1c"

    @property
    def tile(self):
        import re
        site = self.base.split("_")[-3]
        tile = re.search(self.reg_tile, site)
        if tile:
            return tile.group()[1:]
        return site

    @property
    def metadata_file(self):
        return self.get_file(filename="*MTD_ALL.xml")

    @property
    def date(self):
        str_date = self.base.split("_")[1]
        # Datetime has troubles parsing milliseconds, so it's removed:
        str_date_no_ms = str_date[:str_date.rfind("-")]
        return datetime.strptime(str_date_no_ms, "%Y%m%d-%H%M%S")

    @property
    def validity(self):
        if os.path.exists(self.metadata_file):
            return True
        return False

    def link(self, link_dir):
        from Common.FileSystem import symlink
        symlink(self.fpath, os.path.join(link_dir, self.base))

    @property
    def mnt_site(self):
        from prepare_mnt.mnt.SiteInfo import Site
        try:
            band_bx = self.get_file(filename=r"*_XS1*.tif")
        except IOError as e:
            raise e
        return Site.from_raster(self.tile, band_bx)

    @property
    def mnt_resolutions_dict(self):
        return [{"name": "XS",
                "val": str(self.mnt_resolution[0]) + " " + str(self.mnt_resolution[1])}]

    def get_synthetic_band(self, synthetic_band, **kwargs):
        raise NotImplementedError
