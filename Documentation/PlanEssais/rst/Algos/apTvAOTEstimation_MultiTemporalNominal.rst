apTvAOTEstimation_MultiTemporalNominal
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Objectif
********
Validation de l'application "AOTEstimation" en mode nominal Multi-Temporel

Description
***********

Le test "AOTEstimation_MultiTemporalNominal" calcul la valeur de l'AOT en mode nominal Multi-Temporel.Le module AOTEstimation génère une image contenant pour chaque pixel, l'épaisseur optique des aérosols estimée.


Liste des données d’entrées
***************************

- toac AOTEstimation/MultiTemporal/Sudouest_20061114_MS_fmsat_ortho_toac.tif
- shd AOTEstimation/MultiTemporal/Sudouest_20061114_MS_fmsat_ortho_shd.tif
- dtm AOTEstimation/MultiTemporal/Sudouest_20061114_MS_fmsat_ortho_dtm.tif
- cld AOTEstimation/MultiTemporal/Sudouest_20061114_MS_fmsat_ortho_cld.tif
- l2ndt AOTEstimation/MultiTemporal/Sudouest_20061114_MS_fmsat_ortho_lL2inNDT.tif
- l2rtc AOTEstimation/MultiTemporal/Sudouest_20061114_MS_fmsat_ortho_lL2inRTC.tif
- edg AOTEstimation/MultiTemporal/Sudouest_20061114_MS_fmsat_ortho_edg.tif
- sat AOTEstimation/MultiTemporal/Sudouest_20061114_MS_fmsat_ortho_sat.tif
- l2rta AOTEstimation/MultiTemporal/Sudouest_20061114_MS_fmsat_ortho_lL2inRTA.tif
- l2pxd AOTEstimation/MultiTemporal/Sudouest_20061114_MS_fmsat_ortho_lL2inPXD.tif
- was AOTEstimation/MultiTemporal/Sudouest_20061114_MS_fmsat_ortho_was.tif
- snow AOTEstimation/MultiTemporal/Sudouest_20061114_MS_fmsat_ortho_snw.tif
- lutheader AOTEstimation/MultiTemporal/giplut.xml
- lutimage AOTEstimation/MultiTemporal/lutAOTMT1.mha
- mode multitemporal
- xmlparamsfile AOTEstimation/MultiTemporal/AOTParams_LANDSAT.xml
- temporal.altitudemean 0
- jday 8036
- reall2nodata 0
- darkestaot 0.0430164
- temporal.lutmap AOTEstimation/MultiTemporal/giplutmap.xml

         

Liste des produits de sortie
****************************

- aotmask TVA/apTvAOTEstimationMultitemporalNominal_aotmask.tif
- aotcomputed TVA/apTvAOTEstimationtemporalNominal_aotcomputed.tif


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
Le test génère en sortie l'image des valeurs de l'AOT ainsi que le masque des valeurs calculées ou non.

Mise en oeuvre du test
**********************

Ce test est exécuté en lançant la commande :
ctest -R apTvAOTEstimation_MultiTemporalNominal

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
ST-169-AOT-0010 (C) ; ST-169-AOT-0100 (C) ; ST-169-AOT-0200 (C) ; ST-169-AOT-0300 (C) ;
ST-169-AOT-0500 (C) ; ST-169-AOT-0700 (C) ; ST-169-AOT-0800 (C) ; ST-169-AOT-0600 (C) ;
ST-169-AOTOversampling-0010 (C) ;
