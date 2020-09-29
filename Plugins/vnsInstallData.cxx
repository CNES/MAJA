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
 * VERSION : 2.0.0 : FA : LAIG-FA-MAJA-152790-CS : 23 fevrier 2017 : Correction qualite taux de commentaire *
 * VERSION : 1.0.0 : FA : LAIG-FA-MAC-1988-CNES : 14 octobre 2016 : Correction qualite (code mort)          *
 * VERSION : 5-1-0 : FA : LAIG-FA-MAC-145739-CS : 5 aout 2016 : Audit code - variable const                 *
 * VERSION : 4-4-0 : FA : LAIG-FA-MAC-127944-CS : 3 juin 2015 : Correction code pour la qualite             *
 * VERSION : 1-0-0 : <TypeFT> : <NumFT> : 1 janvier 2010 : Creation
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id: vnsSystemRasterize.cxx 3746 2012-01-31 15:51:20Z tfeuvrie $
 *                                                                                                          *
 ************************************************************************************************************/
#include <string>
#include <iostream>
#include <cstdlib>
#include <fstream>
#include "vnsSystem.h"
#include <itksys/SystemTools.hxx>

int
main(int argc, char * argv[])
{
    if (argc != 3)
    {
        std::cout << "Usage   : " << argv[0] << std::endl;
        return -1;
    }
    const std::string inputFilename(argv[1]);
    const std::string outputFilename(argv[2]);

    // The first argv is the input filename (full path) : it could be a .EEF or .HDR files or a .DBL.DIR directory
    // The second is the output directory to install the output file
    // The output file et the .EEF or the .HDR file and for the DBL.DIR, its the DBL compress file or the directory DBL.DIR.
    int code_return(0);

    // if not a directory, then it is a file
    if (itksys::SystemTools::FileIsDirectory(inputFilename.c_str()) == false)
    {
        // Try to copy
        const bool bresult = itksys::SystemTools::CopyFileAlways(inputFilename.c_str(), outputFilename.c_str());
        if (bresult == false)
        {
            // if failed trace it and update the return code
            std::cout << "Error while copy the inputfilename <" << inputFilename << "> to the destination filename  <" << outputFilename
                    << "> !!" << std::endl;
            code_return = 1;
        }
    }
    else
    {
        // it is a directory
        //Check if it's a DBL.DIR extention

        // Compress the directory in output directory
        const std::string basePath = itksys::SystemTools::GetFilenamePath(inputFilename);
        // base module is YYY.DBL.DIR
        const std::string baseModule = itksys::SystemTools::GetFilenameName(inputFilename);

        // add the outputdirectory + YYY.DBL
        const vns::System::ReturnStatus result = vns::System::CallCommandBasic(
                "tar -cjf " + outputFilename + " -C " + basePath + " " + baseModule + " --exclude '.svn'");
        if (result.first != 0)
        {
            // if failed trace it and update the return code
            std::cout << "Error while compress the inputfilename <" << outputFilename << "> from the directory <" << inputFilename << "> !!, Log of execution :"<<result.second
                    << std::endl;
            code_return = 2;
        }
    }

    return code_return;
}

