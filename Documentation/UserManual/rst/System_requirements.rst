SYSTEM REQUIREMENTS
===================

Host platform
-------------

MAJA software shall be installed, used and operated on a Linux platform;
MAJA is available for all 5.x, 6.x and 7.x versions of the OS.
Performance tests (cf. [RD02]_  ) suggest to use a platform with at least 4
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

-  Caching data directory volume: ~100 GBytes

   The caching directory is named “.maja-temporary-directory” and it is
   created in the output directory; by default, it is automatically
   deleted at the end of each successful processing.

   Cleanup options are configurable via dedicated parameters in the
   “MAJAUserConfigSystem.xml” configuration file (by default value is
   “enable”).

   For investigation purposes, it is then possible to disable this
   cleanup and keep all the intermediary files (see :ref:`operating_manual:user configuration files` ).
   


