apTvDispatchZoneToVectorWeight
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Objectif
********
Validation de l'application "DispatchZoneToVector" avec des images non conforme en entrée

Description
***********

Le module "DispatchZoneToVector" déconcaténe plusieurs images contenant chacune une zone mais plusieurs bande en plusieurs bandes contenant chacune leurs zones respectives. Cette application est spécique au masque DETF de Sentinel2 format muscate. Cette version du test considére que les bande ne suivent pas correctement l'ordre, il manque des bande pour un détécteur donné. Il arrive qu'une certaine bande ne touche pas un détécteur.


Liste des données d’entrées
***************************

- Plusieurs images definissant les zones mais contenant chacune plusieurs bandes répartie sur plusieurs bit, par exemple un fichier S2_Muscate : SENTINEL2B_20171008-105012-463_L2A_T31TCH_C_V1-0_DTF_R3-D06.tif.Les images n'ont pas la bonne liste de bande et les bits ne sont pas dans le bon ordre.  


Liste des produits de sortie
****************************

Image dont chaque bande contiens ses zones couverte (DETFOO par exemple). Une des bandes ne contiens pas tous le détécteur car non presente sur celui-ci

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
Le test génère en sortie une image dont la répartition est selon les images de zone d'entrée pour chaque bande. Une des bandes ne contiens pas tous le détécteur car non presente sur celui-ci

Mise en oeuvre du test
**********************

Ce test est exécuté en lançant la commande :
ctest -R apTvDispatchZoneToVectorWeight

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
