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

set(LXML_URL "https://files.pythonhosted.org/packages/ef/7a/a42b825b27eaa0deedec913e797dd3e4bef51f21d5f0027a87562342fd25/lxml-4.3.4-cp37-cp37m-manylinux1_x86_64.whl")
set(LXML_URL_MD5 6aa2f79923076da35cba6b09157d5b3f)
set(LXML_DEPENDS PYTHON)
set(LXML_AUTOCONF_BUILD 1)
build_projects(LXML_DEPENDS)

set(LXML_SB_SRC ${CMAKE_BINARY_DIR}/LXML/source/)
  
ExternalProject_Add(LXML
  PREFIX LXML
  URL "${LXML_URL}"
  URL_MD5 ${LXML_URL_MD5}
  TMP_DIR      LXML/tmp
  STAMP_DIR    LXML/stamp
  SOURCE_DIR   LXML/source
  INSTALL_DIR ${SB_INSTALL_PREFIX}
  DOWNLOAD_DIR ${DOWNLOAD_DIR}
  DOWNLOAD_NO_EXTRACT ON
  DEPENDS ${LXML_DEPENDS}
  CONFIGURE_COMMAND ${SB_INSTALL_PREFIX}/bin/pip3.7 -v --disable-pip-version-check --no-cache-dir --no-color
    install <DOWNLOADED_FILE>
  BUILD_COMMAND ""
  INSTALL_COMMAND ""
  LOG_DOWNLOAD ${WRITE_LOG}
  LOG_CONFIGURE ${WRITE_LOG}
  LOG_BUILD ${WRITE_LOG}
  LOG_INSTALL ${WRITE_LOG}
  )

SUPERBUILD_PATCH_SOURCE(LXML)
