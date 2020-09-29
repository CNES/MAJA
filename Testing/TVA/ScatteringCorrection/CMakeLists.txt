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
# VERSION : 1-0-0 : <TypeFT> : <NumFT> : 25 janvier 2019 : Creation                                        #
#                                                                                                          #
#                                                                                                          #
# FIN-HISTORIQUE                                                                                           #
#                                                                                                          #
# $Id                                                                                                      #
#                                                                                                          #
############################################################################################################

maja_test_application(NAME apTvScatteringCorrection
                     APP  ScatteringCorrection
                     OPTIONS -lutmap ${MAJADATA_TVA_INPUT}/ScatteringCorrectionTest/giplutmap.xml
                             -toac ${MAJADATA_TVA_INPUT}/ScatteringCorrectionTest/apTvSudouest_20060819_toac.tif
                             -aot ${MAJADATA_TVA_INPUT}/ScatteringCorrectionTest/apTvSudouest_20060819_aot.tif
                             -edg ${MAJADATA_TVA_INPUT}/ScatteringCorrectionTest/apTvSudouest_20060819_edg.tif
                             -dtm ${MAJADATA_TVA_INPUT}/ScatteringCorrectionTest/apTvSudouest_20060819_dtm.tif
                             -l2nodata 0
                             -l2bandincoarse 0 1 2 3
                             -tocr ${MAJA_TEST_OUTPUT_ROOT}/TVA/apTvScatteringCorrectionImageFilterTest.tif

                     VALID   --compare-image ${EPSILON_7}
                             ${MAJADATA_TVA_BASELINE}/apTvScatteringCorrectionImageFilterTest.tif
                             ${MAJA_TEST_OUTPUT_ROOT}/TVA/apTvScatteringCorrectionImageFilterTest.tif

                    )
