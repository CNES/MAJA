pyTv-S2-L2NOMINAL-001-SENTINEL2-ALGO_CHAIN
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Titre
*****
Validation fonctionnelle et numérique de la chaîne L2 (Pre-PostProcessing, entrée/sorties) avec un produit SENTINEL2 natif PDGS en mode NOMINAL. Les paramètres de la couche Standalone sont passés en ligne de commande (pas de JobOrder).

Objectif
********

L’objectif de cet essai est de valider le fonctionnement global de la chaîne L2 en mode NOMINAL sur des produits SENTINEL2 natifs PDGS comportant différentes résolutions.
Plus particulièrement dans le cadre du développement d’un plug-in SENTINEL2 natif PDGS, ce test permet de vérifier que la chaine est capable de relire un produits L2 au format déjà
développé pour le plug-in SENTINEL2 mais généré par ce plug-in.
Les produits L1 fournis en entrée sont conformes au document S2-PDGS-TAS-DI-PSD (12) (DA15).
Les produits L2 fournis en entrée et générés en sortie sont conforme au format du plug-in Sentinel-2.


Description
***********

Les options de traitement sont :

- Méthode SPECTROTEMPORAL pour l’estimation des aérosols,
- Correction de l’environnement et des pentes,
- Ecriture à la résolution L2.



Liste des données d’entrées
***************************

Produits :

- S2A_OPER_PRD_MSIL1C_PDMC_20161109T171237_R135_V20160924T074932_20160924T081448.SAFE
- S2A_OPER_SSC_L2VALD_36JTT____20160914.HDR                            
- S2__TEST_AUX_REFDE2_36JTT____5001.DBL
- S2__TEST_AUX_REFDE2_36JTT____5001.DBL.DIR


Gipps:

- S2A_TEST_GIP_L2DIFT_L_CONTINEN_50003_20150703_21000101.DBL    
- S2A_TEST_GIP_L2WATV_L_CONTINEN_50003_20150703_21000101.DBL.DIR
- S2A_OPER_SSC_L2VALD_36JTT____20160914.DBL                       
- S2A_TEST_GIP_L2DIFT_L_CONTINEN_50003_20150703_21000101.DBL.DIR 
- S2A_TEST_GIP_L2WATV_L_CONTINEN_50003_20150703_21000101.HDR
- S2A_OPER_SSC_L2VALD_36JTT____20160914.DBL.DIR          
- S2A_TEST_GIP_L2DIFT_L_CONTINEN_50003_20150703_21000101.HDR  
- S2A_TEST_GIP_L2DIRT_L_CONTINEN_50003_20150703_21000101.DBL  
- S2A_TEST_GIP_CKEXTL_S_36JTT____50001_00000000_99999999.EEF             
- S2A_TEST_GIP_L2DIRT_L_CONTINEN_50003_20150703_21000101.DBL.DIR 
- S2A_TEST_GIP_CKQLTL_S_36JTT____50001_00000000_99999999.EEF                 
- S2A_TEST_GIP_L2DIRT_L_CONTINEN_50003_20150703_21000101.HDR  
- S2__TEST_EXO_METDTA_20090101T000000_20111231T230000.DBL
- S2A_TEST_GIP_L2ALBD_L_CONTINEN_50004_20150703_21000101.DBL            
- S2A_TEST_GIP_L2SMAC_L_ALLSITES_50003_20150703_21000101.EEF
- S2__TEST_EXO_METDTA_20090101T000000_20111231T230000.DBL.BIN
- S2A_TEST_GIP_L2ALBD_L_CONTINEN_50004_20150703_21000101.DBL.DIR           
- S2A_TEST_GIP_L2TOCR_L_CONTINEN_50003_20150703_21000101.DBL 
- S2__TEST_EXO_METDTA_20090101T000000_20111231T230000.HDR
- S2A_TEST_GIP_L2ALBD_L_CONTINEN_50004_20150703_21000101.HDR           
- S2A_TEST_GIP_L2TOCR_L_CONTINEN_50003_20150703_21000101.DBL.DIR 
- S2__TEST_GIP_L2SITE_S_36JTT____50001_00000000_99999999.EEF
- S2A_TEST_GIP_L2TOCR_L_CONTINEN_50003_20150703_21000101.HDR    
- S2A_TEST_GIP_L2COMM_L_ALLSITES_50008_20150703_21000101.EEF           
- S2A_TEST_GIP_L2WATV_L_CONTINEN_50003_20150703_21000101.DBL



Liste des produits de sortie
****************************

Produit L2A SENTINEL2 Natif

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

Les tests COMP_ASCII et COMP_IMAGE associés permettent de valider la non regression.

Mise en oeuvre du test
**********************

Ce test est exécuté en lançant la commande :
ctest -R pyTv-S2-L2NOMINAL-001-SENTINEL2-ALGO_CHAIN

Exigences
*********
Ce test couvre les exigences suivantes :
MACCS-Exigence 50 (C) ; MACCS-Exigence 550 (C) ; MACCS-Exigence 630 (C) ;



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

