apTvTOACorrection
~~~~~~~~~~~~~~~~~~

Objectif
********
Validation de l'application "TOACorrection"

Description
***********

Le test "TOACorrection" applique la correction TOA à une image. Le module AtmosphericAbsorptionCorrection corrige la réflectance au sommet de l'atmosphère de l'absorption atmosphérique. Ce test utilise des contenus en ozone et en vapeur d'eau fixés par défaut. Ce filtre génère l'image corrigée aux résolutions L2 coarse.


Liste des données d’entrées
***************************

- toa AthmosphericAbsorption/Sudouest_20060819_MS_fmsat_ortho_toa.tif
- pressure aptvAthmosphericAbsorption_PressureFilter.tif
- smac AthmosphericAbsorption/gipsmacmap.xml
- wateramountdefault 3
- ozoneamount 0.299999999999999988897769753748
- thetas 35.5579999999999998294697434176
- thetav 22.4879999999999995452526491135 22.4879999999999995452526491135 22.4879999999999995452526491135 22.4879999999999995452526491135
- nodata -10
- bandlist 0 1 2 3

         

Liste des produits de sortie
****************************

- toac TVA/aptvAthmosphericAbsorption_TOACorrection.tif    


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
ctest -R apTvTOACorrection

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
ST-169-AbsorptionCorr-0010 (C) ; ST-169-AbsorptionCorr-0020 (C) ; ST-169-AbsorptionCorr-
0030 (C) ; ST-169-AbsorptionCorr-0030 (C) ; ST-169-AbsorptionCorr-0040 (C) ; ST-169-AbsorptionCorr-0050 (C) ;
