.. role:: math(raw)
   :format: html latex
..

DESCRIPTION DE LA PHASE DE TESTS DE VALIDATION ALGORITHMIQUE
============================================================

[chapter:PhaseDeTestsValidationAlgorithmique]

*(ref. PhaseTestsValidationAlgorithmique.tex)*

Objectifs de la phase
---------------------

[sec:ObjectifsTVA]

L’objectif des tests de validation algorithmique est de :

-  Réaliser une validation numérique des traitements algorithmiques de
   "haut niveau" codés,

-  Vérifier le comportement de ces traitements dans certains cas
   dégradés ou aux limites qui ne seront pas repris en validation,

avec le responsable algorithmique CNES.

Chaque module algorithmique suivant fait l’objet d’un ou plusieurs tests
permettant de couvrir l’ensemble des fonctionnalités de chaque composant
:

|image| [fig:ArborescenceModulesAlgorithmiques]

Stratégie et méthode des tests numériques
-----------------------------------------

[sec:StrategieTVA]

Les données **Formosat** sont principalement utilisées pour la
validation numérique (à l’exception du test de détection des cirrus qui
nécessite le plan d’altitude stéréoscopique des nuages présent dans les
produits Ven\ :math:`\mu`\ s). Suite au développment de la chaîne MAJA,
des tests supplémentaires avec des produits **Landsat** et **Sentinel2**
ont été ajoutés pour prendre en compte les dernières évolutions de la
chaîne comme la création de nouveaux algorithmes (détection de la neige,
des cirrus, estimation des aérosols, etc.) ou la gestion de produits à
multiples résolutions.

L’essentiel de la validation numérique est menée en comparant chaque
image du produit de niveau 2 générée par les chaînes scientifiques
Ven\ :math:`\mu`\ s à celle produite par le prototype développé au
CESBIO.

L’outil de comparaison de l’OTB effectue une différence des images pixel
à pixel. Pour chaque bande, il génère l’image des différences et calcule
la somme des différences. Le test est validé si ces valeurs sont nulles
à un epsilon près, qui est un paramètre d’entrée du test. L’outil
calcule également le nombre de pixels sur lesquels sont mesurés ces
différences. Compte tenu de la complexité des algorithmes, la valeur
nulle semble difficile à atteindre. La valeur du epsilon sera fixée en
concertation avec le CNES et en tenant compte des résultats obtenus pour
les tests unitaires. Il convient également de déterminer si la
différence mesurée est acceptable par rapport à la donnée testée (ordre
de grandeur des écarts par rapport aux valeurs issues du prototype)

Comme pour les tests unitaires, une analyse importante est l’analyse
visuelle des images et des images de différence. Elle permet de détecter
une structuration non aléatoire des différences sur l’image mais aussi
de localiser les différences (proche d’un nuage, d’une zone d’eau,
etc.).

A cette analyse des images de différence, s’ajoutent d’autres moyens
d’analyse. Une stratégie de validation différente est adoptée en
fonction du types de données à comparer. Plusieurs types de données sont
comparés : les images contenant des grandeurs physiques, les images de
dates et les masques.

La validation des produits composites passe dans un premier temps par la
comparaison des plans « date » pour visualiser les zones qui diffèrent.
Les différences mesurées lors de la comparaison des images de date vont
se retrouver sur les images composite de réflectance.
La validation des masques présents dans le produits L2 se décompose en
deux étapes :

-  une analyse visuelle en traçant les contours des masques sur les
   images (outil de quick look),

-  une analyse statistique des différences entre le masque généré et le
   masque de référence si besoin est.

En effet, la probabilité d’avoir des masques parfaitement identiques
entre les masques issus des chaînes Ven\ :math:`\mu`\ s et les masques
de référence est faible. Pour comparer ces masques, la simple différence
réalisée pixel à pixel peut être complétée par la mise en place
d’indicateurs statistiques. Ces derniers permettent de quantifier la
performance des détections. Les performances du masque obtenu sont alors
exprimées par une matrice de confusion par rapport au masque de
référence.

A ces indicateurs s’ajoutent le calcul des proportions de bonnes et de
fausses détections c’est-à-dire le nombre de pixels correctement
identifiés par rapport au nombre de pixels de l’image.

Une fois l’algorithme validé, les résultats du test sont référencés et
dès lors, systématiquement contrôlés à chaque nouvelle exécution du
test. Cette démarche assure la non-régression des algorithmes développés
tout au long de la phase de codage. Les résultats des tests joués sont
systématiquement envoyés au serveur CDash pour être mis en forme et
offrir au développeur un environnement de contrôle ergonomique et le
suivi de l’évolution des résultats au cours du temps.

Formalisation des tests de validation algorithmique
---------------------------------------------------

[sec:FormalisationTVA]

La nomenclature adoptée pour l’identification des tests algorithmiques
est la suivante :

**<produit>-<catégorie>-<traitement>-<nature>-<thématique>**
Ex : VE-TVA-L2-N-CLOUD

Avec :
**Produit** identifie le type de produit image utilisé et peut prendre
les valeurs :

-  VE : produit image Ven\ :math:`\mu`\ s\ :math:`\mu`\ s,

-  F2 : produit image Formosat-2 ,

-  L5 : produit image Landsat L5,

-  L7 : produit image Landsat L7,

-  S2A : produit image Sentinel2 S2A,

-  S2B : produit image Sentinel2 S2B,

**Catégorie** = TVA = Test de Validation Algorithmique (numérique):

**Traitement** identifie le type de traitement et peut prendre les
valeurs :

-  L2 : Algorithmes de MAJA pour le cas L2,

-  L3 : Algorithmes de MAJA pour le cas L3,

**Nature** identifie la nature du test et peut prendre les valeurs :

-  F : test fonctionnel,

-  N : test numérique,

**Thématique** est une abréviation du thème autour duquel s’organise le
test, sur 10 caractères maximum.

.. |image| image:: Algo-Folders.eps
