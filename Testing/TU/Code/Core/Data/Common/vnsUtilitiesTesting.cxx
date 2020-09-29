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
 * VERSION : 1-0-3 : DM : 310 : 13 septembre 2011 : Creation                                                *
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id: vnsUtilitiesUTCDate.cxx 3205 2011-09-14 17:16:24Z tfeuvrie $
 *                                                                                                          *
 ************************************************************************************************************/
#include "vnsUtilities.h"
#include <iostream>
#include <iomanip>
#include <fstream>
#include <cstdlib>
#include "vcl_cmath.h"
#include <iterator>
#include <algorithm>

int
vnsUtilitiesTesting(int /*argc*/, char * /*argv*/[])
{
//    if (argc < 3)
//    {
//        std::cout << "Usage   : " << argv[0] << " UTCDate outputJulianDay" << std::endl;
//        return EXIT_FAILURE;
//    }

    vns::Utilities::SetEnv("TOTO", "DEDE");
    std::cout << vns::Utilities::GetEnv("TOTO")<< std::endl;

    vns::Utilities::ListOfStrings l_LTCProductMHAFilename;
    vns::Utilities::ListOfStrings l_LTCProductMHAFilenameInDisc;

    l_LTCProductMHAFilenameInDisc.push_back("1");
    l_LTCProductMHAFilenameInDisc.push_back("2");
    l_LTCProductMHAFilenameInDisc.push_back("3");
    l_LTCProductMHAFilenameInDisc.push_back("8");
    l_LTCProductMHAFilenameInDisc.push_back("7");
    l_LTCProductMHAFilenameInDisc.push_back("0");
    l_LTCProductMHAFilenameInDisc.push_back("9");

    l_LTCProductMHAFilename.push_back("3");
    l_LTCProductMHAFilename.push_back("8");
    l_LTCProductMHAFilename.push_back("7");


    std::sort(l_LTCProductMHAFilename.begin(),  l_LTCProductMHAFilename.end());
    std::sort(l_LTCProductMHAFilenameInDisc.begin(),  l_LTCProductMHAFilenameInDisc.end());
    vns::Utilities::ListOfStrings l_DeltaMha;
    std::set_difference(l_LTCProductMHAFilenameInDisc.begin(),  l_LTCProductMHAFilenameInDisc.end(), l_LTCProductMHAFilename.begin(),  l_LTCProductMHAFilename.end(), std::back_inserter(l_DeltaMha));

    vns::Utilities::PrintVector("l_LTCProductMHAFilename", l_LTCProductMHAFilename, std::cout);
    vns::Utilities::PrintVector("l_LTCProductMHAFilenameInDisc", l_LTCProductMHAFilenameInDisc, std::cout);
    vns::Utilities::PrintVector("l_DeltaMha", l_DeltaMha, std::cout);

    return EXIT_SUCCESS;
}
