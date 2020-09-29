apTvAOTEstimationMultiSpectral
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Objectif
********
Validation de l'application "AOTEstimation" en mode init Multi-Spectral

Description
***********

Le test "AOTEstimationMultiSpectral" calcul la valeur de l'AOT en mode init Multi-Spectral. Le module AOTEstimation génère une image contenant pour chaque pixel, l'épaisseur optique des aérosols estimée.


Liste des données d’entrées
***************************

- toac AOTEstimation/MultiSpectral/L930662_20020325_L7_197_030_USGS_toa_240m.tif
- shd AOTEstimation/MultiSpectral/L930662_20020325_L7_197_030_USGS_toa_240m.shd
- dtm DTM/LANDSAT/VE_TEST_AUX_REFDE2_L930662__0001.DBL.DIR/L930662_240m.mnt_ALC
- cld AOTEstimation/MultiSpectral/L930662_20020325_L7_197_030_USGS_toa_240m.nua
- edg AOTEstimation/MultiSpectral/L930662_20020325_L7_197_030_USGS_toa_bord_240m
- sat AOTEstimation/MultiSpectral/L930662_20020325_L7_197_030_USGS_toa_240m.sat
- was AOTEstimation/MultiSpectral/L930662_20020325_L7_197_030_USGS_toa_240m.water
- snow AOTEstimation/MultiSpectral/L930662_20020325_L7_197_030_USGS_toa_240m.nei
- lutheader AOTEstimation/MultiSpectral/giplut.xml
- lutimage AOTEstimation/MultiSpectral/lutAOTMS.mha
- mode multispectral
- xmlparamsfile AOTEstimation/MultiSpectral/AOTParams_LANDSAT.xml
- reall2nodata 0.
- darkestaot 0.13
- jday 6617
- init true
         
         

Liste des produits de sortie
****************************

- aotmask TVA/apTvAOTEstimationMultiSpectral_aotmask.tif
- aotcomputed TVA/apTvAOTEstimationMultiSpectral_aotcomputed.tif


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
Le test génère en sortie l'image des valeurs de l'AOT ainsi que le masque des valeurs calculées ou non.

Mise en oeuvre du test
**********************

Ce test est exécuté en lançant la commande :
ctest -R apTvAOTEstimationMultiSpectral

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
