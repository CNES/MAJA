apTvStats
~~~~~~~~~~~~~~~~

Objectif
********
Validation de l'application "Stats"

Description
***********

Le module "Stats" calcul des statistiques sur l'image.


Liste des données d’entrées
***************************

Image lambda

Liste des produits de sortie
****************************

Statistiques de l'image : moyenne, écart type, nombre de pixels valide

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
Le test génère en sortie des statistiques en accord avec le contenu de l'image.

Mise en oeuvre du test
**********************

Ce test est exécuté en lançant la commande :
ctest -R apTvStat

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
