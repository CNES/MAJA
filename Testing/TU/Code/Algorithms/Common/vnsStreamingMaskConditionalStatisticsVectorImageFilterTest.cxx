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
 * VERSION : 1-0-0 : <TypeFT> : <NumFT> : 19 avr. 2010 : Creation                                                           
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id$
 *                                                                                                          *
 ************************************************************************************************************/

#include "vnsLoggers.h"
#include "vnsStreamingMaskConditionalStatisticsVectorImageFilter.h"
#include "otbImageFileReader.h"

#include "otbImage.h"
#include "otbVectorImage.h"
#include "otbImageFileWriter.h"

#include "itkOrImageFilter.h"

int vnsStreamingMaskConditionalStatisticsVectorImageFilterTest(int argc, char * argv [])
{
    if(argc!=5)
    {
        std::cerr << argv[0] << " <input filename> <mask1 filename> <foreground value> <output filename>" << std::endl;
        return EXIT_FAILURE;
    }
    // Configure Logger
    vns::Loggers::GetInstance()->Initialize(argv[0]);
    vns::Loggers::GetInstance()->SetMinLevel(vns::LoggerBase::DEBUG);

    const unsigned int Dimension = 2;
    const char * inputFileName  = argv[1];
    const char * maskFileName  = argv[2];
    const unsigned int foreground_value = atoi(argv[3]);
    const char * outputFileName = argv[4];
    
    /** Pixel typedefs */
    typedef float InputPixelType;
    typedef unsigned char MaskPixelType;

    /** Image typedefs */
    typedef otb::VectorImage<InputPixelType,Dimension>    InputImageType;
    typedef otb::Image<MaskPixelType,Dimension>           InputMaskType;
    typedef otb::ImageFileReader<InputImageType>    ReaderType;
    typedef otb::ImageFileReader<InputMaskType>     MaskReaderType;

    typedef vns::StreamingMaskConditionalStatisticsVectorImageFilter<InputImageType,InputMaskType>  FilterType;

    /** instantiating the filter */
    FilterType::Pointer filter = FilterType::New();
    ReaderType::Pointer reader  = ReaderType::New();
    MaskReaderType::Pointer maskReader  = MaskReaderType::New();

    reader->SetFileName(inputFileName);

    maskReader->SetFileName(maskFileName);

    filter->SetInput(reader->GetOutput());
    filter->SetMaskInput(maskReader->GetOutput());
    filter->SetEnableStandardDeviation(true);
    filter->SetForegroundValue(foreground_value);

    filter->Update();

    std::ofstream file;
    file.open(outputFileName);
    file<<"Minimum: "<<filter->GetMinimum()<<std::endl;
    file<<"Maximum: "<<filter->GetMaximum()<<std::endl;
    file<<std::fixed;
    file.precision(5);
    file<<"Sum: "<<filter->GetSum()<<std::endl;
    file<<"Mean: "<<filter->GetMean()<<std::endl;
    file<<"Standard deviation: "<<filter->GetStandardDeviation()<<std::endl;
    if (filter->GetEnableSecondOrderStats())
        {file<<"Correlation : "<<filter->GetCorrelation()<<std::endl;
    	file<<"Covariance: "<<filter->GetCovariance()<<std::endl;
    	}
    file<<"Number of valid pixels: "<<filter->GetNbOfValidValues()<<std::endl;
    file<<"Number of excluded pixels: "<<filter->GetNbOfExcludedValues()<<std::endl;
    file<<"IsValid: "<<filter->GetIsValid()<<std::endl;
    file.close();

    return EXIT_SUCCESS;
}
