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
function(prepare_file_list file_list_result)

  set(file_list
    "${SUPERBUILD_INSTALL_DIR}/bin/xmllint"
    "${SUPERBUILD_INSTALL_DIR}/bin/xsltproc"
    "${SUPERBUILD_INSTALL_DIR}/bin/python3.7"
    )
  if(WITH_VALIDATION)
    list(APPEND file_list "${MAJA_TEST_EXE_DIR}/vnsDummyProgram")
    list(APPEND file_list "${MAJA_TEST_EXE_DIR}/vnsInstallDataCxx")
    #list(APPEND file_list "${MAJA_TEST_EXE_DIR}/vnsTestMainLauncher")
  endif()
  #message(FATAL_ERROR "file_list=${file_list}")
  file(GLOB file_list_glob
    "${SUPERBUILD_INSTALL_DIR}/bin/gdal*${EXE_EXT}"
    "${SUPERBUILD_INSTALL_DIR}/bin/ogr*${EXE_EXT}"
    "${SUPERBUILD_INSTALL_DIR}/lib/otb/applications/*${LIB_EXT}"
    "${SUPERBUILD_INSTALL_DIR}/lib/applications/*${LIB_EXT}"
    "${SUPERBUILD_INSTALL_DIR}/bin/otbTestDriver*"
    "${MAJA_TEST_EXE_DIR}/*[T|t]est*${EXE_EXT}"
    )

  list(REMOVE_ITEM file_list_glob  "${SUPERBUILD_INSTALL_DIR}/bin/gdal-config")
  #vnsTestMainLauncher[.sh] files get into file_list_glob.
  #we have to remove them explicitly!
  if(NOT WITH_VALIDATION)
    list(REMOVE_ITEM file_list_glob
      "${MAJA_TEST_EXE_DIR}/vnsTestMainLauncher"
      "${MAJA_TEST_EXE_DIR}/vnsTestMainLauncher.sh"
      )
  endif()

  list(APPEND file_list "${file_list_glob}")

  list(APPEND file_list "${SUPERBUILD_INSTALL_DIR}/lib/otb/python/_otbApplication${PYMODULE_EXT}")
     
  
  # foreach(file ${file_list_glob})
  #   get_filename_component(file_NAME ${file} NAME)
  #   list(APPEND file_list "${file}")
  # endforeach()

  #reverse file list and return
  list(REVERSE file_list)
  set(${file_list_result} ${file_list} PARENT_SCOPE)
endfunction()
