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
# $Id: FindPugixml.cmake 5694 2013-05-25 16:27:12Z tfeuvrie $
#                                                                                                          #
############################################################################################################
# - Find HDF4 library
# Find HDF4-C includes and libraries
# This module defines
#  HDF4_INCLUDE_DIRS,
#  HDF4_LIBRARIES,.
#  HDF4_FOUND.
#

FIND_PATH(HDF4_INCLUDE_DIR hdf.h)
#FIND_PATH(HDF4_LIBRARY_DIR df mfhdf)

FIND_LIBRARY(HDF4_DF_LIBRARY 
    NAME df hdf_debug
    HINTS ${MAJA_COTS_INSTALL_DIR}/hdf/lib
    PATHS ${MAJA_COTS_INSTALL_DIR}/hdf/lib)

FIND_LIBRARY(HDF4_MFHDF_LIBRARY 
    NAME mfhdf mfhdf_debug
    HINTS ${MAJA_COTS_INSTALL_DIR}/hdf/lib
    PATHS ${MAJA_COTS_INSTALL_DIR}/hdf/lib)


SET(HDF4_LIBRARIES)
SET(HDF4_FOUND FALSE)

if(HDF4_MFHDF_LIBRARY AND HDF4_DF_LIBRARY)
   set( HDF4_LIBRARIES "${HDF4_MFHDF_LIBRARY};${HDF4_DF_LIBRARY}")
   #MARK_AS_ADVANCED(HDF4_INCLUDE_DIR HDF4_LIBRARY_DIR HDF4_DF_LIBRARY_DIR HDF4_MFHDF_LIBRARY_DIR)
   MARK_AS_ADVANCED(HDF4_INCLUDE_DIR HDF4_DF_LIBRARY_DIR HDF4_MFHDF_LIBRARY_DIR)
   set(HDF4_FOUND TRUE)
else()   
#ELSE(HDF4_MFHDF_LIBRARY AND HDF4_DF_LIBRARY)
   # IF(NOT HDF4_MFHDF_LIBRARY)
   #    MARK_AS_ADVANCED(HDF4_DF_LIBRARY)
   # ENDIF(NOT HDF4_MFHDF_LIBRARY)
   # IF(NOT HDF4_DF_LIBRARY)
   #    MARK_AS_ADVANCED(HDF4_MFHDF_LIBRARY)
   # ENDIF(NOT HDF4_DF_LIBRARY)
   set(HDF4_FOUND FALSE)
endif()
#ENDIF(HDF4_MFHDF_LIBRARY AND HDF4_DF_LIBRARY)

if(HDF4_FOUND)
  MESSAGE(STATUS "  HDF4 includes       : ${HDF4_INCLUDE_DIR}")
  MESSAGE(STATUS "  HDF4 mfhdf library  : ${HDF4_MFHDF_LIBRARY}")
  MESSAGE(STATUS "  HDF4 df library     : ${HDF4_DF_LIBRARY}")
#ELSE(PUGIXML_FOUND)
#    MESSAGE(FATAL_ERROR "  Hdf4 not found !")
endif() #HDF4_FOUND

#    MESSAGE(FATAL_ERROR "  Hdf4 not found !")
mark_as_advanced(
  HDF4_INCLUDE_DIR
  HDF4_MFHDF_LIBRARY
  HDF4_DF_LIBRARY
)
