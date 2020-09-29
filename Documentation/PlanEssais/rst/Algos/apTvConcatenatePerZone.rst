apTvConcatenatePerZone
~~~~~~~~~~~~~~~~~~~~~~

Objectif
********
Validation de l'application "ConcatenatePerZone"

Description
***********

Le module "ConcatenatePerZone" concaténe plusieurs images ayant chacune un identifiant de zone en fonction d'un masque de zone.


Liste des données d’entrées
***************************

- Plusieurs images representant chacune une zone (par exemple images d'angle pour chaque détécteur Sentinel2). 
- Masque de zone

Liste des produits de sortie
****************************

Image dont le contenu est la répartition selon le masque des images d'entrée. Typiquement un assemblage des differents detecteur.

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
Le test génère en sortie une image dont la répartition est selon le masque des images d'entrée.

Mise en oeuvre du test
**********************

Ce test est exécuté en lançant la commande :
ctest -R apTvConcatenatePerZone

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
