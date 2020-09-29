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

set(LOADER_REGEX_EOL "E")
set(DEST_BIN_DIR bin)
set(LIB_PREFIX lib)
set(DEST_LIB_DIR lib)
set(EXE_EXT "")
set(SCRIPT_EXT ".sh")
set(LIB_EXT ".so")
set(PYMODULE_EXT ".so")
set(STATIC_LIB_EXT ".a")

set(SYSTEM_DLLS
  libm.so
  libc.so.*
  libstdc*
  libgcc_s.so
  librt.so
  libdl.so
  libpthread.so
  libidn.so
  libgomp.so*
  ld-linux-x86-64.so*
  libX11.so*
  libXi.so*  #GLUT
  libXcursor.so* #GLFW
  libXinerama.so*  #GLFW
  libXext.so*
  libXau.so*
  libXdmcp.so*
  libXxf86vm.so*
  libdrm.so.2
  libGL.so*
  libGLU.so*
  libXrender.so*
  libSM.so*
  libICE.so*
  libXrandr.so*
  libutil.so*
  libcrypt.so*
  libcrypto.so*
  )
