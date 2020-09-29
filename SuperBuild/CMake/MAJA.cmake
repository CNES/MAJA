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
#                                        __  __    __     ____   __                                        #
#                                       (  \/  )  /__\   (_  _) /__\                                       #
#                                        )    (  /(__)\ .-_)(  /(__)\                                      #
#                                       (_/\/\_)(__)(__)\____)(__)(__)                                     #
#                                                                                                          #
#                                                                                                          #
############################################################################################################
# HISTORIQUE                                                                                               #
#                                                                                                          #
# VERSION : 3.2.1 : FA : LAIG-FA-MAJA-2872-CNES : 1 octobre 2018 : MAJA inclue les TV VENUS Muscate        #
# VERSION : 3.1.0 : DM : LAIG-DM-MAJA-2526-CNES : 9 avril 2018 : Montée de version de d'OTB 6.0            #
#                                                                                                          #
# FIN-HISTORIQUE                                                                                           #
#                                                                                                          #
# $Id$                                                                                                     #
#                                                                                                          #
############################################################################################################

set(MAJA_DEPENDS OTB PUGIXML LIBGD LIBXSLT LXML SCIPY)
build_projects(MAJA_DEPENDS)

add_custom_target(COTS
  DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/maja_cots_done.txt
  WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
  COMMENT "Built maja cots: ${MAJA_DEPENDS}"
  VERBATIM
  )

add_custom_command(OUTPUT maja_cots_done.txt
  COMMAND cmake -E touch maja_cots_done.txt
  DEPENDS ${MAJA_DEPENDS}
  )

ExternalProject_Add(MAJA
  DOWNLOAD_COMMAND ""
  PREFIX       MAJA
  TMP_DIR      MAJA/tmp
  STAMP_DIR    MAJA/stamp
  BINARY_DIR   MAJA/build      
  SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../"
  INSTALL_DIR ${SB_INSTALL_PREFIX}
  CMAKE_COMMAND ${SB_CMAKE_COMMAND}
  CMAKE_CACHE_ARGS
  ${SB_CMAKE_CACHE_ARGS}
  ${DISABLE_CXX_WARNING_OPTION}
  -DBUILD_TESTING:BOOL=${BUILD_TESTING}
  -DMAJA_ENABLE_COVERAGE:BOOL=OFF
  -DENABLE_TU:BOOL=${ENABLE_TU}
  -DENABLE_TV:BOOL=${ENABLE_TV}
  -DENABLE_TVA:BOOL=${ENABLE_TVA}
  -DMAJA_TEST_OUTPUT_ROOT:PATH=${MAJA_TEST_OUTPUT_ROOT}
  -DMAJADATA_SOURCE_DIR:PATH=${MAJADATA_SOURCE_DIR}
  -DPYTHON_EXECUTABLE:PATH=${SB_INSTALL_PREFIX}/bin/python3
  DEPENDS ${MAJA_DEPENDS}
  LOG_DOWNLOAD 1
  LOG_CONFIGURE 1
  LOG_BUILD 0
  LOG_INSTALL 1
  )

set(plugins_enabled)
FOREACH(plugin_name  ${MAJA_LIST_OF_SPECTRAL_CAMERA_AVAILABLE})
  MESSAGE(STATUS "${plugin_name}: ${PLUGIN_${plugin_name}}")
  if(PLUGIN_${plugin_name})
    list(APPEND plugins_enabled "${plugin_name}")
  endif()
endforeach()
message (STATUS "List of plugins enabled: ${plugins_enabled}")
