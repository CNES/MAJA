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

###################################################################################################
"""
from orchestrator.common.logger.maja_logging import configure_logger
from orchestrator.cots.otb.otb_app_handler import OtbAppHandler
from orchestrator.modules.maja_module import MajaModule
from orchestrator.common.maja_exceptions import *
import orchestrator.common.constants as constants
import os
LOGGER = configure_logger(__name__)


class MajaWaterMasking(MajaModule):
    """
    classdocs
    """

    NAME = "WaterMask"

    def __init__(self):
        """
        Constructor
        """
        super(MajaWaterMasking, self).__init__()
        self._water_app = None
        self.in_keys_to_check = ["Params.Caching","Params.InitMode", "AppHandler", "Plugin", "L1Reader","L2Reader","L2COMM", "DEM"]
        self.out_keys_to_check = ["RayleighIPTOCR",constants.CLOUD_MASK_ALL_CLOUDS,"dtm_shd","SunglintFlag"]
        self.out_keys_provided = ["WaterMask", "PossibleWaterMask", "TestedWaterMask"]

    def run(self, dict_of_input, dict_of_output):
        LOGGER.info("Water masking computation start")

        caching = dict_of_input.get("Params").get("Caching")
        if caching:
            water_working = dict_of_input.get("AppHandler").get_directory_manager().get_temporary_directory("WaterMaskingProc_",
                                            do_always_remove=True)
        else:
            water_working = ""

        water_mask = os.path.join(water_working, "water_sub.tif")
        possible_water_mask = os.path.join(water_working, "possible_water_sub.tif")
        tested_water_mask = os.path.join(water_working, "tested_water_sub.tif")
        param_water = {"tocr": dict_of_output.get("RayleighIPTOCR"),
                       "edg": dict_of_input.get("L1Reader").get_value("IPEDGSubOutput"),
                       "cldsum": dict_of_output.get(constants.CLOUD_MASK_ALL_CLOUDS),
                       "dtmshd": dict_of_output["dtm_shd"],
                       "srtmwat": dict_of_input.get("DEM").MSK,
                       "demslc": dict_of_input.get("DEM").SLC,
                       "redbandtocr": dict_of_input.get("Params").get("RedBandIndex_DateD"),
                       "nirbandtocr": dict_of_input.get("Params").get("NIRBandIndex_DateD"),
                       "nirbandrcr": dict_of_input.get("Params").get("NIRBandIndex_DateDm1"),
                       "waterndvithreshold": dict_of_input.get("L2COMM").get_value_f("WaterNDVIThreshold"),
                       "waterreflectancevariationthreshold": dict_of_input.get("L2COMM").get_value_f(
                           "WaterReflectanceVariationThreshold"),
                       "maximumsunglintreflectance": dict_of_input.get("L2COMM").get_value_f(
                           "MaximumSunglintReflectance"),
                       "srtmgaussiansigma": dict_of_input.get("L2COMM").get_value_f("SRTMSigmaSmoothing"),
                       "srtmpercent": dict_of_input.get("L2COMM").get_value_f("SRTMPercent"),
                       "watermasknumber": dict_of_input.get("L2COMM").get_value_i("WaterMaskNumber"),
                       "minpercentpossiblewater": dict_of_input.get("L2COMM").get_value_f("MinPercentPossibleWater"),
                       "waterslopethreshold": dict_of_input.get("L2COMM").get_value_f("WaterSlopeThreshold"),
                       "waterredreflectancethreshold": dict_of_input.get("L2COMM").get_value_f(
                           "WaterRedReflectanceThreshold"),
                       "reall2nodata": dict_of_input.get("Params").get("RealL2NoData"),
                       "was": water_mask + ":uint8",
                       "pwa": possible_water_mask + ":uint8",
                       "twa": tested_water_mask + ":uint8"
                       }

        if not dict_of_input.get("Params").get("InitMode"):
            param_water["l2rcr"] = dict_of_input.get("L2Reader").get_value("RCRImage")
            param_water["l2pwa"] = dict_of_input.get("L2Reader").get_value("PWAImage")
            param_water["l2twa"] = dict_of_input.get("L2Reader").get_value("TWAImage")

        if dict_of_input.get("Params").get("InitMode"):
            param_water["initmode"] = dict_of_input.get("Params").get("InitMode")

        if dict_of_output["SunglintFlag"]:
            param_water["sunglintflag"] = dict_of_output["SunglintFlag"]

        self._water_app = OtbAppHandler("WaterMask", param_water, write_output=True)

        #  Update output dictionary
        dict_of_output["WaterMask"] = self._water_app.getoutput()["was"]
        dict_of_output["PossibleWaterMask"] = self._water_app.getoutput()["pwa"]
        dict_of_output["TestedWaterMask"] = self._water_app.getoutput()["twa"]

