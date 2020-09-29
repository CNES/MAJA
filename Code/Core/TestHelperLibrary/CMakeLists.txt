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
#                                                                                                          #
# Author: CS Systemes d'Information  (France)                                                              #
#                                                                                                          #
############################################################################################################
# HISTORIQUE                                                                                               #
#                                                                                                          #
# VERSION : 2.0.0 : DM : LAIG-DM-MAJA-153436-CS : 6 mars 2017 : Refactoring pour ameliorer la qualite      #
# VERSION : 5-1-0 : FA : LAIG-FA-MAC-144674-CS : 9 aout 2016 : Correction remise en forme, etc.            #
# VERSION : 4-0-0 : FA : LAIG-FA-MAC-117040-CS : 13 mars 2014 : Modifications mineures                     #
# VERSION : 2-0-0 : DM : LAIG-DM-MAC-146-CNES : 14 octobre 2011 : Modification pour prise en compte        #
#                                                            d'evolutions liees a MACCS                    #
#                                                                                                          #
# VERSION : 1-0-0 : <TypeFT> : <NumFT> : 17 decembre 2010 : Creation                                                           
#                                                                                                          #
# FIN-HISTORIQUE                                                                                           #
#                                                                                                          #
# $Id$
#                                                                                                          #
############################################################################################################


# Sources of non-templated classes.
SET(LIBRARY_NAME "TestHelperLibrary")

# Create library
ADD_LIBRARY(${MAJA_LIBRARY_PREFIXNAME}${LIBRARY_NAME} vnsTestHelper.cxx)
IF(MAJA_CORE_LIBRARY_PROPERTIES)
  SET_TARGET_PROPERTIES(${MAJA_LIBRARY_PREFIXNAME}${LIBRARY_NAME} PROPERTIES ${MAJA_CORE_LIBRARY_PROPERTIES})
ENDIF(MAJA_CORE_LIBRARY_PROPERTIES)
TARGET_LINK_LIBRARIES(${MAJA_LIBRARY_PREFIXNAME}${LIBRARY_NAME} ${OTB_LIBRARIES}
${MAJA_LIBRARY_PREFIXNAME}DataIO 
${MAJA_LIBRARY_PREFIXNAME}DataCommon
${MAJA_LIBRARY_PREFIXNAME}AlgorithmsCommon
) 


# Create the Launcher executable (used by TV and TVA environment validation (from MAJA-Validation)
ADD_EXECUTABLE(vnsTestMainLauncher vnsTestMainLauncher.cxx)
TARGET_LINK_LIBRARIES(vnsTestMainLauncher ${MAJA_LIBRARY_PREFIXNAME}${LIBRARY_NAME})

# Create the Dummy Program (used by TV and TVA)
SET(processing "vnsDummyProgram")
ADD_EXECUTABLE(${processing} ${processing}.cxx )
#TARGET_LINK_LIBRARIES(${processing} OTBImageIO OTBCommon)

CONFIGURE_FILE(     ${CMAKE_CURRENT_SOURCE_DIR}/LAUNCHER.processing.bash.cmake.in
                    ${CMAKE_CURRENT_BINARY_DIR}/${processing}.bash @ONLY)

# Installed only with DEVEL context = TV validation context
#    IF(NOT MAJA_INSTALL_NO_LIBRARIES)
IF(NOT MAJA_INSTALL_NO_DEVELOPMENT)
  INSTALL(TARGETS
    ${MAJA_LIBRARY_PREFIXNAME}${LIBRARY_NAME}
    vnsDummyProgram
    vnsTestMainLauncher
    RUNTIME DESTINATION ${MAJA_INSTALL_BIN_DIR}
    LIBRARY DESTINATION ${MAJA_INSTALL_LIB_DIR}
    ARCHIVE DESTINATION ${MAJA_INSTALL_LIB_DIR})

  FILE(GLOB header_files   "${CMAKE_CURRENT_SOURCE_DIR}/*.h")
  FILE(GLOB template_files "${CMAKE_CURRENT_SOURCE_DIR}/*.txx")
  INSTALL(FILES ${header_files} ${template_files}
    DESTINATION ${MAJA_INSTALL_INCLUDE_DIR}/${LIBRARY_NAME}
    COMPONENT Development)

ENDIF(NOT MAJA_INSTALL_NO_DEVELOPMENT)

