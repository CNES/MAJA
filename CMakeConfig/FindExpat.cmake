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
# 6 Nov 2017 : synchronize this file with FindEXPAT from CMAke 3.5
# VERSION : 5-1-0 : FA : LAIG-FA-MAC-144674-CS : 6 juin 2016 : Modification passage des COTS en parametre  #
# VERSION : 3-2-0 : DM : LAIG-DM-MAC-697-CNES : 20 novembre 2013 : Creation suite maj de l'OTB             #
#                                                                                                          #
# FIN-HISTORIQUE                                                                                           #
#                                                                                                          #
# $Id: FindPugixml.cmake 5694 2013-05-25 16:27:12Z tfeuvrie $
#                                                                                                          #
############################################################################################################
# - Find expat
# Find the native EXPAT headers and libraries.
#
#  EXPAT_INCLUDE_DIRS - where to find expat.h, etc.
#  EXPAT_LIBRARIES    - List of libraries when using expat.
#  EXPAT_FOUND        - True if expat found.

#=============================================================================
# Copyright 2006-2009 Kitware, Inc.
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

# # Look for the header file.
# FIND_PATH(EXPAT_INCLUDE_DIR NAMES expat.h)

# # Look for the library.
# FIND_LIBRARY(EXPAT_LIBRARY expat 
#         HINTS ${MAJA_COTS_INSTALL_DIR}/expat/lib
#         PATHS ${MAJA_COTS_INSTALL_DIR}/expat/lib
# )
#  # PATH_SUFFIXES libexpat)

# # handle the QUIETLY and REQUIRED arguments and set EXPAT_FOUND to TRUE if 
# # all listed variables are TRUE
# INCLUDE(FindPackageHandleStandardArgs)
# FIND_PACKAGE_HANDLE_STANDARD_ARGS(EXPAT DEFAULT_MSG EXPAT_LIBRARY EXPAT_INCLUDE_DIR)

# # Copy the results to the output variables.
# IF(EXPAT_FOUND)
#   SET(EXPAT_LIBRARIES ${EXPAT_LIBRARY})
#   SET(EXPAT_INCLUDE_DIRS ${EXPAT_INCLUDE_DIR})
#   message(STATUS "  EXPAT includes      : ${EXPAT_INCLUDE_DIRS}")
#   message(STATUS "  EXPAT library       : ${EXPAT_LIBRARIES}")
# ENDIF(EXPAT_FOUND)

# MARK_AS_ADVANCED(EXPAT_INCLUDE_DIR EXPAT_LIBRARY)


#find_package(PkgConfig QUIET)

#pkg_check_modules(PC_EXPAT QUIET expat)

# Look for the header file.
find_path(EXPAT_INCLUDE_DIR NAMES expat.h) # HINTS ${PC_EXPAT_INCLUDE_DIRS})

# Look for the library.
find_library(EXPAT_LIBRARY NAMES expat libexpat) # HINTS ${PC_EXPAT_LIBRARY_DIRS})

if (EXPAT_INCLUDE_DIR AND EXISTS "${EXPAT_INCLUDE_DIR}/expat.h")
  file(STRINGS "${EXPAT_INCLUDE_DIR}/expat.h" expat_version_str
    REGEX "^#[\t ]*define[\t ]+XML_(MAJOR|MINOR|MICRO)_VERSION[\t ]+[0-9]+$")  
  unset(EXPAT_VERSION_STRING)
  foreach(VPART MAJOR MINOR MICRO)
    foreach(VLINE ${expat_version_str})
      if(VLINE MATCHES "^#[\t ]*define[\t ]+XML_${VPART}_VERSION[\t ]+([0-9]+)$")
        set(EXPAT_VERSION_PART "${CMAKE_MATCH_1}")
        if(EXPAT_VERSION_STRING)
          set(EXPAT_VERSION_STRING "${EXPAT_VERSION_STRING}.${EXPAT_VERSION_PART}") 
        else()
          set(EXPAT_VERSION_STRING "${EXPAT_VERSION_PART}")
        endif()
      endif()
    endforeach()
  endforeach()
endif ()

# handle the QUIETLY and REQUIRED arguments and set EXPAT_FOUND to TRUE if
# all listed variables are TRUE
include(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(EXPAT
  REQUIRED_VARS EXPAT_LIBRARY EXPAT_INCLUDE_DIR
  VERSION_VAR EXPAT_VERSION_STRING)

# Copy the results to the output variables.
if(EXPAT_FOUND)
  set(EXPAT_LIBRARIES ${EXPAT_LIBRARY})
  set(EXPAT_INCLUDE_DIRS ${EXPAT_INCLUDE_DIR})
endif()
#message(FATAL_ERROR "xx")
mark_as_advanced(EXPAT_INCLUDE_DIR EXPAT_LIBRARY)
