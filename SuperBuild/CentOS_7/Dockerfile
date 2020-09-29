FROM maja-build-env

COPY SuperBuild /opt/src/SuperBuild

COPY CMakeConfig /opt/src/CMakeConfig

RUN mkdir -p /opt/data \
 && ctest -VV -S /opt/src/SuperBuild/CI/maja_cots.cmake \
 && rm -rf /opt/build-cots /opt/data \
 && rm -rf /opt/superbuild-archives \
 && rm -rf /opt/src
