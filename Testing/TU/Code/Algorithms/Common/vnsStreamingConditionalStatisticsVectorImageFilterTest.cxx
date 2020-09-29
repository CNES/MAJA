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
 * VERSION : 1-0-0 : <TypeFT> : <NumFT> : 2 avr. 2010 : Creation                                                           
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id$
 *                                                                                                          *
 ************************************************************************************************************/

#include "vnsLoggers.h"
#include "vnsStreamingConditionalStatisticsVectorImageFilter.h"
#include "otbVectorImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"

int vnsStreamingConditionalStatisticsVectorImageFilterTest(int argc, char * argv [])
{
    if(argc!=4)
    {
    	std::cerr << argv[0] << " <input filename> <output filename> <excluded value>" << std::endl;
        return EXIT_FAILURE;
    }
    // Configure Logger
    vns::Loggers::GetInstance()->Initialize(argv[0]);
    vns::Loggers::GetInstance()->SetMinLevel(vns::LoggerBase::DEBUG);

    const unsigned int Dimension = 2;
    const char * inputFileName  = argv[1];
    const char * outputFileName = argv[2];
    
    /** Pixel typedefs */
    typedef double InputPixelType;

    InputPixelType lExcludedValue = static_cast<InputPixelType>(atof(argv[3]));

    /** Image typedefs */
    typedef otb::VectorImage<InputPixelType,Dimension>    InputImageType;
    typedef otb::ImageFileReader<InputImageType>    ReaderType;

    typedef vns::StreamingConditionalStatisticsVectorImageFilter<InputImageType>  FilterType;

    /** instantiating the filter */
    FilterType::Pointer filter = FilterType::New();
    ReaderType::Pointer reader  = ReaderType::New();
    reader->SetFileName(inputFileName);

    filter->SetInput(reader->GetOutput());
    filter->SetExcludedValue(lExcludedValue);
    filter->Update();



    std::ofstream file;
    file.open(outputFileName);
    file<<"Minimum: "<<filter->GetMinimum()<<std::endl;
    file<<"Maximum: "<<filter->GetMaximum()<<std::endl;
    file<<std::fixed;
    file.precision(5);
    file<<"Sum: "<<filter->GetSum()<<std::endl;
    file<<"Mean: "<<filter->GetMean()<<std::endl;
    if (filter->GetEnableSecondOrderStats() == true)
    	file<<"Correl: "<<filter->GetCorrelation()<<std::endl;
		file<<"Covariance: "<<filter->GetCovariance()<<std::endl;
	file<<"Number of valid pixels : "<<filter->GetNbOfValidValues()<<std::endl;
	file<<"Number of excluded pixels : "<<filter->GetNbOfExcludedValues()<<std::endl;
    file.close();


    return EXIT_SUCCESS;
}
