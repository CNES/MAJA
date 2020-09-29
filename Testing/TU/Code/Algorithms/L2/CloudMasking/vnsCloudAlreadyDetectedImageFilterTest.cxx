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
 * VERSION : 1-0-0 : <TypeFT> : <NumFT> : 1 juil. 2010 : Creation                                                           
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id$
 *                                                                                                          *
 ************************************************************************************************************/
#include "vnsLoggers.h"
#include "vnsCloudAlreadyDetectedImageFilter.h"
#include "otbImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"

#include "itkMultiThreader.h"

#include "itkConnectedComponentImageFilter.h"
#include <fstream>

int vnsCloudAlreadyDetectedImageFilterTest(int argc, char * argv [])
{
    if(argc!=7)
    {
        std::cerr << argv[0] << " <IPCLDExt output filename> "
                "<IPCLDRefl filename> <IPCLDReflVar filename> <output filename>"
                "<Excluded label value> <Inside mask value>"<< std::endl;
        return EXIT_FAILURE;
    }
    // Configure Logger
    vns::Loggers::GetInstance()->Initialize(argv[0]);
    vns::Loggers::GetInstance()->SetMinLevel(vns::LoggerBase::DEBUG);

    const unsigned int Dimension = 2;
    const char * l_IPCLDExtFileName = argv[1];
    const char * l_IPCLDReflFileName  = argv[2];
    const char * l_IPCLDReflVarFileName = argv[3];
    const char * l_OutputFileName = argv[4];

    unsigned int l_ExcludedLabelValue = atoi(argv[5]);
    unsigned int l_InsideValue = atoi(argv[6]);

    /** Pixel typedefs */
    typedef unsigned char InputMaskPixelType;
    typedef unsigned short LabelPixelType;

    /** Image typedefs */
    typedef otb::Image<InputMaskPixelType,Dimension>        InputMaskType;
    typedef otb::Image<LabelPixelType,Dimension>            LabelImageType;

    typedef otb::ImageFileReader<InputMaskType>             InputMaskReaderType;
    typedef otb::ImageFileWriter<LabelImageType>            LabelWriterType;

    typedef vns::CloudAlreadyDetectedImageFilter<InputMaskType, LabelImageType>  FilterType;

    // Labelize filter type
    typedef itk::ConnectedComponentImageFilter<InputMaskType, LabelImageType> LabelizeFilterType;

    itk::MultiThreader::SetGlobalDefaultNumberOfThreads(16);


    /** instantiating the filter */
    LabelizeFilterType::Pointer l_LabelFilter = LabelizeFilterType::New();
    FilterType::Pointer l_Filter = FilterType::New();
    InputMaskReaderType::Pointer l_IPCLDReflReader  = InputMaskReaderType::New();
    InputMaskReaderType::Pointer l_IPCLDReflVarReader  = InputMaskReaderType::New();
    InputMaskReaderType::Pointer l_IPCLDExtReader  = InputMaskReaderType::New();

    // Readers
    l_IPCLDReflReader->SetFileName(l_IPCLDReflFileName);
    l_IPCLDReflVarReader->SetFileName(l_IPCLDReflVarFileName);
    l_IPCLDExtReader->SetFileName(l_IPCLDExtFileName);

    // ---------------
    // Image of labels
    // ---------------
    l_LabelFilter->SetInput(l_IPCLDExtReader->GetOutput());

//        typedef otb::ImageFileWriter<LabelImageType>           LabelWriterType;
//        LabelWriterType::Pointer l_LabelWriter = LabelWriterType::New();

//        l_LabelWriter->SetFileName("/MILO/VENUS/caroline/Test/Tu/mask_B01Label.tif");
//        l_LabelWriter->SetInput(l_LabelFilter->GetOutput());
//        l_LabelWriter->Update();

    l_Filter->SetExcludedLabelValue(l_ExcludedLabelValue);
    l_Filter->SetInsideValue(l_InsideValue);

    // Input images
    l_Filter->SetIPCLDRefl(l_IPCLDReflReader->GetOutput());
    l_Filter->SetIPCLDReflVar(l_IPCLDReflVarReader->GetOutput());
    l_Filter->SetIPCLDExtLabel(l_LabelFilter->GetOutput());

    l_Filter->Update();

    std::fstream file(l_OutputFileName, std::ios::out);
    file<<l_Filter;
    file.close();


    return EXIT_SUCCESS;
}
