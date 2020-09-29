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

set(MUPARSER_URL "https://github.com/beltoforion/muparser/archive/v2.2.5.tar.gz")
set(MUPARSER_URL_MD5 02dae671aa5ad955fdcbcd3fee313fb7)

ExternalProject_Add(MUPARSER
  URL "${MUPARSER_URL}"
  URL_MD5 ${MUPARSER_URL_MD5}
  PREFIX MUPARSER
  TMP_DIR      MUPARSER/tmp
  STAMP_DIR    MUPARSER/stamp
  SOURCE_DIR   MUPARSER/source
  BINARY_DIR   MUPARSER/build  
  INSTALL_DIR  ${SB_INSTALL_PREFIX}
  DOWNLOAD_DIR ${DOWNLOAD_DIR}
  DEPENDS ${MUPARSER_DEPENDS}
  CMAKE_COMMAND ${SB_CMAKE_COMMAND}
  CMAKE_CACHE_ARGS ${SB_CMAKE_CACHE_ARGS} ${DISABLE_CXX_WARNING_OPTION}
  PATCH_COMMAND ${CMAKE_COMMAND} -E copy
  ${CMAKE_SOURCE_DIR}/patches/MUPARSER/CMakeLists.txt ${CMAKE_BINARY_DIR}/MUPARSER/source/
  LOG_DOWNLOAD ${WRITE_LOG}
  LOG_CONFIGURE ${WRITE_LOG}
  LOG_BUILD ${WRITE_LOG}
  LOG_INSTALL ${WRITE_LOG}
  )
SUPERBUILD_PATCH_SOURCE(MUPARSER)


