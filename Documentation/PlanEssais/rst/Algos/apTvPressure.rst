apTvPressure
~~~~~~~~~~~~~~~~

Objectif
********
Validation de l'application "Pressure"

Description
***********

Le test "Pressure" calcul la valeur de la pression athmosphérique en fonction de l'altitude.


Liste des données d’entrées
***************************

 - dtm DTM/FORMOSAT/VE_TEST_AUX_REFDE2_SUDOUEST_0001.DBL.DIR/Sudouest_96m.mnt_ALC
         
         

Liste des produits de sortie
****************************

- pressure TVA/aptvAthmosphericAbsorption_PressureFilter.tif


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
Le test génère en sortie l'image des valeurs de pression.

Mise en oeuvre du test
**********************

Ce test est exécuté en lançant la commande :
ctest -R apTvPressure

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
Néan
