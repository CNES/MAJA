apTvDarkestPixelAOTNominal
~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Objectif
********
Validation de l'application "DarkestPixelAOTNominal" calcul la valeur du DarkestPixel pour l'algorithm d'AOTEstimation

Description
***********

Le module "DarkestPixelAOTNominal" calcul la valeur du DarkestPixel pour l'algorithm d'AOTEstimation .


Liste des données d’entrées
***************************

 - toac AOTEstimation/L930662_20020212_L7_198_030_USGS_toac_240m.tif
 - shd  AOTEstimation/L930662_20020212_L7_198_030_USGS_shd_240m.tif
 - dtm  DTM/LANDSAT/LN_TEST_AUX_REFDE2_EU93066200A00B_0001.DBL.DIR/LN_TEST_AUX_REFDE2_EU93066200A00B_0001_ALC.tif
 - cld  AOTEstimation/L930662_20020212_L7_198_030_USGS_l2cld_240m.tif
 - l2ndt AOTEstimation/L930662_20020212_L7_198_030_USGS_l2ndt_240m.tif
 - l2rtc AOTEstimation/L930662_20020212_L7_198_030_USGS_l2rtc_240m.tif
 - edg AOTEstimation/L930662_20020212_L7_198_030_USGS_edg_240m.tif
 - lutheader AOTEstimation/giplut.xml
 - lutimage  AOTEstimation/lutAOT.mha
 - darkestchanneltoac 0
 - thresholddarkpixel 0.015
 - darksurfacereflectance 0
 - darkestchannelrtc 0

Liste des produits de sortie
****************************

Valeurs du DarkestPixel

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
Le test génère en sortie la valeurs du DarkestPixel.

Mise en oeuvre du test
**********************

Ce test est exécuté en lançant la commande :
ctest -R apTvDarkestPixelAOTNominal

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
Néant
