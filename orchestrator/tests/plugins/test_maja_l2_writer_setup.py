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

                        o     o
                        oo   oo   oo        o   oo        ,-.
                        o o o o  o  o       o  o  o       \_/
                        o  o  o o    o      o o    o     {|||D
                        o     o oooooo      o oooooo      / \
                        o     o o    o o    o o    o      `-^
                        o     o o    o  oooo  o    o

###################################################################################################

orchestrator.tests.plugins.test_maja_sentinel2_l1_pimage_file_reader -- shortdesc

orchestrator.tests.plugins.test_maja_sentinel2_l1_pimage_file_reader is a description

It defines classes_and_methods


###################################################################################################
"""

from __future__ import absolute_import
from orchestrator.common.logger.maja_logging import configure_logger
from orchestrator.common.gipp_utils import *

import glob

def set_input_l2_writer(input_dir, dict_img):

    # Set input images
    sre_list = []
    fre_list = []
    l2sat_list = []
    l2stl_list = []
    l2tgs_list = []
    l2aot_list = []
    l2tao_list = []
    l2vap_list = []
    l2iwc_list = []
    l2pix_list = []
    l2edg_list = []
    l2cld_list = []
    dfp_list = []

    os.chdir(input_dir)

    for file in os.listdir(input_dir):
        if file.endswith('.mha') :
            dict_img["ltc"] = os.path.join(input_dir, file)
            break
        else:
            dict_img["ltc"] = None


    if os.path.isfile(os.path.join(input_dir, 'cloud_cla.tif')):
        dict_img["cla"] = os.path.join(input_dir + 'cloud_cla.tif')
    else:
        dict_img["cla"] = None

    if os.path.isfile(os.path.join(input_dir, 'cloud_snow_mask.tif')):
        dict_img["snow"] = os.path.join(input_dir + 'cloud_snow_mask.tif')
    else:
        dict_img["snow"] = None

    if os.path.isfile(os.path.join(input_dir, 'dtm_shd.tif')):
        try:
            dict_img["dtm_shd"] = os.path.join(input_dir + 'dtm_shd.tif')
        except FileNotFoundError:
            LOGGER.debug("The file " + os.path.basename(
                os.path.join(input_dir, 'dtm_shd.tif')) + "doesn't exist ! ")

    if os.path.isfile(os.path.join(input_dir, 'dtm_hid.tif')):
        try:
            dict_img["dtm_hid"] = os.path.join(input_dir + 'dtm_hid.tif')
        except FileNotFoundError:
            LOGGER.debug("The file " + os.path.basename(
                os.path.join(input_dir, 'dtm_hid.tif')) + "doesn't exist ! ")


    if os.path.isfile(os.path.join(input_dir, 'water_sub.tif')):
        try:
            dict_img["was"] = os.path.join(input_dir + 'water_sub.tif')
        except FileNotFoundError:
            LOGGER.debug("The file " + os.path.basename(
                os.path.join(input_dir, 'water_sub.tif')) + "doesn't exist ! ")


    if os.path.isfile(os.path.join(input_dir, 'possible_water_sub.tif')):
        try:
            dict_img["pwa"] = os.path.join(input_dir + 'possible_water_sub.tif')
        except FileNotFoundError:
            LOGGER.debug("The file " + os.path.basename(
                os.path.join(input_dir, 'possible_water_sub.tif')) + "doesn't exist ! ")


    if os.path.isfile(os.path.join(input_dir, 'tested_water_sub.tif')):
        try:
            dict_img["twa"] = os.path.join(input_dir + 'tested_water_sub.tif')
        except FileNotFoundError:
            LOGGER.debug("The file " + os.path.basename(
                os.path.join(input_dir, 'tested_water_sub.tif')) + "doesn't exist ! ")


    if os.path.isfile(os.path.join(input_dir, 'composite_rta.tif')):
        try:
            dict_img["rta_composite"] = os.path.join(input_dir + 'composite_rta.tif')
        except FileNotFoundError:
            LOGGER.debug("The file " + os.path.basename(
                os.path.join(input_dir, 'composite_rta.tif')) + "doesn't exist ! ")

    if os.path.isfile(os.path.join(input_dir, 'composite_rtc.tif')):
        try:
            dict_img["rtc_composite"] = os.path.join(input_dir + 'composite_rtc.tif')
        except FileNotFoundError:
            LOGGER.debug("The file " + os.path.basename(
                os.path.join(input_dir, 'composite_rtc.tif')) + "doesn't exist ! ")

    if os.path.isfile(os.path.join(input_dir, 'composite_rcr.tif')):
        try:
            dict_img["rcr_composite"] = os.path.join(input_dir + 'composite_rcr.tif')
        except FileNotFoundError:
            LOGGER.debug("The file " + os.path.basename(
                os.path.join(input_dir, 'composite_rcr.tif')) + "doesn't exist ! ")

    if os.path.isfile(os.path.join(input_dir, 'composite_sto.tif')):
        try:
            dict_img["sto_composite"] = os.path.join(input_dir + 'composite_sto.tif')
        except FileNotFoundError:
            LOGGER.debug("The file " + os.path.basename(
                os.path.join(input_dir, 'composite_sto.tif')) + "doesn't exist ! ")

    if os.path.isfile(os.path.join(input_dir, 'composite_pxd.tif')):
        try:
            dict_img["pxd_composite"] = os.path.join(input_dir + 'composite_pxd.tif')
        except FileNotFoundError:
            LOGGER.debug("The file " + os.path.basename(
                os.path.join(input_dir, 'composite_pxd.tif')) + "doesn't exist ! ")

    if os.path.isfile(os.path.join(input_dir, 'composite_ndt.tif')):
        try:
            dict_img["ndt_composite"] = os.path.join(input_dir + 'composite_ndt.tif')
        except FileNotFoundError:
            LOGGER.debug("The file " + os.path.basename(
                os.path.join(input_dir, 'composite_ndt.tif')) + "doesn't exist ! ")

    l2cld_img_R1 = [os.path.join(input_dir, 'cloud_all_R1.tif'),
                    os.path.join(input_dir, 'cloud_allclouds_R1.tif'),
                    os.path.join(input_dir, 'cloud_shadows_R1.tif'),
                    os.path.join(input_dir, 'cloud_shadvar_R1.tif'),
                    os.path.join(input_dir, 'cloud_refl_R1.tif'),
                    os.path.join(input_dir, 'cloud_reflvar_R1.tif'),
                    os.path.join(input_dir, 'cloud_ext_R1.tif'),
                    os.path.join(input_dir, 'cloud_alt_R1.tif'),
                    os.path.join(input_dir, 'cloud_cirrus_R1.tif')]

    l2cld_img_R2 = [os.path.join(input_dir, 'cloud_all_R2.tif'),
                    os.path.join(input_dir, 'cloud_allclouds_R2.tif'),
                    os.path.join(input_dir, 'cloud_shadows_R2.tif'),
                    os.path.join(input_dir, 'cloud_shadvar_R2.tif'),
                    os.path.join(input_dir, 'cloud_refl_R2.tif'),
                    os.path.join(input_dir, 'cloud_reflvar_R2.tif'),
                    os.path.join(input_dir, 'cloud_ext_R2.tif'),
                    os.path.join(input_dir, 'cloud_alt_R2.tif'),
                    os.path.join(input_dir, 'cloud_cirrus_R2.tif')]

    if all(os.path.isfile(l2cld_img_R1[i]) for i in range(0, len(l2cld_img_R1) - 1)):
        try:
            l2cld_list.append(l2cld_img_R1)
            if all(os.path.isfile(l2cld_img_R2[i]) for i in range(0, len(l2cld_img_R2) - 1)):
                l2cld_list.append(l2cld_img_R2)

        except FileNotFoundError:
            LOGGER.debug("Missing one image of L2 CLD image list ! ")


    tmp_cld = [os.path.join(input_dir, 'cloud_sum_dilated_masked.tif'),
                   os.path.join(input_dir, 'cloud_all_dilated_masked.tif'),
                   os.path.join(input_dir, 'cloud_shadow_dilated_masked.tif'),
                   os.path.join(input_dir, 'cloud_constzero.tif'),
                   os.path.join(input_dir, 'cloud_refl_dilated_masked.tif'),
                   os.path.join(input_dir, 'cloud_constzero.tif'),
                   os.path.join(input_dir, 'cloud_constzero.tif'),
                   os.path.join(input_dir, 'cloud_alt_dilated_masked.tif'),
                   os.path.join(input_dir, 'cloud_cirrus_dilated_masked.tif')]

    if all(os.path.isfile(tmp_cld[i]) for i in range(0, len(tmp_cld) - 1)):
        try:
            dict_img["CLD_List"] = tmp_cld
        except FileNotFoundError:
            LOGGER.debug("Missing one image of CLD image list ! ")



    if os.path.isfile(os.path.join(input_dir, 'sre_R1.tif')):
        try:
            sre_list.append(os.path.join(
                input_dir, 'sre_R1.tif'))
            if os.path.isfile(os.path.join(input_dir, 'sre_R2.tif')):
                sre_list.append(os.path.join(
                    input_dir, 'sre_R2.tif'))
        except FileNotFoundError:
            LOGGER.debug("The file " + os.path.basename(
                os.path.join(input_dir, 'sre_list.tif')) + "doesn't exist ! ")

    if os.path.isfile(os.path.join(input_dir, 'fre_R1.tif')):
        try:
            fre_list.append(os.path.join(
                input_dir, 'fre_R1.tif'))
            if os.path.isfile(os.path.join(input_dir, 'fre_R2.tif')):
                fre_list.append(os.path.join(
                    input_dir, 'fre_R2.tif'))
        except FileNotFoundError:
            LOGGER.debug("The file " + os.path.basename(
                os.path.join(input_dir, 'fre.tif')) + "doesn't exist ! ")

    if os.path.isfile(os.path.join(input_dir, 'L1_SAT_Masks_R1.tif')):
        l2sat_list.append(os.path.join(input_dir, 'L1_SAT_Masks_R1.tif'))
        if os.path.isfile(os.path.join(input_dir, 'L1_SAT_Masks_R2.tif')):
            l2sat_list.append(os.path.join(input_dir, 'L1_SAT_Masks_R2.tif'))
    elif os.path.isfile(os.path.join(input_dir, 'tmp_sat_resample_0.tif')):
        l2sat_list.append(os.path.join(input_dir, 'tmp_sat_resample_0.tif'))
    else:
        raise OSError("Cannot find l2sat file")


    if os.path.isfile(os.path.join(input_dir, 'stl_R1.tif')):
        try:
            l2stl_list.append(os.path.join(
                input_dir, 'stl_R1.tif'))
            if os.path.isfile(os.path.join(input_dir, 'stl_R2.tif')):
                l2stl_list.append(os.path.join(
                    input_dir, 'stl_R2.tif'))
        except FileNotFoundError:
            LOGGER.debug("The file " + os.path.basename(
                os.path.join(input_dir, 'stl_R1.tif')) + "doesn't exist ! ")


    if os.path.isfile(os.path.join(input_dir, 'tgs_R1.tif')):
        try:
            l2tgs_list.append(os.path.join(
                input_dir, 'tgs_R1.tif'))
            if os.path.isfile(os.path.join(input_dir, 'tgs_R2.tif')):
                l2tgs_list.append(os.path.join(
                    input_dir, 'tgs_R2.tif'))
        except FileNotFoundError:
            LOGGER.debug("The file " + os.path.basename(
                os.path.join(input_dir, 'tgs_R1.tif')) + "doesn't exist ! ")


    if os.path.isfile(os.path.join(input_dir, 'aot_R1.tif')):
        try:
            l2aot_list.append(os.path.join(
                input_dir, 'aot_R1.tif'))
            if os.path.isfile(os.path.join(input_dir, 'aot_R2.tif')):
                l2aot_list.append(os.path.join(
                    input_dir, 'aot_R2.tif'))
        except FileNotFoundError:
            LOGGER.debug("The file " + os.path.basename(
                os.path.join(input_dir, 'aot_R1.tif')) + "doesn't exist ! ")


    if os.path.isfile(os.path.join(input_dir, 'aotmask_R1.tif')):
        try:
            l2tao_list.append(os.path.join(
                input_dir, 'aotmask_R1.tif'))
            if os.path.isfile(os.path.join(input_dir, 'aotmask_R2.tif')):
                l2tao_list.append(os.path.join(
                    input_dir, 'aotmask_R2.tif'))
        except FileNotFoundError:
            LOGGER.debug("The file " + os.path.basename(
                os.path.join(input_dir, 'aotmask_R1.tif')) + "doesn't exist ! ")

    if os.path.isfile(os.path.join(input_dir, 'vap_R1.tif')):
        try:
            l2vap_list.append(os.path.join(
                input_dir, 'vap_R1.tif'))
            if os.path.isfile(os.path.join(input_dir, 'vap_R2.tif')):
                l2vap_list.append(os.path.join(
                    input_dir, 'vap_R2.tif'))
        except FileNotFoundError:
            LOGGER.debug("The file " + os.path.basename(
                os.path.join(input_dir, 'vap_R1.tif')) + "doesn't exist ! ")

    if os.path.isfile(os.path.join(input_dir, 'vapmask_R1.tif')):
        try:
            l2iwc_list.append(os.path.join(
                input_dir, 'vapmask_R1.tif'))
            if os.path.isfile(os.path.join(input_dir, 'vapmask_R2.tif')):
                l2iwc_list.append(os.path.join(
                    input_dir, 'vapmask_R2.tif'))
        except FileNotFoundError:
            LOGGER.debug("The file " + os.path.basename(
                os.path.join(input_dir, 'vapmask_R1.tif')) + "doesn't exist ! ")

    if os.path.isfile(os.path.join(input_dir, 'Masks_const_.tif')):
        l2pix_list.append(os.path.join(
            input_dir, "Masks_const_.tif"))
    elif os.path.isfile(os.path.join(input_dir, 'tmp_dfp_resample_0.tif')):
        l2pix_list.append(os.path.join(
            input_dir, 'tmp_dfp_resample_0.tif'))
    else:
        for file in os.listdir(input_dir):
            if "_DFP_R1" in file:
                l2pix_list.append(os.path.join(
                    input_dir, file))
        for file in os.listdir(input_dir):
            if "_DFP_R2" in file:
                l2pix_list.append(os.path.join(
                    input_dir, file))


    if os.path.isfile(os.path.join(input_dir, 'IPEDGRealL2_R1.tif')):
        try:
            l2edg_list.append(os.path.join(
                input_dir, 'IPEDGRealL2_R1.tif'))
            if os.path.isfile(os.path.join(input_dir, 'IPEDGRealL2_R2.tif')):
                l2edg_list.append(os.path.join(
                    input_dir, 'IPEDGRealL2_R2.tif'))
        except FileNotFoundError:
            LOGGER.debug("The file " + os.path.basename(
                os.path.join(input_dir, 'IPEDGRealL2_R1.tif')) + "doesn't exist ! ")

    if os.path.isfile(os.path.join(input_dir, 'L1_DFP_Masks_R1.tif')):
        dfp_list.append(os.path.join(
            input_dir, 'L1_DFP_Masks_R1.tif'))
        if os.path.isfile(os.path.join(input_dir, 'L1_DFP_Masks_R2.tif')):
            dfp_list.append(os.path.join(
                    input_dir, 'L1_DFP_Masks_R2.tif'))

    dict_img["SRE_List"] = sre_list
    dict_img["FRE_List"] = fre_list
    dict_img["L2SAT_List"] = l2sat_list
    dict_img["L2STL_List"] = l2stl_list
    dict_img["L2TGS_List"] = l2tgs_list
    dict_img["L2AOT_List"] = l2aot_list
    dict_img["L2TAO_List"] = l2tao_list
    dict_img["L2VAP_List"] = l2vap_list
    dict_img["L2IWC_List"] = l2iwc_list
    dict_img["L2PIX_List"] = l2pix_list
    dict_img["L2EDG_List"] = l2edg_list
    dict_img["L2CLD_List"] = l2cld_list
    dict_img["L2DFP_List"] = dfp_list


    LOGGER.debug(dict_img["SRE_List"] )
    LOGGER.debug(dict_img["FRE_List"])
    LOGGER.debug(dict_img["L2SAT_List"])
    LOGGER.debug(dict_img["L2STL_List"])
    LOGGER.debug(dict_img["L2TGS_List"])
    LOGGER.debug(dict_img["L2AOT_List"])
    LOGGER.debug(dict_img["L2VAP_List"])
    LOGGER.debug(dict_img["L2IWC_List"])
    LOGGER.debug(dict_img["L2PIX_List"])
    LOGGER.debug(dict_img["L2EDG_List"])
    LOGGER.debug(dict_img["L2CLD_List"])
    LOGGER.debug(dict_img["L2DFP_List"])



