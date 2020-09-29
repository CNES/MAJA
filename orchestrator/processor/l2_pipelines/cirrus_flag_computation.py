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


class MajaCirrusFlagComputation(MajaModule):
    """
    classdocs
    """
    NAME = "CirrusFlag"

    def __init__(self):
        """
        Constructor
        """
        super(MajaCirrusFlagComputation, self).__init__()
        self.in_keys_to_check = ["L1Reader","L2COMM", "L1Info"]
        self.out_keys_to_check = ["CLA_Sub"]
        self.out_keys_provided = ["CirrusFlag"]

    def run(self, dict_of_input, dict_of_output):
        LOGGER.info("Cirrus flag computation start")
        param_cirrus = {"cla": dict_of_output["CLA_Sub"],
                        "edg": dict_of_input.get("L1Reader").get_value("IPEDGSubOutput"),
                        "altthreshold": int(dict_of_input.get("L2COMM").get_value("AltitudeThreshold")),
                        "minpercentcirrus": float(dict_of_input.get("L2COMM").get_value("MinPercentCirrus")),
                        "minpercentcloud": float(dict_of_input.get("L2COMM").get_value("MinPercentCloud")),
                        "minstdcirrus": float(dict_of_input.get("L2COMM").get_value("MinRstdCirrus")),
                        "minpercentstdcirrus": float(dict_of_input.get("L2COMM").get_value("MinPercentStdCirrus")),
                        "nodata": dict_of_input.get("L1Info").RealL1NoData
                        }
        cirrus_mask_app = OtbAppHandler("CirrusFlag", param_cirrus, write_output=True)
        dict_of_output["CirrusFlag"] = cirrus_mask_app.getoutput()["cirrusflag"]
        cirrus_mask_app = None

