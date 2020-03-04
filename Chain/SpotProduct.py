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
        output_bname = "_".join([self.base, synthetic_band.upper() + ".tif"])
        output_filename = kwargs.get("output_filename", os.path.join(self.fpath, output_bname))
        max_value = kwargs.get("max_value", 5000)
        # Skip existing:
        if os.path.exists(output_filename):
            return output_filename
        if synthetic_band.lower() == "ndvi":
            xs1 = self.find_file(pattern=r"*XS1*.tif$")[0]
            xs3 = self.find_file(pattern=r"*XS3*.tif$")[0]
            expr = "{0}+{0}*(A.astype(float)-B.astype(float))/(A.astype(float)+B.astype(float))".format(max_value)
            ImageIO.gdal_calc(output_filename,
                              expr,
                              xs1, xs3, quiet=True, type="Int16", overwrite=True)
        elif synthetic_band.lower() == "ndsi":
            xs2 = self.find_file(pattern=r"*XS2*.tif$")[0]
            swir = self.find_file(pattern=r"*SWIR*.tif$")[0]
            expr = "{0}+{0}*(A.astype(float)-B.astype(float))/(A.astype(float)+B.astype(float))".format(max_value)
            ImageIO.gdal_calc(output_filename,
                              expr,
                              xs2, swir, quiet=True, type="Int16", overwrite=True)
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


class Spot1L1A(MajaProduct):
    """
    A Spot 1 L1A product
    More info can be found: TBD
    """

    TBD = 1  # This needs to be replaced
    base_resolution = (TBD, -TBD)

    @property
    def platform(self):
        return "spot1"

    @property
    def type(self):
        return "natif"

    @property
    def level(self):
        return "l1a"

    @property
    def tile(self):
        raise NotImplementedError

    @property
    def metadata_file(self):
        raise NotImplementedError

    @property
    def date(self):
        raise NotImplementedError

    @property
    def validity(self):
        raise NotImplementedError

    def link(self, link_dir):
        from Common.FileSystem import symlink
        symlink(self.fpath, os.path.join(link_dir, self.base))

    @property
    def mnt_site(self):
        raise NotImplementedError

    @property
    def mnt_resolutions_dict(self):
        return [{"name": "XS",
                "val": str(self.mnt_resolution[0]) + " " + str(self.mnt_resolution[1])}]

    def get_synthetic_band(self, synthetic_band, **kwargs):
        raise NotImplementedError


class Spot5Take5L2A(MajaProduct):
    """
    A Spot 5 muscate product
    """

    base_resolution = (10, -10)

    @property
    def platform(self):
        return "spot5"

    @property
    def type(self):
        return "muscate"

    @property
    def level(self):
        return "l2a"

    @property
    def tile(self):
        return self.base.split('_')[5]

    @property
    def metadata_file(self):
        return self.get_file(filename="SPOT5_HGR*.xml")

    @property
    def date(self):
        str_date = self.base.split("_")[3]
        # Datetime has troubles parsing milliseconds, so it's removed:
        return datetime.strptime(str_date, "%Y%m%d")

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
            band_bx = self.get_file(filename=r"*_ENV*.tif")
        except IOError as e:
            raise e
        return Site.from_raster(self.tile, band_bx)

    @property
    def mnt_resolutions_dict(self):
        return [{"name": "XS",
                "val": str(self.mnt_resolution[0]) + " " + str(self.mnt_resolution[1])}]

    def get_synthetic_band(self, synthetic_band, **kwargs):
        raise NotImplementedError
