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
# VERSION : 1-0-0 : <TypeFT> : <NumFT> : 30 janvier 2019 : Creation                                        #
#                                                                                                          #
# FIN-HISTORIQUE                                                                                           #
#                                                                                                          #
# $Id                                                                                                      #
#                                                                                                          #
############################################################################################################


maja_test_application(NAME apTvCloudReflectance
                     APP  CloudReflectance
                     OPTIONS
                     -tocr ${MAJADATA_TVA_INPUT}/CloudMaskingTest/Sudouest_20060326_MS_fmsat_ortho_tocr.tif
                     -edg ${MAJADATA_TVA_INPUT}/CloudMaskingTest/Sudouest_20060326_MS_fmsat_ortho_edg.tif
                     -sat ${MAJADATA_TVA_INPUT}/CloudMaskingTest/Sudouest_20060326_MS_fmsat_ortho_sat.tif
                     -initmode true
                     -foregroundvalue 1
                     -bluebandtocr 0
                     -redbandtocr 2
                     -nirbandtocr 3
                     -reflectancewaterband 0
                     -redreflectancethresholdvalue 0.149999999999999994448884876874
                     -bluereflectancethresholdvalue 0.25
                     -nirredratiomaxthresholdvalue 2
                     -nirredratiominthresholdvalue 0.800000000000000044408920985006
                     -nodata -10
                     -correlbandtocr 0
                     -waterthreshold 1
                     -swir.bandtocr 0
                     -swir.nirswirratiomin 1
                     -refl ${MAJA_TEST_OUTPUT_ROOT}/TVA/apTv31TCJ_20150904_S2A_L1Csimu_cloud_reflectance.tif
                     VALID   --compare-image 0.0
                             ${MAJADATA_TVA_BASELINE}/apTvReflectanceThresholdTest.tif
                             ${MAJA_TEST_OUTPUT_ROOT}/TVA/apTv31TCJ_20150904_S2A_L1Csimu_cloud_reflectance.tif

                    )


maja_test_application(NAME apTvCloudReflectanceVariation
                     APP  CloudReflectance
                     OPTIONS
                     -tocr ${MAJADATA_TVA_INPUT}/CloudMaskingTest/Lacrau_20060813_MS_fmsat_ortho_tocr.tif
                     -sat ${MAJADATA_TVA_INPUT}/CloudMaskingTest/Lacrau_20060813_MS_fmsat_ortho_sat.tif
                     -edg ${MAJADATA_TVA_INPUT}/CloudMaskingTest/Lacrau_20060813_MS_fmsat_ortho_edg.tif
                     -l2was ${MAJADATA_TVA_INPUT}/CloudMaskingTest/Lacrau_20060813_MS_fmsat_ortho_l2was.tif
                     -l2rcr ${MAJADATA_TVA_INPUT}/CloudMaskingTest/Lacrau_20060813_MS_fmsat_ortho_l2rcr.tif
                     -l2sto ${MAJADATA_TVA_INPUT}/CloudMaskingTest/Lacrau_20060813_MS_fmsat_ortho_l2sto.tif
                     -l2pxd ${MAJADATA_TVA_INPUT}/CloudMaskingTest/Lacrau_20060813_MS_fmsat_ortho_l2pxd.tif
                     -l2ndt ${MAJADATA_TVA_INPUT}/CloudMaskingTest/Lacrau_20060813_MS_fmsat_ortho_l2ndt.tif
                     -initmode false
                     -foregroundvalue 1
                     -bluebandtocr 0
                     -redbandtocr 2
                     -nirbandtocr 3
                     -reflectancewaterband 2
                     -bluereflectancethresholdvalue 0.200000000000000011102230246252
                     -redreflectancethresholdvalue 0.149999999999999994448884876874
                     -nirredratiomaxthresholdvalue 2
                     -nirredratiominthresholdvalue 0.800000000000000044408920985006
                     -nodata -10
                     -correlbandtocr 0
                     -waterthreshold 0.0800000000000000016653345369377
                     -nominal.seradius 1
                     -nominal.bluebandrcr 0
                     -nominal.redbandrcr 2
                     -nominal.nirbandrcr 3
                     -nominal.correlbandrcr 0
                     -nominal.jday 8260
                     -nominal.cloudforgettingduration 30
                     -nominal.minvarthreshold 0.0299999999999999988897769753748
                     -nominal.maxvarthreshold 0.0899999999999999966693309261245
                     -nominal.knirblue 1
                     -nominal.kredblue 1.5
                     -nominal.deltathreshold 0.00500000000000000010408340855861
                     -nominal.minpixelcorrelation 15
                     -nominal.correlthreshold 0.900000000000000022204460492503
                     -nominal.ncorrel 3 3
                     -nominal.stolistofdates 0 0 0 0 0 0 0 0 0 0
                     -swir.bandtocr 0
                     -swir.nirswirratiomin 1
                     -refl ${MAJA_TEST_OUTPUT_ROOT}/TVA/apTvReflectanceVariation_Refl.tif
                     -reflvar ${MAJA_TEST_OUTPUT_ROOT}/TVA/apTvReflectanceVariation_ReflVar.tif
                     -reflext ${MAJA_TEST_OUTPUT_ROOT}/TVA/apTvReflectanceVariation_ReflExt.tif
                     VALID   --compare-n-images 0 3
                             ${MAJA_TEST_OUTPUT_ROOT}/TVA/apTvReflectanceVariation_Refl.tif
                             ${MAJADATA_TVA_BASELINE}/apTvReflectanceVariation_Refl.tif

                             ${MAJADATA_TVA_BASELINE}/apTvReflectanceVariation_ReflExt.tif
                             ${MAJA_TEST_OUTPUT_ROOT}/TVA/apTvReflectanceVariation_ReflExt.tif

                             ${MAJADATA_TVA_BASELINE}/apTvReflectanceVariation_ReflVar.tif
                             ${MAJA_TEST_OUTPUT_ROOT}/TVA/apTvReflectanceVariation_ReflVar.tif
                    )


maja_test_application(NAME apTvCloudExtension
                     APP  CloudExtension
                     OPTIONS
                     -tocr ${MAJADATA_TVA_INPUT}/CloudMaskingTest/Lacrau_20060813_MS_fmsat_ortho_tocr.tif
                     -edg ${MAJADATA_TVA_INPUT}/CloudMaskingTest/Lacrau_20060813_MS_fmsat_ortho_edg.tif
                     -cldext ${MAJADATA_TVA_BASELINE}/apTvReflectanceVariation_ReflExt.tif
                     -cldrefl ${MAJADATA_TVA_BASELINE}/apTvReflectanceVariation_Refl.tif
                     -cldreflvar ${MAJADATA_TVA_BASELINE}/apTvReflectanceVariation_ReflVar.tif
                     -l2rcr ${MAJADATA_TVA_INPUT}/CloudMaskingTest/Lacrau_20060813_MS_fmsat_ortho_l2rcr.tif
                     -l2sto ${MAJADATA_TVA_INPUT}/CloudMaskingTest/Lacrau_20060813_MS_fmsat_ortho_l2sto.tif
                     -l2ndt ${MAJADATA_TVA_INPUT}/CloudMaskingTest/Lacrau_20060813_MS_fmsat_ortho_l2ndt.tif
                     -nodata -10
                     -correlbandtocr 0
                     -correlbandrcr 0
                     -jday 8260
                     -minpixelcorrelation 15
                     -correlthreshold 0.900000000000000022204460492503
                     -stolistofdates 0 0 0 0 0 0 0 0 0 0
                     -ncorrel 3 3
                     -discardedratiomin 0.2
                     -ext ${MAJA_TEST_OUTPUT_ROOT}/TVA/apTvCloudExtension.tif
                     VALID   --compare-image 0.0
                             ${MAJADATA_TVA_BASELINE}/apTvCloudExtension.tif
                             ${MAJA_TEST_OUTPUT_ROOT}/TVA/apTvCloudExtension.tif

                    )


maja_test_application(NAME apTvCloudShadVar
                     APP  CloudShadVar
                     OPTIONS
                     -tocr ${MAJADATA_TVA_INPUT}/CloudMaskingTest/Lacrau_20060813_MS_fmsat_ortho_tocr.tif
                     -edg ${MAJADATA_TVA_INPUT}/CloudMaskingTest/Lacrau_20060813_MS_fmsat_ortho_edg.tif
                     -cldshad ${MAJADATA_TVA_INPUT}/CloudMaskingTest/Lacrau_20060813_MS_fmsat_ortho_cldshad.tif
                     -cldall ${MAJADATA_TVA_INPUT}/CloudMaskingTest/Lacrau_20060813_MS_fmsat_ortho_cldall.tif
                     -vie ${MAJADATA_TVA_INPUT}/CloudMaskingTest/Lacrau_20060813_MS_fmsat_ortho_vie.tif
                     -dtm ${MAJADATA_TVA_INPUT}/DTM/FORMOSAT/Lacrau_96m.mnt
                     -sol ${MAJADATA_TVA_INPUT}/CloudMaskingTest/Lacrau_20060813_MS_fmsat_ortho_sol.tif
                     -l2rcr ${MAJADATA_TVA_INPUT}/CloudMaskingTest/Lacrau_20060813_MS_fmsat_ortho_l2rcr.tif
                     -l2was ${MAJADATA_TVA_INPUT}/CloudMaskingTest/Lacrau_20060813_MS_fmsat_ortho_l2was.tif
                     -l2ndt ${MAJADATA_TVA_INPUT}/CloudMaskingTest/Lacrau_20060813_MS_fmsat_ortho_l2ndt.tif
                     -l2shad ${MAJADATA_TVA_INPUT}/CloudMaskingTest/Lacrau_20060813_MS_fmsat_ortho_shad.tif
                     -l2shadvar ${MAJADATA_TVA_INPUT}/CloudMaskingTest/Lacrau_20060813_MS_fmsat_ortho_shad_var.tif
                     -l2sto ${MAJADATA_TVA_INPUT}/CloudMaskingTest/Lacrau_20060813_MS_fmsat_ortho_l2sto.tif
                     -shadbandtocr 2
                     -shadbandrcr 2
                     -viehref 3000
                     -solhref 3000
                     -nodata -10
                     -maxdarkening 0.8
                     -minpercentile 0.1
                     -maxcla 8226
                     -compositelabelratio 0.800000000000000044408920985006
                     -minpixelcorrelation 15
                     -correlthreshold 0.900000000000000022204460492503
                     -ncorrel 3 3
                     -stolistofdates 0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0
                     -jday 8260
                     -shadvar ${MAJA_TEST_OUTPUT_ROOT}/TVA/apTvCloudShadVar.tif

                     VALID   --compare-image 0.0
                             ${MAJADATA_TVA_BASELINE}/apTvCloudShadVar.tif
                             ${MAJA_TEST_OUTPUT_ROOT}/TVA/apTvCloudShadVar.tif

                    )


maja_test_application(NAME apTvCloudSnowCorrection
                     APP  CloudSnowCorrection
                     OPTIONS
                     -snowin ${MAJADATA_TVA_INPUT}/CloudMaskingTest/L930863_20130815_L8_196_030_USGS_toa_240m.neige_avant_correction
                     -edg ${MAJADATA_TVA_INPUT}/CloudMaskingTest/L930863_20130815_L8_196_030_USGS_toa_bord_240m
                     -cldreflvar ${MAJADATA_TVA_INPUT}/CloudMaskingTest/L930863_20130815_L8_196_030_USGS_toa_240m.refl_avant_correction
                     -cldrefl ${MAJADATA_TVA_INPUT}/CloudMaskingTest/L930863_20130815_L8_196_030_USGS_toa_240m.reflvar_avant_correction
                     -seradius 1
                     -refl ${MAJA_TEST_OUTPUT_ROOT}/TVA/apTvCloudSnowCorrection_reflvar.tif
                     -snow ${MAJA_TEST_OUTPUT_ROOT}/TVA/apTvCloudSnowCorrection_snow.tif
                     VALID   --compare-n-images 0 2
                             ${MAJADATA_TVA_BASELINE}/apTvCloudSnowCorrection_reflvar.tif
                             ${MAJA_TEST_OUTPUT_ROOT}/TVA/apTvCloudSnowCorrection_reflvar.tif

                             ${MAJADATA_TVA_BASELINE}/apTvCloudSnowCorrection_snow.tif
                             ${MAJA_TEST_OUTPUT_ROOT}/TVA/apTvCloudSnowCorrection_snow.tif

                    )


maja_test_application(NAME apTvCloudMaskUndersampling
                     APP  CloudMaskUndersampling
                     OPTIONS
                     -dtm ${MAJADATA_TVA_INPUT}/CloudMaskingTest/VE_TEST_AUX_REFDE2_CAMARGUE_0001_ALC.TIF
                     -im ${MAJADATA_TVA_INPUT}/CloudMaskingTest/VE_TEST_VSC_PDTANX_L1NOTV_CAMARGUE_20120103_CLA.DBL.TIF
                     -nodata 0
                     -cla ${MAJA_TEST_OUTPUT_ROOT}/TVA/apTvCloudMaskUndersampling_CLA.tif
                     -clalt ${MAJA_TEST_OUTPUT_ROOT}/TVA/apTvCloudMaskUndersampling_CLDAlt.tif
                     VALID   --compare-n-images 0 2
                             ${MAJADATA_TVA_BASELINE}/apTvCloudSnowCorrection_reflvar.tif
                             ${MAJA_TEST_OUTPUT_ROOT}/TVA/apTvCloudSnowCorrection_reflvar.tif
                             ${MAJADATA_TVA_BASELINE}/apTvCloudSnowCorrection_snow.tif
                             ${MAJA_TEST_OUTPUT_ROOT}/TVA/apTvCloudSnowCorrection_snow.tif

                    )



maja_test_application(NAME apTvCloudShadowWithAltitudeFilterInit
                     APP  CloudShadow
                     OPTIONS
                     -tocr ${MAJADATA_TVA_INPUT}/CloudMaskingTest/Sudouest_20060326_MS_fmsat_ortho_tocr.tif
                     -edg ${MAJADATA_TVA_INPUT}/CloudMaskingTest/Sudouest_20060326_MS_fmsat_ortho_edg.tif
                     -cldall  ${MAJADATA_TVA_INPUT}/CloudMaskingTest/Sudouest_20060326_MS_fmsat_ortho_cldall.tif
                     -cla  ${MAJADATA_TVA_INPUT}/CloudMaskingTest/Sudouest_20060326_MS_fmsat_ortho_cla.tif
                     -vie  ${MAJADATA_TVA_INPUT}/CloudMaskingTest/Sudouest_20060326_MS_fmsat_ortho_vie.tif
                     -dtm ${MAJADATA_TVA_INPUT}/DTM/FORMOSAT/VE_TEST_AUX_REFDE2_SUDOUEST_0001.DBL.DIR/Sudouest_96m.mnt_ALC
                     -initmode true
                     -sol1.in  ${MAJADATA_TVA_INPUT}/CloudMaskingTest/Sudouest_20060326_MS_fmsat_ortho_sol1.tif
                     -sol1.h 2000
                     -solhref 1000
                     -defaultalt 500
                     -ksigma 3
                     -deltahmax 8000
                     -deltahmin 0
                     -deltahstep 500
                     -shadbandtocr 0
                     -shadbandrcr 0
                     -viehref 3000
                     -nodata -10
                     -algo.withalt.refinement 1
                     -algo.withalt.absnbpixthresh 20
                     -algo.withalt.threshdiffim 10
                     -algo.withalt.sol2.h 0
                     -algo.withoutalt.cloudcovermult 2
                     -algo.withoutalt.shadratiomax 0
                     -algo.withoutalt.shadratiomin 0
                     -algo.withoutalt.shadratiopixnb 0
                     -algo.withoutalt.maxdarkening 0
                     -algo.withoutalt.minpercentile 0
                     -algo.withoutalt.seradius 0
                     -shadow ${MAJA_TEST_OUTPUT_ROOT}/TVA/apTvSudouest_20060326_MS_fmsat_ortho_cloudShadowWithAltitude_init_shadow.tif
                     -l2cla ${MAJA_TEST_OUTPUT_ROOT}/TVA/apTvSudouest_20060326_MS_fmsat_ortho_cloudShadowWithAltitude_init_l2cla.tif
                     VALID   --compare-n-images 0.0 2
                             ${MAJADATA_TVA_BASELINE}/apTvSudouest_20060326_MS_fmsat_ortho_cloudShadowWithAltitude_l2cla_init.tif
                             ${MAJA_TEST_OUTPUT_ROOT}/TVA/apTvSudouest_20060326_MS_fmsat_ortho_cloudShadowWithAltitude_init_l2cla.tif
                             ${MAJADATA_TVA_BASELINE}/apTvSudouest_20060326_MS_fmsat_ortho_cloudShadowWithAltitude_shadow_init.tif
                             ${MAJA_TEST_OUTPUT_ROOT}/TVA/apTvSudouest_20060326_MS_fmsat_ortho_cloudShadowWithAltitude_init_shadow.tif

                    )



maja_test_application(NAME apTvCloudShadowWithoutAltitudeFilterNominal
                     APP  CloudShadow
                     OPTIONS
                     -tocr ${MAJADATA_TVA_INPUT}/CloudMaskingTest/Lacrau_20060813_MS_fmsat_ortho_tocr.tif
                     -edg ${MAJADATA_TVA_INPUT}/CloudMaskingTest/Lacrau_20060813_MS_fmsat_ortho_edg.tif
                     -cldall ${MAJADATA_TVA_INPUT}/CloudMaskingTest/Lacrau_20060813_MS_fmsat_ortho_cldall.tif
                     -cla ${MAJADATA_TVA_INPUT}/CloudMaskingTest/Lacrau_20060813_MS_fmsat_ortho_cla.tif
                     -vie ${MAJADATA_TVA_INPUT}/CloudMaskingTest/Lacrau_20060813_MS_fmsat_ortho_vie.tif
                     -dtm ${MAJADATA_TVA_INPUT}/DTM/FORMOSAT/Lacrau_96m.mnt
                     -initmode 0
                     -l2rcr ${MAJADATA_TVA_INPUT}/CloudMaskingTest/Lacrau_20060813_MS_fmsat_ortho_rcr.tif
                     -l2was ${MAJADATA_TVA_INPUT}/CloudMaskingTest/Lacrau_20060813_MS_fmsat_ortho_was.tif
                     -l2ndt ${MAJADATA_TVA_INPUT}/CloudMaskingTest/Lacrau_20060813_MS_fmsat_ortho_ndt.tif
                     -sol1.in ${MAJADATA_TVA_INPUT}/CloudMaskingTest/Lacrau_20060813_MS_fmsat_ortho_sol1.tif
                     -sol1.h 3000
                     -solhref 4000
                     -defaultalt 500
                     -ksigma 0
                     -deltahmax 8000
                     -deltahmin 0
                     -deltahstep 250
                     -shadbandtocr 2
                     -shadbandrcr 2
                     -viehref 3000
                     -nodata -10
                     -algo withoutalt
                     -algo.withoutalt.cloudcovermult 2
                     -algo.withoutalt.shadratiomax 1
                     -algo.withoutalt.shadratiomin 0.5
                     -algo.withoutalt.shadratiopixnb 10
                     -algo.withoutalt.maxdarkening 0.800000000000000044408920985006
                     -algo.withoutalt.minpercentile 0.100000000000000005551115123126
                     -algo.withoutalt.seradius 2
                     -shadow  ${MAJA_TEST_OUTPUT_ROOT}/TVA/apTvLacrau_20060813_MS_fmsat_ortho_cloudShadowWithoutAltitude_nominal_shadow.tif
                     VALID   --compare-image ${EPSILON_5}
                        ${MAJADATA_TVA_BASELINE}/apTvLacrau_20060813_MS_fmsat_ortho_cloudShadowWithoutAltitude_nominal_shadow.tif
                        ${MAJA_TEST_OUTPUT_ROOT}/TVA/apTvLacrau_20060813_MS_fmsat_ortho_cloudShadowWithoutAltitude_nominal_shadow.tif


                   )
