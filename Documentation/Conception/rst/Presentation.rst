Présentation générale de MAJA
=============================

Chaînes opérationnelles
-----------------------

Le principe des chaînes opérationnelles est exactement le même que
celles des chaînes VENµS L2/L3. Elles sont simplement ajustées pour être
conforme à MAJA.

Identification des chaines
~~~~~~~~~~~~~~~~~~~~~~~~~~

Les cinq chaînes scientifiques identifiées sont :

-  MAJA_L2_INIT_CHAIN,

-  MAJA_L2_NOMINAL_CHAIN,

-  MAJA_L2_BACKWARD_CHAIN,

Cas d’utilisation de MAJA
~~~~~~~~~~~~~~~~~~~~~~~~~

Lancée par l’utilisateur MAJA ou par un centre utilisateur, avec en
ligne de commande (cf. [MU]_)  les paramètres (sous forme d’options),
l’application **maja** exécute au choix l’un des traitements listés
section précédente :ref:`presentation:identification des chaines`.

On note que l’application **maja** n’a pas besoin de TaskTable pour
fonctionner.

Ainsi, c’est le centre opérationnel qui va enchaîner successivement les
traitements (dans ce cas un seul traitement), définis dans la TaskTable
**« VIP »**. Si le CNES désire installer MAJA au VIP, il sera nécessaire
de définir de nouvelles TaskTables spécifiques à MAJA.

L’application “maja”
~~~~~~~~~~~~~~~~~~~~

L’application **maja** est modifiée, elle
enchaîne désormais les traitements en lançant des applications OTB des algorithms.
La lecture et l'écriture des produits de faits en python, de même que l'enchainement
des algorithms.

MAJA est **multi-satellite**. En effet, la chaîne peut traiter des
produits de niveau 1 provenant de la même mission, mais de capteurs
différents. Par exemple, pour le cas Sentinel2, la chaîne ingère
indifféremment des produits S2A, S2B et S2C.

Dans cette version de MAJA, il est possible de fournir en sortie des 
produits au format MUSCATE à partir de produits au format Natif.

Note importante : la description des plugins (et la procédure
d’implémentation) est décrite en détail dans le Software Developer
Manual [SDM]_.

Interfaces
----------

Présentation générale
~~~~~~~~~~~~~~~~~~~~~

Le principe retenu sur le format des données produites en sortie est le
suivant : chaque produit L2 doit être conforme à son format « capteur »
d’origine. La constitution (format, nombre de fichiers, etc…) d’un
produit L2 est donc dépendante du satellite. En d’autres termes, un
produit VENµS de niveau 2 doit être conforme à l’ICD Venµs, un produit
Sentinel-2 de niveau 2 doit être conforme à l’ICD S2, etc...

Dans les faits, les schémas des produits correspondant à chacun des
capteurs sont gérés par capteur, dans le **Plugin** correspondant. Ainsi
chaque capteur possède ses propres schémas de produits (L1, L2 et L3).

De manière générale, tous les schémas des produits, des fichiers GIPPs
et des fichiers de configuration liés au capteur sont gérés dans le
plugin dudit capteur.

Se reporter au chapitre 3 du [SDM]_ pour une présentation détaillée des
interfaces externes/internes de MAJA.

Interface des produits images 
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

   Répond à: Exigence 70 dans : [DA01]_ Conformité : PC

   Répond à: Exigence 80 dans : [DA01]_ Conformité : PC

Produits de NIVEAU 1
^^^^^^^^^^^^^^^^^^^^

Le format des produits de niveau 1 VENµS est inchangé.

Produits L1 Sentinel-2 (niveau 1-C) (S2A et S2B)
''''''''''''''''''''''''''''''''''''''''''''''''

Les méta-données
                

La chaîne contrôle la cohérence et l’intégrité des données Sentinel-2 de
niveau 1-C par l’application des schémas de l’ « \ *ICD-Sentinel-2* »
[PSD]_ pour les produits issus du PDGS.

Les informations nécessaires aux traitements algorithmiques sont
présentes dans les fichiers d’entête du produit Sentinel-2.

Une des spécificités des produits Sentinel-2 par rapport au produit
Venµs est la fourniture des masques associés à un produit de niveau 1
sous forme vecteur (format GML) dans le header du produit (masques des
pixels saturés, aberrants, no_data).

La valeur du **No_Data** est renseignée dans le nœud
Product_Characteristics/Product_Image_Characteristics/Special_Values/,
il y a deux valeurs définies :

.. code::

   SPECIAL_VALUE_TEXT : NODATA
   SPECIAL_VALUE_INDEX : 0

Il existe également un masque des No_Data au format vecteur dans les
fichiers d'entête. Ce masque est utilisé pour générer le masque des
No_Data au format image utilisé dans la chaîne.

Les informations sur les **bandes spectrales** sont définies dans le
nœud : Product_Characteristics/Spectral_Information_List :

.. code:: xml

     <Spectral_Information band_id="0" physical_band="B1">
          <RESOLUTION>60</RESOLUTION>
          <Wavelenght>
               <MIN unit="nm">414</MIN>
               <MAX unit="nm">472</MAX>
               <CENTRAL unit="nm">443.0</CENTRAL>
          </Wavelenght>
          <Spectral_Response>
               <STEP unit="nm">1.0</STEP>
               <VALUES>0.00000726 0.0000082 0.00000245 …</VALUES>
          </Spectral_Response>
     </Spectral_Information>

Les informations sur les tuiles sont contenues dans le nœud
Data_Strip/Tiles_List/Tile, avec un « Id » unique par tuile. La chaîne
MAJA ne traite qu’une tuile. Il est donc nécessaire de récupérer l’Id de
la tuile traitée pour aller chercher les informations dans le bon nœud
« Tile » du header principal.

Les données SOL et VIE sont décrites dans le Header du produit 1-C. Par
exemple, une liste de nœuds nommés « Viewing_Incidence_Angles_Grids »
définissent les angles pour chaque bande et pour chaque zone. Ces
informations sont extraites du header pour reconstituer l’image des
angles de prises de vues et en déduire également le masque des zones.

Le header principal fournit donc :

-  les angles solaires et les angles de prises de vue. Ils sont définis
   respectivement dans les nœuds « Sun_Angles_Grid » et
   « Viewing_Incidence_Angles_Grids ».

-  le chemin pointant sur les fichiers des masques (format GML).

Dans les fichiers de masque (dans le nœud Mask_List/MASK_FILENAME), il y
a pour chaque bande :

-  Les pixels aberrants,

-  Les pixels saturés.

   Ces données sont à lire afin d’extraire ces informations et en faire
   des masques (rasterisation).

Images
      

La donnée image est au format JPEG2000, avec 13 fichiers, un par bande
spectrale.

Les fichiers sont listés dans le header xml, dans le nœud
Product_Characteristics/Product_Image_Characteristics/Data_Access/DATA_FILENAME.
Cependant, il faut s’assurer que les noms des fichiers lus correspondent
bien à la tuile traitée.

La lecture des images JPEG2000 est réalisée avec l’OTB.

Les produits au format MUSCATE
''''''''''''''''''''''''''''''

Les produits de niveau 1 Landsat 5/7/8 et Sentinel2 (S2A et S2B) au
format MUSCATE sont lus en respectant les interfaces définies dans le [PSC]_.

Produits de NIVEAU 2
^^^^^^^^^^^^^^^^^^^^

Les produits de niveau 2 VENµS sont écrits en respectant les interfaces
[DA06]_ et [DA07]_. 

La chaîne MAJA génère des produits de niveau 2 au format
« EarthExplorer » pour les capteurs Formosat, Sentinel-2 GPP, Sentinel2
natif PDGS (S2A et S2B), Landsat (L5 et L7), Landsat 8 et Spot4. Ces
produits sont conformes à un format qui est défini dans une nouvelle
interface appellée « \ **ICD-MAJA** ». Elle assure la conformité de tous
les produits générés par MAJA dans le format défini pour chaque
satellite, tout en s’assurant qu’un produit VENµS est bien conforme à
l’ICD Venµs.

Les produits de niveau 2 Sentinel2 GPP (S2A et S2B) sont écrits en
respectant les interfaces définies dans le Plugin SENTINEL2_GPP
(création des schémas de ces produits de niveau 2 dans un ICD
Sentinel2_GPP).

Les produits de niveau 2 Sentinel2 PDGS (S2A et S2B) sont écrits en
respectant les interfaces définies dans le Plugin SENTINEL2 (création
des schémas de ces produits dans un ICD Sentinel2).

Les produits de niveau 2 Landsat 8 natif sont écrits en respectant les
interfaces définies dans le Plugin LANDSAT8 (création des schémas de ces
produits dans un ICD LANDSAT8).

Une description détaillée (sous forme de tableaux Excel) de la
constitution de ces produits est présentée dans le [MU]_.

   Répond à: Exigence 50 dans : [DA01]_ Conformité : C

   Répond à: Exigence 60 dans : [DA01]_ Conformité : C

   Répond à: Exigence 440 dans : [DA01]_ Conformité : C

   Répond à: Exigence 450 dans : [DA01]_ Conformité : C

   Répond à: Exigence 600 dans : [DA01]_ Conformité : C

   Répond à: Exigence 610 dans : [DA01]_ Conformité : C

   Répond à: Exigence 650 dans : [DA01]_ Conformité : C

Le produit de niveau 2A généré à partir du produit Sentinel-2 1-C est
écrit dans un format inspiré du format VENµS comme décrit dans le
[DA06]_.

Le produit est constitué d’un fichier d’entête .HDR contenant toutes les
informations générales du produit image (comme un produit VENUS) et d’un
répertoire .DBL.DIR contenant les fichiers constitutifs du produit
image.

Le produit de niveau 2, généré à partir de produits Landsat 5 et 7, est
écrit dans un format inspiré du format VENµS comme décrit dans le
[DA06]_. Il ressemble aux produits Venµs et Formosat de niveau 2
actuellement générés par la chaîne L2 VENµS.

Les données lues et écrites dans la partie privée, ne sont pas impactées
par la multi-résolution de Sentinel-2, elles sont écrites à la
résolution réduite des produits L2 (L2Coarse).

Produits au format MUSCATE :

Les produits de niveau 2 Landsat 5/7/8 et Sentinel2 (S2A et S2B) au
format MUSCATE sont écrits en respectant les interfaces définies dans le
[PSC]_.

Interface des fichiers de configuration
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Les fichiers de configuration sont les suivants :

-  MAJA\ **Admin**\ ConfigSystem.xml : les paramètres de configuration
   destinés au fonctionnement de la chaîne et ne devant en aucun cas
   être modifiés par l’utilisateur,

-  MAJA\ **User**\ ConfigSystem.xml : les paramètres de configuration
   utilisateurs communs à la chaîne, ne dépendant pas du capteur,

-  MAJA\ **User**\ Config_<CAPTEUR>.xml : les paramètres de
   configuration utilisateurs dépendant du capteur.

-  MAJA\ **Admin**\ Config_<CAPTEUR>.xml : les paramètres de
   configuration administrateurs dépendant du capteur.

Les paramètres de configuration dépendant du capteur et destinés au
fonctionnement de la chaîne sont pris en charge par le plugin et
implémentés dans le code source. Leur schéma est défini dans les schémas
au niveau du **Plugin**.

La solution technique implémentée permet de générer un produit de niveau
2 qui sera identique quelles que soient les conditions de « génération
», c’est-à-dire qu’il ait été généré :

-  en mode INIT ou

-  en mode BACKWARD ou NOMINAL avec un produit de niveau 2 en entrée
   issu du même capteur ou non.

Ainsi dans le produit de niveau 2 les produits de réflectances de la
partie PRIVATE contiendront un nombre identique de bandes, quelque sois
le capteur traité (à une condition près, décrit ci-dessous).

Note : le fichier de configuration
MAJA\ **Admin**\ Config_<CAPTEUR>.xml contient la liste des longueurs
d’ondes théoriques pour chaque bande du capteur.

La gestion des résolutions est revue. Elle est désormais gérée dans le
code.

**Note** : les modifications apportées au fichier de configuration
n’impacte aucunement l’interface avec le VIP.

Interface des GIPPs
~~~~~~~~~~~~~~~~~~~

La conception a permis de dire qu’aucun nouveau GIPP n’est nécessaire
pour les besoins de MAJA ; seuls les GIPPs actuels sont revus.

Par exemple, le GIPP principal GIP_L2COMM est mis à jour. Le nœud
contenant la définition des bandes et leurs indices informatiques est
déplacé dans le fichier de configuration (ou dans les factories de
lecture/écriture des produits). Les nouveaux paramètres tels que ceux
liés au paramétrage du SnowMasking sont créés.

Pour connaître en détail les éléments supprimés/modifiés/ajoutés des
GIPPs, il convient de se reporter aux GIPPs mis à jour et livrés en fin
de phase de conception. Ces GIPPs sont utilisés par le CNES pour définir
les contextes de validation de la maquette.

Remarque : La phase de développement a permis d’identifier la création
de paramètres dont les valeurs dépendent du satellite. En effet, pour
l’algorithme d’AOT_Estimation, il est nécessaire de définir, dans le cas
par exemple de Sentinel2, des valeurs pour les deux paramètres suivant
« MS_Slope » et « MS_Yintercept » différentes pour le satellite S2A et
pour le satellite S2B. Ces valeurs restent définies dans le fichier GIP
L2COMM ; en conséquence, il est décidé qu’il y a en entrée de MAJA
autant de fichier GIP_L2COMM que de satellite traité. Ainsi par exemple
pour le cas Sentinel2, les deux fichiers S2A et S2B seront exigés en
entrée de MAJA.

Le GIPP L2COMM est modifié. Il contient désormais **seulement** les
paramètres des algorithmes activés pour le **capteur** concerné. Ainsi,
pour le cas VENUS, les paramètres du SnowMasking n’existent plus. En
conséquence, tous les schémas des fichiers GIPPs sont désormais définis
dans l’ICD du **plugin** concerné, assurant ainsi l’indépendance des
interfaces des GIPPs d’un capteur à l’autre.

Pour la version MACCS 4.0, le GIP_L2COMM a une nouvelle fois été mis à
jour. Des paramètres communs ont été déplacés tel que le paramètre
Slope_Min_Cos_I désormais utilisé dans les algorithmes Snow Masking et
Slope Correction. De nouveaux paramètres ont également été ajoutés suite
aux évolutions algorithmiques apportées dans cette version. La liste des
algorithmes modifiés et les évolutions sont décrites dans :ref:`algorithms:description des evolutions algorithmiques de la chaine`.

Arborescence du code source
---------------------------

La mise en œuvre du MAJA générique a eu pour conséquence une
modification importante de l’arborescence du projet.

.. image:: Art/ArborescenceGenerale.png
   :width: 3.70347in
   :height: 6.25903in
   :title: Arborescence du code source de MAJA

Cmake est utilisé pour compiler le projet MAJA et ses dépendances.

Les répertoires « Testing », “\ **Checking**\ ”, “\ **CMakeConfig**\ ”,
“\ **ddc**\ ” et “\ **Doxygen**\ ”, non décrits ici, sont utilisés pour
gérer le projet.

Le répertoire « make » contient les fichiers permettant de définir les
options de compilations pour les modes « release » et « debug ») en
fonction du compilateur sélectionné (ex : gcc ou icc).

Le répertoire « \ **Utilities »** contient les outils externes intégré
au projet. On trouve par exemple le répertoire « I3D » contient des
algorithmes pour calculer une direction à partir d’angles de visée et
des angles solaires en utilisant un MNT.

Le répertoire « \ **orchestrator»** contient le code python de l'orchestrateur.
Il est en charge de lire/écrire les produits dans les differents formats de
capteur. Il enchaine les algorithmes en fonction du mode choisis (INIT, NOMINAL et BACKWARD)
Le répertoire « \ **orchestrator»** est organisé comme suit :


.. image:: Art/ArborescenceOrchestrateur.png


Figure 6: Vue générale du répertoire « orchestrator » de MAJA

Ce répertoire  est constitué des sous
répertoires suivants :

-  Le répertoire « \ **algorithms »** contient le code source des
   algorithmes implémentés en python dans MAJA. Ce répertoire est détaillé dans le
   chapitre 5 "Les algorithmes ».

-  Le répertoire « \ **common »** contient les fonctions communes transverses comme 
   les outils de lecture/ecriture des xml ... Il est également constitués des sous-répertoires
   suivants :

 * Le répertoire « conf » contient des fonctionnalités de configuration 
   notamment pour lire les fichiers de configuration des plugins
   algorithmes

 * Le répertoire « dem » contient le code source relatif à la lecture des dem

 * Le répertoire « earth_explorer » contient les éléments commun relatifs au format earth explorer.

 * Le répertoire « interfaces » contient les outils de lecture/ecriture des fichiers interfaces avec les applications.
   En effet certaines interfaces des applications algorithmes sont trop complexes pour être mise en paramètres
   et dons passe sous forme de xml.

 * Le répertoire « logger » contient le code relatif à la configuration du logger de l'ordonanceur.

 * Le répertoire « muscate » contient les éléments commun relatifs au format muscate.

-  Le répertoire « \ **cots »** contient les wrappers de cots externe comme l'otb ou gdal

-  Le répertoire « \ **launcher »** contient le code du lanceur d'application permettant notamment de lire les paramètres
   de l'application

-  Le répertoire « \ **modules »** contient la définition de l'interface d'un module. Ceci est une interface générique 
   de module de traitement. Une factory permet alors de fournir une instance de l'algorithme. 
   Par exemple getinstanceof("CirrusCorrection") renvois un module de correction des cirrus.

-  Le répertoire « \ **plugins »** contient le code python permettant de gérer la lecture et l'ecriture des differents
   formats de produits capteurs.

-  Le répertoire « \ **processor »** contient le code python des differentes chaines INIT/NOMINAL/BACKWARD.

-  Le répertoire « \ **tests »** contient l'ensembles des tests du code pythonde l'orchestrateur.


Le répertoire « \ **Packaging »** contient les procédure cmake permettant de generer le paquet binaire ".run" de maja.

Le répértoire « \ **Superbuild »** contient les fichiers CMake
permettant de construire Maja à l'aide des SuperbuildArchives
qui contiennent l'ensemble des dépendances, afin de ne pas
avoir à installer les dépendances sur la machine. De plus
cela permet de créer une version de Maja incluant l'ensemble 
de ses dépendances.

Le répertoire « \ **Plugins »** contient l’ensemble des ressources (xsd,GIPPs et templates)
des differents capteurs. Ce répertoire est décrit dans le [SDM].

Le répertoire « \ **Code »** est organisé comme suit :

.. image:: Art/ArborescenceCode.png

Figure 6: Vue générale du répertoire « Code » de MAJA

Ce répertoire contient tout le code source de MAJA qui ne dépend pas des
produits gérés et donc des plugins. Ce répertoire est constitué des sous
répertoires suivants :

-  Le répertoire « \ **Algorithms »** contient le code source des
   algorithmes implémentés dans MAJA. Ce répertoire est détaillé dans le
   chapitre 5 "Les algorithmes ».

-  Le répertoire « \ **Core »** est constitués des sous-répertoires
   suivants :

 * Le répertoire « Data » contient des fonctionnalités de bases et
   rendent des services aux couches de plus haut niveau telles que les
   algorithmes

 * Le répertoire « Application » contient le code source des exécutables
   C++ de MAJA (un ou plusieurs exécutables pour chaque algorithme, 
   ainsi que des applications communes transverses)

 *  Le répertoire « Scripts » contient l’ensemble des scripts du projet
    (.sh and .py). Il contient par exemple les scripts principaux
    « \ **maja** » (fichier bash et python).

**Attention :** Ce répertoire « Code » n’est pas impacté par l’ajout
d’un nouveau Plugin, du moment où celui-ci n’implique pas l’ajout d’un
nouvel algorithme ou la modification d’un existant.

Arborescence de déploiement
---------------------------

L’arborescence de déploiement est décrite en détail dans le [MU]_.

Architecture statique
---------------------

Du fait de l'utilisation de deux langages au sein de MAJA deux architecture se cotoient.
D'un coté les algorithmes, codés en C++, et de l'autre l'orchestrateur, codé en Python.

Architecture de la partie c++
-----------------------------

Les différentes couches C++ de MAJA se présentent comme ceci (du bas vers le
haut):

-  La couche « \ **Data** » contient l’ensemble des fonctionnalités de
   bases et offrent des services au couches supérieures. Par exemple,
   elle contient des classes pour faciliter la manipulation de fichiers
   XML (lecture, écriture, ….), les classes de base pour
   la gestion des loggers, des classes d’exceptions pour gérer les
   erreurs, les classes de bases pour lire les LUTs ou les fichiers d'interfaces.

-  La couche « \ **Algorithms** » contient l’ensemble des classes
   implémentant les algorithmes de MAJA, celles-ci sont alors
   encapsulées dans des applications OTB afin d'être appelées
   par l'orchestrateur python.

-  La couche « \ **Script** » contient le script principal **maja**
   ainsi que des scripts de haut niveau utilisés pour lancer MAJA


Architecture de la partie Python
--------------------------------

Les différentes couches Python de MAJA se présentent comme ceci (du bas vers le
haut):

-  La couche « \ **Data** » contient l’ensemble des fonctionnalités de
   bases et offrent des services au couches supérieures. Par exemple,
   elle contient des classes pour faciliter la manipulation de fichiers
   XML (lecture, écriture, ….), les classes de base pour
   la gestion des loggers, des classes d’exceptions pour gérer les
   erreurs, les classes de bases pour lire les fichiers d'interfaces et de configuration.

-  La couche « \ **Plugins** » contient l’ensemble des classes
   implémentant les plugins de MAJA. Les plugins sont implémentés sous forme de factory.
   Pour chaque produit que l'on souhaite lire on demande à la factory qui serait capable
   de le lire. Chaque plugin intègre :

    * Un lecteur d'informations sur le produits L1
    * Un lecteur de produit L1
    * Un lecteur de produit L2
    * Un writer de produit L2 (métadonnées et images)

-  La couche « \ **Chain** » contient l'enchainement permettant de passer d'un produit L1/L2
   à un produit de sortie L2. Les chaines disponible sont décrite dans le [MU]_


On note que l’on s’appuie sur l’OTB pour lire les fichiers JPEG2000 du
produit Sentinel-2. Les masques des produits Sentinel-2 au format GML
sont lus avec GDAL.

Les évolutions nécessaires pour implémenter MAJA sont présentées dans
les sections suivantes.

Les composants logiciels
------------------------

Composants Data Manager
~~~~~~~~~~~~~~~~~~~~~~~

Présentation
^^^^^^^^^^^^

De manière générale, toutes les procédures de lecture et d’écriture sur
les produits de niveaux 1, 2 sont prises en charge et implémentées
dans les plugins. L’architecture est décrite en détail dans le document
[SDM]_.

Phase d’ « Initialisation » et management principal de l’application
''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''

L'initialisation des paramètres généraux de l'application ainsi que la lecture des
fichiers de configuration système sont effectués en python par la classe AppHandler du launcher.
Celle-ci récupère les paramètres et initialize un contexte d'éxécution.

Lecture des angles de prises de vue :
'''''''''''''''''''''''''''''''''''''

Dans la chaîne MAJA, aucune modification n’est apportée aux calculs des
angles de prises de vue réalisés dans la chaîne VENµS L2 pour les
capteurs Venµs et Formosat :

-  pour Venµs, des grilles contenant les décalages des pixels en
   ligne/colonne (X,Y) sont fournies avec les produits de niveau 1.
   Toutefois, la variation des angles dans le champ n’est prise en
   compte que pour la détection des ombres et des faces cachées. Les
   autres algorithmes utilisent les valeurs des angles au centre de
   l’image.

-  pour Formosat, ces grilles n’existent pas et sont générées par la
   chaîne à partir des valeurs d’angles solaires et de visée constants
   sur toute l’image.

Les grilles d’angles des capteurs Sentinel-2 et Landsat sont également
générées au niveau de cette Factory.

Comme pour Formosat, Landsat, Landsat8 et Spot4 ne disposent pas de ces
grilles en entrée. Des images constantes sont donc générées à partir des
valeurs des angles solaires et de visée au centre de l’image.

En revanche, Sentinel-2 est un capteur à champ large pour lequel la
variation des angles dans le champ ne peut plus être négligée. Il est
donc nécessaire de disposer de la valeur de ces angles par pixel. Pour
se ramener au format utilisé dans la chaîne VENµS, des grilles d’angles
(image raster) doivent être produites. Ces grilles d’angles de prises de
vue sont générées à partir des images vecteurs contenues dans le header
principal d’un produit Sentinel-2.

**Pour les angles solaires**, ces images vecteurs sont simplement
converties en images raster et les valeurs d’angles sont ramenées en
position (X,Y) dans l’image. Les grilles ainsi obtenues sont ensuite ré
échantillonnées à la résolution réduite des produits L2.

**Pour les angles de visée**, des images vecteurs sont fournies pour
chaque bande spectrale et surtout chaque détecteur. En effet, une tuile
traitée dans la chaîne est potentiellement acquise par plusieurs
détecteurs. Ces images sont ramenées à la résolution réduite des
produits L2 en interpolant les valeurs d’angles au niveau des zones de
recouvrement des détecteurs. L’image raster des angles de visée est
finalement reconstituée à l’aide d’un masque associant chaque pixel à
chaque détecteur de la tuile.

Pour simplifier les calculs à la pleine résolution dans l’algorithme de
correction atmosphérique, l’image est divisée, par bande spectrale, en
sous zones d’angles de visée constants. Des grilles d’angles de visée
par détecteur sont générées et contiennent la valeur moyenne des angles
sur chaque zone. Une image à pleine résolution donne pour chaque pixel
la zone à laquelle il appartient.

Il est important de noter que si les grilles contiennent déjà des
coordonnées physiques, il n’est plus nécessaire de multiplier les
valeurs d’angles par la taille du pixel au niveau de chaque algorithme.

Module SubSampling Sentinel-2
'''''''''''''''''''''''''''''

**Images de réflectance**

Lors de la lecture d’un produit Sentinel-2 de niveau 1, les images de
réflectance sont scindées en trois images contenant chacune les bandes
de résolution identique (10m, 20m, 60m). Ces images sont
rééchantillonnées à la résolution réduite des produits L2 puis
reconcaténées dans une vecteur image unique.

**Masque des bords**

Dans le cas Venµs et Sentinel-2, un masque des bords est généré à partir de l’image de
réflectance de niveau 1 en déclarant comme pixels de bords d’image tous
les pixels à No_Data dans le produit de niveau 1.

**Masque des pixels saturés**

Pour Sentinel-2, les pixels saturés sont contenus dans des fichiers GML
sous forme de polygones. Des fonctionnalités présentes dans GDAL
permettent de convertir des polygones en une image raster.

Deux masques de pixels saturés sont fournis dans le header principal
d’un produit Sentinel-2 1C :

-  Un masque des pixels saturés « bord » et

-  Un masque des pixels saturés radiométriques.

Une fois les images raster produites, un *ou* logique est réalisé entre
les deux masques. Ces masques sont ensuite fournis au filtre Subsampling
pour être ré échantillonnées aux différentes résolutions.

Note sur la gestion de la multi-resolution
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

La correspondance des bandes spectrales avec leurs indices
informatiques, initialement définies dans le GIP_L2COMM, est ajoutée
dans la factory de chaque capteur. Ainsi, une table de correspondance
d’indice est définie spécifiquement pour Sentinel-2 afin de faire le
lien entre les bandes thématiques (B01, B02, …) et leurs indices
informatiques en fonction de la résolution :

-  B02 => indice 1 pour la résolution 10m,

-  B03 => indice 2 pour la résolution 10m,

-  B05 => indice 1 pour la résolution 20 m,

-  etc.

Ces tables d’indices sont utilisées pour identifier les bandes en
fonction des deux images de type « VectorImage » utilisées pour chacune
des deux résolutions.

La classe python **BandInformations** contient une table de correspondance
associant l’indice informatique dans l’image au code de la bande. Dans
le cas Sentinel-2, cette classe contient les deux tables de
correspondance pour les résolutions 10m et 20m. Une structure est
définie pour associer à chaque bande spectrale d’une résolution donnée :

-  son indice dans un produit complet comportant les 13 bandes
   spectrales : tabR10[3].index = 7,

-  la chaîne de caractère associée à cette bande : tabR10[3].bandcode =
   « B07 ».

Cette classe offre également un ensemble de « services » tels que le
renvoi de l’indice à partir de l’indice informatique de la bande dans
l’image de résolution associée, le renvoi de l’indice à partir du code
ou l’inverse.

Composants IO
~~~~~~~~~~~~~

Gestion des LUTs dépendantes des bandes spectrales
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Une fois encore, les contraintes multi capteurs et multi résolutions ont
des impacts sur la gestion des LUTs au niveau de la classe
python L2Processor.

Une LUT est définie par capteur et par bande spectrale. Cela implique
que les fichiers GIPPs associés sont dépendants du satellite et que la
chaîne peut, au cours d’un même traitement, gérer plusieurs LUTs : une
LUT pour le produit à la date D (landsat 5 par exemple) et une autre
pour le produit à la date D-1 (qui peut être un produit Landsat 7). Au
lieu de registrer un seul fichier GIPP par traitement, la chaîne L2 doit
maintenant travailler avec autant de fichiers GIPPs que de produits
traités.

De plus, comme la résolution L2 dépend de la bande spectrale dans les
produits Sentinel-2, les traitements lancés à la pleine résolution L2
sont séparés de manière à ne traiter qu’une seule résolution à la fois.
La liste des fichiers associés à un plugin est établie en utilisant 
la table de correspondance entre les bandes
spectrales et les résolutions. Par exemple, pour chaque bande spectrale
*bs* à la résolution de 10m, le fichier LUT correspondant est chargé
(table_correspondance[bs] = indice de la bande parmi les 13 bandes d’un
produit sentinel-2, cf section :ref:`presentation:note sur la gestion de la multi-resolution`).

Une dernière spécificité des produits Sentinel-2 doit être prise en
compte lors de la génération des LUTs réduites. La variation des angles
de visée dans le champ ne peut plus être négligée à la pleine résolution
des produits L2. L’image est divisée en zones d’angles de visée
constants. Une **LUT réduite est générée par zone** en fixant les
valeurs des angles zénithaux solaire et de visée et de l’angle azimutal
relatif.

Finalement, sont créés autant de vecteurs de **LookUpTable**
contenant une bande par zone, que de résolutions L2 présentes dans le
produit traité.

I3D
~~~

I3D est une librairie C++ permettant de calculer l’intersection d’une
direction avec un MNT en utilisant :

-  Un Mnt,

-  Un point de départ,

-  Un vecteur de visée.

La librairie a été modifiée afin de s’intégrer au pipeline OTB notamment
en permettant d’injecter le MNT sous forme de buffer.

L’algorithme commence en définissant une boite englobante à partir de
l’altitude min et max du MNT ainsi que des limites de la zone chargée
(typiquement le nombre de ligne et colonne en repère capteur).

Ensuite les différents points d’intersection entre la ligne de visée et
les plans du cube sont listés. Ces points représentent des points
d’entrée du rayon qui permettront ensuite d’établir l’intersection. Ces
points sont ensuite filtrés. Avant l’évolution FT-2168 ces points
subissaient non seulement un test d’inclusion dans le cube mais
également un test d’égalité. Cependant le test d’égalité a été enlevé
car de toute façon seul le point le plus pertinent est conservé.

Après filtrage le nombre de points trouvés permet de déterminer le cas
de figure:

-  0 point : le rayon ne passe pas par le cube, aucune intersection
   possible,

-  1 point : le rayon est tangent au cube, aucune intersection possible,

-  2 points et plus : avant la FT-2168 lorsque plus de 2 points étaient
   trouvés le programme sortait en erreur et ne cherchait pas plus loin.
   Cependant bien qu’au sens mathématique il ne puisse y avoir plus de
   deux points il se trouve que les imprécisions du modèle numérique
   permettent de trouver 2 points très proches mais pas égaux au sens
   informatique sur les arrête du cube ce qui résultait en une sortie
   prématurée du programme alors que les conditions étaient valides.
   Dorénavant dès qu’au moins deux points sont trouvés dans le cube la
   recherche d’intersection continue.

Le segment définis par le point d’entrée et la direction de visée est
alors parcouru pour déterminer l’intersection avec le MNT.

.. image:: Art/image5.png
   :width: 5.89583in
   :height: 3.26042in


Une autre utilisation de l’algorithme permet de déterminer si un point
est caché ou dans l’ombre d’un relief. Pour cela, le point fourni à
l’algorithme est le point que l’on souhaite analyser, la direction de
visée étant celle du capteur.

Si l’algorithme trouve une intersection avec le MNT différente du point
d’entrée alors le point est caché.

.. image:: Art/image6.png
   :width: 5.34375in
   :height: 3.20833in

