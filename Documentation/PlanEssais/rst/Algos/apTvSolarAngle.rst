apTvSolarAngle
~~~~~~~~~~~~~~~~

Objectif
********
Validation de l'application "SolarAngle"

Description
***********

Le module "SolarAngle" calcul les angles solaires à partir des données capteur.


Liste des données d’entrées
***************************

- SOL1 : Image des angles solaire capteur
- Hauteurs de réference 


Liste des produits de sortie
****************************

Image contenant les angle solaire pour chaque pixel

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
Le test génère en sortie une image contenant les angles solaires.

Mise en oeuvre du test
**********************

Ce test est exécuté en lançant la commande :
ctest -R apTvSolarAngle

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
