apTvImageListWriter
~~~~~~~~~~~~~~~~~~~~

Objectif
********
Validation de l'application "ImageListWriter"

Description
***********

Le module "ImageListWriter" ecrit simultanéement une liste d'image.


Liste des données d’entrées
***************************

Liste d'images

Liste des produits de sortie
****************************

Liste d'images

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
Le test génère en sortie une copie des images d'entrées.

Mise en oeuvre du test
**********************

Ce test est exécuté en lançant la commande :
ctest -R apTvImageListWriter

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
