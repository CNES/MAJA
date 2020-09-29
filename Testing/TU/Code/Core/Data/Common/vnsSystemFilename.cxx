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
 * VERSION : 1-0-0 : <TypeFT> : <NumFT> : 1 janvier 2010 : Creation
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id: vnsSystem.cxx 2961 2011-05-09 08:58:54Z tfeuvrie $
 *                                                                                                          *
 ************************************************************************************************************/
#if defined(_MSC_VER)
//Warning about: identifier was truncated to '255' characters in the debug information (MVC6.0 Debug)
#pragma warning( disable : 4786 )
#endif

#include <iostream>
#include <cstdlib>
#include "vnsSystem.h"
#include "itksys/SystemTools.hxx"

int
vnsSystemFilenameTest(int /*argc*/, char * argv[])
{
    unsigned int cpt(1);
    const char * inputFilename = argv[cpt++];

    setenv("VAR_ENV", "CONTENU", 1);
    std::cout << "     VAR_ENV: " << getenv("VAR_ENV") << std::endl;

//    ossimFilename ossimInputFilename = inputFilename;
    std::string stringInputFilename = inputFilename;
    std::string l_ConvertToUnixSlashes = stringInputFilename;

    std::cout << " -----------------------------------------" << std::endl;
    std::cout << "inputFilename: " << inputFilename << std::endl;
//    std::cout << "  OSSIM" << std::endl;
//    std::cout << "           no extension: " << ossimInputFilename.noExtension() << std::endl;
//    std::cout << "           extension: " << ossimInputFilename.ext() << std::endl;
//    std::cout << "           file: " << ossimInputFilename.file() << std::endl;
//    std::cout << "           path: " << ossimInputFilename.path() << std::endl;
//    std::cout << "           expand: " << ossimInputFilename.expand() << std::endl;
//    std::cout << " -----------------------------------------" << std::endl;
//    std::cout << "  OTHER" << std::endl;
    std::cout << "           GetFilenameWithoutExtension: " << itksys::SystemTools::GetFilenameLastExtension(stringInputFilename)
            << std::endl;
    std::cout << "           GetFilenameWithoutLastExtension: " << itksys::SystemTools::GetFilenameWithoutLastExtension(stringInputFilename)
            << std::endl;
    std::cout << "           GetFilenameExtension: " << itksys::SystemTools::GetFilenameExtension(stringInputFilename) << std::endl;
    std::cout << "           GetFilenameLastExtension: " << itksys::SystemTools::GetFilenameLastExtension(stringInputFilename) << std::endl;
    std::cout << "           GetFilenameName: " << itksys::SystemTools::GetFilenameName(stringInputFilename) << std::endl;
    std::cout << "           GetFilenamePath: " << itksys::SystemTools::GetFilenamePath(stringInputFilename) << std::endl;
    std::cout << "           GetRealPath: " << itksys::SystemTools::GetRealPath(stringInputFilename.c_str()) << std::endl;
    std::cout << "           ConvertToOutputPath: " << itksys::SystemTools::ConvertToOutputPath(stringInputFilename.c_str()) << std::endl;
    itksys::SystemTools::ConvertToUnixSlashes(l_ConvertToUnixSlashes);
    std::cout << "           ConvertToUnixSlashes: " <<  l_ConvertToUnixSlashes<< std::endl;
    vns::Utilities::ListOfStrings l_ListOfStrings;
    std::string l_string2;
    itksys::SystemTools::SplitPathRootComponent(stringInputFilename.c_str(), &l_string2);
//    vns::Utilities::PrintVector(l_ListOfStrings, std::cout);
    std::cout << "           SplitPathRootComponent: " <<  l_string2<< std::endl;
    //SplitPath
    std::cout << "           vns::Utilities::Expand: " << vns::Utilities::Expand(stringInputFilename) << std::endl;


    std::string filename2 = "~/tmp/${VAR_ENV}/../${TOTO}/dede.txt";
//    std::string filename2 = "~/tmp/../dede.txt";
//    ossimInputFilename = filename2;
    stringInputFilename = filename2;
    l_ConvertToUnixSlashes = filename2;
    std::cout << " -----------------------------------------" << std::endl;
    std::cout << "inputFilename: " << filename2 << std::endl;
//    std::cout << "  OSSIM" << std::endl;
//    std::cout << "           no extension: " << ossimInputFilename.noExtension() << std::endl;
//    std::cout << "           extension: " << ossimInputFilename.ext() << std::endl;
//    std::cout << "           file: " << ossimInputFilename.file() << std::endl;
//    std::cout << "           path: " << ossimInputFilename.path() << std::endl;
//    std::cout << "           expand: " << ossimInputFilename.expand() << std::endl;
    std::cout << " -----------------------------------------" << std::endl;
    std::cout << "  OTHER" << std::endl;
    std::cout << "           GetFilenameWithoutExtension: " << itksys::SystemTools::GetFilenameLastExtension(stringInputFilename)
            << std::endl;
    std::cout << "           GetFilenameWithoutLastExtension: " << itksys::SystemTools::GetFilenameWithoutLastExtension(stringInputFilename)
            << std::endl;
    std::cout << "           GetFilenameExtension: " << itksys::SystemTools::GetFilenameExtension(stringInputFilename) << std::endl;
    std::cout << "           GetFilenameLastExtension: " << itksys::SystemTools::GetFilenameLastExtension(stringInputFilename) << std::endl;
    std::cout << "           GetFilenameName: " << itksys::SystemTools::GetFilenameName(stringInputFilename) << std::endl;
    std::cout << "           GetFilenamePath: " << itksys::SystemTools::GetFilenamePath(stringInputFilename) << std::endl;
    std::cout << "           GetRealPath: " << itksys::SystemTools::GetRealPath(stringInputFilename.c_str()) << std::endl;
    std::cout << "           ConvertToOutputPath: " << itksys::SystemTools::ConvertToOutputPath(stringInputFilename.c_str()) << std::endl;
    itksys::SystemTools::ConvertToUnixSlashes(l_ConvertToUnixSlashes);
    std::cout << "           ConvertToUnixSlashes: " <<  l_ConvertToUnixSlashes<< std::endl;
    std::cout << "           vns::Utilities::Expand: " << vns::Utilities::Expand(stringInputFilename) << std::endl;

    std::string filename3 = "~/tmp/${VAR_ENV}/dede.txt";
//    ossimInputFilename = filename3;
    stringInputFilename = filename3;
    l_ConvertToUnixSlashes = filename3;

    std::cout << " -----------------------------------------" << std::endl;
    std::cout << "inputFilename: " << filename3 << std::endl;
//    std::cout << "  OSSIM" << std::endl;
//    std::cout << "           no extension: " << ossimInputFilename.noExtension() << std::endl;
//    std::cout << "           extension: " << ossimInputFilename.ext() << std::endl;
//    std::cout << "           file: " << ossimInputFilename.file() << std::endl;
//    std::cout << "           path: " << ossimInputFilename.path() << std::endl;
//    std::cout << "           expand: " << ossimInputFilename.expand() << std::endl;
    std::cout << " -----------------------------------------" << std::endl;
    std::cout << "  OTHER" << std::endl;
    std::cout << "           GetFilenameWithoutExtension: " << itksys::SystemTools::GetFilenameLastExtension(stringInputFilename)
            << std::endl;
    std::cout << "           GetFilenameWithoutLastExtension: " << itksys::SystemTools::GetFilenameWithoutLastExtension(stringInputFilename)
            << std::endl;
    std::cout << "           GetFilenameExtension: " << itksys::SystemTools::GetFilenameExtension(stringInputFilename) << std::endl;
    std::cout << "           GetFilenameLastExtension: " << itksys::SystemTools::GetFilenameLastExtension(stringInputFilename) << std::endl;
    std::cout << "           GetFilenameName: " << itksys::SystemTools::GetFilenameName(stringInputFilename) << std::endl;
    std::cout << "           GetFilenamePath: " << itksys::SystemTools::GetFilenamePath(stringInputFilename) << std::endl;
    std::cout << "           GetRealPath: " << itksys::SystemTools::GetRealPath(stringInputFilename.c_str()) << std::endl;
    std::cout << "           ConvertToOutputPath: " << itksys::SystemTools::ConvertToOutputPath(stringInputFilename.c_str()) << std::endl;
    itksys::SystemTools::ConvertToUnixSlashes(l_ConvertToUnixSlashes);
    std::cout << "           ConvertToUnixSlashes: " <<  l_ConvertToUnixSlashes<< std::endl;
    std::cout << "           vns::Utilities::Expand: " << vns::Utilities::Expand(stringInputFilename) << std::endl;


//    ossimFilename refFileName(inputFilename);
//    ossimFilename drivePart("");
//    ossimFilename pathPart("");
//    ossimFilename filePart("");
//    ossimFilename extPart("");
//    refFileName.split(drivePart, pathPart, filePart, extPart);
//    std::cout << " -----------------------------------------" << std::endl;
//    std::cout << "inputFilename: " << filename2 << std::endl;
//    std::cout << "  OSSIM" << std::endl;
//    std::cout << "   ossimFilename split: " << std::endl;
//    std::cout << "           drivePart: " << drivePart << std::endl;
//    std::cout << "           pathPart: " << pathPart << std::endl;
//    std::cout << "           filePart: " << filePart << std::endl;
//    std::cout << "           extPart: " << extPart << std::endl;
//
//    std::cout << "  ossimString explode : " << std::endl;
//
//    // BUG OSSIM. Oblige de copier dans filePart2 avec c_str() car filePart est modifie par explode().
//    const ossimFilename filePart2 = filePart.c_str();
//    std::vector<ossimString> filenamekey = filePart.explode("_");
//    for (unsigned int i = 0; i < filenamekey.size(); i++)
//    {
//        std::cout << "           explode[]: " << filenamekey[i] << std::endl;
//    }
    std::cout << " -----------------------------------------" << std::endl;
    std::cout << "  OTHER" << std::endl;
    std::vector<std::string> filenamekey2;
    itksys::SystemTools::SplitPath(inputFilename,filenamekey2 );
    for (unsigned int i = 0; i < filenamekey2.size(); i++)
    {
        std::cout << "           SplitPath[]: " << filenamekey2[i] << std::endl;
    }
    std::string the_filename = itksys::SystemTools::GetFilenameName(inputFilename);
    filenamekey2.clear();
//    filenamekey2 = itksys::SystemTools::SplitString(the_filename.c_str());
    itksys::SystemTools::Split(the_filename.c_str(),filenamekey2,'_' );
    for (unsigned int i = 0; i < filenamekey2.size(); i++)
    {
        std::cout << "           Split[]: " << filenamekey2[i] << std::endl;
    }

    return EXIT_SUCCESS;
}

