apTvSlopeCorrection
~~~~~~~~~~~~~~~~~~~~~~

Objectif
********
Validation de l'application "SlopeCorrection"

Description
***********

Le test "SlopeCorrection" applique la correction des effets de pentes à une image.

Le module SlopeCorrection corrige radiométriquement l'image de reflectance de surface corrigée des effets de l'environnement (issue de "EnvironmentCorrection") des effets liés à la topographie. Cette correction s'appuie sur les images de pente, d'aspect et de MNT fournies en entrée, ainsi que sur les LUT Dirt, Dift et Satm.


Liste des données d’entrées
***************************

- tdirsub SlopeCorrection/15SVD_20091211_S2A_L1Csimu_tdir.tif
- tdifsub SlopeCorrection/15SVD_20091211_S2A_L1Csimu_tdif.tif
- dtmasc DTM/SENTINEL2/VE_TEST_AUX_REFDE2_15SDV____0001.DBL.DIR/S2N_TEST_AUX_REFDE2_15SVD_0001_ASP_R2_multipliedByScalar.tif
- dtmslc DTM/SENTINEL2/VE_TEST_AUX_REFDE2_15SDV____0001.DBL.DIR/S2N_TEST_AUX_REFDE2_15SVD_0001_SLP_R2_multipliedByScalar.tif
- rhoenv SlopeCorrection/15SVD_20091211_S2A_L1Csimu_rho_env_10m.tif
- incangle SlopeCorrection/15SVD_20091211_S2A_L1Csimu_IncidenceAngle.tif
- sre SlopeCorrection/15SVD_20091211_S2A_L1Csimu_surf_env_10m.tif
- thetas 64.165163327
- phis 162.402228733
- thetav 0 0 0 0 0 0
- phiv 0 0 0 0 0 0
- mincosi 0.200000000000000011102230246252
- mincose 0.100000000000000005551115123126
- mincosratio 0.200000000000000011102230246252
- nodata -10

Liste des produits de sortie
****************************

- fre TVA/apTvSlopeCorrection_15SVD_20091211_S2A_fre.tif
- tgs TVA/apTvSlopeCorrection_15SVD_20091211_S2A_tgs.tif
- stl TVA/apTvSlopeCorrection_15SVD_20091211_S2A_stl.tif       


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
ctest -R apTvSlopeCorrection

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
ST-169-SlopeCorr-0010 (C) ; ST-169-SlopeCorr-0020 (C) ; ST-169-SlopeCorr-0030 (C) ; ST-169-SlopeCorr-0040 (C) ; ST-169-SlopeCorr-0050 (C) ; ST-169-SlopeCorr-0060 (C) ;


