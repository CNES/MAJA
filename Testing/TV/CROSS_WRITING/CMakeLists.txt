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
#                        o     o                                                                           #
#                        oo   oo   oo        o   oo        ,-.                                             #
#                        o o o o  o  o       o  o  o       \_/                                             #
#                        o  o  o o    o      o o    o     {|||D                                            #
#                        o     o oooooo      o oooooo      / \                                             #
#                        o     o o    o o    o o    o      `-^                                             #
#                        o     o o    o  oooo  o    o                                                      #
#                                                                                                          #
############################################################################################################
#                                                                                                          #
# Author: CS Systemes d'Information  (France)                                                              #
#                                                                                                          #
############################################################################################################
# HISTORIQUE                                                                                               #
#                                                                                                          #
# VERSION : 1-0-0 : <TypeFT> : <NumFT> : 22 mars 2019 : Creation                                           #
#                                                                                                          #
# FIN-HISTORIQUE                                                                                           #
#                                                                                                          #
# $Id                                                                                                      #
#                                                                                                          #
#                                                                                                          #
############################################################################################################

set(TEST_DRIVER_MAJA vnsTestMainLauncher)
set(TEST_DRIVER ${CMAKE_INSTALL_PREFIX}/bin/otbTestDriver.sh)

if(ENABLE_TV)
    file(MAKE_DIRECTORY ${MAJA_TEST_OUTPUT_ROOT}/TV_CROSS_WRITING/S2)
    file(MAKE_DIRECTORY ${MAJA_TEST_OUTPUT_ROOT}/TV_CROSS_WRITING/L8)
    file(MAKE_DIRECTORY ${MAJA_TEST_OUTPUT_ROOT}/TV_CROSS_WRITING/VE)
endif()


add_test(NAME pyTv-S2-L2INIT-CROSSWRITING-SENTINEL2-SENTINEL2MUSCATE
          COMMAND ${TEST_DRIVER}
          Execute
          ${PYTHON_EXECUTABLE} ${ORCHESTRATOR}/launcher/maja.py
          "--input" "${MAJADATA_TV_SENTINEL2_INPUT}/S2-V-L2INIT-001-F-N-SENTINEL2-GENERAL/"
          "--output" "${MAJA_TEST_OUTPUT_ROOT}/TV_CROSS_WRITING/S2"
          "-m" "L2INIT"
          "-l" "DEBUG"
          "-p" "SENTINEL2_MUSCATE"
          "--stylesheet" "${MAJADATA_TV_SENTINEL2_INPUT}/S2-V-L2INIT-001-F-N-SENTINEL2-GENERAL/styleSheet_Overloading_GIPPs_Values.xsl"
          )
set_tests_properties(pyTv-S2-L2INIT-CROSSWRITING-SENTINEL2-SENTINEL2MUSCATE PROPERTIES TIMEOUT 300000)

add_test(NAME pyTv-S2-L2INIT-CROSSWRITING-LANDSAT8-LANDSAT8MUSCATE
          COMMAND ${TEST_DRIVER}
          Execute
          ${PYTHON_EXECUTABLE} ${ORCHESTRATOR}/launcher/maja.py
          "--input" "${MAJADATA_TV_LANDSAT8_INPUT}/L8-V-L2INIT-001-F-N-LANDSAT8-GENERAL/"
          "--output" "${MAJA_TEST_OUTPUT_ROOT}/TV_CROSS_WRITING/L8"
          "-m" "L2INIT"
          "-l" "DEBUG"
          "-p" "LANDSAT8_MUSCATE"
          "--stylesheet" "${MAJADATA_TV_LANDSAT8_INPUT}/L8-V-L2INIT-001-F-N-LANDSAT8-GENERAL/styleSheet_Overloading_GIPPs_Values.xsl"
          )
set_tests_properties(pyTv-S2-L2INIT-CROSSWRITING-LANDSAT8-LANDSAT8MUSCATE PROPERTIES TIMEOUT 300000)

add_test(NAME pyTv-S2-L2INIT-CROSSWRITING-VENUS-VENUSMUSCATE
          COMMAND ${TEST_DRIVER}
          Execute
          ${PYTHON_EXECUTABLE} ${ORCHESTRATOR}/launcher/maja.py
          "--input" "${MAJADATA_TV_VENUS_INPUT}/VE-V-L2INIT-002-F-N-VENUS-IFPDS/"
          "--output" "${MAJA_TEST_OUTPUT_ROOT}/TV_CROSS_WRITING/VE"
          "-m" "L2INIT"
          "-l" "DEBUG"
          "-p" "VENUS_MUSCATE"
          "--stylesheet" "${MAJADATA_TV_VENUS_INPUT}/VE-V-L2INIT-002-F-N-VENUS-IFPDS/styleSheet_Overloading_GIPPs_Values.xsl"
          )
set_tests_properties(pyTv-S2-L2INIT-CROSSWRITING-VENUS-VENUSMUSCATE PROPERTIES TIMEOUT 300000)
