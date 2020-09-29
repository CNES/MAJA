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
# VERSION : 4.0.0                                                                                          #
#                                                                                                          #
# FIN-HISTORIQUE                                                                                           #
#                                                                                                          #
# $Id$                                                                                                     #
#                                                                                                          #
############################################################################################################



set(PATCHELF_PREFIX_DIR "${CMAKE_BINARY_DIR}/PATCHELF")
set(PATCHELF_SOURCE_DIR  "${PATCHELF_PREFIX_DIR}/src/patchelf")
# PATCHELF_BINARY_DIR is same as PATCHELF_SOURCE_DIR
set(PATCHELF_INSTALL_DIR ${SB_INSTALL_PREFIX})
set(PATCHELF_STAMP_DIR   "${PATCHELF_PREFIX_DIR}/stamp/patchelf")
set(PATCHELF_TMP_DIR     "${PATCHELF_PREFIX_DIR}/tmp/patchelf")


if( __EXTERNAL_PATCHELF__)
  return()
else()
  set(__EXTERNAL_PATCHELF__ 1)
endif()


#set(PATCHELF_ENV_COMMAND env CC=${CMAKE_C_COMPILER} CXX=${CMAKE_CXX_COMPILER} CXXFLAGS=-static)
set(PATCHELF_ENV_COMMAND env CC=${CMAKE_C_COMPILER} CXX=${CMAKE_CXX_COMPILER})
ExternalProject_Add(PATCHELF
  PREFIX "${PATCHELF_PREFIX_DIR}"
  URL                 "http://nixos.org/releases/patchelf/patchelf-0.9/patchelf-0.9.tar.bz2"
  URL_MD5             d02687629c7e1698a486a93a0d607947
  DOWNLOAD_DIR        "${DOWNLOAD_DIR}"
  SOURCE_DIR          "${PATCHELF_SOURCE_DIR}"
  BINARY_DIR          "${PATCHELF_SOURCE_DIR}"
  INSTALL_DIR         "${PATCHELF_INSTALL_DIR}"
  STAMP_DIR           "${PATCHELF_STAMP_DIR}"
  TMP_DIR             "${PATCHELF_TMP_DIR}"
  CONFIGURE_COMMAND
  ${PATCHELF_ENV_COMMAND}
  ${PATCHELF_SOURCE_DIR}/configure
  INSTALL_COMMAND ""
  LOG_DOWNLOAD 1
  LOG_CONFIGURE 1
  LOG_BUILD 1
  LOG_INSTALL 1
  )
SUPERBUILD_PATCH_SOURCE(PATCHELF)

if(WIN32)
  set(_SB_PATCHELF_EXECUTABLE ${CMAKE_BINARY_DIR}/PATCHELF/src/PATCHELF/patchelf.exe)
elseif(UNIX)
  set(_SB_PATCHELF_EXECUTABLE ${SB_INSTALL_PREFIX}/bin/patchelf)
endif()


