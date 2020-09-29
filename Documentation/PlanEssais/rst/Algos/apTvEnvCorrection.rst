apTvEnvCorrection
~~~~~~~~~~~~~~~~~~~~

Objectif
********
Validation de l'application "EnvCorrection"

Description
***********

Le test "EnvCorrection" applique la correction envirronemantale à une image.

Le module EnvironmentCorrection corrige l'image de réflectance de surface (issue de "Scattering Correction") des effets de l'environnement. Cette correction s'appuie sur les LUT Tdir, Tdift et Satm ainsi que sur le MNT fourni en entrée.


Liste des données d’entrées
***************************

 - tdir EnvCorrection/Sudouest_20060329_MS_fmsat_ortho_surf_96m_Tdir.tif
 - tdif EnvCorrection/Sudouest_20060329_MS_fmsat_ortho_surf_96m_Tdif.tif
 - albd EnvCorrection/Sudouest_20060329_MS_fmsat_ortho_surf_96m_Satm.tif
 - toc EnvCorrection/Sudouest_20060329_MS_fmsat_ortho_surf_96m_toc.tif
 - edg EnvCorrection/Sudouest_20060329_MS_fmsat_ortho_surf_96m_edg.tif
 - rhoenvsub apTvComputeRhoEnv.tif
 - nodata -10
       

Liste des produits de sortie
****************************

 - sre ${MAJA_TEST_OUTPUT_ROOT}/TVA/apTvEnvCorrection_SRE.tif
 - rhoenv ${MAJA_TEST_OUTPUT_ROOT}/TVA/apTvEnvCorrection_RhoEnv.tif


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
Le test génère en sortie l'image corrigée.

Mise en oeuvre du test
**********************

Ce test est exécuté en lançant la commande :
ctest -R apTvEnvCorrection

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
ST-169-EnvironmentCorr-0010(C) ;ST-169-EnvironmentCorr-0020(C) ;ST-169-EnvironmentCorr-0030 (C) ; ST-169-EnvironmentCorr-0040 (C) ; ST-169-EnvironmentCorr-0050(C) ;
