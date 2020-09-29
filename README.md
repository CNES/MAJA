# MAJA
<img  title="Ambaro Bay, Madagascar" src="https://labo.obs-mip.fr/wp-content-labo/uploads/sites/19/2017/05/20160406.png" width="300" height="300" align="right"  />
<img  title="logo MAJA" src="https://labo.obs-mip.fr/wp-content-labo/uploads/sites/19/2015/11/logo_maja.png" alt="" width="80"  /> 
<img  title="logo CNES" src="https://labo.obs-mip.fr/wp-content-labo/uploads/sites/19/2015/03/Logo-CNES-horizontal-bleu-300-e1425986175723.png" alt="" width="130"  />
<img  title="logo CESBIO" src="https://labo.obs-mip.fr/wp-content-labo/uploads/sites/19/2012/12/logo_cesbio.png" alt="" width="110"  />
<img  title="logo CSSI" src="https://www.embedded-france.org/wp-content/uploads/2018/06/cs_2008_fr-1.jpg" alt="" width="150"  /> 
<img  title="logo DLR" src="https://labo.obs-mip.fr/wp-content-labo/uploads/sites/19/2015/11/logo_DLR.jpg" alt="" width="90"  />


1. [Installing Maja](#Requirements)
2. [MAJA output format](#format)
3. [Running Maja](#run)
4. [Running Maja with Start_maja](#run_startmaja)
5. [Generation the documentation](#doc)
6. [Running the tests](#tests)
7. [FAQ](#faq)
8. [Contributors](#contrib)
9. [References](#references)



MAJA (for Maccs-Atcor Joint Algorithm), is an atmospheric correction and cloud screening software based on [the MACCS processor](https://labo.obs-mip.fr/multitemp/maccs-how-it-works/), developped for CNES by CS-SI, from a method and a prototype developped at CESBIO, <sup>[1](#ref1)</sup> <sup>[2](#ref2)</sup> <sup>[3](#ref3)</sup>.
In 2017, thanks to an agreement between CNES and DLR and to some funding from ESA, we started adding methods from DLR 's atmospheric correction software ATCOR into MACCS.
MACCS then became MAJA. 

Currently, Maja is allows the following processing steps:
* Atmospheric correction
* Cloud detection
* Estimation of the Aerosol-Optical-Depth (AOT)
* Correction of environmental- and slope-effects

Maja is based on a **multi-temporal** method - allowing to refine the outputs using the previous inputs.
Check the chapters ['Running Maja'](#run) and ['Generating the documentation'](#doc) in order to get more information about the method.

<a name="Requirements"></a>
## 1 - Installing Maja

The following instructions will set you up to get a working copy on your system.

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
This will create the file `binpkg.dir/MAJA-4-x.run` inside your build/ folder. You just have to unzip the provided package and use
the following command :
```
bash binpkg.dir/MAJA-4-x.run --target /path/to/install
```

### Setting additional environment variables for Maja

All necessary environment variables can be found inside the file `/path/to/maja/install/bin/.majaenv.sh`.

In the case where you used e.g. a different compiler for maja, you can append your own variables to it.


<a name="format"></a>
## 2 - Maja output format

Maja currently supports 3 different platforms with 2 format types (called plugins) each:

| Plugin name | Additional info |
| :--------: | --------: |
| Venus   | [Format description](https://labo.obs-mip.fr/multitemp/ven%c2%b5s-l2a-product-format/)  |
| Venus-Muscate   | [Format description](https://labo.obs-mip.fr/multitemp/format-of-ven%c2%b5s-l2a-produced-by-muscate/)  |
| Sentinel2   | [Format description](https://labo.obs-mip.fr/multitemp/sentinel-2/majas-native-sentinel-2-format/)  |
| Sentinel2-Muscate   | [Format description](https://labo.obs-mip.fr/multitemp/sentinel-2/theias-sentinel-2-l2a-product-format/)  |
| Landsat8   | [Format description](https://labo.obs-mip.fr/multitemp/landsat/product-format/)  |
| Landsat8-Muscate   | [Format description](https://labo.obs-mip.fr/multitemp/landsat/theias-landsat-8-l2a-product-format/)  |

<a name="run"></a>
## 3 - Running Maja

After compiling, you will be able to run maja in the following path: `<path/to/maja-install>/maja/4.x/bin/maja`.
Run `maja --help` to see a full list of parameters.

We recommend the use of a basic orchestrator, called Start_maja in order to process a time-series of images. Check the chapters ['Running Maja with Start_maja'](#run_startmaja)  in order to get more information about the method.


### GIPPs files

MAJA uses GIPPs files to configure the different algorithms of the chain. You can find some sets here : 

http://tully.ups-tlse.fr/olivier/gipp_maja/tree/master/


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
- repWork is a directory to store the temporary files
- repL1 is where to find the L1C data (without the site name which is added aferward optionally)
  - Les produits .SAFE doivent donc être stockés à l'emplacement suivant : repL1  = repL1/site
- repL2 is for the L2A data (without the site name which is added aferwards, optionally again)
- exeMaja is where the Maja binary code is
- repCAMS is where CAMS data are stored. You do not need to specify this directory.


To run MAJA, Start_maja copies all the necessary data in a temporary input folder. Here is an example of its content in nominal mode.
<details><summary>Click to expand folder structure. </summary>
<p>
To run MAJA, Start_maja copies all the necessary data in a temporary input folder. Here is an example of its content in nominal mode:

```
S2A_MSIL1C_20180316T103021_N0206_R108_T32TMR_20180316T123927.SAFE
S2A_TEST_GIP_CKEXTL_S_31TJF____10001_20150703_21000101.EEF
S2A_TEST_GIP_CKQLTL_S_31TJF____10005_20150703_21000101.EEF
S2A_TEST_GIP_L2ALBD_L_CONTINEN_10005_20150703_21000101.DBL.DIR
S2A_TEST_GIP_L2ALBD_L_CONTINEN_10005_20150703_21000101.HDR
S2A_TEST_GIP_L2COMM_L_ALLSITES_10008_20150703_21000101.EEF
S2A_TEST_GIP_L2DIFT_L_CONTINEN_10005_20150703_21000101.DBL.DIR
S2A_TEST_GIP_L2DIFT_L_CONTINEN_10005_20150703_21000101.HDR
S2A_TEST_GIP_L2DIRT_L_CONTINEN_10005_20150703_21000101.DBL.DIR
S2A_TEST_GIP_L2DIRT_L_CONTINEN_10005_20150703_21000101.HDR
S2A_TEST_GIP_L2SMAC_L_ALLSITES_10005_20150703_21000101.EEF
S2A_TEST_GIP_L2TOCR_L_CONTINEN_10005_20150703_21000101.DBL.DIR
S2A_TEST_GIP_L2TOCR_L_CONTINEN_10005_20150703_21000101.HDR
S2A_TEST_GIP_L2WATV_L_CONTINEN_10005_20150703_21000101.DBL.DIR
S2A_TEST_GIP_L2WATV_L_CONTINEN_10005_20150703_21000101.HDR
S2B_OPER_SSC_L2VALD_32TMR____20180308.DBL.DIR
S2B_OPER_SSC_L2VALD_32TMR____20180308.HDR
S2B_TEST_GIP_CKEXTL_S_31TJF____10001_20150703_21000101.EEF
S2B_TEST_GIP_CKQLTL_S_31TJF____10005_20150703_21000101.EEF
S2B_TEST_GIP_L2ALBD_L_CONTINEN_10003_20150703_21000101.DBL.DIR
S2B_TEST_GIP_L2ALBD_L_CONTINEN_10003_20150703_21000101.HDR
S2B_TEST_GIP_L2COMM_L_ALLSITES_10008_20150703_21000101.EEF
S2B_TEST_GIP_L2DIFT_L_CONTINEN_10002_20150703_21000101.DBL.DIR
S2B_TEST_GIP_L2DIFT_L_CONTINEN_10002_20150703_21000101.HDR
S2B_TEST_GIP_L2DIRT_L_CONTINEN_10002_20150703_21000101.DBL.DIR
S2B_TEST_GIP_L2DIRT_L_CONTINEN_10002_20150703_21000101.HDR
S2B_TEST_GIP_L2SMAC_L_ALLSITES_10005_20150703_21000101.EEF
S2B_TEST_GIP_L2TOCR_L_CONTINEN_10002_20150703_21000101.DBL.DIR
S2B_TEST_GIP_L2TOCR_L_CONTINEN_10002_20150703_21000101.HDR
S2B_TEST_GIP_L2WATV_L_CONTINEN_10005_20150703_21000101.DBL.DIR
S2B_TEST_GIP_L2WATV_L_CONTINEN_10005_20150703_21000101.HDR
S2__TEST_AUX_REFDE2_T32TMR_0001.DBL.DIR
S2__TEST_AUX_REFDE2_T32TMR_0001.HDR
S2__TEST_GIP_L2SITE_S_31TJF____10001_00000000_99999999.EEF
```

* .SAFE file is the input product
* L2VALD files are the L2A products, which is the result from a previous execution  of MAJA
* GIP are parameter files for S2A and S2B, that you will find in this repository
* REFDE2 files are the DTM files. How to obtain them is explained in `prepare_dtm`. 

A "userconf" folder is also necessary, but it is also provided in this repository.

</p>
</details>

### Retrieve Sentinel-2 L1C data.
The use of peps_download.py to download Sentinel-2 L1C products is recommended : https://github.com/olivierhagolle/peps_download

- For instance, with peps_download.py (you need to have registered at https://peps.cnes.fr and store the account and password in peps.txt file.
`python ./peps_download.py -c S2ST -l 'Avignon' -a peps.txt -d 2017-01-01 -f 2017-04-01 -w /path/to/L1C_DATA/Avignon`

- I tend to store the data per site. A given site can contain several tiles. All the L1C tiles corresponding to a site are stored in a directory named /path/to/L1C_DATA/Site

- Unzip the LIC files in /path/to/L1C_DATA/Avignon

<a name="parameters"></a>

### Parameters
The tool needs a lot of configuration files which are provided in three directories "userconf", "GIPP_S2AS2B_xxx" LUT_S2AS2B_xxx. I tend to never change the "userconf", the Look_up tables in the LUT directory depend on the satellitre, but do not change frequently with time, but the GIPP_S2AS2B, which contains the parameters, may  change often. Most of the parameters lie within the L2COMM file. When I want to test different sets of parameters, I create a new GIPP folder, which I name GIPP_xxx, where *GIPP_xxx* is passed as a parameter of the command line with option -g . 

**GIPP**

We provide two sets of parameters, one to work without CAMS data, and one to work with CAMS data. The latter needs a lot of disk space (~1.5 GB), as the LUT are provided not only for one aerosol type, but for for 5 aerosol types, and 6 water vapour contents. As Github limits the repository size to 1 GB, we are using a gitlab repository to distribute the parameters (GIPP):  
- Parameters without CAMS : http://tully.ups-tlse.fr/olivier/gipp_maja/tree/master/SENTINEL2_TM_CAMS
- Parameters with CAMS: http://tully.ups-tlse.fr/olivier/gipp_maja/tree/master/SENTINEL2_TM

You can retrieve the data with command lines using git
'''
git clone http://tully.ups-tlse.fr/olivier/gipp_maja.git
'''
This command will download all our GIPP folders, and then you will only need to move the right one to your <repCode> directory.
  
**LUT**

The look-up tables are too big to be but on our gitlab server, we provide them on zenodo DOI server : , and unzip them in a LUT_S2A_S2B_xxx folder (I know, it's a bit complicated). They contain all the LUTS, whAtever the option you choose (with or without CAMS).

- Look-up tables for Sentinel2: https://zenodo.org/record/2636694  
You can download them directly with curl utility:

```
curl -o LUT_MAJA_S2A_S2B_CAMS_H2ONew_20190410.tgz https://zenodo.org/record/2636694/files/LUT_MAJA_S2A_S2B_CAMS_H2ONew_20190411.tgz?download=1
cd <repCode>
tar  xvf LUT_MAJA_S2A_S2B_CAMS_H2ONew_20190411.tgz
```
The file is a rather big one, 1GB, so downloading it will take a while. But as you are prepared to process time series of Sentinel-2, I know you have a good network and a lot of disk space.


### Create DTM
A DTM folder is needed to process data with MAJA. Of course, it depends on the tile you want to process. This DTM must be stored in the DTM folder, which is defined within the code. 
A tool exists to create this DTM, [it is available in the "prepare_mnt" folder](StartMaja/prepare_mnt/Readme.md).


### Download CAMS data
if you intend to use the data from Copernicus Atmosphere Monitoring Service (CAMS), that we use to get an information on the aerosol type, you will need to download the CAMS data. 

CAMS data can be downloaded after a simple registration, but these days, probably due to a large success, it takes more than a day to download a day of CAMS data. Through agreements with ECMWF or through your countries weather agency, it is possible to get a priviledged access, which grants far better performances. To get a better access, it is also possible to download data month per month, instead of day per day. Donwloading a month takes almost the same time as downloading a day. But it does not work for real time processing, which needs day per day downloads.

if you want to use CAMS option, follow [cams_download tool instructions](StartMaja/cams_download)


### Execute start_maja.py

After compiling, you will be able to run startmaja in the following path: <path/to/maja-install>/maja/4.x/bin/startmaja. Run startmaja --help to see a full list of parameters.
Here is an example of command line.
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
* -z directly uses zipped L1C files

Caution, *when a product has more than 90% of clouds, the L2A is not issued*. However, a folder with _NOTVALD_ in the filename is created.

### Known Errors

Some Sentinel-2 L1C products lack the angle information which is required by MAJA. In this case, MAJA stops processing with an error message. This causes issues particularly in the backward mode. These products were acquired in February and March 2016 and have not been reprocessed by ESA (despite repeated asks from my side). You should remove them from the folder which contains the list of L1C products to process. 

We still have some difficulties with detecting the edges of the swath, which results in false detections of clouds or shadows on the edges. We hope to solve this shortly.




<a name="doc"></a>
## GIPPs files

MAJA uses GIPPs files to configure the different algorithms of the chain. You can find some sets here : 

http://tully.ups-tlse.fr/olivier/gipp_maja/tree/master/


<a name="doc"></a>
## 5 - Generation the documentation

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

TBD

<a name="references"></a>
## 9 - References :
<a name="ref1">1</a>: A multi-temporal method for cloud detection, applied to FORMOSAT-2, VENµS, LANDSAT and SENTINEL-2 images, O Hagolle, M Huc, D. Villa Pascual, G Dedieu, Remote Sensing of Environment 114 (8), 1747-1755

<a name="ref2">2</a>: Correction of aerosol effects on multi-temporal images acquired with constant viewing angles: Application to Formosat-2 images, O Hagolle, G Dedieu, B Mougenot, V Debaecker, B Duchemin, A Meygret, Remote Sensing of Environment 112 (4), 1689-1701

<a name="ref3">3</a>: A Multi-Temporal and Multi-Spectral Method to Estimate Aerosol Optical Thickness over Land, for the Atmospheric Correction of FormoSat-2, LandSat, VENμS and Sentinel-2 Images, O Hagolle, M Huc, D Villa Pascual, G Dedieu, Remote Sensing 7 (3), 2668-2691

<a name="ref4">4</a>: MAJA's ATBD, O Hagolle, M. Huc, C. Desjardins; S. Auer; R. Richter, https://doi.org/10.5281/zenodo.1209633

