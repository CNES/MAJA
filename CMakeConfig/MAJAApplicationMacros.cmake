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

macro(maja_test_application)
  cmake_parse_arguments(TESTAPPLICATION  "" "NAME;APP" "OPTIONS;TESTENVOPTIONS;VALID" ${ARGN} )
  if(otb-module)
    otb_add_test(NAME ${TESTAPPLICATION_NAME}
                  COMMAND ${CMAKE_INSTALL_PREFIX}/bin/otbTestDriver.sh
                  ${TESTAPPLICATION_VALID}
                  Execute $<TARGET_FILE:otbApplicationLauncherCommandLine>
                  ${TESTAPPLICATION_APP}
                  $<TARGET_FILE_DIR:otbapp_${TESTAPPLICATION_APP}>
                  ${TESTAPPLICATION_OPTIONS}
                  -testenv ${TESTAPPLICATION_TESTENVOPTIONS})
    # Be sure that the ${otb-module}-all target triggers the build of commandline launcher and testdriver
    add_dependencies(${otb-module}-all otbApplicationLauncherCommandLine)
    add_dependencies(${otb-module}-all otbTestDriver)
  else()
    add_test(NAME ${TESTAPPLICATION_NAME}
            COMMAND ${CMAKE_INSTALL_PREFIX}/bin/otbTestDriver.sh
            ${TESTAPPLICATION_VALID}
            Execute $<TARGET_FILE:otbApplicationLauncherCommandLine>
            ${TESTAPPLICATION_APP}
            $<TARGET_FILE_DIR:otbapp_${TESTAPPLICATION_APP}>
            ${TESTAPPLICATION_OPTIONS}
            -testenv ${TESTAPPLICATION_TESTENVOPTIONS})
  endif()
endmacro()
