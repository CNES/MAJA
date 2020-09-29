#
# Copyright (C) 2005-2017 Centre National d'Etudes Spatiales (CNES)
#
# This file is part of Orfeo Toolbox
#
#     https://www.orfeo-toolbox.org/
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
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
# VERSION : 3.1.1 : FA : LAIG-FA-MAJA-2754-CNES : 29 aout 2018 : portabilité s2cal (MPI pour OSSIM: OFF)   #
# VERSION : 3.1.0 : DM : LAIG-DM-MAJA-2526-CNES : 9 avril 2018 : Montée de version de d'OTB 6.0            #
#                                                                                                          #
# FIN-HISTORIQUE                                                                                           #
#                                                                                                          #
# $Id$                                                                                                     #
#                                                                                                          #
############################################################################################################


cmake_minimum_required(VERSION 3.10.2)

project(MAJA-SuperBuild)

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/CMake
  ${CMAKE_MODULE_PATH})

include(ExternalProject)
 
option(USE_DEFAULT_INSTALL_PREFIX "Install to default prefix /usr/local for unix" OFF)
mark_as_advanced(USE_DEFAULT_INSTALL_PREFIX)
get_filename_component(PARENT_OF_BINARY_DIR ${CMAKE_BINARY_DIR} PATH)
if (CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT AND NOT USE_DEFAULT_INSTALL_PREFIX)
  set (CMAKE_INSTALL_PREFIX
    "/opt"
    CACHE
    PATH
    "Install path prefix, prepended onto install directories."
    FORCE )
endif()

include(${CMAKE_SOURCE_DIR}/../CMakeConfig/CommonCmakeOptions.cmake)
option(BUILD_STATIC_LIBS "Build with static libraries." OFF)
mark_as_advanced(BUILD_STATIC_LIBS)

option(WRITE_LOG "write (configure, build, test) outputs to file" ON)
mark_as_advanced(WRITE_LOG)

include(CTest)
include(SuperBuild_Macro)

# Configure location where source tar-balls are downloaded
find_path(DOWNLOAD_DIR
  NAMES MAJA.SUPERBUILD.README
  HINTS $ENV{DOWNLOAD_DIR}
  "/MAJA_SHARED_FOLDER/superbuild-archives/"
  "${CMAKE_CURRENT_BINARY_DIR}/downloads"
  )

if(NOT DOWNLOAD_DIR)
  message(FATAL_ERROR "DOWNLOAD_DIR must be set.!")
endif()

if(NOT MAJA_VERSION)
  message(FATAL_ERROR "MAJA_VERSION must be set.!")
endif()

set(SB_INSTALL_PREFIX ${CMAKE_INSTALL_PREFIX}/maja/${MAJA_VERSION})

file(TO_NATIVE_PATH "${SB_INSTALL_PREFIX}" SB_INSTALL_PREFIX_NATIVE)

if(CMAKE_INSTALL_PREFIX STREQUAL "/usr/local")
  message(WARNING
    "The CMAKE_INSTALL_PREFIX variable seems to be set by default : "
    "${CMAKE_INSTALL_PREFIX}. Be aware that this directory will be used"
    " during the build (even without calling the install target). Please "
    "make sure you want to use this directory as the SuperBuild output.")
endif()

if(DEFINED ENV{CMAKE_PREFIX_PATH})
  set(SB_CMAKE_PREFIX_PATH "$ENV{CMAKE_PREFIX_PATH};${SB_INSTALL_PREFIX}")
else()
  set(SB_CMAKE_PREFIX_PATH "${SB_INSTALL_PREFIX}")
endif()

list(REMOVE_DUPLICATES SB_CMAKE_PREFIX_PATH)

set(SB_CMAKE_CACHE_ARGS)
list(APPEND SB_CMAKE_CACHE_ARGS "-DCMAKE_BUILD_TYPE:STRING=${CMAKE_BUILD_TYPE}")
list(APPEND SB_CMAKE_CACHE_ARGS "-DCMAKE_INSTALL_PREFIX:PATH=${SB_INSTALL_PREFIX}")
list(APPEND SB_CMAKE_CACHE_ARGS "-DBUILD_SHARED_LIBS:BOOL=${BUILD_SHARED_LIBS}")
list(APPEND SB_CMAKE_CACHE_ARGS "-DCMAKE_PREFIX_PATH:PATH=${SB_CMAKE_PREFIX_PATH}")
list(APPEND SB_CMAKE_CACHE_ARGS "-DCMAKE_SKIP_BUILD_RPATH:BOOL=${CMAKE_SKIP_BUILD_RPATH}")
list(APPEND SB_CMAKE_CACHE_ARGS "-DCMAKE_BUILD_WITH_INSTALL_RPATH:BOOL=${CMAKE_BUILD_WITH_INSTALL_RPATH}")
list(APPEND SB_CMAKE_CACHE_ARGS "-DCMAKE_INSTALL_RPATH_USE_LINK_PATH:BOOL=${CMAKE_INSTALL_RPATH_USE_LINK_PATH}")
list(APPEND SB_CMAKE_CACHE_ARGS "-DCMAKE_PREFIX_PATH:STRING=${SB_CMAKE_PREFIX_PATH}")
foreach(prefix_path ${SB_CMAKE_PREFIX_PATH})
list(APPEND SB_CMAKE_CACHE_ARGS "-DCMAKE_INSTALL_RPATH:PATH=${prefix_path}/lib")
endforeach()

#message("SB_CMAKE_CACHE_ARGS=${SB_CMAKE_CACHE_ARGS}")

foreach(cmake_var
    CMAKE_C_FLAGS
    CMAKE_CXX_FLAGS
    CMAKE_EXE_LINKER_FLAGS
    CMAKE_SHARED_LINKER_FLAGS
    CMAKE_MODULE_LINKER_FLAGS
    CMAKE_STATIC_LINKER_FLAGS
    )
  if(${cmake_var})
    list(APPEND SB_CMAKE_CACHE_ARGS "-D${cmake_var}:STRING=${${cmake_var}}")
    list(APPEND SB_CMAKE_CACHE_ARGS "-D${cmake_var}:STRING=${${cmake_var}}")
  endif()
endforeach()

if(CMAKE_VERBOSE_MAKEFILE)
  list(APPEND SB_CMAKE_CACHE_ARGS "-DCMAKE_VERBOSE_MAKEFILE:BOOL=ON")
endif()

set(SB_CMAKE_ARGS "-G${CMAKE_GENERATOR}")

set(SB_CONFIGURE_ARGS)
list(APPEND SB_CONFIGURE_ARGS "--prefix=${SB_INSTALL_PREFIX}")

if(BUILD_SHARED_LIBS)
  list(APPEND SB_CONFIGURE_ARGS "--enable-static=no")
  list(APPEND SB_CONFIGURE_ARGS "--enable-shared=yes")
else()
  list(APPEND SB_CONFIGURE_ARGS "--enable-static=yes")
  list(APPEND SB_CONFIGURE_ARGS "--enable-shared=no")
endif()

if(CMAKE_BUILD_TYPE MATCHES "Debug")
  list(APPEND SB_CONFIGURE_ARGS "--enable-debug=yes")
endif()

set(SB_ENV_CONFIGURE_CMD env CC=${CMAKE_C_COMPILER} CXX=${CMAKE_CXX_COMPILER})

if(CMAKE_C_FLAGS)
  list(APPEND SB_ENV_CONFIGURE_CMD "CFLAGS=${CMAKE_C_FLAGS}")
endif()
if(CMAKE_CXX_FLAGS)
  list(APPEND SB_ENV_CONFIGURE_CMD "CXXFLAGS=${CMAKE_CXX_FLAGS}")
endif()
if(CMAKE_EXE_LINKER_FLAGS)
  list(APPEND SB_ENV_CONFIGURE_CMD "LDFLAGS=${CMAKE_EXE_LINKER_FLAGS}")
endif()
if(CMAKE_SHARED_LINKER_FLAGS)
  list(APPEND SB_ENV_CONFIGURE_CMD "LDFLAGS=${CMAKE_SHARED_LINKER_FLAGS}")
endif()
if(CMAKE_MODULE_LINKER_FLAGS)
  list(APPEND SB_ENV_CONFIGURE_CMD "LDFLAGS=${CMAKE_MODULE_LINKER_FLAGS}")
endif()

#~ list(APPEND SB_ENV_CONFIGURE_CMD "LDFLAGS=-R${SB_INSTALL_PREFIX}/lib")
#~ list(APPEND SB_ENV_CONFIGURE_CMD "LDFLAGS=-L${SB_INSTALL_PREFIX}/lib")
#~ list(APPEND SB_ENV_CONFIGURE_CMD "CPPFLAGS=-I${SB_INSTALL_PREFIX}/include")
list(APPEND SB_ENV_CONFIGURE_CMD "LD_LIBRARY_PATH=${SB_INSTALL_PREFIX}/lib:$ENV{LD_LIBRARY_PATH}")
list(APPEND SB_ENV_CONFIGURE_CMD "PKG_CONFIG_PATH=${SB_INSTALL_PREFIX}/lib/pkgconfig")


list(REMOVE_DUPLICATES SB_ENV_CONFIGURE_CMD)

if("${CMAKE_CXX_COMPILER_VERSION}" VERSION_LESS "4.5") 
  set(DISABLE_CXX_WARNING_OPTION "-DCMAKE_CXX_FLAGS:STRING=-w")
endif()

configure_file(${CMAKE_SOURCE_DIR}/CMake/CTestCustom.cmake.in
  ${CMAKE_BINARY_DIR}/CTestCustom.cmake @ONLY)

find_program(PATCHELF_COMMAND NAMES ${CMAKE_BINARY_DIR}/PATCHELF/src/patchelf/src/)
find_program(EGREP_COMMAND NAMES egrep)

set(EXPAT_URL "http://downloads.sourceforge.net/project/expat/expat/2.1.0/expat-2.1.0.tar.gz")
set(EXPAT_URL_MD5 dd7dab7a5fea97d2a6a43f511449b7cd)
set(EXPAT_CONFIG_ARGS -DBUILD_examples:BOOL=OFF -DBUILD_tests:BOOL=OFF -DBUILD_tools:BOOL=OFF)

set(ZLIB_URL "http://downloads.sourceforge.net/project/libpng/zlib/1.2.8/zlib-1.2.8.tar.gz")
set(ZLIB_URL_MD5 44d667c142d7cda120332623eab69f40)

set(PNG_URL "http://downloads.sourceforge.net/project/libpng/libpng16/older-releases/1.6.16/lpng1616.zip")
set(PNG_URL_MD5 c90c9587c9a5c735327fb3f6900f6b03)
set(PNG_DEPENDS ZLIB)
set(PNG_CONFIG_ARGS -DPNG_TESTS:BOOL=OFF -DPNG_STATIC:BOOL=${BUILD_STATIC_LIBS})

set(PROJ_URL "http://download.osgeo.org/proj/proj-4.9.3.tar.gz")
set(PROJ_URL_MD5 d598336ca834742735137c5674b214a1)
set(PROJ_CONFIG_ARGS
  -DBUILD_LIBPROJ_SHARED:BOOL=TRUE
  -DVERSIONED_OUTPUT:BOOL=FALSE
  -DBUILD_FRAMEWORKS_AND_BUNDLE:BOOL=FALSE
  -DPROJ_LIB_SUBDIR:STRING=lib
  -DPROJ_INCLUDE_SUBDIR:STRING=include)
  
set(TIFF_URL "http://download.osgeo.org/libtiff/tiff-4.0.8.tar.gz")
set(TIFF_URL_MD5 2a7d1c1318416ddf36d5f6fa4600069b)
set(TIFF_DEPENDS ZLIB JPEG)
set(TIFF_CONFIG_ARGS
  -DCMAKE_INSTALL_LIBDIR:PATH=lib
  -DCMAKE_INSTALL_BINDIR:PATH=bin
  -DCMAKE_WINDOWS_EXPORT_ALL_SYMBOLS:BOOL=TRUE
  -DBUILD_TESTING:BOOL=FALSE
  -Djpeg:BOOL=TRUE
  -Dlzma:BOOL=FALSE
  -Djbig:BOOL=FALSE
  -Dzlib:BOOL=TRUE
  -DWITH_OPENGL:BOOL=FALSE
  -Dpixarlog:BOOL=TRUE
  -Dcxx:BOOL=FALSE
  -Dwith_opengl:BOOL=FALSE
  -Dwith_tools:BOOL=FALSE
  -Dwith_test:BOOL=FALSE
  -Dwith_contrib:BOOL=FALSE
  -Dwith_docs:BOOL=FALSE)
  
set(GEOTIFF_URL "http://download.osgeo.org/geotiff/libgeotiff/libgeotiff-1.4.2.tar.gz")
set(GEOTIFF_URL_MD5 96ab80e0d4eff7820579957245d844f8)
set(GEOTIFF_DEPENDS TIFF PROJ JPEG ZLIB)
set(GEOTIFF_CONFIG_ARGS
  -DBUILD_TESTING:BOOL=OFF
  -DPROJ4_OSGEO4W_HOME:PATH=${SB_INSTALL_PREFIX})

set(SQLITE_URL "http://www.sqlite.org/2015/sqlite-amalgamation-3080801.zip")
set(SQLITE_URL_MD5 b1cbcbd710bdfd762dc169f1676053b5)

set(OPENJPEG_URL "https://github.com/uclouvain/openjpeg/archive/v2.2.0.tar.gz")
set(OPENJPEG_URL_MD5 269bb0b175476f3addcc0d03bd9a97b6)
#set(OPENJPEG_URL "file:///${DOWNLOAD_DIR}/openjpeg-2.0.0-with-patch-revision-2313.tar.gz")
#set(OPENJPEG_URL_MD5 7d095cb98dedfbe25af19c547161a3de)
set(OPENJPEG_DEPENDS ZLIB TIFF PNG)
set(OPENJPEG_CONFIG_ARGS
  -DBUILD_CODEC:BOOL=ON
  -DBUILD_DOC:BOOL=OFF
  -DBUILD_JPIP:BOOL=OFF
  -DBUILD_JPWL:BOOL=OFF
  -DBUILD_MJ2:BOOL=OFF
  -DBUILD_PKGCONFIG_FILES:BOOL=ON
  -DBUILD_THIRDPARTY:BOOL=OFF
  -DBUILD_THIRDPARTY_LCMS:BOOL=ON)
  
set(GEOS_URL "http://download.osgeo.org/geos/geos-3.6.1.tar.bz2")
set(GEOS_URL_MD5 c97e338b3bc81f9848656e9d693ca6cc)

set(GEOS_CONFIG_ARGS
   -DGEOS_ENABLE_TESTS:BOOL=OFF
   -DGEOS_ENABLE_MACOSX_FRAMEWORK:BOOL=OFF
   -DGEOS_BUILD_STATIC:BOOL=${BUILD_STATIC_LIBS}
   -DGEOS_BUILD_SHARED:BOOL=${BUILD_SHARED_LIBS})

set(OPENTHREADS_URL "http://www.openscenegraph.org/downloads/stable_releases/OpenSceneGraph-3.4.0/source/OpenSceneGraph-3.4.0.zip")
set(OPENTHREADS_URL_MD5 a5e762c64373a46932e444f6f7332496)

set(OSSIM_DEPENDS TIFF GEOTIFF GEOS JPEG PNG OPENTHREADS)
set(OSSIM_URL "http://download.osgeo.org/ossim/source/ossim-1.8.20/ossim-1.8.20-3.tar.gz")
set(OSSIM_URL_MD5 eb2265db0d4d9201e255b92317121cfd)
set(OSSIM_CONFIG_ARGS
  -DBUILD_OSSIM_FREETYPE_SUPPORT:BOOL=OFF
  -DBUILD_OSSIM_MPI_SUPPORT:BOOL=OFF
  -DBUILD_OSSIM_APPS:BOOL=OFF
  -DBUILD_OSSIM_TEST_APPS:BOOL=OFF
  -DINSTALL_ARCHIVE_DIR:STRING=lib
  -DINSTALL_LIBRARY_DIR:STRING=lib  
  )

set(BOOST_URL "https://downloads.sourceforge.net/project/boost/boost/1.60.0/boost_1_60_0.tar.bz2")
set(BOOST_URL_MD5 65a840e1a0b13a558ff19eeb2c4f0cbe)

set(TINYXML_URL "http://downloads.sourceforge.net/project/tinyxml/tinyxml/2.6.2/tinyxml_2_6_2.tar.gz")
set(TINYXML_URL_MD5 c1b864c96804a10526540c664ade67f0)

#set(MUPARSER_URL "https://launchpad.net/ubuntu/+archive/primary/+files/muparser_1.34.orig.tar.gz")
#set(MUPARSER_URL_MD5 0c4f4bf86aa2a5a737adc0e08cb77737)

# set(MUPARSERX_URL "https://github.com/beltoforion/muparserx/archive/v4.0.7.zip")
# set(MUPARSERX_URL_MD5 67819fc478436ea8f647c804e6f2e1a2)

set(FREETYPE_URL "http://download.savannah.gnu.org/releases/freetype/freetype-2.6.tar.gz")
set(FREETYPE_URL_MD5 1d733ea6c1b7b3df38169fbdbec47d2b)

set(LIBGD_URL "https://github.com/libgd/libgd/releases/download/gd-2.2.5/libgd-2.2.5.tar.gz")
set(LIBGD_URL_MD5 ab2bd17470b51387eadfd5289b5c0dfb)
set(LIBGD_DEPENDS TIFF JPEG PNG FREETYPE)
set(LIBGD_CONFIG_ARGS
  -DCMAKE_INSTALL_LIBDIR:STRING=lib
  -DENABLE_PNG:BOOL=1
  -DENABLE_JPEG:BOOL=1
  -DENABLE_TIFF:BOOL=1
  -DENABLE_ICONV:BOOL=1
  -DENABLE_XPM:BOOL=0
  -DENABLE_FREETYPE:BOOL=1
  -DENABLE_FONTCONFIG:BOOL=0
  )

set(LIBXML2_URL  "ftp://xmlsoft.org/libxslt/libxml2-2.9.7.tar.gz")
set(LIBXML2_URL_MD5 896608641a08b465098a40ddf51cefba)
set(LIBXML2_AUTOCONF_BUILD 1)
set(LIBXML2_CONFIG_ARGS "--with-python=${SB_INSTALL_PREFIX}/bin/python3" "--with-lzma=no")
set(LIBXML2_DEPENDS PYTHON)

set(LIBXSLT_URL  "ftp://xmlsoft.org/libxslt/libxslt-1.1.32.tar.gz")
set(LIBXSLT_URL_MD5 1fc72f98e98bf4443f1651165f3aa146)
set(LIBXSLT_AUTOCONF_BUILD 1)
set(LIBXSLT_CONFIG_ARGS
  --without-python
  --with-crypto=no
  --with-debug=no
  --with-debugger=no
  --with-libxml-prefix=${SB_INSTALL_PREFIX}
  )

set(LIBXSLT_DEPENDS LIBXML2)

set(NETCDF_URL "https://github.com/Unidata/netcdf-c/archive/v4.4.1.1.tar.gz")
set(NETCDF_URL_MD5 9210fd5355bee868684d9b8f83064aa6)
set(NETCDF_DEPENDS HDF4)
set(NETCDF_CONFIG_ARGS
  --disable-netcdf4
  --enable-hdf4
  --enable-dap=no)
set(NETCDF_AUTOCONF_BUILD 1)

set(PUGIXML_URL "https://github.com/zeux/pugixml/releases/download/v1.5/pugixml-1.5.tar.gz")
set(PUGIXML_URL_MD5 77a69633cc2fe0858fd177e01e95cb54)
set(PUGIXML_CONFIG_ARGS
  -DCMAKE_INSTALL_LIBDIR:STRING=lib
  -DCMAKE_INSTALL_BINDIR:STRING=bin)


function(build_projects projects)
  foreach(project ${${projects}})
    build_project(${project})
  endforeach()
endfunction()

function(build_project P_NAME)
  if(TARGET ${P_NAME})
    return()
  endif()

  if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/CMake/${P_NAME}.cmake)
    message(STATUS "Using CMake/${P_NAME}.cmake" )
    include(${CMAKE_CURRENT_SOURCE_DIR}/CMake/${P_NAME}.cmake)
    return()
  endif()

  foreach(DEP ${${P_NAME}_DEPENDS})
     build_project(${DEP})
   endforeach()

   if(${P_NAME}_AUTOCONF_BUILD)
     ExternalProject_Add(${P_NAME}
       URL "${${P_NAME}_URL}"
       URL_MD5 ${${P_NAME}_URL_MD5}
       PREFIX       ${P_NAME}
       TMP_DIR      ${P_NAME}/tmp
       STAMP_DIR    ${P_NAME}/stamp
       DOWNLOAD_DIR ${DOWNLOAD_DIR}
       SOURCE_DIR   ${P_NAME}/source
       BINARY_DIR   ${P_NAME}/build
       INSTALL_DIR ${SB_INSTALL_PREFIX}
       DEPENDS ${${P_NAME}_DEPENDS}
       CONFIGURE_COMMAND "${SB_ENV_CONFIGURE_CMD};${CMAKE_BINARY_DIR}/${P_NAME}/source/configure"
       ${SB_CONFIGURE_ARGS}
       ${${P_NAME}_CONFIG_ARGS}
       LOG_DOWNLOAD ${WRITE_LOG}
       LOG_CONFIGURE ${WRITE_LOG}
       LOG_BUILD ${WRITE_LOG}
       LOG_INSTALL ${WRITE_LOG}
       )     
   else()
     ExternalProject_Add(${P_NAME}
       URL "${${P_NAME}_URL}"
       URL_MD5 ${${P_NAME}_URL_MD5}
       PREFIX       ${P_NAME}
       TMP_DIR      ${P_NAME}/tmp
       STAMP_DIR    ${P_NAME}/stamp
       DOWNLOAD_DIR ${DOWNLOAD_DIR}
       SOURCE_DIR   ${P_NAME}/source
       BINARY_DIR   ${P_NAME}/build
       INSTALL_DIR ${SB_INSTALL_PREFIX}
       DEPENDS ${${P_NAME}_DEPENDS}
       CMAKE_COMMAND ${CMAKE_COMMAND}
       CMAKE_CACHE_ARGS ${SB_CMAKE_CACHE_ARGS}
       ${${P_NAME}_CONFIG_ARGS}
       LOG_DOWNLOAD ${WRITE_LOG}
       LOG_CONFIGURE ${WRITE_LOG}
       LOG_BUILD ${WRITE_LOG}
       LOG_INSTALL ${WRITE_LOG}
       )
   endif()
if(IS_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/patches/${P_NAME}/)
  SUPERBUILD_PATCH_SOURCE(${P_NAME})
endif()

endfunction() 

build_project(MAJA)

add_custom_target(binpkg
  DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/.maja_bin_pkg.stamp
  WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
  COMMENT "MAJA-${MAJA_VERSION} binary package is created successfully!"
  VERBATIM
  )

add_custom_command(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/.maja_bin_pkg.stamp
  COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_CURRENT_BINARY_DIR}/bin_pkg.dir
  COMMAND ${CMAKE_COMMAND} -E chdir ${CMAKE_CURRENT_BINARY_DIR}/bin_pkg.dir ${CMAKE_COMMAND}
  ${CMAKE_CURRENT_SOURCE_DIR}/../Packaging
  -DSUPERBUILD_BINARY_DIR=${CMAKE_BINARY_DIR}
  -DSUPERBUILD_INSTALL_DIR=${SB_INSTALL_PREFIX}
  -DWITH_VALIDATION=OFF
  -DDOWNLOAD_DIR=${DOWNLOAD_DIR}
  COMMAND ${CMAKE_COMMAND} --build ${CMAKE_CURRENT_BINARY_DIR}/bin_pkg.dir --target install
  COMMAND ${CMAKE_COMMAND} -E touch ${CMAKE_CURRENT_BINARY_DIR}/.maja_bin_pkg.stamp
  )

add_custom_target(valpkg
  DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/.maja_val_pkg.stamp
  WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
  COMMENT "MAJA-${MAJA_VERSION} validation package is created successfully!"
  VERBATIM
  )

add_custom_command(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/.maja_val_pkg.stamp
  COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_CURRENT_BINARY_DIR}/val_pkg.dir
  COMMAND ${CMAKE_COMMAND} -E chdir ${CMAKE_CURRENT_BINARY_DIR}/val_pkg.dir ${CMAKE_COMMAND}
  ${CMAKE_CURRENT_SOURCE_DIR}/../Packaging
  -DSUPERBUILD_BINARY_DIR=${CMAKE_BINARY_DIR}
  -DSUPERBUILD_INSTALL_DIR=${SB_INSTALL_PREFIX}
  -DWITH_VALIDATION=ON
  -DDOWNLOAD_DIR=${DOWNLOAD_DIR}
  COMMAND ${CMAKE_COMMAND} --build ${CMAKE_CURRENT_BINARY_DIR}/val_pkg.dir --target install
  COMMAND ${CMAKE_COMMAND} -E touch ${CMAKE_CURRENT_BINARY_DIR}/.maja_val_pkg.stamp
  )

message(STATUS "SYSTEM_HOSTNAME           = ${SYSTEM_HOSTNAME}")
message(STATUS "MAJA_VERSION             = ${MAJA_VERSION}")
message(STATUS "MAJA_CORE_VERSION        = ${MAJA_CORE_VERSION}")
message(STATUS "MAJA_ALGORITHMS_VERSION  = ${MAJA_ALGORITHMS_VERSION}")
message(STATUS "CMAKE_INSTALL_PREFIX     = ${SB_INSTALL_PREFIX}")
message(STATUS "PATCHELF_COMMAND         = ${PATCHELF_COMMAND}")
message(STATUS "EGREP_COMMAND (for HDF4) = ${EGREP_COMMAND}")
#message(STATUS "To install to a different directory, re-run cmake -DCMAKE_INSTALL_PREFIX=/your/preferred/path")
