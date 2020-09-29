apTvCloudShadowWithoutAltitudeFilterNominal
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Objectif
********
Validation de l'application "CloudShadowWithoutAltitudeFilterNominal"

Description
***********

Le module "CloudShadowWithoutAltitudeFilterNominal" calcul le masque d'ombre des nuages.


Liste des données d’entrées
***************************

- tocr CloudMaskingTest/Lacrau_20060813_MS_fmsat_ortho_tocr.tif
- edg CloudMaskingTest/Lacrau_20060813_MS_fmsat_ortho_edg.tif
- cldall CloudMaskingTest/Lacrau_20060813_MS_fmsat_ortho_cldall.tif
- cla CloudMaskingTest/Lacrau_20060813_MS_fmsat_ortho_cla.tif
- vie CloudMaskingTest/Lacrau_20060813_MS_fmsat_ortho_vie.tif
- dtm DTM/FORMOSAT/Lacrau_96m.mnt
- initmode 0
- l2rcr CloudMaskingTest/Lacrau_20060813_MS_fmsat_ortho_rcr.tif
- l2was CloudMaskingTest/Lacrau_20060813_MS_fmsat_ortho_was.tif
- l2ndt CloudMaskingTest/Lacrau_20060813_MS_fmsat_ortho_ndt.tif
- sol1.in CloudMaskingTest/Lacrau_20060813_MS_fmsat_ortho_sol1.tif
- sol1.h 3000
- solhref 4000
- defaultalt 500
- ksigma 0
- l2coarseres 96
- deltahmax 8000
- deltahmin 0
- deltahstep 250
- shadbandtocr 2
- shadbandrcr 2
- viehref 3000
- nodata -10
- algo withoutalt
- algo.withoutalt.cloudcovermult 2
- algo.withoutalt.shadratiomax 1
- algo.withoutalt.shadratiomin 0.5
- algo.withoutalt.shadratiopixnb 10
- algo.withoutalt.maxdarkening 0.800000000000000044408920985006
- algo.withoutalt.minpercentile 0.100000000000000005551115123126
- algo.withoutalt.seradius 2



Liste des produits de sortie
****************************

- shadow  TVA/apTvLacrau_20060813_MS_fmsat_ortho_cloudShadowWithoutAltitude_nominal_shadow.tif

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
ctest -R apTvCloudShadowWithoutAltitudeFilterNominal

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

