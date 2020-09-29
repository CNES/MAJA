Operating manual
******************

This chapter describes the operating manual of MAJA.

Memory management
#################

MAJA processing can be performed on the whole image or by portions of
image called “strip”.

The strip size is automatically computed by the OTB pipeline footprint computation using the RAM parameter, the image sizes and the pipeline complexity.

It is then possible to raise or reduce the memory printfoot during MAJA
execution by changing this RAM parameter that can be found in the MAJAUserConfigSystem.xml configuration file ( see :ref:`operating_manual:user configuration files`)


Configuration and setting files
-------------------------------

User configuration files
~~~~~~~~~~~~~~~~~~~~~~~~

The user parameters required by MAJA are gathered in the following
configuration files:

=================================== ==========================================================================
**File**                            **Comment**
MAJAUserConfigSystem.xml (.xsd)     Contains all user configuration parameters (no Image parameters)
MAJAUserConfig_<MISSION>.xml (.xsd) Contains all <MISSION> user configuration parameters (no Image parameters)
=================================== ==========================================================================

Note:

-  Parameter definitions are set as html comment in the xml file,

-  The MAJAUserConfigSystem.xml and each MAJAUserConfig_<MISSION>.xml
   are installed in the ““<installation-directory>/etc/conf/user”
   directory.

   The “MAJAUserConfigSystem.xml” contains for example the following
   parameters that the user could be led to modify:

-  Number of cores used for the execution (“NbThreads’ field). By
   default, the number of threads is set to 1,

-  RAM maximum usage. By default, the RAM max is set to 8192,

-  The notes (or comments) inserted in the "Note” field in the output
   header product,

-  The field “EnableCleaningCachingDirectoryBeforeProcessing” is used to
   clean the caching directory at the beginning of the maja execution
   (the default value is false), Set to true in the relaunch case.

-  The field “EnableCleaningCachingDirectoryAfterProcessing” is used to
   clean the caching directory at the end of the maja execution (the
   default value is true),

-  The field “EnableCleaningTemporaryDirectory” is used to clean the
   temporary directory after processing (at the end of execution).
   Notice that this directory also contains the caching directory. If
   true, it cleans also this caching directory and therefore cancels the
   effect of the two previous options.

-  The field “CheckXMLFilesWithSchema” is used to enable or disable
   checking of the interfaces (control of inputs and outputs data with
   schemas),

-  The field “CleanInputZipFiles” is use to enable or disable cleaning
   the input product compressed (remove the .DBL files) after they have
   been uncompressed.

-  The field “ZipFiles” is use to enable or disable the compression of
   the output product (creation of the .DBL file)

-  The field “CleanFiles” is use to enable or disable cleaning the
   directory of the output product (.DBL.DIR directory),

-  The field “EnableL2ResolutionProcessing” is use to enable or disable
   the generation of the output product at L2 resolution (the default
   value is true).

   The “MAJAUserConfig_<MISSION>.xml” contains for example the following
   parameters:

-  | The option parameter that defines if a default constant value is
     used to set the ozone content
     (“Use_Default_Constant_Ozone_Amount”).
   | If the option is to use the default constant value, this value of
     ozone amount is available in the GIPP L2COMM. If the “ozone” option
     is set to false, the ozone content will be read in the meteo data.

   In “MAJAUserConfig_<MISSION>.xml”, only the two previous options in
   Atmospheric_Absorption_Correction tag can be modified according to
   the availability of meteo data (for ozone amount) and the values of
   Water_Vapor_Band_Code and Water_Vapor_Reference_Band_Code bands code
   parameters in the GIP \_L2COMM according to the L1 product (for water
   vapor amount).

   Default values are set for the other parameters and they should not
   be modified by the user.

   An example of the MAJAUserConfig_SENTINEL2.xml is given below with
   default values:

=========================================================================================================

.. code:: xml
	  
   <Config xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
   xsi:schemaLocation="./MAJAUserConfig.xsd" xmlns="http://maja.fr">

   <Business>

   <!-- Image divisions size (number of lines) -->

   <ImageDivision method="strips">

   <NbStrips>

   <!-- Image divisions size (in number of lines) for the L2 Init
   Processing -->

   <L2InitProcessing>250</L2InitProcessing>

   <!-- Image divisions size (in number of lines) for the L2 Nominal
   Processing -->

   <L2NominalProcessing>100</L2NominalProcessing>

   <!-- Image divisions size (in number of lines) L2 Backward Processing
   -->

   <L2BackwardProcessing>100</L2BackwardProcessing>

   <!-- Image divisions size (in number of lines) L3 Processing -->

   <L3Processing>200</L3Processing>

   <!-- Image divisions size (in number of lines) Checktool -->

   <ChecktoolProcessing>200</ChecktoolProcessing>

   </NbStrips>

   </ImageDivision>

   </Business>

   <Algorithms>

   <GRID_Reference_Altitudes>

   <!-- First reference altitude of solar grid (in meter) -->

   <SOLH1>3000</SOLH1>

   <!-- First reference altitude of solar direction (in meter) -->

   <SOLHRef>4000</SOLHRef>

   <!-- Reference altitude of viewing direction (in meter) -->

   <VIEHRef>3000</VIEHRef>

   </GRID_Reference_Altitudes>

   <Atmospheric_Absorption_Correction>

   <!-- Option to use default constant value available in GIPP instead
   of meteorological data to determine the ozone amount -->

   <Use_Default_Constant_Ozone_Amount>false</Use_Default_Constant_Ozone_Amount>

   </Atmospheric_Absorption_Correction>

   </Algorithms>

   <Config>

=========================================================================================================

Administration configuration files
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

The following files are necessary to configure MAJA:

==================================== ==========================================================================
**File**                             **Comment**
MAJAAdminConfigSystem.xml (.xsd)     Contains all administration configuration parameters (no Image parameters)
MAJAAdminConfig_<MISSION>.xml (.xsd) Contains all <MISSION> configuration parameters
==================================== ==========================================================================

The MAJAAdminConfigSystem.xml file is installed in the
“<installation-directory>/etc/conf/admin” directory.

The MAJAAdminConfigSystem.xml file defines few parameters as:

-  The <System> field is used to set the name of the system. For
   example, set the name of the “operational centre” (VIP, SL2P, etc.).
   This value is inserted in all headers files of the L2 and L3 product
   in the field <Fixed_Header/Source/System>

-  The tolerances’ thresholds for footprint matching between the L1, L2,
   L2Coarse and DEM

-  The option to measure the performance of the chain (time processing
   and memory footprint): <EnablePerformanceMeasureAlgorithms>

   The MAJAAdmin_<MISSION>.xml files defines few parameters as:

-  The theoretical wavelength for each band: this values are used to set
   the theoretical wavelength in the composite XML headers in the L2
   product (reflectances and LTC data)

   An example of the MAJAAdminConfig_SENTINEL2.xml is given below with
   default values:

=========================================================================================================

.. code:: xml
    
    <Config xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" xsi:schemaLocation="./MAJAAdminConfig.xsd">
    <Composite>
    <!-- List of the bands with the theoretical wavelengths -->
    <!-- Used for composite algorithms -->
    <List_of_Band_Theoretical_Wavelength count="13">
    <Band_Theoretical_Wavelength sk="B1" unit="nanometer">443</Band_Theoretical_Wavelength>
    <Band_Theoretical_Wavelength sk="B2" unit="nanometer">490</Band_Theoretical_Wavelength>
    <Band_Theoretical_Wavelength sk="B3" unit="nanometer">560</Band_Theoretical_Wavelength>
    <Band_Theoretical_Wavelength sk="B4" unit="nanometer">665</Band_Theoretical_Wavelength>
    <Band_Theoretical_Wavelength sk="B5" unit="nanometer">705</Band_Theoretical_Wavelength>
    <Band_Theoretical_Wavelength sk="B6" unit="nanometer">740</Band_Theoretical_Wavelength>
    <Band_Theoretical_Wavelength sk="B7" unit="nanometer">780</Band_Theoretical_Wavelength>
    <Band_Theoretical_Wavelength sk="B8" unit="nanometer">840</Band_Theoretical_Wavelength>
    <Band_Theoretical_Wavelength sk="B8A" unit="nanometer">865</Band_Theoretical_Wavelength>
    <Band_Theoretical_Wavelength sk="B9" unit="nanometer">940</Band_Theoretical_Wavelength>
    <Band_Theoretical_Wavelength sk="B10" unit="nanometer">1380</Band_Theoretical_Wavelength>
    <Band_Theoretical_Wavelength sk="B11" unit="nanometer">1600</Band_Theoretical_Wavelength>
    <Band_Theoretical_Wavelength sk="B12" unit="nanometer">2200</Band_Theoretical_Wavelength>
    </List_of_Band_Theoretical_Wavelength>
    </Composite>
    </Config>

=========================================================================================================

The following files are required to run MAJA:

============ ========================================================================
**File**     **Comment**
A font file. By default, the Font file used is the “Amble-Italic.ttf” true type file.
============ ========================================================================

**Warning:** The /etc/conf directory contains the configuration files
used by the chain and could be modified by users. **The /share/conf
directory contains INTER**\ **NAL data used by the chain that should NOT
be modified by users**.

GIPP configuration file (GIP_L2COMM)
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

The GIP_L2COMM is used to configure the different algorithms of the
chain. One instance of this file is defined for each mission. Different
kinds of parameters are set in this file. If an algorithm is never
activated for a sensor (general configuration parameters §4.4), no
parameter is defined in the GIP_L2COMM. Only parameters that modify the
chain processing are detailed here after.

The file contains the following parameters defining processing options
of the chain:

-  The method used to estimate the aerosol optical thickness is set in
   the <AOT_Method> parameter. Values are: MULTITEMPORAL, MULTISPECTRAL
   or SPECTROTEMPORAL

-  The implementation of environment correction is triggered via the
   <Env_Corr_Option> parameter

-  The option to refine the cloud altitude instead of trust the
   stereoscopic altitude (available with VENuS products):
   <Refinement_Option>

-  If the parameter “Use_Default_Constant_Ozone_Amount” is set to true
   in the MAJAUserConfig_<MISSION>.xml file, the parameter
   “Ozone_Amount_Default_Value” is read by the chain.

-  The option to use or not the CAMS data “Use_Cams_Data” to activate
   the CAMS data search in order to enhance the quality of corrections.
   The list of models to use is given in the “List_Of_Models” node so as
   the “Time_Window_CAMS” validity period. If no valid data is found the
   “Constant_Model” is used to compute the athmospheric composition.

-  For the water vapour amount, the parameter
   “Water_Amount_Default_Value”:

   -  is always used for Formosat, Landsat 5, 7 and 8 or Spot4
         (therefore the parameter “Use_Default_Constant_Water_Amount”
         does not exist in the GIP_L2COMM for those sensors),

   -  is optional for VENUS and Sentinel2. It is used if the
         “Use_Default_Constant_Water_Amount” is set to true. In this
         case, the GIPP “GIP_L2WATV” is not mandatory in input.
         Otherwise, the water vapour content is interpolated within the
         LUT GIP_L2WATV using the ratio of the reflectance at 865 and
         910 (or 940) nm.

It is important to note that this file contains especially the list of
all the thematic bands used in the different algorithms of the chain.
Only these bands are stored in the composite products contained in the
private part of the L2 product (RTC, RTA, RCR and LTC files). Those
thematic bands are set in different nodes of the GIPP file:

For the ‘reflectance’ files of the product (RTC, RTA and RCR):

-  <Thematic_Definition> node:

-  **Blue_Band_Code**

-  **Red_Band_Code**

-  **NIR_Band_Code**

-  **SWIR1_Band_Code**

-  <Cloud_Masking> node

-  **Correl_Band_Code**

-  **Shadow_Band_Code**

-  <Rain_Flag> node:

-  **Water_Band_Code**

-  <AOT_Estimation> node

-  **Dark_Band_Code**

-  **Var_Band_Code**

-  **MT_AOT_Band_Code**

For the ‘LTC’ file of the product:

-  <AOT_Estimation> node

-  **MT_AOT_Band_Code**

Attention: in the composite product (and LTC), the band is identified
with the dedicated “theoretical wavelength” (and not the band code
name).

This file contains also general parameters as:

-  the no data value set in the output L2 product: <No_Data>

-  the thematic band definition:

-  bands used for all the algorithms: blue, green, red, NIR, SWIR bands,

-  band used to generate the quicklook: blue, green, red bands

-  the maximum percentage of cloudy or no data pixels for the product to
   be considered as valid

-  The water vapor quantification value in g/cm² of the water vapor data
   (ATB file) in the <VAP_Quantification_Value> parameter and the
   no_data value of this image plan. Those values are also indicated in
   the header of the ATB file.

-  The AOT quantification value (dimensionless values) and no data value
   (ATB file) in the <AOT_Quantification_Value> and <AOT_No_Data_Value>
   parameters. Those values are also indicated in the header of the ATB
   file.

Log messages
------------

The log messages raised by MAJA are compliant (in terms of format) with
the nomenclature described in the section 4.2 of the [AD01].

One log message consists of:

-  The date,

-  The machine name,

-  The processor name (written in the JobOrder file),

-  The processor version,

-  The process identifier (PID),

- For Progress : Maximum RAM used during the processing, disk usage in working directory, time spend, cpu average load

-  The type of message in increasing order of severity:

-  [D] for Debug,

-  [I] for Info,

-  [P] for Progress,

-  [W] for Warning,

-  [E] for Error.

-  The message itself.

For example: *2011-03-02T17:03:44.518677 milo.si.c-s.fr maja-processing
01.00 [000000032108] [P] Starting L2Processor PreProcessing() ….*

The default log level is [I] ; in this case, all Info, Progress, Warning
and Errors messages are displayed.

Errors management
-----------------

Error messages are sorted in 7 categories and each category has its
specific code error.

The following table gathered the errors that can occur according to each
possible process.

.. tabularcolumns:: |p{1in}|p{1in}|p{3.25in}|p{0,75in}|
		    
====================================== ================ ====================================================================================================================================================================================================================================================================================================================================================================================================================================== ===============
**Process**                            **Category**     **General description and comment**                                                                                                                                                                                                                                                                                                                                                                                                    **Return code**
**L2Init, L2Nominal, L2Backward**      **« Chain »**    The error is raised at a high execution level, in the “ScientificProcessing” process (algorithmic chain processing). The error can be due to an incorrect input data (missing file in a input product for example, invalid JobOrder, etc.). Generally, this error is an user error.                                                                                                                                                    135
**(Scientific**                        **(maja)**                                                                                                                                                                                                                                                                                                                                                                                                                                             
**Processing)**                                                                                                                                                                                                                                                                                                                                                                                                                                                                                 
\                                      **« Business »** The raised error comes from the algorithmic chain. If such an error occurs, it is highly probable that this error is an internal one (software anomaly). In this case, the operator has to refer to the project manager.                                                                                                                                                                                                               134
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                            
                                       **(maja)**                                                                                                                                                                                                                                                                                                                                                                                                                                             
\                                      **« Data »**     The raised error comes from the data access chain layer (read/write). Such an error occurs when an input data (or a data generated during the chain processing) doesn’t comply its contents (missing or incomplete file, wrong format, etc.). In this case, the operator has to control that the data specified in the JobOrder are valid (see the product interfaces). In any case, the operator has to refer to the project manager. 133
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                            
                                       **(maja)**                                                                                                                                                                                                                                                                                                                                                                                                                                             
\                                      **« OTB »        Low layer error raised by the OTB/ITK library. In any case, the operator has to refer to the project manager to report the bug.                                                                                                                                                                                                                                                                                                        131
                                       (cots)**                                                                                                                                                                                                                                                                                                                                                                                                                                               
\                                      **« alloc »      Memory allocation error (such as *bad_alloc*). It occurs when the system failed to reserve the memory needed by the chain processing. This low layer error is raised by the stlc++ base library.                                                                                                                                                                                                                                       130
                                       (cots)**                                                                                                                                                                                                                                                                                                                                                                                                                                               
                                                        In this case, the operator has to refer to the project manager to report the bug.                                                                                                                                                                                                                                                                                                                                                     
\                                      **« std »        Low layer error (other than **bad_alloc**) raised by the stlc++ base library.                                                                                                                                                                                                                                                                                                                                                          129
                                       (cots)**                                                                                                                                                                                                                                                                                                                                                                                                                                               
                                                        In this case, the operator has to refer to the project manager to report the bug.                                                                                                                                                                                                                                                                                                                                                     
\                                      **« Unknown »    Non identified error (unlikely case). In this case, the operator has to refer to the project manager to report the bug.                                                                                                                                                                                                                                                                                                                128
                                       (cots)**                                                                                                                                                                                                                                                                                                                                                                                                                                               
====================================== ================ ====================================================================================================================================================================================================================================================================================================================================================================================================================================== ===============

The error messages are compliant with the nomenclature described in the
section [AD01].

: Example of the command line help of the maja
==============================================

For the version 4.0 of MAJA, the maja –help produces the following
helper lines

.. code::
   
   usage: maja.py [-h] [-j JOBORDER] [-l {INFO,PROGRESS,WARNING,DEBUG,ERROR}]
               [-m {L2INIT,L2NOMINAL,L2BACKWARD}] [-t]
               [--stylesheet STYLESHEET] [-acs ADMINCONF] [-i INPUT]
               [-o OUTPUT] [-w WORKINGDIR] [-p PLUGIN] [-ucs CONF]
               [--NbThreads NBTHREADS] [--CheckXMLFilesWithSchema]
               [--CleanInputZipFiles] [--CleanFiles] [--ZipFiles]
               [--EnableCleaningCachingDirectoryBeforeProcessing]
               [--EnableCleaningCachingDirectoryAfterProcessing]
               [--EnableCleaningTemporaryDirectory] [--TileId TILEID]
               [--perfos-log] [--perfos-report] [-v]
   ./maja [options] 

            MAJA Chains 

            CNES All rights reserved. For more details, see Copyright.txt file.

            Description: 

            ------------

            The L2 processor offers advanced atmospheric correction algorithms including

            water vapour and aerosol estimates based on multitemporal data analysis.

            It also provides cloud mask generation.

                - Cloud masking

                    * Cloud detection

                    * Shadow detection

               - Atmospheric correction

                   * Gaseous absorption correction

                   * Scattering correction

               - Environment and slope correction

                   * Environment effects

                   * Slope correction

               - Composite image update

            The data and GIPPs files mandatory for MAJA are:

                - For L2 processing:

                    * GIP_L2COMM

                    * GIP_L2DIRT

                    * GIP_L2DIFT

                    * GIP_L2SMAC

                    * GIP_L2WATV

                    * GIP_L2TOCR

                    * GIP_L2ALBD

                    * GIP_L2SITE

                    * EXO_METDTA

                    * AUX_REFDE2

            Processing description:

            -----------------------

            The maja launches the following processes:

                - launches the pre-processing treatment

                    * Uncompresses all data (DBL package files and BZ2 images files)

                    * Check input data with the schemas

                    * Deletes all tarballs (if option is enable in the Configuration file)

                    * Applies a specific stylesheet on GIPPs files

                - launches the scientific-processing treatment

                    * Reads image products

                    * Applies algorithms

                    * Formats EE and writes datas

                - launches the post-processing treatment

                    * Check output data with the schemas

                    * Compress BZ2 all .TIF images data files

                    * Generates the .DBL image product data (L2/L3)

             For more details, report to the SUM (Software User Manual, ref. LAIG-MU-MAC-010-CS)

             Author: CS Systemes d'Information  (France)

             User cases:

             -----------

             1. First user case:  Use only a JobOrder file to launch maja processing. 

                                  -> use the '--jobOrder' option.

                                  Note: the possible values for the <Processor_Name> field are:

                                     *  m_MAJA_L2_INIT_CHAIN 

                                     *  m_MAJA_L2_NOMINAL_CHAIN 

                                     *  m_MAJA_L2_BACKWARD_CHAIN 
               
             2. Second user case: Use command line parameters to launch maja processing

                                  -> for example, use at least the '--mode' and '--input' options

                                   
                                   
                                   

   optional arguments:
     -h, --help            show this help message and exit
     -j JOBORDER, --jobOrder JOBORDER
                           Specify the JobOrder file (xml file)
     -l {INFO,PROGRESS,WARNING,DEBUG,ERROR}, --loglevel {INFO,PROGRESS,WARNING,DEBUG,ERROR}
                           Log level use and set to the JobOrder generated.
     -m {L2INIT,L2NOMINAL,L2BACKWARD}, --mode {L2INIT,L2NOMINAL,L2BACKWARD}
                           Processing mode.
     -t, --enableTest      Enable/Disable the field value 'Test' set in the JobOrder generated.
     --stylesheet STYLESHEET
                           XML Stylesheet filename, used to overloads parameters in the XML configuration files and GIPP files. See the [MU] for an example of StyleSheet.
     -acs ADMINCONF, --adminconf ADMINCONF
                           Administration Configuration directory (contains for example the MAJAAdminConfigSystem.xml)
     -i INPUT, --input INPUT
                           Input data directory: must be contain images, all GIPPs files, the DTM, etc.).
                                                   The directory must be contain only one L1 product for the 'L2INIT' mode, a list of L1 products 
                                                   for the 'L2BACKWARD' mode, one
                                                   L1 product and one L2 product for the 'L2NOMINAL' mode and a list of L2 products 
                                                   
     -o OUTPUT, --output OUTPUT
                           Output data directory (product directory). Default value: '.'
     -w WORKINGDIR, --workingdir WORKINGDIR
                           Working directory (working 'temporary' directory). Default value: the '--output parameter value'
     -p PLUGIN, --plugin PLUGIN
                           Output plugin to use to write L2 product. Available plugins : SENTINEL2 , SENTINEL2_MUSCATE , VENUS_MUSCATE , LANDSAT8_MUSCATE. Default is to use the MUSCATE version of the input L1 product plugin
     -ucs CONF, --conf CONF
                           User Configuration directory (contains for example MAJAUserConfigSystem.xml)
     --NbThreads NBTHREADS
                           UserConfigSystem overloads value for the parameter 'NbThreads'
     --CheckXMLFilesWithSchema
                           UserConfigSystem overloads value for the parameter 'CheckXMLFilesWithSchema'
     --CleanInputZipFiles  UserConfigSystem overloads value for the parameter 'CleanInputZipFiles'
     --CleanFiles          UserConfigSystem overloads value for the parameter 'CleanFiles'
     --ZipFiles            UserConfigSystem overloads value for the parameter 'ZipFiles'
     --EnableCleaningCachingDirectoryBeforeProcessing
                           UserConfigSystem overloads value for the parameter 'EnableCleaningCachingDirectoryBeforeProcessing'
     --EnableCleaningCachingDirectoryAfterProcessing
                           UserConfigSystem overloads value for the parameter 'EnableCleaningCachingDirectoryAfterProcessing'
     --EnableCleaningTemporaryDirectory
                           UserConfigSystem overloads value for the parameter 'EnableCleaningTemporaryDirectory'
     --TileId TILEID       Set the Tile id of the Sentinel2 L1 product (Only necessary for SENTINEL2 plug-in and only for the L1C product with PSD version < PSD 14)
     --perfos-log          Enable performance measurement in log
     --perfos-report       Enable synthesize performance measurement in log and xml file
     -v, --version         Display version information and exit
