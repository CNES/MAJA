#
#.rst:
# FindGDAL
# --------
#
#
#
# Locate gdal
#
# This module accepts the following environment variables:
#
# ::
#
#     GDAL_DIR or GDAL_ROOT - Specify the location of GDAL
#
#
#
# This module defines the following CMake variables:
#
# ::
#
#     GDAL_FOUND - True if libgdal is found
#     GDAL_LIBRARY - A variable pointing to the GDAL library
#     GDAL_INCLUDE_DIR - Where to find the headers

#=============================================================================
# Copyright 2007-2009 Kitware, Inc.
#
# Distributed under the OSI-approved BSD License (the "License");
# see accompanying file Copyright.txt for details.
#
# This software is distributed WITHOUT ANY WARRANTY; without even the
# implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
# See the License for more information.
#=============================================================================
# (To distribute this file outside of CMake, substitute the full
#  License text for the above reference.)

#
# $GDALDIR is an environment variable that would
# correspond to the ./configure --prefix=$GDAL_DIR
# used in building gdal.
#
# Created by Eric Wing. I'm not a gdal user, but OpenSceneGraph uses it
# for osgTerrain so I whipped this module together for completeness.
# I actually don't know the conventions or where files are typically
# placed in distros.
# Any real gdal users are encouraged to correct this (but please don't
# break the OS X framework stuff when doing so which is what usually seems
# to happen).

# This makes the presumption that you are include gdal.h like
#
#include "gdal.h"

find_path(GDAL_INCLUDE_DIR gdal.h
  PATH_SUFFIXES
     include/gdal
     include/GDAL
     include
)

if(UNIX)
  # Use gdal-config to obtain the library version (this should hopefully
  # allow us to -lgdal1.x.y where x.y are correct version)
  # For some reason, libgdal development packages do not contain
  # libgdal.so...
  find_program(GDAL_CONFIG gdal-config PATH_SUFFIXES bin)
  if(GDAL_CONFIG)
    exec_program(${GDAL_CONFIG} ARGS --libs OUTPUT_VARIABLE GDAL_CONFIG_LIBS)
    if(GDAL_CONFIG_LIBS)
      string(REGEX MATCHALL "-l[^ ]+" _gdal_dashl ${GDAL_CONFIG_LIBS})
      string(REPLACE "-l" "" _gdal_lib "${_gdal_dashl}")
      string(REGEX MATCHALL "-L[^ ]+" _gdal_dashL ${GDAL_CONFIG_LIBS})
      string(REPLACE "-L" "" _gdal_libpath "${_gdal_dashL}")
    endif()
  endif()
endif()

find_library(GDAL_LIBRARY
  NAMES         ${_gdal_lib} gdal gdal_i gdal1.5.0 gdal1.4.0 gdal1.3.2 GDAL
  HINTS         ${_gdal_libpath}
  PATH_SUFFIXES lib
)

get_filename_component(GDAL_INSTALL_DIR ${GDAL_INCLUDE_DIR} PATH)
find_path (GDAL_APPS_DIR gdal_rasterize
  HINTS ${GDAL_INSTALL_DIR}
  PATH_SUFFIXES  bin apps
  DOC "path to gdal apps"
  NO_DEFAULT_PATH
  NO_CMAKE_ENVIRONMENT_PATH
  NO_CMAKE_PATH
  NO_SYSTEM_ENVIRONMENT_PATH
  NO_CMAKE_SYSTEM_PATH
  NO_CMAKE_FIND_ROOT_PATH
  )

if(NOT GDAL_APPS_DIR)
  message(STATUS "Cannot find gdal_rasterize. find_package(GDAL) will fail")
 endif()
if (GDAL_INCLUDE_DIR AND EXISTS "${GDAL_INCLUDE_DIR}/gdal_version.h")
  file(STRINGS "${GDAL_INCLUDE_DIR}/gdal_version.h" gdal_version_str
    REGEX "^#[\t ]*define[\t ]+GDAL_VERSION_(MAJOR|MINOR|REV|BUILD)[\t ]+[0-9]+$")
  unset(GDAL_VERSION_STRING)
  foreach(VPART MAJOR MINOR REV BUILD)
    foreach(VLINE ${gdal_version_str})
      if(VLINE MATCHES "^#[\t ]*define[\t ]+GDAL_VERSION_${VPART}[\t ]+([0-9]+)$")
        set(GDAL_VERSION_PART "${CMAKE_MATCH_1}")
        if(GDAL_VERSION_STRING)
          set(GDAL_VERSION_STRING "${GDAL_VERSION_STRING}.${GDAL_VERSION_PART}")
        else()
          set(GDAL_VERSION_STRING "${GDAL_VERSION_PART}")
        endif()
      endif()
    endforeach()
  endforeach()
endif ()

include(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(GDAL
  REQUIRED_VARS GDAL_LIBRARY GDAL_INCLUDE_DIR GDAL_APPS_DIR
  VERSION_VAR GDAL_VERSION_STRING)


set(GDAL_LIBRARIES ${GDAL_LIBRARY})
set(GDAL_INCLUDE_DIRS ${GDAL_INCLUDE_DIR})
mark_as_advanced(GDAL_INCLUDE_DIR GDAL_LIBRARY GDAL_APPS_DIR)
