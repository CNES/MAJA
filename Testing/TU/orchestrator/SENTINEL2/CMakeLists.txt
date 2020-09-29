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
SET(PREFIX "Sentinel2")

add_test(NAME pyTu${PREFIX}_GenerateEDGImagefromTOAImages
          COMMAND ${TEST_DRIVER}
          --compare-n-images ${EPSILON_4} 3
          ${MAJADATA_TU_SENTINEL2_BASELINE}/TestGenerateEDGImageFromTOAImages/MaskOrMask12.tif
          ${MAJA_TEST_OUTPUT_ROOT}/TU/SENTINEL2/MaskOrMask12.tif
          ${MAJADATA_TU_SENTINEL2_BASELINE}/TestGenerateEDGImageFromTOAImages/IPEDGMaskL2_R1.tif
          ${MAJA_TEST_OUTPUT_ROOT}/TU/SENTINEL2/IPEDGMaskL2_R1.tif
          ${MAJADATA_TU_SENTINEL2_BASELINE}/TestGenerateEDGImageFromTOAImages/IPEDGMaskL2_R2.tif
          ${MAJA_TEST_OUTPUT_ROOT}/TU/SENTINEL2/IPEDGMaskL2_R2.tif
          Execute
          ${PYTHON_EXECUTABLE} ${TEST_ORCHESTRATOR}/PythonTestDriver.py test_maja_sentinel2_generate_edg_images_from_toa
          ${MAJADATA_TU_SENTINEL2_INPUT}/TestGenerateEDGImageFromTOAImages/
          ${MAJADATA_TU_SENTINEL2_INPUT}/S2-V-L2INIT-001-F-N-SENTINEL2-GENERAL/S2__TEST_AUX_REFDE2_36JTT____5001.HDR
          ${MAJA_TEST_OUTPUT_ROOT}/TU/SENTINEL2/
          )

add_test(NAME pyTu${PREFIX}_L1ImageInfo
         COMMAND ${TEST_DRIVER}
         Execute
         ${PYTHON_EXECUTABLE} ${TEST_ORCHESTRATOR}/test_maja_l1_image_info.py
         "-i" ${MAJADATA_TU_SENTINEL2_INPUT}/S2-V-L2INIT-001-F-N-SENTINEL2-GENERAL/
         "-t" "36JTT"

          )
          
add_test(NAME pyTu${PREFIX}_L1ImageFileReader
          COMMAND ${TEST_DRIVER}
          --compare-n-images ${EPSILON_4} 18

          #IPEDGSubOutput
          ${MAJADATA_TU_SENTINEL2_BASELINE}/L1Reader/IPEDGSubOutput.tif
          ${MAJA_TEST_OUTPUT_ROOT}/TU/SENTINEL2/IPEDGSubOutput.tif

          #L1TOACirrus
          ${MAJADATA_TU_SENTINEL2_BASELINE}/L1Reader/L1TOACirrusImage.tif
          ${MAJA_TEST_OUTPUT_ROOT}/TU/SENTINEL2/L1TOACirrusImage.tif

          #IPAOTSubOutput
          ${MAJADATA_TU_SENTINEL2_BASELINE}/L1Reader/IPTOASubOutput.tif
          ${MAJA_TEST_OUTPUT_ROOT}/TU/SENTINEL2/IPTOASubOutput.tif

          # IPSATSubOutput
          ${MAJADATA_TU_SENTINEL2_BASELINE}/L1Reader//IPSATSubOutput.tif
          ${MAJA_TEST_OUTPUT_ROOT}/TU/SENTINEL2/IPSATSubOutput.tif

          #CLAImage
          ${MAJADATA_TU_SENTINEL2_BASELINE}/L1Reader//CLAImage.tif
          ${MAJA_TEST_OUTPUT_ROOT}/TU/SENTINEL2/CLAImage.tif

          # L2EDGOutputList
          ${MAJADATA_TU_SENTINEL2_BASELINE}/L1Reader//L2EDGOutputList_R1.tif
          ${MAJA_TEST_OUTPUT_ROOT}/TU/SENTINEL2/L2EDGOutputList_R1.tif
          ${MAJADATA_TU_SENTINEL2_BASELINE}/L1Reader//L2EDGOutputList_R2.tif
          ${MAJA_TEST_OUTPUT_ROOT}/TU/SENTINEL2/L2EDGOutputList_R2.tif

          #SOL1Image
          ${MAJADATA_TU_SENTINEL2_BASELINE}/L1Reader//SOL1Image.tif
          ${MAJA_TEST_OUTPUT_ROOT}/TU/SENTINEL2/SOL1Image.tif

          #L2SATMaskList
          ${MAJADATA_TU_SENTINEL2_BASELINE}/L1Reader//L2SATMaskList_R1.tif
          ${MAJA_TEST_OUTPUT_ROOT}/TU/SENTINEL2/L2SATMaskList_R1.tif
          ${MAJADATA_TU_SENTINEL2_BASELINE}/L1Reader//L2SATMaskList_R2.tif
          ${MAJA_TEST_OUTPUT_ROOT}/TU/SENTINEL2/L2SATMaskList_R2.tif

          #L2TOAImageList
          ${MAJADATA_TU_SENTINEL2_BASELINE}/L1Reader//L2TOAImageList_R1.tif
          ${MAJA_TEST_OUTPUT_ROOT}/TU/SENTINEL2/L2TOAImageList_R1.tif
          ${MAJADATA_TU_SENTINEL2_BASELINE}/L1Reader//L2TOAImageList_R2.tif
          ${MAJA_TEST_OUTPUT_ROOT}/TU/SENTINEL2/L2TOAImageList_R2.tif

          #DTMVieImage
          ${MAJADATA_TU_SENTINEL2_BASELINE}/L1Reader//DTMVieImage.tif
          ${MAJA_TEST_OUTPUT_ROOT}/TU/SENTINEL2/DTMVieImage.tif

          #ShadowVIEImage
          ${MAJADATA_TU_SENTINEL2_BASELINE}/L1Reader//ShadowVIEImage.tif
          ${MAJA_TEST_OUTPUT_ROOT}/TU/SENTINEL2/ShadowVIEImage.tif

          #L2PIX
          ${MAJADATA_TU_SENTINEL2_BASELINE}/L1Reader//L2PIXImageList_R1.tif
          ${MAJA_TEST_OUTPUT_ROOT}/TU/SENTINEL2/L2PIXImageList_R1.tif
          ${MAJADATA_TU_SENTINEL2_BASELINE}/L1Reader//L2PIXImageList_R2.tif
          ${MAJA_TEST_OUTPUT_ROOT}/TU/SENTINEL2/L2PIXImageList_R2.tif

          # AngleZoneMaskList
          ${MAJADATA_TU_SENTINEL2_BASELINE}/L1Reader//AngleZoneMaskList_R1.tif
          ${MAJA_TEST_OUTPUT_ROOT}/TU/SENTINEL2/AngleZoneMaskList_R1.tif
          ${MAJADATA_TU_SENTINEL2_BASELINE}/L1Reader//AngleZoneMaskList_R2.tif
          ${MAJA_TEST_OUTPUT_ROOT}/TU/SENTINEL2/AngleZoneMaskList_R2.tif

          Execute
          ${PYTHON_EXECUTABLE} ${TEST_ORCHESTRATOR}/test_maja_l1_image_reader.py
          "-i" ${MAJADATA_TU_SENTINEL2_INPUT}/S2-V-L2INIT-001-F-N-SENTINEL2-GENERAL
          "-o" ${MAJA_TEST_OUTPUT_ROOT}/TU/SENTINEL2
          "-t" "36JTT"
          )
set_tests_properties(pyTu${PREFIX}_L1ImageFileReader PROPERTIES TIMEOUT 300000)
     
