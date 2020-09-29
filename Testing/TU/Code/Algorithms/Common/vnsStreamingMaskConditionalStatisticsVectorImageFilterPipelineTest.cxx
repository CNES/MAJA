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
 * $Id: vnsStreamingMaskConditionalStatisticsVectorImageFilterTest.cxx 6888 2013-11-29 14:08:29Z tfeuvrie $
 *                                                                                                          *
 ************************************************************************************************************/

#include "vnsLoggers.h"
#include "vnsStreamingMaskConditionalStatisticsVectorImageFilter.h"
#include "otbImageFileReader.h"

#include "otbImage.h"
#include "otbVectorImage.h"
#include "otbImageFileWriter.h"

#include "itkOrImageFilter.h"

int vnsStreamingMaskConditionalStatisticsVectorImageFilterPipelineTest(int argc, char * argv [])
{
    if(argc!=6)
    {
        std::cerr << argv[0] << " <input filename> <mask1 filename> <mask2 filename> <mask3 filename> <output filename>" << std::endl;
        return EXIT_FAILURE;
    }
    // Configure Logger
    vns::Loggers::GetInstance()->Initialize(argv[0]);
    vns::Loggers::GetInstance()->SetMinLevel(vns::LoggerBase::DEBUG);

    const unsigned int Dimension = 2;
    const char * inputFileName  = argv[1];
    const char * cloudMaskFileName  = argv[2];
    const char * waterMaskFileName  = argv[3];
    const char * noDataMaskFileName  = argv[4];
    const char * outputFileName = argv[5];
    
    /** Pixel typedefs */
    typedef short InputPixelType;

    /** Image typedefs */
    typedef otb::VectorImage<InputPixelType,Dimension>    InputImageType;
    typedef otb::Image<InputPixelType,Dimension>           InputMaskType;
    typedef otb::ImageFileReader<InputImageType>    ReaderType;
    typedef otb::ImageFileReader<InputMaskType>     MaskReaderType;

    typedef itk::OrImageFilter<InputMaskType,InputMaskType,InputMaskType> OrFilterType;
    typedef vns::StreamingMaskConditionalStatisticsVectorImageFilter<InputImageType,InputMaskType>  FilterType;

    /** instantiating the filter */
    FilterType::Pointer filter = FilterType::New();
    ReaderType::Pointer reader  = ReaderType::New();
    OrFilterType::Pointer orFilter = OrFilterType::New();
    OrFilterType::Pointer orFilter2 = OrFilterType::New();
    MaskReaderType::Pointer cloudMaskReader  = MaskReaderType::New();
    MaskReaderType::Pointer waterMaskReader  = MaskReaderType::New();
    MaskReaderType::Pointer noDataMaskReader  = MaskReaderType::New();

    reader->SetFileName(inputFileName);

    cloudMaskReader->SetFileName(cloudMaskFileName);
    waterMaskReader->SetFileName(waterMaskFileName);
    noDataMaskReader->SetFileName(noDataMaskFileName);

    orFilter->SetInput1(cloudMaskReader->GetOutput());
    orFilter->SetInput2(waterMaskReader->GetOutput());

    orFilter2->SetInput1(orFilter->GetOutput());
    orFilter2->SetInput2(noDataMaskReader->GetOutput());

    filter->SetInput(reader->GetOutput());
    filter->SetMaskInput(orFilter2->GetOutput());
    filter->SetEnableExcludeValue(false);
    filter->SetEnableStandardDeviation(true);
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
    	file<<"Correlation : "<<filter->GetCorrelation()<<std::endl;
    	file<<"Covariance: "<<filter->GetCovariance()<<std::endl;
    file<<"Number of valid pixels: "<<filter->GetNbOfValidValues()<<std::endl;
    file<<"Number of excluded pixels: "<<filter->GetNbOfExcludedValues()<<std::endl;
    file<<"IsValid: "<<filter->GetIsValid()<<std::endl;
    file.close();


    return EXIT_SUCCESS;
}
