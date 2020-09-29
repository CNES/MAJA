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

================================== =====================================================================================================================
.. image:: ./Art/install_tree.png  **/<install_dir>/maja**: MAJA base directory
           
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
           		            
           
			            **/schemas**: contains all the ICD schemas used by the chains (EarthExplorer, Venus, Maja, Senitnel2, etc…)
           
			            /MAJA_VERSIONS_FOR_SW_COMPONENT: show the version of MAJA
================================== =====================================================================================================================
