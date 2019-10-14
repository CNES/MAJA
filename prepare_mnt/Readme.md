# DTM and water mask tool as input for MUSCATE

This tool uses SRTM files from CGIAR-JRC processing, to provide the DTM used as input to MACCS/MAJA prototype, which includes also slope and aspect, 
at full and coarse resolution. It also uses SRTM water bodies files to produce the water masks used within MACCS. 
A special version for MAJA is described at the end of the Readme file, together with a conversion tool to obtain the input format needed for *MAJA operational version*


## SRTM CGIAR files :
The DTM tiles (by tiles of 5 degrees) can be fetched here http://srtm.csi.cgiar.org/SELECTION/inputCoord.asp
More documentation on the product is avaiilable here : http://www.cgiar-csi.org/data/srtm-90m-digital-elevation-database-v4-1

## SWBD 
Documentation is available here https://dds.cr.usgs.gov/srtm/version2_1/SWBD/SWBD_Documentation/SWDB_Product_Specific_Guidance.pdf
There was a ftp site, but it does not seem to be available.
Data can be downloaded from https://earthexplorer.usgs.gov/

In "data sets", select  
- Digital Elevation
  - SRTM
    - SRTM Water Body Data


## User manual
The tool requires a recent version of gdal (Minimum 1.11)


The parameter file, on my computer, is as follows :
```
INDIR_MNT =/mnt/data/DONNEES_AUX/SRTM
OUTDIR_MNT=/mnt/data/mnt
INDIR_EAU=/mnt/data/DONNEES_AUX/masque_eau
OUTDIR_EAU =/mnt/data/mnt
```

MNT means DTM and EAU means water


It also needs a file site. An example is provided : CVersailles.txt, which was used for SPOT4 (Take5), and 32SNE.txt, for Sentinel-2 tile 32SNE


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

Here is an example for tile 32TSNE (in Tunisia)

```
proj=UTM32N
EPSG_out=32632
chaine_proj=EPSG:32632
tx_min=0
ty_min=0
tx_max=0
ty_max=0
pas_x=109800
pas_y=109800
orig_x=499980
orig_y=4000020
marge=0
```

# Sentinel-2

A dedicated tool has been written for Sentinel-2 :

``` python tuilage_mnt_eau_S2.py -p parameters.txt -s 32SNE.txt -m SRTM```

`-c` is the coarse resolution used to speed some proceses in MAJA. It is 240m.


This tool generates data with the format needed for the prototype version of MACCS


A converter is available to obtain the data format to use as input of MAJA (operational version)

``` python conversion_format_maja.py -t 32SNE -f mnt/32SNE ```




