# -*- coding: utf-8 -*-
#
# Copyright (C) 2020 Centre National d'Etudes Spatiales (CNES)
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#    http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#
#
"""
###################################################################################################

                        o     o
                        oo   oo   oo        o   oo        ,-.
                        o o o o  o  o       o  o  o       \_/
                        o  o  o o    o      o o    o     {|||D
                        o     o oooooo      o oooooo      / \
                        o     o o    o o    o o    o      `-^
                        o     o o    o  oooo  o    o

###################################################################################################

orchestrator.algorithms.geometric_flags.geometric_flags -- shortdesc

orchestrator.algorithms.geometric_flags.geometric_flags is a description

It defines classes_and_methods

###################################################################################################
"""


from math import pi, acos, sin, cos

from orchestrator.common.logger import maja_logging
LOGGER = maja_logging.configure_logger(__name__)


class GeometricFlagsGenerator(object):
    """
    This class provides two flags:

    - a sunglint flag which is raised if the geometry is close to the sunglint direction.
    Indeed, above water surfaces, near sun glint direction, reflectances are very dependent on
    wind speed and viewing and solar angles.
    - a hot-spot flag which is raised if the geometry is close to the hot-spot direction.
    Indeed, above land surfaces, near the backscattering direction, quick variation of
    reflectances happen.
    """

    def __init__(self, theta_s, theta_v, phi_s, phi_v, sunglint_threshold, hotspot_threshold):
        """
        Constructor
        """
        self.theta_s = theta_s
        self.theta_v = theta_v
        self.phi_s = phi_s
        self.phi_v = phi_v
        self.sunglint_threshold = sunglint_threshold
        self.hotspot_threshold = hotspot_threshold

    @property
    def sunglint_distance(self):
        theta_s_rad = self.theta_s * pi / 180.
        phi_s_rad = self.phi_s * pi / 180.
        sunglint_distance = []

        for i in range(0, len(self.phi_v)):
            phi_v_rad = self.phi_v[i] * pi / 180.
            theta_v_rad = self.theta_v[i] * pi / 180.
            sunglint_distance_rad = acos(sin(theta_s_rad) * sin(theta_v_rad) * cos(phi_s_rad - phi_v_rad + pi)
                                         + cos(theta_s_rad) * cos(theta_v_rad))
            sunglint_distance.append(sunglint_distance_rad * 180 / pi)

        return sunglint_distance

    def is_sunglint(self):
        """
        defines the flag sunglint

        - compute the sunglint distance
        - compare it to the threshold hard coded in the constants file
        """

        for s in self.sunglint_distance:
            if s < self.sunglint_threshold:
                return True
        return False

    @property
    def hot_spot_distance(self):
        theta_s_rad = self.theta_s * pi / 180.
        phi_s_rad = self.phi_s * pi / 180.
        hotspot_distance = []

        # print theta_v_rad, phi_v_rad, theta_s_rad, phi_s_rad
        for i in range(0, len(self.phi_v)):
            theta_v_rad = self.theta_v[i] * pi / 180.
            phi_v_rad = self.phi_v[i] * pi / 180.
            hotspot_distance_rad = acos(sin(theta_s_rad) * sin(theta_v_rad) * cos(phi_s_rad - phi_v_rad)
                                        + cos(theta_s_rad) * cos(theta_v_rad))
            hotspot_distance.append(hotspot_distance_rad * 180 / pi)

        return hotspot_distance

    def is_hotspot(self):
        """
        defines the flag hotspot

        - compute the hotspot distance
        - compare it to the threshold hard coded in the constants file
        """

        for d in self.hot_spot_distance:
            if d < self.hotspot_threshold:
                return True

        return False
