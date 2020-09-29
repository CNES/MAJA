apTvDirectionalCorrection
~~~~~~~~~~~~~~~~~~~~~~~~~

Objectif
********
Validation de l'application "DirectionalCorrection"

Description
***********

Le module "DirectionalCorrection" calcul les coefficients de correction directionnelle.


Liste des données d’entrées
***************************

- données d'orientation


Liste des produits de sortie
****************************

Valeurs des coefficients de correction

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

Les valeurs de corrections doivent être credible par rapport à l'orientation du satellite

Mise en oeuvre du test
**********************

Ce test est exécuté en lançant la commande :
ctest -R apTvDirectionalCorrection

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
