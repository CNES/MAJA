apTvCloudMaskUndersampling
~~~~~~~~~~~~~~~~~~~~~~~~~~

Objectif
********
Validation de l'application "CloudMaskUndersampling"

Description
***********

Le module "CloudMaskUndersampling" réechantillonne les image CLA sur un DTM donné.


Liste des données d’entrées
***************************

- Image CLA
- DTM

Liste des produits de sortie
****************************

Image lambda reéchantillonnée sur l'emprise du DTM avec l'interpolateur spécial CLA

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
Le test génère en sortie une image dont l'emprise est calée sur le DTM

Mise en oeuvre du test
**********************

Ce test est exécuté en lançant la commande :
ctest -R apTvCloudMaskUndersampling

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
ST-169-CloudMasking-0010 (C) ; ST-169-CloudMasking-0100 (C) ; ST-169-CloudMasking-0200) ; ST-169-CloudMasking-0500 (C) ; ST-169-CloudMasking-0600 (C) ; ST-169-CloudMasking- (C) ; ST-169-CloudMasking-1000 (C) ; ST-169-CloudMasking-1600 (C) ; ST-169-CloudMasking-1700 (C) ; ST-169-CloudMasking-0300 (C) ; ST-169-CloudMasking-0400 (C) ;ST-169-CloudMasking-0410 (C) ; ST-169-CloudMasking-0420 (C) ; ST-169-CloudMasking-0430(C) ; ST-169-CloudMasking-1100 (C) ; ST-169-CloudMasking-1110 (C) ; ST-169-CloudMasking-CS - Communication & Systèmes — SETG-PE-MAJA-010-1120 (C) ; ST-169-CloudMasking-1130 (C) ; ST-169-CloudMasking-1140 (C) ; ST-169-CloudMasking-1150 (C) ; ST-169-CloudMasking-1160 (C) ; ST-169-CloudMasking-1170 (C) ;ST-169-CloudMasking-1180 (C) ; ST-169-CloudMasking-1200 (C) ; ST-169-CloudMasking-1300(C) ; ST-169-CloudMasking-1400 (C) ; ST-169-CloudMasking-1500 (C) ; ST-169-CloudMasking-1510 (C) ; ST-169-CloudMasking-1520 (C) ;
