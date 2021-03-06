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
 * VERSION : 1-0-0 : <TypeFT> : <NumFT> : 23 avr. 2010 : Creation                                                           
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id$
 *                                                                                                          *
 ************************************************************************************************************/
#include "vnsLoggers.h"
#include "otbImageFileReader.h"
#include "vnsTOACorrectionImageFunctor.h"

#include "otbVectorImage.h"
#include "otbImage.h"
#include "otbImageFileWriter.h"

#include "itkTernaryFunctorImageFilter.h"

int
vnsTOACorrectionFunctorTest(int argc, char * argv[])
{
    if (argc != 6)
    {
        std::cerr << argv[0]
                << " <input TOA filename> <input DTM filename> <input Water vapour filename> <input ozone amount> <output filename>"
                << std::endl;
        return EXIT_FAILURE;
    }
    // Configure Logger
    vns::Loggers::GetInstance()->Initialize(argv[0]);
    vns::Loggers::GetInstance()->SetMinLevel(vns::LoggerBase::DEBUG);

    const unsigned int Dimension = 2;
    const char * inputTOAFileName = argv[1];
    const char * inputMNTFileName = argv[2];
    const char * inputWaterFileName = argv[3];
    const char * o3Amount = atof(argv[4]);
    const char * outputFileName = argv[5];


    /** Pixel typedefs */
    typedef double PixelType;

    /** Image typedefs */
    typedef otb::VectorImage<PixelType>           VectorImageType;
    typedef otb::Image<PixelType>                 ImageType;
    typedef otb::ImageFileReader<ImageType>       ReaderType;
    typedef otb::ImageFileReader<VectorImageType> VectorReaderType;
    typedef otb::ImageFileWriter<VectorImageType> WriterType;

//    typedef vns::Functor::TOACorrectionFunctor<VectorImageType::PixelType,PixelType, PixelType,VectorImageType::PixelType> FunctorType;
    typedef vns::Functor::TOACorrectionImageFunctor<VectorImageType::PixelType,PixelType, PixelType,VectorImageType::PixelType> FunctorType;
    typedef itk::TernaryFunctorImageFilter<VectorImageType, ImageType, ImageType, VectorImageType, FunctorType>            ImageFilterType;

    /** instantiating the filter */
    ImageFilterType::Pointer  filter      = ImageFilterType::New();
    VectorReaderType::Pointer toaReader   = VectorReaderType::New();
    ReaderType::Pointer       pressReader = ReaderType::New();
    ReaderType::Pointer       waterReader = ReaderType::New();

    WriterType::Pointer      writer = WriterType::New();

    toaReader->SetFileName(inputTOAFileName);
    pressReader->SetFileName(inputMNTFileName);
    waterReader->SetFileName(inputWaterFileName);
    
    writer->SetFileName(outputFileName);

    filter->SetInput1(toaReader->GetOutput());
    filter->SetInput2(waterReader->GetOutput());
    filter->SetInput3(pressReader->GetOutput());
    //filter->SetOzoneAmount( o3Amount );
    filter->GetFilter.SetOzoneAmount( o3Amount );
    //filter->GetFilter.SetPhiS( 2. );
    //filter->GetFilter.SetPhiV( 5.);
    filter->GetFilter.SetThetaS( 12. );
    filter->GetFilter.SetThetaV( 22.);

    writer->SetInput(filter->GetOutput());

    writer->Update();

    return EXIT_SUCCESS;
}
