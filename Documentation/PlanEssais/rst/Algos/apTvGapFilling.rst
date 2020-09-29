apTvGapFilling
~~~~~~~~~~~~~~~~

Objectif
********
Validation de l'application "GapFilling" qui applique l'algo du GapFilling de l'algorithm d'AOTEstimation

Description
***********

Le module "GapFilling" applique l'algo du GapFilling de l'algorithm d'AOTEstimation


Liste des données d’entrées
***************************

- im AOTEstimation/L930662_20020212_L7_198_030_USGS_aot.tif
- initwindowsradius 2
- maxwindowsradius 16
- reall2nodata -10
- mean 0.02948
- defaultvalue 0.2
- hasvalidpixels 1


Liste des produits de sortie
****************************
  
- out GapFilling_filledImage.tif
- outmask GapFilling_mask.tif

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
Le test génère en sortie l'image GapFilled ainsi que le masque de GapFilling.

Mise en oeuvre du test
**********************

Ce test est exécuté en lançant la commande :
ctest -R apTvGapFilling

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
