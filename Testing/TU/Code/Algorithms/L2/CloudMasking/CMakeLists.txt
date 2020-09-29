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
# VERSION : 1-0-0 : <TypeFT> : <NumFT> : 23 avril 2009 : Creation                                                           
#                                                                                                          #
# FIN-HISTORIQUE                                                                                           #
#                                                                                                          #
# $Id$
#                                                                                                          #
############################################################################################################
IF( NOT MAJA_DISABLE_CXX_TESTING )
SET(PREFIX "AlgorithmsL2Cloud")


SET(TESTS1 ${CXX_TEST_PATH}/vnsL2CloudTests1)
SET(TESTS2 ${CXX_TEST_PATH}/vnsL2CloudTests2)


# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ vnsL2CloudTests1 ~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

# -------   vnsReflectanceThresholdFunctorTest.cxx
ADD_TEST(${PREFIX}ReflectanceThresholdFunctorTest1 ${TESTS1}
    --compare-image ${NOTOL}
    ${BASELINEDATATU_IMAGES}/${PREFIX}ReflectanceThresholdFunctorTestCLDRefl1.tif
    ${TEMP}/${PREFIX}ReflectanceThresholdFunctorTestCLDRefl1.tif
    vnsReflectanceThresholdFunctorTest
    ${INPUT_MAJADATATU_IMAGES}/Romania_Extract_3x3.tif
    ${INPUT_MAJADATATU_IMAGES}/mask_3x3_9.tif
    ${INPUT_MAJADATATU_IMAGES}/mask_B02.tif
    ${INPUT_MAJADATATU_IMAGES}/mask_B04.tif
    ${TEMP}/${PREFIX}ReflectanceThresholdFunctorTestCLDRefl1.tif
    1 # blue band
    3 # water band
    2 # red band index
    2 # nir band index
    2 # swir band index
    false # has swir band ?
    255 # foreground value (water mask = 1)
    59. # reflectance threshold
    0. # water reflectance threshold
    1000  # red reflectance threshold
    2  # nirred ratio max threshold
    0.8  # nirred ratio min threshold
    1  # nirswir ratio min threshold
    )
    
ADD_TEST(${PREFIX}ReflectanceThresholdFunctorTest2 ${TESTS1}
    --compare-image ${NOTOL}
    ${BASELINEDATATU_IMAGES}/${PREFIX}ReflectanceThresholdFunctorTestCLDRefl2.tif
    ${TEMP}/${PREFIX}ReflectanceThresholdFunctorTestCLDRefl2.tif
    vnsReflectanceThresholdFunctorTest
    ${INPUT_MAJADATATU_IMAGES}/Romania_Extract_3x3.tif
    ${INPUT_MAJADATATU_IMAGES}/mask_3x3_9.tif
    ${INPUT_MAJADATATU_IMAGES}/mask_B02.tif
    ${INPUT_MAJADATATU_IMAGES}/mask_B04.tif
    ${TEMP}/${PREFIX}ReflectanceThresholdFunctorTestCLDRefl2.tif
    3 # blue band
    3 # water band
    2 # red band index
    2 # nir band index
    2 # swir band index
    false # has swir band ?
    255 # foreground value (water mask = 1)
    108. # reflectance threshold
    0. # water reflectance threshold
    1000  # red reflectance threshold
    2  # nirred ratio max threshold
    0.8  # nirred ratio min threshold
    1  # nirswir ratio min threshold
    )    

ADD_TEST(${PREFIX}ReflectanceThresholdFunctorTest3 ${TESTS1}
    --compare-image ${NOTOL}
    ${BASELINEDATATU_IMAGES}/${PREFIX}ReflectanceThresholdFunctorTestCLDRefl3.tif
    ${TEMP}/${PREFIX}ReflectanceThresholdFunctorTestCLDRefl3.tif
    vnsReflectanceThresholdFunctorTest
    ${INPUT_MAJADATATU_IMAGES_SURF_RAY}/Sudouest_20060329_MS_fmsat_ortho_surf_ray_96m.tif
    ${INPUT_MAJADATATU_IMAGES_MSQ}/Sudouest_20060329_MS_fmsat_ortho_toa_sat_96m
    ${INPUT_MAJADATATU_IMAGES_BORD96}/Sudouest_20060329_MS_fmsat_ortho_toa_bord_96m
    ${INPUT_MAJADATATU_IMAGES_WATER}/Sudouest_20060329_MS_fmsat_ortho_toa_96m.water
    ${TEMP}/${PREFIX}ReflectanceThresholdFunctorTestCLDRefl3.tif
    1 # blue band
    3 # water band
    2 # red band index
    2 # nir band index
    2 # swir band index
    false # has swir band ?
    1 # foreground value (water mask = 1)
    250 # reflectance threshold
    1000  # water reflectance threshold
    1000  # red reflectance threshold
    2  # nirred ratio max threshold
    0.8  # nirred ratio min threshold
    1  # nirswir ratio min threshold
    )   
    
ADD_TEST(${PREFIX}ReflectanceThresholdFunctorTest4 ${TESTS1}
    --compare-image ${NOTOL}
    ${BASELINEDATATU_IMAGES}/${PREFIX}ReflectanceThresholdFunctorTestCLDRefl4.tif
    ${TEMP}/${PREFIX}ReflectanceThresholdFunctorTestCLDRefl4.tif
    vnsReflectanceThresholdFunctorTest
    ${INPUT_MAJADATATU_IMAGES_SURF_RAY}/Sudouest_20060819_MS_fmsat_ortho_surf_ray_96m.tif
    ${INPUT_MAJADATATU_IMAGES_MSQ}/Sudouest_20060819_MS_fmsat_ortho_toa_sat_96m
    ${INPUT_MAJADATATU_IMAGES_BORD96}/Sudouest_20060819_MS_fmsat_ortho_toa_bord_96m
    ${INPUT_MAJADATATU_IMAGES_WATER}/Sudouest_20060819_MS_fmsat_ortho_toa_96m.water
    ${TEMP}/${PREFIX}ReflectanceThresholdFunctorTestCLDRefl4.tif
    1 # blue band
    3 # water band
    2 # red band index
    2 # nir band index
    2 # swir band index
    false # has swir band ?
    1 # foreground value (water mask = 1)
    250  # reflectance threshold
    1000. # water reflectance threshold
    1000  # red reflectance threshold
    2  # nirred ratio max threshold
    0.8  # nirred ratio min threshold
    1  # nirswir ratio min threshold
    )   
    
# ------- ReflectanceThresholdFunctorWaterTest.cxx    
ADD_TEST(${PREFIX}ReflectanceThresholdFunctorWaterTest ${TESTS1}
    --compare-image ${NOTOL}
    ${BASELINEDATATU_IMAGES}/${PREFIX}ReflectanceThresholdFunctorWaterTest.tif
    ${TEMP}/${PREFIX}ReflectanceThresholdFunctorWaterTest.tif
    vnsReflectanceThresholdFunctorTest
    ${INPUT_MAJADATATU_IMAGES}/Romania_Extract_3x3.tif
    ${INPUT_MAJADATATU_IMAGES}/mask_B03.tif
    ${INPUT_MAJADATATU_IMAGES}/mask_EDG.tif
    ${INPUT_MAJADATATU_IMAGES}/mask_CLD.tif
    ${TEMP}/${PREFIX}ReflectanceThresholdFunctorWaterTest.tif
    1  # blue band
    3  # water band
    2 # red band index
    2 # nir band index
    2 # swir band index
    false # has swir band ?
    255  # foreground value (water mask = 1)
    100    # reflectance threshold  
    80 # water reflectance threshold
    1000  # red reflectance threshold
    2  # nirred ratio max threshold
    0.8  # nirred ratio min threshold
    1  # nirswir ratio min threshold
    )           
    
# -------   vnsReflectanceVariationCorrelationImageFilterNew.cxx
ADD_TEST(${PREFIX}ReflectanceVariationCorrelationImageFilterNew ${TESTS1}
    vnsReflectanceVariationCorrelationImageFilterNew
    )

# -------   vnsCrossCorrelationFunctorCompileTest.cxx
ADD_TEST(${PREFIX}CrossCorrelationFunctorCompileTest ${TESTS1}
    vnsCrossCorrelationFunctorCompileTest
    ) 


#ADD_TEST(${PREFIX}CorrelChangeDetectionTest ${TESTS1}
#    otbCorrelChangeDetectionTest
#    ${INPUT_MAJADATATU_IMAGES_SURF_RAY}/Sudouest_20060329_MS_fmsat_ortho_surf_ray_96m.B4
#    ${INPUT_MAJADATATU_IMAGES_SURF_RAY_COMP}/Sudouest_20060326_MS_fmsat_ortho_surf_ray_comp_96m.B4
#    ${INPUT_MAJADATATU_IMAGES_SURF_RAY_COMP}/Sudouest_20060326_MS_fmsat_ortho_surf_ray_comp_96m.B4
#    3
#    ${TEMP}/${PREFIX}CrossCorrelationImageTest.tif
#    )  

# -------   vnsReflectanceVariationCorrelationImageFilterTest.cxx
ADD_TEST(${PREFIX}ReflectanceVariationCorrelationImageFilterTest ${TESTS1}
    --compare-n-images ${NOTOL} 2
    ${BASELINEDATATU_IMAGES}/${PREFIX}ReflectanceVariationCorrelationImageFilterTestCLDReflVar.tif
    ${TEMP}/${PREFIX}ReflectanceVariationCorrelationImageFilterTestCLDReflVar.tif
    ${BASELINEDATATU_IMAGES}/${PREFIX}ReflectanceVariationCorrelationImageFilterTestCLDExt.tif
    ${TEMP}/${PREFIX}ReflectanceVariationCorrelationImageFilterTestCLDExt.tif
    vnsReflectanceVariationCorrelationImageFilterTest
    ${INPUT_MAJADATATU_IMAGES_SURF_RAY}/Sudouest_20060329_MS_fmsat_ortho_surf_ray_96m.tif
    ${INPUT_MAJADATATU_IMAGES_SURF_RAY_COMP}/Sudouest_20060326_MS_fmsat_ortho_surf_ray_comp_96m.tif
    ${INPUT_MAJADATATU_IMAGES_PILE}/Sudouest_20060326_MS_fmsat_ortho_toa_96m.compt
    ${INPUT_MAJADATATU_IMAGES_DATE}/Sudouest_20060326_MS_fmsat_ortho_toa_96m.date
    ${INPUT_MAJADATATU_IMAGES_BORD96}/Sudouest_20060329_MS_fmsat_ortho_toa_bord_96m
    ${INPUT_MAJADATATU_IMAGES_BORD96}/Sudouest_20060326_MS_fmsat_ortho_toa_comp_bord_NIR.tif
    ${INPUT_MAJADATATU_IMAGES_WATER}/Sudouest_20060326_MS_fmsat_ortho_toa_96m.water
    ${BASELINEDATATU_IMAGES}/${PREFIX}ReflectanceThresholdFunctorTestCLDRefl3.tif
    ${TEMP}/${PREFIX}ReflectanceVariationCorrelationImageFilterTestCLDReflVar.tif
    ${TEMP}/${PREFIX}ReflectanceVariationCorrelationImageFilterTestCLDExt.tif
    -10
    1
    3
    4
    2006
    3
    29
    30
    0.030
    1.
    1.0
    1.5
    0.005
    15
    4
    90
    0
    )  
      
ADD_TEST(${PREFIX}ReflectanceVariationCorrelationImageFilterTest2 ${TESTS1}
    --compare-n-images ${NOTOL} 2
    ${BASELINEDATATU_IMAGES}/${PREFIX}ReflectanceVariationCorrelationImageFilterTest2CLDReflVar.tif
    ${TEMP}/${PREFIX}ReflectanceVariationCorrelationImageFilterTest2CLDReflVar.tif
    ${BASELINEDATATU_IMAGES}/${PREFIX}ReflectanceVariationCorrelationImageFilterTest2CLDExt.tif
    ${TEMP}/${PREFIX}ReflectanceVariationCorrelationImageFilterTest2CLDExt.tif
    vnsReflectanceVariationCorrelationImageFilterTest
    ${INPUT_MAJADATATU_IMAGES_SURF_RAY}/Sudouest_20060819_MS_fmsat_ortho_surf_ray_96m.tif
    ${INPUT_MAJADATATU_IMAGES_SURF_RAY_COMP}/Sudouest_20060810_MS_fmsat_ortho_surf_ray_comp_96m.tif
    ${INPUT_MAJADATATU_IMAGES_PILE}/Sudouest_20060810_MS_fmsat_ortho_toa_96m.compt
    ${INPUT_MAJADATATU_IMAGES_DATE}/Sudouest_20060810_MS_fmsat_ortho_toa_96m.date
    ${INPUT_MAJADATATU_IMAGES_BORD96}/Sudouest_20060819_MS_fmsat_ortho_toa_bord_96m
    ${INPUT_MAJADATATU_IMAGES_BORD96}/Sudouest_20060810_MS_fmsat_ortho_toa_comp_bord
    ${INPUT_MAJADATATU_IMAGES_WATER}/Sudouest_20060810_MS_fmsat_ortho_toa_96m.water
    ${BASELINEDATATU_IMAGES}/${PREFIX}ReflectanceThresholdFunctorTestCLDRefl4.tif
    ${TEMP}/${PREFIX}ReflectanceVariationCorrelationImageFilterTest2CLDReflVar.tif
    ${TEMP}/${PREFIX}ReflectanceVariationCorrelationImageFilterTest2CLDExt.tif
    -10      # No Data
    1        # blue
    3        # Red
    4        # NIR
    2006     # date
    8
    19
    30       # Forgetting duration
    0.030    # Min Var Threshold
    1.       # Max Var Threshold
    1.0      # KNIR blue
    1.5      # KRed blue
    0.03     # delta threshold
    15       # Min pixel correl
    1        # Correl band
    90       # Correl threshold
    0        # radius
    )

# -------   With dilatation of the cloud mask
ADD_TEST(${PREFIX}ReflectanceVariationCorrelationImageFilterDilateTest ${TESTS1}
    --compare-n-images ${NOTOL} 2
#    ${INPUT_MAJADATATU_IMAGES_NUA}/Sudouest_20060329_MS_fmsat_ortho_nua_96m_B2.tif
    ${BASELINEDATATU_IMAGES}/${PREFIX}ReflectanceVariationCorrelationImageFilterDilateTestCLDReflVar.tif
    ${TEMP}/${PREFIX}ReflectanceVariationCorrelationImageFilterDilateTestCLDReflVar.tif
    ${BASELINEDATATU_IMAGES}/${PREFIX}ReflectanceVariationCorrelationImageFilterDilateTestCLDExt.tif
    ${TEMP}/${PREFIX}ReflectanceVariationCorrelationImageFilterDilateTestCLDExt.tif
    vnsReflectanceVariationCorrelationImageFilterTest
    ${INPUT_MAJADATATU_IMAGES_SURF_RAY}/Sudouest_20060329_MS_fmsat_ortho_surf_ray_96m.tif
    ${INPUT_MAJADATATU_IMAGES_SURF_RAY_COMP}/Sudouest_20060326_MS_fmsat_ortho_surf_ray_comp_96m.tif
    ${INPUT_MAJADATATU_IMAGES_PILE}/Sudouest_20060326_MS_fmsat_ortho_toa_96m.compt
    ${INPUT_MAJADATATU_IMAGES_DATE}/Sudouest_20060326_MS_fmsat_ortho_toa_96m.date
    ${INPUT_MAJADATATU_IMAGES_BORD96}/Sudouest_20060329_MS_fmsat_ortho_toa_bord_96m
    ${INPUT_MAJADATATU_IMAGES_BORD96}/Sudouest_20060326_MS_fmsat_ortho_toa_comp_bord_NIR.tif
    ${INPUT_MAJADATATU_IMAGES_WATER}/Sudouest_20060326_MS_fmsat_ortho_toa_96m.water
    ${BASELINEDATATU_IMAGES}/${PREFIX}ReflectanceThresholdFunctorTestCLDRefl3.tif
    ${TEMP}/${PREFIX}ReflectanceVariationCorrelationImageFilterDilateTestCLDReflVar.tif
    ${TEMP}/${PREFIX}ReflectanceVariationCorrelationImageFilterDilateTestCLDExt.tif
    -10
    1
    3
    4
    2006
    3
    29
    30
    0.030    # Min Var Threshold
    1.       # Max Var Threshold
    1.0
    1.5
    0.005
    15
    4
    90
    1
    )


# -------   vnsTernaryLogicalOrFunctorTest.cxx
ADD_TEST(${PREFIX}TernaryLogicalOrFunctorTest ${TESTS1}
    --compare-image ${NOTOL}
    ${BASELINEDATATU_IMAGES}/${PREFIX}TernaryLogicalOrFunctorTest.tif
    ${TEMP}/${PREFIX}TernaryLogicalOrFunctorTest.tif
    vnsTernaryLogicalOrFunctorTest
    ${INPUT_MAJADATATU_IMAGES}/mask_B01.tif
    ${INPUT_MAJADATATU_IMAGES}/mask_B02.tif
    ${INPUT_MAJADATATU_IMAGES}/mask_B03.tif
    ${TEMP}/${PREFIX}TernaryLogicalOrFunctorTest.tif
    ) 
    
ADD_TEST(${PREFIX}TernaryLogicalOrFunctorTest2 ${TESTS1}
    --compare-image ${NOTOL}
    ${BASELINEDATATU_IMAGES}/${PREFIX}TernaryLogicalOrFunctorTestIPCLDAll.tif
    ${TEMP}/${PREFIX}TernaryLogicalOrFunctorTestIPCLDAll.tif
    vnsTernaryLogicalOrFunctorTest
    ${INPUT_MAJADATATU_IMAGES}/F2_IPCLA.tif
    ${BASELINEDATATU_IMAGES}/${PREFIX}ReflectanceThresholdFunctorTestCLDRefl3.tif
    ${BASELINEDATATU_IMAGES}/${PREFIX}ReflectanceVariationCorrelationImageFilterTestCLDReflVar.tif
    ${TEMP}/${PREFIX}TernaryLogicalOrFunctorTestIPCLDAll.tif
    )     
    
    
# -------   vnsCloudMaskUndersamplingImageFilterNew.cxx
ADD_TEST(${PREFIX}StereoCorrectionImageFilterNew ${TESTS1}
    vnsStereoCorrectionImageFilterNew
    )        

# -------   vnsStereoCorrectionImageFilterTest.cxx
ADD_TEST(${PREFIX}StereoCorrectionImageFilterTest ${TESTS1}
    --compare-n-images ${NOTOL} 2
    ${BASELINEDATATU_IMAGES}/${PREFIX}StereoCorrectionImageFilterTestIPCLDAllLabel.tif
    ${TEMP}/${PREFIX}StereoCorrectionImageFilterTestIPCLDAllLabel.tif
    ${BASELINEDATATU_IMAGES}/${PREFIX}StereoCorrectionImageFilterTestIPCLA.tif
    ${TEMP}/${PREFIX}StereoCorrectionImageFilterTestIPCLA.tif
    vnsStereoCorrectionImageFilterTest
    ${INPUT_MAJADATATU_IMAGES}/F2_IPCLA.tif
    ${INPUT_MAJADATATU_IMAGES}/${PREFIX}TernaryLogicalOrFunctorTestIPCLDAll.tif
    ${TEMP}/${PREFIX}StereoCorrectionImageFilterTestIPCLDAllLabel.tif
    ${TEMP}/${PREFIX}StereoCorrectionImageFilterTestIPCLA.tif
    0
    0
    3
    500
    VENUS
    ${TEMP}/
    )


ADD_TEST(${PREFIX}StereoCorrectionImageFilterTest2 ${TESTS1}
    --compare-n-images ${NOTOL} 2
    ${BASELINEDATATU_IMAGES}/${PREFIX}StereoCorrectionImageFilterTest2IPCLDAllLabel.tif
    ${TEMP}/${PREFIX}StereoCorrectionImageFilterTest2IPCLDAllLabel.tif
    ${BASELINEDATATU_IMAGES}/${PREFIX}StereoCorrectionImageFilterTest2IPCLA.tif
    ${TEMP}/${PREFIX}StereoCorrectionImageFilterTest2IPCLA.tif
    vnsStereoCorrectionImageFilterTest
    ${INPUT_MAJADATATU_IMAGES}/F2_IPCLATest.tif
    ${INPUT_MAJADATATU_IMAGES}/${PREFIX}TernaryLogicalOrFunctorTestIPCLDAll.tif
    ${TEMP}/${PREFIX}StereoCorrectionImageFilterTest2IPCLDAllLabel.tif
    ${TEMP}/${PREFIX}StereoCorrectionImageFilterTest2IPCLA.tif
    0
    0
    3
    500
    VENUS
    ${TEMP}/
    )      

ADD_TEST(${PREFIX}StereoAltitudeCorrectionImageFilterNew ${TESTS1}
    vnsStereoAltitudeCorrectionImageFilterNew
    )    

# -------   vnsShadowVariationThresholdImageFilterNew.cxx
ADD_TEST(${PREFIX}ShadowVariationThresholdImageFilterNew ${TESTS1}
    vnsShadowVariationThresholdImageFilterNew
)

# -------   vnsShadowVariationThresholdImageFilterTest.cxx
ADD_TEST(${PREFIX}ShadowVariationThresholdImageFilterTest ${TESTS1}
    --compare-image ${NOTOL}
    ${BASELINEDATATU_IMAGES}/${PREFIX}ShadowVariationThresholdImageFilterTestIPCLDShadVar.tif
    ${TEMP}/${PREFIX}ShadowVariationThresholdImageFilterTestIPCLDShadVar.tif
    vnsShadowVariationThresholdImageFilterTest
    ${INPUT_MAJADATATU_IMAGES_SURF_RAY}/Sudouest_20060329_MS_fmsat_ortho_surf_ray_96m.tif
    ${INPUT_MAJADATATU_IMAGES_SURF_RAY_COMP}/Sudouest_20060326_MS_fmsat_ortho_surf_ray_comp_96m.tif
    ${INPUT_MAJADATATU_IMAGES_PILE}/Sudouest_20060326_MS_fmsat_ortho_toa_96m.compt
    ${INPUT_MAJADATATU_IMAGES_DATE}/Sudouest_20060326_MS_fmsat_ortho_toa_96m.date
    ${INPUT_MAJADATATU_IMAGES_BORD96}/Sudouest_20060329_MS_fmsat_ortho_toa_bord_96m
    ${INPUT_MAJADATATU_IMAGES_BORD96}/Sudouest_20060326_MS_fmsat_ortho_toa_comp_bord.tif
    ${INPUT_MAJADATATU_IMAGES_WATER}/Sudouest_20060326_MS_fmsat_ortho_toa_96m.water
    ${INPUT_MAJADATATU_IMAGES_OMBRE}/Sudouest_20060329_MS_fmsat_ortho_toa_96m.ombre
    ${TEMP}/${PREFIX}ShadowVariationThresholdImageFilterTestIPCLDShadVar.tif
    -10    # No_Data
#    3      # Red band
    2006   # Date
    3
    29
#    30     # Forgetting duration
#    0.66   # Min shadow variation
#    15     # Min pixel correlation
#    4      # Correl band
#    90     # Correl Threshold
    0      # Dilatation option
    )   

ADD_TEST(${PREFIX}ShadowVariationThresholdImageFilterTest2 ${TESTS1}
   --compare-image ${NOTOL}
    ${BASELINEDATATU_IMAGES}/${PREFIX}ShadowVariationThresholdImageFilterTest2IPCLDShadVar.tif
    ${TEMP}/${PREFIX}ShadowVariationThresholdImageFilterTest2IPCLDShadVar.tif
    vnsShadowVariationThresholdImageFilterTest
    ${INPUT_MAJADATATU_IMAGES_SURF_RAY}/Sudouest_20060819_MS_fmsat_ortho_surf_ray_96m.tif
    ${INPUT_MAJADATATU_IMAGES_SURF_RAY_COMP}/Sudouest_20060810_MS_fmsat_ortho_surf_ray_comp_96m.tif
    ${INPUT_MAJADATATU_IMAGES_PILE}/Sudouest_20060810_MS_fmsat_ortho_toa_96m.compt
    ${INPUT_MAJADATATU_IMAGES_DATE}/Sudouest_20060810_MS_fmsat_ortho_toa_96m.date
    ${INPUT_MAJADATATU_IMAGES_BORD96}/Sudouest_20060819_MS_fmsat_ortho_toa_bord_96m
    ${INPUT_MAJADATATU_IMAGES_BORD96}/Sudouest_20060810_MS_fmsat_ortho_toa_comp_bord.hdr
    ${INPUT_MAJADATATU_IMAGES_WATER}/Sudouest_20060810_MS_fmsat_ortho_toa_96m.water
    ${INPUT_MAJADATATU_IMAGES_OMBRE}/Sudouest_20060819_MS_fmsat_ortho_toa_96m.ombre
    ${TEMP}/${PREFIX}ShadowVariationThresholdImageFilterTest2IPCLDShadVar.tif
    -10    # No_Data
#    3      # Red band
    2006   # Date
    8
    19
 #   30     # Forgetting duration
  #  0.66   # Min shadow variation
   # 15     # Min pixel correlation
   # 1      # Correl band
   # 90     # Correl Threshold
    0      # Dilatation option
    )
       
ADD_TEST(${PREFIX}ShadowVariationThresholdImageFilterDilateTest2 ${TESTS1}
   --compare-image ${NOTOL}
    ${BASELINEDATATU_IMAGES}/${PREFIX}ShadowVariationThresholdImageFilterDilateTest2IPCLDShadVar.tif
    ${TEMP}/${PREFIX}ShadowVariationThresholdImageFilterDilateTest2IPCLDShadVar.tif
    vnsShadowVariationThresholdImageFilterTest
    ${INPUT_MAJADATATU_IMAGES_SURF_RAY}/Sudouest_20060819_MS_fmsat_ortho_surf_ray_96m.tif
    ${INPUT_MAJADATATU_IMAGES_SURF_RAY_COMP}/Sudouest_20060810_MS_fmsat_ortho_surf_ray_comp_96m.tif
    ${INPUT_MAJADATATU_IMAGES_PILE}/Sudouest_20060810_MS_fmsat_ortho_toa_96m.compt
    ${INPUT_MAJADATATU_IMAGES_DATE}/Sudouest_20060810_MS_fmsat_ortho_toa_96m.date
    ${INPUT_MAJADATATU_IMAGES_BORD96}/Sudouest_20060819_MS_fmsat_ortho_toa_bord_96m
    ${INPUT_MAJADATATU_IMAGES_BORD96}/Sudouest_20060810_MS_fmsat_ortho_toa_comp_bord.hdr
    ${INPUT_MAJADATATU_IMAGES_WATER}/Sudouest_20060810_MS_fmsat_ortho_toa_96m.water
    ${INPUT_MAJADATATU_IMAGES_OMBRE}/Sudouest_20060819_MS_fmsat_ortho_toa_96m.ombre
    ${TEMP}/${PREFIX}ShadowVariationThresholdImageFilterDilateTest2IPCLDShadVar.tif
    -10
#    3
    2006
    8
    19
 #   10
  #  0.66
   # 15
   # 1
   # 90
    1
    )
    
# -------   vnsConditionalDifferenceImageFilterNew.cxx
ADD_TEST(${PREFIX}ConditionalDifferenceImageFilterNew ${TESTS1}
    vnsConditionalDifferenceImageFilterNew
)

# -------   vnsConditionalDifferenceImageFilterNew.cxx
ADD_TEST(${PREFIX}ConditionalDifferenceImageFilterTest ${TESTS1}
   --compare-image ${NOTOL}
    ${BASELINEDATATU_IMAGES}/${PREFIX}ConditionalDifferenceImageFilterTest.tif
    ${TEMP}/${PREFIX}ConditionalDifferenceImageFilterTest.tif
    vnsConditionalDifferenceImageFilterTest
    ${INPUT_MAJADATATU_IMAGES_SURF_RAY}/Sudouest_20060329_MS_fmsat_ortho_surf_ray_96m.tif
    ${INPUT_MAJADATATU_IMAGES_SURF_RAY_COMP}/Sudouest_20060326_MS_fmsat_ortho_surf_ray_comp_96m.tif
    ${INPUT_MAJADATATU_IMAGES_NUA}/Sudouest_20060329_MS_fmsat_ortho_toa_96m.nua
    ${INPUT_MAJADATATU_IMAGES_BORD96}/Sudouest_20060329_MS_fmsat_ortho_toa_bord_96m
    ${INPUT_MAJADATATU_IMAGES_WATER}/Sudouest_20060326_MS_fmsat_ortho_toa_96m.water
    ${INPUT_MAJADATATU_IMAGES_BORD96}/Sudouest_20060326_MS_fmsat_ortho_toa_comp_bord.tif
    ${TEMP}/${PREFIX}ConditionalDifferenceImageFilterTest.tif
    -10
)

# -------   vnsShadowProjectionImageFilterNew.cxx - 1 
ADD_TEST(${PREFIX}ShadowProjectionImageFilterNew ${TESTS1}
    vnsShadowProjectionImageFilterNew
)

# -------   vnsShadowProjectionImageFilterTest.cxx - 2
ADD_TEST(${PREFIX}ShadowProjectionImageFilterTest ${TESTS1}
   --compare-n-ascii ${NOTOL} 1
    ${BASELINEDATATU_FILES}/${PREFIX}ShadowProjectionImageFilterTestIPCLDShad.txt
    ${TEMP}/${PREFIX}ShadowProjectionImageFilterTestIPCLDShad.txt
    vnsShadowProjectionImageFilterTest
    ${INPUT_MAJADATATU_IMAGES_NUA}/Sudouest_20060329_MS_fmsat_ortho_toa_nua_96m_IPCLDAll.tif
    ${INPUT_MAJADATATU_IMAGES_DTM}/Sudouest_96m.mnt
    ${TEMP}/${PREFIX}ShadowProjectionImageFilterTestIPCLDShad.txt
    3000
    3000
    21.98
    350.58
    45.98
    146.16
    2000
    8
    -8
)

# -------   vnsShadowProjectionImageFilterTest.cxx - 3
ADD_TEST(${PREFIX}ShadowProjectionImageFilterTest2 ${TESTS1}
    --compare-n-ascii ${NOTOL} 1
    ${BASELINEDATATU_FILES}/${PREFIX}ShadowProjectionImageFilterTest2IPCLDShad.txt
    ${TEMP}/${PREFIX}ShadowProjectionImageFilterTest2IPCLDShad.txt
    vnsShadowProjectionImageFilterTest
    ${INPUT_MAJADATATU_IMAGES_NUA}/Sudouest_20060819_MS_fmsat_ortho_toa_nua_96m_IPCLDAll.tif
    ${INPUT_MAJADATATU_IMAGES_DTM}/Sudouest_96m.mnt
    ${TEMP}/${PREFIX}ShadowProjectionImageFilterTest2IPCLDShad.txt
    3000
    3000
    22.488
    348.006
    35.558
    140.318
    2000
    8
    -8
)

# -------   vnsShadowMaskProjectionNew.cxx - 1 
ADD_TEST(${PREFIX}ShadowMaskProjectionNew ${TESTS1}
    vnsShadowMaskProjectionNew
)

# -------   vnsShadowMaskProjectionTest.cxx - 1 
ADD_TEST(${PREFIX}ShadowMaskProjectionTest ${TESTS1}
   --compare-image ${NOTOL}
    ${BASELINEDATATU_IMAGES}/${PREFIX}ShadowMaskProjectionImageFilterTest2IPCLDShad.tif
    ${TEMP}/${PREFIX}ShadowMaskProjectionTest2IPCLDShad.tif
    vnsShadowMaskProjectionTest
    ${INPUT_MAJADATATU_IMAGES}/ShadowMap.txt
    ${TEMP}/${PREFIX}ShadowMaskProjectionTest2IPCLDShad.tif
)

# -------   vnsNoDataShadowImageFilterNew.cxx
ADD_TEST(${PREFIX}NoDataShadowImageFilterNew ${TESTS1}
    vnsNoDataShadowImageFilterNew
)

# -------   vnsNoDataShadowImageFiltertest.cxx
ADD_TEST(${PREFIX}NoDataShadowImageFilterTest ${TESTS1}
    --compare-n-ascii ${NOTOL} 1
    ${BASELINEDATATU_FILES}/${PREFIX}NoDataShadowImageFilterTestMap.txt
    ${TEMP}/${PREFIX}NoDataShadowImageFilterTestMap.txt
    vnsNoDataShadowImageFilterTest
    ${INPUT_MAJADATATU_IMAGES}/ShadowMaskDetDifferenceRefl.tif
    ${INPUT_MAJADATATU_IMAGES}/NoDataShadowMap.txt
    ${TEMP}/${PREFIX}NoDataShadowImageFilterTestMap.txt
)

# -------   vnsCloudLabelUpdatingImageFilterTest.cxx
ADD_TEST(${PREFIX}CloudLabelUpdatingImageFilterTest ${TESTS1}
    --compare-n-images ${NOTOL} 1
    ${BASELINEDATATU_IMAGES}/${PREFIX}CloudLabelUpdatingImageFilterTest.tif
    ${TEMP}/${PREFIX}CloudLabelUpdatingImageFilterTest.tif
    vnsCloudLabelUpdatingImageFilterTest
    ${INPUT_MAJADATATU_IMAGES}/ShadowLabel.tif
    ${INPUT_MAJADATATU_IMAGES}/LabelList.txt
    ${TEMP}/${PREFIX}CloudLabelUpdatingImageFilterTest.tif
)

# -------   vnsCloudLabelRemovalImageFilterTest.cxx
ADD_TEST(${PREFIX}CloudLabelRemovalImageFilterTest ${TESTS1}
    --compare-n-images ${NOTOL} 1
    ${BASELINEDATATU_IMAGES}/${PREFIX}CloudLabelRemovalImageFilterTest.tif
    ${TEMP}/${PREFIX}CloudLabelRemovalImageFilterTest.tif
    vnsCloudLabelRemovalImageFilterTest
    ${INPUT_MAJADATATU_IMAGES}/ShadowLabel.tif
    ${INPUT_MAJADATATU_IMAGES}/LabelList.txt
    ${TEMP}/${PREFIX}CloudLabelRemovalImageFilterTest.tif
)


# -------   vnsSnowMaskValidationByLabelImageFilterNew
ADD_TEST(${PREFIX}SnowMaskValidationByLabelImageFilterNew ${TESTS1}
    vnsSnowMaskValidationByLabelImageFilterNew
)

# -------   vnsSnowMaskValidationByLabelImageFilterTest
ADD_TEST(${PREFIX}SnowMaskValidationByLabelImageFilterTest ${TESTS1}
    vnsSnowMaskValidationByLabelImageFilterTest
    ${INPUT_MAJADATATU_IMAGES}/mask_B03.tif
    ${INPUT_MAJADATATU_IMAGES}/mask_B02.tif
    ${INPUT_MAJADATATU_IMAGES}/mask_B01.tif
    ${INPUT_MAJADATATU_IMAGES}/mask_B03.tif
)

# -------   vnsSnowMaskCorrectionByLabelImageFilterNew
ADD_TEST(${PREFIX}SnowMaskCorrectionByLabelImageFilterNew ${TESTS1}
    vnsSnowMaskCorrectionByLabelImageFilterNew
)

# -------   vnsSnowMaskCorrectionImageFilterNew
ADD_TEST(${PREFIX}SnowMaskCorrectionImageFilterNew ${TESTS1}
    vnsSnowMaskCorrectionImageFilterNew
)

# -------   vnsSnowMaskCorrectionImageFilterTest
ADD_TEST(${PREFIX}SnowMaskCorrectionImageFilterTest ${TESTS1}
    --compare-n-images ${NOTOL} 2
    ${BASELINEDATATU_IMAGES}/${PREFIX}SnowMaskCorrectionImageFilterTestIPCLDReflSub.tif
    ${TEMP}/${PREFIX}SnowMaskCorrectionImageFilterTestIPCLDReflSub.tif
    ${BASELINEDATATU_IMAGES}/${PREFIX}SnowMaskCorrectionImageFilterTestIPSNWSub.tif
    ${TEMP}/${PREFIX}SnowMaskCorrectionImageFilterTestIPSNWSub.tif
    vnsSnowMaskCorrectionImageFilterTest
    ${INPUT_MAJADATATU_IMAGES}/mask_B02.tif
    ${INPUT_MAJADATATU_IMAGES}/mask_B02.tif
    ${INPUT_MAJADATATU_IMAGES}/mask_B03.tif
    ${INPUT_MAJADATATU_IMAGES}/mask_All.tif
    1
    ${TEMP}/${PREFIX}SnowMaskCorrectionImageFilterTestIPCLDReflSub.tif
    ${TEMP}/${PREFIX}SnowMaskCorrectionImageFilterTestIPSNWSub.tif
    LANDSAT_MUSCATE_PROTO
    ${TEMP}/
)


# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ vnsL2CloudTests2 ~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

# -------   vnsSolarDirectionForCloudAltitudeFunctorTest.cxx
ADD_TEST(${PREFIX}SolarDirectionForCloudAltitudeFuntorTest ${TESTS2}
    --compare-image ${NOTOL}
    ${BASELINEDATATU_IMAGES}/${PREFIX}SolarDirectionForCloudAltitudeFunctorTestIPSOL.tif
    ${TEMP}/${PREFIX}SolarDirectionForCloudAltitudeFunctorTestIPSOL.tif
    vnsSolarDirectionForCloudAltitudeFunctorTest
    ${INPUT_MAJADATATU_IMAGES}/F2_IPSOL.tif
    ${INPUT_MAJADATATU_IMAGES}/F2_IPSOL.tif
    ${INPUT_MAJADATATU_IMAGES}/F2_IPCLATest.tif
    ${TEMP}/${PREFIX}SolarDirectionForCloudAltitudeFunctorTestIPSOL.tif
    3000
    8000   
)

# -------   vnsValidShadowMaskImageFilter
ADD_TEST(${PREFIX}ValidShadowMaskImageFilterNew ${TESTS2}
    vnsValidShadowMaskImageFilterNew
)
 
# -------   vnsValidShadowMaskImageFilter
ADD_TEST(${PREFIX}ValidShadowMaskImageFilterTest ${TESTS2}
    --compare-n-ascii ${NOTOL} 1
    ${BASELINEDATATU_FILES}/${PREFIX}ValidShadowMaskImageFilterTest.txt
    ${TEMP}/${PREFIX}ValidShadowMaskImageFilterTest.txt
vnsValidShadowMaskImageFilterTest
    ${INPUT_MAJADATATU_IMAGES}/shadlabel_100_100.png
    ${INPUT_MAJADATATU_IMAGES}/cadran_100_100.png
    ${INPUT_MAJADATATU_IMAGES}/cadran_100_100.png
    ${TEMP}/${PREFIX}ValidShadowMaskImageFilterTest.txt
)

# -------   vnsAltitudeCorrectionFunctorTest.cxx
ADD_TEST(${PREFIX}AltitudeCorrectionFunctorTest ${TESTS2}
    --compare-image ${NOTOL} 
    ${BASELINEDATATU_IMAGES}/${PREFIX}AltitudeCorrectionFunctorTestIPCLA.tif
    ${TEMP}/${PREFIX}AltitudeCorrectionFunctorTestIPCLA.tif
vnsAltitudeCorrectionFunctorTest
    ${INPUT_MAJADATATU_IMAGES}/cadran_100_100.png
    ${INPUT_MAJADATATU_IMAGES}/label_100_100.png
    ${TEMP}/${PREFIX}AltitudeCorrectionFunctorTestIPCLA.tif
)


# -------   vnsCloudAlreadyDetectedImageFilterNew.cxx
ADD_TEST(${PREFIX}CloudAlreadyDetectedImageFilterNew ${TESTS2}
    vnsCloudAlreadyDetectedImageFilterNew
)

# -------   vnsCloudAlreadyDetectedImageFilterTest.cxx
ADD_TEST(${PREFIX}CloudAlreadyDetectedImageFilterTest ${TESTS2}
    --compare-n-ascii ${NOTOL} 1
    ${BASELINEDATATU_FILES}/${PREFIX}CloudAlreadyDetectedImageFilterTest.txt
    ${TEMP}/${PREFIX}CloudAlreadyDetectedImageFilterTest.txt
    vnsCloudAlreadyDetectedImageFilterTest
    ${BASELINEDATATU_IMAGES}/${PREFIX}ReflectanceVariationCorrelationImageFilterTestCLDExt.tif
    ${BASELINEDATATU_IMAGES}/${PREFIX}ReflectanceThresholdFunctorTestCLDRefl3.tif
    ${BASELINEDATATU_IMAGES}/${PREFIX}ReflectanceVariationCorrelationImageFilterTestCLDReflVar.tif
    ${TEMP}/${PREFIX}CloudAlreadyDetectedImageFilterTest.txt
    0
    1
)

ADD_TEST(${PREFIX}CloudAlreadyDetectedImageFilterTest2 ${TESTS2}
    --compare-n-ascii ${NOTOL} 1
    ${BASELINEDATATU_FILES}/${PREFIX}CloudAlreadyDetectedImageFilterTest2.txt
    ${TEMP}/${PREFIX}CloudAlreadyDetectedImageFilterTest2.txt
    vnsCloudAlreadyDetectedImageFilterTest
    ${INPUT_MAJADATATU_IMAGES}/mask_B01.tif
    ${INPUT_MAJADATATU_IMAGES}/mask_B01.tif
    ${INPUT_MAJADATATU_IMAGES}/mask_B02.tif
    ${TEMP}/${PREFIX}CloudAlreadyDetectedImageFilterTest2.txt
    0
    255
)

ADD_TEST(${PREFIX}CloudAlreadyDetectedImageFilterTest3 ${TESTS2}
    --compare-n-ascii ${NOTOL} 1
    ${BASELINEDATATU_FILES}/${PREFIX}CloudAlreadyDetectedImageFilterTest3.txt
    ${TEMP}/${PREFIX}CloudAlreadyDetectedImageFilterTest3.txt
    vnsCloudAlreadyDetectedImageFilterTest
    ${INPUT_MAJADATATU_IMAGES}/mask_B01.tif
    ${INPUT_MAJADATATU_IMAGES}/mask_B04.tif
    ${INPUT_MAJADATATU_IMAGES}/mask_B04.tif
    ${TEMP}/${PREFIX}CloudAlreadyDetectedImageFilterTest3.txt
    0
    255
)


# ------- Test CloudMasking sans IPSOL2 et avec masque Neige et Cirrus.
ADD_TEST(${PREFIX}ValidateMaskFromLabelMapAndValidityMapImageFilterTest ${TESTS2}
   --compare-image ${NOTOL}
        ${BASELINEDATATU_IMAGES}/${PREFIX}ValidateMaskFromLabelMapAndValidityMapImageFilterOutput.tif
        ${TEMP}/${PREFIX}ValidateMaskFromLabelMapAndValidityMapImageFilterOutput.tif
    vnsValidateMaskFromLabelMapAndValidityMapImageFilterTest
        ${INPUT_MAJADATATU_IMAGES}/ValidateMaskFromLabelMapAndValidityMap_Input.tif
        ${INPUT_MAJADATATU_IMAGES}/ValidateMaskFromLabelMapAndValidityMap_InputLabel.tif
        ${TEMP}/${PREFIX}ValidateMaskFromLabelMapAndValidityMapImageFilterOutput.tif
)

# -------   vnsCloudGeomMasksToKeptLabelMasksFilterTest.cxx
ADD_TEST(${PREFIX}CloudGeomMasksToKeptLabelMasksFilterTest ${TESTS2}
   --compare-image ${NOTOL}
        ${BASELINEDATATU_IMAGES}/${PREFIX}CloudGeomMasksToKeptLabelMasksFilterTestOutput.tif
        ${TEMP}/${PREFIX}CloudGeomMasksToKeptLabelMasksFilterTestOutput.tif
    vnsCloudGeomMasksToKeptLabelMasksFilterTest
        ${INPUT_MAJADATATU_IMAGES}/CloudGeomMasksToKeptLabelMasksFilterTest_Input_1.tif
        ${INPUT_MAJADATATU_IMAGES}/CloudGeomMasksToKeptLabelMasksFilterTest_Input_2.tif
        ${INPUT_MAJADATATU_IMAGES}/CloudGeomMasksToKeptLabelMasksFilterTest_Input_3.tif
        ${TEMP}/${PREFIX}CloudGeomMasksToKeptLabelMasksFilterTestOutput.tif
)

# -------   vnsCloudGeomMasksToKeptLabelMasksFilterGeneratedInputs.cxx
ADD_TEST(${PREFIX}CloudGeomMasksToKeptLabelMasksFilterGeneratedInputs ${TESTS2}
    vnsCloudGeomMasksToKeptLabelMasksFilterGeneratedInputs
)

# -------   vnsFlattenCloudMaskImageListFilterGenerateInputs.cxx
ADD_TEST(${PREFIX}FlattenCloudMaskImageListFilterGenerateInputs ${TESTS2}
    vnsFlattenCloudMaskImageListFilterGenerateInputs
)

# -------   vnsFlattenCloudMaskImageListFilter.cxx
ADD_TEST(${PREFIX}FlattenCloudMaskImageListFilterTest ${TESTS2}
   --compare-image ${NOTOL}
   ${BASELINEDATATU_IMAGES}/${PREFIX}FlattenCloudMaskImageListFilterTestOutput.tif
        ${TEMP}/${PREFIX}FlattenCloudMaskImageListFilterTestOutput.tif
        vnsFlattenCloudMaskImageListFilter
        ${TEMP}/${PREFIX}FlattenCloudMaskImageListFilterTestOutput.tif
        ${INPUT_MAJADATATU_IMAGES}/FlattenCloudMaskImageListFilterTest_Input_1.tif
        ${INPUT_MAJADATATU_IMAGES}/FlattenCloudMaskImageListFilterTest_Input_2.tif
        ${INPUT_MAJADATATU_IMAGES}/FlattenCloudMaskImageListFilterTest_Input_3.tif
)

# Src1 sources files
SET(SRCS1
vnsL2CloudTests1.cxx
vnsReflectanceThresholdFunctorTest.cxx
vnsCrossCorrelationFunctorCompileTest.cxx
vnsReflectanceVariationCorrelationImageFilterNew.cxx
vnsReflectanceVariationCorrelationImageFilterTest.cxx
vnsTernaryLogicalOrFunctorTest.cxx
vnsStereoCorrectionImageFilterNew.cxx
vnsStereoCorrectionImageFilterTest.cxx
vnsStereoAltitudeCorrectionImageFilterNew.cxx
vnsShadowVariationThresholdImageFilterNew.cxx
vnsShadowVariationThresholdImageFilterTest.cxx
vnsConditionalDifferenceImageFilterNew.cxx
vnsConditionalDifferenceImageFilterTest.cxx
vnsShadowProjectionImageFilterNew.cxx
vnsShadowProjectionImageFilterTest.cxx
vnsShadowMaskProjectionNew.cxx
vnsShadowMaskProjectionTest.cxx
vnsSnowMaskValidationByLabelImageFilterNew.cxx
vnsSnowMaskValidationByLabelImageFilterTest.cxx
vnsSnowMaskCorrectionByLabelImageFilterNew.cxx
vnsSnowMaskCorrectionImageFilterNew.cxx
vnsSnowMaskCorrectionImageFilterTest.cxx
vnsNoDataShadowImageFilterNew.cxx
vnsNoDataShadowImageFilterTest.cxx
vnsCloudLabelUpdatingImageFilterTest.cxx
vnsCloudLabelRemovalImageFilterTest.cxx
)


# Src2 sources files
SET(SRCS2
vnsL2CloudTests2.cxx
vnsSolarDirectionForCloudAltitudeFunctorTest.cxx
vnsValidShadowMaskImageFilterNew.cxx
vnsValidShadowMaskImageFilterTest.cxx
vnsAltitudeCorrectionFunctorTest.cxx
vnsCloudAlreadyDetectedImageFilterNew.cxx
vnsCloudAlreadyDetectedImageFilterTest.cxx
vnsValidateMaskFromLabelMapAndValidityMapImageFilterTest.cxx
vnsCloudGeomMasksToKeptLabelMasksFilterTest.cxx
vnsCloudGeomMasksToKeptLabelMasksFilterGeneratedInputs.cxx
vnsFlattenCloudMaskImageListFilterGenerateInputs.cxx
vnsFlattenCloudMaskImageListFilter.cxx
)



ADD_EXECUTABLE(vnsL2CloudTests1 ${SRCS1})
TARGET_LINK_LIBRARIES(vnsL2CloudTests1 ${OTB_LIBRARIES}
${MAJA_LIBRARY_PREFIXNAME}DataIO 
${MAJA_LIBRARY_PREFIXNAME}DataCommon 
${MAJA_LIBRARY_PREFIXNAME}AlgorithmsCommon
${MAJA_LIBRARY_PREFIXNAME}UtilitiesI3D
${MAJA_LIBRARY_PREFIXNAME}TestHelperLibrary
${MAJA_LIBRARY_PREFIXNAME}TestingMiscellaneous
${MAJA_IMPORT_LIBRARIES} )


ADD_EXECUTABLE(vnsL2CloudTests2 ${SRCS2})
TARGET_LINK_LIBRARIES(vnsL2CloudTests2 ${OTB_LIBRARIES}
${MAJA_LIBRARY_PREFIXNAME}DataIO 
${MAJA_LIBRARY_PREFIXNAME}DataCommon 
${MAJA_LIBRARY_PREFIXNAME}AlgorithmsCommon
${MAJA_LIBRARY_PREFIXNAME}UtilitiesI3D
${MAJA_LIBRARY_PREFIXNAME}TestHelperLibrary
${MAJA_IMPORT_LIBRARIES} )

ENDIF( NOT MAJA_DISABLE_CXX_TESTING )
