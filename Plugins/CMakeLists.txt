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
############################################################################################################
#                                                                                                          #
#                                ooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo         #
#                             o                                                                            #
#                          o                                                                               #
#                        o                                                                                 #
#                      o                                                                                   #
#                     o       ooooooo       ooooooo    o         o      oo                                 #
#    o              o       o        o     o       o   o         o     o   o                               #
#      o           o       o          o   o         o  o         o    o      o                             #
#        o        o       o           o   o         o  o         o    o        o                           #
#         o      o        o      oooo     o         o  o         o   o           o                         #
#          o    o          o              o         o  o         o   o           o                         #
#           o  o            o             o         o  o o      o   o          o                           #
#            oo              oooooooo    o         o   o  oooooo   o      oooo                             #
#                                                     o                                                    #
#                                                     o                                                    #
#                                                    o                            o                        #
#                                                    o            o      oooo     o   o      oooo          #
#                                                   o             o         o    o    o         o          #
#                                                   o            o       ooo     o   o       ooo           #
#                                                               o       o       o   o          o           #
#                                                               ooooo   oooo    o   ooooo  oooo            #
#                                                                              o                           #
#                                                                                                          #
############################################################################################################
#                                                                                                          #
# Author: CS Systemes d'Information  (France)                                                              #
#                                                                                                          #
############################################################################################################
# HISTORIQUE                                                                                               #
#                                                                                                          #
# VERSION : 3.1.0 : DM : LAIG-DM-MAJA-2572-CNES : 22 mars 2018 : Nouveau Plugin SENTINEL2_TM               #
# VERSION : 5-1-0 : FA : LAIG-FA-MAC-1483-CNES : 24 mai 2016 : Organisation des schémas                    #
# VERSION : 4-0-0 : FA : LAIG-FA-MAC-117040-CS : 13 mars 2014 : Modifications mineures                     #
# VERSION : 2-0-0 : DM : LAIG-DM-MAC-146-CNES : 14 octobre 2011 : Modification pour prise en compte        #
#                                                            d'evolutions liees a MACCS                    #
#                                                                                                          #
# VERSION : 1-0-0 : <TypeFT> : <NumFT> : 15 novembre 2009 : Creation                                                           
#                                                                                                          #
# FIN-HISTORIQUE                                                                                           #
#                                                                                                          #
# $Id: CMakeLists.txt 4032 2012-04-12 08:41:39Z tfeuvrie $
#                                                                                                          #
############################################################################################################

execute_process(COMMAND ${CMAKE_COMMAND} -E make_directory "${CMAKE_CURRENT_BINARY_DIR}/.stamps/")

function(COPY_EXTRAS_FILES plugin_name output_dir input_files)
  message(STATUS "Copy input files for ${plugin_name} plugin to ${output_dir} ")
  foreach(input_file ${input_files})
    get_filename_component(input_file_EXT "${input_file}" EXT)
    get_filename_component(input_file_NAME_WE "${input_file}" NAME_WE)
    get_filename_component(input_file_NAME "${input_file}" NAME)
    get_filename_component(input_file_PATH "${input_file}" PATH)
    set(output_file_EXT ${input_file_EXT})
    set(stamp_file "")
    if("${input_file_EXT}" MATCHES ".DBL.DIR")
      set(output_file_EXT ".DBL")
      set(output_file "${output_dir}/${input_file_NAME_WE}${output_file_EXT}")
    else()
      set(output_file "${output_dir}/${input_file_NAME}")
    endif()
    if(EXISTS "${output_file}")
      file(SHA256 "${output_file}" sha256_hash)
      set(stamp_file "${CMAKE_CURRENT_BINARY_DIR}/.stamps/${sha256_hash}")    
    endif()
    if("${stamp_file}" STREQUAL "" OR
	NOT EXISTS "${stamp_file}")
      if(EXISTS "${input_file}/")
	#ex: tar cvzf VE_TEST_GIP_L2ALBD_L_CONTINEN_00001_00000000_99999999.DBL VE_TEST_GIP_L2ALBD_L_CONTINEN_00001_00000000_99999999.DBL.DIR
	#"tar -cjf " + outputFilename + " -C " + basePath + " " + baseModule + " --exclude '.svn'");
	message(STATUS "Creating tar file: ${output_file}")
	execute_process(COMMAND
      	  "tar" "cvjf" "${output_file}" -C "${input_file_PATH}" "${input_file_NAME}"
      	  WORKING_DIRECTORY ${input_file_PATH}
      	  RESULT_VARIABLE tar_rv
      	  OUTPUT_VARIABLE tar_ov)
	if(NOT tar_rv EQUAL 0)
	  message(FATAL_ERROR " tar_rv failed with output: '${tar_ov}'")		
	endif()
      else()
	message(STATUS "copy ${input_file_NAME} to ${output_dir}/")
	file(COPY ${input_file} DESTINATION "${output_dir}/")
      endif()

      file(SHA256 "${output_file}" sha256_hash)
      set(stamp_file "${CMAKE_CURRENT_BINARY_DIR}/.stamps/${sha256_hash}")
      if("${stamp_file}" STREQUAL "")
	message(FATAL_ERROR "ss=${output_file}")
      endif()  
      message(STATUS "touch ${stamp_file}")
      execute_process(COMMAND ${CMAKE_COMMAND} -E touch "${stamp_file}")
    endif() #(NOT EXISTS "${stamp_file}")
    endforeach()
endfunction() #COPY_EXTRAS_FILES)


#-----------------------------------------------------------------------------
# Builf executable used to geenrated and copy Gipps data (cpy files .HDR and .EEF 
# and compress the DBL.DIR in .DBL file)
ADD_EXECUTABLE( vnsInstallDataCxx vnsInstallData.cxx)
TARGET_LINK_LIBRARIES(vnsInstallDataCxx 
    ${MAJA_LIBRARY_PREFIXNAME}DataCommon
    ${OTB_LIBRARIES}
    ${ITK_LIBRARIES}
    )

if(MAJA_CORE_LIBRARY_PROPERTIES)
  SET_TARGET_PROPERTIES(vnsInstallDataCxx PROPERTIES ${MAJA_CORE_LIBRARY_PROPERTIES})
endif() #MAJA_CORE_LIBRARY_PROPERTIES


# Add subdirectories 
#ADD_SUBDIRECTORY(Common)
ADD_SUBDIRECTORY(Common/Interfaces)
#FOREACH(spectral_camera ${MAJA_LIST_OF_SPECTRAL_CAMERA})
#  ADD_SUBDIRECTORY(${spectral_camera})
#ENDFOREACH() #spectral_camera

#-----------------------------------------------------------------------------
# Install in the BINARIES package (no in DEVEL package) 
IF(NOT MAJA_INSTALL_NO_LIBRARIES)
  # Loop for all Spectral camera
  FOREACH(spectral_camera ${MAJA_LIST_OF_SPECTRAL_CAMERA})
    #--------------------------------------------------------------------------------
    # INSTALL Schemas
    #--------------------------------------------------------------------------------
    file(COPY ${spectral_camera}/Interfaces/Schemas/ 
      DESTINATION ${MAJA_BINARY_DIR}/${MAJA_INSTALL_SCHEMAS_DIR}/${spectral_camera})
    # MUSCATE Schemas
    if( "${spectral_camera}" STREQUAL "LANDSAT_MUSCATE" OR
	"${spectral_camera}" STREQUAL "LANDSAT8_MUSCATE" OR
	"${spectral_camera}" STREQUAL "SENTINEL2_MUSCATE" OR
	"${spectral_camera}" STREQUAL "SENTINEL2_TM" OR
        "${spectral_camera}" STREQUAL "VENUS_MUSCATE")
      file(COPY Common/Interfaces/Schemas/PSC-SL-411-0032-CG_Schemas
        DESTINATION ${MAJA_BINARY_DIR}/${MAJA_INSTALL_SCHEMAS_DIR}/${spectral_camera})
      file(COPY Common/Interfaces/Schemas/PSC-SL-411-0032-CG_Schemas_VERSIONS
        DESTINATION ${MAJA_BINARY_DIR}/${MAJA_INSTALL_SCHEMAS_DIR}/${spectral_camera} )
    endif()
    
    # Install Templates dir:
    file(COPY ${spectral_camera}/Interfaces/Config/Templates 
      DESTINATION ${MAJA_BINARY_DIR}/${MAJA_INSTALL_ADMIN_CONFIG_DIR}/${spectral_camera}
      )                
    #--------------------------------------------------------------------------------
    # Fichier modifiable user dans 
    # /etc/config/user
    #--------------------------------------------------------------------------------
    # Install MAJAUserConfig_${spectral_camera}.xml in CONFIG USER DIR
    file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/${spectral_camera}/Interfaces/Config/MAJAUserConfig_${spectral_camera}.xml 
      DESTINATION ${MAJA_BINARY_DIR}/${MAJA_INSTALL_CONF_USER_DIR} )
    # since V 4-1                DESTINATION ${MAJA_INSTALL_CONF_USER_DIR}/${spectral_camera} )
    
    #--------------------------------------------------------------------------------
    # Fichier modifiable admin dans 
    # /etc/config/admin
    #--------------------------------------------------------------------------------
    # Install MAJAAdminConfig_${spectral_camera}.xml in CONFIG ADMIN DIR
    file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/${spectral_camera}/Interfaces/Config/MAJAAdminConfig_${spectral_camera}.xml 
      DESTINATION ${MAJA_BINARY_DIR}/${MAJA_INSTALL_CONF_ADMIN_DIR} )
    # since V 4-1                DESTINATION ${MAJA_INSTALL_CONF_ADMIN_DIR}/${spectral_camera} )
  ENDFOREACH(spectral_camera)
endif() #NOT MAJA_INSTALL_NO_LIBRARIES


# COPY GIPP for VALIDATION directory
IF(NOT MAJA_INSTALL_NO_DEVELOPMENT)
  # Loop for all Spectral camera
  FOREACH(spectral_camera ${MAJA_LIST_OF_SPECTRAL_CAMERA})
    # create the directory to avoid loosing case on windows
    SET(INPUT_DIR  ${CMAKE_CURRENT_SOURCE_DIR}/${spectral_camera}/Interfaces/Data/Gipps)
    SET(OUTPUT_DIR ${MAJA_BINARY_DIR}/${MAJA_INSTALL_DATA_VALIDATION_DIR}/${spectral_camera})
    FILE(MAKE_DIRECTORY ${OUTPUT_DIR})
    # Read the list of selected files
    FILE(GLOB LIST_OF_FILES_EXTRACTED "${INPUT_DIR}/*")
    COPY_EXTRAS_FILES( "${spectral_camera}-VALIDATION" ${OUTPUT_DIR} "${LIST_OF_FILES_EXTRACTED}" )
    #MESSAGE(STATUS "Copying data Gipps for validation for ${spectral_camera} spectral camera")
    file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/README-VALIDATION
      DESTINATION ${MAJA_BINARY_DIR}/${MAJA_INSTALL_DATA_VALIDATION_DIR}/${spectral_camera} )
  ENDFOREACH(spectral_camera)
ENDIF() #NOT MAJA_INSTALL_NO_DEVELOPMENT
