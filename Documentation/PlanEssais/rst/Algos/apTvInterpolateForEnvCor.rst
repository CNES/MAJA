apTvInterpolateForEnvCor
~~~~~~~~~~~~~~~~~~~~~~~~~

Objectif
********
Validation de l'application "InterpolateForEnvCor"

Description
***********

Le module "InterpolateForEnvCor" calcul les lut de transmission pour la correction environnementale.


Liste des données d’entrées
***************************

- lutalbd EnvCorrection/giplutalbedomap.xml
- luttdir SlopeCorrection/giplutTdirmap.xml
- luttdif SlopeCorrection/giplutTdifmap.xml
- aot SlopeCorrection/15SVD_20091211_S2A_L1Csimu_aot.tif
- dtm DTM/SENTINEL2/VE_TEST_AUX_REFDE2_15SDV____0001.DBL.DIR/S2N_TEST_AUX_REFDE2_15SVD_0001_ALC.tif
- edg SlopeCorrection/15SVD_20091211_S2A_L1Csimu_edg.tif
- thetav 0 0 0 0 0 0 0 0 0 0 0 0 0


Liste des produits de sortie
****************************

- tdir TVA/apTvInterpolateForEnvCorr_Tdir.tif
- tdif TVA/apTvInterpolateForEnvCorr_Tdif.tif
- albd TVA/apTvInterpolateForEnvCorr_albedo.tif



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
Le test génère en sortie les images de lut interolées

Mise en oeuvre du test
**********************
Ce test est exécuté en lançant la commande :
ctest -R apTvInterpolateForEnvCor

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

