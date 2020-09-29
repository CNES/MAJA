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

maja_test_application(NAME apTvDarkestPixelAOTNominal
                     APP  DarkestPixelAOT
                     OPTIONS
                     -toac ${MAJADATA_TVA_INPUT}/AOTEstimation/L930662_20020212_L7_198_030_USGS_toac_240m.tif
                     -shd ${MAJADATA_TVA_INPUT}/AOTEstimation/L930662_20020212_L7_198_030_USGS_shd_240m.tif
                     -dtm ${MAJADATA_TVA_INPUT}/DTM/LANDSAT/LN_TEST_AUX_REFDE2_EU93066200A00B_0001.DBL.DIR/LN_TEST_AUX_REFDE2_EU93066200A00B_0001_ALC.tif
                     -cld ${MAJADATA_TVA_INPUT}/AOTEstimation/L930662_20020212_L7_198_030_USGS_l2cld_240m.tif
                     -l2ndt ${MAJADATA_TVA_INPUT}/AOTEstimation/L930662_20020212_L7_198_030_USGS_l2ndt_240m.tif
                     -l2rtc ${MAJADATA_TVA_INPUT}/AOTEstimation/L930662_20020212_L7_198_030_USGS_l2rtc_240m.tif
                     -edg ${MAJADATA_TVA_INPUT}/AOTEstimation/L930662_20020212_L7_198_030_USGS_edg_240m.tif
                     -lutheader  ${MAJADATA_TVA_INPUT}/AOTEstimation/giplut.xml
                     -lutimage  ${MAJADATA_TVA_INPUT}/AOTEstimation/lutAOT.mha
                     -darkestchanneltoac 0
                     -thresholddarkpixel 0.015
                     -darksurfacereflectance 0
                     -darkestchannelrtc 0C

                     TESTENVOPTIONS ${MAJA_TEST_OUTPUT_ROOT}/TVA/apTvDarkestPixelAOTFlag.txt

                     VALID   --compare-ascii ${EPSILON_6}
                            ${MAJADATA_TVA_BASELINE}/apTvDarkestPixelAOTFlag.txt
                            ${MAJA_TEST_OUTPUT_ROOT}/TVA/apTvDarkestPixelAOTFlag.txt

                    )


maja_test_application(NAME apTvGapFilling
                     APP  GapFilling
                     OPTIONS
                     -im ${MAJADATA_TVA_INPUT}/AOTEstimation/L930662_20020212_L7_198_030_USGS_aot.tif
                     -initwindowsradius 2
                     -maxwindowsradius 16
                     -reall2nodata -10
                     -mean 0.0294888857403667603573893529756
                     -defaultvalue 0.2
                     -hasvalidpixels 1
                     -out ${MAJA_TEST_OUTPUT_ROOT}/TVA/apTvGapFilling_filledImage.tif
                     -outmask ${MAJA_TEST_OUTPUT_ROOT}/TVA/apTvGapFilling_mask.tif

                     VALID   --compare-n-images ${EPSILON_5} 2
                            ${MAJADATA_TVA_BASELINE}/apTvGapFilling_filledImage.tif
                            ${MAJA_TEST_OUTPUT_ROOT}/TVA/apTvGapFilling_filledImage.tif

                            ${MAJADATA_TVA_BASELINE}/apTvGapFilling_mask.tif
                            ${MAJA_TEST_OUTPUT_ROOT}/TVA/apTvGapFilling_mask.tif

                    )


maja_test_application(NAME apTvAOTEstimation_SpectroTemporalNominal
                     APP  AOTEstimation
                     OPTIONS
                     -toac ${MAJADATA_TVA_INPUT}/AOTEstimation/SpectroTemporal/L930662_20020212_L7_198_030_USGS_toac_240m.tif
                     -shd ${MAJADATA_TVA_INPUT}/AOTEstimation/SpectroTemporal/L930662_20020212_L7_198_030_USGS_shd_240m.tif
                     -dtm ${MAJADATA_TVA_INPUT}/DTM/LANDSAT/LN_TEST_AUX_REFDE2_EU93066200A00B_0001.DBL.DIR/LN_TEST_AUX_REFDE2_EU93066200A00B_0001_ALC.tif
                     -cld ${MAJADATA_TVA_INPUT}/AOTEstimation/SpectroTemporal/L930662_20020212_L7_198_030_USGS_l2cld_240m.tif
                     -l2ndt ${MAJADATA_TVA_INPUT}/AOTEstimation/SpectroTemporal/L930662_20020212_L7_198_030_USGS_l2ndt_240m.tif
                     -l2rtc ${MAJADATA_TVA_INPUT}/AOTEstimation/SpectroTemporal/L930662_20020212_L7_198_030_USGS_l2rtc_240m.tif
                     -edg ${MAJADATA_TVA_INPUT}/AOTEstimation/SpectroTemporal/L930662_20020212_L7_198_030_USGS_edg_240m.tif
                     -lutheader  ${MAJADATA_TVA_INPUT}/AOTEstimation/SpectroTemporal/giplut.xml
                     -lutimage  ${MAJADATA_TVA_INPUT}/AOTEstimation/SpectroTemporal/lutAOT.mha
                     -mode spectrotemporal
                     -xmlparamsfile ${MAJADATA_TVA_INPUT}/AOTEstimation/SpectroTemporal/AOTParams_LANDSAT.xml
                     -temporal.altitudemean 759.934899999999970532371662557
                     -reall2nodata -10
                     -jday 6617
                     -darkestaot 0.11594
                     -temporal.lutmap ${MAJADATA_TVA_INPUT}/AOTEstimation/SpectroTemporal/giplutmap.xml

                    -sat ${MAJADATA_TVA_INPUT}/AOTEstimation/SpectroTemporal/L930662_20020212_L7_198_030_USGS_sat_240m.tif
                    -l2rta ${MAJADATA_TVA_INPUT}/AOTEstimation/SpectroTemporal/L930662_20020212_L7_198_030_USGS_l2rta_240m.tif
                    -l2pxd ${MAJADATA_TVA_INPUT}/AOTEstimation/SpectroTemporal/L930662_20020212_L7_198_030_USGS_l2pxd_240m.tif

                     -was ${MAJADATA_TVA_INPUT}/AOTEstimation/SpectroTemporal/L930662_20020212_L7_198_030_USGS_was_240m.tif
                     -snow ${MAJADATA_TVA_INPUT}/AOTEstimation/SpectroTemporal/L930662_20020212_L7_198_030_USGS_snow_240m.tif

                     -aotmask ${MAJA_TEST_OUTPUT_ROOT}/TVA/apTvAOTEstimationSpectrotemporalNominal_aotmask.tif
                     -aotcomputed ${MAJA_TEST_OUTPUT_ROOT}/TVA/apTvAOTEstimationSpectrotemporalNominal_aotcomputed.tif

                     VALID   --compare-n-images ${EPSILON_4} 2
                            ${MAJADATA_TVA_BASELINE}/apTvAOTEstimationSpectrotemporalNominal_aotcomputed.tif
                            ${MAJA_TEST_OUTPUT_ROOT}/TVA/apTvAOTEstimationSpectrotemporalNominal_aotcomputed.tif

                            ${MAJADATA_TVA_BASELINE}/apTvAOTEstimationSpectrotemporalNominal_aotmask.tif
                            ${MAJA_TEST_OUTPUT_ROOT}/TVA/apTvAOTEstimationSpectrotemporalNominal_aotmask.tif

                   )



maja_test_application(NAME apTvAOTMean_SpectroTemporalNominal
                     APP  AOTEstimation
                     OPTIONS
                     -toac ${MAJADATA_TVA_INPUT}/AOTEstimation/SpectroTemporal/L930662_20020212_L7_198_030_USGS_toac_240m.tif
                     -shd ${MAJADATA_TVA_INPUT}/AOTEstimation/SpectroTemporal/L930662_20020212_L7_198_030_USGS_shd_240m.tif
                     -dtm ${MAJADATA_TVA_INPUT}/DTM/LANDSAT/LN_TEST_AUX_REFDE2_EU93066200A00B_0001.DBL.DIR/LN_TEST_AUX_REFDE2_EU93066200A00B_0001_ALC.tif
                     -cld ${MAJADATA_TVA_INPUT}/AOTEstimation/SpectroTemporal/L930662_20020212_L7_198_030_USGS_l2cld_240m.tif
                     -l2ndt ${MAJADATA_TVA_INPUT}/AOTEstimation/SpectroTemporal/L930662_20020212_L7_198_030_USGS_l2ndt_240m.tif
                     -l2rtc ${MAJADATA_TVA_INPUT}/AOTEstimation/SpectroTemporal/L930662_20020212_L7_198_030_USGS_l2rtc_240m.tif
                     -edg ${MAJADATA_TVA_INPUT}/AOTEstimation/SpectroTemporal/L930662_20020212_L7_198_030_USGS_edg_240m.tif
                     -lutheader  ${MAJADATA_TVA_INPUT}/AOTEstimation/giplut.xml
                     -lutimage  ${MAJADATA_TVA_INPUT}/AOTEstimation/lutAOT.mha
                     -mode spectrotemporal
                     -xmlparamsfile ${MAJADATA_TVA_INPUT}/AOTEstimation/SpectroTemporal/AOTParams_LANDSAT.xml
                     -temporal.altitudemean 759.934899999999970532371662557
                     -reall2nodata -10
                     -jday 6617
                     -darkestaot 0.11594
                     -temporal.lutmap ${MAJADATA_TVA_INPUT}/AOTEstimation/giplutmap.xml

                    -sat ${MAJADATA_TVA_INPUT}/AOTEstimation/L930662_20020212_L7_198_030_USGS_sat_240m.tif
                    -l2rta ${MAJADATA_TVA_INPUT}/AOTEstimation/L930662_20020212_L7_198_030_USGS_l2rta_240m.tif
                    -l2pxd ${MAJADATA_TVA_INPUT}/AOTEstimation/L930662_20020212_L7_198_030_USGS_l2pxd_240m.tif

                     -was ${MAJADATA_TVA_INPUT}/AOTEstimation/L930662_20020212_L7_198_030_USGS_was_240m.tif
                     -snow ${MAJADATA_TVA_INPUT}/AOTEstimation/L930662_20020212_L7_198_030_USGS_snow_240m.tif

                     -aotmask ${MAJA_TEST_OUTPUT_ROOT}/TVA/apTvAOTEstimationSpectrotemporalNominal_aotmask.tif
                     -aotcomputed ${MAJA_TEST_OUTPUT_ROOT}/TVA/apTvAOTEstimationSpectrotemporalNominal_aotcomputed.tif

                     TESTENVOPTIONS ${MAJA_TEST_OUTPUT_ROOT}/TVA/apTvAOTEstimation_ST_Flag.txt

                     VALID  --compare-ascii ${EPSILON_4}
                            ${MAJADATA_TVA_BASELINE}/apTvAOTEstimation_ST_Flag.txt
                            ${MAJA_TEST_OUTPUT_ROOT}/TVA/apTvAOTEstimation_ST_Flag.txt


                   )



maja_test_application(NAME apTvAOTEstimation_MultiTemporalNominal
                     APP  AOTEstimation
                     OPTIONS
                     -toac ${MAJADATA_TVA_INPUT}/AOTEstimation/MultiTemporal/Sudouest_20061114_MS_fmsat_ortho_toac.tif
                     -shd ${MAJADATA_TVA_INPUT}/AOTEstimation/MultiTemporal/Sudouest_20061114_MS_fmsat_ortho_shd.tif
                     -dtm ${MAJADATA_TVA_INPUT}/AOTEstimation/MultiTemporal/Sudouest_20061114_MS_fmsat_ortho_dtm.tif
                     -cld ${MAJADATA_TVA_INPUT}/AOTEstimation/MultiTemporal/Sudouest_20061114_MS_fmsat_ortho_cld.tif
                     -l2ndt ${MAJADATA_TVA_INPUT}/AOTEstimation/MultiTemporal/Sudouest_20061114_MS_fmsat_ortho_lL2inNDT.tif
                     -l2rtc ${MAJADATA_TVA_INPUT}/AOTEstimation/MultiTemporal/Sudouest_20061114_MS_fmsat_ortho_lL2inRTC.tif
                     -edg ${MAJADATA_TVA_INPUT}/AOTEstimation/MultiTemporal/Sudouest_20061114_MS_fmsat_ortho_edg.tif
                     -sat ${MAJADATA_TVA_INPUT}/AOTEstimation/MultiTemporal/Sudouest_20061114_MS_fmsat_ortho_sat.tif
                     -l2rta ${MAJADATA_TVA_INPUT}/AOTEstimation/MultiTemporal/Sudouest_20061114_MS_fmsat_ortho_lL2inRTA.tif
                     -l2pxd ${MAJADATA_TVA_INPUT}/AOTEstimation/MultiTemporal/Sudouest_20061114_MS_fmsat_ortho_lL2inPXD.tif
                     -was ${MAJADATA_TVA_INPUT}/AOTEstimation/MultiTemporal/Sudouest_20061114_MS_fmsat_ortho_was.tif
                     -snow ${MAJADATA_TVA_INPUT}/AOTEstimation/MultiTemporal/Sudouest_20061114_MS_fmsat_ortho_snw.tif
                     -lutheader ${MAJADATA_TVA_INPUT}/AOTEstimation/MultiTemporal/giplut.xml
                     -lutimage ${MAJADATA_TVA_INPUT}/AOTEstimation/MultiTemporal/lutAOTMT1.mha
                     -mode multitemporal
                     -xmlparamsfile ${MAJADATA_TVA_INPUT}/AOTEstimation/MultiTemporal/AOTParams_LANDSAT.xml
                     -temporal.altitudemean 0
                     -jday 8036
                     -reall2nodata 0
                     -darkestaot 0.0430164
                     -temporal.lutmap ${MAJADATA_TVA_INPUT}/AOTEstimation/MultiTemporal/giplutmap.xml

                     -aotmask ${MAJA_TEST_OUTPUT_ROOT}/TVA/apTvAOTEstimationMultiTemporalNominal_aotmask.tif
                     -aotcomputed ${MAJA_TEST_OUTPUT_ROOT}/TVA/apTvAOTEstimationMultiTemporalNominal_aotcomputed.tif

                     VALID   --compare-n-images ${EPSILON_4} 2
                     ${MAJADATA_TVA_BASELINE}/apTvAOTEstimationMultiTemporalNominal_aotmask.tif
                     ${MAJA_TEST_OUTPUT_ROOT}/TVA/apTvAOTEstimationMultiTemporalNominal_aotmask.tif
                     ${MAJADATA_TVA_BASELINE}/apTvAOTEstimationMultiTemporalNominal_aotcomputed.tif
                     ${MAJA_TEST_OUTPUT_ROOT}/TVA/apTvAOTEstimationMultiTemporalNominal_aotcomputed.tif


                    )


maja_test_application(NAME apTvAOTEstimationMultiSpectral
                     APP  AOTEstimation
                     OPTIONS
                     -toac ${MAJADATA_TVA_INPUT}/AOTEstimation/MultiSpectral/L930662_20020325_L7_197_030_USGS_toa_240m.tif
                     -shd ${MAJADATA_TVA_INPUT}/AOTEstimation/MultiSpectral/L930662_20020325_L7_197_030_USGS_toa_240m.shd
                     -dtm ${MAJADATA_TVA_INPUT}/DTM/LANDSAT/VE_TEST_AUX_REFDE2_L930662__0001.DBL.DIR/L930662_240m.mnt_ALC
                     -cld ${MAJADATA_TVA_INPUT}/AOTEstimation/MultiSpectral/L930662_20020325_L7_197_030_USGS_toa_240m.nua
                     -edg ${MAJADATA_TVA_INPUT}/AOTEstimation/MultiSpectral/L930662_20020325_L7_197_030_USGS_toa_bord_240m
                     -sat ${MAJADATA_TVA_INPUT}/AOTEstimation/MultiSpectral/L930662_20020325_L7_197_030_USGS_toa_240m.sat
                     -was ${MAJADATA_TVA_INPUT}/AOTEstimation/MultiSpectral/L930662_20020325_L7_197_030_USGS_toa_240m.water
                     -snow ${MAJADATA_TVA_INPUT}/AOTEstimation/MultiSpectral/L930662_20020325_L7_197_030_USGS_toa_240m.nei
                     -lutheader ${MAJADATA_TVA_INPUT}/AOTEstimation/MultiSpectral/giplut.xml
                     -lutimage ${MAJADATA_TVA_INPUT}/AOTEstimation/MultiSpectral/lutAOTMS.mha
                     -mode multispectral
                     -xmlparamsfile ${MAJADATA_TVA_INPUT}/AOTEstimation/MultiSpectral/AOTParams_LANDSAT.xml
                     -reall2nodata 0.
                     -darkestaot 0.13
                     -jday 6617
                     -init true
                     -aotmask ${MAJA_TEST_OUTPUT_ROOT}/TVA/apTvAOTEstimationMultiSpectral_aotmask.tif
                     -aotcomputed ${MAJA_TEST_OUTPUT_ROOT}/TVA/apTvAOTEstimationMultiSpectral_aotcomputed.tif

                     VALID   --compare-n-images ${EPSILON_4} 2
                     ${MAJADATA_TVA_BASELINE}/apTvAOTEstimationMultiSpectral_aotcomputed.tif
                     ${MAJA_TEST_OUTPUT_ROOT}/TVA/apTvAOTEstimationMultiSpectral_aotcomputed.tif
                     ${MAJADATA_TVA_BASELINE}/apTvAOTEstimationMultiSpectral_aotmask.tif
                     ${MAJA_TEST_OUTPUT_ROOT}/TVA/apTvAOTEstimationMultiSpectral_aotmask.tif

                    )

maja_test_application(NAME apTvAOTEstimationSpectroTemporalCAMS
                     APP  AOTEstimation
                     OPTIONS
                     -toac ${MAJADATA_TVA_INPUT}/AOTEstimation/SpectroTemporalCAMS/normal/20160614/PRIVE/toa_abs/31TFJR108_20160614_S2A_L1Csimu_toa_240m_coeff.tif
                     -shd ${MAJADATA_TVA_INPUT}/AOTEstimation/SpectroTemporalCAMS/normal/20160614/PRIVE/mnt/31TFJR108_20160614_S2A_L1Csimu_toa_240m.shd
                     -dtm ${MAJADATA_TVA_INPUT}/DTM/SENTINEL2/S2__OPER_AUX_REFDE2_31TFJ____0001.DBL.DIR/S2__OPER_AUX_REFDE2_31TFJ____0001_ALC.tif
                     -cld ${MAJADATA_TVA_INPUT}/AOTEstimation/SpectroTemporalCAMS/normal/20160614/PRIVE/nua/31TFJR108_20160614_S2A_L1Csimu_toa_240m.nua
                     -l2ndt ${MAJADATA_TVA_INPUT}/AOTEstimation/SpectroTemporalCAMS/MasqueNoir.tif
                     -l2rtc ${MAJADATA_TVA_INPUT}/AOTEstimation/SpectroTemporalCAMS/normal/20160607/PRIVE/surf_comp/31TFJR008_20160607_S2A_L1Csimu_surf_comp_240m_coeff.tif
                     -edg ${MAJADATA_TVA_INPUT}/AOTEstimation/SpectroTemporalCAMS/normal/20160614/PRIVE/bord/31TFJR108_20160614_S2A_L1Csimu_toa_bord_240m.hdr
                     -lutheader  ${MAJADATA_TVA_INPUT}/AOTEstimation/SpectroTemporalCAMS/giplut.xml
                     -lutimage  ${MAJADATA_TVA_INPUT}/AOTEstimation/SpectroTemporalCAMS/lutAOT.mha
                     -mode spectrotemporal
                     -xmlparamsfile ${MAJADATA_TVA_INPUT}/AOTEstimation/SpectroTemporalCAMS/AOTParams_SENTINEL2.xml
                     -temporal.altitudemean 182.243020728
                     -reall2nodata -10
                     -jday 11853
                     -darkestaot 0.123605
                     -temporal.lutmap ${MAJADATA_TVA_INPUT}/AOTEstimation/SpectroTemporalCAMS/giplutmap.xml

                    -sat ${MAJADATA_TVA_INPUT}/AOTEstimation/SpectroTemporalCAMS/MasqueNoir.tif
                    -l2rta ${MAJADATA_TVA_INPUT}/AOTEstimation/SpectroTemporalCAMS/normal/20160607/PRIVE/toa_comp/31TFJR008_20160607_S2A_L1Csimu_toa_comp_240m_coef.tif
                    -l2pxd ${MAJADATA_TVA_INPUT}/AOTEstimation/SpectroTemporalCAMS/normal/20160607/PRIVE/date/31TFJR008_20160607_S2A_L1Csimu_toa_240m.date

                     -was ${MAJADATA_TVA_INPUT}/AOTEstimation/SpectroTemporalCAMS/normal/20160614/PRIVE/water/31TFJR108_20160614_S2A_L1Csimu_toa_240m.water
                     -snow ${MAJADATA_TVA_INPUT}/AOTEstimation/SpectroTemporalCAMS/normal/20160614/PRIVE/nei/31TFJR108_20160614_S2A_L1Csimu_toa_240m.nei
                     -cams.aot ${MAJADATA_TVA_INPUT}/AOTEstimation/SpectroTemporalCAMS/ReduceLutComputationCAMSTest001-AOT.tif
                     -cams.altmean 182.243020728
                     -cams.nodatatau -1
                     -aotmask ${MAJA_TEST_OUTPUT_ROOT}/TVA/apTvAOTEstimationSpectrotemporalCams_aotmask.tif
                     -aotcomputed ${MAJA_TEST_OUTPUT_ROOT}/TVA/apTvAOTEstimationSpectrotemporalCams_aotcomputed.tif
                     -aotcams ${MAJA_TEST_OUTPUT_ROOT}/TVA/apTvAOTEstimationSpectrotemporalCams_aotcams.tif
                     TESTENVOPTIONS ${MAJA_TEST_OUTPUT_ROOT}/TVA/apTvAOTEstimationCams_ST_Flag.txt

                     VALID   --compare-n-images ${EPSILON_4} 2
                     ${MAJADATA_TVA_BASELINE}/apTvAOTEstimationSpectrotemporalCams_aotcomputed.tif
                     ${MAJA_TEST_OUTPUT_ROOT}/TVA/apTvAOTEstimationSpectrotemporalCams_aotcomputed.tif

                     ${MAJADATA_TVA_BASELINE}/apTvAOTEstimationSpectrotemporalCams_aotmask.tif
                     ${MAJA_TEST_OUTPUT_ROOT}/TVA/apTvAOTEstimationSpectrotemporalCams_aotmask.tif

                   )                    
