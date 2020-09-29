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
 * VERSION : 1-0-0 : <TypeFT> : <NumFT> : 18 juin 2010 : Creation                                                           
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id: vnsCloudLabelRemovalImageFilterTest.cxx 6888 2013-11-29 14:08:29Z tfeuvrie $
 *                                                                                                          *
 ************************************************************************************************************/
#include "vnsCloudLabelRemovalFunctor.h"
#include "itkUnaryFunctorImageFilter.h"
#include "otbImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "vnsLoggers.h"

#include <fstream>

int vnsCloudLabelRemovalImageFilterTest(int argc, char * argv [])
{
  if (argc != 4)
    {
      std::cerr << argv[0] << "<Shadow label image filename> "
              "<label list> <output label image filename>"<< std::endl;
      return EXIT_FAILURE;
    }

    // Configure Logger
    vns::Loggers::GetInstance()->Initialize(argv[0]);
    vns::Loggers::GetInstance()->SetMinLevel(vns::LoggerBase::DEBUG);

    /** Pixel typedefs */
    typedef unsigned int       LabelPixelType;
    /** Image typedefs */
    typedef otb::Image<LabelPixelType> LabelImageType;
    typedef otb::ImageFileReader<LabelImageType>  	   		LabelReaderType;
    typedef otb::ImageFileWriter<LabelImageType>           	LabelWriterType;

    typedef vns::Functor::CloudLabelRemovalFunctor<LabelPixelType>  FunctorType;
    typedef itk::UnaryFunctorImageFilter<LabelImageType, LabelImageType, FunctorType>	FilterType;


    typedef std::list<LabelPixelType>	LabelListType;

    LabelReaderType::Pointer labelReader  = LabelReaderType::New();
    
    labelReader->SetFileName(argv[1]);
    const char * listFileName  = argv[2];
    const char * outputFileName = argv[3];

    /** instantiating the filter */
    FilterType::Pointer filter = FilterType::New();


    LabelListType l_List;

	std::fstream l_File;

	l_File.open(listFileName, std::ios::in);
	if (l_File.fail())
	{
		std::cout << "CloudLabelRemovalImageFilter : Failed to open list file ! "<<listFileName <<std::endl;
	}

	// Read the input file to initialize the shadow map
	std::string line;

	// Offset
	std::getline(l_File, line);
	std::getline(l_File, line);

	// Loop the list
	while (std::getline(l_File, line))
	{
		if (line.empty() == false)
		{
			l_List.push_back(static_cast<LabelPixelType> (atoi(line.c_str())));
		}

	}

	l_File.close();

	filter->GetFunctor().SetLabelList(l_List);
    filter->SetInput( labelReader->GetOutput() );

    LabelWriterType::Pointer labelWriter  = LabelWriterType::New();

    labelWriter->SetFileName(outputFileName);
    labelWriter->SetInput(filter->GetOutput());

    labelWriter->Update();

    return EXIT_SUCCESS;
}
