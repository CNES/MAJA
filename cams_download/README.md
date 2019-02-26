# download CAMS aerosol data

This tool is designed to download daily CAMS near-real-time forecast products from ECMWF. Otherwise, the graphical interface provided by ECMWF is available here: http://apps.ecmwf.int/datasets/data/macc-nrealtime/levtype=sfc/. We are providing two tools to download CAMS data and package them as expected by MAJA.

download_CAMS_daily.py downloads CAMS data day per day (ane even twice a day). But with a standard ECMWF account, it is slow. It takes currently (Feb 2019) about a day to download a day.

download_CAMS.py downloads monthly files, and splits them into daily files, except for the current month which is still downloaded daily. It can download several months per day. To split the monthly files into daily files, it is necessary to install netCDF utilities, either :
- [cdo, within the nco package](https://code.mpimet.mpg.de/projects/cdo) (by default)
- ncks netCDF kitchen sink (which does not seem to be well maintained anymore, sorry if we are wrong)

The tool downloads several types of fields :
- the Aerosol Optical Thickness AOT, for 5 types of aerosols, which are stored in the AOT file
- the mixing ratios of each aerosol types as a function of altitude (expressed as model levels), stored in MR file 
- the Relative Humidity as a function of altitude expressed in pressure levels, stored in the RH file.

These files are available currently twice a day, and some of the aprameters are only availble as a 3 hour forecast, and not an analysis. The files are downloaded in netCDF format and look like this:
```
CAMS_AOT_yyyymmdd_UTC_hhmm.nc
CAMS_MR_yyyymmdd_UTC_hhmm.nc
CAMS_RH_yyyymmdd_UTC_hhmm.nc
```

The files are then converted in one archive using the Earth Explorer format, which was selected as the standard format for MAJA external data. We have a HDR xml file, and a bzipped DBL archive, which contains the three files above for a given time and date. (I regret this choice which complexifies the use of data within MAJA, compared to using the plain netCDF files, but it the current situation)



# Configuration

 - Create a ECMWF account: https://apps.ecmwf.int/registration/

 - The folder ecmwfapi/ and the file setup.py must be in the working directory.

 - Get your API key at: https://api.ecmwf.int/v1/key/

 - Create the file '.ecmwfapirc' in your home and type:
```
{
"url" : "https://api.ecmwf.int/v1",
"key" : "your_key",
"email" : "your_email_address"
}
```

# Example

`python download_CAMS_daily.py -d 20180101 -f 20180102  -w /path/to/my/CAMSfolderNetCDF -a  /path/to/my/CAMSfolderNetDBL`

or

`python download_CAMS.py -d 20180101 -f 20180102  -w /path/to/my/CAMSfolderNetCDF -a  /path/to/my/CAMSfolderNetDBL`


# Parameters

The user can choose the following parameters with the command line:

 - w: where CAMS files will be stored

 - d,f: min/max range of dates to download

 - w: path to folder where netcdf data are stored (can be considered as a temporary file)
 - a: path to folder where DBL/HDR files are stored
 - k: to keep the netcdf files

Other parameters could be accessed within the code :

 - step: forecast step in hours (default 3, ony value available so far)

 - type of file to download: surface, pressure or model (default all)

 - name of output files

 - grid: spatial resolution of downloaded files

 - format: grib or netcdf

# Info on downloaded files

 - Surface files: Aerosol Optical Thickness (AOT) at 550 nm for the five aerosol models (BlackCarbon, Dust, Sulfate, SeaSalt and OrganicMatter).
```
Variable name	  Description
suaod550	  Sulphate Aerosol Optical Depth at 550nm
bcaod550	  BlackCarbon Aerosol Optical Depth at 550nm
ssaod550	  SeaSalt Aerosol Optical Depth at 550nm
duaod550	  Dust Aerosol Optical Depth at 550nm
omaod550	  OrganicMatter Aerosol Optical Depth at 550nm
```

 - Model files: mass Mixing Ratios (MR: expressed in kg of aerosol per kg of dry air [kg/kg]) of the aerosol models at 60 different altitude levels (model levels).
```
Variable name	  Description
aermr01	 	  Sea Salt Aerosol (0.03 - 0.5 um) Mixing Ratio
aermr02		  Sea Salt Aerosol (0.5 - 5 um) Mixing Ratio
aermr03		  Sea Salt Aerosol (5 - 20 um) Mixing Ratio
aermr04		  Dust Aerosol (0.03 - 0.55 um) Mixing Ratio
aermr05		  Dust Aerosol (0.55 - 0.9 um) Mixing Ratio
aermr06		  Dust Aerosol (0.9 - 20 um) Mixing Ratio
aermr07		  Hydrophobic Organic Matter Aerosol Mixing Ratio
aermr08		  Hydrophilic Organic Matter Aerosol Mixing Ratio
aermr09		  Hydrophobic Black Carbon Aerosol Mixing Ratio
aermr10		  Hydrophilic Black Carbon Aerosol Mixing Ratio
aermr11		  Sulphate Aerosol Mixing Ratio
```

 - Pressure files: Relative Humidity (RH: expressed in %) at 22 different altitude levels (pressure levels).
```
Variable name	  Description
r	 	  Relative Humidity
```

Information about these variables can also be found at http://atmosphere.copernicus.eu/ftp-access-global-data under the “Forecast surface parameters” and “Forecast model level parameters” sections, or at http://apps.ecmwf.int/codes/grib/param-db/
