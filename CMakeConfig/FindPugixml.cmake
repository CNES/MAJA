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
# VERSION : 5-1-0 : FA : LAIG-FA-MAC-144674-CS : 6 juin 2016 : Modification passage des COTS en parametre  #
# VERSION : 3-0-0 : DM : LAIG-DM-MAC-239-CNES : 10 mai 2013 : Creation pour la prise en compte de la DM du  #
#                                                            MACCS generique (PM LAIG-NT-MAC-0239-CNES)    #
#                                                                                                          #
# FIN-HISTORIQUE                                                                                           #
#                                                                                                          #
# $Id$
#                                                                                                          #
############################################################################################################
# - Find PUGIXML library
# Find PUGIXML-C includes and libraries
# This module defines
#  PUGIXML_INCLUDE_DIRS,
#  PUGIXML_LIBRARIES,.
#  PUGIXML_FOUND.
#

find_path(PUGIXML_INCLUDE_DIR NAMES pugixml.hpp)

find_library(PUGIXML_LIBRARY NAMES pugixml)

# handle the QUIETLY and REQUIRED arguments and set PUGIXML_FOUND to TRUE if
# all listed variables are TRUE
include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(PUGIXML DEFAULT_MSG
  PUGIXML_LIBRARY PUGIXML_INCLUDE_DIR)

if(PUGIXML_FOUND)
  message(STATUS "  PUGIXML includes    : ${PUGIXML_INCLUDE_DIR}")
  message(STATUS "  PUGIXML library     : ${PUGIXML_LIBRARY}")
endif()

mark_as_advanced(
  PUGIXML_INCLUDE_DIR
  PUGIXML_LIBRARY
)
