apTvCloudReflectance
~~~~~~~~~~~~~~~~~~~~

Objectif
********
Validation de l'application "CirrusReflectance"

Description
***********

Le module "CirrusReflectance" calcul le masque de reflectance de nuages.


Liste des données d’entrées
***************************

- tocr CloudMaskingTest/Sudouest_20060326_MS_fmsat_ortho_tocr.tif
- edg CloudMaskingTest/Sudouest_20060326_MS_fmsat_ortho_edg.tif
- sat CloudMaskingTest/Sudouest_20060326_MS_fmsat_ortho_sat.tif
- initmode true
- foregroundvalue 1
- bluebandtocr 0
- redbandtocr 2
- nirbandtocr 3
- reflectancewaterband 0
- redreflectancethresholdvalue 0.149999999999999994448884876874
- bluereflectancethresholdvalue 0.25
- nirredratiomaxthresholdvalue 2
- nirredratiominthresholdvalue 0.800000000000000044408920985006
- nodata -10
- correlbandtocr 0
- waterthreshold 1
- swir.bandtocr 0
- swir.nirswirratiomin 1


Liste des produits de sortie
****************************

- refl TVA/apTv31TCJ_20150904_S2A_L1Csimu_cloud_reflectance.tif


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
Le test génère en sortie le masque reflectance du produit.

Mise en oeuvre du test
**********************
Ce test est exécuté en lançant la commande :
ctest -R apTvCloudReflectance

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

