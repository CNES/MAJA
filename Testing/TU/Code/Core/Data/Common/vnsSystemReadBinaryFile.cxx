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
 * $Id: vnsSystemRasterize.cxx 3746 2012-01-31 15:51:20Z tfeuvrie $
 *                                                                                                          *
 ************************************************************************************************************/
#if defined(_MSC_VER)
//Warning about: identifier was truncated to '255' characters in the debug information (MVC6.0 Debug)
#pragma warning( disable : 4786 )
#endif

#include <iostream>
#include <fstream>
#include <vector>
#include <cstdlib>
#include "vnsSystem.h"
#include "vnsLoggers.h"
#include "vnsUtilities.h"
#include "itkTimeProbe.h"

int
vnsSystemReadBinaryFile(int argc, char * argv[])
{
    if (argc != 4)
    {
        std::cout << "Usage   : " << argv[0] << std::endl;
    }
    // Configure Logger
    vns::Loggers::GetInstance()->Initialize(argv[0]);
    vns::Loggers::GetInstance()->SetMinLevel(vns::LoggerBase::SUPER_DEBUG);

    const std::string inputfilename(argv[1]);
    const std::string outputfilenameORIGIN(argv[2]);
    const std::string outputfilenameREADVECTOR(argv[3]);
    std::cout << "inputfilename:             " << inputfilename << std::endl;
    std::cout << "outputfilenameORIGIN:      " << outputfilenameORIGIN << std::endl;
    std::cout << "outputfilenameREADVECTOR:  " << outputfilenameREADVECTOR << std::endl;

    std::streamoff l_buffer_size(0);
    /* Warning necessary : not a pipeline */
    itk::TimeProbe clock;
    clock.Start();
    char * l_BuffersBands = vns::System::ReadBinaryFile(inputfilename, l_buffer_size);

    const size_t l_SizeVar(sizeof(float));
    vns::Utilities::ListOfFloats l_vf;

    const unsigned int l_NbValues(l_buffer_size / l_SizeVar);
    std::cout << "l_SizeVar:          " << l_SizeVar << std::endl;
    std::cout << "l_buffer_size:      " << l_buffer_size << std::endl;
    std::cout << "l_NbValues:         " << l_NbValues << std::endl;
    float var(0.);
    // Number of components per pixel
    for (unsigned int cpt = 0; cpt < l_buffer_size; cpt += l_SizeVar)
    {
        memcpy((void*) (&var), (const void*) &(l_BuffersBands[cpt]), l_SizeVar);
        l_vf.push_back(var);
    }

    clock.Stop();
    vnsLogDebugMacro("vns::System::ReadBinaryFile + convert done (time is "<<vns::Utilities::GetStringFormatedTime(clock.GetTotal())<<").")

//    vns::Utilities::PrintVector("V origin", l_vf, std::cout);

    std::ofstream flux(outputfilenameORIGIN.c_str());
    flux << std::setprecision(15);
    vns::Utilities::PrintVector("V", l_vf, flux);

    itk::TimeProbe clock2;
    clock2.Start();
    vns::Utilities::ListOfFloats l_vf2 = vns::System::ReadBinaryFileIntoVector<float>(inputfilename);

    clock2.Stop();
    vnsLogDebugMacro("vns::System::ReadBinaryFileIntoVectorValue (time is "<<vns::Utilities::GetStringFormatedTime(clock2.GetTotal())<<").")
    std::ofstream flux2(outputfilenameREADVECTOR.c_str());
    flux2 << std::setprecision(15);
    vns::Utilities::PrintVector("V", l_vf2, flux2);

    flux.close();
    flux2.close();

    delete[] l_BuffersBands;
    return EXIT_SUCCESS;
}

