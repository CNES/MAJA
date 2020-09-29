apTvAOTMean_SpectroTemporalNominal
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Objectif
********
Validation de l'application "AOTEstimation" en mode nominal Spectro-Temporel

Description
***********

Le test "AOTMean_SpectroTemporalNominal" calcul la valeur de l'AOT moyen en mode nominal Spectro-Temporel.


Liste des données d’entrées
***************************

- toac AOTEstimation/SpectroTemporal/L930662_20020212_L7_198_030_USGS_toac_240m.tif
- shd AOTEstimation/SpectroTemporal/L930662_20020212_L7_198_030_USGS_shd_240m.tif
- dtm DTM/LANDSAT/LN_TEST_AUX_REFDE2_EU93066200A00B_0001.DBL.DIR/LN_TEST_AUX_REFDE2_EU93066200A00B_0001_ALC.tif
- cld AOTEstimation/SpectroTemporal/L930662_20020212_L7_198_030_USGS_l2cld_240m.tif
- l2ndt AOTEstimation/SpectroTemporal/L930662_20020212_L7_198_030_USGS_l2ndt_240m.tif
- l2rtc AOTEstimation/SpectroTemporal/L930662_20020212_L7_198_030_USGS_l2rtc_240m.tif
- edg AOTEstimation/SpectroTemporal/L930662_20020212_L7_198_030_USGS_edg_240m.tif
- lutheader  AOTEstimation/SpectroTemporal/giplut.xml
- lutimage  AOTEstimation/SpectroTemporal/lutAOT.mha
- mode spectrotemporal
- xmlparamsfile AOTEstimation/SpectroTemporal/AOTParams_LANDSAT.xml
- temporal.altitudemean 759.934899999999970532371662557
- reall2nodata -10
- jday 6617
- darkestaot 0.11594
- temporal.lutmap AOTEstimation/SpectroTemporal/giplutmap.xml
- sat AOTEstimation/SpectroTemporal/L930662_20020212_L7_198_030_USGS_sat_240m.tif
- l2rta AOTEstimation/SpectroTemporal/L930662_20020212_L7_198_030_USGS_l2rta_240m.tif
- l2pxd AOTEstimation/SpectroTemporal/L930662_20020212_L7_198_030_USGS_l2pxd_240m.tif
- was AOTEstimation/SpectroTemporal/L930662_20020212_L7_198_030_USGS_was_240m.tif
- snow AOTEstimation/SpectroTemporal/L930662_20020212_L7_198_030_USGS_snow_240m.tif




Liste des produits de sortie
****************************

- Valeurs de l'AOT moyen


Prérequis
*********
Il n’y a pas de prérequis.

Durée attendue
***************
La durée d’exécution de l’essai n’est pas un critère attendu.

Epsilon utilisé sur la non regression
*************************************
0

Vérifications à effectuer
**************************
Le test calcul la valeurs moyenne de l'AOT sur l'image.

Mise en oeuvre du test
**********************

Ce test est exécuté en lançant la commande :
ctest -R apTvAOTMean_SpectroTemporalNominal

Journal d’essai de la recette
*****************************

Notes sur le déroulement du test
--------------------------------
Rien de particulier n’a été noté lors du déroulement du test.

Conclusion du déroulement du test
---------------------------------
RAS

Validation du test
------------------

================== =================
Date de validation    Résultat
26/11/2010              OK
================== =================

Exigences
*********
Ce test couvre les exigences suivantes :
ST-169-AOT-0010 (C) ; ST-169-AOT-0100 (C) ; ST-169-AOT-0200 (C) ; ST-169-AOT-0300 (C) ;
ST-169-AOT-0500 (C) ; ST-169-AOT-0600 (C) ;
