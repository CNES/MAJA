apTvWaterVaporPostPro
~~~~~~~~~~~~~~~~~~~~~

Objectif
********
Validation de l'application "WaterVaporPostPro"

Description
***********

Le module "WaterVaporPostPro" ameliore le masque de vapeur d'eau


Liste des données d’entrées
***************************

- tocr WaterVaporPostProcessing/31TFJ_20150908_S2A_L1Csimu_surf_240m.tif
- vapin  WaterVaporPostProcessing/31TFJ_20150908_S2A_L1Csimu_surf_VAP_SUB.tif
- cldsum WaterVaporPostProcessing/31TFJ_20150908_S2A_L1Csimu_toa_240m.CLDSum.tif
- defaultwatervapor 0
- threshold 200
- thresholdref 100
- referencebandcode 8
- bandcode 9
- nodata 0
- sevalidradius 1
- initwindowradius 2
- maxwindowradius 16


Liste des produits de sortie
****************************

- mask TVA/apTvWaterVaporPostPro_mask_240m.tif
- vap TVA/apTvWaterVaporPostPro_VAP_Filtered_240m.tif



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
Le test génère en sortie l'image de vapeur d'eau ainsi que le masque.

Mise en oeuvre du test
**********************
Ce test est exécuté en lançant la commande :
ctest -R apTvWaterVaporPostPro

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

