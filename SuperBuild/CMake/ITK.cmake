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

set(ITK_ENABLED_MODULES
  Common
  FiniteDifference
  GPUCommon
  GPUFiniteDifference
  ImageAdaptors
  ImageFunction
  Mesh
  QuadEdgeMesh
  SpatialObjects
  #TestKernel
  Transform

  AnisotropicSmoothing
  AntiAlias
  BiasCorrection
  BinaryMathematicalMorphology
  Colormap
  Convolution
  CurvatureFlow
  Deconvolution
  Denoising
  #DiffusionTensorImage
  DisplacementField
  DistanceMap
  FastMarching
  FFT
  GPUAnisotropicSmoothing
  GPUImageFilterBase
  GPUSmoothing
  GPUThresholding
  ImageCompare
  ImageCompose
  ImageFeature
  ImageFilterBase
  ImageFusion
  ImageGradient
  ImageGrid
  ImageIntensity
  ImageLabel
  ImageSources
  ImageStatistics
  LabelMap
  MathematicalMorphology
  Path
  QuadEdgeMeshFiltering
  Smoothing
  SpatialFunction
  Thresholding

  Eigen
  #FEM
  NarrowBand
  NeuralNetworks
  Optimizers
  Optimizersv4
  Polynomials
  Statistics

  RegistrationCommon
  #FEMRegistration
  GPURegistrationCommon
  GPUPDEDeformableRegistration
  Metricsv4
  PDEDeformableRegistration
  RegistrationMethodsv4

  #BioCell
  Classifiers
  ConnectedComponents
  DeformableMesh
  KLMRegionGrowing
  LabelVoting
  LevelSets
  LevelSetsv4
  #LevelSetsv4Visualization
  MarkovRandomFieldsClassifiers
  RegionGrowing
  SignedDistanceFunction
  Voronoi
  Watersheds
  IOMeta
  IOJPEG
  IOTIFF  
  )

set(ITK_MODULES_CMAKE_CACHE)
foreach(ITK_MODULE ${ITK_ENABLED_MODULES})
  list(APPEND ITK_MODULES_CMAKE_CACHE -DModule_ITK${ITK_MODULE}:BOOL=ON)
endforeach()
set(ITK_URL  "https://sourceforge.net/projects/itk/files/itk/4.13/InsightToolkit-4.13.1.tar.gz")
set(ITK_URL_MD5 c7e229802c4ee64e1b2a6d48b1df67e9)
set(ITK_DEPENDS ZLIB TIFF JPEG PNG EXPAT)
build_projects(ITK_DEPENDS)

ExternalProject_Add(ITK
  URL          "${ITK_URL}"
  URL_MD5      ${ITK_URL_MD5}
  PREFIX       ITK
  TMP_DIR      ITK/tmp
  STAMP_DIR    ITK/stamp
  SOURCE_DIR   ITK/source
  BINARY_DIR   ITK/build
  INSTALL_DIR ${SB_INSTALL_PREFIX}
  DOWNLOAD_DIR ${DOWNLOAD_DIR}
  CMAKE_CACHE_ARGS
  ${SB_CMAKE_CACHE_ARGS}
  -DITK_BUILD_DEFAULT_MODULES:BOOL=OFF
  -DUSE_FFTWF:BOOL=OFF
  -DUSE_FFTWD:BOOL=OFF
  -DBUILD_TESTING:BOOL=OFF
  -DBUILD_EXAMPLES:BOOL=OFF
  -DITK_USE_SYSTEM_EXPAT:BOOL=ON
  -DITK_USE_SYSTEM_ZLIB:BOOL=ON
  -DITK_USE_SYSTEM_TIFF:BOOL=ON
  -DITK_USE_SYSTEM_JPEG:BOOL=ON  
  -DITK_FORBID_DOWNLOADS:BOOL=ON
  -DITKGroup_Core:BOOL=OFF
  -DExternalData_URL_TEMPLATES:STRING=
  ${ITK_MODULES_CMAKE_CACHE}
  DEPENDS ${ITK_DEPENDS}
  CMAKE_COMMAND ${SB_CMAKE_COMMAND}
  LOG_DOWNLOAD ${WRITE_LOG}
  LOG_CONFIGURE ${WRITE_LOG}
  LOG_BUILD ${WRITE_LOG}
  LOG_INSTALL ${WRITE_LOG}
  )

SUPERBUILD_PATCH_SOURCE(ITK)

