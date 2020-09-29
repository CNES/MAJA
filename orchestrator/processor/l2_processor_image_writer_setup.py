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

from orchestrator.cots.otb.algorithms.otb_constant_image import constant_image
from orchestrator.cots.otb.otb_app_handler import OtbAppHandler
from orchestrator.common.gipp_utils import *
import os
LOGGER = configure_logger(__name__)


def setup_l2_image_writer(self, p_OutputL2ImageFileWriter, in_global_dict, out_global_dict, l_WriteL2Products,
                          l_CopyPrivateFromL2InputToL2Output, l_IsDefaultAOT):
    l2_write_working = in_global_dict.get("AppHandler").get_directory_manager().get_temporary_directory("L2Write_",
                                            do_always_remove=True)
    l_WriteL2ProductToL2Resolution = in_global_dict["Params"]["WriteL2ProductToL2Resolution"]
    l_EnvCorOption = in_global_dict["Params"]["EnvCorOption"]

    p_OutputL2ImageFileWriter.set_dtm(in_global_dict["DEM"])
    p_OutputL2ImageFileWriter.set_current_plugin_base(p_OutputL2ImageFileWriter.plugin)
    p_OutputL2ImageFileWriter.set_copy_private_from_l2_input_to_l2_output(l_CopyPrivateFromL2InputToL2Output)
    p_OutputL2ImageFileWriter.set_write_l2_products(l_WriteL2Products)
    p_OutputL2ImageFileWriter.set_output_directory(in_global_dict["Params"]["L2OutputDirectory"])
    p_OutputL2ImageFileWriter.set_l2_image_file_reader(in_global_dict["L2Reader"])
    p_OutputL2ImageFileWriter.set_l1_image_informations_provider(in_global_dict["L1Info"])
    p_OutputL2ImageFileWriter.set_real_l2_nodata(in_global_dict["Params"]["RealL2NoData"])
    l_GIPPL2COMMHandler = in_global_dict["L2COMM"]
    l_bandsdefinition = in_global_dict.get("Plugin").BandsDefinitions
    # Set the L1 Data Filter informations
    # ReflectanceQuantification is 0.001(not 1000)
    # VAP_Quantification_Value is 0.050(not 20)
    # AOT_Quantification_Value is 0.005(not 200)
    p_OutputL2ImageFileWriter.set_reflectance_quantification_value(in_global_dict["L1Info"].ReflectanceQuantification)
    p_OutputL2ImageFileWriter.set_aot_quantification_value(l_GIPPL2COMMHandler.get_value_f("AOTQuantificationValue"))
    p_OutputL2ImageFileWriter.set_vap_quantification_value(l_GIPPL2COMMHandler.get_value_f("VAPQuantificationValue"))
    p_OutputL2ImageFileWriter.set_vap_nodata_value(l_GIPPL2COMMHandler.get_value_f("VAPNodataValue"))
    p_OutputL2ImageFileWriter.set_aot_nodata_value(l_GIPPL2COMMHandler.get_value_f("AOTNodataValue"))
    p_OutputL2ImageFileWriter.set_quicklook_min_refl_redband(l_GIPPL2COMMHandler.get_value("QuicklookMinReflRedBand"))
    p_OutputL2ImageFileWriter.set_quicklook_min_refl_greenband(
        l_GIPPL2COMMHandler.get_value("QuicklookMinReflGreenBand"))
    p_OutputL2ImageFileWriter.set_quicklook_min_refl_blueband(l_GIPPL2COMMHandler.get_value("QuicklookMinReflBlueBand"))
    p_OutputL2ImageFileWriter.set_quicklook_max_refl_redband(l_GIPPL2COMMHandler.get_value("QuicklookMaxReflRedBand"))
    p_OutputL2ImageFileWriter.set_quicklook_max_refl_greenband(
        l_GIPPL2COMMHandler.get_value("QuicklookMaxReflGreenBand"))
    p_OutputL2ImageFileWriter.set_quicklook_max_refl_blueband(l_GIPPL2COMMHandler.get_value("QuicklookMaxReflBlueBand"))
    p_OutputL2ImageFileWriter.set_quicklook_red_band_code(l_GIPPL2COMMHandler.get_value("QuicklookRedBandCode"))
    p_OutputL2ImageFileWriter.set_quicklook_green_band_code(l_GIPPL2COMMHandler.get_value("QuicklookGreenBandCode"))
    p_OutputL2ImageFileWriter.set_quicklook_blue_band_code(l_GIPPL2COMMHandler.get_value("QuicklookBlueBandCode"))
    p_OutputL2ImageFileWriter.set_projection_ref(in_global_dict["L1Info"].ProjectionRef)
    p_OutputL2ImageFileWriter.set_write_public_product(l_WriteL2ProductToL2Resolution)
    p_OutputL2ImageFileWriter.set_env_cor_option(l_EnvCorOption)
    # == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == ==
    # IF NOT BACKWARD MODE OR IF IT IS THE LAST PRODUCT IN BACKWARD MODE
    #  = > WRITE THE PUBLIC PART OF L2 PRODUCT
    # == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == ==
    LOGGER.debug("WriteL2ProductToL2Resolution enable: " + str(l_WriteL2ProductToL2Resolution))
    if l_WriteL2ProductToL2Resolution:
        LOGGER.debug("EnvCorOption enable: " + str(l_EnvCorOption))
        # ---------------------------------------------------------------------------
        # Public
        # ---------------------------------------------------------------------------
        # Images of surface reflectance
        # If environment option is True
        if l_EnvCorOption:
            # Reflectances are corrected from environment effects
            p_OutputL2ImageFileWriter.set_sre_image_list(out_global_dict["SRE_List"])
            p_OutputL2ImageFileWriter.set_fre_image_list(out_global_dict["FRE_List"])
            # STL and TGS masks are added in the MSK product
            p_OutputL2ImageFileWriter.set_stl_image_list(out_global_dict["STL_List"])
            p_OutputL2ImageFileWriter.set_tgs_image_list(out_global_dict["TGS_List"])
        else:
            # Reflectances after atmospheric correction
            p_OutputL2ImageFileWriter.set_sre_image_list(out_global_dict["L2TOCList"])
            # Note that no FREImage is set
            l_nbRes = len(l_bandsdefinition.ListOfL2Resolution)
            l2_list = []
            for r in range(0, l_nbRes):
                l_res = l_bandsdefinition.ListOfL2Resolution[r]
                const_mask = os.path.join(l2_write_working, "constant_mask_" + l_res + ".tif:uint8")
                out_const_app = constant_image(in_global_dict.get("DEM").ALTList[r], 0, const_mask)
                l2_list.append(out_const_app.getoutput().get("out"))
            p_OutputL2ImageFileWriter.set_stl_image_list(l2_list)
            p_OutputL2ImageFileWriter.set_tgs_image_list(l2_list)

        # Set AOT
        p_OutputL2ImageFileWriter.set_aot_image_list(out_global_dict["L2AOTList"])
        p_OutputL2ImageFileWriter.set_tao_image_list(out_global_dict["L2AOTMaskList"])

        # ATB Atmospheric and biophysical parameters
        p_OutputL2ImageFileWriter.set_vap_image_list(out_global_dict["L2VAPList"])

        # Write the IWC mask, the VAP Interpolated value
        LOGGER.debug("Set the IWCImageList to the L2ImageFileWriter")
        p_OutputL2ImageFileWriter.set_iwc_image_list(out_global_dict["L2VAPMaskList"])

        # MSK Geophysical masks
        p_OutputL2ImageFileWriter.set_hid_image(out_global_dict["dtm_hid"])
        p_OutputL2ImageFileWriter.set_shd_image(out_global_dict["dtm_shd"])
        if in_global_dict["Params"]["SnowBandAvailable"]:
            p_OutputL2ImageFileWriter.set_snw_image(out_global_dict["cld_snow"])

        # QLT Quality masks
        p_OutputL2ImageFileWriter.set_sat_image_list(in_global_dict["L1Reader"].get_value("L2SATImageList"))
        p_OutputL2ImageFileWriter.set_pix_image_list(in_global_dict["L1Reader"].get_value("L2PIXImageList"))
        p_OutputL2ImageFileWriter.set_edg_image_list(in_global_dict["L1Reader"].get_value("L2EDGOutputList"))
        if in_global_dict["Params"]["DFPMaskAvailable"]:
            p_OutputL2ImageFileWriter.set_dfp_image_list(in_global_dict["L1Reader"].get_value("L2DFPImageList"))

    # Note: if p_finalize_backward == True, then the mode is NOMINAL ( not INIT) ?
    # = > NO, because the first step of Backward is to find the first valid product to start the loop of backward.
    # The first valid could be the last product.
    #  In this case, p_finalize_backward == True and the mode is INIT
    LOGGER.debug(in_global_dict["Params"]["CheckingConditionalClouds"])

    if in_global_dict["Params"]["FinalizeBackWard"] and in_global_dict["Params"]["BackWardMode"]\
            and in_global_dict["Params"]["CheckingConditionalClouds"] == False:
        LOGGER.debug(
            "Use case write only CLA, CLD and set WAS from the previous L2 input product. Processing mode INIT is " +
            str(in_global_dict["Params"]["InitMode"]) + ", BACKWARD is '" + str(in_global_dict["Params"]["BackWardMode"]))
        p_OutputL2ImageFileWriter.set_write_only_cla_cld(True)
        p_OutputL2ImageFileWriter.set_cld_image(out_global_dict["CLDList"])
        p_OutputL2ImageFileWriter.set_l2_cld_image_list(out_global_dict["L2CLDList"])
        p_OutputL2ImageFileWriter.set_cla_image(out_global_dict["cld_l2cla"])
        # FA LAIG - FA - MAC - 885 - CNES - add the test here, only if NOMINAL, read in the input L2 product
        if not in_global_dict["Params"]["InitMode"]:
            p_OutputL2ImageFileWriter.set_was_image(in_global_dict["L2Reader"].get_value("WASImage"))
    else:
        # si p_finalize_backward et(p_mode.IsBackward() vrai p_checking_conditional_clouds faux
        # WRITE THE PRIVATE PART OF L2 PRODUCT
        # Composite product
        # Images of reflectance
        p_OutputL2ImageFileWriter.set_rta_image(out_global_dict.get("Composite_rta"))
        p_OutputL2ImageFileWriter.set_rtc_image(out_global_dict.get("Composite_rtc"))
        p_OutputL2ImageFileWriter.set_rcr_image(out_global_dict.get("Composite_rcr"))
        p_OutputL2ImageFileWriter.set_sto_image(out_global_dict.get("Composite_sto"))
        # Masks

        p_OutputL2ImageFileWriter.set_pxd_image(out_global_dict.get("Composite_pxd"))
        p_OutputL2ImageFileWriter.set_ndt_image(out_global_dict.get("Composite_ndt"))
        p_OutputL2ImageFileWriter.set_cld_image(out_global_dict["CLDList"])
        p_OutputL2ImageFileWriter.set_l2_cld_image_list(out_global_dict["L2CLDList"])
        p_OutputL2ImageFileWriter.set_cla_image(out_global_dict["cld_l2cla"])
        #  Update output dictionary
        p_OutputL2ImageFileWriter.set_was_image(out_global_dict.get("WaterMask"))
        p_OutputL2ImageFileWriter.set_pwa_image(out_global_dict.get("PossibleWaterMask"))
        p_OutputL2ImageFileWriter.set_twa_image(out_global_dict.get("TestedWaterMask"))
        p_OutputL2ImageFileWriter.set_write_ltc(in_global_dict["Params"]["DealingLTC"])


        # LAIG - FA - MAC - 1180 - CNES
        # Remove the latest dates before adding the current one
        # only if the STO image file was updated
        if not l_CopyPrivateFromL2InputToL2Output:
            maxSTODates = l_GIPPL2COMMHandler.get_value_i("NumberOfStackImages")
            l_STOListOfStringDates = in_global_dict["Params"].get("STOListOfStringDates")
            if len(l_STOListOfStringDates) >= int(maxSTODates):
                l_STOListOfStringDates = l_STOListOfStringDates[maxSTODates - 1::]
            # the order of the list is more understandable with pop_back
            l_STOListOfStringDates.insert(0, in_global_dict["L1Info"].ProductDateStr)
            in_global_dict["Params"]["STOListOfStringDates"] = l_STOListOfStringDates

        # ---------------------------------------------------------------------------
        # Write LTC only if available
        if in_global_dict["Params"]["DealingLTC"]:
            tmp_ltc = os.path.join(l2_write_working, "tmp_ltc.mha")
            # ---------------------------------------------------------------------------
            # Reduced LUTs with select the composite band
            param_extractchannels = {
                "lutimage": out_global_dict.get("cr_lut"), "lut": tmp_ltc, "cl": [
                    str(i) for i in in_global_dict["Params"]["ListOfBandIndexForLTCComposite_DateD"]]}
            extractChannels_app = OtbAppHandler("LutExtractChannels", param_extractchannels)
            p_OutputL2ImageFileWriter.set_ltc_image(extractChannels_app.getoutput()["lut"])

    LOGGER.debug("Starting L2 product writing ...")
    p_OutputL2ImageFileWriter.write(l2_write_working)
    LOGGER.debug("End of L2 product writing ...")
    # ** ** * END WRITING DATA ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** **
