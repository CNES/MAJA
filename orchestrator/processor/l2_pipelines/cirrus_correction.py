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
from orchestrator.common.maja_utils import is_croco_on
from orchestrator.cots.otb.algorithms.otb_write_images import write_images
from orchestrator.cots.otb.algorithms.otb_clean_pipe import clean_pipe
from orchestrator.plugins.common.base.maja_plugin_base import PluginBase
from orchestrator.common.maja_exceptions import *
import orchestrator.common.xml_tools as xml_tools
from orchestrator.common.constants import *
import os
LOGGER = configure_logger(__name__)


class MajaCirrusCorrection(MajaModule):
    """
    classdocs
    """
    NAME = "CirrusCorrection"

    def __init__(self):
        """
        Constructor
        """
        super(MajaCirrusCorrection, self).__init__()
        self.in_keys_to_check = ["Params.Caching", "AppHandler", "Plugin", "L1Reader","L2COMM", "DEM"]
        self.out_keys_to_check = []
        self.out_keys_provided = []
        self._l2_pipeline = OtbPipelineManager()
   
    def run(self, dict_of_input, dict_of_output):
        LOGGER.info("Cirrus Correction start")
        cirrus_working = dict_of_input.get("AppHandler").get_directory_manager().get_temporary_directory("CirrusCorrProc_",
                                            do_always_remove=True)
        refres_name = dict_of_input.get("Plugin").CirrusCorrectionReferenceResolution
        refres_idx = 9999
        bands_definition = dict_of_input.get("Plugin").BandsDefinitions
        for i in range(0, len(bands_definition.ListOfL2Resolution)):
            if bands_definition.ListOfL2Resolution[i] == refres_name:
                refres_idx = i
        # TODO test refres_idx
        result = {}
        ref_result = self._gamma_compute_at_res(dict_of_input, dict_of_output, refres_idx, cirrus_working)
        result[bands_definition.ListOfL2Resolution[refres_idx]] = ref_result
        if ref_result[2]:
            LOGGER.info("Cirrus flag enabled on reference resolution")
            l_gammamean = ref_result[3] * ref_result[4]
            l_nbgamma = ref_result[3]
            for i in range(0, len(bands_definition.ListOfL2Resolution)):
                if bands_definition.ListOfL2Resolution[i] != refres_name:
                    res_result = self._gamma_compute_at_res(dict_of_input, dict_of_output, i, cirrus_working)
                    result[bands_definition.ListOfL2Resolution[i]] = res_result
                    l_gammamean = l_gammamean + res_result[3] * res_result[4]
                    l_nbgamma = l_nbgamma + res_result[3]
            l_gammamean = l_gammamean / l_nbgamma
            l_gamma_max = float(dict_of_input.get("L2COMM").get_value("CirrusCorrectionMaxGammaValue"))
            l_gamma_min = float(dict_of_input.get("L2COMM").get_value("CirrusCorrectionMinGammaValue"))
            # Boundary set for gamma
            if l_gammamean > l_gamma_max:
                l_gammamean = l_gamma_max
            if l_gammamean < l_gamma_min:
                l_gammamean = l_gamma_min

            LOGGER.info("Gamma : " + str(l_gammamean))

            # Apply correction
            for i in range(0, len(bands_definition.ListOfL2Resolution)):
                l_res = bands_definition.ListOfL2Resolution[i]
                corrected_toa = os.path.join(cirrus_working, "toac_" + l_res + ".tif")
                l_gammaswir = l_gammamean * float(
                    dict_of_input.get("L2COMM").get_value("CirrusCorrectionSWIRGammaFactor"))
                l_l2bandcodes = bands_definition.get_list_of_l2_band_code(l_res)
                swir_band_list = xml_tools.as_string_list(
                    dict_of_input.get("L2COMM").get_value("CirrusCorrectionSWIRBandCodes"))
                # Param of cirrus apply app
                param_cirrus_apply = {"l2toa": dict_of_output["AtmoAbsIPTOA_" + l_res],
                                      "l2edg": dict_of_input.get("L1Reader").get_value("L2EDGOutputList")[i],
                                      "l2dtm": dict_of_input.get("DEM").ALTList[i],
                                      "l2cirrus": result[l_res][0],
                                      "cirrusmask": result[l_res][1],
                                      "resolution": bands_definition.get_l1_resolution(l_res),
                                      "maxdistance": float(dict_of_input.get("L2COMM").get_value("CirrusCorrectionMaxDistance")),
                                      "thresholdgain": float(
                                          dict_of_input.get("L2COMM").get_value("CirrusMaskThresholdGain")),
                                      "thresholdoffset": float(
                                          dict_of_input.get("L2COMM").get_value("CirrusMaskThresholdOffset")),
                                      "thresholdfactor": float(
                                          dict_of_input.get("L2COMM").get_value("CirrusCorrectionThresholdFactor")),
                                      "gammamean": l_gammamean,
                                      "swirgammafactor": l_gammaswir,
                                      "maxreflectance": float(dict_of_input.get("L2COMM").get_value("CirrusMaxReflectance")),
                                      "bandcodelist": l_l2bandcodes,
                                      "swirbandcodelist": swir_band_list ,
                                      "correctedtoa": corrected_toa
                                      }
                app = OtbAppHandler("CirrusCorrectionApply", param_cirrus_apply,
                                    write_output=(False or is_croco_on("cirruscorrection")))
                self._l2_pipeline.add_otb_app(app)
                dict_of_output["L2TOA_" + l_res] = app.getoutput().get("correctedtoa")
        else:
            LOGGER.info("Cirrus flag disabled on reference resolution, no cirrus correction")
        # return cirrus corrected flag
        dict_of_output["CirrusCorrected"] = ref_result[2]
        return ref_result[2]

    def _gamma_compute_at_res(self, dict_of_input, dict_of_output, p_res, p_working):
        bands_definition = dict_of_input.get("Plugin").BandsDefinitions
        l_res = bands_definition.ListOfL2Resolution[p_res]
        LOGGER.info("Cirrus gamma compute at res " + l_res + " start")

        # ---------- Resample L1 Cirrus to resolution ---------------
        l1cirrus_resampled = os.path.join(p_working, "toacirrus_" + l_res + ".tif")
        param_l1cirrus_resample = {"dtm": dict_of_input.get("DEM").ALTList[p_res],
                                   "im": dict_of_input.get("L1Reader").get_value("L1TOACirrusImage"),
                                   "interp": "bco",
                                   "out": l1cirrus_resampled
                                   }
        app_l1cirrus_resamp = OtbAppHandler("Resampling", param_l1cirrus_resample, write_output=True)

        # ---------- Resample Cirrus cloud to resolution ---------------
        cld_cirrus_resampled = dict_of_output[CLOUD_MASK_CIRRUS + "_" + l_res]
        cirrusmask_resampled = os.path.join(p_working, "cirrusmask_" + l_res + ".tif")
        # ---------- Resample All cloud to resolution ---------------
        cld_all_resampled = dict_of_output[CLOUD_MASK_ALL + "_" + l_res]
        allmask_resampled = os.path.join(p_working, "allmask_" + l_res + ".tif")
        # ---------- Resample Refl cloud to resolution ---------------
        cld_refl_resampled = dict_of_output[CLOUD_MASK_REFL + "_" + l_res]
        allreflresampled = os.path.join(p_working, "reflmask_" + l_res + ".tif")
        # -----   Caching of L2TOA
        l2toa_cach = os.path.join(p_working, "l2toa_" + l_res + ".tif")
        #Write to caching
        write_images([cld_cirrus_resampled,cld_all_resampled,cld_refl_resampled, dict_of_output["L2TOA_" + l_res]],
                     [cirrusmask_resampled,allmask_resampled,allreflresampled,l2toa_cach])
        # Update dict
        clean_pipe(dict_of_output[CLOUD_MASK_CIRRUS + "_" + l_res])
        clean_pipe(dict_of_output[CLOUD_MASK_ALL + "_" + l_res])
        clean_pipe(dict_of_output[CLOUD_MASK_REFL + "_" + l_res])
        clean_pipe(dict_of_output["L2TOA_" + l_res])
        dict_of_output[CLOUD_MASK_CIRRUS + "_" + l_res] = cirrusmask_resampled
        dict_of_output[CLOUD_MASK_ALL + "_" + l_res] = allmask_resampled
        dict_of_output[CLOUD_MASK_REFL + "_" + l_res] = allreflresampled
        dict_of_output["L2TOA_" + l_res] = l2toa_cach


        # --------------------- Gamma compute for the resolution
        # construct band list
        gamm_band_list = xml_tools.as_string_list(dict_of_input.get(
            "L2COMM").get_value("CirrusCorrectionGammaBandCodes"))
        l_l2bandcodes = bands_definition.get_list_of_l2_band_code(l_res)
        l_resbandlist = []
        for i in range(0, len(l_l2bandcodes)):
            if l_l2bandcodes[i] in gamm_band_list:
                l_resbandlist.append(str(i))
        cirrus_mask = os.path.join(p_working, "cirrus_corr_mask_" + l_res + ".tif")
        param_gamma_compute = {"l2toa": l2toa_cach,
                               "l2edg": dict_of_input.get("L1Reader").get_value("L2EDGOutputList")[p_res],
                               "l2dtm": dict_of_input.get("DEM").ALTList[p_res],
                               "l2cirrus": app_l1cirrus_resamp.getoutput().get("out"),
                               "bandlist": l_resbandlist,
                               "cloudcirrus": cirrusmask_resampled,
                               "cloudall": allmask_resampled,
                               "cloudrefl": allreflresampled,
                               "cirrusminpixinlist": int(dict_of_input.get("L2COMM").get_value("CirrusCorrectionMinPixInList")),
                               "mask": cirrus_mask + ":uint8"}
        gamma_compute_app = OtbAppHandler("GammaCompute", param_gamma_compute, write_output=True)
        cirr_flag = bool(gamma_compute_app.getoutput()["cirrusflag"])
        gamma = float(gamma_compute_app.getoutput()["gamma"])
        del gamma_compute_app
        del app_l1cirrus_resamp
        return [l1cirrus_resampled, cirrus_mask, cirr_flag , len(l_resbandlist),gamma]
