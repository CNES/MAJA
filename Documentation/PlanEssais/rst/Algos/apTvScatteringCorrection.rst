apTvScatteringCorrection
~~~~~~~~~~~~~~~~~~~~~~~~

Objectif
********
Validation de l'application "ScatteringCorrection"

Description
***********

Le test "ScatteringCorrection" applique la correction du scattering à une image. Le module ScatteringCorrection corrige les images de réflectance des effets de la diffusion liée aux molécules et aux aérosols. Une image du contenu en aérosols est fournie en entrée.


Liste des données d’entrées
***************************

- lutmap ScatteringCorrectionTest/giplutmap.xml
- toac ScatteringCorrectionTest/apTvSudouest_20060819_toac.tif
- aot ScatteringCorrectionTest/apTvSudouest_20060819_aot.tif
- edg ScatteringCorrectionTest/apTvSudouest_20060819_edg.tif
- dtm ScatteringCorrectionTest/apTvSudouest_20060819_dtm.tif
- l2nodata 0
- l2bandincoarse 0 1 2 3

         

Liste des produits de sortie
****************************

- tocr TVA/apTvScatteringCorrectionImageFilterTest.tif


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
Le test génère en sortie l'image corrigée.

Mise en oeuvre du test
**********************

Ce test est exécuté en lançant la commande :
ctest -R apTvScatteringCorrection

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
ST-169-SactteringCorr-0010 (C) ;

