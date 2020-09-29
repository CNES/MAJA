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
 * VERSION : 1-0-0 : <TypeFT> : <NumFT> : 24 mars 2010 : Creation                                                           
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id$
 *                                                                                                          *
 ************************************************************************************************************/

#include <iostream>
#include "vnsLookUpTable.h"
#include "vnsLookUpTableExtractor.h"
#include "otbMath.h"

int vnsLookUpTableLUTExtraction3_2( int argc, char * argv[] )
{

    if (argc != 5)
      {
        std::cout << "Usage   : " << argv[0]
            << " value1 value2 value3 precision"
            << std::endl;
        std::cout << "value1 is used for extraction" << std::endl;
        std::cout << "value2 and 3 are used for interpolation in both luts" << std::endl;
        std::cout << "precision is used for checking results" << std::endl;

        std::cout << "Exemples: " << argv[0] << " 0.120 0.3126 31.0 0.0000001" << std::endl;
        return EXIT_FAILURE;
      }

    std::cout << "Testing vns::LookUpTable sub-LUT extraction" << std::endl;

    // Configure Logger
    vns::Loggers::GetInstance()->Initialize(argv[0]);
    vns::Loggers::GetInstance()->SetMinLevel(vns::LoggerBase::DEBUG);

    typedef double TValue;
    typedef vns::LookUpTable<TValue, 3> LUTType;
    typedef LUTType::PointType FullPointType;

    typedef vns::LookUpTable<TValue, 2> ReducedLUTType;
    typedef ReducedLUTType::PointType ReducedPointType;

    typedef vns::LookUpTableExtractor<LUTType, ReducedLUTType> LUTExtractorType;
    typedef LUTExtractorType::ExtractPointType ExtractPointType;

    LUTType::Pointer Full_LUT = LUTType::New();


    std::cout << "First parameter : ThetaS" << std::endl;
    LUTType::ParameterValuesType values;
    values.ParameterName = "ThetaS";
    values.ParameterValues.push_back(0.0000);
    values.ParameterValues.push_back(0.0800);
    values.ParameterValues.push_back(0.1600);
    values.ParameterValues.push_back(0.2400);
    Full_LUT->AddParameterValues(values);

    std::cout << "Second parameter : ThetaV" << std::endl;
    values.ParameterName = "ThetaV";
    values.ParameterValues.clear();
    values.ParameterValues.push_back(0.2500);
    values.ParameterValues.push_back(0.3125);
    values.ParameterValues.push_back(0.3750);
    values.ParameterValues.push_back(0.4375);
    Full_LUT->AddParameterValues(values);

    std::cout << "Third parameter : DeltaPHI" << std::endl;
    values.ParameterName = "DeltaPHI";
    values.ParameterValues.clear();
    values.ParameterValues.push_back(0.0);
    values.ParameterValues.push_back(30.0);
    values.ParameterValues.push_back(60.0);
    values.ParameterValues.push_back(90.0);
    Full_LUT->AddParameterValues(values);


    // Load values
    LUTType::ParametersValuesType getValues;
    getValues = Full_LUT->GetParametersValues();

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
            for (int k = 0; k < size[2]; k++)
              {
                lutIndex[2] = k;
                // Compute value (constant in miniLUT for test)
                TValue value = 100 * i + 10 * j + k;
                Full_LUT->SetValue(lutIndex, value);
              }
          }
      }

    // Extraction of dimension 2 sub-LUT

    ExtractPointType point;
    point[0] = atof(argv[1]);

    std::cout << "Avant extraction" << std::endl;
    ReducedLUTType::Pointer Sub_LUT = LUTExtractorType::ExtractLUT(Full_LUT.GetPointer(), point);
    std::cout << "Apres extraction" << std::endl;


    // Test interpolation (full vs reduced)
    FullPointType FullPoint;
    FullPoint[0] = point[0];

    ReducedPointType ReducedPoint;

    double value1 = atof(argv[2]);
    FullPoint[1] = value1;
    ReducedPoint[0] = value1;

    double value2 = atof(argv[3]);
    FullPoint[2] = value2;
    ReducedPoint[1] = value2;

    double FullInterp = Full_LUT->InterpolateValue(FullPoint);

    double ReducedInterp = Sub_LUT->InterpolateValue(ReducedPoint);

    double precision = atof(argv[4]);
    if (vcl_abs(FullInterp - ReducedInterp) > precision)
    {
        std::cerr << "Ecart a l'extraction :" << std::endl;
        std::cerr << "Full LUT    : " << FullInterp << std::endl;
        std::cerr << "Reduced LUT : " << ReducedInterp << std::endl;

        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
