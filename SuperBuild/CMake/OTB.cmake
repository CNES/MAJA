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

set(OTB_URL "https://www.orfeo-toolbox.org/packages/archives/OTB/OTB-7.0.0.tar.gz")
set(OTB_URL_MD5 314b985d112dab6d407dd6d35bccc790    )

set(OTB_DEPENDS GDAL ITK OPENJPEG GEOTIFF OSSIM TINYXML MUPARSER BOOST SWIG NUMPY PYTHON)
build_projects(OTB_DEPENDS)

ExternalProject_Add(OTB
  URL "${OTB_URL}"
  URL_MD5 ${OTB_URL_MD5}
  PREFIX       OTB
  TMP_DIR      OTB/tmp
  STAMP_DIR    OTB/stamp
  SOURCE_DIR   OTB/source
  BINARY_DIR   OTB/build  
  INSTALL_DIR  ${SB_INSTALL_PREFIX}
  DOWNLOAD_DIR ${DOWNLOAD_DIR}
  DEPENDS ${OTB_DEPENDS}
  CMAKE_COMMAND ${SB_CMAKE_COMMAND}
  CMAKE_CACHE_ARGS
  ${SB_CMAKE_CACHE_ARGS}
  ${DISABLE_CXX_WARNING_OPTION}
  -DOTB_USE_MUPARSER:BOOL=ON
  -DOTB_USE_6S:BOOL=OFF
  -DOTB_USE_SIFTFAST:BOOL=OFF
  -DOTB_BUILD_DEFAULT_MODULES:BOOL=OFF
  -DOTBGroup_Core:BOOL=OFF
  -DOTBGroup_IO:BOOL=ON
  -DModule_OTBIndices:BOOL=ON
  -DModule_OTBAppImageUtils:BOOL=ON
  -DModule_OTBAppMathParser:BOOL=ON
  -DModule_OTBAppMorphology:BOOL=ON
  -DModule_OTBAppProjection:BOOL=ON
  -DModule_OTBStreaming:BOOL=ON
  -DModule_OTBStatistics:BOOL=ON
  -DModule_OTBConvolution:BOOL=ON
  -DModule_OTBProjection:BOOL=ON
  -DModule_OTBMathParser:BOOL=ON  
  -DOTB_USE_MUPARSERX:BOOL=OFF
  -DModule_OTBImageManipulation:BOOL=ON
  -DModule_OTBCommandLine:BOOl=ON
  -DModule_OTBCommandLineParser:BOOL=ON
  -DModule_OTBCommandLine:BOOL=ON
  -DModule_OTBSWIGWrapper:BOOL=ON
  -DOTBGroup_Applications:BOOL=ON
  -DOTB_WRAP_PYTHON:BOOL=ON
  -DSWIG_EXECUTABLE:PATH=${SB_INSTALL_PREFIX}/bin/swig
  -DPYTHON_EXECUTABLE:PATH=${SB_INSTALL_PREFIX}/bin/python3
  LOG_DOWNLOAD ${WRITE_LOG}
  LOG_CONFIGURE ${WRITE_LOG}
  LOG_BUILD ${WRITE_LOG}
  LOG_INSTALL ${WRITE_LOG}
  )
SUPERBUILD_PATCH_SOURCE(OTB)


