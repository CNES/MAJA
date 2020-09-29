#
# Copyright (C) 2020 Centre National d'Etudes Spatiales (CNES)
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
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
#                                                                                                          #
# Author: CS Systemes d'Information  (France)                                                              #
#                                                                                                          #
############################################################################################################
# HISTORIQUE                                                                                               #
#                                                                                                          #
# VERSION : 2-0-0 : DM : LAIG-DM-MAC-146-CNES : 14 octobre 2011 : Modification pour prise en compte             #
#                                                            d'evolutions liees a MACCS                    #
#                                                                                                          #
# VERSION : 1-0-0 : <TypeFT> : <NumFT> : 15 novembre 2009 : Creation                                                           
#                                                                                                          #
# FIN-HISTORIQUE                                                                                           #
#                                                                                                          #
# $Id$
#                                                                                                          #
############################################################################################################

# Sources of non-templated classes.
SET(LIBRARY_NAME "DataIO")

FILE(GLOB FILES_SRCS "vns*.cxx" )

# Create library
ADD_LIBRARY(${MAJA_LIBRARY_PREFIXNAME}${LIBRARY_NAME}
  ${FILES_SRCS})
TARGET_LINK_LIBRARIES(${MAJA_LIBRARY_PREFIXNAME}${LIBRARY_NAME}
  ${MAJA_LIBRARY_PREFIXNAME}DataCommon
  ${MAJA_LIBRARY_PREFIXNAME}UtilitiesI3D
  ${MAJA_IMPORT_LIBRARIES} )

IF(MAJA_CORE_LIBRARY_PROPERTIES)
  SET_TARGET_PROPERTIES(${MAJA_LIBRARY_PREFIXNAME}${LIBRARY_NAME} PROPERTIES ${MAJA_CORE_LIBRARY_PROPERTIES})
ENDIF(MAJA_CORE_LIBRARY_PROPERTIES)

IF(NOT MAJA_INSTALL_NO_LIBRARIES)
  INSTALL(TARGETS ${MAJA_LIBRARY_PREFIXNAME}${LIBRARY_NAME}
    RUNTIME DESTINATION ${MAJA_INSTALL_BIN_DIR} COMPONENT RuntimeLibraries
    LIBRARY DESTINATION ${MAJA_INSTALL_LIB_DIR} COMPONENT RuntimeLibraries
    ARCHIVE DESTINATION ${MAJA_INSTALL_LIB_DIR} COMPONENT Development)
ENDIF(NOT MAJA_INSTALL_NO_LIBRARIES)

IF(NOT MAJA_INSTALL_NO_DEVELOPMENT)
  FILE(GLOB header_files   "${CMAKE_CURRENT_SOURCE_DIR}/*.h")
  FILE(GLOB template_files "${CMAKE_CURRENT_SOURCE_DIR}/*.txx")
  INSTALL(FILES ${header_files} ${template_files}
    DESTINATION ${MAJA_INSTALL_INCLUDE_DIR}/${LIBRARY_NAME}
    COMPONENT Development)
ENDIF(NOT MAJA_INSTALL_NO_DEVELOPMENT)



