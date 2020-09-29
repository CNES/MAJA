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
 * VERSION : 1-0-0 : <TypeFT> : <NumFT> : 22 juil. 2010 : Creation                                                           
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id$
 *                                                                                                          *
 ************************************************************************************************************/

#include "vnsLoggers.h"
#include "vnsSRTMWaterMaskImproveImageFilter.h"
#include "otbImage.h"
#include "otbVectorImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "vnsConstantVectorImageSource.h"

int vnsSRTMWaterMaskImproveImageFilterTest(int argc, char * argv [])
{
    if(argc!=13)
    {
        std::cerr << argv[0] << "<IPPWA filename> <IPTWA filename> "
                "<L2inPWA filename> <L2inTWA filename> <SRTM water mask filename> "
                "<L2WAS output filename> <L2outPWA filename> <L2outTWA filename> "
                "<Init Mode option> <SRTM percent rate> "
                "<Minimum percent possible water> <Number of water mask>" << std::endl;
        return EXIT_FAILURE;
    }
    // Configure Logger
    vns::Loggers::GetInstance()->Initialize(argv[0]);
    vns::Loggers::GetInstance()->SetMinLevel(vns::LoggerBase::DEBUG);

    const char * l_IPPWAFileName = argv[1];
    const char * l_IPTWAFileName = argv[2];
    const char * l_L2inPWAFileName = argv[3];
    const char * l_L2inTWAFileName = argv[4];
    const char * l_SRTMWATFileName = argv[5];
    const char * l_L2WASFileName = argv[6];
    const char * l_L2outPWAFileName = argv[7];
    const char * l_L2outTWAFileName = argv[8];

    bool l_InitMode = static_cast<bool>(atoi(argv[9]));
    unsigned short l_SRTMPercent = atoi(argv[10]);
    unsigned short l_MinPercentPossibleWater = atoi(argv[11]);
    unsigned int l_WaterMaskNumber = atoi(argv[12]);

    const unsigned int Dimension = 2;

    unsigned int l_NbBand = 1;
    unsigned int l_NLines = 334;
    unsigned int l_NCols = 375;

    /** Pixel typedefs */
    typedef unsigned char InputPixelType;
    typedef unsigned char SRTMPixelType;
    typedef unsigned char OutputPixelType;
    /** Image typedefs */
    typedef otb::Image<InputPixelType,Dimension> InputMaskType;
    typedef otb::VectorImage<InputPixelType,Dimension> VectorMaskType;
    typedef otb::Image<SRTMPixelType,Dimension> SRTMMaskType;
    typedef otb::Image<OutputPixelType,Dimension> OutputImageType;

    typedef vns::SRTMWaterMaskImproveImageFilter<InputMaskType,
                                                 VectorMaskType,
                                                 SRTMMaskType,
                                                 OutputImageType>  FilterType;

    typedef vns::ConstantVectorImageSource<VectorMaskType>         ConstantVectorFilterType;

    /** instantiating the filter */
    typedef otb::ImageFileReader<InputMaskType>          MaskReaderType;
    typedef otb::ImageFileReader<VectorMaskType>         VectorReaderType;
    typedef otb::ImageFileReader<SRTMMaskType>           SRTMReaderType;
    typedef otb::ImageFileWriter<OutputImageType>        WriterType;
    typedef otb::ImageFileWriter<VectorMaskType>          VectorWriterType;

    VectorMaskType::PixelType l_Pixel;


    /** instantiating the filter */
    FilterType::Pointer l_Filter = FilterType::New();
    MaskReaderType::Pointer l_IPPWAReader  = MaskReaderType::New();
    MaskReaderType::Pointer l_IPTWAReader  = MaskReaderType::New();
    VectorReaderType::Pointer l_L2inPWAReader  = VectorReaderType::New();
    VectorReaderType::Pointer l_L2inTWAReader  = VectorReaderType::New();
    SRTMReaderType::Pointer l_SRTMReader = SRTMReaderType::New();
    WriterType::Pointer l_L2WASWriter = WriterType::New();
    VectorWriterType::Pointer l_L2outPWAWriter = VectorWriterType::New();
    VectorWriterType::Pointer l_L2outTWAWriter = VectorWriterType::New();


    l_IPPWAReader->SetFileName(l_IPPWAFileName);
    l_IPTWAReader->SetFileName(l_IPTWAFileName);
    l_SRTMReader->SetFileName(l_SRTMWATFileName);

    l_Filter->SetIPPWAInput(l_IPPWAReader->GetOutput());
    l_Filter->SetIPTWAInput(l_IPTWAReader->GetOutput());
    l_Filter->SetSRTMWATInput(l_SRTMReader->GetOutput());

    // --------------------------------------------------
    // INIT MODE
    // --------------------------------------------------
    // Generate L2inPWA and L2inTWA constant images
    if (l_InitMode == true)
    {
        ConstantVectorFilterType::Pointer l_ConstantFilter = ConstantVectorFilterType::New();

        l_IPPWAReader->GenerateOutputInformation();
        VectorMaskType::PointType l_IPPWAOrigin;
        l_IPPWAOrigin = l_IPPWAReader->GetOutput()->GetOrigin();

        VectorMaskType::SizeValueType l_Size[2];
        l_Size[0] = l_NCols;
        l_Size[1] = l_NLines;
        l_ConstantFilter->SetSize(l_Size);

        VectorMaskType::SpacingValueType l_Spacing[2] = {96., -96.};
        l_ConstantFilter->SetSpacing( l_Spacing );

        l_ConstantFilter->SetOrigin( static_cast<VectorMaskType::PointType>(l_IPPWAOrigin) );

        l_Pixel.SetSize(l_NbBand);
        for (unsigned int i=0 ; i<l_NbBand ; i++)
        {
            l_Pixel.SetElement(i,0);
        }
        l_ConstantFilter->SetConstantValue(l_Pixel);

        l_ConstantFilter->Update();

        VectorMaskType::RegionType l_Region = l_ConstantFilter->GetOutput()->GetLargestPossibleRegion();

        std::cout<<l_Region<<std::endl;


 //       l_Filter->SetL2inPWAInput(l_ConstantFilter->GetOutput());
 //       l_Filter->SetL2inTWAInput(l_ConstantFilter->GetOutput());
    }
    else
    {
        l_L2inPWAReader->SetFileName(l_L2inPWAFileName);
        l_L2inTWAReader->SetFileName(l_L2inTWAFileName);
        l_Filter->SetL2inPWAInput(l_L2inPWAReader->GetOutput());
        l_Filter->SetL2inTWAInput(l_L2inTWAReader->GetOutput());
    }


    l_Filter->SetInitMode(l_InitMode);
    l_Filter->SetSRTMPercent(static_cast<double>(l_SRTMPercent));
    l_Filter->SetMinPercentPossibleWater(static_cast<double>(l_MinPercentPossibleWater));
    l_Filter->SetWaterMaskNumber(l_WaterMaskNumber);

    l_Filter->SetNumberOfThreads(1);


    // final water output mask
    l_L2WASWriter->SetFileName(l_L2WASFileName);
    l_L2WASWriter->SetInput(l_Filter->GetL2WASOutput());

    l_L2WASWriter->Update();

    // possible water flag
    l_L2outPWAWriter->SetFileName(l_L2outPWAFileName);
    l_L2outPWAWriter->SetInput(l_Filter->GetL2PWAOutput());

    l_L2outPWAWriter->Update();

    // tested pixel flag
    l_L2outTWAWriter->SetFileName(l_L2outTWAFileName);
    l_L2outTWAWriter->SetInput(l_Filter->GetL2TWAOutput());

    l_L2outTWAWriter->Update();


    return EXIT_SUCCESS;
}
