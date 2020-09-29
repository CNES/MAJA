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


class MajaSnowMaskComputation(MajaModule):
    """
    classdocs
    """

    NAME = "SnowMask"

    def __init__(self):
        """
        Constructor
        """
        super(MajaSnowMaskComputation, self).__init__()
        self._incidence_app = None
        self._snow_app = None
        self.in_keys_to_check = ["Params.Caching", "AppHandler", "Plugin", "L1Reader","L2COMM", "DEM",
                                 "L1Info"]
        self.out_keys_to_check = ["RayleighIPTOCR"]
        self.out_keys_provided = ["SnowMask"]

    def run(self, dict_of_input, dict_of_output):
        LOGGER.info("Snow mask computation start")
        caching = dict_of_input.get("Params").get("Caching")
        if caching:
            snow_working = dict_of_input.get("AppHandler").get_directory_manager().get_temporary_directory("SnowMaskProc_",
                                            do_always_remove=True)
        else:
            snow_working = ""
        snow_mask = os.path.join(snow_working, "snow_mask_sub.tif")
        incidence_image = os.path.join(snow_working, "incidence_angles_sub.tif")
        ref_band_code = dict_of_input.get("L2COMM").get_value("GreenBandCode")
        red_band_code = dict_of_input.get("L2COMM").get_value("RedBandCode")
        l_SwirBandAvailable = dict_of_input.get("Params").get("SwirBandAvailable")
        abs_band_code = dict_of_input.get("L2COMM").get_value("SWIR1BandCode")
        bands_definition = dict_of_input.get("Plugin").BandsDefinitions
        ref_band_idx = bands_definition.get_band_id_in_l2_coarse(ref_band_code)
        red_band_idx = bands_definition.get_band_id_in_l2_coarse(red_band_code)
        abs_band_idx = bands_definition.get_band_id_in_l2_coarse(abs_band_code)

        if dict_of_input.get("DEM").ASC is not None:
            param_incidence = {"demasc": dict_of_input.get("DEM").ASC,
                               "demslc": dict_of_input.get("DEM").SLC,
                               "thetas": float(dict_of_input.get("L1Info").SolarAngle["sun_zenith_angle"]),
                               "phis": float(dict_of_input.get("L1Info").SolarAngle["sun_azimuth_angle"]),
                               "angles": incidence_image}
            self._incidence_app = OtbAppHandler("IncidenceAngle", param_incidence, write_output=False)
            dict_of_output["SolarIncidenceAngle"] = self._incidence_app.getoutput()["angles"]

        param_snow = {"tocr": dict_of_output["RayleighIPTOCR"],
                      "edg": dict_of_input.get("L1Reader").get_value("IPEDGSubOutput"),
                      "refband": ref_band_idx,
                      "absband": abs_band_idx,
                      "redband": red_band_idx,
                      "seradius": dict_of_input.get("L2COMM").get_value_i("SESnowRadius"),
                      "ndsithresh": dict_of_input.get("L2COMM").get_value_f("NDSIThreshold"),
                      "redthresh": dict_of_input.get("L2COMM").get_value_f("RedReflectanceThreshold"),
                      "thetas": float(dict_of_input.get("L1Info").SolarAngle["sun_zenith_angle"]),
                      "mincos": float(dict_of_input.get("L2COMM").get_value("SlopeMinCosI")),
                      "mask": snow_mask + ":uint8"
                      }
        if self._incidence_app is not None:
            param_snow["angles"] = self._incidence_app.getoutput()["angles"]
        if l_SwirBandAvailable:
            param_snow["swirthresh"] = dict_of_input.get("L2COMM").get_value_f("SWIR1ReflectanceThreshold")
        if "CirrusMask" in dict_of_output:
            param_snow["cirrusmask"] = dict_of_output.get("CirrusMask")
        if not dict_of_input.get("Params").get("InitMode") and dict_of_input.get("L2COMM").get_value("UseWaterMask"):
            param_snow["watermask"] = dict_of_input.get("L2Reader").get_value("WASImage")

        self._snow_app = OtbAppHandler("SnowMask", param_snow, write_output=False)

        dict_of_output["SnowMask"] = self._snow_app.getoutput()["mask"]

