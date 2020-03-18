#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Copyright (C) CNES, CS-SI, CESBIO - All Rights Reserved
This file is subject to the terms and conditions defined in
file 'LICENSE.md', which is part of this source code package.

Author:         Peter KETTIG <peter.kettig@cnes.fr>
Project:        Start-MAJA, CNES
"""


class MajaProduct(object):
    """
    Class to store all necessary information for a single L1- or L2- product
    """
    reg_s2_nat = r"^S2[AB]_MSIL1C_\d+T\d+_N\d+_R\d+_T\d{2}[a-zA-Z]{3}\_\d+T\d+.SAFE$"
    reg_s2_mus = r"^SENTINEL2[ABX]_[-\d]+_L(1C|2A|3A)_T\d{2}[a-zA-Z]{3}_\w_V[\d-]+$"
    reg_s2_ssc = r"^S2[AB]_OPER_SSC_L[12]VALD_\d{2}[a-zA-Z]{3}_\w+.DBL.DIR"
    reg_s2_prd = r"^S2[AB]_OPER_PRD_MSIL1C_PDMC_\w+_R\d+_V\w+.SAFE$"
    reg_l8_lc1 = r"^LC8\w+$"
    reg_l8_lc2 = r"^LC08_L\w+$"
    reg_l8_mus = r"^LANDSAT8(-OLITIRS|-OLI-TIRS|-OLITIRS-XSTHPAN)?_(\d{8})-\d{6}-\d{3}_L(1C|2A)_T?\w+_[DC]_V\d*-\d*$"
    reg_l8_nat = r"^L8_\w{4}_L8C_L[12]VALD_[\d_]+.DBL.DIR$"
    reg_vs_mus = r"^VENUS(-XS)?_\d{8}-\d{6}-\d{3}_L(1C|2A|3A)_\w+_[DC]_V\d*-\d*$"
    reg_vs_nat = r"^VE_\w{4}_VSC_L[12]VALD_\w+.DBL.DIR$"
    reg_tile = r"T\d{2}[a-zA-Z]{3}"

    base_resolution = (None, None)

    def __init__(self, filepath, **kwargs):
        """
        Set the path to the root product folder
        :param filepath: The full path to the root product folder
        """
        from os import path as p
        self.fpath = p.realpath(filepath)
        self.base = p.basename(self.fpath)
        self.mnt_resolution = kwargs.get("mnt_resolution", self.base_resolution)

    def __str__(self):
        return "\n".join(["Product:   " + self.base,
                          "Acq-Date:  " + self.date.strftime("%Y-%m-%d %H:%M:%S"),
                          "Platform:  " + self.platform,
                          "Level:     " + self.level,
                          "Tile/Site: " + self.tile,
                          ""])

    def __repr__(self):
        return self.__str__()

    def factory(self):
        """
        Detect the underlying product
        :return:
        """
        import re
        from Chain.S2Product import Sentinel2SSC, Sentinel2Muscate, Sentinel2Natif
        from Chain.L8Product import Landsat8LC1, Landsat8LC2, Landsat8Muscate, Landsat8Natif
        from Chain.VSProduct import VenusNatif, VenusMuscate
        # Sentinel-2
        if re.search(self.reg_s2_nat, self.base):
            return Sentinel2Natif(self.fpath)
        if re.search(self.reg_s2_mus, self.base):
            return Sentinel2Muscate(self.fpath)
        if re.search(self.reg_s2_ssc, self.base):
            return Sentinel2SSC(self.fpath)
        # Landsat-8
        if re.search(self.reg_l8_nat, self.base):
            return Landsat8Natif(self.fpath)
        if re.search(self.reg_l8_mus, self.base):
            return Landsat8Muscate(self.fpath)
        if re.search(self.reg_l8_lc1, self.base):
            return Landsat8LC1(self.fpath)
        if re.search(self.reg_l8_lc2, self.base):
            return Landsat8LC2(self.fpath)
        # Venus
        if re.search(self.reg_vs_mus, self.base):
            return VenusMuscate(self.fpath)
        if re.search(self.reg_vs_nat, self.base):
            return VenusNatif(self.fpath)
        pass

    @property
    def platform(self):
        raise NotImplementedError

    @property
    def tile(self):
        raise NotImplementedError

    @property
    def type(self):
        raise NotImplementedError

    @property
    def type_xml_maja(self):
        platform = self.platform
        ptype = self.type
        types = {"sentinel2": {"natif": "SENTINEL-2_", "muscate": "SENTINEL2_", "ssc": "SENTINEL-2_"},
                 "landsat8": {"lc1": "LANDSAT_8", "lc2": "LANDSAT_8", "muscate": "LANDSAT8"},
                 "venus": {"natif": "VENuS", "muscate": "VENUS"},
                 "spot5": {"muscate": "SPOT5"},
                 "spot4": {"muscate": "SPOT4"},
                 "pleiades": {"muscate": "PLEIADES"}
                 }
        return types[platform][ptype]

    def get_file(self, **kwargs):
        """
        Find file in the root folder
        :param kwargs: filename The filename to search for
        :return: The path to the files found
        """
        from Common.FileSystem import get_file
        return get_file(root=self.fpath, **kwargs)

    def find_file(self, pattern):
        """
        Find file of in any folder-depth
        :param pattern: The pattern to search for
        :return: The path to the files found
        """
        from Common.FileSystem import find
        return find(path=self.fpath, pattern=pattern)

    @property
    def metadata_file(self):
        raise NotImplementedError

    @property
    def level(self):
        raise NotImplementedError

    @property
    def date(self):
        raise NotImplementedError

    @property
    def validity(self):
        raise NotImplementedError

    def link(self, link_dir):
        raise NotImplementedError

    @property
    def mnt_site(self):
        raise NotImplementedError

    @property
    def mnt_resolutions_dict(self):
        raise NotImplementedError

    @property
    def platform_str(self):
        platform_choices = {"sentinel2": "S2_",
                            "landsat8": "L8",
                            "venus": "VE",
                            "spot5": "SPOT5",
                            "spot4": "SPOT4",
                            "pleiades": "PLEIADES"}

        return platform_choices[self.platform]

    def get_mnt(self, **kwargs):
        from prepare_mnt.mnt.MNTFactory import MNTFactory
        return MNTFactory(site=self.mnt_site, platform_id=self.platform_str,
                          mission_field=self.type_xml_maja, mnt_resolutions=self.mnt_resolutions_dict,
                          **kwargs).factory()

    def get_synthetic_band(self, synthetic_band, **kwargs):
        raise NotImplementedError

    def _reproject_to_epsg(self, img, outpath, epsg):
        import shutil
        import tempfile
        from Common import ImageIO
        tmpfile = tempfile.mktemp(prefix="reproject_", suffix=".tif")
        ImageIO.gdal_warp(tmpfile, img, t_srs="EPSG:%s" % epsg,
                          tr=" ".join(map(str, self.base_resolution)),
                          q=True)
        shutil.move(tmpfile, outpath)

    def reproject(self, **kwargs):
        import os
        from Common import FileSystem, ImageIO
        out_dir = kwargs.get("out_dir", self.fpath)
        assert os.path.isdir(out_dir)
        out_dir = os.path.join(out_dir, self.base)
        FileSystem.create_directory(out_dir)
        patterns = kwargs.get("patterns", [r".(tif|jp2)$"])
        imgs = [self.find_file(pattern=p) for p in patterns]
        epsg = kwargs.get("epsg", None)
        # Flatten
        imgs = [i for img in imgs for i in img]
        for img in imgs:
            if not epsg:
                drv = ImageIO.open_tiff(img)
                epsg = ImageIO.get_s2_epsg_code(drv)
            outpath = os.path.join(out_dir, os.path.basename(img))
            self._reproject_to_epsg(img, outpath, epsg)
        return out_dir

    def __lt__(self, other):
        return self.date < other.date

    def __eq__(self, other):
        return self.date == other.date and \
               self.level == other.level and \
               self.metadata_file == other.metadata_file and \
               self.tile == other.tile and \
               self.platform == other.platform
