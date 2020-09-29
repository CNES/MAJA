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
# VERSION : 1-0-0 : <TypeFT> : <NumFT> : 04 janvier 2019 : Creation                                        #
#                                                                                                          #
# FIN-HISTORIQUE                                                                                           #
#                                                                                                          #
# $Id                                                                                                      #
#                                                                                                          #
############################################################################################################

maja_test_application(NAME apTvCirrusMask
                     APP  CirrusMask
                     OPTIONS -toa ${MAJADATA_TVA_INPUT}/CirrusMaskTest/15SVD_20091211_S2A_L1Csimu_toa_240m_coef_001.tif
                             -edg ${MAJADATA_TVA_INPUT}/CirrusMaskTest/15SVD_20091211_S2A_L1Csimu_toa_bord_240m
                             -dtm ${MAJADATA_TVA_INPUT}/DTM/SENTINEL2/VE_TEST_AUX_REFDE2_15SDV____0001.DBL.DIR/S2N_TEST_AUX_REFDE2_15SVD_0001_ALC_roi.tif
                             -bandidx 10
                             -threshoff 0.35
                             -threshgain 0
                             -minpercent 10
                             -altref 2000
                             -mask ${MAJA_TEST_OUTPUT_ROOT}/TVA/apTv15SVD_20091211_S2A_L1Csimu_CirrusMask.tif

                     VALID   --compare-image 0.0
                             ${MAJADATA_TVA_BASELINE}/apTv15SVD_20091211_S2A_L1Csimu_CirrusMask.tif
                             ${MAJA_TEST_OUTPUT_ROOT}/TVA/apTv15SVD_20091211_S2A_L1Csimu_CirrusMask.tif
                    )

