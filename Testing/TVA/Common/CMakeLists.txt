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


maja_test_application(NAME apTvSmoothingBis
                     APP SmoothingBis
                     OPTIONS
                     -in ${MAJADATA_TVA_BASELINE}/apTvGapFilling_filledImage.tif
                     -type gaussian
                     -type.gaussian.radius 2
                     -type.gaussian.maxkernelwidth 8
                     -type.gaussian.maxerror 1e-15
                     -out ${MAJA_TEST_OUTPUT_ROOT}/TVA/apTvSmoothingBis.tif

                     VALID   --compare-image ${EPSILON_5}
                             ${MAJADATA_TVA_BASELINE}/apTvSmoothingBis.tif
                             ${MAJA_TEST_OUTPUT_ROOT}/TVA/apTvSmoothingBis.tif

                    )


maja_test_application(NAME apTvResampling
                     APP Resampling
                     OPTIONS
                     -dtm ${MAJADATA_TVA_INPUT}/DTM/LANDSAT/LN_TEST_AUX_REFDE2_EU93066200A00B_0001.DBL.DIR/LN_TEST_AUX_REFDE2_EU93066200A00B_0001_ALC.tif
                     -im ${MAJADATA_TVA_BASELINE}/apTvSmoothingBis.tif
                     -interp linear
                     -padradius 4
                     -out ${MAJA_TEST_OUTPUT_ROOT}/TVA/apTvResampling.tif

                     VALID   --compare-image ${EPSILON_5}
                             ${MAJADATA_TVA_BASELINE}/apTvResampling.tif
                             ${MAJA_TEST_OUTPUT_ROOT}/TVA/apTvResampling.tif

                    )


maja_test_application(NAME apTvStats
                     APP Stats
                     OPTIONS
                     -im ${MAJADATA_TVA_BASELINE}/apTvGapFilling_filledImage.tif
                     TESTENVOPTIONS ${MAJA_TEST_OUTPUT_ROOT}/TVA/apTvStatsFlag.txt

                     VALID --compare-ascii ${EPSILON_8}
                        ${MAJADATA_TVA_BASELINE}/apTvStatsFlag.txt
                        ${MAJA_TEST_OUTPUT_ROOT}/TVA/apTvStatsFlag.txt

                    )


maja_test_application(NAME apTvMultiplyByScalar
                     APP MultiplyByScalar
                     OPTIONS
                     -im ${MAJADATA_TVA_INPUT}/AthmosphericAbsorption/Sudouest_20060819_MS_fmsat_ortho_toa.tif
                     -coef 1000
                     -out ${MAJA_TEST_OUTPUT_ROOT}/TVA/apTvMultiplyByScalar.tif

                     VALID --compare-image ${EPSILON_5}
                        ${MAJADATA_TVA_BASELINE}/apTvMultiplyByScalar.tif
                        ${MAJA_TEST_OUTPUT_ROOT}/TVA/apTvMultiplyByScalar.tif

                    )


maja_test_application(NAME apTvBinaryConcatenate
                     APP BinaryConcatenate
                     OPTIONS
                     -im ${MAJADATA_TVA_INPUT}/Common/WaterMaskingImageFilter-PWA.tif
                     -out ${MAJA_TEST_OUTPUT_ROOT}/TVA/apTvBinaryConcatenate.tif
                     VALID --compare-image ${EPSILON_5}
                        ${MAJADATA_TVA_BASELINE}/apTvBinaryConcatenate.tif
                        ${MAJA_TEST_OUTPUT_ROOT}/TVA/apTvBinaryConcatenate.tif

                    )

maja_test_application(NAME apTvBinaryToVector
                     APP BinaryToVector
                     OPTIONS
                     -im ${MAJADATA_TVA_INPUT}/Common/L930562_20130905_L8_199_030_USGS_toa_240m.tif
                     -nbcomp 8
                     -out ${MAJA_TEST_OUTPUT_ROOT}/TVA/apTvBinaryToVector.tif
                     VALID --compare-image ${EPSILON_5}
                        ${MAJADATA_TVA_BASELINE}/apTvBinaryToVector.tif
                        ${MAJA_TEST_OUTPUT_ROOT}/TVA/apTvBinaryToVector.tif

                    )


maja_test_application(NAME apTvConcatenatePerZone
                     APP ConcatenatePerZone
                     OPTIONS
                     -mask ${MAJADATA_TVA_INPUT}/Common/S2A_TEST_MSK_GPPL1C_065_01_15SVD_DET_FOOTPRINT_457_457.tif
                     -il ${MAJADATA_TVA_INPUT}/Common/constantAngleInZone4.tif ${MAJADATA_TVA_INPUT}/Common/constantAngleInZone6.tif
                     -zonelist 4 6
                     -out ${MAJA_TEST_OUTPUT_ROOT}/TVA/apTvConcatenatePerZone.tif

                     VALID --compare-image 0.0
                     ${MAJADATA_TVA_BASELINE}/apTvConcatenatePerZone.tif
                     ${MAJA_TEST_OUTPUT_ROOT}/TVA/apTvConcatenatePerZone.tif

                    )
                    
maja_test_application(NAME apTvDispatchZonesToVectorNoWeight
                     APP DispatchZonesToVector
                     OPTIONS
                     -il ${MAJADATA_TVA_INPUT}/Common/SENTINEL2B_20171008-105012-463_L2A_T31TCH_C_V1-0_DTF_R3-D06.tif ${MAJADATA_TVA_INPUT}/Common/SENTINEL2B_20171008-105012-463_L2A_T31TCH_C_V1-0_DTF_R3-D09.tif
                     -outvals 6 9
                     -nbcomp 3
                     -out ${MAJA_TEST_OUTPUT_ROOT}/TVA/apTvDispatchZonesToVectorNoWeight.tif

                     VALID --compare-image 0.0
                     ${MAJADATA_TVA_BASELINE}/apTvDispatchZonesToVectorNoWeight.tif
                     ${MAJA_TEST_OUTPUT_ROOT}/TVA/apTvDispatchZonesToVectorNoWeight.tif

                    )

maja_test_application(NAME apTvDispatchZonesToVectorWeight
                     APP DispatchZonesToVector
                     OPTIONS
                     -il ${MAJADATA_TVA_INPUT}/Common/SENTINEL2B_20171008-105012-463_L2A_T31TCH_C_V1-0_DTF_R3-D06.tif ${MAJADATA_TVA_INPUT}/Common/SENTINEL2B_20171008-105012-463_L2A_T31TCH_C_V1-0_DTF_R3-D09.tif
                     -outvals 6 9
                     -nbcomp 3
                     -outindexes 1 2 3 0 1 2
                     -out ${MAJA_TEST_OUTPUT_ROOT}/TVA/apTvDispatchZonesToVectorWeight.tif

                     VALID --compare-image 0.0
                     ${MAJADATA_TVA_BASELINE}/apTvDispatchZonesToVectorWeight.tif
                     ${MAJA_TEST_OUTPUT_ROOT}/TVA/apTvDispatchZonesToVectorWeight.tif

                    )

maja_test_application(NAME apTvIncidenceAngle
                     APP IncidenceAngle
                     OPTIONS
                     -demasc ${MAJADATA_TVA_INPUT}/DTM/FORMOSAT/VE_TEST_AUX_REFDE2_SUDOUEST_0001.DBL.DIR/Sudouest_8m_ASP_coef01.tif
                     -demslc ${MAJADATA_TVA_INPUT}/DTM/FORMOSAT/VE_TEST_AUX_REFDE2_SUDOUEST_0001.DBL.DIR/Sudouest_8m_SLP_coef01.tif
                     -thetas 45.975222101108
                     -phis 146.15637691771684
                     -angles ${MAJA_TEST_OUTPUT_ROOT}/TVA/apTvIncidenceAngle.tif

                     VALID --compare-image ${EPSILON_4}
                     ${MAJADATA_TVA_BASELINE}/apTvIncidenceAngle.tif
                     ${MAJA_TEST_OUTPUT_ROOT}/TVA/apTvIncidenceAngle.tif

                    )


maja_test_application(NAME apTvOzoneExtractor
                     APP OzoneExtractor
                     OPTIONS
                     -im ${MAJADATA_TVA_INPUT}/Common/METEO_2010030612?&resol=7
                     -lon 180
                     -lat 5
                     TESTENVOPTIONS ${MAJA_TEST_OUTPUT_ROOT}/TVA/apTvOzoneExtractorFlag.txt

                     VALID --compare-ascii ${EPSILON_8}
                        ${MAJADATA_TVA_BASELINE}/apTvOzoneExtractorFlag.txt
                        ${MAJA_TEST_OUTPUT_ROOT}/TVA/apTvOzoneExtractorFlag.txt


                    )


maja_test_application(NAME apTvAngleListToImage
                     APP AngleListToImage
                     OPTIONS
                     -dtm ${MAJADATA_TVA_INPUT}/Common/TOASubImage.tif
                     -angles ${MAJADATA_TVA_INPUT}/Common/singleAngle.xml
                     -out ${MAJA_TEST_OUTPUT_ROOT}/TVA/apTvAngleListToImage.tif

                     VALID --compare-image ${EPSILON_4}
                     ${MAJADATA_TVA_BASELINE}/apTvAngleListToImage.tif
                     ${MAJA_TEST_OUTPUT_ROOT}/TVA/apTvAngleListToImage.tif

                    )


maja_test_application(NAME apTvSolarAngle
                     APP SolarAngle
                     OPTIONS
                     -sol1.in ${MAJADATA_TVA_INPUT}/Common/VENUS_CAMARGUE_SOL1.tif
                     -sol1.h 3000
                     -sol2.in ${MAJADATA_TVA_INPUT}/Common/VENUS_CAMARGUE_SOL2.tif
                     -sol2.h 8000
                     -solhref 1000
                     -sol ${MAJA_TEST_OUTPUT_ROOT}/TVA/apTvSolarAngle.tif

                     VALID --compare-image ${EPSILON_4}
                     ${MAJADATA_TVA_BASELINE}/apTvSolarAngle.tif
                     ${MAJA_TEST_OUTPUT_ROOT}/TVA/apTvSolarAngle.tif


                    )


maja_test_application(NAME apTvImageListWriter
                     APP ImageListWriter
                     OPTIONS
                     -il ${MAJADATA_TVA_INPUT}/Common/GomaAvant.tif ${MAJADATA_TVA_INPUT}/Common/GomaApres.tif
                     -filenames ${MAJA_TEST_OUTPUT_ROOT}/TVA/apTvMultiImageFileWriter1.tif ${MAJA_TEST_OUTPUT_ROOT}/TVA/apTvMultiImageFileWriter2.tif
                     -nblinesperstrip 50
                     VALID --compare-n-images ${EPSILON_4} 2
                     ${MAJADATA_TVA_INPUT}/Common/GomaAvant.tif
                     ${MAJA_TEST_OUTPUT_ROOT}/TVA/apTvMultiImageFileWriter1.tif
                     ${MAJADATA_TVA_INPUT}/Common/GomaApres.tif
                     ${MAJA_TEST_OUTPUT_ROOT}/TVA/apTvMultiImageFileWriter2.tif
                   )



maja_test_application(NAME apTvRoundImage
                     APP RoundImage
                     OPTIONS
                     -im ${MAJADATA_TVA_INPUT}/Common/TOASubImage.tif?&bands=1
                     -out ${MAJA_TEST_OUTPUT_ROOT}/TVA/apTvRoundImage.tif int16
                     VALID --compare-image ${NOTOL}
                     ${MAJADATA_TVA_BASELINE}/apTvRoundImage.tif
                     ${MAJA_TEST_OUTPUT_ROOT}/TVA/apTvRoundImage.tif
                    )


maja_test_application(NAME apTvRoundVectorImage
                     APP RoundImage
                     OPTIONS
                     -im ${MAJADATA_TVA_INPUT}/Common/TOASubImage.tif?&bands=1,2
                     -out ${MAJA_TEST_OUTPUT_ROOT}/TVA/apTvRoundVectorImage.tif uint8
                     VALID --compare-image ${NOTOL}
                     ${MAJADATA_TVA_BASELINE}/apTvRoundVectorImage.tif
                     ${MAJA_TEST_OUTPUT_ROOT}/TVA/apTvRoundVectorImage.tif
                    )


maja_test_application(NAME apTvRescaleIntensity
                     APP RescaleIntensity
                     OPTIONS
                     -in ${MAJADATA_TVA_INPUT}/Common/TOASubImage.tif?&bands=1
                     -out ${MAJA_TEST_OUTPUT_ROOT}/TVA/apTvRescaleIntensity.tif uint8
                     -outmin 25
                     -outmax 245
                     VALID --compare-image ${NOTOL}
                     ${MAJADATA_TVA_BASELINE}/apTvRescaleIntensity.tif
                     ${MAJA_TEST_OUTPUT_ROOT}/TVA/apTvRescaleIntensity.tif
                    )


maja_test_application(NAME apTvOneBandEqualThreshold
                     APP OneBandEqualThreshold
                     OPTIONS
                     -im ${MAJADATA_TVA_INPUT}/Common/TOASubImage.tif
                     -out ${MAJA_TEST_OUTPUT_ROOT}/TVA/apTvOneBandEqualThreshold.tif uint8
                     -thresholdvalue 0
                     -equalvalue 255
                     -outsidevalue 0
                     VALID --compare-image ${NOTOL}
                     ${MAJADATA_TVA_BASELINE}/apTvOneBandEqualThreshold.tif
                     ${MAJA_TEST_OUTPUT_ROOT}/TVA/apTvOneBandEqualThreshold.tif
                    )

