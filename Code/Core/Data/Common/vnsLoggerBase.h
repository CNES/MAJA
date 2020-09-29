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
 * VERSION : 1.0.0 : FA : LAIG-FA-MAC-1987-CNES : 18 octobre 2016 : Correction qualite (destructeur virtuel)*
 * VERSION : 1.0.0 : FA : LAIG-FA-MAC-148399-CS : 11 octobre 2016 : Audit code - Correction qualite         *
 * VERSION : 5-1-0 : FA : LAIG-FA-MAC-145739-CS : 27 juin 2016 : Audit code - Supp de la macro ITK_EXPORT   *
 * VERSION : 1-0-0 : <TypeFT> : <NumFT> : 15 décembre 2010 : Creation
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id$
 *                                                                                                          *
 ************************************************************************************************************/

#ifndef __vnsLoggerBase_h
#define __vnsLoggerBase_h

#include "itkObject.h"
#include "itkObjectFactory.h"
#include "itkMultipleLogOutput.h"
#include "itkRealTimeClock.h"

namespace vns
{
    /** \class LoggerBase
     *   \brief Class LoggerBase is meant for logging information during a run.
     *
     * \author Hee-Su Kim, Compute Science Dept. Kyungpook National University,
     *                     ISIS Center, Georgetown University.
     *
     *
     *  \ingroup OSSystemObjects LoggingObjects
     */

    class LoggerBase : public itk::Object
    {

    public:

        typedef LoggerBase Self;
        typedef itk::Object Superclass;
        typedef itk::SmartPointer<Self> Pointer;
        typedef itk::SmartPointer<const Self> ConstPointer;

        /** Run-time type information (and related methods). */
        itkTypeMacro( LoggerBase, Object );

        typedef itk::MultipleLogOutput::OutputType OutputType;

        /** Definition of types of messages. These codes will be used to regulate
         * the level of detail of messages reported to the final outputs */
        typedef enum
        {
            SUPER_DEBUG = 0, DEBUG = 1, INFO = 2, PROGRESS = 3, WARNING = 4, ERROR_FATAL = 5
        } LevelType;

        itkSetStringMacro (Name);
        itkGetStringMacro (Name);

        /** Provides a default formatted log entry */
        virtual std::string
        BuildFormattedEntry(LevelType level, std::string const & content);

        /** Set/Get the min priority level for the current logger. Only messages that have
         * priorities equal or greater than the one set here will be posted to the
         * current outputs */
        itkSetEnumMacro(MinLevel,LevelType)
        itkGetEnumMacro(MinLevel,LevelType)

        /** Set/Get the max priority level for the current logger. Only messages that have
         * priorities equal or lower than the one set here will be posted to the
         * current outputs */
        itkSetEnumMacro(MaxLevel, LevelType)
        itkGetEnumMacro(MaxLevel, LevelType)

        /** Registers another output stream with the multiple output. */
        virtual void
        AddLogOutput(OutputType* output);

        /** Write formatted message */
        virtual void
        Write(LevelType level, std::string const & content);
        /** Helper methods */
        void
        SuperDebug(std::string const& message)
        {
            this->Write(LoggerBase::SUPER_DEBUG, message);
        }
        void
        Debug(std::string const& message)
        {
            this->Write(LoggerBase::DEBUG, message);
        }
        void
        Info(std::string const& message)
        {
            this->Write(LoggerBase::INFO, message);
        }
        void
        Progress(std::string const& message)
        {
            this->Write(LoggerBase::PROGRESS, message);
        }
        void
        Warning(std::string const& message)
        {
            this->Write(LoggerBase::WARNING, message);
        }
        void
        Error(std::string const& message)
        {
            this->Write(LoggerBase::ERROR_FATAL, message);
        }

        void SetStdLogLevel(std::string value);
        virtual void
        Flush();

    protected:

        /** Constructor */
        LoggerBase();

        /** Destructor */
        virtual
        ~LoggerBase();

        /** Print contents of a LoggerBase */
        virtual void
        PrintSelf(std::ostream &os, itk::Indent indent) const;

        /** Build formatted date for output */
        std::string
        BuildFormattedDate();

    protected:

        std::string
        GetLevelString(const LevelType & level);

        LevelType m_MinLevel;
        LevelType m_MaxLevel;

        //PriorityLevelType m_LevelForFlushing;

        itk::MultipleLogOutput::Pointer m_Output;

        itk::RealTimeClock::Pointer m_Clock;

        static std::string m_HumanReadableFormatDate;
        static std::string m_HumanReadableFormatMicroSeconds;
        static std::string m_PIDFormat;
        static unsigned int m_NumberOfLevels;

        std::string m_Name;
        std::string m_ProcessorName;
        std::string m_NodeName;
        std::string m_PID;

    };
// class LoggerBase

}// namespace vns

#endif  // __vnsLoggerBase_h
