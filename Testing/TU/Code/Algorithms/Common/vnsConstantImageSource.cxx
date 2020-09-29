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
 * VERSION : 1-0-0 : <TypeFT> : <NumFT> : 20 mai 2010 : Creation                                                           
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id$
 *                                                                                                          *
 ************************************************************************************************************/

#include "vnsLoggers.h"
#include "vnsConstantImageSource.h"
#include "otbImage.h"
#include "otbImageFileWriter.h"
#include "vnsUtilities.h"

int vnsConstantImageSource(int argc, char * argv [])
{
    if(argc < 5)
    {
    	std::cerr << argv[0] << "<output filename> <nbline> <nbcolumn> <value>" << std::endl;
        return EXIT_FAILURE;
    }
    // Configure Logger
    vns::Loggers::GetInstance()->Initialize(argv[0]);
    vns::Loggers::GetInstance()->SetMinLevel(vns::LoggerBase::DEBUG);

    const unsigned int Dimension = 2;
    const char * outputFileName = argv[1];
    const int nbline(atoi(argv[2]));
    const int nbcol(atoi(argv[3]));
    std::string value(argv[4]);
    
    /** Pixel typedefs */
    typedef double OutputPixelType;
    /** Image typedefs */
    typedef otb::Image<OutputPixelType,Dimension>   OutputImageType;
    typedef OutputImageType::PixelType PixelType;
    typedef otb::ImageFileWriter<OutputImageType>   WriterType;
    typedef vns::ConstantImageSource<OutputImageType>  FilterType;

    /** instantiating the filter */
    FilterType::Pointer filter = FilterType::New();
    WriterType::Pointer writer = WriterType::New();
    writer->SetFileName(outputFileName);


    OutputImageType::SizeValueType randomSize[2];

    randomSize[0] = nbline;
    randomSize[1] = nbcol;
    filter->SetSize(randomSize);

    OutputImageType::SpacingValueType spacing[2] = {1., 1.};
    filter->SetSignedSpacing( spacing );

    OutputImageType::PointValueType origin[2] = {0, 0};
    filter->SetOrigin( origin );

    filter->SetConstantValue(vns::Utilities::AsDouble(value));

    writer->SetInput(filter->GetOutput());
    writer->Update();

    return EXIT_SUCCESS;
}
