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
import os
from orchestrator.common.maja_exceptions import *
from orchestrator.modules.maja_module import MajaModule
LOGGER = configure_logger(__name__)


class MajaCirrusMaskComputation(MajaModule):
    """
    classdocs
    """
    NAME = "CirrusMask"

    def __init__(self):
        """
        Constructor
        """
        super(MajaCirrusMaskComputation, self).__init__()
        self._cirrus_mask_app = None
        self.in_keys_to_check = [ "AppHandler", "Plugin", "L1Reader","L2COMM", "DEM"]
        self.out_keys_to_check = []
        self.out_keys_provided = ["CirrusFlag", "CirrusMask"]

    def run(self, dict_of_input, dict_of_output):
        LOGGER.info("Cirrus mask computation start")
        cirrus_working = dict_of_input.get("AppHandler").get_directory_manager().get_temporary_directory("CirrusMask_Computation_",
                                            do_always_remove=True)
        cirrus_mask = os.path.join(cirrus_working, "cirrus_mask_sub.tif")
        cirrus_band_code = dict_of_input.get("L2COMM").get_value("CirrusBandCode")
        bands_definition = dict_of_input.get("Plugin").BandsDefinitions
        cirrus_band_idx = bands_definition.get_band_id_in_l2_coarse(cirrus_band_code)
        param_cirrus = {"toa": dict_of_input.get("L1Reader").get_value("IPTOASubOutput"),
                        "edg": dict_of_input.get("L1Reader").get_value("IPEDGSubOutput"),
                        "dtm": dict_of_input.get("DEM").ALC,
                        "bandidx": cirrus_band_idx,
                        "threshoff": float(dict_of_input.get("L2COMM").get_value("CirrusMaskThresholdOffset")),
                        "threshgain": float(dict_of_input.get("L2COMM").get_value("CirrusMaskThresholdGain")),
                        "minpercent": float(dict_of_input.get("L2COMM").get_value("MinPercentMaskCirrus")),
                        "altref" : float(dict_of_input.get("L2COMM").get_value("CirrusAltRef")),
                        "mask": cirrus_mask + ":uint8"
                        }
        self._cirrus_mask_app = OtbAppHandler("CirrusMask", param_cirrus, write_output=True)
        dict_of_output["CirrusMask"] = self._cirrus_mask_app.getoutput()["mask"]
        dict_of_output["CirrusFlag"] = self._cirrus_mask_app.getoutput()["cirrusflag"]
