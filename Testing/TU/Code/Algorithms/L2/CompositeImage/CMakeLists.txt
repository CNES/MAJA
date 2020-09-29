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
# VERSION : 1-0-0 : <TypeFT> : <NumFT> : 01 avril 2009 : Creation                                                           
#                                                                                                          #
# FIN-HISTORIQUE                                                                                           #
#                                                                                                          #
# $Id$
#                                                                                                          #
############################################################################################################
IF( NOT MAJA_DISABLE_CXX_TESTING )
SET(PREFIX "AlgorithmsL2Composite")


SET(TESTS1 ${CXX_TEST_PATH}/vnsL2CompositeTests1)

# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ vnsL2CompositeTests1 ~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


ADD_TEST(${PREFIX}HighAOTFunctorTest ${TESTS1}
    --compare-image ${NOTOL}
    ${BASELINEDATATU_IMAGES}/${PREFIX}HighAOTFunctorTestHighAOT.tif
    ${TEMP}/${PREFIX}HighAOTFunctorTestHighAOT.tif
    vnsHighAOTFunctorTest
    ${INPUT_MAJADATATU_IMAGES_TAU2S}/Maroc_20060324_MS_fmsat_ortho_toa_96m.tau2s
    ${INPUT_MAJADATATU_IMAGES_NUA}/Maroc_20060324_MS_fmsat_ortho_toa_nua_96m_IPCLDSum.tif
    ${TEMP}/${PREFIX}HighAOTFunctorTestHighAOT.tif
    1
    0.05
    0.02
    )

# -------   vnsBinaryLogicalOrFunctorTest.cxx
ADD_TEST(${PREFIX}BinaryLogicalOrFunctorTest ${TESTS1}
    --compare-image ${NOTOL}
    ${BASELINEDATATU_IMAGES}/${PREFIX}BinaryLogicalOrFunctorTest.tif
    ${TEMP}/${PREFIX}BinaryLogicalOrFunctorTest.tif
    vnsBinaryLogicalOrFunctorTest
    ${INPUT_MAJADATATU_IMAGES}/mask_B01.tif
    ${INPUT_MAJADATATU_IMAGES}/mask_B02.tif
    ${TEMP}/${PREFIX}BinaryLogicalOrFunctorTest.tif
    ) 
    
#--------------------- vnsHighAOTImageFilterNew.cxx     
ADD_TEST(${PREFIX}HighAOTImageFilterNew ${TESTS1}
    vnsHighAOTImageFilterNew
    )

#--------------------- vnsHighAOTImageFilterTest.cxx     
ADD_TEST(${PREFIX}HighAOTImageFilterTest ${TESTS1}
    --compare-image ${NOTOL}
    ${BASELINEDATATU_IMAGES}/${PREFIX}HighAOTImageFilterTest.tif
    ${TEMP}/${PREFIX}HighAOTImageFilterTest.tif
    vnsHighAOTImageFilterTest
    ${INPUT_MAJADATATU_IMAGES_TAU2S}/Maroc_20060324_MS_fmsat_ortho_toa_96m.tau2s
    ${INPUT_MAJADATATU_IMAGES_BORD96}/Maroc_20060324_MS_fmsat_ortho_toa_bord_96m
    ${INPUT_MAJADATATU_IMAGES_NUA}/Maroc_20060324_MS_fmsat_ortho_toa_nua_96m_IPCLDSum.tif
    ${TEMP}/${PREFIX}HighAOTImageFilterTest.tif
    1
    0.1
    0.04343
    0.04486
    ) 
    
#----------------------------- vnsCompositeImageUpdateImageFilterNew.cxx
 ADD_TEST(${PREFIX}CompositeImageUpdateImageFilterNew ${TESTS1}
    vnsCompositeImageUpdateImageFilterNew
    )    
 
#----------------------------- vnsCompositeImageUpdateImageFilterTest.cxx
 ADD_TEST(${PREFIX}CompositeImageUpdateImageFilterInitTest ${TESTS1}
    --without-threads
    --compare-n-images ${NOTOL} 6
    ${BASELINEDATATU_IMAGES}/${PREFIX}CompositeImageUpdateImageFilterInitTestL2RTA.tif
    ${TEMP}/${PREFIX}CompositeImageUpdateImageFilterInitTestL2RTA.tif
    ${BASELINEDATATU_IMAGES}/${PREFIX}CompositeImageUpdateImageFilterInitTestL2RCR.tif
    ${TEMP}/${PREFIX}CompositeImageUpdateImageFilterInitTestL2RCR.tif
    ${BASELINEDATATU_IMAGES}/${PREFIX}CompositeImageUpdateImageFilterInitTestL2RTC.tif
    ${TEMP}/${PREFIX}CompositeImageUpdateImageFilterInitTestL2RTC.tif
    ${BASELINEDATATU_IMAGES}/${PREFIX}CompositeImageUpdateImageFilterInitTestL2STO.tif
    ${TEMP}/${PREFIX}CompositeImageUpdateImageFilterInitTestL2STO.tif 
    ${BASELINEDATATU_IMAGES}/${PREFIX}CompositeImageUpdateImageFilterInitTestL2PXD.tif
    ${TEMP}/${PREFIX}CompositeImageUpdateImageFilterInitTestL2PXD.tif
    ${BASELINEDATATU_IMAGES}/${PREFIX}CompositeImageUpdateImageFilterInitTestL2NDT.tif
    ${TEMP}/${PREFIX}CompositeImageUpdateImageFilterInitTestL2NDT.tif
    vnsCompositeImageUpdateImageFilterTest
    1
    ${INPUT_MAJADATATU_IMAGES_TOA_ABS}/Maroc_20060324_MS_fmsat_ortho_toa_96m.tif
    ${INPUT_MAJADATATU_IMAGES_SURF_RAY}/Maroc_20060324_MS_fmsat_ortho_surf_ray_96m.tif
    ${INPUT_MAJADATATU_IMAGES_SURF}/Maroc_20060324_MS_fmsat_ortho_surf_96m.tif
    ${INPUT_MAJADATATU_IMAGES_BORD96}/Maroc_20060324_MS_fmsat_ortho_toa_bord_96m
    ${INPUT_MAJADATATU_IMAGES_NUA}/Maroc_20060324_MS_fmsat_ortho_toa_nua_96m_IPCLDSum.tif
    ${INPUT_MAJADATATU_IMAGES_MSQ}/Maroc_20060324_MS_fmsat_ortho_toa_sat_96m
    ${INPUT_MAJADATATU_IMAGES_HIGHAOT}/HighAOTImage.tif
    ${TEMP}/${PREFIX}CompositeImageUpdateImageFilterInitTestL2RTA.tif
    ${TEMP}/${PREFIX}CompositeImageUpdateImageFilterInitTestL2RCR.tif
    ${TEMP}/${PREFIX}CompositeImageUpdateImageFilterInitTestL2RTC.tif
    ${TEMP}/${PREFIX}CompositeImageUpdateImageFilterInitTestL2STO.tif
    ${TEMP}/${PREFIX}CompositeImageUpdateImageFilterInitTestL2PXD.tif
    ${TEMP}/${PREFIX}CompositeImageUpdateImageFilterInitTestL2NDT.tif
    -10
    -10000
    0.5
    60
    0
    0
    0
    2006
    3
    24
    0.043
    10
    1
    )  

 ADD_TEST(${PREFIX}CompositeImageUpdateImageFilterInitFlagTest ${TESTS1}
     --without-threads
    --compare-n-images ${NOTOL} 6
    ${BASELINEDATATU_IMAGES}/${PREFIX}CompositeImageUpdateImageFilterInitFlagTestL2RTA.tif
    ${TEMP}/${PREFIX}CompositeImageUpdateImageFilterInitFlagTestL2RTA.tif
    ${BASELINEDATATU_IMAGES}/${PREFIX}CompositeImageUpdateImageFilterInitFlagTestL2RCR.tif
    ${TEMP}/${PREFIX}CompositeImageUpdateImageFilterInitFlagTestL2RCR.tif
    ${BASELINEDATATU_IMAGES}/${PREFIX}CompositeImageUpdateImageFilterInitFlagTestL2RTC.tif
    ${TEMP}/${PREFIX}CompositeImageUpdateImageFilterInitFlagTestL2RTC.tif
    ${BASELINEDATATU_IMAGES}/${PREFIX}CompositeImageUpdateImageFilterInitFlagTestL2STO.tif
    ${TEMP}/${PREFIX}CompositeImageUpdateImageFilterInitFlagTestL2STO.tif 
    ${BASELINEDATATU_IMAGES}/${PREFIX}CompositeImageUpdateImageFilterInitFlagTestL2PXD.tif
    ${TEMP}/${PREFIX}CompositeImageUpdateImageFilterInitFlagTestL2PXD.tif
    ${BASELINEDATATU_IMAGES}/${PREFIX}CompositeImageUpdateImageFilterInitFlagTestL2NDT.tif
    ${TEMP}/${PREFIX}CompositeImageUpdateImageFilterInitFlagTestL2NDT.tif
    vnsCompositeImageUpdateImageFilterTest
    1
    ${INPUT_MAJADATATU_IMAGES_TOA_ABS}/Maroc_20060324_MS_fmsat_ortho_toa_96m.tif
    ${INPUT_MAJADATATU_IMAGES_SURF_RAY}/Maroc_20060324_MS_fmsat_ortho_surf_ray_96m.tif
    ${INPUT_MAJADATATU_IMAGES_SURF}/Maroc_20060324_MS_fmsat_ortho_surf_96m.tif
    ${INPUT_MAJADATATU_IMAGES_BORD96}/Maroc_20060324_MS_fmsat_ortho_toa_bord_96m
    ${INPUT_MAJADATATU_IMAGES_NUA}/Maroc_20060324_MS_fmsat_ortho_toa_nua_96m_IPCLDSum.tif
    ${INPUT_MAJADATATU_IMAGES_MSQ}/Maroc_20060324_MS_fmsat_ortho_toa_sat_96m
    ${INPUT_MAJADATATU_IMAGES_HIGHAOT}/HighAOTImage.tif
    ${TEMP}/${PREFIX}CompositeImageUpdateImageFilterInitFlagTestL2RTA.tif
    ${TEMP}/${PREFIX}CompositeImageUpdateImageFilterInitFlagTestL2RCR.tif
    ${TEMP}/${PREFIX}CompositeImageUpdateImageFilterInitFlagTestL2RTC.tif
    ${TEMP}/${PREFIX}CompositeImageUpdateImageFilterInitFlagTestL2STO.tif
    ${TEMP}/${PREFIX}CompositeImageUpdateImageFilterInitFlagTestL2PXD.tif
    ${TEMP}/${PREFIX}CompositeImageUpdateImageFilterInitFlagTestL2NDT.tif
    -10
    -10000
    0.5
    60
    1
    0
    0
    2006
    3
    24
    0.043
    10
    1
    )
    
 ADD_TEST(${PREFIX}CompositeImageUpdateImageFilterTest ${TESTS1}
    --without-threads
    --compare-n-images ${NOTOL} 6
#    ${INPUT_MAJADATATU_IMAGES_TOA_COMP}/Maroc_20060324_MS_fmsat_ortho_toa_comp_96m.tif
    ${BASELINEDATATU_IMAGES}/${PREFIX}CompositeImageUpdateImageFilterTestL2RTA.tif
    ${TEMP}/${PREFIX}CompositeImageUpdateImageFilterTestL2RTA.tif
#    ${INPUT_MAJADATATU_IMAGES_SURF_RAY_COMP}/Maroc_20060324_MS_fmsat_ortho_surf_ray_comp_96m.tif
    ${BASELINEDATATU_IMAGES}/${PREFIX}CompositeImageUpdateImageFilterTestL2RCR.tif
    ${TEMP}/${PREFIX}CompositeImageUpdateImageFilterTestL2RCR.tif
#    ${INPUT_MAJADATATU_IMAGES_SURF_COMP}/Maroc_20060324_MS_fmsat_ortho_surf_comp_96m.tif
    ${BASELINEDATATU_IMAGES}/${PREFIX}CompositeImageUpdateImageFilterTestL2RTC.tif
    ${TEMP}/${PREFIX}CompositeImageUpdateImageFilterTestL2RTC.tif
#    ${INPUT_MAJADATATU_IMAGES_PILE}/Maroc_20060324_MS_fmsat_ortho_toa_96m.nua
    ${BASELINEDATATU_IMAGES}/${PREFIX}CompositeImageUpdateImageFilterTestL2STO.tif
    ${TEMP}/${PREFIX}CompositeImageUpdateImageFilterTestL2STO.tif 
#    ${INPUT_MAJADATATU_IMAGES_DATE}/Maroc_20060324_MS_fmsat_ortho_toa_96m.date
    ${BASELINEDATATU_IMAGES}/${PREFIX}CompositeImageUpdateImageFilterTestL2PXD.tif
    ${TEMP}/${PREFIX}CompositeImageUpdateImageFilterTestL2PXD.tif
#    ${INPUT_MAJADATATU_IMAGES_BORD96}/Maroc_20060324_MS_fmsat_ortho_toa_comp_bord.tif
    ${BASELINEDATATU_IMAGES}/${PREFIX}CompositeImageUpdateImageFilterTestL2NDT.tif
    ${TEMP}/${PREFIX}CompositeImageUpdateImageFilterTestL2NDT.tif  
    vnsCompositeImageUpdateImageFilterTest
    0            # init mode    
    ${INPUT_MAJADATATU_IMAGES_TOA_ABS}/Maroc_20060324_MS_fmsat_ortho_toa_96m.tif
    ${INPUT_MAJADATATU_IMAGES_SURF_RAY}/Maroc_20060324_MS_fmsat_ortho_surf_ray_96m.tif
    ${INPUT_MAJADATATU_IMAGES_SURF}/Maroc_20060324_MS_fmsat_ortho_surf_96m.tif
    ${INPUT_MAJADATATU_IMAGES_BORD96}/Maroc_20060324_MS_fmsat_ortho_toa_bord_96m
    ${INPUT_MAJADATATU_IMAGES_NUA}/Maroc_20060324_MS_fmsat_ortho_toa_nua_96m_IPCLDSum.tif
    ${INPUT_MAJADATATU_IMAGES_MSQ}/Maroc_20060324_MS_fmsat_ortho_toa_sat_96m
    ${INPUT_MAJADATATU_IMAGES_HIGHAOT}/HighAOTImage.tif
    ${TEMP}/${PREFIX}CompositeImageUpdateImageFilterTestL2RTA.tif
    ${TEMP}/${PREFIX}CompositeImageUpdateImageFilterTestL2RCR.tif
    ${TEMP}/${PREFIX}CompositeImageUpdateImageFilterTestL2RTC.tif
    ${TEMP}/${PREFIX}CompositeImageUpdateImageFilterTestL2STO.tif
    ${TEMP}/${PREFIX}CompositeImageUpdateImageFilterTestL2PXD.tif
    ${TEMP}/${PREFIX}CompositeImageUpdateImageFilterTestL2NDT.tif
    -10          # real No_data
    -10000       # No_data
    0.5          # Max AOT
    60           # Max age
    0            # Hotspot flag
    0            # Cirrus flag
    0            # Rain flag
    2006         # current date
    3
    24
    0.043        # AOT mean
    10           # STO band number
    1            # STO band index
    ${INPUT_MAJADATATU_IMAGES_TOA_COMP}/Maroc_20060322_MS_fmsat_ortho_toa_comp_96m.tif
    ${INPUT_MAJADATATU_IMAGES_SURF_RAY_COMP}/Maroc_20060322_MS_fmsat_ortho_surf_ray_comp_96m.tif
    ${INPUT_MAJADATATU_IMAGES_SURF_COMP}/Maroc_20060322_MS_fmsat_ortho_surf_comp_96m.tif
    ${INPUT_MAJADATATU_IMAGES_PILE}/Maroc_20060322_MS_fmsat_ortho_toa_96m.nua
    ${INPUT_MAJADATATU_IMAGES_DATE}/Maroc_20060322_MS_fmsat_ortho_toa_96m.date
    ${INPUT_MAJADATATU_IMAGES_BORD96}/Maroc_20060322_MS_fmsat_ortho_toa_comp_bord.tif
    )
    
 ADD_TEST(${PREFIX}CompositeImageUpdateImageFilterFlagTest ${TESTS1}
     --without-threads
    --compare-n-images ${NOTOL} 6
    ${BASELINEDATATU_IMAGES}/${PREFIX}CompositeImageUpdateImageFilterFlagTestL2RTA.tif
    ${TEMP}/${PREFIX}CompositeImageUpdateImageFilterFlagTestL2RTA.tif
    ${BASELINEDATATU_IMAGES}/${PREFIX}CompositeImageUpdateImageFilterFlagTestL2RCR.tif
    ${TEMP}/${PREFIX}CompositeImageUpdateImageFilterFlagTestL2RCR.tif
    ${BASELINEDATATU_IMAGES}/${PREFIX}CompositeImageUpdateImageFilterFlagTestL2RTC.tif
    ${TEMP}/${PREFIX}CompositeImageUpdateImageFilterFlagTestL2RTC.tif
    ${BASELINEDATATU_IMAGES}/${PREFIX}CompositeImageUpdateImageFilterFlagTestL2STO.tif
    ${TEMP}/${PREFIX}CompositeImageUpdateImageFilterFlagTestL2STO.tif 
    ${BASELINEDATATU_IMAGES}/${PREFIX}CompositeImageUpdateImageFilterFlagTestL2PXD.tif
    ${TEMP}/${PREFIX}CompositeImageUpdateImageFilterFlagTestL2PXD.tif
    ${BASELINEDATATU_IMAGES}/${PREFIX}CompositeImageUpdateImageFilterFlagTestL2NDT.tif
    ${TEMP}/${PREFIX}CompositeImageUpdateImageFilterFlagTestL2NDT.tif
    vnsCompositeImageUpdateImageFilterTest
    0            # init mode    
    ${INPUT_MAJADATATU_IMAGES_TOA_ABS}/Maroc_20060324_MS_fmsat_ortho_toa_96m.tif
    ${INPUT_MAJADATATU_IMAGES_SURF_RAY}/Maroc_20060324_MS_fmsat_ortho_surf_ray_96m.tif
    ${INPUT_MAJADATATU_IMAGES_SURF}/Maroc_20060324_MS_fmsat_ortho_surf_96m.tif
    ${INPUT_MAJADATATU_IMAGES_BORD96}/Maroc_20060324_MS_fmsat_ortho_toa_bord_96m
    ${INPUT_MAJADATATU_IMAGES_NUA}/Maroc_20060324_MS_fmsat_ortho_toa_nua_96m_IPCLDSum.tif
    ${INPUT_MAJADATATU_IMAGES_MSQ}/Maroc_20060324_MS_fmsat_ortho_toa_sat_96m
    ${INPUT_MAJADATATU_IMAGES_HIGHAOT}/HighAOTImage.tif
    ${TEMP}/${PREFIX}CompositeImageUpdateImageFilterFlagTestL2RTA.tif
    ${TEMP}/${PREFIX}CompositeImageUpdateImageFilterFlagTestL2RCR.tif
    ${TEMP}/${PREFIX}CompositeImageUpdateImageFilterFlagTestL2RTC.tif
    ${TEMP}/${PREFIX}CompositeImageUpdateImageFilterFlagTestL2STO.tif
    ${TEMP}/${PREFIX}CompositeImageUpdateImageFilterFlagTestL2PXD.tif
    ${TEMP}/${PREFIX}CompositeImageUpdateImageFilterFlagTestL2NDT.tif
    -10          # Real No_data
    -10000       # No_data
    0.5          # Max AOT
    60           # Max age
    0            # Hotspot flag
    1            # Cirrus flag
    0            # Rain flag
    2006         # current date
    3
    24
    0.043        # AOT mean
    10           # STO band number
    1            # STO band index
    ${INPUT_MAJADATATU_IMAGES_TOA_COMP}/Maroc_20060322_MS_fmsat_ortho_toa_comp_96m.tif
    ${INPUT_MAJADATATU_IMAGES_SURF_RAY_COMP}/Maroc_20060322_MS_fmsat_ortho_surf_ray_comp_96m.tif
    ${INPUT_MAJADATATU_IMAGES_SURF_COMP}/Maroc_20060322_MS_fmsat_ortho_surf_comp_96m.tif
    ${INPUT_MAJADATATU_IMAGES_PILE}/Maroc_20060322_MS_fmsat_ortho_toa_96m.nua
    ${INPUT_MAJADATATU_IMAGES_DATE}/Maroc_20060322_MS_fmsat_ortho_toa_96m.date
    ${INPUT_MAJADATATU_IMAGES_BORD96}/Maroc_20060322_MS_fmsat_ortho_toa_comp_bord.tif
    )      

   
# Src1 sources files
SET(SRCS1
vnsL2CompositeTests1.cxx
vnsHighAOTFunctorTest.cxx
vnsBinaryLogicalOrFunctorTest.cxx
vnsHighAOTImageFilterNew.cxx
vnsHighAOTImageFilterTest.cxx
vnsCompositeImageUpdateImageFilterNew.cxx
vnsCompositeImageUpdateImageFilterTest.cxx
)


ADD_EXECUTABLE(vnsL2CompositeTests1 ${SRCS1})
TARGET_LINK_LIBRARIES(vnsL2CompositeTests1 ${OTB_LIBRARIES} 
${MAJA_LIBRARY_PREFIXNAME}DataIO 
${MAJA_LIBRARY_PREFIXNAME}DataCommon 
${MAJA_LIBRARY_PREFIXNAME}TestHelperLibrary )



ENDIF( NOT MAJA_DISABLE_CXX_TESTING )

