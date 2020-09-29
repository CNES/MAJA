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
 * VERSION : 1-0-0 : <TypeFT> : <NumFT> : 18 nov. 2011 : Creation
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id$
 *                                                                                                          *
 ************************************************************************************************************/

#include "vnsDirectionalCorrectionCompute.h"

#include <iostream>
#include <fstream>
#include <iomanip>

int
vnsDirectionalCorrectionComputeTest(int /*argc*/, char * argv[])
{
    const char* outFilename = argv[1];

    const double theta_s =39.9999222;
    const double phi_s = 156.748;
    const double theta_v0 = 0.0;
    const double phi_v0 =0.0;
    double theta_v_arr[] = {6.192296,6.192296,6.192296,6.192296,6.192296,6.192296,6.192296,6.192296,
            6.192296,6.192296,6.192296,6.192296,6.192296};
    std::vector<double> theta_v(theta_v_arr,theta_v_arr + sizeof(theta_v_arr) / sizeof(double) );
    double phi_v_arr[] = {104.09984,104.09984,104.09984,104.09984,104.09984
            ,104.09984,104.09984,104.09984,104.09984,104.09984,104.09984,104.09984,104.09984};
    std::vector<double> phi_v(phi_v_arr,phi_v_arr + sizeof(phi_v_arr) / sizeof(double) );
    double coefs_R_arr[] = {0,0.480620155,0.4441041348,0.3396449704,0.4,0.5,0.7,
            0.4962819269,0.4962819269,0,0,0.3364431487,0.2404063205};
    std::vector<double> coefs_R(coefs_R_arr,coefs_R_arr + sizeof(coefs_R_arr) / sizeof(double) );
    double coefs_V_arr[] = {0,0.1020671835,0.1362940276,0.1343195266,0.25,0.2,0.15,0.1066925315,
            0.1066925315,0,0,0.1320699708,0.1455981941};
    std::vector<double> coefs_V(coefs_V_arr,coefs_V_arr + sizeof(coefs_V_arr) / sizeof(double) );
    //const unsigned int nb_band = 13;

    std::vector<double> coefs = vns::DirectionalCorrectionCompute::ComputeCorrectionCoeffs(theta_s,phi_s,
            theta_v0,phi_v0,theta_v, phi_v, coefs_R,coefs_V);

    std::fstream file(outFilename, std::fstream::out);
    for(unsigned int i = 0; i < coefs.size(); i++) {
        std::cout<<"Coeff "<<i<<" = "<<std::setprecision(9)<<coefs[i]<<std::endl;
        std::cout<<"thetha_v "<<i<<" = "<<std::setprecision(9)<<theta_v[i]<<std::endl;
        std::cout<<"phi_v "<<i<<" = "<<std::setprecision(9)<<phi_v[i]<<std::endl;
        std::cout<<"coefs_R "<<i<<" = "<<std::setprecision(9)<<coefs_R[i]<<std::endl;
        std::cout<<"coefs_V "<<i<<" = "<<std::setprecision(9)<<coefs_V[i]<<std::endl;
        file<<"Coeff "<<i<<" = "<<std::setprecision(9)<<coefs[i]<<std::endl;
        file<<"thetha_v "<<i<<" = "<<std::setprecision(9)<<theta_v[i]<<std::endl;
        file<<"phi_v "<<i<<" = "<<std::setprecision(9)<<phi_v[i]<<std::endl;
        file<<"coefs_R "<<i<<" = "<<std::setprecision(9)<<coefs_R[i]<<std::endl;
        file<<"coefs_V "<<i<<" = "<<std::setprecision(9)<<coefs_V[i]<<std::endl;
    }
    file.close();

    return EXIT_SUCCESS;
}

