apTvResampling
~~~~~~~~~~~~~~~~

Objectif
********
Validation de l'application "Resampling"

Description
***********

Le module "Resampling" réechantillonne une image sur un DTM donné.


Liste des données d’entrées
***************************

- Image lambda
- DTM coarse

Liste des produits de sortie
****************************

Image lambda reéchantillonnée sur l'emprise du DTM

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
ctest -R apTvResampling

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
