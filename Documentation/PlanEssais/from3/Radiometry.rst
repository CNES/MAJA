Radiometry
==========

Remote sensing is not just a matter of taking pictures, but also –
mostly – a matter of measuring physical values. In order to properly
deal with physical magnitudes, the numerical values provided by the
sensors have to be calibrated. After that, several indices with physical
meaning can be computed.

Calibration functionnalities (absolute and relative) and even
atmospheric correction routines will be available in future versions of
OTB. Please note that the 6S Radiative Transfer Code [1]_ is already
included in the OTB source code and compiles out of the box. Calibration
and atmospheric corrections in OTB will be based on it.

In the current version of OTB, several vegetation indices are already
available. They are presented in this chapter.

Vegetation Index
----------------

[sec:VegetationIndex]

Introduction
~~~~~~~~~~~~

A vegetation index is a quantitative measure used to measure biomass or
vegetative vigor, usually formed from combinations of several spectral
bands, whose values are added, divided, or multiplied in order to yield
a single value that indicates the amount or vigor of vegetation.

NDVI
~~~~

[secNDVI] NDVI was one of the most successful of many attempts to simply
and quickly identify vegetated areas and their *condition*, and it
remains the most well-known and used index to detect live green plant
canopies in multispectral remote sensing data. Once the feasibility to
detect vegetation had been demonstrated, users tended to also use the
NDVI to quantify the photosynthetic capacity of plant canopies. This,
however, can be a rather more complex undertaking if not done properly.

ARVI
~~~~

[secARVI]

AVI
~~~

[secAVI]

Atmospheric Corrections
-----------------------

[secAtmosphericCorrections]

.. [1]
   http://6s.ltdri.org/
