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
# VERSION : 1-0-0 : <TypeFT> : <NumFT> : 15 novembre 2009 : Creation                                                           
#                                                                                                          #
# FIN-HISTORIQUE                                                                                           #
#                                                                                                          #
# $Id$
#                                                                                                          #
############################################################################################################


IF( NOT MAJA_DISABLE_CXX_TESTING )
SET(PREFIX "AlgorithmsCommon")

SET(TESTS1 ${CXX_TEST_PATH}/vnsAlgorithmsCommonTests1)
SET(TESTS2 ${CXX_TEST_PATH}/vnsAlgorithmsCommonTests2)

# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ vnsAlgorithmsCommonTests1 ~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

# -------    vnsProcessingModeTest
ADD_TEST(${PREFIX}ProcessingModeTest ${TESTS1}
    vnsProcessingModeTest
)

# -------    vnsConfigCommonParametersNew.cxx
ADD_TEST(${PREFIX}vnsConfigCommonParametersNew ${TESTS1}
    vnsConfigCommonParametersNew
)

# -------    vnsStreamingConditionalStatisticsVectorImageFilterNew.cxx
ADD_TEST(${PREFIX}StreamingConditionalStatisticsVectorImageFilterNew ${TESTS1}
    vnsStreamingConditionalStatisticsVectorImageFilterNew
    )    

# -------    vnsStreamingConditionalStatisticsVectorImageFilterTest.cxx
ADD_TEST(${PREFIX}StreamingConditionalStatisticsVectorImageFilterTest ${TESTS1}
   --compare-n-ascii ${NOTOL} 1
    ${BASELINEDATATU_FILES}/${PREFIX}StreamingConditionalStatisticsVectorImageFilterTest.txt
    ${TEMP}/${PREFIX}StreamingConditionalStatisticsVectorImageFilterTest.txt
    vnsStreamingConditionalStatisticsVectorImageFilterTest
    ${INPUT_MAJADATATU_IMAGES}/Romania_Extract_3x3.tif
    ${TEMP}/${PREFIX}StreamingConditionalStatisticsVectorImageFilterTest.txt
    113
    )

# -------    vnsStreamingConditionalStatisticsVectorImageFilterTest.cxx
ADD_TEST(${PREFIX}StreamingConditionalStatisticsImageFilterTest01 ${TESTS1}
   --compare-n-ascii ${NOTOL} 1
    ${BASELINEDATATU_FILES}/${PREFIX}StreamingConditionalStatisticsImageFilterTest01.txt
    ${TEMP}/${PREFIX}StreamingConditionalStatisticsImageFilterTest01.txt
    vnsStreamingConditionalStatisticsImageFilterTest
    ${INPUT_MAJADATATU_IMAGES}/Romania_Extract_3x3_OneBand.tif
    ${TEMP}/${PREFIX}StreamingConditionalStatisticsImageFilterTest01.txt
    100
    )

ADD_TEST(${PREFIX}StreamingConditionalStatisticsImageFilterTest02 ${TESTS1}
   --compare-n-ascii ${NOTOL} 1
    ${BASELINEDATATU_FILES}/${PREFIX}StreamingConditionalStatisticsImageFilterTest02.txt
    ${TEMP}/${PREFIX}StreamingConditionalStatisticsImageFilterTest02.txt
    vnsStreamingConditionalStatisticsImageFilterTest
    ${INPUT_MAJADATATU_IMAGES}/Romania_Extract_3x3_OneBand.tif
    ${TEMP}/${PREFIX}StreamingConditionalStatisticsImageFilterTest02.txt
    102
    )

ADD_TEST(${PREFIX}StreamingConditionalStatisticsImageFilterTestWithMask01 ${TESTS1}
   --compare-n-ascii ${NOTOL} 1
        ${BASELINEDATATU_FILES}/${PREFIX}StreamingConditionalStatisticsImageFilterTestWithMask01.txt
        ${TEMP}/${PREFIX}StreamingConditionalStatisticsImageFilterTestWithMask01.txt
    vnsStreamingConditionalStatisticsImageFilterTest
    ${INPUT_MAJADATATU_IMAGES}/mask_CLD.tif
    ${INPUT_MAJADATATU_IMAGES}/mask_3x3_oneband.tif
    ${TEMP}/${PREFIX}StreamingConditionalStatisticsImageFilterTestWithMask01.txt
    8 # foreground value
)
ADD_TEST(${PREFIX}StreamingConditionalStatisticsImageFilterTestWithMask02 ${TESTS1}
   --compare-n-ascii ${NOTOL} 1
        ${BASELINEDATATU_FILES}/${PREFIX}StreamingConditionalStatisticsImageFilterTestWithMask02.txt
        ${TEMP}/${PREFIX}StreamingConditionalStatisticsImageFilterTestWithMask02.txt
    vnsStreamingConditionalStatisticsImageFilterTest
    ${INPUT_MAJADATATU_IMAGES}/mask_CLD.tif
    ${INPUT_MAJADATATU_IMAGES}/mask_3x3_oneband.tif
    ${TEMP}/${PREFIX}StreamingConditionalStatisticsImageFilterTestWithMask02.txt
    0 # foreground value
)
ADD_TEST(${PREFIX}StreamingConditionalStatisticsImageFilterTestWithMask03 ${TESTS1}
   --compare-n-ascii ${NOTOL} 1
        ${BASELINEDATATU_FILES}/${PREFIX}StreamingConditionalStatisticsImageFilterTestWithMask03.txt
        ${TEMP}/${PREFIX}StreamingConditionalStatisticsImageFilterTestWithMask03.txt
    vnsStreamingConditionalStatisticsImageFilterTest
    ${INPUT_MAJADATATU_IMAGES}/mask_CLD.tif
    ${INPUT_MAJADATATU_IMAGES}/mask_3x3_oneband.tif
    ${TEMP}/${PREFIX}StreamingConditionalStatisticsImageFilterTestWithMask03.txt
    255 # foreground value
)


# -------    vnsStreamingMaskConditionalStatisticsVectorImageFilterNew.cxx
ADD_TEST(${PREFIX}StreamingMaskConditionalStatisticsVectorImageFilterNew ${TESTS1}
    vnsStreamingMaskConditionalStatisticsVectorImageFilterNew
    )    

# -------    vnsStreamingMaskConditionalStatisticsVectorImageFilterPipelineTest.cxx
ADD_TEST(${PREFIX}StreamingMaskConditionalStatisticsVectorImageFilterTest ${TESTS1}
    vnsStreamingMaskConditionalStatisticsVectorImageFilterTest
    ${INPUT_MAJADATATU_IMAGES}/ResizeVIEFilter_band_id0_detector_id8_0.tif
    ${INPUT_MAJADATATU_IMAGES}/IPZoneMaskSubReaders_band_id0_0.tif
    8 # foreground value
    ${TEMP}/${PREFIX}StreamingMaskConditionalStatisticsVectorImageFilterTest_THOMAS.txt
    )

# -------    vnsStreamingMaskConditionalStatisticsVectorImageFilterPipelineTest.cxx
ADD_TEST(${PREFIX}StreamingMaskConditionalStatisticsVectorImageFilterPipelineTest ${TESTS1}
   --compare-n-ascii ${NOTOL} 1
    ${BASELINEDATATU_FILES}/${PREFIX}StreamingMaskConditionalStatisticsVectorImageFilterTest.txt
    ${TEMP}/${PREFIX}StreamingMaskConditionalStatisticsVectorImageFilterTest.txt
    vnsStreamingMaskConditionalStatisticsVectorImageFilterPipelineTest
    ${INPUT_MAJADATATU_IMAGES}/Romania_Extract_3x3.tif
    ${INPUT_MAJADATATU_IMAGES}/mask_B01.tif
    ${INPUT_MAJADATATU_IMAGES}/mask_B02.tif
    ${INPUT_MAJADATATU_IMAGES}/mask_B03.tif
    ${TEMP}/${PREFIX}StreamingMaskConditionalStatisticsVectorImageFilterTest.txt
    )

# -------    vnsStreamingMaskConditionalStatisticsVectorImageFilterPipelineTest.cxx
ADD_TEST(${PREFIX}StreamingMaskConditionalStatisticsVectorImageFilterPipelineAllMaskedTest ${TESTS1}
   --compare-n-ascii ${NOTOL} 1
    ${BASELINEDATATU_FILES}/${PREFIX}StreamingMaskConditionalStatisticsVectorImageFilterAllMaskedTest.txt
    ${TEMP}/${PREFIX}StreamingMaskConditionalStatisticsVectorImageFilterAllMaskedTest.txt
    vnsStreamingMaskConditionalStatisticsVectorImageFilterPipelineTest
    ${INPUT_MAJADATATU_IMAGES}/Romania_Extract_3x3.tif
    ${INPUT_MAJADATATU_IMAGES}/mask_All.tif
    ${INPUT_MAJADATATU_IMAGES}/mask_B02.tif
    ${INPUT_MAJADATATU_IMAGES}/mask_B03.tif
    ${TEMP}/${PREFIX}StreamingMaskConditionalStatisticsVectorImageFilterAllMaskedTest.txt
    )

# -------    vnsvnsRegionForSubsampleNew.cxx
ADD_TEST(${PREFIX}vnsRegionForSubsampleNew ${TESTS1}
    vnsRegionForSubsampleNew        
)    

# -------   vnsApplyMaskFunctor
ADD_TEST(${PREFIX}ApplyMaskFunctorNew ${TESTS1}
    vnsApplyMaskFunctorNew
)

# -------   vnsChangeValueFunctorTest.cxx
ADD_TEST(${PREFIX}ChangeValueFunctorTest ${TESTS1}
    --compare-image ${NOTOL} 
    ${BASELINEDATATU_IMAGES}/${PREFIX}ChangeValueFunctorTest.tif
    ${TEMP}/${PREFIX}ChangeValueFunctorTest.tif
    vnsChangeValueFunctorTest
    ${INPUT_MAJADATATU_IMAGES}/Romania_Extract_30x30.tif
    ${INPUT_MAJADATATU_IMAGES}/Romania_Extract_Mask_30x30.tif
    ${TEMP}/${PREFIX}ChangeValueFunctorTest.tif
    0
    0
    )    


# -------    vnsBinaryConcatenationVectorImageFilterNew.cxx    
ADD_TEST(${PREFIX}BinaryConcatenationVectorImageFilterNew ${TESTS1}
    vnsBinaryConcatenationVectorImageFilterNew
    )

# -------    vnsBinaryConcatenationVectorImageFilterTest.cxx    
ADD_TEST(${PREFIX}BinaryConcatenationVectorImageFilterTest ${TESTS1}
    --compare-n-images ${NOTOL} 1
    ${BASELINEDATATU_IMAGES}/${PREFIX}BinaryConcatenationVectorImageFilterTest.tif
    ${TEMP}/${PREFIX}BinaryConcatenationVectorImageFilterTest.tif
    vnsBinaryConcatenationVectorImageFilterTest
    ${INPUT_MAJADATATU_IMAGES}/mask_3x3.tif
    ${TEMP}/${PREFIX}BinaryConcatenationVectorImageFilterTest.tif
    )

# -------    vnsBinaryConcatenationVectorImageFilterFailed.cxx  
ADD_TEST(${PREFIX}BinaryConcatenationVectorImageFilterFailed ${TESTS1}
    vnsBinaryConcatenationVectorImageFilterFailed
    ${INPUT_MAJADATATU_IMAGES}/mask_3x3_9.tif
    ${TEMP}/binaryMask_3x3_9.tif
    )    
SET_TESTS_PROPERTIES(${PREFIX}BinaryConcatenationVectorImageFilterFailed PROPERTIES WILL_FAIL true)

# -------    vnsBinaryConcatenationVectorImageFilterTestType.cxx
ADD_TEST(${PREFIX}BinaryConcatenationVectorImageFilterTestType ${TESTS1}
    --compare-n-images ${NOTOL} 1
    ${BASELINEDATATU_IMAGES}/${PREFIX}BinaryConcatenationVectorImageFilterTestType.tif
    ${TEMP}/${PREFIX}BinaryConcatenationVectorImageFilterTestType.tif
    vnsBinaryConcatenationVectorImageFilterTestType
    ${INPUT_MAJADATATU_IMAGES}/mask_3x3_9.tif
    ${TEMP}/${PREFIX}BinaryConcatenationVectorImageFilterTestType.tif
    )    
# -------    vnsBinaryToVectorImageFilterNew.cxx    
ADD_TEST(${PREFIX}BinaryToVectorImageFilterNew ${TESTS1}
    vnsBinaryToVectorImageFilterNew
    )
# -------    vnsBinaryToVectorImageFilterTest.cxx    
ADD_TEST(${PREFIX}BinaryToVectorImageFilterTest ${TESTS1}
    --compare-n-images ${NOTOL} 1
        ${BASELINEDATATU_IMAGES}/${PREFIX}BinaryToVectorImageFilterTest.tif
        ${TEMP}/${PREFIX}BinaryToVectorImageFilterTest.tif
    vnsBinaryToVectorImageFilterTest
    ${INPUT_MAJADATATU_IMAGES}/${PREFIX}BinaryConcatenationVectorImageFilterTest.tif
    4
    ${TEMP}/${PREFIX}BinaryToVectorImageFilterTest.tif
    )

# -------    vnsBinaryToVectorImageFilterTest.cxx    
ADD_TEST(${PREFIX}BinaryToVectorImageFilterTestNotValid ${TESTS1}
    vnsBinaryToVectorImageFilterTest
    ${TEMP}/${PREFIX}BinaryConcatenationVectorImageFilterTest.tif
    32
    ${TEMP}/${PREFIX}BinaryToVectorImageFilterTest.tif
    )
SET_TESTS_PROPERTIES(${PREFIX}BinaryToVectorImageFilterTestNotValid PROPERTIES WILL_FAIL true)

# -------    ConstantImageSource    
ADD_TEST(${PREFIX}ConstantImageSourceNew ${TESTS1}
    vnsConstantImageSourceNew
    )

ADD_TEST(${PREFIX}ConstantImageSource ${TESTS1}
    --compare-n-images ${NOTOL} 1
        ${BASELINEDATATU_IMAGES}/${PREFIX}ConstantImageSource.tif
        ${TEMP}/${PREFIX}ConstantImageSource.tif
    vnsConstantImageSource
    ${TEMP}/${PREFIX}ConstantImageSource.tif
    10 20 # Image size
    412.14788 # Value
    )
# -------    ConstantVectorImageSource    
ADD_TEST(${PREFIX}ConstantVectorImageSourceNew ${TESTS1}
    vnsConstantVectorImageSourceNew
    )

ADD_TEST(${PREFIX}ConstantVectorImageSource ${TESTS1}
    --compare-n-images ${NOTOL} 1
        ${BASELINEDATATU_IMAGES}/${PREFIX}ConstantVectorImageSource.tif
        ${TEMP}/${PREFIX}ConstantVectorImageSource.tif
    vnsConstantVectorImageSource
    ${TEMP}/${PREFIX}ConstantVectorImageSource.tif
    10 20 # Image size
    3 # Nb value 
    1.2 3.5 78.25 # List of values
    )

   
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ vnsAlgorithmsCommonTests2 ~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

# -------    LinearInterpolateImageFunctionNew    
ADD_TEST(${PREFIX}LinearInterpolateImageFunctionNew ${TESTS1}
    vnsLinearInterpolateImageFunctionNew
    )
    

# -------    vnsChangeValueVectorImageFilterNew    
ADD_TEST(${PREFIX}ChangeValueVectorImageFilterNew ${TESTS1}
    vnsChangeValueVectorImageFilterNew
    )
  # -------    vnsChangeValueVectorImageFilterTest   
ADD_TEST(${PREFIX}ChangeValueVectorImageFilterTest ${TESTS1}
    --compare-n-images ${NOTOL} 1
        ${BASELINEDATATU_IMAGES}/${PREFIX}ChangeValueVectorImageFilterTest.tif
        ${TEMP}/${PREFIX}ChangeValueVectorImageFilterTest.tif
    vnsChangeValueVectorImageFilterTest
    #${INPUT_MAJADATATU_IMAGES_ORTHO_SURF_COR_ENV}/Sudouest_20060329_MS_fmsat_ortho_surf_env_8m.tif
    ${INPUT_MAJADATATU_IMAGES}/Mask_ROI_IKO_PAN_LesHalles.tif
    ${TEMP}/${PREFIX}ChangeValueVectorImageFilterTest.tif
    )

  # -------    vnsChangeValueWithMaskVectorFunctorTest   
ADD_TEST(${PREFIX}ChangeValueWithMaskVectorFunctorTest ${TESTS1}
    --compare-n-images ${NOTOL} 1
        ${BASELINEDATATU_IMAGES}/${PREFIX}ChangeValueWithMaskVectorImageFilterTest.tif
        ${TEMP}/${PREFIX}ChangeValueWithMaskVectorImageFilterTest.tif
    vnsChangeValueWithMaskVectorFunctorTest
    ${INPUT_MAJADATATU_IMAGES_ORTHO_SURF_COR_ENV}/Sudouest_20060329_MS_fmsat_ortho_surf_env_8m.tif
    ${INPUT_MAJADATATU_IMAGES_BORD}/Sudouest_20060329_MS_fmsat_ortho_toa_bord
    #${INPUT_MAJADATATU_IMAGES}/Romania_Extract.tif
    #${INPUT_MAJADATATU_IMAGES}/Mask_Romania_Extract.tif
    ${TEMP}/${PREFIX}ChangeValueWithMaskVectorImageFilterTest.tif
    )
            
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ vnsAlgorithmsCommonTests3 ~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
            
# -------    vnsConvolutionVectorImageFilterNew   
ADD_TEST(${PREFIX}ConvolutionVectorImageFilterNew ${TESTS2}
    vnsConvolutionVectorImageFilterNew
    )
    
    
# -------    vnsConvolutionVectorImageFilterTest.cxx   
ADD_TEST(${PREFIX}ConvolutionVectorImageFilterTest ${TESTS2}
    --compare-n-images ${NOTOL} 1
    ${BASELINEDATATU_IMAGES}/${PREFIX}ConvolutionVectorImageWithMask.tif
    ${TEMP}/${PREFIX}ConvolutionVectorImageWithMask.tif
    vnsConvolutionVectorImageFilterTest
    ${INPUT_MAJADATATU_IMAGES}/Romania_Extract.tif
    ${INPUT_MAJADATATU_IMAGES}/Mask_Romania_Extract.tif
    ${TEMP}/${PREFIX}ConvolutionVectorImageWithMask.tif
    -1 # NoData value
    )
    
# -------    vnsConvolutionVectorImageFilterTest.cxx - ValidationConvolution 
ADD_TEST(${PREFIX}ConvolutionVectorImageFilterTestValid ${TESTS2}    
    --compare-n-images ${NOTOL} 1
    ${BASELINEDATATU_IMAGES}/${PREFIX}ConvolutionVectorImageValidation.tif
    ${TEMP}/${PREFIX}ConvolutionVectorImageValidation.tif
    vnsConvolutionVectorImageFilterTest
    ${INPUT_MAJADATATU_IMAGES_SURF}/Sudouest_20060329_MS_fmsat_ortho_surf_96m_261010.tif # rho_surf_sub (convolution)
    ${INPUT_MAJADATATU_IMAGES_BORD96}/Sudouest_20060329_MS_fmsat_ortho_toa_bord_96m # mask_bord_sub (D) (convolution)
    ${TEMP}/${PREFIX}ConvolutionVectorImageValidation.tif
    -1 # NoData value
    )
   
                
ADD_TEST(${PREFIX}ThresholdImageFunctorTest ${TESTS2}
    --compare-n-images ${NOTOL} 1
    ${BASELINEDATATU_IMAGES}/${PREFIX}ThresholdImageFunctorTest.tif
    ${TEMP}/${PREFIX}ThresholdImageFunctorTest.tif
    vnsThresholdImageFunctorTest
    ${INPUT_MAJADATATU_IMAGES}/mask_B01.tif
    ${TEMP}/${PREFIX}ThresholdImageFunctorTest.tif
    100
    0
    1
    )

# -------    vnsThresholdVectorImageFunctorTest.cxx
ADD_TEST(${PREFIX}ThresholdVectorImageFunctorTest1 ${TESTS2}
    --compare-n-images ${NOTOL} 1
    ${BASELINEDATATU_IMAGES}/${PREFIX}BinaryThresholdVectorImageFunctorTest1.tif
    ${TEMP}/${PREFIX}BinaryThresholdVectorImageFunctorTest1.tif
    vnsThresholdVectorImageFunctorTest
    ${INPUT_MAJADATATU_IMAGES}/ROI_IKO_PAN_LesHalles.tif
    ${TEMP}/${PREFIX}BinaryThresholdVectorImageFunctorTest1.tif
    100
    1
    0
    )

ADD_TEST(${PREFIX}ThresholdVectorImageFunctorTest2 ${TESTS2}
    --compare-n-images ${NOTOL} 1
    ${BASELINEDATATU_IMAGES}/${PREFIX}BinaryThresholdVectorImageFunctorTest2.tif
    ${TEMP}/${PREFIX}BinaryThresholdVectorImageFunctorTest2.tif
    vnsThresholdVectorImageFunctorTest
    ${INPUT_MAJADATATU_IMAGES}/Romania_Extract.tif
    ${TEMP}/${PREFIX}BinaryThresholdVectorImageFunctorTest2.tif
    150
    1
    0
    )

# -------    vnsValueCountImageFilterNew.cxx
ADD_TEST(${PREFIX}ValueCountImageFilterNew ${TESTS2}
    vnsValueCountImageFilterNew
    )            
  
# -------    vnsValueCountImageFilterTest.cxx
ADD_TEST(${PREFIX}ValueCountImageFilterTest ${TESTS2}
   --compare-n-ascii ${NOTOL} 1
    ${BASELINEDATATU_FILES}/${PREFIX}ValueCountImageFilterTest.txt
    ${TEMP}/${PREFIX}ValueCountImageFilterTest.txt
    vnsValueCountImageFilterTest
    ${INPUT_MAJADATATU_IMAGES}/mask_B01.tif
    ${TEMP}/${PREFIX}ValueCountImageFilterTest.txt
    255
    )
# -------    vnsValueCountImageFilterTest.cxx
ADD_TEST(${PREFIX}ValueCountImageFilterTest2 ${TESTS2}
   --compare-n-ascii ${NOTOL} 1
    ${BASELINEDATATU_FILES}/${PREFIX}ValueCountImageFilterTest2.txt
    ${TEMP}/${PREFIX}ValueCountImageFilterTest2.txt
    vnsValueCountImageFilterTest
    ${INPUT_MAJADATATU_IMAGES}/mask_B03.tif
    ${TEMP}/${PREFIX}ValueCountImageFilterTest2.txt
    255
    ) 

# -------    vnsThresholdVectorImageFilterNew.cxx
ADD_TEST(${PREFIX}ThresholdVectorImageFilterNew ${TESTS2}
    vnsThresholdVectorImageFilterNew
) 

# -------    vnsThresholdVectorImageFilterTest.cxx
ADD_TEST(${PREFIX}ThresholdVectorImageFilterTest ${TESTS2}
   --compare-image ${NOTOL}
    ${BASELINEDATATU_FILES}/${PREFIX}ThresholdVectorImageFilterTest.tif
    ${TEMP}/${PREFIX}ThresholdVectorImageFilterTest.tif
    vnsThresholdVectorImageFilterTest
    ${INPUT_MAJADATATU_IMAGES}/Romania_Extract.tif
    ${TEMP}/${PREFIX}ThresholdVectorImageFilterTest.tif
    ) 


ADD_TEST(${PREFIX}MultiChannelExtractROI_OTB ${TESTS2}
    vnsMultiChannelExtractROI
    "OTB"
    8876 11779 
    15  1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 
    10 # Nb StreamDivision
    1 12 #First last channel
    ) 

ADD_TEST(${PREFIX}MultiChannelExtractROI_VENUS ${TESTS2}
    vnsMultiChannelExtractROI
    "VENUS"
    8876 11779 
    15  1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 
    10 # Nb StreamDivision
    1 12 # First last channel
    ) 

ADD_TEST(${PREFIX}ListOfStringToImageSourceNew ${TESTS2}
    vnsListOfStringToImageSourceNew
    ) 

ADD_TEST(${PREFIX}ListOfStringToImageSourceTest ${TESTS2}
    --compare-n-images ${NOTOL} 1
        ${INPUT_MAJADATATU_IMAGES}/image_matrix_values_23x23.tif
        ${TEMP}/${PREFIX}_image_matrix_values_23x23.tif
    vnsListOfStringToImageSourceTest
    ${INPUT_MAJADATATU_IMAGES}/ascii_matrix_values_23x23.txt
    0. # Origni
    5000. #Spacing
    ${TEMP}/${PREFIX}_image_matrix_values_23x23.tif
    ) 



# -------    vnsOneBandEqualThresholdFunctorTest.cxx
ADD_TEST(${PREFIX}EqualThresholdFunctorTest1 ${TESTS2}
    --compare-n-images ${NOTOL} 1
    ${BASELINEDATATU_IMAGES}/${PREFIX}EqualThresholdFunctorTest1.tif
    ${TEMP}/${PREFIX}EqualThresholdFunctorTest1.tif
    vnsEqualThresholdFunctorTest
    ${INPUT_MAJADATATU_IMAGES}/Romania_Extract_B01.tif
    ${TEMP}/${PREFIX}EqualThresholdFunctorTest1.tif
    150
    1000
    0
    )
    
# -------    vnsConcatenatePerZoneVectorImageFilterNew.cxx
ADD_TEST(${PREFIX}ConcatenatePerZoneVectorImageFilterNew ${TESTS2}
    vnsConcatenatePerZoneVectorImageFilterNew
    )
# -------    vnsConcatenatePerZoneVectorImageFilterTest.cxx
ADD_TEST(${PREFIX}ConcatenatePerZoneVectorImageFilterTest ${TESTS2}
    --compare-n-images ${NOTOL} 1
        ${BASELINEDATATU_IMAGES}/${PREFIX}ConcatenatePerZoneVectorImageFilterTest.tif
        ${TEMP}/${PREFIX}ConcatenatePerZoneVectorImageFilterTest.tif
    vnsConcatenatePerZoneVectorImageFilterTest
    ${INPUT_MAJADATATU_IMAGES}/constantAngleInZone4.tif  #grille angles 1
    ${INPUT_MAJADATATU_IMAGES}/constantAngleInZone6.tif   #grille angles 2
    ${INPUT_MAJADATATU_IMAGES}/S2A_TEST_MSK_GPPL1C_065_01_15SVD_DET_FOOTPRINT_457_457.tif #mask des zones
    ${TEMP}/${PREFIX}ConcatenatePerZoneVectorImageFilterTest.tif
    )

ADD_TEST(${PREFIX}MultiplyByVectorFunctorTest ${TESTS2}
    --compare-n-images ${NOTOL} 1
    ${BASELINEDATATU_IMAGES}/${PREFIX}MultiplyByVectorFunctorTest.tif
    ${TEMP}/${PREFIX}MultiplyByVectorFunctorTest.tif
    vnsMultiplyByVectorFunctorTest
    ${INPUT_MAJADATATU_IMAGES}/mask_3x3.tif
    ${TEMP}/${PREFIX}MultiplyByVectorFunctorTest.tif
    1
    10
    100
    1000
    )

# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ vnsAlgorithmsCommonTests4 ~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

ADD_TEST(${PREFIX}OrVectorImageToImageFilterTest ${TESTS2}
    --compare-n-images ${NOTOL} 1
    ${BASELINEDATATU_IMAGES}/${PREFIX}OrVectorImageToImageFilterTest.tif
    ${TEMP}/${PREFIX}OrVectorImageToImageFilterTest.tif
    vnsOrVectorImageToImageFilterTest
    ${INPUT_MAJADATATU_IMAGES}/mask_3x3.tif
    ${TEMP}/${PREFIX}OrVectorImageToImageFilterTest.tif
)


ADD_TEST(${PREFIX}ShiftScaleImageFilterTest ${TESTS2}
    --compare-n-images ${NOTOL} 1
        ${BASELINEDATATU_IMAGES}/${PREFIX}ShiftScaleImageFilterTest.tif
        ${TEMP}/${PREFIX}ShiftScaleImageFilterTest.tif
    vnsShiftScaleImageFilterTest
    ${INPUT_MAJADATATU_IMAGES}/image_for_checking_L2QLExtractEdge_mask_UC_0_1.tif
    ${TEMP}/${PREFIX}ShiftScaleImageFilterTest.tif
)

ADD_TEST(${PREFIX}GradientMagnitudeImageFilterTest_UseSpacingTrue ${TESTS2}
    --compare-n-images ${NOTOL} 1
        ${BASELINEDATATU_IMAGES}/${PREFIX}GradientMagnitudeImageFilterTest_UseSpacingTrue.tif
        ${TEMP}/${PREFIX}GradientMagnitudeImageFilterTest_UseSpacingTrue.tif
    vnsGradientMagnitudeImageFilterTest
    ${INPUT_MAJADATATU_IMAGES}/image_for_checking_L2QLExtractEdge_mask_UC_0_255.tif
    ${TEMP}/${PREFIX}GradientMagnitudeImageFilterTest_UseSpacingTrue.tif
    1
)
ADD_TEST(${PREFIX}GradientMagnitudeImageFilterTest_UseSpacingFalse ${TESTS2}
    --compare-n-images ${NOTOL} 1
        ${BASELINEDATATU_IMAGES}/${PREFIX}GradientMagnitudeImageFilterTest_UseSpacingFalse.tif
        ${TEMP}/${PREFIX}GradientMagnitudeImageFilterTest_UseSpacingFalse.tif
    vnsGradientMagnitudeImageFilterTest
    ${INPUT_MAJADATATU_IMAGES}/image_for_checking_L2QLExtractEdge_mask_UC_0_255.tif
    ${TEMP}/${PREFIX}GradientMagnitudeImageFilterTest_UseSpacingFalse.tif
    0
)

ADD_TEST(${PREFIX}MaskUpdatingImageFilterTest ${TESTS2}
    --compare-n-images ${NOTOL} 1
        ${BASELINEDATATU_IMAGES}/${PREFIX}MaskUpdatingImageFilterTest.tif
        ${TEMP}/${PREFIX}MaskUpdatingImageFilterTest.tif
    vnsMaskUpdatingImageFilterTest
    ${INPUT_MAJADATATU_IMAGES}/mask_All.tif
    ${INPUT_MAJADATATU_IMAGES}/mask_B01.tif
    ${TEMP}/${PREFIX}MaskUpdatingImageFilterTest.tif
)

ADD_TEST(${PREFIX}ImageUpdatingWithMaskImageFilterTest ${TESTS2}
    --compare-n-images ${NOTOL} 1
        ${BASELINEDATATU_IMAGES}/${PREFIX}ImageUpdatingWithMaskImageFilterTest.tif
        ${TEMP}/${PREFIX}ImageUpdatingWithMaskImageFilterTest.tif
    vnsImageUpdatingWithMaskImageFilterTest
    ${INPUT_MAJADATATU_IMAGES}/mask_B02.tif
    ${INPUT_MAJADATATU_IMAGES}/mask_CLD.tif
    ${INPUT_MAJADATATU_IMAGES}/mask_B01.tif
    ${TEMP}/${PREFIX}ImageUpdatingWithMaskImageFilterTest.tif
)


ADD_TEST(${PREFIX}MultiChannelExtractImageFilterTest_2D_extract_2_bands ${TESTS2}
    --compare-n-images ${NOTOL} 1
        ${BASELINEDATATU_IMAGES}/${PREFIX}Multi_channel_extract_2D_nb_bands_2.tif
        ${TEMP}/${PREFIX}Multi_channel_extract_2D_nb_bands_2.tif
    vnsMultiChannelExtractImageFilterTest
    2D
    ${INPUT_MAJADATATU_IMAGES}/Romania_Extract_15Bands_80x80_ssech_10.tif
    ${TEMP}/${PREFIX}Multi_channel_extract_2D_nb_bands_2.tif
    2 3 )


ADD_TEST(${PREFIX}MultiChannelExtractImageFilterTest_3D_extract_1_band ${TESTS2}
    --compare-n-images ${NOTOL} 1
        ${BASELINEDATATU_IMAGES}/${PREFIX}Multi_channel_extract_3D_nb_bands_1.mha
        ${TEMP}/${PREFIX}Multi_channel_extract_3D_nb_bands_1.mha
    vnsMultiChannelExtractImageFilterTest
    3D
    ${INPUT_MAJADATATU_IMAGES}/dataIOMapLookUpTableFileReaderRegisterLUT_VENUS_CAMARGUE_20120101.mha
    ${TEMP}/${PREFIX}Multi_channel_extract_3D_nb_bands_1.mha
    5 )

ADD_TEST(${PREFIX}MultiChannelExtractImageFilterTest_3D_extract_3_bands ${TESTS2}
    --compare-n-images ${NOTOL} 1
        ${BASELINEDATATU_IMAGES}/${PREFIX}Multi_channel_extract_3D_nb_bands_3.mha
        ${TEMP}/${PREFIX}Multi_channel_extract_3D_nb_bands_3.mha
    vnsMultiChannelExtractImageFilterTest
    3D
    ${INPUT_MAJADATATU_IMAGES}/dataIOMapLookUpTableFileReaderRegisterLUT_VENUS_CAMARGUE_20120101.mha
    ${TEMP}/${PREFIX}Multi_channel_extract_3D_nb_bands_3.mha
    1 5 8 )

ADD_TEST(${PREFIX}MultiChannelExtractImageFilterTest_3D_extract_5_bands ${TESTS2}
    --compare-n-images ${NOTOL} 1
        ${BASELINEDATATU_IMAGES}/${PREFIX}Multi_channel_extract_3D_nb_bands_5.mha
        ${TEMP}/${PREFIX}Multi_channel_extract_3D_nb_bands_5.mha
    vnsMultiChannelExtractImageFilterTest
    3D
    ${INPUT_MAJADATATU_IMAGES}/dataIOMapLookUpTableFileReaderRegisterLUT_VENUS_CAMARGUE_20120101.mha
    ${TEMP}/${PREFIX}Multi_channel_extract_3D_nb_bands_5.mha
    1 5 8 10 11 )


IF(MAJA_TU_ENABLE_PERFORMANCES_TESTING)


#=====================================================================================================================================
#=====================================================================================================================================
#==============================                                               ========================================================
#==============================    DEBUT TESTS  FOR  RESAMPLING VALIDATION    ========================================================
#==============================                                               ========================================================
#=====================================================================================================================================
#=====================================================================================================================================

SET(nbthreads "8")


# --------------------------------------------------------------------------------------------------------------------
# SENTINEL 2
# L2   10m   ->   240 m
#     240m   ->   10m      
#      20m   ->   240m
#     240m   ->   20m      
SET(mode footprint) # centered or footprint


#####################################################################################################################
# SURECH
# linear-itk
# linear-maccs
# linear-maccs avec filtre dedicated (VectorInterpolateImage..)
#####################################################################################################################
# bco / itk-linear / maccs-linear / maccs-linear-with-radius
SET(radius 1) # 2 * 240/10
SET(nb_threads 1)
SET(padding_radius 2)
SET(number_of_lines_stripped_streaming 300)
# PadAndResamplerReSampling
FOREACH(gridfilename  ${S2_DEM_L2RESOLUTION_R1} ${S2_DEM_L2RESOLUTION_R2}) 
    GET_FILENAME_COMPONENT(INPUT_GRIDBASENAME_WITHOUT_EXTENSION "${gridfilename}" NAME)

    SET(imagefilename ${S2_TOA_L2COARSERESOLUTION})
    GET_FILENAME_COMPONENT(INPUT_BASENAME_WITHOUT_EXTENSION "${imagefilename}" NAME)

    SET(PREFIX_RESAMPLER_BENCHMARK "${PREFIX}_PERFO_SURECH_MONOBAND")
    FOREACH(method linear-itk linear-maccs) 
        FOREACH(imagetype image vector-image) 
            SET(TU_NAME ${PREFIX_RESAMPLER_BENCHMARK}_GRID_${INPUT_GRIDBASENAME_WITHOUT_EXTENSION}_${INPUT_BASENAME_WITHOUT_EXTENSION}_radius${radius}_${mode}_${imagetype}_${method}_nblines_${number_of_lines_stripped_streaming})
            ADD_TEST(${TU_NAME} ${TESTS1}
                vnsPadAndResamplerReSamplingTest
                    --InputImage ${imagefilename}
                    --OutputImage ${TEMP}/${TU_NAME}.tif
                    --ReferenceGridImage ${gridfilename}
                    --ImageType ${imagetype}    # image / vector-image / list-image 
                    --Interpolator ${method}    # bco / linear / linear-with-radius 
                    --Radius ${radius}
                    --PaddingRadius ${padding_radius}  # PaddingRadius for pre processing before resampling
                    --NbThreads ${nb_threads} 
                    --NumberOfLinesStrippedStreaming ${number_of_lines_stripped_streaming})
            SET_TESTS_PROPERTIES(${TU_NAME} PROPERTIES TIMEOUT ${MAJA_TIMEOUT_TU_PERF})
        ENDFOREACH(imagetype)
    ENDFOREACH(method)

    # DedicatedFastResampleVectorImageFilter
    SET(method linear-maccs)
    SET(PREFIX_RESAMPLER_BENCHMARK "${PREFIX}_PERFO_SURECH_SIXBANDS")
    SET(imagefilename ${S2_TOA_L2COARSERESOLUTION_SIX_BANDS})
    GET_FILENAME_COMPONENT(INPUT_BASENAME_WITHOUT_EXTENSION "${imagefilename}" NAME)
    
    SET(imagetype vector-image)
    FOREACH(method linear-itk linear-maccs) 
        SET(TU_NAME ${PREFIX_RESAMPLER_BENCHMARK}_GRID_${INPUT_GRIDBASENAME_WITHOUT_EXTENSION}_${INPUT_BASENAME_WITHOUT_EXTENSION}_radius${radius}_${mode}_${imagetype}_${method}_nblines_${number_of_lines_stripped_streaming}_OtbFrameWork)
        ADD_TEST(${TU_NAME} ${TESTS1}
            vnsPadAndResamplerReSamplingTest
                --InputImage ${imagefilename}
                --OutputImage ${TEMP}/${TU_NAME}.tif
                --ReferenceGridImage ${gridfilename}
                --ImageType ${imagetype}    # image / vector-image / list-image 
                --Interpolator ${method}    # bco / linear / linear-with-radius 
                --Radius ${radius}
                --PaddingRadius ${padding_radius}  # PaddingRadius for pre processing before resampling
                --NbThreads ${nb_threads} 
                --NumberOfLinesStrippedStreaming ${number_of_lines_stripped_streaming})
        SET_TESTS_PROPERTIES(${TU_NAME} PROPERTIES TIMEOUT ${MAJA_TIMEOUT_TU_PERF})
    ENDFOREACH(method)
ENDFOREACH(gridfilename)



#####################################################################################################################
# SOUSECH
# bco-otb
# linear-itk
# linear-maccs-with-radius
# linear-maccs
# linear-maccs avec filtre dedicated (VectorInterpolateImage..)
#####################################################################################################################
SET(gridfilename ${S2_DEM_L2COARSERESOLUTION})
GET_FILENAME_COMPONENT(INPUT_GRIDBASENAME_WITHOUT_EXTENSION "${gridfilename}" NAME)
# PadAndResamplerReSampling
FOREACH(imagefilename  ${S2_DEM_L2RESOLUTION_R1} ${S2_DEM_L2RESOLUTION_R2}) 
    GET_FILENAME_COMPONENT(INPUT_BASENAME_WITHOUT_EXTENSION "${imagefilename}" NAME)

    SET(PREFIX_RESAMPLER_BENCHMARK "${PREFIX}_PERFO_SOUSECH_MONOBAND")
#    FOREACH(method bco-otb linear-itk linear-maccs linear-maccs-with-radius) 
    FOREACH(method bco-otb linear-maccs-with-radius) 
        FOREACH(imagetype image vector-image) 
            SET(TU_NAME ${PREFIX_RESAMPLER_BENCHMARK}_GRID_${INPUT_GRIDBASENAME_WITHOUT_EXTENSION}_${INPUT_BASENAME_WITHOUT_EXTENSION}_radius${radius}_${mode}_${imagetype}_${method}_nblines_${number_of_lines_stripped_streaming}_OtbFrameWork)
            ADD_TEST(${TU_NAME} ${TESTS1}
                vnsPadAndResamplerReSamplingTest
                    --InputImage ${imagefilename}
                    --OutputImage ${TEMP}/${TU_NAME}.tif
                    --ReferenceGridImage ${gridfilename}
                    --ImageType ${imagetype}    # image / vector-image / list-image 
                    --Interpolator ${method}    # bco / linear / linear-with-radius 
                    --Radius ${radius}
                    --PaddingRadius ${padding_radius}  # PaddingRadius for pre processing before resampling
                    --NbThreads ${nb_threads} 
                    --NumberOfLinesStrippedStreaming ${number_of_lines_stripped_streaming})
            SET_TESTS_PROPERTIES(${TU_NAME} PROPERTIES TIMEOUT ${MAJA_TIMEOUT_TU_PERF})
        ENDFOREACH(imagetype)
    ENDFOREACH(method)
ENDFOREACH(imagefilename)


SET(imagefilename  ${S2_TOA_L2RESOLUTION_SIX_BANDS_R1}) 
GET_FILENAME_COMPONENT(INPUT_BASENAME_WITHOUT_EXTENSION "${imagefilename}" NAME)

SET(PREFIX_RESAMPLER_BENCHMARK "${PREFIX}_PERFO_SOUSECH_SIXBANDS")
GET_FILENAME_COMPONENT(INPUT_BASENAME_WITHOUT_EXTENSION "${imagefilename}" NAME)

SET(imagetype vector-image)

#FOREACH(method bco-otb linear-itk linear-maccs linear-maccs-with-radius) 
FOREACH(method bco-otb linear-maccs-with-radius) 
#        SET(TU_NAME ${PREFIX_RESAMPLER_BENCHMARK}_${INPUT_BASENAME_WITHOUT_EXTENSION}_GRID_${INPUT_GRIDBASENAME_WITHOUT_EXTENSION}_radius_${radius}_${mode}_${method}_nb_lines_${number_of_lines_stripped_streaming}_nb_threads_${nb_threads}_padding_radius_${padding_radius}_${imagetype})
    SET(TU_NAME ${PREFIX_RESAMPLER_BENCHMARK}_GRID_${INPUT_GRIDBASENAME_WITHOUT_EXTENSION}_${INPUT_BASENAME_WITHOUT_EXTENSION}_radius${radius}_${mode}_${imagetype}_${method}_nblines_${number_of_lines_stripped_streaming}_OtbFrameWork)
    ADD_TEST(${TU_NAME} ${TESTS1}
        vnsPadAndResamplerReSamplingTest
            --InputImage ${imagefilename}
            --OutputImage ${TEMP}/${TU_NAME}.tif
            --ReferenceGridImage ${gridfilename}
            --ImageType ${imagetype}    # image / vector-image / list-image 
            --Interpolator ${method}    # bco / linear / linear-with-radius 
            --Radius ${radius}
            --PaddingRadius ${padding_radius}  # PaddingRadius for pre processing before resampling
            --NbThreads ${nb_threads} 
            --NumberOfLinesStrippedStreaming ${number_of_lines_stripped_streaming})
    SET_TESTS_PROPERTIES(${TU_NAME} PROPERTIES TIMEOUT ${MAJA_TIMEOUT_TU_PERF})
ENDFOREACH(method)

#VENUS
SET(imagefilename ${VE_DEM_L2COARSERESOLUTION})
GET_FILENAME_COMPONENT(INPUT_BASENAME_WITHOUT_EXTENSION "${imagefilename}" NAME)
SET(gridfilename  ${VE_DEM_L2RESOLUTION})
GET_FILENAME_COMPONENT(INPUT_GRIDBASENAME_WITHOUT_EXTENSION "${gridfilename}" NAME)

SET(PREFIX_RESAMPLER_BENCHMARK "${PREFIX}_PERFO_SURECH_MONOBAND")
#FOREACH(method bco-otb linear-itk linear-maccs linear-maccs-with-radius) 
FOREACH(method bco-otb linear-maccs-with-radius) 
    FOREACH(imagetype image vector-image) 
        SET(TU_NAME ${PREFIX_RESAMPLER_BENCHMARK}_GRID_${INPUT_GRIDBASENAME_WITHOUT_EXTENSION}_${INPUT_BASENAME_WITHOUT_EXTENSION}_radius${radius}_${mode}_${imagetype}_${method}_nblines_${number_of_lines_stripped_streaming}_OtbFrameWork)
        ADD_TEST(${TU_NAME} ${TESTS1}
            vnsPadAndResamplerReSamplingTest
                --InputImage ${imagefilename}
                --OutputImage ${TEMP}/${TU_NAME}.tif
                --ReferenceGridImage ${gridfilename}
                --ImageType ${imagetype}    # image / vector-image / list-image 
                --Interpolator ${method}    # bco / linear / linear-with-radius 
                --Radius ${radius}
                --PaddingRadius ${padding_radius}  # PaddingRadius for pre processing before resampling
                --NbThreads ${nb_threads} 
                --NumberOfLinesStrippedStreaming ${number_of_lines_stripped_streaming})
        SET_TESTS_PROPERTIES(${TU_NAME} PROPERTIES TIMEOUT ${MAJA_TIMEOUT_TU_PERF})
    ENDFOREACH(imagetype)
ENDFOREACH(method)


SET(method linear-maccs)
SET(imagefilename ${VE_TOA_L1RESOLUTION})
GET_FILENAME_COMPONENT(INPUT_BASENAME_WITHOUT_EXTENSION "${imagefilename}" NAME)
SET(gridfilename  ${VE_TOA_L2RESOLUTION})
GET_FILENAME_COMPONENT(INPUT_GRIDBASENAME_WITHOUT_EXTENSION "${gridfilename}" NAME)

SET(imagetype vector-image)
SET(PREFIX_RESAMPLER_BENCHMARK "${PREFIX}_PERFO_SOUSECH_MULTIBAND")
#FOREACH(method bco-otb linear-itk linear-maccs linear-maccs-with-radius) 
FOREACH(method bco-otb linear-maccs-with-radius) 
    SET(TU_NAME ${PREFIX_RESAMPLER_BENCHMARK}_GRID_${INPUT_GRIDBASENAME_WITHOUT_EXTENSION}_${INPUT_BASENAME_WITHOUT_EXTENSION}_radius${radius}_${mode}_${imagetype}_${method}_nblines_${number_of_lines_stripped_streaming}_OtbFrameWork)
    ADD_TEST(${TU_NAME} ${TESTS1}
        vnsPadAndResamplerReSamplingTest
            --InputImage ${imagefilename}
            --OutputImage ${TEMP}/${TU_NAME}.tif
            --ReferenceGridImage ${gridfilename}
            --ImageType ${imagetype}    # image / vector-image / list-image 
            --Interpolator ${method}    # bco / linear / linear-with-radius 
            --Radius ${radius}
            --PaddingRadius ${padding_radius}  # PaddingRadius for pre processing before resampling
            --NbThreads ${nb_threads} 
            --NumberOfLinesStrippedStreaming ${number_of_lines_stripped_streaming})
    SET_TESTS_PROPERTIES(${TU_NAME} PROPERTIES TIMEOUT ${MAJA_TIMEOUT_TU_PERF})
ENDFOREACH(method)

SET(imagefilename ${VE_TOA_L2RESOLUTION})
GET_FILENAME_COMPONENT(INPUT_BASENAME_WITHOUT_EXTENSION "${imagefilename}" NAME)
SET(gridfilename  ${VE_DEM_L2COARSERESOLUTION})
GET_FILENAME_COMPONENT(INPUT_GRIDBASENAME_WITHOUT_EXTENSION "${gridfilename}" NAME)

SET(PREFIX_RESAMPLER_BENCHMARK "${PREFIX}_PERFO_SOUSECH_MULTIBAND")
FOREACH(method bco-otb linear-itk linear-maccs linear-maccs-with-radius) 
    FOREACH(imagetype vector-image) 
        SET(TU_NAME ${PREFIX_RESAMPLER_BENCHMARK}_GRID_${INPUT_GRIDBASENAME_WITHOUT_EXTENSION}_${INPUT_BASENAME_WITHOUT_EXTENSION}_radius${radius}_${mode}_${imagetype}_${method}_nblines_${number_of_lines_stripped_streaming}_OtbFrameWork)
        ADD_TEST(${TU_NAME} ${TESTS1}
            vnsPadAndResamplerReSamplingTest
                --InputImage ${imagefilename}
                --OutputImage ${TEMP}/${TU_NAME}.tif
                --ReferenceGridImage ${gridfilename}
                --ImageType ${imagetype}    # image / vector-image / list-image 
                --Interpolator ${method}    # bco / linear / linear-with-radius 
                --Radius ${radius}
                --PaddingRadius ${padding_radius}  # PaddingRadius for pre processing before resampling
                --NbThreads ${nb_threads} 
                --NumberOfLinesStrippedStreaming ${number_of_lines_stripped_streaming})
        SET_TESTS_PROPERTIES(${TU_NAME} PROPERTIES TIMEOUT ${MAJA_TIMEOUT_TU_PERF})
    ENDFOREACH(imagetype)
ENDFOREACH(method)




SET(PREFIX_RESAMPLER_BENCHMARK "${PREFIX}_RESAMPLING_WITH_GRID_PAD_AND_RESAMPLER_SIX_BANDS_IMAGE")

SET(imagefilename ${S2_TOA_L2COARSERESOLUTION_SIX_BANDS})
GET_FILENAME_COMPONENT(INPUT_BASENAME_WITHOUT_EXTENSION "${imagefilename}" NAME)
FOREACH(method bco-otb linear-itk linear-maccs linear-maccs-with-radius) 
    FOREACH(imagetype vector-image) # list-image) 
        SET(TU_NAME ${PREFIX_RESAMPLER_BENCHMARK}_${INPUT_BASENAME_WITHOUT_EXTENSION}_GRID_${INPUT_GRIDBASENAME_WITHOUT_EXTENSION}_radius_${radius}_${mode}_${method}_nb_lines_${number_of_lines_stripped_streaming}_${imagetype})
        ADD_TEST(${TU_NAME} ${TESTS1}
            vnsPadAndResamplerReSamplingTest
                --InputImage ${imagefilename}
                --OutputImage ${TEMP}/${TU_NAME}.tif
                --ReferenceGridImage ${gridfilename}
                --ImageType ${imagetype}    # image / vector-image / list-image 
                --Interpolator ${method}    # bco / linear / linear-with-radius 
                --Radius ${radius}
                --PaddingRadius ${padding_radius}  # PaddingRadius for pre processing before resampling
                --NbThreads ${nb_threads} 
                --NumberOfLinesStrippedStreaming ${number_of_lines_stripped_streaming})
        SET_TESTS_PROPERTIES(${TU_NAME} PROPERTIES TIMEOUT ${MAJA_TIMEOUT_TU_PERF})
    ENDFOREACH(imagetype)
ENDFOREACH(method)
###########
SET(gridfilename ${S2_DEM_L2COARSERESOLUTION})
GET_FILENAME_COMPONENT(INPUT_GRIDBASENAME_WITHOUT_EXTENSION "${gridfilename}" NAME)

SET(imagefilename ${S2_TOA_L2RESOLUTION_SIX_BANDS_R1})
GET_FILENAME_COMPONENT(INPUT_BASENAME_WITHOUT_EXTENSION "${imagefilename}" NAME)
FOREACH(method bco-otb linear-itk linear-maccs linear-maccs-with-radius) 
    FOREACH(imagetype vector-image) # list-image) 
#        SET(TU_NAME ${PREFIX_RESAMPLER_BENCHMARK}_${INPUT_BASENAME_WITHOUT_EXTENSION}_GRID_${INPUT_GRIDBASENAME_WITHOUT_EXTENSION}_radius_${radius}_${mode}_${method}_nb_lines_${number_of_lines_stripped_streaming}_nb_threads_${nb_threads}_padding_radius_${padding_radius}_${imagetype})
        SET(TU_NAME ${PREFIX_RESAMPLER_BENCHMARK}_${INPUT_BASENAME_WITHOUT_EXTENSION}_GRID_${INPUT_GRIDBASENAME_WITHOUT_EXTENSION}_radius_${radius}_${mode}_${method}_nb_lines_${number_of_lines_stripped_streaming}_${imagetype})
        ADD_TEST(${TU_NAME} ${TESTS1}
            vnsPadAndResamplerReSamplingTest
                --InputImage ${imagefilename}
                --OutputImage ${TEMP}/${TU_NAME}.tif
                --ReferenceGridImage ${gridfilename}
                --ImageType ${imagetype}    # image / vector-image / list-image 
                --Interpolator ${method}    # bco / linear / linear-with-radius 
                --Radius ${radius}
                --PaddingRadius ${padding_radius}  # PaddingRadius for pre processing before resampling
                --NbThreads ${nb_threads} 
                --NumberOfLinesStrippedStreaming ${number_of_lines_stripped_streaming})
        SET_TESTS_PROPERTIES(${TU_NAME} PROPERTIES TIMEOUT ${MAJA_TIMEOUT_TU_PERF})
    ENDFOREACH(imagetype)
ENDFOREACH(method)



SET(PREFIX_RESAMPLER_BENCHMARK "${PREFIX}_REFERENCE_RESAMPLING_WITH_DEM_GRID_PadAndResampler")

SET(imagefilename ${S2_TOA_L2COARSERESOLUTION_SIX_BANDS})
SET(imagetype "vector-image")
GET_FILENAME_COMPONENT(INPUT_BASENAME_WITHOUT_EXTENSION "${imagefilename}" NAME)
SET(TU_NAME ${PREFIX_RESAMPLER_BENCHMARK}_${INPUT_BASENAME_WITHOUT_EXTENSION}_GRID_${INPUT_GRIDBASENAME_WITHOUT_EXTENSION}_radius_${radius}_${mode}_${method}_nb_lines_${number_of_lines_stripped_streaming}_nb_threads_${nb_threads}_padding_radius_${padding_radius}_${imagetype})
ADD_TEST(${TU_NAME} ${TESTS1}
    vnsPadAndResamplerReSamplingTest
        --InputImage ${imagefilename}
        --OutputImage ${TEMP}/${TU_NAME}.tif
        --ReferenceGridImage ${gridfilename}
        --ImageType ${imagetype}    # image / vector-image / list-image 
        --Interpolator ${method}    # bco / linear / linear-with-radius 
        --Radius ${radius}
        --PaddingRadius ${padding_radius}  # PaddingRadius for pre processing before resampling
        --NbThreads ${nb_threads} 
        --NumberOfLinesStrippedStreaming ${number_of_lines_stripped_streaming})
SET_TESTS_PROPERTIES(${TU_NAME} PROPERTIES TIMEOUT ${MAJA_TIMEOUT_TU_PERF})

SET(method maccs-linear)
SET(imagetype "vector-image")
GET_FILENAME_COMPONENT(INPUT_BASENAME_WITHOUT_EXTENSION "${imagefilename}" NAME)
SET(TU_NAME ${PREFIX_RESAMPLER_BENCHMARK}_${INPUT_BASENAME_WITHOUT_EXTENSION}_GRID_${INPUT_GRIDBASENAME_WITHOUT_EXTENSION}_radius_${radius}_${mode}_${method}_nb_lines_${number_of_lines_stripped_streaming}_nb_threads_${nb_threads}_padding_radius_${padding_radius}_${imagetype})
ADD_TEST(${TU_NAME} ${TESTS1}
    vnsPadAndResamplerReSamplingTest
        --InputImage ${imagefilename}
        --OutputImage ${TEMP}/${TU_NAME}.tif
        --ReferenceGridImage ${gridfilename}
        --ImageType ${imagetype}    # image / vector-image / list-image 
        --Interpolator ${method}    # bco / linear / linear-with-radius 
        --Radius ${radius}
        --PaddingRadius ${padding_radius}  # PaddingRadius for pre processing before resampling
        --NbThreads ${nb_threads} 
        --NumberOfLinesStrippedStreaming ${number_of_lines_stripped_streaming})
SET_TESTS_PROPERTIES(${TU_NAME} PROPERTIES TIMEOUT ${MAJA_TIMEOUT_TU_PERF})



SET(imagefilename ${S2_TOA_L2COARSERESOLUTION_16_BANDS})
SET(imagetype "vector-image")
GET_FILENAME_COMPONENT(INPUT_BASENAME_WITHOUT_EXTENSION "${imagefilename}" NAME)
SET(TU_NAME ${PREFIX_RESAMPLER_BENCHMARK}_${INPUT_BASENAME_WITHOUT_EXTENSION}_GRID_${INPUT_GRIDBASENAME_WITHOUT_EXTENSION}_radius_${radius}_${mode}_${method}_nb_lines_${number_of_lines_stripped_streaming}_nb_threads_${nb_threads}_padding_radius_${padding_radius}_${imagetype})
ADD_TEST(${TU_NAME} ${TESTS1}
    vnsPadAndResamplerReSamplingTest
        --InputImage ${imagefilename}
        --OutputImage ${TEMP}/${TU_NAME}.tif
        --ReferenceGridImage ${gridfilename}
        --ImageType ${imagetype}    # image / vector-image / list-image 
        --Interpolator ${method}    # bco / linear / linear-with-radius 
        --Radius ${radius}
        --PaddingRadius ${padding_radius}  # PaddingRadius for pre processing before resampling
        --NbThreads ${nb_threads} 
        --NumberOfLinesStrippedStreaming ${number_of_lines_stripped_streaming})
SET_TESTS_PROPERTIES(${TU_NAME} PROPERTIES TIMEOUT ${MAJA_TIMEOUT_TU_PERF})

SET(imagefilename ${S2_TOA_L2COARSERESOLUTION_SIX_BANDS})
SET(gridfilename ${S2_DEM_L2RESOLUTION_R1})
GET_FILENAME_COMPONENT(INPUT_BASENAME_WITHOUT_EXTENSION "${imagefilename}" NAME)
GET_FILENAME_COMPONENT(INPUT_GRIDBASENAME_WITHOUT_EXTENSION "${gridfilename}" NAME)
SET(TU_NAME ${PREFIX_RESAMPLER_BENCHMARK}_${INPUT_BASENAME_WITHOUT_EXTENSION}_GRID_${INPUT_GRIDBASENAME_WITHOUT_EXTENSION}_radius_${radius}_${mode}_${method}_nb_lines_${number_of_lines_stripped_streaming}_nb_threads_${nb_threads}_padding_radius_${padding_radius})
ADD_TEST(${TU_NAME} ${TESTS1}
    vnsPadAndResamplerReSamplingTest
        --InputImage ${imagefilename}
        --OutputImage ${TEMP}/${TU_NAME}.tif
        --ReferenceGridImage ${gridfilename}
        --Interpolator ${method} # bco / linear / linear-with-radius 
        --Radius ${radius}
        --PaddingRadius ${padding_radius}  # PaddingRadius for pre processing before resampling
        --NbThreads ${nb_threads} 
        --NumberOfLinesStrippedStreaming ${number_of_lines_stripped_streaming})
SET_TESTS_PROPERTIES(${TU_NAME} PROPERTIES TIMEOUT ${MAJA_TIMEOUT_TU_PERF})

# VENUS
SET(imagefilename ${VE_TOA_L2COARSERESOLUTION})
SET(gridfilename ${VE_DEM_L2RESOLUTION})
GET_FILENAME_COMPONENT(INPUT_BASENAME_WITHOUT_EXTENSION "${imagefilename}" NAME)
GET_FILENAME_COMPONENT(INPUT_GRIDBASENAME_WITHOUT_EXTENSION "${gridfilename}" NAME)

SET(TU_NAME ${PREFIX_RESAMPLER_BENCHMARK}_${INPUT_BASENAME_WITHOUT_EXTENSION}_GRID_${INPUT_GRIDBASENAME_WITHOUT_EXTENSION}_radius_${radius}_${mode}_${method}_nb_lines_${number_of_lines_stripped_streaming}_nb_threads_${nb_threads}_padding_radius_${padding_radius})
ADD_TEST(${TU_NAME} ${TESTS1}
    vnsPadAndResamplerReSamplingTest
        --InputImage ${imagefilename}
        --OutputImage ${TEMP}/${TU_NAME}.tif
        --ReferenceGridImage ${gridfilename}
        --Interpolator ${method} # bco / linear / linear-with-radius 
        --Radius ${radius}
        --PaddingRadius ${padding_radius}  # PaddingRadius for pre processing before resampling
        --NbThreads ${nb_threads} 
        --NumberOfLinesStrippedStreaming ${number_of_lines_stripped_streaming})
SET_TESTS_PROPERTIES(${TU_NAME} PROPERTIES TIMEOUT ${MAJA_TIMEOUT_TU_PERF})



# S2 L1    10m   ->   240 m
SET(PREFIX_RESAMPLER_BENCHMARK "${PREFIX}_REFERENCE_RESAMPLING_PadAndResampler")

SET(factor_num 240)
SET(factor_denom 10)
# BCO
SET(method linear)
SET(radius 0) # 2 * 240/10
SET(nb_threads 1)
SET(padding_radius 2)
SET(number_of_lines_stripped_streaming 300)


SET(imagefilename ${INPUT_MAJADATATU_IMAGES}/images-for-performances-analysis/image-mono-band-uint16-458x458.tif)
GET_FILENAME_COMPONENT(INPUT_BASENAME_WITHOUT_EXTENSION "${imagefilename}" NAME)

SET(TU_NAME ${PREFIX_RESAMPLER_BENCHMARK}_${INPUT_BASENAME_WITHOUT_EXTENSION}_factor_${factor_num}_${factor_denom}_radius_${radius}_${mode}_${method}_nb_lines_${number_of_lines_stripped_streaming}_nb_threads_${nb_threads}_padding_radius_${padding_radius})
ADD_TEST(${TU_NAME} ${TESTS1}
    vnsPadAndResamplerReSamplingTest
        --InputImage ${imagefilename}
        --OutputImage ${TEMP}/${TU_NAME}.tif
        --InputOutputResols ${factor_num} ${factor_denom}
        --Interpolator ${method} # bco / linear / linear-with-radius 
        --Convention ${mode} # centered / footprint
        --Radius ${radius}
        --PaddingRadius ${padding_radius}  # PaddingRadius for pre processing before resampling
        --NbThreads ${nb_threads} 
        --NumberOfLinesStrippedStreaming ${number_of_lines_stripped_streaming})
SET_TESTS_PROPERTIES(${TU_NAME} PROPERTIES TIMEOUT ${MAJA_TIMEOUT_TU_PERF})


SET(PREFIX_RESAMPLER_BENCHMARK "${PREFIX}_REFERENCE_RESAMPLING_ResamplerHelper")



SET(imagefilename ${INPUT_MAJADATATU_IMAGES}/images-for-performances-analysis/image-six-bands-uint16-458x458.tif)
GET_FILENAME_COMPONENT(INPUT_BASENAME_WITHOUT_EXTENSION "${imagefilename}" NAME)

SET(TU_NAME ${PREFIX_RESAMPLER_BENCHMARK}_${INPUT_BASENAME_WITHOUT_EXTENSION}_factor_${factor_num}_${factor_denom}_radius_${radius}_${mode}_${method}_nb_lines_${number_of_lines_stripped_streaming}_nb_threads_${nb_threads}_padding_radius_${padding_radius})
ADD_TEST(${TU_NAME} ${TESTS1}
    vnsPadAndResamplerReSamplingTest
        --InputImage ${imagefilename}
        --OutputImage ${TEMP}/${TU_NAME}.tif
        --InputOutputResols ${factor_num} ${factor_denom}
        --Interpolator ${method} # bco / linear / linear-with-radius 
        --Convention ${mode} # centered / footprint
        --Radius ${radius}
        --PaddingRadius ${padding_radius}  # PaddingRadius for pre processing before resampling
        --NbThreads ${nb_threads} 
        --NumberOfLinesStrippedStreaming ${number_of_lines_stripped_streaming})
SET_TESTS_PROPERTIES(${TU_NAME} PROPERTIES TIMEOUT ${MAJA_TIMEOUT_TU_PERF})



SET(factor_num 10)
SET(factor_denom 240)
# BCO
SET(method bco)
SET(radius 48) # 2 * 240/10
SET(nb_threads 1)
SET(padding_radius 10)
SET(number_of_lines_stripped_streaming 150)

SET(imagefilename ${INPUT_MAJADATATU_IMAGES}/images-for-performances-analysis/image-mono-band-uint16-10980x10980.tif)
GET_FILENAME_COMPONENT(INPUT_BASENAME_WITHOUT_EXTENSION "${imagefilename}" NAME)

SET(TU_NAME ${PREFIX_RESAMPLER_BENCHMARK}_${INPUT_BASENAME_WITHOUT_EXTENSION}_factor_${factor_num}_${factor_denom}_radius_${radius}_${mode}_${method}_nb_lines_${number_of_lines_stripped_streaming}_nb_threads_${nb_threads}_padding_radius_${padding_radius})
ADD_TEST(${TU_NAME} ${TESTS1}
    vnsPadAndResamplerReSamplingTest
        --InputImage ${imagefilename}
        --OutputImage ${TEMP}/${TU_NAME}.tif
        --InputOutputResols ${factor_num} ${factor_denom}
        --Interpolator ${method} # bco / linear / linear-with-radius 
        --Convention ${mode} # centered / footprint
        --Radius ${radius}
        --PaddingRadius ${padding_radius}  # PaddingRadius for pre processing before resampling
        --NbThreads ${nb_threads} 
        --NumberOfLinesStrippedStreaming ${number_of_lines_stripped_streaming})
SET_TESTS_PROPERTIES(${TU_NAME} PROPERTIES TIMEOUT ${MAJA_TIMEOUT_TU_PERF})

SET(factor_num 10)
SET(factor_denom 240)
# BCO
SET(method bco)
SET(radius 48) # 2 * 240/10
SET(nb_threads 1)
SET(padding_radius 10)
SET(number_of_lines_stripped_streaming 150)


SET(PREFIX_RESAMPLER_BENCHMARK "${PREFIX}_CheckingPadAndResampler")
#SET(imagefilename ${INPUT_MAJADATA_IMAGES_L1_SENTINEL2}/S2A_TEST_PRD_GPPL1C_065_20091211165851_20091211165932_0003/S2A_TEST_IMG_GPPL1C_065_20091211165851_20091211165932_01_000000_15SVD_0003.jp2)
SET(imagefilename ${INPUT_MAJADATATU_IMAGES}/images-for-performances-analysis/image-mono-band-uint16-10980x10980.jp2)
GET_FILENAME_COMPONENT(INPUT_BASENAME_WITHOUT_EXTENSION "${imagefilename}" NAME)

SET(TU_NAME ${PREFIX_RESAMPLER_BENCHMARK}_${INPUT_BASENAME_WITHOUT_EXTENSION}_factor_${factor_num}_${factor_denom}_radius_${radius}_${mode}_${method}_nb_lines_${number_of_lines_stripped_streaming}_nb_threads_${nb_threads}_padding_radius_${padding_radius})
ADD_TEST(${TU_NAME} ${TESTS1}
    vnsPadAndResamplerReSamplingTest
        --InputImage ${imagefilename}
        --OutputImage ${TEMP}/${TU_NAME}.tif
        --InputOutputResols ${factor_num} ${factor_denom}
        --Interpolator ${method} # bco / linear / linear-with-radius 
        --Convention ${mode} # centered / footprint
        --Radius ${radius}
        --PaddingRadius ${padding_radius} # PaddingRadius for pre processing before resampling
        --NbThreads ${nb_threads} 
        --NumberOfLinesStrippedStreaming ${number_of_lines_stripped_streaming})
SET_TESTS_PROPERTIES(${TU_NAME} PROPERTIES TIMEOUT ${MAJA_TIMEOUT_TU_PERF})

SET(PREFIX_RESAMPLER_BENCHMARK "${PREFIX}_CheckingPadAndResampler")
SET(imagefilename ${INPUT_MAJADATATU_IMAGES}/images-for-performances-analysis/image-mono-band-uint16-10980x10980.tif)
GET_FILENAME_COMPONENT(INPUT_BASENAME_WITHOUT_EXTENSION "${imagefilename}" NAME)

SET(TU_NAME ${PREFIX_RESAMPLER_BENCHMARK}_${INPUT_BASENAME_WITHOUT_EXTENSION}_factor_${factor_num}_${factor_denom}_radius_${radius}_${mode}_${method}_nb_lines_${number_of_lines_stripped_streaming}_nb_threads_${nb_threads}_padding_radius_${padding_radius})
ADD_TEST(${TU_NAME} ${TESTS1}
    vnsPadAndResamplerReSamplingTest
        --InputImage ${imagefilename}
        --OutputImage ${TEMP}/${TU_NAME}.tif
        --InputOutputResols ${factor_num} ${factor_denom}
        --Interpolator ${method} # bco / linear / linear-with-radius 
        --Convention ${mode} # centered / footprint
        --Radius ${radius}
        --PaddingRadius ${padding_radius}  # PaddingRadius for pre processing before resampling
        --NbThreads ${nb_threads} 
        --NumberOfLinesStrippedStreaming ${number_of_lines_stripped_streaming})
SET_TESTS_PROPERTIES(${TU_NAME} PROPERTIES TIMEOUT ${MAJA_TIMEOUT_TU_PERF})


SET(imagefilename ${INPUT_MAJADATA_IMAGES_L1_SENTINEL2}/S2A_TEST_PRD_GPPL1C_065_20091211165851_20091211165932_0003/S2A_TEST_IMG_GPPL1C_065_20091211165851_20091211165932_01_000000_15SVD_0003.jp2)
GET_FILENAME_COMPONENT(INPUT_BASENAME_WITHOUT_EXTENSION "${imagefilename}" NAME)
SET(factor_num 10)
SET(factor_denom 240)
# LINEAR WITH RADIUS
SET(method linear-with-radius)
SET(radius 24) # 1 * 240/10

SET(PREFIX_RESAMPLER_BENCHMARK "${PREFIX}_CheckingPadAndResampler")
SET(TU_NAME ${PREFIX_RESAMPLER_BENCHMARK}_${INPUT_BASENAME_WITHOUT_EXTENSION}_factor_${factor_num}_${factor_denom}_radius_${radius}_${mode}_${method}_nb_lines_${number_of_lines_stripped_streaming}_nb_threads_${nb_threads}_padding_radius_${padding_radius})
ADD_TEST(${TU_NAME} ${TESTS1}
    vnsPadAndResamplerReSamplingTest
        --InputImage ${imagefilename}
        --OutputImage ${TEMP}/${TU_NAME}.tif
        --InputOutputResols ${factor_num} ${factor_denom}
        --Interpolator ${method} # bco / linear / linear-with-radius 
        --Convention ${mode} # centered / footprint
        --Radius ${radius}
        --PaddingRadius ${padding_radius}  # PaddingRadius for pre processing before resampling
        --NbThreads ${nb_threads} 
        --NumberOfLinesStrippedStreaming ${number_of_lines_stripped_streaming})
SET_TESTS_PROPERTIES(${TU_NAME} PROPERTIES TIMEOUT ${MAJA_TIMEOUT_TU_PERF})



   
#=====================================================================================================================================
#=====================================================================================================================================
#==============================                                               ========================================================
#==============================    FIN   TESTS  FOR  RESAMPLING VALIDATION    ========================================================
#==============================                                               ========================================================
#=====================================================================================================================================
#=====================================================================================================================================

ENDIF(MAJA_TU_ENABLE_PERFORMANCES_TESTING)


ADD_TEST(${PREFIX}ImageListToImageListApplyMaskWithFunctorFilterTest ${TESTS1}
        --compare-n-images ${NOTOL} 3 
            ${BASELINEDATATU_IMAGES}/${PREFIX}ImageListToImageListApplyMaskWithFunctorFilterTestOutput.tifindex0.tif
            ${TEMP}/${PREFIX}ImageListToImageListApplyMaskWithFunctorFilterTestOutput.tifindex0.tif
            ${BASELINEDATATU_IMAGES}/${PREFIX}ImageListToImageListApplyMaskWithFunctorFilterTestOutput.tifindex0.tif
            ${TEMP}/${PREFIX}ImageListToImageListApplyMaskWithFunctorFilterTestOutput.tifindex1.tif
            ${BASELINEDATATU_IMAGES}/${PREFIX}ImageListToImageListApplyMaskWithFunctorFilterTestOutput.tifindex0.tif
            ${TEMP}/${PREFIX}ImageListToImageListApplyMaskWithFunctorFilterTestOutput.tifindex2.tif
    vnsImageListToImageListApplyMaskWithFunctorFilterTest
        1 # ExcludedValue
        9 # DefaultValue
        ${INPUT_MAJADATATU_IMAGES}/ValidateMaskFromLabelMapAndValidityMap_Input.tif
        ${INPUT_MAJADATATU_IMAGES}/ValidateMaskFromLabelMapAndValidityMap_InputLabel.tif
        ${TEMP}/${PREFIX}ImageListToImageListApplyMaskWithFunctorFilterTestOutput.tif
)




# Src1 sources files
SET(SRCS1
vnsBusinessCommonTests1.cxx
vnsProcessingModeTest.cxx
vnsConfigCommonParametersNew.cxx
vnsStreamingConditionalStatisticsVectorImageFilterNew.cxx
vnsStreamingConditionalStatisticsVectorImageFilterTest.cxx
vnsStreamingConditionalStatisticsImageFilterTest.cxx
vnsStreamingMaskConditionalStatisticsVectorImageFilterNew.cxx
vnsStreamingMaskConditionalStatisticsVectorImageFilterTest.cxx
vnsStreamingMaskConditionalStatisticsVectorImageFilterPipelineTest.cxx
vnsRegionForSubsampleNew.cxx
vnsBinaryConcatenationVectorImageFilterNew.cxx
vnsBinaryConcatenationVectorImageFilterTest.cxx
vnsBinaryConcatenationVectorImageFilterFailed.cxx
vnsBinaryConcatenationVectorImageFilterTestType.cxx
vnsBinaryToVectorImageFilterNew.cxx
vnsBinaryToVectorImageFilterTest.cxx
vnsConstantImageSourceNew.cxx
vnsConstantImageSource.cxx
vnsConstantVectorImageSourceNew.cxx
vnsConstantVectorImageSource.cxx
vnsChangeValueFunctorTest.cxx
vnsApplyMaskFunctorNew.cxx
vnsPadAndResamplerReSamplingTest.cxx
vnsLinearInterpolateImageFunctionNew.cxx
vnsLinearInterpolateImageFunctionTest.cxx
vnsChangeValueVectorImageFilterNew.cxx
vnsChangeValueVectorImageFilterTest.cxx
vnsChangeValueWithMaskVectorFunctorTest.cxx
vnsImageListToImageListApplyMaskWithFunctorFilterTest.cxx
)

# Src3 sources files
SET(SRCS2
vnsBusinessCommonTests2.cxx
vnsConvolutionVectorImageFilterNew.cxx
vnsConvolutionVectorImageFilterTest.cxx
vnsThresholdImageFunctorTest.cxx
vnsThresholdVectorImageFunctorTest.cxx
vnsValueCountImageFilterNew.cxx
vnsValueCountImageFilterTest.cxx
vnsThresholdVectorImageFilterNew.cxx
vnsThresholdVectorImageFilterTest.cxx
vnsMultiChannelExtractROI.cxx
vnsListOfStringToImageSourceNew.cxx
vnsListOfStringToImageSourceTest.cxx
vnsEqualThresholdFunctorTest.cxx
vnsConcatenatePerZoneVectorImageFilterNew.cxx
vnsConcatenatePerZoneVectorImageFilterTest.cxx
vnsMultiplyByVectorFunctorTest.cxx
vnsOrVectorImageToImageFilterTest.cxx
vnsShiftScaleImageFilterTest.cxx
vnsGradientMagnitudeImageFilterTest.cxx
vnsMaskUpdatingImageFilterTest.cxx
vnsImageUpdatingWithMaskImageFilterTest.cxx
vnsMultiChannelExtractImageFilterTest.cxx
)

ADD_EXECUTABLE(vnsAlgorithmsCommonTests1 ${SRCS1})
TARGET_LINK_LIBRARIES(vnsAlgorithmsCommonTests1 ${OTB_LIBRARIES}
${MAJA_LIBRARY_PREFIXNAME}DataIO 
${MAJA_LIBRARY_PREFIXNAME}AlgorithmsCommon 
${MAJA_LIBRARY_PREFIXNAME}DataCommon
${MAJA_LIBRARY_PREFIXNAME}TestHelperLibrary )


ADD_EXECUTABLE(vnsAlgorithmsCommonTests2 ${SRCS2})
TARGET_LINK_LIBRARIES(vnsAlgorithmsCommonTests2 ${OTB_LIBRARIES}
${MAJA_LIBRARY_PREFIXNAME}DataIO 
${MAJA_LIBRARY_PREFIXNAME}AlgorithmsCommon 
${MAJA_LIBRARY_PREFIXNAME}DataCommon
${MAJA_LIBRARY_PREFIXNAME}TestHelperLibrary
${MAJA_IMPORT_LIBRARIES} )


ENDIF( NOT MAJA_DISABLE_CXX_TESTING )
