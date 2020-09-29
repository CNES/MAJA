#!/bin/sh -x
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

INSTALL_DIR=$1
CMAKE_BINARY_DIR=$2
PATCHELF_EXE=$CMAKE_BINARY_DIR/PATCHELF/src/patchelf/src/patchelf

"$PATCHELF_EXE" "--set-rpath" "$INSTALL_DIR/lib" "$INSTALL_DIR/bin/python3.7"
"$PATCHELF_EXE" "--set-rpath" "$INSTALL_DIR/lib" "$INSTALL_DIR/bin/python3.7m"
"chmod" "u+w" "$INSTALL_DIR/lib/libpython3.so"
"chmod" "u+w" "$INSTALL_DIR/lib/libpython3.7m.so.1.0"

pylibs=$(find "$INSTALL_DIR/lib/" -type f -name libpython3*.so*)
binaries=$(find "$INSTALL_DIR/lib/python3.7/lib-dynload" -type f -exec file {} \; | grep -i elf| grep -i dynamically | cut -f1 -d':')
for bfile in $binaries $pylibs; do
    if [ -f "$bfile" ]; then
        "$PATCHELF_EXE" "--set-rpath" "$INSTALL_DIR/lib" "$bfile"
    fi
done

rm -rf $INSTALL_DIR/lib/python3.7/test
