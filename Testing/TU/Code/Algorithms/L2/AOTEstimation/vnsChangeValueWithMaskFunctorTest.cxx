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
 * VERSION : 1-0-0 : <TypeFT> : <NumFT> : 26 mars 2010 : Creation                                                           
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id$
 *                                                                                                          *
 ************************************************************************************************************/

#include "vnsLoggers.h"
#include "vnsChangeValueWithMaskFunctor.h"
#include "otbImageFileReader.h"

#include "otbImage.h"
#include "otbVectorImage.h"
#include "otbImageFileWriter.h"

#include "itkBinaryFunctorImageFilter.h"

int
vnsChangeValueWithMaskFunctorTest(int argc, char * argv[])
{
    if (argc != 6)
    {
        std::cerr << argv[0] << " <input filename> <mask filename> <output filename> <background value> <default value>" << std::endl;
        return EXIT_FAILURE;
    }
    // Configure Logger
    vns::Loggers::GetInstance()->Initialize(argv[0]);
    vns::Loggers::GetInstance()->SetMinLevel(vns::LoggerBase::DEBUG);

    const unsigned int Dimension = 2;
    const char * inputFileName = argv[1];
    const char * maskFileName = argv[2];
    const char * outputFileName = argv[3];

    /** Pixel typedefs */
    typedef double InputPixelType;
    typedef unsigned char MaskPixelType;
    typedef double OutputPixelType;

    /** Parameters definition */
    MaskPixelType lBackgroundValue = static_cast<unsigned char>(atoi(argv[4]));
    OutputPixelType lDefaultValue = static_cast<double>(atoi(argv[5]));

    std::cout << "InputImage=" << inputFileName << std::endl;
    std::cout << "MaskImage=" << maskFileName << std::endl;
    std::cout << "OutputImage=" << outputFileName << std::endl;
    std::cout << "BackgroundValue=" << lBackgroundValue << std::endl;
    std::cout << "DefaultValue=" << lDefaultValue << std::endl;

    /** Image typedefs */
    typedef otb::Image<InputPixelType, Dimension> InputImageType;
    typedef otb::Image<MaskPixelType, Dimension> MaskImageType;
    typedef otb::Image<OutputPixelType, Dimension> OutputImageType;
    typedef otb::ImageFileReader<InputImageType> ReaderType;
    typedef otb::ImageFileReader<MaskImageType> MaskReaderType;
    typedef otb::ImageFileWriter<OutputImageType> WriterType;

    typedef vns::Functor::ChangeValueWithMaskFunctor<InputImageType::PixelType, MaskPixelType, OutputImageType::PixelType> FunctorType;

    typedef itk::BinaryFunctorImageFilter<InputImageType, MaskImageType, OutputImageType, FunctorType> ImageFilterType;

    /** instantiating the filter */
    ReaderType::Pointer reader = ReaderType::New();
    MaskReaderType::Pointer maskReader = MaskReaderType::New();
    WriterType::Pointer writer = WriterType::New();
    ImageFilterType::Pointer filter = ImageFilterType::New();

    reader->SetFileName(inputFileName);
    maskReader->SetFileName(maskFileName);
    writer->SetFileName(outputFileName);

    filter->GetFunctor().SetBackgroundValue(lBackgroundValue);
    filter->GetFunctor().SetDefaultValue(lDefaultValue);

    filter->SetInput1(reader->GetOutput());
    filter->SetInput2(maskReader->GetOutput());
    writer->SetInput(filter->GetOutput());

    writer->Update();

    return EXIT_SUCCESS;
}
