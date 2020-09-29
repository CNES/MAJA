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
 * VERSION : 1-0-0 : <TypeFT> : <NumFT> : 7 juin 2010 : Creation                                                           
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id$
 *                                                                                                          *
 ************************************************************************************************************/

#include "vnsLoggers.h"
#include "vnsShadowVariationThresholdImageFilter.h"
#include "otbImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"

#include "vnsLoggers.h"
#include "vnsReflectanceVariationCorrelationImageFilter.h"
#include "otbImage.h"
#include "otbVectorImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "otbMultiplyByScalarImageFilter.h"
#include "vnsDate.h"

#include "itkBinaryDilateImageFilter.h"
#include "itkBinaryBallStructuringElement.h"

int vnsShadowVariationThresholdImageFilterTest(int argc, char * argv [])
{

    std::cout<<argc<<std::endl;

    if(argc!=15)
    {
        std::cerr << argv[0] << " <IPTOCR image filename> <L2inRCR image filename> <L2inSTO filename> <L2inPXD filename>  "
                "<IPEDG filename> <L2inNDT filename> "
                "<L2inWAS filename> <IPCLDOutShad filename> "
                "<IPCLDShadVar filename> "
                "<NoData value> <Red Band> "
                "<Year> <Month> <Day> <Forgetting duration> "
                "<Minimum shadow variation> <MinPixelCorrel> <CorrelBand> "
                "<CorrelThreshold> <Dilatation option>"<< std::endl;
        return EXIT_FAILURE;
    }
    // Configure Logger
    vns::Loggers::GetInstance()->Initialize(argv[0]);
    vns::Loggers::GetInstance()->SetMinLevel(vns::LoggerBase::DEBUG);

    const unsigned int Dimension = 2;
    const char * l_IPTOCRFileName  = argv[1];
    const char * l_L2inRCRFileName  = argv[2];
    const char * l_L2inSTOFileName  = argv[3];
    const char * l_L2inPXDFileName  = argv[4];
    const char * l_IPEDGFileName  = argv[5];
    const char * l_L2inNDTFileName  = argv[6];
    const char * l_L2inWASFileName  = argv[7];
    const char * l_IPCLDOutShadFileName  = argv[8];
    const char * l_IPCLDShadVarFileName = argv[9];

    short l_NoData = atoi(argv[10]);
    //unsigned int l_RedBand = atoi(argv[11])-1;

    int l_Year = atoi(argv[11]);
    int l_Month = atoi(argv[12]);
    int l_Day = atoi(argv[13]);
    //double l_ForgettingDuration = atof(argv[15]);
    //double l_MinShadowVariation = atof(argv[16]);
    //unsigned int l_MinPixelCorrel = atoi(argv[17]);
    //unsigned int l_CorrelBand = atoi(argv[18])-1;
    //unsigned short l_CorrelThreshold = atoi(argv[19]);
    unsigned short l_DilatationOption = atoi(argv[14]);
    
    //Compute input date in julian days
    vns::Tm inputDate;
    inputDate.tm_mday = l_Day;
    inputDate.tm_mon = l_Month;
    inputDate.tm_year = l_Year;
    inputDate.tm_hour = 0;
    inputDate.tm_min = 0;
    inputDate.tm_sec = 0;
    double l_JDay = vns::Date::GetJulianDayAsDouble(inputDate);

    std::cout << "l_JDay = " << l_JDay << std::endl;

    inputDate.tm_mday = 01;
    inputDate.tm_mon = 01;
    inputDate.tm_year = 1984;
    double l_JDayRef = vns::Date::GetJulianDayAsDouble(inputDate);

    std::cout<<"l_JDayRef = "<<l_JDayRef<<std::endl;

    l_JDay = l_JDay - l_JDayRef;

    std::cout<<"l_JDay = "<<l_JDay<<std::endl;

    /** Pixel typedefs */
    typedef double InputPixelType;
    typedef short DatePixelType;
    typedef unsigned char InputMaskPixelType;
    typedef double OutputPixelType;

    /** Image typedefs */
    typedef otb::/*Vector*/Image<InputPixelType,Dimension>      InputImageType;
    typedef otb::Image<InputMaskPixelType,Dimension>        InputMaskType;
    typedef otb::Image<DatePixelType,Dimension>             DateImageType;
    typedef otb::Image<OutputPixelType,Dimension>           OutputImageType;

    typedef otb::ImageFileReader<InputImageType>            ReaderType;
    typedef otb::ImageFileReader<InputMaskType>             InputMaskReaderType;
    typedef otb::ImageFileReader<DateImageType>             DateReaderType;
    typedef otb::ImageFileWriter<OutputImageType>           MaskWriterType;

    /** Filter typedefs */
    typedef vns::ShadowVariationThresholdImageFilter<InputImageType,InputMaskType,OutputImageType>  FilterType;
    typedef otb::MultiplyByScalarImageFilter<InputImageType,InputImageType>  MultiplyFilterType;

    typedef itk::BinaryBallStructuringElement<OutputPixelType, Dimension>             StructuringElementType;
    typedef itk::BinaryDilateImageFilter<OutputImageType, OutputImageType, StructuringElementType >  DilateFilterType;


    /** instantiating the filter */
    MultiplyFilterType::Pointer l_MultiplyFilter1 = MultiplyFilterType::New();
    MultiplyFilterType::Pointer l_MultiplyFilter2 = MultiplyFilterType::New();
    MultiplyFilterType::Pointer l_MultiplyFilter3 = MultiplyFilterType::New();
    FilterType::Pointer l_Filter = FilterType::New();
    ReaderType::Pointer l_IPTOCRReader  = ReaderType::New();
    ReaderType::Pointer l_L2inRCRReader  = ReaderType::New();
    ReaderType::Pointer l_L2inSTOReader  = ReaderType::New();
    InputMaskReaderType::Pointer l_IPEDGReader = InputMaskReaderType::New();
    InputMaskReaderType::Pointer l_L2inNDTReader = InputMaskReaderType::New();
    InputMaskReaderType::Pointer l_L2inWASReader = InputMaskReaderType::New();
    InputMaskReaderType::Pointer l_IPCLDOutShadReader = InputMaskReaderType::New();
    DateReaderType::Pointer l_L2inPXDReader  = DateReaderType::New();
    MaskWriterType::Pointer l_IPCLDShadVarWriter = MaskWriterType::New();

    DilateFilterType::Pointer l_BinaryDilateFilter = DilateFilterType::New();

    l_IPTOCRReader->SetFileName(l_IPTOCRFileName);
    l_L2inRCRReader->SetFileName(l_L2inRCRFileName);
    l_L2inSTOReader->SetFileName(l_L2inSTOFileName);
    l_L2inPXDReader->SetFileName(l_L2inPXDFileName);
    l_IPEDGReader->SetFileName(l_IPEDGFileName);
    l_L2inNDTReader->SetFileName(l_L2inNDTFileName);
    l_L2inWASReader->SetFileName(l_L2inWASFileName);
    l_IPCLDOutShadReader->SetFileName(l_IPCLDOutShadFileName);
    l_IPCLDShadVarWriter->SetFileName(l_IPCLDShadVarFileName);

    l_MultiplyFilter1->SetInput(l_IPTOCRReader->GetOutput());
    l_MultiplyFilter1->SetCoef(0.001);
//    l_Filter->SetIPTOCRInput(l_MultiplyFilter1->GetOutput());
    l_Filter->SetReflRatio(l_MultiplyFilter1->GetOutput());

    l_MultiplyFilter2->SetInput(l_L2inRCRReader->GetOutput());
    l_MultiplyFilter2->SetCoef(0.001);
//    l_Filter->SetL2inRCRInput(l_MultiplyFilter2->GetOutput());

    l_MultiplyFilter3->SetInput(l_L2inSTOReader->GetOutput());
    l_MultiplyFilter3->SetCoef(0.001);
//    l_Filter->SetL2inSTOInput(l_MultiplyFilter3->GetOutput());



	l_L2inSTOReader->UpdateOutputInformation();
	std::vector<double> l_STOListOfJDates;
	l_STOListOfJDates.push_back(l_JDay);
	for(unsigned int iii=0;iii<l_L2inSTOReader->GetOutput()->GetNumberOfComponentsPerPixel()-1;iii++)
	{
		l_STOListOfJDates.push_back(0.0);
	}
	//l_Filter->SetSTOListOfDates(l_STOListOfJDates);


//    l_Filter->SetL2inPXDInput(l_L2inPXDReader->GetOutput());
//    l_Filter->SetIPEDGInput(l_IPEDGReader->GetOutput());
//    l_Filter->SetL2inNDTInput(l_L2inNDTReader->GetOutput());
//    l_Filter->SetL2inWASInput(l_L2inWASReader->GetOutput());
    l_Filter->SetIPCLDOutShadInput(l_IPCLDOutShadReader->GetOutput());

    l_Filter->SetNoData(l_NoData);
//    l_Filter->SetRedBand(l_RedBand);
//    l_Filter->SetCorrelBand(l_CorrelBand);
    l_Filter->SetThresholdValue(0.); // TODO : set value
//    l_Filter->SetRedBandTOCR(l_RedBand);
//    l_Filter->SetRedBandRCR(l_RedBand);
//    l_Filter->SetCorrelBandTOCR(l_CorrelBand);
//    l_Filter->SetCorrelBandRCR(l_CorrelBand);


//    l_Filter->SetJDay(l_JDay);
//    l_Filter->SetForgettingDuration(l_ForgettingDuration);
//    l_Filter->SetMinShadowVariation(l_MinShadowVariation);
//    l_Filter->SetMinPixelCorrelation(l_MinPixelCorrel);
//    l_Filter->SetCorrelThreshold(static_cast<double>(l_CorrelThreshold));
//    l_Filter->SetNCorrel(3);

    std::cout<<l_Filter<<std::endl;


    if (l_DilatationOption == 1)
    {
        // ---------------
        // With dilatation
        // ---------------


        StructuringElementType  l_StructuringElement;
        l_StructuringElement.SetRadius( 1 );  // 5x5 structuring element
        l_StructuringElement.CreateStructuringElement();

        l_BinaryDilateFilter->SetKernel( l_StructuringElement );
        l_BinaryDilateFilter->SetInput(l_Filter->GetIPCLDShadVarOutput());
        l_BinaryDilateFilter->SetDilateValue( 1 );

        l_IPCLDShadVarWriter->SetInput(l_BinaryDilateFilter->GetOutput());

    }
    else
    {
        // ------------------
        // Without dilatation
        // ------------------
        l_IPCLDShadVarWriter->SetInput(l_Filter->GetIPCLDShadVarOutput());
    }

    l_IPCLDShadVarWriter->Update();


    return EXIT_SUCCESS;
}
