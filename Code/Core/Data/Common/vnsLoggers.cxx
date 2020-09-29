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
 * VERSION : 1-0-0-3 : FA : 311 : 21 septembre 2011 : Correction sur multi-thread - Logger non threade,       *
 *                                                  ajout patch ITK 3.20 V3 (Delay)                         *
 * VERSION : 1-0-0 : <TypeFT> : <NumFT> : 15 décembre 2010 : Creation
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id$
 *                                                                                                          *
 ************************************************************************************************************/
#include "vnsLoggers.h"

namespace vns
{
    /** Initialize the singleton */
    Loggers::Pointer Loggers::Instance = NULL;

    // Manage the singleton
    // Ensure instance uniqueness
    Loggers::Pointer
    Loggers::GetInstance()
    {
        if (Instance.IsNull() == true)
        {
            // The Logger must be instantiated and initialized with the ConfigureLoggers method
            // Creates a Default Logger and initializes to a default value for TU, etc...
            Instance = Loggers::New();
            Instance->Initialize("MACCSNotInitializedProcess");
        }
        return Instance;
    }

    // create a logger and add it into Loggers
    void
    Loggers::Initialize(const std::string & processor_name)
    {
        // Create the loggers
        m_LoggerSet["STDCOUT"] = LoggerType::New();
        m_LoggerSet["STDCERR"] = LoggerType::New();
        StdStreamLogOutputType::Pointer m_StdCoutLogOutput = StdStreamLogOutputType::New();
        StdStreamLogOutputType::Pointer m_StdCerrLogOutput = StdStreamLogOutputType::New();

        //Set global configuration for all loggers
        SetName(processor_name); // Processor name

        // Configure StdCout logger
        m_StdCoutLogOutput->SetStream(std::cout);
        this->GetStdCoutLogger()->AddLogOutput(m_StdCoutLogOutput);

        // Configure StdCerr logger
        m_StdCerrLogOutput->SetStream(std::cerr);
        this->GetStdCerrLogger()->AddLogOutput(m_StdCerrLogOutput);

    }

    Loggers::LoggerType*
    Loggers::GetLogger(const LoggerNameType &name)
    {
        ContainerType::iterator loggerItr = m_LoggerSet.find(name);
        if (loggerItr == m_LoggerSet.end())
        {
            return NULL;
        }
        return loggerItr->second.GetPointer();
    }

    void
    Loggers::SetMinLevel(LevelType level)
    {
        ContainerType::iterator itr = m_LoggerSet.begin();
        while (itr != m_LoggerSet.end())
        {
            (*itr).second->SetMinLevel(level);
            ++itr;
        }
    }

    void
    Loggers::SetMaxLevel(LevelType level)
    {
        ContainerType::iterator itr = m_LoggerSet.begin();
        while (itr != m_LoggerSet.end())
        {
            (*itr).second->SetMaxLevel(level);
            ++itr;
        }
    }

    void
    Loggers::SetName(const std::string & name)
    {
        ContainerType::iterator itr = m_LoggerSet.begin();
        while (itr != m_LoggerSet.end())
        {
            (*itr).second->SetName(name);
            ++itr;
        }
    }

// Print contents of a Loggers
    void
    Loggers::PrintSelf(std::ostream &os, itk::Indent indent) const
    {
        Superclass::PrintSelf(os, indent);
        os << indent << "The number of loggers: " << m_LoggerSet.size() << std::endl;
    }

} // namespace vns
