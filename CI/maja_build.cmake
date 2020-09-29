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
get_filename_component(CI_BASE_DIR ${MAJA_SOURCE_DIR} DIRECTORY)

set (CTEST_PROJECT_NAME "maja")
set ( CTEST_BUILD_CONFIGURATION "Release" )

set ( CTEST_CMAKE_GENERATOR "Ninja" )

set ( PROJECT_SOURCE_DIR "${MAJA_SOURCE_DIR}" )
set ( CTEST_SOURCE_DIRECTORY "${MAJA_SOURCE_DIR}" )
set ( CTEST_BINARY_DIRECTORY "${MAJA_SOURCE_DIR}/build/" )

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

set (CTEST_INSTALL_DIRECTORY "/opt/cots/maja/${MAJA_VERSION}")
set (CMAKE_COMMAND "cmake")

set(CTEST_GIT_UPDATE_CUSTOM "${CMAKE_COMMAND}" "-E" "echo" "No update")

if(NOT EXISTS "${MAJA_SOURCE_DIR}/data")
  file(MAKE_DIRECTORY "${MAJA_SOURCE_DIR}/data")
endif()

ctest_start (Experimental)

ctest_update( SOURCE "${MAJA_SOURCE_DIR}"
  CAPTURE_CMAKE_ERROR _update_error)

if (_update_error EQUAL -1)
  message(STATUS "Ignoring ctest_update() errors")
endif()

set (MAJA_CONFIGURE_OPTIONS
"-DCMAKE_BUILD_TYPE=${CTEST_BUILD_CONFIGURATION}"
"-DCMAKE_PREFIX_PATH:PATH=${CTEST_INSTALL_DIRECTORY}"
"-DCMAKE_INSTALL_PREFIX:PATH=${CTEST_INSTALL_DIRECTORY}"
"-DPYTHON_EXECUTABLE:PATH=${CTEST_INSTALL_DIRECTORY}/bin/python3"
"-DMAJADATA_SOURCE_DIR:PATH=${MAJA_SOURCE_DIR}/data"
"-DBUILD_TESTING:BOOL=ON"
"-DMAJA_ENABLE_COVERAGE:BOOL=OFF"
"-DENABLE_TU:BOOL=ON"
"-DENABLE_TVA:BOOL=ON"
"-DENABLE_TV:BOOL=ON"
"-DMAJA_TEST_OUTPUT_ROOT:PATH=${CTEST_BINARY_DIRECTORY}/Testing/Temporary"
#~ "-DPLUGIN_VENUS:BOOL=${PLUGIN_VENUS}"
#~ "-DPLUGIN_SENTINEL2:BOOL=${PLUGIN_SENTINEL2}"
#~ "-DPLUGIN_SENTINEL2_TM:BOOL=${PLUGIN_SENTINEL2_TM}"
#~ "-DPLUGIN_SENTINEL2_GPP:BOOL=${PLUGIN_SENTINEL2_GPP}"
#~ "-DPLUGIN_SENTINEL2_MUSCATE:BOOL=${PLUGIN_SENTINEL2_MUSCATE}"
#~ "-DPLUGIN_FORMOSAT_MUSCATE_PROTO:BOOL=${PLUGIN_FORMOSAT_MUSCATE_PROTO}"
#~ "-DPLUGIN_SPOT4_MUSCATE_PROTO:BOOL=${PLUGIN_SPOT4_MUSCATE_PROTO}"
#~ "-DPLUGIN_LANDSAT_MUSCATE_PROTO:BOOL=${PLUGIN_LANDSAT_MUSCATE_PROTO}"
#~ "-DPLUGIN_LANDSAT8_MUSCATE_PROTO:BOOL=${PLUGIN_LANDSAT8_MUSCATE_PROTO}"
#~ "-DPLUGIN_LANDSAT_MUSCATE:BOOL=${PLUGIN_LANDSAT_MUSCATE}"
#~ "-DPLUGIN_LANDSAT8_MUSCATE:BOOL=${PLUGIN_LANDSAT8_MUSCATE}"
#~ "-DPLUGIN_LANDSAT8:BOOL=${PLUGIN_LANDSAT8}"
)

ctest_configure(BUILD "${CTEST_BINARY_DIRECTORY}"
    SOURCE "${CTEST_SOURCE_DIRECTORY}"
    OPTIONS "${MAJA_CONFIGURE_OPTIONS}"
    RETURN_VALUE _configure_rv
    CAPTURE_CMAKE_ERROR _configure_error
    )

if ( NOT _configure_rv EQUAL 0 )
  #~ ctest_submit()
  message( FATAL_ERROR "An error occurs during ctest_configure. Dependencies might be buggy.")
endif()

# Build MAJA and install it
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

# run tests
set(_test_rv 0)
#~ ctest_test(PARALLEL_LEVEL 4
           #~ RETURN_VALUE _test_rv
           #~ CAPTURE_CMAKE_ERROR _test_error
           #~ )

if ( NOT _test_rv EQUAL 0 )
  message( SEND_ERROR "An error occurs during ctest_test: ${_test_error}")
endif()

#~ ctest_submit()
