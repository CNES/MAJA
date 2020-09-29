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
from orchestrator.common.xml_tools import *
from orchestrator.modules.maja_module import MajaModule
from orchestrator.common.maja_exceptions import *
import os
LOGGER = configure_logger(__name__)


class MajaDTMProcessing(MajaModule):
    """
    classdocs
    """

    NAME = "DTMProcessing"

    def __init__(self):
        """
        Constructor
        """
        super(MajaDTMProcessing, self).__init__()
        self._solar_app = None
        self._dtm_app = None
        self.in_keys_to_check = ["Params.Caching", "AppHandler", "Plugin", "L1Reader","L2COMM", "DEM"]
        self.out_keys_to_check = []
        self.out_keys_provided = ["dtm_shd", "dtm_hid"]

    def run(self, dict_of_input, dict_of_output):
        LOGGER.info("DTMProcessing computation start")
        LOGGER.debug("Caching %s", dict_of_input.get("Params").get("Caching"))
        dtm_working = dict_of_input.get("AppHandler").get_directory_manager().get_temporary_directory("DTmProc_",
                                            do_always_remove=True)
        solar_imge = os.path.join(dtm_working, "solar_angle.tif")
        hid_imge = os.path.join(dtm_working, "dtm_hid.tif")
        shd_imge = os.path.join(dtm_working, "dtm_shd.tif")
        grid_ref_alt = dict_of_input.get("Plugin").ConfigUserCamera.get_Algorithms().get_GRID_Reference_Altitudes()
        param_solar = {"sol1.in": dict_of_input.get("L1Reader").get_value("SOL1Image"),
                       "sol1.h": grid_ref_alt.get_SOLH1(),
                       "solhref": grid_ref_alt.get_SOLHRef(),
                       "sol": solar_imge}
        if dict_of_input.get("Plugin").GRIDReferenceAltitudesSOL2GridAvailable:
            param_solar["sol2.in"] = dict_of_input.get("L1Reader").get_value("SOL2Image")
            param_solar["sol2.h"] = grid_ref_alt.get_SOLH2()

        self._solar_app = OtbAppHandler("SolarAngle", param_solar, write_output=True)

        param_dtm = {"viedtm": dict_of_input.get("L1Reader").get_value("DTMVIEImage"),
                     "edg": dict_of_input.get("L1Reader").get_value("IPEDGSubOutput"),
                     "distthreshold": float(dict_of_input.get("L2COMM").get_value("DistanceThreshold")),
                     "solhref": self._solar_app.getoutput().get("outhref"),
                     "viehref": grid_ref_alt.get_VIEHRef(),
                     "hid": hid_imge,
                     "shd": shd_imge,
                     "dtm": dict_of_input.get("DEM").ALC,
                     "solar": self._solar_app.getoutput()["sol"]
                     }
        self._dtm_app = OtbAppHandler("DTMProcessing", param_dtm, write_output=True)
        # Update global dict with new output
        dict_of_output["dtm_shd"] = self._dtm_app.getoutput()["shd"]
        dict_of_output["dtm_hid"] = self._dtm_app.getoutput()["hid"]

