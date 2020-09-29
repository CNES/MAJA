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

#if defined(_MSC_VER)
   //Warning about: identifier was truncated to '255' characters in the debug information (MVC6.0 Debug)
#pragma warning( disable : 4786 )
#endif

#include <iostream>
#include "vnsVectorLookUpTable.h"
#include "vnsLoggers.h"

int vnsVectorLookUpTableLUT3( int /*argc*/, char * argv[] )
{
    std::cout << "Testing vns::VectorLookUpTable with values" << std::endl;

    // Configure Logger
    vns::Loggers::GetInstance()->Initialize(argv[0]);
    vns::Loggers::GetInstance()->SetMinLevel(vns::LoggerBase::DEBUG);

    typedef vns::VectorLookUpTable<double, 3> LUTType;

    LUTType::Pointer R_TestLUT = LUTType::New();


    std::cout << "First parameter : ThetaS" << std::endl;
    LUTType::ParameterValuesType values;
    values.ParameterName = "ThetaS";
    values.ParameterValues.push_back(0.0000);
    values.ParameterValues.push_back(0.0800);
    values.ParameterValues.push_back(0.1600);
    values.ParameterValues.push_back(0.2400);
    R_TestLUT->AddParameterValues(values);

    std::cout << "Second parameter : ThetaV" << std::endl;
    values.ParameterName = "ThetaV";
    values.ParameterValues.clear();
    values.ParameterValues.push_back(0.2500);
    values.ParameterValues.push_back(0.3125);
    values.ParameterValues.push_back(0.3750);
    values.ParameterValues.push_back(0.4375);
    R_TestLUT->AddParameterValues(values);

    std::cout << "Third parameter : DeltaPHI" << std::endl;
    values.ParameterName = "DeltaPHI";
    values.ParameterValues.clear();
    values.ParameterValues.push_back(0.0);
    values.ParameterValues.push_back(30.0);
    values.ParameterValues.push_back(60.0);
    values.ParameterValues.push_back(90.0);
    R_TestLUT->AddParameterValues(values);

    LUTType::ParametersValuesType getValues;
    getValues = R_TestLUT->GetParametersValues();

    if (fabs(getValues[2].ParameterValues[2] - 60.0) > 1e-4)
    {
        return EXIT_FAILURE;
    }

    std::cout << "Value[2][2] = 60.0 : OK" << std::endl;

    return EXIT_SUCCESS;
}
