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
# VERSION : 1-0-0 : <TypeFT> : <NumFT> : 20 aout 2009 : Creation                                                           
#                                                                                                          #
# FIN-HISTORIQUE                                                                                           #
#                                                                                                          #
# $Id$
#                                                                                                          #
############################################################################################################
IF( NOT MAJA_DISABLE_CXX_TESTING )
SET(PREFIX "AlgorithmsL2Slope")


SET(TESTS1 ${CXX_TEST_PATH}/vnsL2SlopeTests1)

# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~ vnsL2SlopeTests1 ~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


# -------   vnsIncidenceAngleFunctorNew.cxx
ADD_TEST(${PREFIX}IncidenceAngleFunctorNew ${TESTS1}
    vnsIncidenceAngleFunctorNew
)

# -------   vnsIncidenceAngleFunctorTest.cxx
ADD_TEST(${PREFIX}IncidenceAngleFunctorTest ${TESTS1}
    --compare-n-images ${NOTOL} 1
    ${BASELINEDATATU_IMAGES}/${PREFIX}InternalIncidenceAngle.tif
    ${TEMP}/${PREFIX}InternalIncidenceAngleImage.tif
    vnsIncidenceAngleFunctorTest
    ${INPUT_MAJADATATU_IMAGES_DTM}/Sudouest_8m.aspect  # input1
    ${INPUT_MAJADATATU_IMAGES_DTM}/Sudouest_8m.slope   # input2
    45.975205                                           # theta_s
    146.156298                                          # phi_s
    ${TEMP}/${PREFIX}InternalIncidenceAngleImage.tif    # output1
)




# Src1 sources files
SET(SRCS1
vnsL2SlopeTests1.cxx
vnsIncidenceAngleFunctorNew.cxx
vnsIncidenceAngleFunctorTest.cxx
)


ADD_EXECUTABLE(vnsL2SlopeTests1 ${SRCS1})
TARGET_LINK_LIBRARIES(vnsL2SlopeTests1 ${OTB_LIBRARIES}
${MAJA_LIBRARY_PREFIXNAME}DataIO 
${MAJA_LIBRARY_PREFIXNAME}DataCommon 
${MAJA_LIBRARY_PREFIXNAME}AlgorithmsCommon
${MAJA_LIBRARY_PREFIXNAME}TestHelperLibrary )


ENDIF( NOT MAJA_DISABLE_CXX_TESTING )
