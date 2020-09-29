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

maja_test_application(NAME apTvComposite
                     APP  Composite
                     OPTIONS -tocr ${MAJADATA_TVA_INPUT}/CompositeTest/apTvSudouest_20060326_tocr.tif
                             -toac ${MAJADATA_TVA_INPUT}/CompositeTest/apTvSudouest_20060326_toa.tif
                            -toc ${MAJADATA_TVA_INPUT}/CompositeTest/apTvSudouest_20060326_toc.tif
                            -edg ${MAJADATA_TVA_INPUT}/CompositeTest/apTvSudouest_20060326_edg.tif
                            -cldsum ${MAJADATA_TVA_INPUT}/CompositeTest/apTvSudouest_20060326_pcld.tif
                            -sat ${MAJADATA_TVA_INPUT}/CompositeTest/apTvSudouest_20060326_pdat.tif
                            -aot ${MAJADATA_TVA_INPUT}/CompositeTest/apTvSudouest_20060326_aot_mask.tif
                            -initmode true
                            -reall2nodata -10
                            -l2nodata -10000
                            -sigmaaot 1
                            -minaot 0.10000000000000000555111512312
                            -maxaot 0.5
                            -maxage 60
                            -hotspotflag 0
                            -cirrusflag 0
                            -rainflag 0
                            -jday 8120
                            -stobandnumber 10
                            -stobandindex 0
                            -aotradius 1
                            -rta ${MAJA_TEST_OUTPUT_ROOT}/TVA/apTvCompositeImageFilter_RTA.tif
                            -rcr ${MAJA_TEST_OUTPUT_ROOT}/TVA/apTvCompositeImageFilter_RCR.tif
                            -rtc ${MAJA_TEST_OUTPUT_ROOT}/TVA/apTvCompositeImageFilter_RTC.tif
                            -sto ${MAJA_TEST_OUTPUT_ROOT}/TVA/apTvCompositeImageFilter_STO.tif
                            -pxd ${MAJA_TEST_OUTPUT_ROOT}/TVA/apTvCompositeImageFilter_PXD.tif
                            -ndt ${MAJA_TEST_OUTPUT_ROOT}/TVA/apTvCompositeImageFilter_NDT.tif


                     VALID   --compare-n-images ${EPSILON_3} 6
                             ${MAJADATA_TVA_BASELINE}/apTvCompositeImageFilter_RTA.tif
                             ${MAJA_TEST_OUTPUT_ROOT}/TVA/apTvCompositeImageFilter_RTA.tif

                             ${MAJADATA_TVA_BASELINE}/apTvCompositeImageFilter_RCR.tif
                             ${MAJA_TEST_OUTPUT_ROOT}/TVA/apTvCompositeImageFilter_RCR.tif

                             ${MAJADATA_TVA_BASELINE}/apTvCompositeImageFilter_RTC.tif
                             ${MAJA_TEST_OUTPUT_ROOT}/TVA/apTvCompositeImageFilter_RTC.tif

                             ${MAJADATA_TVA_BASELINE}/apTvCompositeImageFilter_STO.tif
                             ${MAJA_TEST_OUTPUT_ROOT}/TVA/apTvCompositeImageFilter_STO.tif

                             ${MAJADATA_TVA_BASELINE}/apTvCompositeImageFilter_PXD.tif
                             ${MAJA_TEST_OUTPUT_ROOT}/TVA/apTvCompositeImageFilter_PXD.tif

                             ${MAJADATA_TVA_BASELINE}/apTvCompositeImageFilter_NDT.tif
                             ${MAJA_TEST_OUTPUT_ROOT}/TVA/apTvCompositeImageFilter_NDT.tif

                    )


