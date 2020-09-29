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

maja_test_application(NAME apTvGammaCompute
                     APP  GammaCompute
                     OPTIONS -l2toa ${MAJADATA_TVA_INPUT}/CirrusCorrectionTest/11SLT_20150803_S2A_L1Csimu_toa_abs_10m.tiff
                             -l2edg ${MAJADATA_TVA_INPUT}/CirrusCorrectionTest/11SLT_20150803_S2A_L1Csimu_bord_10m.bord_final
                             -l2dtm ${MAJADATA_TVA_INPUT}/DTM/SENTINEL2/11SLT/11SLT_10m.mnt
                             -l2cirrus ${MAJADATA_TVA_INPUT}/CirrusCorrectionTest/11SLT_20150803_S2A_L1Csimu_toa_60m.B10_resampled.tif
                             -bandlist 2 3
                             -cloudcirrus ${MAJADATA_TVA_INPUT}/CirrusCorrectionTest/11SLT_20150803_S2A_L1Csimu_cloud_cirrus_10m.tif
                             -cloudall ${MAJADATA_TVA_INPUT}/CirrusCorrectionTest/11SLT_20150803_S2A_L1Csimu_all_mask_10m.tif
                             -cloudrefl ${MAJADATA_TVA_INPUT}/CirrusCorrectionTest/11SLT_20150803_S2A_L1Csimu_refl_mask_10m.tif
                             -cirrusminpixinlist 100
                             -mask  ${MAJA_TEST_OUTPUT_ROOT}/TVA/apTv11SLT_20150803_mask_cirrus_10m.tif
                     TESTENVOPTIONS ${MAJA_TEST_OUTPUT_ROOT}/TVA/file.txt

                     VALID   --compare-image ${EPSILON_4}
                             ${MAJADATA_TVA_BASELINE}/apTv11SLT_20150803_mask_cirrus_10m.tif
                             ${MAJA_TEST_OUTPUT_ROOT}/TVA/apTv11SLT_20150803_mask_cirrus_10m.tif

                    )


maja_test_application(NAME apTvCirrusCorrectionApply
                     APP  CirrusCorrectionApply
                     OPTIONS -l2toa ${MAJADATA_TVA_INPUT}/CirrusCorrectionTest/11SLT_20150803_S2A_L1Csimu_toa_abs_coeff_001_10m.tif
                             -l2edg ${MAJADATA_TVA_INPUT}/CirrusCorrectionTest/11SLT_20150803_S2A_L1Csimu_bord_10m.bord_final
                             -l2dtm ${MAJADATA_TVA_INPUT}/DTM/SENTINEL2/11SLT/11SLT_10m_roi.tif
                             -l2cirrus ${MAJADATA_TVA_INPUT}/CirrusCorrectionTest/11SLT_20150803_S2A_L1Csimu_toa_60m.B10_resampled_coef_001.tif
                             -cirrusmask ${MAJADATA_TVA_BASELINE}/apTv11SLT_20150803_mask_cirrus_10m.tif
                             -resolution 10
                             -maxdistance 1000
                             -thresholdfactor 0.95
                             -thresholdoffset 0.015
                             -thresholdgain 0.00002
                             -gammamean 1.055496216
                             -swirgammafactor 2
                             -maxreflectance 0.07
                             -bandcodelist B2 B3 B4 B8
                             -swirbandcodelist B11 B12
                             -correctedtoa ${MAJA_TEST_OUTPUT_ROOT}/TVA/apTv11SLT_20150803_corrected_TOA_10m.tif
                     VALID   --compare-image 0.01
                             ${MAJADATA_TVA_BASELINE}/apTv11SLT_20150803_corrected_TOA_10m.tif
                             ${MAJA_TEST_OUTPUT_ROOT}/TVA/apTv11SLT_20150803_corrected_TOA_10m.tif

                    )

