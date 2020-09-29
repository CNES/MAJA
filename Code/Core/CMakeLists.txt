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
############################################################################################################
#                                                                                                          #
#                                ooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo         #
#                             o                                                                            #
#                          o                                                                               #
#                        o                                                                                 #
#                      o                                                                                   #
#                     o       ooooooo       ooooooo    o         o      oo                                 #
#    o              o       o        o     o       o   o         o     o   o                               #
#      o           o       o          o   o         o  o         o    o      o                             #
#        o        o       o           o   o         o  o         o    o        o                           #
#         o      o        o      oooo     o         o  o         o   o           o                         #
#          o    o          o              o         o  o         o   o           o                         #
#           o  o            o             o         o  o o      o   o          o                           #
#            oo              oooooooo    o         o   o  oooooo   o      oooo                             #
#                                                     o                                                    #
#                                                     o                                                    #
#                                                    o                            o                        #
#                                                    o            o      oooo     o   o      oooo          #
#                                                   o             o         o    o    o         o          #
#                                                   o            o       ooo     o   o       ooo           #
#                                                               o       o       o   o          o           #
#                                                               ooooo   oooo    o   ooooo  oooo            #
#                                                                              o                           #
#                                                                                                          #
############################################################################################################
#                                                                                                          #
# Author: CS Systemes d'Information  (France)                                                              #
#                                                                                                          #
############################################################################################################
# HISTORIQUE                                                                                               #
#                                                                                                          #
# VERSION : 3.1.1 : FA : LAIG-FA-MAJA-2754-CNES : 6 aout 2018 : Problème à l'exécution après déploiement   #
# VERSION : 3.1.0 : DM : LAIG-DM-MAJA-2526-CNES : 9 avril 2018 : Montée de version de d'OTB 6.0            #
# VERSION : 2-0-0 : DM : LAIG-DM-MAC-146-CNES : 14 octobre 2011 : Modification pour prise en compte        #
#                                                            d'evolutions liees a MACCS                    #
#                                                                                                          #
# VERSION : 1-0-0 : <TypeFT> : <NumFT> : 15 novembre 2009 : Creation                                       #
#                                                                                                          #
# FIN-HISTORIQUE                                                                                           #
#                                                                                                          #
# $Id: CMakeLists.txt 5707 2013-05-27 10:25:18Z tfeuvrie $
#                                                                                                          #
############################################################################################################


#-----------------------------------------------------------------------------
# Add libraries subdirectories 
ADD_SUBDIRECTORY(Data)
ADD_SUBDIRECTORY(Applications)

IF( MAJA_ENABLE_GEN_TESTHELPER_LIBRARY_UTILITIES )
    message("*******************  Build the TestHelperLibrary *********************")
    ADD_SUBDIRECTORY(TestHelperLibrary)
ENDIF( MAJA_ENABLE_GEN_TESTHELPER_LIBRARY_UTILITIES )


if ( ENABLE_TU OR ENABLE_TVA OR ENABLE_TV)
        set(processing "vnsTestMainLauncher") 
        CONFIGURE_FILE(${CMAKE_CURRENT_SOURCE_DIR}/script.sh.in
          ${MAJA_BINARY_DIR}/bin/vnsTestMainLauncher.sh @ONLY)
  
        set(processing "otbTestDriver") 
        CONFIGURE_FILE(${CMAKE_CURRENT_SOURCE_DIR}/script.sh.in
          ${MAJA_BINARY_DIR}/bin/otbTestDriver.sh @ONLY)
        INSTALL(PROGRAMS ${MAJA_BINARY_DIR}/bin/otbTestDriver.sh DESTINATION ${MAJA_INSTALL_BIN_DIR})     

        set(processing "vnsDummyProgram")
        CONFIGURE_FILE(${CMAKE_CURRENT_SOURCE_DIR}/script.sh.in
          ${MAJA_BINARY_DIR}/bin/vnsDummyProgram.sh @ONLY)
endif ( ENABLE_TU OR ENABLE_TVA OR ENABLE_TV)

get_filename_component(GDAL_INSTALL_DIR ${GDAL_INCLUDE_DIR} PATH)
set(GDAL_DATA ${GDAL_INSTALL_DIR}/share/gdal)

if(NOT EXISTS "${GDAL_DATA}/")
  message(FATAL_ERROR "Cannot continue without GDAL_DATA : ${GDAL_DATA}")
endif()

if(NOT EXISTS "${GDAL_DATA}/epsg.wkt")
  message(FATAL_ERROR "espg.wkt not found in '${GDAL_DATA}'")
endif()

set(GEOTIFF_CSV ${GDAL_INSTALL_DIR}/share/epsg_csv)

#for build tree
set(MY_MAJA_INSTALL_DIR ${CMAKE_INSTALL_PREFIX})
set(MY_MAJA_APP_DIR ${MAJA_BINARY_DIR}/lib)
set(MY_MAJA_PYTHON_DIR ${MAJA_SOURCE_DIR})
set(MY_MAJA_LIB_DIR "${MAJA_BINARY_DIR}/lib:${CMAKE_INSTALL_PREFIX}/lib")
set(MY_MAJA_PATH_DIRS "${MAJA_BINARY_DIR}/bin:${GDAL_INSTALL_DIR}/bin")
CONFIGURE_FILE(${CMAKE_CURRENT_SOURCE_DIR}/majaenv.sh.in
  ${MAJA_BINARY_DIR}/bin/.majaenv.sh @ONLY)

#for install_tree tree
set(MY_MAJA_INSTALL_DIR ${CMAKE_INSTALL_PREFIX})
set(MY_MAJA_APP_DIR ${CMAKE_INSTALL_PREFIX}/lib/applications)
set(MY_MAJA_PYTHON_DIR ${CMAKE_INSTALL_PREFIX}/lib/python)
set(MY_MAJA_LIB_DIR ${CMAKE_INSTALL_PREFIX}/lib)
set(MY_MAJA_PATH_DIRS "${CMAKE_INSTALL_PREFIX}/bin")
if(NOT "${GDAL_INSTALL_DIR}" STREQUAL "${CMAKE_INSTALL_PREFIX}")
  set(MY_MAJA_PATH_DIRS "${MY_MAJA_PATH_DIRS}:${GDAL_INSTALL_DIR}/bin")
endif()
CONFIGURE_FILE(${CMAKE_CURRENT_SOURCE_DIR}/majaenv.sh.in
  ${MAJA_BINARY_DIR}/bin/majaenv_install_tree.sh @ONLY)

#only need to do install() command on majaenv_install_tree.sh
install(FILES ${MAJA_BINARY_DIR}/bin/majaenv_install_tree.sh
  RENAME .majaenv.sh
  DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)

#for packaging
set(MY_MAJA_INSTALL_DIR "OUT_DIR")
set(GDAL_INSTALL_DIR "OUT_DIR")
set(GDAL_DATA "OUT_DIR/share/gdal")
set(GEOTIFF_CSV "OUT_DIR/share/epsg_csv")
set(MY_MAJA_APP_DIR "${MAJA_INSTALL_DIR}/lib/applications")
set(MY_MAJA_PYTHON_DIR "${MAJA_INSTALL_DIR}/lib/python")
set(MY_MAJA_LIB_DIR "${MAJA_INSTALL_DIR}/lib")
set(MY_MAJA_PATH_DIRS "OUT_DIR/bin")
CONFIGURE_FILE(${CMAKE_CURRENT_SOURCE_DIR}/majaenv.sh.in
               ${MAJA_BINARY_DIR}/bin/majaenv_pkg_tree.sh @ONLY)

IF(MAJA_ENABLE_GEN_TESTHELPER_LIBRARY_UTILITIES)
  message(WARN "MAJA_ENABLE_GEN_TESTHELPER_LIBRARY_UTILITIES is deprecated")
endif()

if(BUILD_TESTING)
  ADD_SUBDIRECTORY(TestHelperLibrary)
endif()

