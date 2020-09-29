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
from orchestrator.modules.maja_module import MajaModule
from orchestrator.common.maja_exceptions import *
from orchestrator.common.constants import *
LOGGER = configure_logger(__name__)


class MajaValidityL2Cloud(MajaModule):
    """
    classdocs
    """

    NAME = "ValidityL2Cloud"

    def __init__(self):
        """
        Constructor
        """
        super(MajaValidityL2Cloud, self).__init__()
        self.in_keys_to_check = []
        self.out_keys_to_check = [CLOUD_MASK_ALL_CLOUDS]
        self.out_keys_provided = ["CloudRate"]

    def run(self, dict_of_input, dict_of_output):
        LOGGER.info("Cloud percentage validity check start")
        param_stats = {"im": dict_of_output.get(CLOUD_MASK_ALL_CLOUDS),
                       "exclude": 1}
        l2_stat = OtbAppHandler("Stats", param_stats)
        # Test on the value
        LOGGER.debug("L2Cloud : excludedcount : "+str(l2_stat.getoutput()["excludedcount"]))
        LOGGER.debug("L2Cloud : validcount : " + str(l2_stat.getoutput()["validcount"]))
        LOGGER.debug("L2Cloud : nodatanumber : " + str(dict_of_output["L2NoDataNumber"]))
        l_cloudrate = 0
        if l2_stat.getoutput()["excludedcount"] == 0 and l2_stat.getoutput()["validcount"] == 0:
            l_cloudrate = 0
        else:
            l_cloudrate = l2_stat.getoutput()["excludedcount"] * 100.0 / \
                (l2_stat.getoutput()["excludedcount"] + l2_stat.getoutput()["validcount"] - dict_of_output["L2NoDataNumber"])
        LOGGER.debug("L2 cloudrate : " + str(l_cloudrate))
        dict_of_output["CloudRate"] = l_cloudrate

