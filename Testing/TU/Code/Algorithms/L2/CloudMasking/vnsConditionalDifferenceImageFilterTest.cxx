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
 * VERSION : 1-0-0 : <TypeFT> : <NumFT> : 8 juin 2010 : Creation                                                           
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id$
 *                                                                                                          *
 ************************************************************************************************************/

#include "vnsLoggers.h"
#include "vnsConditionalDifferenceImageFilter.h"
#include "otbImageFileReader.h"
#include "vnsMacro.h"

#include "otbImage.h"
#include "otbVectorImage.h"
#include "otbImageFileWriter.h"
#include "otbMultiplyByScalarImageFilter.h"

int
vnsConditionalDifferenceImageFilterTest(int argc, char * argv[])
{
    if (argc != 9)
    {
        std::cerr << argv[0] << "<IPTOCR input filename> <L2inRCR input filename> <L2CLDSub mask filename> "
                "<IPEDG mask filename> <L2inWAS filename> <L2inNDT mask filename> "
                "<output filename> <No_Data value>" << std::endl;
        return EXIT_FAILURE;
    }
    // Configure Logger
    vns::Loggers::GetInstance()->Initialize(argv[0]);
    vns::Loggers::GetInstance()->SetMinLevel(vns::LoggerBase::DEBUG);

    const unsigned int Dimension = 2;
    const char * l_IPTOCRFileName = argv[1];
    const char * l_L2inRCRFileName = argv[2];
    const char * l_L2CLDSubFileName = argv[3];
    const char * l_IPEDGFileName = argv[4];
    const char * l_L2inWASFileName = argv[5];
    const char * l_L2inNDTFileName = argv[6];
    const char * l_outputFileName = argv[7];

    /** Pixel typedefs */
    typedef double InputPixelType;
    typedef unsigned char MaskPixelType;
    typedef double OutputPixelType;

    /** Parameters definition */
    vns::RealNoDataType l_NoData = static_cast<vns::RealNoDataType>(atof(argv[8]));

    /** Image typedefs */
    typedef otb::VectorImage<InputPixelType, Dimension> InputImageType;
    typedef otb::Image<MaskPixelType, Dimension> InputMaskType;
    typedef otb::Image<OutputPixelType, Dimension> OutputImageType;
    typedef otb::ImageFileReader<InputImageType> ReaderType;
    typedef otb::ImageFileReader<InputMaskType> InputMaskReaderType;
    typedef otb::ImageFileWriter<OutputImageType> WriterType;

    typedef vns::ConditionalDifferenceImageFilter<InputImageType, InputMaskType, OutputImageType> FilterType;
    typedef otb::MultiplyByScalarImageFilter<InputImageType, InputImageType> MultiplyFilterType;

    /** instantiating the filter */
    MultiplyFilterType::Pointer l_MultiplyFilter1 = MultiplyFilterType::New();
    MultiplyFilterType::Pointer l_MultiplyFilter2 = MultiplyFilterType::New();
    FilterType::Pointer l_Filter = FilterType::New();
    ReaderType::Pointer l_IPTOCRReader = ReaderType::New();
    ReaderType::Pointer l_L2inRCRReader = ReaderType::New();
    InputMaskReaderType::Pointer l_L2CLDSubReader = InputMaskReaderType::New();
    InputMaskReaderType::Pointer l_IPEDGReader = InputMaskReaderType::New();
    InputMaskReaderType::Pointer l_L2inWASReader = InputMaskReaderType::New();
    InputMaskReaderType::Pointer l_L2inNDTReader = InputMaskReaderType::New();

    WriterType::Pointer l_Writer = WriterType::New();

    l_IPTOCRReader->SetFileName(l_IPTOCRFileName);
    l_L2inRCRReader->SetFileName(l_L2inRCRFileName);
    l_L2CLDSubReader->SetFileName(l_L2CLDSubFileName);
    l_IPEDGReader->SetFileName(l_IPEDGFileName);
    l_L2inNDTReader->SetFileName(l_L2inNDTFileName);
    l_L2inWASReader->SetFileName(l_L2inWASFileName);
    l_Writer->SetFileName(l_outputFileName);

    l_MultiplyFilter1->SetInput(l_IPTOCRReader->GetOutput());
    l_MultiplyFilter1->SetCoef(0.001);
    l_Filter->SetIPTOCRInput(l_MultiplyFilter1->GetOutput());

    l_MultiplyFilter2->SetInput(l_L2inRCRReader->GetOutput());
    l_MultiplyFilter2->SetCoef(0.001);
    l_Filter->SetL2inRCRInput(l_MultiplyFilter2->GetOutput());

    l_Filter->SetNoData(l_NoData);

    l_Filter->SetL2CLDSubInput(l_L2CLDSubReader->GetOutput());
    l_Filter->SetIPEDGInput(l_IPEDGReader->GetOutput());
    l_Filter->SetL2inWASInput(l_L2inWASReader->GetOutput());
    l_Filter->SetL2inNDTInput(l_L2inNDTReader->GetOutput());

    l_Writer->SetInput(l_Filter->GetOutput());
    l_Writer->Update();

    return EXIT_SUCCESS;
}
