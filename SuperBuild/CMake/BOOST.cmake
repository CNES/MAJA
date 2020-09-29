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

set(BOOST_URL "http://downloads.sourceforge.net/project/boost/boost/1.69.0/boost_1_69_0.tar.bz2")
set(BOOST_URL_MD5 a1332494397bf48332cb152abfefcec2)
set(BOOST_DEPENDS)
set(BOOST_AUTOCONF_BUILD 1)
build_projects(BOOST_DEPENDS)

set(BOOST_SB_CONFIG ${SB_CONFIGURE_ARGS} )
if(OTB_TARGET_SYSTEM_ARCH_IS_X64)
  set(BOOST_SB_CONFIG ${SB_CONFIGURE_ARGS} address-model=64)
endif()

set(_SB_BOOST_LIBRARYDIR ${SB_INSTALL_PREFIX}/lib)
set(BOOST_SB_SRC ${CMAKE_BINARY_DIR}/BOOST/source/)
set(BOOST_SB_CONFIG
  ${BOOST_SB_CONFIG} 
  variant=release
  link=shared
  threading=multi
  runtime-link=shared
  --prefix=${SB_INSTALL_PREFIX}
  --includedir=${SB_INSTALL_PREFIX}/include
  --libdir=${_SB_BOOST_LIBRARYDIR}
  --with-system
  --with-serialization
  --with-filesystem
  --with-test
  --with-date_time
  --with-program_options
  --with-thread
)

if(UNIX)
  set(BOOST_BOOTSTRAP_FILE "./bootstrap.sh")
  set(BOOST_B2_EXE "./b2")
else()
  set(BOOST_BOOTSTRAP_FILE "bootstrap.bat")
  set(BOOST_B2_EXE "b2.exe")
endif()

set(BOOST_CONFIGURE_COMMAND ${CMAKE_COMMAND}
  -E chdir ${BOOST_SB_SRC}
  ${BOOST_BOOTSTRAP_FILE}
  --prefix=${SB_INSTALL_PREFIX}
  )

set(BOOST_BUILD_COMMAND ${CMAKE_COMMAND}
  -E chdir ${BOOST_SB_SRC}
  ${BOOST_B2_EXE}
  ${BOOST_SB_CONFIG}
  install
  )
  
ExternalProject_Add(BOOST
  PREFIX BOOST
  URL "${BOOST_URL}"
  URL_MD5 ${BOOST_URL_MD5}
  TMP_DIR      BOOST/tmp
  STAMP_DIR    BOOST/stamp
  SOURCE_DIR   BOOST/source
  INSTALL_DIR ${SB_INSTALL_PREFIX}
  DOWNLOAD_DIR ${DOWNLOAD_DIR}
  CONFIGURE_COMMAND "${BOOST_CONFIGURE_COMMAND}"
  BUILD_COMMAND ${BOOST_BUILD_COMMAND}
  INSTALL_COMMAND ""
  LOG_DOWNLOAD ${WRITE_LOG}
  LOG_CONFIGURE ${WRITE_LOG}
  LOG_BUILD ${WRITE_LOG}
  LOG_INSTALL ${WRITE_LOG}
  )

#HINT: avoid all uses of  _SB_* in External_<project>.cmake
# and depend on much saner CMAKE_PREFIX_PATH for cmake projects.
if(MSVC)
  set(_SB_Boost_INCLUDE_DIR ${SB_INSTALL_PREFIX}/include/boost-1_60)
else()
  set(_SB_Boost_INCLUDE_DIR ${SB_INSTALL_PREFIX}/include)
endif()

SUPERBUILD_PATCH_SOURCE(BOOST)
