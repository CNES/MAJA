apTvDispatchZoneToVector
~~~~~~~~~~~~~~~~~~~~~~~~

Objectif
********
Validation de l'application "DispatchZoneToVector" avec des images conforme en entrée

Description
***********

Le module "DispatchZoneToVector" déconcaténe plusieurs images contenant chacune une zone mais plusieurs bande en plusieurs bandes contenant chacune leurs zones respectives. Cette application est spécique au masque DETF de Sentinel2 format muscate. Cette version du test considéere que les bande suivent correctement l'ordre, il ne manque pas de bande pour un détécteur donné.


Liste des données d’entrées
***************************

- Plusieurs images definissant les zones mais contenant chacune plusieurs bandes répartie sur plusieurs bit, par exemple un fichier S2_Muscate : SENTINEL2B_20171008-105012-463_L2A_T31TCH_C_V1-0_DTF_R3-D06.tif. 


Liste des produits de sortie
****************************

Image dont chaque bande contiens ses zones couverte (DETFOO par exemple).

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
Le test génère en sortie une image dont la répartition est selon les images de zone d'entrée pour chaque bande.

Mise en oeuvre du test
**********************

Ce test est exécuté en lançant la commande :
ctest -R apTvDispatchZoneToVector

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
Néant
