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

import logging
from StartMaja.prepare_mnt.mnt.SRTM import SRTM
from StartMaja.prepare_mnt.mnt.EuDEM import EuDEM


class MNTFactory:
    """
    Create a given DEM in Maja format
    """
    def __init__(self, site, platform_id, mission_field, mnt_resolutions, **kwargs):
        self.type_dem = kwargs.get("type_dem", "any").lower()
        self.site = site
        self.plaform_id = platform_id
        self.mission_field = mission_field
        self.mnt_resolutions = mnt_resolutions
        self.coarse_res = kwargs.get("coarse_res", None)  # Do not write coarse resolution images if set to None
        self.full_res_only = kwargs.get("full_res_only", False)
        self.kwargs = kwargs

    def factory(self):
        """
        Checks the given mnt_type and returns the class accordingly
        :return: A derived MNTBase object, None if mnt_type unknown.
        """
        error = None
        # TODO Add more options here: ALOS, TDX...
        if self.type_dem in ["eudem", "any"]:
            try:
                return EuDEM(site=self.site,
                             **self.kwargs).to_maja_format(platform_id=self.plaform_id,
                                                           mission_field=self.mission_field,
                                                           mnt_resolutions=self.mnt_resolutions,
                                                           coarse_res=self.coarse_res,
                                                           full_res_only=self.full_res_only)
            except Exception as e:
                error = e
                logger.error(e)

        if self.type_dem in ["srtm", "any"]:
            # SRTM is distributed in 90m.
            # Thus, all initial calculation has to be done at this resolution:
            self.site.res_x, self.site.res_y = 90, -90
            try:
                return SRTM(site=self.site,
                            **self.kwargs).to_maja_format(platform_id=self.plaform_id,
                                                          mission_field=self.mission_field,
                                                          mnt_resolutions=self.mnt_resolutions,
                                                          coarse_res=self.coarse_res,
                                                          full_res_only=self.full_res_only)
            except ValueError as e:
                error = e
                logger.error(e)

        if not error:
            raise ValueError("No DEM type set. Please select 'eudem', 'srtm' or 'all'.")

        raise error


if __name__ == "__main__":
    pass
else:
    logger = logging.getLogger("root")
