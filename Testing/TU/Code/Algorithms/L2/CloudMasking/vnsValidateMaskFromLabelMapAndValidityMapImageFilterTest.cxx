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
 * $Id: vnsConditionalDifferenceImageFilterTest.cxx 8615 2014-08-01 10:45:09Z tfeuvrie $
 *                                                                                                          *
 ************************************************************************************************************/

#include "vnsLoggers.h"
#include "otbImageFileReader.h"
#include "vnsMacro.h"

#include "otbImage.h"
#include "otbImageFileWriter.h"
#include "vnsValidateMaskFromLabelMapAndValidityMapImageFilter.h"

int
vnsValidateMaskFromLabelMapAndValidityMapImageFilterTest(int argc, char * argv[])
{
    if (argc != 4)
    {
        std::cerr << argv[0] << "usage" << std::endl;
        return EXIT_FAILURE;
    }
    // Configure Logger
    vns::Loggers::GetInstance()->Initialize(argv[0]);
    vns::Loggers::GetInstance()->SetMinLevel(vns::LoggerBase::DEBUG);

    const char * l_InputFileName = argv[1];
    const char * l_LabelInputFileName = argv[2];
    const char * l_OutFileName = argv[3];
    std::cout <<"l_InputFileName : "<< l_InputFileName <<std::endl;
    std::cout <<"l_LabelInputFileName : "<< l_LabelInputFileName <<std::endl;
    std::cout <<"l_OutFileName : "<< l_OutFileName <<std::endl;

    typedef unsigned short LabelPixelType;
    typedef otb::Image<LabelPixelType> LabelImageType;
    typedef vns::VNSMaskImageType InputMaskType;


    typedef std::map<LabelPixelType, bool> MapType;

    MapType l_map;
    l_map[90] = false;
    l_map[45] = true;
    l_map[65] = true;
    l_map[114] = false;

    vns::Utilities::PrintMap("Map", l_map, std::cout);

    /** Image typedefs */
    typedef otb::ImageFileReader<InputMaskType> ReaderType;
    typedef otb::ImageFileReader<LabelImageType> InputMaskReaderType;
    typedef otb::ImageFileWriter<InputMaskType> WriterType;

    typedef vns::ValidateMaskFromLabelMapAndValidityMapImageFilter<InputMaskType, LabelImageType, InputMaskType> ValidateMaskFromLabelMapAndValidityMapImageFilterType;
    typedef /*typename*/ ValidateMaskFromLabelMapAndValidityMapImageFilterType::Pointer ValidateMaskFromLabelMapAndValidityMapImageFilterPointerType;

    ValidateMaskFromLabelMapAndValidityMapImageFilterPointerType l_ShadowVariationPixelDiscardingFilter =  ValidateMaskFromLabelMapAndValidityMapImageFilterType::New();

    /** instantiating the filter */
    ReaderType::Pointer l_InputReader = ReaderType::New();
    InputMaskReaderType::Pointer l_LabelReader = InputMaskReaderType::New();

    WriterType::Pointer l_Writer = WriterType::New();

    l_InputReader->SetFileName(l_InputFileName);
    l_LabelReader->SetFileName(l_LabelInputFileName);

    l_ShadowVariationPixelDiscardingFilter->SetMaskInput(l_InputReader->GetOutput());
    l_ShadowVariationPixelDiscardingFilter->SetLabelInput(l_LabelReader->GetOutput());
    l_ShadowVariationPixelDiscardingFilter->SetValidityMap(l_map);


    l_Writer->SetFileName(l_OutFileName);
    l_Writer->SetInput(l_ShadowVariationPixelDiscardingFilter->GetOutput());
    l_Writer->Update();

    return EXIT_SUCCESS;
}
