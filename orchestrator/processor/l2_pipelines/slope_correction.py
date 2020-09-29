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
from orchestrator.cots.otb.algorithms.otb_extract_roi import extract_roi
from orchestrator.cots.otb.algorithms.otb_write_images import write_images
from orchestrator.common.maja_utils import is_croco_on
from orchestrator.modules.maja_module import MajaModule
from orchestrator.common.maja_exceptions import *
import os
LOGGER = configure_logger(__name__)


class MajaSlopeCorrection(MajaModule):
    """
    classdocs
    """

    NAME = "SlopeCorrection"

    def __init__(self):
        """
        Constructor
        """
        super(MajaSlopeCorrection, self).__init__()
        self.in_keys_to_check = ["Params.Caching", "AppHandler", "Plugin", "L1Reader","L2COMM", "DEM",
                                "L2DIRT", "L2DIFT","L1Info"]
        self.out_keys_to_check = ["AOT_Sub"]
        self.out_keys_provided = ["FRE_List","STL_List","TGS_List"]
        self._l2_app_pipeline = OtbPipelineManager()

    def run(self, dict_of_input, dict_of_output):
        LOGGER.info("Slope Correction start")
        slope_working = dict_of_input.get("AppHandler").get_directory_manager().get_temporary_directory("SlopeCorrProc_",
                                            do_always_remove=True)
        caching = dict_of_input.get("Params").get("Caching")
        # Compute transmission
        tdif_image = os.path.join(slope_working, "tdif_sub.tif")
        tdir_image = os.path.join(slope_working, "tdir_sub.tif")
        param_interpolate = {"luttdir": dict_of_input.get("L2DIRT"),
                             "luttdif": dict_of_input.get("L2DIFT"),
                             "aot": dict_of_output["AOT_Sub"],
                             "dtm": dict_of_input.get("DEM").ALC,
                             "edg": dict_of_input.get("L1Reader").get_value("IPEDGSubOutput"),
                             "thetas": float(dict_of_input.get("L1Info").SolarAngle["sun_zenith_angle"]),
                             "tdir": tdir_image,
                             "tdif": tdif_image
                             }
        interpolate_app = OtbAppHandler("ComputeTransmission", param_interpolate, write_output=True)

        # Compute env corr for each l2 resolution
        bands_definition = dict_of_input.get("Plugin").BandsDefinitions
        l_nbRes = len(bands_definition.ListOfL2Resolution)
        fre_list = []
        stl_list = []
        tgs_list = []
        for r in range(0, l_nbRes):
            l_res = bands_definition.ListOfL2Resolution[r]
            l_l2bandcodes = bands_definition.get_list_of_l2_band_code(l_res)
            l_l2bandidx = [bands_definition.get_band_id_in_l2_coarse(b) for b in l_l2bandcodes]
            l_l2bandchannels = ["Channel" + str(b + 1) for b in l_l2bandidx]
            tdifl2_filename = os.path.join(slope_working, "tdif_" + l_res + ".tif")
            tdirl2_filename = os.path.join(slope_working, "tdir_" + l_res + ".tif")
            fre_image = os.path.join(slope_working, "fre_" + l_res + ".tif")
            tgs_image = os.path.join(slope_working, "tgs_" + l_res + ".tif")
            stl_image = os.path.join(slope_working, "stl_" + l_res + ".tif")
            incangle_image = os.path.join(slope_working, "incangle_" + l_res + ".tif")
            # Extract tdif
            tdifl2_app = extract_roi(
                interpolate_app.getoutput()["tdif"],
                l_l2bandidx,
                tdifl2_filename,write_output=False)
            self._l2_app_pipeline.add_otb_app(tdifl2_app)
            tdifl2_image = tdifl2_app.getoutput()["out"]
            # Extract tdir
            tdirl2_app = extract_roi(
                interpolate_app.getoutput()["tdir"],
                l_l2bandidx,
                tdirl2_filename, write_output=False)
            self._l2_app_pipeline.add_otb_app(tdirl2_app)
            tdirl2_image = tdirl2_app.getoutput()["out"]
            # Compute incidence angles
            param_incangle = {"demasc": dict_of_input.get("DEM").ASPList[r],
                              "demslc": dict_of_input.get("DEM").SLPList[r],
                              "thetas": float(dict_of_input.get("L1Info").SolarAngle["sun_zenith_angle"]),
                              "phis": float(dict_of_input.get("L1Info").SolarAngle["sun_azimuth_angle"]),
                              "angles": incangle_image
                              }
            incangle_app = OtbAppHandler("IncidenceAngle", param_incangle,write_output=False)
            self._l2_app_pipeline.add_otb_app(incangle_app)
            # Compute slope correction
            l_thetav = []
            l_phiv = []

            for ang in [dict_of_input.get("L1Info").ListOfViewingAnglesPerBandAtL2CoarseResolution[b] for b in l_l2bandidx]:
                l_thetav.append(ang.get("incidence_zenith_angle"))
                l_phiv.append(ang.get("incidence_azimuth_angle"))


            param_slopecorr = {"tdirsub": tdirl2_image,
                               "tdifsub": tdifl2_image,
                               "dtmasc": dict_of_input.get("DEM").ASPList[r],
                               "dtmslc": dict_of_input.get("DEM").SLPList[r],
                               "rhoenv": dict_of_output["RhoEnv_" + l_res],
                               "incangle": incangle_app.getoutput().get("angles"),
                               "sre": dict_of_output["SRE_" + l_res],
                               "thetas": float(dict_of_input.get("L1Info").SolarAngle["sun_zenith_angle"]),
                               "phis": float(dict_of_input.get("L1Info").SolarAngle["sun_azimuth_angle"]),
                               "thetav": l_thetav,
                               "phiv": l_phiv,
                               "mincosi": dict_of_input.get("L2COMM").get_value_f("SlopeMinCosI"),
                               "mincose": dict_of_input.get("L2COMM").get_value_f("MinCosE"),
                               "mincosratio": dict_of_input.get("L2COMM").get_value_f("MinCosRatio"),
                               "nodata": dict_of_input.get("Params").get("RealL2NoData"),
                               "fre": fre_image,
                               "tgs": tgs_image,
                               "stl": stl_image
                               }
            slope_app = OtbAppHandler("SlopeCorrection", param_slopecorr,write_output=False)
            self._l2_app_pipeline.add_otb_app(slope_app)
            if is_croco_on("slopecorrection"):
                dict_of_output["FRE_" + l_res] = fre_image
                dict_of_output["TGS_" + l_res] = tgs_image
                dict_of_output["STL_" + l_res] = stl_image
                write_images([slope_app.getoutput().get("fre"),slope_app.getoutput().get("tgs"),slope_app.getoutput().get("stl")],
                             [fre_image,tgs_image,stl_image])
            else:
                dict_of_output["FRE_" + l_res] = slope_app.getoutput().get("fre")
                dict_of_output["TGS_" + l_res] = slope_app.getoutput().get("tgs")
                dict_of_output["STL_" + l_res] = slope_app.getoutput().get("stl")
            fre_list.append(dict_of_output["FRE_" + l_res])
            tgs_list.append(dict_of_output["TGS_" + l_res])
            stl_list.append(dict_of_output["STL_" + l_res])
        dict_of_output["FRE_List"] = fre_list
        dict_of_output["TGS_List"] = tgs_list
        dict_of_output["STL_List"] = stl_list

