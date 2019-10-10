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


class DEMInfo:
    def __init__(self, site, dem_full_res):
        from Common import ImageIO
        import numpy as np
        self.name = site.nom
        self.epsg = site.epsg_str
        self.ulx = site.ul[0]
        self.uly = site.ul[1]
        self.resx = site.res_x
        self.resy = site.res_y
        self.lx = site.px
        self.ly = site.py
        self.alt = dem_full_res
        arr, drv = ImageIO.tiff_to_array(self.alt, array_only=False)
        self.mean_alt = np.mean(arr)
        self.std_dev_alt = np.std(arr)
        self.short_description = ImageIO.get_utm_description(drv)
        self.nodata = ImageIO.get_nodata_value(drv)
        res_arr = ImageIO.get_resolution(drv)
        self.dem_subsampling_ratio = str(int(float(res_arr[0]) / float(site.res_x)))

