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
# to get the original .whl just change .tar.gz to  .whl in the following URL
set(SCIPY_URL "https://files.pythonhosted.org/packages/dd/82/c1fe128f3526b128cfd185580ba40d01371c5d299fcf7f77968e22dfcc2e/scipy-1.4.1-cp37-cp37m-manylinux1_x86_64.whl")
set(SCIPY_URL_MD5 b2a9ee8c5ee393f6a52eb387163ad785)
set(SCIPY_DEPENDS PYTHON NUMPY)
set(SCIPY_AUTOCONF_BUILD 1)
build_projects(SCIPY_DEPENDS)

set(SCIPY_SB_SRC ${CMAKE_BINARY_DIR}/SCIPY/source/)

ExternalProject_Add(SCIPY
  PREFIX SCIPY
  URL "${SCIPY_URL}"
  URL_MD5 ${SCIPY_URL_MD5}
  TMP_DIR      SCIPY/tmp
  STAMP_DIR    SCIPY/stamp
  SOURCE_DIR   SCIPY/source
  INSTALL_DIR ${SB_INSTALL_PREFIX}
  DEPENDS ${SCIPY_DEPENDS}
  DOWNLOAD_DIR ${DOWNLOAD_DIR}
  DOWNLOAD_NO_EXTRACT ON
  CONFIGURE_COMMAND ${SB_INSTALL_PREFIX}/bin/pip3.7 -v --disable-pip-version-check --no-cache-dir --no-color
    install <DOWNLOADED_FILE>
  BUILD_COMMAND ""
  INSTALL_COMMAND ""
  LOG_DOWNLOAD ${WRITE_LOG}
  LOG_CONFIGURE ${WRITE_LOG}
  LOG_BUILD ${WRITE_LOG}
  LOG_INSTALL ${WRITE_LOG}
  )

SUPERBUILD_PATCH_SOURCE(SCIPY)
