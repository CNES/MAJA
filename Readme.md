<img  title="logo CNES" src="https://github.com/petket-5/Start_maja_int/blob/181205_start_maja_reprog/images/start_maja_cnes.png" alt="" width="80"  /> <img  title="logo CESBIO" src="https://github.com/petket-5/Start_maja_int/blob/181205_start_maja_reprog/images/start_maja_cesbio.png" alt="" width="80"  /> <img  title="logo DLR" src="https://github.com/petket-5/Start_maja_int/blob/181205_start_maja_reprog/images/start_maja_dlr.png" alt="" width="80"  /> <img  title="logo MAJA" src="https://github.com/petket-5/Start_maja_int/blob/181205_start_maja_reprog/images/start_maja_maja.png" alt="" width="80"  /> 

# Content

[![Build Status](https://travis-ci.com/petket-5/Start_maja_int.svg?branch=181205_start_maja_reprog)](https://travis-ci.com/petket-5/Start_maja_int)
[![Coverage Status](https://coveralls.io/repos/github/petket-5/Start_maja_int/badge.svg)](https://coveralls.io/github/petket-5/Start_maja_int)

1. [Introduction](#intro)
2. [System](#System)
3. [Change log](#ChangeLog)
4. [MAJA output format](#format)
5. [Get and Install MAJA](#maja)
6. [start_maja user_manual](#User_manual)
8. [Questions](#Questions)

<a href="http://www.cesbio.ups-tlse.fr/multitemp/wp-content/uploads/2017/05/20160406.png"><img  title="Ambaro Bay, Madagascar" src="http://www.cesbio.ups-tlse.fr/multitemp/wp-content/uploads/2017/05/20160406-300x300.png" alt="" width="300" height="300" align="middle"  /></a>

<a name="intro"></a>
# Introduction

** The issues with CAMS option have been solved with MAJA 3.3. Activating the CAMS option is recommended over regions with occasionnal or permanent dust aerosols or pollution** 

The following script will help you run the MAJA L2A processor on your computer, for Sentinel-2 data only so far. You can also run MAJA on [CNES PEPS collaborative ground segment](https://theia.cnes.fr) using the [maja-peps script also available on github](https://github.com/olivierhagolle/maja_peps). Using PEPS will be much easier, but is not meant for mass processing.

MAJA stands for Maccs-Atcor Joint Algorithm. This atmospheric correction and cloud screening software is based [on MACCS processor](http://www.cesbio.ups-tlse.fr/multitemp/?p=6203), developped for CNES by CS-SI company, from a method and a prototype developped at CESBIO, <sup>[1](#ref1)</sup> <sup>[2](#ref2)</sup> <sup>[3](#ref3)</sup>. In 2017, thanks to an agreement between CNES and DLR and to some funding from ESA, we started adding methods from DLR 's atmospheric correction software ATCOR into MACCS. MACCS then became MAJA. 

- The first version resulting from this collaboration was MAJA V1-0. If you are using this version, you will also need [the version v1_0 of start_maja](https://github.com/olivierhagolle/Start_maja/releases/tag/v1.0).

- A second version of MAJA, v2-1 was used in Theia, but was not distributed to users, because the version 3 was available shortly afterwards. 

- This version of start_maja.py is made to run MAJA 3.2 or above. 

MAJA has a very unique feature among all atmospheric correction processors: it uses multi-temporal criteria to improve cloud detection and aerosol retrieval. Because of this feature, it is important to use MAJA to process *time series* of images and not single images. Moreover, these images have to be processed chronologically. To initialise processing of a time series, a special mode is used, named "backward mode". To get a correct first product, we process in fact a small number of products in anti-chronological order (default value of number of images processed in backward mode is 8, but consider increasing it if your region is very cloudy). Then all the products are processed in "nominal" mode and chronological order. When a product is fully or nearly fully cloudy, it is not issued to save processing time and disk space.

For more information about MAJA methods but without details, please read : http://www.cesbio.ups-tlse.fr/multitemp/?p=6203
To get all details on the methods, MAJA's ATBD is available here : http://tully.ups-tlse.fr/olivier/maja_atbd/blob/master/atbd_maja.pdf, or reference <sup>[1](#ref4)</sup>, below.


MAJA needs parameters, that ESA names GIPP. We have also set-up [an internal repository](http://tully.ups-tlse.fr/olivier/gipp_maja/tree/master) containing parameters for all sensors actually processed by MAJA, including Sentinel-2, Venµs and LANDSAT 8. This repository is kept up to date with the operational processors. See also [the parameters section](#parameters) below.


<a name="System"></a>
# System
MAJA works on Linux platforms. We have tested it for **Linux RedHat 6+, CentOS 6+, Ubuntu 12+**. It requires at least 8GB of memory per instance of MAJA running in parallel. It also requires disk space (1GB per input L1C, 2GB per outpult L2A), and can use several threads in parallel. This is set in the userconf files, and the default value is 8 threads. Above 8, the improvement of performances is not linear with the number of threads.

On our two years old computer, 8GB and 8 threads, it takes 22 minutes to make a L2A product, except for initialisation in "backward mode" (the first product in a time series, which takes about 1 hour).



<a name="ChangeLog"></a>
# Change Log

## V3.3 (2019/05/07)
Start-Maja did not change, but MAJA changed a lot. A summary of the changes is (provided here)[http://www.cesbio.ups-tlse.fr/multitemp/?p=15666]. **We really encourage you** to use this version as it improves a lot the performances, regarding both atmospheric correction and cloud detection.


## V3.2.1 (2019/04/10)
We changed a little the command line interface to store the GIPP parameters, and the LUTS in different folders. GIPP parameters evolve faster than LUTS, so it is better to separate them. I replaced the `-c context` option by two other options, `-g GIPP_folder -l LUT_folder`

## V3.2 (2019/02/01)
We moved start-maja to a new repository, pertaining to CNES and not to Olivier Hagolle's personal github. It is also an opportunity to clean the repository, as the initial one had binary parameters in it, had grown a lot and took a long time to download. So we started from scratch.

The older repository is still accessible : from https://github.com/olivierhagolle/Start_maja, but will not be updated anymore.

Several improvements were brought :
- in the command line interface
- to adapt it to CNES HPC context (optional of course)
- to account from MAJA V3.2 and work with CAMS data.
- to simplify DTM preparation (thanks to Peter Kettig contribution)
- we removed this stupid (OH's) idea to remove the "GIPP_" characters to form the context name

MAJA V3.2 brings a couple of improvements compared to V3.1:
- MAJA 3.2 adapts to a bug from Sentinel-2 L1C products, which sometimes (but quite frequently) provide the detector footprints in an incorrect order since October 2018.
- The CAMS data can also be used as a default value for AOT estimates. The default CAMS AOT is used with a low weight in the cost function. If MAJA does not find many suitable pixels to estimate the AOT, the CAMS value will have an influence, but in general, a large number of measurements are available in an image, and in that case, CAMS has no influence (except on the aerosol type, see below, V3.1). Finally, this improvement will be usefull over snow covered landscapes, or bright deserts, of for images almost fully covered by clouds.

## V3.1 (2018/07/09)
<details><summary>Older versions (click to unfold)</summary>
<p>

Until MAJA V3.1 there were two output formats, one for the products generated at Theia, and one for the products generated by MAJA used with standard ESA L1C products. In the future,  we will adopt the output format of Theia. However, for  this version, we provide a choise of two outputs. To choose which output format is used by MAJA, you will need to choose between two binary versions:
- the MAJA version with "Sentinel2-TM" plugin will provide the Theia format as output. [This format is described here](http://www.cesbio.ups-tlse.fr/multitemp/?page_id=8352). 

- the other version will go on with the current format, [described here](http://www.cesbio.ups-tlse.fr/multitemp/?page_id=10464)

MAJA 3.1 ships several improvements :

- the main improvement is the use of Copernicus Atmosphere Monitoring Service (CAMS) aerosol products, which are used to constrain the aerosol type in the estimates. This brings a major improvement in places where the aerosols can differ a lot from a continental model which was used so far,it might slightly degraded the reults where the aerosol model was the correct one. However, a bug on the time and mlocation interpolation of CAMS data was found, and we recommend to activate the CAMS option only when it is fixed with MAJA 3.1.2. 

- since version V2-1, MAJA also includes a correction for thin cirrus clouds and a directional effect correction used to improve the estimate of AOT when using Sentinel-2 time series coming from adjacent orbits. More information is available here: http://www.cesbio.ups-tlse.fr/multitemp/?p=13291

- depending on the executable downloaded, you can have access to the same output format as the one used by MUSCATE processing center. 

- and finally, MAJA is now provided for RedHat or Ubuntu Linux families. 

### V1.0 (2018/07/09)
We just added a tag, v1.0 to get a similar version number as the one used for MAJA. The corresponding release [can be accessed here](https://github.com/olivierhagolle/Start_maja/releases/tag/v1.0)

### v.0.9.1 (2018/03/29)
Added MAJA error catching. As a result, the processing of a whole time series stops if MAJA fails for a given date.

### v0.9 (2017/10/02)
- this version of start_maja works with both S2A and S2B
- we have found errors, especially regarding water vapour, in the parameters we provided in the "GIPP_nominal" folder. These parameters have been removed and we strongly advise you to do the same.
- we have updated the parameters and provided them for both S2A and S2B in the folder GIPP_S2AS2B

</p>
</details>

<a name="format"></a>
# Data format 
We provide two versions of MAJA's binary code depending on the format you wish to use :
- the MAJA version with "Sentinel2-TM" plugin uses the Theia format as output. [This format is described here](http://www.cesbio.ups-tlse.fr/multitemp/?page_id=8352). 

- the other version still uses the native format, [described here](http://www.cesbio.ups-tlse.fr/multitemp/?page_id=10464). We might decide to stop support for this format in the coming versions.

<a name="maja"></a>
# Get MAJA
## Get MAJA Sofware

MAJA is provided as a binary code and should at least work on RedHat (6 and 7), Cent 0S, or Ubuntu recent versions. Its licence prevents commercial use of the code. For a licence allowing commercial use, please contact CNES (Olivier Hagolle). MAJA's distribution site is https://logiciels.cnes.fr/en/content/maja. 

MAJA is provided under two versions depending on the format you would like to use. 

If you wish to use MUSCATE format, which is documented [here](http://www.cesbio.ups-tlse.fr/multitemp/?page_id=8352), you will have to download the TM binary.

If you wish to use the native format, which is documented [here](http://www.cesbio.ups-tlse.fr/multitemp/?page_id=10464), as for MAJA 1_0, you will have to download the "NoTM" version. Anyway, be aware that we will probably not maintain that version in the coming years.

## install MAJA
Installation of MAJA is straightforward on linux systems. You just have to unzip the provided package and use 
the following command :

`>>> bash MAJA-3.2.2_TM.run --target /path/to/install`

To use this start_maja.py, you will need to configure the directories within the folder.txt file.



<a name="User_manual"></a>
# Start_maja User manual 
The basic supervisor **start_maja** enables to process successively all files in a time series of Sentinel-2 images for a given tile, stored in a folder. The initialisation of the time series is performed with the "backward mode", and then all the dates are processed in "nominal" mode. The backward mode takes much more time than the nominal mode. On my computer, which is a fast one, the nominal mode takes 15 minutes, and the backward mode takes almost one hour. No control is done on the outputs, and it does not check if the time elapsed between two successive products used as input is not too long and would require restarting the initialisation in backward mode.

The description below will explain how to process a set of data above tile 31TFJ, near Avignon in Provence, France. To process any other tile, you will need to prepare the DTM and store the data in the DTM folder.

## Install

- [Install MAJA](https://logiciels.cnes.fr/en/content/maja)

- Clone the current repository to get start_maja.py
`git clone https://github.com/CNES/Start-MAJA.git`

## Prepare folders and input files

- To use the start_maja script, you need to configure the directories, within the folder.txt file.
Here is my own configuration, also provided in the folders.txt file in this repository.
```
[PATH]
repWork=maja-work
repGipp=maja-gipp
repMnt=/work/Maja/DTM
repL1  =/datalake/S2-L1C
repL2  =/mnt/data/SENTINEL2/L2A_MAJA
exeMaja=/opt/Maja_3.3.1/bin/maja
repCAMS=/mnt/data/CAMS
```
- repWork is a directory to store the temporary files
- repL1 is where to find the L1C data (without the site name which is added aferward optionally)
  - Les produits .SAFE doivent donc être stockés à l'emplacement suivant : repL1  = repL1/site
- repL2 is for the L2A data (without the site name which is added aferwards, optionally again)
- exeMaja is where the Maja binary code is
- repCAMS is where CAMS data are stored. You do not need to specify this directory.

```
#files downloaded grom github
Readme.md       # This readme
start_maja.py   # Orchestrator 
cams_download/  # Utilities to downlaod cams data
Common/         # Common libraries
prepare_dtm/    # Modules to prepare DTM files
userconf/        # Folder which contains configuration files for MAJA
```

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

## Retrieve Sentinel-2 L1C data.
The use of peps_download.py to download Sentinel-2 L1C products is recommended : https://github.com/olivierhagolle/peps_download

- For instance, with peps_download.py (you need to have registered at https://peps.cnes.fr and store the account and password in peps.txt file.
`python ./peps_download.py -c S2ST -l 'Avignon' -a peps.txt -d 2017-01-01 -f 2017-04-01 -w /path/to/L1C_DATA/Avignon`

- I tend to store the data per site. A given site can contain several tiles. All the L1C tiles corresponding to a site are stored in a directory named /path/to/L1C_DATA/Site

- Unzip the LIC files in /path/to/L1C_DATA/Avignon

<a name="parameters"></a>

## Parameters
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


## Create DTM
A DTM folder is needed to process data with MAJA. Of course, it depends on the tile you want to process. This DTM must be stored in the DTM folder, which is defined within the code. A tool exists to create this DTM, [it is available in the "prepare_dtm" folder](https://github.com/CNES/Start-MAJA/blob/master/prepare_dtm/Readme.md).


## Download CAMS data
if you intend to use the data from Copernicus Atmosphere Monitoring Service (CAMS), that we use to get an information on the aerosol type, you will need to download the CAMS data. 

CAMS data can be downloaded after a simple registration, but these days, probably due to a large success, it takes more than a day to download a day of CAMS data. Through agreements with ECMWF or through your countries weather agency, it is possible to get a priviledged access, which grants far better performances. To get a better access, it is also possible to download data month per month, instead of day per day. Donwloading a month takes almost the same time as downloading a day. But it does not work for real time processing, which needs day per day downloads.

if you want to use CAMS option, follow cams_download tool instructions : https://github.com/CNES/Start-MAJA/tree/master/cams_download

## Execute start_maja.py

Here is an example of command line
```
Usage   : python ./start_maja.py -f <folder_file> -t <tile name> -s <Site Name> -d <start date>
Example : python ./start_maja.py -f folders.txt -t 31TFJ -s Avignon -d 20170101 -e 20180101
```
Description of command line options :
* -f provides the folders filename
* -t is the tile number
* -s is the site name
* -d (aaaammdd) is the first date to process within the time series
* -e (aaaammdd) is the last date to process within the time serie-s
* -z directly uses zipped L1C files

Caution, *when a product has more than 90% of clouds, the L2A is not issued*. However, a folder with _NOTVALD_ in the filename is created.

## Known Errors

Some Sentinel-2 L1C products lack the angle information which is required by MAJA. In this case, MAJA stops processing with an error message. This causes issues particularly in the backward mode. These products were acquired in February and March 2016 and have not been reprocessed by ESA (despite repeated asks from my side). You should remove them from the folder which contains the list of L1C products to process. 

We still have some difficulties with detecting the edges of the swath, which results in false detections of clouds or shadows on the edges. We hope to solve this shortly.


<a name="Questions"></a>
# Questions
If you have issues or questions with MAJA, please raise an issue on this github repository. It will serve as a forum.

# References :
<a name="ref1">1</a>: A multi-temporal method for cloud detection, applied to FORMOSAT-2, VENµS, LANDSAT and SENTINEL-2 images, O Hagolle, M Huc, D. Villa Pascual, G Dedieu, Remote Sensing of Environment 114 (8), 1747-1755

<a name="ref2">2</a>: Correction of aerosol effects on multi-temporal images acquired with constant viewing angles: Application to Formosat-2 images, O Hagolle, G Dedieu, B Mougenot, V Debaecker, B Duchemin, A Meygret, Remote Sensing of Environment 112 (4), 1689-1701

<a name="ref3">3</a>: A Multi-Temporal and Multi-Spectral Method to Estimate Aerosol Optical Thickness over Land, for the Atmospheric Correction of FormoSat-2, LandSat, VENμS and Sentinel-2 Images, O Hagolle, M Huc, D Villa Pascual, G Dedieu, Remote Sensing 7 (3), 2668-2691

<a name="ref4">4</a>: MAJA's ATBD, O Hagolle, M. Huc, C. Desjardins; S. Auer; R. Richter, https://doi.org/10.5281/zenodo.1209633


    
   

   
