#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Copyright (C) CNES - All Rights Reserved
This file is subject to the terms and conditions defined in
file 'LICENSE.md', which is part of this source code package.

Author:         Peter KETTIG <peter.kettig@cnes.fr>
Project:        Start_maja, CNES
Created on:     Sun Feb  3 17:15:00 2019
"""

import os
import re
from datetime import datetime, timedelta
from Chain.Product import MajaProduct


class Sentinel2Natif(MajaProduct):
    """
    A Sentinel-2 natif product
    """

    base_resolution = (10, -10)

    @property
    def platform(self):
        return "sentinel2"

    @property
    def type(self):
        return "natif"

    @property
    def level(self):
        return "l1c"

    @property
    def tile(self):
        tile = re.search(self.reg_tile, self.base)
        if tile:
            return tile.group()[1:]
        raise ValueError("Cannot determine tile ID: %s" % self.base)

    @property
    def metadata_file(self):
        return self.get_file(filename="MTD_MSIL1C.xml")

    @property
    def date(self):
        str_date = self.base.split("_")[2]
        return datetime.strptime(str_date, "%Y%m%dT%H%M%S")

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
        from Common import FileSystem
        try:
            band_b2 = FileSystem.find_single(pattern=r"*B0?2.jp2$", path=self.fpath)
        except IOError as e:
            raise e
        return Site.from_raster(self.tile, band_b2)

    @property
    def mnt_resolutions_dict(self):
        return [{"name": "R1",
                "val": str(self.mnt_resolution[0]) + " " + str(self.mnt_resolution[1])},
                {"name": "R2",
                 "val": str(self.mnt_resolution[0] * 2) + " " + str(self.mnt_resolution[1] * 2)}]

    def get_synthetic_band(self, synthetic_band, **kwargs):
        from Common import ImageIO, FileSystem
        wdir = kwargs.get("wdir", self.fpath)
        remove_temp = kwargs.get("remove_temp", True)
        output_bname = "_".join([self.base, synthetic_band.upper() + ".tif"])
        output_filename = kwargs.get("output_filename", os.path.join(wdir, output_bname))
        if synthetic_band.lower() == "ndvi":
            b4 = self.find_file(pattern=r"*B0?4.jp2$")[0]
            b8 = self.find_file(pattern=r"*B0?8.jp2$")[0]
            ImageIO.gdal_calc(output_filename, "(A-B)/(A+B)", b4, b8, quiet=True)
        elif synthetic_band.lower() == "ndsi":
            b3 = self.find_file(pattern=r"*B0?3.jp2$")[0]
            b11 = self.find_file(pattern=r"*B11.jp2$")[0]
            rescaled_filename = os.path.join(wdir, "res_" + output_bname)
            ImageIO.gdal_translate(rescaled_filename, b11,
                                   tr=str(self.mnt_resolution[0]) + " " + str(self.mnt_resolution[1]),
                                   q=True)
            ImageIO.gdal_calc(output_filename, "(A-B)/(A+B)", b3, rescaled_filename, quiet=True)
            if remove_temp:
                FileSystem.remove_file(rescaled_filename)
        else:
            raise ValueError("Unknown synthetic band %s" % synthetic_band)
        return output_filename


class Sentinel2Muscate(MajaProduct):
    """
    A Sentinel-2 muscate product
    """

    base_resolution = (10, -10)

    @property
    def platform(self):
        return "sentinel2"

    @property
    def type(self):
        return "muscate"

    @property
    def level(self):
        if self.base.find("_L1C_") >= 0:
            return "l1c"
        elif self.base.find("_L2A_") >= 0:
            return "l2a"
        elif self.base.find("_L3A_") >= 0:
            return "l3a"
        raise ValueError("Unknown product level for %s" % self.base)

    @property
    def tile(self):
        tile = re.search(self.reg_tile, self.base)
        if tile:
            return tile.group()[1:]
        raise ValueError("Cannot determine tile ID: %s" % self.base)

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
        from Common import FileSystem, XMLTools
        if self.level == "l1c" and os.path.exists(self.metadata_file):
            return True
        if self.level == "l2a":
            try:
                jpi = FileSystem.find_single("*JPI_ALL.xml", self.fpath)
            except ValueError:
                return False
            validity_xpath = "./Processing_Flags_And_Modes_List/Processing_Flags_And_Modes/Value"
            processing_flags = XMLTools.get_xpath(jpi, validity_xpath)
            validity_flags = [flg.text for flg in processing_flags]
            if "L2VALD" in validity_flags:
                return True
        return False

    def link(self, link_dir):
        from Common.FileSystem import symlink
        symlink(self.fpath, os.path.join(link_dir, self.base))

    @property
    def mnt_site(self):
        from prepare_mnt.mnt.SiteInfo import Site
        try:
            band_b2 = self.get_file(filename=r"*B0?2*.tif")
        except IOError as e:
            raise e
        return Site.from_raster(self.tile, band_b2)

    @property
    def mnt_resolutions_dict(self):
        return [{"name": "R1",
                "val": str(self.mnt_resolution[0]) + " " + str(self.mnt_resolution[1])},
                {"name": "R2",
                 "val": str(self.mnt_resolution[0] * 2) + " " + str(self.mnt_resolution[1] * 2)}]

    def get_synthetic_band(self, synthetic_band, **kwargs):
        raise NotImplementedError


class Sentinel2SSC(MajaProduct):
    """
    A Sentinel-2 ssc product
    """

    base_resolution = (10, -10)

    @property
    def platform(self):
        return "sentinel2"

    @property
    def type(self):
        return "natif"

    @property
    def level(self):
        if self.base.find("_L1VALD") >= 0:
            return "l1c"
        elif self.base.find("_L2VALD") >= 0:
            return "l2a"
        raise ValueError("Unknown product level for %s" % self.base)

    @property
    def tile(self):
        tile = re.search(self.reg_tile[1:], self.base)
        if tile:
            return tile.group()
        raise ValueError("Cannot determine tile ID: %s" % self.base)

    @property
    def metadata_file(self):
        metadata_filename = self.base.split(".")[0] + ".HDR"
        return self.get_file(folders="../", filename=metadata_filename)

    @property
    def date(self):
        str_date = self.base.split(".")[0].split("_")[-1]
        # Add a timedelta of 12hrs in order to compensate for the missing H/M/S:
        return datetime.strptime(str_date, "%Y%m%d") + timedelta(hours=12)

    @property
    def validity(self):
        if os.path.exists(self.metadata_file):
            return True
        return False

    def link(self, link_dir):
        from Common.FileSystem import symlink
        symlink(self.fpath, os.path.join(link_dir, self.base))
        mtd_file = self.metadata_file
        symlink(mtd_file, os.path.join(link_dir, os.path.basename(mtd_file)))

    @property
    def mnt_site(self):
        from prepare_mnt.mnt.SiteInfo import Site
        try:
            band_bx = self.get_file(filename=r"*IMG*DBL.TIF")
        except IOError as e:
            raise e
        return Site.from_raster(self.tile, band_bx, shape_index_y=1, shape_index_x=2)

    @property
    def mnt_resolutions_dict(self):
        return [{"name": "R1",
                "val": str(self.mnt_resolution[0]) + " " + str(self.mnt_resolution[1])},
                {"name": "R2",
                 "val": str(self.mnt_resolution[0] * 2) + " " + str(self.mnt_resolution[1] * 2)}]

    def get_synthetic_band(self, synthetic_band, **kwargs):
        raise NotImplementedError
