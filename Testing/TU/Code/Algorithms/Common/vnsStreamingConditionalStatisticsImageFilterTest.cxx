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
 * $Id: vnsStreamingConditionalStatisticsVectorImageFilterTest.cxx 8615 2014-08-01 10:45:09Z tfeuvrie $
 *                                                                                                          *
 ************************************************************************************************************/

#include "vnsLoggers.h"
#include "vnsStreamingConditionalStatisticsImageFilter.h"
#include "otbVectorImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"

int vnsStreamingConditionalStatisticsImageFilterTest(int argc, char * argv [])
{
    if(argc!=4 && argc!=5)
    {
    	std::cerr << argv[0] << " <input filename> <output filename> <excluded value>" << std::endl;
        return EXIT_FAILURE;
    }
    // Configure Logger
    vns::Loggers::GetInstance()->Initialize(argv[0]);
    vns::Loggers::GetInstance()->SetMinLevel(vns::LoggerBase::DEBUG);

    const unsigned int Dimension = 2;
    unsigned int cpt(1);
    const char * inputFileName  = argv[cpt++];
    std::string inputMaskFilename;
    bool l_UseMask(false);
    if( argc == 5)
    {
        inputMaskFilename = argv[cpt++];
        l_UseMask = true;
    }
    const char * outputFileName = argv[cpt++];
    
    /** Pixel typedefs */
    typedef double InputPixelType;
    /** Image typedefs */
    typedef otb::Image<InputPixelType,Dimension>    InputImageType;
    typedef otb::ImageFileReader<InputImageType>    ReaderType;

    typedef otb::ImageFileReader<otb::Image<unsigned char,Dimension> >    MaskReaderType;

    InputPixelType lExcludedValue = static_cast<InputPixelType>(atof(argv[cpt++]));


    typedef vns::StreamingConditionalStatisticsImageFilter<InputImageType>  FilterType;

    /** instantiating the filter */
    FilterType::Pointer filter = FilterType::New();
    ReaderType::Pointer reader  = ReaderType::New();
    MaskReaderType::Pointer mask_reader  = MaskReaderType::New();
    reader->SetFileName(inputFileName);
    if (l_UseMask == true)
    {
        mask_reader->SetFileName(inputMaskFilename.c_str());
        filter->SetMaskInput(mask_reader->GetOutput());
        filter->SetEnableExcludeValue(false);
    }
    else
    {
    	filter->SetEnableExcludeValue(true);
    }
    filter->SetEnableVariance(true);
    filter->SetInput(reader->GetOutput());
    filter->SetExcludedValue(lExcludedValue);

    filter->SetMaskForegroundValue(lExcludedValue);
    filter->Update();



    std::ofstream file;
    file.open(outputFileName);
    file <<"Minimum: "<<filter->GetMinimum()<<std::endl;
    file<<"Maximum: "<<filter->GetMaximum()<<std::endl;
    file<<std::fixed;
    file.precision(5);
    file<<"Sum: "<<filter->GetSum()<<std::endl;
    file<<"Mean: "<<filter->GetMean()<<std::endl;
    file<<"Variance: "<<filter->GetVariance()<<std::endl;
    file<<"StandardDeviation: "<<filter->GetStandardDeviation()<<std::endl;
//    if (filter->GetEnableSecondOrderStats() == true)
//    	file<<"Correl: "<<filter->GetCorrelation()<<std::endl;
//		file<<"Covariance: "<<filter->GetCovariance()<<std::endl;
	file<<"Number of valid pixels : "<<filter->GetNbOfValidValues()<<std::endl;
    file<<"Number of excluded pixels : "<<filter->GetNbOfExcludedValues()<<std::endl;
    file << "MaskForegroundValue: " << (int)filter->GetMaskForegroundValue() << std::endl;
    file << "ExcludedValue: " << filter->GetExcludedValue() << std::endl;
    file<<"Number of excluded pixels : "<<filter->GetNbOfExcludedValues()<<std::endl;
    file.close();


    return EXIT_SUCCESS;
}
