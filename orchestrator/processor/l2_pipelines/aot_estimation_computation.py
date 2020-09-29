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
from orchestrator.cots.otb.algorithms.otb_constant_image import constant_image
from orchestrator.cots.otb.otb_pipeline_manager import OtbPipelineManager
from orchestrator.common.constants import AOTEstimation
from orchestrator.common.interfaces.maja_xml_aot_params import AOT_Estimation,Multi_Spectral_MethodType,Multi_Temporal_MethodType,Spectro_Temporal_MethodType
import orchestrator.common.xml_tools as xml_tools
import orchestrator.common.constants as constants
from orchestrator.common.maja_exceptions import *
from orchestrator.modules.maja_module import MajaModule
import os,io
LOGGER = configure_logger(__name__)


class ModuleAOTEstimationComputation(MajaModule):

    NAME = "AOTEstimation"

    def __init__(self):
        super(ModuleAOTEstimationComputation, self).__init__()
        self._aot_pipeline = OtbPipelineManager()

        self.in_keys_to_check = ["Params.Caching", "Params.WriteL2ProductToL2Resolution",
                                 "Params.DarkBandIndex_DateD", "Params.DarkBandIndex_DateD",
                                 "Params.AOTMethod", "Params.InitMode",
                                 "Params.MTAOTbandsDateD",
                                 "AppHandler", "Plugin", "L1Reader", "L2COMM", "DEM",
                                 "L2TOCR", "L2SITE" ]

        self.out_keys_to_check = ["AtmoAbsIPTOAC","dtm_shd","cr_lut"]
        self.out_keys_provided = ["AOT_Sub"]

    def cleanup(self):
        self._aot_pipeline.free_otb_app()

    def run(self, dict_of_input, dict_of_output):
        LOGGER.info("AOT estimation computation start")
        LOGGER.debug("Caching %s", dict_of_input.get("Params").get("Caching"))
        l_caching = dict_of_input.get("Params").get("Caching")
        l_writeL2 = dict_of_input.get("Params").get("WriteL2ProductToL2Resolution")

        aot_working = dict_of_input.get("AppHandler").get_directory_manager().get_temporary_directory("AOTEstimationProc_", do_always_remove=True)

        init_Mode = dict_of_input.get("Params").get("InitMode")
        bands_definition = dict_of_input.get("Plugin").BandsDefinitions
        # TODO concatenate cloud masks
        param_darkest = {"toac": dict_of_output["AtmoAbsIPTOAC"],
                         "cld": dict_of_output[constants.CLOUD_MASK_ALL],
                         "dtm": dict_of_input.get("DEM").ALC,
                         "edg": dict_of_input.get("L1Reader").get_value("IPEDGSubOutput"),
                         "shd": dict_of_output["dtm_shd"],
                         "lutheader": dict_of_input.get("L2TOCR"),
                         "lutimage": dict_of_output["cr_lut"],
                         "darkestchanneltoac": dict_of_input.get("Params").get("DarkBandIndex_DateD"),
                         "thresholddarkpixel": float(dict_of_input.get("L2SITE").get_value("DarkPixelThreshold")),
                         "darksurfacereflectance": float(
                             dict_of_input.get("L2SITE").get_value("DarkestPixelSurfaceReflectance")),
                         "darkestchannelrtc": dict_of_input.get("Params").get("DarkBandIndex_DateD")
                         }
        if not init_Mode:
            param_darkest["l2ndt"] = dict_of_input.get("L2Reader").get_value("NDTImage")
            param_darkest["l2rtc"] = dict_of_input.get("L2Reader").get_value("RTCImage")
        else:
            param_darkest["init"] = init_Mode

        darkest_app = OtbAppHandler("DarkestPixelAOT", param_darkest, write_output=True)
        aot_computed = darkest_app.getoutput()["aotcomputed"]
        darkest_aot = darkest_app.getoutput()["darkestaot"]
        l_aotmethod = dict_of_input.get("Params").get("AOTMethod")
        LOGGER.debug("AOT Estimation Computation Method: "+ str(l_aotmethod))
        #AOT Params xml holder
        xml_aot_param = AOT_Estimation()
        #Common parameters
        xml_aot_param.set_Sampling_Interval_Min(dict_of_input.get("L2COMM").get_value_i("SamplingIntervalMin"))
        xml_aot_param.set_Sampling_Interval_Max(dict_of_input.get("L2COMM").get_value_i("SamplingIntervalMax"))
        xml_aot_param.set_Increment_N_Distance(dict_of_input.get("L2COMM").get_value_i("IncrementNDistance"))
        xml_aot_param.set_Half_Window_Size(dict_of_input.get("L2COMM").get_value_i("HalfWindowSize"))
        xml_aot_param.set_Max_Iterations(dict_of_input.get("L2COMM").get_value_i("MaxIterations"))
        xml_aot_param.set_Step_AOT(dict_of_input.get("L2COMM").get_value_i("StepAOT"))
        xml_aot_param.set_N_Pix_Min(dict_of_input.get("L2COMM").get_value_i("NPixMin"))
        xml_aot_param.set_F_Tolerance(dict_of_input.get("L2COMM").get_value_f("FTolerance"))
        xml_aot_param.set_G_Tolerance(dict_of_input.get("L2COMM").get_value_f("GTolerance"))
        xml_aot_param.set_X_Tolerance(dict_of_input.get("L2COMM").get_value_f("XTolerance"))
        xml_aot_param.set_Epsilon_Function(dict_of_input.get("L2COMM").get_value_f("EpsilonFunction"))
        xml_aot_param.set_AOT_initial_D(dict_of_input.get("L2COMM").get_value_f("AOTinitialD"))
        xml_aot_param.set_W_dark(dict_of_input.get("L2COMM").get_value_f("Wdark"))
        xml_aot_param.set_W_bounds(dict_of_input.get("L2COMM").get_value_f("Wbounds"))
        xml_aot_param.set_AOT_min(dict_of_input.get("L2COMM").get_value_f("AOTmin"))
        xml_aot_param.set_AOT_Max_Age(dict_of_input.get("L2COMM").get_value_f("AOTMaxAge"))
        xml_aot_param.set_SE_Mask_Dilatation_Radius(dict_of_input.get("L2COMM").get_value_f("SEMaskDilatationRadius"))
        xml_aot_param.set_SE_Valid_AOT_Radius(dict_of_input.get("L2COMM").get_value_f("SEValidAOTRadius"))


        aot_image = os.path.join(aot_working, "aot_computed.tif")
        aot_mask_estimation = os.path.join(aot_working, "aot_mask_computed.tif")
        param_aot = {"toac": dict_of_output["AtmoAbsIPTOAC"],
                     "sat": dict_of_input.get("L1Reader").get_value("IPSATSubOutput"),
                     "was": dict_of_output["WaterMask"],
                     "dtm": dict_of_input.get("DEM").ALC,
                     "edg": dict_of_input.get("L1Reader").get_value("IPEDGSubOutput"),
                     "shd": dict_of_output["dtm_shd"],
                     "lutheader": dict_of_input.get("L2TOCR"),
                     "lutimage": dict_of_output["cr_lut"],
                     "cld": dict_of_output[constants.CLOUD_MASK_ALL],
                     "reall2nodata": dict_of_input.get("Params").get("RealL2NoData"),
                     "jday": dict_of_input.get("Params").get("JDay"),
                     "darkestaot": darkest_aot,
                     "aotcomputed": aot_image,
                     "aotmask": aot_mask_estimation
                     }
        if init_Mode:
            param_aot["init"] = init_Mode

        if l_aotmethod == AOTEstimation.MULTISPECTRAL:
            param_aot["mode"] = "multispectral"
        elif l_aotmethod == AOTEstimation.MULTITEMPORAL:
            param_aot["mode"] = "multitemporal"
        elif l_aotmethod == AOTEstimation.SPECTROTEMPORAL:
            param_aot["mode"] = "spectrotemporal"

        if l_aotmethod == AOTEstimation.SPECTROTEMPORAL or l_aotmethod == AOTEstimation.MULTITEMPORAL:
            stdevbandid = bands_definition.get_band_id_in_l2_coarse(
                dict_of_input.get("L2COMM").get_value("StdevBandCode"))
            if not init_Mode:
                param_aot["temporal.lutmap"] = dict_of_input.get("L2Reader").get_value("LTCImage")
            param_aot["temporal.altitudemean"] = dict_of_input.get("DEM").ALT_Mean
            xml_aot_param.set_Multi_Temporal_Method(Multi_Temporal_MethodType())
            xml_aot_param.get_Multi_Temporal_Method().set_First_AOT(dict_of_input.get("L2COMM").get_value_f("FirstAOT"))
            xml_aot_param.get_Multi_Temporal_Method().set_Second_AOT(dict_of_input.get("L2COMM").get_value_f("SecondAOT"))
            xml_aot_param.get_Multi_Temporal_Method().set_TOA_Reflectance_Min(dict_of_input.get("L2COMM").get_value_f("TOAReflectanceMin"))
            xml_aot_param.get_Multi_Temporal_Method().set_TOA_Reflectance_Max(dict_of_input.get("L2COMM").get_value_f("TOAReflectanceMax"))
            xml_aot_param.get_Multi_Temporal_Method().set_TOA_Reflectance_Step(dict_of_input.get("L2COMM").get_value_f("TOAReflectanceStep"))
            xml_aot_param.get_Multi_Temporal_Method().set_Min_Difference_Thresholds_Calculation(dict_of_input.get("L2COMM").get_value_f("MinDifferenceThreshold"))
            xml_aot_param.get_Multi_Temporal_Method().set_Relative_Var_Max(dict_of_input.get("L2COMM").get_value_f("RelativeVarMax"))
            xml_aot_param.get_Multi_Temporal_Method().set_Var_Band_Code_D(dict_of_input.get("Params").get("VarBandIndex_DateD"))
            xml_aot_param.get_Multi_Temporal_Method().set_Var_Band_Code_Dm1(dict_of_input.get("Params").get("VarBandIndex_DateDm1"))
            xml_aot_param.get_Multi_Temporal_Method().set_Stdev_Band_Code(stdevbandid)
            xml_aot_param.get_Multi_Temporal_Method().set_Std_Noise(dict_of_input.get("L2COMM").get_value_f("StdNoise"))
            xml_aot_param.get_Multi_Temporal_Method().set_Stdev_Min(dict_of_input.get("L2COMM").get_value_f("StdevMin"))
            xml_aot_param.get_Multi_Temporal_Method().set_AOT_initial_Dm1(dict_of_input.get("L2COMM").get_value_f("AOTinitialDm1"))
            xml_aot_param.get_Multi_Temporal_Method().set_W_MT_equation2(dict_of_input.get("L2COMM").get_value_f("WMTequation2"))
            xml_aot_param.get_Multi_Temporal_Method().set_MT_AOT_Band_Code_D([str(i) for i in dict_of_input.get("Params").get("MTAOTbandsDateD")])
            xml_aot_param.get_Multi_Temporal_Method().set_MT_AOT_Band_Reflectance_Composite_Dm1([str(i) for i in dict_of_input.get("Params").get(
                "MTAOTbandsOfTheReflectancesComposite_DateDm1")])
            xml_aot_param.get_Multi_Temporal_Method().set_MT_AOT_Band_LUT_Composite_Dm1([str(i) for i in dict_of_input.get("Params").get(
                "MTAOTbandsOfTheLTCComposite_DateDm1")])

        if l_aotmethod == AOTEstimation.MULTISPECTRAL or l_aotmethod == AOTEstimation.SPECTROTEMPORAL:
            lMSAOTRedBand = bands_definition.get_band_id_in_l2_coarse(
                dict_of_input.get("L2COMM").get_value("MSAOTRedBandCode"))
            lNDVIAOTBand1 = bands_definition.get_band_id_in_l2_coarse(
                dict_of_input.get("L2COMM").get_value("NDVIAOTBand1Code"))
            lNDVIAOTBand2 = bands_definition.get_band_id_in_l2_coarse(
                dict_of_input.get("L2COMM").get_value("NDVIAOTBand2Code"))
            l_AOTUsedBandCodes = xml_tools.as_string_list(dict_of_input.get("L2COMM").get_value("MSAOTBandCode"))
            lMSAOTbands = [str(bands_definition.get_band_id_in_l2_coarse(f)) for f in l_AOTUsedBandCodes]

            xml_aot_param.set_Multi_Spectral_Method(Multi_Spectral_MethodType())
            xml_aot_param.get_Multi_Spectral_Method().set_MS_AOT_Band_Code(lMSAOTbands)
            xml_aot_param.get_Multi_Spectral_Method().set_MS_AOT_Red_Band_Code(lMSAOTRedBand)
            xml_aot_param.get_Multi_Spectral_Method().set_NDVI_AOT_Band1_Code(lNDVIAOTBand1)
            xml_aot_param.get_Multi_Spectral_Method().set_NDVI_AOT_Band2_Code(lNDVIAOTBand2)
            xml_aot_param.get_Multi_Spectral_Method().set_Red_TOA_Threshold(dict_of_input.get("L2COMM").get_value_f("RedTOAThreshold"))
            xml_aot_param.get_Multi_Spectral_Method().set_NDVI_Threshold(dict_of_input.get("L2COMM").get_value_f("NDVIThreshold"))
            xml_aot_param.get_Multi_Spectral_Method().set_MS_Slope(dict_of_input.get("L2COMM").get_value_f("MSSlope"))
            xml_aot_param.get_Multi_Spectral_Method().set_MS_YIntercept(dict_of_input.get("L2COMM").get_value_f("MSYIntercept"))
            xml_aot_param.get_Multi_Spectral_Method().set_NDVI_Slope(dict_of_input.get("L2COMM").get_value_f("NDVISlope"))
            xml_aot_param.get_Multi_Spectral_Method().set_NDVI_YIntercept(dict_of_input.get("L2COMM").get_value_f("NDVIYIntercept"))
            xml_aot_param.get_Multi_Spectral_Method().set_W_MS(dict_of_input.get("L2COMM").get_value_f("WMS"))

        if l_aotmethod == AOTEstimation.SPECTROTEMPORAL:
            spectrotemp = Spectro_Temporal_MethodType()
            xml_aot_param.set_Spectro_Temporal_Method(spectrotemp)
            xml_aot_param.get_Spectro_Temporal_Method().set_MT_Weight_Threshold(dict_of_input.get("L2COMM").get_value_f("MTWeightThreshold"))
            xml_aot_param.get_Spectro_Temporal_Method().set_A_Coefficient(dict_of_input.get("L2COMM").get_value_f("ACoefficient"))
            xml_aot_param.get_Spectro_Temporal_Method().set_B_Coefficient(dict_of_input.get("L2COMM").get_value_f("BCoefficient"))

        const_mask_filename = os.path.join(aot_working, "cloud_constzero.tif")
        constant_app = constant_image(
            dict_of_input.get("DEM").ALC,
            0,
            const_mask_filename +
            ":uint8",
            write_output=l_caching)
        constant_mask = constant_app.getoutput()["out"]

        if dict_of_input.get("Params").get("SnowBandAvailable"):
            param_aot["snow"] = dict_of_output.get("cld_snow")
        else:
            param_aot["snow"] = constant_mask

        if not init_Mode:
            param_aot["l2ndt"] = dict_of_input.get("L2Reader").get_value("NDTImage")
            param_aot["l2pxd"] = dict_of_input.get("L2Reader").get_value("PXDImage")
            param_aot["l2rtc"] = dict_of_input.get("L2Reader").get_value("RTCImage")
            param_aot["l2rta"] = dict_of_input.get("L2Reader").get_value("RTAImage")

        # Cams related parameters
        if dict_of_input.get("Params").get("UseCamsData"):
            xml_aot_param.set_AOT_KPondCAMS(dict_of_input.get("L2COMM").get_value_f("KPondsCams"))
            xml_aot_param.set_AOT_HeightScale(dict_of_input.get("L2COMM").get_value_f("HeighScale"))
            param_aot["cams.nodatatau"] = dict_of_input.get("Params").get("RealL2NoData")
            param_aot["cams.altmean"] = dict_of_input.get("DEM").ALT_Mean
            param_aot["cams.aot"] = dict_of_input.get("CAMS_AOT")
            aot_cams_image = os.path.join(aot_working, "aot_cams.tif")
            param_aot["aotcams"] = aot_cams_image

        #Write down the xml interface
        output = io.StringIO()
        output.write('<?xml version="1.0" ?>\n')
        xml_aot_param.export(output, 0, name_='AOTEstimation', namespacedef_='', pretty_print=True)
        l_xml_param_filename = os.path.join(aot_working, "AOTParams.xml")
        with open(l_xml_param_filename, "w") as fh:
            fh.write(output.getvalue().replace("    ", "  "))
        LOGGER.info("Writed aot params to " + l_xml_param_filename)
        param_aot["xmlparamsfile"] = l_xml_param_filename
        # Effectively launch the app
        aot_app = OtbAppHandler("AOTEstimation", param_aot, write_output=l_caching)
        if not l_caching:
            self._aot_pipeline.add_otb_app(aot_app)

        aot_mean = aot_app.getoutput()["aotmean"]
        aot_valid = aot_app.getoutput()["aotvalid"]

        # If cams used then no gapfilling needed
        if dict_of_input.get("Params").get("UseCamsData"):
            smoothing_input = aot_app.getoutput()["aotcomputed"]
            mask_input = aot_app.getoutput()["aotmask"]
        else:
            aot_gapfilled_image = os.path.join(aot_working, "aot_gapfilled.tif")
            aot_gapfilled_mask = os.path.join(aot_working, "aot_gapfilled_mask.tif")
            param_gapfilling = {"im": aot_app.getoutput()["aotcomputed"],
                                "initwindowsradius": dict_of_input.get("L2COMM").get_value_i("InitWindowRadius"),
                                "maxwindowsradius": dict_of_input.get("L2COMM").get_value_i("MaxWindowRadius"),
                                "reall2nodata": dict_of_input.get("Params").get("RealL2NoData"),
                                "mean": aot_mean,
                                "hasvalidpixels": aot_valid,
                                "defaultvalue": dict_of_input.get("L2COMM").get_value_f("DefaultAOT"),
                                "out": aot_gapfilled_image,
                                "outmask": aot_gapfilled_mask + ":uint8"
                                }

            gap_filling_app = OtbAppHandler("GapFilling", param_gapfilling,
                                            write_output=(l_caching or l_writeL2))
            if not (l_caching or l_writeL2):
                self._aot_pipeline.add_otb_app(gap_filling_app)
            smoothing_input = gap_filling_app.getoutput()["out"]
            mask_input = gap_filling_app.getoutput()["outmask"]

        aot_smoothed_image = os.path.join(aot_working, "aot_smoothed.tif")
        param_smoothing = {"in": smoothing_input,
                           "out": aot_smoothed_image,
                           "type": "gaussian",
                           "type.gaussian.radius": dict_of_input.get("L2COMM").get_value_f("SigmaSmoothingAOT"),
                           "type.gaussian.maxkernelwidth": 4.0 * dict_of_input.get("L2COMM").get_value_f(
                               "SigmaSmoothingAOT"),
                           "type.gaussian.maxerror": 1e-15
                           }

        smoothed_app = OtbAppHandler("SmoothingBis", param_smoothing,
                                     write_output=l_caching)
        if (not l_caching):
            self._aot_pipeline.add_otb_app(smoothed_app)

        aot_sub_image = os.path.join(aot_working, "aot_sub.tif")
        param_subresampling = {"dtm": dict_of_input.get("DEM").ALC,
                               "im": smoothed_app.getoutput()["out"],
                               "interp": "linear",
                               "out": aot_sub_image
                               }

        subresampling_app = OtbAppHandler("Resampling", param_subresampling, write_output=(l_caching or l_writeL2))
        if (not (l_caching or l_writeL2)):
            self._aot_pipeline.add_otb_app(subresampling_app)
        dict_of_output["AOT_Sub"] = subresampling_app.getoutput()["out"]

        if l_writeL2:
            l_nbRes = len(bands_definition.ListOfL2Resolution)
            aot_list = []
            aotmask_list = []
            for r in range(0, l_nbRes):
                l_res = bands_definition.ListOfL2Resolution[r]
                aot_image = os.path.join(aot_working, "aot_" + l_res + ".tif")
                aot_mask = os.path.join(aot_working, "aotmask_" + l_res + ".tif")
                param_subresampling["dtm"] = dict_of_input.get("DEM").ALTList[r]
                param_subresampling["im"] = subresampling_app.getoutput()["out"]
                param_subresampling["out"] = aot_image
                resamp_aot_app = OtbAppHandler("Resampling", param_subresampling)

                dict_of_output["AOT_" + l_res] = resamp_aot_app.getoutput().get("out")
                param_maskresampling = {"dtm": dict_of_input.get("DEM").ALTList[r],
                                        "im": mask_input,
                                        "interp": "linear",
                                        "out": aot_mask + ":uint8",
                                        "threshold": 0.25
                                        }
                resamp_aotmask_app = OtbAppHandler("Resampling", param_maskresampling)
                dict_of_output["AOTMASK_" + l_res] = resamp_aotmask_app.getoutput().get("out")
                aot_list.append(resamp_aot_app.getoutput().get("out"))
                aotmask_list.append(resamp_aotmask_app.getoutput().get("out"))
            dict_of_output["L2AOTList"] = aot_list
            dict_of_output["L2AOTMaskList"] = aotmask_list


