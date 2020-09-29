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
 * VERSION : 1-0-0 : <TypeFT> : <NumFT> : 11 mai 2010 : Creation                                                           
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id$
 *                                                                                                          *
 ************************************************************************************************************/

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

int
vnsReflectanceVariationCorrelationImageFilterTest(int argc, char * argv[])
{
    if (argc != 28)
    {
        std::cerr << argv[0] << " <IPTOCR image filename> <L2inRCR image filename> <L2inSTO filename> <L2inPXD filename>  "
                "<IPEDG filename> <L2inNDT filename>"
                "<L2inWAS filename> <IPCLDRefl filename> "
                "<IPCLDReflVar filename> <IPCLDExt filename> "
                "<NoData value> <Blue Band> <Red Band> <NIR Band> "
                "<Year> <Month> <Day> <Forgetting duration> <Min var threshold> <Max var threshold> "
                "<KNIRBlue> <KRedBlue> <Delta threshold> <MinPixelCorrel> <CorrelBand>"
                "<CorrelThreshold> <Dilatation option>" << std::endl;
        return EXIT_FAILURE;
    }
    // Configure Logger
    vns::Loggers::GetInstance()->Initialize(argv[0]);
    vns::Loggers::GetInstance()->SetMinLevel(vns::LoggerBase::DEBUG);

    unsigned int cpt(1);

    const unsigned int Dimension = 2;
    const char * l_IPTOCRFileName = argv[cpt++];
    const char * l_L2inRCRFileName = argv[cpt++];
    const char * l_L2inSTOFileName = argv[cpt++];
    const char * l_L2inPXDFileName = argv[cpt++];
    const char * l_IPEDGFileName = argv[cpt++];
    const char * l_L2inNDTFileName = argv[cpt++];
    const char * l_L2inWASFileName = argv[cpt++];
    const char * l_IPCLDReflFileName = argv[cpt++];
    const char * l_IPCLDReflVarFileName = argv[cpt++];
    const char * l_IPCLDExtFileName = argv[cpt++];

    short l_NoData = atoi(argv[cpt++]);
    unsigned int l_BlueBand = atoi(argv[cpt++]) - 1;
    unsigned int l_RedBand = atoi(argv[cpt++]) - 1;
    unsigned int l_NIRBand = atoi(argv[cpt++]) - 1;

    int l_Year = atoi(argv[cpt++]);
    int l_Month = atoi(argv[cpt++]);
    int l_Day = atoi(argv[cpt++]);
    double l_ForgettingDuration = atof(argv[cpt++]);
    double l_MinVarThreshold = atof(argv[cpt++]);
    double l_MaxVarThreshold = atof(argv[cpt++]);
    double l_KNIRBlue = atof(argv[cpt++]);
    double l_KRedBlue = atof(argv[cpt++]);
    double l_DeltaThreshold = atof(argv[cpt++]);
    unsigned int l_MinPixelCorrel = atoi(argv[cpt++]);
    unsigned int l_CorrelBand = atoi(argv[cpt++]) - 1;
    unsigned short l_CorrelThreshold = atoi(argv[cpt++]);
    unsigned short l_DilatationOption = atoi(argv[cpt++]);

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

    std::cout << "l_JDayRef = " << l_JDayRef << std::endl;

    l_JDay = l_JDay - l_JDayRef;

    std::cout << "l_JDay = " << l_JDay << std::endl;

    /** Pixel typedefs */
    typedef double InputPixelType;
    typedef short DatePixelType;
    typedef unsigned char InputMaskPixelType;
    typedef unsigned char OutputPixelType;

    /** Image typedefs */
    typedef otb::VectorImage<InputPixelType, Dimension> InputImageType;
    typedef otb::Image<InputMaskPixelType, Dimension> InputMaskType;
    typedef otb::Image<DatePixelType, Dimension> DateImageType;
    typedef otb::Image<OutputPixelType, Dimension> OutputImageType;

    typedef otb::ImageFileReader<InputImageType> ReaderType;
    typedef otb::ImageFileReader<InputMaskType> InputMaskReaderType;
    typedef otb::ImageFileReader<DateImageType> DateReaderType;
    typedef otb::ImageFileWriter<OutputImageType> MaskWriterType;

    /** Filter typedefs */
    typedef vns::ReflectanceVariationCorrelationImageFilter<InputImageType, InputMaskType, DateImageType, OutputImageType> FilterType;
    typedef otb::MultiplyByScalarImageFilter<InputImageType, InputImageType> MultiplyFilterType;

    typedef itk::BinaryBallStructuringElement<OutputPixelType, Dimension> StructuringElementType;
    typedef itk::BinaryDilateImageFilter<OutputImageType, OutputImageType, StructuringElementType> DilateFilterType;

    /** instantiating the filter */
    MultiplyFilterType::Pointer l_MultiplyFilter1 = MultiplyFilterType::New();
    MultiplyFilterType::Pointer l_MultiplyFilter2 = MultiplyFilterType::New();
    MultiplyFilterType::Pointer l_MultiplyFilter3 = MultiplyFilterType::New();
    FilterType::Pointer l_Filter = FilterType::New();
    ReaderType::Pointer l_IPTOCRReader = ReaderType::New();
    ReaderType::Pointer l_L2inRCRReader = ReaderType::New();
    ReaderType::Pointer l_L2inSTOReader = ReaderType::New();
    InputMaskReaderType::Pointer l_IPEDGReader = InputMaskReaderType::New();
    InputMaskReaderType::Pointer l_L2inNDTReader = InputMaskReaderType::New();
    InputMaskReaderType::Pointer l_L2inWASReader = InputMaskReaderType::New();
    InputMaskReaderType::Pointer l_IPCLDReflReader = InputMaskReaderType::New();
    DateReaderType::Pointer l_L2inPXDReader = DateReaderType::New();
    MaskWriterType::Pointer l_IPCLDReflVarWriter = MaskWriterType::New();
    MaskWriterType::Pointer l_IPCLDExtWriter = MaskWriterType::New();

    DilateFilterType::Pointer l_BinaryDilateFilter = DilateFilterType::New();

    l_IPTOCRReader->SetFileName(l_IPTOCRFileName);
    l_L2inRCRReader->SetFileName(l_L2inRCRFileName);
    l_L2inSTOReader->SetFileName(l_L2inSTOFileName);
    l_L2inPXDReader->SetFileName(l_L2inPXDFileName);
    l_IPEDGReader->SetFileName(l_IPEDGFileName);
    l_L2inNDTReader->SetFileName(l_L2inNDTFileName);
    l_L2inWASReader->SetFileName(l_L2inWASFileName);
    l_IPCLDReflReader->SetFileName(l_IPCLDReflFileName);
    l_IPCLDReflVarWriter->SetFileName(l_IPCLDReflVarFileName);
    l_IPCLDExtWriter->SetFileName(l_IPCLDExtFileName);

    l_MultiplyFilter1->SetInput(l_IPTOCRReader->GetOutput());
    l_MultiplyFilter1->SetCoef(0.001);
    l_Filter->SetIPTOCRInput(l_MultiplyFilter1->GetOutput());

    l_MultiplyFilter2->SetInput(l_L2inRCRReader->GetOutput());
    l_MultiplyFilter2->SetCoef(0.001);
    l_Filter->SetL2inRCRInput(l_MultiplyFilter2->GetOutput());

    l_MultiplyFilter3->SetInput(l_L2inSTOReader->GetOutput());
    l_MultiplyFilter3->SetCoef(0.001);
    l_Filter->SetL2inSTOInput(l_MultiplyFilter3->GetOutput());

    l_L2inSTOReader->UpdateOutputInformation();
    std::vector<double> l_STOListOfJDates;
    l_STOListOfJDates.push_back(l_JDay);
    for (unsigned int iii = 0; iii < l_L2inSTOReader->GetOutput()->GetNumberOfComponentsPerPixel() - 1; iii++)
    {
        l_STOListOfJDates.push_back(0.0);
    }
    l_Filter->SetSTOListOfDates(l_STOListOfJDates);

    l_Filter->SetL2inPXDInput(l_L2inPXDReader->GetOutput());
    l_Filter->SetIPEDGInput(l_IPEDGReader->GetOutput());
    l_Filter->SetL2inNDTInput(l_L2inNDTReader->GetOutput());
    l_Filter->SetConditionalMaskInput(l_L2inWASReader->GetOutput());
    l_Filter->SetIPCLDReflInput(l_IPCLDReflReader->GetOutput());

    l_Filter->SetNoData(l_NoData);

//    l_Filter->SetBlueBand(l_BlueBand);
//    l_Filter->SetRedBand(l_RedBand);
//    l_Filter->SetNIRBand(l_NIRBand);
//    l_Filter->SetCorrelBand(l_CorrelBand);

    l_Filter->SetBlueBandTOCR(l_BlueBand);
    l_Filter->SetBlueBandRCR(l_BlueBand);
    l_Filter->SetRedBandTOCR(l_RedBand);
    l_Filter->SetRedBandRCR(l_RedBand);
    l_Filter->SetNIRBandTOCR(l_NIRBand);
    l_Filter->SetNIRBandRCR(l_NIRBand);
    l_Filter->SetCorrelBandTOCR(l_CorrelBand);
    l_Filter->SetCorrelBandRCR(l_CorrelBand);

    l_Filter->SetJDay(l_JDay);
    l_Filter->SetForgettingDuration(l_ForgettingDuration);
    l_Filter->SetMinVarThreshold(l_MinVarThreshold);
    l_Filter->SetMaxVarThreshold(l_MaxVarThreshold);
    l_Filter->SetKNIRBlue(l_KNIRBlue);
    l_Filter->SetKRedBlue(l_KRedBlue);
    l_Filter->SetDeltaThreshold(l_DeltaThreshold);
    l_Filter->SetMinPixelCorrelation(l_MinPixelCorrel);
    l_Filter->SetCorrelThreshold(static_cast<double>(l_CorrelThreshold));
    l_Filter->SetNCorrel(3);

    //   l_Filter->SetNumberOfThreads(1);

    //   std::cout<<l_Filter<<std::endl;

    if (l_DilatationOption == 1)
    {
        // ---------------
        // With dilatation
        // ---------------

        StructuringElementType l_StructuringElement;
        l_StructuringElement.SetRadius(2);  // 5x5 structuring element
        l_StructuringElement.CreateStructuringElement();

        l_BinaryDilateFilter->SetKernel(l_StructuringElement);
        l_BinaryDilateFilter->SetInput(l_Filter->GetIPCLDReflVarOutput());
        l_BinaryDilateFilter->SetDilateValue(1);

        l_IPCLDReflVarWriter->SetInput(l_BinaryDilateFilter->GetOutput());

    }
    else
    {
        // ------------------
        // Without dilatation
        // ------------------
        l_IPCLDReflVarWriter->SetInput(l_Filter->GetIPCLDReflVarOutput());
    }

    //   l_IPCLDExtWriter->SetNumberOfDivisionsStrippedStreaming(1);

    l_IPCLDExtWriter->SetInput(l_Filter->GetIPCLDExtOutput());

    l_IPCLDReflVarWriter->Update();
    l_IPCLDExtWriter->Update();

    return EXIT_SUCCESS;
}
