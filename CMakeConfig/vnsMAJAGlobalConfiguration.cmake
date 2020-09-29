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
# HISTORIQUE                                                                                               #
#                                                                                                          #
# VERSION : 3.1.0 : DM : LAIG-DM-MAJA-2572-CNES : 22 mars 2018 : Nouveau Plugin SENTINEL2_TM               #
# VERSION : 4-0-0 : DM : LAIG-DM-MAC-613-CNES : 17 decembre 2013 : Creation pour DM de la V 4.0.0          #
#                                                                                                          #
# FIN-HISTORIQUE                                                                                           #
#                                                                                                          #
# $Id$                                                                                                     #
#                                                                                                          #
############################################################################################################

INCLUDE(${CMAKE_CURRENT_LIST_DIR}/CommonCmakeOptions.cmake)

# ATTENTION: Versions with 3 digits, with dot separator (.)

#-----------------------------------------------------------------------------
# MAJA version number.  An even minor number corresponds to releases.
IF(NOT MAJA_VERSION)
    SET(MAJA_VERSION "0.0.0")
ENDIF(NOT MAJA_VERSION)

STRING(REGEX REPLACE "([0-9]+)\\.([0-9]+)\\.([0-9]+)" "\\1" MAJA_VERSION_MAJOR "${MAJA_VERSION}")
STRING(REGEX REPLACE "([0-9]+)\\.([0-9]+)\\.([0-9]+)" "\\2" MAJA_VERSION_MINOR "${MAJA_VERSION}")
STRING(REGEX REPLACE "([0-9]+)\\.([0-9]+)\\.([0-9]+)" "\\3" MAJA_VERSION_PATCH "${MAJA_VERSION}")

IF(NOT MAJA_NO_LIBRARY_VERSION)
  # This setting of SOVERSION assumes that any API change
  # will increment either the minor or major version number of MAJA.
  SET(MAJA_LIBRARY_PROPERTIES
      VERSION   "${MAJA_VERSION_MAJOR}.${MAJA_VERSION_MINOR}.${MAJA_VERSION_PATCH}"
      SOVERSION "${MAJA_VERSION_MAJOR}.${MAJA_VERSION_MINOR}"
     )
ENDIF(NOT MAJA_NO_LIBRARY_VERSION)


# Define the Logger version (use by LoggerBase.cxx and Logger.py)
# TODO: Use the MAJA_VERSION_MAJOR, MAJA_VERSION_MINOR to generate automaticaly this LOGGER_STRING VERSION

IF(NOT MAJA_VERSION_CHAIN)
    SET(MAJA_VERSION_CHAIN "00.00")
ENDIF(NOT MAJA_VERSION_CHAIN)

SET(MAJA_VERSION_LOGGER_STRING ${MAJA_VERSION_CHAIN})
SET(MAJA_VERSION_MAJA_PROCESSING_STRING ${MAJA_VERSION_CHAIN})

# Version string should not include patch level.  The major.minor is
# enough to distinguish available features of the toolkit.
# Version for install directory (only 2 digits)
SET(MAJA_VERSION_FOR_SW_COMPONENT "${MAJA_VERSION_MAJOR}.${MAJA_VERSION_MINOR}")

# Define the Schema version (use to write L2 and L3 product)
# TODO : the variable MAJA_FILE_VERSION must be removed ! 
SET(MAJA_FILE_VERSION "0001" CACHE STRING "Version of the XML file (xxxx format)" FORCE)

#-----------------------------------------------------------------------------
# Set the MAJA_COTS_VERSION working with le MAJA_VERSION
IF(NOT MAJA_COTS_VERSION)
    SET(MAJA_COTS_VERSION "0.0.0")
ENDIF(NOT MAJA_COTS_VERSION)

STRING(REGEX REPLACE "([0-9]+)\\.([0-9]+)\\.([0-9]+)" "\\1" MAJA_COTS_VERSION_MAJOR "${MAJA_COTS_VERSION}")
STRING(REGEX REPLACE "([0-9]+)\\.([0-9]+)\\.([0-9]+)" "\\2" MAJA_COTS_VERSION_MINOR "${MAJA_COTS_VERSION}")

SET(MAJA_COTS_VERSION_FOR_SW_COMPONENT "${MAJA_COTS_VERSION_MAJOR}.${MAJA_COTS_VERSION_MINOR}")


#-----------------------------------------------------------------------------
# MAJA CORE version number.  An even minor number corresponds to releases.
STRING(REGEX REPLACE "([0-9]+)\\.([0-9]+)\\.([0-9]+)" "\\1" MAJA_CORE_VERSION_MAJOR "${MAJA_CORE_VERSION}")
STRING(REGEX REPLACE "([0-9]+)\\.([0-9]+)\\.([0-9]+)" "\\2" MAJA_CORE_VERSION_MINOR "${MAJA_CORE_VERSION}")
STRING(REGEX REPLACE "([0-9]+)\\.([0-9]+)\\.([0-9]+)" "\\3" MAJA_CORE_VERSION_PATCH "${MAJA_CORE_VERSION}")
IF(NOT MAJA_NO_LIBRARY_VERSION)
  # This setting of SOVERSION assumes that any API change
  # will increment either the minor or major version number of MAJA.
  SET(MAJA_CORE_LIBRARY_PROPERTIES
      VERSION   "${MAJA_CORE_VERSION_MAJOR}.${MAJA_CORE_VERSION_MINOR}.${MAJA_CORE_VERSION_PATCH}"
      SOVERSION "${MAJA_CORE_VERSION_MAJOR}.${MAJA_CORE_VERSION_MINOR}"
     )
ENDIF(NOT MAJA_NO_LIBRARY_VERSION)


#-----------------------------------------------------------------------------
# MAJA ALGORITHMS version number.  An even minor number corresponds to releases.
STRING(REGEX REPLACE "([0-9]+)\\.([0-9]+)\\.([0-9]+)" "\\1" MAJA_ALGORITHMS_VERSION_MAJOR "${MAJA_ALGORITHMS_VERSION}")
STRING(REGEX REPLACE "([0-9]+)\\.([0-9]+)\\.([0-9]+)" "\\2" MAJA_ALGORITHMS_VERSION_MINOR "${MAJA_ALGORITHMS_VERSION}")
STRING(REGEX REPLACE "([0-9]+)\\.([0-9]+)\\.([0-9]+)" "\\3" MAJA_ALGORITHMS_VERSION_PATCH "${MAJA_ALGORITHMS_VERSION}")
IF(NOT MAJA_NO_LIBRARY_VERSION)
  # This setting of SOVERSION assumes that any API change
  # will increment either the minor or major version number of MAJA.
  SET(MAJA_ALGORITHMS_LIBRARY_PROPERTIES
      VERSION   "${MAJA_ALGORITHMS_VERSION_MAJOR}.${MAJA_ALGORITHMS_VERSION_MINOR}.${MAJA_ALGORITHMS_VERSION_PATCH}"
      SOVERSION "${MAJA_ALGORITHMS_VERSION_MAJOR}.${MAJA_ALGORITHMS_VERSION_MINOR}"
     )
ENDIF(NOT MAJA_NO_LIBRARY_VERSION)

if(PROJECT_NAME STREQUAL "MAJA-SuperBuild")
  unset(MAJA_LIST_OF_SPECTRAL_CAMERA CACHE PARENT_SCOPE)
else()
  unset(MAJA_LIST_OF_SPECTRAL_CAMERA CACHE)
endif()

FOREACH(spectral_camera ${MAJA_LIST_OF_SPECTRAL_CAMERA_AVAILABLE})
  list(APPEND MAJA_LIST_OF_SPECTRAL_CAMERA "${spectral_camera}")
endforeach(spectral_camera)
message (STATUS "List of spectral camera set: ${MAJA_LIST_OF_SPECTRAL_CAMERA}")

#-----------------------------------------------------------------------------
# MAJA PLugin version number.  An even minor number corresponds to releases.
FOREACH(spectral_camera ${MAJA_LIST_OF_SPECTRAL_CAMERA})

    STRING(REGEX REPLACE "([0-9]+)\\.([0-9]+)\\.([0-9]+)" "\\1" MAJA_PLUGIN_${spectral_camera}_VERSION_MAJOR "${MAJA_PLUGIN_${spectral_camera}_VERSION}")
    STRING(REGEX REPLACE "([0-9]+)\\.([0-9]+)\\.([0-9]+)" "\\2" MAJA_PLUGIN_${spectral_camera}_VERSION_MINOR "${MAJA_PLUGIN_${spectral_camera}_VERSION}")
    STRING(REGEX REPLACE "([0-9]+)\\.([0-9]+)\\.([0-9]+)" "\\3" MAJA_PLUGIN_${spectral_camera}_VERSION_PATCH "${MAJA_PLUGIN_${spectral_camera}_VERSION}")
    IF(NOT MAJA_NO_LIBRARY_VERSION)
      # This setting of SOVERSION assumes that any API change
      # will increment either the minor or major version number of MAJA.
      SET(MAJA_PLUGIN_${spectral_camera}_LIBRARY_PROPERTIES
          VERSION   "${MAJA_PLUGIN_${spectral_camera}_VERSION_MAJOR}.${MAJA_PLUGIN_${spectral_camera}_VERSION_MINOR}.${MAJA_PLUGIN_${spectral_camera}_VERSION_PATCH}"
          SOVERSION "${MAJA_PLUGIN_${spectral_camera}_VERSION_MAJOR}.${MAJA_PLUGIN_${spectral_camera}_VERSION_MINOR}"
         )
    ENDIF(NOT MAJA_NO_LIBRARY_VERSION)

ENDFOREACH(spectral_camera)








