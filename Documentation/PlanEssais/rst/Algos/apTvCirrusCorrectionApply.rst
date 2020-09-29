apTvCirrusCorrectionApply
~~~~~~~~~~~~~~~~~~~~~~~~~~

Objectif
********
Validation de l'application "CirrusCorrection"

Description
***********

Le test "CirrusCorrection" applique la correction Cirrus à une image.


Liste des données d’entrées
***************************

- l2toa CirrusCorrectionTest/11SLT_20150803_S2A_L1Csimu_toa_abs_coeff_001_10m.tif
- l2edg CirrusCorrectionTest/11SLT_20150803_S2A_L1Csimu_bord_10m.bord_final
- l2dtm DTM/SENTINEL2/11SLT/11SLT_10m_roi.tif
- l2cirrus CirrusCorrectionTest/11SLT_20150803_S2A_L1Csimu_toa_60m.B10_resampled_coef_001.tif
- cirrusmask apTv11SLT_20150803_mask_cirrus_10m.tif
- resolution 10
- maxdistance 1000
- thresholdfactor 0.95
- thresholdoffset 0.015
- thresholdgain 0.00002
- gammamean 1.055496216
- swirgammafactor 2
- maxreflectance 0.07
- bandcodelist B2 B3 B4 B8
- swirbandcodelist B11 B12


Liste des produits de sortie
****************************

- correctedtoa TVA/apTv11SLT_20150803_corrected_TOA_10m.tif       


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
Le test génère en sortie l'image corrigée.

Mise en oeuvre du test
**********************

Ce test est exécuté en lançant la commande :
ctest -R apTvCirrusCorrectionApply

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
ST-169-CirrusCorr-0010 (C) ;
