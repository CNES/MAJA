apTvCirrusMask
~~~~~~~~~~~~~~~~

Objectif
********
Validation de l'application "CirrusMask"

Description
***********

Le module "CirrusMask" calcul le masque cirrus du produit.


Liste des données d’entrées
***************************

- toa CirrusMaskTest/15SVD_20091211_S2A_L1Csimu_toa_240m_coef_001.tif
- edg CirrusMaskTest/15SVD_20091211_S2A_L1Csimu_toa_bord_240m
- dtm DTM/SENTINEL2/VE_TEST_AUX_REFDE2_15SDV____0001.DBL.DIR/S2N_TEST_AUX_REFDE2_15SVD_0001_ALC_roi.tif
- bandidx 10
- threshoff 0.35
- threshgain 0
- minpercent 10
- altref 2000



Liste des produits de sortie
****************************

- mask TVA/apTv15SVD_20091211_S2A_L1Csimu_CirrusMask.tif

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
Le test génère en sortie le masque cirrus du produit.

Mise en oeuvre du test
**********************
Ce test est exécuté en lançant la commande :
ctest -R apTvCirrusMask

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
ST-169-CirrusFlagS2-0020 (C) ; ST-169-CirrusFlagS2-0030 (C) ;
