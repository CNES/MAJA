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

set(HDF4_URL "http://support.hdfgroup.org/ftp/HDF/HDF_Current/src/hdf-4.2.13.tar.gz")
set(HDF4_URL_MD5 a6aa950b3fce5162b96496d8ea0b82bf)
set(HDF4_DEPENDS ZLIB JPEG)
build_projects(HDF4_DEPENDS)
set(HDF4_CONFIG_ARGS
  ${SB_CONFIGURE_ARGS}
  --enable-fortran=no
  --enable-netcdf=no
  )

add_configure_option(HDF4 JPEG --with-jpeg)
add_configure_option(HDF4 ZLIB --with-zlib)
set(HDF4_CONFIGURE_COMMAND "${SB_ENV_CONFIGURE_CMD};EGREP=${EGREP_COMMAND};${CMAKE_BINARY_DIR}/HDF4/source/configure")
ExternalProject_Add(HDF4
  URL          "${HDF4_URL}"
  URL_MD5      ${HDF4_URL_MD5}
  PREFIX       HDF4
  TMP_DIR      HDF4/tmp
  STAMP_DIR    HDF4/stamp
  SOURCE_DIR   HDF4/source
  BINARY_DIR   HDF4/build
  INSTALL_DIR  ${SB_INSTALL_PREFIX}
  DOWNLOAD_DIR ${DOWNLOAD_DIR}
  DEPENDS           ${HDF4_DEPENDS}
  CONFIGURE_COMMAND ${HDF4_CONFIGURE_COMMAND} ${HDF4_CONFIG_ARGS}
  LOG_DOWNLOAD      ${WRITE_LOG}
  LOG_CONFIGURE     ${WRITE_LOG}
  LOG_BUILD ${WRITE_LOG}
  LOG_INSTALL ${WRITE_LOG}
  )
