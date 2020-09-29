#!/usr/bin/env bash
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
#
# ==============================================================================

set -u  # Check for undefined variables

die() {
  # Print a message and exit with code 1.
  #
  # Usage: die <error_message>
  #   e.g., die "Something bad happened."

  echo $@
  exit 1
}

echo "Collecting system information..."

OUTPUT_FILE=maja_env.txt
python_bin_path=$(which python || which python3 || die "Cannot find Python binary")

{
echo
echo '== check python ==================================================='
} >> ${OUTPUT_FILE}

cat <<EOF > /tmp/check_python.py
import platform

print("""python version: %s
python branch: %s
python build version: %s
python compiler version: %s
python implementation: %s
""" % (
platform.python_version(),
platform.python_branch(),
platform.python_build(),
platform.python_compiler(),
platform.python_implementation(),
))
EOF
${python_bin_path} /tmp/check_python.py 2>&1  >> ${OUTPUT_FILE}

{
echo
echo '== check os platform ==============================================='
} >> ${OUTPUT_FILE}

cat <<EOF > /tmp/check_os.py
import platform

print("""os: %s
os kernel version: %s
os release version: %s
os platform: %s
linux distribution: %s
linux os distribution: %s
mac version: %s
uname: %s
architecture: %s
machine: %s
""" % (
platform.system(),
platform.version(),
platform.release(),
platform.platform(),
platform.linux_distribution(),
platform.dist(),
platform.mac_ver(),
platform.uname(),
platform.architecture(),
platform.machine(),
))
EOF
${python_bin_path} /tmp/check_os.py 2>&1  >> ${OUTPUT_FILE}

{
  echo
  echo '== are we in docker ============================================='
  num=`cat /proc/1/cgroup | grep docker | wc -l`;
  if [ $num -ge 1 ]; then
    echo "Yes"
  else
    echo "No"
  fi
  
  echo
  echo '== compiler ====================================================='
  c++ --version 2>&1
  
  echo
  echo '== check for virtualenv ========================================='
  ${python_bin_path} -c "import sys;print(hasattr(sys, \"real_prefix\"))"
  
  echo
}
echo "Wrote environment to ${OUTPUT_FILE}. You can review the contents of that file."
echo "and use it to populate the fields in the github issue template."
echo
echo "cat ${OUTPUT_FILE}"
