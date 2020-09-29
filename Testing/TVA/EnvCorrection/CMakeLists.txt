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
# VERSION : 1-0-0 : <TypeFT> : <NumFT> : 28 fevrier 2019 : Creation                                        #
#                                                                                                          #
#                                                                                                          #
# FIN-HISTORIQUE                                                                                           #
#                                                                                                          #
# $Id                                                                                                      #
#                                                                                                          #
############################################################################################################

maja_test_application(NAME apTvComputeRhoEnv
                     APP  ComputeRhoEnv
                     OPTIONS
                     -toc ${MAJADATA_TVA_INPUT}/EnvCorrection/Sudouest_20060329_MS_fmsat_ortho_surf_96m_tocSub.tif
                     -edg ${MAJADATA_TVA_INPUT}/EnvCorrection/Sudouest_20060329_MS_fmsat_ortho_surf_96m_edgSub.tif
                     -radius 10
                     -filtercoeff 1
                     -rhoenv ${MAJA_TEST_OUTPUT_ROOT}/TVA/apTvComputeRhoEnv.tif

                     VALID   --compare-image ${EPSILON_5}
                             ${MAJADATA_TVA_BASELINE}/apTvComputeRhoEnv.tif
                             ${MAJA_TEST_OUTPUT_ROOT}/TVA/apTvComputeRhoEnv.tif

                   )


maja_test_application(NAME apTvEnvCorrection
                     APP  EnvCorrection
                     OPTIONS
                     -tdir ${MAJADATA_TVA_INPUT}/EnvCorrection/Sudouest_20060329_MS_fmsat_ortho_surf_96m_Tdir.tif
                     -tdif ${MAJADATA_TVA_INPUT}/EnvCorrection/Sudouest_20060329_MS_fmsat_ortho_surf_96m_Tdif.tif
                     -albd ${MAJADATA_TVA_INPUT}/EnvCorrection/Sudouest_20060329_MS_fmsat_ortho_surf_96m_Satm.tif
                     -toc ${MAJADATA_TVA_INPUT}/EnvCorrection/Sudouest_20060329_MS_fmsat_ortho_surf_96m_toc.tif
                     -edg ${MAJADATA_TVA_INPUT}/EnvCorrection/Sudouest_20060329_MS_fmsat_ortho_surf_96m_edg.tif
                     -rhoenvsub ${MAJADATA_TVA_BASELINE}/apTvComputeRhoEnv.tif
                     -nodata -10
                     -sre ${MAJA_TEST_OUTPUT_ROOT}/TVA/apTvEnvCorrection_SRE.tif
                     -rhoenv ${MAJA_TEST_OUTPUT_ROOT}/TVA/apTvEnvCorrection_RhoEnv.tif

                     VALID   --compare-n-images ${EPSILON_6} 2
                             ${MAJADATA_TVA_BASELINE}/apTvEnvCorrection_SRE.tif
                             ${MAJA_TEST_OUTPUT_ROOT}/TVA/apTvEnvCorrection_SRE.tif

                             ${MAJADATA_TVA_BASELINE}/apTvEnvCorrection_RhoEnv.tif
                             ${MAJA_TEST_OUTPUT_ROOT}/TVA/apTvEnvCorrection_RhoEnv.tif

                   )


maja_test_application(NAME apTvInterpolateForEnvCorr
                     APP  InterpolateForEnvCorr
                     OPTIONS

                     -lutalbd ${MAJADATA_TVA_INPUT}/EnvCorrection/giplutalbedomap.xml
                     -luttdir ${MAJADATA_TVA_INPUT}/SlopeCorrection/giplutTdirmap.xml
                     -luttdif ${MAJADATA_TVA_INPUT}/SlopeCorrection/giplutTdifmap.xml
                     -aot ${MAJADATA_TVA_INPUT}/SlopeCorrection/15SVD_20091211_S2A_L1Csimu_aot.tif
                     -dtm ${MAJADATA_TVA_INPUT}/DTM/SENTINEL2/VE_TEST_AUX_REFDE2_15SDV____0001.DBL.DIR/S2N_TEST_AUX_REFDE2_15SVD_0001_ALC.tif
                     -edg ${MAJADATA_TVA_INPUT}/SlopeCorrection/15SVD_20091211_S2A_L1Csimu_edg.tif
                     -thetav 0 0 0 0 0 0 0 0 0 0 0 0 0
                     -tdir ${MAJA_TEST_OUTPUT_ROOT}/TVA/apTvInterpolateForEnvCorr_Tdir.tif
                     -tdif ${MAJA_TEST_OUTPUT_ROOT}/TVA/apTvInterpolateForEnvCorr_Tdif.tif
                     -albd ${MAJA_TEST_OUTPUT_ROOT}/TVA/apTvInterpolateForEnvCorr_albedo.tif

                     VALID   --compare-n-images ${EPSILON_5} 3
                     ${MAJADATA_TVA_BASELINE}/apTvInterpolateForEnvCorr_Tdir.tif
                     ${MAJA_TEST_OUTPUT_ROOT}/TVA/apTvInterpolateForEnvCorr_Tdir.tif

                     ${MAJADATA_TVA_BASELINE}/apTvInterpolateForEnvCorr_Tdif.tif
                     ${MAJA_TEST_OUTPUT_ROOT}/TVA/apTvInterpolateForEnvCorr_Tdif.tif

                     ${MAJADATA_TVA_BASELINE}/apTvInterpolateForEnvCorr_albedo.tif
                     ${MAJA_TEST_OUTPUT_ROOT}/TVA/apTvInterpolateForEnvCorr_albedo.tif


                   )
