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
#include "vnsBinaryLogicalOrFunctor.h"

#include "otbImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"

#include "itkBinaryFunctorImageFilter.h"

int
vnsBinaryLogicalOrFunctorTest(int argc, char * argv[])
{
    if (argc != 4)
    {
        std::cerr << argv[0] << " <input image filename1> <input image filename2> <output filename>" << std::endl;
        return EXIT_FAILURE;
    }
    // Configure Logger
    vns::Loggers::GetInstance()->Initialize(argv[0]);
    vns::Loggers::GetInstance()->SetMinLevel(vns::LoggerBase::DEBUG);

    const unsigned int Dimension = 2;
    const char * l_InputFileName1 = argv[1];
    const char * l_InputFileName2 = argv[2];
    const char * l_OutputFileName = argv[3];

    /** Pixel typedefs */
    typedef unsigned char PixelType;

    /** Image typedefs */
    typedef otb::Image<PixelType, Dimension> InputImageType;
    typedef otb::Image<PixelType, Dimension> OutputImageType;
    typedef otb::ImageFileReader<InputImageType> ReaderType;
    typedef otb::ImageFileWriter<OutputImageType> WriterType;

    typedef vns::Functor::BinaryLogicalOrFunctor<InputImageType::PixelType, InputImageType::PixelType, OutputImageType::PixelType> FunctorType;

    typedef itk::BinaryFunctorImageFilter<InputImageType, InputImageType, OutputImageType, FunctorType> FilterType;

    /** instantiating the filter */
    FilterType::Pointer l_Filter = FilterType::New();
    ReaderType::Pointer l_Reader1 = ReaderType::New();
    ReaderType::Pointer l_Reader2 = ReaderType::New();
    WriterType::Pointer l_Writer = WriterType::New();

    l_Reader1->SetFileName(l_InputFileName1);
    l_Reader2->SetFileName(l_InputFileName2);
    l_Writer->SetFileName(l_OutputFileName);

    l_Filter->SetInput(l_Reader1->GetOutput());
    l_Filter->SetInput2(l_Reader2->GetOutput());
    l_Writer->SetInput(l_Filter->GetOutput());

    l_Writer->SetFileName(l_OutputFileName);
    l_Writer->Update();

    return EXIT_SUCCESS;
}
