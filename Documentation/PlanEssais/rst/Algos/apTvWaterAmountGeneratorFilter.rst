apTvWaterAmountGenerator
~~~~~~~~~~~~~~~~~~~~~~~~

Objectif
********
Validation de l'application "WaterAmountGenerator"

Description
***********

Le test "WaterAmountGenerator" calcul la valeur de la vapeur d'eau.


Liste des données d’entrées
***************************

- toa AthmosphericAbsorption/31TFJ_20150908_S2A_L1Csimu_toa_240m.tif
- gipwatv AthmosphericAbsorption/giplutmap.xml
- watervaporreferencebandid 8
- watervaporbandid 9
- vapnodatavalue 0
- watervapormaxthreshold 5
- solarzenith 39.999222
- viewingzenith 6.191545

         

Liste des produits de sortie
****************************

- wateramount TVA/aptvAthmosphericAbsorption_WaterAmount.tif


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
Le test génère en sortie l'image des valeurs de vapeur d'eau.

Mise en oeuvre du test
**********************

Ce test est exécuté en lançant la commande :
ctest -R apTvWaterAmountGenerator

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
