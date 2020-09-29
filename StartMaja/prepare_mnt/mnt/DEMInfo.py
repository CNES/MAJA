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

from StartMaja.Common.GDalDatasetWrapper import GDalDatasetWrapper
import numpy as np


class DEMInfo:
    def __init__(self, site, dem_full_res):
        self.name = site.nom
        self.epsg = site.epsg_str
        self.ulx = site.ul[0]
        self.uly = site.ul[1]
        self.resx = site.res_x
        self.resy = site.res_y
        self.lx = site.px
        self.ly = site.py
        self.alt = dem_full_res
        driver = GDalDatasetWrapper.from_file(dem_full_res)
        self.mean_alt = np.mean(driver.array)
        self.std_dev_alt = np.std(driver.array)
        self.short_description = driver.utm_description
        self.nodata = driver.nodata_value
        res_arr = driver.resolution
        self.dem_subsampling_ratio = str(int(float(res_arr[0]) / float(site.res_x)))

