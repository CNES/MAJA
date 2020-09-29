CS Systèmes d'Information

======================================= =============================================== ==== ================= ============
|image0|                                MAJA                                                                  
Business Unit ESPACE                                                                                          
Département Payload Data & Applications                                                                       
Image Processing                        **SETG-**\ **MU-**\ **MAJA-**\ **010-**\ **CS**                       
\                                       Change                                          : 01 Date              : 20/01/2017
\                                       Issue                                           : 04 Date              : 24/06/2019
Ref.  : CSSI/SPACE/PD&A/MAJA/MU                                                              Distribution Code  : E
USER, INSTALLATION AND OPERATING MANUAL                                                                       
MAJA CHAINS                                                                                                   
[MU]                                                                                                          
======================================= =============================================== ==== ================= ============

=================================== =================
Written by :                        Date : 22/05/201 
                                                     
ESQUIS Benjamin CSSI/ESPACE/PDA/IP                   
Written by :                        Date : 22/05/201 
                                                     
BRICIER Aurélien CSSI/ESPACE/PDA/IP                  
Approved by :                       Date : 22/05/201 
                                                     
OLIVIE Francis CSSI/ESPACE/DSM                       
For application :                   Date : 18/04/2018
                                                     
Esquis Benjamin CSSI/ESPACE/PDA/IP                   
=================================== =================

Index Sheet

======================================================================================== =================================================== ====================== =============
CONFIDENTIALITY :                                                                        KEYWORDS : MAJA; MI, MU, ME; STAND-ALONE                                  
DLP                                                                                                                                                                
TITLE :                                                                                                                                                            
                                                                                                                                                                   
| USER, INSTALLATION AND OPERATING MANUAL                                                                                                                          
| MAJA CHAINS                                                                                                                                                      
                                                                                                                                                                   
[MU]                                                                                                                                                               
   AUTHOR(s) :                                                                                                                                                     
                                                                                                                                                                   
   BRICIER Aurélien CSSI/ESPACE/PDA/IP                                                                                                                             
SUMMARY : This is MAJA user, installation and operating manual in a stand-alone context.                                                                           
RELATED DOCUMENTS : Stand alone document.                                                LOCALIZATION :                                                            
                                                                                          CSSI/SPACE/PD&A/MAJA                                                     
Volume : 1                                                                               TOTAL NUMBER OF PAGES : 60                          COMPOSITE DOCUMENT : N LANGUAGE : EN
                                                                                                                                                                   
                                                                                         INCLUDING PRELIMINARY PAGES : gdocNBPAGESLIMINAIRES                       
                                                                                                                                                                   
                                                                                         NUMBER OF SUPPL. PAGES : 0                                                
CONFIGURATION MANAGEMENT : NG                                                            CM RESP. :                                                                
CAUSE D'EVOLUTION : Update document for MAJA v3.3.1                                                                                                                
CONTRACT : Marché ACIS n°131348                                                                                                                                    
HOST SYSTEM : WordVersion                                                                                                                                          
======================================================================================== =================================================== ====================== =============

Internal Distribution

================== ==================== =================== ============
Name               Entity               Internal Postal Box Observations
OLIVIE Francis     CSSI/ESPACE/DSM                         
CHOGNARD Catherine CSSI/ESPACE/PDA/PDGS                    
BRICIER Aurélien   CSSI/ESPACE/PDA/IP                      
\                                                          
\                                                          
\                                                          
\                                                          
\                                                          
\                                                          
\                                                          
\                                                          
\                                                          
================== ==================== =================== ============

External Distribution

================== ======================== ============
Name               Entity                   Observations
BAILLARIN Simon    DNO/OT/IS               
DESJARDINS Camille DSO/SI/MO               
HAGOLLE Olivier    DSO/SI/CB               
LARIF Marie-France DNO/SC/3S               
LONJOU Vincent     DSO/SI/MO               
PETRUCCI Beatrice  DNO/OT/IS               
DELHEM Yamina      SCALAGENT POUR DNO/DA/AQ
LASSALLE Pierre    DNO/OT/IS               
\                                          
================== ======================== ============

Changes

===== ==== ========== ===============================================================================================
Issue Rev. Date       Reference, Author(s), Reasons for evolution
01    04   24/06/2019  CSSI/SPACE/PD&A/MAJA/MU
                     
                      ESQUIS Benjamin CSSI/ESPACE/PDA/IP
                     
                      Update document for MAJA v3.3.1
01    03   18/04/2018  CSSI/SPACE/PD&A/MAJA/MU
                     
                      BRICIER Aurélien CSSI/ESPACE/PDA/IP
                     
                      Update document for MAJA v3.1.0
01    02   19/10/2017  CSSI/SPACE/PD&A/MAJA/MU
                     
                      ESQUIS Benjamin CSSI/ESPACE/PDA/IP
                     
                      Update document for MAJA v3.0.0.
01    01   26/06/2017  CSSI/SPACE/PD&A/MAJA/MU
                     
                      FEUVRIER Thomas CSSI/ESPACE/PDA/IP
                     
                      Update document for MAJA v2.1.0. Add installation prerequisites for 'gzip' and 'bzip2' packages
01    00   20/01/2017  CSSI/SPACE/PD&A/MAJA/MU
                     
                      FEUVRIER Thomas CSSI/ESPACE/PDA/IP
                     
                      Previous document reference LAIG-MU-MAC-010-CS Ed.3 Rev.12
                     
                      Creation of document for MAJA v1.0
\                    
\                    
===== ==== ========== ===============================================================================================

Table of contents

Glossary and list of TBC and TBD items 1

1. Reference And applicable documents 1

2. SYSTEM REQUIREMENTS 2

2.1. Host platform 2

2.2. Space disk requirements 2

3. Installation manual 3

3.1. User Account and permission for installation 3

3.2. MAJA installation 4

3.2.1. Requirement 4

3.2.2. Description of the package delivered 4

3.2.3. Package installation 4

3.2.4. Arborescence of the installation of MAJA 4

4. User manual 6

4.1. User account and permission 6

4.2. Launching MAJA processing 6

4.3. Interfaces of MAJA chains 7

4.3.1. MAJA Processing modes 7

4.3.2. Processing Interfaces 10

4.3.3. Operational Interfaces 12

4.3.4. Revisit improvement via sensors mixing 12

4.4. The algorithms 13

4.5. The products 13

4.5.1. Venµs Image products 15

4.5.1.1. Level 1 product content 15

4.5.1.2. Level 2 product content 16

4.5.1.3. Level 3 product content 17

4.5.2. Sentinel 2 Image products 17

4.5.3. The Landsat L5/L7 Image products 19

4.5.3.1. Level 1 product content 19

4.5.3.2. Level 2 product content 20

4.5.4. The Landsat L8 Image products 21

4.5.4.1. Level 1 product content 21

4.5.4.2. Level 2 product content 22

4.5.5. The Spot4 Image products 23

4.5.5.1. Level 1 product content 23

4.5.5.2. Level 2 product content 24

4.5.6. The Formosat2 Image products 25

4.5.6.1. Level 1 product content 25

4.5.6.2. Level 2 product content 26

4.5.6.3. Level 3 product content 27

4.5.7. The Landsat L5/L7 “MUSCATE” Image products 27

4.5.8. The Landsat L8 “MUSCATE” Image products 27

4.5.9. The Landsat L8 “native” Image products 27

4.5.9.1. Level 1 product content 27

4.5.9.2. Level 2 product content 27

4.5.10. The Sentinel2 “native” Image products 27

4.5.10.1. Level 1 product content 27

4.5.10.2. Level 2 product content 27

4.5.11. The Sentinel2 “MUSCATE” Image products 28

4.6. auxiliary data of MAJA 28

4.7. GIPPs files of MAJA 29

5. Operating manual 31

5.1. Memory management 31

5.2. Configuration and setting files 32

5.2.1. User configuration files 32

5.2.2. Administration configuration files 34

5.2.3. GIPP configuration file (GIP_L2COMM) 34

5.3. Log messages 36

5.4. Errors management 37

ANNEX A : EXAMPLE OF THE COMMAND LINE HELP OF THE MAJA A.38

ANNEX C : EXAMPLE OF MAJA JOB ORDER FILE C.1

Glossary and list of TBC and TBD items
======================================

== ==============
CR Change Request
\ 
\ 
== ==============

**List of TBC items:**

**List of TBD items:**

Reference And applicable documents
==================================

Here below the list of reference and applicable documents for this
manual:

[AD01]: ESA Generic Processor Interfaces Guidelines, issue 1, revision 0
of 20/10/2006.

[RD01]: Dossier de Performances, SETG\ **-DP-MAJA-010-CS,** (in french).

[RD02]: Technical Note. Venus Ground Segment Interfaces file format
specification, VE-NT-GSSM-196-CNES

[RD03]: Technical Note Sentinel-2 MAJA Level-2A Product Format
Specification, GS2-NT-GSL2-1320-CNES

[RD04]: MAJA Level 1 and 2 products specification for FORMOSAT2,
LANDSAT5, 7, 8 AND SPOT4 (“proto” format”, PSC-IF-411-0081-CNES

[RD05]: Spécification de format des produits (format “muscate”),
PSC-SL-411-0032-CG

[RD06]: Sentinel-2 Products. Specification Document, S2-PDGS-TAS-DI-PSD,
V12 and V14 (both products formats handled by MAJA software)

[RD07]: Landsat8 (L8), Level 1 (L1) Data Format Control Book (DFCB),
V8.0

SYSTEM REQUIREMENTS
===================

Host platform
-------------

MAJA software shall be installed, used and operated on a Linux platform;
MAJA is available for all 5.x, 6.x and 7.x versions of the OS.
Performance tests (cf. RD02) suggest to use a platform with at least 4
Gb of RAM.

Space disk requirements
-----------------------

Data volumes change considerably according to the mission (Formosat,
Landsat, Venus or Sentinel-2) processed in MAJA execution.

Space disk dimensioning for the host platform has been performed
considering the worst cases between all the figures observed with the
data of the different spectral cameras.

At least the following space disk shall be available on the platform to
run one single execution of MAJA:

-  Input data volume: 4 GBytes (1 GBytes for GIPPs files and DTM and 3
   GBytes for the L1 image product),

-  Output data volume: 4 GBytes (L2 image product)

-  Caching data directory volume: ~30 GBytes

   The caching directory is named “.maja-temporary-directory” and it is
   created in the output directory; by default, it is automatically
   deleted at the end of each successful processing.

   Cleanup options are configurable via dedicated parameters in the
   “MAJAUserConfigSystem.xml” configuration file (by default value is
   “enable”).

   For investigation purposes, it is then possible to disable this
   cleanup and keep all the intermediary files (see §5.2.1).

Installation manual 
====================

This section describes the procedure to install MAJA software.

MAJA **installation** consists of one step:

1. Installation of the COTS and the scientific chains for operational
   context (MAJA).

User Account and permission for installation
--------------------------------------------

The installation directory is configurable and must be defined during
the install process. It is mandatory to have the right of writing in the
installation directory.

MAJA installation
-----------------

Requirement
~~~~~~~~~~~

A valid MAJA binary package

Description of the package delivered
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

MAJA is delivered as a .run package:

MAJA-<Version>.run

The validity of this binary package is checked automatically during
installation.

Where <**Version**> is the version number of the MAJA configuration item
given as **"X.Y.Z", being:**

**X**: first version digit , incremented for versions implementing major
CR

**Y**: second version digit, incremented for versions implementing minor
CR or correcting bugs

**Z**: third version digit, incremented to indicate patch version.

Package installation
~~~~~~~~~~~~~~~~~~~~

To show install options, execute the following command:

$ MAJA-<Version>.run --help

To install MAJA in a specific destination directory, execute the
following command:

$ MAJA-<Version>.run --target <install directory>

Arborescence of the installation of MAJA
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

MAJA installation creates the following arborescence (example for “/opt”
installation directory):

=========== =====================================================================================================================
|Capture-1| **/<install_dir>/maja**: MAJA base directory
           
            **/<Version>**: contains the version <Version> of the scientific chains version <Version> (formatted with two digits)
           
            **/bin**: contains the executables and scripts
           
            /etc: contains user data files
           
            /conf: configuration files
           
            /admin: admin configuration files, use only by the administrator of MAJA
           
            /user: users configuration files
           
            **/lib:** contains the library
           
            **/share:** contains the resources files
           
            /config: contains the internal configuration files for MAJA Administrator. Do not modify.
           
            **/ddc: ddc files for project management.** . Do not modify.
           
            /doc: copyright information
           
            **/examples: contains examples of GIPPs files**
           
            **/schemas**: contains all the ICD schemas used by the chains (EarthExplorer, Venus, Maja, Senitnel2, etc…)
           
            /MAJA_VERSIONS_FOR_SW_COMPONENT: show the version of MAJA
=========== =====================================================================================================================

User manual
===========

This chapter describes the user manual for the MAJA chains.

User account and permission
---------------------------

The user needs at least read and execution rights within the MAJA
install directory (see § 3.2.3).

Launching MAJA processing
-------------------------

Figure 1: Illustration of using MAJA

MAJA is launched in command line in two possible ways (example for
“/opt” installation directory):

-  Providing the JobOrder file as argument of the command line:

   $ /<path to maja installation dir>/bin/maja --jobOrder
   ./myJobOrder.xml

Figure 2: Example to execute MAJA with a JobOrder

The JobOrder file details the processing mode, the access path to all
the inputs (images, GIPP, DTM, meteo data) and to the configuration
files, the access path to the directory containing all the output
produced by MAJA.

   Note: the possible values for the <Processor_Name> node are:

-  MAJA_L2_INIT_CHAIN

-  MAJA_L2_NOMINAL_CHAIN

-  MAJA_L2_BACKWARD_CHAIN

-  MAJA_L3_CHAIN

-  MAJA_L1_CHECKTOOL_CHAIN

-  MAJA_L2_CHECKTOOL_CHAIN

-  MAJA_L3_CHECKTOOL_CHAIN

   An example of Job Order file is provided in [AD01].

-  Providing all the processing details (processing mode, input data
   directory, output directory, etc) as arguments of the command line:

   $ /<path to maja installation dir>/bin/maja

   $ --mode L2INIT # Processing mode

   $ --input ./Input/Images # Input data directory

   $ --conf ./Input/Conf # User configuration files directory$

   $ --TileId “ID” # TileId, only necessary for PSD13 Sentinel2 products

   $ --output ./WorkingDir # Output working directory

Figure 3: Example to execute MAJA with specifics options in command line

The user can obtain the help to launch MAJA processing executing the
following command line:

$ /<path to maja installation dir>/maja --help

This command produces the helper lines detailed in Annex A

Additional details about the processing mode, the inputs expected by
MAJA and the outputs produced are provided in the following paragraphs.

Interfaces of MAJA chains
-------------------------

MAJA Processing modes
~~~~~~~~~~~~~~~~~~~~~

The level 2 chain implements successively several different algorithms
such as atmospheric correction, cloud and snow detection or slope and
environment correction in order to generate level 2 products. Some of
those algorithms are multi temporal, therefore the chain uses level-2
product of date D-1 (the last available level 2 product) to generate the
level 2 product of date D.

An initialization process for the first product of a time series has
been developed (*Init mode*); in this mode the product is generated with
a priori values and is just used to start a new time series. This first
level 2 product of date D is then used to generate the product of date
D+1 in *nominal mode* and so on for all the time series of level 1
products.

Figure 4: MAJA Init and Nominal modes

The level 2 product generated in init mode are usually of inferior
quality. In order to avoid the former, the *backward mode* has been
added in MAJA. This mode is used to improve the quality of the first
level 2 product of a time series. The L1 products are processed from the
youngest (date D+N) back to the oldest (date D).

In this mode the youngest L1 product (D+N) is processed in init mode.
The older L1 products of the time series (D+i) are then processed in
nominal mode using the level 2 product of date D+i+1 as input and so on
backward to the oldest ones (D). The oldest level 2 product of date D is
then used as input to reprocess all the products of the time series from
the oldest to the youngest one in *nominal mode*.

Figure 5: Backward mode (example with five products)

MAJA allows seven different processing modes:

-  Level-2 products generation:

   -  Init Mode

   -  Backward Mode

   -  Nominal Mode

-  Level-3 products generation:

-  Checktool processing:

   -  On Level-1 products

   -  On Level-2 products

   -  On Level-3 products

It is possible to activate automatically checktool processing after
Level-2 or Level-3 production to have quality indications about the
products generated. The generation of the quicklook is activated via the
GIPP “…_GIP_CKQLTL_...” (Compute_QL= true/false) that defines also the
bands that shall be included in the quicklook image; the generation of
extracts activated via the GIPP “…_GIP_CKEXTL_...”
(Compute_Extract_Points = true/false) that defines the bands and the
points to extract.

Processing Interfaces
~~~~~~~~~~~~~~~~~~~~~

The following figures show MAJA inputs and outputs for all the
processing modes.

In particular, the input products for the Level-2 processing are:

-  1 Level-1 product for the Init Mode

-  N Level-1 products for the Backward Mode

-  1 Level-1 product and 1 Level-2 product for the Nominal Mode.

Figure 6: L2 processing data interface (Init, Nominal and Backward
modes)

Figure 7: L3 processing data interface

Figure 8: L1/L2/L3 checking tools processing data interface

When MAJA is launched, all the input data shall be available in the
input directory:

-  L1, L2, L3 image product (according to the processing mode),

-  DTM data covering the tile (see section 4.6)

-  Meteo data applicable for the acquisition date of EACH L1C product
   (see section 4.6)

-  Production parameters: GIPP applicable for the acquisition date of
   EACH L1C product (see section 4.7)

   The output data generated by MAJA will all be in the specified output
   directory:

-  L2 or L3 Image product and/or quicklook and/or extracts (according to
   the processing mode)

-  Production report file (named …__PMC_…)

   Additional details on the Inputs and Outputs are provided in 4.5, 4.6
   and 4.7.

Operational Interfaces
~~~~~~~~~~~~~~~~~~~~~~

MAJA respect the Interface detailed in [AD01].

Among these interfaces, there are:

-  The « JobOrder »,

-  The « Logging »,

-  The « ExitCode »,

-  The « ProductReport ».

   The log messages are displayed in the standard output.

   The error messages are displayed in the standard error output.

   For additional details on these interfaces see § 5.

Revisit improvement via sensors mixing
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

MAJA offers the possibility of processing product time series acquired
by different sensors in order to improve the revisit over an area; the
following conditions shall be satisfied:

-  The images to be processed shall have exactly the same footprint

-  The images to be processed shall have the same pixel origin
   convention (center pixel or upper left corner),

-  The coarse resolution chosen for the PRIVATE images of the L2 product
   and for the DEM low resolution images shall be identical for the two
   sensors,

-  The bands selected for the multi-temporal processing and stored in
   the PRIVATE part of L2 product shall be the same for the two sensors
   (see section 5.2.3),

Note that the bands used for multi-temporal are indicated in GIPP L2COMM
and are used to detect the clouds, shadows, aerosols, etc. Then these
data are used for atmospheric correction of all the bands to process.

The algorithms
--------------

Processing options are defined as a function of sensor in the chain.
Depending on the spectral bands and stereoscopic capabilities of the
satellite, some methods can or cannot be applied to the times series of
the satellite. Table 1 summarizes the main options.

Table 1: algorithms and sensors

========================== ==================== ========= =================================== ======================================= =========================================== ====================
\                          **Formosat**         **Venµs** **Landsat(5/7)**                    **Sentinel2**                           **Landsat8**                                **Spot4**
                                                                                                                                                                                 
                           **(“proto” format)**           **(“proto” and “muscate” formats)** **(GPP, “muscate” and native formats)** **(“proto”, “muscate” and native formats)** **(“proto” format)**
Stereoscopic                                    X                                                                                                                                
                                                                                                                                                                                 
cloud detection                                                                                                                                                                  
Water vapour determination                      X                                             X                                                                                  
Snow detection                                            X                                   X                                       X                                           X
Cirrus flag                                     X                                                                                                                                
Cirrus mask                                                                                   X                                       X                                          
========================== ==================== ========= =================================== ======================================= =========================================== ====================

Note: “proto” is the “muscate proto” format.

The products
------------

**General information on Level-2 products**

In Level-2 products:

- Water Vapour data is expressed in g/cm2

- Atmospheric Optical Thickness is dimensionless.

The Scale factors can be found in the main HDR in “Quantification_value”
tags.

It should be noted that the LTC plan (Luts of Top Of Canopy reflectance)
are not consistent in the output L2 product. It depends on the method
used to estimate the aerosol optical thickness. If the processed method
is multi spectral, only the current date is used and the LTC set in the
composite products are not necessary. Therefore, a composite product
generated with the multi spectral method could not be reused to process
a L1 product with another method.

Some clarification should be made to the STO file stored in the private
part of the L2 product. The “STO.DBL.DIR” file contains the TOA
reflectance images after correction for absorbing atmospheric molecules
for a given spectral band (“Correl_Band_Code” parameter set in the
GIP_L2COMM file) and for a maximum number of dates
(“Number_Of_Stack_Images” parameter set also in the GIP_L2COMM). In this
file, the images are stacked as follows:

Band 1 = D (date of the current product)

   Band 2 = D+1

   Band 3 = D+2

   ...

Band 10 = D+9

In the “STO.HDR” file, the list of dates is stored in the
“List_of_Bands” tag:

   <Band sn="1">20130719</Band> => date D

   <Band sn="2">20130703</Band> => date D+1

   ...

<Band sn="10">20130905</Band> => date D+9

Therefore the current date is added at the top of the stack (band 1),
the other dates shift back and the oldest date stored in the STO file
(which is the most recent date in backward mode) is removed (for
instance the 11\ :sup:`th` date if the STO file contained yet 10 dates).

**Peculiarities on Level-2 products depending on the algorithms:**

In line with Table 1, Level-2 products have slight differences due to
slightly different algorithms applied to the input data:

-  Venµs Level-2 product is the only one providing the cloud altitude
   image because is the only sensor for which stereoscopy method in
   cloud masking can be applied;

-  Cirrus mask is provided exclusively in Landsat8 and Sentinel-2
   products

-  Snow mask is provided exclusively in Landsat 5, 7, 8 and Sentinel-2
   products

-  The quality mask indicating if water vapor mask has been estimated or
   interpolated is provided exclusively in Venµs and Sentinel-2
   products.

**Validity of the Level-2 product:**

A Level-2 product is declared **valid** (flag L2VALD) when:

-  The input Level-1 product is not too cloudy\* and is not contains too
   many ‘no_data’ pixels*\*

-  In nominal mode, the input Level-2 product is not too cloudy*,

-  The output Level-2 product computed is not contains too many
   ‘no_data’ pixels

In the backward mode, the Level-2 product is declared valid if at least
one of the Level-2 product of the series is determined as valid (the
above conditions do not apply).

**\*** A product is identified as too cloudy when the number of cloudy
pixel is upper than the “Max_Cloud_Percentage” (parameter set in the
GIP_L2COMM). Note that the ’no_data’ pixels are excluded to compute this
rate.

**\*\*** A product contains too many ‘no_data’ pixels when the number
‘no_data’ pixels is upper than the “Max_No_Data_percentage” parameters
(parameter set in the GIP_L2COMM).

Figure 9: Backward mode (example with three non-valid products)

In **backward** mode, MAJA:

1. **find the first valid L2 product (init mode)**: MAJA ingests the
   first T0+5 product, but it’s detected as non valid (conditions of
   validity are described before) and it’s excluded. The following T0+4
   product is ingested and detected as valid,

2. **processes the series (nominal mode):** produces all previous
   intermediate L2 products, each non valid product are skipped. The
   T0+3 is computed, the T0+2 is ingested but excluded (non valid) and
   the T0+1 is computed,

3. **processes the final L2 product (nominal mode):** MAJA ingests the
   T0 product but it’s detected as non valid and excluded. In this way,
   the latest final product computed in the series is the T0+1 and the
   date of this product is modified and set to T0.

Venµs Image products
~~~~~~~~~~~~~~~~~~~~

This section details the content of each Venµs product (see [RD02]).

Level 1 product content
^^^^^^^^^^^^^^^^^^^^^^^

|image2|

Table 2: Level 1 Venµs Image product

The Venµs L1 product conforms to the VENµS ICD.

Level 2 product content
^^^^^^^^^^^^^^^^^^^^^^^

|image3|

Table 3: Level 2 Venµs Image product

The scale factors of each plan (SRE, FRE, ATB, etc.) are contained in
the associated header (.HDR) (e.g. xml tags
Reflectance_Quantification_Value or AOT_Quantification_Value). In order
to optimize the size of the L2 product, some quality masks are
concatenated in a unique file in which each bit is associated to a
specific mask.

For instance, the cloud and cloud shadow mask at full resolution
contains multiple binary masks for each pixel of the image. See the (*)
in the previous Table for the description of the CLD bits.

For example, the value 19 (10011) means that the pixel is a cloudy pixel
detected by the reflectance threshold algorithm.

Level 3 product content
^^^^^^^^^^^^^^^^^^^^^^^

|image4|

Table 4: Level 3 Venµs Image product

Sentinel 2 Image products
~~~~~~~~~~~~~~~~~~~~~~~~~

The Sentinel2 L1 GPP product conforms with the GS2 ICD (see [RD03]).
This section details the content of Level 2 Sentinel 2 product.

The chain allows to mix Sentinel 2A and Sentinel 2B products in nominal
and backward modes.

|image5|

Table 5: Level 2 Sentinel 2 Image product

A TIF file is created per data (SRE, FRE, ATB, …) and per resolution (10
and 20 meters).

The scale factors are provided in the global header of the level 2
product (e.g. in the <Reflectance_Quantification_Value> tag  so BOA
reflectance = X \* 0.001).

The Landsat L5/L7 Image products
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

.. _level-1-product-content-1:

Level 1 product content
^^^^^^^^^^^^^^^^^^^^^^^

The LANDSAT L1 product conforms with the specification contained in the
document [RD04].

The chain allows to mix Landsat 5 and Landsat 7 products in nominal and
backward modes.

The directory of a LANDSAT L1 product contains an image header file and
a geoTIF file but also a subdirectory MASK that contains the mask of
saturated pixels.

|image6|

Table 6: Level 1 Landsat L5/L7 Image product

.. _level-2-product-content-1:

Level 2 product content
^^^^^^^^^^^^^^^^^^^^^^^

The structure of LANDSAT and VENUS level 2 products is nearly the same.
The differences are found in the number of spectral bands and the
resolution of images. The level 2 product does not contain angle grids.

|image7|

Table 7: Level 2 Landsat L5/L7 Image product

The Landsat L8 Image products
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

.. _level-1-product-content-2:

Level 1 product content
^^^^^^^^^^^^^^^^^^^^^^^

The LANDSAT8 L1 product conforms with the specification contained in the
document [RD04].

The directory of a LANDSAT8 L1 product contains an image header file and
a geoTIF file but also a subdirectory MASK that contains the mask of
saturated pixels.

|image8|

Table 8: Level 1 Landsat L8 Image product

.. _level-2-product-content-2:

Level 2 product content
^^^^^^^^^^^^^^^^^^^^^^^

The structure of LANDSAT8 and VENUS level 2 products is nearly the same.
The differences are found in the number of spectral bands and the
resolution of images. The cirrus mask is added to the cloud mask. The
level 2 product does not contain angle grids.

|image9|

Table 9: Level 2 Landsat L8 Image product

The Spot4 Image products
~~~~~~~~~~~~~~~~~~~~~~~~

.. _level-1-product-content-3:

Level 1 product content
^^^^^^^^^^^^^^^^^^^^^^^

The SPOT4 L1 product conforms to the specification contained in the
document [RD04].

The directory of a SPOT4 L1 product contains an image header file and a
geoTIF file but also a subdirectory MASK that contains the mask of
saturated pixels.

|image10|

Table 10: Level 1 Spot4 Image product

.. _level-2-product-content-3:

Level 2 product content
^^^^^^^^^^^^^^^^^^^^^^^

The structure of SPOT4 and VENUS level 2 products is nearly the same.
The differences are found in the number of spectral bands and the
resolution of images. The level 2 product does not contain angle grids.

|image11|

Table 11: Level 2 Spot4 Image product

The Formosat2 Image products
~~~~~~~~~~~~~~~~~~~~~~~~~~~~

.. _level-1-product-content-4:

Level 1 product content
^^^^^^^^^^^^^^^^^^^^^^^

The FORMSOAT2 L1 product is conform to the specification contained in
the document [RD04].

The directory of a FORMOSAT2 L1 product contains an image header file
and a geoTIF file but also a subdirectory MASK that contains the mask of
saturated pixels.

|image12|

Table 12: Level 1 Formosat2 Image product

.. _level-2-product-content-4:

Level 2 product content
^^^^^^^^^^^^^^^^^^^^^^^

The structure of Formosat2 and VENUS level 2 products is nearly the
same. The differences are found in the number of spectral bands and the
resolution of images. The level 2 product does not contain angle grids.

|image13|

Table 13: Level 2 Formosat2 Image product

.. _level-3-product-content-1:

Level 3 product content
^^^^^^^^^^^^^^^^^^^^^^^

|image14|

The Landsat L5/L7 “MUSCATE” Image products 
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

The LANDSAT 5/7 L1 and L2 products conform with the specifications
contained in the document [RD05].

The Landsat L8 “MUSCATE” Image products 
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

The LANDSAT 8 L1 and L2 products conform with the specifications
contained in the document [RD05].

The Landsat L8 “native” Image products 
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

.. _level-1-product-content-5:

Level 1 product content
^^^^^^^^^^^^^^^^^^^^^^^

The LANDSAT 8 L1 products conform with the specifications contained in
the document [RD07].

.. _level-2-product-content-5:

Level 2 product content
^^^^^^^^^^^^^^^^^^^^^^^

The structure of the level 2 products is described in the previous
section 4.5.4.2..

The Sentinel2 “native” Image products 
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

.. _level-1-product-content-6:

Level 1 product content
^^^^^^^^^^^^^^^^^^^^^^^

The Sentinel2 L1 products conform with the specifications contained in
the document [RD06].

.. _level-2-product-content-6:

Level 2 product content
^^^^^^^^^^^^^^^^^^^^^^^

The Level 2 Sentinel 2 native product is described in the previous
section 4.5.2.

The Sentinel2 “MUSCATE” Image products 
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

The Sentinel2 L1 and L2 products conform with the specifications
contained in the document [RD05].

The Venus “MUSCATE” Image products 
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

The Venus L1 and L2 products conform with the specifications contained
in the document [RD05].

auxiliary data of MAJA
----------------------

============================== ======================================================================================================================================================================
**File**                       **Comment**
XXX_EXO_METDTA_XXX (HDR + DBL) Archive that contains the ozone image.
                              
                               | In the MAJAUserConfig_<MISSION>.xml, if the value of the “Use_Default_Constant_Ozone_Amount” field is false, the input data EXO_METDTA is mandatory.
                               | If it’s true, the default constant ozone value used is the value set in “Atmospheric_Absorption_Correction/Ozone_Amount_Default_Value” field in the GIP_L2COMM file.
XXX_EXO_CAMS_XXX (HDR + DBL)   Archive that contains the CAMS data.
                              
                               | In the GIP_L2COMM file, if the value of the “Use_Cams_Data” field is true, the input datas EXO_CAMS will be parsed to find suitable CAMS data for the product.
                               | If it is false or not suitable for the product the CONSTANT_Model from the GIPP L2COMM File will be used instead.
XXX_AUX_REFDE2_XXX (HDR + DBL) DEM Archive that contains:
                              
                               -  The altitude image: ALT
                              
                               -  The altitude image at L2 coarse resolution: ALC
                              
                               -  The aspect image at L2 resolution: ASP
                              
                               -  The aspect image at L2 coarse resolution: ASC
                              
                               -  The slope image at L2 resolution: SLP
                              
                               -  The slope image at L2 coarse resolution: SLC
                              
                               -  The water mask: MSK
                              
                                  All these files have exactly the same footprint of the Level-1 product to process.
============================== ======================================================================================================================================================================

Please note the particular case of SENTINEL2 where the output Level 2
product contains two resolutions (R1 = 10m and R2= 20m) depending on the
spectral band. In this case, the images of the altitude (ALT), the
aspect (ASP) and the slope (SLP) are provided with the two resolutions
R1 and R2 (e.g: \_ALT_R1.tif and \_ALT_R2.tif).

By default the constant model will be used for the various LUT ( TOCR,
ALBD, DIRT, DIFT). However these luts have been computed using an
average model of atmosphere composition. In order to be more precise the
CAMS data can be used to establish the exact proportions of each model
according to the product localisation and time. To enable this you have
to turn on the Use_Cams_Data field in the L2COMM Gipp and provide:

-  At least one CAMS file (.HDR +.DBL) acquired in the range
   [Product_time – time_windows][Product_time+time_windows]. The
   time_windows is a parameter in the L2COMM GIPPP.

-  LUTS for each selected model in the Model_List node of L2COMM : TOCR,
   DIRT, DIFT and ALBD. Please note that the TOCR luts for CAMS models
   have some information added in the HDR compared to a constant lut,
   see example provided in the install folder.

The current CAMS format provided by Copernicus contains data for Dust,
Seasalt, Organic Matter, Black Carbon and Sulphate. Each model
proportion is then associated with it’s own lut to compute the final LUT
used in the processing. If no suitable CAMS are found for a given
product then the constant model will be used.The unit of the ozone
content is Kg.m\ :sup:`-2` but to be conformed with SMAC and 6S this
content is converted to cm.atm.m\ :sup:`-2`. The conversion from
kg.m\ :sup:`-2` to cm.atm.m\ :sup:`-2` is:

1 Dobson Unit (DU) is:

2.6867 x 1020 mol.m\ :sup:`-2`

4.4615 x 10-4 mol.m\ :sup:`-2`

2.1416 x 10-5 Kg[O3].m\ :sup:`-2`

1 Kg.m\ :sup:`-2` = 46694 Dobson

1 cm.atm.m\ :sup:`-2` = 1000 dobson = 1 Jacobson

1 Kg.m\ :sup:`-2` = 46.694 cm.atm.m\ :sup:`-2`

If the meteo data is not available, a default value is set in the
GIP_L2COMM file. Its unit is cm.atm.m\ :sup:`-2`.

Generally, the ozone content varies between 250 and 480 Dobson (0,25 and
0,48 cm.atm.m\ :sup:`-2`). By default the value is set to 0,3
cm.atm.m\ :sup:`-2`.The meteo data are detected in the chain with their
“EXO_METDTA” keyword. The “Mission” field set in the header (.HDR) of
the meteo data is not used.

-  In Init and Nominal modes, only one file is required in the input
   directory otherwise an error is raised. In those cases, the validity
   dates are not read by MAJA.

-  In backward mode, one meteo data should be available for each
   processed L1 product. For each L1 product, the chain looks for the
   associated meteo data and checks if the product date is included in
   the validity “start” and “stop” dates of this meteo data.

GIPPs files of MAJA
-------------------

The GIPP files used in MAJA are listed in the following table.

=============================== ====================================================================================================================================================================================================================================================================================================================
**File**                        **Comment**
**L2, L3 & Checking Tools**    
XXX_GIP_L2COMM_XXX.EEF          Contains all the L2/L3 common parameters.
**L2**                         
XXX_GIP_L2TOCR_XXX.HDR (+ .DBL) Contains the LUT of Canopy reflectance. One for the constant model in the L2COMM Gipp file and one for each CAMS model in the Model_List of L2COMM if this functionality is activated.In case of model LUT for CAMS usage some additional datas have to be put in the HDR like the extinctionCoeffs ( see examples).
XXX_GIP_L2DIRT_XXX.HDR (+ .DBL) Contains the LUT of Direct Transmission. One for the constant model in the L2COMM Gipp file and one for each CAMS model in the Model_List of L2COMM if this functionality is activated.
XXX_GIP_L2DIFT_XXX.HDR (+ .DBL) Contains the LUT of Diffuse Transmission. One for the constant model in the L2COMM Gipp file and one for each CAMS model in the Model_List of L2COMM if this functionality is activated.
XXX_GIP_L2ALBD_XXX.HDR (+ .DBL) Contains the LUT of Atmospheric Albedo. One for the constant model in the L2COMM Gipp file and one for each CAMS model in the Model_List of L2COMM if this functionality is activated.
XXX_GIP_L2WATV_XXX.HDR (+ .DBL) Contains the LUT of Water Vapor.
XXX_GIP_L2SMAC_XXX.EEF          Contains the SMAC coefficients.
XXX_GIP_L2SITE_XXX_<SITE>.EEF   Contains the site parameters.
**L3**                         
XXX_GIP_L3COMM_XXX.EEF          Contains all the L3 common parameters.
**Checking Tools**             
XXX_CKQLTL_XXX.EEF              Contains quick look generation tool parameters.
XXX_CKEXTL_XXX.EEF              Contains extract generation tool parameters.
=============================== ====================================================================================================================================================================================================================================================================================================================

Notes:

-  Parameter definitions are set as html comments in the xml file,

-  The specific checking tools parameters are precisely described (as
   html comments) in the GIPPs “CKQLT” and “CKEXTL” xml files. To
   disable (or enable) the generation of the quicklooks, set the value
   of the field “Compute_QL” to false (or true) in the CKQLTL GIP file.
   In the same way, to disable (or enable) the generation of the
   extracts points, set the value of the field “Compute_Extract_Points”
   to false (or true) in the CKEXTL file,

-  More instances examples of these files as installed in the
   “../share/examples” directory

-  Contrary to the meteo data, the “Mission » field is read in the GIPPs
   in order to detect which GIPP is associated to the processed L1
   product (because of the mixing of sensors). On the other hand, the
   validity dates are not considered

Operating manual
================

This chapter describes the operating manual of MAJA.

Memory management
-----------------

MAJA processing can be performed on the whole image or by portions of
image called “strip”.

The strip size is defined as the number of lines composing the strip and
it is a parameter of the configuration file
**MAJAUserConfig_<MISSION>.xml**.

It is then possible to raise or reduce the memory printfoot during MAJA
execution by changing this parameter.

In addition, MAJA uses the GDAL library to read and write raster images.
It’s in charge of users to adjust the GDAL configuration parameters in
relation with the host machine characteristics.

For example, to manage the memory cache used by gdal, set the
GDAL_CACHEMAX size, as follow:

$

$ export GDAL_CACHEMAX=512

$

Figure 10: Example to set the GDAL cache max size variable

Note: the memory cache size used by the MAJA application includes the
GDAL memory cache size.

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

-  Image division (strip) settings for each application of MAJA (L2INIT,
   L2NOMINAL, L2BACKWARD, L3 and CHECKTOOL),

-  L2 coarse resolution set in the “L2CoarseResolution” field

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

   <!-- L2 coarse resolution in meter -->

   <L2CoarseResolution>240</L2CoarseResolution>

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
   stereoscopic altitude (available with VENµS products):
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

==================================== ================ ====================================================================================================================================================================================================================================================================================================================================================================================================================================== ===============
**Process**                          **Category**     **General description and comment**                                                                                                                                                                                                                                                                                                                                                                                                    **Return code**
**L2Init, L2Nominal, L2Backward, L3, **« Chain »**    The error is raised at a high execution level, in the “ScientificProcessing” process (algorithmic chain processing). The error can be due to an incorrect input data (missing file in a input product for example, invalid JobOrder, etc.). Generally, this error is an user error.                                                                                                                                                    135
Checktool                                                                                                                                                                                                                                                                                                                                                                                                                                                                                   
**\ (Scientific                      **(maja)**                                                                                                                                                                                                                                                                                                                                                                                                                                             
Processing)                                                                                                                                                                                                                                                                                                                                                                                                                                                                                 
\                                    **« Business »** The raised error comes from the algorithmic chain. If such an error occurs, it is highly probable that this error is an internal one (software anomaly). In this case, the operator has to refer to the project manager.                                                                                                                                                                                                               134
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                            
                                     **(maja)**                                                                                                                                                                                                                                                                                                                                                                                                                                             
\                                    **« Data »**     The raised error comes from the data access chain layer (read/write). Such an error occurs when an input data (or a data generated during the chain processing) doesn’t comply its contents (missing or incomplete file, wrong format, etc.). In this case, the operator has to control that the data specified in the JobOrder are valid (see the product interfaces). In any case, the operator has to refer to the project manager. 133
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                            
                                     **(maja)**                                                                                                                                                                                                                                                                                                                                                                                                                                             
\                                    **« OTB »        Low layer error raised by the OTB/ITK library. In any case, the operator has to refer to the project manager to report the bug.                                                                                                                                                                                                                                                                                                        131
                                     (cots)**                                                                                                                                                                                                                                                                                                                                                                                                                                               
\                                    **« alloc »      Memory allocation error (such as *bad_alloc*). It occurs when the system failed to reserve the memory needed by the chain processing. This low layer error is raised by the stlc++ base library.                                                                                                                                                                                                                                       130
                                     (cots)**                                                                                                                                                                                                                                                                                                                                                                                                                                               
                                                      In this case, the operator has to refer to the project manager to report the bug.                                                                                                                                                                                                                                                                                                                                                     
\                                    **« std »        Low layer error (other than **bad_alloc**) raised by the stlc++ base library.                                                                                                                                                                                                                                                                                                                                                          129
                                     (cots)**                                                                                                                                                                                                                                                                                                                                                                                                                                               
                                                      In this case, the operator has to refer to the project manager to report the bug.                                                                                                                                                                                                                                                                                                                                                     
\                                    **« Unknown »    Non identified error (unlikely case). In this case, the operator has to refer to the project manager to report the bug.                                                                                                                                                                                                                                                                                                                128
                                     (cots)**                                                                                                                                                                                                                                                                                                                                                                                                                                               
==================================== ================ ====================================================================================================================================================================================================================================================================================================================================================================================================================================== ===============

The error messages are compliant with the nomenclature described in the
section [AD01].

: Example of the command line help of the maja
==============================================

For the version 3.3 of MAJA, the maja –help produces the following
helper lines

maja-processing-3.3.0 3.3 [000000006971] [I] MAJA Software Version :
3.3.0

./maja [options]

MAJA Chains

CNES All rights reserved. For more details, see Copyright.txt file.

Description:

------------

The L2 processor offers advanced atmospheric correction algorithms
including

water vapour and aerosol estimates based on multitemporal data analysis.

It also provides cloud mask generation.

- Cloud masking

\* Cloud detection

\* Shadow detection

- Atmospheric correction

\* Gaseous absorption correction

\* Scattering correction

- Environment and slope correction

\* Environment effects

\* Slope correction

- Composite image update

The L3 processor is used to merge L2 time series over short periods (~10
days)

in order to remove cloudy regions and generate useful syntheses for

downstream users

The data and GIPPs files mandatory for MAJA are:

- Common GIPPs files (for L2, L3 and Checktools processing):

\* GIP_CKEXTL

\* GIP_CKQLTL

- For L2 processing:

\* GIP_L2COMM

\* GIP_L2DIRT

\* GIP_L2DIFT

\* GIP_L2SMAC

\* GIP_L2WATV

\* GIP_L2TOCR

\* GIP_L2ALBD

\* GIP_L2SITE

\* EXO_METDTA

\* AUX_REFDE2

- For L3 processing:

\* GIP_L3COMM

Processing description:

-----------------------

The maja launches the following processes:

- launches the pre-processing treatment

\* Uncompresses all data (DBL package files and BZ2 images files)

\* Check input data with the schemas

\* Deletes all tarballs (if option is enable in the Configuration file)

\* Applies a specific stylesheet on GIPPs files

- launches the scientific-processing treatment

\* Reads image products

\* Applies algorithms

\* Formats EE and writes datas

- launches the checktool-processing treatment

\* Compute checktool on the outputs images products

- launches the post-processing treatment

\* Check output data with the schemas

\* Compress BZ2 all .TIF images data files

\* Generates the .DBL image product data (L2/L3)

For more details, report to the SUM (Software User Manual, ref.
LAIG-MU-MAC-010-CS)

Author: CS Systemes d'Information (France)

User cases:

-----------

1. First user case: Use only a JobOrder file to launch maja processing.

-> use the '--jobOrder' option.

Note: the possible values for the <Processor_Name> field are:

\* MAJA_L2_INIT_CHAIN

\* MAJA_L2_NOMINAL_CHAIN

\* MAJA_L2_BACKWARD_CHAIN

\* MAJA_L3_CHAIN

\* MAJA_L1_CHECKTOOL_CHAIN

\* MAJA_L2_CHECKTOOL_CHAIN

\* MAJA_L3_CHECKTOOL_CHAIN

2. Second user case: Use command line parameters to launch maja
processing

-> for example, use at least the '--mode' and '--input' options

Usage :
/DATA1/besquis/MAJA/centos-6.9/superbuild/install/maja/3.3.0/bin/maja-processing-3.3.0

[--help|-h] : Help

[--version|-v] : Version

[--jobOrder|-j] : Specify the JobOrder file (xml file) (1 parameter)

[--loglevel|-l] : Log level use and set to the JobOrder generated.
Possible values: 'INFO', 'PROGRESS', 'WARNING', 'DEBUG', 'ERROR'.
Default value: 'INFO' (1 parameter)

[--mode|-m] : Processing mode. Possible values: 'L2INIT', 'L2NOMINAL',
'L2BACKWARD', 'L3', 'L1CHECKTOOL', 'L2CHECKTOOL', 'L3CHECKTOOL'. Default
value: 'L2INIT' (1 parameter)

[--enableTest|-t] : Enable/Disable the field value 'Test' set in the
JobOrder generated. Possible values: 'true', 'false'. Default value:
'false' (1 parameter)

[--stylesheet|-s] : XML Stylesheet filename, used to overloads
parameters in the XML configuration files and GIPP files. See the [MU]
for an example of StyleSheet. (1 parameter)

[--admin-conf|-acs] : Administration Configuration directory (contains
for example the MAJAAdminConfigSystem.xml) (1 parameter)

[--input|-i] : Input data directory: must be contain images, all GIPPs
files, the DTM, etc.). The directory must be contain only one L1 product
for the 'L2INIT' mode, a list of L1 products for the 'L2BACKWARD' mode,
one L1 product and one L2 product for the 'L2NOMINAL' mode and a list of
L2 products for the L3 mode. Default value: '.' (1 parameter)

[--output|-o] : Output data directory (product directory). Default
value: '.' (1 parameter)

[--working-dir|-w] : Working directory (working 'temporary' directory).
Default value: the '--output parameter value' (1 parameter)

[--conf|-ucs] : User Configuration directory (contains for example
MAJAUserConfigSystem.xml) (1 parameter)

[--sensingTime|-s] : Start and Stop Sensing Time (two values). Mandatory
for 'L3' mode, not used for the others modes. Note: Time stamp in the
format YYYYMMDD_HHMMSSuuuuuu.

ex: --sensingTime 20100824_105015000000 20100824_123030000000 (2
parameters)

[--NbThreads] : UserConfigSystem overloads value for the parameter
'NbThreads' (1 parameter)

[--CheckXMLFilesWithSchema] : UserConfigSystem overloads value for the
parameter 'CheckXMLFilesWithSchema' (1 parameter)

[--CleanInputZipFiles] : UserConfigSystem overloads value for the
parameter 'CleanInputZipFiles' (1 parameter)

[--CleanFiles] : UserConfigSystem overloads value for the parameter
'CleanFiles' (1 parameter)

[--ZipFiles] : UserConfigSystem overloads value for the parameter
'ZipFiles' (1 parameter)

[--EnableCleaningCachingDirectoryBeforeProcessing] : UserConfigSystem
overloads value for the parameter
'EnableCleaningCachingDirectoryBeforeProcessing' (1 parameter)

[--EnableCleaningCachingDirectoryAfterProcessing] : UserConfigSystem
overloads value for the parameter
'EnableCleaningCachingDirectoryAfterProcessing' (1 parameter)

[--EnableCleaningTemporaryDirectory] : UserConfigSystem overloads value
for the parameter 'EnableCleaningTemporaryDirectory' (1 parameter)

[--TileId] : Set the Tile id of the Sentinel2 L1 product (Only necessary
for SENTINEL2 plug-in and only for the L1C product with PSD version <
PSD 14) (1 parameter)

[--perfos-log] : Enable performance measurement in log

[--perfos-report] : Enable synthesize performance measurement in log and
xml file

Example of Stylesheet
=====================

This is an example of a stylesheet. In this example, the ‘NbThreads’ and
‘AOT_Method’ parameters are overloaded with a new value:

<?xml version="1.0" encoding="UTF-8"?>

<xsl:stylesheet version="1.0"
xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance">

<xsl:output method="xml" version="1.0" encoding="UTF-8" indent="yes"
omit-xml-declaration="no" />

<xsl:template match="Refining_Scenario">

<Refining_Scenario
xmlns="http://eop-cfi.esa.int/CFI">NONE</Refining_Scenario>

</xsl:template>

<xsl:template match="*">

<xsl:choose>

<xsl:when test="local-name(.)=\ **'AOT_Method'**">

<xsl:copy>

<xsl:copy-of select="@*" />

<xsl:value-of select="**'SPECTROTEMPORAL'**" />

</xsl:copy>

</xsl:when>

<xsl:when test="local-name(.)=\ **'NbThreads'**">

<xsl:copy>

<xsl:copy-of select="@*" />

<xsl:value-of select="**'8'**" />

</xsl:copy>

</xsl:when>

<xsl:otherwise>

<xsl:copy>

<xsl:for-each select="@*">

<xsl:copy><xsl:value-of select="."/></xsl:copy>

</xsl:for-each>

<xsl:apply-templates select="node()"/>

</xsl:copy>

</xsl:otherwise>

</xsl:choose>

</xsl:template>

<!-- Copy text, comments and PIs (xml-stylesheet, etc.) -->

<xsl:template match="comment() \| processing-instruction() \| text()">

<xsl:copy>

<xsl:apply-templates />

</xsl:copy>

</xsl:template>

</xsl:stylesheet>

Note: set the stylesheet file with the --stylesheet command line option.

: Example of maja job order file
================================

<?xml version="1.0" encoding="ISO-8859-1"?>

<Ipf_Job_Order xmlns:a="http://www.acsys.it/schemas/IPF"
xmlns:xsi="http://www.w3.org/2000/10/XMLSchema-instance"
xsi:schemaLocation="http://www.acsys.it/schemas/VenusJobOrder.xsd">

<Ipf_Conf>

<Processor_Name>MAJA_L2_INIT_CHAIN</Processor_Name>

<Version>03.03</Version>

<Order_Type/>

<Production_Request_Id/>

<Stdout_Log_Level>INFO</Stdout_Log_Level>

<Stderr_Log_Level>INFO</Stderr_Log_Level>

<Test>false</Test>

<Breakpoint_Enable>true</Breakpoint_Enable>

<Acquisition_Station/>

<Processing_Station/>

<Config_Files/>

<Sensing_Time>

<Start>20000101_000000000000</Start>

<Stop>20000102_000000000000</Stop>

</Sensing_Time>

<List_of_GIPP_Files/>

</List_of_GIPP_Files>

</Ipf_Conf>

<List_of_Ipf_Procs>

<Ipf_Proc>

<Task_Name>L2_INIT_PROCESSING</Task_Name>

<Task_Version>03.03</Task_Version>

<Breakpoint>

<List_of_Brk_Files count="0"/>

</Breakpoint>

<List_of_Inputs count="13">

<Input>

<File_Type>FSC_L1VALD</File_Type>

<File_Name_Type>Physical</File_Name_Type>

<List_of_File_Names count="1">

<File_Name>./Sudouest_20060317_MS_fmsat_ortho_toa.hdr</File_Name>

</List_of_File_Names>

<List_of_Time_Intervals count="1">

<Time_Interval>

<Start/>

<Stop/>

<File_Name/>

</Time_Interval>

</List_of_Time_Intervals>

</Input>

<Input>

<File_Type>GIP_CKEXTL</File_Type>

<File_Name_Type>Physical</File_Name_Type>

<List_of_File_Names count="1">

<File_Name>./VE_TEST_GIP_CKEXTL_S_SUDOUEST_00001_00000000_99999999.EEF</File_Name>

</List_of_File_Names>

<List_of_Time_Intervals count="1">

<Time_Interval>

<Start/>

<Stop/>

<File_Name/>

</Time_Interval>

</List_of_Time_Intervals>

</Input>

<Input>

<File_Type>GIP_CKQLTL</File_Type>

<File_Name_Type>Physical</File_Name_Type>

<List_of_File_Names count="1">

<File_Name>./VE_TEST_GIP_CKQLTL_S_SUDOUEST_00001_00000000_99999999.EEF</File_Name>

</List_of_File_Names>

<List_of_Time_Intervals count="1">

<Time_Interval>

<Start/>

<Stop/>

<File_Name/>

</Time_Interval>

</List_of_Time_Intervals>

</Input>

<Input>

<File_Type>GIP_L2COMM</File_Type>

<File_Name_Type>Physical</File_Name_Type>

<List_of_File_Names count="1">

<File_Name>./VE_TEST_GIP_L2COMM_L_ALLSITES_00001_00000000_99999999.EEF</File_Name>

</List_of_File_Names>

<List_of_Time_Intervals count="1">

<Time_Interval>

<Start/>

<Stop/>

<File_Name/>

</Time_Interval>

</List_of_Time_Intervals>

</Input>

<Input>

<File_Type>GIP_L2DIRT</File_Type>

<File_Name_Type>Physical</File_Name_Type>

<List_of_File_Names count="1">

<File_Name>./F2_TEST_GIP_L2DIRT_L_CONTINEN_00001_00000000_99999999.DBL</File_Name>

</List_of_File_Names>

<List_of_Time_Intervals count="1">

<Time_Interval>

<Start/>

<Stop/>

<File_Name/>

</Time_Interval>

</List_of_Time_Intervals>

</Input>

<Input>

<File_Type>GIP_L2DIFT</File_Type>

<File_Name_Type>Physical</File_Name_Type>

<List_of_File_Names count="1">

<File_Name>./F2_TEST_GIP_L2DIFT_L_CONTINEN_00001_00000000_99999999.DBL</File_Name>

</List_of_File_Names>

<List_of_Time_Intervals count="1">

<Time_Interval>

<Start/>

<Stop/>

<File_Name/>

</Time_Interval>

</List_of_Time_Intervals>

</Input>

<Input>

<File_Type>GIP_L2SMAC</File_Type>

<File_Name_Type>Physical</File_Name_Type>

<List_of_File_Names count="1">

<File_Name>./F2_TEST_GIP_L2SMAC_L_ALLSITES_00001_00000000_99999999.EEF</File_Name>

</List_of_File_Names>

<List_of_Time_Intervals count="1">

<Time_Interval>

<Start/>

<Stop/>

<File_Name/>

</Time_Interval>

</List_of_Time_Intervals>

</Input>

<Input>

<File_Type>GIP_L2TOCR</File_Type>

<File_Name_Type>Physical</File_Name_Type>

<List_of_File_Names count="1">

<File_Name>./F2_TEST_GIP_L2TOCR_L_CONTINEN_00001_00000000_99999999.DBL</File_Name>

</List_of_File_Names>

<List_of_Time_Intervals count="1">

<Time_Interval>

<Start/>

<Stop/>

<File_Name/>

</Time_Interval>

</List_of_Time_Intervals>

</Input>

<Input>

<File_Type>GIP_L2ALBD</File_Type>

<File_Name_Type>Physical</File_Name_Type>

<List_of_File_Names count="1">

<File_Name>./F2_TEST_GIP_L2ALBD_L_CONTINEN_00001_00000000_99999999.DBL</File_Name>

</List_of_File_Names>

<List_of_Time_Intervals count="1">

<Time_Interval>

<Start/>

<Stop/>

<File_Name/>

</Time_Interval>

</List_of_Time_Intervals>

</Input>

<Input>

<File_Type>GIP_L2SITE</File_Type>

<File_Name_Type>Physical</File_Name_Type>

<List_of_File_Names count="1">

<File_Name>./VE_TEST_GIP_L2SITE_S_SUDOUEST_00001_00000000_99999999.EEF</File_Name>

</List_of_File_Names>

<List_of_Time_Intervals count="1">

<Time_Interval>

<Start/>

<Stop/>

<File_Name/>

</Time_Interval>

</List_of_Time_Intervals>

</Input>

<Input>

<File_Type>EXO_METDTA</File_Type>

<File_Name_Type>Physical</File_Name_Type>

<List_of_File_Names count="1">

<File_Name>./VE_TEST_EXO_METDTA_20110101T000000_20111231T230000.DBL</File_Name>

</List_of_File_Names>

<List_of_Time_Intervals count="1">

<Time_Interval>

<Start/>

<Stop/>

<File_Name/>

</Time_Interval>

</List_of_Time_Intervals>

</Input>

<Input>

<File_Type>AUX_REFDE2</File_Type>

<File_Name_Type>Physical</File_Name_Type>

<List_of_File_Names count="1">

<File_Name>./VE_TEST_AUX_REFDE2_SUDOUEST_0001.DBL</File_Name>

</List_of_File_Names>

<List_of_Time_Intervals count="1">

<Time_Interval>

<Start/>

<Stop/>

<File_Name/>

</Time_Interval>

</List_of_Time_Intervals>

</Input>

<Input>

<File_Type>GIP_L2WATV</File_Type>

<File_Name_Type>Physical</File_Name_Type>

<List_of_File_Names count="1">

<File_Name>./F2_TEST_GIP_L2WATV_L_CONTINEN_00001_00000000_99999999.DBL</File_Name>

</List_of_File_Names>

<List_of_Time_Intervals count="1">

<Time_Interval>

<Start/>

<Stop/>

<File_Name/>

</Time_Interval>

</List_of_Time_Intervals>

</Input>

</List_of_Inputs>

<List_of_Outputs count="5">

<Output>

<File_Type>FSC_L2VALD</File_Type>

<File_Name_Type>Directory</File_Name_Type>

<File_Name>./output/</File_Name>

</Output>

<Output>

<File_Type>FSC_L2NOTV</File_Type>

<File_Name_Type>Directory</File_Name_Type>

<File_Name>./output/</File_Name>

</Output>

<Output>

<File_Type>PMC_L2REPT</File_Type>

<File_Name_Type>Directory</File_Name_Type>

<File_Name>./output/</File_Name>

</Output>

<Output>

<File_Type>QCK_L2VALD</File_Type>

<File_Name_Type>Directory</File_Name_Type>

<File_Name>./output/</File_Name>

</Output>

<Output>

<File_Type>QCK_L2NOTV</File_Type>

<File_Name_Type>Directory</File_Name_Type>

<File_Name>./output/</File_Name>

</Output>

</List_of_Outputs>

</Ipf_Proc>

</List_of_Ipf_Procs>

<Processor_Conf>

<File_Name/>

</Processor_Conf>

<Parameter_File>

<File_Name/>

</Parameter_File>

<List_of_Attachments/>

</Ipf_Job_Order>

.. |image0| image:: ./media/image1.jpg
   :width: 0.82639in
   :height: 0.80903in
.. |Capture-1| image:: ./media/image4.jpg
   :width: 3.34792in
   :height: 2.83472in
.. |image2| image:: ./media/image12.emf
   :width: 7.26111in
   :height: 2.4in
.. |image3| image:: ./media/image13.emf
   :width: 7.07847in
   :height: 6.89583in
.. |image4| image:: ./media/image14.emf
   :width: 7.07847in
   :height: 2.05208in
.. |image5| image:: ./media/image15.emf
   :width: 6.91319in
   :height: 8.76528in
.. |image6| image:: ./media/image16.emf
   :width: 6.47847in
   :height: 1.52153in
.. |image7| image:: ./media/image17.emf
   :width: 7.07847in
   :height: 6.44375in
.. |image8| image:: ./media/image18.emf
   :width: 6.89583in
   :height: 0.91319in
.. |image9| image:: ./media/image19.emf
   :width: 7.07847in
   :height: 6.59097in
.. |image10| image:: ./media/image20.emf
   :width: 7.01736in
   :height: 0.93056in
.. |image11| image:: ./media/image21.emf
   :width: 7.07847in
   :height: 6.44375in
.. |image12| image:: ./media/image22.emf
   :width: 7.01736in
   :height: 0.93056in
.. |image13| image:: ./media/image23.emf
   :width: 7.07847in
   :height: 6.30417in
.. |image14| image:: ./media/image24.emf
   :width: 7.07847in
   :height: 2.05208in
