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
 * VERSION : 1-0-0 : <TypeFT> : <NumFT> : 7 juil. 2010 : Creation                                                           
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id$
 *                                                                                                          *
 ************************************************************************************************************/
#include "vnsGapFillingImageFilter.h"
#include "vnsLoggers.h"
#include "otbImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "otbMultiplyByScalarImageFilter.h"

int
vnsGapFillingImageFilterTest(int argc, char * argv[])
{
    if (argc != 10)
    {
        return EXIT_FAILURE;
    }

    // 1 images
    const char * InputImageFileName = argv[1];
    // output
    const char * OutputFilledImageFileName =  argv[2];
    const char * outputMaskFileName =  argv[3];

    double lMeanValue = static_cast< double> (atof(argv[4]));
    double lDefaultAOT = static_cast< double> (atof(argv[5]));
    unsigned int lAOTHasValidPixel = static_cast< unsigned int> (atoi(argv[6]));
    unsigned int lInitWindowRadius = static_cast< unsigned int> (atoi(argv[7]));
    unsigned int lMaxWindowRadius = static_cast< unsigned int> (atoi(argv[8]));
    double lNoData = static_cast< double> (atof(argv[9]));

    // Configure Logger
    vns::Loggers::GetInstance()->Initialize(argv[0]);
    vns::Loggers::GetInstance()->SetMinLevel(vns::LoggerBase::DEBUG);

    /** Pixel typedefs */
    typedef double ImagePixelType;
    typedef short MaskPixelType;

    /** Image typedefs */
    typedef otb::Image<ImagePixelType, 2> InputImageType;
    typedef otb::Image<ImagePixelType, 2> OutputImageType;
    typedef otb::Image<MaskPixelType, 2> OutputMaskType;
    typedef vns::GapFillingImageFilter<InputImageType,OutputImageType,OutputMaskType> FilterType;
    /** Readers typedef */
    typedef otb::ImageFileReader<InputImageType>        ReaderType;
    typedef ReaderType::Pointer                         ReaderPointerType;
    typedef otb::ImageFileWriter<OutputImageType>        WriterType;
    typedef WriterType::Pointer                         WriterPointerType;
    typedef otb::ImageFileWriter<OutputMaskType>        MaskWriterType;
    typedef MaskWriterType::Pointer                     MaskWriterPointerType;
    typedef otb::MultiplyByScalarImageFilter<InputImageType,InputImageType>  MultiplyFilterType;


    /** instantiating the filter */
    FilterType::Pointer lGapFillingImageFilter = FilterType::New();

    /** Instantiating the readers ... */
    ReaderPointerType imageReader = ReaderType::New();
    WriterPointerType imageWriter = WriterType::New();
    MaskWriterPointerType maskWriter = MaskWriterType::New();
    MultiplyFilterType::Pointer lMultiplyFilter = MultiplyFilterType::New();
    MultiplyFilterType::Pointer lMultiplyFilter2 = MultiplyFilterType::New();

    imageReader->SetFileName(InputImageFileName);

    lMultiplyFilter->SetInput(imageReader->GetOutput());
    lMultiplyFilter->SetCoef(0.001);

    lGapFillingImageFilter->SetInputImage(lMultiplyFilter->GetOutput()); //lMultiplyFilter-> //imageReader
    lGapFillingImageFilter->SetMeanValue(lMeanValue);
    lGapFillingImageFilter->SetDefaultValue(lDefaultAOT);
    lGapFillingImageFilter->SetHasValidPixel(lAOTHasValidPixel);
    lGapFillingImageFilter->SetInitWindowRadius(lInitWindowRadius);
    lGapFillingImageFilter->SetMaxWindowRadius(lMaxWindowRadius);
    lGapFillingImageFilter->SetNoData(lNoData);

    lMultiplyFilter2->SetInput(lGapFillingImageFilter->GetOutputFilledImage());
    lMultiplyFilter2->SetCoef(1000.);

    imageWriter->SetInput(lMultiplyFilter2->GetOutput());
    imageWriter->SetFileName(OutputFilledImageFileName);
    imageWriter->Update();

    maskWriter->SetInput(lGapFillingImageFilter->GetOutputMask());
    maskWriter->SetFileName(outputMaskFileName);
//    maskWriter->Update();


    return EXIT_SUCCESS;
}
