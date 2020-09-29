apTvGammaCompute
~~~~~~~~~~~~~~~~

Objectif
********
Validation de l'application "GammaCompute"

Description
***********

Le test "GammaCompute" calcul le gamma d'une liste d'image pour la correction cirrus.


Liste des données d’entrées
***************************
         
- l2toa CirrusCorrectionTest/11SLT_20150803_S2A_L1Csimu_toa_abs_10m.tiff
- l2edg CirrusCorrectionTest/11SLT_20150803_S2A_L1Csimu_bord_10m.bord_final
- l2dtm DTM/SENTINEL2/11SLT/11SLT_10m.mnt
- l2cirrus CirrusCorrectionTest/11SLT_20150803_S2A_L1Csimu_toa_60m.B10_resampled.tif
- bandlist 2 3
- cloudcirrus CirrusCorrectionTest/11SLT_20150803_S2A_L1Csimu_cloud_cirrus_10m.tif
- cloudall CirrusCorrectionTest/11SLT_20150803_S2A_L1Csimu_all_mask_10m.tif
- cloudrefl CirrusCorrectionTest/11SLT_20150803_S2A_L1Csimu_refl_mask_10m.tif
- cirrusminpixinlist 100


Liste des produits de sortie
****************************
Le test génére la valeur du gamma ainsi que le mask de cirrus réechantillonné

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
Le test génère en sortie une valeur de gamma ainsi que le mask de cirrus réechantillonné

Mise en oeuvre du test
**********************

Ce test est exécuté en lançant la commande :
ctest -R apTvGammaCompute

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
Néan
