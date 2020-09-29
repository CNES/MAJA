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
 * VERSION : 1-0-0 : <TypeFT> : <NumFT> : 22 novembre 2011 : Creation
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id:
 *                                                                                                          *
 ************************************************************************************************************/
#include "vnsListOfStringToImageSource.h"
#include "otbImage.h"
#include "vnsUtilities.h"
#include "otbImageFileWriter.h"
#include <fstream>
#include <iostream>

int
vnsListOfStringToImageSourceTest(int /*argc*/, char * argv[])
{
    typedef otb::Image<double> ImageType;
    typedef vns::ListOfStringToImageSource<ImageType> FilterType;
    typedef vns::Utilities::ListOfStrings ListOfStrings;
    typedef otb::ImageFileWriter<ImageType> WriterType;

    ImageType::PointType l_Origin;
    ImageType::SpacingType l_Spacing;

    FilterType::Pointer filter = FilterType::New();
    WriterType::Pointer writer = WriterType::New();

    const char * l_ASCIIheader = argv[1];
    l_Origin.Fill(atof(argv[2]));
    l_Spacing.Fill(atof(argv[3]));
    const char * l_OutputFilename = argv[4];

    std::ifstream inputfile(l_ASCIIheader);

    ListOfStrings list_of_lines;
    std::string line;
    while (std::getline(inputfile, line))
    {
        list_of_lines.push_back(line);
    }
    std::cout <<" Nb Lignes : "<<list_of_lines.size()<<std::endl;

    filter->SetListOfString(list_of_lines, false /* p_ActivateDataExtrapolation */);
    filter->SetOrigin(l_Origin);
    filter->SetSpacing(l_Spacing);
    filter->Update();

    std::cout << "Filter: "<< filter<< std::endl;

    writer->SetFileName(l_OutputFilename);
    writer->SetInput(filter->GetOutput());
    writer->Update();

    return EXIT_SUCCESS;
}
