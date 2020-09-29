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
#
############################################################################################################
#                                                                                                          #
#                        o     o                                                                           #
#                        oo   oo   oo        o   oo        ,-.                                             #
#                        o o o o  o  o       o  o  o       \_/                                             #
#                        o  o  o o    o      o o    o     {|||D                                            #
#                        o     o oooooo      o oooooo      / \                                             #
#                        o     o o    o o    o o    o      `-^                                             #
#                        o     o o    o  oooo  o    o                                                      #
#                                                                                                          #
############################################################################################################
#                                                                                                          #
# Author: CS Systemes d'Information  (France)                                                              #
#                                                                                                          #
############################################################################################################
# HISTORIQUE                                                                                               #
#                                                                                                          #
# VERSION : 1-0-0 : <TypeFT> : <NumFT> : 24 janvier 2019 : Creation                                        #
#                                                                                                          #
# FIN-HISTORIQUE                                                                                           #
#                                                                                                          #
# $Id                                                                                                      #
#                                                                                                          #
############################################################################################################


maja_test_application(NAME apTvWaterMask
                     APP  WaterMask
                     OPTIONS -tocr ${MAJADATA_TVA_INPUT}/WaterMaskingTest/31TFJ_20150928_S2A_L1Csimu_surf_ray_240m.tif
                     -edg ${MAJADATA_TVA_INPUT}/WaterMaskingTest/31TFJ_20150928_S2A_L1Csimu_toa_bord_240m
                     -cldsum ${MAJADATA_TVA_INPUT}/WaterMaskingTest/31TFJ_20150928_S2A_cld.tif
                     -dtmshd ${MAJADATA_TVA_INPUT}/WaterMaskingTest/31TFJ_20150928_S2A_L1Csimu_toa_240m.shd
                     -l2rcr ${MAJADATA_TVA_INPUT}/WaterMaskingTest/31TFJ_20150918_S2A_L1Csimu_surf_ray_comp_240m.tif
                     -l2pwa ${MAJADATA_TVA_INPUT}/WaterMaskingTest/31TFJ_20150918_S2A_L1Csimu_toa_240m.compt
                     -l2twa ${MAJADATA_TVA_INPUT}/WaterMaskingTest/31TFJ_20150918_S2A_L1Csimu_toa_240m.pos
                     -sunglintflag true
                     -demslc ${MAJADATA_TVA_INPUT}/WaterMaskingTest/31TFJ_240m_slc.tif
                     -srtmwat ${MAJADATA_TVA_INPUT}/WaterMaskingTest/31TFJ_240m.eau
                     -redbandtocr 3
                     -nirbandtocr 8
                     -nirbandrcr 8
                     -waterndvithreshold 0.01
                     -waterreflectancevariationthreshold 0.04
                     -maximumsunglintreflectance 0.12
                     -srtmgaussiansigma 10
                     -srtmpercent 51
                     -watermasknumber 10
                     -minpercentpossiblewater 50
                     -waterslopethreshold 0.01
                     -waterredreflectancethreshold 0.05
                     -reall2nodata -10
                     -was ${MAJA_TEST_OUTPUT_ROOT}/TVA/apTvWaterMaskingImageFilterTest-WAS.tif
                     -pwa ${MAJA_TEST_OUTPUT_ROOT}/TVA/apTvWaterMaskingImageFilterTest-PWA.tif
                     -twa ${MAJA_TEST_OUTPUT_ROOT}/TVA/apTvWaterMaskingImageFilterTest-TWA.tif

                     VALID   --compare-n-images ${EPSILON_4} 3
                             ${MAJADATA_TVA_BASELINE}/apTvWaterMaskingImageFilterTest-WAS.tif
                             ${MAJA_TEST_OUTPUT_ROOT}/TVA/apTvWaterMaskingImageFilterTest-WAS.tif

                             ${MAJADATA_TVA_BASELINE}/apTvWaterMaskingImageFilterTest-PWA.tif
                             ${MAJA_TEST_OUTPUT_ROOT}/TVA/apTvWaterMaskingImageFilterTest-PWA.tif

                             ${MAJADATA_TVA_BASELINE}/apTvWaterMaskingImageFilterTest-TWA.tif
                             ${MAJA_TEST_OUTPUT_ROOT}/TVA/apTvWaterMaskingImageFilterTest-TWA.tif

                    )


