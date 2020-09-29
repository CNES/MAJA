.. role:: math(raw)
   :format: html latex
..

Environnement de validation
===========================

[chapter:EnvironnementValidation]

*(ref. Environnement.tex)*

Configuration matérielle et dimensionnement
-------------------------------------------

[sec:ConfigurationMaterielle] L’ensemble des essais se déroule sur la
configuration de développement à savoir :

-  Une machine 64 bits avec Linux version RedHat6.

Il faut environ **180 Go** d’espace disque pouvoir générer tous les
tests des TV et des TVA, réparti comme ceci :

-  Pour les données d’entrées et les données de références (MAJA-Data) :
   **200 Go**,

-  Pour les données produites après exécution de tous les TV : **20 Go**
   de données de sortie et **60 Go** de données de caching, soit **80
   Go**,

-  Pour les données produites après exécution de tous les TVA : **6 Go**
   (incluant données de sortie et données de caching).

Nous précisons de plus que lors des tests de régression entre les
sorties de référence et celles générées par la chaîne, si les deux
images comparées diffèrent, une image de différence est générée. Ceci
provoque de facto une augmentation de la mémoire utilisée.

**Configuration matérielle minimale pour travailler dans de bonnes
conditions :** Pour lancer les TVs et effectuer la validation de MAJA
dans des conditions idéales, il est nécessaire de disposer d’une machine
suffisamment puissante. Ainsi il est préconnisé d’utiliser une machine
possédant au minimum 4 coeurs, avec au minimum 4 Go de RAM.

**Configuration pour une utilisation d’espace disque minimale :**

Pour utiliser le minimum d’espace disque, il faut alors procécéder comme
ceci :

-  ne pas lancer les TVA (nécessaire uniquement pour validation fine des
   algorithmes, en cas de régression)

-  laisser les données d’entrées et de références (répertoire MAJA-Data)
   sur le DVD ou les stocker sur un support externe (de type disque dur)

-  activer l’option **EnableCleaningCachingDirectory** dans le fichier
   de configuration ’MAJAUserConfigSystem.xml’ (fichiers de
   configuration situés dans le paquet MAJA-Validations/Data/Config)
   permettant ainsi d’effacer systématiquement les données de caching
   générées à chaque exécution d’un test.

Ainsi, l’espace disque disponible requis descend alors à **26 Go** (20
Go de données de sortie et 6 Go supplémentaire de données de caching
nécessaire lors de l’exéction).

Moyens de validation
--------------------

[sec:MoyensValidation] La phase de validation repose sur les moyens
suivants :

-  Les plans d’essais contenant les scénarii prêts à être déroulés,

-  La configuration matérielle de développement,

-  La configuration matérielle cible, au CNES, équipée du socle
   technique, des logiciels de base et des COTS nécessaires à
   l’exploitation, afin de réaliser les essais de préintégration des
   chaînes de traitement L2 et L3, ainsi que les essais de performances,

-  Des outils de rejeu automatique des essais, permettant de vérifier
   les non-régressions, pour les traitements batch uniquement (cf outils
   ctest et CDash présenté section 3.5.2),

-  Des jeux de données (cf section suivante)

-  Chaîne de référence fournie par le CNES,

-  La couche « stand-alone » développée par CS.

Outils
------

Moniteur de tests
~~~~~~~~~~~~~~~~~

L’environnement de test et de validation utilisé est « ctest » (outil
intégré à CMake). En effet, avec CMake, il est extrêmement aisé de créer
et d’ajouter des nouveaux tests dans un CMakeLists.txt (commande
ADD\_TEST), de les compiler et de les lancer. Cet environnement est
notamment utilisé au quotidien par l’équipe de développement CS sur le
projet Orféo Toolbox, qui crée des tests automatiques. Cet outil permet
la définition et l’enchaînement automatique (ou interactif) d’essais et
ce, sur tous types de plates-formes et environnements de compilation.
Cet outil assure un suivi de ces essais conformément aux usages dans ce
type de développement en traçant les résultats sous forme de journal de
bord. Cet outil permet d’automatiser un grand nombre de tâches de
validation fastidieuses, et facilite le rejeu d’essais de non-régression
pendant les phases de développement, de validation, de garantie et de
maintenance. Cet outil est donc utilisé pour lancer les tests unitaires
et de validation.

Exploitation des résultats des tests : Serveur CDash
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

L’outil CDash permet d’exploiter les résultats produits par l’outil de
test ctest.
Attention, cet outil n’est déployé que sur la plate-forme CS pour la
phase de valition et de recette usine. Il n’est pas nécessaire de
l’installer sur site lors de la recette site.

Données de validation
---------------------

Données fournies par le CNES
~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Ce chapitre liste les données fournies par le CNES pour valider la
chaîne.

Cas Ven\ :math:`\mu`\ s
^^^^^^^^^^^^^^^^^^^^^^^

Une série de produits sur le site CAMARGUE de niveau 1 (avec et sans
nuage) , décalés de deux jours, est disponible pour les essais:

-  Produit image 3 : VE\_TEST\_VSC\_L1VALD\_CAMARGUE\_20120101

-  Produit image 4 : VE\_TEST\_VSC\_L1VALD\_CAMARGUE\_20120103

Les produits précédents contiennent des valeurs de réflectance très
élévées et non réalistes. Les pixels sont donc tous déclarés nuageux par
l’algorithme de détection des nuages présents dans la chaîne L2. Pour
pouvoir poursuivre les traitements après le module de détection des
nuages, il est nécessaire d’augmenter le seuil sur la couverture
nuageuse figurant dans les GIPP (GIP\_L2COMM). Les tests réalisés sur
ces données sont donc des tests d’ordre fonctionnel et ne permettent pas
de valider numériquement les algorithmes.

Des données météo/ozone sont utilisées pour corriger les images de
réflectance de l’absorption gazeuse.

-  Donnée ozone du type
   VE\_TEST\_EXO\_METDTA\_20110101T000000\_20111231T230000.DBL

La donnée MNT utilisée est la suivante :

-  Donnée MNT du type VE\_TEST\_AUX\_REFDE2\_CAMARGUE\_0001.DBL

Cas Formosat (au format MUSCATE\_PROTO)
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

*Images du Sud-ouest*

Les images du Sud-Ouest (le site identifié est **EUSOuest00A00B**) sont
majoritairement utilisées dans les tests de validation. Ces images sont
acquises avec des conditions angulaires particulières (glitter)
permettant notamment la détection des pixels d’eau.

\|p3cm\|p2cm\|p7cm\|

Site & Identifiant & Commentaires
Sud-Ouest & 20060329 & Image pluvieuse.
& 20060614 & Fausses détections nuageuses (seulement partiellement
éliminées) sur des champs après récolte.
& 20060807, 20060819 & Quelques ombres de nuages extérieurs à l’image
détectées.
& 20060502 & AOT élevée à proximité des nuages (écriture composite).
& 20060514 & AOT moyenne trop élevée, pas d’écriture des composites.
& 20060317, 20060602, 20061006 & Seuil de couverture nuageuse dépassé.
& 20060525, 20061009 & Option d’extension des nuages

*Images du Maroc*

Plusieurs images partiellement saturées (nuages) sont utilisées pour
tester la direction de projection des ombres et des faces cachées liées
au relief, la détection des nuages, l’écriture des composites, etc.

\|p3cm\|p2cm\|p7cm\|

Site & Identifiant & Commentaires
Maroc & 20051229 & Image pluvieuse : traitée mais non utilisée pour la
mise à jour des composites.
& 20060216, 20060322 & Pixels éliminés de l’inversion car trop forte
variation dans PIR : la carte nbpix présente des valeurs plus faibles là
où l’image devient ’verte’
& 20060324 & AOT élevée à proximité des nuages (écriture composite).

Cas Landsat 5 et Landsat 7 (au format MUSCATE\_PROTO)
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Les images LANDSAT L5 et L7 permettent de valider les évolutions
algorithmiques de la chaîne MAJA tels que la détection de la neige et la
prise en compte de ce masque lors de la détection des nuages, de la
pluie ou lors de la génération des images composites. Ces données
permettent également de valider les fonctionnalités multi-capteurs de la
chaîne en utilisant dans un même traitement des produits L5 et L7.
Ces images sont au format définit dans l’interface du PTSC (contenant un
fichier tif et un fichier xml). Elles sont listées ci-dessous :

-  L5\_TEST\_LSC\_PDTIMG\_L1TILE\_TM\_\_\_\_EU93066200A00B\_20091201

-  L5\_TEST\_LSC\_PDTIMG\_L1TILE\_TM\_\_\_\_EU93066200A00B\_20100118

-  L5\_TEST\_LSC\_PDTIMG\_L1TILE\_TM\_\_\_\_EU93066200A00B\_20091217

-  L7\_TEST\_LSC\_PDTIMG\_L1TILE\_ETM\_\_\_EU93066200A00B\_20100315

Sont également disponibles :

-  un MNT au format Ven\ :math:`\mu`\ s : plans altitude, aspect, slope
   et water mask,

-  comme pour Formosat, les valeurs des angles solaires et de prise de
   vue au centre de l’image.

Cas Sentinel 2 (S2A et S2B)
^^^^^^^^^^^^^^^^^^^^^^^^^^^

Les images SENTINEL2 peuvent également être traitées dans la chaîne
MAJA. Ces images sont à des résolutions différentes en fonction des
bandes spectrales et permettent donc de valider dans la chaîne :

-  La lecture spécifique d’un produit SENTINEL2 et la conversion au
   format image des polygones décrits dans des fichiers gml,

-  la prise en compte de résolutions multiples dans un même produit,

-  les algorithmes de détection des cirrus (génération d’un masque de
   cirrus spécifique aux produits SENTINEL2) et de la neige,

-  la prise en compte d’angles de prise de vue variables en fonction des
   détecteurs lors de la correction de la diffusion.

Les produits de niveau 1 contiennent :

Installation de l’environnement de validation
---------------------------------------------

Pré-requis
~~~~~~~~~~

Pour l’environnement de validation, il est nécessaire :

-  d’installer MAJA (paquet ’maja’). Pour ce faire, dérouler la
   procédure d’installation décrite section §2.3 du document "USER,
   INSTALLATION, and OPERATING MANUAL, MAJA L2 & L3 CHAINS" (fichier
   "SETG-MU-MAJA-010-CS").

-  d’installer MAJA DEVEL (paquet ’maja-devel’). Pour ce faire, dérouler
   la procédure d’installation décrite section §2.3 du document "USER,
   INSTALLATION, and OPERATING MANUAL, MAJA L2 & L3 CHAINS" (fichier
   "SETG-MU-MAJA-010-CS"), à partir du paquet binaire ’maja-devel’
   fourni.

Paquets
~~~~~~~

Pour installer l’environnement de validation, il est nécessaire de
disposer des paquets suivants :

-  MAJA-Validation-<Version>.tgz : contient la procédure de génération
   du contexte de validation (TV)

-  MAJA-Data-<Version>.tgz : contient toutes les données d’entrées,
   utilisées par les TVs

Avec Version : Numéro de version de l’article de configuration sous
forme X.Y.Z, où :

-  X : premier digit de la version, pour évolution majeures de
   fonctionnalités algorithmiques ou d’interfaces

-  Y : deuxième digit de la version, pour évolution mineures de
   fonctionnalités algorithmiques ou d’interfaces (correction d’anomalie
   bloquantes, amélioration, etc.)

-  Z : troisième digit de la version, pour la livraison de patch
   correctif, ou sur la génération de slivrables (paquets binaiers,
   etc.)

Extraction des paquets
~~~~~~~~~~~~~~~~~~~~~~

Les deux paquets sont déposés dans le répertoire de travail destiné à la
validation, puis extrait, en exécutant les commandes suivantes :

Variables d’environnement nécessaires
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Aucune.

Installation des paquets
~~~~~~~~~~~~~~~~~~~~~~~~

Pour installer l’environnement de validation il suffit d’exécuter les
commandes suivante :

**Note sur la procédure d’installation :** le script se charge de
vérifier les COTS suivants pré-requis avant d’installer l’environnement
de validation :

-  gcc

-  make

-  cmake

-  maja

-  maja-devel

**Remarque :** Si l’erreur suivante apparait :
c’est que le fichier n’est pas en mode "execution" (+x). Dans ce cas,
lancer la commande :
et relancez la commande d’installation ci-dessus.

Après exécution de la commande, le répertoire de validation contient le
répertoire , organisé comme ceci :

-  : contient l’ensemble de données d’entrées pour chaque TV

-  : contient l’ensemble de données produites pour chaque TV

-  : contient le moniteur de test. Répertoire de lancement des TVs

**** Chaque fois que la commande d’installation de l’environnement de
validation est lancée, le répertoire précédemment généré est nettoyé
avant d’être re-créé.

Démarrer avec l’environnement de validation...
----------------------------------------------

Cette section présente quelques notes permettant de démarrer avec
l’environnement de validation.

Pour pouvoir commencer il faut avant tout se déplacer dans le répertoire
. Pour ce faire, exécuter la commande suivante :

Ensuite, pour **afficher** la liste des TVs jouables, il suffit
d’exécuter la commande suivante :

Pour **exécuter** un ou plusieurs test(s), il est possible de lancer au
choix les commandes suivantes :

-  : lance tous les TVs

-  : lance le TV identifié "VE-P-L2INIT-001-F-N-VENUS-IFPDS"

-  : lance que les TVs commençant par VE-... (TVs VENUS)

-  : lance le TV numéro 2

-  : lance les TVs numéros 5 à 8 et affiche le déroulement des
   traitements à l’écran

