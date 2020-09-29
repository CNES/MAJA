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
# VERSION : 1-0-0 : <TypeFT> : <NumFT> : 29 janvier 2019 : Creation                                        #
#                                                                                                          #
# FIN-HISTORIQUE                                                                                           #
#                                                                                                          #
# $Id                                                                                                      #
#                                                                                                          #
############################################################################################################

maja_test_application(NAME apTvDTMProcessing
                     APP  DTMProcessing
                     OPTIONS -solar ${MAJADATA_TVA_INPUT}/DTMProcessingTest/Maroc_20061216_solar.tif
                             -viedtm ${MAJADATA_TVA_INPUT}/DTMProcessingTest/Maroc_20061216_vie.tif
                             -edg ${MAJADATA_TVA_INPUT}/DTMProcessingTest/Maroc_20061216_edg.tif
                             -dtm ${MAJADATA_TVA_INPUT}/DTM/FORMOSAT/VE_TEST_AUX_REFDE2_MAROC____0001.DBL.DIR/Maroc_96m.mnt_ALC
                             -distthreshold 96
                             -solhref 4000
                             -viehref 4000
                             -hid ${MAJA_TEST_OUTPUT_ROOT}/TVA/apTvDTMProcessing_hid.tif
                             -shd ${MAJA_TEST_OUTPUT_ROOT}/TVA/apTvDTMProcessing_shd.tif

                     VALID   --compare-n-images ${EPSILON_4} 2
                             ${MAJADATA_TVA_BASELINE}/DTMProcessingTest-001MAHID.tif
                             ${MAJA_TEST_OUTPUT_ROOT}/TVA/apTvDTMProcessing_hid.tif

                             ${MAJADATA_TVA_BASELINE}/DTMProcessingTest-001MASHD.tif
                             ${MAJA_TEST_OUTPUT_ROOT}/TVA/apTvDTMProcessing_shd.tif

                    )
