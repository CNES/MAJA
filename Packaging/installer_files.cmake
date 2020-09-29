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
# Rashad Kanavath: modified for MAJA

macro(installer_files)
  configure_file(${CMAKE_CURRENT_SOURCE_DIR}/Files/linux_pkgsetup.in
    ${CMAKE_CURRENT_BINARY_DIR}/pkgsetup @ONLY)

  install(PROGRAMS
    ${CMAKE_BINARY_DIR}/pkgsetup
    ${CMAKE_BINARY_DIR}/make_symlinks
    "${PATCHELF_PROGRAM}"
    Files/README
    DESTINATION ${PKG_STAGE_DIR}
    )

    set(GDAL_INSTALL_DIR "OUT_DIR")
    set(MY_MAJA_INSTALL_DIR "OUT_DIR")
    set(GDAL_DATA "OUT_DIR/share/gdal")
    set(GEOTIFF_CSV "OUT_DIR/share/epsg_csv")
    set(MY_MAJA_APP_DIR "OUT_DIR/lib/applications")
    set(MY_MAJA_PYTHON_DIR "OUT_DIR//lib/python")
    set(MY_MAJA_LIB_DIR "OUT_DIR//lib")
    set(MY_MAJA_PATH_DIRS "OUT_DIR/bin")

    CONFIGURE_FILE(${CMAKE_CURRENT_SOURCE_DIR}/../Code/Core/majaenv.sh.in
       majaenv_pkg_tree.sh @ONLY)
  
  install(PROGRAMS ${CMAKE_CURRENT_BINARY_DIR}/majaenv_pkg_tree.sh
    RENAME .majaenv.sh
    DESTINATION ${PKG_STAGE_DIR}/bin)

  install(PROGRAMS
    ${MAJA_TEST_EXE_DIR}/maja
    DESTINATION ${PKG_STAGE_DIR}/bin)

  install(PROGRAMS
    ${START_MAJA_TEST_EXE_DIR}/startmaja
    DESTINATION ${PKG_STAGE_DIR}/bin)

  install(PROGRAMS
    ${START_MAJA_TEST_EXE_DIR}/dtmcreation
    DESTINATION ${PKG_STAGE_DIR}/bin)

  install(PROGRAMS
    ${START_MAJA_TEST_EXE_DIR}/camsdownload
    DESTINATION ${PKG_STAGE_DIR}/bin)
endmacro()




