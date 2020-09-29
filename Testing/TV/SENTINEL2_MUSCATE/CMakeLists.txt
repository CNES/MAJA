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


set(TEST_DRIVER_MAJA ${MAJA_BINARY_DIR}/bin/vnsTestMainLauncher.sh)
set(TEST_DRIVER ${CMAKE_INSTALL_PREFIX}/bin/otbTestDriver.sh)
SET(MAJADATA_TV_SENTINEL2_MUSCATE_BASELINE ${MAJADATA_TV}/SENTINEL2_MUSCATE/Baseline)

if(ENABLE_TV)
    file(MAKE_DIRECTORY ${MAJA_TEST_OUTPUT_ROOT}/TV_SENTINEL2_MUSCATE)
    file(MAKE_DIRECTORY ${MAJA_TEST_OUTPUT_ROOT}/TV_SENTINEL2_MUSCATE/SENTINEL2_MUSCATE_INIT)
    file(MAKE_DIRECTORY ${MAJA_TEST_OUTPUT_ROOT}/TV_SENTINEL2_MUSCATE/SENTINEL2_MUSCATE_INIT_CAMS)
    file(MAKE_DIRECTORY ${MAJA_TEST_OUTPUT_ROOT}/TV_SENTINEL2_MUSCATE/SENTINEL2_MUSCATE_INIT_NEWCAMS)
    file(MAKE_DIRECTORY ${MAJA_TEST_OUTPUT_ROOT}/TV_SENTINEL2_MUSCATE/SENTINEL2_MUSCATE_NOMINAL)
    file(MAKE_DIRECTORY ${MAJA_TEST_OUTPUT_ROOT}/TV_SENTINEL2_MUSCATE/SENTINEL2_MUSCATE_BACKWARD)
endif()


add_test(NAME pyTv-S2-L2INIT-011-SENTINEL2-MUSCATE-GENERAL_CHAIN
          COMMAND ${TEST_DRIVER}
          Execute
          ${PYTHON_EXECUTABLE} ${ORCHESTRATOR}/launcher/maja.py
          "--input" "${MAJADATA_TV_SENTINEL2_MUSCATE_INPUT}/S2-V-L2INIT-011-F-N-SENTINEL2_MUSCATE-GENERAL/"
          "--output" "${MAJA_TEST_OUTPUT_ROOT}/TV_SENTINEL2_MUSCATE/SENTINEL2_MUSCATE_INIT"
          "-m" "L2INIT"
          "-l" "DEBUG"
          "--stylesheet" "${MAJADATA_TV_SENTINEL2_MUSCATE_INPUT}/S2-V-L2INIT-011-F-N-SENTINEL2_MUSCATE-GENERAL/styleSheet_Overloading_GIPPs_Values.xsl"
          )
set_tests_properties(pyTv-S2-L2INIT-011-SENTINEL2-MUSCATE-GENERAL_CHAIN PROPERTIES TIMEOUT 300000)

add_test(NAME pyTv-S2-L2INIT-011-SENTINEL2-MUSCATE-GENERAL_COMP_IMAGE
        COMMAND ${TEST_DRIVER_MAJA}
        "--from-directory-compare-all-images" ${EPSILON_3}
        ${MAJADATA_TV_SENTINEL2_MUSCATE_BASELINE}/S2-V-L2INIT-011-F-N-SENTINEL2_MUSCATE-GENERAL/SENTINEL2B_20171008-105012-463_L2A_T31TCH_C_V1-0/
        ${MAJADATA_TV_SENTINEL2_MUSCATE_INPUT}/S2-V-L2INIT-011-F-N-SENTINEL2_MUSCATE-GENERAL/
        ${MAJA_TEST_OUTPUT_ROOT}/TV_SENTINEL2_MUSCATE/SENTINEL2_MUSCATE_INIT/SENTINEL2B_20171008-105012-463_L2A_T31TCH_C_V1-0/
        "--recursively" "true" "--output-directory"
        ${MAJA_TEST_OUTPUT_ROOT}/TV_SENTINEL2_MUSCATE/SENTINEL2_MUSCATE_INIT/testing-compare-output-directory
        ${MAJA_BINARY_DIR}/bin/vnsDummyProgram.sh
        )

add_test(NAME  pyTv-S2-L2INIT-011-SENTINEL2-MUSCATE-GENERAL_COMP_ASCII
          COMMAND ${TEST_DRIVER_MAJA}
          "--from-directory-compare-all-ascii" ${EPSILON_3}
          ${MAJADATA_TV_SENTINEL2_MUSCATE_BASELINE}/S2-V-L2INIT-011-F-N-SENTINEL2_MUSCATE-GENERAL/SENTINEL2B_20171008-105012-463_L2A_T31TCH_C_V1-0/
          ${MAJADATA_TV_SENTINEL2_MUSCATE_INPUT}/S2-V-L2INIT-011-F-N-SENTINEL2_MUSCATE-GENERAL/
          ${MAJA_TEST_OUTPUT_ROOT}/TV_SENTINEL2_MUSCATE/SENTINEL2_MUSCATE_INIT/SENTINEL2B_20171008-105012-463_L2A_T31TCH_C_V1-0/
          "--exclude-filename-with-key-matching" "8" "MAJAAdminConfig" "MAJAUserConfig" "PMC_LxREPT" "PMC_L1REPT" "PMC_L2REPT" "PMC_L3REPT" "perfos.xml" "JobOrder.xml"
          "--ignore-lines-with" "4" "Creator_Version" "Creation_Date" "Date_Time" "Chain_Version"
          "--recursively" "true" "--output-directory"
          ${MAJA_TEST_OUTPUT_ROOT}/TV_SENTINEL2_MUSCATE/SENTINEL2_MUSCATE_INIT/testing-compare-output-directory
          ${MAJA_BINARY_DIR}/bin/vnsDummyProgram.sh
          )

add_test(NAME pyTv-S2-L2INIT-012-SENTINEL2-MUSCATE-CAMS_CHAIN
          COMMAND ${TEST_DRIVER}
          Execute
          ${PYTHON_EXECUTABLE} ${ORCHESTRATOR}/launcher/maja.py
          "--input" "${MAJADATA_TV_SENTINEL2_MUSCATE_INPUT}/S2-V-L2INIT-012-F-N-SENTINEL2_MUSCATE-CAMS/"
          "--output" "${MAJA_TEST_OUTPUT_ROOT}/TV_SENTINEL2_MUSCATE/SENTINEL2_MUSCATE_INIT_CAMS"
          "-m" "L2INIT"
          "-l" "DEBUG"
          "--stylesheet" "${MAJADATA_TV_SENTINEL2_MUSCATE_INPUT}/S2-V-L2INIT-012-F-N-SENTINEL2_MUSCATE-CAMS/styleSheet_Overloading_GIPPs_Values.xsl"
          )
set_tests_properties(pyTv-S2-L2INIT-012-SENTINEL2-MUSCATE-CAMS_CHAIN PROPERTIES TIMEOUT 300000)

add_test(NAME pyTv-S2-L2INIT-012-SENTINEL2-MUSCATE-CAMS_COMP_IMAGE
        COMMAND ${TEST_DRIVER_MAJA}
        "--from-directory-compare-all-images" ${EPSILON_3}
        ${MAJADATA_TV_SENTINEL2_MUSCATE_BASELINE}/S2-V-L2INIT-012-F-N-SENTINEL2_MUSCATE-CAMS/SENTINEL2B_20171008-105012-463_L2A_T31TCH_C_V1-0/
        ${MAJADATA_TV_SENTINEL2_MUSCATE_INPUT}/S2-V-L2INIT-012-F-N-SENTINEL2_MUSCATE-CAMS/
        ${MAJA_TEST_OUTPUT_ROOT}/TV_SENTINEL2_MUSCATE/SENTINEL2_MUSCATE_INIT_CAMS/SENTINEL2B_20171008-105012-463_L2A_T31TCH_C_V1-0/
        "--recursively" "true" "--output-directory"
        ${MAJA_TEST_OUTPUT_ROOT}/TV_SENTINEL2_MUSCATE/SENTINEL2_MUSCATE_INIT_CAMS/testing-compare-output-directory
        ${MAJA_BINARY_DIR}/bin/vnsDummyProgram.sh
        )

add_test(NAME  pyTv-S2-L2INIT-012-SENTINEL2-MUSCATE-CAMS_COMP_ASCII
          COMMAND ${TEST_DRIVER_MAJA}
          "--from-directory-compare-all-ascii" ${EPSILON_3}
          ${MAJADATA_TV_SENTINEL2_MUSCATE_BASELINE}/S2-V-L2INIT-012-F-N-SENTINEL2_MUSCATE-CAMS/SENTINEL2B_20171008-105012-463_L2A_T31TCH_C_V1-0/
          ${MAJADATA_TV_SENTINEL2_MUSCATE_INPUT}/S2-V-L2INIT-012-F-N-SENTINEL2_MUSCATE-CAMS/
          ${MAJA_TEST_OUTPUT_ROOT}/TV_SENTINEL2_MUSCATE/SENTINEL2_MUSCATE_INIT_CAMS/SENTINEL2B_20171008-105012-463_L2A_T31TCH_C_V1-0/
          "--exclude-filename-with-key-matching" "8" "MAJAAdminConfig" "MAJAUserConfig" "PMC_LxREPT" "PMC_L1REPT" "PMC_L2REPT" "PMC_L3REPT" "perfos.xml" "JobOrder.xml"
          "--ignore-lines-with" "4" "Creator_Version" "Creation_Date" "Date_Time" "Chain_Version"
          "--recursively" "true" "--output-directory"
          ${MAJA_TEST_OUTPUT_ROOT}/TV_SENTINEL2_MUSCATE/SENTINEL2_MUSCATE_INIT_CAMS/testing-compare-output-directory
          ${MAJA_BINARY_DIR}/bin/vnsDummyProgram.sh
          )



#
# New CAMS test
#
add_test(NAME pyTv-S2-L2INIT-012-SENTINEL2-MUSCATE-NEWCAMS_CHAIN
          COMMAND ${TEST_DRIVER}
          Execute
          ${PYTHON_EXECUTABLE} ${ORCHESTRATOR}/launcher/maja.py
          "--input" "${MAJADATA_TV_SENTINEL2_MUSCATE_INPUT}/S2-V-L2INIT-012-F-N-SENTINEL2_MUSCATE-NEWCAMS/"
          "--output" "${MAJA_TEST_OUTPUT_ROOT}/TV_SENTINEL2_MUSCATE/SENTINEL2_MUSCATE_INIT_NEWCAMS"
          "-m" "L2INIT"
          "-l" "DEBUG"
          "--stylesheet" "${MAJADATA_TV_SENTINEL2_MUSCATE_INPUT}/S2-V-L2INIT-012-F-N-SENTINEL2_MUSCATE-NEWCAMS/styleSheet_Overloading_GIPPs_Values.xsl"
          )
set_tests_properties(pyTv-S2-L2INIT-012-SENTINEL2-MUSCATE-NEWCAMS_CHAIN PROPERTIES TIMEOUT 300000)

add_test(NAME pyTv-S2-L2INIT-012-SENTINEL2-MUSCATE-NEWCAMS_COMP_IMAGE
        COMMAND ${TEST_DRIVER_MAJA}
        "--from-directory-compare-all-images" ${EPSILON_3}
        ${MAJADATA_TV_SENTINEL2_MUSCATE_BASELINE}/S2-V-L2INIT-012-F-N-SENTINEL2_MUSCATE-NEWCAMS/SENTINEL2B_20171008-105012-463_L2A_T31TCH_C_V1-0/
        ${MAJADATA_TV_SENTINEL2_MUSCATE_INPUT}/S2-V-L2INIT-012-F-N-SENTINEL2_MUSCATE-NEWCAMS/
        ${MAJA_TEST_OUTPUT_ROOT}/TV_SENTINEL2_MUSCATE/SENTINEL2_MUSCATE_INIT_NEWCAMS/SENTINEL2B_20171008-105012-463_L2A_T31TCH_C_V1-0/
        "--recursively" "true" "--output-directory"
        ${MAJA_TEST_OUTPUT_ROOT}/TV_SENTINEL2_MUSCATE/SENTINEL2_MUSCATE_INIT_NEWCAMS/testing-compare-output-directory
        ${MAJA_BINARY_DIR}/bin/vnsDummyProgram.sh
        )

add_test(NAME  pyTv-S2-L2INIT-012-SENTINEL2-MUSCATE-NEWCAMS_COMP_ASCII
          COMMAND ${TEST_DRIVER_MAJA}
          "--from-directory-compare-all-ascii" ${EPSILON_3}
          ${MAJADATA_TV_SENTINEL2_MUSCATE_BASELINE}/S2-V-L2INIT-012-F-N-SENTINEL2_MUSCATE-NEWCAMS/SENTINEL2B_20171008-105012-463_L2A_T31TCH_C_V1-0/
          ${MAJADATA_TV_SENTINEL2_MUSCATE_INPUT}/S2-V-L2INIT-012-F-N-SENTINEL2_MUSCATE-NEWCAMS/
          ${MAJA_TEST_OUTPUT_ROOT}/TV_SENTINEL2_MUSCATE/SENTINEL2_MUSCATE_INIT_NEWCAMS/SENTINEL2B_20171008-105012-463_L2A_T31TCH_C_V1-0/
          "--exclude-filename-with-key-matching" "8" "MAJAAdminConfig" "MAJAUserConfig" "PMC_LxREPT" "PMC_L1REPT" "PMC_L2REPT" "PMC_L3REPT" "perfos.xml" "JobOrder.xml"
          "--ignore-lines-with" "4" "Creator_Version" "Creation_Date" "Date_Time" "Chain_Version"
          "--recursively" "true" "--output-directory"
          ${MAJA_TEST_OUTPUT_ROOT}/TV_SENTINEL2_MUSCATE/SENTINEL2_MUSCATE_INIT_NEWCAMS/testing-compare-output-directory
          ${MAJA_BINARY_DIR}/bin/vnsDummyProgram.sh
          )

#
# End New CAMS test
#
add_test(NAME pyTv-S2-L2NOMINAL-011-F-N-SENTINEL2_MUSCATE-GENERAL_CHAIN
          COMMAND ${TEST_DRIVER}
          Execute
          ${PYTHON_EXECUTABLE} ${ORCHESTRATOR}/launcher/maja.py
          "--input" "${MAJADATA_TV_SENTINEL2_MUSCATE_INPUT}/S2-V-L2NOMINAL-011-F-N-SENTINEL2_MUSCATE-GENERAL/"
          "--output" "${MAJA_TEST_OUTPUT_ROOT}/TV_SENTINEL2_MUSCATE/SENTINEL2_MUSCATE_NOMINAL"
          "-m" "L2NOMINAL"
          "-l" "DEBUG"
          "--stylesheet" "${MAJADATA_TV_SENTINEL2_MUSCATE_INPUT}/S2-V-L2NOMINAL-011-F-N-SENTINEL2_MUSCATE-GENERAL/styleSheet_Overloading_GIPPs_Values.xsl"
          )
set_tests_properties(pyTv-S2-L2NOMINAL-011-F-N-SENTINEL2_MUSCATE-GENERAL_CHAIN PROPERTIES TIMEOUT 300000)

add_test(NAME pyTv-S2-L2NOMINAL-011-F-N-SENTINEL2_MUSCATE-GENERAL_COMP_IMAGE
        COMMAND ${TEST_DRIVER_MAJA}
        "--from-directory-compare-all-images" ${EPSILON_3}
        ${MAJADATA_TV_SENTINEL2_MUSCATE_BASELINE}/S2-V-L2NOMINAL-011-F-N-SENTINEL2_MUSCATE-GENERAL/SENTINEL2B_20171010-105012-463_L2A_T31TCH_C_V1-0/
        ${MAJADATA_TV_SENTINEL2_MUSCATE_INPUT}/S2-V-L2NOMINAL-011-F-N-SENTINEL2_MUSCATE-GENERAL/
        ${MAJA_TEST_OUTPUT_ROOT}/TV_SENTINEL2_MUSCATE/SENTINEL2_MUSCATE_NOMINAL/SENTINEL2B_20171010-105012-463_L2A_T31TCH_C_V1-0/
        "--recursively" "true" "--output-directory"
        ${MAJA_TEST_OUTPUT_ROOT}/TV_SENTINEL2_MUSCATE/SENTINEL2_MUSCATE_NOMINAL/testing-compare-output-directory
        ${MAJA_BINARY_DIR}/bin/vnsDummyProgram.sh
        )

add_test(NAME  pyTv-S2-L2NOMINAL-011-F-N-SENTINEL2_MUSCATE-GENERAL_COMP_ASCII
          COMMAND ${TEST_DRIVER_MAJA}
          "--from-directory-compare-all-ascii" ${EPSILON_3}
          ${MAJADATA_TV_SENTINEL2_MUSCATE_BASELINE}/S2-V-L2NOMINAL-011-F-N-SENTINEL2_MUSCATE-GENERAL/SENTINEL2B_20171010-105012-463_L2A_T31TCH_C_V1-0/
          ${MAJADATA_TV_SENTINEL2_MUSCATE_INPUT}/S2-V-L2NOMINAL-011-F-N-SENTINEL2_MUSCATE-GENERAL/
          ${MAJA_TEST_OUTPUT_ROOT}/TV_SENTINEL2_MUSCATE/SENTINEL2_MUSCATE_NOMINAL/SENTINEL2B_20171010-105012-463_L2A_T31TCH_C_V1-0/
          "--exclude-filename-with-key-matching" "8" "MAJAAdminConfig" "MAJAUserConfig" "PMC_LxREPT" "PMC_L1REPT" "PMC_L2REPT" "PMC_L3REPT" "perfos.xml" "JobOrder.xml"
          "--ignore-lines-with" "4" "Creator_Version" "Creation_Date" "Date_Time" "Chain_Version"
          "--recursively" "true" "--output-directory"
          ${MAJA_TEST_OUTPUT_ROOT}/TV_SENTINEL2_MUSCATE/SENTINEL2_MUSCATE_NOMINAL/testing-compare-output-directory
          ${MAJA_BINARY_DIR}/bin/vnsDummyProgram.sh
          )


add_test(NAME pyTv-S2-L2BACKWARD-011-F-N-SENTINEL2_MUSCATE-GENERAL_CHAIN
          COMMAND ${TEST_DRIVER}
          Execute
          ${PYTHON_EXECUTABLE} ${ORCHESTRATOR}/launcher/maja.py
          "--input" "${MAJADATA_TV_SENTINEL2_MUSCATE_INPUT}/S2-V-L2BACKWARD-011-F-N-SENTINEL2_MUSCATE-GENERAL/"
          "--output" "${MAJA_TEST_OUTPUT_ROOT}/TV_SENTINEL2_MUSCATE/SENTINEL2_MUSCATE_BACKWARD"
          "-m" "L2BACKWARD"
          "-l" "DEBUG"
          "--TileId" "15SVD"
          "--stylesheet" "${MAJADATA_TV_SENTINEL2_MUSCATE_INPUT}/S2-V-L2BACKWARD-011-F-N-SENTINEL2_MUSCATE-GENERAL/styleSheet_Overloading_GIPPs_Values.xsl"
          )
set_tests_properties(pyTv-S2-L2BACKWARD-011-F-N-SENTINEL2_MUSCATE-GENERAL_CHAIN PROPERTIES TIMEOUT 300000)

add_test(NAME pyTv-S2-L2BACKWARD-011-F-N-SENTINEL2_MUSCATE-GENERAL_COMP_IMAGE
        COMMAND ${TEST_DRIVER_MAJA}
        "--from-directory-compare-all-images" ${EPSILON_3}
        ${MAJADATA_TV_SENTINEL2_MUSCATE_BASELINE}/S2-V-L2BACKWARD-011-F-N-SENTINEL2_MUSCATE-GENERAL/SENTINEL2B_20171008-105012-463_L2A_T31TCH_C_V1-0/
        ${MAJADATA_TV_SENTINEL2_MUSCATE_INPUT}/S2-V-L2BACKWARD-011-F-N-SENTINEL2_MUSCATE-GENERAL/
        ${MAJA_TEST_OUTPUT_ROOT}/TV_SENTINEL2_MUSCATE/SENTINEL2_MUSCATE_BACKWARD/SENTINEL2B_20171008-105012-463_L2A_T31TCH_C_V1-0/
        "--recursively" "true" "--output-directory"
        ${MAJA_TEST_OUTPUT_ROOT}/TV_SENTINEL2_MUSCATE/SENTINEL2_MUSCATE_BACKWARD/testing-compare-output-directory
        ${MAJA_BINARY_DIR}/bin/vnsDummyProgram.sh
        )


add_test(NAME pyTv-S2-L2BACKWARD-011-F-N-SENTINEL2_MUSCATE-GENERAL_COMP_ASCII
        COMMAND ${TEST_DRIVER_MAJA}
        "--from-directory-compare-all-ascii" ${EPSILON_3}
        ${MAJADATA_TV_SENTINEL2_MUSCATE_BASELINE}/S2-V-L2BACKWARD-011-F-N-SENTINEL2_MUSCATE-GENERAL/SENTINEL2B_20171008-105012-463_L2A_T31TCH_C_V1-0/
        ${MAJADATA_TV_SENTINEL2_MUSCATE_INPUT}/S2-V-L2BACKWARD-011-F-N-SENTINEL2_MUSCATE-GENERAL/
        ${MAJA_TEST_OUTPUT_ROOT}/TV_SENTINEL2_MUSCATE/SENTINEL2_MUSCATE_BACKWARD/SENTINEL2B_20171008-105012-463_L2A_T31TCH_C_V1-0/
        "--exclude-filename-with-key-matching" "8" "MAJAAdminConfig" "MAJAUserConfig" "PMC_LxREPT" "PMC_L1REPT" "PMC_L2REPT" "PMC_L3REPT" "perfos.xml" "JobOrder.xml"
        "--ignore-lines-with" "4" "Creator_Version" "Creation_Date" "Date_Time" "Chain_Version"
        "--recursively" "true" "--output-directory"
        ${MAJA_TEST_OUTPUT_ROOT}/TV_SENTINEL2_MUSCATE/SENTINEL2_MUSCATE_BACKWARD/testing-compare-output-directory
        ${MAJA_BINARY_DIR}/bin/vnsDummyProgram.sh
        )
