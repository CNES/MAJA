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
 * VERSION : 1-0-0 : <TypeFT> : <NumFT> : 27 août 2010 : Creation                                                           
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id$
 *                                                                                                          *
 ************************************************************************************************************/

#include "vnsLoggers.h"
#include "vnsHighAOTFunctor.h"
#include "otbImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"

#include "itkBinaryFunctorImageFilter.h"

#include "otbMultiplyByScalarImageFilter.h"

int vnsHighAOTFunctorTest(int argc, char * argv [])
{
    if(argc!=7)
    {
    	std::cerr << argv[0] << " <IPAOTSub input filename> <input mask filename> <output filename> "
                                "<Coef sigma AOT> <AOT Mean> <AOT std>"<< std::endl;
        return EXIT_FAILURE;
    }
    // Configure Logger
    vns::Loggers::GetInstance()->Initialize(argv[0]);
    vns::Loggers::GetInstance()->SetMinLevel(vns::LoggerBase::DEBUG);

    const unsigned int Dimension = 2;
    const char * inputFileName  = argv[1];
    const char * maskFileName = argv[2];
    const char * outputFileName = argv[3];
    
    unsigned short l_CoefSigmaAOT = atoi(argv[4]);
    double l_AOTMean = atof(argv[5]);
    double l_AOTStd = atof(argv[6]);


    /** Pixel typedefs */
    typedef double InputPixelType;
    typedef unsigned char MaskPixelType;
    typedef unsigned char OutputPixelType;

    /** Image typedefs */
    typedef otb::Image<InputPixelType,Dimension>    InputImageType;
    typedef otb::Image<MaskPixelType,Dimension>     InputMaskType;
    typedef otb::Image<OutputPixelType,Dimension>   OutputImageType;

    typedef otb::ImageFileReader<InputImageType>    ReaderType;
    typedef otb::ImageFileReader<InputMaskType>    MaskReaderType;
    typedef otb::ImageFileWriter<OutputImageType>   WriterType;

    typedef vns::Functor::HighAOTFunctor<InputImageType::PixelType,
                                         InputMaskType::PixelType,
                                         OutputImageType::PixelType>  FunctorType;
    typedef itk::BinaryFunctorImageFilter<InputImageType, InputMaskType, OutputImageType, FunctorType> ImageFilterType;

    typedef otb::MultiplyByScalarImageFilter<InputImageType, InputImageType>  MultiplyFilterType;

    /** instantiating the filter */
     MultiplyFilterType::Pointer l_MultiplyFilter = MultiplyFilterType::New();
     ImageFilterType::Pointer l_Filter = ImageFilterType::New();
     ReaderType::Pointer l_Reader = ReaderType::New();
     MaskReaderType::Pointer l_MaskReader = MaskReaderType::New();
     WriterType::Pointer l_Writer = WriterType::New();

     l_Reader->SetFileName(inputFileName);
     l_MaskReader->SetFileName(maskFileName);
     l_Writer->SetFileName(outputFileName);

     l_MultiplyFilter->SetInput(l_Reader->GetOutput());
     l_MultiplyFilter->SetCoef(0.001);

     InputPixelType l_ThresholdValue;
     l_ThresholdValue = l_AOTMean + l_CoefSigmaAOT * l_AOTStd;

     std::cout<<"    T="<<l_ThresholdValue<<std::endl;

     l_Filter->GetFunctor().SetOutputSize(1);
     l_Filter->GetFunctor().SetHighAOTThresholdValue(l_ThresholdValue);
     l_Filter->GetFunctor().SetInsideValue(1);
     l_Filter->GetFunctor().SetOutputValue(0);

     l_Filter->SetInput1(l_MultiplyFilter->GetOutput());
     l_Filter->SetInput2(l_MaskReader->GetOutput());

     l_Writer->SetInput(l_Filter->GetOutput());

     l_Writer->Update();

    return EXIT_SUCCESS;
}
