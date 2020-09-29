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

set(PYTHON_URL "https://www.python.org/ftp/python/3.7.4/Python-3.7.4.tar.xz")
set(PYTHON_URL_MD5 d33e4aae66097051c2eca45ee3604803)
set(PYTHON_DEPENDS ZLIB EXPAT SQLITE PATCHELF)
set(PYTHON_AUTOCONF_BUILD 1)
build_projects(PYTHON_DEPENDS)

set(PYTHON_CONFIG_ARGS
  ${SB_CONFIGURE_ARGS}
  --enable-ipv6
  --enable-shared
  --enable-unicode=ucs4
  --with-system-ffi
  --with-system-expat
   )

ExternalProject_Add(PYTHON
  PREFIX PYTHON
  URL "${PYTHON_URL}"
  URL_MD5 ${PYTHON_URL_MD5}
  TMP_DIR      PYTHON/tmp
  STAMP_DIR    PYTHON/stamp
  SOURCE_DIR   PYTHON/source
  INSTALL_DIR ${SB_INSTALL_PREFIX}
  DOWNLOAD_DIR ${DOWNLOAD_DIR}
  DEPENDS ${PYTHON_DEPENDS}
  CONFIGURE_COMMAND "${SB_ENV_CONFIGURE_CMD};${CMAKE_BINARY_DIR}/PYTHON/source/configure"
  ${PYTHON_CONFIG_ARGS}
  BUILD_IN_SOURCE 1
  LOG_DOWNLOAD ${WRITE_LOG}
  LOG_CONFIGURE ${WRITE_LOG}
  LOG_BUILD ${WRITE_LOG}
  LOG_INSTALL ${WRITE_LOG}
  )

ExternalProject_Add_Step(PYTHON post_install
    COMMAND /bin/sh -x
    ${CMAKE_SOURCE_DIR}/patches/PYTHON/post_install.sh
    ${SB_INSTALL_PREFIX}
    ${CMAKE_BINARY_DIR}
    DEPENDEES install)
SUPERBUILD_PATCH_SOURCE(PYTHON)
