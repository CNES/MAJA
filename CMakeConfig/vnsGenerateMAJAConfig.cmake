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
# VERSION : 1-0-0 : <TypeFT> : <NumFT> : 1 avril 2010 : Creation                                                           
#                                                                                                          #
# FIN-HISTORIQUE                                                                                           #
#                                                                                                          #
# $Id$
#                                                                                                          #
############################################################################################################

# Generate the MAJAConfig.cmake file in the build tree.  Also configure
# one for installation.  The file tells external projects how to use
# MAJA.

#-----------------------------------------------------------------------------
# Settings specific to the build tree.

# The "use" file.
SET(MAJA_USE_FILE ${PROJECT_BINARY_DIR}/UseMAJA.cmake)

# The library dependencies file.
SET(MAJA_LIBRARY_DEPENDS_FILE ${PROJECT_BINARY_DIR}/MAJALibraryDepends.cmake)

# Package directories (cmake files...)
SET(MAJA_CMAKEMODULE_DIRS_CONFIG ${PROJECT_SOURCE_DIR}/CMakeConfig)


# The build settings file.
SET(MAJA_BUILD_SETTINGS_FILE ${PROJECT_BINARY_DIR}/MAJABuildSettings.cmake)

# Library directory.
SET(MAJA_LIBRARY_DIRS_CONFIG ${MAJA_LIBRARY_PATH})
SET(MAJA_EXECUTABLE_DIRS_CONFIG ${MAJA_EXECUTABLE_PATH})

# Add OTB library dir
SET( MAJA_LIBRARY_DIRS_CONFIG ${MAJA_LIBRARY_DIRS_CONFIG} ${OTB_LIBRARY_DIRS} )

# Determine the include directories needed.
SET(MAJA_INCLUDE_DIRS_CONFIG
  ${MAJA_INCLUDE_DIRS_BUILD_TREE}
  ${MAJA_INCLUDE_DIRS_BUILD_TREE_CXX}
  ${MAJA_INCLUDE_DIRS_SYSTEM} 
)

# Add absolue includes paths
SET(MAJA_INCLUDE_DIRS_CONFIG ${MAJA_INCLUDE_DIRS_CONFIG}
        ${MAJA_INCLUDE_ABSOLUE_DIRS}  )


#-----------------------------------------------------------------------------
# Configure MAJAConfig.cmake for the BUILD tree.
CONFIGURE_FILE(${PROJECT_SOURCE_DIR}/CMakeConfig/MAJAConfig.cmake.in
               ${PROJECT_BINARY_DIR}/MAJAConfig.cmake @ONLY IMMEDIATE)

#-----------------------------------------------------------------------------
#-----------------------------------------------------------------------------
#-----------------------------------------------------------------------------
#-----------------------------------------------------------------------------
# Settings specific to the install tree.

# The library dependencies file.
SET(MAJA_LIBRARY_DEPENDS_FILE "\${MAJA_INSTALL_PREFIX}/${MAJA_INSTALL_PACKAGE_DIR}/MAJALibraryDepends.cmake")

# The "use" file.
SET(MAJA_USE_FILE \${MAJA_INSTALL_PREFIX}/${MAJA_INSTALL_PACKAGE_DIR}/UseMAJA.cmake)

# The build settings file.
SET(MAJA_BUILD_SETTINGS_FILE \${MAJA_INSTALL_PREFIX}/${MAJA_INSTALL_PACKAGE_DIR}/MAJABuildSettings.cmake)


# Include directories.
SET(MAJA_INCLUDE_DIRS_CONFIG \${MAJA_INSTALL_PREFIX}/${MAJA_INSTALL_INCLUDE_DIR})
FOREACH(DIR ${MAJA_INCLUDE_RELATIVE_DIRS})
  LIST(APPEND MAJA_INCLUDE_DIRS_CONFIG \${MAJA_INSTALL_PREFIX}/${MAJA_INSTALL_INCLUDE_DIR}/${DIR})
ENDFOREACH(DIR)
# Add root include dir
LIST(APPEND MAJA_INCLUDE_DIRS_CONFIG \${MAJA_INSTALL_PREFIX}/${MAJA_INSTALL_INCLUDE_DIR} )
IF(MAJA_INCLUDE_DIRS_SYSTEM)
  LIST(APPEND MAJA_INCLUDE_DIRS_CONFIG ${MAJA_INCLUDE_DIRS_SYSTEM})
ENDIF(MAJA_INCLUDE_DIRS_SYSTEM)

# Add absolue includes paths
SET(MAJA_INCLUDE_DIRS_CONFIG ${MAJA_INCLUDE_DIRS_CONFIG}
        ${MAJA_INCLUDE_ABSOLUE_DIRS}  )

# Include directories.
#       SET(MAJA_INCLUDE_DIRS_CONFIG 
#         ${MAJA_INCLUDE_DIRS_INSTALL_TREE}
#         ${MAJA_INCLUDE_DIRS_SYSTEM}
#       )

# Package directories (cmake files...)
set(MAJA_CMAKEMODULE_DIRS_CONFIG "\${MAJA_INSTALL_PREFIX}/${MAJA_INSTALL_CMAKEMODULE_DIR}")


# Link directories.
SET(MAJA_LIBRARY_DIRS_CONFIG "\${MAJA_INSTALL_PREFIX}/${MAJA_INSTALL_LIB_DIR}")
SET(MAJA_EXECUTABLE_DIRS_CONFIG "\${MAJA_INSTALL_PREFIX}/${MAJA_INSTALL_BIN_DIR}")

#-----------------------------------------------------------------------------
# Configure MAJAConfig.cmake for the INSTALL tree.

# Construct the proper number of GET_FILENAME_COMPONENT(... PATH)
# calls to compute the installation prefix.
STRING(REGEX REPLACE "/" ";" MAJA_INSTALL_PACKAGE_DIR_COUNT
  "${MAJA_INSTALL_PACKAGE_DIR}")
SET(MAJA_CONFIG_CODE "
# Compute the installation prefix from this ITKConfig.cmake file location.
GET_FILENAME_COMPONENT(MAJA_INSTALL_PREFIX \"\${CMAKE_CURRENT_LIST_FILE}\" PATH)")
FOREACH(p ${MAJA_INSTALL_PACKAGE_DIR_COUNT})
  SET(MAJA_CONFIG_CODE
    "${MAJA_CONFIG_CODE}\nGET_FILENAME_COMPONENT(MAJA_INSTALL_PREFIX \"\${MAJA_INSTALL_PREFIX}\" PATH)"
    )
ENDFOREACH(p)

# Add OTB library dir
SET( MAJA_LIBRARY_DIRS_CONFIG ${MAJA_LIBRARY_DIRS_CONFIG} ${OTB_LIBRARY_DIRS} )

CONFIGURE_FILE(${PROJECT_SOURCE_DIR}/CMakeConfig/MAJAConfig.cmake.in
               ${PROJECT_BINARY_DIR}/Utilities/MAJAConfig.cmake @ONLY IMMEDIATE)


