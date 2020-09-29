apTvBinaryToVector
~~~~~~~~~~~~~~~~~~

Objectif
********
Validation de l'application "BinaryToVector"

Description
***********

Le module "BinaryToVector" split les differents bit d'une image masque multi-bit en autant d'image monobit contenant chacune un bit de l'image d'entrée.


Liste des données d’entrées
***************************

Image de type masque multibit sur un octet ayant une seule bande

Liste des produits de sortie
****************************

Image de type masque ayant une plusieurs bande dont chacune représente un bit de masque d'entrée

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
Le test génère en sortie une image ayant une plusieurs bande dont chacune représente un bit de masque d'entrée.

Mise en oeuvre du test
**********************

Ce test est exécuté en lançant la commande :
ctest -R apTvBinaryToVector

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
