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
from orchestrator.cots.otb.otb_pipeline_manager import OtbPipelineManager
from orchestrator.modules.maja_module import MajaModule
from orchestrator.common.maja_exceptions import *
from orchestrator.common.maja_utils import is_croco_on
import os
LOGGER = configure_logger(__name__)


class MajaScatteringCorrection(MajaModule):
    """
    classdocs
    """

    NAME = "ScatteringCorrection"

    def __init__(self):
        """
        Constructor
        """
        super(MajaScatteringCorrection, self).__init__()
        self._scattering_sub_app = None
        self.in_keys_to_check = ["Params.Caching", "AppHandler", "Plugin", "L1Reader","L2COMM", "DEM","Params.RealL2NoData"]
        self.out_keys_to_check = ["AtmoAbsIPTOAC","hr_lutmap","AOT_Sub"]
        self.out_keys_provided = ["TOC_sub"]
        self._l2_pipeline = OtbPipelineManager()

    def run(self, dict_of_input, dict_of_output):
        LOGGER.info("Scattering Correction start")
        scat_working = dict_of_input.get("AppHandler").get_directory_manager().get_temporary_directory("ScatteringProc_",
                                            do_always_remove=True)
        l_caching = dict_of_input.get("Params").get("Caching")
        l_writeL2 = dict_of_input.get("Params").get("WriteL2ProductToL2Resolution")
        tocr_sub_image = os.path.join(scat_working, "tocr_sub.tif")
        bands_definition = dict_of_input.get("Plugin").BandsDefinitions
        #  Coarse scattering correction
        param_scattering = {"lutmap": dict_of_output["hr_lutmap"],
                            "toac": dict_of_output["AtmoAbsIPTOAC"],
                            "aot": dict_of_output["AOT_Sub"],
                            "edg": dict_of_input.get("L1Reader").get_value("IPEDGSubOutput"),
                            "dtm": dict_of_input.get("DEM").ALC,
                            "l2nodata": dict_of_input.get("Params").get("RealL2NoData"),
                            "l2bandincoarse": bands_definition.get_list_of_band_id_in_l2_coarse(),
                            "tocr": tocr_sub_image
                            }
        self._scattering_sub_app = OtbAppHandler("ScatteringCorrection", param_scattering, write_output=False)
        dict_of_output["TOC_sub"] = self._scattering_sub_app.getoutput()["tocr"]
        # l2 scattering correction
        tocr_list = []
        if l_writeL2:
            l_nbRes = len(bands_definition.ListOfL2Resolution)
            for r in range(0, l_nbRes):
                l_res = bands_definition.ListOfL2Resolution[r]
                tocr_image = os.path.join(scat_working, "tocr_" + l_res + ".tif")
                if dict_of_input.get("Plugin").WideFieldSensor:
                    param_scattering["anglezone"] = dict_of_input.get("L1Reader").get_value("AngleZoneMaskList")[r]
                param_scattering["dtm"] = dict_of_input.get("DEM").ALTList[r]
                param_scattering["edg"] = dict_of_input.get("L1Reader").get_value("L2EDGOutputList")[r]
                param_scattering["aot"] = dict_of_output["AOT_" + l_res]
                param_scattering["toac"] = dict_of_output["L2TOA_" + l_res]
                param_scattering["tocr"] = tocr_image
                l_l2bandids = bands_definition.get_list_of_l2_coarse_band_id_associated_to_l2_band_code(
                    bands_definition.get_list_of_l2_band_code(l_res))
                param_scattering["l2bandincoarse"] = [str(b) for b in l_l2bandids]
                scat_app = OtbAppHandler("ScatteringCorrection", param_scattering,
                                         write_output=is_croco_on("scatteringcorrection"))
                self._l2_pipeline.add_otb_app(scat_app)
                dict_of_output["TOC_" + l_res] = scat_app.getoutput().get("tocr")
                tocr_list.append(dict_of_output["TOC_" + l_res])
            dict_of_output["L2TOCList"] = tocr_list
