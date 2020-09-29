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

#include "itkMacro.h"
#include "vnsLoggers.h"
#include "vnsWaterAmountImageGenerator.h"
#include "otbVectorImage.h"
#include "otbImage.h"
#include "vnsLookUpTable.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "itkImageFileWriter.h"

int
vnsWaterAmountImageGeneratorTest(int argc, char * argv[])
{
    if (argc != 3)
    {
        std::cerr << argv[0] << " <TOA input filename> <output filename>" << std::endl;
        return EXIT_FAILURE;
    }
    // Configure Logger
    vns::Loggers::GetInstance()->Initialize(argv[0]);
    vns::Loggers::GetInstance()->SetMinLevel(vns::LoggerBase::DEBUG);

    const char * toaFilename = argv[1];
    const char * outFilename = argv[2];

    typedef double PixelType;
    typedef otb::VectorImage<PixelType> VectorInpuImageType;
    typedef otb::Image<PixelType> ImageType;
    typedef vns::LookUpTable<PixelType, 2> LUTType;
    typedef otb::ImageFileReader<VectorInpuImageType> VectorReaderType;
    typedef otb::ImageFileReader<ImageType> ReaderType;
    typedef otb::ImageFileWriter<ImageType> WriterType;

    typedef vns::WaterAmountImageGenerator<VectorInpuImageType, ImageType, LUTType> WaterAmountImageGeneratorType;

    // Readers creation
    VectorReaderType::Pointer toaReader = VectorReaderType::New();
    toaReader->SetFileName(toaFilename);
    std::cout << toaFilename << std::endl;

    // LUT creation
    LUTType::Pointer lut = LUTType::New();

    // toa1/toa2 parameter
    LUTType::ParameterValuesType values;
    values.ParameterName = "TOA";
    values.ParameterValues.push_back(0.0000);
    values.ParameterValues.push_back(1.);
    values.ParameterValues.push_back(5.1600);
    values.ParameterValues.push_back(20.2400);
    values.ParameterValues.push_back(100.2400);
    values.ParameterValues.push_back(1000.2400);
    values.ParameterValues.push_back(10000.2400);
    lut->AddParameterValues(values);

    // Altitude
    values.ParameterName = "Altitude";
    values.ParameterValues.clear();
    values.ParameterValues.push_back(0);
    values.ParameterValues.push_back(10);
    values.ParameterValues.push_back(150);
    values.ParameterValues.push_back(500);
    values.ParameterValues.push_back(1000);
    values.ParameterValues.push_back(5000);
    lut->AddParameterValues(values);

    // Load values
    LUTType::ParametersValuesType getValues;
    getValues = lut->GetParametersValues();

    int size[getValues.size()];
    for (unsigned int i = 0; i < getValues.size(); i++)
    {
        size[i] = getValues[i].ParameterValues.size();
    }

    // Parcours de la LUT et chargement
    LUTType::IndexType lutIndex;
    for (int i = 0; i < size[0]; i++)
    {
        lutIndex[0] = i;
        for (int j = 0; j < size[1]; j++)
        {
            lutIndex[1] = j;
            // Compute value (constant in miniLUT for test)
            double value = 10 * i + j;
            lut->SetValue(lutIndex, value);
        }
    }

////  typedef itk::ImageFileWriter<LUTType>           LUTWriterType;
//  typedef otb::ImageFileWriter<LUTType>           LUTWriterType;
//  LUTWriterType::Pointer wlut = LUTWriterType::New();
//  wlut->SetFileName("lol.tif");
//  wlut->SetInput(lut);
//  wlut->Update();

// Filter instanciation
    WaterAmountImageGeneratorType::Pointer wator = WaterAmountImageGeneratorType::New();

    wator->SetIPTOAInput(toaReader->GetOutput());
    wator->SetLUT(lut);
    wator->SetWaterVaporReferenceBandId(1);
    wator->SetWaterVaporBandId(2);

    // Writer
    WriterType::Pointer writer = WriterType::New();
    writer->SetFileName(outFilename);
    writer->SetInput(wator->GetOutput());
    writer->Update();

    return EXIT_SUCCESS;
}
