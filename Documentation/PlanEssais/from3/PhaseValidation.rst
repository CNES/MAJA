.. role:: math(raw)
   :format: html latex
..

DESCRIPTION DE LA PHASE D’INTEGRATION ET DE VALIDATION
======================================================

[chater:PhaseIntegrationEtValidation] *(ref. PhaseValidation.tex)*

Les essais menés durant la phase d’intégration et de validation sont des
essais dits fonctionnels et/ou numériques. Ils permettent de tester
toutes les fonctionnalités du logiciel avec variation des paramètres
d’entrée et des contextes d’utilisation. Ils couvrent les cas
d’activation nominaux, les cas dégradés, les tests de performances. Ils
permettent de répondre à la question « est-ce que le logiciel répond
bien au besoin ? ».

Phase d’intégration
-------------------

[sec:Integration]

La phase d’intégration de MAJA consistent simplement à installer MAJA
(les COTS et la chaîne MAJA) et vérifier son intégrité.
De plus, l’intégration de MAJA peut être également vérifié en jouant des
tests de validation.

Objectifs de la phase d’intégration
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

L’objectif de la phase de tests d’intégration est de :

-  Vérifier la compatibilité des COTS utilisés,

-  Vérifier le comportement des appels entre plusieurs composants codés
   et testés unitairement,

-  Vérifier le comportement des communications entre ces composants à
   intégrer

Description du processus d’intégration
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

La méthode retenue pour réaliser la phase d’intégration est la suivante
:

-  Réaliser les intégrations des COTS au sein des composants logiciels
   du projet,

-  Réaliser les intégrations 2 à 2 de chaque composant lié dans
   l’architecture logicielle (ayant une dépendance),

-  Réaliser de façon ascendante l’intégration des composants déjà
   intégrés 2 à 2.

Les tests d’intégration entre les composants d’un même projet permettent
de valider le fonctionnement global des différents composants en les
faisant communiquer entre eux. Ces tests sont réalisés une fois que
chaque composant a pu être testé de façon séparée et indépendante. Ils
montrent que les échanges d’interfaces, les activations de processus et
autres moyens de communication et d’activation fonctionnent correctement
ensemble, permettant ainsi à l’ensemble du projet d’être intégré à un
niveau supérieur du système.

Stratégie d’intégration
~~~~~~~~~~~~~~~~~~~~~~~

Les tests d’intégration se répartissent comme suit :

-  Des tests d’intégration des COTS sont mis en place afin de s’assurer
   que les COTS sont bien installés et vérifier la compatibilité des
   versions. Ces tests sont intégrés au [PE].

Remarque : des tests d’intégration des COTS sont également exécutés dès
la phase de compilation de MAJA, contrôlant ainsi au plus tôt la
compatibilité des COTS installés,

-  Les tests unitaires mis en place lors de la phase de développement
   couvrent l’ensemble des composants logiciels développés et assurent
   une intégration complète de MAJA. Ils sont décrits section 3.5.3. Une
   sélection précise (AD) des tests unitaires sera intégrée au [PE],

-  Des tests d’intégration spécifiques sont réalisés durant la phase de
   développement, et sont conservés une fois cette phase terminée. Ces
   tests d’intégration sont intégrés au [PE].

Phase de validation
-------------------

[sec:Validation]

Ces travaux concernent la validation de MAJA (chaînes et outils de
contrôle et de validation des produits). Cette phase prend en compte la
production des données de validation et de recette.

Objectifs de la phase de validation
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

L’objectif général de la validation est de garantir que le ou les
composants implémentés fournissent des résultats exacts. Des tests de
non-régression sont également mis en place afin de garantir l’exactitude
des résultats au fur et à mesure des développements. Cette phase de
validation permet notamment de :

-  Vérifier l’adéquation des fonctionnalités offertes par le système aux
   exigences,

-  Garantir, en complément des différents essais réalisés au niveau des
   composants, une vérification des exigences compréhensible par
   l’utilisateur,

-  Valider les interfaces avec les entités externes,

-  Pérenniser un ensemble de scripts/jeux de données/résultats pour
   chaque essai afin de pouvoir les rejouer ultérieurement en cas
   d’évolution du système,

-  Apprêter le système pour son entrée en recette.

Description du processus de validation
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Le processus de validation est itératif. Il est initialisé pendant la
phase de conception, précisé tout au long de la progression du
développement, et complété pour la livraison sur site. Le processus est
composé des étapes suivantes :

-  Définition des tests : cette activité consiste à prévoir les tests
   qui devront démontrer que le système est conforme aux exigences
   utilisateur. Cette définition est réalisée en deux étapes :

   -  La première consiste à produire un plan de validation. Elle
      s’attachera notamment à définir et identifier les essais unitaires
      et données constituant le plan de validation. Cette étape est
      réalisée durant les phases de conception et éventuellement
      complété au cours des développements,

   -  La deuxième consiste à produire les plans d’essais détaillés (PED)
      de chacun des essais. Ces PED contiennent toutes les informations
      décrites section 3.6.1.1,

-  Vérification de la couverture des essais : cette activité est
   réalisée avec l’outil Reqtify, et une matrice de couverture «
   exigence/cas de tests » est produite.

-  Les étapes d’exécution sont les suivantes :

   -  Génération de la version logicielle à valider par l’équipe projet
      à partir des sources gérés par l’équipe de gestion en
      configuration CS (la procédure de génération fait l’objet
      elle-même d’un test),

   -  Installation du logiciel (l’installation fait l’objet elle-même
      d’un test),

   -  Pour chaque essai de validation :

      -  Installation des contextes d’essais, tels que définis dans le
         PED de l’essai,

      -  Génération automatique du journal d’essai en début d’essai en
         fonction de sa description dans le PED,

      -  Mise en œuvre de l’essai et rédaction de la partie résultats
         obtenus dans le journal d’essai,

      -  Récupération des contextes de fin d’essai et exécution
         éventuelle de la procédure de vérification des résultats
         (comparaison du contexte récupéré par rapport à un contexte de
         référence).

   -  Mise à jour du rapport de la campagne d’essais.

La phase de validation conduit à la mise en œuvre de l’ensemble des cas
de tests identifiés. Ces cas de tests sont complétés et finalisés lors
de cette phase. Les données et outils de validation sont mis en œuvre et
adaptés le cas échéant. Les tests et données de validation sont
maintenus pendant l’ensemble du cycle de vie du logiciel. Ce processus
est tout d’abord mis en œuvre sur la machine de développement à CS sur
laquelle est également réalisée la recette usine des chaînes MAJA L2, L3
ainsi que des outils associés. Suite à une phase d’intégration sur la
configuration cible CNES, les tests associés à des produits simulés
(choix du capteur) pourront être exécutés.
La recette site sera réalisée sur la configuration cible CNES.

Stratégie de validation
~~~~~~~~~~~~~~~~~~~~~~~

Les essais sont définis de façon à couvrir tous les cas d’utilisation
nominaux et dégradés des chaînes MAJA L2 L3 et de l’outil de contrôle et
de validation. En phase de validation, la chaîne produit en mode « TRACE
» des données intermédiaires au produit L2 final pour faciliter
notamment la validation numérique. La validation se décompose en essais
qui correspondent aux différents cas d’exécution des fonctions du
système. Ainsi, on identifie les familles d’essais correspondant aux
différentes chaînes scientifiques :

-  MAJA\_L2\_INIT\_CHAIN,

-  MAJA\_L2\_NOMINAL\_CHAIN,

-  MAJA\_L2\_BACKWARD\_CHAIN,

-  MAJA\_L3\_CHAIN,

-  MAJA\_CHECKTOOL\_CHAIN.

Pour chaque famille, les essais de validation se divisent en plusieurs
catégories :

-  Les essais fonctionnels,

-  Les essais numériques,

-  Les cas dégradés.

Les essais fonctionnels
^^^^^^^^^^^^^^^^^^^^^^^

Les produits **simulés** (Ven:math:`\mu`\ s et Sentinel2) sont utilisés
pour les validations fonctionnelles. Ces essais sont définis de façon à
couvrir tous les cas possibles d’utilisations, c’est-à-dire les cas
nominaux et les cas dégradés. Ces essais contrôlent que les données
produites sont conformes aux spécifications d’interfaces ICD et
EarthExplorer de MAJA. On contrôle ainsi :

-  la structuration des produits (nomenclature),

-  la constitution des produits :

   -  format des images

   -  conformité des fichiers XML aux schémas

   -  intégrité du produit (produits manquants)

-  le paquetage/dépaquetage des produits (pre/postprocessing)

On contrôle également les spécifications d’interfaces PDS tels que :

-  la prise en compte du fichier de configuration,

-  le rapport de production (postprocessing),

-  les plages de valeurs du code de retour,

-  le format des messages de Log émis.

Outre le contrôle des interfaces, des tests de validation des différents
cas d’utilisation de la chaîne sont mis en place (cheminement dans les
différentes branches au sein du mode de fonctionnement testé) :

-  activation de l’algorithme de corrections des effets de
   l’environnement (et des pentes).

**Cas produits non valides :** Actuellement, un seul cas « non valide »
est prévu : il correspond à un seuil sur **la couverture nuageuse**. Un
essai met en évidence ce cas et un deuxième essai contrôle que ce
produit ne peut pas être traité par la chaîne L3. Lorsqu’un produit est
déclaré non valide, alors le nom du fichier porte la nommenclature
"NOTV" (en remplacement du "VALD").

Les essais numériques
^^^^^^^^^^^^^^^^^^^^^

L’essentiel de la validation numérique est réalisé lors des tests de
validation algorithmique. Ces tests sont mis en place composant par
composant essentiellement sur des images Formosat et Landsat, issues du
prototype développé au CESBIO.
A ce stade, chaque algorithme a été validé séparemment. Il s’agit
maintenant de valider que l’enchaînement de tous ces composants dans
MAJA donne bien les mêmes résultats numériques que les tests effectués
pour chaque composant.

L’idée est donc de valider chaque image intermédiaire produite par la
chaîne (algorithme par algorithme) en les comparant aux images issues
des tests de validation algorithmiques. Toutefois, une différence des
images pixel à pixel n’est pas réalisable dans la chaîne globale. En
effet, les sorties de chaque algorithme devenant les entrées des
algorithmes suivants, chaque composant lancé dans la chaîne utilise des
données d’entrée différentes de celles utilisées dans les tests
algorithmiques.

Comme pour les tests unitaires, la validation est effectuée par une
analyse visuelle des images et des images de différence. Plusieurs types
de données sont comparées :

-  les images contenant des grandeurs physiques produites notamment par
   les algorithmes RayleighCorrection, ScatteringCorrection,
   EnvironmentCorrection et SlopeCorrection. Ces images donnent des
   résultats numériques "identiques" à une tolérance près liées
   notamment aux images d’entrée.

-  les masques binaires dans lesquels des fausses détections peuvent
   apparaître. Ces masques comme les masques de nuages ou les masques
   d’eau sont fournis en entrée des algorithmes suivants et ont un
   impact important sur la génération des images finales comme la
   génération des images composites.

-  les images composites sont très sensibles à des différences présentes
   dans les masques utilisés en entrée. L’épaisseur optique aérosols
   moyenne sur l’image, une couverture nuageuse plus ou mois importante
   peuvent changer totalement les images composites. Des différences
   plus locales comme des fortes AOT à proximité des nuages, des masques
   de nuages plus ou moins étendus ou la localisation des ombres des
   nuages (liées à l’affinage de l’altitude des nuages) entrainent des
   changements locaux au niveau des images composites. Les images de
   dates sont de bons indicateurs pour analyser les différences
   observées.

Ces essais sont donc réalisés sur des images Formosat et Landsat
fournies par le CESBIO sur lesquelles la chaîne est lancée dans son
intégralité. Quelques tests suffisent pour couvrir l’ensemble des
fonctionnalités à valider :

-  mode init, nominal et backward,

-  images sur différents sites (différence de relief, d’AOT),

-  images à des dates différentes (pluvieuses, variation de la
   couverture nuageuse, présence de glitter, ... ).

Les cas dégradés
^^^^^^^^^^^^^^^^

Le traitement spécifique des cas dégradés n’est pas défini dans le
document de spécification des algorithmes.

Plusieurs essais sont mis en place pour couvrir tous les cas dégradés
identifiés :

-  Si un pixel est initialisé à No\_Data, le pixel vaut No\_Data sur
   toutes les bandes. Les pixels à No\_Data sont pris en compte dans le
   masque des bords d’image si ce n’est récurrent.

-  L’absence d’une bande spectrale n’est pas gérée dans la chaîne et
   provoque une sortie en erreur.

-  L’absence de MNT entraîne également une sortie en erreur.

-  Le cas des pixels saturés est géré algorithme par algorithme :

   -  CloudMasking : lors du seuillage sur la réflectance dans le bleu
      uniquement,

   -  AOTEstimation : pas de prise en compte des pixels saturés lors de
      l’estimation locale de l’épaisseur optique des aérosols,

   -  CompositeImage : pas de mise à jour dans l’image composite pour
      les pixels saturés.

Ces essais sont notamment important dans le cas d’une utilisation des
chaînes en mode stand-alone.

Performances
------------

Ces travaux concernent les mesures de performances des chaînes de
traitement et des outils de contrôle et de validation des produits, pour
le cas des produits VEN\ :math:`\mu`\ S. Le Plan des essais de
performances fait l’objet d’un chapitre particulier dans le PE.

Objectifs des essais de performance
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Les objectifs des tests de performances sont décrits dan le [DP].

Description du processus de mesures de performances
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Les outils et méthodes de mesure des performances sont décrits dans le
[DP].

Stratégie
~~~~~~~~~

Les produits Venus simulés sont utilisés pour les essais en performance.

Les essais sont définis ainsi :

-  Mesure des performances en mémoire : pour contrôler l’empreinte
   mémoire, plusieurs essais sont définis permettant de tester les
   différents paramètres des méthodes de découpage des images.

-  Mesure des performances en temps : plusieurs essais sont définis
   permettant de tester les différents cas d’utilisation :

-  Méthode découpages des images,

-  nombres de cœurs utilisés par la traitement : par exemple dans un
   contexte VIP, un seul cœur doit être utilisé. Par contre, il est
   intéressant de mesurer les performances en temps d’exécution en
   exploitant toutes les capacités de la machine (utilisation des 16
   cœurs).

Toutes ces mesures sont reportées dans le chapitre suivant.

Formalisation des tests de validation
-------------------------------------

[sec:FormalisationTV]

La description d’un essai contient les éléments suivants :

-  Référence de l’essai,

-  Titre de l’essai,

-  Objectif de l’essai,

-  Description de l’essai,

-  Catégorie du test : PERFORMANCE, INTEGRATION, VALIDATION,

-  Type de test : NOMINAL ou DEGRADE,

-  Nature du test : FONCTIONNEL ou NUMERIQUE

-  Pré-requis (liste des contextes à installer préalablement à un essai,
   ...),

-  Traçabilité (interfaces couvertes, spécifications couvertes,
   exigences, ...). Pour chaque exigence, on indique la conformité : CF
   (conforme), NC (non conforme), PC (partiellement conforme) ou CD
   (conforme avec déviation)

-  Moyens nécessaires (simulateurs,…),

-  Durée d’exécution attendue,

-  Durée d’exécution obtenue,

-  Description de la mise en œuvre de l’essai,

-  Performances attendues,

-  Vérification à effectuer,

-  Vérifications effectuées.

Nomenclature des tests de validation
------------------------------------

[sec:NomenclatureTV]

La nomenclature adoptée pour l’identification des essais d’intégration
et de validation est la suivante :

**<Idcapteur>-<catégorie>-<traitement>-<nd’essai>-<nature>-<type>-<produit>-<thématique>**
Ex : VE-P-L2INIT-001-F-N-VENUS-CLOUD

Avec :
**VE** : Id Capteur
peut prendre les valeurs :

-  VE : produit image Venus,

-  F2 : produit image Formosat-2,

-  L5 ou L7 : produit image Landsat L5 ou L7,

-  S2A ou S2B : produit image Sentinel2 A ou B

**Catégorie** identifie la catégorie du test et peut prendre les valeurs
:

-  P : Test de performance

-  I : Test d’intégration

-  V : Test de validation

**Traitement** identifie le type de traitement et peut prendre les
valeurs :

-  L2INIT : Chaîne L2 en mode Init,

-  L2NOMI : Chaîne L2 en mode Nominal,

-  L2BACK : Chaîne L2 en mode Backward,

-  L3 : Chaîne L3,

-  CHCKTLS : L’outil de contrôle et de validation

**Numéro** d’essai :

-  000 : valeur codée sur 3 caractères, valeur unique pour chaque
   <traitement>

**Nature** identifie la nature du test et peut prendre les valeurs :

-  F : test fonctionnel,

-  N : test numérique,

**Type** identifie le type de l’essai et peut prendre les valeurs :

-  N : nominal,

-  D : dégradé,

-  L : aux limites

**Produit** identifie le type de produit image utilisé et peut prendre
les valeurs :

-  VENUS : produit image Venus,

-  FORMOSAT : produit image Formosat-2,

-  LANDSAT : produit image Landsat,

-  SENTINEL2 : produit image Sentinel2

**Thématique** est une abréviation du thème autour duquel s’organise le
test, sur 10 caractères maximum.

Note sur l’affichage des résultats d’exécution des essais
---------------------------------------------------------

Les résultats des essais sont insérés en fin de section de chaque cas
test.
Parmis les informations extraites et affichées dans ce plan, le **temps
d’éxécution** ainsi que **l’empreinte mémoire** allouée lors de
l’exécution du test sont reportées pour chaque test. En ce sens, chaque
test fait référence de test de performances.

Déroulement des essais de validation
------------------------------------

Les essais sont quant à eux décrits au chapitre suivant.
