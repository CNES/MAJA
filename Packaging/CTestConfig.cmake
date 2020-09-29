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

set(CTEST_PROJECT_NAME "MAJA")
set(CTEST_NIGHTLY_START_TIME "20:00:00 CEST")
set(CTEST_DROP_METHOD "http")
set(CTEST_DROP_SITE "172.26.46.217/cdash")
set(CTEST_DROP_LOCATION "/submit.php?project=MAJA")
set(CTEST_DROP_SITE_CDASH TRUE)
set(CTEST_CUSTOM_MAXIMUM_FAILED_TEST_OUTPUT_SIZE 4096)
set(CTEST_CUSTOM_MAXIMUM_PASSED_TEST_OUTPUT_SIZE 4096)
