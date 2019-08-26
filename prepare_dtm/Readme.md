# prepare_mnt

The *prepare_mnt* tool uses SRTM files from CGIAR-JRC processing to provide the DTM (Digital Terrain Model or MNT in french) used as input to the MAJA processor. It also uses SRTM water bodies files to produce the water masks used within MAJA.

* [Requirements](#requirements)
* [Usage](#usage)
* [Download of SRTM-CGIAR files](#download-of-srtm-cgiar-files)
* [License](#license)
* [Contributors](#contributors)

## Requirements

The tool requires the following dependencies:
* Python 2.7 or 3.6, however other Python3 versions should also work. [Download](https://www.python.org/downloads/release/python-366/)
* GDAL version >= 1.11, can be downloaded [here](http://trac.osgeo.org/gdal/wiki/DownloadingGdalBinaries)
* SRTM and SWBD data for the desired region. Download is described in [this section](#download-of-srtm-cgiar-files)

## Usage

There are multiple ways of creating the DTM for MAJA. All of them will be explained in the following sections.

### DTMCreation-tool using a L1 product

The easiest way to launch the DTM creation is using the DTMCreation tool and a Level-1 product you want to process later.
To run the DTMCreation, you need to specify the following paths:
* -s: Folder to the SRTM zip-archives ([See here](#srtm-tiles))
* -w: Folder to the SWBD zip-archives ([See here](#swbd-srtm-water-body-data))
* -i: Folder of the Level-1 product
* -o: Folder where you want the output to be written to

For a complete list of optional parameters to be set, please run:

```
python DTMCreation.py --help
```

The following line executes the DTM creation for the example tile 29RPQ with an L1 product downloaded from [peps.cnes.fr](https://peps.cnes.fr/rocket/#/home) (using [peps-download](https://github.com/olivierhagolle/peps_download)):

```
python DTMCreation.py -i S2A_MSIL1C_20180706T110621_N0206_R137_T29RPQ_20180706T132942.SAFE/ -s ~/MAJA/DTM/SRTM/ -w ~/MAJA/DTM/Water/ -o ~/MAJA/DTM/29RPQ
```

### DTMCreation-tool using the Sentinel2 tiling grid kml

If you don't have a Level-1 product available yet you can download the [Sentinel-2 tiling grid kml-file](https://sentinel.esa.int/documents/247904/1955685/S2A_OPER_GIP_TILPAR_MPC__20151209T095117_V20150622T000000_21000101T000000_B00.kml) from the ESA [Sentinel-2 data products page](https://sentinel.esa.int/web/sentinel/missions/sentinel-2/data-products).

Running the DTMCreation needs therefore the following parameters:
* -s: Folder to the SRTM zip-archives ([See here](#srtm-tiles))
* -w: Folder to the SWBD zip-archives ([See here](#swbd-srtm-water-body-data))
* -o: Folder where you want the output to be written to
* -k: File-Path to the kml file
* -g: Granule- or Tile-ID (e.g. 29RPQ)

```
python DTMCreation.py -k S2A_OPER_GIP_TILPAR_MPC__20151209T095117_V20150622T000000_21000101T000000_B00.kml -g 20LPP -s /work/CESBIO/projects/Maja/SRTM/ -w /work/CESBIO/projects/Maja/SWBD/ -o ../DTM
```

### Manual operation

Each of the used python files can also be called individually. This requires to specify manually the Tile_Example.txt parameters
as well as the paths in the parameters.txt file. This is the original way prepare_mnt was working and is only recommended to advanced users who are in need of the intermediate outputs.

The parameter file, on my computer, is as follows :
```
INDIR_MNT =/mnt/data/DONNEES_AUX/SRTM
OUTDIR_MNT=/mnt/data/mnt
INDIR_EAU=/mnt/data/DONNEES_AUX/masque_eau
OUTDIR_EAU =/mnt/data/mnt
```

MNT means DTM and EAU means water


It also needs a file site. An example is provided : Tile_Example.txt, for Sentinel-2 tile 29RPQ

- proj is the projection name,
- EPSG_OUT, is the EPSG code of the projection,
- chaine_proj is the string to use to define it in gdal commands
 - you may find the information in the xml file provided with a granule :
    ```
      <HORIZONTAL_CS_NAME>WGS84 / UTM zone 32N</HORIZONTAL_CS_NAME>
      <HORIZONTAL_CS_CODE>EPSG:32632</HORIZONTAL_CS_CODE>
    ```
- The 4 values can stay equal to zero to produce only one tile. They can be integers if you want to generate a grid of tiles. For Sentinel-2 only produce one tile at a time.

      tx_min=0
      ty_min=0
      tx_max=0
      ty_max=0

- pas_x and pas_y are the image size in m. Please keep the same values as below.
- orig_x and orig_y are the coordinates of the upper left corner in m (gdalinfo can provide the information)
- marge is the size of the overlap region between tiles
  	For Sentinel-2, the margin is 0 as we produce DTM tile by tile due to the complex naming of tiles...

Here is an example for tile 29RPQ (in Northern Africa):

```
proj=UTM29N
EPSG_out=32629
chaine_proj=EPSG:32629
tx_min=0
ty_min=0
tx_max=0
ty_max=0
pas_x=109800
pas_y=109800
orig_x=600000
orig_y=3500040
marge=0
```

A dedicated tool has been written for Sentinel-2 :

```
python tuilage_mnt_eau_S2.py -p parameters.txt -s 29RPQ.txt -m SRTM
```
This tool generates data with the format needed for the prototype version of MACCS (MAJA's predecessor).
A converter is available to obtain the data format to use as input of the current version of MAJA.
```
python conversion_format_maja.py -t 29RPQ -f mnt/29RPQ
```

## Download of SRTM CGIAR files

The three methods presented all have in common that the needed files to create the DTM have to be downloaded from two external sources:

### SRTM tiles

The SRTM tiles (by tiles of 5 degrees) can be fetched here: http://srtm.csi.cgiar.org/SELECTION/inputCoord.asp

To get the correct archives, specify the maximum and minimum coordinates in Lon/Lat of the product. If you don't know those values
you can run the script `getCoordinates.py` with an input image  of your choice or the Sentinel-2 Tiling Grid kml. For a PEPS L1 product these examples can be obtained in `GRANULE/L1C_TILEID_xxxxxx_DATE/IMG_DATA/*jp2`.
For a product of tile 29RPQ the following output is given:
```
python getCoordinates.py ~/MAJA/inputs/L1/29RPQ/S2A_MSIL1C_20180706T110621_N0206_R137_T29RPQ_20180706T132942.SAFE/GRANULE/L1C_T29RPQ_A015864_20180706T112137/IMG_DATA/T29RPQ_20180706T110621_B02.jp2
```
The same with the kml:
```
python getCoordinates.py ~/Downloads/S2A_OPER_GIP_TILPAR_MPC__20151209T095117_V20150622T000000_21000101T000000_B00.kml 29RPQ
```

```
---Coordinates---
Lon min: -7.95642833975
Lat min: 30.626390258
Lon max: -6.78825383349
Lat max: 31.6311926292
```

More documentation on the product is available here : http://www.cgiar-csi.org/data/srtm-90m-digital-elevation-database-v4-1

### SBWD (SRTM Water Body data)

Data can be downloaded from https://earthexplorer.usgs.gov/

Documentation is available [here](https://dds.cr.usgs.gov/srtm/version2_1/SWBD/SWBD_Documentation/SWDB_Product_Specific_Guidance.pdf).

**Note**: You have to create an account in order to access the data. It is free of charge, though.

--add pictures here--

In "data sets", select  
- Digital Elevation
  - SRTM
    - SRTM Water Body Data

## License

This project is licensed under the GPLv3 License - see the [LICENSE.md](LICENSE.md) file for details

## Contributors

* **CS France** - *Initial Work*
* **Olivier Hagolle** - *Initial Work*
* **Peter KETTIG** - *Development of Start_maja* - peter.kettig@cnes.fr
