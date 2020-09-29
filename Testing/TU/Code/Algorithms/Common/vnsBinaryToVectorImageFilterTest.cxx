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
 * VERSION : 1-0-0 : <TypeFT> : <NumFT> : 6 avr. 2010 : Creation                                                           
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id$
 *                                                                                                          *
 ************************************************************************************************************/

#include "vnsLoggers.h"
#include "vnsBinaryToVectorImageFilter.h"
#include "otbImage.h"
#include "otbVectorImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"

int vnsBinaryToVectorImageFilterTest(int argc, char * argv [])
{
    if(argc!=4)
    {
    	std::cerr << argv[0] << " <input filename> <number of input component value> <output filename>" << std::endl;
        return EXIT_FAILURE;
    }
    // Configure Logger
    vns::Loggers::GetInstance()->Initialize(argv[0]);
    vns::Loggers::GetInstance()->SetMinLevel(vns::LoggerBase::DEBUG);

    const unsigned int Dimension = 2;
    const char * inputFileName  = argv[1];
    unsigned int numberOfComponentPerPixel(atoi(argv[2]));
    const char * outputFileName = argv[3];
    
    /** Pixel typedefs */
    typedef unsigned short InputPixelType;
    typedef unsigned char  OutputPixelType;
    /** Image typedefs */
    typedef otb::Image<InputPixelType,Dimension>    InputImageType;
    typedef otb::VectorImage<OutputPixelType,Dimension>   OutputImageType;
    typedef otb::ImageFileReader<InputImageType>    ReaderType;
    typedef otb::ImageFileWriter<OutputImageType>   WriterType;

    typedef vns::BinaryToVectorImageFilter<InputImageType,OutputImageType>  FilterType;
    std::cout <<" Number of components per pixel extracted: "<<numberOfComponentPerPixel<<std::endl;

    /** instantiating the filter */
    FilterType::Pointer filter = FilterType::New();
    ReaderType::Pointer reader  = ReaderType::New();
    WriterType::Pointer writer = WriterType::New();
    reader->SetFileName(inputFileName);
    writer->SetFileName(outputFileName);

    filter->SetInput(reader->GetOutput());
    filter->SetNumberOfComponentsPerPixel(numberOfComponentPerPixel);
    writer->SetInput(filter->GetOutput());

    writer->Update();
    std::cout <<" Output image number of components per pixel generated: "<<filter->GetOutput()->GetNumberOfComponentsPerPixel()<<std::endl;

    InputImageType::RegionType l_ReqRegion;
    InputImageType::SizeType l_ReqSize;
    l_ReqSize.Fill(1);
    l_ReqRegion.SetSize(l_ReqSize);
    InputImageType::IndexType l_ReqId;
    l_ReqId.Fill(1);
    l_ReqRegion.SetIndex(l_ReqId);
    filter->GetOutput()->SetRequestedRegion(l_ReqRegion);
    filter->GetOutput()->Update();
    std::cout <<" : "<<static_cast<int>(filter->GetOutput()->GetPixel(l_ReqId)[0])<<std::endl;
    std::cout <<" : "<<static_cast<int>(filter->GetOutput()->GetPixel(l_ReqId)[1])<<std::endl;
    std::cout <<" : "<<static_cast<int>(filter->GetOutput()->GetPixel(l_ReqId)[2])<<std::endl;
    std::cout <<" : "<<static_cast<int>(filter->GetOutput()->GetPixel(l_ReqId)[3])<<std::endl;
    
    l_ReqId.Fill(2);
    l_ReqRegion.SetIndex(l_ReqId);
    filter->GetOutput()->SetRequestedRegion(l_ReqRegion);
    filter->GetOutput()->Update();
    std::cout <<" : "<<static_cast<int>(filter->GetOutput()->GetPixel(l_ReqId)[0])<<std::endl;
    std::cout <<" : "<<static_cast<int>(filter->GetOutput()->GetPixel(l_ReqId)[1])<<std::endl;
    std::cout <<" : "<<static_cast<int>(filter->GetOutput()->GetPixel(l_ReqId)[2])<<std::endl;
    std::cout <<" : "<<static_cast<int>(filter->GetOutput()->GetPixel(l_ReqId)[3])<<std::endl;

    return EXIT_SUCCESS;
}
