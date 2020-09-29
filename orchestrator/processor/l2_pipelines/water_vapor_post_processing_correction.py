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
from orchestrator.common.xml_tools import as_bool
import orchestrator.common.constants as constants
from orchestrator.cots.otb.algorithms.otb_constant_image import constant_image
from orchestrator.cots.otb.algorithms.otb_write_images import write_images
from orchestrator.common.maja_utils import is_croco_on
from orchestrator.modules.maja_module import MajaModule

import os
LOGGER = configure_logger(__name__)


class MajaWaterVaporPostProcessing(MajaModule):
    """
    classdocs
    """

    NAME = "WaterVaporPostProcessing"

    def __init__(self):
        """
        Constructor
        """
        super(MajaWaterVaporPostProcessing, self).__init__()
        self.in_keys_to_check = ["Params.Caching", "AppHandler", "Plugin", "L1Reader","L2Reader","L2COMM", "DEM"]
        self.out_keys_to_check = ["RayleighIPTOCR","dtm_shd","SunglintFlag"]
        self.out_keys_provided = ["L2VAPList", "L2VAPMaskList"]
        self._l2_pipeline = OtbPipelineManager()

    def run(self, dict_of_input, dict_of_output):
        LOGGER.info("Water vapor post processing Correction start")
        caching = dict_of_input.get("Params").get("Caching")
        wvpp_working = dict_of_input.get("AppHandler").get_directory_manager().get_temporary_directory("WaterVaporPPProc_",
                                            do_always_remove=True)
        vap_default = float(dict_of_input.get("L2COMM").get_value("WaterAmountDefaultValue"))
        bands_definition = dict_of_input.get("Plugin").BandsDefinitions
        l_nbRes = len(bands_definition.ListOfL2Resolution)
        # Does the user choose to compute the VAP image ? (GIP_L2COMM_UseDefaultConstantWaterAmount)
        l_UseDefaultConstantWaterAmount = False
        if not dict_of_input.get("Plugin").WaterVapourDetermination or as_bool(
                dict_of_input.get("L2COMM").get_value("GIP_L2COMM_UseDefaultConstantWaterAmount")):
            l_UseDefaultConstantWaterAmount = True

        vap_list = []
        vapmask_list = []
        if l_UseDefaultConstantWaterAmount:
            for r in range(0, l_nbRes):
                l_res = bands_definition.ListOfL2Resolution[r]
                vap_image_filename = os.path.join(wvpp_working, "vap_" + l_res + ".tif")
                vap_mask_filename = os.path.join(wvpp_working, "vapmask_" + l_res + ".tif:uint8")
                vap_image = constant_image(dict_of_input.get("DEM").ALTList[r], vap_default, vap_image_filename,write_output=False)
                self._l2_pipeline.add_otb_app(vap_image)
                dict_of_output["VAP_" + l_res] = vap_image.getoutput()["out"]
                vap_mask = constant_image(dict_of_input.get("DEM").ALTList[r], 0, vap_mask_filename,write_output=False)
                self._l2_pipeline.add_otb_app(vap_mask)
                dict_of_output["VAPMASK_" + l_res] = vap_mask.getoutput()["out"]
                vap_list.append(dict_of_output["VAP_" + l_res])
                vapmask_list.append(dict_of_output["VAPMASK_" + l_res])
        else:
            l_refbandcode = dict_of_input.get("L2COMM").get_value("WaterVaporReferenceBandCode")
            l_bandcode = dict_of_input.get("L2COMM").get_value("WaterVaporBandCode")
            vap_subimage_filename = os.path.join(wvpp_working, "vap_sub.tif")
            vap_submask_filename = os.path.join(wvpp_working, "vapmask_sub.tif")
            param_watervpp = {"tocr": dict_of_output["RayleighIPTOCR"],
                              "vapin": dict_of_output["VAP_Sub"],
                              "cldsum": dict_of_output[constants.CLOUD_MASK_ALL],
                              "defaultwatervapor": vap_default,
                              "threshold": dict_of_input.get("L2COMM").get_value_f("WaterVaporReflectanceThreshold"),
                              "thresholdref": dict_of_input.get("L2COMM").get_value_f(
                                  "WaterVaporReflectanceThresholdRef"),
                              "referencebandcode": bands_definition.get_band_id_in_l2_coarse(l_refbandcode),
                              "bandcode": bands_definition.get_band_id_in_l2_coarse(l_bandcode),
                              "nodata": dict_of_input.get("L2COMM").get_value_f("VAPNodataValue"),
                              "sevalidradius": dict_of_input.get("L2COMM").get_value_i("WaterVaporSEValidRadius"),
                              "initwindowradius": dict_of_input.get("L2COMM").get_value_i(
                                  "WaterVaporInitWindowRadius"),
                              "maxwindowradius": dict_of_input.get("L2COMM").get_value_i("WaterVaporMaxWindowRadius"),
                              "mask": vap_submask_filename + ":uint8",
                              "vap": vap_subimage_filename
                              }
            watervpp_app = OtbAppHandler("WaterVaporPostPro", param_watervpp, write_output=False)
            self._l2_pipeline.add_otb_app(watervpp_app)

            for r in range(0, l_nbRes):
                l_res = bands_definition.ListOfL2Resolution[r]
                vap_image_filename = os.path.join(wvpp_working, "vap_" + l_res + ".tif")
                vap_mask_filename = os.path.join(wvpp_working, "vapmask_" + l_res + ".tif")
                param_subresampling = {"dtm": dict_of_input.get("DEM").ALTList[r],
                                       "im": watervpp_app.getoutput()["vap"],
                                       "interp": "linear",
                                       "out": vap_image_filename
                                       }
                subresampling_app = OtbAppHandler("Resampling", param_subresampling, write_output=False)
                self._l2_pipeline.add_otb_app(subresampling_app)

                param_maskresampling = {"dtm": dict_of_input.get("DEM").ALTList[r],
                                        "im": watervpp_app.getoutput()["mask"],
                                        "interp": "linear",
                                        "out": vap_mask_filename + ":uint8",
                                        "threshold": 0.25
                                        }
                maskresampling_app = OtbAppHandler("Resampling", param_maskresampling, write_output=False)
                self._l2_pipeline.add_otb_app(maskresampling_app)
                if is_croco_on("watervaporpostprocessing"):
                    write_images([subresampling_app.getoutput()["out"],maskresampling_app.getoutput()["out"]],
                                 [vap_image_filename,vap_mask_filename])
                    dict_of_output["VAP_" + l_res] = vap_image_filename
                    dict_of_output["VAPMASK_" + l_res] = vap_mask_filename
                else:
                    dict_of_output["VAP_" + l_res] = subresampling_app.getoutput()["out"]
                    dict_of_output["VAPMASK_" + l_res] = maskresampling_app.getoutput()["out"]
                vap_list.append(dict_of_output["VAP_" + l_res])
                vapmask_list.append(dict_of_output["VAPMASK_" + l_res])
        dict_of_output["L2VAPList"] = vap_list
        dict_of_output["L2VAPMaskList"] = vapmask_list

