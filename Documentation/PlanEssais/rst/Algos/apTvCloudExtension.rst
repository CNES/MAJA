apTvCloudReflectance
~~~~~~~~~~~~~~~~~~~~

Objectif
********
Validation de l'application "CloudExtension"

Description
***********

Le module "CloudExtension" calcul le masque d'extension des nuages.


Liste des données d’entrées
***************************

- tocr CloudMaskingTest/Lacrau_20060813_MS_fmsat_ortho_tocr.tif
- edg CloudMaskingTest/Lacrau_20060813_MS_fmsat_ortho_edg.tif
- cldext apTvReflectanceVariation_ReflExt.tif
- cldrefl apTvReflectanceVariation_Refl.tif
- cldreflvar apTvReflectanceVariation_ReflVar.tif
- l2rcr CloudMaskingTest/Lacrau_20060813_MS_fmsat_ortho_l2rcr.tif
- l2sto CloudMaskingTest/Lacrau_20060813_MS_fmsat_ortho_l2sto.tif
- l2ndt CloudMaskingTest/Lacrau_20060813_MS_fmsat_ortho_l2ndt.tif
- nodata -10
- correlbandtocr 0
- correlbandrcr 0
- jday 8260
- minpixelcorrelation 15
- correlthreshold 0.900000000000000022204460492503
- stolistofdates 0 0 0 0 0 0 0 0 0 0
- ncorrel 3 3
- discardedratiomin 0.2


Liste des produits de sortie
****************************

- ext TVA/apTvCloudExtension.tif


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
Le test génère en sortie le masque extension du produit.

Mise en oeuvre du test
**********************
Ce test est exécuté en lançant la commande :
ctest -R apTvCloudExtension

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
ST-169-CloudMasking-0300 (C) ;ST-169-CloudMasking-0010 (C) ; ST-169-CloudMasking-0100 (C) ; ST-169-CloudMasking-0200(C) ; ST-169-CloudMasking-0500 (C) ; ST-169-CloudMasking-0600 (C) ; ST-169-CloudMasking-0700 (C) ; ST-169-CloudMasking-1000 (C) ; ST-169-CloudMasking-1600 (C) ; ST-169-CloudMasking-1700 (C) ; ST-169-CloudMasking-0300 (C) ; ST-169-CloudMasking-0400 (C) ;ST-169-CloudMasking-0410 (C) ; ST-169-CloudMasking-0420 (C) ; ST-169-CloudMasking-0430(C) ; ST-169-CloudMasking-1100 (C) ; ST-169-CloudMasking-1110 (C) ; ST-169-CloudMasking-1120 (C) ; ST-169-CloudMasking-1130 (C) ; ST-169-CloudMasking-1140 (C) ; ST-169-CloudMasking-1150 (C) ; ST-169-CloudMasking-1160 (C) ; ST-169-CloudMasking-1170 (C) ;ST-169-CloudMasking-1180 (C) ; ST-169-CloudMasking-1200 (C) ; ST-169-CloudMasking-1300(C) ; ST-169-CloudMasking-1400 (C) ; ST-169-CloudMasking-1500 (C) ; ST-169-CloudMasking-1510 (C) ; ST-169-CloudMasking-1520 (C) ;

