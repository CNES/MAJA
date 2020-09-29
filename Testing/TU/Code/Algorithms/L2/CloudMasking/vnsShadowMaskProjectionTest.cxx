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
 * VERSION : 1-0-0 : <TypeFT> : <NumFT> : 12 juil. 2010 : Creation                                                           
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id$
 *                                                                                                          *
 ************************************************************************************************************/


#include "vnsLoggers.h"
#include "vnsShadowMaskProjection.h"
#include "otbImage.h"
#include "otbVectorImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"

#include "itkConnectedComponentImageFilter.h"
#include "vnsUtilities.h"
#include "vnsConstantVectorImageSource.h"
#include "vnsConstantImageSource.h"

#include <fstream>

int vnsShadowMaskProjectionTest(int argc, char * argv [])
{
    if(argc!=3)
    {
        std::cerr << argv[0] << " <ShadowMap filename> <Output filename> "<< std::endl;
        return EXIT_FAILURE;
    }
    // Configure Logger
    vns::Loggers::GetInstance()->Initialize(argv[0]);
    vns::Loggers::GetInstance()->SetMinLevel(vns::LoggerBase::DEBUG);

    const unsigned int Dimension = 2;
    const char * l_ShadowMapFileName  = argv[1];
    const char * l_OutputFileName  = argv[2];



    /** Pixel typedefs */
    typedef unsigned short OutputPixelType;

    /** Image typedefs */
    typedef otb::Image<OutputPixelType,Dimension> OutputImageType;

    typedef otb::ImageFileWriter<OutputImageType>           WriterType;

    typedef vns::ShadowMaskProjection<OutputImageType>  FilterType;

    typedef unsigned long                          LinearIndex;
    typedef unsigned short                        LabelImagePixelType;
    typedef std::map<LinearIndex, LabelImagePixelType>    LabelMapType;


    /** instantiating the filter */
    FilterType::Pointer l_Filter = FilterType::New();
    WriterType::Pointer l_IPCLDShadWriter = WriterType::New();

    OutputImageType::SizeValueType l_Size[2];

    l_Size[0] = 375;
    l_Size[1] = 334;
    l_Filter->SetSize(l_Size);

    OutputImageType::SpacingValueType l_spacing[2] = {96., -96.};
    l_Filter->SetSpacing( l_spacing );

    OutputImageType::PointValueType l_origin[2] = {489336, 1845368};
    l_Filter->SetOrigin( l_origin );


    LabelMapType l_ShadowMap;

    std::fstream l_File;

    l_File.open(l_ShadowMapFileName, std::ios::in);
    if (l_File.fail())
    {
        std::cout << "ShadowMaskProjection : Failed to open map file ! "<<l_ShadowMapFileName <<std::endl;
    }

    // Read the input file to initialize the shadow map
    std::string line;

    // Offset
    std::getline(l_File, line);
    std::getline(l_File, line);

    // Loop the map
    while (std::getline(l_File, line))
     {

        std::string str(line);

        if (line.empty() == false)
        {
            vns::Utilities::ListOfStrings filenamekey = vns::Utilities::SplitString(str, ' ');

            //std::cout<<filenamekey[0].toInt()<<"      "<<filenamekey[1].toInt()<<std::endl;

            l_ShadowMap[static_cast<LinearIndex> (vns::Utilities::AsInt(filenamekey[0]))]
                    = static_cast<LabelImagePixelType> (vns::Utilities::AsInt(filenamekey[1]));
        }

    }

    l_File.close();

    l_Filter->SetShadowMap(l_ShadowMap);
    l_Filter->SetNumberOfThreads(1);

    // -------
    // Writer
    // -------
    l_IPCLDShadWriter->SetFileName(l_OutputFileName);
    l_IPCLDShadWriter->SetInput(l_Filter->GetIPCLDShadOutput());

    l_IPCLDShadWriter->Update();

    return EXIT_SUCCESS;
}
