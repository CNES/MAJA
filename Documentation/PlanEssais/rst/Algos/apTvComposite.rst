apTvComposite
~~~~~~~~~~~~~~~~

Objectif
********
Validation de l'application "Composite"

Description
***********

Le module "Composite" calcul les images composites d'un produit.Le module "CompositeImage" est chargé de la mise à jour des images composites contenues dans la partie privée des produits de niveau 2


Liste des données d’entrées
***************************

- tocr CompositeTest/apTvSudouest_20060326_tocr.tif
- toac CompositeTest/apTvSudouest_20060326_toa.tif
- toc CompositeTest/apTvSudouest_20060326_toc.tif
- edg CompositeTest/apTvSudouest_20060326_edg.tif
- cldsum CompositeTest/apTvSudouest_20060326_pcld.tif
- sat CompositeTest/apTvSudouest_20060326_pdat.tif
- aot CompositeTest/apTvSudouest_20060326_aot_mask.tif
- initmode true
- reall2nodata -10
- l2nodata -10000
- sigmaaot 1
- minaot 0.10000000000000000555111512312
- maxaot 0.5
- maxage 60
- hotspotflag 0
- cirrusflag 0
- rainflag 0
- jday 8120
- stobandnumber 10
- stobandindex 0
- aotradius 1
                           


Liste des produits de sortie
****************************

- rta TVA/apTvCompositeImageFilter_RTA.tif
- rcr TVA/apTvCompositeImageFilter_RCR.tif
- rtc TVA/apTvCompositeImageFilter_RTC.tif
- sto TVA/apTvCompositeImageFilter_STO.tif
- pxd TVA/apTvCompositeImageFilter_PXD.tif
- ndt TVA/apTvCompositeImageFilter_NDT.tif

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
ctest -R apTvComposite

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
ST-169-Composite-0010 (C) ; ST-169-Composite-0020 (C) ; ST-169-Composite-0030 (C) ; ST-169-
Composite-0040 (C) ; ST-169-Composite-0050 (C) ;

