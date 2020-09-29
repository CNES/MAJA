apTvReduceLutDirCorr
~~~~~~~~~~~~~~~~~~~~

Objectif
********
Validation de l'application "ReduceLut" en appliquant la correction directionnelle

Description
***********

Le module "ReduceLut" extrait une lut 3D  à partir d'une lut 6D et des angles de vues du détécteur. De plus une correction directionnelle est appliquée dans le cas d'un capteur ayant les données necessaires


Liste des données d’entrées
***************************

- solar.azimuth 143.195
- solar.zenith 24.0717 
- viewing.zenith 6.08321 6.08321 6.08321 6.08321 6.08321 6.08321 6.08321 6.08321 6.08321 6.08321 6.08321 6.08321 6.08321
- viewing.azimuth 104.105 104.105 104.105 104.105 104.105 104.105 104.105 104.105 104.105 104.105 104.105 104.105 104.105
- lut ReduceLutComputationTest/giplut_cams.xml
- refzenith 0
- refazimuth 0
- dircorr true
- coeffsr 0 0.480620155 0.4441041348 0.3396449704 0.3396449704 0.4179634487 0.4962819269 0.4962819269 0.4962819269 0 0 0.3364431487 0.240406320
- coeffsv 0 0.1020671835 0.1362940276 0.1343195266 0.1343195266 0.1205060291 0.1066925315 0.1066925315 0.1066925315 0 0 0.1320699708 0.1455981941

Liste des produits de sortie
****************************

 - reducelut TVA/apTvReduceLutDirCorr.mhd


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
La lut 3D doit correspondre à la lut6D extraite sur les paramètres

Mise en oeuvre du test
**********************

Ce test est exécuté en lançant la commande :
ctest -R apTvReduceLutDirCorr

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
ST-169-ReducedLUT-0010 (C) ;

