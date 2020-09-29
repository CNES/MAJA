apTvAngleListToImage
~~~~~~~~~~~~~~~~~~~~

Objectif
********
Validation de l'application "AngleListToImage"

Description
***********

Le module "AngleListToImage" calcul une image à partir d'une liste d'angles. Cela permet notamment de créer une image des angles à partir de métadonnées (S2).


Liste des données d’entrées
***************************

- Les angles sous format texte selon la nomenclature d'interface
- Le DTM sur lequel les angles doivent être calculés


Liste des produits de sortie
****************************

Image contenant les angle d'incidence pour chaque pixel

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
Le test génère en sortie une image contenant les angles.

Mise en oeuvre du test
**********************

Ce test est exécuté en lançant la commande :
ctest -R apTvAngleListToImage

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
