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
from orchestrator.modules.maja_module import MajaModule
from orchestrator.common.maja_exceptions import *
from orchestrator.common.logger.maja_logging import configure_logger
from orchestrator.cots.otb.otb_app_handler import OtbAppHandler
import os
LOGGER = configure_logger(__name__)


class MajaCloudMaskUndersampling(MajaModule):
    """
    classdocs
    """

    NAME = "CloudMaskUndersampling"

    def __init__(self):
        """
        Constructor
        """
        super(MajaCloudMaskUndersampling, self).__init__()
        self._undersampling_app = None

    def run(self, dict_of_input, dict_of_output):
        LOGGER.info("Cloud Mask Undersampling start")
        caching = dict_of_input.get("Params").get("Caching")
        cla_working = ""
        if caching:
            cla_working = dict_of_input.get("AppHandler").get_directory_manager().get_temporary_directory("CloudMaskUnderProc_",
                                            do_always_remove=True)
        l1_cla = dict_of_input.get("L1Reader").get_value("CLAImage")
        cla_image = os.path.join(cla_working, "cla_image_sub.tif")
        clalt_image = os.path.join(cla_working, "clalt_image_sub.tif")
        param_resamp_cla = {"dtm": dict_of_input.get("DEM").ALC,
                            "im": l1_cla,
                            "nodata": dict_of_input.get("L1Info").RealL1NoData,
                            "cla": cla_image,
                            "clalt": clalt_image}
        self._undersampling_app = OtbAppHandler("CloudMaskUndersampling", param_resamp_cla, write_output=caching)
        dict_of_output["CLA_Sub"] = self._undersampling_app.getoutput()["cla"]
        dict_of_output["CLALT_Sub"] = self._undersampling_app.getoutput()["clalt"]

