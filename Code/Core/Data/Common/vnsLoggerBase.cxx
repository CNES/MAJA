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
 * VERSION : 1.0.0 : FA : LAIG-FA-MAC-148399-CS : 11 octobre 2016 : Audit code - Correction qualite         *
 * VERSION : 2-0-0 : DM : LAIG-DM-MAC-146-CNES : 14 octobre 2011 : Modification pour prise en compte             *
 *                                                            d'evolutions liees a MACCS                    *
 *                                                                                                          *
 * VERSION : 1-0-0 : <TypeFT> : <NumFT> : 15 décembre 2010 : Creation
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id$
 *                                                                                                          *
 ************************************************************************************************************/

#include "vnsLoggerBase.h"
#include "itksys/SystemTools.hxx"

#include <cstdio>
#include "vnsSystem.h"
#include "vnsConfigure.h"
#include "vnsUtilities.h"
#include <exception>
#include <cmath>

namespace vns
{

    std::string LoggerBase::m_HumanReadableFormatDate = "%Y-%m-%dT%H:%M:%S";
    std::string LoggerBase::m_HumanReadableFormatMicroSeconds = "%6f";
    std::string LoggerBase::m_PIDFormat = "%012d";

    LoggerBase::LoggerBase()
    {
        m_MinLevel = LoggerBase::DEBUG;
        m_MaxLevel = LoggerBase::ERROR_FATAL;
        std::string l_loglevel = "DEBUG";
        const bool l_exist = itksys::SystemTools::GetEnv("MAJA_LOGGER_LEVEL", l_loglevel);
        if (l_exist)
        {
        	this->SetStdLogLevel(l_loglevel);
        }

        m_Clock = itk::RealTimeClock::New();
        m_Output = itk::MultipleLogOutput::New();

        // Processor Name
        m_ProcessorName = System::GetProcessName();

        // Node name
        m_NodeName = System::GetHostName();

        // PID
        const int pid = System::GetPID();
        char l_strPID[13];
        const int code_return = std::snprintf(l_strPID, sizeof(l_strPID), m_PIDFormat.c_str(), pid);
        if (code_return < 0)
        {
            vnsStaticExceptionDataMacro("Internal error while getting the date with snprintf function.")
        }
        m_PID = l_strPID;
    }

    LoggerBase::~LoggerBase()
    {
    }

// Adds an output stream to the MultipleLogOutput for writing.
    void
    LoggerBase::AddLogOutput(OutputType* output)
    {
        // delegates to MultipleLogOutput
        m_Output->AddLogOutput(output);
    }

    void
    LoggerBase::Write(LevelType level, std::string const & content)
    {
        if ((m_MinLevel <= level) && (m_MaxLevel >= level))
        {
            // Write formatted entry
            m_Output->Write(this->BuildFormattedEntry(level, content));
            // Flush anyway
            m_Output->Flush();
        }
    }
    void
    LoggerBase::Flush()
    {
        m_Output->Flush();
    }

    std::string
    LoggerBase::GetLevelString(const LevelType & level)
    {
        static const std::string l_LevelString[] =
            { "SuperD", "D", "I", "P", "W", "E" };
        return l_LevelString[level];
    }
    std::string
    LoggerBase::BuildFormattedEntry(LevelType level, std::string const & content)
    {
        std::ostringstream s;

        // Date and time
        s << BuildFormattedDate();

        // Rest of message-relative data
        s << " " << m_Name << " " << MAJA_VERSION_LOGGER_STRING << " [" << m_PID << "] ["
                << GetLevelString(level) << "] " << content << std::endl;

        return s.str();
    }

    std::string
    LoggerBase::BuildFormattedDate()
    {
        std::ostringstream s;

        // Get full time
        double time = itksys::SystemTools::GetTime();
        // Date and time till seconds
        std::string timeText = itksys::SystemTools::GetCurrentDateTime(this->m_HumanReadableFormatDate.c_str());
        s << timeText;

        // Microseconds (if available on platform)
        double Seconds(0.);
        double microSeconds = modf(time, &Seconds);

        char microSecondsText[9];
        const int code_return = std::snprintf(microSecondsText, sizeof(microSecondsText), m_HumanReadableFormatMicroSeconds.c_str(),
                microSeconds);
        if (code_return < 0)
        {
            vnsStaticExceptionDataMacro("Internal error while getting the microseconds with snprintf function.")
        }

        // Omit "0." and keep only fractional part
        s << "." << &microSecondsText[2];

        return s.str();
    }

    /** Get Stdout/err log level */
    void
	LoggerBase::SetStdLogLevel(std::string value)
    {
    	// ---------------------------------------------------------------------------------------------
    	// Init log level
    	LevelType l_Level = LoggerBase::DEBUG;
    	// ---------------------------------------------------------------------------------------------
    	// Set the log level to INFO level
    	if (value == "INFO")
    	{
    		l_Level = LoggerBase::INFO;
    	}
    	// ---------------------------------------------------------------------------------------------
    	// Set the log level to PROGRESS level
    	else if (value == "PROGRESS")
    	{
    		l_Level = LoggerBase::PROGRESS;
    	}
    	// ---------------------------------------------------------------------------------------------
    	// Set the log level to DEBUG level
    	else if (value == "DEBUG")
    	{
    		l_Level = LoggerBase::DEBUG;
    	}
    	// ---------------------------------------------------------------------------------------------
    	// Set the log level to WARNING level
    	else if (value == "WARNING")
    	{
    		l_Level = LoggerBase::WARNING;
    	}
    	// ---------------------------------------------------------------------------------------------
    	// Set the log level to ERROR level
    	else if (value == "ERROR")
    	{
    		l_Level = LoggerBase::ERROR_FATAL;
    	}
    	// For Development
    	// ---------------------------------------------------------------------------------------------
    	// Set the log level to SUPER DEBUG level
    	else if (value == "SUPER_DEBUG")
    	{
    		l_Level = LoggerBase::SUPER_DEBUG;
    	}
    	// ---------------------------------------------------------------------------------------------
    	else
    	{
    		vnsExceptionDataMacro(
    				"Invalid 'StderrLogLevel' "<<value<<". The possible values are 'INFO', 'PROGRESS', 'WARNING', 'DEBUG' or 'ERROR'.")
    	}
    	this->SetMinLevel(l_Level);
    }

    /** Print contents of a LoggerBase */
    void
    LoggerBase::PrintSelf(std::ostream &os, itk::Indent indent) const
    {
        Superclass::PrintSelf(os, indent);

        os << indent << "Name: " << this->GetName() << std::endl;
        os << indent << "MinLevel: " << this->GetMinLevel() << std::endl;
        os << indent << "MaxLevel: " << this->GetMinLevel() << std::endl;
        //os << indent << "LevelForFlushing: " << this->GetLevelForFlushing() << std::endl;
        // os << indent << "TimeStampFormat: " << this->m_TimeStampFormat << std::endl;
        os << indent << "HumanReadableFormatDate: " << this->m_HumanReadableFormatDate << std::endl;
        os << indent << "HumanReadableFormatMicroSeconds: " << this->m_HumanReadableFormatMicroSeconds << std::endl;
        os << indent << "PIDFormat: " << this->m_PIDFormat << std::endl;
    }

} //namespace vns
