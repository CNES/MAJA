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
#include "vnsHighAOTImageFilter.h"
#include "otbImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"

#include "otbMultiplyByScalarImageFilter.h"

int vnsHighAOTImageFilterTest(int argc, char * argv [])
{
    if(argc!=9)
    {
    	std::cerr << argv[0] << " <IPAOTSub filename> <IPEDG filename> "
    	        "<L2CLDSub filename> <output filename> "
    	        "<CoefSgmaAOT> <Maximum high AOT std> <AOT mean> <AOT std>" << std::endl;
        return EXIT_FAILURE;
    }
    // Configure Logger
    vns::Loggers::GetInstance()->Initialize(argv[0]);
    vns::Loggers::GetInstance()->SetMinLevel(vns::LoggerBase::DEBUG);

    const unsigned int Dimension = 2;
    const char * l_IPAOTSubFileName  = argv[1];
    const char * l_IPEDGFileName  = argv[2];
    const char * l_L2CLDSubFileName  = argv[3];
    const char * l_OutputFileName = argv[4];

    unsigned short l_CoefSigmaAOT = atoi(argv[5]);
    double l_MinimumHighAOTStd = atof(argv[6]);
    double l_AOTMean = atof(argv[7]);
    double l_AOTStd = atof(argv[8]);

    /** Pixel typedefs */
    typedef double InputPixelType;
    typedef unsigned char MaskPixelType;
    typedef unsigned char OutputPixelType;

    /** Image typedefs */
    typedef otb::Image<InputPixelType,Dimension> InputImageType;
    typedef otb::Image<MaskPixelType,Dimension> InputMaskType;
    typedef otb::Image<OutputPixelType,Dimension> OutputImageType;

    typedef otb::ImageFileReader<InputImageType>         IPAOTSubReaderType;
    typedef otb::ImageFileReader<InputMaskType>          MaskReaderType;
    typedef otb::ImageFileWriter<OutputImageType>        WriterType;

    typedef vns::HighAOTImageFilter<InputImageType, InputMaskType, OutputImageType>  FilterType;

    typedef otb::MultiplyByScalarImageFilter<InputImageType,InputImageType>  MultiplyFilterType;


    /** instantiating the filter */
    MultiplyFilterType::Pointer l_MultiplyFilter = MultiplyFilterType::New();
    FilterType::Pointer l_Filter = FilterType::New();
    IPAOTSubReaderType::Pointer l_IPAOTSubReader  = IPAOTSubReaderType::New();
    MaskReaderType::Pointer l_IPEDGReader = MaskReaderType::New();
    MaskReaderType::Pointer l_L2CLDSubReader = MaskReaderType::New();
    WriterType::Pointer l_Writer = WriterType::New();

    l_IPAOTSubReader->SetFileName(l_IPAOTSubFileName);
    l_IPEDGReader->SetFileName(l_IPEDGFileName);
    l_L2CLDSubReader->SetFileName(l_L2CLDSubFileName);
    l_Writer->SetFileName(l_OutputFileName);

    l_MultiplyFilter->SetInput(l_IPAOTSubReader->GetOutput());
    l_MultiplyFilter->SetCoef(0.001);

    l_Filter->SetIPAOTSubInput(l_MultiplyFilter->GetOutput());
    l_Filter->SetIPEDGInput(l_IPEDGReader->GetOutput());
    l_Filter->SetL2CLDSubInput(l_L2CLDSubReader->GetOutput());

    l_Filter->SetAOTMean(l_AOTMean);
    l_Filter->SetAOTStd(l_AOTStd);
    l_Filter->SetCoefSigmaAOT(l_CoefSigmaAOT);
    l_Filter->SetMinimumHighAOTStd(l_MinimumHighAOTStd);

    l_Writer->SetInput(l_Filter->GetOutput());

    l_Writer->Update();

    return EXIT_SUCCESS;
}
