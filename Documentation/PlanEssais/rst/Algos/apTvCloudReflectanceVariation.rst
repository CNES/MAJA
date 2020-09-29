apTvCloudReflectance
~~~~~~~~~~~~~~~~~~~~

Objectif
********
Validation de l'application "CirrusReflectanceVariation"

Description
***********

Le module "CirrusReflectanceVariation" calcul le masque de variation reflectance de nuages.


Liste des données d’entrées
***************************

- tocr CloudMaskingTest/Lacrau_20060813_MS_fmsat_ortho_tocr.tif
- sat CloudMaskingTest/Lacrau_20060813_MS_fmsat_ortho_sat.tif
- edg CloudMaskingTest/Lacrau_20060813_MS_fmsat_ortho_edg.tif
- l2was CloudMaskingTest/Lacrau_20060813_MS_fmsat_ortho_l2was.tif
- l2rcr CloudMaskingTest/Lacrau_20060813_MS_fmsat_ortho_l2rcr.tif
- l2sto CloudMaskingTest/Lacrau_20060813_MS_fmsat_ortho_l2sto.tif
- l2pxd CloudMaskingTest/Lacrau_20060813_MS_fmsat_ortho_l2pxd.tif
- l2ndt CloudMaskingTest/Lacrau_20060813_MS_fmsat_ortho_l2ndt.tif
- initmode false
- foregroundvalue 1
- bluebandtocr 0
- redbandtocr 2
- nirbandtocr 3
- reflectancewaterband 2
- bluereflectancethresholdvalue 0.200000000000000011102230246252
- redreflectancethresholdvalue 0.149999999999999994448884876874
- nirredratiomaxthresholdvalue 2
- nirredratiominthresholdvalue 0.800000000000000044408920985006
- nodata -10
- correlbandtocr 0
- waterthreshold 0.0800000000000000016653345369377
- nominal.seradius 1
- nominal.bluebandrcr 0
- nominal.redbandrcr 2
- nominal.nirbandrcr 3
- nominal.correlbandrcr 0
- nominal.jday 8260
- nominal.cloudforgettingduration 30
- nominal.minvarthreshold 0.0299999999999999988897769753748
- nominal.maxvarthreshold 0.0899999999999999966693309261245
- nominal.knirblue 1
- nominal.kredblue 1.5
- nominal.deltathreshold 0.00500000000000000010408340855861
- nominal.minpixelcorrelation 15
- nominal.correlthreshold 0.900000000000000022204460492503
- nominal.ncorrel 3 3
- nominal.stolistofdates 0 0 0 0 0 0 0 0 0 0
- swir.bandtocr 0
- swir.nirswirratiomin 1


Liste des produits de sortie
****************************

- refl TVA/apTvReflectanceVariation_Refl.tif
- reflvar TVA/apTvReflectanceVariation_ReflVar.tif
- reflext TVA/apTvReflectanceVariation_ReflExt.tif

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
Le test génère en sortie le masque reflectance variation du produit.

Mise en oeuvre du test
**********************
Ce test est exécuté en lançant la commande :
ctest -R apTvCloudReflectanceVariation

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

ST-169-CloudMasking-0010 (C) ; ST-169-CloudMasking-0100 (C) ; ST-169-CloudMasking-0200(C) ; ST-169-CloudMasking-0500 (C) ; ST-169-CloudMasking-0600 (C) ; ST-169-CloudMasking-0700 (C) ; ST-169-CloudMasking-1000 (C) ; ST-169-CloudMasking-1600 (C) ; ST-169-CloudMasking-1700 (C) ; ST-169-CloudMasking-0300 (C) ; ST-169-CloudMasking-0400 (C) ;ST-169-CloudMasking-0410 (C) ; ST-169-CloudMasking-0420 (C) ; ST-169-CloudMasking-0430(C) ; ST-169-CloudMasking-1100 (C) ; ST-169-CloudMasking-1110 (C) ; ST-169-CloudMasking-1120 (C) ; ST-169-CloudMasking-1130 (C) ; ST-169-CloudMasking-1140 (C) ; ST-169-CloudMasking-1150 (C) ; ST-169-CloudMasking-1160 (C) ; ST-169-CloudMasking-1170 (C) ;ST-169-CloudMasking-1180 (C) ; ST-169-CloudMasking-1200 (C) ; ST-169-CloudMasking-1300(C) ; ST-169-CloudMasking-1400 (C) ; ST-169-CloudMasking-1500 (C) ; ST-169-CloudMasking-1510 (C) ; ST-169-CloudMasking-1520 (C) ;

