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


class MNTFactory:
    """
    Create a given DEM in Maja format
    """
    def __init__(self, site, platform_id, mission_field, mnt_resolutions, **kwargs):
        self.mnt_type = kwargs.get("mnt_type", "srtm")
        self.site = site
        self.plaform_id = platform_id
        self.mission_field = mission_field
        self.mnt_resolutions = mnt_resolutions
        self.coarse_res = kwargs.get("coarse_res", (240, -240))
        self.kwargs = kwargs

    def factory(self):
        """
        Checks the given mnt_type and returns the class accordingly
        :return: A derived MNTBase object, None if mnt_type unknown.
        """
        from prepare_mnt.mnt.SRTM import SRTM
        # TODO Add more options here: ALOS, TDX, EuDEM...
        if self.mnt_type == "srtm":
            return SRTM(site=self.site,
                        **self.kwargs).to_maja_format(platform_id=self.plaform_id,
                                                      mission_field=self.mission_field,
                                                      mnt_resolutions=self.mnt_resolutions,
                                                      coarse_res=self.coarse_res)
        return None
