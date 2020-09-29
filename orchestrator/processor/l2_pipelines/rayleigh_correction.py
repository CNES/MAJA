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
import os
LOGGER = configure_logger(__name__)


class MajaRayleighCorrection(MajaModule):
    """
    classdocs
    """

    NAME = "RayleighCorrection"

    def __init__(self):
        """
        Constructor
        """
        super(MajaRayleighCorrection, self).__init__()
        self._rayleigh_app = None
        self.in_keys_to_check = ["Params.Caching", "AppHandler", "Plugin", "L1Reader","L2COMM", "DEM",
                                 "L2TOCR","Params.RealL2NoData"]
        self.out_keys_to_check = ["AtmoAbsIPTOAC","cr_lut"]
        self.out_keys_provided = ["RayleighIPTOCR"]

    def run(self, dict_of_input, dict_of_output):
        LOGGER.info("Rayleigh Correction start")
        caching = dict_of_input.get("Params").get("Caching")
        if caching:
            ray_working = dict_of_input.get("AppHandler").get_directory_manager().get_temporary_directory("RayleighProc_",
                                            do_always_remove=True)
        else:
            ray_working = ""

        tocr_image = os.path.join(ray_working, "tocr_sub.tif")
        param_rayleigh = {"lutimage": dict_of_output.get("cr_lut"),
                          "lutheader": dict_of_input.get("L2TOCR"),
                          "toa": dict_of_output.get("AtmoAbsIPTOAC"),
                          "edg": dict_of_input.get("L1Reader").get_value("IPEDGSubOutput"),
                          "dtm": dict_of_input.get("DEM").ALC,
                          "nodata": dict_of_input.get("Params").get("RealL2NoData"),
                          "defaultaot": float(dict_of_input.get("L2COMM").get_value("RayleighCorrectionDefaultAOT")),
                          "tocr": tocr_image}
        self._rayleigh_app = OtbAppHandler("RayleighCorrection", param_rayleigh, write_output=True)
        dict_of_output["RayleighIPTOCR"] = self._rayleigh_app.getoutput()["tocr"]

