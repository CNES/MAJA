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
 * VERSION : 1.0.0 : FA : LAIG-FA-MAC-148399-CS : 12 octobre 2016 : Audit code - Correction qualite :       *
 *                                                               Regle : Missing parameter name in function *
 * VERSION : 5-1-0 : FA : LAIG-FA-MAC-145739-CS : 27 juin 2016 : Audit code - Supp de la macro ITK_EXPORT   *
 * VERSION : 1-0-0-3 : FA : 311 : 21 septembre 2011 : Correction sur multi-thread - Logger non threade,       *
 *                                                  ajout patch ITK 3.20 V3 (Delay)                         *
 * VERSION : 1-0-0-2 : FA : 281 : 7 juin 2011 : Erreur sur le message d'erreur non affiche dans le LOG        *
 *                                                                                                          *
 * VERSION : 1-0-0 : <TypeFT> : <NumFT> : 15 decembre 2010 : Creation
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id$
 *                                                                                                          *
 ************************************************************************************************************/
#ifndef __vnsThreadLogger_h
#define __vnsThreadLogger_h

#include "itkMacro.h"
#include "itkMultiThreader.h"
#include "vnsLogger.h"
#include "itkSimpleFastMutexLock.h"

#include <string>
#include <queue>

namespace vns
{
    /** \class ThreadLogger
     *  \brief Class ThreadLogger is meant for providing logging service
     *  as a separate thread.
     *
     *
     *
     *
     *  \ingroup OSSystemObjects LoggingObjects
     */

    class ThreadLogger : public Logger
    {

    public:

        typedef ThreadLogger Self;
        typedef Logger Superclass;
        typedef itk::SmartPointer<Self> Pointer;
        typedef itk::SmartPointer<const Self> ConstPointer;

        /** Run-time type information (and related methods). */
        itkTypeMacro( ThreadLogger, Logger )

        /** New macro for creation of through a Smart Pointer */
        itkNewMacro( Self )

        typedef Logger::OutputType OutputType;

        typedef Logger::LevelType LevelType;
        typedef unsigned int DelayType;
        /** Definition of types of operations for ThreadLogger. */
        typedef enum
        {
            SET_MIN_LEVEL, SET_MAX_LEVEL, ADD_LOG_OUTPUT, WRITE, FLUSH
        } OperationType;

        /** Set the min priority level for the current logger. Only messages that have
         * priorities equal or greater than the one set here will be posted to the
         * current outputs */
        virtual void
        SetMinLevel(const LevelType & level);

        /** Set the max priority level for the current logger. Only messages that have
         * priorities equal or lower than the one set here will be posted to the
         * current outputs */
        virtual void
        SetMaxLevel(const LevelType & level);

        /** Get the priority level for the current logger. Only messages that have
         * priorities equal or greater than the one set here will be posted to the
         * current outputs */
        virtual LevelType
        GetMinLevel() const;
        virtual LevelType
        GetMaxLevel() const;

        //virtual void SetLevelForFlushing( LevelType level );

        //  virtual LevelType GetLevelForFlushing() const;

        /** Registers another output stream with the multiple output. */
        virtual void
        AddLogOutput(OutputType* output);

        virtual void
        Write(LevelType level, std::string const & content);

        virtual void
        Flush();
        /** Set the delay in milliseconds between checks to see if there are any
         *  low priority messages to be processed.
         */
          virtual void SetDelay(DelayType delay);

        /** Get the delay in milliseconds between checks to see if there are any
         *  low priority messages to be processed.
         */
          virtual DelayType GetDelay() const;

    protected:

        /** Constructor */
        ThreadLogger();

        /** Destructor */
        virtual
        ~ThreadLogger();

        /** Print contents of a ThreadLogger */
        virtual void
        PrintSelf(std::ostream &os, itk::Indent indent) const;

        static ITK_THREAD_RETURN_TYPE ThreadFunction(void* pInfoStruct);

    private:

        typedef std::queue<OperationType> OperationContainerType;

        typedef std::queue<std::string> MessageContainerType;

        typedef std::queue<LevelType> LevelContainerType;

        typedef std::queue<OutputType::Pointer> OutputContainerType;

        itk::MultiThreader::Pointer m_Threader;

        int m_ThreadID;

        OperationContainerType m_OperationQ;

        MessageContainerType m_MessageQ;

        LevelContainerType m_LevelQ;

        OutputContainerType m_OutputQ;

        itk::SimpleFastMutexLock m_Mutex;

        DelayType m_Delay;
    }; // class ThreadLogger


} // namespace vns


#endif  // __vnsThreadLogger_h
