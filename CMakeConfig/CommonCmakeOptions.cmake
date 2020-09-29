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


# Prefix for MAJA libraries name
SET(MAJA_LIBRARY_PREFIXNAME "Maja")

# /etc : Cond User and Admin: Pour installer les fichiers de conf user, donnes exemple
IF(NOT MAJA_INSTALL_CONF_USER_DIR)
  SET(MAJA_INSTALL_CONF_USER_DIR "etc/conf/user")
ENDIF() #NOT MAJA_INSTALL_CONF_USER_DIR)

IF(NOT MAJA_INSTALL_CONF_ADMIN_DIR)
  SET(MAJA_INSTALL_CONF_ADMIN_DIR "etc/conf/admin")
ENDIF() #NOT MAJA_INSTALL_CONF_ADMIN_DIR)

# Plugins
IF(NOT MAJA_INSTALL_SHARE_PLUGINS_DIR)
  SET(MAJA_INSTALL_SHARE_PLUGINS_DIR "share/config")
ENDIF(NOT MAJA_INSTALL_SHARE_PLUGINS_DIR)

# Validation (for MAJA-Validation compoent (Gipps)
IF(NOT MAJA_INSTALL_DATA_VALIDATION_DIR)
  SET(MAJA_INSTALL_DATA_VALIDATION_DIR "share/validation")
ENDIF() #NOT MAJA_INSTALL_DATA_VALIDATION_DIR)

set(SYSTEM_HOSTNAME)
execute_process(COMMAND hostname -s
  OUTPUT_VARIABLE SYSTEM_HOSTNAME
  RESULT_VARIABLE hostname_rv)

string(STRIP "${SYSTEM_HOSTNAME}" SYSTEM_HOSTNAME)
if(NOT hostname_rv EQUAL 0 OR NOT SYSTEM_HOSTNAME)
  message(FATAL_ERROR "cannot find system hostname. error=${hostname_rv}")
endif()

set(MAJA_TEST_OUTPUT_ROOT "/MAJA_SHARED_FOLDER/${SYSTEM_HOSTNAME}" CACHE PATH "input/output dir for tv testing")
option(ENABLE_TU "Build TU testing" OFF)
option(ENABLE_TVA "Build TVA testing" OFF)
option(ENABLE_TV "Build TV testing" OFF)


set(CMAKE_SKIP_BUILD_RPATH  FALSE)
set(CMAKE_BUILD_WITH_INSTALL_RPATH FALSE)
set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

list(FIND CMAKE_PLATFORM_IMPLICIT_LINK_DIRECTORIES "${CMAKE_INSTALL_PREFIX}/lib" isSystemDir)
if("${isSystemDir}" STREQUAL "-1")
  set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")
endif("${isSystemDir}" STREQUAL "-1")

option(BUILD_SHARED_LIBS "Build shared libraries." ON)
mark_as_advanced(BUILD_SHARED_LIBS)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Choose the type of build, options are: Debug Release RelWithDebInfo MinSizeRel" FORCE)
endif()
# BUILD_TESTING is created by default by CTest.cmake, and is ON by default.
option(BUILD_TESTING "Build testing." ON)
mark_as_advanced(BUILD_TESTING)

include(${CMAKE_CURRENT_LIST_DIR}/MAJAConfigurationSetting.cmake)

if(UNIX)
  execute_process(COMMAND uname -m
    ERROR_VARIABLE ev
    OUTPUT_VARIABLE MY_SYSTEM_ARCH
    OUTPUT_STRIP_TRAILING_WHITESPACE
    )
endif()
 
set(LINUX FALSE)
if( "${CMAKE_SYSTEM_NAME}"  MATCHES "Linux")
  set(LINUX TRUE)
endif()

# Define the MAJA-NG-Data dir
FIND_PATH(MAJADATA_SOURCE_DIR MAJA_DATA_README.txt HINTS "/MAJA_SHARED_FOLDER/MAJA-Data")
if(ENABLE_TVA)
  if(NOT MAJADATA_SOURCE_DIR OR NOT EXISTS "${MAJADATA_SOURCE_DIR}")
    message(FATAL_ERROR "MAJADATA_SOURCE_DIR doesn't exists or not set")
  endif()
  message(STATUS "MAJADATA_SOURCE_DIR       = ${MAJADATA_SOURCE_DIR}")
endif()

if(ENABLE_TV)
  if(NOT MAJADATA_SOURCE_DIR OR NOT EXISTS "${MAJADATA_SOURCE_DIR}")
    message(FATAL_ERROR "MAJADATA_SOURCE_DIR doesn't exists or not set")
  endif()
  message(STATUS "MAJADATA_SOURCE_DIR       = ${MAJADATA_SOURCE_DIR}")
endif()

if(ENABLE_TU)
  if(NOT MAJADATA_SOURCE_DIR OR NOT EXISTS "${MAJADATA_SOURCE_DIR}")
    message(FATAL_ERROR "MAJADATA_SOURCE_DIR doesn't exists or not set")
  endif()
  message(STATUS "MAJADATA_SOURCE_DIR       = ${MAJADATA_SOURCE_DIR}")
endif()
