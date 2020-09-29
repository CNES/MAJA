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
# VERSION : 1-0-0 : <TypeFT> : <NumFT> : 25 mai 2009 : Creation                                                           
#                                                                                                          #
# FIN-HISTORIQUE                                                                                           #
#                                                                                                          #
# $Id$
#                                                                                                          #
############################################################################################################
IF( NOT MAJA_DISABLE_CXX_TESTING )

SET(TESTS ${CXX_TEST_PATH}/vnsUtilitiesTests)
SET(PREFIX "utilities")


# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ vnsUtilitiesTests ~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
   
ADD_TEST(${PREFIX}I3DTest1 ${TESTS}
    --compare-ascii ${NOTOL}
        ${BASELINEDATATU_FILES}/${PREFIX}I3DTest1.txt
        ${TEMP}/${PREFIX}I3DTest1.txt
    vnsIntersector3DTest
    ${INPUT_MAJADATATU_IMAGES}/Extrait_MNT/mnt_Pelican_02
    3.511581 42.715973 10000 0 0 -1
    ${TEMP}/${PREFIX}I3DTest1.txt
    )
ADD_TEST(${PREFIX}I3DTest2 ${TESTS}
    --compare-ascii ${NOTOL}
        ${BASELINEDATATU_FILES}/${PREFIX}I3DTest2.txt
        ${TEMP}/${PREFIX}I3DTest2.txt
    vnsIntersector3DTest
    ${INPUT_MAJADATATU_IMAGES}/Extrait_MNT/mnt_Pelican_02
    3.511581 42.715973 10000 0 1 -0.0001
    ${TEMP}/${PREFIX}I3DTest2.txt
    )
ADD_TEST(${PREFIX}I3DTest3 ${TESTS}
    --compare-ascii ${NOTOL}
        ${BASELINEDATATU_FILES}/${PREFIX}I3DTest3.txt
        ${TEMP}/${PREFIX}I3DTest3.txt
    vnsIntersector3DTest
    ${INPUT_MAJADATATU_IMAGES}/Extrait_MNT/mnt_Pelican_02
    3.511581 42.715973 2000 0.01 -0.003 -1000
    ${TEMP}/${PREFIX}I3DTest3.txt
    )


ADD_TEST(${PREFIX}I3DTestBufferedDEM1 ${TESTS}
    --compare-ascii ${NOTOL}
        ${BASELINEDATATU_FILES}/${PREFIX}I3DTest1.txt
        ${TEMP}/${PREFIX}I3DTestBufferedDEM1.txt
    vnsIntersector3DTestBuffer
    ${INPUT_MAJADATATU_IMAGES}/Extrait_MNT/mnt_Pelican_02
    3.511581 42.715973 10000 0 0 -1
    ${TEMP}/${PREFIX}I3DTestBufferedDEM1.txt
    )
ADD_TEST(${PREFIX}I3DTestBufferedDEM2 ${TESTS}
    --compare-ascii ${NOTOL}
        ${BASELINEDATATU_FILES}/${PREFIX}I3DTest2.txt
        ${TEMP}/${PREFIX}I3DTestBufferedDEM2.txt
    vnsIntersector3DTestBuffer
    ${INPUT_MAJADATATU_IMAGES}/Extrait_MNT/mnt_Pelican_02
    3.511581 42.715973 10000 0 1 -0.0001
    ${TEMP}/${PREFIX}I3DTestBufferedDEM2.txt
    )
ADD_TEST(${PREFIX}I3DTestBufferedDEM3 ${TESTS}
    --compare-ascii ${NOTOL}
        ${BASELINEDATATU_FILES}/${PREFIX}I3DTest3.txt
        ${TEMP}/${PREFIX}I3DTestBufferedDEM3.txt
    vnsIntersector3DTestBuffer
    ${INPUT_MAJADATATU_IMAGES}/Extrait_MNT/mnt_Pelican_02
    3.511581 42.715973 2000 0.01 -0.003 -1000
    ${TEMP}/${PREFIX}I3DTestBufferedDEM3.txt
    )

ADD_TEST(${PREFIX}I3DTestBufferedWithOTBDEM1 ${TESTS}
    --compare-ascii ${EPSILON_2}
        ${BASELINEDATATU_FILES}/${PREFIX}I3DTestBufferedWithOTBDEM1.txt
        ${TEMP}/${PREFIX}I3DTestBufferedWithOTBDEM1.txt
    vnsIntersector3DTestBufferWithOTB
    ${INPUT_MAJADATATU_IMAGES}/Extrait_MNT/mnt_Pelican_02
    3.511581 42.715973 10000 0 0 -1
    ${TEMP}/${PREFIX}I3DTestBufferedWithOTBDEM1.txt
    )

SET(POINTS 3.511581 42.715973 2000 0.01 -0.003 -1000)

ADD_TEST(${PREFIX}I3DTestBufferedDEM_Pelican_tif_RAW ${TESTS}
    --compare-ascii ${NOTOL}
        ${BASELINEDATATU_FILES}/${PREFIX}I3DTestBufferedDEM_Pelican_tif_raw.txt
        ${TEMP}/${PREFIX}I3DTestBufferedDEM_Pelican_tif_raw.txt
    vnsIntersector3DTestBufferWithOTB
    ${INPUT_MAJADATATU_IMAGES}/Extrait_MNT/mnt_Pelican_02_raw.tif
    ${POINTS}
    ${TEMP}/${PREFIX}I3DTestBufferedDEM_Pelican_tif_raw.txt
    )
    
ADD_TEST(${PREFIX}I3DTestBufferedDEM_Pelican_tif_MACCS_ORIGIN_CENTRE_PIXEL ${TESTS}
    --compare-ascii ${NOTOL}
        ${BASELINEDATATU_FILES}/${PREFIX}I3DTestBufferedDEM_Pelican_tif_maccs_origin_in_center_pixel.txt
        ${TEMP}/${PREFIX}I3DTestBufferedDEM_Pelican_tif_maccs_origin_in_center_pixel.txt
    vnsIntersector3DTestBufferWithOTB
    ${INPUT_MAJADATATU_IMAGES}/Extrait_MNT/mnt_Pelican_02_maccs_origin_in_center_pixel.tif
    ${POINTS}
    ${TEMP}/${PREFIX}I3DTestBufferedDEM_Pelican_tif_maccs_origin_in_center_pixel.txt
    )



SET(POINTS 509507.7391082958201877773  4290605.415747237391769886 3000   -117.0653114406096904076549  15.84670709405193278485058 -3000)

ADD_TEST(${PREFIX}I3DTestS2_15SVD_240m_ENVI ${TESTS}
    --compare-ascii ${NOTOL}
        ${BASELINEDATATU_FILES}/${PREFIX}I3DTestS2_15SVD_240m_ENVI.txt
        ${TEMP}/${PREFIX}I3DTestS2_15SVD_240m_ENVI.txt
    vnsIntersector3DTest
    ${INPUT_MAJADATATU_IMAGES}/15SVD_240m.mnt
    ${POINTS}
    ${TEMP}/${PREFIX}I3DTestS2_15SVD_240m_ENVI.txt
    )

SET(POINTS 659507.7391082958201877773  4800000.415747237391769886 3000   -117.0653114406096904076549  15.84670709405193278485058 -3000)

ADD_TEST(${PREFIX}I3DTestS2_15SVD_240m_TIF ${TESTS}
    --compare-ascii ${NOTOL}
        ${BASELINEDATATU_FILES}/${PREFIX}I3DTestS2_15SVD_240m_TIF.txt
        ${TEMP}/${PREFIX}I3DTestS2_15SVD_240m_TIF.txt
    vnsIntersector3DTestBufferWithOTB
    ${INPUT_MAJADATATU_IMAGES_DTM}/S2__OPER_AUX_REFDE2_31TFJ____0001.DBL.DIR/S2__OPER_AUX_REFDE2_31TFJ____0001_ALC.tif
    ${POINTS}
    ${TEMP}/${PREFIX}I3DTestS2_15SVD_240m_TIF.txt
    )

# Erreur du 18 mars 2015. Suite montée de version OTB 4.4.0 - decalage gdal origini 0.5 pixel


SET(POINTS 509744.718937853816896677017212 4399850.94161325879395008087158 3000  16.7371984076300464039377402514    97.5447828489823649533718707971   -3000)
SET(POINTS 509744.7189378539915196598      4399850.941613259725272655      3000  16.73719840763000021865992         97.5447828489823933750813         -3000)

ADD_TEST(${PREFIX}I3DTestS2_15SVD_240m_TIF_TestErreur ${TESTS}
    vnsIntersector3DTestBufferWithOTB
    ${INPUT_MAJADATATU_IMAGES_DTM}/S2__OPER_AUX_REFDE2_31TFJ____0001.DBL.DIR/S2__OPER_AUX_REFDE2_31TFJ____0001_ALC.tif
    ${POINTS}
    ${TEMP}/${PREFIX}I3DTestS2_15SVD_240m_TIF_TestErreur.txt
    )

# Src1 sources files
SET(SRCS1
vnsUtilitiesTests.cxx
vnsIntersector3DTest.cxx
vnsIntersector3DTestBuffer.cxx
vnsIntersector3DTestBufferWithOTB.cxx
)


ADD_EXECUTABLE(vnsUtilitiesTests ${SRCS1})
TARGET_LINK_LIBRARIES(vnsUtilitiesTests
  ${OTB_LIBRARIES}
${MAJA_LIBRARY_PREFIXNAME}UtilitiesI3D 
${MAJA_LIBRARY_PREFIXNAME}DataCommon 
${FREETYPE_LIBRARIES}
${MAJA_LIBRARY_PREFIXNAME}TestHelperLibrary 
)

ENDIF( NOT MAJA_DISABLE_CXX_TESTING )
