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
get_filename_component(MAJA_SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR} DIRECTORY)
get_filename_component(MAJA_SOURCE_DIR ${MAJA_SOURCE_DIR} DIRECTORY)
get_filename_component(CI_BASE_DIR ${MAJA_SOURCE_DIR} DIRECTORY)

set ( SUPERBUILD_SOURCE_DIR "${MAJA_SOURCE_DIR}/SuperBuild" )

set (CTEST_PROJECT_NAME "maja-cots")
set ( CTEST_BUILD_CONFIGURATION "Release" )

set ( CTEST_CMAKE_GENERATOR "Unix Makefiles" )
set ( CTEST_BUILD_FLAGS "-j8")

set ( PROJECT_SOURCE_DIR "${SUPERBUILD_SOURCE_DIR}" )
set ( CTEST_SOURCE_DIRECTORY "${SUPERBUILD_SOURCE_DIR}" )
set ( CTEST_BINARY_DIRECTORY "${CI_BASE_DIR}/build-cots" )

# Detect site ( bionic / xenial / rh6 / rh7 )
if(NOT DEFINED IMAGE_NAME)
  if(DEFINED ENV{IMAGE_NAME})
    set(IMAGE_NAME $ENV{IMAGE_NAME})
  endif()
endif()
set ( CTEST_SITE "${IMAGE_NAME}" )

set (CTEST_BUILD_NAME maja-cots)

set (CTEST_INSTALL_DIRECTORY "${CI_BASE_DIR}/cots")
set (CMAKE_COMMAND "cmake")

set(CTEST_GIT_UPDATE_CUSTOM "${CMAKE_COMMAND}" "-E" "echo" "No update")

# handle data and superbuild-archive dirs
if(NOT EXISTS "${CI_BASE_DIR}/data")
  file(MAKE_DIRECTORY "${CI_BASE_DIR}/data")
endif()
if(NOT EXISTS "${CI_BASE_DIR}/superbuild-archives")
  file(MAKE_DIRECTORY "${CI_BASE_DIR}/superbuild-archives")
endif()

ctest_start (Experimental)

ctest_update( SOURCE "${MAJA_SOURCE_DIR}"
  CAPTURE_CMAKE_ERROR _update_error)

if (_update_error EQUAL -1)
  message(STATUS "Ignoring ctest_update() errors")
endif()

set (COTS_CONFIGURE_OPTIONS
"-DCMAKE_BUILD_TYPE=${CTEST_BUILD_CONFIGURATION}"
"-DCMAKE_INSTALL_PREFIX:PATH=${CTEST_INSTALL_DIRECTORY}"
"-DDOWNLOAD_DIR:PATH=${CI_BASE_DIR}/superbuild-archives"
"-DMAJADATA_SOURCE_DIR:PATH=${CI_BASE_DIR}/data")

ctest_configure(BUILD "${CTEST_BINARY_DIRECTORY}"
    SOURCE "${SUPERBUILD_SOURCE_DIR}"
    OPTIONS "${COTS_CONFIGURE_OPTIONS}"
    RETURN_VALUE _configure_rv
    CAPTURE_CMAKE_ERROR _configure_error
    )

if ( NOT _configure_rv EQUAL 0 )
  #~ ctest_submit()
  message( FATAL_ERROR "An error occurs during ctest_configure. Dependencies might be buggy.")
endif()

# Build the COTS
ctest_build(BUILD "${CTEST_BINARY_DIRECTORY}"
            TARGET COTS
            RETURN_VALUE _build_rv
            NUMBER_ERRORS _build_nb_err
            CAPTURE_CMAKE_ERROR _build_error
            )

if ( ( NOT "${_build_nb_err}" EQUAL 0 ) OR ( "${_build_error}" EQUAL -1 ))
  # TODO: , just inspect failed projects and dump the out/err logs here
  message( FATAL_ERROR "An error occurs during ctest_build.")
endif()

# Maybe no need for a submit
#~ ctest_submit()
