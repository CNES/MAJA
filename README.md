# MAJA
<img  title="Ambaro Bay, Madagascar" src="https://labo.obs-mip.fr/wp-content-labo/uploads/sites/19/2017/05/20160406.png" width="300" height="300" align="right"  /> <img  title="logo MAJA" src="https://labo.obs-mip.fr/wp-content-labo/uploads/sites/19/2015/11/logo_maja.png" alt="" width="80"  />  <img  title="logo CNES" src="https://labo.obs-mip.fr/wp-content-labo/uploads/sites/19/2015/03/Logo-CNES-horizontal-bleu-300-e1425986175723.png" alt="" width="130"  /> <img  title="logo CESBIO" src="https://labo.obs-mip.fr/wp-content-labo/uploads/sites/19/2012/12/logo_cesbio.png" alt="" width="110"  /> <img  title="logo CSSI" src="https://www.embedded-france.org/wp-content/uploads/2018/06/cs_2008_fr-1.jpg" alt="" width="150"  />  <img  title="logo DLR" src="https://labo.obs-mip.fr/wp-content-labo/uploads/sites/19/2015/11/logo_DLR.jpg" alt="" width="90"  />


1. [Installing Maja](#Requirements)
2. [MAJA output format](#format)
3. [Running Maja as standalone](#run)
4. [Running Maja with Start_maja](#run_startmaja)
5. [Generate the documentation](#doc)
6. [Running the tests](#tests)
7. [FAQ](#faq)
8. [Contributors](#contrib)
9. [References](#references)



MAJA (for Maccs-Atcor Joint Algorithm), is an atmospheric correction and cloud screening software based on [the MACCS processor](https://labo.obs-mip.fr/multitemp/maccs-how-it-works/), developped for CNES by CS-SI, from a method and a prototype developped at CESBIO, <sup>[1](#ref1)</sup> <sup>[2](#ref2)</sup> <sup>[3](#ref3)</sup>.
In 2017, thanks to an agreement between CNES and DLR and to some funding from ESA, we started adding methods from DLR 's atmospheric correction software ATCOR into MACCS.
MACCS then became MAJA. 

Currently, Maja allows the following processing steps:
* Atmospheric correction
* Cloud detection
* Estimation of the Aerosol-Optical-Depth (AOT)
* Correction of environmental- and slope-effects

Maja is based on a **multi-temporal** method - allowing to refine the outputs using the previous inputs.
Check the chapters ['Running Maja'](#run) and ['Generating the documentation'](#doc) in order to get more information about this.

<a name="Requirements"></a>
## 1 - Installing Maja

The following instructions will set you up to get a working copy on your system.

### Precompiled binaries

Maja is available as a self-extracting archive via the following link:

[**Download Maja**](https://logiciels.cnes.fr/en/content/maja)

You just have to unzip the provided package and use the following command :
```
./MAJA-4-x.run --target /path/to/install
```

### Requirements

In order to compile Maja, you will need the following prerequisites:
* cmake >= 3.4
* MAJA-SuperbuildArchive_V4.x.tar.gz --> Contains all dependencies of Maja
* GCC with support of C++ 14
* Maja-4-x archive --> Contains the code of Maja itself

### Compiling from source

In order to compile Maja from source, run the following commands:
```  
cd Maja-4-x-archive
mkdir build
mkdir install
cd build
cmake ../SuperBuild \
	-DDOWNLOAD_DIR=<Path/to>/Superbuild-archives \
	-DENABLE_TU=OFF \
	-DENABLE_TV=OFF \
	-DENABLE_TVA=OFF \
	-DCMAKE_INSTALL_PREFIX=`pwd`/../install
make
```	

Follow the [next chapter](#run) in order to see how to run Maja.

### Creating a binary package

If you wish to install maja on another system without re-compiling, you can generate a binary-package after compiling using the following command:
```
cd Maja-4-x-archive/build
make binpkg
```
This will create the file `binpkg.dir/MAJA-4-x.run` inside your build folder.

### Setting additional environment variables for Maja

All necessary environment variables can be found inside the file `/path/to/maja/install/bin/.majaenv.sh`.

In the case where you used e.g. a different compiler for maja, you have to append your own variables to it.


<a name="format"></a>
## 2 - Maja output format

Maja currently supports 3 different platforms, each with 2 format types (called plugins):

| Plugin name | Additional info |
| :--------: | --------: |
| Venus   | [Format description](https://labo.obs-mip.fr/multitemp/ven%c2%b5s-l2a-product-format/)  |
| Venus-Muscate   | [Format description](https://labo.obs-mip.fr/multitemp/format-of-ven%c2%b5s-l2a-produced-by-muscate/)  |
| Sentinel2   | [Format description](https://labo.obs-mip.fr/multitemp/sentinel-2/majas-native-sentinel-2-format/)  |
| Sentinel2-Muscate   | [Format description](https://labo.obs-mip.fr/multitemp/sentinel-2/theias-sentinel-2-l2a-product-format/)  |
| Landsat8   | [Format description](https://labo.obs-mip.fr/multitemp/landsat/product-format/)  |
| Landsat8-Muscate   | [Format description](https://labo.obs-mip.fr/multitemp/landsat/theias-landsat-8-l2a-product-format/)  |

<a name="run"></a>
## 3 - Running Maja as standalone

After compiling, you will be able to run maja in the following path: `<path/to/maja-install>/maja/4.x/bin/maja`.
Run `maja --help` to see a full list of parameters.

We recommend the use of our basic orchestrator, called Start_maja in order to process a time-series of images. Check the chapters ['Running Maja with Start_maja'](#run_startmaja)  in order to get more information.

<a name="run_startmaja"></a>
## 4 - Running Maja with Start_maja

The basic supervisor **start_maja** enables to process successively all files in a time series of Sentinel-2 images for a given tile, stored in a folder. The initialisation of the time series is performed with the "backward mode", and then all the dates are processed in "nominal" mode. The backward mode takes much more time than the nominal mode. On my computer, which is a fast one, the nominal mode takes 15 minutes, and the backward mode takes almost one hour. No control is done on the outputs, and it does not check if the time elapsed between two successive products used as input is not too long and would require restarting the initialisation in backward mode.

The description below will explain how to process a set of data above tile 31TFJ, near Avignon in Provence, France. To process any other tile, you will need to prepare the DTM and store the data in the DTM folder.

### Prepare folders and input files

- To use the start_maja script, you need to configure the directories, within the folder.txt file.
Here is an example of configuration that must be adapted to your own directory structure.
```
[Maja_Inputs]
repWork=./work
repGipp=./gipp
repMNT=./dtm
repL1  =/path/to/L1C
repL2  =/path/to/L2A
exeMaja=/path/to/bin/maja
repCAMS=/path/to/CAMS

[DTM_Creation]
repRAW=./dtm/raw 
repGSW=./dtm/gsw 
```
* repWork is a directory to store the temporary files
* repGipp is the folder where Start_maja automatically downloads the GIPP-set for each plugin.
* repMNT stores the DTM (MNT in french) in Maja format
* repL1 is where to find the L1C data (without the site name which is added aferward optionally)
* repL2 is for the L2A data (without the site name which is added aferwards, optionally again)
* exeMaja is where the Maja binary exe is located
* repCAMS is where CAMS data is stored. You do not need to specify this directory if you decide to not process with CAMS option.
* repRAW stores the raw DTM archives (such as the ones for SRTM, which have the name `srtm_37_04.zip`)
* repGSW stores the raw Water-Mask files (such as the one for GSW, which have the name `occurrence_0E_50N*.tif`)

To run MAJA, Start_maja copies all the necessary data in a temporary input folder. Here is an example of its content in nominal mode.
<details><summary>Click to expand folder structure. </summary>
<p>

```
S2A_MSIL1C_20200313T095031_N0209_R079_T33UYQ_20200313T102505.SAFE
S2A_TEST_GIP_CKEXTL_S_ALLSITES_00001_20190626_21000101.EEF
S2A_TEST_GIP_CKEXTL_S_ALLSITES_10001_20190626_21000101.EEF
S2A_TEST_GIP_CKQLTL_S_ALLSITES_00001_20190626_21000101.EEF
S2A_TEST_GIP_CKQLTL_S_ALLSITES_10001_20190626_21000101.EEF
S2A_TEST_GIP_L2ALBD_L_CONTINEN_00001_20190626_21000101.DBL.DIR
S2A_TEST_GIP_L2ALBD_L_CONTINEN_00001_20190626_21000101.HDR
S2A_TEST_GIP_L2COMM_L_ALLSITES_00001_20190626_21000101.EEF
S2A_TEST_GIP_L2COMM_L_ALLSITES_10001_20190626_21000101.EEF
S2A_TEST_GIP_L2DIFT_L_CONTINEN_00001_20190626_21000101.DBL.DIR
S2A_TEST_GIP_L2DIFT_L_CONTINEN_00001_20190626_21000101.HDR
S2A_TEST_GIP_L2DIRT_L_CONTINEN_00001_20190626_21000101.DBL.DIR
S2A_TEST_GIP_L2DIRT_L_CONTINEN_00001_20190626_21000101.HDR
S2A_TEST_GIP_L2SMAC_L_ALLSITES_00001_20190626_21000101.EEF
S2A_TEST_GIP_L2TOCR_L_CONTINEN_00001_20190626_21000101.DBL.DIR
S2A_TEST_GIP_L2TOCR_L_CONTINEN_00001_20190626_21000101.HDR
S2A_TEST_GIP_L2WATV_L_CONTINEN_00001_20190626_21000101.DBL.DIR
S2A_TEST_GIP_L2WATV_L_CONTINEN_00001_20190626_21000101.HDR
S2B_TEST_GIP_CKEXTL_S_ALLSITES_00001_20190626_21000101.EEF
S2B_TEST_GIP_CKEXTL_S_ALLSITES_10001_20190626_21000101.EEF
S2B_TEST_GIP_CKQLTL_S_ALLSITES_00001_20190626_21000101.EEF
S2B_TEST_GIP_CKQLTL_S_ALLSITES_10001_20190626_21000101.EEF
S2B_TEST_GIP_L2ALBD_L_CONTINEN_00001_20190626_21000101.DBL.DIR
S2B_TEST_GIP_L2ALBD_L_CONTINEN_00001_20190626_21000101.HDR
S2B_TEST_GIP_L2COMM_L_ALLSITES_00001_20190626_21000101.EEF
S2B_TEST_GIP_L2COMM_L_ALLSITES_10001_20190626_21000101.EEF
S2B_TEST_GIP_L2DIFT_L_CONTINEN_00001_20190626_21000101.DBL.DIR
S2B_TEST_GIP_L2DIFT_L_CONTINEN_00001_20190626_21000101.HDR
S2B_TEST_GIP_L2DIRT_L_CONTINEN_00001_20190626_21000101.DBL.DIR
S2B_TEST_GIP_L2DIRT_L_CONTINEN_00001_20190626_21000101.HDR
S2B_TEST_GIP_L2SMAC_L_ALLSITES_00001_20190626_21000101.EEF
S2B_TEST_GIP_L2TOCR_L_CONTINEN_00001_20190626_21000101.DBL.DIR
S2B_TEST_GIP_L2TOCR_L_CONTINEN_00001_20190626_21000101.HDR
S2B_TEST_GIP_L2WATV_L_CONTINEN_00001_20190626_21000101.DBL.DIR
S2B_TEST_GIP_L2WATV_L_CONTINEN_00001_20190626_21000101.HDR
S2__TEST_AUX_REFDE2_33UYQ_0001.DBL.DIR
S2__TEST_AUX_REFDE2_33UYQ_0001.HDR
S2__TEST_GIP_L2SITE_S_ALLSITES_00001_20190626_21000101.EEF
SENTINEL2B_20200308-095659-128_L2A_T33UYQ_C_V1-0
```

* .SAFE file is the input L1C product
* `SENTINEL2_*_L2A_*` files are the L2A products, which is the result from a previous run of MAJA
Then for each of the following items you will have a `DIR` or `DBL` folder/archive and a `HDR` file:
* GIP are parameter files for each satellite (S2A or S2B in this example)
* REFDE2 files are the DTM (Digital Terrain Model) files. How to obtain them is explained in `prepare_dtm`. 

A "userconf" folder is also necessary, but it is already provided by Start_Maja.
</p>
</details>

### Retrieve Sentinel-2 L1C data.
The use of peps_download.py to download Sentinel-2 L1C products is recommended : https://github.com/olivierhagolle/peps_download

- For instance, with peps_download.py (you need to have registered at https://peps.cnes.fr and store the account and password in peps.txt file.
`python ./peps_download.py -c S2ST -l 'Avignon' -a peps.txt -d 2017-01-01 -f 2017-04-01 -w /path/to/L1C_DATA/Avignon`

- Some users tend to store the data per site. A given site can contain several S2-tiles. All the L1C tiles corresponding to a site are stored in a directory named /path/to/L1C_DATA/Site


<a name="parameters"></a>
### Create DTM
A DTM folder is needed to process data with MAJA which needs to have the same geographical extent as the L1C input product - It depends on the tile you want to process. A tool exists to create this DTM, [it is available in the "prepare_mnt" folder](StartMaja/prepare_mnt/Readme.md).
When using Start_maja the creation of the folder in Maja-format is automatically attempted. For this the `repRAW` and `repGSW` directories in your `folders.txt` file need to be set.


### Download CAMS data
if you intend to use the data from Copernicus Atmosphere Monitoring Service (CAMS), that we use to get an information on the aerosol type, you will need to download the CAMS data. 

CAMS data can be downloaded after a simple registration, but these days, probably due to a large success, it takes more than a day to download a day of CAMS data. Through agreements with ECMWF or through your countries weather agency, it is possible to get a priviledged access, which grants far better performances. To get a better access, it is also possible to download data month per month, instead of day per day. Donwloading a month takes almost the same time as downloading a day. But it does not work for real time processing, which needs day per day downloads.

if you want to use CAMS option, follow [cams_download tool instructions](StartMaja/cams_download)

### GIPP Files

MAJA uses GIPP-files (Ground Image Processing Parameters) to configure the different algorithms of the chain without having to recompile the code. You can find all sets here : 

http://osr-cesbio.ups-tlse.fr/gitlab_cesbio/kettigp/maja-gipp

Start_Maja automatically downloads the GIPPs necessary for each plugin and links them to the folder.
You only need to modify the parameters if you want to change the behavior of the processing chain.

### userconf

On top of the GIPP files, a global user configuration folder is used, named `userconf`. Should you want to debug the processing chain,
we recommend to check the parameters that are listed in the `MAJAUserConfigSystem.xml` or for each plugin `MAJAUserConfig_*xml`.

### Execute start_maja.py

After installing Maja, you will be able to run startmaja in the following path: <path/to/maja-install>/maja/4.x/bin/startmaja.
Run `startmaja --help` to see a full list of parameters.
Here is an example of a command line.
```
Usage   : <path/to/maja-install>/maja/4.x/bin/startmaja -f <folder_file> -t <tile name> -s <Site Name> -d <start date>
Example : <path/to/maja-install>/maja/4.x/bin/startmaja -f folders.txt -t 31TFJ -s Avignon -d 20170101 -e 20180101
```
Description of command line options :
* -f provides the folders filename
* -t is the tile number
* -s is the site name
* -d (aaaammdd) is the first date to process within the time series
* -e (aaaammdd) is the last date to process within the time serie-s

Caution, *when a product has more than 90% of clouds, the L2A is not issued* - This results in the output folders to contain only a metadata (MTD*xml) file but no rasters/images.

### Known Errors

Some Sentinel-2 L1C products lack the angle information which is required by MAJA. In this case, MAJA stops processing with an error message. This causes issues particularly in the backward mode. These products were acquired in February and March 2016 and have not been reprocessed by ESA (despite repeated requests). You should remove them from the folder which contains the list of L1C products to process. 

<a name="doc"></a>
## 5 - Generate the documentation

The documentation can be compiled by setting the cmake flag `BUILD_DOCS=ON`.
It is deactivated by default.
To run the document compilation, simply execute `make` after activating the option.
The following requirements are needed in order to compile the pdf:

* sphinx-build
* sphinx-rtd-theme
* latex

<a name="tests"></a>
## 6 - Running the Tests

Maja needs two auxiliary archives to run the tests, called "MAJA-DATA" and "MAJA-Validation".
To run the compilation with tests, please execute the following commands:
```
cd Maja-4-x-archive
mkdir testing build install
cd build
cmake ../SuperBuild \
	-DMAJADATA_SOURCE_DIR=</path/to>/MAJA-DATA \
	-DMAJA_TEST_OUTPUT_ROOT=`pwd`/../testing \
	-DDOWNLOAD_DIR=</path/to>/Superbuild-archives \
	-DENABLE_TV=ON \
	-DENABLE_TVA=ON \
	-DCMAKE_INSTALL_PREFIX=`pwd`/../install
make –j<NbThreads>
make install
```

Afterwards, run the tests using the following command:
```
ctest -N    # List all available tests
make test   # Runs the tests
```

<a name="faq"></a>
## 7 - FAQ

If you have issues or questions with MAJA, please raise an issue on this repository.
It will serve as a forum.

<a name="contrib"></a>
## 8 - Contributors

* **Centre National d'Etudes Spatiales (CNES)** 
* **Centre d'Etudes Spatiales de la Biosphère (CESBIO)**
* **CS-SI France**

<a name="references"></a>
## 9 - References :
<a name="ref1">1</a>: A multi-temporal method for cloud detection, applied to FORMOSAT-2, VENµS, LANDSAT and SENTINEL-2 images, O Hagolle, M Huc, D. Villa Pascual, G Dedieu, Remote Sensing of Environment 114 (8), 1747-1755

<a name="ref2">2</a>: Correction of aerosol effects on multi-temporal images acquired with constant viewing angles: Application to Formosat-2 images, O Hagolle, G Dedieu, B Mougenot, V Debaecker, B Duchemin, A Meygret, Remote Sensing of Environment 112 (4), 1689-1701

<a name="ref3">3</a>: A Multi-Temporal and Multi-Spectral Method to Estimate Aerosol Optical Thickness over Land, for the Atmospheric Correction of FormoSat-2, LandSat, VENμS and Sentinel-2 Images, O Hagolle, M Huc, D Villa Pascual, G Dedieu, Remote Sensing 7 (3), 2668-2691

<a name="ref4">4</a>: MAJA's ATBD, O Hagolle, M. Huc, C. Desjardins; S. Auer; R. Richter, https://doi.org/10.5281/zenodo.1209633

