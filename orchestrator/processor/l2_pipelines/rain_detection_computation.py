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
from orchestrator.cots.otb.otb_app_handler import OtbAppHandler
import orchestrator.common.constants as constants
from orchestrator.cots.otb.algorithms.otb_extract_roi import extract_roi
from orchestrator.modules.maja_module import MajaModule
from orchestrator.common.maja_exceptions import *
import os
LOGGER = configure_logger(__name__)


class MajaRainDetection(MajaModule):
    """
    classdocs
    """
    NAME = "RainDetection"

    def __init__(self):
        """
        Constructor
        """
        super(MajaRainDetection, self).__init__()
        self.in_keys_to_check = ["Params.Caching","Params.SnowBandAvailable", "AppHandler", "Plugin", "L1Reader",
                                 "L2Reader", "L2COMM", "DEM","Params.WaterBandIndex_DateD",
                                 "Params.WaterBandIndex_DateDm1"]
        self.out_keys_to_check = ["RayleighIPTOCR"]
        self.out_keys_provided = ["RainFlag"]

    def run(self, dict_of_input, dict_of_output):
        LOGGER.info("Rain detection computation start")
        rain_working = dict_of_input.get("AppHandler").get_directory_manager().get_temporary_directory("RainFlagProc_",
                                            do_always_remove=True)
        image_list = [dict_of_output.get(constants.CLOUD_MASK_ALL_CLOUDS),
                      dict_of_output.get("WaterMask")]
        image_list.append(dict_of_input.get("L1Reader").get_value("IPEDGSubOutput"))
        image_list.append(dict_of_input.get("L2Reader").get_value("WASImage"))
        image_list.append(dict_of_input.get("L2Reader").get_value("NDTImage"))
        exp = "im1b1 || im2b1 || im3b1 || im4b1 || im5b1"
        or_all = os.path.join(rain_working, "OrAll.tif")

        if dict_of_input.get("Params").get("SnowBandAvailable"):
            image_list.append(dict_of_output.get("cld_snow"))
            exp = exp + " || im6b1"

        param_orall = {"il": image_list,
                       "exp": exp,
                       "out": or_all
                       }
        orall_app = OtbAppHandler("BandMath", param_orall, write_output=False)

        tocr_extract = os.path.join(rain_working, "Tocr_Extract.tif")
        tocrextract_image_app = extract_roi(dict_of_output.get("RayleighIPTOCR"),
                                            [dict_of_input.get("Params").get("WaterBandIndex_DateD")],
                                            tocr_extract, write_output=False)
        rcr_extract = os.path.join(rain_working, "Rcr_Extract.tif")

        rcrextract_image_app = extract_roi(dict_of_input.get("L2Reader").get_value("RCRImage"),
                                           [dict_of_input.get("Params").get("WaterBandIndex_DateDm1")],
                                           rcr_extract, write_output=False)
        param_rcrstat = {"im": rcrextract_image_app.getoutput()["out"],
                         "mask": orall_app.getoutput()["out"],
                         }
        rcrstat_app = OtbAppHandler("Stats", param_rcrstat, write_output=False)
        rcr_mean = rcrstat_app.getoutput()["mean"]

        param_tocrstat = {"im": tocrextract_image_app.getoutput()["out"],
                          "mask": orall_app.getoutput()["out"],
                          }
        tocrstat_app = OtbAppHandler("Stats", param_tocrstat, write_output=False)
        tocr_mean = tocrstat_app.getoutput()["mean"]

        diffMean = rcr_mean - tocr_mean

        if diffMean > dict_of_input.get("L2COMM").get_value_f("ReflectanceDecreaseThreshold"):
            dict_of_output["RainFlag"] = True
        else:
            dict_of_output["RainFlag"] = False
