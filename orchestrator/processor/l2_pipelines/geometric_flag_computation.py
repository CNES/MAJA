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
"""
###################################################################################################
#
#                        o     o
#                        oo   oo   oo        o   oo        ,-.
#                        o o o o  o  o       o  o  o       \_/
#                        o  o  o o    o      o o    o     {|||)<
#                        o     o oooooo      o oooooo      / \
#                        o     o o    o o    o o    o      `-^
#                        o     o o    o  oooo  o    o
#
###################################################################################################

orchestrator.processor.base_processor -- shortdesc

orchestrator.processor.base_processor is the base of all processors

It defines method mandatory for a processor


###################################################################################################
"""
from orchestrator.common.logger.maja_logging import configure_logger
from orchestrator.algorithms.geometric_flags.geometric_flags import GeometricFlagsGenerator
from orchestrator.modules.maja_module import MajaModule
from orchestrator.common.maja_exceptions import *
LOGGER = configure_logger(__name__)


class MajaGeometricFlag(MajaModule):
    """
    classdocs
    """

    NAME = "GeometricFlag"

    def __init__(self):
        """
        Constructor
        """
        super(MajaGeometricFlag, self).__init__()
        self.in_keys_to_check = ["L1Info", "L2COMM"]
        self.out_keys_to_check = []
        self.out_keys_provided = ["HotSpotFlag", "SunglintFlag"]

    def run(self, dict_of_input, dict_of_output):
        LOGGER.info("Geometric Flag start")
        sunglintthreshold = float(dict_of_input.get("L2COMM").get_value("SunglintThreshold"))
        hotspotthreshold = float(dict_of_input.get("L2COMM").get_value("HotspotThreshold"))
        solarzenith = float(dict_of_input.get("L1Info").SolarAngle["sun_zenith_angle"])
        solarazimuth = float(dict_of_input.get("L1Info").SolarAngle["sun_zenith_angle"])
        viewing_zenith = [float(i) for i in
                          dict_of_input.get("L1Info").ListOfViewingZenithAnglesPerBandAtL2CoarseResolution]
        viewing_azimuth = [float(i) for i in
                           dict_of_input.get("L1Info").ListOfViewingAzimuthAnglesPerBandAtL2CoarseResolution]

        maja_object = GeometricFlagsGenerator(solarzenith, viewing_zenith, solarazimuth, viewing_azimuth,
                                              sunglintthreshold, hotspotthreshold)

        dict_of_output["SunglintFlag"] = maja_object.is_sunglint()
        dict_of_output["HotSpotFlag"] = maja_object.is_hotspot()

