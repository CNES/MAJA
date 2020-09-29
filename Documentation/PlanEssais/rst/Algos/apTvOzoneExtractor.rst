apTvOzoneExtractor
~~~~~~~~~~~~~~~~~~

Objectif
********
Validation de l'application "OzoneExtractor"

Description
***********

Le module "OzoneExtractor" extrait la valeur d'ozone d'un fichier météo.


Liste des données d’entrées
***************************

- Fichier météo
- Lat/lon souhaité


Liste des produits de sortie
****************************

Valeur d'ozone pour la localisation demandée

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
La valeurs d'ozone doit correspondre avec le fichier d'entrée (attention aux conversions)

Mise en oeuvre du test
**********************

Ce test est exécuté en lançant la commande :
ctest -R apTvOzoneExtractor

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
