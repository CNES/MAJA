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
# VERSION : 5-1-0 : FA : LAIG-FA-MAC-144674-CS : 6 juin 2016 : Creation                                    #
#                                                                                                          #
# FIN-HISTORIQUE                                                                                           #
#                                                                                                          #
# $Id: FindPugixml.cmake 5694 2013-05-25 16:27:12Z tfeuvrie $
#                                                                                                          #
############################################################################################################
# - Find OpenJpeg library
# Find OPENJPEG and libraries
# This module defines
#  OPENJPEG_LIBRARIES,.
#

FIND_LIBRARY(OPENJPEG_LIBRARY 
    NAME openjp2
    HINTS ${MAJA_COTS_INSTALL_DIR}/openjpeg/lib
    PATHS ${MAJA_COTS_INSTALL_DIR}/openjpeg/lib)

if(OPENJPEG_LIBRARY)
  set(OPENJPEG_LIBRARIES ${OPENJPEG_LIBRARY})
  message(STATUS "  OPENJPEG library    : ${OPENJPEG_LIBRARIES}")
endif(OPENJPEG_LIBRARY)

mark_as_advanced(
  OPENJPEG_LIBRARY
)
