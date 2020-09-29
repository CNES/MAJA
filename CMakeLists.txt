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
cmake_minimum_required(VERSION 3.10.2)
PROJECT(MAJA)
############################################################################################################
#                                                                                                          #
#                                        __  __    __     ____   __                                        #
#                                       (  \/  )  /__\   (_  _) /__\                                       #
#                                        )    (  /(__)\ .-_)(  /(__)\                                      #
#                                       (_/\/\_)(__)(__)\____)(__)(__)                                     #
#                                                                                                          #
#                                                                                                          #
############################################################################################################
#                                                                                                          #
# Author: CS Systemes d'Information  (France)                                                              #
#                                                                                                          #
############################################################################################################
# HISTORIQUE                                                                                               #
# 
# VERSION : 3.0.0 : DM : LAIG-DM-MAJA-2369-CNES : 28 aout 2017 : Introduction données des CAMS             #
# VERSION : 2.0.0 : DM : LAIG-DM-MAJA-156758-CS : 25 avril 2017 : Paquet RPM - ajout des dépendances 'gd'  #
#                                                                                             et 'libxml2' #
# VERSION : 2.0.0 : DM : LAIG-DM-MAJA-153436-CS : 6 mars 2017 : Refactoring pour ameliorer la qualite      #
# VERSION : 1.0.0 : DM : LAIG-DM-MAC-151090-CS : 28 novembre 2016 : Renommage de MACCS a MAJA            #
# VERSION : 1.0.0 : FA : LAIG-FA-MAC-148399-CS : 11 octobre 2016 : Audit code - Correction qualite         #
# VERSION : 5-1-0 : FA : LAIG-FA-MAC-144674-CS : 6 juin 2016 : Modification passage des COTS en parametre  #
# VERSION : 5-0-0 : FA : LAIG-FA-MAC-143764-CS : 12 avril 2016 : Mise en forme du code (indentation, etc.) #
# VERSION : 4-0-0 : FA : LAIG-FA-MAC-117040-CS : 13 mars 2014 : Modifications mineures                     #
# VERSION : 3-3-0 : DM : LAIG-DM-MAC-744-CNES : 17 fevrier 2014 : Remplacement de S2N_ par S2__            #
#                                                                               (plugin SENTINEL2)         #
# VERSION : 3-2-0 : DM : LAIG-DM-MAC-697-CNES : 29 novembre 2013 : Mise à jour du plugin SENTINEL2         #
# VERSION : 3-1-0 : DM : LAIG-DM-MAC-614-CNES : 28 octobre 2013 : Creation plugin LANDSAT                  #
# VERSION : 3-0-0 : DM : LAIG-DM-MAC-239-CNES : 10 mai 2013 : Modification pour la prise en compte de la DM #
#                                                            du MACCS generique (PM LAIG-NT-MAC-0239-CNES) #
# VERSION : 2-3-0 : DM : LAIG-DM-MAC-399-CNES : 28 aout 2012 : Modification suite a l'install relocatable  #
# VERSION : 2-1-0 : FA : LAIG-FA-MAC-335-CNES : 30 mai 2012 : Verification de l'install de la lib GD.      #
# VERSION : 2-0-0 : DM : LAIG-DM-MAC-146-CNES : 14 octobre 2011 : Modification pour prise en compte        #
#                                                            d'evolutions liees a MACCS                    #
#                                                                                                          #
# VERSION : 1-0-0-2 : FA : 284 : 2 juin 2011 : Correction pour la version dans les TaskTables                #
#                                                                                                          #
# VERSION : 1-0-0 : <TypeFT> : <NumFT> : 15 novembre 2009 : Creation                                                           
#                                                                                                          #
# FIN-HISTORIQUE                                                                                           #
#                                                                                                          #
# $Id$
#                                                                                                          #
############################################################################################################

message(STATUS "CMAKE_SYSTEM_NAME: ${CMAKE_SYSTEM_NAME}")
message(STATUS "CMAKE_HOST_SYSTEM_NAME: ${CMAKE_HOST_SYSTEM_NAME}")
message(STATUS "CMAKE_SYSTEM_VERSION: ${CMAKE_SYSTEM_VERSION}")
message(STATUS "CMAKE_HOST_SYSTEM_VERSION: ${CMAKE_HOST_SYSTEM_VERSION}")

set(MAJA_LIST_OF_SPECTRAL_CAMERA "" CACHE INTERNAL "MAJA_LIST_OF_SPECTRAL_CAMERA")

#-----------------------------------------------------------------------------
# Manage Versions and Plugins information
INCLUDE(${PROJECT_SOURCE_DIR}/CMakeConfig/vnsMAJAGlobalConfiguration.cmake)
INCLUDE(${PROJECT_SOURCE_DIR}/CMakeConfig/MAJAApplicationMacros.cmake)

# MESSAGE(STATUS "--------------------------------------")
# MESSAGE(STATUS "ENV MAJA_INSTALL_DIR: '$ENV{MAJA_INSTALL_DIR}'")
# MESSAGE(STATUS "--------------------------------------")
IF(COMMAND CMAKE_POLICY)
  CMAKE_POLICY(SET CMP0003 NEW)
  CMAKE_POLICY(SET CMP0010 NEW)
ENDIF()

if(POLICY CMP0033)
  cmake_policy(SET CMP0033 NEW)
endif()
if(POLICY CMP0050)
  cmake_policy(SET CMP0050 OLD)
endif()

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


#SET(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${PROJECT_SOURCE_DIR} ${PROJECT_SOURCE_DIR}/CMakeConfig)
SET(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${PROJECT_SOURCE_DIR}/CMakeConfig)
# MAJA option to access large files (> 4Go).
ADD_DEFINITIONS(
  -D_LARGEFILE_SOURCE
  -D_FILE_OFFSET_BITS=64
  -D_LARGEFILE64_SOURCE
  -D_LARGE_FILES
)

if(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
  set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -Wl,--no-as-needed")
  set(CMAKE_MODULE_LINKER_FLAGS "${CMAKE_MODULE_LINKER_FLAGS} -Wl,--no-as-needed")
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,--no-as-needed")
  set(MAJA_REQUIRED_LINK_FLAGS "${MAJA_REQUIRED_LINK_FLAGS} -Wl,--no-as-needed")
endif()

# Use to generate the cpack name (rpm)
IF(NOT MAJA_PACKAGE_NAME)
    SET( MAJA_PACKAGE_NAME "maja")
ENDIF(NOT MAJA_PACKAGE_NAME)


set(MAJA_IMPORT_LIBRARIES)

include(MAJACheckLibraryMacro)

SET(ITK_NO_IO_FACTORY_REGISTER_MANAGER 1)

#Import Expat
find_package(Expat REQUIRED)
include_directories(${EXPAT_INCLUDE_DIR})
set(MAJA_IMPORT_LIBRARIES ${MAJA_IMPORT_LIBRARIES} ${EXPAT_LIBRARY})

# Import Netcdf
FIND_PACKAGE(Netcdf REQUIRED)
include_directories(${NETCDF_INCLUDE_DIR})
set(MAJA_IMPORT_LIBRARIES ${MAJA_IMPORT_LIBRARIES} ${NETCDF_LIBRARY})

# Import HDF4
FIND_PACKAGE(Hdf4 REQUIRED)
include_directories(${HDF4_INCLUDE_DIR})
# Check HDF installation
CHECK_LIBRARY(HDF4)
set(MAJA_IMPORT_LIBRARIES ${MAJA_IMPORT_LIBRARIES}
  ${HDF4_MFHDF_LIBRARY} ${HDF4_DF_LIBRARY})

# Import GDAL
FIND_PACKAGE(GDAL REQUIRED)
include_directories(${GDAL_INCLUDE_DIR})
message(STATUS "Found gdal_rasterize: ${GDAL_APPS_DIR}/gdal_rasterize")
set(MAJA_IMPORT_LIBRARIES ${MAJA_IMPORT_LIBRARIES} ${GDAL_LIBRARY})

# Import OTB
FIND_PACKAGE(OTB REQUIRED)
#message(STATUS "not including OTB_USE_FILE '${OTB_USE_FILE}'")
message(STATUS "OTB_DIR  : ${OTB_DIR}")
# Add compiler flags used by OTB.
set(CMAKE_C_FLAGS "${OTB_REQUIRED_C_FLAGS} ${CMAKE_C_FLAGS}")
set(CMAKE_CXX_FLAGS "${OTB_REQUIRED_CXX_FLAGS} ${CMAKE_CXX_FLAGS}")
set(CMAKE_EXE_LINKER_FLAGS "${OTB_REQUIRED_LINK_FLAGS} ${CMAKE_EXE_LINKER_FLAGS}")
set(CMAKE_SHARED_LINKER_FLAGS "${OTB_REQUIRED_LINK_FLAGS} ${CMAKE_SHARED_LINKER_FLAGS}")
set(CMAKE_MODULE_LINKER_FLAGS "${OTB_REQUIRED_LINK_FLAGS} ${CMAKE_MODULE_LINKER_FLAGS}")
#update MAJA_IMPORT_LIBRARIES with OTB_LIBRARIES
set(MAJA_IMPORT_LIBRARIES ${MAJA_IMPORT_LIBRARIES} ${OTB_LIBRARIES})
set(OTB_SOURCE_DIR ${OTB_SOURCE_DIR})

#ITK is already found when find_package(OTB) is called.
#The story doesn't end there. Sadly we got a sequel to find_package(ITK)!
#backup ITK_LIBRARIES
set(OTB_ITK_LIBRARIES ${ITK_LIBRARIES})
# run find_package(ITK) for ITKIOMeta ITKMetaIO
# These two components are additionally required by MAJA
if(CMAKE_VERSION VERSION_LESS "2.8.12.4")
  list(APPEND ITK_MODULES_REQUESTED ITKIOMeta ITKMetaIO)
  list(APPEND OTB_ITK_LIBRARIES ITKIOMeta ITKMetaIO)
endif()
FIND_PACKAGE(ITK REQUIRED COMPONENTS ITKIOMeta ITKMetaIO)
# # update ITK_LIBRARIES. we need all the other itk modules from OTB + MetaIO (MAJA)
list(APPEND ITK_LIBRARIES ${OTB_ITK_LIBRARIES})
# #https://itk.org/Wiki/ITK/FAQ#NoFactoryException (needs to include itk_use_file)
include(${ITK_USE_FILE})
list(REMOVE_DUPLICATES ITK_LIBRARIES)
include_directories(BEFORE ${OTB_INCLUDE_DIRS})
set(MAJA_IMPORT_LIBRARIES ${MAJA_IMPORT_LIBRARIES} ${ITK_LIBRARIES})

find_package(Pugixml REQUIRED)
include_directories(${PUGIXML_INCLUDE_DIR})
set(MAJA_IMPORT_LIBRARIES ${MAJA_IMPORT_LIBRARIES} ${PUGIXML_LIBRARY})
#-----------------------------------------------------------------------------
# Import GD
find_package(GD REQUIRED)
include_directories(${GD_INCLUDE_DIR})
# Check GD installation
CHECK_LIBRARY(GD)

#find Python
set(PythonInterp_FIND_VERSION)
find_package(PythonInterp REQUIRED)


# Add Externals libraries include dir into the include path
#INCLUDE_DIRECTORIES( ${MAJA_EXTERNALS_LIBRARIES_INCLUDE_DIRS} )

#-----------------------------------------------------------------------------
# Enable running cppcheck for each module on its source and test directories.
option(MAJA_CPPCHECK_TEST "Run cppcheck for static code analysis" OFF)
mark_as_advanced(MAJA_CPPCHECK_TEST)

# Setup build locations.
if(NOT CMAKE_RUNTIME_OUTPUT_DIRECTORY)
  set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${MAJA_BINARY_DIR}/bin)
endif()
if(NOT CMAKE_LIBRARY_OUTPUT_DIRECTORY)
  set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${MAJA_BINARY_DIR}/lib)
endif()
if(NOT CMAKE_ARCHIVE_OUTPUT_DIRECTORY)
  set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${MAJA_BINARY_DIR}/lib)
endif()

execute_process(COMMAND ${CMAKE_COMMAND} -E make_directory
  ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
  
SET(MAJA_INCLUDE_DIRS_BUILD_TREE)
# Create the list of include directories needed for OTB header files.
INCLUDE(${PROJECT_SOURCE_DIR}/CMakeConfig/vnsIncludeDirectories.cmake)

INCLUDE_DIRECTORIES(
  ${MAJA_INCLUDE_DIRS_BUILD_TREE}
  ${MAJA_INCLUDE_DIRS_BUILD_TREE_CXX}
  ${MAJA_INCLUDE_DIRS_SYSTEM}
)


#-----------------------------------------------------------------------------
# Installation vars.
# MAJA_INSTALL_DIR              - root dir (init.sh, etc.)
# MAJA_INSTALL_BIN_DIR          - binary dir (executables)
# MAJA_INSTALL_LIB_DIR          - library dir (libs)
# MAJA_INSTALL_CONF_DIR         - conf dir (config files, dictionary, ICD, etc)
# MAJA_INSTALL_INCLUDE_DIR      - include dir (headers)
# MAJA_INSTALL_PACKAGE_DIR      - package/export configuration files
# MAJA_INSTALL_NO_DEVELOPMENT   - do not install development files
# MAJA_INSTALL_NO_RUNTIME       - do not install runtime files
# MAJA_INSTALL_NO_DOCUMENTATION - do not install documentation files

if(DEFINED ENV{MAJA_INSTALL_DIR})
  set(MAJA_INSTALL_DIR "$ENV{MAJA_INSTALL_DIR}")
endif()

IF(NOT MAJA_INSTALL_DIR)
  SET(MAJA_INSTALL_DIR "${CMAKE_INSTALL_PREFIX}")
ENDIF(NOT MAJA_INSTALL_DIR)

IF(NOT MAJA_INSTALL_BIN_DIR)
  SET(MAJA_INSTALL_BIN_DIR "bin")
ENDIF(NOT MAJA_INSTALL_BIN_DIR)

IF(NOT MAJA_INSTALL_LIB_DIR)
  SET(MAJA_INSTALL_LIB_DIR "lib")
ENDIF(NOT MAJA_INSTALL_LIB_DIR)

IF(NOT MAJA_INSTALL_INCLUDE_DIR)
  SET(MAJA_INSTALL_INCLUDE_DIR "include")
ENDIF(NOT MAJA_INSTALL_INCLUDE_DIR)

IF(NOT MAJA_INSTALL_PACKAGE_DIR)
  SET(MAJA_INSTALL_PACKAGE_DIR "lib")
ENDIF(NOT MAJA_INSTALL_PACKAGE_DIR)

if(NOT OTB_INSTALL_APP_DIR)
  set(OTB_INSTALL_APP_DIR "lib/applications")
endif()

# CmakeConfig modules
SET(MAJA_INSTALL_CMAKEMODULE_DIR ${MAJA_INSTALL_PACKAGE_DIR}/cmakemodules)

# /share 

IF(NOT MAJA_INSTALL_SCHEMAS_DIR)
  SET(MAJA_INSTALL_SCHEMAS_DIR "share/schemas")
ENDIF(NOT MAJA_INSTALL_SCHEMAS_DIR)

# DOC
IF(NOT MAJA_INSTALL_DOC_DIR)
  SET(MAJA_INSTALL_DOC_DIR "share/docs")
ENDIF(NOT MAJA_INSTALL_DOC_DIR)

# DDC
IF(NOT MAJA_INSTALL_DDC_ROOT_DIR)
  SET(MAJA_INSTALL_DDC_ROOT_DIR "share")
ENDIF(NOT MAJA_INSTALL_DDC_ROOT_DIR)

# Dictionaries
IF(NOT MAJA_INSTALL_ADMIN_CONFIG_DIR)
  SET(MAJA_INSTALL_ADMIN_CONFIG_DIR "share/config")
ENDIF(NOT MAJA_INSTALL_ADMIN_CONFIG_DIR)

# Some of the are moved to CMakeConfig/CommonCmakeOptions.cmake
# This was for reuse with maja and maja-val binary package

IF(NOT MAJA_INSTALL_NO_DEVELOPMENT)
  SET(MAJA_INSTALL_NO_DEVELOPMENT 0)
ENDIF(NOT MAJA_INSTALL_NO_DEVELOPMENT)

IF(NOT MAJA_INSTALL_NO_RUNTIME)
  SET(MAJA_INSTALL_NO_RUNTIME 0)
ENDIF(NOT MAJA_INSTALL_NO_RUNTIME)

IF(NOT MAJA_INSTALL_NO_DOCUMENTATION)
  SET(MAJA_INSTALL_NO_DOCUMENTATION 0)
ENDIF(NOT MAJA_INSTALL_NO_DOCUMENTATION)

SET(MAJA_INSTALL_NO_LIBRARIES ${MAJA_INSTALL_NO_RUNTIME})


IF(BUILD_SHARED_LIBS)
  IF(MAJA_INSTALL_NO_RUNTIME AND MAJA_INSTALL_NO_DEVELOPMENT)
    SET(MAJA_INSTALL_NO_LIBRARIES ON)
  ENDIF(MAJA_INSTALL_NO_RUNTIME AND MAJA_INSTALL_NO_DEVELOPMENT)
ELSE()
  IF(MAJA_INSTALL_NO_DEVELOPMENT)
    SET(MAJA_INSTALL_NO_LIBRARIES ON)
  ENDIF(MAJA_INSTALL_NO_DEVELOPMENT)
ENDIF()


IF("${CMAKE_INSTALL_PREFIX}" STREQUAL "${CMAKE_BINARY_DIR}")
  MESSAGE(FATAL_ERROR
    "The current CMAKE_INSTALL_PREFIX points at the build tree:\n"
    "  ${CMAKE_INSTALL_PREFIX}\n"
    "This is not supported."
    )
ENDIF()


# For Developpers : activate full compilation warning
OPTION(MAJA_COMPILE_WITH_FULL_WARNING "MAJA developers : Compilation with FULL warning (use only for venus developers)." ON)
IF(MAJA_COMPILE_WITH_FULL_WARNING)
  IF(CMAKE_COMPILER_IS_GNUCXX)
    SET(MAJA_FULL_WARNING_LIST_PARAMETERS "-Wall -Wunused  -Wunused-function  -Wunused-label  -Wunused-parameter -Wunused-value -Wunused-variable -Wsign-compare -Wparentheses -Wunknown-pragmas -Wswitch -Wno-deprecated" CACHE STRING "List of compilation parameters.")
    MARK_AS_ADVANCED(MAJA_FULL_WARNING_LIST_PARAMETERS)
    SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${MAJA_FULL_WARNING_LIST_PARAMETERS}")
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${MAJA_FULL_WARNING_LIST_PARAMETERS}")

    # Specific options: add for optimisation and analyse performances
#    SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Winline")
#    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Winline")

    # Add Specifics flags for RELEASE mode
    SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -Wuninitialized")
    SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wuninitialized")
  ENDIF(CMAKE_COMPILER_IS_GNUCXX)
ENDIF(MAJA_COMPILE_WITH_FULL_WARNING)

#-----------------------------------------------------------------------------
# For Developpers : activate gprof compilation flags
OPTION(MAJA_PROFILE "MAJA developers : Compilation with GPROF options flags (use only for venus developers)." OFF)
MARK_AS_ADVANCED(MAJA_PROFILE)
IF(MAJA_PROFILE)
  IF(CMAKE_COMPILER_IS_GNUCXX)
    SET(MAJA_PROFILE_PARAMETERS "-pg" CACHE STRING "List of profiling parameters.")
    MARK_AS_ADVANCED(MAJA_PROFILE_PARAMETERS)
    SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${MAJA_PROFILE_PARAMETERS}")
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${MAJA_PROFILE_PARAMETERS}")
    SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${MAJA_PROFILE_PARAMETERS}")
    SET(CMAKE_MODULE_LINKER_FLAGS "${CMAKE_MODULE_LINKER_FLAGS} ${MAJA_PROFILE_PARAMETERS}")
    SET(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} ${MAJA_PROFILE_PARAMETERS}")
  ENDIF(CMAKE_COMPILER_IS_GNUCXX)
  # Find gprof program !
  FIND_PROGRAM(MAJA_PROFILE_PERF_COMMAND gprof )
ENDIF(MAJA_PROFILE)

#-----------------------------------------------------------------------------
# Instantiate the templated filters in the Venus libraries
OPTION(MAJA_CXX_INSTANTIATION "Include into the Venus libraries the instantiation of templated filters." ON)
MARK_AS_ADVANCED(MAJA_CXX_INSTANTIATION)


#-----------------------------------------------------------------------------
# Code location
ADD_SUBDIRECTORY(Utilities)
ADD_SUBDIRECTORY(Code)


#-----------------------------------------------------------------------------
# Plugins location
ADD_SUBDIRECTORY(Plugins)

#-----------------------------------------------------------------------------
# Plugins location
ADD_SUBDIRECTORY(StartMaja)

#-----------------------------------------------------------------------------
# DDC location
IF(NOT MAJA_INSTALL_NO_LIBRARIES)
    # INSTALL ddc
    INSTALL(DIRECTORY ddc 
            DESTINATION ${MAJA_INSTALL_DDC_ROOT_DIR}
            PATTERN ".svn" EXCLUDE)
ENDIF(NOT MAJA_INSTALL_NO_LIBRARIES)


#-----------------------------------------------------------------------------
# Output directories.


#-----------------------------------------------------------------------------
# Enable / Disable testing
INCLUDE(${CMAKE_ROOT}/Modules/Dart.cmake)
MARK_AS_ADVANCED(TCL_TCLSH DART_ROOT)
CONFIGURE_FILE(CMakeConfig/CTestCustom.cmake.in CTestCustom.cmake)
set(CXX_TEST_PATH ${MAJA_BINARY_DIR}/bin)
INCLUDE(CTest)

if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/Testing")
    ADD_SUBDIRECTORY(Testing)
endif() #if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/Testing")

#-----------------------------------------------------------------------------
# include cppcheck macros
if(MAJA_CPPCHECK_TEST)
  include( MAJACppCheckTest )
endif()

#-----------------------------------------------------------------------------
# Generate MAJAConfig.cmake

# Save library dependencies.
#SET(MAJA_LIBRARY_DEPENDS_FILE ${PROJECT_BINARY_DIR}/MAJALibraryDepends.cmake)
#EXPORT_LIBRARY_DEPENDENCIES(${MAJA_LIBRARY_DEPENDS_FILE})

# Library directory.
#SET(MAJA_LIBRARY_DIRS ${OTB_LIBRARY_DIRS} ${LIBRARY_OUTPUT_PATH})

SET(MAJA_USE_FILE ${PROJECT_BINARY_DIR}/UseMAJA.cmake)

#CONFIGURE_FILE(${PROJECT_SOURCE_DIR}/CMakeConfig/MAJAConfig.cmake.in
#               ${PROJECT_BINARY_DIR}/MAJAConfig.cmake @ONLY IMMEDIATE)

CONFIGURE_FILE(${PROJECT_SOURCE_DIR}/CMakeConfig/UseMAJA.cmake.in
               ${MAJA_USE_FILE} @ONLY IMMEDIATE)

# Configure MAJA_VERSIONS file 
SET(MAJA_VERSIONS_FILE ${PROJECT_BINARY_DIR}/MAJA_VERSION_FOR_SW_COMPONENT)

CONFIGURE_FILE(${PROJECT_SOURCE_DIR}/CMakeConfig/MAJA_VERSIONS.cmake.in
               ${MAJA_VERSIONS_FILE} @ONLY IMMEDIATE)

# Install VERSION file in the BINARIES package (no in DEVEL package) 
IF(NOT MAJA_INSTALL_NO_LIBRARIES)
  INSTALL(FILES 
    ${MAJA_VERSIONS_FILE}
    DESTINATION ${CMAKE_INSTALL_PREFIX} )
ENDIF()

IF(NOT MAJA_INSTALL_NO_DEVELOPMENT)
  #    ${PROJECT_BINARY_DIR}/MAJALibraryDepends.cmake
  INSTALL(FILES
    ${MAJA_USE_FILE}
    DESTINATION ${MAJA_INSTALL_PACKAGE_DIR})
  
  INSTALL(FILES
    ${PROJECT_SOURCE_DIR}/CMakeConfig/FindExpat.cmake
    ${PROJECT_SOURCE_DIR}/CMakeConfig/FindGD.cmake
    ${PROJECT_SOURCE_DIR}/CMakeConfig/FindHdf4.cmake
    ${PROJECT_SOURCE_DIR}/CMakeConfig/FindPugixml.cmake
    ${PROJECT_SOURCE_DIR}/CMakeConfig/FindGDAL.cmake
    DESTINATION ${MAJA_INSTALL_CMAKEMODULE_DIR}
    )

ENDIF() #NOT MAJA_INSTALL_NO_DEVELOPMENT

# Save library dependencies.
#EXPORT_LIBRARY_DEPENDENCIES(${PROJECT_BINARY_DIR}/MAJALibraryDepends.cmake)

# Create the MAJAConfig.cmake file containing the MAJA configuration.
INCLUDE (${PROJECT_SOURCE_DIR}/CMakeConfig/vnsGenerateMAJAConfig.cmake)

# Setup valgrind option (default hard coded value is ctest is invalid)
SET(VALGRIND_COMMAND_OPTIONS  "--show-reachable=yes --workaround-gcc296-bugs=yes --num-callers=50")

# Provide an option for generating documentation.
add_subdirectory(Doxygen)

# uninstall target
configure_file("${CMAKE_SOURCE_DIR}/CMakeConfig/cmake_uninstall.cmake.in"
  "${CMAKE_BINARY_DIR}/cmake_uninstall.cmake"
  IMMEDIATE @ONLY)

add_custom_target(uninstall
  COMMAND ${CMAKE_COMMAND} -P ${CMAKE_BINARY_DIR}/cmake_uninstall.cmake)

install(DIRECTORY ${MAJA_BINARY_DIR}/share DESTINATION ${CMAKE_INSTALL_PREFIX})
install(DIRECTORY ${MAJA_BINARY_DIR}/etc DESTINATION ${CMAKE_INSTALL_PREFIX})
file(COPY ${PROJECT_SOURCE_DIR}/orchestrator  DESTINATION ${MAJA_BINARY_DIR}/lib/python/)
CONFIGURE_FILE(${MAJA_BINARY_DIR}/lib/python/orchestrator/common/version.py
               ${MAJA_BINARY_DIR}/lib/python/orchestrator/common/version.py)
install(DIRECTORY ${MAJA_BINARY_DIR}/lib/python/orchestrator DESTINATION ${CMAKE_INSTALL_PREFIX}/lib/python/)


file(COPY ${PROJECT_SOURCE_DIR}/StartMaja  DESTINATION ${MAJA_BINARY_DIR}/lib/python/)
install(DIRECTORY ${MAJA_BINARY_DIR}/lib/python/StartMaja DESTINATION ${CMAKE_INSTALL_PREFIX}/lib/python/)


#if(ENABLE_TV)
#  message(STATUS "ENABLE_TV=${ENABLE_TV}.We will configure TVs.")
#endif()
#if(ENABLE_TVA)
#  message(STATUS "ENABLE_TVA=${ENABLE_TVA}. We will configure TVAs")
#endif()
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/Testing" AND ENABLE_TU)
  message(STATUS "ENABLE_TU=${ENABLE_TU}. We will configure TUs")
endif()


#----------------------------------------------------------------------
# Provide an option for generating documentation.
option(BUILD_DOCS "Build the documentations." OFF)

if (BUILD_DOCS)
    add_subdirectory(Documentation/UserManual)
    add_subdirectory(Documentation/Conception)
endif()
