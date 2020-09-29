pyTv-S2-L2INIT-CROSSWRITING-VENUS-VENUSMUSCATE
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Titre
*****
Validation fonctionnelle et performances de la chaîne L2 en mode INIT avec un produit VENUS Natif mais produisant un VENUS Muscate (mode INIT).

Objectif
********
L'objectif de ce test est de validé le traitement en mode INIT d'un produit VENUS Natif et de produire en sortie un produit VENUS Muscate.


Description
***********
Instance INIT de VENUS Natif afin de produire un VENUS Muscate


Liste des données d’entrées
***************************

Produits:

- VE_TEST_AUX_REFDE2_CAMARGUE_0001.DBL.DIR               
- VE_TEST_AUX_REFDE2_CAMARGUE_0001.HDR                 
- VE_VM01_VSC_L1VALD__PE_R_U__20120101.DBL.DIR
- VE_VM01_VSC_L1VALD__PE_R_U__20120101.HDR

Gipps

- VE_TEST_GIP_CKQLTL_S_CAMARGUE_00001_00000000_99999999.EEF 
- VE_TEST_GIP_L2DIRT_L_CONTINEN_00001_00000000_99999999.DBL.DIR 
- VE_TEST_GIP_L2WATV_L_CONTINEN_00001_00000000_99999999.DBL.DIR
- VE_TEST_GIP_L2ALBD_L_CONTINEN_00001_00000000_99999999.DBL.DIR 
- VE_TEST_GIP_L2DIRT_L_CONTINEN_00001_00000000_99999999.HDR     
- VE_TEST_GIP_L2WATV_L_CONTINEN_00001_00000000_99999999.HDR
- VE_TEST_GIP_L2ALBD_L_CONTINEN_00001_00000000_99999999.HDR    
- VE_TEST_GIP_L2SITE_S_CAMARGUE_00001_00000000_99999999.EEF    
- VE_TEST_EXO_METDTA_20110101T000000_20111231T230000.DBL.BIN 
- VE_TEST_GIP_L2COMM_L_ALLSITES_00001_00000000_99999999.EEF  
- VE_TEST_GIP_L2SMAC_L_ALLSITES_00001_00000000_99999999.EEF 
- VE_TEST_EXO_METDTA_20110101T000000_20111231T230000.HDR    
- VE_TEST_GIP_L2DIFT_L_CONTINEN_00001_00000000_99999999.DBL.DIR 
- VE_TEST_GIP_L2TOCR_L_CONTINEN_00001_00000000_99999999.DBL.DIR
- VE_TEST_GIP_CKEXTL_S_CAMARGUE_00001_00000000_99999999.EEF 
- VE_TEST_GIP_L2DIFT_L_CONTINEN_00001_00000000_99999999.HDR  
- VE_TEST_GIP_L2TOCR_L_CONTINEN_00001_00000000_99999999.HDR



Liste des produits de sortie
****************************
Produit L2A Venus Muscate

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
ctest -R pyTv-S2-L2INIT-CROSSWRITING-VENUS-VENUSMUSCATE



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

