Les algorithmes
===============

Séquencement des étapes de la chaîne L2
---------------------------------------

Le développement de l’outil MAJA ne remet pas fondamentalement en cause
l’enchaînement des algorithmes de la chaîne VENµS L2.

Le câblage de la chaîne L2 est remanié pour prendre en compte les
spécificités algorithmiques de chaque capteur :

-  Quelques algorithmes, tels que la détection des cirrus ou de la
   neige, sont spécifiques à un capteur donné et nécessitent un câblage
   particulier. Des paramètres définis dans le fichier de configuration
   permettent d’activer ou pas un algorithme en fonction du capteur
   considéré.

-  Le traitement à plusieurs résolutions L2 pour un même produit
   Sentinel-2 entraîne des modifications dans l’enchaînement des
   algorithmes de la chaîne existante.

Des remaniements étant déjà opérés dans la classe *vnsL2Processor*, la
duplication des appels aux algorithmes *Cloud Masking*, *Water Masking*
et *Scattering Correction* en fonction du mode de traitement (Init ou
Nominal) est supprimée. Pour une meilleure lisibilité dans le code, le
câblage des données ou le paramétrage des algorithmes spécifiques à un
mode donné sont encapsulés dans des tests.

La gestion des résolutions multiples n’apparaît pas au niveau de
l’enchaînement des algorithmes si ce n’est dans le passage des images
d’entrée aux différents filtres. Les appels multiples aux différentes
résolutions L2 sont gérés à l’intérieur même des filtres algorithmiques
de manière à ne pas remonter dans la chaîne principale des traitements
nécessaires à la génération d’un produit intermédiaire validé dans les
tests de validation algorithmiques (TVA).

La lecture ou l’écriture des produits à résolutions multiples sont
prises en charges par le plugin dédié.

Les évolutions apportées dans la version MACCS 4.0 ne modifient pas
l’enchainement des algorithmes dans la chaine L2.

.. image:: Art/image7.png
   :width: 5.58333in
   :height: 5.75in

Figure 8 : Séquencement des étapes de la chaîne L2

Arborescence du code source
---------------------------

Le répertoire des algorithmes est organisé comme suit :

.. image:: Art/image8.png
   :width: 4.125in
   :height: 5.8125in
   :title: Arborescence du code source des algorithmes de MAJA

Le répertoire « \ **Algorithms »** contient le code source des
algorithmes implémentés dans MAJA. Ce répertoire est constitué des sous
répertoires suivants :

-  Le répertoire “Common” contient des filtres de base utilisés par
   plusieurs traitements algorithmiques de haut niveau,

-  Le répertoire « L2 » contient les algorithmes des chaînes images,
   répartis dans des sous répertoires :

   *  “AOTEstimation”

   *  “CirrusFlag”

   *  “CloudMasking”

   *  “CompositeImage”

   *  “EnvironmentCorrection”

   *  “RainDetection”

   *  “ScatteringCorrection”

   *  “SnowMasking”

   *  “WaterMasking”

   *  “AtmosphericAbsorptionCorrection”

   *  “CirrusMask”

   *  “DTMProcessing”

   *  “GeometricFlags”

   *  “RayleighCorrection”

   *  “SlopeCorrection”

   *  “Subsampling”


Les composants logiciels
------------------------

Démarche générale d’implémentation
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Les traitements algorithmiques de la chaîne VENµS L2 sont modifiés pour
prendre en compte les produits Sentinel-2 (Level 1-C).

De manière générale, les algorithmes de la chaîne MAJA doivent supportés
deux nouvelles contraintes. Ils doivent être à la fois :

-  **multi capteurs** pour pouvoir traiter des produits L1 VENµS et
   Sentinel-2 et

-  **multi résolutions** pour supporter les produits Sentinel-2 (level
   1-C, S2A et S2B) dans lesquels les bandes spectrales ont des
   résolutions différentes.

De plus, lors du traitement des produits Sentinel-2, certains
algorithmes travaillant à la pleine résolution des produits L2
(correction de la diffusion) doivent maintenant prendre en compte la
**variation des angles de visée dans l’image**.

Enfin, de nombreuses évolutions sont apportées à l’\ **algorithme
d’estimation de l’épaisseur optique des aérosols** avec l’ajout de
**deux nouvelles méthodes** : la méthode multi-spectrale et la méthode
spectro-temporelle.

Il est important de noter que l’ajout ou le remaniement d’algorithmes
entraîne une mise à jour du GIPP L2COMM contenant les paramètres de
traitement (voir section :ref:`presentation:interface des gipps`).

Avant de détailler les évolutions dans chaque algorithme, il convient
d’analyser l’impact des contraintes multi capteurs et multi résolutions
sur l’ensemble des algorithmes de manière à définir différents choix
d’implémentation réalisables dans la chaîne L2.

Dans un premier temps, le travail d’analyse mené pour VENµS sur la
circulation des données dans la chaîne a été repris pour les produits
Sentinel-2 de manière à identifier les nouvelles entrées/sorties de la
chaîne et les algorithmes concernés par la gestion de plusieurs
résolution L2.

.. image:: Art/image9.png
   :width: 6.84375in
   :height: 5.5in
.. image:: Art/image10.png
   :width: 6.59375in
   :height: 4.875in
.. image:: Art/image11.png
   :width: 6.34375in
   :height: 6.53125in

Figure 10 Circulation des données dans la chaîne L2 pour un produit
Sentinel-2

Pour gérer la composante multi capteurs dans la chaîne, différents choix
d’implémentation ont été identifiés :

-  Câblage des algorithmes spécifiques à un capteur via des paramètres
   de test ajoutés dans le fichier de configuration,

-  Combler l’absence d’un plan image en entrée d’un algorithme par la
   génération d’une image constante :

-  soit au moment de lire les données de niveau 1 comme c’est le cas
   pour le plan contenant l’altitude des nuages (ce qui est totalement
   transparent dans l’enchaînement des algorithmes),

-  soit lors du câblage des algorithmes via des paramètres de test
   (génération d’une image de label des nuages avec un label unique pour
   toute l’image),

-  Ajouter des entrées optionnelles aux algorithmes. Cela implique que
   chaque accès à cette entrée est géré par un test d’existence de cette
   donnée. (la génération de l’image composite utilise la partie privée
   d’un produit L2 à la date D-1 dans le cas nominal mais pas en mode
   init).

Chaque solution présente des avantages et des inconvénients. Par
exemple, l’utilisation d’entrées optionnelles dans les couches basses
des algorithmes impliquent soit l’ajout des tests dans des boucles sur
les pixels à chaque fois que l’on veut manipuler la donnée soit de
dupliquer le code. En revanche, cela est tout à fait envisageable quand
ces données sont utilisées dans des filtres dits « composite » qui ne
change pas l’implémentation de l’algorithme appelé.

La génération d’image constante permet, quant à elle, de conserver le
code le plus générique possible pour tous les capteurs tant que cela
n’impacte pas les performances de la chaîne (à la résolution réduite L2
par exemple). Un compromis entre garder la généricité du code et lancer
des algorithmes qui ne donneront aucun résultat significatif doit être
trouvé. C’est pourquoi, il n’est pas possible de définir une ligne de
conduite unique pour tous les algorithmes présents dans la chaîne. Une
analyse des évolutions algorithmiques de la chaîne L2 est nécessaire
pour choisir au cas par cas la solution la plus appropriée.

Description des evolutions algorithmiques de la chaine
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Dans ce chapitre, seuls les traitements algorithmiques concernés par les
évolutions MAJA sont présentés.

**Note pour la version 3.0** : les spécificités liées aux satellites
Formosat et Landsat sont présentées même si ces capteurs (i.e. les
plugins) correspondants ne sont pas traités dans la version 3.0. Ils le
seront dans la prochaine version.

**Note pour la version 4.0** : les plug-ins des capteurs Formosat,
Landsat 5, 7 et 8, et Spot4 ont été développés. Des évolutions
algorithmiques (décrites dans la version 2.2 du document
LAIG-ST-MAC-20-CNES_SpecMethode-Trace) ont également été apportées
entrainant la mise à jour de certains GIPPs et notamment du GIPP L2COMM.
Les algorithmes concernés sont :

-  Cirrus Mask

-  Snow Masking

-  Cloud Masking

-  Water Masking

-  AOT Estimation

   **Note pour la version MAJA 1.0 :** la méthode de détection des
   nuages de CloudMasking a été modifiée pour intégrer la méthode du
   DLR. De plus la correction atmosphérique a été modifiée pour ajouter
   en post processing l’estimation de la vapeur d’eau. Le GIPP L2COMM a
   été modifié afin d’ajouter les nouveaux paramètres nécessaires.

   **Note pour la version MAJA 4.0 :** la méthode de détection des
   nuages de CloudMasking a été modifiée pour intégrer la méthode d'Olivier
   Hagole. Le GIPP L2COMM a été modifié afin d’ajouter les nouveaux paramètres nécessaires.

   Les algorithmes concernés sont :

-  Correction of Atmospheric Absorption

   **Note pour la version MAJA 3.0 :** L'intégration des données CAMS au sein de
   MAJA permet maintenant une bien meilleure correction en fonction de la zone 
   géographique.


-  Cloud Masking

   **Note pour la version MAJA 2.0 :** Des évolutions algorithmiques ont
   été apportées à cette version concernant la correction des effets
   directionnels d’une part et la correction des nuages cirrus d’autre
   part. Des modifications du GIPP L2COMM ont découlé de ces évolutions.

   Les Algorithmes ajoutés sont donc :

-  Cirrus Correction

-  Directional correction

AngleGrid processing
^^^^^^^^^^^^^^^^^^^^

Ce module a été ajouté dans le [DA02] pour spécifier le calcul des
angles solaires et de visée en fonction des différents capteurs. De
manière générale :

-  les angles de visée sont constants pour les satellites Landsat et
   Formosat mais dépendent de la position du pixel et de la bande
   spectrale dans le cas Venµs et Sentinel-2,

-  les angles solaires sont constants pour les capteurs Landsat et
   Formosat mais dépendent de la position du pixel pour Venµs et
   Sentinel-2 (mais non de la bande spectrale).

Dans la chaîne VENµS L2, la génération des grilles d’angles n’est pas
réalisée dans un module spécifique mais au niveau de la « Factory »
chargée de la lecture du produit de niveau 1 en fonction de chaque
capteur.

DTM Processing
^^^^^^^^^^^^^^

Peu de changement sont réalisés dans cet algorithme si ce n’est la prise
en compte des différentes grilles d’angles solaires et de visée en
fonction du capteur.

Dans la chaîne VENµS L2, le filtre attend deux grilles d’angle de visée
en entrée (VIEB5 et VIEB6), ces dernières étant disponibles dans un
produit Venµs. Pour s’adapter à Venµs, deux grilles identiques sont
créées pour Formosat avec une valeur d’angle constante sur toute
l’image.

Pour adapter ce filtre aux produits Sentinel-2 et Landsat, une image
multi bandes, où chaque paire de bandes (grilles X et Y) correspond à un
plan (B5, B6, ...), est générée lors des prétraitements (au niveau de la
« Factory » qui gère la lecture des produits de niveau 1) et fournie en
entrée du filtre. Cette solution permet de garder le code le plus
générique possible.

Reduced Lut Computation
^^^^^^^^^^^^^^^^^^^^^^^

Ce module a été ajouté dans la version 2.0 de MAJA bien que l’algorithme
d’extraction de la mini-lut à partir de la LUT globale était disponible
avant. Cependant l’ajout de la correction des effets directionnels qui
s’applique directement sur cette mini-lut a justifié que cet algorithme
soit désormais une boite à part entière.

Il s’agit ici d’extraire par interpolation sur les angles solaires et
les angles de vues du produit une sous-lut propre au produit qui va
ensuite être utilisé dans les algorithmes suivant. On obtient alors une
lut à 3 dimensions alors que la lut d’entrée est,elle, à 6 dimensions.

La correction des effets directionnels consiste quant à elle à appliquer
pour chaque bande des coefficients multiplicateurs préalablement calculé
par l’algorithme. Cette correction permet de simuler un produit ayant
été acquis avec un angle différent de celui réel afin de pouvoir faire
de multi temporel avec deux produit ayant été pris avec des angles
légèrement différents. Cela est notamment le cas sur SENTINEL2 avec deux
acquisitions de la même zone sur deux orbites différentes.

A partir de MAJA v3.0, ce module a également la responsabilité, sous
réserve d’activation et de disponibilité de la donnée exogène, de
générer les LUTs DIFT, DIRT, ALBD et TOCR à partir des données CAMS
indiquant les teneurs de l’atmosphère relativement à certains aérosols,
ainsi que des LUTs relatives à ces même aérosols. Il est dorénavant
possible de d’estimer, grâce aux données CAMS, des lut DIFT, DIRT, ALBD
et TOCR plus représentatives de la réalité physique lors de
l’acquisition en lieu et place des LUTs constantes utilisées par défaut.
En contrepartie, les données auxiliaires à fournir à l’application MAJA
sont plus nombreuses et ont une période de validité contraignante, c’est
pourquoi la capacité d’utiliser des LUTs constantes est toujours
disponible et utilisée dans le cas général.

Subsampling
^^^^^^^^^^^

Modifier le module actuel « Susampling » afin qu’il gère l’aspect multi
résolution de S2 n’est pas la solution retenue. La solution consiste à
utiliser ce module pour les cas VENµS, Formosat et Landast. Pour le cas
Sentinel-2, on développe un nouveau module SubSamplingS2, en évitant
autant que faire se peut la duplication de code.

Par ailleurs, l’appel au module SubSampling est déplacé dans les Factory
afin de regrouper les spécificités liées aux formats des produits L1
dans le Plugin du capteur dédié.

A partir de la version 4.2, les étapes de rééchantilonnage sont
réalisées par un module commun à tout MACCS, la classe
vnsPadAndResampleImageFilter. Elle s’appuie sur des filtres
d’interpolation de l’OTB, d’ITK et certains développés dans MACCS pour
des soucis de performances.

Le filtre de l’OTB StreamingResampleImageFilter initialement utilisé
pour réaliser les rééchantillonnages a été remplacé à partir de la
version 4.8 par le filtre vnsGridResampleImageFilter, beaucoup plus
performant en temps d’exécution, pour des résultats numériques
identiques.

Correction of Atmospheric Absorption
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Quelques modifications sont apportées à l’algorithme de correction
atmosphérique par rapport à celui utilisé dans la chaîne VENµS L2.

Tout d’abord, il est important de noter, même si cela n’a pas d’impact
au niveau de l’algorithme lui-même, que les coefficients SMAC sont
dépendants du satellite S2-A, S2-B, L5, L7. En effet, les coefficients
SMAC sont lus dans des GIPPs. La chaîne utilise le fichier GIPP associé
au produit traité.

D’autre part, le contenu en vapeur d’eau n’est plus fonction du rapport
entre les réflectances à 910 et 865 nm (cas Venµs) mais entre les bandes
940 nm et 865 nm (pour Sentinel-2). Il convient d’utiliser un
paramétrage générique adapté à tous les capteurs.

La routine smac_inverse (fournie par le CNES) est maintenant dépendante
des angles de visée. Cette routine, ayant été ré implémentée par CS dans
la chaîne VENµS L2, est mise à jour.

La principale évolution de cet algorithme est liée à la prise en compte
des différentes résolutions d’un produit Sentinel-2. Dans la chaîne
VENµS, ce filtre est appelé deux fois, une fois à la résolution réduite
L2 et une fois à la pleine résolution. L’image du contenu en vapeur
d’eau est générée, à chaque résolution L2, à partir d’une LUT en
fonction du rapport des réflectances à 910 et 865 nm. Dans un produit
Sentinel-2, ces bandes sont à des résolutions L2 différentes et donc
contenues dans des vecteurs images différentes. Ce filtre est donc
remanié. Il génère l’image du contenu en vapeur d’eau à la résolution
réduite L2 et la ré échantillonne aux différentes résolutions L2 avant
d’appliquer les corrections gazeuses à une ou plusieurs images de
réflectance (10m et 20m pour Sentinel-2).

A partir de MAJA 1.0, l’estimation de la vapeur d’eau est post-processée
afin de limiter les erreurs d’estimation sur les zones sombres. Elle est
notamment « bouchée » à l’aide d’un algorithme de « gap filling » et
completée d’un masque indiquant si la teneur en vapeur d’eau à été
estimée ou bouchée pour chaque pixel.Cirrus Flag

Aucune modification n’est apportée par rapport à la chaîne VENµS L2.
Cependant, il est important de noter que cet algorithme est désormais
spécifique à VENµS. En effet, le plan d’altitude des nuages n’est pas
disponible dans les produits L1 Formosat et Landsat et une autre méthode
de détection des cirrus est définie pour Sentinel-2.

Pour uniformiser la logique choisie pour les algorithmes de détection
des cirrus et de neige, qui sont spécifiques à chaque capteur, un
paramètre est ajouté au fichier de configuration de manière à activer ou
non cet algorithme en fonction du capteur traité.

Cirrus Masking
^^^^^^^^^^^^^^

   Répond à: ST-169-CirrusMaskS2-0010 dans : DA02 Conformité : C

   Répond à: ST-169-CirrusFlagS2-0020 dans : DA02 Conformité : C

   Répond à: ST-169-CirrusFlagS2-0030 dans : DA02 Conformité : C

Les produits Sentinel-2 possèdent une bande spectrale à 1,38 µm qui a
une très forte absorption à la vapeur d’eau. De ce fait, un simple
seuillage sur la valeur de la réflectance TOA de cette bande permet de
détecter un pixel nuageux de type cirrus. Cependant, la projection des
ombres des nuages restant délicate, l’indicateur global « cirrus_flag »
est conservé.

Cet indicateur est levé lorsque le pourcentage de pixels nuageux de type
cirrus est supérieur à un seuil.

Cette classe est spécifique aux produits Sentinel-2 et génère en sortie
un masque des pixels de type cirrus. Un paramètre permettant d’activer
ou pas cet algorithme est donc ajouté dans le fichier de configuration.

Par ailleurs, ce masque étant réutilisé dans les algorithmes Snow
Masking et Cloud Masking, une image nulle est générée dans L2Processor
pour les autres capteurs de manière à conserver le code le plus
générique possible. En effet, il est préférable, dans ce cas, de
produire une image constante plutôt que de gérer des entrées images
conditionnelles. L’utilisation d’entrées optionnelles modifie le cœur
des méthodes pour l’adapter aux entrées fournies et peut entraîner des
duplications impropres de code en fonction du capteur.

Dans la version 4.0, les modifications apportées à cet algorithme
concernent tous les capteurs. Le seuil sur la réflectance 1.38µm au-delà
duquel un pixel est déclaré nuageux n'est plus un simple paramètre GIPP
mais est calculé à partir d'une équation linéaire en fonction de
l'altitude (seuil_reflectance = gain \* altitude +offset).

Le MNT (à la résolution réduite des produits L2) est donc ajouté en
entrée de l'algorithme.

Deux nouveaux paramètres GIP_L2COMM sont créés pour remplacer le
paramètre Cirrus_Refl_Threshold : Cirrus_Mask_Threshold_Offset et
Cirrus_Mask_Threshold_Gain.

Snow Masking
^^^^^^^^^^^^

L’algorithme de détection de la neige est mis en place dans la chaîne
MAJA pour les capteurs Sentinel-2, Landsat et Venus.
Les capteurs Sentinel-2 et Landsat disposant d'une bande spectrale SWIR (Short-Wave
Infra-Red) ils peuvent l'utiliser afin de générer ce masque. L'algorithme concernant Venus est plus simple et n'utilise pas
de bande SWIR car non présente sur le capteur mais utilise une bande proche afin d'approximer le même comportement.
En effet bien que non SWIR la bande rouge absorbe partiellement les longueur d'ondes SWIR et permet donc la détection de neige.


La détection de la neige sur les images Sentinel-2 repose sur un
seuillage de l’indice NDSI définit par :

.. image:: Art/NDSI.png
   :width: 2.5in
   :height: 1.5in
    

La réflectance de la bande SWIR ( bande rouge pour Venus) permet de distinguer nettement la neige,
qui a une réflectance faible dans cette bande, des nuages qui ont eux
une réflectance élevée. De ce fait, les pixels neigeux ont un indice
NDSI élevé tandis que les nuages ont un indice faible tendant vers zéro.
Un Seuillage sur cet indice permet donc de séparer les pixels neigeux
des pixels nuageux.

L’eau pouvant causer des fausses détections, un seuillage supplémentaire
dans le rouge permet de palier ces fausses détections.

Un filtrage est également effectué sur les bords de l’image.

Enfin, une opération de morphologie mathématique de type fermeture
(dilatation + érosion) est appliquée au masque de neige.

Le masque de neige est utilisé dans de nombreux algorithmes (Cloud
Masking, Rain Detection, AOT Estimation, Composite Image). Le câblage de
cet algorithme dans L2Processor est réalisé via un paramètre présent
dans le fichier de configuration.

Comme décrit ci-dessus, le calcul du NDSI (Normalized Difference Snow
Index) et les tests permettant de détectés un pixel neigeux étaient
réalisés sur les valeurs de réflectance TOA corrigées de l'absorption
atmosphérique et de la diffusion sur le rayleigh. Dans cette nouvelle
version, ces valeurs de réflectance sont préalablement corrigées des
effets de pente au premier ordre :

   refl_corr = refl_surf_ray \* cos thetaS / cos ThetaI

   thetaS : angle zenithal solaire

   thetaI : l’angle d’incidence solaire est l’angle entre la normale à
   la pente et la direction solaire

Le calcul de l'angle d'incidence solaire est réalisé à la résolution
réduite des produits L2 (L2 coarse resolution). Ce calcul est déjà
présent dans l’algorithme de correction des pentes et pourra être
factorisé.

La détection de la neige sur les zones d’eau n’est plus réalisée excepté
en mode init et uniquement si le paramètre Use_Water_mask est fixé à
vrai.

Cloud Masking
^^^^^^^^^^^^^

La méthode de détection des nuages et de leurs ombres est similaire à
celle de la chaîne VENµS L2. Seules quelques évolutions sont à noter :

-  La prise en compte du masque de neige lors du seuillage sur la
   réflectance dans le bleu,

-  La prise en compte du masque de neige dans la méthode de seuillage
   sur la variation de réflectance,

-  La prise en compte du masque de cirrus lors de la génération du
   masque de nuages issus des critères stéréoscopiques et
   radiométriques,

-  Extraire le sous échantillonnage de l’image d’altitude des nuages
   spécifique à Venµs et le remonter dans L2Processor.

Pour les capteurs Sentinel-2, Formosat et Landsat, une image nulle du
plan d’altitude des nuages est créée à la résolution réduite L2 au
niveau du DataManager lors de la lecture des produits de niveau 1.
L’appel au sous échantillonnage de ce masque consiste, dans ce cas, à
une simple recopie de l’image d’entrée. Le masque associé, utilisé dans
Cloud Masking, est créé sans distinction pour tous les capteurs.

-  La labellisation des nuages lors de l’affinage de l’altitude pour les
   images Sentinel-2 et Landsat est la même que pour Formosat, c’est à
   dire un label unique pour tous les nuages de l’image,

-  L’altitude des nuages pour les produits Sentinel-2, Formosat et
   Landsat est constante sur toute l’image et fixée à une valeur
   d’altitude par défaut,

-  Dans l’algorithme de projection des ombres, l’utilisation de la
   grille VIEB5 d’un produit Venµs n’a plus de sens pour les capteurs
   Sentinel-2 et Landsat. La bande spectrale de référence pour la
   direction de visée est spécifique à chaque capteur et paramétrable.

Remarque : la manipulation de ces grilles dans les algorithmes est
réalisée en coordonnées « physiques » et non pas en coordonnées
« image ». Si cette conversion est réalisée lors de la lecture d’un
produit de niveau 1, elle doit être supprimée au niveau des algorithmes.

Les principales évolutions de la version 4.0 de MACCS sont réalisées
dans l’algorithme de détection des nuages.

A partir de MAJA 1.0, la détection initiale mono-temporelle des nuage
est effectuée à l’aide de la méthode du DLR impactant les seuils
appliqués aux différentes bandes afin de décider si un pixel est nuageux
ou non.

Reflectance Threshold
'''''''''''''''''''''

Le seuillage sur la réflectance dans le bleu et les pixels saturés est
complété par un seuillage sur la réflectance dans le proche infrarouge
et sur le masque d’eau à la date D-1 (après érosion).

La détection de nuages au-dessus de l’eau est améliorée par l’ajout d’un
seuillage supplémentaire sur la réflectance. Ce seuil dépend de la
présence de glitter ou pas sur l’image.

Deux paramètres GIP_L2COMM sont ajoutés : Blue_reflectance_threshold et
PIR\_ reflectance_threshold.

Reflectance Variation Threshold
'''''''''''''''''''''''''''''''

Une légère modification est apportée sur le calcul du seuil sur la
variation de réflectance dans le bleu. Ce seuil est désormais affecté à
la valeur minimum entre le seuil calculé en fonction de la durée écoulée
entre les deux produits et un seuil maximum fixé (paramètre d’entrée)
qui est ajouté au GIP_L2COMM.

Le GIPP Blue_Var_Threshold est remplacé par les paramètres
Min_threshold_var_blue et Max_threshold_var_blue.

Snow mask correction
''''''''''''''''''''

Un nouvel algorithme de correction du masque de neige est ajouté au
module Cloud Masking.

En effet, de fausses détections de neige pouvant apparaître au cœur de
nuages épais, il est préférable de supprimer du masque de neige ces
zones neigeuses situées à proximité de nuages ou de no_data et de les
ajouter au masque de nuages issu du seuillage sur la réflectance.

Pour déterminer ces zones, l’algorithme suivant est mis en place :

-  le masque de neige est labellisé puis dilaté.

-  pour chaque label, l’algorithme teste si tous les pixels déclarés
   neigeux uniquement dans le masque de neige dilaté (et pas dans le
   masque de neige initial) sont également nuageux ou dans les bords de
   l'image (no_data)

-  Si c’est le cas, tous les pixels de ce label (non dilaté) sont
   désactivés dans le masque de neige et ajoutés au masque de nuages
   issu du seuillage sur la réflectance.

Le masque de nuages utilisé inclut le masque de nuages déterminé par les
seuillage sur la réflectance et le masque d’eau (Reflactance Threshold)
et par la détection des variations de réflectance entre la date D et la
date D-1.

Le module Cloud Masking renvoie le masque de neige et le masque de
nuages corrigé.

L’interface de ce nouvel algorithme est décrite dans le tableau
suivant :

======================= ========================================================================================
\                       Description
Entrées                
Cloud_mask_sub.refl     Masque des nuages obtenu par seuillage sur la réflectance dans le bleu et le PIR
Cloud_mask_sub.refl_var Masque des nuages obtenu par seuillage sur la variation de réflectance
Edge_mask_sub           Masque des bords de l’image et des no_data
Snow_mask_sub           Masque de neige
Sorties                
Cloud_mask_sub.refl     Masque des nuages obtenu par seuillage sur la réflectance dans le bleu et le PIR corrigé
Snow_mask_sub           Masque de neige corrigé
======================= ========================================================================================

Shadow mask determination
'''''''''''''''''''''''''

Le masque des ombres est généré à partir d'équations géométriques qui
utilisent le masque des nuages, l'altitude des nuages, les angles
solaires et de visée et le MNT pour projeter les nuages au sol et
déterminer la position des ombres. La position des ombres au sol est
donc liée à l'altitude des nuages. Pour améliorer la localisation de ces
ombres, un algorithme d'affinement de l'altitude des nuages fondé sur
des critères radiométriques a été mis en place dans la chaîne L2. Il
procède par itération sur l'altitude des nuages pour rechercher
l'altitude qui correspond à la zone d'assombrissement maximum (due aux
ombres) entre l'image à la date D et celle à la date D-1.

Seuls les produits de niveau 1 Venµs disposent d'un plan contenant
l'altitude des nuages déterminée par stéréoscopie.

A partir de la version **5.0** de MACCS (DM **982**), pour les capteurs
différents de Venµs, l'approche est différente. L'absence de donnée
d'altitude des nuages nécessite de s'adapter en déterminant une zone
dans laquelle des ombres peuvent se trouver. Pour cela, au lieu
d'estimer l'altitude des nuages, leur ombre projetée va être calculée
pour chaque altitude dans une certaine gamme. Au sein de ces zones, il
sera alors appliqué différents tests statistiques relatifs à la
radiométrie de ces zones. La détermination des ombres n'est plus
itérative et ne permet alors plus d'estimer l’altitude des nuages. Ces
opérations sont assurées par la classe
vns::ShadowMaskDeterminationWithoutKnownAltitude. Ainsi, afin de
paramétrer ces nouveaux traitements, de nouveaux paramètres ont été
ajoutés au fichier GIPP_L2COMM.

Water Masking
^^^^^^^^^^^^^

Deux tests sont ajoutés dans la version MACCS 4.0 :

-  Un test sur la sélection des pixels valides qui exclut les pixels
   pour lesquels la pente est supérieure à un seuil

-  Un test supplémentaire sur la réflectance de surface corrigée du
   rayleigh dans le rouge qui vient s’ajouter au test sur le NDVI.

L’interface de cet algorithme a donc été légèrement modifiée. L’image
des pentes fournie avec le MNT à la résolution réduite des produits L2
est utlisée et deux nouveaux paramètres GIP_L2COMM sont créés :
Water_red_refl_threshold, Water_slope_threshold.

Rain detection
^^^^^^^^^^^^^^

L’algorithme de détection de la pluie prend en compte le masque de
neige. Un masque de neige constant et nul est donné en entrée du filtre
pour les capteurs Venµs et Formosat.

De plus, il faut remplacer le paramètre NIR_band par Water_Band dans les
GIPP.

Estimation of Aerosol Optical Properties
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

D’importantes évolutions sont apportées à cet algorithme. Il existe
maintenant trois méthodes pour estimer l’épaisseur optique des
aérosols :

-  La méthode multi temporelle repose sur la stabilité de la réflectance
   de surface pour des images du même site acquises à des dates
   différentes. Il s’agit de la méthode actuellement implémentée dans la
   chaîne VENµS L2.

-  La méthode multi spectrale utilise des relations empiriques entre les
   bandes spectrales rouge et proche infrarouge.

-  La méthode spectro temporelle combine les deux méthodes précédentes.

Trois nouveaux filtres vnsMultiTemporalComputeAOTImageFilter,
vnsMultiSpectralComputeAOTImageFilter et
vnsSpectroTemporalComputeAOTImageFilter sont créées. Le filtre présent
dans la chaîne VENµS est repris pour les méthodes multi temporelle et
spectro-temporelle.

La sélection des pixels pour l’estimation locale de l’épaisseur optique
des aérosols est déclinée en deux versions pour les méthodes multi
temporelle et multi spectrale. Le masque des pixels neigeux est pris en
compte dans les deux cas. Le critère de sélection sur la variation
relative de réflectance entre la date D et la date D-1 est remplacé par
un seuillage sur la réflectance dans le rouge et sur le NDVI dans la
méthode multi spectrale.

La fonction de coût utilisée pour l’estimation de l’épaisseur optique
des aérosols est maintenant dépendante de la méthode utilisée. La
fonction de coût de la méthode multi temporelle est la même que dans la
chaîne VENµS.

Une nouvelle fonction de coût est ajoutée pour la méthode multi
spectrale. La méthode spectro temporelle utilise les deux.

.. image:: Art/image12.png
   :width: 7.57292in
   :height: 5.10417in

Figure 11 : diagramme de classes pour les fonctions de coût

Un nouveau template (de type enum) est ajouté à la classe
vnsAOTEstimation pour définir la méthode à employer. De plus, la partie
algorithmique (ré échantillonnage et redécoupage) concernant le passage
de la résolution réduite L2 à la pleine résolution pour les images de
sortie (AOT et TAO) va être appelée autant de fois qu’il y a de
résolutions L2 dans le produit traité.

Seule une évolution a été apportée à cet algorithme dans la version
MACCS 4.0 et concerne la sélection des pixels pour la méthode multi
temporelle.

Un nouveau seuillage sur la réflectance dans le bleu est ajouté lors de
la sélection des pixels utilisés pour le calcul de l’épaisseur optique
des aérosols via la méthode multi temporelle. L’objectif est d’éliminer
les valeurs de réflectance TOA pour lesquelles l’épaisseur optique des
aérosols ne peut être calculée. En effet dans une gamme de réflectance
donnée, l’impact de l’épaisseur optique aérosols n’est pas quantifiable
sur la mesure de la réflectance.

Pour effectuer ce seuillage, deux seuils encadrant les valeurs de
réflectance rejetées sont calculés en recherchant dans une gamme de
réflectance (pour des conditions d’acquisition données) les valeurs de
réflectance TOA pour lesquelles la différence entre les réflectances de
surface obtenues pour deux valeurs extrêmes d’AOT est faible (inférieure
à un seuil). Le passage de réflectance TOA aux valeurs de réflectance de
surface est réalisé via les mini LUTs calculées pour les conditions
d’acquisition (valeurs des angles au centre de l’image) associées au
produit traité.

===================================== ===================================================================================================
**Paramètres**                        **Description**
Min_Difference_Thresholds_Calculation Seuil sur la différence entre des réflectances de surface obtenues pour deux valeurs extrêmes d’AOT
First_AOT                             Valeur minimum d’AOT pour calculer la première gamme de réflectance
Second_AOT                            Valeur maximum d’AOT pour calculer la première gamme de réflectance
TOA_Reflectance_Min                   Valeur minimum de la réflectance TOA pour déterminer la gamme de réflectance
TOA_Reflectance_Max                   Valeur maximum de la réflectance TOA pour déterminer la gamme de réflectance
TOA_Reflectance_Step                  Pas entre deux valeurs de réflectance dans la gamme.
===================================== ===================================================================================================

Cirrus Correction
^^^^^^^^^^^^^^^^^

L’algorithme de correction des zones de cirrus a été introduit dans MAJA
V2.0. Celui-ci permet de corriger les zones de l’image impactées par les
nuages cirrus afin d’améliorer le rendu visuel. Afin de traiter au mieux
les images, la distance de Maurer est utilisée afin de pondérer la
correction permettant ainsi d’appliquer une correction plus importante
au centre des nuages par rapport à leurs bords. Seuls les capteurs
disposant d’une bande cirrus peuvent activer la correction cirrus.

Scaterring Correction
^^^^^^^^^^^^^^^^^^^^^

Dans la chaîne VENµS L2, la variation des angles solaires et de visée
dans le champ est négligée lors de la correction de la dispersion
atmosphérique à pleine résolution. De ce fait, la LUT ne dépend plus que
de la réflectance de surface, de l’épaisseur optique des aérosols et de
l’altitude. L’algorithme travaille avec une LUT dite « réduite » pour
des valeurs d’angles constantes.

Pour Sentinel-2, la variation des angles dans le champ est trop
importante pour être négligée. Les angles sont alors considérés
constants par zone dans l’image. Une image contenant la valeur moyenne
des angles de visée par zone est créée au niveau du DataManager. La
modification consiste donc à prendre en compte cette variation d’angle
sur l’image en utilisant une LUT « réduite » par zone. Ces LUTs réduites
sont générées dans la classe L2Processor et un vecteur de LUTs (par
zone) est passé en entrée de l’algorithme.

De plus, pour gérer les différentes résolutions L2 d’un produit
Sentinel-2, deux vecteurs de LUTs réduites sont générés dans
L2Processor, l’un contenant les bandes spectrales à 10m et l’autre les
bandes spectrales à 20m.

Comme pour l’algorithme de correction atmosphérique, une classe est
chargée de l’enchaînement des appels aux classes algorithmiques pures.
Elle permet de gérer les traitements aux différentes résolutions en
fonction des capteurs. Cette classe attend une entrée par type d’image
(réflectance, masque, MNT) et par résolution (résolution réduite,
résolutions L2).

Composite Image
^^^^^^^^^^^^^^^

Il faut maintenant prendre en compte le masque de neige, pour la
génération de l’image composite.

Dans la version MACCS 4.0, la recherche des zones à forte épaisseur
optique des aérosols à proximité de nuages est supprimée. Toutes les
zones à forte AOT sont désormais prises en compte.

Corrections for environnement effects
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

L’algorithme de correction des effets de l’environnement n’est pas
modifié par rapport à la chaîne VENµS L2 si ce n’est qu’il faut
maintenant gérer les deux résolutions L2 (10 m et 20 m) d’un produit
Sentinel-2.

Pour cela, les traitements à la résolution réduite L2 et ceux à la
pleine résolution sont séparés dans des filtres distincts :

-  le filtre existant génère les images de réflectance d’environnement,
   de transmission directe et diffuse et d’albedo à la résolution
   réduite des produits L2,

-  un deuxième ré échantillonne les images à la pleine résolution L2,
   les retaille, supprime les effets de flou sur les bords de l’image de
   réflectance de l’environnement, génère l’image de transmission totale
   et applique le terme correctif à la réflectance de surface corrigée
   de la diffusion.

Ce découpage permet d’appeler les traitements à la pleine résolution
autant de fois qu’il y a de résolutions différentes. Les images à la
résolution réduite L2 étant générées pour toutes les bandes spectrales,
les tables de correspondance entre les bandes spectrales à la résolution
« L2 coarse » et celles à la résolution L2 sont utilisées dans le
Functor générant l’image de réflectance de surface finale.

Dans le cas Sentinel-2, deux images de réflectance de surface et deux
images de réflectance de l’environnement (10 m et 20 m) sont générées en
sortie et réutilisées pour la correction des pentes.

Slope Correction
^^^^^^^^^^^^^^^^

Dans l’algorithme de correction des pentes, la problématique multi
résolution liée à Sentinel-2 est traitée de la même façon que dans
l’algorithme de correction de l’environnement. Les traitements à la
résolution réduite des produits L2 et à la pleine résolution sont donc
séparés. Les tables de correspondance des bandes spectrales entre les
différentes résolutions sont également utilisées.

Description des évolutions des « outils de contrôle et de validation »
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Évolutions générales
^^^^^^^^^^^^^^^^^^^^

De manière générale, les traitements actuellement disponibles dans
l’outil implémenté dans les chaînes L2 VENµS évoluent afin de prendre en
compte des produits des niveaux 1, 2 pour les capteurs Sentinel-2 et
Landsat.

Le module de lecture de la partie publique d‘un produit L2 est modifié
pour traiter les données à plusieurs résolutions.

Les évolutions spécifiques sont décrites ci-dessous.

Évolutions spécifiques
^^^^^^^^^^^^^^^^^^^^^^

Seules quelques évolutions spécifiques sont apportées à cet outil :

-  Pour les cas Sentinel-2 et Landsat, les contours du masque de neige
   seront soulignés dans le Quicklook généré (dans Exigence 100),

-  Un contrôle est réalisé pour vérifier que les 3 canaux sélectionnés
   par l’utilisateur correspondent bien à des bandes de même résolution
   (dans Exigence 120),

-  L’extraction de pixels sur les plans de résolutions 20 et 60 m d’un
   produit Sentinel de niveau 1 ou 2 est ajoutée : à partir des
   coordonnées du pixel central défini dans la résolution 10m, il faut
   déterminer le pixel équivalent (superposable) pour les plans de
   résolutions 20 et 60 m (prendre en compte le facteur de résolution
   sur la taille de la fenêtre à utiliser) (dans Exigence 210).
