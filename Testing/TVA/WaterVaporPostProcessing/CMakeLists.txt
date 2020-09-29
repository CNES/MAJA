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
# VERSION : 1-0-0 : <TypeFT> : <NumFT> :  13 fevrier 2019 : Creation                                       #
#                                                                                                          #
# FIN-HISTORIQUE                                                                                           #
#                                                                                                          #
# $Id                                                                                                      #
#                                                                                                          #
############################################################################################################

maja_test_application(NAME apTvWaterVaporPostPro
                     APP  WaterVaporPostPro
                     OPTIONS
                        -tocr ${MAJADATA_TVA_INPUT}/WaterVaporPostProcessing/31TFJ_20150908_S2A_L1Csimu_surf_240m.tif
                        -vapin  ${MAJADATA_TVA_INPUT}/WaterVaporPostProcessing/31TFJ_20150908_S2A_L1Csimu_surf_VAP_SUB.tif
                        -cldsum ${MAJADATA_TVA_INPUT}/WaterVaporPostProcessing/31TFJ_20150908_S2A_L1Csimu_toa_240m.CLDSum.tif
                        -defaultwatervapor 0
                        -threshold 200
                        -thresholdref 100
                        -referencebandcode 8
                        -bandcode 9
                        -nodata 0
                        -sevalidradius 1
                        -initwindowradius 2
                        -maxwindowradius 16
                        -mask ${MAJA_TEST_OUTPUT_ROOT}/TVA/apTvWaterVaporPostPro_mask_240m.tif
                        -vap ${MAJA_TEST_OUTPUT_ROOT}/TVA/apTvWaterVaporPostPro_VAP_Filtered_240m.tif

                     VALID   --compare-n-images ${EPSILON_7} 2
                     ${MAJADATA_TVA_BASELINE}/apTvWaterVaporPostPro_mask_240m.tif
                     ${MAJA_TEST_OUTPUT_ROOT}/TVA/apTvWaterVaporPostPro_mask_240m.tif

                     ${MAJADATA_TVA_BASELINE}/apTvWaterVaporPostPro_VAP_Filtered_240m.tif
                     ${MAJA_TEST_OUTPUT_ROOT}/TVA/apTvWaterVaporPostPro_VAP_Filtered_240m.tif

                    )
