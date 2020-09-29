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
 * VERSION : 1-0-0 : <TypeFT> : <NumFT> : 21 septembre 2010 : Creation
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id$
 *                                                                                                          *
 ************************************************************************************************************/


#include "vnsMapLookUpTableFileReader.h"
#include "vnsVectorLookUpTable.h"
#include "vnsUtilities.h"
#include "vnsLogger.h"

#include "itkImageFileWriter.h"
#include "otbImageFileWriter.h"

int
vnsMapLookUpTableFileReaderTest(int , char* argv[])
{




    typedef double                                      PixelType;
    typedef vns::VectorLookUpTable< PixelType, 6 >      LUTType;
    typedef LUTType::Pointer                            LUTPointerType;
    typedef vns::MapLookUpTableFileReader< LUTType >    ReaderType;

    typedef LUTType::ParameterValuesType ParameterValuesType;
    typedef std::map<unsigned int, LUTPointerType> LUTMapType;

    typedef vns::Utilities::ListOfFilenames ListOfFilenames;

    // Configure Logger
    vns::Loggers::GetInstance()->Initialize(argv[0]);
    vns::Loggers::GetInstance()->SetMinLevel(vns::LoggerBase::DEBUG);

    typedef otb::ImageFileReader<otb::VectorImage<double,2> > OReaderType;

    OReaderType::Pointer oreader = OReaderType::New();

    const char * inLUTFile0 = argv[2];
    oreader->SetFileName(argv[1]);
    oreader->UpdateOutputInformation();

    typedef itk::ImageFileWriter<otb::VectorImage<double, 2> > VWriterType;


    VWriterType::Pointer vwriter = VWriterType::New();
    vwriter->SetInput(oreader->GetOutput());
    vwriter->SetFileName(inLUTFile0);
    vwriter->Update();
    
    ReaderType::Pointer reader = ReaderType::New();
   
    //WriterType::Pointer writer = WriterType::New();

/**
 *
 * List of filenames creation
 *
 */


    ListOfFilenames lLUTFilenames;
    lLUTFilenames.push_back(inLUTFile0);
    lLUTFilenames.push_back(inLUTFile0);


    /* Read the parameters LUT_indexes of the HDR file */
    std::vector<double> Solar_Zenith_Angle_Indexes;
    std::vector<double> Altitude_Indexes;
    std::vector<double> AOT_Indexes;

    // Read the parameters LUT_indexes of the HDR file
    Solar_Zenith_Angle_Indexes = vns::Utilities::SplitStringAsDouble("0.0000 7.0000 14.0000 21.0000 28.0000 35.0000 42.0000 49.0000 56.0000 63.0000 70.0000");
    Altitude_Indexes = vns::Utilities::SplitStringAsDouble("0 1000 2000 3000");
    AOT_Indexes = vns::Utilities::SplitStringAsDouble("0.0000 0.0625 0.1250 0.1875 0.2500 0.3125 0.3750 0.4375 0.5000 0.5625 0.6250 0.6875 0.7500 0.8125 0.8750 0.9375 1.0000 1.0625 1.1250 1.1875 1.2500 1.3125 1.3750 1.4375 1.5000");

    /* parameters are added one by one to the LUT */
    ParameterValuesType newParam1;
    newParam1.ParameterName = "Solar_Zenith_Angle_Indexes";
    newParam1.ParameterValues = Solar_Zenith_Angle_Indexes;

    ParameterValuesType newParam2;
    newParam2.ParameterName = "Altitude_Indexes";
    newParam2.ParameterValues = Altitude_Indexes;


    ParameterValuesType newParam3;
    newParam3.ParameterName="AOT_Indexes";
    newParam3.ParameterValues=AOT_Indexes;

    // Set the List of filenames
    reader->SetListOfFilenames(lLUTFilenames,"19840101");
    // Add the parameters index value
    reader->AddParameterValues(newParam1);
    reader->AddParameterValues(newParam2);
    reader->AddParameterValues(newParam3);

    // Load the file
    reader->GenerateLUTMap();

    LUTMapType lLUTMap = reader->GetLUTMap();

    return EXIT_SUCCESS;
  }
