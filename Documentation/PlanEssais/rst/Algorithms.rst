LES TESTS DE VALIDATION ALGORITHMIQUE
=====================================

Objectifs
----------

L’objectif des tests de validation algorithmique est de :
- Réaliser une validation numérique des traitements algorithmiques de "haut niveau" codés sous forme d'applications,
- Vérifier le comportement de ces traitements dans certains cas dégradés ou aux limites qui ne seront pas repris en validation,
avec le responsable algorithmique CNES.
Chaque module algorithmique suivant fait l’objet d’un ou plusieurs tests permettant de couvrir l’ensemble des fonctionnalités de chaque composant :


Stratégie et méthode des tests numériques
------------------------------------------

Les données Formosat sont principalement utilisées pour la validation numérique (à l’exception du test de détection des cirrus qui nécessite le plan d’altitude stéréoscopique des nuages présent dans les produits Venμs). Suite au développment de la chaîne MAJA, des tests supplémentaires avec des produits Landsat et Sentinel2 ont été ajoutés pour prendre en compte les dernières évolutions de la chaîne comme la création de nouveaux algorithmes (détection de la neige, des cirrus, estimation des aérosols, etc.) ou la gestion de produits à multiples résolutions. L’essentiel de la validation numérique est menée en comparant chaque image du produit de niveau 2 générée par les chaînes scientifiques Venμs à celle produite précedement par le prototype développé au CESBIO. L’outil de comparaison de l’OTB effectue une différence des images pixel à pixel. Pour chaque bande, il génère l’image des différences et calcule la somme des différences. Le test est validé si ces valeurs sont nulles à un epsilon près, qui est un paramètre d’entrée du test. L’outil calcule également le nombre de pixels sur lesquels sont mesurés ces différences. Compte tenu de la complexité des algorithmes, la valeur nulle semble difficile à atteindre. La valeur du epsilon sera fixée en concer- tation avec le CNES et en tenant compte des résultats obtenus pour les tests unitaires. Il convient également de déterminer si la différence mesurée est acceptable par rapport à la donnée testée (ordre de grandeur des écarts par rapport aux valeurs issues du prototype) Comme pour les tests unitaires, une analyse importante est l’analyse visuelle des images et des images de différence. Elle permet de détecter une structuration non aléatoire des différences sur l’image mais aussi de localiser les différences (proche d’un nuage, d’une zone d’eau, etc.). A cette analyse des images de différence, s’ajoutent d’autres moyens d’analyse. Une stratégie de validation différente est adoptée en fonction du types de données à comparer. Plusieurs types de données sont comparés : les images contenant des grandeurs physiques, les images de dates et les masques. La validation des produits composites passe dans un premier temps par la comparaison des plans « date » pour visualiser les zones qui diffèrent. Les différences mesurées lors de la comparaison des images de date vont se retrouver sur les images composite de réflectance. La validation des masques présents dans le produits L2 se traduit par une analyse statistique  des différences entre le masque généré et le masque de référence si besoin est. En effet, la probabilité d’avoir des masques parfaitement identiques entre les masques issus des chaînes Venμs et les masques de référence est faible. Pour comparer ces masques, la simple différence réalisée pixel à pixel peut être complétée par la mise en place d’indicateurs statistiques. Ces derniers permettent de quantifier la performance des détections. Les performances du masque obtenu sont alors exprimées par une matrice de confusion par rapport au masque de référence. A ces indicateurs s’ajoutent le calcul des proportions de bonnes et de fausses détections c’est-à-dire le nombre de pixels correctement identifiés par rapport au nombre de pixels de l’image. Une fois l’algorithme validé, les résultats du test sont référencés et dès lors, systématiquement contrôlés à chaque nouvelle exécution du test. Cette démarche assure la non-régression des algo- rithmes développés tout au long de la phase de codage. Les résultats des tests joués sont systématiquement envoyés au serveur CDash pour être mis en forme et offrir au développeur un environnement de contrôle ergonomique et le suivi de l’évolution des résultats au cours du temps.


Formalisation des tests de validation algorithmique
---------------------------------------------------
La nomenclature adoptée pour l’identification des tests algorithmiques est la suivante :
apTv<traitement><nature>
Ex : apTvAOTEstimation_SpectroTemporalNominal


LISTE DES TESTS DE VALIDATION ALGORITHMIQUE
-------------------------------------------

Ce chapitre présente la liste des tests mis en place pour réaliser la validation numérique des modules
algorithmiques.


======= ======================================================= =============================================================================
1       apTvSmoothingBis                                        Application d'une gaussienne sur l'image
2       apTvResampling                                          Réechantillonnage d'une image sur un DTM
3       apTvStats                                               Calculs de statistiques sur une image
4       apTvMultiplyByScalar                                    Multiplication d'une image par un scalaire
5       apTvBinaryConcatenate                                   Concatenation de plusieurs bandes binaires(0-1)
6       apTvBinaryToVector                                      Déconcaténation en plan de bits
7       apTvConcatenatePerZone                                  Concaténation d'image selon un masque de zones
8       apTvDispatchZonesToVectorNoWeight                       Répartition de masques de zone 
9       apTvDispatchZonesToVectorWeight                         Répartition de masques de zone (bandes non ordonées)
10      apTvIncidenceAngle                                      Calcul des angles d'incidences
11      apTvOzoneExtractor                                      Extraction du volume d'ozone
12      apTvAngleListToImage					Création d'une image à partir d'une liste d'angles
13      apTvSolarAngle                                          Calcul des angles solaires
14      apTvImageListWriter                                     Ecriture simultanée d'une liste d'images
15      apTvRoundImage                                          Arrondis d'une image
16      apTvRoundVectorImage                                    Arrondis d'une image à plusieurs bandes
17      apTvRescaleIntensity                                    Modification de la radiometrie
18      apTvOneBandEqualThreshold                               Création d'un masque par test de valeur
19      apTvDarkestPixelAOTNominal                              Calcul du DarkestPixel pour le calcul de l'AOT
20      apTvGapFilling                                          Algorithm de gap filling
21      apTvAOTEstimation_SpectroTemporalNominal                Calcul de l'AOT en SpectroTemporel
22      apTvAOTMean_SpectroTemporalNominal                      Calcul de l'AOT moyen en SpectroTemporel
23      apTvAOTEstimation_MultiTemporalNominal                  Calcul de l'AOT en MultiTemporel
24      apTvAOTEstimationMultiSpectral                          Calcul de l'AOT en MultiSpectral
25      apTvAOTEstimationSpectroTemporalCAMS                    Calcul de l'AOT en SpectroTemporel avec CAMS
26      apTvPressureFilter                                      Calcul de la pression
27      apTvTOACorrection                                       Correction TOA de l'image
28      apTvWaterAmountGeneratorFilter                          Calcul d'un volume d'eau
29      apTvGammaCompute                                        Calcul d'un gamma pour la correction cirrus
30      apTvCirrusCorrectionApply                               Correction Cirrus d'une image
31      apTvCirrusFlag                                          Calcul du flag Cirrus
32      apTvCirrusMask                                          Calcul du masque de Cirrus
33      apTvComposite                                           Calcul des images composites
34      apTvDTMProcessing                                       Algorithme DTMProcessing 
35      apTvComputeRhoEnv                                       Calcul du RhoEnv
36      apTvEnvCorrection                                       Correction des effets environnementaux
37      apTvInterpolateForEnvCorr                               Interpolation spécifique pour les effets environnementaux
38      apTvCloudReflectance                                    Calcul du masque de nuages reflectance
39      apTvCloudReflectanceVariation                           Calcul du masque de variation des nuages reflectance
40      apTvCloudExtension                                      Calcul du masque de nuages "extension"
41      apTvCloudShadVar                                        Calcul du masque de variation des ombres des nuages
42      apTvCloudSnowCorrection                                 Correction des effets de neiges
43      apTvCloudMaskUndersampling                              Reechantillonnage spécifique aux images CLA
44      apTvCloudShadowWithAltitudeFilterInit                   Calcul du masque des ombres des nuages connaissant leurs altitudes
45      apTvCloudShadowWithoutAltitudeFilterNominal             Calcul du masque des ombres des nuages sans leurs altitudes
46      apTvRayleighCorrection                                  Correction Rayleigh
47      apTvCamsCompute                                         Extraction des données CAMS (ancien format)
48      apTvNewCamsCompute                                      Extraction des données CAMS (nouveau format)
49      apTvNewCamsMixedCompute                                 Extraction des données CAMS (ancien et nouveau format)
50      apTvDirectionalCorrection                               Calcul des coefficients de correction directionelle
51      apTvReduceLut                                           Extraction de MiniLut
52      apTvReduceLutDirCorr                                    Extraction de minilut avec correction directionnelle
53      apTvExtractLutChannel                                   Extraction d'une bande d'une LUT
54      apTvScatteringCorrection                                Correction du scattering
55      apTvSlopeCorrection                                     Correction des effets de pentes
56      apTvComputeTransmission                                 Calcul de la transmission athmosphérique
57      apTvSnowMask                                            Calcul du masque de neige
58      apTvSnowMaskVenus                                       Calcul du masque de neige (cas VENUS)
59      apTvWaterMask                                           Calcul du masque d'eau
60      apTvWaterVaporPostPro                                   Post processing du masque d'eau
61      apTvBandMath                                            Outil générique d'algorithmie sur image (OTB)
62      apTvExtractROI                                          Extraction de zone/bande (OTB)
63      apTvManageNoData                                        Application/Extraction de masques de NoData
64      apTvBinaryMorphologicalOperation                        Opérateurs Morphologiques
65      apTvConvertCartoToGeoPoint                              Conversion de coordonnées cartographiques
66      apTvConcatenateImages                                   Concaténation d'image en une seule
======= ======================================================= =============================================================================

DESCRIPTION DÉTAILLÉE DES TESTS DE VALIDATION ALGORITHMIQUE
------------------------------------------------------------

.. toctree::
	:maxdepth: 5
	:caption: apTvAngleListToImage

	Algos/apTvAngleListToImage

.. toctree::
	:maxdepth: 5
	:caption: apTvAOTEstimationMultiSpectral

	Algos/apTvAOTEstimationMultiSpectral

.. toctree::
	:maxdepth: 5
	:caption: apTvAOTEstimation_MultiTemporalNominal

	Algos/apTvAOTEstimation_MultiTemporalNominal

.. toctree::
	:maxdepth: 5
	:caption: apTvAOTEstimationSpectroTemporalCAMS

	Algos/apTvAOTEstimationSpectroTemporalCAMS

.. toctree::
	:maxdepth: 5
	:caption: apTvAOTEstimation_SpectroTemporalNominal 
	
	Algos/apTvAOTEstimation_SpectroTemporalNominal 

.. toctree::
	:maxdepth: 5
	:caption: apTvAOTMean_SpectroTemporalNominal
	
	Algos/apTvAOTMean_SpectroTemporalNominal

.. toctree::
	:maxdepth: 5
	:caption: apTvBinaryConcatenate
	
	Algos/apTvBinaryConcatenate

.. toctree::
	:maxdepth: 5
	:caption: apTvBinaryToVector

	Algos/apTvBinaryToVector

.. toctree::
	:maxdepth: 5
	:caption: apTvCamsCompute

	Algos/apTvCamsCompute

.. toctree::
	:maxdepth: 5
	:caption: apTvCirrusCorrectionApply

	Algos/apTvCirrusCorrectionApply

.. toctree::
	:maxdepth: 5
	:caption: apTvCirrusFlag

	Algos/apTvCirrusFlag

.. toctree::
	:maxdepth: 5
	:caption: apTvCirrusMask

	Algos/apTvCirrusMask

.. toctree::
	:maxdepth: 5
	:caption: apTvCloudExtension

	Algos/apTvCloudExtension

.. toctree::
	:maxdepth: 5
	:caption: apTvCloudMaskUndersampling

	Algos/apTvCloudMaskUndersampling

.. toctree::
	:maxdepth: 5
	:caption: apTvCloudReflectance

	Algos/apTvCloudReflectance

.. toctree::
	:maxdepth: 5
	:caption: apTvCloudReflectanceVariation

	Algos/apTvCloudReflectanceVariation

.. toctree::
	:maxdepth: 5
	:caption: apTvCloudShadowWithAltitudeFilterInit

	Algos/apTvCloudShadowWithAltitudeFilterInit

.. toctree::
	:maxdepth: 5
	:caption: apTvCloudShadowWithoutAltitudeFilterInit

	Algos/apTvCloudShadowWithoutAltitudeFilterInit

.. toctree::
	:maxdepth: 5
	:caption: apTvCloudShadVar

	Algos/apTvCloudShadVar

.. toctree::
	:maxdepth: 5
	:caption: apTvCloudSnowCorrection

	Algos/apTvCloudSnowCorrection

.. toctree::
	:maxdepth: 5
	:caption: apTvComposite
	
	Algos/apTvComposite

.. toctree::
	:maxdepth: 5
	:caption: apTvComputeRhoEnv

	Algos/apTvComputeRhoEnv

.. toctree::
	:maxdepth: 5
	:caption: apTvComputeTransmission

	Algos/apTvComputeTransmission

.. toctree::
	:maxdepth: 5
	:caption: apTvConcatenatePerZone

	Algos/apTvConcatenatePerZone

.. toctree::
	:maxdepth: 5
	:caption: apTvDarkestPixelAOTNominal

	Algos/apTvDarkestPixelAOTNominal

.. toctree::
	:maxdepth: 5
	:caption: apTvDirectionalCorrection
	
	Algos/apTvDirectionalCorrection

.. toctree::
	:maxdepth: 5
	:caption: apTvDispatchZonesToVectorNoWeight
	
	Algos/apTvDispatchZonesToVectorNoWeight

.. toctree::
	:maxdepth: 5
	:caption: apTvDispatchZonesToVectorWeight

	Algos/apTvDispatchZonesToVectorWeight

.. toctree::
	:maxdepth: 5
	:caption: apTvDTMProcessing

	Algos/apTvDTMProcessing

.. toctree::
	:maxdepth: 5
	:caption: apTvEnvCorrection

	Algos/apTvEnvCorrection

.. toctree::
	:maxdepth: 5
	:caption: apTvExtractLutChannel

	Algos/apTvExtractLutChannel

.. toctree::
	:maxdepth: 5
	:caption: apTvGammaCompute

	Algos/apTvGammaCompute

.. toctree::
	:maxdepth: 5
	:caption: apTvGapFilling

	Algos/apTvGapFilling

.. toctree::
	:maxdepth: 5
	:caption: apTvImageListWriter

	Algos/apTvImageListWriter

.. toctree::
	:maxdepth: 5
	:caption: apTvIncidenceAngle

	Algos/apTvIncidenceAngle

.. toctree::
	:maxdepth: 5
	:caption: apTvInterpolateForEnvCor

	Algos/apTvInterpolateForEnvCor

.. toctree::
	:maxdepth: 5
	:caption: apTvMultiplyByScalar

	Algos/apTvMultiplyByScalar

.. toctree::
	:maxdepth: 5
	:caption: apTvNewCamsCompute

	Algos/apTvNewCamsCompute

.. toctree::
	:maxdepth: 5
	:caption: apTvNewCamsMixedCompute

	Algos/apTvNewCamsMixedCompute

.. toctree::
	:maxdepth: 5
	:caption: apTvOneBandEqualThreshold

	Algos/apTvOneBandEqualThreshold

.. toctree::
	:maxdepth: 5
	:caption: apTvOzoneExtractor

	Algos/apTvOzoneExtractor

.. toctree::
	:maxdepth: 5
	:caption: apTvPressure

	Algos/apTvPressure

.. toctree::
	:maxdepth: 5
	:caption: apTvRayleighCorrection

	Algos/apTvRayleighCorrection

.. toctree::
	:maxdepth: 5
	:caption: apTvReduceLutDirCorr

	Algos/apTvReduceLutDirCorr

.. toctree::
	:maxdepth: 5
	:caption: apTvReduceLut

	Algos/apTvReduceLut

.. toctree::
	:maxdepth: 5
	:caption: apTvResampling

	Algos/apTvResampling

.. toctree::
	:maxdepth: 5
	:caption: apTvRescaleIntensity

	Algos/apTvRescaleIntensity

.. toctree::
	:maxdepth: 5
	:caption: apTvRoundImage

	Algos/apTvRoundImage

.. toctree::
	:maxdepth: 5
	:caption: apTvRoundVectorImage

	Algos/apTvRoundVectorImage

.. toctree::
	:maxdepth: 5
	:caption: apTvScatteringCorrection

	Algos/apTvScatteringCorrection

.. toctree::
	:maxdepth: 5
	:caption: apTvSlopeCorrection

	Algos/apTvSlopeCorrection

.. toctree::
	:maxdepth: 5
	:caption: apTvSmoothingBis

	Algos/apTvSmoothingBis

.. toctree::
	:maxdepth: 5
	:caption: apTvSnowMask

	Algos/apTvSnowMask

.. toctree::
	:maxdepth: 5
	:caption: apTvSnowMaskVenus

	Algos/apTvSnowMaskVenus

.. toctree::
	:maxdepth: 5
	:caption: apTvSolarAngle

	Algos/apTvSolarAngle

.. toctree::
	:maxdepth: 5
	:caption: apTvStats

	Algos/apTvStats

.. toctree::
	:maxdepth: 5
	:caption: apTvTOACorrection

	Algos/apTvTOACorrection

.. toctree::
	:maxdepth: 5
	:caption: apTvWaterAmountGeneratorFilter

	Algos/apTvWaterAmountGeneratorFilter

.. toctree::
	:maxdepth: 5
	:caption: apTvWaterMask

	Algos/apTvWaterMask

.. toctree::
	:maxdepth: 5
	:caption: apTvWaterVaporPostPro

	Algos/apTvWaterVaporPostPro

