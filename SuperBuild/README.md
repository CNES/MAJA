# Introduction
This folder contains cmake scripts  used to configure, build and install MAJA + MAJA-COTS. 
To build MAJA (+COTS) with default configuration, you can follow below steps.

* open a terminal and move to a build directory.
* run cmake configure on `SuperBuild` source. 
* run make to configure, build and install MAJA. 

# Usage and examples

We assume some directories given below:

 * maja repository is cloned in  ``/MAJA_SHARED_FOLDER/Logiciel/MAJA``
 * maja data source directory is available at  ``/MAJA_SHARED_FOLDER/Logiciel/MAJA-Data``
 * maja validation repository is cloned in  ``/MAJA_SHARED_FOLDER/Logiciel/MAJA-Validation``
 * maja-cots's source archives are available at ``/MAJA_SHARED_FOLDER/superbuild-archives``
 * build directory of choice is ``$HOME/projects/maja/build``

```bash
cd ~/projects/maja/build
cmake /MAJA_SHARED_FOLDER/Logiciel/MAJA/SuperBuild \
-DMAJADATA_SOURCE_DIR=/MAJA_SHARED_FOLDER/MAJA-Data \
-DMAJA_TEST_OUTPUT_ROOT=/MAJA_SHARED_FOLDER/$(hostname -s) \
-DDOWNLOAD_DIR=/MAJA_SHARED_FOLDER/superbuild-archives
```

Default values are set for ``MAJADATA_SOURCE_DIR, DMAJA_TEST_OUTPUT_ROOT, DOWNLOAD_DIR``. This is to ease building for developers and users. one has to make sure these directories exists and serve their purpose. So on your host machine, download directory must be a copy of ``/MAJA_SHARED_FOLDER/superbuild-archives``. A typical configure command with default values is a simple one liner!
``
 cd ~/projects/maja/build && cmake /MAJA_SHARED_FOLDER/Logiciel/MAJA/SuperBuild
``
Default install prefix is set to ``/opt/maja/<MAJA_VERSION>``. To install to a different directory, one can set cmake's ``CMAKE_INSTALL_PREFIX`` variable.
For example ``cmake -DCMAKE_INSTALL_PREFIX=$HOME/local``

The above command will install MAJA and MAJA-COTS to ``$HOME/local/maja/<MAJA_VERSION>``. Here MAJA and its COTS are always installed onto
``<whatever install prefix>/maja/<MAJA_VERSION>``. This is non-optional and must be remembered.

During configure step, it creates directories required to configure, build and install
all of MAJA-Cots and then MAJA!. CMake configure ends with generation of makefiles that will allows users to build projects.
So you have individual targets like GDAL, ITK, OTB which can be build using ``make <TARGET_NAME>`` where name of target can be GDAL, ITK or MAJA!

Running ``make`` is sufficient enough to build and install everything!

It is important to note here that "make" will actually do a "make install".

# MAJA-COTS

 A custom cmake target called COTS is available in superbuild's binary directory.
 It allows one to build and install all of MAJA-COTS. The purpose of this special target is to have easy install all of COTS without installing MAJA. 
 Users does not need to remember names and other details of individual dependencies to MAJA. 
 One can setup a development environment with a single make command. ( ``make COTS`` ). 

# NOTES
 * Source archives for all projects except MAJA are search in ``DOWNLOAD_DIR``.
 * value of DOWNLOAD_DIR is set to:
   * cmake variable DOWNLOAD_DIR if defined through cmake argument. (see above code)
   * An Environment variable ``DOWNLOAD_DIR`` if present
   * or ``<CMAKE_CURRENT_BINARY_DIR>/downloads``  where  ``CMAKE_CURRENT_BINARY_DIR`` is your build directory
 * By default install prefix is set to ``/opt/maja/MAJA_VERSION_STRING``.
 * dependencies are handled in a clean way using cmake's dependency logic. 
 * ``make MAJA`` will build and install all projects
 * ``make GDAL`` will only build upto GDAL. i.e, no ITK, OTB, MAJA
 * All dependencies of MAJA (a.k.a MAJA-COTS) are configured exclusively for MAJA project.
 * Output from build, configure, install are save to file in build directory ``<PROJECT_NAME>/stamp/GDAL-build-err.log``.
 * Example for GDAL
   * GDAL/stamp/GDAL-configure-out.log
   * GDAL/stamp/GDAL-configure-err.log
   * GDAL/stamp/GDAL-build-out.log
   * GDAL/stamp/GDAL-build-err.log
   * GDAL/stamp/GDAL-install-out.log
   * GDAL/stamp/GDAL-install-err.log

# Testing

Tests must be executed from MAJA build directory which is different from superbuild's build directory. 
Build directory of MAJA will always be relative to superbuild's. 
In the above case where we have superbuild binary directory as ``$HOME/projects/maja/build``
MAJA build directory is ``$HOME/projects/maja/build/MAJA/build``

ctest must be launched from this directory. (assuming user is on build directory already)

```bash
cd MAJA/build
ctest -VV
```

## Listing available tests

list of tests that will be executed by ctest command can be known by running ``ctest -N``

## Listing ctest labels
Starting MAJA-3.1.0, all TV tests are labeled depending on "test type" and spectral camera (a.k.a plugin)
In order to list tests that are only related to a single spectral camera, one can use below command:

```bash
ctest -N -L SENTINEL2_CHAIN
```

To print all available labels:

```bash
ctest --print-lables
```

# Creating package

MAJA superbuild also comes with support of creating binary packages for users and developers!.
These packages of-course can be distributed easily. Installation of such packages requires zero administrative privileges
unlike traditional unix or linux packages. And the installation itself is dead simple and is basically running ".run" file!

NOTE: FOR COMPATIBILITY REASONS, PACKAGES MUST BE GENERATED WITH AN OLD VERSION OF C AND C++ RUNTIME.
THIS ALLOW US TO DISTRIBUTE PACKAGES TO DIFFERENT LINUX (X86_64) DISTROS EASILY. 
C AND C++ RUNTIME PROVIDES BACKWARD COMPATIBILITY FOR BINARIES. 
FOR MORE ON THIS ISSUE SEE MANY Q&As, BLOG ARCTILES AND DOCUMENTATIONS AVAILABLE FREELY ON INTERNET.

## Binary package (for users)

Starting from MAJA 3.1.0, there is a custom target called ``binpkg`` in your superbuild's binary directory.
Creation of package is same as running any cmake target!. This can be done by running ``make binpkg``
Once above command finished successfully, you will have ``bin_pkg/MAJA-<MAJA_VERSION>.run``

### Installation and usage

Execute ``./bin_pkg/MAJA-3.1.0.run``.

You now have maja processing executable in /opt/maja/3.1.0/bin/maja. 
To launch processing you don't need to set any extra environment variables such as ``PATH``,``LD_LIBRARY_PATH`` et al. 
The binary package is supposed to work as standalone and without having a zero conflict with any other libraries or even a different installation of MAJA. 

By default packages are installed to ``/opt/maja/<MAJA_VERSION>``. To install to a different directory you can pass ``--target`` argument. 
For example, below command will install validation package to /tmp/bin_pkg!
```bash 
cd /tmp/
./MAJA-3.1.0.run --target /tmp/maja_install
```

The above command gives maja executable in ``/tmp/maja_install/bin/maja``

## Validation package (for developers)

Starting from MAJA 3.1.0, there is a custom target called ``valpkg`` in your superbuild's binary directory.
Creation of package is same as running any cmake target!. This can be done by running ``make valpkg`` 
Once above command finished successfully, you will have ``val_pkg/MAJA-<MAJA_VERSION>-VALIDATION.run``

### Installation and usage

Execute ``MAJA-3.1.0-VALIDATION.run``

```bash 
cd /tmp/
~/projects/maja/build/val_pkg/MAJA-3.1.0-VALIDATION.run
```
You now have validation processing executables in ./MAJA-3.1.0-VALIDATION/bin/maja. To execute test or run processing you don't need to set any extra environment variables such as ``PATH``, ``LD_LIBRARY_PATH`` et al. 
Package is supposed to work as standalone and without having a zero conflict with any other libraries or even a different installation of MAJA. 
Default installation path for validation package is ./MAJA-3.1.0-VALIDATION and not ``/opt/maja/3.1.0``. 
This is to avoid accidental overwrites to existing maja installation. 
You are at liberty to install in any other directory of choice using ``--target`` argument. 
For example, below command will install validation package to /tmp/validation_pkg!

``~/projects/maja/build/val_pkg/MAJA-3.1.0-VALIDATION.run --target /tmp/validation_pkg``

Validation package comes with a copy of ``MAJA-Validation`` repository, a CMakeLists.txt and other files required to configure, build and run TV and TVA tests.
To run TVA from validation package do the following command:

```bash
mkdir $HOME/projects/valpkg #(create build directory)
cd $HOME/projects/valpkg
cmake $HOME/projects/MAJA-3.1.0-VALIDATION
```
