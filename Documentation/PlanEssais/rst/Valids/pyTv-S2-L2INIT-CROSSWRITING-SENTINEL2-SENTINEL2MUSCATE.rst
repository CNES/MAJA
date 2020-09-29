pyTv-S2-L2INIT-CROSSWRITING-SENTINEL2-SENTINEL2MUSCATE
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Titre
*****
Validation fonctionnelle et performances de la chaîne L2 en mode INIT avec un produit SENTINEL2 Natif mais produisant un SENTINEL2 Muscate (mode INIT).

Objectif
********
L'objectif de ce test est de validé le traitement en mode INIT d'un produit SENTINEL2 Natif et de produire en sortie un produit SENTINEL2 Muscate.


Description
***********
Instance INIT de SENTINEL2 Natif afin de produire un SENTINEL2 Muscate


Liste des données d’entrées
***************************

Produits:

- S2__TEST_AUX_REFDE2_36JTT____5001.DBL
- S2__TEST_AUX_REFDE2_36JTT____5001.HDR
- S2A_MSIL1C_20160914T074612_N0204_R135_T36JTT_20160914T081456.SAFE 

Gipps

- S2A_TEST_GIP_L2DIFT_L_CONTINEN_50003_20150703_21000101.DBL      
- S2A_TEST_GIP_L2TOCR_L_CONTINEN_50003_20150703_21000101.DBL.DIR 
- S2__TEST_EXO_METDTA_20090101T000000_20111231T230000.DBL
- S2A_TEST_GIP_CKEXTL_S_36JTT____50001_00000000_99999999.EEF         
- S2A_TEST_GIP_L2DIFT_L_CONTINEN_50003_20150703_21000101.DBL.DIR  
- S2A_TEST_GIP_L2TOCR_L_CONTINEN_50003_20150703_21000101.HDR     
- S2__TEST_EXO_METDTA_20090101T000000_20111231T230000.DBL.BIN
- S2A_TEST_GIP_CKQLTL_S_36JTT____50001_00000000_99999999.EEF        
- S2A_TEST_GIP_L2DIFT_L_CONTINEN_50003_20150703_21000101.HDR      
- S2A_TEST_GIP_L2WATV_L_CONTINEN_50003_20150703_21000101.DBL      
- S2__TEST_EXO_METDTA_20090101T000000_20111231T230000.HDR
- S2A_TEST_GIP_L2ALBD_L_CONTINEN_50004_20150703_21000101.DBL         
- S2A_TEST_GIP_L2DIRT_L_CONTINEN_50003_20150703_21000101.DBL      
- S2A_TEST_GIP_L2WATV_L_CONTINEN_50003_20150703_21000101.DBL.DIR 
- S2__TEST_GIP_L2SITE_S_36JTT____50001_00000000_99999999.EEF
- S2A_TEST_GIP_L2ALBD_L_CONTINEN_50004_20150703_21000101.DBL.DIR     
- S2A_TEST_GIP_L2DIRT_L_CONTINEN_50003_20150703_21000101.DBL.DIR  
- S2A_TEST_GIP_L2WATV_L_CONTINEN_50003_20150703_21000101.HDR      
- S2A_TEST_GIP_L2ALBD_L_CONTINEN_50004_20150703_21000101.HDR        
- S2A_TEST_GIP_L2DIRT_L_CONTINEN_50003_20150703_21000101.HDR      
- S2A_TEST_GIP_L2SMAC_L_ALLSITES_50003_20150703_21000101.EEF      
- S2__TEST_AUX_REFDE2_36JTT____5001.DBL.DIR
- S2A_TEST_GIP_L2COMM_L_ALLSITES_50008_20150703_21000101.EEF        
- S2A_TEST_GIP_L2TOCR_L_CONTINEN_50003_20150703_21000101.DBL      



Liste des produits de sortie
****************************
Produit L2A SENTINEL2 Muscate

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
ctest -R pyTv-S2-L2INIT-CROSSWRITING-SENTINEL2-SENTINEL2MUSCATE



Exigences
*********
Ce test couvre les exigences suivantes :
Néant


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

