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
/*=========================================================================

 Program:   Insight Segmentation & Registration Toolkit
 Module:    $RCSfile: itkThreadLoggerTest.cxx,v $
 Language:  C++
 Date:      $Date: 2005-11-05 03:49:12 $
 Version:   $Revision: 1.6 $

 Copyright (c) Insight Software Consortium. All rights reserved.
 See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

 This software is distributed WITHOUT ANY WARRANTY; without even
 the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 PURPOSE.  See the above copyright notices for more information.

 =========================================================================*/

#if defined(_MSC_VER)
//Warning about: identifier was truncated to '255' characters in the debug information (MVC6.0 Debug)
#pragma warning( disable : 4786 )
#endif

#include <iostream>
#include <fstream>
#include "itkStdStreamLogOutput.h"
#include "vnsThreadLogger.h"
#include "itkMultiThreader.h"

//----------------------------------------------------------------------------
// Macros for simplifying the use of logging
//
#define vnsLogMacro( x, y)  \
{         \
  if (this->GetLogger() ) \
    {  \
       if( this->GetLogger()->GetMinLevel() <= ::vns::LoggerBase::x ) \
          this->GetLogger()->Write(::vns::LoggerBase::x, std::string(y) + std::string(" : message should appear") ); \
       else \
          this->GetLogger()->Write(::vns::LoggerBase::x, std::string(y) + std::string(" : message should NOT appear") ); \
    }  \
}

#define vnsLogMacroStatic( obj, x, y)  \
{         \
  if (obj->GetLogger() ) \
    {  \
       if( obj->GetLogger()->GetMinLevel() <= ::vns::LoggerBase::x ) \
          obj->GetLogger()->Write(::vns::LoggerBase::x, std::string(y) + std::string(" : message should appear") ); \
       else \
          obj->GetLogger()->Write(::vns::LoggerBase::x, std::string(y) + std::string(" : message should NOT appear") ); \
    }  \
}

class LoggerTester
{
public:
    vns::Logger*
    GetLogger()
    {
        return m_Logger;
    }
    void
    SetLogger(vns::ThreadLogger* logger)
    {
        m_Logger = logger;
    }
    void
    log()
    {
        vnsLogMacro( DEBUG, "DEBUG message by vnsLogMacro" );
        vnsLogMacro( INFO, "INFO message by vnsLogMacro" );
        vnsLogMacro( WARNING, "WARNING message by vnsLogMacro" );
        vnsLogMacro( PROGRESS, "PROGRESS message by vnsLogMacro" );
        vnsLogMacro( ERROR_FATAL, "ERROR message by vnsLogMacro" );
    }
    static void
    logStatic(LoggerTester* tester)
    {
        vnsLogMacroStatic( tester, DEBUG, "DEBUG message by vnsLogMacroStatic" );
        vnsLogMacroStatic( tester, INFO, "INFO message by vnsLogMacroStatic" );
        vnsLogMacroStatic( tester, WARNING, "WARNING message by vnsLogMacroStatic" );
        vnsLogMacroStatic( tester, PROGRESS, "PROGRESS message by vnsLogMacroStatic" );
        vnsLogMacroStatic( tester, ERROR_FATAL, "ERROR message by vnsLogMacroStatic" );
    }
private:
    vns::ThreadLogger* m_Logger;
};

int
vnsLoggerTest(int argc, char * argv[])
{
    if (argc < 6)
    {
        std::cout << "Usage: " << argv[0] << " minLevel processorName stdouput (cout or cerr) logTestFilename NbThreads" << std::endl;
        return EXIT_FAILURE;
    }

    // Create an ITK StdStreamLogOutputs
    itk::StdStreamLogOutput::Pointer coutput = itk::StdStreamLogOutput::New();
    itk::StdStreamLogOutput::Pointer foutput = itk::StdStreamLogOutput::New();
    std::string stdoutput(argv[3]);
    if (stdoutput == "cout")
    {
        coutput->SetStream(std::cout);
    }
    else if (stdoutput == "cerr")
    {
        coutput->SetStream(std::cerr);
    }
    else
    {
        std::cout << "Testing bad argument." << std::endl;
        return EXIT_FAILURE;
    }

    std::ofstream fout(argv[4]);

    itk::MultiThreader::SetGlobalDefaultNumberOfThreads(atoi(argv[5]));

    foutput->SetStream(fout);

    // Create an VNS ThreadLogger
    vns::ThreadLogger::Pointer logger = vns::ThreadLogger::New();

    std::cout << "Testing vns::ThreadLogger" << std::endl;

    // Setting the logger
    logger->SetName(argv[2]);
    if (std::string(argv[1]) == "DEBUG")
        logger->SetMinLevel(vns::LoggerBase::DEBUG);
    else if (std::string(argv[1]) == "INFO")
        logger->SetMinLevel(vns::LoggerBase::INFO);
    else if (std::string(argv[1]) == "PROGRESS")
        logger->SetMinLevel(vns::LoggerBase::PROGRESS);
    else if (std::string(argv[1]) == "ERROR")
        logger->SetMinLevel(vns::LoggerBase::ERROR_FATAL);
    else if (std::string(argv[1]) == "WARNING")
        logger->SetMinLevel(vns::LoggerBase::WARNING);
    else
    {
        std::cout << "Testing bad argument LEVEL." << std::endl;
        return EXIT_FAILURE;
    }
    //logger->SetLevelForFlushing(vns::LoggerBase::FATAL_ERROR);

    std::cout << "  Adding console and file stream LogOutputs" << std::endl;
    logger->AddLogOutput(coutput);
    logger->AddLogOutput(foutput);

    // Printing the logger's member variables
    std::cout << "  Name: " << logger->GetName() << std::endl;
    std::cout << "  Min Level: " << logger->GetMinLevel() << std::endl;
    std::cout << "  Max Level: " << logger->GetMaxLevel() << std::endl;
    std::cout << "  NbThreads: " << itk::MultiThreader::GetGlobalDefaultNumberOfThreads() << std::endl;

    std::cout << "  ------------------------------------------------------------------------------------" << std::endl;
    std::cout << "  Messages LOG: " << std::endl;
    //std::cout << "  Level For Flushing: " << logger->GetLevelForFlushing() << std::endl;

    // Logging by the vnsLogMacro from a class with itk::ThreadLogger
    LoggerTester tester;
    tester.SetLogger(logger);
    tester.log();
    // Logging by the vnsLogMacroStatic from a class with itk::ThreadLogger
    std::cout << "  ------------------------------------------------------------------------------------" << std::endl;
    std::cout << "  Messages LOG Static: " << std::endl;
    LoggerTester::logStatic(&tester);

    std::cout << "  ------------------------------------------------------------------------------------" << std::endl;
    std::cout << "  The printed order of 'Messages ##' below might not be predictable because of multi-threaded logging" << std::endl;
    std::cout << "  But the logged messages will be in order." << std::endl;
    std::cout << "  Each line is an atom for synchronization." << std::endl;
    std::cout << "  ------------------------------------------------------------------------------------" << std::endl;
    // Writing by the logger
    logger->Write(vns::LoggerBase::DEBUG, "This is the DEBUG message.");
    std::cout << "  Message #1" << std::endl;
    logger->Write(vns::LoggerBase::INFO, "This is the INFO message.");
    logger->Write(vns::LoggerBase::PROGRESS, "This is the PROGRESS message.");
    logger->Write(vns::LoggerBase::WARNING, "This is the WARNING message.");
    std::cout << "  Message #2" << std::endl;
    logger->Write(vns::LoggerBase::PROGRESS, "This is the PROGRESS message.");
    logger->Write(vns::LoggerBase::ERROR_FATAL, "This is the FATAL_ERROR message.");
    //logger->Write(vns::LoggerBase::MUSTFLUSH, "This is the MUSTFLUSH message.");
    std::cout << "  Message #3" << std::endl;
    logger->Flush();
    std::cout << "  ------------------------------------------------------------------------------------" << std::endl;
    std::cout << "  Flushing by the ThreadLogger is synchronized." << std::endl;

    return EXIT_SUCCESS;
}

