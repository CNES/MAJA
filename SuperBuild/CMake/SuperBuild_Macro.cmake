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
# VERSION : 3.1.0 : DM : LAIG-DM-MAJA-2526-CNES : 9 avril 2018 : Montée de version de d'OTB 6.0            #
#                                                                                                          #
# FIN-HISTORIQUE                                                                                           #
#                                                                                                          #
# $Id$                                                                                                     #
#                                                                                                          #
############################################################################################################


include(CMakeParseArguments)

# Macro to add a configure variable to ${proj}_SB_CONFIG
# optional 3rd argument : suffix to the variable
macro(add_configure_option project lib config_arg)
  set(suffix "${ARGV3}")  
  if(TARGET ${lib})
    set(${project}_CONFIG_ARGS
      ${${project}_CONFIG_ARGS}
      "${config_arg}=${SB_INSTALL_PREFIX}${suffix}")
  else()
    set(${project}_CONFIG_ARGS ${${project}_CONFIG_ARGS}
      "${config_arg}=yes")    
  endif()
endmacro()

macro(SUPERBUILD_PATCH_SOURCE project)
  set(${project}_PATCH_DIR ${CMAKE_SOURCE_DIR}/patches/${project})
  string(TOLOWER ${project} patch_dir_prefix)
  set(PATCH_ARGS "${ARGV1}")

  if(WIN32)
    set(DIFF_FILE_MATCH_STRING "win")
    if(NOT PATCH_ARGS)
      set(PATCH_ARGS "--binary")
    endif()
  else()
    if(APPLE)
      set(DIFF_FILE_MATCH_STRING "macx")
    else() #Linux
      set(DIFF_FILE_MATCH_STRING "linux")
    endif()
  endif() #WIN32

  #if an optional diff file is mentioned. then only that is applied
  set(DOT_DIFF_FILES "${ARGV2}")

  if(NOT DOT_DIFF_FILES)
    #If DOT_DIFF_FILES is empty,
    #we try to list files ending with all.diff of <DIFF_FILE_MATCH_STRING>.diff
    #the final list is stored in DOT_DIFF_FILES and applied using patch.cmake
    #Custom patches requrired for...

    #glob all files ending with DIFF_FILE_MATCH_STRING.diff
    file(GLOB files_list "${${project}_PATCH_DIR}/${patch_dir_prefix}*${DIFF_FILE_MATCH_STRING}.diff")

    #glob all files ending with all.diff
    file(GLOB all_files_list "${${project}_PATCH_DIR}/${patch_dir_prefix}*all.diff")

    #merge two list for the final one
    list(APPEND files_list ${all_files_list})

    #because we are passing it cmake_command using -D!!
    string(REPLACE ";" " " DOT_DIFF_FILES "${files_list}")
  endif()

  if(DOT_DIFF_FILES)
    message(STATUS "Custom patches required for ${project}")
    ExternalProject_Add_Step(${project} ${project}_custom_patch
      COMMAND
      ${CMAKE_COMMAND}
      -DSOURCE_DIR=${CMAKE_BINARY_DIR}/${project}/source/
      -DDOT_DIFF_FILES=${DOT_DIFF_FILES}
      -DPATCH_ARGS=${PATCH_ARGS}
      -P ${CMAKE_SOURCE_DIR}/CMake/patch.cmake
      DEPENDEES patch update
      DEPENDERS configure
      )
  endif()

endmacro(SUPERBUILD_PATCH_SOURCE)
