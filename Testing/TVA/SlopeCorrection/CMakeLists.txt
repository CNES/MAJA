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
# VERSION : 1-0-0 : <TypeFT> : <NumFT> : 11 Fevrier 2019 : Creation                                        #
#                                                                                                          #
#                                                                                                          #
# FIN-HISTORIQUE                                                                                           #
#                                                                                                          #
# $Id                                                                                                      #
#                                                                                                          #
############################################################################################################

maja_test_application(NAME apTvSlopeCorrection
                     APP  SlopeCorrection
                     OPTIONS
                     -tdirsub ${MAJADATA_TVA_INPUT}/SlopeCorrection/15SVD_20091211_S2A_L1Csimu_tdir.tif
                     -tdifsub ${MAJADATA_TVA_INPUT}/SlopeCorrection/15SVD_20091211_S2A_L1Csimu_tdif.tif
                     -dtmasc ${MAJADATA_TVA_INPUT}/DTM/SENTINEL2/VE_TEST_AUX_REFDE2_15SDV____0001.DBL.DIR/S2N_TEST_AUX_REFDE2_15SVD_0001_ASP_R2_multipliedByScalar.tif
                     -dtmslc ${MAJADATA_TVA_INPUT}/DTM/SENTINEL2/VE_TEST_AUX_REFDE2_15SDV____0001.DBL.DIR/S2N_TEST_AUX_REFDE2_15SVD_0001_SLP_R2_multipliedByScalar.tif
                     -rhoenv ${MAJADATA_TVA_INPUT}/SlopeCorrection/15SVD_20091211_S2A_L1Csimu_rho_env_10m.tif
                     -incangle ${MAJADATA_TVA_INPUT}/SlopeCorrection/15SVD_20091211_S2A_L1Csimu_IncidenceAngle.tif
                     -sre ${MAJADATA_TVA_INPUT}/SlopeCorrection/15SVD_20091211_S2A_L1Csimu_surf_env_10m.tif
                     -thetas 64.165163327
                     -phis 162.402228733
                     -thetav 0 0 0 0 0 0
                     -phiv 0 0 0 0 0 0
                     -mincosi 0.200000000000000011102230246252
                     -mincose 0.100000000000000005551115123126
                     -mincosratio 0.200000000000000011102230246252
                     -nodata -10
                     -fre ${MAJA_TEST_OUTPUT_ROOT}/TVA/apTvSlopeCorrection_15SVD_20091211_S2A_fre.tif
                     -tgs ${MAJA_TEST_OUTPUT_ROOT}/TVA/apTvSlopeCorrection_15SVD_20091211_S2A_tgs.tif
                     -stl ${MAJA_TEST_OUTPUT_ROOT}/TVA/apTvSlopeCorrection_15SVD_20091211_S2A_stl.tif

                     VALID   --compare-n-images ${EPSILON_3} 3
                     ${MAJADATA_TVA_BASELINE}/apTvSlopeCorrection_15SVD_20091211_S2A_fre.tif
                     ${MAJA_TEST_OUTPUT_ROOT}/TVA/apTvSlopeCorrection_15SVD_20091211_S2A_fre.tif

                     ${MAJADATA_TVA_BASELINE}/apTvSlopeCorrection_15SVD_20091211_S2A_tgs.tif
                     ${MAJA_TEST_OUTPUT_ROOT}/TVA/apTvSlopeCorrection_15SVD_20091211_S2A_tgs.tif

                     ${MAJADATA_TVA_BASELINE}/apTvSlopeCorrection_15SVD_20091211_S2A_stl.tif
                     ${MAJA_TEST_OUTPUT_ROOT}/TVA/apTvSlopeCorrection_15SVD_20091211_S2A_stl.tif

                    )


maja_test_application(NAME apTvComputeTransmission
                     APP  ComputeTransmission
                     OPTIONS
                     -luttdir ${MAJADATA_TVA_INPUT}/SlopeCorrection/giplutTdirmap.xml
                     -luttdif ${MAJADATA_TVA_INPUT}/SlopeCorrection/giplutTdifmap.xml
                     -aot ${MAJADATA_TVA_INPUT}/SlopeCorrection/15SVD_20091211_S2A_L1Csimu_aot.tif
                     -dtm ${MAJADATA_TVA_INPUT}/DTM/SENTINEL2/VE_TEST_AUX_REFDE2_15SDV____0001.DBL.DIR/S2N_TEST_AUX_REFDE2_15SVD_0001_ALC.tif
                     -edg ${MAJADATA_TVA_INPUT}/SlopeCorrection/15SVD_20091211_S2A_L1Csimu_edg.tif
                     -thetas 64.1652
                     -tdir ${MAJA_TEST_OUTPUT_ROOT}/TVA/15SVD_20091211_S2A_L1Csimu_tdir.tif
                     -tdif ${MAJA_TEST_OUTPUT_ROOT}/TVA/15SVD_20091211_S2A_L1Csimu_tdif.tif

                     VALID   --compare-n-images ${EPSILON_4} 2
                     ${MAJADATA_TVA_BASELINE}/apTvComputeTransmissionTdir.tif
                     ${MAJA_TEST_OUTPUT_ROOT}/TVA/15SVD_20091211_S2A_L1Csimu_tdir.tif

                     ${MAJADATA_TVA_BASELINE}/apTvComputeTransmissionTdif.tif
                     ${MAJA_TEST_OUTPUT_ROOT}/TVA/15SVD_20091211_S2A_L1Csimu_tdif.tif

                    )
