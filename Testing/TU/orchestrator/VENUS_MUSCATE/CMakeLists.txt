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
set(PREFIX "VenusMuscate")



add_test(NAME pyTu${PREFIX}_L1ImageInfo
         COMMAND ${TEST_DRIVER}
         Execute
         ${PYTHON_EXECUTABLE} ${TEST_ORCHESTRATOR}/test_maja_l1_image_info.py
         "-i" ${MAJADATA_TU_VENUS_MUSCATE_INPUT}/L1Reader/VE-P-L2INIT-002-F-N-VENUS_MUSCATE-GENERAL
          )

add_test(NAME pyTu${PREFIX}_L1ImageReader
          COMMAND ${TEST_DRIVER}
          --compare-n-images ${EPSILON_4} 12
          #IPEDGSubOutput
          ${MAJADATA_TU_VENUS_MUSCATE_BASELINE_L1Reader}/IPEDGSubOutput.tif
          ${MAJA_TEST_OUTPUT_ROOT}/TU/VENUS_MUSCATE/IPEDGSubOutput.tif

          #IPAOTSubOutput
          ${MAJADATA_TU_VENUS_MUSCATE_BASELINE_L1Reader}/IPTOASubOutput.tif
          ${MAJA_TEST_OUTPUT_ROOT}/TU/VENUS_MUSCATE/IPTOASubOutput.tif

          # IPSATSubOutput
          ${MAJADATA_TU_VENUS_MUSCATE_BASELINE_L1Reader}/IPSATSubOutput.tif
          ${MAJA_TEST_OUTPUT_ROOT}/TU/VENUS_MUSCATE/IPSATSubOutput.tif

          #CLAImage
          ${MAJADATA_TU_VENUS_MUSCATE_BASELINE_L1Reader}/CLAImage.tif
          ${MAJA_TEST_OUTPUT_ROOT}/TU/VENUS_MUSCATE/CLAImage.tif

          # L2EDGOutputList
          ${MAJADATA_TU_VENUS_MUSCATE_BASELINE_L1Reader}/L2EDGOutputList_R1.tif
          ${MAJA_TEST_OUTPUT_ROOT}/TU/VENUS_MUSCATE/L2EDGOutputList_R1.tif
          #SOL1Image
          ${MAJADATA_TU_VENUS_MUSCATE_BASELINE_L1Reader}/SOL1Image.tif
          ${MAJA_TEST_OUTPUT_ROOT}/TU/VENUS_MUSCATE/SOL1Image.tif

          #SOL2Image
          ${MAJADATA_TU_VENUS_MUSCATE_BASELINE_L1Reader}/SOL2Image.tif
          ${MAJA_TEST_OUTPUT_ROOT}/TU/VENUS_MUSCATE/SOL2Image.tif

          #L2SATMaskList
          ${MAJADATA_TU_VENUS_MUSCATE_BASELINE_L1Reader}/L2SATMaskList_R1.tif
          ${MAJA_TEST_OUTPUT_ROOT}/TU/VENUS_MUSCATE/L2SATMaskList_R1.tif

          #L2TOAImageList
          ${MAJADATA_TU_VENUS_MUSCATE_BASELINE_L1Reader}/L2TOAImageList_R1.tif
          ${MAJA_TEST_OUTPUT_ROOT}/TU/VENUS_MUSCATE/L2TOAImageList_R1.tif

          #DTMVieImage
          ${MAJADATA_TU_VENUS_MUSCATE_BASELINE_L1Reader}/DTMVieImage.tif
          ${MAJA_TEST_OUTPUT_ROOT}/TU/VENUS_MUSCATE/DTMVieImage.tif

          #ShadowVIEImage
          ${MAJADATA_TU_VENUS_MUSCATE_BASELINE_L1Reader}/ShadowVIEImage.tif
          ${MAJA_TEST_OUTPUT_ROOT}/TU/VENUS_MUSCATE/ShadowVIEImage.tif

          #L2PIX
          ${MAJADATA_TU_VENUS_MUSCATE_BASELINE_L1Reader}/L2PIXImageList_R1.tif
          ${MAJA_TEST_OUTPUT_ROOT}/TU/VENUS_MUSCATE/L2PIXImageList_R1.tif

          Execute
          ${PYTHON_EXECUTABLE} ${TEST_ORCHESTRATOR}/test_maja_l1_image_reader.py
          "-i" ${MAJADATA_TU_VENUS_MUSCATE_INPUT}/L1Reader/VE-P-L2INIT-002-F-N-VENUS_MUSCATE-GENERAL
          "-o" ${MAJA_TEST_OUTPUT_ROOT}/TU/VENUS_MUSCATE
          )


add_test(NAME pyTu${PREFIX}_L2ImageWriter_chain
          COMMAND ${TEST_DRIVER}
          Execute
          ${PYTHON_EXECUTABLE} ${TEST_ORCHESTRATOR}/PythonTestDriver.py test_maja_l2_writer
          ${MAJADATA_TU_VENUS_MUSCATE_INPUT}/L1Reader/VE-P-L2INIT-002-F-N-VENUS_MUSCATE-GENERAL
          ${MAJADATA_TU_VENUS_MUSCATE_INPUT}/L2Writer/
          ${MAJA_TEST_OUTPUT_ROOT}/TU/VENUS_MUSCATE
          "1"
          "3"
          "0"
          )

add_test(NAME pyTu${PREFIX}_L2ImageWriter_comp_images
          COMMAND ${MAJA_BINARY_DIR}/bin/vnsTestMainLauncher.sh
          "--from-directory-compare-all-images" ${EPSILON_3}
          ${MAJADATA_TU_VENUS_MUSCATE_BASELINE_L2Writer}/VENUS-XS_20180201-033854-000_L2A_KRATIE_C_V1-0/
          ${MAJADATA_TU_VENUS_MUSCATE_INPUT}/L1Reader/VE-P-L2INIT-002-F-N-VENUS_MUSCATE-GENERAL
          ${MAJA_TEST_OUTPUT_ROOT}/TU/VENUS_MUSCATE/VENUS-XS_20180201-033854-000_L2A_KRATIE_C_V1-0/
          "--recursively" "true" "--output-directory"
          ${MAJA_TEST_OUTPUT_ROOT}/TU/VENUS_MUSCATE/testing-compare-output-directory
          ${MAJA_BINARY_DIR}/bin/vnsDummyProgram.sh
          )
set_tests_properties(pyTu${PREFIX}_L2ImageWriter_comp_images
    PROPERTIES DEPENDS pyTu${PREFIX}_L2ImageWriter_chain)


add_test(NAME pyTu${PREFIX}_L2ImageWriter_comp_ascii
          COMMAND ${MAJA_BINARY_DIR}/bin/vnsTestMainLauncher.sh
          "--from-directory-compare-all-ascii" ${EPSILON_3}
          ${MAJADATA_TU_VENUS_MUSCATE_BASELINE_L2Writer}/VENUS-XS_20180201-033854-000_L2A_KRATIE_C_V1-0/
          ${MAJADATA_TU_VENUS_MUSCATE_INPUT}/L1Reader/VE-P-L2INIT-002-F-N-VENUS_MUSCATE-GENERAL
          ${MAJA_TEST_OUTPUT_ROOT}/TU/VENUS_MUSCATE/VENUS-XS_20180201-033854-000_L2A_KRATIE_C_V1-0/
          "--exclude-filename-with-key-matching" "8" "MAJAAdminConfig" "MAJAUserConfig" "PMC_LxREPT" "PMC_L1REPT" "PMC_L2REPT" "PMC_L3REPT" "perfos.xml" "JobOrder.xml"
          "--ignore-lines-with" "4" "Creator_Version" "Creation_Date" "Date_Time" "Chain_Version"
          "--recursively" "true" "--output-directory"
          ${MAJA_TEST_OUTPUT_ROOT}/TU/VENUS_MUSCATE/testing-compare-output-directory
          ${MAJA_BINARY_DIR}/bin/vnsDummyProgram.sh
          )
set_tests_properties(pyTu${PREFIX}_L2ImageWriter_comp_ascii
    PROPERTIES DEPENDS pyTu${PREFIX}_L2ImageWriter_chain)



add_test(NAME pyTu${PREFIX}_Venus_L2HeaderWriter_cross
          COMMAND ${TEST_DRIVER}
          Execute ${PYTHON_EXECUTABLE}
          ${TEST_ORCHESTRATOR}/test_maja_l2_cross_writer.py
          ${MAJADATA_TU_VENUS_INPUT}/VE-V-L2INIT-001-F-N-VENUS-CHILE
          "${MAJA_TEST_OUTPUT_ROOT}/TU/VENUS_MUSCATE/cross"
          )



