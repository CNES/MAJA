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
# Build the UserManual
#

if (NOT UNIX)
  message (STATUS "Not on Unix: skipping UserManual build.")
  return()
endif()

message(STATUS "")
message(STATUS "Configuring UserManual...")

find_program(SH_INTERP sh)
mark_as_advanced(SH_INTERP)

# Note: we do not use either:
# - find_program(SPHINX_BUILD NAMES sphinx-build)
# - find_packages(PythonInterp)
# because they don't work with python virtualenv (in CMake < 3.13.0),
# so in calls to add_custom_target() below, we call them directly.
# See CMake bug: https://gitlab.kitware.com/cmake/cmake/issues/18302

find_program(LATEX_COMMAND NAMES latex)
mark_as_advanced(LATEX_COMMAND)

find_program(TAR_COMMAND NAMES tar)
mark_as_advanced(TAR_COMMAND)

find_program(PDFLATEX_COMMAND NAMES pdflatex )
mark_as_advanced(PDFLATEX_COMMAND)

find_program(MAKEINDEX_COMMAND NAMES makeindex )
mark_as_advanced(MAKEINDEX_COMMAND)

# Check that we found everything we need
foreach(cmd
    LATEX_COMMAND
    TAR_COMMAND
    PDFLATEX_COMMAND
    MAKEINDEX_COMMAND
    SH_INTERP)
  if(NOT ${cmd})
    message(FATAL_ERROR "Error while configuring Cookbook, ${cmd} not set. Cannot continue")
  endif()
endforeach()

set(RST_SOURCE_DIR  ${CMAKE_CURRENT_SOURCE_DIR}/rst)
set(RST_BINARY_DIR  ${CMAKE_CURRENT_BINARY_DIR}/rst)
set(LATEX_DIR       ${CMAKE_CURRENT_BINARY_DIR}/latex)

# Print summary of Cookbook configuration
message(STATUS "RST_SOURCE_DIR = ${RST_SOURCE_DIR}")
message(STATUS "RST_BINARY_DIR = ${RST_BINARY_DIR}")
message(STATUS "LATEX_DIR = ${LATEX_DIR}")

# Clean any existing build
macro(remove_and_make_directories)
  foreach(dir in ${ARGV})
    execute_process(COMMAND ${CMAKE_COMMAND} -E remove_directory ${dir})
    execute_process(COMMAND ${CMAKE_COMMAND} -E make_directory ${dir})
  endforeach()
endmacro()

remove_and_make_directories(
  ${RST_BINARY_DIR}
  ${RST_BINARY_DIR}/Applications/
  ${LATEX_DIR}
  ${CMAKE_CURRENT_BINARY_DIR}/_static
  )

file(COPY ${RST_SOURCE_DIR}  DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/Art DESTINATION  ${RST_BINARY_DIR})

set(SPHINX_CONF_DIR ${CMAKE_CURRENT_BINARY_DIR})

set(MAJA_COPYRIGHT_TEXT "2019-2020 CNES. The documentation of the MAJA software is lincensed under the Creative Commons Attribution 4.0 international license (CC BY 4.0)"

configure_file(${RST_SOURCE_DIR}/conf.py.in ${SPHINX_CONF_DIR}/conf.py @ONLY)
message(${SPHINX_CONF_DIR}/conf.py)

 add_custom_target(CookBookTexFromRST
   COMMAND sphinx-build
   -b latex
   ${RST_BINARY_DIR}
   ${LATEX_DIR}
   -v
   -c ${SPHINX_CONF_DIR}
   WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
   COMMENT "Generating Tex files from rst"
   VERBATIM)

set(USERMANUAL_TEX UserManual-${MAJA_VERSION_MAJOR}.${MAJA_VERSION_MINOR}.${MAJA_VERSION_PATCH})

 add_custom_target(CookBookPDF
   ALL
   COMMAND ${PDFLATEX_COMMAND} ${USERMANUAL_TEX}.tex
   COMMAND ${PDFLATEX_COMMAND} ${USERMANUAL_TEX}.tex
   COMMAND ${PDFLATEX_COMMAND} ${USERMANUAL_TEX}.tex
   COMMAND ${PDFLATEX_COMMAND} ${USERMANUAL_TEX}.tex
   COMMAND ${PDFLATEX_COMMAND} ${USERMANUAL_TEX}.tex
   WORKING_DIRECTORY ${LATEX_DIR}
   DEPENDS CookBookTexFromRST
   COMMENT "Building RST documentation in pdf")
