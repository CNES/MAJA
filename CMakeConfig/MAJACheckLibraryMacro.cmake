#
# Copyright (C) 2020 Centre National d'Etudes Spatiales (CNES)
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
#                                                                                                          #
# VERSION : 1-0-0 : <TypeFT> : <NumFT> : 1 avril 2010 : Creation                                                           
#                                                                                                          #
# FIN-HISTORIQUE                                                                                           #
#                                                                                                          #
# $Id$
#                                                                                                          #
############################################################################################################
#
# Checks libary installation
#
# NAME - variable to set the name of the library
#



function(CHECK_LIBRARY NAME)
  MESSAGE(STATUS "Checking the installation of the ${NAME} library")
  set(include_dirs)
  if(${NAME}_INCLUDE_DIRS)
    set(include_dirs ${${NAME}_INCLUDE_DIRS})
  elseif(${NAME}_INCLUDE_DIR)
    set(include_dirs ${${NAME}_INCLUDE_DIR})
  else()
    message(FATAL_ERROR "${NAME}_INCLUDE_DIR or ${NAME}_INCLUDE_DIRS is unset")
    return()
  endif()

  set(link_libraries)
  if(${NAME}_LIBRARIES)
    set(link_libraries ${${NAME}_LIBRARIES})
  elseif(${NAME}_LIBRARY)
    set(link_libraries ${${NAME}_LIBRARY})
  else()
    message(FATAL_ERROR "${NAME}_LIBRARY or ${NAME}_LIBRARIES is unset")
    return()
  endif()

  set(COMPIL_RESULT 0)
  set(RUN_RESULT 0)
  TRY_RUN(RUN_RESULT COMPIL_RESULT
    ${CMAKE_CURRENT_BINARY_DIR}/CMakeConfig
    ${CMAKE_CURRENT_SOURCE_DIR}/CMakeConfig/Test${NAME}.cxx
    CMAKE_FLAGS
      "-DINCLUDE_DIRECTORIES:PATH=${include_dirs}"
      "-w"
      "-DLINK_LIBRARIES:STRING=${link_libraries}"    
    COMPILE_OUTPUT_VARIABLE comp
    RUN_OUTPUT_VARIABLE run
    )

if(NOT COMPIL_RESULT)
    message(FATAL_ERROR
      "Compile and link of executable with ${NAME} library - FAILED\n 
		   Message: ${comp}. \n
		   link_libraries=${link_libraries} \ninclude_dirs=${include_dirs}")
    return()
endif()

if(NOT RUN_RESULT)
    message(STATUS "Running of hdf4 tests - FAILED")
    message(FATAL_ERROR
      "Try to run a executable compiled with ${NAME} library failed.\n Message: ${run}. 
	  	   Linked with ${NAME} library (${link_libraries}  with ${include_dirs} include dir) failed.")
    return()
endif()

message(STATUS "${NAME} library check - SUCCESS")

endfunction() #CHECK_LIBRARY NAME

