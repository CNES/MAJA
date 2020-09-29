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
#include "vnsPossibleWaterFlagImageFilter.h"
#include "otbImage.h"
#include "otbVectorImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "otbMultiplyByScalarImageFilter.h"

int vnsPossibleWaterFlagImageFilterTest(int argc, char * argv [])
{
    if(argc!=18)
    {
    	std::cerr << argv[0] << " <IPTOCR filename> <L2inRCR filename> "
    	        "<IPEDG filename> "
    	        "<L2CLDSub filename> <Cast shadow mask> "
    	        "<Slope Sub filename> "
    	        "<IPPWA output filename> <IPTWA output filename> "
    	        "<Init mode option> <Sunglint flag> "
    	        "<Red band> <NIR band> <Water NDVI Threshold> "
    	        "<Water reflectance variation threshold> <maximum sunglint reflectance> "
    	        "<Water red reflectance threshold> <Water slope threshold> " << std::endl;
        return EXIT_FAILURE;
    }

    // Configure Logger
    vns::Loggers::GetInstance()->Initialize(argv[0]);
    vns::Loggers::GetInstance()->SetMinLevel(vns::LoggerBase::DEBUG);

    unsigned int cpt(1);
    const char * l_IPTOCRFileName  = argv[cpt++];
    const char * l_L2inRCRFileName = argv[cpt++];
    const char * l_IPEDGFileName = argv[cpt++];
    const char * l_L2CLDSubFileName = argv[cpt++];
    const char * l_IPSHDSubFileName = argv[cpt++];
    const char * l_SlopeFileName = argv[cpt++];
    const char * l_IPPWAFileName = argv[cpt++];
    const char * l_IPTWAFileName = argv[cpt++];


    bool l_InitMode = static_cast<bool>(atoi(argv[cpt++]));
    bool l_SunglintFlag = static_cast<bool>(atoi(argv[cpt++]));

    unsigned int l_RedBand = atoi(argv[cpt++])-1;
    unsigned int l_NIRBand = atoi(argv[cpt++])-1;

    double l_WaterNDVIThreshold = atof(argv[cpt++]);
    double l_WaterReflectanceVariationThreshold = atof(argv[cpt++]);
    double l_MaximumSunglintReflectance = atof(argv[cpt++]);
    
    double l_WaterSlopeThreshold = atof(argv[cpt++]);
    double l_WaterRedReflectanceThreshold = atof(argv[cpt++]);

    const unsigned int Dimension = 2;
    /** Pixel typedefs */
    typedef double InputPixelType;
    typedef unsigned char InputMaskPixelType;
    typedef unsigned char OutputPixelType;
    /** Image typedefs */
    typedef otb::VectorImage<InputPixelType,Dimension>            InputImageType;
    typedef otb::Image<InputMaskPixelType,Dimension>              InputMaskType;
    typedef otb::Image<InputPixelType,Dimension>            	  ImageType;
    typedef otb::Image<OutputPixelType,Dimension>                 OutputImageType;

    typedef vns::PossibleWaterFlagImageFilter<InputImageType,
                                              InputMaskType,
                                              ImageType,
                                              OutputImageType>  FilterType;

    typedef otb::MultiplyByScalarImageFilter<InputImageType, InputImageType>   MultiplyFilterType;

    typedef otb::ImageFileReader<InputMaskType>          MaskReaderType;
    typedef otb::ImageFileReader<InputImageType>         InputImageReaderType;
    typedef otb::ImageFileReader<ImageType>         	 ImageReaderType;
    typedef otb::ImageFileWriter<OutputImageType>        WriterType;


    /** instantiating the filter */
    FilterType::Pointer l_Filter = FilterType::New();
    MultiplyFilterType::Pointer l_MultiplyFilter = MultiplyFilterType::New();
    MultiplyFilterType::Pointer l_MultiplyFilter2 = MultiplyFilterType::New();
    InputImageReaderType::Pointer l_IPTOCRReader  = InputImageReaderType::New();
    InputImageReaderType::Pointer l_L2inRCRReader  = InputImageReaderType::New();
    MaskReaderType::Pointer l_IPEDGReader  = MaskReaderType::New();
    MaskReaderType::Pointer l_L2CLDSubReader  = MaskReaderType::New();
    MaskReaderType::Pointer l_IPSHDSubReader  = MaskReaderType::New();
    ImageReaderType::Pointer l_SlopeReader  = ImageReaderType::New();
    WriterType::Pointer l_IPPWAWriter = WriterType::New();
    WriterType::Pointer l_IPTWAWriter = WriterType::New();

    l_IPTOCRReader->SetFileName(l_IPTOCRFileName);
    l_L2inRCRReader->SetFileName(l_L2inRCRFileName);
    l_IPEDGReader->SetFileName(l_IPEDGFileName);
    l_L2CLDSubReader->SetFileName(l_L2CLDSubFileName);
    l_IPSHDSubReader->SetFileName(l_IPSHDSubFileName);
    l_SlopeReader->SetFileName(l_SlopeFileName);

    /** Multiply by 0.001 inputs */
    l_MultiplyFilter->SetInput(l_IPTOCRReader->GetOutput());
    l_MultiplyFilter->SetCoef(0.001);

    l_MultiplyFilter2->SetInput(l_L2inRCRReader->GetOutput());
    l_MultiplyFilter2->SetCoef(0.001);


    l_Filter->SetIPTOCRInput(l_MultiplyFilter->GetOutput());
    l_Filter->SetIPEDGInput(l_IPEDGReader->GetOutput());
    l_Filter->SetL2CLDSubSummaryInput(l_L2CLDSubReader->GetOutput());
    l_Filter->SetIPSHDSubInput(l_IPSHDSubReader->GetOutput());
    l_Filter->SetSlopeSubInput(l_SlopeReader->GetOutput());

    if (l_InitMode == false)
    {
        l_Filter->SetL2inRCRInput(l_MultiplyFilter2->GetOutput());
    }

    l_Filter->SetInitMode(l_InitMode);
    l_Filter->SetSunglintFlag(l_SunglintFlag);
//    l_Filter->SetRedBand(l_RedBand);
//    l_Filter->SetNIRBand(l_NIRBand);
    l_Filter->SetRedBandTOCR(l_RedBand);
    l_Filter->SetNIRBandTOCR(l_NIRBand);
    l_Filter->SetNIRBandRCR(l_NIRBand);
    l_Filter->SetWaterNDVIThreshold(l_WaterNDVIThreshold);
    l_Filter->SetWaterSlopeThreshold(l_WaterSlopeThreshold);
    l_Filter->SetWaterRedReflectanceThreshold(l_WaterRedReflectanceThreshold);
    l_Filter->SetWaterReflectanceVariationThreshold(l_WaterReflectanceVariationThreshold);
    l_Filter->SetMaximumSunglintReflectance(l_MaximumSunglintReflectance);

    l_Filter->SetNumberOfThreads(1);

    std::cout<<l_Filter<<std::endl;

    // first output mask
    l_IPPWAWriter->SetFileName(l_IPPWAFileName);
    l_IPPWAWriter->SetInput(l_Filter->GetIPPWAOutput());

    std::cout<<l_IPPWAWriter<<std::endl;

    l_IPPWAWriter->Update();

    // second output mask
    l_IPTWAWriter->SetFileName(l_IPTWAFileName);
    l_IPTWAWriter->SetInput(l_Filter->GetIPTWAOutput());

    l_IPTWAWriter->Update();

    return EXIT_SUCCESS;
}
