apTvRayleighCorrection
~~~~~~~~~~~~~~~~~~~~~~

Objectif
********
Validation de l'application "RayleighCorrection"

Description
***********

Le test "RayleighCorrection" applique la correction rayleigh à une image. 

Le module RayleighCorrection corrige les images de réflectance TOA des effets de la diffusion liés aux molécules et aux aérosols. 
L'estimation du contenu en aérosols n'ayant pas encore été réalisée, une valeur faible d'épaisseur optique aérosol (voire nulle ici) est fixée par défaut


Liste des données d’entrées
***************************


- lutimage RayleighCorrection/lut.mha
- lutheader RayleighCorrection/giplut.xml
- toa RayleighCorrection/Sudouest_20060819_MS_fmsat_ortho_toa_96m_AOTnull.tif
- edg RayleighCorrection/Sudouest_20060819_MS_fmsat_ortho_toa_bord_96m
- dtm DTM/FORMOSAT/VE_TEST_AUX_REFDE2_SUDOUEST_0001.DBL.DIR/Sudouest_96m.mnt_ALC
- nodata -10
- defaultaot 0
         

Liste des produits de sortie
****************************

- tocr TVA/apTvRayleighCorrection.tif


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
ctest -R apTvRayleighCorrection

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
ST-169-RayleighCorr-0010 (C) ;
