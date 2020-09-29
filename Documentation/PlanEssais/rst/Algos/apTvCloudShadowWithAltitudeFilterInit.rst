apTvCloudShadowWithAltitudeFilterInit
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Objectif
********
Validation de l'application "CloudShadowWithAltitudeFilterInit"

Description
***********

Le module "CloudShadowWithAltitudeFilterInit" calcul le masque d'ombre des nuages.


Liste des données d’entrées
***************************

 - tocr CloudMaskingTest/Sudouest_20060326_MS_fmsat_ortho_tocr.tif
 - edg CloudMaskingTest/Sudouest_20060326_MS_fmsat_ortho_edg.tif
 - cldall  CloudMaskingTest/Sudouest_20060326_MS_fmsat_ortho_cldall.tif
 - cla  CloudMaskingTest/Sudouest_20060326_MS_fmsat_ortho_cla.tif
 - vie  CloudMaskingTest/Sudouest_20060326_MS_fmsat_ortho_vie.tif
 - dtm DTM/FORMOSAT/VE_TEST_AUX_REFDE2_SUDOUEST_0001.DBL.DIR/Sudouest_96m.mnt_ALC
 - initmode true
 - sol1.in  CloudMaskingTest/Sudouest_20060326_MS_fmsat_ortho_sol1.tif
 - sol1.h 2000
 - solhref 1000
 - defaultalt 500
 - ksigma 3
 - l2coarseres 96
 - deltahmax 8000
 - deltahmin 0
 - deltahstep 500
 - shadbandtocr 0
 - shadbandrcr 0
 - viehref 3000
 - nodata -10
 - algo.withalt.refinement 1
 - algo.withalt.absnbpixthresh 20
 - algo.withalt.threshdiffim 10
 - algo.withalt.sol2.h 0
 - algo.withoutalt.cloudcovermult 2
 - algo.withoutalt.shadratiomax 0
 - algo.withoutalt.shadratiomin 0
 - algo.withoutalt.shadratiopixnb 0
 - algo.withoutalt.maxdarkening 0
 - algo.withoutalt.minpercentile 0
 - algo.withoutalt.seradius 0


Liste des produits de sortie
****************************

 - shadow TVA/apTvSudouest_20060326_MS_fmsat_ortho_cloudShadowWithAltitude_init_shadow.tif
 - l2cla TVA/apTvSudouest_20060326_MS_fmsat_ortho_cloudShadowWithAltitude_init_l2cla.tif


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
Le test génère en sortie le masque d'ombre des nuages.

Mise en oeuvre du test
**********************
Ce test est exécuté en lançant la commande :
ctest -R apTvCloudShadowWithAltitudeFilterInit

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
ST-169-CloudMasking-0010 (C) ; ST-169-CloudMasking-0100 (C) ; ST-169-CloudMasking-0200(C) ; ST-169-CloudMasking-0500 (C) ; ST-169-CloudMasking-0600 (C) ; ST-169-CloudMasking-0700 (C) ; ST-169-CloudMasking-1000 (C) ; ST-169-CloudMasking-1600 (C) ; ST-169-CloudMasking-1700 (C) ; ST-169-CloudMasking-0300 (C) ; ST-169-CloudMasking-0400 (C) ;ST-169-CloudMasking-0410 (C) ; ST-169-CloudMasking-0420 (C) ; ST-169-CloudMasking-0430(C) ; ST-169-CloudMasking-1100 (C) ; ST-169-CloudMasking-1110 (C) ; ST-169-CloudMasking-1120 (C) ; ST-169-CloudMasking-1130 (C) ; ST-169-CloudMasking-1140 (C) ; ST-169-CloudMasking-1150 (C) ; ST-169-CloudMasking-1160 (C) ; ST-169-CloudMasking-1170 (C) ;ST-169-CloudMasking-1180 (C) ; ST-169-CloudMasking-1200 (C) ; ST-169-CloudMasking-1300(C) ; ST-169-CloudMasking-1400 (C) ; ST-169-CloudMasking-1500 (C) ; ST-169-CloudMasking-1510 (C) ; ST-169-CloudMasking-1520 (C) ;

