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
 * $Id$
 *                                                                                                          *
 ************************************************************************************************************/

#include "otbImage.h"
#include "otbVectorImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "otbCommandLineArgumentParser.h"
#include "itkMultiThreader.h"


#include "vnsMacro.h"
#include "vnsGRIBImageIOFactory.h"
#include "itkObjectFactoryBase.h"
#include "itkMetaImageIOFactory.h"

template<typename ImageType>
    void
    copy_image(const std::string & inputFilename, const std::string & outputFilename, const unsigned int mode,
            const unsigned long nbtile)
    {
        typedef otb::ImageFileReader<ImageType> ReaderType;
        typedef otb::ImageFileWriter<ImageType> WriterType;

        typename ReaderType::Pointer reader = ReaderType::New();
        typename WriterType::Pointer writer = WriterType::New();

        reader->SetFileName(inputFilename);

        writer->SetFileName(outputFilename);
        writer->SetInput(reader->GetOutput());
        switch (mode)
        {
        case vns::NUMBER_OF_LINES_STRIPPED_STREAMING:
            std::cout << "Mode division: SetNumberOfLinesStrippedStreaming" << std::endl;
            writer->SetNumberOfLinesStrippedStreaming(nbtile);
            break;
        case vns::NUMBER_OF_DIVISIONS_STRIPPED_STREAMING:
            std::cout << "Mode division: SetNumberOfDivisionsStrippedStreaming" << std::endl;
            writer->SetNumberOfDivisionsStrippedStreaming(nbtile);
            break;
        case vns::NUMBER_OF_DIVISIONS_TILED_STREAMING:
            std::cout << "Mode division: SetNumberOfDivisionsTiledStreaming" << std::endl;
            writer->SetNumberOfDivisionsTiledStreaming(nbtile);
            break;
        case vns::AUTOMATIC_STRIPPED_STREAMING:
            std::cout << "Mode division: SetAutomaticStrippedStreaming" << std::endl;
            writer->SetAutomaticStrippedStreaming();
            break;
        case vns::AUTOMATIC_TILED_STREAMING:
            std::cout << "Mode division:SetAutomaticTiledStreaming " << std::endl;
            writer->SetAutomaticTiledStreaming();
            break;
        case vns::AUTOMATIC_ADAPTATIVE_STREAMING:
            std::cout << "Mode division: SetAutomaticAdaptativeStreaming" << std::endl;
            writer->SetAutomaticAdaptativeStreaming();
            break;
        }
        writer->Update();

    }

int
otbImageFileReaderTest(int argc, char* argv[])
{
    // -------------------------------------------------------------------------------------------------------
    // Register the MACCS ImageIO
    itk::ObjectFactoryBase::RegisterFactory( vns::GRIBImageIOFactory::New() );
    itk::ObjectFactoryBase::RegisterFactory( itk::MetaImageIOFactory::New() );


    // Parse command line parameters
    typedef otb::CommandLineArgumentParser ParserType;
    ParserType::Pointer parser = ParserType::New();

    parser->SetProgramDescription("Copy image.");
    parser->AddInputImage();
    parser->AddOutputImage();
    parser->AddOption("--ImageType", "ImageType: VI(VectoreImage)_D(dboule)_2D(Dimensions)", "-i", 1, true);
    parser->AddOption("--ModeStreamDivisions", "ModeStreamDivisions", "-m", 1, true);
    parser->AddOption("--TilingStreamDivisions", "TilingStreamDivisions", "-t", 1, true);
    parser->AddOption("--NbThreads", "NbThreads", "-n", 1, true);

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

    const std::string TYPE = parseResult->GetParameterString("--ImageType");
    const unsigned long nbtile = parseResult->GetParameterULong("--TilingStreamDivisions");
    const int nbthreads = parseResult->GetParameterInt("--NbThreads");
    const unsigned int mode = parseResult->GetParameterULong("--ModeStreamDivisions");
    std::cout << "parseResult->GetInputImage() :" << parseResult->GetInputImage() << std::endl;
    std::cout << "parseResult->GetOutputImage() :" << parseResult->GetOutputImage() << std::endl;
    std::cout << "TYPE           : " << TYPE << std::endl;
    std::cout << "nbtile         : " << nbtile << std::endl;
    std::cout << "nbthreads      : " << nbthreads << std::endl;
    std::cout << "Input filename : " << parseResult->GetInputImage().c_str() << std::endl;
    std::cout << "Output filename: " << parseResult->GetOutputImage().c_str() << std::endl;

    itk::MultiThreader::SetGlobalDefaultNumberOfThreads(nbthreads);
    const int checkNumberOfThreads = itk::MultiThreader::GetGlobalDefaultNumberOfThreads();
    if (nbthreads != checkNumberOfThreads)
    {
        std::cout << "Impossible to affect "<<nbthreads<<" number of threads for the test. The number of threads use is "<<checkNumberOfThreads<<" !";
        return EXIT_FAILURE;
    }

    if (TYPE == "VI_D_2D")
        copy_image<otb::VectorImage<double, 2> >(parseResult->GetInputImage().c_str(), parseResult->GetOutputImage().c_str(), mode, nbtile);
    else if (TYPE == "VI_F_2D")
        copy_image<otb::VectorImage<float, 2> >(parseResult->GetInputImage().c_str(), parseResult->GetOutputImage().c_str(), mode, nbtile);
    else if (TYPE == "VI_S_2D")
        copy_image<otb::VectorImage<short, 2> >(parseResult->GetInputImage().c_str(), parseResult->GetOutputImage().c_str(), mode, nbtile);
    else if (TYPE == "VI_UC_2D")
        copy_image<otb::VectorImage<unsigned char, 2> >(parseResult->GetInputImage().c_str(), parseResult->GetOutputImage().c_str(), mode,
                nbtile);
    else if (TYPE == "I_D_2D")
        copy_image<otb::Image<double, 2> >(parseResult->GetInputImage().c_str(), parseResult->GetOutputImage().c_str(), mode, nbtile);
    else if (TYPE == "I_UC_2D")
        copy_image<otb::Image<unsigned char, 2> >(parseResult->GetInputImage().c_str(), parseResult->GetOutputImage().c_str(), mode, nbtile);

    return EXIT_SUCCESS;
}
