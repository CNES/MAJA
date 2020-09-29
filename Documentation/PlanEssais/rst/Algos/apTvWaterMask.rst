apTvWaterMask
~~~~~~~~~~~~~~~~

Objectif
********
Validation de l'application "WaterMask"

Description
***********

Le module "WaterMask" calcul le masque d'eau du produit.


Liste des données d’entrées
***************************

- tocr WaterMaskingTest/31TFJ_20150928_S2A_L1Csimu_surf_ray_240m.tif
- edg WaterMaskingTest/31TFJ_20150928_S2A_L1Csimu_toa_bord_240m
- cldsum WaterMaskingTest/31TFJ_20150928_S2A_cld.tif
- dtmshd WaterMaskingTest/31TFJ_20150928_S2A_L1Csimu_toa_240m.shd
- l2rcr WaterMaskingTest/31TFJ_20150918_S2A_L1Csimu_surf_ray_comp_240m.tif
- l2pwa WaterMaskingTest/31TFJ_20150918_S2A_L1Csimu_toa_240m.compt
- l2twa WaterMaskingTest/31TFJ_20150918_S2A_L1Csimu_toa_240m.pos
- sunglintflag true
- demslc WaterMaskingTest/31TFJ_240m_slc.tif
- srtmwat WaterMaskingTest/31TFJ_240m.eau
- redbandtocr 3
- nirbandtocr 8
- nirbandrcr 8
- waterndvithreshold 0.01
- waterreflectancevariationthreshold 0.04
- maximumsunglintreflectance 0.12
- srtmgaussiansigma 10
- srtmpercent 51
- watermasknumber 10
- minpercentpossiblewater 50
- waterslopethreshold 0.01
- waterredreflectancethreshold 0.05
- reall2nodata -10



Liste des produits de sortie
****************************

- was TVA/apTvWaterMaskingImageFilterTest-WAS.tif
- pwa TVA/apTvWaterMaskingImageFilterTest-PWA.tif
- twa TVA/apTvWaterMaskingImageFilterTest-TWA.tif


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
Le test génère en sortie le masque d'eau du produit.

Mise en oeuvre du test
**********************
Ce test est exécuté en lançant la commande :
ctest -R apTvWaterMask

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
ST-169-WaterMasking-0010 (C) ; ST-169-WaterMasking-0020 (C) ; ST-169-WaterMasking-0030(C) ; ST-169-WaterMasking-0040 (C) ; ST-169-WaterMasking-0050 (C) ; ST-169-WaterMasking-0060 (C) ; ST-169-WaterMasking-0060 (C) ;
