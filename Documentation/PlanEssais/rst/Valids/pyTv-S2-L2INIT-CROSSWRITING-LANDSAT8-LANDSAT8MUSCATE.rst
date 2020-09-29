pyTv-S2-L2INIT-CROSSWRITING-LANDSAT8-LANDSAT8MUSCATE
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Titre
*****
Validation fonctionnelle et performances de la chaîne L2 en mode INIT avec un produit LANDSAT8 Natif mais produisant un LANDSAT8 Muscate (mode INIT).

Objectif
********
L'objectif de ce test est de validé le traitement en mode INIT d'un produit LANDSAT8 Natif et de produire en sortie un produit LANDSAT8 Muscate.


Description
***********
Instance INIT de LANDSAT8 Natif afin de produire un LANDSAT8 Muscate


Liste des données d’entrées
***************************

Produits:

- L8_TEST_AUX_REFDE2_199029_0001.DBL                              
- L8_TEST_AUX_REFDE2_199029_0001.DBL.DIR                           
- L8_TEST_AUX_REFDE2_199029_0001.HDR                               
- LC08_L1TP_199029_20170527_20170615_01_T1

Gipps:

- L8_TEST_GIP_CKQLTL_S_EU93056200A00B_50001_00000000_99999999.EEF  
- L8_TEST_GIP_L2DIFT_L_CONTINEN_50001_00000000_99999999.HDR       
- L8_TEST_GIP_L2TOCR_L_CONTINEN_50001_00000000_99999999.DBL.DIR
- L8_TEST_GIP_L2ALBD_L_CONTINEN_50001_00000000_99999999.DBL        
- L8_TEST_GIP_L2DIRT_L_CONTINEN_50001_00000000_99999999.DBL        
- L8_TEST_GIP_L2TOCR_L_CONTINEN_50001_00000000_99999999.HDR
- L8_TEST_GIP_L2ALBD_L_CONTINEN_50001_00000000_99999999.DBL.DIR    
- L8_TEST_GIP_L2DIRT_L_CONTINEN_50001_00000000_99999999.DBL.DIR    
- L8_TEST_EXO_METDTA_20090101T000001_20111231T230000.DBL           
- L8_TEST_GIP_L2ALBD_L_CONTINEN_50001_00000000_99999999.HDR        
- L8_TEST_GIP_L2DIRT_L_CONTINEN_50001_00000000_99999999.HDR        
- L8_TEST_EXO_METDTA_20090101T000001_20111231T230000.DBL.BIN       
- L8_TEST_GIP_L2COMM_L_ALLSITES_50001_00000000_99999999.EEF        
- L8_TEST_GIP_L2SITE_S_EU93056200A00B_50001_00000000_99999999.EEF
- L8_TEST_EXO_METDTA_20090101T000001_20111231T230000.HDR           
- L8_TEST_GIP_L2DIFT_L_CONTINEN_50001_00000000_99999999.DBL        
- L8_TEST_GIP_L2SMAC_L_ALLSITES_50001_00000000_99999999.EEF
- L8_TEST_GIP_CKEXTL_S_EU93056200A00B_50001_00000000_99999999.EEF  
- L8_TEST_GIP_L2DIFT_L_CONTINEN_50001_00000000_99999999.DBL.DIR    
- L8_TEST_GIP_L2TOCR_L_CONTINEN_50001_00000000_99999999.DBL




Liste des produits de sortie
****************************
Produit L2A LANDSAT8 Muscate

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
ctest -R pyTv-S2-L2INIT-CROSSWRITING-LANDSAT8-LANDSAT8MUSCATE


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

