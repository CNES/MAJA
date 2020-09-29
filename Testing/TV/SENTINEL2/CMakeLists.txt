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
SET(MAJADATA_TV_SENTINEL2_BASELINE ${MAJADATA_TV}/SENTINEL2/Baseline)

if(ENABLE_TV)
    file(MAKE_DIRECTORY ${MAJA_TEST_OUTPUT_ROOT}/TV_SENTINEL2)
    file(MAKE_DIRECTORY ${MAJA_TEST_OUTPUT_ROOT}/TV_SENTINEL2/SENTINEL2_INIT)
    file(MAKE_DIRECTORY ${MAJA_TEST_OUTPUT_ROOT}/TV_SENTINEL2/SENTINEL2_NOMINAL)
    file(MAKE_DIRECTORY ${MAJA_TEST_OUTPUT_ROOT}/TV_SENTINEL2/SENTINEL2_BACKWARD)
endif()


add_test(NAME pyTv-S2-L2NOMINAL-001-SENTINEL2-ALGO_CHAIN
          COMMAND ${TEST_DRIVER}
          Execute
          ${PYTHON_EXECUTABLE} ${ORCHESTRATOR}/launcher/maja.py
          "--input" "${MAJADATA_TV_SENTINEL2_INPUT}/S2-V-L2NOMINAL-001-F-N-SENTINEL2-ALGO/"
          "--output" "${MAJA_TEST_OUTPUT_ROOT}/TV_SENTINEL2/SENTINEL2_NOMINAL"
          "-m" "L2NOMINAL"
          "-l" "DEBUG"
          "--stylesheet" "${MAJADATA_TV_SENTINEL2_INPUT}/S2-V-L2NOMINAL-001-F-N-SENTINEL2-ALGO/styleSheet_Overloading_GIPPs_Values.xsl"
          "--TileId" "36JTT"
          "-p"  "SENTINEL2"
          )
set_tests_properties(pyTv-S2-L2NOMINAL-001-SENTINEL2-ALGO_CHAIN PROPERTIES TIMEOUT 300000)

add_test(NAME pyTv-S2-L2NOMINAL-001-SENTINEL2-ALGO_COMP_IMAGE
        COMMAND ${TEST_DRIVER_MAJA}
        ${MAJA_BINARY_DIR}/bin/vnsTestMainLauncher.sh
        "--from-directory-compare-all-images" ${EPSILON_3}
        ${MAJADATA_TV_SENTINEL2_BASELINE}/S2-V-L2NOMINAL-001-F-N-SENTINEL2-ALGO/S2A_OPER_SSC_L2VALD_36JTT____20160924.DBL.DIR
        ${MAJADATA_TV_SENTINEL2_INPUT}/S2-V-L2NOMINAL-001-F-N-SENTINEL2-ALGO/
        ${MAJA_TEST_OUTPUT_ROOT}/TV_SENTINEL2/SENTINEL2_NOMINAL/S2A_OPER_SSC_L2VALD_36JTT____20160924.DBL.DIR
        "--recursively" "true" "--output-directory"
        ${MAJA_TEST_OUTPUT_ROOT}/TV_SENTINEL2/SENTINEL2_NOMINAL/testing-compare-output-directory
        ${MAJA_BINARY_DIR}/bin/vnsDummyProgram.sh
        )

add_test(NAME  pyTv-S2-L2NOMINAL-001-SENTINEL2-ALGO_COMP_ASCII
          COMMAND ${TEST_DRIVER_MAJA}
          ${MAJA_BINARY_DIR}/bin/vnsTestMainLauncher.sh
          "--from-directory-compare-all-ascii" ${EPSILON_3}
          ${MAJADATA_TV_SENTINEL2_BASELINE}/S2-V-L2NOMINAL-001-F-N-SENTINEL2-ALGO/S2A_OPER_SSC_L2VALD_36JTT____20160924.DBL.DIR
          ${MAJADATA_TV_SENTINEL2_INPUT}/S2-V-L2NOMINAL-001-F-N-SENTINEL2-ALGO/
          ${MAJA_TEST_OUTPUT_ROOT}/TV_SENTINEL2/SENTINEL2_NOMINAL/S2A_OPER_SSC_L2VALD_36JTT____20160924.DBL.DIR
          "--exclude-filename-with-key-matching" "8" "MAJAAdminConfig" "MAJAUserConfig" "PMC_LxREPT" "PMC_L1REPT" "PMC_L2REPT" "PMC_L3REPT" "perfos.xml" "JobOrder.xml"
          "--ignore-lines-with" "4" "Creator_Version" "Creation_Date" "Date_Time" "Chain_Version"
          "--recursively" "true" "--output-directory"
          ${MAJA_TEST_OUTPUT_ROOT}/TV_SENTINEL2/SENTINEL2_INIT/testing-compare-output-directory
          ${MAJA_BINARY_DIR}/bin/vnsDummyProgram.sh
          )

