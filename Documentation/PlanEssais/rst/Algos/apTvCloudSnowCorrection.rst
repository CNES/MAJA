apTvCloudSnowCorrection
~~~~~~~~~~~~~~~~~~~~~~~~

Objectif
********
Validation de l'application "CloudSnowCorrection"

Description
***********

Le module "CloudSowCorrection" calcul le masque de correction des effets de neige en prenant en compte les nuages.

Le module "SnowMaskCorrection" permet de corriger les fausses détection de neige située à l'intérieur des nuages. 
Les pixels neigeux entièrement entourés de nuages sont déclarés nuageux plutôt que neigeux.


Liste des données d’entrées
***************************

- snowin CloudMaskingTest/L930863_20130815_L8_196_030_USGS_toa_240m.neige_avant_correction
- edg CloudMaskingTest/L930863_20130815_L8_196_030_USGS_toa_bord_240m
- cldreflvar CloudMaskingTest/L930863_20130815_L8_196_030_USGS_toa_240m.refl_avant_correction
- cldrefl CloudMaskingTest/L930863_20130815_L8_196_030_USGS_toa_240m.reflvar_avant_correction
- seradius 1


Liste des produits de sortie
****************************

- snow TVA/apTvCloudSnowCorrection_snow.tif
- refl TVA/apTvCloudSnowCorrection_reflvar.tif

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
Le test génère en sortie le masque de correction des effets de neige.

Mise en oeuvre du test
**********************
Ce test est exécuté en lançant la commande :
ctest -R apTvCloudSnowCorrection

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
ST-169-CloudMasking-0400(C);ST-169-CloudMasking-0500(C);

