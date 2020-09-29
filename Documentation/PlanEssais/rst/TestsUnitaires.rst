LES TESTS UNITAIRES
===================

Objectifs
----------

L’objectif des tests unitaires est de :

- Réaliser une validation fonctionnelle des traitements codés,
- Réaliser de façon naturelle et ascendante une intégration des couches basses du logiciel qui sont disponibles.

Chaque répertoire (composant logiciel) contient l’ensemble des tests unitaires mis en place permettant de valider les classes développées du composant logiciel concerné.


Stratégie et méthode des tests unitaires
-----------------------------------------

Ces tests unitaires ne sont pas intégrés au [PE]. 
Ils seront toutefois mis à jour et maintenus suite aux évolutions introduites dans les phases suivantes, pour faciliter notamment la phase de maintenance.

L’outil « ctest » permet d’activer les traitements conformément à la stratégie définie ci-dessus. Les contextes de jeux de données sont conservés avec le module lanceur, de même que les résultats d’exécution qui sont mis en référence une fois le TU validé. Les résultats des tests, soumis sur le serveur cdash, sont par conséquent archivés et consultables aisément sous la forme du dashboard (cf. [DJ] pour une présentation du dashboard).

A la fin de la phase de développement, plus de 700 tests unitaires ont été mis en place.
s
Ces tests unitaires couvrent l’ensemble des composants logiciels développés et assurent une intégration complète de MAJA. Pour chaque classe développée, au minimum deux tests unitaires sont mis en place et ce dès le début du codage de la classe. Pour la validation numérique des algorithmes (ex : interpolation de LUTs, algorithme de Levenberg-Marquartz, etc.), CS utilise autant que faire se peut, des données fournies pas le CNES et référencées dans la base gérée en configuration. Chaque algorithme peut ainsi être validé en utilisant en entrée les produits intermédiaires (simple format TIFF) issus de la maquette développée par le CESBIO.

L’outil de comparaison de l’OTB effectue une différence des images pixel à pixel. Pour chaque bande, il génère l’image différence, calcule la somme des différences. Le test est validé si ces valeurs sont nulles à un epsilon près, qui est un paramètre d’entrée du test.  L’outil calcule également le nombre de pixels sur lesquels sont mesurées ces différences. Il peut être intéressant de calculer si cet écart est inférieur à epsilon pour chaque pixel de manière à ne pas masquer une différence importante mais locale sur l’image. Une analyse visuelle des images et des images de différence est réalisée. Elle permet, par exemple, de détecter une structuration non aléatoire des différences sur l’image. Les différences obtenues entre les images sont alors analysées pour en connaître l’origine et déterminer s’il s’agit d’une différence de méthode (lié par exemple à l’utilisation d’interpolateurs ou d’outils de corrélation différents) ou d’un problème lié au développement. Il convient également de déterminer si la différence mesurée est acceptable par rapport à la donnée testée (ordre de grandeur des écarts par rapport aux valeurs issues du prototype).

L’objectif des tests unitaires est de couvrir tous les cas possibles :

- cas nominaux,
- cas aux limites.

Une fois l’algorithme validé, les résultats du test sont référencés et dès lors, systématiquement contrôlés à chaque nouvelle exécution du test. Cette démarche assure la non-régression des algorithmes développés tout au long de la phase de codage. Les résultats des tests joués sont systématiquement envoyés au serveur CDash pour être mis en forme et offrir au développeur un environnement de contrôle ergonomique et permettre le suivi de l’évolution des résultats au cours du temps.


Formalisation des tests unitaires
---------------------------------

Les tests unitaires ne font pas l’objet d’une description textuelle formelle au travers d’une fiche ou d’un plan d’essai.
