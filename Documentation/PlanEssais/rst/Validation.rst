LES ESSAIS DE VALIDATION
=====================================

Objectifs
----------

Les essais menés durant la phase d’intégration et de validation sont des essais dits fonctionnels et/ou numériques. Ils permettent de tester toutes les fonctionnalités du logiciel avec variation des paramètres d’entrée et des contextes d’utilisation. Ils couvrent les cas d’activation nominaux, les cas dégradés, les tests de performances. Ils permettent de répondre à la question « est-ce que le logiciel répond bien au besoin ? ».

L’objectif général de la validation est de garantir que le ou les composants implémentés fournissent des résultats exacts. Des tests de non-régression sont également mis en place afin de garantir l’exactitude des résultats au fur et à mesure des développements. Cette phase de validation permet notamment de : 

- Vérifier l’adéquation des fonctionnalités offertes par le système aux exigences,
- Garantir, en complément des différents essais réalisés au niveau des composants, une vérification des exigences compréhensible par l’utilisateur,
-  Valider les interfaces avec les entités externes,
-  Pérenniser un ensemble de scripts/jeux de données/résultats pour chaque essai afin de pouvoir les rejouer ultérieurement en cas d’évolution du système,
- Apprêter le système pour son entrée en recette.


Description
-----------

Le processus de validation est itératif. Il est initialisé pendant la phase de conception, précisé tout au long de la progression du développement, et complété pour la livraison sur site. Le processus est composé des étapes suivantes :

- Définition des tests : cette activité consiste à prévoir les tests qui devront démontrer que le système est conforme aux exigences utilisateur. Cette définition est réalisée en deux étapes :

 * La première consiste à produire un plan de validation. Elle s’attachera notamment à définir et identifier les essais unitaires et données constituant le plan de validation. Cette étape est réalisée durant les phases de conception et éventuellement complété au cours des développements, 
 * La deuxième consiste à produire les plans d’essais détaillés (PED) de chacun des essais. Ces PED contiennent toutes les informations décrites section 3.6.1.1,

- Vérification de la couverture des essais : cette activité est réalisée avec l’outil Reqtify, et une matrice de couverture « exigence/cas de tests » est produite.
- Les étapes d’exécution sont les suivantes :
- Génération de la version logicielle à valider par l’équipe projet à partir des sources gérés par l’équipe de gestion en configuration CS (la procédure de génération fait l’objet elle-même d’un test),
- Installation du logiciel (l’installation fait l’objet elle-même d’un test),
- Pour chaque essai de validation :
  
  * Installation des contextes d’essais, tels que définis dans le PED de l’essai,
  * Génération automatique du journal d’essai en début d’essai en fonction de sa description dans le PED,
  * Mise en œuvre de l’essai et rédaction de la partie résultats obtenus dans le journal d’essai,
  * Récupération des contextes de fin d’essai et exécution éventuelle de la procédure de vérification des résultats (comparaison du contexte récupéré par rapport à un contexte de référence).
  * Mise à jour du rapport de la campagne d’essais.

La phase de validation conduit à la mise en œuvre de l’ensemble des cas de tests identifiés. Ces cas de tests sont complétés et finalisés lors de cette phase. Les données et outils de validation sont mis en œuvre et adaptés le cas échéant. Les tests et données de validation sont maintenus pendant l’ensemble du cycle de vie du logiciel. Ce processus est tout d’abord mis en œuvre sur la machine de développement à CS sur laquelle est également réalisée la recette usine des chaînes MAJA L2, L3 ainsi que des outils associés. Suite à une phase d’intégration sur la configuration cible CNES, les tests associés à des produits simulés (choix du capteur) pourront être exécutés. La recette site sera réalisée sur la configuration cible CNES.

Formalisation des tests de validation 
--------------------------------------

La nomenclature adoptée pour l’identification des essais d’intégration et de validation est la
suivante :

S2-<traitement>-<num d’essai>-<nature>-<type>-<produit>-<thématique>_<MODE>

Ex : S2-L2INIT-001-F-N-VENUS-CLOUD

Avec :

Traitement identifie le type de traitement et peut prendre les valeurs :

- L2INIT : Chaîne L2 en mode Init,
- L2NOMINAL : Chaîne L2 en mode Nominal,
- L2BACKWARD : Chaîne L2 en mode Backward,

Numéro d’essai :

- 000 : valeur codée sur 3 caractères, valeur unique pour chaque <traitement> 

Nature identifie la nature du test et peut prendre les valeurs :

- F : test fonctionnel,
- N : test numérique,

Type identifie le type de l’essai et peut prendre les valeurs :

- N : nominal,
- D : dégradé,
- L : aux limites

Produit identifie le type de produit image utilisé et peut prendre les valeurs :

- VENUS : produit image Venus,
- FORMOSAT : produit image Formosat-2,
- LANDSAT : produit image Landsat,
- SENTINEL2 : produit image Sentinel2

Thématique est une abréviation du thème autour duquel s’organise le test, sur 10 caractères maxi-
mum.

Le mode peut prendre les valeurs :

- CHAIN : éxecution de la chaine
- COMP_ASCII : non regression des fichiers textes
- COMP_IMAGE : Comparaison des images

LISTE DES ESSAIS
------------------

Ce chapitre présente la liste des essais mis en place pour valider MAJA.

==== ==== =================================================================== ============================================================================================================
num  Val  Id                                                                  Commentaire
==== ==== =================================================================== ============================================================================================================
1     OK  pyTv-S2-L2NOMINAL-001-SENTINEL2-ALGO_CHAIN                          Validation fonctionnelle et numérique de la chaîne L2 avec un produit SENTINEL2 natif PDGS en mode NOMINAL.
2     OK  pyTv-S2-L2NOMINAL-001-SENTINEL2-ALGO_COMP_IMAGE                     Non regression Images du test CHAIN correspondant
3     OK  pyTv-S2-L2NOMINAL-001-SENTINEL2-ALGO_COMP_ASCII                     Non regression Texte du test CHAIN correspondant
4     OK  pyTv-S2-L2INIT-011-SENTINEL2-MUSCATE-GENERAL_CHAIN                  Validation fonctionnelle et numérique avec un produit SENTINEL2 Muscate en mode INIT.
5     OK  pyTv-S2-L2INIT-011-SENTINEL2-MUSCATE-GENERAL_COMP_IMAGE             Non regression Images du test CHAIN correspondant
6     OK  pyTv-S2-L2INIT-011-SENTINEL2-MUSCATE-GENERAL_COMP_ASCII             Non regression Texte du test CHAIN correspondant
7     OK  pyTv-S2-L2INIT-012-SENTINEL2-MUSCATE-CAMS_CHAIN                     Validation fonctionnelle et numérique avec un produit SENTINEL2 Muscate en mode INIT avec CAMS.
8     OK  pyTv-S2-L2INIT-012-SENTINEL2-MUSCATE-CAMS_COMP_IMAGE                Non regression Images du test CHAIN correspondant
9     OK  pyTv-S2-L2INIT-012-SENTINEL2-MUSCATE-CAMS_COMP_ASCII                Non regression Texte du test CHAIN correspondant
10    OK  pyTv-S2-L2INIT-012-SENTINEL2-MUSCATE-NEWCAMS_CHAIN                  Validation fonctionnelle et numérique avec un produit SENTINEL2 Muscate en mode INIT avec NewCAMS.
11    OK  pyTv-S2-L2INIT-012-SENTINEL2-MUSCATE-NEWCAMS_COMP_IMAGE             Non regression Images du test CHAIN correspondant
12    OK  pyTv-S2-L2INIT-012-SENTINEL2-MUSCATE-NEWCAMS_COMP_ASCII             Non regression Texte du test CHAIN correspondant
13    OK  pyTv-S2-L2NOMINAL-011-F-N-SENTINEL2_MUSCATE-GENERAL_CHAIN           Validation fonctionnelle et numérique avec un produit SENTINEL2 Muscate en mode NOMINAL.
14    OK  pyTv-S2-L2NOMINAL-011-F-N-SENTINEL2_MUSCATE-GENERAL_COMP_IMAGE      Non regression Images du test CHAIN correspondant
15    OK  pyTv-S2-L2NOMINAL-011-F-N-SENTINEL2_MUSCATE-GENERAL_COMP_ASCII      Non regression Texte du test CHAIN correspondant
16    OK  pyTv-S2-L2BACKWARD-011-F-N-SENTINEL2_MUSCATE-GENERAL_CHAIN          Validation fonctionnelle et numérique avec un produit SENTINEL2 Muscate en mode BACKWARD.
17    OK  pyTv-S2-L2BACKWARD-011-F-N-SENTINEL2_MUSCATE-GENERAL_COMP_IMAGE     Non regression Images du test CHAIN correspondant
18    OK  pyTv-S2-L2BACKWARD-011-F-N-SENTINEL2_MUSCATE-GENERAL_COMP_ASCII     Non regression Texte du test CHAIN correspondant
19    OK  pyTv-S2-L2INIT-CROSSWRITING-SENTINEL2-SENTINEL2MUSCATE              Validation fonctionnelle et numérique avec un produit SENTINEL2 Natif vers Muscate en mode INIT .
20    OK  pyTv-S2-L2INIT-CROSSWRITING-LANDSAT8-LANDSAT8MUSCATE                Validation fonctionnelle et numérique avec un produit LANDSAT8 Natif vers Muscate en mode INIT .
21    OK  pyTv-S2-L2INIT-CROSSWRITING-VENUS-VENUSMUSCATE                      Validation fonctionnelle et numérique avec un produit VENUS Natif vers Muscate en mode INIT.
==== ==== =================================================================== ============================================================================================================


.. toctree::
	:maxdepth: 5
	:caption: pyTv-S2-L2NOMINAL-001-SENTINEL2-ALGO_CHAIN

	Valids/pyTv-S2-L2NOMINAL-001-SENTINEL2-ALGO_CHAIN

.. toctree::
	:maxdepth: 5
	:caption: pyTv-S2-L2INIT-011-SENTINEL2-MUSCATE-GENERAL_CHAIN

	Valids/pyTv-S2-L2INIT-011-SENTINEL2-MUSCATE-GENERAL_CHAIN


.. toctree::
	:maxdepth: 5
	:caption: pyTv-S2-L2INIT-012-SENTINEL2-MUSCATE-CAMS_CHAIN

	Valids/pyTv-S2-L2INIT-012-SENTINEL2-MUSCATE-CAMS_CHAIN


.. toctree::
	:maxdepth: 5
	:caption: pyTv-S2-L2INIT-012-SENTINEL2-MUSCATE-NEWCAMS_CHAIN 

	Valids/pyTv-S2-L2INIT-012-SENTINEL2-MUSCATE-NEWCAMS_CHAIN

.. toctree::
	:maxdepth: 5
	:caption: pyTv-S2-L2NOMINAL-011-F-N-SENTINEL2_MUSCATE-GENERAL_CHAIN

	Valids/pyTv-S2-L2NOMINAL-011-F-N-SENTINEL2_MUSCATE-GENERAL_CHAIN

.. toctree::
	:maxdepth: 5
	:caption: pyTv-S2-L2BACKWARD-011-F-N-SENTINEL2_MUSCATE-GENERAL_CHAIN 

	Valids/pyTv-S2-L2BACKWARD-011-F-N-SENTINEL2_MUSCATE-GENERAL_CHAIN

.. toctree::
	:maxdepth: 5
	:caption: pyTv-S2-L2INIT-CROSSWRITING-SENTINEL2-SENTINEL2MUSCATE

	Valids/pyTv-S2-L2INIT-CROSSWRITING-SENTINEL2-SENTINEL2MUSCATE


.. toctree::
	:maxdepth: 5
	:caption: pyTv-S2-L2INIT-CROSSWRITING-LANDSAT8-LANDSAT8MUSCATE

	Valids/pyTv-S2-L2INIT-CROSSWRITING-LANDSAT8-LANDSAT8MUSCATE

.. toctree::
	:maxdepth: 5
	:caption: pyTv-S2-L2INIT-CROSSWRITING-VENUS-VENUSMUSCATE

	Valids/pyTv-S2-L2INIT-CROSSWRITING-VENUS-VENUSMUSCATE
