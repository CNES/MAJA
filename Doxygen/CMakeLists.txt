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
option(BUILD_DOCUMENTATION
  "Build doxygen-based code documentation." OFF)

mark_as_advanced(BUILD_DOCUMENTATION)

# -------------------------------------------------------------
#
# Generate Doxygen configuration file in all configuration file
#
if( ${DOXYGEN_DOT_FOUND} )
  set( ITK_HAVE_DOT "YES" )
else()
  set( ITK_HAVE_DOT "NO" )
endif()



if (BUILD_DOCUMENTATION)
   set( DOXYGEN_TEST_DIR ${MACCS_SOURCE_DIR}/Doxygen )
   configure_file(${MACCS_SOURCE_DIR}/Doxygen/Doxyfile.config.in
	            ${MAJA_BINARY_DIR}/Doxygen/Doxyfile.config)

   add_custom_target(Documentation ALL
    COMMAND doxygen ${MAJA_BINARY_DIR}/Doxygen/Doxyfile.config 
    MAIN_DEPENDENCY ${MAJA_BINARY_DIR}/Doxygen/Doxyfile.config
    WORKING_DIRECTORY ${MAJA_BINARY_DIR}/Doxygen
    )
endif (BUILD_DOCUMENTATION)
