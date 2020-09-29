apTvExtractLutChannel
~~~~~~~~~~~~~~~~~~~~~~

Objectif
********
Validation de l'application "ExtractLutChannel"

Description
***********

Le module "ExtractLutChannel" extrait une bande d'un fichier de lut mha.


Liste des données d’entrées
***************************

- Fichier lut
- Channel voulue


Liste des produits de sortie
****************************

Lut à une bande

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
La lut de sortie doit correspondre à la bande voulue

Mise en oeuvre du test
**********************

Ce test est exécuté en lançant la commande :
ctest -R apTvExtractLutChannel

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
