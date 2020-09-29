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
#                                        __  __    __     ____   __                                        #
#                                       (  \/  )  /__\   (_  _) /__\                                       #
#                                        )    (  /(__)\ .-_)(  /(__)\                                      #
#                                       (_/\/\_)(__)(__)\____)(__)(__)                                     #
#                                                                                                          #
#                                                                                                          #
############################################################################################################
# HISTORIQUE                                                                                               #
#                                                                                                          #
# VERSION : 3.1.0 : DM : LAIG-DM-MAJA-2526-CNES : 9 avril 2018 : Montée de version de d'OTB 6.0            #
#                                                                                                          #
# FIN-HISTORIQUE                                                                                           #
#                                                                                                          #
# $Id$                                                                                                     #
#                                                                                                          #
############################################################################################################

set(SWIG_URL "http://downloads.sourceforge.net/project/swig/swig/swig-3.0.7/swig-3.0.7.tar.gz")
set(SWIG_URL_MD5 7fff46c84b8c630ede5b0f0827e3d90a)
set(SWIG_DEPENDS PYTHON BOOST)
set(SWIG_AUTOCONF_BUILD 1)
build_projects(SWIG_DEPENDS)


set(SWIG_CONFIG_ARGS
  ${SB_CONFIGURE_ARGS}
  --with-pcre=yes
  --with-python3=${SB_INSTALL_PREFIX}/bin/python3
  )


ExternalProject_Add(SWIG
  PREFIX SWIG
  URL "${SWIG_URL}"
  URL_MD5 ${SWIG_URL_MD5}
  TMP_DIR      SWIG/tmp
  STAMP_DIR    SWIG/stamp 
  SOURCE_DIR   SWIG/source
  #BINARY_DIR   SWIG/build #uncomment for BUILD_IN_SOURCE 1
  INSTALL_DIR ${SB_INSTALL_PREFIX}
  DOWNLOAD_DIR ${DOWNLOAD_DIR}
  DEPENDS ${SWIG_DEPENDS}
  CONFIGURE_COMMAND "${SB_ENV_CONFIGURE_CMD};${CMAKE_BINARY_DIR}/SWIG/source/configure"
  ${SWIG_CONFIG_ARGS}
  BUILD_IN_SOURCE 1
  LOG_DOWNLOAD ${WRITE_LOG}
  LOG_CONFIGURE ${WRITE_LOG}
  LOG_BUILD ${WRITE_LOG}
  LOG_INSTALL ${WRITE_LOG}
  )

SUPERBUILD_PATCH_SOURCE(SWIG)

if(WIN32)
  set(_SB_SWIG_EXECUTABLE ${CMAKE_BINARY_DIR}/SWIG/src/SWIG/swig.exe)
elseif(UNIX)
  set(_SB_SWIG_EXECUTABLE ${SB_INSTALL_PREFIX}/bin/swig)
endif()
