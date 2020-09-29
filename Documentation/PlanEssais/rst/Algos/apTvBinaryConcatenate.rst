apTvBinaryConcatenate
~~~~~~~~~~~~~~~~~~~~~~

Objectif
********
Validation de l'application "BinaryConcatenate"

Description
***********

Le module "BinaryConcatenate" concatene les bandes d'un masque à plusieurs bandes en une seule bande dont chaque bit représente une bande d'entrée.


Liste des données d’entrées
***************************

Image de type masque ayant plusieurs bandes

Liste des produits de sortie
****************************

Image de type masque ayant une seule bande dont chaque bit représente une bande d'entré

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
Le test génère en sortie une image ayant une seule bande dont chaque bit représente une bande d'entré .

Mise en oeuvre du test
**********************

Ce test est exécuté en lançant la commande :
ctest -R apTvBinaryConcatenate

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
