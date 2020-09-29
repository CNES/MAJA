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
  Module:    $RCSfile: itkLoggerManagerTest.cxx,v $
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
#include <algorithm>
#include <iomanip>

#include "vnsThreadLogger.h"
#include "vnsLoggers.h"
#include "vnsLoggerBase.h"
#include "itkMacro.h"

class LoggersTest : public itk::Object
{

public:
  
  typedef LoggersTest                Self;
  typedef itk::Object                    Superclass;
  typedef itk::SmartPointer<Self>        Pointer;
  typedef itk::SmartPointer<const Self>  ConstPointer;
  
  /** Run-time type information (and related methods). */
  itkTypeMacro( LoggerTest, Object );
  /** New macro for creation of through a Smart Pointer */
  itkNewMacro( Self );
  
  
  virtual void RunTesting()
    {
//    vnsLogDebugMacro(  "DEBUG message by vnsLogDebugMacro num: " << 1425.4587 << " !!" );
//    vnsLogInfoMacro(   "INFO message by vnsLogInfoMacro : " << int(145) << " " << (char*)("char star") << std::setprecision(6)<<" " << float(12.14574784587) );
    vnsLogDebugMacro(  "DEBUG message by vnsLogDebugMacro" );
    vnsLogInfoMacro(   "INFO message by vnsLogInfoMacro");
    vnsLogProgressMacro(   "PROGRESS message by vnsLogProgressMacro" );
    vnsLogWarningMacro(  "WARNING message by vnsLogWarningMacro" );
    vnsLogErrorMacro(  "ERROR message by vnsLogErrorMacro" );


//    itkExceptionMacro("Error");
//    vnsReportFileErrorMacro(  "ERROR message by vnsReportFileErrorMacro" );
    }
  

  
protected:
  
  /** Constructor */
  LoggersTest(){};
  
  /** Destructor */
  virtual ~LoggersTest(){};
  
  /** Print contents of a LoggerBase */
  virtual void PrintSelf(std::ostream &os, itk::Indent indent) const
  {  
    Superclass::PrintSelf(os,indent);
  }

  
};  // class LoggersTest


int vnsLoggersTest( int argc, char *argv [] )
{
  try
    {
    if (argc < 4)
      {
      std::cout << "Usage: " << argv[0] << " minLevel ProcessorName reportLogFilename" << std::endl;
      return EXIT_FAILURE;
      }
    
    vns::Loggers::GetInstance()->Initialize(argv[2]); //, argv[3] );//"stdStream.txt", "Report.txt");
//    vns::Loggers::GetInstance()->
    const std::string l_Mode(argv[1]);

    if( l_Mode == "DEBUG") vns::Loggers::GetInstance()->SetMinLevel(vns::LoggerBase::DEBUG);
    if( l_Mode == "INFO") vns::Loggers::GetInstance()->SetMinLevel(vns::LoggerBase::INFO);
    if( l_Mode == "PROGRESS") vns::Loggers::GetInstance()->SetMinLevel(vns::LoggerBase::PROGRESS);
    if( l_Mode == "ERROR") vns::Loggers::GetInstance()->SetMinLevel(vns::LoggerBase::ERROR_FATAL);
    if( l_Mode == "WARNING") vns::Loggers::GetInstance()->SetMinLevel(vns::LoggerBase::WARNING);


    //vns::Loggers::GetInstance()->SetLevelForFlushing(vns::LoggerBase::FATAL_ERROR);
    //vns::Loggers::GetInstance()->SetTimeStampFormat(vns::LoggerBase::HUMANREADABLE);


    std::cout << "Testing vns::LoggerManager" << std::endl;
    LoggersTest::Pointer  tester = LoggersTest::New();
    tester->RunTesting();

    }
  catch (itk::ExceptionObject &e)
    {
      std::cerr << "TESTING FAILURE : "<< e << std::endl;
    return EXIT_FAILURE;
    }
  catch(const char * errmsg)
    {
    std::cerr << "Exception catched !! : " << errmsg << std::endl;
    return EXIT_FAILURE;
    }

  std::cout << "[PASSED]" << std::endl;
  return EXIT_SUCCESS;
}


