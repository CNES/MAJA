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
# VERSION : 1-0-0 : <TypeFT> : <NumFT> : 15 novembre 2009 : Creation                                                           
#                                                                                                          #
# FIN-HISTORIQUE                                                                                           #
#                                                                                                          #
# $Id$
#                                                                                                          #
############################################################################################################

project(MAJATesting)

INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/LibMiscellaneous)

#-----------------------------------------------------------------------------
# Define the Input/output data dir
SET(INTERFACES                      ${MAJA_SOURCE_DIR}/Interfaces)
SET(INTERFACES_BINARY               ${MAJA_BINARY_DIR}/Interfaces)
SET(INTERFACESDATA                  ${INTERFACES}/Data)

SET(PLUGIN_BASE_DIR                 ${MAJA_SOURCE_DIR}/Plugins)
SET(PLUGIN_BASE_BINARY_DIR          ${MAJA_BINARY_DIR}/Plugins)

SET(INTERFACES_PLUGIN_COMMON         ${PLUGIN_BASE_DIR}/Common/Interfaces)
SET(INTERFACES_PLUGIN_COMMON_CONF    ${INTERFACES_PLUGIN_COMMON}/Config)

SET(SCHEMAS                         ${INTERFACES}/Schemas)

SET(CONFDATA                        ${INTERFACES}/Config)
SET(CONFDATA_BINARY                 ${INTERFACES_BINARY}/Config)
SET(CONF_SCHEMAS                    ${SCHEMAS}/Maccs)

SET(SCRIPTS_SOURCE_DIR              ${MAJA_SOURCE_DIR}/Code/Scripts)
SET(SCRIPTS_BINARY_DIR              ${MAJA_BINARY_DIR}/Code/Scripts)

# Official ICD Schemas
SET(ICD_OFFICIAL_SCHEMAS            ${SCHEMAS})
SET(ICD_OFFICIAL_SCHEMAS_VENUS      ${ICD_OFFICIAL_SCHEMAS}/VENUS/Venus)
SET(ICD_OFFICIAL_SCHEMAS_MAJA      ${ICD_OFFICIAL_SCHEMAS}/Maja)

SET(ICD_OFFICIAL_SCHEMAS_SENTINEL2_GPP  ${ICD_OFFICIAL_SCHEMAS}/Sentinel2/GS2-ICD-GS-80-CNES/GS2_DIMAP)
SET(TEMP                            ${PROJECT_BINARY_DIR}/Temporary)
FILE(MAKE_DIRECTORY ${TEMP})

SET(ORCHESTRATOR                    ${MAJA_SOURCE_DIR}/orchestrator)
SET(START_MAJA                   ${MAJA_SOURCE_DIR}/StartMaja)
SET(TEST_ORCHESTRATOR ${ORCHESTRATOR}/tests)
SET(MAJA_TEST_OUTPUT                ${MAJA_TEST_OUTPUT_ROOT})


#-----------------------------------------------------------------------------
# Define base of large input data for TVA, TV and TU

SET(MAJADATA_TU ${MAJADATA_SOURCE_DIR}/TU)
SET(MAJADATA_TV ${MAJADATA_SOURCE_DIR}/TV)
SET(MAJADATA_TVA ${MAJADATA_SOURCE_DIR}/TVA)

SET(MAJADATA_TV_SENTINEL2_MUSCATE_INPUT ${MAJADATA_TV}/SENTINEL2_MUSCATE/Input)
SET(MAJADATA_TV_SENTINEL2_INPUT ${MAJADATA_TV}/SENTINEL2/Input)
SET(MAJADATA_TV_VENUS_MUSCATE_INPUT ${MAJADATA_TV}/VENUS_MUSCATE/Input)
SET(MAJADATA_TV_LANDSAT8_MUSCATE_INPUT ${MAJADATA_TV}/LANDSAT8_MUSCATE/Input)
SET(MAJADATA_TV_LANDSAT8_INPUT ${MAJADATA_TV}/LANDSAT8/Input)
SET(MAJADATA_TV_VENUS_INPUT ${MAJADATA_TV}/VENUS/Input)


SET(MAJADATA_TU_CODE ${MAJADATA_TU}/Code)
SET(INPUT_MAJADATATU_IMAGES                    ${MAJADATA_TU_CODE}/Images)
SET(INPUT_MAJADATATU_FILES                     ${MAJADATA_TU_CODE}/Files)
SET(INPUT_MAJADATATU_IMAGES_DTM  ${INPUT_MAJADATATU_IMAGES}/dtm)
SET(BASELINEDATATU_FILES              ${MAJADATA_TU_CODE}/Baseline/Files)

#-----------------------------------------------------------------------------
#Tolerance sur diff pixel image
SET(NOTOL 0.0)
SET(EPSILON_2 0.01)
SET(EPSILON_3 0.001)
SET(EPSILON_4 0.0001)
SET(EPSILON_5 0.00001)
SET(EPSILON_6 0.000001)
SET(EPSILON_7 0.0000001)
SET(EPSILON_8 0.00000001)
SET(EPSILON_12 0.000000000001)

#-----------------------------------------------------------------------------
IF(MAJA_BINARY_DIR)
    ADD_SUBDIRECTORY(LibMiscellaneous)
    if(ENABLE_TU)
        # Add testing subdirectories
        message(STATUS "Testing/ directory is added because ENABLE_TU=${ENABLE_TU}.")
        ADD_SUBDIRECTORY(TU)
    else()
        message(STATUS "Testing/ directory is exlcuded because ENABLE_TU=${ENABLE_TU}.")
    endif()
    if(ENABLE_TV)
        # Add testing subdirectories
        message(STATUS "Testing/ directory is added because ENABLE_TV=${ENABLE_TV}.")
        ADD_SUBDIRECTORY(TV)
        install(DIRECTORY ${MAJA_BINARY_DIR}/share DESTINATION ${CMAKE_INSTALL_PREFIX})
    else()
        message(STATUS "Testing/ directory is exlcuded because ENABLE_TV=${ENABLE_TV}.")
    endif()
    if(ENABLE_TVA)
        # Add testing subdirectories
        message(STATUS "Testing/ directory is added because ENABLE_TVA=${ENABLE_TVA}.")
        ADD_SUBDIRECTORY(TVA)
    else()
        message(STATUS "Testing/ directory is exlcuded because ENABLE_TVA=${ENABLE_TVA}.")
    endif()
ENDIF(MAJA_BINARY_DIR)
