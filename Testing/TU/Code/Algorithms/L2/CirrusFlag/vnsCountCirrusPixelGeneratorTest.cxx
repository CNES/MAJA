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
 * VERSION : 1-0-0 : <TypeFT> : <NumFT> : 9 avr. 2010 : Creation                                                           
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id$
 *                                                                                                          *
 ************************************************************************************************************/

#include "vnsLoggers.h"
#include "vnsCountCirrusPixelGenerator.h"
#include "otbImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"

int vnsCountCirrusPixelGeneratorTest(int argc, char * argv [])
{
    if(argc!=6)
    {
        std::cerr << argv[0] << " <input filename> <input edg sub> <output filename> <altitude threshold> <no_data>" << std::endl;
        return EXIT_FAILURE;
    }
    // Configure Logger
    vns::Loggers::GetInstance()->Initialize(argv[0]);
    vns::Loggers::GetInstance()->SetMinLevel(vns::LoggerBase::DEBUG);

    const unsigned int Dimension = 2;
    const char * inputFileName  = argv[1];
    const char * inputEDGSub  = argv[2];
    const char * outputFileName = argv[3];
    
    /** Pixel typedefs */
    typedef double InputPixelType;

    InputPixelType lAltitudeThreshold = static_cast<InputPixelType>(atoi(argv[4]));
    short l_L1NoData = static_cast<short>(atoi(argv[5]));

    /** Image typedefs */
    typedef otb::Image<InputPixelType,Dimension>    InputImageType;
    typedef otb::Image<unsigned char,Dimension> InputMaskType;
    typedef otb::ImageFileReader<InputImageType>    ReaderType;
    typedef otb::ImageFileReader<InputMaskType>    MaskReaderType;

    typedef vns::CountCirrusPixelGenerator<InputImageType,InputMaskType>  FilterType;

    /** instantiating the filter */
    FilterType::Pointer filter = FilterType::New();
    ReaderType::Pointer reader  = ReaderType::New();
    MaskReaderType::Pointer l_MaskReader  = MaskReaderType::New();
    reader->SetFileName(inputFileName);
    l_MaskReader->SetFileName(inputEDGSub);

    filter->SetInput(reader->GetOutput());
    filter->SetEDGSub(l_MaskReader->GetOutput());
    filter->SetAltitudeThreshold(lAltitudeThreshold);
    filter->SetL1NoData(l_L1NoData);
    filter->Update();

    std::ofstream file;
    file.open(outputFileName);
    file<<"Number of cirrus pixels: "<<filter->GetCirrusPixelNumber()<<std::endl;
    file<<"Number of valid pixels: "<<filter->GetValidPixelNumber()<<std::endl;
    file<<std::fixed;
    file.close();


    return EXIT_SUCCESS;
}
