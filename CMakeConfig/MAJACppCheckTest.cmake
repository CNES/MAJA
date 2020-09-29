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
# VERSION : 1.0.0 : FA : LAIG-FA-MAC-148399-CS : 26 octobre 2016 : Audit code - Correction qualite         #
#                                                                                                          #
# FIN-HISTORIQUE                                                                                           #
#                                                                                                          #
# $Id$                                                                                                     #
#                                                                                                          #
############################################################################################################

if(NOT CPPCHECK_FOUND)
  find_package(cppcheck QUIET)
endif()

if(CPPCHECK_FOUND)
  if(NOT TARGET all_cppcheck)
    add_custom_target(all_cppcheck)
    set_target_properties(all_cppcheck PROPERTIES EXCLUDE_FROM_ALL TRUE)
  endif()
endif()

if(CPPCHECK_FOUND)
  set(CPPCHECK_ARGS ${CPPCHECK_FORCE_ARG} ${CPPCHECK_VERBOSE_ARG} ${CPPCHECK_ALL_ARG})
  #list all dir containing at leat one cxx file
  # Select only runtime source files excluding Testing directory
  
  file(GLOB_RECURSE cxx_files Code/*.cxx Plugins/*.cxx)
  list( LENGTH cxx_files _num_cxx_files )

  if( ${_num_cxx_files} GREATER 0 )

    get_property(mytargdef DIRECTORY
          "${CMAKE_CURRENT_SOURCE_DIR}" PROPERTY COMPILE_DEFINITIONS)

    set( cppcheck_def )
    foreach( compiledef ${mytargdef} )
       set( cppcheck_def ${cppcheck_def} -D${compiledef} )
    endforeach()

    set( cppcheck_include )
#    get_property( mytargINCLUDES DIRECTORY
#      "${CMAKE_CURRENT_SOURCE_DIR}" PROPERTY INCLUDE_DIRECTORIES)

#    foreach( _includeDirs ${mytargINCLUDES} )
#      set( _cppcheck_include ${_cppcheck_include} -I${_includeDirs} )
#    endforeach()

    # let's take of include dirs here
    foreach( includeDirs ${MAJA_INCLUDE_DIRS_BUILD_TREE} )
      set( cppcheck_include ${cppcheck_include} -I${includeDirs} )
    endforeach()

    set(files)
    foreach(source ${cxx_files})
        #message("CPP CHECK file : ${source}")
    #      if ( ${source} MATCHES ".*Plugin.*" )		   
        list(APPEND files "${source}")
    #  endif()
    endforeach()

    set( cppcheck_compile_args ${cppcheck_include} ${cppcheck_def} )
    add_test(
  	NAME
        cppcheck_test
        COMMAND
        "${CPPCHECK_EXECUTABLE}"
        ${CPPCHECK_TEMPLATE_ARG}
        ${CPPCHECK_ARGS}
        ${cppcheck_compile_args}
        ${files})

    set_tests_properties(cppcheck_test
        PROPERTIES
        TIMEOUT "18000"
        FAIL_REGULAR_EXPRESSION
          "${CPPCHECK_FAIL_REGULAR_EXPRESSION}")

  endif()
  
endif()
