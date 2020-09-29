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


maja_test_application(NAME apTvBandMath
                     APP BandMath
                     APP  BandMath
                     OPTIONS -il ${MAJADATA_TVA_INPUT}/OTBApplications/poupees_sub_c1.png
                                  ${MAJADATA_TVA_INPUT}/OTBApplications/poupees_sub_c2.png
                                  ${MAJADATA_TVA_INPUT}/OTBApplications/poupees_sub.png
                             -out ${MAJA_TEST_OUTPUT_ROOT}/TVA/apTvBandMath.tif
                             -exp "cos(im1b1)+im2b1*im3b1-im3b2+ndvi(im3b3,im3b4)"
                     VALID   --compare-image ${NOTOL}
                             ${MAJADATA_TVA_BASELINE}/apTvBandMath.tif
                             ${MAJA_TEST_OUTPUT_ROOT}/TVA/apTvBandMath.tif
                    )


maja_test_application(NAME apTvExtractROI
                     APP ExtractROI
                     OPTIONS -in ${MAJADATA_TVA_INPUT}/CompositeTest/apTvSudouest_20060326_toa.tif
                             -out ${MAJA_TEST_OUTPUT_ROOT}/TVA/apTvExtractROI.tif
                             -cl Channel2
                     VALID   --compare-image ${NOTOL}
                             ${MAJADATA_TVA_BASELINE}/apTvExtractROI.tif
                             ${MAJA_TEST_OUTPUT_ROOT}/TVA/apTvExtractROI.tif
                    )


maja_test_application(NAME apTvManageNoData
                     APP ManageNoData
                     OPTIONS -in ${MAJADATA_TVA_INPUT}/OTBApplications/apTvManageNoData_l2toa.tif
                             -mode apply
                             -mode.apply.mask ${MAJADATA_TVA_INPUT}/OTBApplications/apTvManageNoData_edg.tif
                             -mode.apply.ndval -10
                             -out ${MAJA_TEST_OUTPUT_ROOT}/TVA/apTvManageNoData.tif
                     VALID   --compare-image ${NOTOL}
                             ${MAJADATA_TVA_BASELINE}/apTv_ManageNoData.tif
                             ${MAJA_TEST_OUTPUT_ROOT}/TVA/apTvManageNoData.tif
                    )


maja_test_application(NAME apTvBinaryMorphologicalOperation
                     APP BinaryMorphologicalOperation
                     OPTIONS -in ${MAJADATA_TVA_INPUT}/OTBApplications/QB_Toulouse_Ortho_PAN_Mask.tif
                             -channel 1
                             -structype ball
                             -xradius 10
                             -filter opening
                             -filter.opening.foreval 1.0
                             -filter.opening.backval 0.0
                             -out ${MAJA_TEST_OUTPUT_ROOT}/TVA/apTvBinaryMorphologicalOperation.tif
                     VALID   --compare-image ${NOTOL}
                   			 ${MAJADATA_TVA_BASELINE}/apTvBinaryMorphologicalOperation.tif
                 		     ${MAJA_TEST_OUTPUT_ROOT}/TVA/apTvBinaryMorphologicalOperation.tif
                     )




maja_test_application(NAME apTvConvertCartoToGeoPoint
                     APP ConvertCartoToGeoPoint
                     OPTIONS -carto.x 374100.828
                             -carto.y 4829184.806
                 	         -mapproj utm
                 	         -mapproj.utm.northhem true
                 	         -mapproj.utm.zone 31
                 	 TESTENVOPTIONS ${MAJA_TEST_OUTPUT_ROOT}/TVA/apTvConvertCartoToGeoPoint.txt
                     VALID   --compare-ascii ${NOTOL}
                             ${MAJADATA_TVA_BASELINE}/apTvConvertCartoToGeoPoint.txt
                 		     ${MAJA_TEST_OUTPUT_ROOT}/TVA/apTvConvertCartoToGeoPoint.txt
                    )




maja_test_application(NAME apTvConcatenateImages
                     APP ConcatenateImages
                     OPTIONS -il ${MAJADATA_TVA_INPUT}/OTBApplications/poupees_sub_c1.png
                                 ${MAJADATA_TVA_INPUT}/OTBApplications/poupees_sub_c2.png
                                 ${MAJADATA_TVA_INPUT}/OTBApplications/poupees_sub.png
                              -out ${MAJA_TEST_OUTPUT_ROOT}/TVA/apTvConcatenateImages.tif
                     VALID   --compare-image ${NOTOL}
                            ${MAJADATA_TVA_BASELINE}/apTvConcatenateImages.tif
                 		    ${MAJA_TEST_OUTPUT_ROOT}/TVA/apTvConcatenateImages.tif
                    )

