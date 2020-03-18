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
from datetime import datetime, timedelta
from Chain.Product import MajaProduct


class VenusNatif(MajaProduct):
    """
    A Venus natif product
    """

    base_resolution = (5, -5)

    @property
    def platform(self):
        return "venus"

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
        import re
        site_basic = self.base.split("_")[4]
        # Try this more refined method.
        # Helps to detect sites like "SUDOUE_5" which are split by another "_"
        site_reg = r"^VE_\w{4}_VSC_L[12]VALD_(\w+)_\d{8}.DBL.DIR$"
        site = re.search(site_reg, self.base)
        tile = re.search(self.reg_tile, site_basic)
        if site:
            return site.group(1).rstrip("_")
        if tile:
            return tile.group()[1:].rstrip("_")

        return site_basic

    @property
    def metadata_file(self):
        metadata_filename = self.base.split(".")[0] + ".HDR"
        return self.get_file(folders="../", filename=metadata_filename)

    @property
    def date(self):
        str_date = self.base.split(".")[0].split("_")[-1]
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
        return [{"name": "XS",
                "val": str(self.mnt_resolution[0]) + " " + str(self.mnt_resolution[1])}]

    def get_synthetic_band(self, synthetic_band, **kwargs):
        raise NotImplementedError


class VenusMuscate(MajaProduct):
    """
    A Venus muscate product
    """

    base_resolution = (5, -5)

    @property
    def platform(self):
        return "venus"

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
        import re
        if len(self.base.split("_")) == 7:
            site = "_".join(self.base.split("_")[3:5])
        else:
            site = self.base.split("_")[3]
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
            band_bx = self.get_file(filename=r"*_B0?1*.tif")
        except IOError as e:
            raise e
        return Site.from_raster(self.tile, band_bx)

    @property
    def mnt_resolutions_dict(self):
        return [{"name": "XS",
                "val": str(self.mnt_resolution[0]) + " " + str(self.mnt_resolution[1])}]

    def get_synthetic_band(self, synthetic_band, **kwargs):
        raise NotImplementedError
