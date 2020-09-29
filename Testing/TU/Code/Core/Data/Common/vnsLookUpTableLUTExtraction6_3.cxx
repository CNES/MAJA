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
 * VERSION : 1-0-0 : <TypeFT> : <NumFT> : 26 mars 2010 : Creation
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id$
 *                                                                                                          *
 ************************************************************************************************************/

#if defined(_MSC_VER)
//Warning about: identifier was truncated to '255' characters in the debug information (MVC6.0 Debug)
#pragma warning( disable : 4786 )
#endif

#include <iostream>
#include "vnsLookUpTable.h"
#include "vnsLookUpTableExtractor.h"
#include "otbMath.h"

int
vnsLookUpTableLUTExtraction6_3(int argc, char * argv[])
{

    if (argc != 8)
    {
        std::cout << "Usage   : " << argv[0]
                << " value1 value2 value3 value4 value5 value6 precision"
                << std::endl;
        std::cout << "value1 2 and 3 are used for extraction" << std::endl;
        std::cout << "value4 5 and 6 are used for interpolation in both luts"
                << std::endl;
        std::cout << "precision is used for checking results" << std::endl;

        std::cout << "Exemples: " << argv[0]
                << " 0.120 0.3126 31.0 1500.0 27.0 0.48 0.0000001"
                << std::endl;
        return EXIT_FAILURE;
    }

    std::cout << "Testing vns::LookUpTable sub-LUT extraction" << std::endl;

    // Configure Logger
    vns::Loggers::GetInstance()->Initialize(argv[0]);
    vns::Loggers::GetInstance()->SetMinLevel(vns::LoggerBase::DEBUG);

    typedef double TValue;
    typedef vns::LookUpTable<TValue, 6> LUTType;
    typedef LUTType::PointType FullPointType;

    typedef vns::LookUpTable<TValue, 3> ReducedLUTType;
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
    values.ParameterValues.push_back(120.0);
    values.ParameterValues.push_back(150.0);
    values.ParameterValues.push_back(180.0);
    Full_LUT->AddParameterValues(values);

    std::cout << "Fourth parameter : H" << std::endl;
    values.ParameterName = "H";
    values.ParameterValues.clear();
    values.ParameterValues.push_back(0.0);
    values.ParameterValues.push_back(1000.0);
    values.ParameterValues.push_back(2000.0);
    values.ParameterValues.push_back(3000.0);
    Full_LUT->AddParameterValues(values);

    std::cout << "Fifth parameter : AOT" << std::endl;
    values.ParameterName = "AOT";
    values.ParameterValues.clear();
    values.ParameterValues.push_back(0.0);
    values.ParameterValues.push_back(7.0);
    values.ParameterValues.push_back(14.0);
    values.ParameterValues.push_back(21.0);
    values.ParameterValues.push_back(28.0);
    values.ParameterValues.push_back(35.0000);
    values.ParameterValues.push_back(42.0000);
    values.ParameterValues.push_back(49.0000);
    values.ParameterValues.push_back(56.0000);
    values.ParameterValues.push_back(63.0000);
    values.ParameterValues.push_back(70.0000);
    Full_LUT->AddParameterValues(values);

    std::cout << "Sixth parameter : R_TOA" << std::endl;
    values.ParameterName = "R_TOA";
    values.ParameterValues.clear();
    values.ParameterValues.push_back(-0.2000);
    values.ParameterValues.push_back(-0.1300);
    values.ParameterValues.push_back(-0.0600);
    values.ParameterValues.push_back(0.0100);
    values.ParameterValues.push_back(0.0800);
    values.ParameterValues.push_back(0.1500);
    values.ParameterValues.push_back(0.2200);
    values.ParameterValues.push_back(0.2900);
    values.ParameterValues.push_back(0.3600);
    values.ParameterValues.push_back(0.4300);
    values.ParameterValues.push_back(0.5000);
    values.ParameterValues.push_back(0.5700);
    values.ParameterValues.push_back(0.6400);
    values.ParameterValues.push_back(0.7100);
    values.ParameterValues.push_back(0.7800);
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
    TValue value = 0;
    for (int i = 0; i < size[0]; i++)
    {
        lutIndex[0] = i;
        for (int j = 0; j < size[1]; j++)
        {
            lutIndex[1] = j;
            for (int k = 0; k < size[2]; k++)
            {
                lutIndex[2] = k;

                for (int l = 0; l < size[3]; l++)
                {
                    lutIndex[3] = l;
                    for (int m = 0; m < size[4]; m++)
                    {
                        lutIndex[4] = m;
                        for (int n = 0; n < size[5]; n++)
                        {
                            lutIndex[5] = n;
                            // Compute value (constant in miniLUT for test)
                            Full_LUT->SetValue(lutIndex, value);
                            value++;
                        }
                        value += 10;
                    }
                    value += 100;
                }
                value += 1000;
            }
            value += 10000;
        }
        value += 100000;
    }

    // Extraction of dimension 2 sub-LUT

    ExtractPointType point;
    point[0] = atof(argv[1]);
    point[1] = atof(argv[2]);
    point[2] = atof(argv[3]);

    std::cout << "Avant extraction" << std::endl;
    ReducedLUTType::Pointer Sub_LUT = LUTExtractorType::ExtractLUT(
            Full_LUT.GetPointer(), point);
    std::cout << "Apres extraction" << std::endl;

    // Test interpolation (full vs reduced)
    FullPointType FullPoint;
    FullPoint[0] = point[0];
    FullPoint[1] = point[1];
    FullPoint[2] = point[2];

    ReducedPointType ReducedPoint;

    double value1 = atof(argv[4]);
    FullPoint[3] = value1;
    ReducedPoint[0] = value1;

    double value2 = atof(argv[5]);
    FullPoint[4] = value2;
    ReducedPoint[1] = value2;

    double value3 = atof(argv[6]);
    FullPoint[5] = value3;
    ReducedPoint[2] = value3;

    double FullInterp = Full_LUT->InterpolateValue(FullPoint);

    double ReducedInterp = Sub_LUT->InterpolateValue(ReducedPoint);

    double precision = atof(argv[7]);

    std::cout << "precision" << precision << std::endl;

    if (vcl_abs(FullInterp - ReducedInterp) > precision)
    {
        std::cerr << "Ecart a l'extraction :" << std::endl;
        std::cerr << "Full LUT    : " << FullInterp << std::endl;
        std::cerr << "Reduced LUT : " << ReducedInterp << std::endl;

        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
