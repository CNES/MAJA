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
############################################################################################################                                                                                             #
#                                                                                                          #
# VERSION : 3.1.0 : DM : LAIG-DM-MAJA-2572-CNES : 22 mars 2018 : Nouveau Plugin SENTINEL2_TM               #
# VERSION : 5-1-0 : FA : LAIG-FA-MAC-1483-CNES : 24 mai 2016 : Organisation des schémas                    #
# VERSION : 4-0-0 : FA : LAIG-FA-MAC-117040-CS : 13 mars 2014 : Modifications mineures                     #
# VERSION : 2-0-0 : DM : LAIG-DM-MAC-146-CNES : 14 octobre 2011 : Modification pour prise en compte        #
#                                                            d'evolutions liees a MACCS                    #
#                                                                                                          #
# VERSION : 1-0-0 : <TypeFT> : <NumFT> : 15 novembre 2009 : Creation                                       #
#                                                                                                          #
# FIN-HISTORIQUE                                                                                           #
#                                                                                                          #
# $Id: CMakeLists.txt 4032 2012-04-12 08:41:39Z tfeuvrie $                                                 #
#                                                                                                          #
############################################################################################################


SET(MAJADATA_TV_STARTMAJA_INPUT ${MAJADATA_TV}/START_MAJA)
SET(MAJADATA_TV_STARTMAJA_BASELINE ${MAJADATA_TV}/START_MAJA/Baseline)

set(TEST_DRIVER ${CMAKE_INSTALL_PREFIX}/bin/otbTestDriver.sh)


if(ENABLE_TV)
    file(MAKE_DIRECTORY ${MAJA_TEST_OUTPUT_ROOT}/TV_START_MAJA/L2-Products)
    file(MAKE_DIRECTORY ${MAJA_TEST_OUTPUT_ROOT}/TV_START_MAJA/work)
endif()

CONFIGURE_FILE(${CMAKE_CURRENT_SOURCE_DIR}/folders.txt.in
  ${MAJA_BINARY_DIR}/Testing/TV/START_MAJA/folders.txt @ONLY)


add_test(NAME pyTv_START_MAJA
         COMMAND ${TEST_DRIVER}
         Execute
	     ${PYTHON_EXECUTABLE} ${MAJA_SOURCE_DIR}/StartMaja/Start_maja.py -f ${MAJA_BINARY_DIR}/Testing/TV/START_MAJA/folders.txt -t 31UDQ --nbackward 2 -y -v)

set_tests_properties(pyTv_START_MAJA PROPERTIES TIMEOUT 300000)


add_test(NAME pyTv_START_MAJA_COMP_IMAGE
        COMMAND ${TEST_DRIVER_MAJA}
        ${MAJA_BINARY_DIR}/bin/vnsTestMainLauncher.sh
        "--from-directory-compare-all-images" ${EPSILON_3}
        ${MAJADATA_TV_STARTMAJA_BASELINE}/SENTINEL2A_20170615-105505-255_L2A_T31UDQ_C_V1-0
        ${MAJADATA_TV_STARTMAJA_INPUT}/
        ${MAJA_TEST_OUTPUT_ROOT}/TV_START_MAJA/L2-Products/31UDQ/SENTINEL2A_20170615-105505-255_L2A_T31UDQ_C_V1-0
        "--recursively" "true" "--output-directory"
        ${MAJA_TEST_OUTPUT_ROOT}/TV_START_MAJA/L2-Products/31UDQ/testing-compare-output-directory
        ${MAJA_BINARY_DIR}/bin/vnsDummyProgram.sh
        )







