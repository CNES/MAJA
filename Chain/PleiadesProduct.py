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
from datetime import datetime
from Chain.Product import MajaProduct


class PleiadesTheiaXS(MajaProduct):
    """
    A Pleiades XS product/acquisition from theia
    """

    base_resolution = (10, -10)

    def __init__(self, filepath, **kwargs):
        """
        Pleiades theia acquisitions are stored in 'volumes'.
        Cf: https://www.cscrs.itu.edu.tr/assets/downloads/PleiadesUserGuide.pdf
        All information is captured from the DIM_*XML file inside the product.
        :param filepath: The path to the FCGC* folder
        :param kwargs: Optional arguments
        """
        from Common import XMLTools, FileSystem
        super(PleiadesTheiaXS, self).__init__(filepath, **kwargs)
        self._ms_folder = self.get_file(filename=r"IMG_PHR\d\w_P?MS_\d*")
        self._dim_file = FileSystem.get_file(root=self._ms_folder, filename=r"DIM_PHR*XML")
        self.base = XMLTools.get_single_xpath(self._dim_file, "./Dataset_Identification/DATASET_NAME")

    @property
    def platform(self):
        return "pleiades"

    @property
    def type(self):
        return "raw"

    @property
    def level(self):
        return "l1c"

    @property
    def tile(self):
        return "_".join(self.base.split("_")[-3:-1])

    @property
    def metadata_file(self):
        return self._dim_file

    @property
    def date(self):
        str_date = self.base.split("_")[2]
        # Remove last second digit
        str_date = str_date[:-1]
        return datetime.strptime(str_date, "%Y%m%d%H%M%S")

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
            band_bx = FileSystem.get_file(root=self._ms_folder, filename=r"IMG_PHR*_P?MS*JP2")
        except IOError as e:
            raise e
        return Site.from_raster(self.tile, band_bx, shape_index_y=1, shape_index_x=2)

    @property
    def mnt_resolutions_dict(self):
        return [{"name": "XS",
                "val": str(self.mnt_resolution[0]) + " " + str(self.mnt_resolution[1])}]

    def get_synthetic_band(self, synthetic_band, **kwargs):
        raise NotImplementedError

    def reproject(self, **kwargs):
        from Common import FileSystem, ImageIO

        images = ["XS1", "XS2", "XS3", "SWIR"]
        out_dir = kwargs.get("out_dir", self.fpath)
        remove_temp = kwargs.get("remove_temp", True)
        assert os.path.isdir(out_dir)
        out_dir = os.path.join(out_dir, self.base)
        FileSystem.create_directory(out_dir)
        # Get image
        ms_imgs = FileSystem.find(path=self._ms_folder, pattern=r"IMG_PHR*_P?MS*JP2")
        # TODO Take all images into account
        ms_img = sorted(ms_imgs)[0]
        epsg = kwargs.get("epsg")
        if not epsg:
            drv = ImageIO.open_tiff(ms_img)
            try:
                _ = ImageIO.get_epsg(drv)
            except ValueError:
                # TODO Find a way to add the WKT to the file.
                FileSystem.remove_directory(out_dir)
                return
        ms_bname = os.path.splitext(os.path.basename(ms_img))[0]
        split_bnames = ["%s_%s.tif" % (ms_bname, i) for i in images]
        for i, img in enumerate(split_bnames):
            full_img = os.path.join(out_dir, img)
            ImageIO.gdal_translate(full_img,
                                   ms_img,
                                   tr=" ".join(list(str(i) for i in self.mnt_resolution)),
                                   r="bilinear",
                                   b=str(i+1),
                                   q=True)
        return out_dir


class PleiadesPreprojected(MajaProduct):
    """
    A Pleiades XS product/acquisition from theia
    """

    base_resolution = (10, -10)

    def __init__(self, filepath, **kwargs):
        """
        A reprojected Pleiades XS product
        """
        super(PleiadesPreprojected, self).__init__(filepath, **kwargs)

    @property
    def platform(self):
        return "pleiades"

    @property
    def type(self):
        return "muscate"

    @property
    def level(self):
        return "l1c"

    @property
    def tile(self):
        return "_".join(self.base.split("_")[-3:-1])

    @property
    def metadata_file(self):
        raise NotImplementedError("No Metadata available")

    @property
    def date(self):
        str_date = self.base.split("_")[2]
        # Remove last second digit
        str_date = str_date[:-1]
        return datetime.strptime(str_date, "%Y%m%d%H%M%S")

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
            band_bx = self.get_file(filename=r"IMG_PHR*_*XS1.tif")
        except IOError as e:
            raise e
        return Site.from_raster(self.tile, band_bx)

    @property
    def mnt_resolutions_dict(self):
        return [{"name": "XS",
                "val": str(self.mnt_resolution[0]) + " " + str(self.mnt_resolution[1])}]

    def get_synthetic_band(self, synthetic_band, **kwargs):
        raise NotImplementedError

    def reproject(self, **kwargs):
        raise NotImplementedError("The product is already reprojected.")
