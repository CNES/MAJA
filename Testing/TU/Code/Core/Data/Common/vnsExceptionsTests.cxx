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
#include "vnsMacro.h"

int
vnsExceptionsClassTests(int /*argc*/, char * /*argv*/[])
{
    // Test the ExceptionData class
   try
    {
        ::vns::ExceptionData e_(__FILE__, __LINE__, "Message of a ExceptionData error", "TestLocation");
        std::cout << e_.GetDescription() << std::endl;
        throw e_;
    }
    catch (vns::ExceptionChain & e)
    {
        std::cerr << "Error : vns::ExceptionChain :"<<std::endl;
        std::cerr << e.GetFile() << ":" << e.GetLine() << ":"<< std::endl;
        std::cerr << e.GetDescription() << std::endl;
        return EXIT_FAILURE;
    }
    catch (vns::ExceptionBusiness & e)
    {
        std::cerr << "Error : vns::ExceptionBusiness :"<<std::endl;
        std::cerr << e.GetFile() << ":" << e.GetLine() << ":"<< std::endl;
        std::cerr << e.GetDescription() << std::endl;
        return EXIT_FAILURE;
    }
    catch (vns::ExceptionData & e)
    {
        std::cerr << "Error : vns::ExceptionData :"<<std::endl;
        std::cerr << e.GetFile() << ":" << e.GetLine() << ":"<< std::endl;
        std::cerr << e.GetDescription() << std::endl;
        return EXIT_SUCCESS;
    }
    catch (vns::ExceptionBase & e)
    {
        std::cerr << "Error : vns::ExceptionBase :"<<std::endl;
        std::cerr << e.GetFile() << ":" << e.GetLine() << ":"<< std::endl;
        std::cerr << e.GetDescription() << std::endl;
        return EXIT_FAILURE;
    }
    catch (itk::ExceptionObject & e)
    {
        std::cerr << "Error : itk::ExceptionObject :"<<std::endl;
        std::cerr << e.GetFile() << ":" << e.GetLine() << ":"<< std::endl;
        std::cerr << e.GetDescription() << std::endl;
        return EXIT_FAILURE;
    }
    catch (std::bad_alloc & err)
    {
        return EXIT_FAILURE;
    }
    catch (const std::exception& e)
    {
        return EXIT_FAILURE;
    }
    catch (...)
    {
        return EXIT_FAILURE;
    }


    // Test the ExceptionBusiness class
    try
    {
        ::vns::ExceptionBusiness e_(__FILE__, __LINE__, "Message of a ExceptionBusiness error", "TestLocation");
        std::cout << e_.GetDescription() << std::endl;
        throw e_;
    }
    catch (vns::ExceptionChain & e)
    {
        std::cerr << "Error : vns::ExceptionChain :"<<std::endl;
        std::cerr << e.GetFile() << ":" << e.GetLine() << ":"<< std::endl;
        std::cerr << e.GetDescription() << std::endl;
        return EXIT_FAILURE;
    }
    catch (vns::ExceptionBusiness & e)
    {
        std::cerr << "Error : vns::ExceptionBusiness :"<<std::endl;
        std::cerr << e.GetFile() << ":" << e.GetLine() << ":"<< std::endl;
        std::cerr << e.GetDescription() << std::endl;
        return EXIT_SUCCESS;
    }
    catch (vns::ExceptionData & e)
    {
        std::cerr << "Error : vns::ExceptionData :"<<std::endl;
        std::cerr << e.GetFile() << ":" << e.GetLine() << ":"<< std::endl;
        std::cerr << e.GetDescription() << std::endl;
        return EXIT_FAILURE;
    }
    catch (vns::ExceptionBase & e)
    {
        std::cerr << "Error : vns::ExceptionBase :"<<std::endl;
        std::cerr << e.GetFile() << ":" << e.GetLine() << ":"<< std::endl;
        std::cerr << e.GetDescription() << std::endl;
        return EXIT_FAILURE;
    }
    catch (itk::ExceptionObject & e)
    {
        std::cerr << "Error : itk::ExceptionObject :"<<std::endl;
        std::cerr << e.GetFile() << ":" << e.GetLine() << ":"<< std::endl;
        std::cerr << e.GetDescription() << std::endl;
        return EXIT_FAILURE;
    }
    catch (std::bad_alloc & err)
    {
        return EXIT_FAILURE;
    }
    catch (const std::exception& e)
    {
        return EXIT_FAILURE;
    }
    catch (...)
    {
        return EXIT_FAILURE;
    }

    // Test the ExceptionChain class
    try
    {
        ::vns::ExceptionChain e_(__FILE__, __LINE__, "Message of a ExceptionChain error", "TestLocation");
        std::cout << e_.GetDescription() << std::endl;
        throw e_;
    }
    catch (vns::ExceptionChain & e)
    {
        std::cerr << "Error : vns::ExceptionChain :"<<std::endl;
        std::cerr << e.GetFile() << ":" << e.GetLine() << ":"<< std::endl;
        std::cerr << e.GetDescription() << std::endl;
        return EXIT_SUCCESS;
    }
    catch (vns::ExceptionBusiness & e)
    {
        std::cerr << "Error : vns::ExceptionBusiness :"<<std::endl;
        std::cerr << e.GetFile() << ":" << e.GetLine() << ":"<< std::endl;
        std::cerr << e.GetDescription() << std::endl;
        return EXIT_FAILURE;
    }
    catch (vns::ExceptionData & e)
    {
        std::cerr << "Error : vns::ExceptionData :"<<std::endl;
        std::cerr << e.GetFile() << ":" << e.GetLine() << ":"<< std::endl;
        std::cerr << e.GetDescription() << std::endl;
        return EXIT_FAILURE;
    }
    catch (vns::ExceptionBase & e)
    {
        std::cerr << "Error : vns::ExceptionBase :"<<std::endl;
        std::cerr << e.GetFile() << ":" << e.GetLine() << ":"<< std::endl;
        std::cerr << e.GetDescription() << std::endl;
        return EXIT_FAILURE;
    }
    catch (itk::ExceptionObject & e)
    {
        std::cerr << "Error : itk::ExceptionObject :"<<std::endl;
        std::cerr << e.GetFile() << ":" << e.GetLine() << ":"<< std::endl;
        std::cerr << e.GetDescription() << std::endl;
        return EXIT_FAILURE;
    }
    catch (std::bad_alloc & err)
    {
        return EXIT_FAILURE;
    }
    catch (const std::exception& e)
    {
        return EXIT_FAILURE;
    }
    catch (...)
    {
        return EXIT_FAILURE;
    }








    return EXIT_SUCCESS;
}
