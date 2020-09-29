pyTv-S2-L2BACKWARD-011-F-N-SENTINEL2_MUSCATE-GENERAL_CHAIN
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Titre
*****

Validation fonctionnelle et numérique de la chaîne L2 (Pre-PostProcessing, entrée/sorties) avec des produits SENTINEL2 Muscate en mode Backward, avec AOT en mode SPECTROTEMPORAL.

Objectif
********


L’objectif de cet essai est de valider le fonctionnement global de la chaîne L2 avec des produits SENTINEL2 au format Muscate.
Cet essai enchaîne les traitements de Pre/Post processing et l’exécution de la chaîne L2 en mode BACKWARD (L2BACK).
Le mode BACKWARD doit lancer une première fois la chaîne en mode INIT sur l’image la plus récente au format PDGS conforme à DA15 puis basculer en mode NOMINAL pour traiter toutes les images précédentes jusqu’à traiter la première image et généré le produit L2 conformément au format déjà développé pour les produits L2 des plug-ins Muscate.
Ce test permet donc de contrôler l’enchaînement des différents modes. Il permet aussi de contrôler que toutes les images intermédiaires (dates D-N à D-1) sont bien produites à la résolution réduite uniquement et que la chaîne est capable de relire les produits générés par ce nouveau plug-in SENTINEL2.

Description
***********

Les options de traitement sont :

- Méthode d’estimation des aérosols : SPECTROTEMPORAL
- Pas d’extension du masque des nuages,
- Lancement des traitements de correction de l’environnement et des pentes,
- Ecriture à la résolution L2.



Liste des données d’entrées
***************************

Les images utilisées en entrée sont les suivantes :

- SENTINEL2B_20171008-105012-463_L1C_T31TCH_C_V1-0/SENTINEL2B_20171008105012-463_L1C_T31TCH_C_V1-0_MTD_ALL.xml
- SENTINEL2B_20171010-105012-463_L1C_T31TCH_C_V1-0/SENTINEL2B_20171010105012-463_L1C_T31TCH_C_V1-0_MTD_ALL.xml
- S2__TEST_AUX_REFDE2_T31TCH_0001.DBL

Les fichiers GIPPs utilisés en entrée sont les suivants :

- S2__TEST_GIP_L2SITE_S_31TJF____10001_00000000_99999999.EEF
- S2A_TEST_GIP_L2ALBD_L_CONTINEN_10003_20150703_21000101.DBL
- S2B_TEST_GIP_L2ALBD_L_CONTINEN_10003_20150703_21000101.DBL
- S2A_TEST_GIP_L2COMM_L_ALLSITES_10008_20150703_21000101.EEF
- S2B_TEST_GIP_L2COMM_L_ALLSITES_10008_20150703_21000101.EEF
- S2A_TEST_GIP_L2DIFT_L_CONTINEN_10002_20150703_21000101.DBL
- S2B_TEST_GIP_L2DIFT_L_CONTINEN_10002_20150703_21000101.DBL
- S2A_TEST_GIP_L2DIRT_L_CONTINEN_10002_20150703_21000101.DBL
- S2B_TEST_GIP_L2DIRT_L_CONTINEN_10002_20150703_21000101.DBL
- S2A_TEST_GIP_L2SMAC_L_ALLSITES_10001_20150703_21000101.EEF
- S2B_TEST_GIP_L2SMAC_L_ALLSITES_10001_20150703_21000101.EEF
- S2A_TEST_GIP_L2TOCR_L_CONTINEN_10002_20150703_21000101.DBL
- S2B_TEST_GIP_L2TOCR_L_CONTINEN_10002_20150703_21000101.DBL
- S2A_TEST_GIP_L2WATV_L_CONTINEN_10002_20150703_21000101.DBL
- S2B_TEST_GIP_L2WATV_L_CONTINEN_10002_20150703_21000101.DBL
- S2A_TEST_GIP_CKEXTL_S_31TJF____10001_20150703_21000101.EEF
- S2B_TEST_GIP_CKEXTL_S_31TJF____10001_20150703_21000101.EEF
- S2A_TEST_GIP_CKQLTL_S_31TJF____10005_20150703_21000101.EEF
- S2B_TEST_GIP_CKQLTL_S_31TJF____10005_20150703_21000101.EEF


Liste des produits de sortie
****************************

Produit L2A au format SENTINEL2 Muscate

Prérequis
*********
Il n’y a pas de prérequis.

Durée attendue
***************
La durée d’exécution de l’essai n’est pas un critère attendu.

Epsilon utilisé sur la non regression
*************************************
0.0001

Vérifications à effectuer
**************************

Les tests COMP_ASCII et COMP_IMAGE associés permettent de valider la non regression

Mise en oeuvre du test
**********************

Ce test est exécuté en lançant la commande :
ctest -R pyTv-S2-L2BACKWARD-011-F-N-SENTINEL2_MUSCATE-GENERAL_CHAIN


Exigences
*********

Ce test couvre les exigences suivantes :

MACCS-Exigence 640 (C) ; MACCS-Exigence 650 (C) ;MACCS-Exigence 30 (C) ; MACCS-Exigence 50 (C) ; MACCS-Exigence 70 (C) ; ST-169-Level1-0010 (C) ; ST-169-Level1-0030 (C) ; ST-169-Mode-0010 (C) ; ST-169-L1toL2-0010 (C) ;ST-169-DTM-0010 (C) ; ST-169-DTM-0020 (C) ; ST-169-DTM-0030 (C) ; ST-169-DTM-0040(C) ; ST-169-DTM-0050 (C) ; ST-169-Subsampling-0010 (C) ; ST-169-Subsampling-0020 (C) ;ST-169-Subsampling-0030 (C) ; ST-169-Subsampling-0040 (C) ; ST-169-Subsampling-0050 (C) ;ST-169-Subsampling-0060 (C) ; ST-169-AbsorptionCorr-0010 (C) ; ST-169-AbsorptionCorr-0030(C) ; ST-169-AbsorptionCorr-0040 (C) ; ST-169-AbsorptionCorr-0050 (C) ; ST-169-RayleighCorr-0010 (C) ; ST-169-GeometricFlags-0010 (C) ; ST-169-GeometricFlags-0020 (C) ; ST-169-GeometricFlags-0030 (C) ; ST-169-CirrusMaskS2-0010 (C) ; ST-169-CloudMasking-0010 (C) ;ST-169-CloudMasking-0100 (C) ; ST-169-CloudMasking-0200 (C) ; ST-169-CloudMasking-0500(C) ; ST-169-CloudMasking-0600 (C) ; ST-169-CloudMasking-0700 (C) ; ST-169-CloudMasking-1000 (C) ; ST-169-CloudMasking-1600 (C) ; ST-169-CloudMasking-1700 (C) ; ST-169-CloudMasking-0300 (C) ; ST-169-CloudMasking-0400 (C) ; ST-169-CloudMasking-0410 (C) ; ST-169-CloudMasking-0420 (C) ; ST-169-CloudMasking-0430 (C) ; ST-169-CloudMasking-1100 (C) ;ST-169-CloudMasking-1110 (C) ; ST-169-CloudMasking-1120 (C) ; ST-169-CloudMasking-1130(C) ; ST-169-CloudMasking-1140 (C) ; ST-169-CloudMasking-1150 (C) ; ST-169-CloudMasking-1160 (C) ; ST-169-CloudMasking-1170 (C) ; ST-169-CloudMasking-1180 (C) ; ST-169-CloudMasking-1200 (C) ; ST-169-CloudMasking-1300 (C) ; ST-169-CloudMasking-1400 (C) ; ST-169-CloudMasking-1500 (C) ; ST-169-CloudMasking-1510 (C) ; ST-169-CloudMasking-1520 (C) ;ST-169-WaterMasking-0010 (C) ; ST-169-WaterMasking-0020 (C) ; ST-169-WaterMasking-0030(C) ; ST-169-WaterMasking-0040 (C) ; ST-169-WaterMasking-0050 (C) ; ST-169-WaterMasking-0060 (C) ; ST-169-RainDetection-0010 (C) ; ST-169-AOT-0010 (C) ; ST-169-AOT-0100 (C) ;ST-169-AOT-0200 (C) ; ST-169-AOT-0300 (C) ; ST-169-AOT-0500 (C) ; ST-169-AOT-0700(C) ; ST-169-AOT-0800 (C) ; ST-169-AOT-0600 (C) ; ST-169-AOTOversampling-0010 (C) ;ST-169-SactteringCorr-0010 (C) ; ST-169-Composite-0010 (C) ; ST-169-Composite-0020 (C) ;ST-169-Composite-0030 (C) ; ST-169-Composite-0040 (C) ; ST-169-Composite-0050 (C) ;




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

