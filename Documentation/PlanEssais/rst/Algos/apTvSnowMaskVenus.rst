apTvSnowMaskVenus
~~~~~~~~~~~~~~~~~~

Objectif
********
Validation de l'application "SnowMask"

Description
***********

Le module "SnowMask" calcul le masque de neige du produit dans le cas Venus (sans bande SWIR).


Liste des données d’entrées
***************************



Liste des produits de sortie
****************************



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
ctest -R apTvSnowMaskVenus

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
