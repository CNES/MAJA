apTvComputeRhoEnv
~~~~~~~~~~~~~~~~~

Objectif
********
Validation de l'application "ComputeRhoEnv"

Description
***********

Le module "RhoEnv" calcul le Rho pour la correction environnementale.


Liste des données d’entrées
***************************

- toc EnvCorrection/Sudouest_20060329_MS_fmsat_ortho_surf_96m_tocSub.tif
- edg EnvCorrection/Sudouest_20060329_MS_fmsat_ortho_surf_96m_edgSub.tif
- radius 10
- filtercoeff 1
                                   

Liste des produits de sortie
****************************

- rhoenv TVA/apTvComputeRhoEnv.tif


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
Le test génère en sortie les images composites

Mise en oeuvre du test
**********************
Ce test est exécuté en lançant la commande :
ctest -R apTvComputeRhoEnv

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

