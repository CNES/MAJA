apTvNewCamsMixedCompute
~~~~~~~~~~~~~~~~~~~~~~~~

Objectif
********
Validation de l'application "CamsCompute" avec des CAMS nouveau format et des anciens format en même temps

Description
***********

Le module "CamsCompute" extrait les valeurs de pondération cams d'un fichier CAMS.


Liste des données d’entrées
***************************

- Fichier CAMS nouvelle version et ancienne version
- Lat/lon souhaité


Liste des produits de sortie
****************************

Valeurs de pondération des modéles pour la localisation demandée

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
Les valeurs de pondération doivent correspondre avec la zone

Mise en oeuvre du test
**********************

Ce test est exécuté en lançant la commande :
ctest -R apTvNewCamsMixedCompute

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
ST-169-ReducedLUT-0010 (C) ;
