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
SET(PREFIX "Landsat8")

add_test(NAME pyTu${PREFIX}_L1ImageInfo
          COMMAND ${TEST_DRIVER}
          Execute
          ${PYTHON_EXECUTABLE} ${TEST_ORCHESTRATOR}/test_maja_l1_image_info.py
          "-i" ${MAJADATA_TU_LANDSAT8_INPUT}/L8-V-L2INIT-001-F-N-LANDSAT8-GENERAL
          )


add_test(NAME pyTu${PREFIX}_L1ImageFileReader
          COMMAND ${TEST_DRIVER}
          Execute
          ${PYTHON_EXECUTABLE} ${TEST_ORCHESTRATOR}/test_maja_l1_image_reader.py
          "-i" ${MAJADATA_TU_LANDSAT8_INPUT}/L8-V-L2INIT-001-F-N-LANDSAT8-GENERAL
          "-o" ${MAJA_TEST_OUTPUT_ROOT}/TU/LANDSAT8
          )


add_test(NAME pyTu${PREFIX}_L1ImageReader_L1TOACirrus
          COMMAND ${VNS_TEST_DRIVER}
         --compare-image ${EPSILON_4}
         ${MAJADATA_TU_LANDSAT8_BASELINE}/L1TOACirrusImage.tif
         ${MAJA_TEST_OUTPUT_ROOT}/TU/LANDSAT8/L1TOACirrusImage.tif
         "--output-directory"
          ${MAJA_TEST_OUTPUT_ROOT}/TU/LANDSAT8/testing-compare-output-directory
          ${MAJA_BINARY_DIR}/bin/vnsDummyProgram.sh
         )



add_test(NAME pyTu${PREFIX}_L1ImageReader_L2PIX
          COMMAND ${VNS_TEST_DRIVER}
          --compare-image ${EPSILON_4}
          ${MAJADATA_TU_LANDSAT8_BASELINE}/L2PIXImageList_R1.tif
          ${MAJA_TEST_OUTPUT_ROOT}/TU/LANDSAT8/L2PIXImageList_R1.tif
          "--output-directory"
          ${MAJA_TEST_OUTPUT_ROOT}/TU/LANDSAT8/testing-compare-output-directory
          ${MAJA_BINARY_DIR}/bin/vnsDummyProgram.sh
          )

add_test(NAME pyTu${PREFIX}_L1ImageReader_L2SAT
          COMMAND ${VNS_TEST_DRIVER}
          --compare-image ${EPSILON_4}
          ${MAJADATA_TU_LANDSAT8_BASELINE}/L2SATMaskList_R1.tif
          ${MAJA_TEST_OUTPUT_ROOT}/TU/LANDSAT8/L2SATMaskList_R1.tif
          "--output-directory"
          ${MAJA_TEST_OUTPUT_ROOT}/TU/LANDSAT8/testing-compare-output-directory
          ${MAJA_BINARY_DIR}/bin/vnsDummyProgram.sh
          )

add_test(NAME pyTu${PREFIX}_L1ImageReader_L2TOA
          COMMAND ${VNS_TEST_DRIVER}
          --compare-image ${EPSILON_4}
          ${MAJADATA_TU_LANDSAT8_BASELINE}/L2TOAImageList_R1.tif
          ${MAJA_TEST_OUTPUT_ROOT}/TU/LANDSAT8/L2TOAImageList_R1.tif
          "--output-directory"
          ${MAJA_TEST_OUTPUT_ROOT}/TU/LANDSAT8/testing-compare-output-directory
          ${MAJA_BINARY_DIR}/bin/vnsDummyProgram.sh
          )


add_test(NAME pyTu${PREFIX}_L1ImageReader_EDGSub
          COMMAND ${VNS_TEST_DRIVER}
          --compare-image ${EPSILON_4}
          ${MAJADATA_TU_LANDSAT8_BASELINE}/IPEDGSubOutput.tif
          ${MAJA_TEST_OUTPUT_ROOT}/TU/LANDSAT8/IPEDGSubOutput.tif
          "--output-directory"
          ${MAJA_TEST_OUTPUT_ROOT}/TU/LANDSAT8/testing-compare-output-directory
          ${MAJA_BINARY_DIR}/bin/vnsDummyProgram.sh
          )


add_test(NAME pyTu${PREFIX}_L1ImageReader_L2EDG
          COMMAND ${VNS_TEST_DRIVER}
          --compare-image ${EPSILON_4}
          ${MAJADATA_TU_LANDSAT8_BASELINE}/L2EDGOutputList_R1.tif
          ${MAJA_TEST_OUTPUT_ROOT}/TU/LANDSAT8/L2EDGOutputList_R1.tif
          "--output-directory"
          ${MAJA_TEST_OUTPUT_ROOT}/TU/LANDSAT8/testing-compare-output-directory
          ${MAJA_BINARY_DIR}/bin/vnsDummyProgram.sh
          )


add_test(NAME pyTu${PREFIX}_L1ImageReader_TOASub
          COMMAND ${VNS_TEST_DRIVER}
          --compare-image ${EPSILON_4}
          ${MAJADATA_TU_LANDSAT8_BASELINE}/IPTOASubOutput.tif
          ${MAJA_TEST_OUTPUT_ROOT}/TU/LANDSAT8/IPTOASubOutput.tif
          "--output-directory"
          ${MAJA_TEST_OUTPUT_ROOT}/TU/LANDSAT8/testing-compare-output-directory
          ${MAJA_BINARY_DIR}/bin/vnsDummyProgram.sh
          )


add_test(NAME pyTu${PREFIX}_L1ImageReader_SATSub
          COMMAND ${VNS_TEST_DRIVER}
          --compare-image ${EPSILON_4}
          ${MAJADATA_TU_LANDSAT8_BASELINE}/IPSATSubOutput.tif
          ${MAJA_TEST_OUTPUT_ROOT}/TU/LANDSAT8/IPSATSubOutput.tif
          "--output-directory"
          ${MAJA_TEST_OUTPUT_ROOT}/TU/LANDSAT8/testing-compare-output-directory
          ${MAJA_BINARY_DIR}/bin/vnsDummyProgram.sh
          )


add_test(NAME pyTu${PREFIX}_L1ImageReader_CLA
          COMMAND ${VNS_TEST_DRIVER}
          --compare-image ${EPSILON_4}
          ${MAJADATA_TU_LANDSAT8_BASELINE}/CLAImage.tif
          ${MAJA_TEST_OUTPUT_ROOT}/TU/LANDSAT8/CLAImage.tif
          "--output-directory"
          ${MAJA_TEST_OUTPUT_ROOT}/TU/LANDSAT8/testing-compare-output-directory
          ${MAJA_BINARY_DIR}/bin/vnsDummyProgram.sh
          )


add_test(NAME pyTu${PREFIX}_L1ImageReader_SOL1
          COMMAND ${VNS_TEST_DRIVER}
          --compare-image ${EPSILON_4}
          ${MAJADATA_TU_LANDSAT8_BASELINE}/SOL1Image.tif
          ${MAJA_TEST_OUTPUT_ROOT}/TU/LANDSAT8/SOL1Image.tif
          "--output-directory"
          ${MAJA_TEST_OUTPUT_ROOT}/TU/LANDSAT8/testing-compare-output-directory
          ${MAJA_BINARY_DIR}/bin/vnsDummyProgram.sh
          )


add_test(NAME pyTu${PREFIX}_L1ImageReader_VIE
          COMMAND ${VNS_TEST_DRIVER}
          --compare-image ${EPSILON_4}
          ${MAJADATA_TU_LANDSAT8_BASELINE}/ShadowVIEImage.tif
          ${MAJA_TEST_OUTPUT_ROOT}/TU/LANDSAT8/ShadowVIEImage.tif
          "--output-directory"
          ${MAJA_TEST_OUTPUT_ROOT}/TU/LANDSAT8/testing-compare-output-directory
          ${MAJA_BINARY_DIR}/bin/vnsDummyProgram.sh
          )





