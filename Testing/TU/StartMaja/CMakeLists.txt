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

if(ENABLE_TU)
    file(MAKE_DIRECTORY ${MAJA_TEST_OUTPUT_ROOT}/TU/StartMaja)
endif()

set(TEST_DRIVER ${CMAKE_INSTALL_PREFIX}/bin/otbTestDriver.sh)

SET(PREFIX "StartMaja")
SET(FOLDER_CHAIN "Chain")
SET(FOLDER_COMMON "Common")
SET(FOLDER_PREPARE_MNT "prepare_mnt")


add_test(NAME pyTu${PREFIX}_${FOLDER_CHAIN}_AuxFile
	COMMAND ${TEST_DRIVER}
	Execute
	${PYTHON_EXECUTABLE} ${START_MAJA}/test/Chain/test_AuxFile.py -v
	WORKING_DIRECTORY ${MAJA_TEST_OUTPUT_ROOT}/TU/StartMaja
	)

add_test(NAME pyTu${PREFIX}_${FOLDER_CHAIN}_GippFile
	COMMAND ${TEST_DRIVER}
	Execute
	${PYTHON_EXECUTABLE} ${START_MAJA}/test/Chain/test_GippFile.py -v
	WORKING_DIRECTORY ${MAJA_TEST_OUTPUT_ROOT}/TU/StartMaja
	)

set_tests_properties(pyTu${PREFIX}_${FOLDER_CHAIN}_GippFile PROPERTIES TIMEOUT 300000)

add_test(NAME pyTu${PREFIX}_${FOLDER_CHAIN}_L8Product
	COMMAND ${TEST_DRIVER}
	Execute
	${PYTHON_EXECUTABLE} ${START_MAJA}/test/Chain/test_L8Product.py -v
	WORKING_DIRECTORY ${MAJA_TEST_OUTPUT_ROOT}/TU/StartMaja
	)

add_test(NAME pyTu${PREFIX}_${FOLDER_CHAIN}_Product
	COMMAND ${TEST_DRIVER}
	Execute
	${PYTHON_EXECUTABLE} ${START_MAJA}/test/Chain/test_Product.py -v
	WORKING_DIRECTORY ${MAJA_TEST_OUTPUT_ROOT}/TU/StartMaja
	)

add_test(NAME pyTu${PREFIX}_${FOLDER_CHAIN}_S2Product
	COMMAND ${TEST_DRIVER}
	Execute
	${PYTHON_EXECUTABLE} ${START_MAJA}/test/Chain/test_S2Product.py -v
	WORKING_DIRECTORY ${MAJA_TEST_OUTPUT_ROOT}/TU/StartMaja
	)

add_test(NAME pyTu${PREFIX}_${FOLDER_CHAIN}_VSProduct
	COMMAND ${TEST_DRIVER}
	Execute
	${PYTHON_EXECUTABLE} ${START_MAJA}/test/Chain/test_VSProduct.py -v
	WORKING_DIRECTORY ${MAJA_TEST_OUTPUT_ROOT}/TU/StartMaja
	)

add_test(NAME pyTu${PREFIX}_${FOLDER_CHAIN}_Workplan
	COMMAND ${TEST_DRIVER}
	Execute
	${PYTHON_EXECUTABLE} ${START_MAJA}/test/Chain/test_Workplan.py -v
	WORKING_DIRECTORY ${MAJA_TEST_OUTPUT_ROOT}/TU/StartMaja
	)

add_test(NAME pyTu${PREFIX}_${FOLDER_CHAIN}_DummyFiles
	COMMAND ${TEST_DRIVER}
	Execute
	${PYTHON_EXECUTABLE} ${START_MAJA}/test/Chain/test_DummyFiles.py -v
	WORKING_DIRECTORY ${MAJA_TEST_OUTPUT_ROOT}/TU/StartMaja
	)


add_test(NAME pyTu${PREFIX}_${FOLDER_COMMON}_FileSystem
	COMMAND ${TEST_DRIVER}
	Execute
	 ${PYTHON_EXECUTABLE} ${START_MAJA}/test/Common/test_FileSystem.py -v
	WORKING_DIRECTORY ${MAJA_TEST_OUTPUT_ROOT}/TU/StartMaja
	)

add_test(NAME pyTu${PREFIX}_${FOLDER_COMMON}_ImageIO
	COMMAND ${TEST_DRIVER}
	Execute
	 ${PYTHON_EXECUTABLE} ${START_MAJA}/test/Common/test_ImageIO.py -v
	WORKING_DIRECTORY ${MAJA_TEST_OUTPUT_ROOT}/TU/StartMaja
	)

add_test(NAME pyTu${PREFIX}_${FOLDER_COMMON}_XMLTools
	COMMAND ${TEST_DRIVER}
	Execute
	 ${PYTHON_EXECUTABLE} ${START_MAJA}/test/Common/test_XMLTools.py -v
	WORKING_DIRECTORY ${MAJA_TEST_OUTPUT_ROOT}/TU/StartMaja
	)

add_test(NAME pyTu${PREFIX}_${FOLDER_COMMON}_GDalDatasetWrapper
	COMMAND ${TEST_DRIVER}
	Execute
	 ${PYTHON_EXECUTABLE} ${START_MAJA}/test/Common/test_GDalDatasetWrapper.py -v
	WORKING_DIRECTORY ${MAJA_TEST_OUTPUT_ROOT}/TU/StartMaja
	)

add_test(NAME pyTu${PREFIX}_${FOLDER_COMMON}_ImageApps
	COMMAND ${TEST_DRIVER}
	Execute
	 ${PYTHON_EXECUTABLE} ${START_MAJA}/test/Common/test_ImageApps.py -v
	WORKING_DIRECTORY ${MAJA_TEST_OUTPUT_ROOT}/TU/StartMaja
	)

add_test(NAME pyTu${PREFIX}_${FOLDER_COMMON}_ImageTools
	COMMAND ${TEST_DRIVER}
	Execute
	 ${PYTHON_EXECUTABLE} ${START_MAJA}/test/Common/test_ImageTools.py -v
	WORKING_DIRECTORY ${MAJA_TEST_OUTPUT_ROOT}/TU/StartMaja
	)

add_test(NAME pyTu${PREFIX}_${FOLDER_PREPARE_MNT}_MNTBase
	COMMAND ${TEST_DRIVER}
	Execute
	 ${PYTHON_EXECUTABLE} ${START_MAJA}/test/prepare_mnt/mnt/test_MNTBase.py -v
	WORKING_DIRECTORY ${MAJA_TEST_OUTPUT_ROOT}/TU/StartMaja
	)

add_test(NAME pyTu${PREFIX}_${FOLDER_PREPARE_MNT}_SRTM
	COMMAND ${TEST_DRIVER}
	Execute
	 ${PYTHON_EXECUTABLE} ${START_MAJA}/test/prepare_mnt/mnt/test_SRTM.py -v
	WORKING_DIRECTORY ${MAJA_TEST_OUTPUT_ROOT}/TU/StartMaja
	)

add_test(NAME pyTu${PREFIX}_${FOLDER_PREPARE_MNT}_EuDEM
	COMMAND ${TEST_DRIVER}
	Execute
	 ${PYTHON_EXECUTABLE} ${START_MAJA}/test/prepare_mnt/mnt/test_EuDEM.py -v
	WORKING_DIRECTORY ${MAJA_TEST_OUTPUT_ROOT}/TU/StartMaja
	)

add_test(NAME pyTu${PREFIX}_Start_maja
	COMMAND ${TEST_DRIVER}
	Execute
	 ${PYTHON_EXECUTABLE} ${START_MAJA}/test/test_Start_maja.py -v
	WORKING_DIRECTORY ${MAJA_TEST_OUTPUT_ROOT}/TU/StartMaja
	)
