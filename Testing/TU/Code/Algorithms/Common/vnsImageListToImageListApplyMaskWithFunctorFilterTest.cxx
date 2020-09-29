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

#include <vnsMaskingImageFunctor.h>
#include "vnsLoggers.h"
#include "otbImageFileReader.h"
#include "vnsMacro.h"

#include "otbImage.h"
#include "otbImageFileWriter.h"
#include "vnsImageListToImageListApplyMaskWithFunctorFilter.h"
#include "vnsUtilities.h"

int
vnsImageListToImageListApplyMaskWithFunctorFilterTest(int argc, char * argv[])
{
    if (argc != 6)
    {
        std::cerr << argv[0] << "usage" << std::endl;
        return EXIT_FAILURE;
    }
    // Configure Logger
    vns::Loggers::GetInstance()->Initialize(argv[0]);
    vns::Loggers::GetInstance()->SetMinLevel(vns::LoggerBase::DEBUG);

    typedef unsigned short LabelPixelType;
    typedef otb::Image<LabelPixelType> LabelImageType;
    typedef vns::VNSMaskImageType InputMaskType;
    int cpt = 1;
    const LabelPixelType l_ExcludedValue=static_cast<LabelPixelType>(atoi(argv[cpt++]));
    const LabelPixelType l_DefaultValue=static_cast<LabelPixelType>(atoi(argv[cpt++]));

    const char * l_InputFileName = argv[cpt++];
    const char * l_LabelInputFileName = argv[cpt++];
    std::string l_OutFileName = argv[cpt++];
    std::cout <<"l_InputFileName : "<< l_InputFileName <<std::endl;
    std::cout <<"l_LabelInputFileName : "<< l_LabelInputFileName <<std::endl;
    std::cout <<"l_OutFileName : "<< l_OutFileName <<std::endl;

    typedef otb::ImageList<InputMaskType> MaskImageListType;
    typedef otb::ImageList<LabelImageType> LabelImageListType;
    MaskImageListType::Pointer l_InputList = MaskImageListType::New();

    /** Image typedefs */
    typedef otb::ImageFileReader<InputMaskType> ReaderType;
    typedef otb::ImageFileReader<LabelImageType> InputMaskReaderType;
    typedef otb::ImageFileWriter<InputMaskType> WriterType;

    typedef vns::Functor::MaskingImageFunctor<unsigned char,LabelPixelType,unsigned char> ChangeValueFunctorType;
    typedef vns::ImageListToImageListApplyMaskWithFunctorFilter<MaskImageListType, LabelImageType, MaskImageListType, ChangeValueFunctorType > FilterType;

    FilterType::Pointer l_Filter =  FilterType::New();
    l_Filter->GetFunctor().SetReplaceValue(l_DefaultValue);
    l_Filter->GetFunctor().SetBackgroundValue(l_ExcludedValue);

    /** instantiating the filter */
    ReaderType::Pointer l_InputReader = ReaderType::New();
    InputMaskReaderType::Pointer l_LabelReader = InputMaskReaderType::New();

    WriterType::Pointer l_Writer = WriterType::New();

    l_InputReader->SetFileName(l_InputFileName);
    l_LabelReader->SetFileName(l_LabelInputFileName);
    l_InputReader->UpdateOutputInformation();
    l_InputList->PushBack(l_InputReader->GetOutput());
    l_InputList->PushBack(l_InputReader->GetOutput());
    l_InputList->PushBack(l_InputReader->GetOutput());
    l_InputList->PushBack(l_InputReader->GetOutput());

    l_Filter->SetInput(l_InputList);
    l_Filter->SetInputMaskImage(l_LabelReader->GetOutput());
    l_Filter->Update();


    l_Writer->SetFileName(l_OutFileName + "index0.tif");
    l_Writer->SetInput(l_Filter->GetOutput()->GetNthElement(0));
    l_Writer->Update();
    l_Writer->SetFileName(l_OutFileName + "index1.tif");
    l_Writer->SetInput(l_Filter->GetOutput()->GetNthElement(1));
    l_Writer->Update();
    l_Writer->SetFileName(l_OutFileName + "index2.tif");
    l_Writer->SetInput(l_Filter->GetOutput()->GetNthElement(2));
    l_Writer->Update();

    return EXIT_SUCCESS;
}
