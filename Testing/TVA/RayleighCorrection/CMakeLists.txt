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

maja_test_application(NAME apTvRayleighCorrection
                     APP  RayleighCorrection
                     OPTIONS
                     -lutimage ${MAJADATA_TVA_INPUT}/RayleighCorrection/lut.mha
                     -lutheader ${MAJADATA_TVA_INPUT}/RayleighCorrection/giplut.xml
                     -toa ${MAJADATA_TVA_INPUT}/RayleighCorrection/Sudouest_20060819_MS_fmsat_ortho_toa_96m_AOTnull.tif
                     -edg ${MAJADATA_TVA_INPUT}/RayleighCorrection/Sudouest_20060819_MS_fmsat_ortho_toa_bord_96m
                     -dtm ${MAJADATA_TVA_INPUT}/DTM/FORMOSAT/VE_TEST_AUX_REFDE2_SUDOUEST_0001.DBL.DIR/Sudouest_96m.mnt_ALC
                     -nodata -10
                     -defaultaot 0
                     -tocr ${MAJA_TEST_OUTPUT_ROOT}/TVA/apTvRayleighCorrection.tif


                     VALID   --compare-image ${EPSILON_5}
                             ${MAJADATA_TVA_BASELINE}/apTvRayleighCorrection.tif
                             ${MAJA_TEST_OUTPUT_ROOT}/TVA/apTvRayleighCorrection.tif

                   )
