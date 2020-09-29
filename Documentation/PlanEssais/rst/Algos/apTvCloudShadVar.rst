apTvCloudShadVar
~~~~~~~~~~~~~~~~

Objectif
********
Validation de l'application "CirrusShadVar"

Description
***********

Le module "CirrusShadVar" calcul les variations sur le masque d'ombre des nuages.


Liste des données d’entrées
***************************

- tocr CloudMaskingTest/Lacrau_20060813_MS_fmsat_ortho_tocr.tif
- edg CloudMaskingTest/Lacrau_20060813_MS_fmsat_ortho_edg.tif
- cldshad CloudMaskingTest/Lacrau_20060813_MS_fmsat_ortho_cldshad.tif
- cldall CloudMaskingTest/Lacrau_20060813_MS_fmsat_ortho_cldall.tif
- vie CloudMaskingTest/Lacrau_20060813_MS_fmsat_ortho_vie.tif
- dtm DTM/FORMOSAT/Lacrau_96m.mnt
- sol CloudMaskingTest/Lacrau_20060813_MS_fmsat_ortho_sol.tif
- l2rcr CloudMaskingTest/Lacrau_20060813_MS_fmsat_ortho_l2rcr.tif
- l2was CloudMaskingTest/Lacrau_20060813_MS_fmsat_ortho_l2was.tif
- l2ndt CloudMaskingTest/Lacrau_20060813_MS_fmsat_ortho_l2ndt.tif
- l2shad CloudMaskingTest/Lacrau_20060813_MS_fmsat_ortho_shad.tif
- l2shadvar CloudMaskingTest/Lacrau_20060813_MS_fmsat_ortho_shad_var.tif
- l2sto CloudMaskingTest/Lacrau_20060813_MS_fmsat_ortho_l2sto.tif
- l2coarseres 96
- shadbandtocr 2
- shadbandrcr 2
- viehref 3000
- solhref 3000
- nodata -10
- maxdarkening 0.8
- minpercentile 0.1
- maxcla 8226
- compositelabelratio 0.800000000000000044408920985006
- minpixelcorrelation 15
- correlthreshold 0.900000000000000022204460492503
- ncorrel 3 3
- stolistofdates 0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0
- jday 8260


Liste des produits de sortie
****************************

- shadvar TVA/apTvCloudShadVar.tif


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
Le test génère en sortie le masque de variation des ombres de nuagesdu produit.

Mise en oeuvre du test
**********************
Ce test est exécuté en lançant la commande :
ctest -R apTvCloudShadVar

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


