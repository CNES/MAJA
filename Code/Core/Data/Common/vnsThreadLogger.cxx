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
 * VERSION : 1.0.0 : FA : LAIG-FA-MAC-148399-CS : 26 octobre 2016 : Audit code - Correction qualite         *
 * VERSION : 1-0-0-3 : FA : 311 : 21 septembre 2011 : Correction sur multi-thread - Logger non threade,       *
 *                                                  ajout patch ITK 3.20 V3 (Delay)                         *
 * VERSION : 1-0-0-2 : FA : 232 : 7 juin 2011 : Log incomplet en mode Backward                                *
 *                                                                                                          *
 * VERSION : 1-0-0-2 : FA : 281 : 7 juin 2011 : Erreur sur le message d'erreur non affiche dans le LOG        *
 *                                                                                                          *
 * VERSION : 1-0-0 : <TypeFT> : <NumFT> : 15 décembre 2010 : Creation
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id$
 *                                                                                                          *
 ************************************************************************************************************/

#include <iostream>
#include "vnsThreadLogger.h"
#include "itksys/SystemTools.hxx"

namespace vns
{

    // Set the min priority level for the current logger. Only messages that have
    // priorities equal or greater than the one set here will be posted to the
    // current outputs
    void
    ThreadLogger::SetMinLevel(const LevelType & level)
    {
        m_Mutex.Lock();
        m_OperationQ.push(SET_MIN_LEVEL);
        m_LevelQ.push(level);
        m_Mutex.Unlock();
    }

    // Set the max priority level for the current logger. Only messages that have
    // priorities equal or lower than the one set here will be posted to the
    // current outputs
    void
    ThreadLogger::SetMaxLevel(const LevelType & level)
    {
        m_Mutex.Lock();
        m_OperationQ.push(SET_MAX_LEVEL);
        m_LevelQ.push(level);
        m_Mutex.Unlock();
    }

    // Get the min priority level for the current logger. Only messages that have
    // priorities equal or greater than the one set here will be posted to the
    // current outputs
    Logger::LevelType
    ThreadLogger::GetMinLevel() const
    {
        m_Mutex.Lock();
        LevelType level = m_MinLevel;
        m_Mutex.Unlock();
        return level;
    }

    // Get the max priority level for the current logger. Only messages that have
    // priorities equal or lower than the one set here will be posted to the
    // current outputs
    Logger::LevelType
    ThreadLogger::GetMaxLevel() const
    {
        m_Mutex.Lock();
        LevelType level = m_MaxLevel;
        m_Mutex.Unlock();
        return level;
    }

    // Adds an output stream to the MultipleLogOutput for writing.
    void
    ThreadLogger::AddLogOutput(OutputType* output)
    {
        m_Mutex.Lock();
        m_OperationQ.push(ADD_LOG_OUTPUT);
        m_OutputQ.push(output);
        m_Mutex.Unlock();
    }
    void
    ThreadLogger::SetDelay(DelayType delay)
    {
        this->m_Mutex.Lock();
        this->m_Delay = delay;
        this->m_Mutex.Unlock();
    }
    ThreadLogger::DelayType
    ThreadLogger::GetDelay() const
    {
        this->m_Mutex.Lock();
        DelayType delay = this->m_Delay;
        this->m_Mutex.Unlock();
        return delay;
    }
    // Write method
    void
    ThreadLogger::Write(LevelType level, std::string const & content)
    {
        m_Mutex.Lock();
        //  LoggerBase::Write(level, content);
        m_OperationQ.push(WRITE);
        m_MessageQ.push(content);
        m_LevelQ.push(level);
        m_Mutex.Unlock();
        if ((m_MinLevel <= level) && (m_MaxLevel >= level))
        {
            // Flush anyway
            this->Flush();
        }
    }

    // Flush method
    void
    ThreadLogger::Flush()
    {
        // --------------------------------------
        // Lock mutex
        m_Mutex.Lock();

        // --------------------------------------
        // Queue treatment
        while (!m_OperationQ.empty())
        {
            switch (m_OperationQ.front())
                {
            // --------------------------------------
            // Set min level
            case ThreadLogger::SET_MIN_LEVEL:
                m_MinLevel = m_LevelQ.front();
                m_LevelQ.pop();
                break;

                // --------------------------------------
                // Set max level
            case ThreadLogger::SET_MAX_LEVEL:
                m_MaxLevel = m_LevelQ.front();
                m_LevelQ.pop();
                break;

                // --------------------------------------
                // Set add log output
            case ThreadLogger::ADD_LOG_OUTPUT:
                m_Output->AddLogOutput(m_OutputQ.front());
                m_OutputQ.pop();
                break;

                // --------------------------------------
                // Write treatment
            case ThreadLogger::WRITE:
                Logger::Write(m_LevelQ.front(), m_MessageQ.front());
                m_LevelQ.pop();
                m_MessageQ.pop();
                break;
                // --------------------------------------
                // Flush treatment
            case ThreadLogger::FLUSH:
                Logger::Flush();
                break;
            default:
                break;
                }
            m_OperationQ.pop();
        }
        // --------------------------------------
        // Flush output
        m_Output->Flush();
        // --------------------------------------
        // Unlock mutex
        m_Mutex.Unlock();

    }

    // Constructor
    ThreadLogger::ThreadLogger()
    {
        this->m_Delay = 300; // ms
        m_Threader = itk::MultiThreader::New();
        m_ThreadID = m_Threader->SpawnThread(ThreadFunction, this);
    }

    // Destructor
    ThreadLogger::~ThreadLogger()
    {
        if (m_Threader)
        {
            m_Threader->TerminateThread(m_ThreadID);
        }
    }

    ITK_THREAD_RETURN_TYPE ThreadLogger::ThreadFunction(void* pInfoStruct)
    {
        // --------------------------------------
        // Get pointer Info structure
        struct itk::MultiThreader::ThreadInfoStruct * pInfo = (struct itk::MultiThreader::ThreadInfoStruct*) pInfoStruct;

        // --------------------------------------
        // Check validity pointer
        if (pInfo == NULL)
        {
            return ITK_THREAD_RETURN_VALUE;
        }

        if (pInfo->UserData == NULL)
        {
            return ITK_THREAD_RETURN_VALUE;
        }

        // --------------------------------------
        // Get logger pointer
        ThreadLogger *pLogger = static_cast<ThreadLogger*>(pInfo->UserData);

        while (1)
        {
            // --------------------------------------
            // Lock the active flag lock
            pInfo->ActiveFlagLock->Lock();
            int activeFlag = *pInfo->ActiveFlag;
            // --------------------------------------
            // Unlock the active flag lock
            pInfo->ActiveFlagLock->Unlock();
            if (!activeFlag)
            {
                break;
            }

            // --------------------------------------
            // Lock the mutex
            pLogger->m_Mutex.Lock();

            // --------------------------------------
            // Queue treatment
            while (!pLogger->m_OperationQ.empty())
            {
                switch (pLogger->m_OperationQ.front())
                    {
                // --------------------------------------
                // Set Min level
                case ThreadLogger::SET_MIN_LEVEL:
                    pLogger->m_MinLevel = pLogger->m_LevelQ.front();
                    pLogger->m_LevelQ.pop();
                    break;
                    // --------------------------------------
                    // Set Max level
                case ThreadLogger::SET_MAX_LEVEL:
                    pLogger->m_MaxLevel = pLogger->m_LevelQ.front();
                    pLogger->m_LevelQ.pop();
                    break;
                    // --------------------------------------
                    // Add log output
                case ThreadLogger::ADD_LOG_OUTPUT:
                    pLogger->m_Output->AddLogOutput(pLogger->m_OutputQ.front());
                    pLogger->m_OutputQ.pop();
                    break;

                    // --------------------------------------
                    // Write case
                case ThreadLogger::WRITE:
                    pLogger->Logger::Write(pLogger->m_LevelQ.front(), pLogger->m_MessageQ.front());
                    pLogger->m_LevelQ.pop();
                    pLogger->m_MessageQ.pop();
                    break;
                    // --------------------------------------
                    // Flush case
                case ThreadLogger::FLUSH:
                    pLogger->Logger::Flush();
                    break;
                default:
                    break;
                    }
                pLogger->m_OperationQ.pop();
            }
            // --------------------------------------
            // Unlock mutex
            pLogger->m_Mutex.Unlock();
            itksys::SystemTools::Delay(pLogger->GetDelay());
        }
        return ITK_THREAD_RETURN_VALUE;
    }

    // Print contents of a ThreadLogger
    void
    ThreadLogger::PrintSelf(std::ostream &os, itk::Indent indent) const
    {
        Superclass::PrintSelf(os, indent);
        // Write members values
        os << indent << "Thread ID: " << m_ThreadID << std::endl;
        os << indent << "Operation Queue Size: " << m_OperationQ.size() << std::endl;
        os << indent << "Message Queue Size: " << m_MessageQ.size() << std::endl;
        os << indent << "Level Queue Size: " << m_LevelQ.size() << std::endl;
        os << indent << "Output Queue Size: " << m_OutputQ.size() << std::endl;
    }

} // namespace vns
