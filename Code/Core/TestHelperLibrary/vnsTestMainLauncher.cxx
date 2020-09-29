/*
* Copyright (C) 2020 Centre National d'Etudes Spatiales (CNES)
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
*    http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*
*/
/************************************************************************************************************
 *                                                                                                          *
 *                                ooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo         *
 *                             o                                                                            *
 *                          o                                                                               *
 *                        o                                                                                 *
 *                      o                                                                                   *
 *                     o       ooooooo       ooooooo    o         o      oo                                 *
 *    o              o       o        o     o       o   o         o     o   o                               *
 *      o           o       o          o   o         o  o         o    o      o                             *
 *        o        o       o           o   o         o  o         o    o        o                           *
 *         o      o        o      oooo     o         o  o         o   o           o                         *
 *          o    o          o              o         o  o         o   o           o                         *
 *           o  o            o             o         o  o o      o   o          o                           *
 *            oo              oooooooo    o         o   o  oooooo   o      oooo                             *
 *                                                     o                                                    *
 *                                                     o                                                    *
 *                                                    o                            o                        *
 *                                                    o            o      oooo     o   o      oooo          *
 *                                                   o             o         o    o    o         o          *
 *                                                   o            o       ooo     o   o       ooo           *
 *                                                               o       o       o   o          o           *
 *                                                               ooooo   oooo    o   ooooo  oooo            *
 *                                                                              o                           *
 *                                                                                                          *
 ************************************************************************************************************
 *                                                                                                          *
 * Author: CS Systemes d'Information  (France)                                                              *
 *                                                                                                          *
 ************************************************************************************************************
 * HISTORIQUE                                                                                               *
 *                                                                                                          *
 * VERSION : 2.0.0 : DM : LAIG-DM-MAJA-153436-CS : 23 mars 2017 : Correction sur la valeur calculee du code *
 *                                                           d'erreur de retour (valeur doit etre negative) *
 * VERSION : 1.0.0 : FA : LAIG-FA-MAC-1988-CNES : 14 octobre 2016 : Correction qualite (code mort)          *
 * VERSION : 5-1-0 : FA : LAIG-FA-MAC-144776-CS : 10 mai 2016 : Correction recherche multibaseline          *
 * VERSION : 5-0-0 : FA : LAIG-FA-MAC-143764-CS : 19 avril 2016 : Mise en forme du code (indentation, etc.) *
 * VERSION : 4-0-0 : FA : LAIG-FA-MAC-117040-CS : 24 avril 2014 : Modifications mineures                    *
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id: vnsTestHelperLibraryLauncher.cxx 2656 2011-02-15 10:25:38Z cvallade $
 *                                                                                                          *
 ************************************************************************************************************/

#include "itkWin32Header.h"
#include <map>
#include <string>
#include <iostream>
#include <fstream>

#include "itkMacro.h"
#include "itkMultiThreader.h"
#include "itksys/Process.h"
#include "itksys/SystemTools.hxx"
#include "vnsUtilities.h"
#include "vnsTestHelper.h"
#include "vnsGRIBImageIOFactory.h"
#include "itkObjectFactoryBase.h"
#include "itkImageToImageFilterCommon.h"
#include "itkTimeProbe.h"

int
main(int ac, char* av[])
{
    /*    char
     time_command_param_format[] =
     "/usr/bin/time --format \"\n \
          ==================================================================================================================== \n \
          Temps \n \
          TEMPS_TOTAL       %E     Temps réel écoulé (en [heures:]minutes:secondes).\n \
          TEMPS_REEL_ECOULE %e     (pas avec tcsh.) Temps réel écoulé (en secondes).\n \
          %S     Nombre total de secondes-CPU passé par le processus en mode noyau.\n \
          %U     Nombre total de secondes-CPU passé par le processus en mode utilisateur.\n \
          TEMPS_CPU %P     Pourcentage de CPU obtenu par cette tâche, calculé comme (%U + %S) / %E.\n \
          ==================================================================================================================== \n \
          Mémoire\n \
          MEMOIRE_MAX       %M     Taille maximale du processus en mémoire physique durant son exécution, en kilo-octets.\n \
          MEMOIRE_MOY       %t     (pas avec tcsh) Taille moyenne du processus en mémoire physique durant son exécution, en kilo-octets.\n \
          %K     Mémoire moyenne (donnée + pile + code) utilisée par le processus, en kilo-octets.\n \
          %D     Taille moyenne des données non-partagées du processus, en kilo-octets.\n \
          %p     (pas avec tcsh.) Taille moyenne de la pile non-partagée du processus, en kilo-octets.\n \
          %X     Taille moyenne du code partagé du processus, en kilo-octets. \n \
          %Z     (pas avec tcsh.) Taille de page du système, en octets. Ceci est une constante sur un système, mais varie de système en système. \n \
          %F     Nombre de fautes de pages majeures durant l’exécution. Ce sont les fautes où des pages ont dû être rechargées depuis le disque. \n \
          %R     Nombre de fautes de pages mineures durant l’exécution. Ce sont les fautes où des pages ne sont plus valides, mais n’ont pas encore été réutilisées.  Les  données  y  sont  encore présentes et seules les tables système doivent être mises à jour. \n \
          MEMOIRE_NBSWAP    %W     Nombre de fois où le processus a été envoyé sur la mémoire de swap. \n \
          %c     Nombre de fois où le processus a été préempté involontairement (sa tranche de temps ayant expiré). \n \
          %w     Nombre de fois où le processus a été commuté volontairement, en passant en attente par exemple de la fin d’une entrée-sortie. \n \
          ==================================================================================================================== \n \
          Entrées/Sorties \n \
          IO_NBREADFILE      %I     Nombre de lecture depuis un fichier \n \
          IO_NBWRITEFILE     %O     Nombre d’écriture dans un fichier \n \
          %r     Nombre de messages reçus par le processus sur une socket. \n \
          %s     Nombre de messages envoyés par le processus sur une socket. \n \
          %k     Nombre de signaux délivrés au processus. \n \
          ==================================================================================================================== \n \
          %x     (pas avec tcsh.) Code de retour de la commande. \" ";
     */

    // Since V 4.5
    bool lFlagCheckPerfos(false);
    unsigned int l_CheckPerfosRamMax(0);
    unsigned int l_CheckPerfosTimeMax(0);
    std::string l_CheckPerfosXmlOutptuResult("");

    bool lFlagRegressionBinary(false);
    bool lFlagRegressionBinaryFromDirectory(false);
    bool l_RecursivelyBinary = false;

    bool lFlagRegressionAscii(false);
    bool lFlagRegressionAsciiFromDirectory(false);
    bool l_RecursivelyAscii = false;

    bool lFlagRegressionImage(false);
    bool lFlagRegressionImageFromDirectory(false);
    bool l_RecursivelyImage = false;

    bool lFlagRegressionMetadata(false);

    double lToleranceDiffValue(0);
    double lEpsilon(0);
    bool lIgnoreOrder(false);
    bool verbose(false);

    double epsilonBoundary(0.0);
    std::string baseOutputdir("");

    std::vector < std::string > baselineFilenamesMetaData;
    std::vector < std::string > testFilenamesMetaData;

    std::vector < std::string > baselineFilenamesBinary;
    std::vector < std::string > testFilenamesBinary;
    vns::TestHelper::StringMap absenceBaselineFilenamesBinary;
    std::string l_ReferenceInputDirectoryBinary;
    std::string l_ReferenceOutDirectoryBinary;
    std::string l_ReferenceOutDirectory2Binary;

    std::vector < std::string > baselineFilenamesImage;
    std::vector < std::string > testFilenamesImage;
    vns::TestHelper::StringMap absenceBaselineFilenamesImage;
    std::string l_ReferenceInputDirectoryImage;
    std::string l_ReferenceOutDirectoryImage;
    std::string l_ReferenceOutDirectory2Image;

    std::vector < std::string > baselineFilenamesAscii;
    std::vector < std::string > testFilenamesAscii;
    vns::TestHelper::StringMap absenceBaselineFilenamesAscii;
    std::string l_ReferenceInputDirectoryAscii;
    std::string l_ReferenceOutDirectoryAscii;
    std::string l_ReferenceOutDirectory2Ascii;

    std::vector < std::string > ignoredLines;
    ignoredLines.clear();

    vns::Utilities::ListOfStrings selectedFilenamesWithKeyMatchingBinary;
    selectedFilenamesWithKeyMatchingBinary.clear();
    vns::Utilities::ListOfStrings selectedFilenamesWithKeyMatchingAscii;
    selectedFilenamesWithKeyMatchingAscii.clear();
    vns::Utilities::ListOfStrings excludedFilenamesWithKeyMatchingAscii;
    excludedFilenamesWithKeyMatchingAscii.clear();
    vns::Utilities::ListOfStrings selectedFilenamesWithKeyMatchingImage;
    selectedFilenamesWithKeyMatchingImage.clear();

    std::vector < std::string > excludedGlobalFilenamesWithKeyMatching;

    vns::TestHelper testHelper;

    // -------------------------------------------------------------------------------------------------------
    // Register the MACCS ImageIO
    itk::ObjectFactoryBase::RegisterFactory(vns::GRIBImageIOFactory::New());

    // On some sgi machines, threads and stl don't mix.
#if defined(__sgi) && defined(_COMPILER_VERSION) && _COMPILER_VERSION <= 730
    itk::MultiThreader::SetGlobalDefaultNumberOfThreads(1);
#endif

    //  RegisterTests();
    //  std::string testToRun;
    //  KALIDEOS adaptation
    if (ac < 2)
    {
        return -1;
    }
    else
    {
        bool continuer = true;

        while (continuer == true)
        {

            if (strcmp(av[1], "--output-directory") == 0)
            {
                baseOutputdir = av[2];
                av += 2;
                ac -= 2;
            }
            if (strcmp(av[1], "--with-threads") == 0)
            {
                int numThreads = atoi(av[2]);
                itk::MultiThreader::SetGlobalDefaultNumberOfThreads(numThreads);
                av += 2;
                ac -= 2;
            }
            else if (strcmp(av[1], "--without-threads") == 0)
            {
                itk::MultiThreader::SetGlobalDefaultNumberOfThreads(1);
                av += 1;
                ac -= 1;
            }
            else if ((strcmp(av[1], "--verbose") == 0) || ((strcmp(av[1], "-V") == 0)))
            {
                verbose = true;
                av += 1;
                ac -= 1;
            }
            if (strcmp(av[1], "--ignore-order") == 0)
            {
                lIgnoreOrder = true;
                av += 1;
                ac -= 1;
            }
            if (strcmp(av[1], "--epsilon-boundary") == 0)
            {
                epsilonBoundary = atof(av[2]);
                av += 2;
                ac -= 2;
            }
            if (strcmp(av[1], "--compare-image") == 0)
            {
                lFlagRegressionImage = true;
                lToleranceDiffValue = (double) (::atof(av[2]));
                baselineFilenamesImage.reserve(1);
                testFilenamesImage.reserve(1);
                baselineFilenamesImage.push_back(av[3]);
                testFilenamesImage.push_back(av[4]);
                av += 4;
                ac -= 4;
            }
            else if (strcmp(av[1], "--global-exclude-filename-with-key-matching") == 0)
            {
                unsigned int nbValues = (unsigned int) (::atoi(av[2]));
                excludedGlobalFilenamesWithKeyMatching.reserve(nbValues);
                // Retrieve all the file names
                for (unsigned int i = 0; i < nbValues; ++i)
                {
                    excludedGlobalFilenamesWithKeyMatching.push_back(av[3 + i]);
                }
                av += 2 + nbValues;
                ac -= 2 + nbValues;
            }
            else if (strcmp(av[1], "--from-directory-compare-all-binary") == 0)
            {
                lFlagRegressionBinary = true;
                lFlagRegressionBinaryFromDirectory = true;
                lToleranceDiffValue = (double) (::atof(av[2]));
                baselineFilenamesImage.clear();
                testFilenamesImage.clear();
                l_ReferenceInputDirectoryBinary = av[3];
                l_ReferenceOutDirectoryBinary = av[4];
                l_ReferenceOutDirectory2Binary = av[5];
                if (itksys::SystemTools::FileIsDirectory(l_ReferenceInputDirectoryBinary) == false)
                {
                    std::cout << "ERROR: The ReferenceInputDirectory <" << l_ReferenceInputDirectoryBinary << "> doesn't exist!"
                            << std::endl;
                    return -1;
                }
                if (itksys::SystemTools::FileIsDirectory(l_ReferenceOutDirectoryBinary) == false)
                {
                    std::cout << "ERROR: The first ReferenceOutDirectory <" << l_ReferenceOutDirectoryBinary << "> doesn't exist!"
                            << std::endl;
                    return -1;
                }
                if (itksys::SystemTools::FileIsDirectory(l_ReferenceOutDirectory2Binary) == false)
                {
                    std::cout << "ERROR: The second ReferenceOutDirectory <" << l_ReferenceOutDirectory2Binary << "> doesn't exist!"
                            << std::endl;
                    return -1;
                }
                av += 5;
                ac -= 5;
                l_RecursivelyBinary = false;
                if (ac > 1)
                {
                    bool continue_to_read_dedicated_options = true;
                    while (continue_to_read_dedicated_options == true)
                    {
                        if (strcmp(av[1], "--select-filename-with-key-matching") == 0)
                        {
                            unsigned int nbSelectedFilenames = (unsigned int) (::atoi(av[2]));
                            for (unsigned int i = 0; i < nbSelectedFilenames; ++i)
                            {
                                selectedFilenamesWithKeyMatchingBinary.push_back(av[3 + i]);
                            }
                            av += 2 + nbSelectedFilenames;
                            ac -= 2 + nbSelectedFilenames;
                        }
                        else if (strcmp(av[1], "--recursively") == 0)
                        {
                            l_RecursivelyBinary = vns::Utilities::AsBool(av[2]);
                            av += 2;
                            ac -= 2;
                        }
                        else
                        {
                            continue_to_read_dedicated_options = false;
                        }
                    }
                }
                vns::TestHelper::RegressionGetListOfAllBinaryFilesFromDirectory(baselineFilenamesBinary, testFilenamesBinary,
                        l_ReferenceInputDirectoryBinary, l_ReferenceOutDirectoryBinary, l_ReferenceOutDirectory2Binary,
                        selectedFilenamesWithKeyMatchingBinary, l_RecursivelyBinary /* recursively */);

            }
            else if (strcmp(av[1], "--from-directory-compare-all-images") == 0)
            {
                lFlagRegressionImage = true;
                lFlagRegressionImageFromDirectory = true;
                lToleranceDiffValue = (double) (::atof(av[2]));
                baselineFilenamesImage.clear();
                testFilenamesImage.clear();
                l_ReferenceInputDirectoryImage = av[3];
                l_ReferenceOutDirectoryImage = av[4];
                l_ReferenceOutDirectory2Image = av[5];
                if (itksys::SystemTools::FileIsDirectory(l_ReferenceInputDirectoryImage) == false)
                {
                    std::cout << "ERROR: The ReferenceInputDirectory <" << l_ReferenceInputDirectoryImage << "> doesn't exist!"
                            << std::endl;
                    return -1;
                }
                if (itksys::SystemTools::FileIsDirectory(l_ReferenceOutDirectoryImage) == false)
                {
                    std::cout << "ERROR: The first ReferenceOutDirectory <" << l_ReferenceOutDirectoryImage << "> doesn't exist!"
                            << std::endl;
                    return -1;
                }
                if (itksys::SystemTools::FileIsDirectory(l_ReferenceOutDirectory2Image) == false)
                {
                    std::cout << "ERROR: The second ReferenceOutDirectory <" << l_ReferenceOutDirectory2Image << "> doesn't exist!"
                            << std::endl;
                    return -1;
                }
                av += 5;
                ac -= 5;
                if (ac > 1)
                {
                    bool continue_to_read_dedicated_options = true;
                    while (continue_to_read_dedicated_options == true)
                    {
                        if (strcmp(av[1], "--select-filename-with-key-matching") == 0)
                        {
                            unsigned int nbSelectedFilenames = (unsigned int) (::atoi(av[2]));
                            for (unsigned int i = 0; i < nbSelectedFilenames; ++i)
                            {
                                selectedFilenamesWithKeyMatchingImage.push_back(av[3 + i]);
                            }
                            av += 2 + nbSelectedFilenames;
                            ac -= 2 + nbSelectedFilenames;
                        }
                        else if (strcmp(av[1], "--recursively") == 0)
                        {
                            l_RecursivelyImage = vns::Utilities::AsBool(av[2]);
                            av += 2;
                            ac -= 2;
                        }
                        else
                        {
                            continue_to_read_dedicated_options = false;
                        }
                    }
                }
                vns::TestHelper::RegressionGetListOfAllImagesFilesFromDirectory(baselineFilenamesImage, testFilenamesImage,
                        l_ReferenceInputDirectoryImage, l_ReferenceOutDirectoryImage, l_ReferenceOutDirectory2Image,
                        selectedFilenamesWithKeyMatchingImage, l_RecursivelyImage /* recursively */);

            }
            else if (strcmp(av[1], "--from-directory-compare-all-ascii") == 0)
            {
                lFlagRegressionAscii = true;
                lFlagRegressionAsciiFromDirectory = true;
                lEpsilon = (double) (::atof(av[2]));
                baselineFilenamesAscii.clear();
                testFilenamesAscii.clear();
                l_ReferenceInputDirectoryAscii = av[3];
                l_ReferenceOutDirectoryAscii = av[4];
                l_ReferenceOutDirectory2Ascii = av[5];
                if (itksys::SystemTools::FileIsDirectory(l_ReferenceInputDirectoryAscii) == false)
                {
                    std::cout << "ERROR: The ReferenceInputDirectory <" << l_ReferenceInputDirectoryAscii << "> doesn't exist!"
                            << std::endl;
                    return -1;
                }
                if (itksys::SystemTools::FileIsDirectory(l_ReferenceOutDirectoryAscii) == false)
                {
                    std::cout << "ERROR: The first ReferenceOutDirectory <" << l_ReferenceOutDirectoryAscii << "> doesn't exist!"
                            << std::endl;
                    return -1;
                }
                if (itksys::SystemTools::FileIsDirectory(l_ReferenceOutDirectory2Ascii) == false)
                {
                    std::cout << "ERROR: The second ReferenceOutDirectory <" << l_ReferenceOutDirectory2Ascii << "> doesn't exist!"
                            << std::endl;
                    return -1;
                }

                av += 5;
                ac -= 5;
                l_RecursivelyAscii = false;
                if (ac > 1)
                {
                    bool continue_to_read_dedicated_options = true;
                    while (continue_to_read_dedicated_options == true)
                    {
                        if (strcmp(av[1], "--select-filename-with-key-matching") == 0)
                        {
                            unsigned int nbSelectedFilenames = (unsigned int) (::atoi(av[2]));
                            for (unsigned int i = 0; i < nbSelectedFilenames; ++i)
                            {
                                selectedFilenamesWithKeyMatchingAscii.push_back(av[3 + i]);
                            }
                            av += 2 + nbSelectedFilenames;
                            ac -= 2 + nbSelectedFilenames;
                        }
                        else if (strcmp(av[1], "--exclude-filename-with-key-matching") == 0)
                        {
                            unsigned int nbSelectedFilenames = (unsigned int) (::atoi(av[2]));
                            for (unsigned int i = 0; i < nbSelectedFilenames; ++i)
                            {
                                excludedFilenamesWithKeyMatchingAscii.push_back(av[3 + i]);
                            }
                            av += 2 + nbSelectedFilenames;
                            ac -= 2 + nbSelectedFilenames;
                        }
                        else if (strcmp(av[1], "--ignore-lines-with") == 0)
                        {
                            unsigned int nbIgnoredLines = (unsigned int) (::atoi(av[2]));
                            for (unsigned int i = 0; i < nbIgnoredLines; ++i)
                            {
                                ignoredLines.push_back(av[3 + i]);
                            }
                            av += 2 + nbIgnoredLines;
                            ac -= 2 + nbIgnoredLines;
                        }
                        else if (strcmp(av[1], "--recursively") == 0)
                        {
                            l_RecursivelyAscii = vns::Utilities::AsBool(av[2]);
                            av += 2;
                            ac -= 2;
                        }
                        else
                        {
                            continue_to_read_dedicated_options = false;
                        }
                    }
                }
                vns::TestHelper::RegressionGetListOfAllAsciiFilesFromDirectory(baselineFilenamesAscii, testFilenamesAscii,
                        l_ReferenceInputDirectoryAscii, l_ReferenceOutDirectoryAscii, l_ReferenceOutDirectory2Ascii,
                        selectedFilenamesWithKeyMatchingAscii, excludedFilenamesWithKeyMatchingAscii, l_RecursivelyAscii /* recursively */);
            }
            else if (strcmp(av[1], "--compare-n-images") == 0)
            {
                lFlagRegressionImage = true;
                lToleranceDiffValue = (double) (::atof(av[2]));
                // Number of comparisons to do
                unsigned int nbComparisons = (unsigned int) (::atoi(av[3]));
                baselineFilenamesImage.reserve(nbComparisons);
                testFilenamesImage.reserve(nbComparisons);
                // Retrieve all the file names
                for (unsigned int i = 0; i < nbComparisons; ++i)
                {
                    baselineFilenamesImage.push_back(av[4 + 2 * i]);
                    testFilenamesImage.push_back(av[5 + 2 * i]);
                }
                av += 3 + 2 * nbComparisons;
                ac -= 3 + 2 * nbComparisons;
            }
            else if (strcmp(av[1], "--compare-binary") == 0)
            {
                lFlagRegressionBinary = true;
                baselineFilenamesBinary.reserve(1);
                testFilenamesBinary.reserve(1);
                baselineFilenamesBinary.push_back(av[2]);
                testFilenamesBinary.push_back(av[3]);
                av += 3;
                ac -= 3;
            }
            else if (strcmp(av[1], "--compare-n-binary") == 0)
            {
                lFlagRegressionBinary = true;
                unsigned int nbComparisons = (unsigned int) (::atoi(av[2]));
                baselineFilenamesBinary.reserve(nbComparisons);
                testFilenamesBinary.reserve(nbComparisons);
                // Retrieve all the file names
                for (unsigned int i = 0; i < nbComparisons; ++i)
                {
                    baselineFilenamesBinary.push_back(av[3 + 2 * i]);
                    testFilenamesBinary.push_back(av[4 + 2 * i]);
                }
                av += 2 + 2 * nbComparisons;
                ac -= 2 + 2 * nbComparisons;
            }
            else if (strcmp(av[1], "--compare-ascii") == 0)
            {
                lFlagRegressionAscii = true;
                lEpsilon = (double) (::atof(av[2]));
                baselineFilenamesAscii.reserve(1);
                testFilenamesAscii.reserve(1);
                baselineFilenamesAscii.push_back(av[3]);
                testFilenamesAscii.push_back(av[4]);
                av += 4;
                ac -= 4;

                if (ac > 1)
                {
                    if (strcmp(av[1], "--ignore-lines-with") == 0)
                    {
                        unsigned int nbIgnoredLines = (unsigned int) (::atoi(av[2]));
                        for (unsigned int i = 0; i < nbIgnoredLines; ++i)
                        {
                            ignoredLines.push_back(av[3 + i]);
                        }
                        av += 2 + nbIgnoredLines;
                        ac -= 2 + nbIgnoredLines;
                    }
                }
            }
            else if (strcmp(av[1], "--compare-n-ascii") == 0)
            {
                lFlagRegressionAscii = true;
                lEpsilon = (double) (::atof(av[2]));
                // Number of comparisons to do
                unsigned int nbComparisons = (unsigned int) (::atoi(av[3]));
                baselineFilenamesAscii.reserve(nbComparisons);
                testFilenamesAscii.reserve(nbComparisons);
                // Retrieve all the file names
                for (unsigned int i = 0; i < nbComparisons; ++i)
                {
                    baselineFilenamesAscii.push_back(av[4 + 2 * i]);
                    testFilenamesAscii.push_back(av[5 + 2 * i]);
                }
                av += 3 + 2 * nbComparisons;
                ac -= 3 + 2 * nbComparisons;

                if (ac > 1)
                {
                    if (strcmp(av[1], "--ignore-lines-with") == 0)
                    {
                        unsigned int nbIgnoredLines = (unsigned int) (::atoi(av[2]));
                        for (unsigned int i = 0; i < nbIgnoredLines; ++i)
                        {
                            ignoredLines.push_back(av[3 + i]);
                        }
                        av += 2 + nbIgnoredLines;
                        ac -= 2 + nbIgnoredLines;
                    }
                }

            }
            else if (strcmp(av[1], "--compare-metadata") == 0)
            {
                lFlagRegressionMetadata = true;
                lToleranceDiffValue = (double) (::atof(av[2]));
                baselineFilenamesMetaData.reserve(1);
                testFilenamesMetaData.reserve(1);
                baselineFilenamesMetaData.push_back(av[3]);
                testFilenamesMetaData.push_back(av[4]);
                av += 4;
                ac -= 4;
            }
            else
            {
                std::cout << "No specific option." << std::endl;
            }
            if (strcmp(av[1], "--check-perfos") == 0)
            {
                lFlagCheckPerfos = true;
                av += 1;
                ac -= 1;
                if (ac > 1)
                {
                    bool continue_to_read_dedicated_options = true;
                    while (continue_to_read_dedicated_options == true)
                    {
                        if (strcmp(av[1], "--ram-max") == 0)
                        {
                            l_CheckPerfosRamMax = (unsigned int) (::atoi(av[2]));
                            av += 2;
                            ac -= 2;
                        }
                        else if (strcmp(av[1], "--time-max") == 0)
                        {
                            l_CheckPerfosTimeMax = (unsigned int) (::atoi(av[2]));
                            av += 2;
                            ac -= 2;
                        }
                        else if (strcmp(av[1], "--xml-output-result") == 0)
                        {
                            l_CheckPerfosXmlOutptuResult = av[2];
                            av += 2;
                            ac -= 2;
                        }
                        else
                        {
                            continue_to_read_dedicated_options = false;
                        }
                    }
                }
            }
            if (av[1][0] != '-')
            {
                continuer = false;
            }
        }
    }
    if ((lFlagRegressionBinary == true) || (lFlagRegressionImage == true) || (lFlagRegressionAscii == true)
            || (lFlagRegressionMetadata == true))
    {
        if (baseOutputdir == "")
        {
            std::cout << "ERROR: The output directory must be specified. Add --output-directory in command line argument  -------------"
                    << std::endl;
            return -1;
        }
    }
    // Run Sub process av[] with argument ...
    ac = ac - 1;
    av = av + 1;
    std::cout << "Star running command : " << av[0] << std::endl;
    char** args = new char*[ac + 1];
    for (int i = 0; i < ac; i++)
    {
        args[i] = av[i];
        std::cout << "Argument : " << args[i] << std::endl;
    }
    args[ac] = NULL;

    itk::TimeProbe clock;
    clock.Start();
    // Call process
    std::cout << "Start processing ..." << std::endl;
    itksysProcess * process = itksysProcess_New();
    itksysProcess_SetCommand(process, args);
    itksysProcess_SetOption(process, itksysProcess_Option_Detach, 0);
    itksysProcess_SetPipeShared(process, itksysProcess_Pipe_STDOUT, true);
    itksysProcess_SetPipeShared(process, itksysProcess_Pipe_STDERR, true);
    itksysProcess_Execute(process);
    itksysProcess_WaitForExit(process, NULL);

    delete[] args;
    int state = itksysProcess_GetState(process);
    std::cout << "Command itk state return '" << state << "'." << std::endl;

    int main_result = itksysProcess_GetExitValue(process);
    std::cout << "Command itk ExitValue return '" << main_result << "'." << std::endl;
    itksysProcess_Kill(process);

    clock.Stop();
    std::cout << "Processing done in " << clock.GetTotal() << " secondes (" << vns::Utilities::GetStringFormatedTime(clock.GetTotal())
            << ")." << std::endl;
    std::cout << "baselineFilenamesAscii.size() : " << baselineFilenamesAscii.size() << std::endl;

    if (main_result != EXIT_SUCCESS)
    {
        std::cout << "-> Test EXIT FAILURE (" << main_result << ")." << std::endl;
    }
    else
    {
        std::cout << "-> Test EXIT SUCESS (" << main_result << ")." << std::endl;
    }

    int compare_result = 0;
    if ((lFlagRegressionImage == false) && (lFlagRegressionBinary == false) && (lFlagRegressionAscii == false)
            && (lFlagRegressionMetadata == false) && (lFlagCheckPerfos == false))
    {
        std::cout << "ERROR: -------------  No control baseline command    -------------" << std::endl;
    }
    else
    {
        std::cout << "TestHelper::RegressionTestAllImages - itk::ImageToImageFilterCommon::GetGlobalDefaultDirectionTolerance  : "
                << itk::ImageToImageFilterCommon::GetGlobalDefaultDirectionTolerance() << std::endl;
        std::cout << "                                    - itk::ImageToImageFilterCommon::GetGlobalDefaultCoordinateTolerance : "
                << itk::ImageToImageFilterCommon::GetGlobalDefaultCoordinateTolerance() << std::endl;

        if (baseOutputdir == "")
        {
            std::cout << "ERROR: The output directory must be specified. Add --output-directory in command line argument  -------------"
                    << std::endl;
            return -1;
        }
        itksys::SystemTools::MakeDirectory(baseOutputdir.c_str());
        try
        {
            std::cout << "-------------  Start control baseline tests    -------------" << std::endl;
            // Make a list of possible baselines
            if ((lFlagRegressionBinary == true) && (testFilenamesBinary.empty() == true))
            {
                vnsLogErrorMacro("The list of binary filename of the baseline is empty !!")
                return -1;
            }
            if ((lFlagRegressionAscii == true) && (testFilenamesAscii.empty() == true))
            {
                vnsLogErrorMacro("The list of ascii filename of the baseline is empty !!")
                return -1;
            }
            if ((lFlagRegressionImage == true) && (testFilenamesImage.empty() == true))
            {
                vnsLogWarningMacro("The list of images filename of the baseline is empty !!")
//                return -1;
            }
            if ((lFlagRegressionMetadata == true) && (testFilenamesMetaData.empty() == true))
            {
                vnsLogErrorMacro("The list of metadata filename of the baseline is empty !!")
                return -1;
            }

            if (verbose)
            {
                std::cout << " Enable VERBOSE mode" << std::endl;
                testHelper.VerboseOn();
            }
            else
            {
                std::cout << " Disable VERBOSE mode" << std::endl;
                testHelper.VerboseOff();
            }
            if (lIgnoreOrder)
            {
                testHelper.IgnoreLineOrderOn();
            }
            else
            {
                testHelper.IgnoreLineOrderOff();
            }

            testHelper.SetToleranceDiffValue(lToleranceDiffValue);
            testHelper.SetEpsilon(lEpsilon);

            if (epsilonBoundary != 0.0)
            {
                testHelper.SetEpsilonBoundaryChecking(epsilonBoundary);
            }

            if (lFlagRegressionAsciiFromDirectory == true)
            {
                absenceBaselineFilenamesAscii = vns::TestHelper::RegressionGetListOfAllAbsenceAsciiFilesFromDirectory(
                        l_ReferenceInputDirectoryAscii, l_ReferenceOutDirectory2Ascii, selectedFilenamesWithKeyMatchingAscii,
                        excludedFilenamesWithKeyMatchingAscii, excludedGlobalFilenamesWithKeyMatching,
                        l_RecursivelyAscii /* recursively */);
            }
            if (lFlagRegressionImageFromDirectory == true)
            {
                absenceBaselineFilenamesImage = vns::TestHelper::RegressionGetListOfAllAbsenceImageFilesFromDirectory(
                        l_ReferenceInputDirectoryImage, l_ReferenceOutDirectory2Image, selectedFilenamesWithKeyMatchingImage,
                        excludedGlobalFilenamesWithKeyMatching, l_RecursivelyImage /* recursively */);
            }
            if (lFlagRegressionBinaryFromDirectory == true)
            {
                absenceBaselineFilenamesBinary = vns::TestHelper::RegressionGetListOfAllAbsenceBinaryFilesFromDirectory(
                        l_ReferenceInputDirectoryBinary, l_ReferenceOutDirectory2Binary, selectedFilenamesWithKeyMatchingBinary,
                        excludedGlobalFilenamesWithKeyMatching, l_RecursivelyBinary /* recursively */);
            }

            // ---------------------------------------------------------------------------------------------
            compare_result -= testHelper.RegressionTestAllImages(baselineFilenamesImage, testFilenamesImage, absenceBaselineFilenamesImage,
                    baseOutputdir);
            // ---------------------------------------------------------------------------------------------
            compare_result -= testHelper.RegressionTestAllMetaData(baselineFilenamesMetaData, testFilenamesMetaData);
            // ---------------------------------------------------------------------------------------------
            compare_result -= testHelper.RegressionTestAllAsciiFiles(baselineFilenamesAscii, testFilenamesAscii,
                    absenceBaselineFilenamesAscii, ignoredLines, baseOutputdir);
            // ---------------------------------------------------------------------------------------------
            compare_result -= testHelper.RegressionTestAllBinaryFiles(baselineFilenamesBinary, testFilenamesBinary,
                    absenceBaselineFilenamesBinary);
            if (lFlagCheckPerfos == true)
            {        // ---------------------------------------------------------------------------------------------
                compare_result -= testHelper.RegressionTestCheckPerformances(l_CheckPerfosRamMax, l_CheckPerfosTimeMax,
                        l_CheckPerfosXmlOutptuResult);
            }
        }
        catch (itk::ExceptionObject & e)
        {
            std::cerr << "otbTestMain 'control baseline test': ITK Exception thrown:" << std::endl;
            std::cerr << e.GetFile() << ":" << e.GetLine() << ":" << std::endl;
            std::cerr << e.GetDescription() << std::endl;
            return -1;
        }
        catch (std::bad_alloc & err)
        {
            std::cerr << "otbTestMain 'control baseline test': Exception bad_alloc thrown: " << std::endl;
            std::cerr << (char*) err.what() << std::endl;
            return -1;
        }
        catch (const std::exception& e)
        {
            std::cerr << "otbTestMain 'control baseline test': std::exception  thrown:" << std::endl;
            std::cerr << e.what() << std::endl;
            return -1;
        }
        catch (...)
        {
            std::cerr << "otbTestMain 'control baseline test': Unknown exception thrown !" << std::endl;
            return -1;
        }
        if (compare_result != 0)
        {
            std::cout << "-> Compare FAILURE (" << compare_result << ")." << std::endl;
        }
        else
        {
            std::cout << "-> Compare SUCESS (" << compare_result << ")." << std::endl;
        }
        std::cout << "-------------  End control baseline tests    -------------" << std::endl;

    }
    return (main_result + compare_result);
}
