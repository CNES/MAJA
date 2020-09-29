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
# VERSION : 2-0-0 : DM : LAIG-DM-MAC-146-CNES : 14 octobre 2011 : Modification pour prise en compte             #
#                                                            d'evolutions liees a MACCS                    #
#                                                                                                          #
# VERSION : 1-0-0 : <TypeFT> : <NumFT> : 01 avril 2009 : Creation                                                           
#                                                                                                          #
# FIN-HISTORIQUE                                                                                           #
#                                                                                                          #
# $Id$
#                                                                                                          #
############################################################################################################
IF( NOT MAJA_DISABLE_CXX_TESTING )
SET(PREFIX "AlgorithmsL2Subsampling")


SET(TESTS1 ${CXX_TEST_PATH}/vnsL2SubsamplingTests1)
SET(ALGO_COMMON_TESTS1 ${CXX_TEST_PATH}/vnsAlgorithmsCommonTests1)

IF(MAJA_TU_ENABLE_PERFORMANCES_TESTING)


#==================================================================================================================
# Image MONO BAND
# Test with otb::Image and otb::VectorImage filters

SET(imagefilename ${INPUT_MAJADATATU_IMAGES}/images-for-performances-analysis/image-mono-band-uint16-458x458.tif)
SET(method linear) #bco linear linear_maccs)
SET(nbthreads 1)
SET(factor_num 240)
SET(factor_denom 10)
SET(imagetype image) # image

#==================================================================================================================
#==================================================================================================================
#==================================================================================================================
#==================================================================================================================
#==================================================================================================================
#==================================================================================================================
# TEST MESUREPERFO PB sur 4.2
# TEST sur ech de l'image image-mono-band-uint16-458x458.tif

SET(imagefilename ${INPUT_MAJADATATU_IMAGES}/images-for-performances-analysis/image-mono-band-uint16-458x458.tif)
GET_FILENAME_COMPONENT(INPUT_BASENAME_WITHOUT_EXTENSION "${imagefilename}" NAME_WE)
SET(TU_NAME ${PREFIX}_REFERENCE_RESAMPLING_MACCS_EXPAND_${INPUT_BASENAME_WITHOUT_EXTENSION}_factor_${factor_num}_${factor_denom}_${method}_MACCS_${imagetype})
SET(method maccs_expand_vi)
ADD_TEST(${TU_NAME} ${TESTS1}
   vnsMainGlobalTestAllResampleImageFiltersTest
                ${imagefilename}
                ${TEMP}/${TU_NAME}.tif
                ${nbthreads}
                ${method}
                24 )
            
SET_TESTS_PROPERTIES(${TU_NAME} PROPERTIES TIMEOUT ${MAJA_TIMEOUT_TU_PERF})

SET(imagefilename ${INPUT_MAJADATATU_IMAGES}/images-for-performances-analysis/image-six-bands-uint16-458x458.tif)
GET_FILENAME_COMPONENT(INPUT_BASENAME_WITHOUT_EXTENSION "${imagefilename}" NAME_WE)
SET(TU_NAME ${PREFIX}_REFERENCE_RESAMPLING_MACCS_EXPAND_${INPUT_BASENAME_WITHOUT_EXTENSION}_factor_${factor_num}_${factor_denom}_${method}_MACCS_${imagetype})
SET(method maccs_expand_vi)
ADD_TEST(${TU_NAME} ${TESTS1}
   vnsMainGlobalTestAllResampleImageFiltersTest
                ${imagefilename}
                ${TEMP}/${TU_NAME}.tif
                ${nbthreads}
                ${method}
                24 )
SET_TESTS_PROPERTIES(${TU_NAME} PROPERTIES TIMEOUT ${MAJA_TIMEOUT_TU_PERF})


ENDIF(MAJA_TU_ENABLE_PERFORMANCES_TESTING)

# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ vnsL2SubsamplingTests1 ~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

# -------    vnsOneBandEqualThresholdFunctorTest.cxx
ADD_TEST(${PREFIX}OneBandEqualThresholdFunctorTest1 ${TESTS1}
    --compare-n-images ${NOTOL} 1
    ${BASELINEDATATU_IMAGES}/${PREFIX}OneBandEqualThresholdFunctorTest1.tif
    ${TEMP}/${PREFIX}OneBandEqualThresholdFunctorTest1.tif
    vnsOneBandEqualThresholdFunctorTest
    ${INPUT_MAJADATATU_IMAGES}/Romania_Extract.tif
    ${TEMP}/${PREFIX}OneBandEqualThresholdFunctorTest1.tif
    150
    1
    0
    )


# -------    vnsGreaterConditionalThresholdImageFunctorTest.cxx
ADD_TEST(${PREFIX}GreaterConditionalThresholdImageFunctorTest ${TESTS1}
    --compare-n-images ${NOTOL} 1
    ${BASELINEDATATU_IMAGES}/${PREFIX}GreaterConditionalThresholdImageFunctorTest.tif
    ${TEMP}/${PREFIX}GreaterConditionalThresholdImageFunctorTest.tif
    vnsGreaterConditionalThresholdImageFunctorTest
    ${INPUT_MAJADATATU_IMAGES}/Romania_Extract.tif
    ${INPUT_MAJADATATU_IMAGES}/Mask_Romania_Extract.tif
    ${TEMP}/${PREFIX}GreaterConditionalThresholdImageFunctorTest.tif
    0
    -999
    )  

# ------------------   vnsLinearSubSampleVectorImageFilterNew
ADD_TEST(${PREFIX}LinearSubSampleVectorImageFilterNew ${TESTS1}
   vnsLinearSubSampleVectorImageFilterNew
)

ADD_TEST(${PREFIX}LinearSubSampleVectorImageFilterRomania ${TESTS1}
--compare-image ${NOTOL}
    ${BASELINEDATATU_IMAGES}/${PREFIX}LinearSubSampleVectorImageFilterRom.tif
    ${TEMP}/${PREFIX}LinearSubSampleVectorImageFilterRom.tif
   vnsLinearSubSampleVectorImageFilter
    ${INPUT_MAJADATATU_IMAGES}/Romania_Extract.tif
    ${TEMP}/${PREFIX}LinearSubSampleVectorImageFilterRom.tif
    2 # sample ratio
)


# ------------------   vnsLinearSubSampleImageFilterNew
ADD_TEST(${PREFIX}LinearSubSampleImageFilterNew ${TESTS1}
   vnsLinearSubSampleImageFilterNew
)


ADD_TEST(${PREFIX}LinearSubSampleImageFilterRomania ${TESTS1}
--compare-image ${NOTOL}
    ${BASELINEDATATU_IMAGES}/${PREFIX}LinearSubSampleImageFilter.tif
    ${TEMP}/${PREFIX}LinearSubSampleImageFilter.tif
   vnsLinearSubSampleImageFilter
    ${INPUT_MAJADATATU_IMAGES}/VE_TEST_AUX_REFDE2_CRAUCAME_0001_ALT_80_80.tif
    ${TEMP}/${PREFIX}LinearSubSampleImageFilter.tif
    3 # sample ratio
    1 # nb threads
)


ADD_TEST(${PREFIX}LinearSubSampleImageFilterFullL1 ${TESTS1}
--compare-image ${NOTOL}
    ${BASELINEDATATU_IMAGES}/${PREFIX}LinearSubSampleImageFilterL1.tif
    ${TEMP}/${PREFIX}LinearSubSampleImageFilterL1.tif
   vnsLinearSubSampleImageFilter
    ${INPUT_MAJADATATU_IMAGES}/VE_TEST_AUX_REFDE2_CRAUCAME_0001_ALT_80_80.tif
    ${TEMP}/${PREFIX}LinearSubSampleImageFilterL1.tif
    2 # sample ratio
    1 # nb threads
    40 # extract index
    20  # extract size
    
)

        
# Src1 sources files
SET(SRCS1
vnsL2SubsamplingTests1.cxx
vnsOneBandEqualThresholdFunctorTest.cxx
vnsGreaterConditionalThresholdImageFunctorTest.cxx
vnsLinearSubSampleVectorImageFilterNew.cxx
vnsLinearSubSampleVectorImageFilter.cxx
vnsLinearSubSampleImageFilterNew.cxx
vnsLinearSubSampleImageFilter.cxx
vnsMainGlobalTestAllResampleImageFiltersTest.cxx
vnsCNESJulienMichelResamplerBuilder.cxx
)

ADD_EXECUTABLE(vnsL2SubsamplingTests1 ${SRCS1})
TARGET_LINK_LIBRARIES(vnsL2SubsamplingTests1 ${OTB_LIBRARIES}
${MAJA_LIBRARY_PREFIXNAME}DataIO 
${MAJA_LIBRARY_PREFIXNAME}DataCommon 
${MAJA_LIBRARY_PREFIXNAME}TestHelperLibrary )

ADD_DEPENDENCIES(vnsL2SubsamplingTests1 vnsAlgorithmsCommonTests1) 


ENDIF( NOT MAJA_DISABLE_CXX_TESTING )
