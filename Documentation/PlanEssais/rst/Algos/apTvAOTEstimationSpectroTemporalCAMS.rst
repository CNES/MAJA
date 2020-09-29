apTvAOTEstimationMultiTemporalCAMS
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Objectif
********
Validation de l'application "AOTEstimation" en mode nominal Spectro-Temporel en utilisant des données CAMS

Description
***********

Le test "AOTEstimationMultiTemporalCAMS" calcul la valeur de l'AOT en mode nominal Spectro-Temporel en utilisant les données CAMS.Le module AOTEstimation génère une image contenant pour chaque pixel, l'épaisseur optique des aérosols estimée.


Liste des données d’entrées
***************************

- toac AOTEstimation/SpectroTemporalCAMS/normal/20160614/PRIVE/toa_abs/31TFJR108_20160614_S2A_L1Csimu_toa_240m_coeff.tif
- shd AOTEstimation/SpectroTemporalCAMS/normal/20160614/PRIVE/mnt/31TFJR108_20160614_S2A_L1Csimu_toa_240m.shd
- dtm DTM/SENTINEL2/S2__OPER_AUX_REFDE2_31TFJ____0001.DBL.DIR/S2__OPER_AUX_REFDE2_31TFJ____0001_ALC.tif
- cld AOTEstimation/SpectroTemporalCAMS/normal/20160614/PRIVE/nua/31TFJR108_20160614_S2A_L1Csimu_toa_240m.nua
- l2ndt AOTEstimation/SpectroTemporalCAMS/MasqueNoir.tif
- l2rtc AOTEstimation/SpectroTemporalCAMS/normal/20160607/PRIVE/surf_comp/31TFJR008_20160607_S2A_L1Csimu_surf_comp_240m_coeff.tif
- edg AOTEstimation/SpectroTemporalCAMS/normal/20160614/PRIVE/bord/31TFJR108_20160614_S2A_L1Csimu_toa_bord_240m.hdr
- lutheader  AOTEstimation/SpectroTemporalCAMS/giplut.xml
- lutimage  AOTEstimation/SpectroTemporalCAMS/lutAOT.mha
- mode spectrotemporal
- xmlparamsfile AOTEstimation/SpectroTemporalCAMS/AOTParams_SENTINEL2.xml
- temporal.altitudemean 182.243020728
- reall2nodata -10
- jday 11853
- darkestaot 0.123605
- temporal.lutmap AOTEstimation/SpectroTemporalCAMS/giplutmap.xml
- sat AOTEstimation/SpectroTemporalCAMS/MasqueNoir.tif
- l2rta AOTEstimation/SpectroTemporalCAMS/normal/20160607/PRIVE/toa_comp/31TFJR008_20160607_S2A_L1Csimu_toa_comp_240m_coef.tif
- l2pxd AOTEstimation/SpectroTemporalCAMS/normal/20160607/PRIVE/date/31TFJR008_20160607_S2A_L1Csimu_toa_240m.date
- was AOTEstimation/SpectroTemporalCAMS/normal/20160614/PRIVE/water/31TFJR108_20160614_S2A_L1Csimu_toa_240m.water
- snow AOTEstimation/SpectroTemporalCAMS/normal/20160614/PRIVE/nei/31TFJR108_20160614_S2A_L1Csimu_toa_240m.nei
- cams.aot AOTEstimation/SpectroTemporalCAMS/ReduceLutComputationCAMSTest001-AOT.tif
- cams.altmean 182.243020728
- cams.nodatatau -1
 


Liste des produits de sortie
****************************

- aotmask TVA/apTvAOTEstimationSpectrotemporalCams_aotmask.tif
- aotcomputed TVA/apTvAOTEstimationSpectrotemporalCams_aotcomputed.tif
- aotcams TVA/apTvAOTEstimationSpectrotemporalCams_aotcams.tif


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
ctest -R apTvAOTEstimationMultiTemporalCAMS

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

