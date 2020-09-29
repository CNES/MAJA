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
 * VERSION : 1-0-0 : <TypeFT> : <NumFT> : 8 septembre 2010 : Creation
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id$
 *                                                                                                          *
 ************************************************************************************************************/
#include "vnsConvolutionVectorImageFilter.h"
#include "otbImage.h"
#include "otbVectorImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "vnsLoggers.h"
#include "itkMultiThreader.h"



int vnsConvolutionVectorImageFilterTest(int argc, char * argv [])
{

    if(argc!=5)
    {
        std::cerr << argv[0] << " <input filename> <mask filename> <output filename> <use the mask:0/1>" << std::endl;
        return EXIT_FAILURE;
    }

    // Configure Logger
    vns::Loggers::GetInstance()->Initialize(argv[0]);
    vns::Loggers::GetInstance()->SetMinLevel(vns::LoggerBase::DEBUG);


    const char * inputFileName  = argv[1];
    const char * maskFileName  = argv[2];
    const char * outputFileName = argv[3];
    double lNoData = static_cast<double> (atof(argv[4]));
// Pour test de PERF
//    itk::MultiThreader::SetGlobalDefaultNumberOfThreads(1);

    const unsigned int Dimension = 2;
    /** Pixel typedefs */
    typedef double InputPixelType;
    typedef double OutputPixelType;
    typedef short MaskPixelType;
    /** Image typedefs */
    typedef otb::VectorImage<InputPixelType,Dimension> InputImageType;
    typedef otb::VectorImage<OutputPixelType,Dimension> OutputImageType;
    typedef otb::Image<MaskPixelType,Dimension> MaskType;


    /** Reader typedef */
    typedef otb::ImageFileReader<InputImageType>    ReaderType;
    typedef ReaderType::Pointer                     ReaderPointerType;

    typedef otb::ImageFileReader<MaskType>          MaskReaderType;
    typedef MaskReaderType::Pointer                 MaskReaderPointerType;

    /* Writer typedef */
    typedef otb::ImageFileWriter<OutputImageType>   WriterType;
    typedef WriterType::Pointer                     WriterPointerType;

    /** Filter typedef */
    //typedef itk::ZeroFluxNeumannBoundaryCondition<InputImageType> BoundaryConditionType;
    typedef vns::ConvolutionVectorImageFilter<InputImageType,MaskType,OutputImageType>  ConvFilterType;

    /** Instantiating the filter */
    ConvFilterType::Pointer convFilter = ConvFilterType::New();
    ReaderPointerType reader           = ReaderType::New();
    MaskReaderPointerType maskReader   = MaskReaderType::New();
    WriterPointerType writer           = WriterType::New();

    reader->SetFileName(inputFileName);
    maskReader->SetFileName(maskFileName);
    writer->SetFileName(outputFileName);

    ConvFilterType::InputSizeType radius;
    radius[0]=10;
    radius[1]=10;

    ConvFilterType::ArrayType filterCoeffs;
    filterCoeffs.SetSize((2*radius[0]+1)*(2*radius[1]+1));
    filterCoeffs.Fill(1);

    convFilter->SetRadius(radius);
    convFilter->SetFilter(filterCoeffs);
    convFilter->SetInput(reader->GetOutput());
    convFilter->SetMask(maskReader->GetOutput());
    convFilter->SetNoData(lNoData);

    writer->SetInput(convFilter->GetOutput());
    writer->Update();

    return EXIT_SUCCESS;
}
