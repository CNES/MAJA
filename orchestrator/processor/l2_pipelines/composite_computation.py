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
import orchestrator.common.constants as constants
import os
LOGGER = configure_logger(__name__)


class MajaCompositeComputation(MajaModule):
    """
    classdocs
    """
    NAME = "Composite"

    def __init__(self):
        """
        Constructor
        """
        super(MajaCompositeComputation, self).__init__()
        self._composite_app = None

    def run(self, dict_of_input, dict_of_output):
        LOGGER.info("Composite computation start")
        caching = dict_of_input.get("Params").get("Caching")
        composite_working = dict_of_input.get("AppHandler").get_directory_manager().get_temporary_directory("CompositeProc_",
                                            do_always_remove=True)
        rta_image = os.path.join(composite_working, "composite_rta.tif")
        rcr_image = os.path.join(composite_working, "composite_rcr.tif")
        rtc_image = os.path.join(composite_working, "composite_rtc.tif")
        sto_image = os.path.join(composite_working, "composite_sto.tif")
        pxd_image = os.path.join(composite_working, "composite_pxd.tif")
        ndt_image = os.path.join(composite_working, "composite_ndt.tif")

        l_cirrusflag = False
        if "CirrusFlag" in dict_of_output:
            l_cirrusflag = dict_of_output["CirrusFlag"]

        l_hotspotflag = False
        if "HotSpotFlag" in dict_of_output:
            l_hotspotflag = dict_of_output["HotSpotFlag"]

        l_rainflag = False
        if "RainFlag" in dict_of_output:
            l_rainflag = dict_of_output["RainFlag"]

        init_Mode = dict_of_input.get("Params").get("InitMode")


        param_composite = {"tocr": dict_of_output["RayleighIPTOCR"],
                           "toac": dict_of_output["AtmoAbsIPTOAC"],
                           "toc": dict_of_output["TOC_sub"],
                           "edg": dict_of_input.get("L1Reader").get_value("IPEDGSubOutput"),
                           "cldsum": dict_of_output[constants.CLOUD_MASK_ALL_CLOUDS],
                           "sat": dict_of_input.get("L1Reader").get_value("IPSATSubOutput"),
                           "aot": dict_of_output["AOT_Sub"],
                           "bandlist": [str(b) for b in dict_of_input.get("Params").get(
                               "ListOfBandIndexForReflectancesComposite_DateD")],
                           "stobandnumber": dict_of_input.get("L2COMM").get_value_i("NumberOfStackImages"),
                           "stobandindex": dict_of_input.get("Params").get(
                               "CorrelBandIndexInReflectancesComposite_DateD"),
                           "aotradius": dict_of_input.get("L2COMM").get_value_i("SEAOTRadius"),
                           "maxaot": dict_of_input.get("L2COMM").get_value_f("MaxAOT"),
                           "minaot": dict_of_input.get("L2COMM").get_value_f("MinimumHighAOTStd"),
                           "sigmaaot": dict_of_input.get("L2COMM").get_value_i("CoefSigmaAOT"),
                           "maxage": dict_of_input.get("L2COMM").get_value_i("MaxAge"),
                           "jday": int(dict_of_input.get("Params").get("JDay")),
                           "reall2nodata": dict_of_input.get("Params").get("RealL2NoData"),
                           "l2nodata": dict_of_input.get("Params").get("L2NoData"),
                           "rta": rta_image,
                           "rcr": rcr_image,
                           "rtc": rtc_image,
                           "sto": sto_image,
                           "pxd": pxd_image,
                           "ndt": ndt_image
                           }
        if init_Mode:
            param_composite["initmode"] = dict_of_input.get("Params").get("InitMode")

        if l_cirrusflag:
            LOGGER.debug("CirrusFlag parameter is set to True")
            param_composite["CirrusFlag"] = l_cirrusflag

        if l_hotspotflag:
            LOGGER.debug("HotSpotFlag parameter is set to True")
            param_composite["HotSpotFlag"] = l_hotspotflag

        if l_rainflag:
            LOGGER.debug("RainFlag parameter is set to True")
            param_composite["RainFlag"] = l_rainflag


        if dict_of_input.get("Params").get("SnowBandAvailable"):
            param_composite["snow"] = dict_of_output["cld_snow"]

        if not dict_of_input.get("Params").get("InitMode"):
            param_composite["l2rta"] = dict_of_input.get("L2Reader").get_value("RTAImage")
            param_composite["l2rcr"] = dict_of_input.get("L2Reader").get_value("RCRImage")
            param_composite["l2rtc"] = dict_of_input.get("L2Reader").get_value("RTCImage")
            param_composite["l2sto"] = dict_of_input.get("L2Reader").get_value("STOImage")
            param_composite["l2pxd"] = dict_of_input.get("L2Reader").get_value("PXDImage")
            param_composite["l2ndt"] = dict_of_input.get("L2Reader").get_value("NDTImage")

        # TODO add multi writer
        self._composite_app = OtbAppHandler("Composite", param_composite, write_output=True)

        dict_of_output["Composite_rta"] = self._composite_app.getoutput()["rta"]
        dict_of_output["Composite_rcr"] = self._composite_app.getoutput()["rcr"]
        dict_of_output["Composite_rtc"] = self._composite_app.getoutput()["rtc"]
        dict_of_output["Composite_sto"] = self._composite_app.getoutput()["sto"]
        dict_of_output["Composite_pxd"] = self._composite_app.getoutput()["pxd"]
        dict_of_output["Composite_ndt"] = self._composite_app.getoutput()["ndt"]

