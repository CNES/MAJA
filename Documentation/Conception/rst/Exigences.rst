Rappel des principales exigences et contraintes
===============================================

Exigences et contraintes générales
----------------------------------

L’outil MAJA (MACCS ATCOR Joint Algorithms) combine plusieurs
traitements :

-  L’estimation de masques de :

   *  Couverture nuageuse et ombres des nuages

   *  Eau

   *  Neige

   *  Ombres de relief

-  L’estimation du contenu en vapeur d’eau

-  L’estimation de l’épaisseur optique des aérosols

-  La correction atmosphérique en utilisant des données exogènes (METEO,
   MNT, …) et l'épaisseur optique des aérosols. La correction
   atmosphérique inclut aussi une correction des effets d’environnement.

-  La correction des variations d'éclairement dues aux pentes en
   utilisant l’information fournie avec le MNT et en considérant une
   fonction de distribution de la réflectance bidirectionnelle.

A partir de la version 3.0.0, l’architecture de MAJA a été modifiée afin
de prendre en compte les exigences de généricité. L’outil est par
conception **multi capteurs**, et chaque capteur est désormais géré dans
un « plugin ». Dans la version 3.0.0, les plugins VENUS et SENTINEL2
sont implémentés ; les plugins FORMOSAT et LANDSAT seront implémentés
dans la prochaine version.

En intégrant les produits Sentinel-2 à la chaîne MAJA, elle doit
maintenant gérer des **résolutions multiples** dans un même produit de
niveau 1.

La version 4.0.0 à marqué un tournant dans MAJA. En effet l'ensemble du logiciel
à été remanié pour permettre une plus grande souplesse quand aux futurs développements.
L'enchainement des algorithms ainsi que la lecture des produits se font dorénavant en python.
Les algorithms quand à eux sont implémentés sous forme d'applications OTB indépendantes.
Cela permet alors de pouvoir tester chaque algorithm indépendament de MAJA, de les
modifiés simplement ou encore de les utilisés dans un contexte différent. Cela permet
également à d'autres contributeurs de pouvoir fournir une application à integrer dans MAJA.

A noté également qu'a partir de la version 4.0.0 Maja n'implémente plus certains plugins obsolètes.

L’outil MAJA doit pouvoir être utilisé sous deux formes :

-  Fonctionnement intégré dans une structure d’accueil (VIP dans le
   projet Venµs) permettant un fonctionnement en mode automatique. Dans
   ce cas, le lancement des traitements utilisera le mode d'utilisation
   défini dans le segment sol de Venµs.

-  Fonctionnement avec une interface en mode ligne (dit aussi « stand
   alone ») qui pourra être utilisée dans un environnement Sentinel-2
   (SL2P ou IQP) ou dans le futur Centre de Traitement National
   multi-missions.

L’applicatif MAJA s’appuie sur différents types de modules :

En c++ :

-  Modules (Bibliothèques) métiers réalisant les différents traitements
   déjà décrits plus haut, sous forme d'applications indépendantes

En python :

-  Modules (Bibliothèques) de gestion et d’accès aux images/masques etc…
-  Modules d’accès aux fichiers de données (XML, de type GIPPs…).

..

   Répond à: Exigence 10 dans : [DA01]_ Conformité : C

Exigences d’interfaces
----------------------

MAJA 4.1.0 peut traiter en entrée des produits de niveau 1 :

-  Venus (format MUSCATE)

-  Venus (format NATIF)

-  Landsat 8 (format MUSCATE)

-  Landsat 8 (format NATIF)

-  Sentinel2 (format MUSCATE)

-  Sentinel2 (format natif)

Pour ce faire, les couches génériques (dites « Factory ») de lecture des
produits et informations connexes (méta-données) sont développées pour
les cas suivants :

-  Venus (format MUSCATE) 

-  Venus (format Natif) 

-  Sentinel-2A 1-C (format MUSCATE)

-  Sentinel-2B 1-C (format MUSCATE)

-  Landsat 8 (format MUSCATE)

-  Landsat 8 (format Natif)

-  Sentinel-2A 1-C (format natif)

-  Sentinel-2B 1-C (format natif)


La trans-écriture de Natif vers Muscate est nativement possible dans Maja et est activée par défaut sauf si l'option de choix du format de sortie est utilisée.

A noter que certains plugins ne permettent pas l'écriture vers le format Natif mais uniquement la lecture ainsi le produit de sortie sera obligatoirement au format muscate:

- Venus Natif

- Landsat 8 Natif


Remarque : Pour Sentinel 2, un produit niveau 1-C couvre une « tuile »
de 100 km x 100 km (selon la définition de « tuile/tile » indiquée dans
le [DA05]).

   Répond à: Exigence 90 dans : [DA01]_ Conformité : C

   Répond à: Exigence 20 dans : [DA01]_ Conformité : C

   Répond à: Exigence 400 dans : [DA01]_ Conformité : C

   Répond à: Exigence 410 dans : [DA01]_ Conformité : C

   Répond à: Exigence 470 dans : [DA01]_ Conformité : C

   Répond à: Exigence 580 dans : [DA01]_ Conformité : C

   Répond à: Exigence 590 dans : [DA01]_ Conformité : C

   Répond à: Exigence 620 dans : [DA01]_ Conformité : C

   Répond à: Exigence 630 dans : [DA01]_ Conformité : C

   Répond à: Exigence 640 dans : [DA01]_ Conformité : C

   Répond à: Exigence 40 dans : [DA01]_ Conformité : C

   Répond à: Exigence 420 dans : [DA01]_ Conformité : C

   Répond à: Exigence 430 dans : [DA01]_ Conformité : C

Exigences des traitements
-------------------------

Les traitements algorithmiques MACCS sont modifiés afin de prendre en
compte les produits Sentinel-2 (Level 1-C). Ainsi, les évolutions
algorithmiques décrites dans cette section prennent en compte les
évolutions décrites dans le [DA02]_.

   Répond à: Exigence 30 dans : [DA01]_ Conformité : C

Exigences de performances
-------------------------

   Répond à: Exigence 160 dans : [DA01]_ Conformité : C

Ne pas dégrader les performances initiales de MACCS (performances
initialement établies sur les chaînes L2/L3 VENµS) conditionne la
conception et les développements à suivre.

Concrètement, dès la phase de développement, on vérifie que les temps
d’exécution des TVs de référence VENµS ne sont pas dégradés dans MACCS.
Ainsi, tout comme les TUs et les TVAs, les tests de performance seront
lancés toutes les nuits, avec un contrôle régulier du temps d’exécution
des tests VENµS par les membres de l’équipe.

Exigences de généricité
-----------------------

   Répond à: Exigence 300 dans : [DA01]_ Conformité : C

   Répond à: Exigence 330 dans : [DA01]_ Conformité : C

La chaîne MAJA est un seul et même logiciel configurable quel que soit
le nombre de capteurs et les nombre de centres utilisateurs. La chaine
de traitement MAJA peut être générée pour traiter les produits de tous
les capteurs, d'un sous-ensemble de capteurs ou d'un capteur unique. En
effet, des options de compilation permettent d’activer les capteurs
souhaités. Ainsi, il est par exemple possible de générer le paquet
binaire de MAJA pour le seul capteur VENUS.

   Répond à: Exigence 310 dans : [DA01]_ Conformité : C

   Répond à: Exigence 320 dans : [DA01]_ Conformité : C

Toutes les spécificités liées à un capteur sont prises en charge par le
Plugin (code source et schémas d’interface). Ainsi, la chaîne de
traitement MAJA peut traiter les produits d'un nouveau capteur sans que
le traitement des produits des autres capteurs ne soit impacté, et la
modification des interfaces entre la chaîne MAJA et un capteur n’impacte
pas les interfaces des autres capteurs avec la chaine MAJA.
