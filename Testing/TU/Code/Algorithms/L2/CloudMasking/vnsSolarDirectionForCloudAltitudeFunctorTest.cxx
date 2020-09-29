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
 * VERSION : 1-0-0 : <TypeFT> : <NumFT> : 21 juin 2010 : Creation                                                           
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id$
 *                                                                                                          *
 ************************************************************************************************************/

#include "vnsLoggers.h"
#include "vnsSolarDirectionForCloudAltitudeFunctor.h"
#include "otbImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "otbVectorImage.h"

#include "itkTernaryFunctorImageFilter.h"

int
vnsSolarDirectionForCloudAltitudeFunctorTest(int argc, char * argv[])
{
    if (argc != 7)
    {
        std::cerr << argv[0] << " <IPSOL1 filename> <IPSOL2 filename> <IPCLA filename> <IPSOL filename> "
                "<H1 value> <H2 value>" << std::endl;
        return EXIT_FAILURE;
    }
    // Configure Logger
    vns::Loggers::GetInstance()->Initialize(argv[0]);
    vns::Loggers::GetInstance()->SetMinLevel(vns::LoggerBase::DEBUG);

    const unsigned int Dimension = 2;
    const char * l_IPSOL1FileName = argv[1];
    const char * l_IPSOL2FileName = argv[2];
    const char * l_IPCLAFileName = argv[3];
    const char * l_IPSOLFileName = argv[4];

    /** Pixel typedefs */
    typedef double InputPixelType;
    typedef double CLAPixelType;
    typedef double OutputPixelType;

    /** Parameters definition */
    CLAPixelType l_H1 = static_cast<CLAPixelType>(atoi(argv[5]));
    CLAPixelType l_H2 = static_cast<CLAPixelType>(atoi(argv[6]));

    /** Image typedefs */
    typedef otb::VectorImage<InputPixelType, Dimension> InputImageType;
    typedef otb::Image<CLAPixelType, Dimension> CLAImageType;
    typedef otb::VectorImage<OutputPixelType, Dimension> OutputImageType;
    typedef otb::ImageFileReader<InputImageType> ReaderType;
    typedef otb::ImageFileReader<CLAImageType> CLAReaderType;
    typedef otb::ImageFileWriter<OutputImageType> WriterType;

    typedef vns::Functor::SolarDirectionForCloudAltitudeFunctor<InputImageType::PixelType, InputImageType::PixelType, CLAPixelType,
            OutputImageType::PixelType> FunctorType;
    typedef itk::TernaryFunctorImageFilter<InputImageType, InputImageType, CLAImageType, OutputImageType, FunctorType> FilterType;

    /** instantiating the filter */
    FilterType::Pointer l_Filter = FilterType::New();
    ReaderType::Pointer l_IPSOL1Reader = ReaderType::New();
    ReaderType::Pointer l_IPSOL2Reader = ReaderType::New();
    CLAReaderType::Pointer l_IPCLAReader = CLAReaderType::New();
    WriterType::Pointer l_IPSOLWriter = WriterType::New();

    l_IPSOL1Reader->SetFileName(l_IPSOL1FileName);
    l_IPSOL2Reader->SetFileName(l_IPSOL2FileName);
    l_IPCLAReader->SetFileName(l_IPCLAFileName);
    l_IPSOLWriter->SetFileName(l_IPSOLFileName);

    l_Filter->SetInput1(l_IPSOL1Reader->GetOutput());
    l_Filter->SetInput2(l_IPSOL2Reader->GetOutput());
    l_Filter->SetInput3(l_IPCLAReader->GetOutput());

    l_Filter->GetFunctor().SetH1(l_H1);
    l_Filter->GetFunctor().SetH2(l_H2);

    l_IPSOLWriter->SetInput(l_Filter->GetOutput());

    l_IPSOLWriter->Update();

    return EXIT_SUCCESS;
}
