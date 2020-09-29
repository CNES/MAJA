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
#include "vnsMultiplyByVectorFunctor.h"

#include "otbVectorImage.h"
#include "otbImageFileWriter.h"

#include "otbFunctorImageFilter.h"

int
vnsMultiplyByVectorFunctorTest(int argc, char * argv[])
{
    if (argc < 3)
    {
        std::cerr << argv[0] << " <input filename> <output filename> <Coefs>" << std::endl;
        return EXIT_FAILURE;
    }

    unsigned int cpt(1);

    // Configure Logger
    vns::Loggers::GetInstance()->Initialize(argv[0]);
    vns::Loggers::GetInstance()->SetMinLevel(vns::LoggerBase::DEBUG);

    const unsigned int Dimension = 2;
    const char * inputFileName = argv[cpt++];
    const char * outputFileName = argv[cpt++];

    /** Pixel typedefs */
    typedef double PixelType;

    /** Image typedefs */
    typedef otb::VectorImage<PixelType, Dimension> InputImageType;
    typedef otb::VectorImage<PixelType, Dimension> OutputImageType;
    typedef otb::ImageFileReader<InputImageType> ReaderType;
    typedef otb::ImageFileWriter<OutputImageType> WriterType;

    typedef vns::Functor::MultiplyByVectorFunctor<InputImageType::PixelType, OutputImageType::PixelType> FunctorType;

    typedef otb::FunctorImageFilter<FunctorType> ImageFilterType;

    /** Parameters definition */
    typedef FunctorType::CoefsVectorType CoefsVectorType;

    /** instantiating the filter */
    ImageFilterType::Pointer filter = ImageFilterType::New();
    ReaderType::Pointer reader = ReaderType::New();
    WriterType::Pointer writer = WriterType::New();
    reader->SetFileName(inputFileName);
    writer->SetFileName(outputFileName);

    // Get image information ( number of bands, dimension, ...)
    reader->GenerateOutputInformation();

    const unsigned short nbBands = reader->GetOutput()->GetNumberOfComponentsPerPixel();

    std::cout << nbBands << std::endl;
    CoefsVectorType coefs;

    for (unsigned short bd = 0; bd < nbBands; bd++)
    {
        coefs.push_back(static_cast<double>(atof(argv[cpt++])));
    }

    // Set the size of the output image ( because the Unary Functor calls GetOutputSize() )
    filter->GetModifiableFunctor().SetOutputSize(nbBands);

    filter->GetModifiableFunctor().SetCoefs(coefs);
    filter->SetInput(reader->GetOutput());
    writer->SetInput(filter->GetOutput());

    writer->Update();

    return EXIT_SUCCESS;
}
