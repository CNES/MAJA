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
from orchestrator.cots.otb.algorithms.otb_constant_image import constant_image
import os
from orchestrator.common.directory_manager import DirectoryManager
from orchestrator.modules.maja_module import MajaModule
from orchestrator.common.maja_exceptions import *
LOGGER = configure_logger(__name__)


class MajaAOTEstimationConstant(MajaModule):
    """
    classdocs
    """
    NAME = "AOTEstimationConstant"

    def __init__(self):
        """
        Constructor
         """
        super(MajaAOTEstimationConstant, self).__init__()
        self._constant_app = None

        self.in_keys_to_check = ["Params.Caching", "Params.WriteL2ProductToL2Resolution",
                                 "AppHandler", "Plugin", "L2COMM", "DEM"
                                 ]

        self.out_keys_to_check = []
        self.out_keys_provided = ["AOT_Sub"]

    def check_requirements(self, dict_of_input, dict_of_output):
        raise MajaNotYetImplemented("Function must be defined in a module")

    def check_conditions(self, dict_of_input, dict_of_output):
        raise MajaNotYetImplemented("Function must be defined in a module")

    def run(self, dict_of_input, dict_of_output):
        LOGGER.info("AOT constant start")
        LOGGER.debug("Caching %s", dict_of_input.get("Params").get("Caching"))
        l_caching = dict_of_input.get("Params").get("Caching")
        l_writeL2 = dict_of_input.get("Params").get("WriteL2ProductToL2Resolution")
        aot_working = dict_of_input.get("AppHandler").get_directory_manager().get_temporary_directory("AOTConstantProc_",
                                            do_always_remove=True)
        aot_default = float(dict_of_input.get("L2COMM").get_value("DefaultAOT"))
        # Coarse AOT constant
        aot_sub_image = os.path.join(aot_working, "aot_sub.tif")
        self._constant_app = constant_image(
            dict_of_input.get("DEM").ALC,
            aot_default,
            aot_sub_image,
            write_output=l_caching)
        dict_of_output["AOT_Sub"] = self._constant_app.getoutput()["out"]

        # L2 AOT
        if l_writeL2:
            bands_definition = dict_of_input.get("Plugin").BandsDefinitions
            l_nbRes = len(bands_definition.ListOfL2Resolution)
            aot_list = []
            aotmask_list = []
            for r in range(0, l_nbRes):
                l_res = bands_definition.ListOfL2Resolution[r]
                aot_image = os.path.join(aot_working, "aot_" + l_res + ".tif")
                aot_mask = os.path.join(aot_working, "aotmask_" + l_res + ".tif:uint8")
                dict_of_output["AOT_" + l_res] = constant_image(dict_of_input.get("DEM").ALTList[r], aot_default,
                                                                aot_image, write_output=True).getoutput()["out"]
                dict_of_output["AOTMASK_" + l_res] = constant_image(dict_of_input.get("DEM").ALTList[r], 0,
                                                                    aot_mask, write_output=True).getoutput()["out"]
                aot_list.append(dict_of_output["AOT_" + l_res])
                aotmask_list.append(dict_of_output["AOTMASK_" + l_res])
            dict_of_output["L2AOTList"] = aot_list
            dict_of_output["L2AOTMaskList"] = aotmask_list

