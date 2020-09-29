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
 * VERSION : 2.0.0 : DM : LAIG-DM-MAJA-153436-CS : 6 mars 2017 : Refactoring pour ameliorer la qualite      *
 * VERSION : 1.0.0 : FA : LAIG-FA-MAC-148399-CS : 27 octobre 2016 : Audit code - Correction qualite         *
 * VERSION : 1.0.0 : FA : LAIG-FA-MAC-1988-CNES : 20 octobre 2016 : Correction qualite (code mort)          *
 * VERSION : 5-1-0 : FA : LAIG-FA-MAC-144776-CS : 10 mai 2016 : Correction recherche multibaseline          *
 * VERSION : 4-0-0 : FA : LAIG-FA-MAC-117040-CS : 31 mars 2014 : Modifications mineures                     *
 * VERSION : 2-1-0 : FA : LAIG-FA-MAC-354-CNES : 8 juin 2012 : Correction qualite (regle ou metrique)       *
 *                                                                                                          *
 * VERSION : 1-0-0 : <TypeFT> : <NumFT> : 17 decembre 2010 : Creation
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id$
 *                                                                                                          *
 ************************************************************************************************************/
#ifndef __vnsTestMain_h
#define __vnsTestMain_h

#include "itkImageToImageFilterCommon.h"
#include "itkWin32Header.h"
#include <map>
#include <string>
#include <iostream>
#include <fstream>

#include "itkMultiThreader.h"
#include "itksys/SystemTools.hxx"

#include "vnsTestHelper.h"

#include "vnsUtilities.h"
#include "vnsGRIBImageIOFactory.h"
#include "vnsAtomicRegistry.h"
#include "itkObjectFactoryBase.h"
#include "itkTimeProbe.h"

// ---------------------------------------------------------------------------------------------
typedef int
(*MainFuncPointer)(int, char*[]);
std::map<std::string, MainFuncPointer> StringToTestFunctionMap;

// ---------------------------------------------------------------------------------------------
#define REGISTER_TEST(test) \
  extern int test(int, char*[]); \
  StringToTestFunctionMap[# test] = test

// ---------------------------------------------------------------------------------------------
void
RegisterTests();
// ---------------------------------------------------------------------------------------------
void
PrintAvailableTests()
{
    std::cout << "Tests available:\n";
    std::map<std::string, MainFuncPointer>::iterator j = StringToTestFunctionMap.begin();
    int i = 0;
    while (j != StringToTestFunctionMap.end())
    {
        std::cout << i << ". " << j->first << "\n";
        ++i;
        ++j;
    }
}

// ---------------------------------------------------------------------------------------------
int
main(int ac, char* av[])
{
    // ---------------------------------------------------------------------------------------------
    // ---------------------------------------------------------------------------------------------
    // ---------------------------------------------------------------------------------------------
    // ---------------------------------------------------------------------------------------------
    // ---------------------------------------------------------------------------------------------
    // ---------------------------------------------------------------------------------------------
    // ---------------------------------------------------------------------------------------------
    // ---------------------------------------------------------------------------------------------
    // ---------------------------------------------------------------------------------------------
    // ---------------------------------------------------------------------------------------------
    // ---------------------------------------------------------------------------------------------
    // ---------------------------------------------------------------------------------------------
    // ---------------------------------------------------------------------------------------------
    // ---------------------------------------------------------------------------------------------
    // ---------------------------------------------------------------------------------------------
    bool lFlagRegressionMetadata(false);
    bool lFlagRegressionBinary(false);
    bool lFlagRegressionBinaryFromDirectory(false);
    bool l_RecursivelyBinary = false;

    bool lFlagRegressionAscii(false);
    bool lFlagRegressionAsciiFromDirectory(false);
    bool l_RecursivelyAscii = false;

    bool lFlagRegressionImage(false);
    bool lFlagRegressionImageFromDirectory(false);
    bool l_RecursivelyImage = false;

    double lToleranceDiffValue(0);
    double lEpsilon(0);
    bool lIgnoreOrder(false);
    bool verbose(false);
    double epsilonBoundary(0.0);

    // -------------------------------------------------------------------------------------------------------
    // Register the MAJA ImageIO
    itk::ObjectFactoryBase::RegisterFactory(vns::GRIBImageIOFactory::New());
    vns::AtomicRegistry<itk::MetaImageIOFactory> metaRegistry;

    // ---------------------------------------------------------------------------------------------
    std::vector<std::string> baselineFilenamesBinary;
    std::vector<std::string> testFilenamesBinary;
    vns::TestHelper::StringMap absenceBaselineFilenamesBinary;
    std::string l_ReferenceInputDirectoryBinary;
    std::string l_ReferenceOutDirectoryBinary;
    std::string l_ReferenceOutDirectory2Binary;

    std::vector<std::string> baselineFilenamesMetaData;
    std::vector<std::string> testFilenamesMetaData;

    // ---------------------------------------------------------------------------------------------
    std::vector<std::string> baselineFilenamesImage;
    std::vector<std::string> testFilenamesImage;
    vns::TestHelper::StringMap absenceBaselineFilenamesImage;
    std::string l_ReferenceInputDirectoryImage;
    std::string l_ReferenceOutDirectoryImage;
    std::string l_ReferenceOutDirectory2Image;

    std::vector<std::string> baselineFilenamesAscii;
    std::vector<std::string> testFilenamesAscii;
    vns::TestHelper::StringMap absenceBaselineFilenamesAscii;

    std::string l_ReferenceInputDirectoryAscii;
    std::string l_ReferenceOutDirectoryAscii;
    std::string l_ReferenceOutDirectory2Ascii;

    std::vector<std::string> ignoredLines;
    ignoredLines.clear();

    vns::Utilities::ListOfStrings selectedFilenamesWithKeyMatchingBinary;
    selectedFilenamesWithKeyMatchingBinary.clear();
    vns::Utilities::ListOfStrings selectedFilenamesWithKeyMatchingAscii;
    selectedFilenamesWithKeyMatchingAscii.clear();
    vns::Utilities::ListOfStrings excludedFilenamesWithKeyMatchingAscii;
    excludedFilenamesWithKeyMatchingAscii.clear();
    vns::Utilities::ListOfStrings selectedFilenamesWithKeyMatchingImage;
    selectedFilenamesWithKeyMatchingImage.clear();

    std::vector<std::string> excludedGlobalFilenamesWithKeyMatching;

    // ---------------------------------------------------------------------------------------------
    vns::TestHelper testHelper;

// On some sgi machines, threads and stl don't mix.
#if defined(__sgi) && defined(_COMPILER_VERSION) && _COMPILER_VERSION <= 730
    itk::MultiThreader::SetGlobalDefaultNumberOfThreads(1);
#endif

    // ---------------------------------------------------------------------------------------------
    RegisterTests();
    std::string testToRun;
    // ---------------------------------------------------------------------------------------------
    if (ac < 2)
    {
        // ---------------------------------------------------------------------------------------------
        PrintAvailableTests();
        std::cout << "To launch a test, enter its number: ";
        int testNum = 0;
        std::cin >> testNum;
        std::map<std::string, MainFuncPointer>::iterator j = StringToTestFunctionMap.begin();
        int i = 0;
        // ---------------------------------------------------------------------------------------------
        while (j != StringToTestFunctionMap.end() && i < testNum)
        {
            ++i;
            ++j;
        }
        // ---------------------------------------------------------------------------------------------
        if (j == StringToTestFunctionMap.end())
        {
            std::cerr << testNum << " is not a valid test number\n";
            return -1;
        }
        testToRun = j->first;
    }
    else
    {
        bool continuer = true;

        while (continuer == true)
        {

            // ---------------------------------------------------------------------------------------------
            if (strcmp(av[1], "--with-threads") == 0)
            {
                int numThreads = atoi(av[2]);
                itk::MultiThreader::SetGlobalDefaultNumberOfThreads(numThreads);
                av = av + 2;
                ac = ac - 2;
            }
            // ---------------------------------------------------------------------------------------------
            else if (strcmp(av[1], "--without-threads") == 0)
            {
                itk::MultiThreader::SetGlobalDefaultNumberOfThreads(1);
                av = av + 1;
                ac = ac - 1;
            }
            else
            {
                std::cout << "Number of threads automatically detected and used is "
                        << itk::MultiThreader::GetGlobalDefaultNumberOfThreads() << "." << std::endl;
            }
            if ((strcmp(av[1], "--verbose") == 0) || ((strcmp(av[1], "-V") == 0)))
            {
                verbose = true;
                av += 1;
                ac -= 1;
            }
            // ---------------------------------------------------------------------------------------------
            if (strcmp(av[1], "--ignore-order") == 0)
            {
                lIgnoreOrder = true;
                av = av + 1;
                ac = ac - 1;
            }
            // ---------------------------------------------------------------------------------------------
            if (strcmp(av[1], "--epsilon-boundary") == 0)
            {
                epsilonBoundary = atof(av[2]);
                av = av + 2;
                ac = ac - 2;
            }
            // ---------------------------------------------------------------------------------------------
            if (strcmp(av[1], "--compare-image") == 0)
            {
                lFlagRegressionImage = true;
                lToleranceDiffValue = (double) (::atof(av[2]));
                baselineFilenamesImage.reserve(1);
                testFilenamesImage.reserve(1);
                baselineFilenamesImage.push_back(av[3]);
                testFilenamesImage.push_back(av[4]);
                av = av + 4;
                ac = ac - 4;
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
                bool l_RecursivelyImage = false;
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
            // ---------------------------------------------------------------------------------------------
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
                av = av + (3 + 2 * nbComparisons);
                ac = ac - (3 + 2 * nbComparisons);
            }
            // ---------------------------------------------------------------------------------------------
            else if (strcmp(av[1], "--compare-binary") == 0)
            {
                lFlagRegressionBinary = true;
                baselineFilenamesBinary.reserve(1);
                testFilenamesBinary.reserve(1);
                baselineFilenamesBinary.push_back(av[2]);
                testFilenamesBinary.push_back(av[3]);
                av = av + 3;
                ac = ac - 3;
            }
            // ---------------------------------------------------------------------------------------------
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
                av = av + (2 + 2 * nbComparisons);
                ac = ac - (2 + 2 * nbComparisons);
            }
            // ---------------------------------------------------------------------------------------------
            else if (strcmp(av[1], "--compare-ascii") == 0)
            {
                lFlagRegressionAscii = true;
                lEpsilon = (double) (::atof(av[2]));
                baselineFilenamesAscii.reserve(1);
                testFilenamesAscii.reserve(1);
                baselineFilenamesAscii.push_back(av[3]);
                testFilenamesAscii.push_back(av[4]);
                av = av + 4;
                ac = ac - 4;

                // ---------------------------------------------------------------------------------------------
                // ---------------------------------------------------------------------------------------------
                if (ac > 1)
                {
                    if (strcmp(av[1], "--ignore-lines-with") == 0)
                    {
                        unsigned int nbIgnoredLines = (unsigned int) (::atoi(av[2]));
                        for (unsigned int i = 0; i < nbIgnoredLines; ++i)
                        {
                            ignoredLines.push_back(av[3 + i]);
                        }
                        av = av + (2 + nbIgnoredLines);
                        ac = ac - (2 + nbIgnoredLines);
                    }
                }
            }
            // ---------------------------------------------------------------------------------------------
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
                av = av + (3 + 2 * nbComparisons);
                ac = ac - (3 + 2 * nbComparisons);

                if (ac > 1)
                {
                    if (strcmp(av[1], "--ignore-lines-with") == 0)
                    {
                        unsigned int nbIgnoredLines = (unsigned int) (::atoi(av[2]));
                        for (unsigned int i = 0; i < nbIgnoredLines; ++i)
                        {
                            ignoredLines.push_back(av[3 + i]);
                        }
                        av = av + (2 + nbIgnoredLines);
                        ac = ac - (2 + nbIgnoredLines);
                    }
                }

            }
            // ---------------------------------------------------------------------------------------------
            // ---------------------------------------------------------------------------------------------
            else if (strcmp(av[1], "--compare-metadata") == 0)
            {
                lFlagRegressionMetadata = true;
                lToleranceDiffValue = (double) (::atof(av[2]));
                baselineFilenamesMetaData.reserve(1);
                testFilenamesMetaData.reserve(1);
                baselineFilenamesMetaData.push_back(av[3]);
                testFilenamesMetaData.push_back(av[4]);
                av = av + 4;
                ac = ac - 4;
            }
            else
            {
                std::cout << "No specific option." << std::endl;
            }
            if (av[1][0] != '-')
            {
                continuer = false;
            }
        }
    }
    testToRun = av[1];
    // ---------------------------------------------------------------------------------------------
    // ---------------------------------------------------------------------------------------------
    // ---------------------------------------------------------------------------------------------
    // ---------------------------------------------------------------------------------------------
    std::map<std::string, MainFuncPointer>::iterator j = StringToTestFunctionMap.find(testToRun);
    if (j != StringToTestFunctionMap.end())
    {
        MainFuncPointer f = j->second;
        int result(0);
        itk::TimeProbe clock;
        clock.Start();
        // Call process
        std::cout << "Start processing ..." << std::endl;
        // ---------------------------------------------------------------------------------------------
        try
        {
            // Invoke the test's "main" function.
            result = (*f)(ac - 1, av + 1);
            if (result != EXIT_SUCCESS)
            {
                std::cout << "-> Test EXIT FAILURE (" << result << ")." << std::endl;
                itkGenericExceptionMacro(<< "Function returns EXIT_FAILURE (no regression test)");
            }
        }
        // ---------------------------------------------------------------------------------------------
        catch (itk::ExceptionObject& e)
        {
            std::cerr << "otbTestMain '" << testToRun << "': ITK Exception thrown:" << std::endl;
            std::cerr << e.GetFile() << ":" << e.GetLine() << ":" << std::endl;
            std::cerr << e.GetDescription() << std::endl;
            result = EXIT_FAILURE;
        }
        // ---------------------------------------------------------------------------------------------
        catch (std::bad_alloc& err)
        {
            std::cerr << "otbTestMain '" << testToRun << "': Exception bad_alloc thrown: " << std::endl;
            std::cerr << (char*) err.what() << std::endl;
            result = EXIT_FAILURE;
        }
        // ---------------------------------------------------------------------------------------------
        catch (const std::exception& e)
        {
            std::cerr << "otbTestMain '" << testToRun << "': std::exception  thrown:" << std::endl;
            std::cerr << e.what() << std::endl;
            result = EXIT_FAILURE;
        }
        // ---------------------------------------------------------------------------------------------
        catch (...)
        {
            std::cerr << "otbTestMain '" << testToRun << "': Unknown exception thrown !" << std::endl;
            result = EXIT_FAILURE;
        }
        clock.Stop();
        std::cout << "Processing done in " << clock.GetTotal() << " secondes (" << vns::Utilities::GetStringFormatedTime(clock.GetTotal())
                << ")." << std::endl;

        // ---------------------------------------------------------------------------------------------
        if (result == EXIT_SUCCESS)
        {
            result = 0;
            std::cout << " -> Test EXIT SUCCESS." << std::endl;
            if ((lFlagRegressionMetadata == false) && (lFlagRegressionAscii == false) && (lFlagRegressionBinary == false)
                    && (lFlagRegressionImage == false))
            {
                std::cout << "-------------  No control baseline tests    -------------" << std::endl;
            }
            else
            {
                try
                {
                    std::cout << "-------------  Start control baseline tests    -------------" << std::endl;
                    // ---------------------------------------------------------------------------------------------
                    // Make a list of possible baselines
                    if ((lFlagRegressionAscii == true) && (baselineFilenamesAscii.empty() == true))
                    {
                        std::cout << "The compare ASCII is activated but the list of filename in the baseline is empty !" << std::endl;
                        return -1;
                    }
                    if ((lFlagRegressionMetadata == true) && (baselineFilenamesMetaData.empty() == true))
                    {
                        std::cout << "The compare METADATA is activated but the list of filename in the baseline is empty !" << std::endl;
                        return -1;
                    }
                    if ((lFlagRegressionImage == true) && (baselineFilenamesImage.empty() == true))
                    {
                        std::cout << "The compare IMAGE is activated but the list of filename in the baseline is empty !" << std::endl;
                        return -1;
                    }
                    if ((lFlagRegressionBinary == true) && (baselineFilenamesBinary.empty() == true))
                    {
                        std::cout << "The compare BINARY is activated but the list of filename in the baseline is empty !" << std::endl;
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

                    // ---------------------------------------------------------------------------------------------
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
                    result += testHelper.RegressionTestAllImages(baselineFilenamesImage, testFilenamesImage, absenceBaselineFilenamesImage,
                            "");
                    // ---------------------------------------------------------------------------------------------
                    result += testHelper.RegressionTestAllMetaData(baselineFilenamesMetaData, testFilenamesMetaData);
                    // ---------------------------------------------------------------------------------------------
                    result += testHelper.RegressionTestAllAsciiFiles(baselineFilenamesAscii, testFilenamesAscii,
                            absenceBaselineFilenamesAscii, ignoredLines, "");
                    // ---------------------------------------------------------------------------------------------
                    result += testHelper.RegressionTestAllBinaryFiles(baselineFilenamesBinary, testFilenamesBinary,
                            absenceBaselineFilenamesBinary);

                }
                // ---------------------------------------------------------------------------------------------
                // ---------------------------------------------------------------------------------------------
                // ---------------------------------------------------------------------------------------------
                catch (itk::ExceptionObject& e)
                {
                    std::cerr << "otbTestMain 'control baseline test': ITK Exception thrown:" << std::endl;
                    std::cerr << e.GetFile() << ":" << e.GetLine() << ":" << std::endl;
                    std::cerr << e.GetDescription() << std::endl;
                    return -1;
                }
                catch (std::bad_alloc& err)
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
                std::cout << "-------------  End control baseline tests    -------------" << std::endl;
            }
        }
        return result;
    }
    PrintAvailableTests();
    std::cerr << "Failure: " << testToRun << ": no test identified " << testToRun << "\n";
    return -1;
}

#endif
