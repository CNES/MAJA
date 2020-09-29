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
get_filename_component(MAJA_SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR} DIRECTORY)

set (CTEST_PROJECT_NAME "maja-pkg")
set ( CTEST_BUILD_CONFIGURATION "Release" )

set ( CTEST_CMAKE_GENERATOR "Unix Makefiles" )
set ( CTEST_BUILD_FLAGS "-j3")

set ( PROJECT_SOURCE_DIR "${MAJA_SOURCE_DIR}" )
set ( CTEST_SOURCE_DIRECTORY "${MAJA_SOURCE_DIR}/Packaging" )
set ( CTEST_BINARY_DIRECTORY "${MAJA_SOURCE_DIR}/build-pkg/" )

find_program(CTEST_GIT_COMMAND NAMES git git.cmd)

# Detect sha1
execute_process(COMMAND ${CTEST_GIT_COMMAND} log -1 --pretty=format:%h
                WORKING_DIRECTORY ${MAJA_SOURCE_DIR}
                OUTPUT_VARIABLE ci_short_sha)

# Detect site ( bionic / xenial / rh6 / rh7 )
if(NOT DEFINED IMAGE_NAME)
  if(DEFINED ENV{IMAGE_NAME})
    set(IMAGE_NAME $ENV{IMAGE_NAME})
  endif()
endif()
set ( CTEST_SITE "${IMAGE_NAME}" )

# detect version
file(STRINGS "${MAJA_SOURCE_DIR}/CMakeConfig/MAJAConfigurationSetting.cmake" _content_ver REGEX "MAJA_VERSION \"[0-9]+\\.[0-9]+\\.[0-9]+\"")
string(REGEX REPLACE ".+\"([0-9]+\\.[0-9]+\\.[0-9]+)\".+" "\\1" MAJA_VERSION "${_content_ver}")
message(STATUS "Maja version: ${MAJA_VERSION}")

set (CTEST_BUILD_NAME ${ci_short_sha})

set (CTEST_INSTALL_DIRECTORY "${MAJA_SOURCE_DIR}/pkg")
set (CMAKE_COMMAND "cmake")

set(CTEST_GIT_UPDATE_CUSTOM "${CMAKE_COMMAND}" "-E" "echo" "No update")

# TODO : git clone maja-data 
if(NOT EXISTS "${MAJA_SOURCE_DIR}/data")
  file(MAKE_DIRECTORY "${MAJA_SOURCE_DIR}/data")
endif()
if(NOT EXISTS "${MAJA_SOURCE_DIR}/superbuild-archives")
  file(MAKE_DIRECTORY "${MAJA_SOURCE_DIR}/superbuild-archives")
endif()

ctest_start (Experimental)

ctest_update( SOURCE "${MAJA_SOURCE_DIR}" )

set (MAJA_PKG_CONFIGURE_OPTIONS
"-DCMAKE_BUILD_TYPE=${CTEST_BUILD_CONFIGURATION}"
"-DCMAKE_INSTALL_PREFIX:PATH=${CTEST_INSTALL_DIRECTORY}"
"-DMAJADATA_SOURCE_DIR:PATH=${MAJA_SOURCE_DIR}/data"
"-DSUPERBUILD_BINARY_DIR:PATH=${MAJA_SOURCE_DIR}/build"
"-DDOWNLOAD_DIR:PATH=${MAJA_SOURCE_DIR}/superbuild-archives"
"-DSUPERBUILD_INSTALL_DIR:PATH=/opt/cots"
"-DWITH_VALIDATION:BOOL=ON"
)

ctest_configure(BUILD "${CTEST_BINARY_DIRECTORY}"
    SOURCE "${CTEST_SOURCE_DIRECTORY}"
    OPTIONS "${MAJA_PKG_CONFIGURE_OPTIONS}"
    RETURN_VALUE _configure_rv
    CAPTURE_CMAKE_ERROR _configure_error
    )

if ( NOT _configure_rv EQUAL 0 )
  #~ ctest_submit()
  message( FATAL_ERROR "An error occurs during ctest_configure. Dependencies might be buggy.")
endif()

# Build package
ctest_build(BUILD "${CTEST_BINARY_DIRECTORY}"
            TARGET install
            RETURN_VALUE _build_rv
            NUMBER_ERRORS _build_nb_err
            CAPTURE_CMAKE_ERROR _build_error
            )

if ( ( NOT "${_build_nb_err}" EQUAL 0 ) OR ( "${_build_error}" EQUAL -1 ))
  #~ ctest_submit()
  message( FATAL_ERROR "An error occurs during ctest_build.")
endif()

#~ ctest_submit()
