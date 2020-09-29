apTvSnowMask
~~~~~~~~~~~~~~~~

Objectif
********
Validation de l'application "SnowMask"

Description
***********

Le module "SnowMask" calcul le masque de neige du produit.


Liste des données d’entrées
***************************

- tocr SnowMaskTest/L930662_20020212_L7_198_030_USGS_tocr.tif
- edg SnowMaskTest/L930662_20020212_L7_198_030_USGS_edg.tif
- angles SnowMaskTest/L930662_20020212_L7_198_030_USGS_angle_image.tif
- refband 1
- absband 4
- redband 2
- seradius 2
- ndsithresh 0.4
- redthresh 0.12
- swirthresh 0.16
- thetas 61.48641010000001
- mincos 0.20


Liste des produits de sortie
****************************

- mask TVA/apTvL930662_20020212_L7_198_030_USGS_snow_mask.tif


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
Le test génère en sortie le masque de neige du produit.

Mise en oeuvre du test
**********************
Ce test est exécuté en lançant la commande :
ctest -R apTvSnowMask

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
ST-169-SnowMasking-0010 (C) ; 
