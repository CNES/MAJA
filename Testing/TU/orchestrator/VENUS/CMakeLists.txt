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


set(TEST_DRIVER ${CMAKE_INSTALL_PREFIX}/bin/otbTestDriver.sh)
set(VNS_TEST_DRIVER ${MAJA_BINARY_DIR}/bin/vnsTestMainLauncher.sh)
SET(PREFIX "Venus")

add_test(NAME pyTu${PREFIX}_L1ImageInfo
         COMMAND ${TEST_DRIVER}
         Execute
         ${PYTHON_EXECUTABLE} ${TEST_ORCHESTRATOR}/test_maja_l1_image_info.py
         "-i" ${MAJADATA_TU_VENUS_INPUT}/VE-V-L2INIT-001-F-N-VENUS-CHILE/
         )


add_test(NAME pyTu${PREFIX}_L1ImageFileReader
          COMMAND ${TEST_DRIVER}
          Execute
          ${PYTHON_EXECUTABLE} ${TEST_ORCHESTRATOR}/test_maja_l1_image_reader.py
          "-i" ${MAJADATA_TU_VENUS_INPUT}/VE-V-L2INIT-001-F-N-VENUS-CHILE
          "-o" ${MAJA_TEST_OUTPUT_ROOT}/TU/VENUS
          )


add_test(NAME pyTu${PREFIX}_L1ImageReader_L1PIX
          COMMAND ${VNS_TEST_DRIVER}
          --compare-image ${EPSILON_4}
          ${MAJADATA_TU_VENUS_BASELINE}/L1PIXImageList_R1.tif
          ${MAJA_TEST_OUTPUT_ROOT}/TU/VENUS/L1PIXImageList_R1.tif
          "--output-directory"
          ${MAJA_TEST_OUTPUT_ROOT}/TU/VENUS/testing-compare-output-directory
          ${MAJA_BINARY_DIR}/bin/vnsDummyProgram.sh
          )


add_test(NAME pyTu${PREFIX}_L1ImageReader_L2PIX
          COMMAND ${VNS_TEST_DRIVER}
          --compare-image ${EPSILON_4}
          ${MAJADATA_TU_VENUS_BASELINE}//L2PIXImageList_R1.tif
          ${MAJA_TEST_OUTPUT_ROOT}/TU/VENUS/L2PIXImageList_R1.tif
          "--output-directory"
          ${MAJA_TEST_OUTPUT_ROOT}/TU/VENUS/testing-compare-output-directory
          ${MAJA_BINARY_DIR}/bin/vnsDummyProgram.sh
          )


add_test(NAME pyTu${PREFIX}_L1ImageReader_EDGSub
          COMMAND ${VNS_TEST_DRIVER}
          --compare-image ${EPSILON_4}
          ${MAJADATA_TU_VENUS_BASELINE}/IPEDGSubOutput.tif
          ${MAJA_TEST_OUTPUT_ROOT}/TU/VENUS/IPEDGSubOutput.tif
          "--output-directory"
          ${MAJA_TEST_OUTPUT_ROOT}/TU/VENUS/testing-compare-output-directory
          ${MAJA_BINARY_DIR}/bin/vnsDummyProgram.sh
          )

add_test(NAME pyTu${PREFIX}_L1ImageReader_L2TOA
          COMMAND ${VNS_TEST_DRIVER}
          --compare-image ${EPSILON_4}
          ${MAJADATA_TU_VENUS_BASELINE}/L2TOAImageList_R1.tif
          ${MAJA_TEST_OUTPUT_ROOT}/TU/VENUS/L2TOAImageList_R1.tif
          "--output-directory"
          ${MAJA_TEST_OUTPUT_ROOT}/TU/VENUS/testing-compare-output-directory
          ${MAJA_BINARY_DIR}/bin/vnsDummyProgram.sh
          )

add_test(NAME pyTu${PREFIX}_L1ImageReader_TOASub
          COMMAND ${VNS_TEST_DRIVER}
          --compare-image ${EPSILON_4}
          ${MAJADATA_TU_VENUS_BASELINE}/IPTOASubOutput.tif
          ${MAJA_TEST_OUTPUT_ROOT}/TU/VENUS/IPTOASubOutput.tif
          "--output-directory"
          ${MAJA_TEST_OUTPUT_ROOT}/TU/VENUS/testing-compare-output-directory
          ${MAJA_BINARY_DIR}/bin/vnsDummyProgram.sh
          )


add_test(NAME pyTu${PREFIX}_L1ImageReader_L2EDG
          COMMAND ${VNS_TEST_DRIVER}
          --compare-image ${EPSILON_4}
          ${MAJADATA_TU_VENUS_BASELINE}/L2EDGOutputList_R1.tif
          ${MAJA_TEST_OUTPUT_ROOT}/TU/VENUS/L2EDGOutputList_R1.tif
          "--output-directory"
          ${MAJA_TEST_OUTPUT_ROOT}/TU/VENUS/testing-compare-output-directory
          ${MAJA_BINARY_DIR}/bin/vnsDummyProgram.sh
          )


add_test(NAME pyTu${PREFIX}_L1ImageReader_L2SAT
          COMMAND ${VNS_TEST_DRIVER}
          --compare-image ${EPSILON_4}
          ${MAJADATA_TU_VENUS_BASELINE}/L2SATMaskList_R1.tif
          ${MAJA_TEST_OUTPUT_ROOT}/TU/VENUS/L2SATMaskList_R1.tif
          "--output-directory"
          ${MAJA_TEST_OUTPUT_ROOT}/TU/VENUS/testing-compare-output-directory
          ${MAJA_BINARY_DIR}/bin/vnsDummyProgram.sh
          )

add_test(NAME pyTu${PREFIX}_L1ImageReader_SATSub
          COMMAND ${VNS_TEST_DRIVER}
          --compare-image ${EPSILON_4}
          ${MAJADATA_TU_VENUS_BASELINE}/IPSATSubOutput.tif
          ${MAJA_TEST_OUTPUT_ROOT}/TU/VENUS/IPSATSubOutput.tif
          "--output-directory"
          ${MAJA_TEST_OUTPUT_ROOT}/TU/VENUS/testing-compare-output-directory
          ${MAJA_BINARY_DIR}/bin/vnsDummyProgram.sh
          )


add_test(NAME pyTu${PREFIX}_L1ImageReader_SOL1
          COMMAND ${VNS_TEST_DRIVER}
          --compare-image ${EPSILON_4}
          ${MAJADATA_TU_VENUS_BASELINE}/SOL1Image.tif
          ${MAJA_TEST_OUTPUT_ROOT}/TU/VENUS/SOL1Image.tif
          "--output-directory"
          ${MAJA_TEST_OUTPUT_ROOT}/TU/VENUS/testing-compare-output-directory
          ${MAJA_BINARY_DIR}/bin/vnsDummyProgram.sh
          )

add_test(NAME pyTu${PREFIX}_L1ImageReader_SOL2
          COMMAND ${VNS_TEST_DRIVER}
          --compare-image ${EPSILON_4}
          ${MAJADATA_TU_VENUS_BASELINE}/SOL2Image.tif
          ${MAJA_TEST_OUTPUT_ROOT}/TU/VENUS/SOL2Image.tif
          "--output-directory"
          ${MAJA_TEST_OUTPUT_ROOT}/TU/VENUS/testing-compare-output-directory
          ${MAJA_BINARY_DIR}/bin/vnsDummyProgram.sh
          )


add_test(NAME pyTu${PREFIX}_L1ImageReader_DTMVIE
          COMMAND ${VNS_TEST_DRIVER}
          --compare-image ${EPSILON_4}
          ${MAJADATA_TU_VENUS_BASELINE}/DTMVieImage.tif
          ${MAJA_TEST_OUTPUT_ROOT}/TU/VENUS/DTMVieImage.tif
          "--output-directory"
          ${MAJA_TEST_OUTPUT_ROOT}/TU/VENUS/testing-compare-output-directory
          ${MAJA_BINARY_DIR}/bin/vnsDummyProgram.sh
          )

add_test(NAME pyTu${PREFIX}_L1ImageReader_ShadowVIE
          COMMAND ${VNS_TEST_DRIVER}
          --compare-image ${EPSILON_4}
          ${MAJADATA_TU_VENUS_BASELINE}/ShadowVIEImage.tif
          ${MAJA_TEST_OUTPUT_ROOT}/TU/VENUS/ShadowVIEImage.tif
          "--output-directory"
          ${MAJA_TEST_OUTPUT_ROOT}/TU/VENUS/testing-compare-output-directory
          ${MAJA_BINARY_DIR}/bin/vnsDummyProgram.sh
          )
