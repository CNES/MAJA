#
# Copyright (C) 2020 Centre National d'Etudes Spatiales (CNES)
#
#Licensed under the Apache License, Version 2.0 (the "License");
#you may not use this file except in compliance with the License.
#You may obtain a copy of the License at
#
#    http://www.apache.org/licenses/LICENSE-2.0
#
#Unless required by applicable law or agreed to in writing, software
#distributed under the License is distributed on an "AS IS" BASIS,
#WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
#See the License for the specific language governing permissions and
#limitations under the License.
#
# target_path and link_path are relative to OUT_DIR

function(update_make_symlinks_script target_path link_path)
  if(target_path)
    file(APPEND
      ${CMAKE_BINARY_DIR}/make_symlinks
      "ln -sf \"$OUT_DIR/${target_path}\" \"$OUT_DIR/${link_path}\"\n")
  endif()
endfunction()
