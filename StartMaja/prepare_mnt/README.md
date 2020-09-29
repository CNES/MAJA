# DTM and water mask tool as input for Maja

This tool uses SRTM files from CGIAR-JRC processing, to provide the DTM used as input to MACCS/MAJA prototype, which includes also slope and aspect, 
at full and coarse resolution. It also uses SRTM water bodies files to produce the water masks used within MACCS. 
A special version for MAJA is described at the end of the Readme file, together with a conversion tool to obtain the input format needed for *MAJA operational version*


## SRTM CGIAR files :
The DTM tiles (by tiles of 5 degrees) are automatically fetched here http://srtm.csi.cgiar.org/SELECTION/inputCoord.asp

More documentation on the product is available here : http://www.cgiar-csi.org/data/srtm-90m-digital-elevation-database-v4-1

## GSW (Global surface water) 
More documentation is provided here: https://global-surface-water.appspot.com/

## User manual
The tool requires a recent version of gdal (Minimum 2.0) and scipy.

To create a DTM for the tile `T31TCJ`, you first need an L1C .SAFE or L2A Maja-product.
The execute:

```
python DTMCreation -p S2A_MSIL1C_20180401T105031_N0206_R051_T31TCJ_20180401T144530.SAFE -o .
```

That will create in your current directory the following *two* files:
* S2__TEST_AUX_REFDE2_T31TCJ_0001.DBL.DIR
* S2__TEST_AUX_REFDE2_T31TCJ_0001.HDR

Which can then directly be used by Maja/Start_maja.

## References

* Jean-Francois Pekel, Andrew Cottam, Noel Gorelick, Alan S. Belward, High-resolution mapping of global surface water and its long-term changes. Nature 540, 418-422 (2016). (doi:10.1038/nature20584)
Contact: jrc-surfacewater@ec.europa.eu
