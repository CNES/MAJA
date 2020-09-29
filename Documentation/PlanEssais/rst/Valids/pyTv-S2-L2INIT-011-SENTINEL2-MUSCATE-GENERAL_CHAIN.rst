pyTv-S2-L2INIT-011-SENTINEL2-MUSCATE-GENERAL_CHAIN
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Titre
*****
Validation fonctionnelle et numérique de la chaîne L2 (Pre-PostProcessing, entrée/sorties) avec un produit L1C SENTINEL2 S2A au format Muscate en mode INIT.



Objectif
********
L’objectif de cet essai est de valider le fonctionnement global de la chaîne L2 avec un produit SENTINEL2 au format Muscate. 


Description
***********

Les options de traitement sont :

- Méthode SPECTROTEMPORAL pour l’estimation des aérosols,
- Correction de l’environnement et des pentes,
- Ecriture aux différentes résolutions L2,
- activation de la correction des cirrus
- Correction Bidirectionnelle sur la minilut


Liste des données d’entrées
***************************

Les images utilisées en entrée sont les suivantes :

- SENTINEL2B_20171008-105012-463_L1C_T31TCH_C_V1-0/SENTINEL2B_20171008105012-463_L1C_T31TCH_C_V1-0_MTD_ALL.xml
- S2__TEST_AUX_REFDE2_T31TCH_0001.DBL

Les fichiers GIPPs utilisés en entrée sont les suivants :

- S2__TEST_GIP_L2SITE_S_31TJF____10001_00000000_99999999.EEF
- S2B_TEST_GIP_L2ALBD_L_CONTINEN_10003_20150703_21000101.DBL
- S2B_TEST_GIP_L2COMM_L_ALLSITES_10008_20150703_21000101.EEF
- S2B_TEST_GIP_L2DIFT_L_CONTINEN_10002_20150703_21000101.DBL
- S2B_TEST_GIP_L2DIRT_L_CONTINEN_10002_20150703_21000101.DBL
- S2B_TEST_GIP_L2SMAC_L_ALLSITES_10001_20150703_21000101.EEF
- S2B_TEST_GIP_L2TOCR_L_CONTINEN_10002_20150703_21000101.DBL
- S2B_TEST_GIP_L2WATV_L_CONTINEN_10002_20150703_21000101.DBL
- S2B_TEST_GIP_CKEXTL_S_31TJF____10001_20150703_21000101.EEF
- S2B_TEST_GIP_CKQLTL_S_31TJF____10005_20150703_21000101.EEF


Liste des produits de sortie
****************************

Produit SENTINEL2 au format Muscate

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
ctest -R pyTv-S2-L2INIT-011-SENTINEL2-MUSCATE-GENERAL_CHAIN


Exigences
*********
Ce test couvre les exigences suivantes :
MACCS-Exigence 640 (C) ; MACCS-Exigence 650 (C) ; MACCS-Exigence 50 (C) ; MACCS-Exigence 630 (C) ;




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

