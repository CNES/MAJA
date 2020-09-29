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

maja_test_application(NAME apTvSnowMask
                     APP  SnowMask
                     OPTIONS -tocr ${MAJADATA_TVA_INPUT}/SnowMaskTest/L930662_20020212_L7_198_030_USGS_tocr.tif
                             -edg ${MAJADATA_TVA_INPUT}/SnowMaskTest/L930662_20020212_L7_198_030_USGS_edg.tif
                             -angles ${MAJADATA_TVA_INPUT}/SnowMaskTest/L930662_20020212_L7_198_030_USGS_angle_image.tif
                             -refband 1
                             -absband 4
                             -redband 2
                             -seradius 2
                             -ndsithresh 0.4
                             -redthresh 0.12
                             -swirthresh 0.16
                             -thetas 61.48641010000001
                             -mincos 0.20
                             -mask ${MAJA_TEST_OUTPUT_ROOT}/TVA/apTvL930662_20020212_L7_198_030_USGS_snow_mask.tif

                     VALID   --compare-image 0.0
                             ${MAJADATA_TVA_BASELINE}/apTvL930662_20020212_L7_198_030_USGS_snow_mask.tif
                             ${MAJA_TEST_OUTPUT_ROOT}/TVA/apTvL930662_20020212_L7_198_030_USGS_snow_mask.tif

                    )

maja_test_application(NAME apTvSnowMaskVenus
                     APP  SnowMask
                     OPTIONS -tocr ${MAJADATA_TVA_INPUT}/SnowMaskVenusTest/vns_caching_RayleighCorrectionIPTOCR0.tif
                             -edg ${MAJADATA_TVA_INPUT}/SnowMaskVenusTest/vns_caching_SubSamplingIPEDG0.tif
                             -refband 9
                             -absband 10
                             -redband 6
                             -seradius 2
                             -ndsithresh 0.0
                             -redthresh 0.12
                             -mask ${MAJA_TEST_OUTPUT_ROOT}/TVA/apTva_snow_mask_venus.tif

#                     VALID   --compare-image 0.0
#                             ${MAJADATA_TVA_BASELINE}/apTvL930662_20020212_L7_198_030_USGS_snow_mask.tif
#                             ${MAJA_TEST_OUTPUT_ROOT}/TVA/apTvL930662_20020212_L7_198_030_USGS_snow_mask.tif
#
                    )   
                 
