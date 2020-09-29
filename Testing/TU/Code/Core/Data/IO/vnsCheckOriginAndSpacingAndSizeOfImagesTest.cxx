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
 * VERSION : 1-0-0 : <TypeFT> : <NumFT> : 18 mars 2010 : Creation                                                           
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id: vnsImageFileReaderTest.cxx 9330 2015-01-13 13:26:05Z tfeuvrie $
 *                                                                                                          *
 ************************************************************************************************************/

#include "otbImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "otbCommandLineArgumentParser.h"
#include "itkMultiThreader.h"

#include "vnsMacro.h"
#include "vnsMath.h"
#include "vnsUtilities.h"
#include "vnsGRIBImageIOFactory.h"
#include "itkObjectFactoryBase.h"
#include "itkMetaImageIOFactory.h"

int
vnsCheckOriginAndSpacingAndSizeOfImagesTest(int argc, char* argv[])
{
    // -------------------------------------------------------------------------------------------------------
    // Register the MACCS ImageIO
    itk::ObjectFactoryBase::RegisterFactory(vns::GRIBImageIOFactory::New());
    itk::ObjectFactoryBase::RegisterFactory(itk::MetaImageIOFactory::New());

    // Parse command line parameters
    typedef otb::CommandLineArgumentParser ParserType;
    ParserType::Pointer parser = ParserType::New();

    bool l_CheckOrigin(false);
    bool l_CheckSpacing(false);
    bool l_CheckSize(false);

    parser->SetProgramDescription("Check footprint of two images.");
    parser->AddOption("--Image1", "Image1", "-i1", 1, true);
    parser->AddOption("--Image2", "Image2", "-i2", 1, true);
    parser->AddOption("--Epsilon", "Epsilon", "-e", 1, false);
    parser->AddOption("--check-origin", "Origin", "", 1, true);
    parser->AddOption("--check-spacing", "Origin", "", 1, true);
    parser->AddOption("--check-size", "Origin", "", 1, true);

    typedef otb::CommandLineArgumentParseResult ParserResultType;
    ParserResultType::Pointer parseResult = ParserResultType::New();

    try
    {
        parser->ParseCommandLine(argc, argv, parseResult);
    }
    catch (itk::ExceptionObject & err)
    {
        std::string descriptionException = err.GetDescription();
        if (descriptionException.find("ParseCommandLine(): Help Parser") != std::string::npos)
        {
            std::cout << "WARNING : output file pixels are converted in 'unsigned char'" << std::endl;
            return EXIT_SUCCESS;
        }
        if (descriptionException.find("ParseCommandLine(): Version Parser") != std::string::npos)
        {
            return EXIT_SUCCESS;
        }
        return EXIT_FAILURE;
    }

    typedef otb::Image<char> ImageType;
    typedef otb::ImageFileReader<ImageType> ReaderType;
    typedef otb::ImageFileWriter<ImageType> WriterType;

    double epsilon = 1.e-6;
    const std::string inputFilename1 = parseResult->GetParameterString("--Image1");
    const std::string inputFilename2 = parseResult->GetParameterString("--Image2");

    if (parseResult->IsOptionPresent("--check-origin") == true)
    {
        l_CheckOrigin = vns::Utilities::AsBool(parseResult->GetParameterString("--check-origin"));
    }
    if (parseResult->IsOptionPresent("--check-spacing") == true)
    {
        l_CheckSpacing =  vns::Utilities::AsBool(parseResult->GetParameterString("--check-spacing"));
    }
    if (parseResult->IsOptionPresent("--check-size") == true)
    {
        l_CheckSize =  vns::Utilities::AsBool(parseResult->GetParameterString("--check-size"));
    }

    std::cout << "Input filename 1 : " << inputFilename1 << std::endl;
    std::cout << "Input filename 2 : " << inputFilename2 << std::endl;
    std::cout << "epsilon          : " << epsilon << std::endl;
    std::cout << "check origin     : " << vns::Utilities::BoolToLowerString(l_CheckOrigin) << std::endl;
    std::cout << "check spacing    : " << vns::Utilities::BoolToLowerString(l_CheckSpacing) << std::endl;
    std::cout << "check size       : " << vns::Utilities::BoolToLowerString(l_CheckSize) << std::endl;

    ReaderType::Pointer reader1 = ReaderType::New();
    ReaderType::Pointer reader2 = ReaderType::New();

    reader1->SetFileName(inputFilename1);
    reader2->SetFileName(inputFilename2);

    reader1->UpdateOutputInformation();
    reader2->UpdateOutputInformation();

    std::cout << "                          - Input image 1  origin centre pixel:         [" << reader1->GetOutput()->GetOrigin() << "]"
            << std::endl;
    std::cout << "                                           spacing:                     [" << reader1->GetOutput()->GetSpacing() << "]"
            << std::endl;
    std::cout << "                                           size:                        ["
            << reader1->GetOutput()->GetLargestPossibleRegion().GetSize() << "]" << std::endl;
    std::cout << "                                           upper left corner:           ["
            << reader1->GetOutput()->GetUpperLeftCorner()[0] << ", " << reader1->GetOutput()->GetUpperLeftCorner()[1] << "]" << std::endl;
    std::cout << "                          - Input image 2  origin centre pixel:         [" << reader2->GetOutput()->GetOrigin() << "]"
            << std::endl;
    std::cout << "                                           spacing:                     [" << reader2->GetOutput()->GetSpacing() << "]"
            << std::endl;
    std::cout << "                                           size:                        ["
            << reader2->GetOutput()->GetLargestPossibleRegion().GetSize() << "]" << std::endl;
    std::cout << "                                           upper left corner:           ["
            << reader2->GetOutput()->GetUpperLeftCorner()[0] << ", " << reader2->GetOutput()->GetUpperLeftCorner()[1] << "]" << std::endl;

    bool egal(true);
    // Check Origin
    if (l_CheckOrigin == true)
    {
        if ( vnsDifferentTab2DimsDoubleWithEpsilonMacro(reader1->GetOutput()->GetOrigin(), reader2->GetOutput()->GetOrigin(), epsilon)
                == true)
        {
            egal = false;
            std::cout << "Check Origin [" << reader1->GetOutput()->GetOrigin() << "] with reference [" << reader2->GetOutput()->GetOrigin()
                    << "] -> FAILED." << std::endl;
        }
        else
        {
            std::cout << "Check Origin [" << reader1->GetOutput()->GetOrigin() << "] with reference [" << reader2->GetOutput()->GetOrigin()
                    << "] -> passed." << std::endl;
        }
    }

    if (l_CheckSpacing == true)
    {
        if ( vnsDifferentTab2DimsDoubleWithEpsilonMacro(reader1->GetOutput()->GetSpacing(), reader2->GetOutput()->GetSpacing(), epsilon)
                == true)
        {
            egal = false;
            std::cout << "Check Spacing [" << reader1->GetOutput()->GetSpacing() << "] with reference ["
                    << reader2->GetOutput()->GetSpacing() << "] -> FAILED." << std::endl;
        }
        else
        {
            std::cout << "Check Spacing [" << reader1->GetOutput()->GetSpacing() << "] with reference ["
                    << reader2->GetOutput()->GetSpacing() << "] -> passed." << std::endl;
        }
    }
    if (l_CheckSize == true)
    {
        if (reader1->GetOutput()->GetLargestPossibleRegion().GetSize() != reader2->GetOutput()->GetLargestPossibleRegion().GetSize())
        {
            egal = false;
            std::cout << "Check Size [" << reader1->GetOutput()->GetLargestPossibleRegion().GetSize() << "] with reference ["
                    << reader2->GetOutput()->GetLargestPossibleRegion().GetSize() << "] -> FAILED." << std::endl;
        }
        else
        {
            std::cout << "Check Size [" << reader1->GetOutput()->GetLargestPossibleRegion().GetSize() << "] with reference ["
                    << reader2->GetOutput()->GetLargestPossibleRegion().GetSize() << "] -> passed." << std::endl;
        }
    }
    if (egal == false)
    {
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
