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
#!/bin/bash

# select platform
if [ "$1" == "xenial" ]; then
  plat="Ubuntu 16.04"
  env_name="maja-env-ubuntu:16.04"
  cots_name="maja-cots-ubuntu"
elif [ "$1" == "centos6" ]; then
  plat="CentOS 6"
  env_name="maja-env-centos:6"
  cots_name="maja-cots-centos6"
elif [ "$1" == "centos7" ]; then
  plat="CentOS 7"
  env_name="maja-env-centos:7"
  cots_name="maja-cots-centos7"
else
  echo Usage: $0  xenial|centos6|centos7
  exit 1
fi

# common docker parameters
dr_proxy="-e http_proxy -e https_proxy -e ftp_proxy -e no_proxy"
dr_opts="-v ${WORKSPACE}:/opt/src ${dr_proxy}"
buildEnvRegistry="registry.gitlab.cnes.fr/maja/maja-build-env"
majaRegistry="registry.gitlab.cnes.fr/maja/maja"
COTS_SHA1="$(git log -1 --pretty=format:%H -- SuperBuild)"

echo "COTS sha1: ${COTS_SHA1}"

env_image="${buildEnvRegistry}/${env_name}"
cots_image="${majaRegistry}/${cots_name}:${COTS_SHA1}"

mkdir -p ${WORKSPACE}/out/$1/logs
chmod 777 ${WORKSPACE}/out/$1/logs
echo "${COTS_SHA1}" > ${WORKSPACE}/out/$1/logs/cots_sha1.log

docker pull ${env_image} || exit 1

BUILD_COTS=0
docker pull ${cots_image} || export BUILD_COTS=1
if [ $BUILD_COTS -eq 0 ]; then
  echo "Container ${cots_name}:${COTS_SHA1} already in registry"
  base_layers="$(docker inspect --format='{{range .RootFS.Layers}}/{{.}}{{end}}' ${env_image})"
  cots_layers="$(docker inspect --format='{{range .RootFS.Layers}}/{{.}}{{end}}' ${cots_image})"
  if [ "$base_layers" != "$(dirname $cots_layers)" ]; then
    echo "Layers don't match, rebuild the COTS image"
    BUILD_COTS=1
  fi
fi
RET_COTS=0
if [ $BUILD_COTS -eq 1 ]; then
  echo "Building COTS image ..."
  docker run --name c_maja_cots ${dr_opts} ${env_image} "-c" "ctest -VV -S /opt/src/SuperBuild/CI/maja_cots.cmake > /opt/logs/cots.log 2>&1 && rm -rf /opt/build-cots /opt/data /opt/superbuild-archives" \
    && docker commit -m="Maja COTS on ${plat}" -a "Jenkins CI" c_maja_cots ${cots_image} \
    && docker push ${cots_image}
  RET_COTS=$?
  docker cp c_maja_cots:/opt/logs ${WORKSPACE}/out/$1
  docker container rm c_maja_cots
fi

exit $RET_COTS
