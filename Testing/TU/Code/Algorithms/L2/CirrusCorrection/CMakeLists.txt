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
SET(PREFIX "AlgorithmsL2CirrusCorrection")


SET(TESTS1 ${CXX_TEST_PATH}/vnsL2CirrusCorrectionTests1)



# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ vnsL2CirrusCorrection1 ~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

# -------    XMLFileHolder class with .xml file test

# -------   vnsCirrusRflectanceFunctorTest.cxx    
ADD_TEST(${PREFIX}CirrusReflectanceFunctorTest1 ${TESTS1}
 --compare-image ${NOTOL}
    ${BASELINEDATATU_IMAGES}/${PREFIX}CirrusReflectanceOutput.tif
    ${TEMP}/${PREFIX}CirrusReflectanceOutput.tif
    vnsCirrusReflectanceFunctorTest
    ${INPUT_MAJADATATU_IMAGES}/vns_CirrusCorrection_CirrusImage_R2.tif
    ${INPUT_MAJADATATU_IMAGES}/11SLT_20m.mnt
    ${INPUT_MAJADATATU_IMAGES}/vns_crocodileclip_CirrusCorrection_CirrusMask_res_R2.tif
    ${TEMP}/${PREFIX}CirrusReflectanceOutput.tif
    0.002
    0.015
    0.95
    )
    

# -------   vnsCirrusMultiMeanTest.cxx
ADD_TEST(${PREFIX}CirrusMultiMeanTest1 ${TESTS1}
   --compare-ascii ${NOTOL}
    ${BASELINEDATATU_IMAGES}/${PREFIX}CirrusMultiMeanOutput.txt
    ${TEMP}/${PREFIX}CirrusMultiMeanOutput.txt
    vnsCirrusMultiMeanTest
    ${INPUT_MAJADATATU_IMAGES}/vns_CirrusCorrection_CirrusImage_R2.tif
    ${INPUT_MAJADATATU_IMAGES}/11SLT_20150803_S2A_L1Csimu_toa_abs_20m.tiff
    ${INPUT_MAJADATATU_IMAGES}/vns_crocodileclip_CirrusCorrection_CirrusMask_res_R2.tif
    ${INPUT_MAJADATATU_IMAGES}/vns_crocodileclip_CirrusCorrection_ThinCirrusMask_res_R2.tif
    ${INPUT_MAJADATATU_IMAGES}/vns_crocodileclip_CirrusCorrection_NotNonCirrusMask_res_R2.tif
    ${TEMP}/${PREFIX}CirrusMultiMeanOutput.txt
    )
    

# Src1 sources files
SET(SRCS1
vnsL2CirrusCorrectionTests1.cxx
vnsCirrusReflectanceFunctorTest.cxx
vnsCirrusCorrectionMultiMeanTest.cxx
)


ADD_EXECUTABLE(vnsL2CirrusCorrectionTests1 ${SRCS1})
TARGET_LINK_LIBRARIES(vnsL2CirrusCorrectionTests1 ${OTB_LIBRARIES} 
${MAJA_LIBRARY_PREFIXNAME}DataIO 
${MAJA_LIBRARY_PREFIXNAME}DataCommon 
${MAJA_LIBRARY_PREFIXNAME}AlgorithmsCommon
${MAJA_LIBRARY_PREFIXNAME}UtilitiesI3D
${MAJA_LIBRARY_PREFIXNAME}TestHelperLibrary
${MAJA_LIBRARY_PREFIXNAME}TestingMiscellaneous
${MAJA_IMPORT_LIBRARIES} )


ENDIF( NOT MAJA_DISABLE_CXX_TESTING )
