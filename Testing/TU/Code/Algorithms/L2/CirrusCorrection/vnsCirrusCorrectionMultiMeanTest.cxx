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
 * VERSION : 1-0-0 : <TypeFT> : <NumFT> : 24 juin 2010 : Creation                                                           
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id$
 *                                                                                                          *
 ************************************************************************************************************/

#include "vnsLoggers.h"
#include "vnsCirrusMultiMeanStatisticsVectorImageFilter.h"
#include "otbImage.h"
#include "vnsMacro.h"
#include "otbObjectList.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "otbMultiplyByScalarImageFilter.h"
#include "vnsUtilities.h"



int
vnsCirrusMultiMeanTest(int argc, char * argv[])
{
    if (argc != 7)
    {
        std::cerr << argv[0] << " <L1Cirrus input filename> <L2TOA input filename> "
                <<"<CirrusMask filename> <ThinCirrusMask filename>"
                << "<NonNotCirrusMask filename> <output filename>"
                << std::endl;
        return EXIT_FAILURE;
    }
    // Configure Logger
    vns::Loggers::GetInstance()->Initialize(argv[0]);
    vns::Loggers::GetInstance()->SetMinLevel(vns::LoggerBase::DEBUG);

    const unsigned int Dimension = 2;
    const char * l_L1TOAFilename = argv[1];
    const char * l_L2TOAFilename = argv[2];
    const char * l_CirrusMaskFileName = argv[3];
    const char * l_ThinCirrusMaskFileName = argv[4];
    const char * l_NotNoCirrusMaskFileName = argv[5];
    const char * l_OutputFilename = argv[6];


    /** Pixel typedefs */
    typedef double InputPixelType;
    typedef unsigned short MaskPixelType;
    typedef double OutputPixelType;

    /** Image typedefs */
    typedef vns::VNSRealVectorImageType                  TOAImageType;
    typedef vns::VNSRealImageType                        ImageType;
    typedef vns::VNSMaskImageType                        InputMaskType;
    typedef otb::Image<InputPixelType, Dimension> InputImageType;
    typedef otb::Image<MaskPixelType, Dimension> MaskImageType;
    typedef otb::Image<OutputPixelType, Dimension> OutputImageType;

    typedef otb::ImageFileReader<InputImageType> ReaderType;
    typedef otb::ImageFileReader<TOAImageType> TOAReaderType;
    typedef otb::ImageFileReader<InputMaskType> MaskReaderType;

    typedef otb::ImageFileWriter<TOAImageType> WriterType;

    //Cirrus correction apply
    typedef vns::CirrusMultiMeanStatisticsVectorImageFilter<TOAImageType, ImageType,InputMaskType >
    CirrusMultiMeanFilterType;
    typedef CirrusMultiMeanFilterType::Pointer MultiMeanPointer;
    typedef otb::ObjectList<CirrusMultiMeanFilterType> CirrusMultiMeanFilterListType;

    typedef otb::MultiplyByScalarImageFilter<TOAImageType, TOAImageType>         MultiplyFilterType;
    typedef MultiplyFilterType::Pointer MultiplyFilterPointer;

    //Read the input



    /** instantiating the filter */
    ReaderType::Pointer l_CirrusReader = ReaderType::New();
    TOAReaderType::Pointer l_TOAReader = TOAReaderType::New();
    MultiplyFilterType::Pointer       l_MultiplyFilter_R1 = MultiplyFilterType::New();
    l_MultiplyFilter_R1->SetInput(l_TOAReader->GetOutput());
    l_MultiplyFilter_R1->SetCoef(0.001);
    MaskReaderType::Pointer l_CirrusMaskReader = MaskReaderType::New();
    MaskReaderType::Pointer l_ThinMaskReader = MaskReaderType::New();
    MaskReaderType::Pointer l_NotNonMaskReader = MaskReaderType::New();

    WriterType::Pointer l_Writer = WriterType::New();

    l_CirrusReader->SetFileName(l_L1TOAFilename);
    l_CirrusMaskReader->SetFileName(l_CirrusMaskFileName);
    l_ThinMaskReader->SetFileName(l_ThinCirrusMaskFileName);
    l_NotNonMaskReader->SetFileName(l_NotNoCirrusMaskFileName);
    l_TOAReader->SetFileName(l_L2TOAFilename);

    //compute mean
    //Pipeline the various mean computation

    MultiMeanPointer l_multiMean = CirrusMultiMeanFilterType::New();
    l_multiMean->SetInput(l_MultiplyFilter_R1->GetOutput());
    l_multiMean->SetL1TOACirrusInput(l_CirrusReader->GetOutput());
    l_multiMean->SetCirrusMaskInput(l_CirrusMaskReader->GetOutput());
    l_multiMean->SetCirrusForegroundValue(static_cast<InputMaskType::PixelType>(1));
    l_multiMean->SetThinCirrusMaskInput(l_ThinMaskReader->GetOutput());
    l_multiMean->SetThinCirrusForegroundValue(static_cast<InputMaskType::PixelType>(1));
    l_multiMean->SetNonCirrusMaskInput(l_NotNonMaskReader->GetOutput());
    l_multiMean->SetNonCirrusForegroundValue(static_cast<InputMaskType::PixelType>(0));

    l_multiMean->Update();

    //Get nb pixels for each mask
    const unsigned int nbPixCirrus = l_multiMean->GetNbOfValidCirrusValues();
    vnsLogDebugValueMacro(nbPixCirrus)
    const unsigned int nbPixThin = l_multiMean->GetNbOfValidThinCirrusValues();
    vnsLogDebugValueMacro(nbPixThin)
    const unsigned int nbPixNon = l_multiMean->GetNbOfValidNonCirrusValues();
    vnsLogDebugValueMacro(nbPixNon)
    const double l_MeanCirrus = l_multiMean->GetL1ThinCirrusMean();
    vnsLogDebugValueMacro(l_MeanCirrus);
    const TOAImageType::PixelType l_NonMean = l_multiMean->GetL2NonCirrusMean();
    vnsLogDebugValueMacro(l_NonMean);
    const TOAImageType::PixelType l_ThinMean = l_multiMean->GetL2ThinCirrusMean();
    vnsLogDebugValueMacro(l_ThinMean);


    std::fstream file(l_OutputFilename, std::ios::out);
    file<<"nbPixCirrus : "<<nbPixCirrus<<std::endl;
    file<<"nbPixThin : "<<nbPixThin<<std::endl;
    file<<"nbPixNon : "<<nbPixNon<<std::endl;
    file<<"MeanCirrus : "<<l_MeanCirrus<<std::endl;
    file<<"NonMean : "<<l_NonMean<<std::endl;
    file<<"ThinMean : "<<l_ThinMean<<std::endl;
    file.close();

    return EXIT_SUCCESS;
}
