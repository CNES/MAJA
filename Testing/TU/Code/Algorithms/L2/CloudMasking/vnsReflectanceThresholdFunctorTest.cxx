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
 * VERSION : 1.0.0 : DM : LAIG-DM-MAC-1991-CNES : 29 novembre 2016 : Update CloudMasking ATCOR              *
 * VERSION : 1-0-0 : <TypeFT> : <NumFT> : 7 mai 2010 : Creation                                             *              
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id$
 *                                                                                                          *
 ************************************************************************************************************/
#include "vnsReflectanceThresholdFunctor.h"

#include "vnsLoggers.h"
#include "otbImageFileReader.h"

#include "otbImage.h"
#include "otbVectorImage.h"
#include "otbImageFileWriter.h"

#include "otbQuaternaryFunctorImageFilter.h"

int
vnsReflectanceThresholdFunctorTest(int argc, char * argv[])
{
    if (argc != 19)
    {
        std::cerr << argv[0] << " <IPTOCR filename> <IPSAT mask filename> <IPEDG mask filename> <IPCLDRefl filename>"
                "<Water mask filename> <Blue band index> <Water band index> <Red band index>"
                "<Nir band index> <Swir band index> <Has Swir band> <ForegroundValue>"
                "<blue threshold value> <water threshold value>"
                "<Red threshold value> <NirRed ratio max threshold value>"
                "<NirRed ratio min threshold value> <NirSwir ratio min threshold value>"<< std::endl;
        return EXIT_FAILURE;
    }
    // Configure Logger
    vns::Loggers::GetInstance()->Initialize(argv[0]);
    vns::Loggers::GetInstance()->SetMinLevel(vns::LoggerBase::DEBUG);

    unsigned int cpt(1);

    const unsigned int Dimension = 2;
    const char * l_IPTOCRFileName = argv[cpt++];
    const char * l_IPSATFileName = argv[cpt++];
    const char * l_IPEDGFileName = argv[cpt++];
    const char * l_IPWATFileName = argv[cpt++];
    const char * l_IPCLDReflFileName = argv[cpt++];

    /** Pixel typedefs */
    typedef double InputPixelType;
    typedef unsigned short MaskPixelType;
    typedef unsigned short OutputMaskPixelType;

    /** Parameters definition */
    unsigned int l_BlueBand = static_cast<unsigned int>(atoi(argv[cpt++]) - 1);
    unsigned int l_WaterBand = static_cast<unsigned int>(atoi(argv[cpt++]) - 1);
    unsigned int l_RedBand = static_cast<unsigned int>(atoi(argv[cpt++]) - 1);
    unsigned int l_NirBand = static_cast<unsigned int>(atoi(argv[cpt++]) - 1);
    unsigned int l_SwirBand = static_cast<unsigned int>(atoi(argv[cpt++]) - 1);
    bool l_HasSwirBand = (strcmp(argv[cpt++],"true") == 0 || strcmp(argv[cpt], "True")== 0 );
    OutputMaskPixelType l_ForegroundValue = static_cast<OutputMaskPixelType>(atoi(argv[cpt++]));
    InputPixelType l_BlueThresholdValue = static_cast<InputPixelType>(atof(argv[cpt++]));
    InputPixelType l_WaterThresholdValue = static_cast<InputPixelType>(atof(argv[cpt++]));
    InputPixelType l_RedThresholdValue = static_cast<InputPixelType>(atof(argv[cpt++]));
    InputPixelType l_NirRedRatioMaxThresholdValue = static_cast<InputPixelType>(atof(argv[cpt++]));
    InputPixelType l_NirRedRatioMinThresholdValue = static_cast<InputPixelType>(atof(argv[cpt++]));
    InputPixelType l_NirSwirRatioMinThresholdValue = static_cast<InputPixelType>(atof(argv[cpt++]));

    std::cout << "InputImage=" << l_IPTOCRFileName << std::endl;
    std::cout << "Saturated Mask Image=" << l_IPSATFileName << std::endl;
    std::cout << "Edge Mask Image=" << l_IPEDGFileName << std::endl;
    std::cout << "Cloud Refl Mask=" << l_IPCLDReflFileName << std::endl;
    std::cout << "Has swir band = " << ((l_HasSwirBand)?"yes":"no") << std::endl;
    std::cout << "Blue Threshold value=" << l_BlueThresholdValue << std::endl;
    std::cout << "Water Threshold value=" << l_WaterThresholdValue << std::endl;
    std::cout << "Red Threshold value=" << l_RedThresholdValue << std::endl;
    std::cout << "NirRed ratio max Threshold value=" << l_NirRedRatioMaxThresholdValue << std::endl;
    std::cout << "NirRed ratio min Threshold value=" << l_NirRedRatioMinThresholdValue << std::endl;
    std::cout << "NirSwir ratio min Threshold value=" << l_NirSwirRatioMinThresholdValue << std::endl;


    /** Image typedefs */
    typedef otb::VectorImage<InputPixelType, Dimension> InputImageType;
    typedef otb::Image<MaskPixelType, Dimension> MaskImageType;
    typedef otb::VectorImage<MaskPixelType, Dimension> VectorMaskImageType;
    typedef otb::Image<OutputMaskPixelType, Dimension> OutputMaskType;

    typedef otb::ImageFileReader<InputImageType> ReaderType;
    typedef otb::ImageFileReader<MaskImageType> MaskReaderType;
    typedef otb::ImageFileReader<VectorMaskImageType> VectorMaskReaderType;

    typedef otb::ImageFileWriter<OutputMaskType> WriterType;

    typedef vns::Functor::ReflectanceThresholdFunctor<InputImageType::PixelType, VectorMaskImageType::PixelType, MaskPixelType,
            OutputMaskPixelType> FunctorType;
    typedef otb::QuaternaryFunctorImageFilter<InputImageType, VectorMaskImageType, MaskImageType, MaskImageType, OutputMaskType, FunctorType> FilterType;

    /** instantiating the filter */
    FilterType::Pointer l_Filter = FilterType::New();
    ReaderType::Pointer l_Reader = ReaderType::New();
    MaskReaderType::Pointer l_NdtMaskReader = MaskReaderType::New();
    MaskReaderType::Pointer l_WaterMaskReader = MaskReaderType::New();
    VectorMaskReaderType::Pointer l_SatMaskReader = VectorMaskReaderType::New();

    WriterType::Pointer l_Writer = WriterType::New();

    l_Reader->SetFileName(l_IPTOCRFileName);
    l_SatMaskReader->SetFileName(l_IPSATFileName);
    l_NdtMaskReader->SetFileName(l_IPEDGFileName);
    l_WaterMaskReader->SetFileName(l_IPWATFileName);

    l_Writer->SetFileName(l_IPCLDReflFileName);

    l_Filter->SetInput1(l_Reader->GetOutput());
    l_Filter->SetInput2(l_SatMaskReader->GetOutput());
    l_Filter->SetInput3(l_NdtMaskReader->GetOutput());
    l_Filter->SetInput4(l_WaterMaskReader->GetOutput());

    l_Filter->GetFunctor().SetBlueBand(l_BlueBand);
    l_Filter->GetFunctor().SetWaterBand(l_WaterBand);
    l_Filter->GetFunctor().SetRedBand(l_RedBand);
    l_Filter->GetFunctor().SetNirBand(l_NirBand);
    if (l_HasSwirBand) {
        l_Filter->GetFunctor().SetSwirBand(l_SwirBand);
    }
    l_Filter->GetFunctor().SetForegroundValue(l_ForegroundValue);
    l_Filter->GetFunctor().SetBlueReflectanceThreshold(l_BlueThresholdValue);
    l_Filter->GetFunctor().SetWaterReflectanceThreshold(l_WaterThresholdValue);
    l_Filter->GetFunctor().SetRedReflectanceThreshold(l_RedThresholdValue);
    l_Filter->GetFunctor().SetNirRedRatioMaxThreshold(l_NirRedRatioMaxThresholdValue);
    l_Filter->GetFunctor().SetNirRedRatioMinThreshold(l_NirRedRatioMinThresholdValue);
    l_Filter->GetFunctor().SetNirSwirRatioMinThreshold(l_NirSwirRatioMinThresholdValue);

    l_Writer->SetInput(l_Filter->GetOutput());

    l_Writer->Update();

    return EXIT_SUCCESS;
}
