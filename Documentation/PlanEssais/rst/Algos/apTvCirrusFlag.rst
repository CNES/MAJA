apTvCirrusFlag
~~~~~~~~~~~~~~~~

Objectif
********
Validation de l'application "CirrusFlag"

Description
***********

Le module "CirrusFlag" calcul le flag cirrus du produit.


Liste des données d’entrées
***************************
- cla CirrusFlagTest/CAMARGUE_20120103_CLA_sub.tif
- edg CirrusFlagTest/CAMARGUE_20120103_VE_ortho_toa_bord_100m
- altthreshold 1000
- minpercentcirrus 10
- minpercentcloud 15
- minstdcirrus 10
- minpercentstdcirrus 500
- nodata -10


Liste des produits de sortie
****************************

- flag cirrus

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
Le test génère en sortie le flag cirrus du produit.

Mise en oeuvre du test
**********************
Ce test est exécuté en lançant la commande :
ctest -R apTvCirrusFlag

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
ST-169-CirrusFlagVE-0010 (C) ; ST-169-CirrusFlagVE-0020 (C) ; ST-169-CirrusFlagVE-0030(C) ; ST-169-CirrusFlagVE-0040 (C) ;
