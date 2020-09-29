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
 * VERSION : 1-0-0 : <TypeFT> : <NumFT> : 30 mars 2010 : Creation                                                           
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id$
 *                                                                                                          *
 ************************************************************************************************************/

#include "vnsLoggers.h"
#include "otbImageFileReader.h"
#include "vnsThresholdVectorImageFunctor.h"

#include "otbVectorImage.h"
#include "otbImageFileWriter.h"

#include "otbFunctorImageFilter.h"

int
vnsThresholdVectorImageFunctorTest(int argc, char * argv[])
{
    if (argc != 6)
    {
        std::cerr << argv[0] << " <input filename> <output filename> <ThresholdValue> <InsideValue> <OutputValue>" << std::endl;
        return EXIT_FAILURE;
    }
    // Configure Logger
    vns::Loggers::GetInstance()->Initialize(argv[0]);
    vns::Loggers::GetInstance()->SetMinLevel(vns::LoggerBase::DEBUG);

    const unsigned int Dimension = 2;
    const char * inputFileName = argv[1];
    const char * outputFileName = argv[2];

    /** Pixel typedefs */
    typedef double PixelType;

    /** Image typedefs */
    typedef otb::VectorImage<PixelType, Dimension> InputImageType;
    typedef otb::VectorImage<PixelType, Dimension> OutputImageType;
    typedef otb::ImageFileReader<InputImageType> ReaderType;
    typedef otb::ImageFileWriter<OutputImageType> WriterType;

    typedef vns::Functor::ThresholdVectorImageFunctor<InputImageType::PixelType, OutputImageType::PixelType> FunctorType;

    typedef otb::FunctorImageFilter<FunctorType> ImageFilterType;

    /** Parameters definition */
    PixelType lThresholdValue = static_cast<double>(atoi(argv[3]));
    PixelType lInsideValue = static_cast<double>(atoi(argv[4]));
    PixelType lOutputValue = static_cast<double>(atoi(argv[5]));

    std::cout << "---> List of parameters : " << std::endl;
    std::cout << "InputImage=" << inputFileName << std::endl;
    std::cout << "OutputImage=" << outputFileName << std::endl;
    std::cout << "ThresholdValue=" << lThresholdValue << std::endl;
    std::cout << "InsideValue=" << lInsideValue << std::endl;
    std::cout << "OutputValue=" << lOutputValue << std::endl;

    /** instantiating the filter */
    ImageFilterType::Pointer filter = ImageFilterType::New();
    ReaderType::Pointer reader = ReaderType::New();
    WriterType::Pointer writer = WriterType::New();
    reader->SetFileName(inputFileName);
    writer->SetFileName(outputFileName);

    // Get image information ( number of bands, dimension, ...)
    reader->GenerateOutputInformation();
    // Set the size of the output image ( because the Unary Functor calls GetOutputSize() )
    filter->GetModifiableFunctor().SetOutputSize(reader->GetOutput()->GetNumberOfComponentsPerPixel());

    filter->GetModifiableFunctor().SetThresholdValue(lThresholdValue);
    filter->GetModifiableFunctor().SetInsideValue(lInsideValue);
    filter->GetModifiableFunctor().SetOutputValue(lOutputValue);

    filter->SetInput(reader->GetOutput());
    writer->SetInput(filter->GetOutput());

    writer->Update();

    return EXIT_SUCCESS;
}
