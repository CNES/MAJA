apTvComputeTransmission
~~~~~~~~~~~~~~~~~~~~~~~~

Objectif
********
Validation de l'application "ComputeTransmission"

Description
***********

Le test "ComputeTransmission" calcul les images de transmission pour la correction des effets de pentes à une image.


Liste des données d’entrées
***************************

- luttdir SlopeCorrection/giplutTdirmap.xml
- luttdif SlopeCorrection/giplutTdifmap.xml
- aot SlopeCorrection/15SVD_20091211_S2A_L1Csimu_aot.tif
- dtm DTM/SENTINEL2/VE_TEST_AUX_REFDE2_15SDV____0001.DBL.DIR/S2N_TEST_AUX_REFDE2_15SVD_0001_ALC.tif
- edg SlopeCorrection/15SVD_20091211_S2A_L1Csimu_edg.tif
- thetas 64.1652



Liste des produits de sortie
****************************

- tdir TVA/15SVD_20091211_S2A_L1Csimu_tdir.tif
- tdif TVA/15SVD_20091211_S2A_L1Csimu_tdif.tif


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
Le test génère en sortie les images de transmission tdir et tdif pour la correction des effets de pentes.

Mise en oeuvre du test
**********************

Ce test est exécuté en lançant la commande :
ctest -R apTvComputeTransmission

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



