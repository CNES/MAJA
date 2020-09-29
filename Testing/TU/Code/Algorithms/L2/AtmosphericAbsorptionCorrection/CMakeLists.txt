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
# VERSION : 1-0-0 : <TypeFT> : <NumFT> : 09 avril 2009 : Creation                                                           
#                                                                                                          #
# FIN-HISTORIQUE                                                                                           #
#                                                                                                          #
# $Id$
#                                                                                                          #
############################################################################################################

IF( NOT MAJA_DISABLE_CXX_TESTING )
SET(PREFIX "AlgorithmsL2AtmosphericAbsorptionCorrection")


SET(TESTS1 ${CXX_TEST_PATH}/vnsAtmosphericAbsorptionCorrectionTests1)

# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~ vnsAtmosphericAbsorptionCorrectionTests1 ~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#------------------------ vnsPressureFunctor  ------------------------ 
ADD_TEST(${PREFIX}PressureFunctorTest ${TESTS1}
   --compare-image ${NOTOL}
    ${BASELINEDATATU_IMAGES}/${PREFIX}PressureFunctorTest.tif
    ${TEMP}/${PREFIX}PressureFunctorTest.tif
    vnsPressureFunctorTest
    ${INPUT_MAJADATATU_IMAGES}/VE_TEST_AUX_REFDE2_CRAUCAME_0001_ALT_80_80.tif
    ${TEMP}/${PREFIX}PressureFunctorTest.tif
    )


#------------------------ vnsSmacInverseContainer  ------------------------ 
ADD_TEST(${PREFIX}SmacCoefContainerTest ${TESTS1}
    vnsSmacCoefContainerTest   
)

#------------------------ vnsTOACorrectionImageFunctor  ------------------------ 
ADD_TEST(${PREFIX}TOACorrectionFunctorTest ${TESTS1}
	--compare-ascii ${NOTOL}
    ${BASELINEDATATU_FILES}/${PREFIX}vnsTOACorrectionFunctorTest.txt
    ${TEMP}/${PREFIX}vnsTOACorrectionFunctorTest.txt
    vnsTOACorrectionFunctorTest
    ${INPUT_MAJADATATU_IMAGES}/coef_FORMOSAT_continental.dat
    ${TEMP}/${PREFIX}vnsTOACorrectionFunctorTest.txt
    12 # water
    5  # pressure
    3  # ozone
    12 # thetaS
    22 # thetaV  
)

#------------------------ vnsTOACorrectionImageFilter ------------------------
ADD_TEST(${PREFIX}TOACorrectionImageFilterNew ${TESTS1}
    vnsTOACorrectionImageFilterNew
)

ADD_TEST(${PREFIX}TOACorrectionImageFilterTest ${TESTS1}
   --compare-image ${NOTOL}
    ${BASELINEDATATU_IMAGES}/${PREFIX}TOACorrectionImageFilterTest.tif
    ${TEMP}/${PREFIX}TOACorrectionImageFilterTest.tif
   vnsTOACorrectionImageFilterTest
    ${INPUT_MAJADATATU_IMAGES}/Romania_Extract_80x80.tif                                                  # TOA image
    ${INPUT_MAJADATATU_IMAGES}/VE_TEST_AUX_REFDE2_CRAUCAME_0001_ALT_80_80.tif                             # water image
    ${BASELINEDATATU_IMAGES}/AlgorithmsL2AtmosphericAbsorptionCorrectionPressureFunctorTest.tif # pressure image
    ${INPUT_MAJADATATU_IMAGES}/coef_FORMOSAT_continental.dat                                              # smac coef file
    ${TEMP}/${PREFIX}TOACorrectionImageFilterTest.tif                                       # output image
    0.005 # ozone amount
    12    # thetaS
    22    # thetaV
)

ADD_TEST(${PREFIX}TOACorrectionImageDefaultFilterTest ${TESTS1}
   --compare-image ${NOTOL}
    ${BASELINEDATATU_IMAGES}/${PREFIX}TOACorrectionImageDefaultFilterTest.tif
    ${TEMP}/${PREFIX}TOACorrectionImageDefaultFilterTest.tif
   vnsTOACorrectionImageDefaultFilterTest
    ${INPUT_MAJADATATU_IMAGES}/Romania_Extract_80x80.tif                                                  # TOA image
    ${BASELINEDATATU_IMAGES}/AlgorithmsL2AtmosphericAbsorptionCorrectionPressureFunctorTest.tif # pressure image
    ${INPUT_MAJADATATU_IMAGES}/coef_FORMOSAT_continental.dat                                              # smac coef file
    ${TEMP}/${PREFIX}TOACorrectionImageDefaultFilterTest.tif                                # output image
    3     # water amount
    0.005 # ozone amount
    12    # thetaS
    22    # thetaV
)


#------------------------ vnsWaterAmountImageGenerator ------------------------
ADD_TEST(${PREFIX}WaterAmountImageGeneratorNew ${TESTS1}
   vnsWaterAmountImageGeneratorNew
)

ADD_TEST(${PREFIX}WaterAmountImageGeneratorTest ${TESTS1}
   --compare-image ${NOTOL}
    ${BASELINEDATATU_IMAGES}/${PREFIX}WaterAmountImageGeneratorTest.tif
    ${TEMP}/${PREFIX}WaterAmountImageGeneratorTest.tif
   vnsWaterAmountImageGeneratorTest
${INPUT_MAJADATATU_IMAGES}/Romania_Extract_80x80.tif                      # TOA image
${TEMP}/${PREFIX}WaterAmountImageGeneratorTest.tif          # output image
)


# Src1 sources files
SET(SRCS1
vnsAtmosphericAbsorptionCorrectionTests1.cxx
vnsPressureFunctorTest.cxx
vnsTOACorrectionFunctorTest.cxx
vnsSmacCoefContainerTest.cxx
vnsTOACorrectionImageFilterNew.cxx
vnsTOACorrectionImageFilterTest.cxx
vnsTOACorrectionImageDefaultFilterTest.cxx
vnsWaterAmountImageGeneratorNew.cxx
vnsWaterAmountImageGeneratorTest.cxx
)

ADD_EXECUTABLE(vnsAtmosphericAbsorptionCorrectionTests1 ${SRCS1})
TARGET_LINK_LIBRARIES(vnsAtmosphericAbsorptionCorrectionTests1 ${OTB_LIBRARIES} 
${MAJA_LIBRARY_PREFIXNAME}DataIO 
${MAJA_LIBRARY_PREFIXNAME}DataCommon 
${MAJA_LIBRARY_PREFIXNAME}AlgorithmsCommon
${MAJA_LIBRARY_PREFIXNAME}TestHelperLibrary )

ENDIF( NOT MAJA_DISABLE_CXX_TESTING )
