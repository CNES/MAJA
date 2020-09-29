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
 * $Id: vnsShadowMaskProjectionTest.cxx 6888 2013-11-29 14:08:29Z tfeuvrie $
 *                                                                                                          *
 ************************************************************************************************************/


#include "vnsLoggers.h"
#include "vnsShadowMaskProjection.h"
#include "otbImage.h"
#include "otbImageFileReader.h"

#include "vnsNoDataShadowImageFilter.h"

#include "itkMultiThreader.h"

#include <fstream>
#include "vnsUtilities.h"

int vnsNoDataShadowImageFilterTest(int argc, char * argv [])
{
    if(argc!=4)
    {
        std::cerr << argv[0] << "<Input filename> <ShadowMap filename> <Output filename> "<< std::endl;
        return EXIT_FAILURE;
    }
    // Configure Logger
    vns::Loggers::GetInstance()->Initialize(argv[0]);
    vns::Loggers::GetInstance()->SetMinLevel(vns::LoggerBase::DEBUG);

    const unsigned int Dimension = 2;
    const char * l_InputFileName  = argv[1];
    const char * l_ShadowMapFileName  = argv[2];
    const char * l_OutputFileName  = argv[3];

    /** Pixel typedefs */
    typedef double InputPixelType;

    /** Image typedefs */
    typedef otb::Image<InputPixelType,Dimension> InputImageType;

    typedef vns::NoDataShadowImageFilter<InputImageType>  FilterType;

    typedef unsigned long                          LinearIndex;
    typedef unsigned short                         LabelType;
    typedef std::map<LinearIndex, LabelType>    LabelMapType;
    typedef std::list<LabelType>				LabelListType;

    typedef otb::ImageFileReader<InputImageType>	ReaderType;

    itk::MultiThreader::SetGlobalDefaultNumberOfThreads(1);


    // instantiating the filter
    FilterType::Pointer l_Filter = FilterType::New();
    ReaderType::Pointer l_Reader = ReaderType::New();

    LabelMapType l_ShadowMap;

    std::fstream l_File;

    l_File.open(l_ShadowMapFileName, std::ios::in);
    if (l_File.fail())
    {
        std::cout << "NoDataShadowImageFilter : Failed to open map file ! "<<l_ShadowMapFileName <<std::endl;
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
            vns::Utilities::ListOfStrings filenamekey = vns::Utilities::SplitString(str, ' ', true /*remove_empty*/ );

            l_ShadowMap[static_cast<LinearIndex> (vns::Utilities::AsInt(filenamekey[0]))]
                    = static_cast<LabelType> (vns::Utilities::AsInt(filenamekey[1]));
        }
    }

    l_File.close();

    l_Reader->SetFileName(l_InputFileName);

    l_Filter->SetDiffImage(l_Reader->GetOutput());
    l_Filter->SetShadowMap(l_ShadowMap);
    l_Filter->SetRealNoDataValue(static_cast<InputPixelType>(-10.));

    l_Filter->Update();

    LabelListType l_LabelKOList = l_Filter->GetLabelKOListOutput();
    unsigned long l_NumberOfLabels = l_Filter->GetNumberOfLabelsOutput();

    if (l_LabelKOList.size() == l_NumberOfLabels)
    {
    	std::cout<<" Tous les labels sont KO !"<<std::endl;
    }

	// Display the map
	std::cout << "====== Output List ======" << std::endl;
	LabelListType::const_iterator iter = l_LabelKOList.begin();
	// Loop the map
	while (iter != l_LabelKOList.end())
	{
		std::cout << " Label KO " << (*iter);
		std::cout << std::endl;
		iter++;
	}
	std::cout << std::endl;

	std::fstream outputFile(l_OutputFileName, std::ios::out);
	outputFile<<l_Filter;
	outputFile.close();

    return EXIT_SUCCESS;
}
