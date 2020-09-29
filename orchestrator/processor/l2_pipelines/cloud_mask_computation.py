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
import orchestrator.common.constants as constants
from orchestrator.cots.otb.algorithms.otb_constant_image import constant_image
from orchestrator.cots.otb.otb_file_utils import otb_copy_image_to_file
from orchestrator.cots.otb.otb_pipeline_manager import OtbPipelineManager
from orchestrator.common.maja_exceptions import *
from orchestrator.modules.maja_module import MajaModule

import os
LOGGER = configure_logger(__name__)


class MajaCloudMaskComputation(MajaModule):
    """
    classdocs
    """

    NAME = "CloudMask"

    def __init__(self):
        """
        Constructor
        """
        super(MajaCloudMaskComputation, self).__init__()
        self.in_keys_to_check = ["Params.Caching", "Params.BlueBandIndex_DateD", "Params.RedBandIndex_DateD",
                                 "Params.NIRBandIndex_DateD", "Params.CorrelBandIndex_DateD",
                                 "Params.ShadowBandIndex_DateD", "Params.ReflectanceWaterBandIndex_DateD",
                                 "Params.InitMode", "Params.SWIRBandAvailable","Params.StoListOfDates",
                                 "AppHandler", "Plugin", "L1Reader","L2Reader", "L2COMM", "DEM"]
        self.out_keys_to_check = ["SunglintFlag", "RayleighIPTOCR" , "CLA_Sub"]
        self.out_keys_provided = ["cld_l2cla", "CLDList",
                                  constants.CLOUD_MASK_ALL, constants.CLOUD_MASK_ALL_CLOUDS,
                                  constants.CLOUD_MASK_SHADOWS, constants.CLOUD_MASK_SHADVAR,
                                  constants.CLOUD_MASK_REFL, constants.CLOUD_MASK_REFL_VAR,
                                  constants.CLOUD_MASK_EXTENSION, constants.CLOUD_MASK_EXTENSION,
                                  constants.CLOUD_MASK_ALT, constants.CLOUD_MASK_CIRRUS
                                  ]
        self._apps = OtbPipelineManager()

    def cleanup(self):
        self._apps.free_otb_app()

    @staticmethod
    def _dilate_and_mask(input, temp, output, mask, radius, caching):
        param_dilate = {"in": input,
                        "out": temp + ":uint8",
                        "structype": "ball",
                        "xradius": radius,
                        "yradius": radius,
                        "filter": "dilate"
                        }
        dilate_app = OtbAppHandler("BinaryMorphologicalOperation", param_dilate, write_output=caching)

        param_mask = {"il": [dilate_app.getoutput()["out"], mask],
                      "out": output + ":uint8",
                      "exp": "(im2b1 !=0)?0:im1b1"
                      }
        mask_app = OtbAppHandler("BandMath", param_mask, write_output=caching)
        return mask_app

    def run(self, dict_of_input, dict_of_output):
        LOGGER.info("Cloud mask computation start")
        init_Mode = dict_of_input.get("Params").get("InitMode")
        cloud_working = ""
        caching = dict_of_input.get("Params").get("Caching")
        if caching:
            cloud_working = dict_of_input.get("AppHandler").get_directory_manager().get_temporary_directory("CloudMaskingProc_",
                                            do_always_remove=True)
        cloud_reflectance_filename = os.path.join(cloud_working, "cloud_refl.tif")
        cloud_reflext_filename = os.path.join(cloud_working, "cloud_reflext.tif")
        cloud_reflvar_filename = os.path.join(cloud_working, "cloud_reflvar.tif")
        const_mask_filename = os.path.join(cloud_working, "cloud_constzero.tif")

        water_treshold = float(dict_of_input.get("L2COMM").get_value("WaterReflectanceThreshold"))
        if dict_of_output.get("SunglintFlag"):
            water_treshold = float(dict_of_input.get("L2COMM").get_value("WaterSunglintReflectanceThreshold"))

        bluebandtocr_idx = dict_of_input.get("Params").get("BlueBandIndex_DateD")
        redbandtocr_idx = dict_of_input.get("Params").get("RedBandIndex_DateD")
        nirbandtocr_idx = dict_of_input.get("Params").get("NIRBandIndex_DateD")
        correlbandtocr_idx = dict_of_input.get("Params").get("CorrelBandIndex_DateD")
        shadowbandtocr_idx = dict_of_input.get("Params").get("ShadowBandIndex_DateD")
        reflwaterband_idx = dict_of_input.get("Params").get("ReflectanceWaterBandIndex_DateD")
        bluebandrcr_idx = 999
        redbandrcr_idx = 999
        nirbandrcr_idx = 999
        correlbandrcr_idx = 999
        shadowbandrcr_idx = 999

        # ---------------------------------------------------------------
        # ------------------- Constant zero mask --------------------------
        constant_app = constant_image(
            dict_of_input.get("DEM").ALC,
            0,
            const_mask_filename +
            ":uint8",
            write_output=caching)
        constant_mask = constant_app.getoutput()["out"]
        # ---------------------------------------------------------------
        # ------------------- Relfectance app --------------------------
        compute_reflvar = False
        param_reflectance = {"tocr": dict_of_output.get("RayleighIPTOCR"),
                             "edg": dict_of_input.get("L1Reader").get_value("IPEDGSubOutput"),
                             "sat": dict_of_input.get("L1Reader").get_value("IPSATSubOutput"),
                             "waterthreshold": water_treshold,
                             "bluebandtocr": bluebandtocr_idx,
                             "redbandtocr": redbandtocr_idx,
                             "nirbandtocr": nirbandtocr_idx,
                             "correlbandtocr": correlbandtocr_idx,
                             "bluereflectancethresholdvalue": dict_of_input.get("L2COMM").get_value_f(
                                 "CloudBlueReflectanceThreshold"),
                             "redreflectancethresholdvalue": dict_of_input.get("L2COMM").get_value_f(
                                 "CloudRedReflectanceThreshold"),
                             "nirredratiomaxthresholdvalue": dict_of_input.get("L2COMM").get_value_f(
                                 "NirRedRatioMaxThreshold"),
                             "nirredratiominthresholdvalue": dict_of_input.get("L2COMM").get_value_f(
                                 "NirRedRatioMinThreshold"),
                             "nodata": dict_of_input.get("Params").get("RealL2NoData"),
                             "foregroundvalue": 1,
                             "reflectancewaterband": reflwaterband_idx,
                             "refl": cloud_reflectance_filename + ":uint8"
                             }


        if dict_of_input.get("Params").get("InitMode"):
            param_reflectance["initmode"] = dict_of_input.get("Params").get("InitMode")

        if dict_of_input.get("Params").get("SnowBandAvailable"):
            param_reflectance["snow"] = dict_of_output.get("SnowMask")


        if dict_of_input.get("Params").get("SWIRBandAvailable"):
            bands_definition = dict_of_input.get("Plugin").BandsDefinitions
            # Get some datas
            swirbandidx = bands_definition.get_band_id_in_l2_coarse(dict_of_input.get("L2COMM").get_value("SWIR1BandCode"))
            param_reflectance["swir.bandtocr"] = swirbandidx
            param_reflectance["swir.nirswirratiomin"] = dict_of_input.get("L2COMM").get_value_f(
                "NirSwirRatioMinThreshold")

        if not init_Mode:
            compute_reflvar = True
            bluebandrcr_idx = dict_of_input.get("Params").get("BlueBandIndex_DateDm1")
            redbandrcr_idx = dict_of_input.get("Params").get("RedBandIndex_DateDm1")
            nirbandrcr_idx = dict_of_input.get("Params").get("NIRBandIndex_DateDm1")
            correlbandrcr_idx = dict_of_input.get("Params").get("CorrelBandIndex_DateDm1")
            shadowbandrcr_idx = dict_of_input.get("Params").get("ShadowBandIndex_DateDm1")
            param_reflectance_nominal = {"l2was": dict_of_input.get("L2Reader").get_value("WASImage"),
                                         "l2rcr": dict_of_input.get("L2Reader").get_value("RCRImage"),
                                         "l2sto": dict_of_input.get("L2Reader").get_value("STOImage"),
                                         "l2pxd": dict_of_input.get("L2Reader").get_value("PXDImage"),
                                         "l2ndt": dict_of_input.get("L2Reader").get_value("NDTImage"),
                                         "nominal.seradius": dict_of_input.get("L2COMM").get_value_i(
                                             "SEInputMasksRadius"),
                                         "nominal.bluebandrcr": bluebandrcr_idx,
                                         "nominal.redbandrcr": redbandrcr_idx,
                                         "nominal.nirbandrcr": nirbandrcr_idx,
                                         "nominal.correlbandrcr": correlbandrcr_idx,
                                         "nominal.jday": dict_of_input.get("Params").get("JDay"),
                                         "nominal.cloudforgettingduration": dict_of_input.get("L2COMM").get_value_i(
                                             "CloudForgettingDuration"),
                                         "nominal.minvarthreshold": dict_of_input.get("L2COMM").get_value_f(
                                             "MinThresholdVarBlue"),
                                         "nominal.maxvarthreshold": dict_of_input.get("L2COMM").get_value_f(
                                             "MaxThresholdVarBlue"),
                                         "nominal.knirblue": dict_of_input.get("L2COMM").get_value_f("KNIRBlue"),
                                         "nominal.kredblue": dict_of_input.get("L2COMM").get_value_f("KRedBlue"),
                                         "nominal.deltathreshold": dict_of_input.get("L2COMM").get_value_f(
                                             "DeltaThreshold"),
                                         "nominal.minpixelcorrelation": dict_of_input.get("L2COMM").get_value_f(
                                             "MinPixelCorrel"),
                                         "nominal.correlthreshold": float(
                                             dict_of_input.get("L2COMM").get_value_f("CorrelThreshold")) / 100.0,
                                         "nominal.ncorrel": dict_of_input.get("L2COMM").get_value_f("NCorrel"),
                                         "nominal.stolistofdates": dict_of_input.get("Params").get("StoListOfDates"),
                                         "reflvar": cloud_reflvar_filename + ":uint8",
                                         "reflext": cloud_reflext_filename + ":uint8"
                                         }
            param_reflectance.update(param_reflectance_nominal)

        # Launch the app
        reflectance_app = OtbAppHandler("CloudReflectance", param_reflectance, write_output=caching)
        self._apps.add_otb_app(reflectance_app)
        cloud_reflectance_image = reflectance_app.getoutput()["refl"]
        cloud_reflext_image = reflectance_app.getoutput()["reflext"]
        if not init_Mode:
            cloud_reflectvar_image = reflectance_app.getoutput()["reflvar"]
        else:
            cloud_reflectvar_image = constant_mask
        # ------------------- Relfectance app ---------------------------
        # ---------------------------------------------------------------

        # ---------------------------------------------------------------
        # ------------------- Snow app ---------------------------------
        tmp_reflectance_image = os.path.join(cloud_working, "cloud_refl_snow.tif")
        if dict_of_input.get("Params").get("SnowBandAvailable"):
            tmp_snow_mask = os.path.join(cloud_working, "cloud_snow_mask.tif")
            param_snow = {"snowin": dict_of_output.get("SnowMask"),
                          "edg": dict_of_input.get("L1Reader").get_value("IPEDGSubOutput"),
                          "cldrefl": cloud_reflectance_image,
                          "seradius": dict_of_input.get("L2COMM").get_value_i("SECloudRadius"),
                          "refl": tmp_reflectance_image + ":uint8",
                          "cldreflvar": cloud_reflectvar_image,
                          "snow": tmp_snow_mask + ":uint8"
                          }
            snow_app = OtbAppHandler("CloudSnowCorrection", param_snow, write_output=caching)
            self._apps.add_otb_app(snow_app)
            cloud_reflectance_image = snow_app.getoutput()["refl"]
            dict_of_output["cld_snow"] = snow_app.getoutput()["snow"]

        # ------------------- Snow app ---------------------------------
        # --------------------------------------------------------------

        # ---------------------------------------------------------------
        # ------------------- Extension app ---------------------------------
        compute_extension = False
        cloud_extension_filename = os.path.join(cloud_working, "cloud_extension.tif")
        if not init_Mode and compute_reflvar:
            compute_extension = True
            param_extension = {"tocr": dict_of_output.get("RayleighIPTOCR"),
                               "edg": dict_of_input.get("L1Reader").get_value("IPEDGSubOutput"),
                               "cldext": cloud_reflext_image,
                               "cldrefl": cloud_reflectance_image,
                               "cldreflvar": cloud_reflectvar_image,
                               "l2rcr": dict_of_input.get("L2Reader").get_value("RCRImage"),
                               "l2sto": dict_of_input.get("L2Reader").get_value("STOImage"),
                               "l2ndt": dict_of_input.get("L2Reader").get_value("NDTImage"),
                               "nodata": dict_of_input.get("Params").get("RealL2NoData"),
                               "correlbandtocr": correlbandtocr_idx,
                               "correlbandrcr": correlbandrcr_idx,
                               "jday": dict_of_input.get("Params").get("JDay"),
                               "minpixelcorrelation": dict_of_input.get("L2COMM").get_value_f("MinPixelCorrel"),
                               "correlthreshold": float(
                                   dict_of_input.get("L2COMM").get_value_f("CorrelThreshold")) / 100.0,
                               "ncorrel": dict_of_input.get("L2COMM").get_value_f("NCorrel"),
                               "discardedratiomin": float(
                                   dict_of_input.get("L2COMM").get_value_f("DiscardedPixelsRatioMin")) / 100.0,
                               "stolistofdates": dict_of_input.get("Params").get("StoListOfDates"),
                               "ext": cloud_extension_filename + ":uint8"
                               }
            extension_app = OtbAppHandler("CloudExtension", param_extension, write_output=caching)
            self._apps.add_otb_app(extension_app)
            cloud_extension_image = extension_app.getoutput()["ext"]
        else:
            cloud_extension_image = constant_mask

        # ------------------- Extension app ---------------------------------
        # ---------------------------------------------------------------

        # ---------------------------------------------------------------
        # ------------------- Cld all concatenate------------------------
        cloud_allnoext_filename = os.path.join(cloud_working, "cloud_all_noextension.tif")
        cloud_all_filename = cloud_allnoext_filename
        param_allnoext = {}
        if dict_of_input.get("Params").get("CirrusBandAvailable"):
            param_allnoext = {"il": [cloud_reflectance_image, cloud_reflectvar_image, dict_of_output.get("CLA_Sub"),
                                     dict_of_output.get("CirrusMask")],
                              "exp": "(im1b1 !=0) || (im2b1!=0) || (im3b1 !=0) || (im4b1 !=0)",
                              "out": cloud_allnoext_filename + ":uint8"
                              }
        else:
            param_allnoext = {"il": [cloud_reflectance_image, cloud_reflectvar_image, dict_of_output.get("CLA_Sub")],
                              "exp": "(im1b1 !=0) || (im2b1!=0) || (im3b1 !=0)",
                              "out": cloud_allnoext_filename + ":uint8"
                              }
        allnoext_app = OtbAppHandler("BandMath", param_allnoext, write_output=caching)
        self._apps.add_otb_app(allnoext_app)
        cloud_allnoext_image = allnoext_app.getoutput()["out"]
        cloud_all_image = allnoext_app.getoutput()["out"]
        if not init_Mode:
            cloud_all_filename = os.path.join(cloud_working, "cloud_all.tif")
            param_all = {"il": [cloud_allnoext_image, cloud_extension_image],
                         "exp": "(im1b1 !=0) || (im2b1!=0)",
                         "out": cloud_all_filename + ":uint8"
                         }
            all_app = OtbAppHandler("BandMath", param_all, write_output=caching)
            self._apps.add_otb_app(all_app)
            cloud_all_image = all_app.getoutput()["out"]
        # ------------------- Cld all concatenate------------------------
        # ---------------------------------------------------------------

        # ---------------------------------------------------------------
        # ------------------- Cld shadow ------------------------
        #caching of ShadowVIE that is know for causing troubles
        shadowvie_filename = os.path.join(cloud_working, "shadow_vie.tif")
        otb_copy_image_to_file(dict_of_input.get("L1Reader").get_value("ShadowVIEImage"),shadowvie_filename)

        cloud_shadow_filename = os.path.join(cloud_working, "cloud_shadow.tif")
        cloud_cla_filename = os.path.join(cloud_working, "cloud_cla.tif")
        grid_ref_alt = dict_of_input.get("Plugin").ConfigUserCamera.get_Algorithms().get_GRID_Reference_Altitudes()
        param_shadow = {"tocr": dict_of_output.get("RayleighIPTOCR"),
                        "edg": dict_of_input.get("L1Reader").get_value("IPEDGSubOutput"),
                        "cldall": cloud_allnoext_image,
                        "cla": dict_of_output.get("CLA_Sub"),
                        "vie": shadowvie_filename,
                        "dtm": dict_of_input.get("DEM").ALC,
                        "sol1.in": dict_of_input.get("L1Reader").get_value("SOL1Image"),
                        "sol1.h": grid_ref_alt.get_SOLH1(),
                        "solhref": grid_ref_alt.get_SOLHRef(),
                        "defaultalt": dict_of_input.get("L2COMM").get_value_i("DefaultAltitude"),
                        "deltahmax": dict_of_input.get("L2COMM").get_value_i("DeltaHMax"),
                        "deltahmin": dict_of_input.get("L2COMM").get_value_i("DeltaHMin"),
                        "deltahstep": dict_of_input.get("L2COMM").get_value_i("DeltaHStep"),
                        "shadbandtocr": shadowbandtocr_idx,
                        "shadbandrcr": shadowbandrcr_idx,
                        "ksigma": 0,
                        "viehref": grid_ref_alt.get_VIEHRef(),
                        "nodata": dict_of_input.get("Params").get("RealL2NoData"),
                        "shadow": cloud_shadow_filename + ":uint8"
                        }

        if dict_of_input.get("Params").get("InitMode"):
            param_shadow["initmode"] = dict_of_input.get("Params").get("InitMode")

        # With alt shadows
        if dict_of_input.get("Params").get("CloudMaskingKnownCloudsAltitude"):
            param_shadow["algo"] = "withalt"
            param_shadow["ksigma"] = int(dict_of_input.get("L2COMM").get_value("KsigmaAltitude"))
            param_shadow["l2cla"] = cloud_cla_filename + ":int16"
            param_shadow["algo.withalt.absnbpixthresh"] = dict_of_input.get("L2COMM").get_value_f("AbsNbpixThreshold")
            param_shadow["algo.withalt.refinement"] = dict_of_input.get("L2COMM").get_value("RefinementOption")
            param_shadow["algo.withalt.threshdiffim"] = dict_of_input.get("L2COMM").get_value_f("ThresholdDiffImage")
            if dict_of_input.get("Plugin").GRIDReferenceAltitudesSOL2GridAvailable:
                param_shadow["algo.withalt.sol2.in"] = dict_of_input.get("L1Reader").get_value("SOL2Image")
                param_shadow["algo.withalt.sol2.h"] = grid_ref_alt.get_SOLH2()

        else:
            param_shadow["algo"] = "withoutalt"
            param_shadow["algo.withoutalt.cloudcovermult"] = dict_of_input.get("L2COMM").get_value_f(
                "CloudCoverMultFactor")
            param_shadow["algo.withoutalt.shadratiomax"] = dict_of_input.get("L2COMM").get_value_f(
                "ShadowCloudRatioMax")
            param_shadow["algo.withoutalt.shadratiomin"] = dict_of_input.get("L2COMM").get_value_f(
                "ShadowCloudRatioMin")
            param_shadow["algo.withoutalt.shadratiopixnb"] = dict_of_input.get("L2COMM").get_value_i(
                "ShadowCloudRatioPixNb")
            param_shadow["algo.withoutalt.maxdarkening"] = dict_of_input.get("L2COMM").get_value_f(
                "ProjShadDarkeningMax")
            param_shadow["algo.withoutalt.minpercentile"] = float(
                dict_of_input.get("L2COMM").get_value_f("DarkeningMinPercentile")) / 100.0
            param_shadow["algo.withoutalt.seradius"] = dict_of_input.get("L2COMM").get_value_i("SECloudRadius")
            dict_of_output["cld_l2cla"] = None

        if not init_Mode:
            param_shadow["l2was"] = dict_of_input.get("L2Reader").get_value("WASImage")
            param_shadow["l2rcr"] = dict_of_input.get("L2Reader").get_value("RCRImage")
            param_shadow["l2ndt"] = dict_of_input.get("L2Reader").get_value("NDTImage")

        shadow_app = OtbAppHandler("CloudShadow", param_shadow, write_output=caching)
        self._apps.add_otb_app(shadow_app)
        cloud_shadow_image = shadow_app.getoutput()["shadow"]
        if dict_of_input.get("Params").get("CloudMaskingKnownCloudsAltitude"):
            dict_of_output["cld_l2cla"] = shadow_app.getoutput()["l2cla"]

        # ------------------- Cld shadow ------------------------
        # -------------------------------------------------------

        # -------------------------------------------------------
        # ------------------- Cld shadvar ------------------------
        compute_shadvar = False
        cloud_shadvar_filename = os.path.join(cloud_working, "cloud_shadvar.tif")
        if not init_Mode:
            param_shadcount = {"im": cloud_shadow_image,
                               "exclude": 1}
            shadcount_app = OtbAppHandler("Stats", param_shadcount)
            if shadcount_app.getoutput()["excludedcount"] > 0:
                compute_shadvar = True
                param_shadvar = {"tocr": dict_of_output.get("RayleighIPTOCR"),
                                 "edg": dict_of_input.get("L1Reader").get_value("IPEDGSubOutput"),
                                 "cldall": cloud_allnoext_image,
                                 "cldshad": cloud_shadow_image,
                                 "vie": dict_of_input.get("L1Reader").get_value("ShadowVIEImage"),
                                 "dtm": dict_of_input.get("DEM").ALC,
                                 "sol": dict_of_input.get("L1Reader").get_value("SOL1Image"),
                                 "l2rcr": dict_of_input.get("L2Reader").get_value("RCRImage"),
                                 "l2was": dict_of_input.get("L2Reader").get_value("WASImage"),
                                 "l2ndt": dict_of_input.get("L2Reader").get_value("NDTImage"),
                                 "l2shad": dict_of_input.get("L2Reader").get_value("VectorizedCLDSubOutput")[
                                     constants.CLOUD_MASK_SHADOWS],
                                 "l2shadvar": dict_of_input.get("L2Reader").get_value("VectorizedCLDSubOutput")[
                                     constants.CLOUD_MASK_SHADVAR],
                                 "l2sto": dict_of_input.get("L2Reader").get_value("STOImage"),
                                 "shadbandtocr": shadowbandtocr_idx,
                                 "shadbandrcr": shadowbandrcr_idx,
                                 "viehref": grid_ref_alt.get_VIEHRef(),
                                 "solhref": grid_ref_alt.get_SOLH1(),
                                 "nodata": dict_of_input.get("Params").get("RealL2NoData"),
                                 "maxdarkening": dict_of_input.get("L2COMM").get_value_f("ShadVarDarkeningMax"),
                                 "minpercentile": float(
                                     dict_of_input.get("L2COMM").get_value_f("ShadVarPercentile")) / 100.0,
                                 "maxcla": dict_of_input.get("L2COMM").get_value_f("MaxCloudAltitude"),
                                 "compositelabelratio": dict_of_input.get("L2COMM").get_value_f(
                                     "ShadVarCompositeLabelRatio"),
                                 "minpixelcorrelation": dict_of_input.get("L2COMM").get_value_f("MinPixelCorrel"),
                                 "correlthreshold": float(
                                     dict_of_input.get("L2COMM").get_value_f("CorrelThreshold")) / 100.0,
                                 "ncorrel": dict_of_input.get("L2COMM").get_value_f("NCorrel"),
                                 "stolistofdates": dict_of_input.get("Params").get("StoListOfDates"),
                                 "jday": dict_of_input.get("Params").get("JDay"),
                                 "shadvar": cloud_shadvar_filename + ":uint8"
                                 }


                shadvar_app = OtbAppHandler("CloudShadVar", param_shadvar, write_output=caching)
                self._apps.add_otb_app(shadvar_app)
                cloud_shadvar_image = shadvar_app.getoutput()["shadvar"]

        if not compute_shadvar:
            cloud_shadvar_image = constant_mask
        # ------------------- Cld shadvar ------------------------
        # -------------------------------------------------------

        # -------------------------------------------------------
        # ------------------- Cld Sum build ---------------------
        cloud_sum_filename = os.path.join(cloud_working, "cloud_sum.tif")
        param_sum = {}
        if not compute_shadvar:
            param_sum = {"il": [cloud_all_image, cloud_shadow_image],
                         "exp": "(im1b1 !=0) || (im2b1!=0)",
                         "out": cloud_sum_filename + ":uint8"
                         }
        else:
            param_sum = {"il": [cloud_all_image, cloud_shadow_image, cloud_shadvar_image],
                         "exp": "(im1b1 !=0) || (im2b1!=0) || (im3b1!=0)",
                         "out": cloud_sum_filename + ":uint8"
                         }
        sum_app = OtbAppHandler("BandMath", param_sum, write_output=caching)
        self._apps.add_otb_app(sum_app)
        cloud_sum_image = sum_app.getoutput()["out"]
        # ------------------- Cld Sum build ---------------------
        # -------------------------------------------------------

        # -------------------------------------------------------
        # ------------------- Cld dilate masks ---------------------

        caching_dilate = True
        if caching_dilate:
            cloud_working = dict_of_input.get("AppHandler").get_directory_manager().get_temporary_directory("CloudMaskingProc_",
                                            do_always_remove=True)
        cloud_reflectance_dilated_filename = os.path.join(cloud_working, "cloud_refl_dilated.tif")
        cloud_reflvar_dilated_filename = os.path.join(cloud_working, "cloud_reflvar_dilated.tif")
        cloud_extension_dilated_filename = os.path.join(cloud_working, "cloud_extension_dilated.tif")
        cloud_all_dilated_filename = os.path.join(cloud_working, "cloud_all_dilated.tif")
        cloud_shadow_dilated_filename = os.path.join(cloud_working, "cloud_shadow_dilated.tif")
        cloud_shadvar_dilated_filename = os.path.join(cloud_working, "cloud_shadvar_dilated.tif")
        cloud_sum_dilated_filename = os.path.join(cloud_working, "cloud_sum_dilated.tif")
        cloud_alt_dilated_filename = os.path.join(cloud_working, "cloud_alt_dilated.tif")
        cloud_cirrus_dilated_filename = os.path.join(cloud_working, "cloud_cirrus_dilated.tif")
        cloud_reflectance_dilated_masked_filename = os.path.join(cloud_working, "cloud_refl_dilated_masked.tif")
        cloud_reflvar_dilated_masked_filename = os.path.join(cloud_working, "cloud_reflvar_dilated_masked.tif")
        cloud_extension_dilated_masked_filename = os.path.join(cloud_working, "cloud_extension_dilated_masked.tif")
        cloud_all_dilated_masked_filename = os.path.join(cloud_working, "cloud_all_dilated_masked.tif")
        cloud_shadow_dilated_masked_filename = os.path.join(cloud_working, "cloud_shadow_dilated_masked.tif")
        cloud_shadvar_dilated_masked_filename = os.path.join(cloud_working, "cloud_shadvar_dilated_masked.tif")
        cloud_sum_dilated_masked_filename = os.path.join(cloud_working, "cloud_sum_dilated_masked.tif")
        cloud_alt_dilated_masked_filename = os.path.join(cloud_working, "cloud_alt_dilated_masked.tif")
        cloud_cirrus_dilated_masked_filename = os.path.join(cloud_working, "cloud_cirrus_dilated_masked.tif")
        mask = dict_of_input.get("L1Reader").get_value("IPEDGSubOutput")
        radius = dict_of_input.get("L2COMM").get_value_i("SECloudRadius")
        cloud_reflectance_dilated_masked_app = MajaCloudMaskComputation._dilate_and_mask(
            cloud_reflectance_image,
            cloud_reflectance_dilated_filename,
            cloud_reflectance_dilated_masked_filename,
            mask,
            radius,
            caching_dilate)
        cloud_reflectance_dilated_masked_image = cloud_reflectance_dilated_masked_app.getoutput()["out"]
        self._apps.add_otb_app(cloud_reflectance_dilated_masked_app)
        if compute_reflvar:
            cloud_reflvar_dilated_masked_app = MajaCloudMaskComputation._dilate_and_mask(
                cloud_reflectvar_image, cloud_reflvar_dilated_filename, cloud_reflvar_dilated_masked_filename, mask, radius, caching_dilate)
            cloud_reflvar_dilated_masked_image = cloud_reflvar_dilated_masked_app.getoutput()["out"]
            self._apps.add_otb_app(cloud_reflvar_dilated_masked_app)
        if compute_extension:
            cloud_extension_dilated_masked_app = MajaCloudMaskComputation._dilate_and_mask(
                cloud_extension_image,
                cloud_extension_dilated_filename,
                cloud_extension_dilated_masked_filename,
                mask,
                radius,
                caching_dilate)
            cloud_extension_dilated_masked_image = cloud_extension_dilated_masked_app.getoutput()["out"]
            self._apps.add_otb_app(cloud_extension_dilated_masked_app)
        cloud_all_dilated_masked_app = MajaCloudMaskComputation._dilate_and_mask(
            cloud_all_image, cloud_all_dilated_filename, cloud_all_dilated_masked_filename, mask, radius, caching_dilate)
        cloud_all_dilated_masked_image = cloud_all_dilated_masked_app.getoutput()["out"]
        self._apps.add_otb_app(cloud_all_dilated_masked_app)
        cloud_shadow_dilated_masked_app = MajaCloudMaskComputation._dilate_and_mask(
            cloud_shadow_image, cloud_shadow_dilated_filename, cloud_shadow_dilated_masked_filename, mask, radius, caching_dilate)
        cloud_shadow_dilated_masked_image = cloud_shadow_dilated_masked_app.getoutput()["out"]
        self._apps.add_otb_app(cloud_shadow_dilated_masked_app)
        if compute_shadvar:
            cloud_shadvar_dilated_masked_app = MajaCloudMaskComputation._dilate_and_mask(
                cloud_shadvar_image, cloud_shadvar_dilated_filename, cloud_shadvar_dilated_masked_filename, mask, radius, caching_dilate)
            cloud_shadvar_dilated_masked_image = cloud_shadvar_dilated_masked_app.getoutput()["out"]
            self._apps.add_otb_app(cloud_shadvar_dilated_masked_app)
        cloud_sum_dilated_masked_app = MajaCloudMaskComputation._dilate_and_mask(
            cloud_sum_image, cloud_sum_dilated_filename, cloud_sum_dilated_masked_filename, mask, radius, caching_dilate)
        cloud_sum_dilated_masked_image = cloud_sum_dilated_masked_app.getoutput()["out"]
        self._apps.add_otb_app(cloud_sum_dilated_masked_app)
        cloud_alt_dilated_masked_app = MajaCloudMaskComputation._dilate_and_mask(dict_of_output.get(
            "CLALT_Sub"), cloud_alt_dilated_filename, cloud_alt_dilated_masked_filename, mask, radius, caching_dilate)
        cloud_alt_dilated_masked_image = cloud_alt_dilated_masked_app.getoutput()["out"]
        self._apps.add_otb_app(cloud_alt_dilated_masked_app)
        if dict_of_input.get("Params").get("CirrusBandAvailable"):
            cloud_cirrus_dilated_masked_app = MajaCloudMaskComputation._dilate_and_mask(dict_of_output.get("CirrusMask"),
                                                                                         cloud_cirrus_dilated_filename,
                                                                                         cloud_cirrus_dilated_masked_filename,
                                                                                         mask, radius, caching_dilate)
            cloud_cirrus_dilated_masked_image = cloud_cirrus_dilated_masked_app.getoutput()["out"]
            self._apps.add_otb_app(cloud_cirrus_dilated_masked_app)
        else:
            cloud_cirrus_dilated_masked_image = constant_mask

        # ------------------- Cld dilate masks ---------------------
        # -------------------------------------------------------

        # -------------------------------------------------------
        # ------------------- Register outputs ------------------
        # Concatenation to generate a vector image of the cloud masks
        # Concatenation of all Mono band images to the vector image
        # *************************************************************************************************
        # *************************************************************************************************
        # ATTENTION : The order of the PushBack define the order of the data in the L2 Product
        #             For ChecktoolProcessorQuickLookL2::Launch method, we use index of these bands
        # *************************************************************************************************
        # *************************************************************************************************
        # AVANT MEMORY IN CHAIN CORE ALGORITHMS - DEPRECATED
        #                    Bit 1 - Cloud_Mask.all : summary Logical or of All cloud, extension and shadow masks
        #                    Bit 2 - Cloud_Mask.all clouds: Logical or of All cloud masks and extension
        #                    Bit 3 - Cloud_Mask.refl : reflectance threshold
        #                    Bit 4 - Cloud_Mask.refl_var : reflectance variation threshold
        #                    Bit 5 - Cloud_Mask.extension : extension of the cloud mask
        #                    Bit 6 - Cloud_Mask.alt : stereoscopic mask  =>  VENUS et vide pour les autres
        #                    Bit 7 - Cloud_Mask.shadows : shadows mask from clouds within image
        #                    Bit 8 - Cloud_Mask.shadvar: shadows mask from clouds outside image bit 1 : Cloud_Mask.all
        #                    Bit 9 - Cirrus => L8 et S2

        # APRES MEMORY IN CHAIN CORE ALGORITHMS : V 4-1-0
        #                    Bit 1 - Cloud_Mask.all : summary Logical or of All cloud, extension and shadow masks
        #                    Bit 2 - Cloud_Mask.all clouds: Logical or of All cloud masks and extension
        #                    Bit 3 - Cloud_Mask.shadows : shadows mask from clouds within image
        #                    Bit 4 - Cloud_Mask.shadvar: shadows mask from clouds outside image bit 1 : Cloud_Mask.all
        #                    Bit 5 - Cloud_Mask.refl : reflectance threshold
        #                    Bit 6 - Cloud_Mask.refl_var : reflectance variation threshold
        #                    Bit 7 - Cloud_Mask.extension : extension of the cloud mask
        #                    Bit 8 - Cloud_Mask.alt : stereoscopic mask  =>  VENUS et vide pour les autres
        #                    Bit 9 - Cirrus : Pour  L8 et S2 et vide pour les autres

        # FORMAT DISK
        #                    Bit 1 - Cloud_Mask.all : summary Logical or of All cloud and shadow masks
        #                    Bit 2 - Cloud_Mask.all clouds: Logical or of All cloud masks
        #                    Bit 3 - Cloud_Mask.shadows : shadows mask from clouds within image
        #                    Bit 4 - Cloud_Mask.shadvar: shadows mask from clouds outside image bit 1 : Cloud_Mask.all
        #                    Bit 5 - Cloud_Mask.refl : reflectance threshold
        #                    Bit 6 - Cloud_Mask.refl_var : reflectance variation threshold
        #                    Bit 7 - Cloud_Mask.extension : extension of the cloud mask
        #                    Bit 8 - VENUS : Cloud_Mask.alt : stereoscopic mask
        #                    Bit 8 - L8 et S2 : Bit 9 - Cirrus
        #                    Bit 8 - Vide pour les autres
        cld_list = []

        dict_of_output[constants.CLOUD_MASK_ALL] = cloud_sum_dilated_masked_image
        cld_list.append(dict_of_output[constants.CLOUD_MASK_ALL])
        dict_of_output[constants.CLOUD_MASK_ALL_CLOUDS] = cloud_all_dilated_masked_image
        cld_list.append(dict_of_output[constants.CLOUD_MASK_ALL_CLOUDS])
        dict_of_output[constants.CLOUD_MASK_SHADOWS] = cloud_shadow_dilated_masked_image
        cld_list.append(dict_of_output[constants.CLOUD_MASK_SHADOWS])
        if compute_shadvar:
            dict_of_output[constants.CLOUD_MASK_SHADVAR] = cloud_shadvar_dilated_masked_image
        else:
            dict_of_output[constants.CLOUD_MASK_SHADVAR] = cloud_shadvar_image
        cld_list.append(dict_of_output[constants.CLOUD_MASK_SHADVAR])
        dict_of_output[constants.CLOUD_MASK_REFL] = cloud_reflectance_dilated_masked_image
        cld_list.append(dict_of_output[constants.CLOUD_MASK_REFL])
        if compute_reflvar:
            dict_of_output[constants.CLOUD_MASK_REFL_VAR] = cloud_reflvar_dilated_masked_image
        else:
            dict_of_output[constants.CLOUD_MASK_REFL_VAR] = cloud_reflectvar_image
        cld_list.append(dict_of_output[constants.CLOUD_MASK_REFL_VAR])
        if compute_extension:
            dict_of_output[constants.CLOUD_MASK_EXTENSION] = cloud_extension_dilated_masked_image
        else:
            dict_of_output[constants.CLOUD_MASK_EXTENSION] = cloud_extension_image
        cld_list.append(dict_of_output[constants.CLOUD_MASK_EXTENSION])
        dict_of_output[constants.CLOUD_MASK_ALT] = cloud_alt_dilated_masked_image
        cld_list.append(dict_of_output[constants.CLOUD_MASK_ALT])
        dict_of_output[constants.CLOUD_MASK_CIRRUS] = cloud_cirrus_dilated_masked_image
        cld_list.append(dict_of_output[constants.CLOUD_MASK_CIRRUS])

        dict_of_output["CLDList"] = cld_list


        LOGGER.debug(cld_list)

