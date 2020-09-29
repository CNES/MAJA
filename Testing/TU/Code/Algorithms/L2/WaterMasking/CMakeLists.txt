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
#                                ooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo         #
#                             o                                                                            #
#                          o                                                                               #
#                        o                                                                                 #
#                      o                                                                                   #
#                     o       ooooooo       ooooooo    o         o      oo                                 #
#    o              o       o        o     o       o   o         o     o   o                               #
#      o           o       o          o   o         o  o         o    o      o                             #
#        o        o       o           o   o         o  o         o    o        o                           #
#         o      o        o      oooo     o         o  o         o   o           o                         #
#          o    o          o              o         o  o         o   o           o                         #
#           o  o            o             o         o  o o      o   o          o                           #
#            oo              oooooooo    o         o   o  oooooo   o      oooo                             #
#                                                     o                                                    #
#                                                     o                                                    #
#                                                    o                            o                        #
#                                                    o            o      oooo     o   o      oooo          #
#                                                   o             o         o    o    o         o          #
#                                                   o            o       ooo     o   o       ooo           #
#                                                               o       o       o   o          o           #
#                                                               ooooo   oooo    o   ooooo  oooo            #
#                                                                              o                           #
#                                                                                                          #
############################################################################################################
#                                                                                                          #
# Author: CS Systemes d'Information  (France)                                                              #
#                                                                                                          #
############################################################################################################
# HISTORIQUE                                                                                               #
#                                                                                                          #
# VERSION : 2-0-0 : DM : LAIG-DM-MAC-146-CNES : 14 octobre 2011 : Modification pour prise en compte        #
#                                                            d'evolutions liees a MACCS                    #
#                                                                                                          #
# VERSION : 1-0-0 : <TypeFT> : <NumFT> : 19 avril 2009 : Creation                                                           
#                                                                                                          #
# FIN-HISTORIQUE                                                                                           #
#                                                                                                          #
# $Id$
#                                                                                                          #
############################################################################################################
IF( NOT MAJA_DISABLE_CXX_TESTING )
SET(PREFIX "AlgorithmsL2Water")


SET(TESTS1 ${CXX_TEST_PATH}/vnsL2WaterTests1)

# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~ vnsL2WaterTests1 ~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

# -------    vnsWatermaskingImageFilter test

# -------   vnsPossibleWaterFlagImageFilterNew.cxx
ADD_TEST(${PREFIX}PossibleWaterFlagImageFilterNew ${TESTS1}
    vnsPossibleWaterFlagImageFilterNew
)

# -------   vnsPossibleWaterFlagImageFilterTest.cxx
ADD_TEST(${PREFIX}PossibleWaterFlagImageFilterTest ${TESTS1}
    --compare-n-images ${NOTOL} 2
    ${BASELINEDATATU_IMAGES}/${PREFIX}PossibleWaterFlagImageFilterTestIPPWA.tif
    ${TEMP}/${PREFIX}PossibleWaterFlagImageFilterTestIPPWA.tif
    ${BASELINEDATATU_IMAGES}/${PREFIX}PossibleWaterFlagImageFilterTestIPTWA.tif
    ${TEMP}/${PREFIX}PossibleWaterFlagImageFilterTestIPTWA.tif
    vnsPossibleWaterFlagImageFilterTest
    ${INPUT_MAJADATATU_IMAGES_SURF_RAY}/Sudouest_20060819_MS_fmsat_ortho_surf_ray_96m.tif
    ${INPUT_MAJADATATU_IMAGES_SURF_RAY_COMP}/Sudouest_20060810_MS_fmsat_ortho_surf_ray_comp_96m.tif
    ${INPUT_MAJADATATU_IMAGES_BORD96}/Sudouest_20060819_MS_fmsat_ortho_toa_bord_96m
    #${INPUT_MAJADATATU_IMAGES_BORD96}/Sudouest_20060810_MS_fmsat_ortho_toa_bord_96m
    ${INPUT_MAJADATATU_IMAGES_NUA}/Sudouest_20060819_MS_fmsat_ortho_toa_nua_96m_IPCLDSum.tif
    ${INPUT_MAJADATATU_IMAGES_CAST_SHW}/image_noire.tif
    ${INPUT_MAJADATATU_IMAGES}/image_slope_sub.tif
    ${TEMP}/${PREFIX}PossibleWaterFlagImageFilterTestIPPWA.tif
    ${TEMP}/${PREFIX}PossibleWaterFlagImageFilterTestIPTWA.tif
    0
    1
    3
    4
    0.01
    0.04
    0.12
    0.2  # Water slope threshold
    0.15 # water red reflectance threshold
)

ADD_TEST(${PREFIX}PossibleWaterFlagImageFilterTest2 ${TESTS1}
    --compare-n-images ${NOTOL} 2
    ${BASELINEDATATU_IMAGES}/${PREFIX}PossibleWaterFlagImageFilterTest2IPPWA.tif
    ${TEMP}/${PREFIX}PossibleWaterFlagImageFilterTest2IPPWA.tif
    ${BASELINEDATATU_IMAGES}/${PREFIX}PossibleWaterFlagImageFilterTest2IPTWA.tif
    ${TEMP}/${PREFIX}PossibleWaterFlagImageFilterTest2IPTWA.tif
    vnsPossibleWaterFlagImageFilterTest
    ${INPUT_MAJADATATU_IMAGES_SURF_RAY}/Sudouest_20060819_MS_fmsat_ortho_surf_ray_96m.tif
    ${INPUT_MAJADATATU_IMAGES_SURF_RAY_COMP}/Sudouest_20060810_MS_fmsat_ortho_surf_ray_comp_96m.tif
    ${INPUT_MAJADATATU_IMAGES_BORD96}/Sudouest_20060819_MS_fmsat_ortho_toa_bord_96m
    #${INPUT_MAJADATATU_IMAGES_BORD96}/Sudouest_20060810_MS_fmsat_ortho_toa_bord_96m
    ${INPUT_MAJADATATU_IMAGES_NUA}/Sudouest_20060819_MS_fmsat_ortho_toa_nua_96m_IPCLDSum.tif
    ${INPUT_MAJADATATU_IMAGES_CAST_SHW}/image_noire.tif
    ${INPUT_MAJADATATU_IMAGES}/image_slope_sub.tif
    ${TEMP}/${PREFIX}PossibleWaterFlagImageFilterTest2IPPWA.tif
    ${TEMP}/${PREFIX}PossibleWaterFlagImageFilterTest2IPTWA.tif
    1
    0
    3
    4
    0.01
    0.04
    0.12
    0.2  # Water slope threshold
    0.15 # water red reflectance threshold
)

ADD_TEST(${PREFIX}PossibleWaterFlagImageFilterTest3 ${TESTS1}
    --compare-n-images ${NOTOL} 2
    ${BASELINEDATATU_IMAGES}/${PREFIX}PossibleWaterFlagImageFilterTest3IPPWA.tif
    ${TEMP}/${PREFIX}PossibleWaterFlagImageFilterTest3IPPWA.tif
    ${BASELINEDATATU_IMAGES}/${PREFIX}PossibleWaterFlagImageFilterTest3IPTWA.tif
    ${TEMP}/${PREFIX}PossibleWaterFlagImageFilterTest3IPTWA.tif
    vnsPossibleWaterFlagImageFilterTest
    ${INPUT_MAJADATATU_IMAGES_SURF_RAY}/Sudouest_20060329_MS_fmsat_ortho_surf_ray_96m.tif
    ${INPUT_MAJADATATU_IMAGES_SURF_RAY_COMP}/Sudouest_20060326_MS_fmsat_ortho_surf_ray_comp_96m.tif
    ${INPUT_MAJADATATU_IMAGES_BORD96}/Sudouest_20060329_MS_fmsat_ortho_toa_bord_96m
    #${INPUT_MAJADATATU_IMAGES_BORD96}/Sudouest_20060326_MS_fmsat_ortho_toa_bord_96m
    ${INPUT_MAJADATATU_IMAGES_NUA}/Sudouest_20060329_MS_fmsat_ortho_toa_nua_96m_IPCLDSum.tif
    ${INPUT_MAJADATATU_IMAGES_CAST_SHW}/image_noire.tif
    ${INPUT_MAJADATATU_IMAGES}/image_slope_sub.tif
    ${TEMP}/${PREFIX}PossibleWaterFlagImageFilterTest3IPPWA.tif
    ${TEMP}/${PREFIX}PossibleWaterFlagImageFilterTest3IPTWA.tif
    0
    1
    3
    4
    0.01
    0.04
    0.12
    0.2  # Water slope threshold
    0.15 # water red reflectance threshold
)

# -------   vnsSRTMWaterMaskImproveImageFilterTest.cxx
ADD_TEST(${PREFIX}SRTMWaterMaskImproveImageFilterTest ${TESTS1}
    --compare-n-images ${NOTOL} 3
    ${BASELINEDATATU_IMAGES}/${PREFIX}SRTMWaterMaskImproveImageFilterTestL2WAS.tif
    ${TEMP}/${PREFIX}SRTMWaterMaskImproveImageFilterTestL2WAS.tif
    ${BASELINEDATATU_IMAGES}/${PREFIX}SRTMWaterMaskImproveImageFilterTestL2PWA.tif
    ${TEMP}/${PREFIX}SRTMWaterMaskImproveImageFilterTestL2PWA.tif
    ${BASELINEDATATU_IMAGES}/${PREFIX}SRTMWaterMaskImproveImageFilterTestL2TWA.tif
    ${TEMP}/${PREFIX}SRTMWaterMaskImproveImageFilterTestL2TWA.tif
    vnsSRTMWaterMaskImproveImageFilterTest
    ${INPUT_MAJADATATU_IMAGES_WATER}/${PREFIX}PossibleWaterFlagImageFilterTestIPPWA.tif
    ${INPUT_MAJADATATU_IMAGES_WATER}/${PREFIX}PossibleWaterFlagImageFilterTestIPTWA.tif
    ${INPUT_MAJADATATU_IMAGES_WATER}/Sudouest_20060810_MS_fmsat_ortho_toa_mask_96m.compt
    ${INPUT_MAJADATATU_IMAGES_WATER}/Sudouest_20060810_MS_fmsat_ortho_toa_mask_96m.pos
    ${INPUT_MAJADATATU_IMAGES_SRTMWAT}/Sudouest_96m.eau
    ${TEMP}/${PREFIX}SRTMWaterMaskImproveImageFilterTestL2WAS.tif
    ${TEMP}/${PREFIX}SRTMWaterMaskImproveImageFilterTestL2PWA.tif
    ${TEMP}/${PREFIX}SRTMWaterMaskImproveImageFilterTestL2TWA.tif
    0
    25
    50
    10
)

ADD_TEST(${PREFIX}SRTMWaterMaskImproveImageFilterInitTest ${TESTS1}
    --compare-n-images ${NOTOL} 3
    ${BASELINEDATATU_IMAGES}/${PREFIX}SRTMWaterMaskImproveImageFilterInitTestL2WAS.tif
    ${TEMP}/${PREFIX}SRTMWaterMaskImproveImageFilterInitTestL2WAS.tif
    ${BASELINEDATATU_IMAGES}/${PREFIX}SRTMWaterMaskImproveImageFilterInitTestL2PWA.tif
    ${TEMP}/${PREFIX}SRTMWaterMaskImproveImageFilterInitTestL2PWA.tif
    ${BASELINEDATATU_IMAGES}/${PREFIX}SRTMWaterMaskImproveImageFilterInitTestL2TWA.tif
    ${TEMP}/${PREFIX}SRTMWaterMaskImproveImageFilterInitTestL2TWA.tif
    vnsSRTMWaterMaskImproveImageFilterTest
    ${INPUT_MAJADATATU_IMAGES_WATER}/${PREFIX}PossibleWaterFlagImageFilterTest2IPPWA.tif
    ${INPUT_MAJADATATU_IMAGES_WATER}/${PREFIX}PossibleWaterFlagImageFilterTest2IPTWA.tif
    ${INPUT_MAJADATATU_IMAGES_WATER}/Sudouest_20060810_MS_fmsat_ortho_toa_mask_96m.compt
    ${INPUT_MAJADATATU_IMAGES_WATER}/Sudouest_20060810_MS_fmsat_ortho_toa_mask_96m.pos
    ${INPUT_MAJADATATU_IMAGES_SRTMWAT}/Sudouest_96m.eau
    ${TEMP}/${PREFIX}SRTMWaterMaskImproveImageFilterInitTestL2WAS.tif
    ${TEMP}/${PREFIX}SRTMWaterMaskImproveImageFilterInitTestL2PWA.tif
    ${TEMP}/${PREFIX}SRTMWaterMaskImproveImageFilterInitTestL2TWA.tif
    1
    25
    50
    10
)



ADD_TEST(${PREFIX}SRTMWaterMaskImproveImageFilterTest2 ${TESTS1}
    --compare-n-images ${NOTOL} 3
    ${BASELINEDATATU_IMAGES}/${PREFIX}SRTMWaterMaskImproveImageFilterTest2L2WAS.tif
    ${TEMP}/${PREFIX}SRTMWaterMaskImproveImageFilterTest2L2WAS.tif
    ${BASELINEDATATU_IMAGES}/${PREFIX}SRTMWaterMaskImproveImageFilterTest2L2PWA.tif
    ${TEMP}/${PREFIX}SRTMWaterMaskImproveImageFilterTest2L2PWA.tif
    ${BASELINEDATATU_IMAGES}/${PREFIX}SRTMWaterMaskImproveImageFilterTest2L2TWA.tif
    ${TEMP}/${PREFIX}SRTMWaterMaskImproveImageFilterTest2L2TWA.tif
    vnsSRTMWaterMaskImproveImageFilterTest
    ${INPUT_MAJADATATU_IMAGES_WATER}/${PREFIX}PossibleWaterFlagImageFilterTest3IPPWA.tif
    ${INPUT_MAJADATATU_IMAGES_WATER}/${PREFIX}PossibleWaterFlagImageFilterTest3IPTWA.tif
    ${INPUT_MAJADATATU_IMAGES_WATER}/Sudouest_20060326_MS_fmsat_ortho_toa_mask_96m.compt
    ${INPUT_MAJADATATU_IMAGES_WATER}/Sudouest_20060326_MS_fmsat_ortho_toa_mask_96m.pos
    ${INPUT_MAJADATATU_IMAGES_SRTMWAT}/Sudouest_96m.eau
    ${TEMP}/${PREFIX}SRTMWaterMaskImproveImageFilterTest2L2WAS.tif
    ${TEMP}/${PREFIX}SRTMWaterMaskImproveImageFilterTest2L2PWA.tif
    ${TEMP}/${PREFIX}SRTMWaterMaskImproveImageFilterTest2L2TWA.tif
    0
    25
    50
    10
)

# Src1 sources files
SET(SRCS1
vnsL2WaterTests1.cxx
vnsPossibleWaterFlagImageFilterNew.cxx
vnsPossibleWaterFlagImageFilterTest.cxx
vnsSRTMWaterMaskImproveImageFilterNew.cxx
vnsSRTMWaterMaskImproveImageFilterTest.cxx
)


ADD_EXECUTABLE(vnsL2WaterTests1 ${SRCS1})
TARGET_LINK_LIBRARIES(vnsL2WaterTests1 ${OTB_LIBRARIES}
${MAJA_LIBRARY_PREFIXNAME}DataIO 
${MAJA_LIBRARY_PREFIXNAME}DataCommon 
${MAJA_LIBRARY_PREFIXNAME}AlgorithmsCommon
${MAJA_LIBRARY_PREFIXNAME}TestHelperLibrary )



ENDIF( NOT MAJA_DISABLE_CXX_TESTING )
