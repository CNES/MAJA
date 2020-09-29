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
#include "vnsAltitudeCorrectionFunctor.h"
#include "otbImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"

#include "itkBinaryFunctorImageFilter.h"

int
vnsAltitudeCorrectionFunctorTest(int argc, char * argv[])
{
    if (argc != 4)
    {
        std::cerr << argv[0] << " <IPCLA input filename> <L2CLDSubLabel filename> "
                "<IPCLA output filename>" << std::endl;
        return EXIT_FAILURE;
    }
    // Configure Logger
    vns::Loggers::GetInstance()->Initialize(argv[0]);
    vns::Loggers::GetInstance()->SetMinLevel(vns::LoggerBase::DEBUG);

    const unsigned int Dimension = 2;
    const char * l_IPinCLAFileName = argv[1];
    const char * l_L2CLDSubLabelFileName = argv[2];
    const char * l_IPCLAFileName = argv[3];

    /** Pixel typedefs */
    typedef double InputPixelType;
    typedef unsigned short LabelPixelType;
    typedef double OutputPixelType;

    /** Image typedefs */
    typedef otb::Image<InputPixelType, Dimension> InputImageType;
    typedef otb::Image<LabelPixelType, Dimension> LabelImageType;
    typedef otb::Image<OutputPixelType, Dimension> OutputImageType;

    typedef otb::ImageFileReader<InputImageType> ReaderType;
    typedef otb::ImageFileReader<LabelImageType> LabelReaderType;

    typedef otb::ImageFileWriter<OutputImageType> WriterType;

    typedef vns::Functor::AltitudeCorrectionFunctor<InputPixelType, LabelPixelType, OutputPixelType> FunctorType;
    typedef itk::BinaryFunctorImageFilter<InputImageType, LabelImageType, OutputImageType, FunctorType> FilterType;

    typedef std::map<LabelPixelType, double> DoubleMapType;

    /** instantiating the filter */
    FilterType::Pointer l_Filter = FilterType::New();
    ReaderType::Pointer l_Reader = ReaderType::New();
    LabelReaderType::Pointer l_LabelReader = LabelReaderType::New();

    WriterType::Pointer l_Writer = WriterType::New();

    // Map initialization
    DoubleMapType l_AltitudeCorrectionMap;

    l_AltitudeCorrectionMap[102] = 1000.;
    l_AltitudeCorrectionMap[229] = 2000.;
    l_AltitudeCorrectionMap[249] = 3000.;

    l_Reader->SetFileName(l_IPinCLAFileName);
    l_LabelReader->SetFileName(l_L2CLDSubLabelFileName);

    l_Writer->SetFileName(l_IPCLAFileName);

    l_Filter->SetInput1(l_Reader->GetOutput());
    l_Filter->SetInput2(l_LabelReader->GetOutput());

    l_Filter->GetFunctor().SetMap(l_AltitudeCorrectionMap);

    l_Writer->SetInput(l_Filter->GetOutput());

    l_Writer->Update();

    return EXIT_SUCCESS;
}
