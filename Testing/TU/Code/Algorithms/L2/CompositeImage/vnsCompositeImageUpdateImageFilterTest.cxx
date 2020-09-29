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
 * VERSION : 1-0-0 : <TypeFT> : <NumFT> : 6 sept. 2010 : Creation                                                           
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id$
 *                                                                                                          *
 ************************************************************************************************************/

#include "vnsLoggers.h"
#include "vnsCompositeImageUpdateImageFilter.h"

#include "otbImage.h"
#include "otbVectorImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "vnsDate.h"

#include "otbMultiplyByScalarImageFilter.h"
#include "vnsMacro.h"
#include "itkCastImageFilter.h"

int vnsCompositeImageUpdateImageFilterTest(int argc, char * argv [])
{
    if((argc!=34) && (argc!=28))
    {
    	std::cerr << argv[0] << "<Init mode option> <IPTOAC filename> <IPTOCR filename> "
    	        "<IPTOC filename> <IPEDG filename> <IPCLDSum filename> "
    	        "<IPSAT filename> <HighAOT mask filename> "
    	        "<L2RTA filename> <L2RCR filename> <L2RTC filename> "
    	        "<L2STO filename> <L2PXD filename> <L2NDT filename> "
    	        "<Real L2 NoData value> <L2 NoData value> <Max AOT> <Max_Age> "
    	        "<HotSpot flag> <Cirrus flag> <Rain flag> <Year> <Month> <day> <AOT mean> "
    	        "<STO band number> <STO band index> "
    	        "<L2inRTA filename> <L2inRCR filename> "
                "<L2inRTC filename> <L2inSTO filename> "
                "<L2inPXD filename> <L2inNDT filename> "<< std::endl;
        return EXIT_FAILURE;
    }
    // Configure Logger
    vns::Loggers::GetInstance()->Initialize(argv[0]);
    vns::Loggers::GetInstance()->SetMinLevel(vns::LoggerBase::DEBUG);

    const unsigned int Dimension = 2;

    unsigned int cpt(1);

    bool l_InitMode = static_cast<bool>(atoi(argv[cpt++]));

    const char * l_IPTOACFileName  = argv[cpt++];
    const char * l_IPTOCRFileName  = argv[cpt++];
    const char * l_IPTOCFileName  = argv[cpt++];
    const char * l_IPEDGFileName  = argv[cpt++];
    const char * l_IPCLDSumFileName  = argv[cpt++];
    const char * l_IPSATFileName = argv[cpt++];
    const char * l_HighAOTFileName = argv[cpt++];
    
    const char * l_L2RTAFileName = argv[cpt++];
    const char * l_L2RCRFileName = argv[cpt++];
    const char * l_L2RTCFileName = argv[cpt++];
    const char * l_L2STOFileName = argv[cpt++];
    const char * l_L2PXDFileName = argv[cpt++];
    const char * l_L2NDTFileName = argv[cpt++];


    double l_RealL2NoData = atoi(argv[cpt++]);
    short l_L2NoData = atoi(argv[cpt++]);
    double l_MaxAOT = atof(argv[cpt++]);
    unsigned short l_MaxAge = static_cast<unsigned short>(atoi(argv[cpt++]));

    bool l_HotSpotFlag = static_cast<bool>(atoi(argv[cpt++]));
    bool l_CirrusFlag = static_cast<bool>(atoi(argv[cpt++]));
    bool l_RainFlag = static_cast<bool>(atoi(argv[cpt++]));

    int l_Year = atoi(argv[cpt++]);
    int l_Month = atoi(argv[cpt++]);
    int l_Day = atoi(argv[cpt++]);

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

    double l_AOTMean = atof(argv[cpt++]);
    unsigned int l_STOBandNumber = atoi(argv[cpt++]);
    unsigned int l_STOBandIndex = atoi(argv[cpt++]-1);

    const char * l_L2inRTAFileName = "";
    const char * l_L2inRCRFileName = "";
    const char * l_L2inRTCFileName = "";
    const char * l_L2inSTOFileName = "";
    const char * l_L2inPXDFileName = "";
    const char * l_L2inNDTFileName = "";

    if (l_InitMode == false)
    {
        l_L2inRTAFileName = argv[cpt++];
        l_L2inRCRFileName = argv[cpt++];
        l_L2inRTCFileName = argv[cpt++];
        l_L2inSTOFileName = argv[cpt++];
        l_L2inPXDFileName = argv[cpt++];
        l_L2inNDTFileName = argv[cpt++];
    }

    /** Pixel typedefs */
    typedef double InputPixelType;
    typedef unsigned char MaskPixelType;
    typedef short DatePixelType;
    typedef double OutputPixelType;
    /** Image typedefs */
    typedef otb::VectorImage<InputPixelType,Dimension> InputImageType;
    typedef otb::Image<MaskPixelType,Dimension>        InputMaskType;
    typedef otb::VectorImage<MaskPixelType,Dimension>  VectorMaskType;
    typedef otb::Image<DatePixelType,Dimension>        DateImageType;
    typedef otb::VectorImage<OutputPixelType,Dimension>      OutputImageType;
    typedef itk::CastImageFilter<OutputImageType, vns::VNSIntegerVectorImageType> CastImageFilterType;

    typedef otb::ImageFileReader<InputImageType>    ReaderType;
    typedef otb::ImageFileReader<InputMaskType>     MaskReaderType;
    typedef otb::ImageFileReader<VectorMaskType>    VectorMaskReaderType;
    typedef otb::ImageFileReader<DateImageType>     DateReaderType;

    typedef otb::ImageFileWriter<vns::VNSIntegerVectorImageType>   WriterType;
    typedef otb::ImageFileWriter<DateImageType>     DateWriterType;
    typedef otb::ImageFileWriter<InputMaskType>     MaskWriterType;

    typedef vns::CompositeImageUpdateImageFilter<InputImageType,
                                                 InputMaskType,
                                                 VectorMaskType,
                                                 DateImageType,
                                                 OutputImageType>  FilterType;

    // Multiply image filter type
    typedef otb::MultiplyByScalarImageFilter<InputImageType,InputImageType>  MultiplyFilterType;
    typedef otb::MultiplyByScalarImageFilter<OutputImageType,OutputImageType>  MultiplyFilterType2;


    /** instantiating the filter */

    MultiplyFilterType::Pointer l_IPTOACMultiplyFilter = MultiplyFilterType::New();
    MultiplyFilterType::Pointer l_L2inRTAMultiplyFilter = MultiplyFilterType::New();
    MultiplyFilterType::Pointer l_IPTOCRMultiplyFilter = MultiplyFilterType::New();
    MultiplyFilterType::Pointer l_L2inRCRMultiplyFilter = MultiplyFilterType::New();
    MultiplyFilterType::Pointer l_IPTOCMultiplyFilter = MultiplyFilterType::New();
    MultiplyFilterType::Pointer l_L2inRTCMultiplyFilter = MultiplyFilterType::New();
    MultiplyFilterType::Pointer l_L2inSTOMultiplyFilter = MultiplyFilterType::New();

    MultiplyFilterType2::Pointer l_L2RTAMultiplyFilter = MultiplyFilterType2::New();
    MultiplyFilterType2::Pointer l_L2RCRMultiplyFilter = MultiplyFilterType2::New();
    MultiplyFilterType2::Pointer l_L2RTCMultiplyFilter = MultiplyFilterType2::New();
    MultiplyFilterType2::Pointer l_L2STOMultiplyFilter = MultiplyFilterType2::New();

    CastImageFilterType::Pointer l_L2RTACastToWrite = CastImageFilterType::New();
    CastImageFilterType::Pointer l_L2RCRCastToWrite = CastImageFilterType::New();
    CastImageFilterType::Pointer l_L2RTCCastToWrite = CastImageFilterType::New();
    CastImageFilterType::Pointer l_L2STOCastToWrite = CastImageFilterType::New();


    FilterType::Pointer l_Filter = FilterType::New();

    ReaderType::Pointer l_IPTOACReader  = ReaderType::New();
    ReaderType::Pointer l_L2inRTAReader  = ReaderType::New();
    ReaderType::Pointer l_IPTOCRReader  = ReaderType::New();
    ReaderType::Pointer l_L2inRCRReader  = ReaderType::New();
    ReaderType::Pointer l_IPTOCReader  = ReaderType::New();
    ReaderType::Pointer l_L2inRTCReader  = ReaderType::New();
    ReaderType::Pointer l_L2inSTOReader  = ReaderType::New();
    DateReaderType::Pointer l_L2inPXDReader  = DateReaderType::New();
    MaskReaderType::Pointer l_IPEDGReader  = MaskReaderType::New();
    MaskReaderType::Pointer l_L2inNDTReader  = MaskReaderType::New();
    MaskReaderType::Pointer l_IPCLDSumReader  = MaskReaderType::New();
    VectorMaskReaderType::Pointer l_IPSATReader  = VectorMaskReaderType::New();
    MaskReaderType::Pointer l_HighAOTReader  = MaskReaderType::New();

    WriterType::Pointer l_L2RTAWriter = WriterType::New();
    WriterType::Pointer l_L2RCRWriter = WriterType::New();
    WriterType::Pointer l_L2RTCWriter = WriterType::New();
    WriterType::Pointer l_L2STOWriter = WriterType::New();
    DateWriterType::Pointer l_L2PXDWriter = DateWriterType::New();
    MaskWriterType::Pointer l_L2NDTWriter = MaskWriterType::New();

    l_IPTOACReader->SetFileName(l_IPTOACFileName);
    l_IPTOCRReader->SetFileName(l_IPTOCRFileName);
    l_IPTOCReader->SetFileName(l_IPTOCFileName);
    l_IPEDGReader->SetFileName(l_IPEDGFileName);
    l_IPCLDSumReader->SetFileName(l_IPCLDSumFileName);
    l_IPSATReader->SetFileName(l_IPSATFileName);
    l_HighAOTReader->SetFileName(l_HighAOTFileName);

    l_L2RTAWriter->SetFileName(l_L2RTAFileName);
    l_L2RCRWriter->SetFileName(l_L2RCRFileName);
    l_L2RTCWriter->SetFileName(l_L2RTCFileName);
    l_L2STOWriter->SetFileName(l_L2STOFileName);
    l_L2PXDWriter->SetFileName(l_L2PXDFileName);
    l_L2NDTWriter->SetFileName(l_L2NDTFileName);


    // ------------------------------------
    // Conversion of the images of reflectance
    // ------------------------------------
    l_IPTOACMultiplyFilter->SetInput(l_IPTOACReader->GetOutput());
    l_IPTOACMultiplyFilter->SetCoef(0.001);
    l_IPTOCRMultiplyFilter->SetInput(l_IPTOCRReader->GetOutput());
    l_IPTOCRMultiplyFilter->SetCoef(0.001);
    l_IPTOCMultiplyFilter->SetInput(l_IPTOCReader->GetOutput());
    l_IPTOCMultiplyFilter->SetCoef(0.001);


    // ------------------------------------
    // Set input images
    // ------------------------------------
    l_Filter->SetIPTOACInput(l_IPTOACMultiplyFilter->GetOutput());
    l_Filter->SetIPTOCRInput(l_IPTOCRMultiplyFilter->GetOutput());
    l_Filter->SetIPTOCInput(l_IPTOCMultiplyFilter->GetOutput());
    l_Filter->SetIPNotValidMaskSubInput(l_IPEDGReader->GetOutput());
    l_Filter->SetIPCLDSumInput(l_IPCLDSumReader->GetOutput());
    l_Filter->SetIPSATInput(l_IPSATReader->GetOutput());
    l_Filter->SetHighAOTMaskInput(l_HighAOTReader->GetOutput());

    if (l_InitMode == false)
    {
        l_L2inRTAReader->SetFileName(l_L2inRTAFileName);
        l_L2inRCRReader->SetFileName(l_L2inRCRFileName);
        l_L2inRTCReader->SetFileName(l_L2inRTCFileName);
        l_L2inSTOReader->SetFileName(l_L2inSTOFileName);
        l_L2inPXDReader->SetFileName(l_L2inPXDFileName);
        l_L2inNDTReader->SetFileName(l_L2inNDTFileName);

        l_L2inRTAMultiplyFilter->SetInput(l_L2inRTAReader->GetOutput());
        l_L2inRTAMultiplyFilter->SetCoef(0.001);
        l_L2inRCRMultiplyFilter->SetInput(l_L2inRCRReader->GetOutput());
        l_L2inRCRMultiplyFilter->SetCoef(0.001);
        l_L2inRTCMultiplyFilter->SetInput(l_L2inRTCReader->GetOutput());
        l_L2inRTCMultiplyFilter->SetCoef(0.001);
        l_L2inSTOMultiplyFilter->SetInput(l_L2inSTOReader->GetOutput());
        l_L2inSTOMultiplyFilter->SetCoef(0.001);


        l_Filter->SetL2inRTAInput(l_L2inRTAMultiplyFilter->GetOutput());
        l_Filter->SetL2inRCRInput(l_L2inRCRMultiplyFilter->GetOutput());
        l_Filter->SetL2inRTCInput(l_L2inRTCMultiplyFilter->GetOutput());
        l_Filter->SetL2inSTOInput(l_L2inSTOMultiplyFilter->GetOutput());
        l_Filter->SetL2inPXDInput(l_L2inPXDReader->GetOutput());
        l_Filter->SetL2inNDTInput(l_L2inNDTReader->GetOutput());
    }

    // ------------------------------
    // Set parameters
    // ------------------------------
    l_Filter->SetInitMode(l_InitMode);
    l_Filter->SetRealL2NoData(l_RealL2NoData);
    l_Filter->SetL2NoData(l_L2NoData);
    l_Filter->SetMaxAOT(l_MaxAOT);
    l_Filter->SetMaxAge(l_MaxAge);
    l_Filter->SetHotspotFlag(l_HotSpotFlag);
    l_Filter->SetCirrusFlag(l_CirrusFlag);
    l_Filter->SetRainFlag(l_RainFlag);
    l_Filter->SetJDay(static_cast<short>(l_JDay));
    l_Filter->SetAOTMean(l_AOTMean);
    l_Filter->SetSTOBandNumber(l_STOBandNumber);
    l_Filter->SetSTOBandIndex(l_STOBandIndex);


    std::cout<<l_Filter<<std::endl;


    // ------------------------------
    // Multiply image filter
    // ------------------------------
    l_L2RTAMultiplyFilter->SetInput(l_Filter->GetL2RTAOutput());
    l_L2RTAMultiplyFilter->SetCoef(1000);
    l_L2RCRMultiplyFilter->SetInput(l_Filter->GetL2RCROutput());
    l_L2RCRMultiplyFilter->SetCoef(1000);
    l_L2RTCMultiplyFilter->SetInput(l_Filter->GetL2RTCOutput());
    l_L2RTCMultiplyFilter->SetCoef(1000);
    l_L2STOMultiplyFilter->SetInput(l_Filter->GetL2STOOutput());
    l_L2STOMultiplyFilter->SetCoef(1000);

    l_L2RTACastToWrite->SetInput(l_L2RTAMultiplyFilter->GetOutput());
    l_L2RCRCastToWrite->SetInput(l_L2RCRMultiplyFilter->GetOutput());
    l_L2RTCCastToWrite->SetInput(l_L2RTCMultiplyFilter->GetOutput());
    l_L2STOCastToWrite->SetInput(l_L2STOMultiplyFilter->GetOutput());

    // ------------------------------
    // Set writers
    // ------------------------------

    l_L2RTAWriter->SetNumberOfDivisionsStrippedStreaming(1);
    l_L2RTAWriter->SetInput(l_L2RTACastToWrite->GetOutput());
    l_L2RTAWriter->Update();

    l_L2RCRWriter->SetNumberOfDivisionsStrippedStreaming(1);
    l_L2RCRWriter->SetInput(l_L2RCRCastToWrite->GetOutput());
    l_L2RCRWriter->Update();

    l_L2RTCWriter->SetNumberOfDivisionsStrippedStreaming(1);
    l_L2RTCWriter->SetInput(l_L2RTCCastToWrite->GetOutput());
    l_L2RTCWriter->Update();

    l_L2STOWriter->SetNumberOfDivisionsStrippedStreaming(1);
    l_L2STOWriter->SetInput(l_L2STOCastToWrite->GetOutput());
    l_L2STOWriter->Update();

    l_L2PXDWriter->SetNumberOfDivisionsStrippedStreaming(1);
    l_L2PXDWriter->SetInput(l_Filter->GetL2PXDOutput());
    l_L2PXDWriter->Update();

    l_L2NDTWriter->SetNumberOfDivisionsStrippedStreaming(1);
    l_L2NDTWriter->SetInput(l_Filter->GetL2NDTOutput());
    l_L2NDTWriter->Update();



    return EXIT_SUCCESS;
}
