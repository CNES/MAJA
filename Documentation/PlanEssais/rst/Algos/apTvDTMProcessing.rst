apTvDTMProcessing
~~~~~~~~~~~~~~~~~~

Objectif
********
Validation de l'application "DTMProcessing"

Description
***********

Le module "DTMProcessing" calcul les parties cachées et les ombre du DTM sur le produit.

Le module "DTMProcessing" détecte les pixels correspondant aux faces cachées par le relief et aux zones d'ombres liées également au relief.
L'image utilisée pour ce test est une image située sur le site Maroc qui présente de légères variations du relief avec des versants NORD à l'ombre. 
Pour accentuer cet effet, les angles d'acquisition sont modifiés


Liste des données d’entrées
***************************
- solar DTMProcessingTest/Maroc_20061216_solar.tif
- viedtm DTMProcessingTest/Maroc_20061216_vie.tif
- edg DTMProcessingTest/Maroc_20061216_edg.tif
- dtm DTM/FORMOSAT/VE_TEST_AUX_REFDE2_MAROC____0001.DBL.DIR/Maroc_96m.mnt_ALC
- distthreshold 96
- solhref 4000
- viehref 4000
                           


Liste des produits de sortie
****************************

- hid TVA/apTvDTMProcessing_hid.tif
- shd TVA/apTvDTMProcessing_shd.tif

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
Le test génère en sortie les images composites

Mise en oeuvre du test
**********************
Ce test est exécuté en lançant la commande :
ctest -R apTvDTMProcessing

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
ST-169-DTM-0010 (C) ; ST-169-DTM-0020 (C) ; ST-169-DTM-0030 (C) ; ST-169-DTM-0040(C) ; ST-169-DTM-0050 (C) ;

