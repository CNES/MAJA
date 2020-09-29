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
 * VERSION : 5-1-0 : FA : LAIG-FA-MAC-145739-CS : 6 juillet 2016 : Audit code - decoupage macro en fonction *
 * VERSION : 5-0-0 : FA : LAIG-FA-MAC-143764-CS : 19 avril 2016 : Mise en forme du code (indentation, etc.) *
 * VERSION : 1-0-0-3 : FA : 311 : 21 septembre 2011 : Correction sur multi-thread - Logger non threade,       *
 *                                                  ajout patch ITK 3.20 V3 (Delay)                         *
 * VERSION : 1-0-0-2 : FA : 281 : 7 juin 2011 : Erreur sur le message d'erreur non affiche dans le LOG        *
 *                                                                                                          *
 * VERSION : 1-0-0 : <TypeFT> : <NumFT> : 15 décembre 2010 : Creation
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id$
 *                                                                                                          *
 ************************************************************************************************************/

#ifndef __vnsLoggers_h
#define __vnsLoggers_h

#include "itkObject.h"
#include "itkObjectFactory.h"
#include "vnsLogger.h"
#include "itksys/SystemTools.hxx"

#include <fstream>
#include "itkStdStreamLogOutput.h"
#include <string>
#include <map>
#include "otbMacro.h"
#include <iomanip> // std::setprecision

/****************************************************************************
 Macros for classical logging
 ****************************************************************************/

#define vnsLogMacro( level, x ) {         \
        std::ostringstream message; message << x;  \
        vns::Loggers::GetInstance()->SendMessageToLog(message.str(), level,  __FILE__, __FUNCTION__, __LINE__); \
    }

#define vnsLogErrorMacro( x ) \
    if( vns::Loggers::GetInstance()->GetStdCoutLogger()->GetMinLevel() <= vns::LoggerBase::ERROR_FATAL) \
    { \
        std::ostringstream message; message << x;  \
        vns::Loggers::GetInstance()->SendMessageToLog(message.str(), vns::LoggerBase::ERROR_FATAL,  __FILE__, __FUNCTION__, __LINE__); \
    }

#define vnsLogWarningMacro( x ) \
    if( vns::Loggers::GetInstance()->GetStdCoutLogger()->GetMinLevel() <= vns::LoggerBase::WARNING) \
    { \
        std::ostringstream message; message << x;  \
        vns::Loggers::GetInstance()->SendMessageToLog(message.str(), vns::LoggerBase::WARNING,  __FILE__, __FUNCTION__, __LINE__); \
    }

#define vnsLogInfoMacro( x ) \
    if( vns::Loggers::GetInstance()->GetStdCoutLogger()->GetMinLevel() <= vns::LoggerBase::INFO) \
    { \
        std::ostringstream message; message << x;  \
        vns::Loggers::GetInstance()->SendMessageToLog(message.str(), vns::LoggerBase::INFO,  __FILE__, __FUNCTION__, __LINE__); \
    }

#define vnsLogProgressMacro( x ) \
    if( vns::Loggers::GetInstance()->GetStdCoutLogger()->GetMinLevel() <= vns::LoggerBase::PROGRESS) \
    { \
        std::ostringstream message; message << x;  \
        vns::Loggers::GetInstance()->SendMessageToLog(message.str(), vns::LoggerBase::PROGRESS,  __FILE__, __FUNCTION__, __LINE__); \
    }

#define vnsLogDebugMacro( x ) \
    if( vns::Loggers::GetInstance()->GetStdCoutLogger()->GetMinLevel() <= vns::LoggerBase::DEBUG) \
    { \
        std::ostringstream message; message << x;  \
        vns::Loggers::GetInstance()->SendMessageToLog(message.str(), vns::LoggerBase::DEBUG, __FILE__, __FUNCTION__, __LINE__); \
    }

#define vnsLogDebugValueMacro( x ) \
    if( vns::Loggers::GetInstance()->GetStdCoutLogger()->GetMinLevel() <= vns::LoggerBase::DEBUG) \
    { \
        std::ostringstream message; message << "" << #x << ": "<< x ;  \
        vns::Loggers::GetInstance()->SendMessageToLog(message.str(), vns::LoggerBase::DEBUG,  __FILE__, __FUNCTION__, __LINE__); \
    }

#define vnsLogDebugDoubleValueMacro( x ) \
    if( vns::Loggers::GetInstance()->GetStdCoutLogger()->GetMinLevel() <= vns::LoggerBase::DEBUG) \
    { \
        std::ostringstream message; message << "" << #x << ": "<< std::setprecision(12) << x ;  \
        vns::Loggers::GetInstance()->SendMessageToLog(message.str(), vns::LoggerBase::DEBUG,  __FILE__, __FUNCTION__, __LINE__); \
    }


#define vnsLogDebugBooleanValueMacro( x ) \
    if( vns::Loggers::GetInstance()->GetStdCoutLogger()->GetMinLevel() <= vns::LoggerBase::DEBUG) \
    { \
        std::ostringstream message; message << "" << #x << ": "<< vns::Utilities::BoolToLowerString(x) ;  \
        vns::Loggers::GetInstance()->SendMessageToLog(message.str(), vns::LoggerBase::DEBUG,  __FILE__, __FUNCTION__, __LINE__); \
    }

// ****************************************************************************
// Log std::map, std::list and std::vector macro
// ****************************************************************************
#define vnsLogVectorDebugMacro(x, V) vnsLogListDebugMacro(x, V)
#define vnsLogVectorSuperDebugMacro(x, V) vnsLogListSuperDebugMacro(x, V)

#define vnsLogListDebugMacro(x,V) \
    if( vns::Loggers::GetInstance()->GetStdCoutLogger()->GetMinLevel() <= vns::LoggerBase::DEBUG) \
    { \
        std::ostringstream comment; \
        comment << x << "(" << #V << ")"; \
        std::ostringstream message; \
        message << vns::Loggers::PrintList(comment.str(), V); \
        vns::Loggers::GetInstance()->SendMessageToLog(message.str(), vns::LoggerBase::DEBUG,  __FILE__, __FUNCTION__, __LINE__); \
    }
#define vnsLogListSuperDebugMacro(x,V) \
    if( vns::Loggers::GetInstance()->GetStdCoutLogger()->GetMinLevel() <= vns::LoggerBase::SUPER_DEBUG) \
    { \
        std::ostringstream comment; \
        comment << x << "(" << #V << ")"; \
        std::ostringstream message; \
        message << vns::Loggers::PrintList(comment.str(), V); \
        vns::Loggers::GetInstance()->SendMessageToLog(message.str(), vns::LoggerBase::DEBUG,  __FILE__, __FUNCTION__, __LINE__); \
    }

#define vnsLogMapDebugMacro(x,V) \
    if( vns::Loggers::GetInstance()->GetStdCoutLogger()->GetMinLevel() <= vns::LoggerBase::DEBUG) \
    { \
        std::ostringstream comment; \
        comment << x << "(" << #V << ")"; \
        std::ostringstream message; \
        message << vns::Loggers::PrintMap(comment.str(), V); \
        vns::Loggers::GetInstance()->SendMessageToLog(message.str(), vns::LoggerBase::DEBUG,  __FILE__, __FUNCTION__, __LINE__); \
    }

#define vnsLogMapSuperDebugMacro(x,V) \
    if( vns::Loggers::GetInstance()->GetStdCoutLogger()->GetMinLevel() <= vns::LoggerBase::SUPER_DEBUG) \
    { \
        std::ostringstream comment; \
        comment << x << "(" << #V << ")"; \
        std::ostringstream message; \
        message << vns::Loggers::PrintMap(comment.str(), V); \
        vns::Loggers::GetInstance()->SendMessageToLog(message.str(), vns::LoggerBase::DEBUG,  __FILE__, __FUNCTION__, __LINE__); \
    }

// ****************************************************************************
// Log image info  macro
// ****************************************************************************
#define vnsLogImageRawInfoDebugMacro( x , inputPointer) \
    if( vns::Loggers::GetInstance()->GetStdCoutLogger()->GetMinLevel() <= vns::LoggerBase::DEBUG) \
    { \
        std::ostringstream message; \
        message << x << std::setprecision(20) << ": Origin=" << inputPointer->GetOrigin() << " Spacing=" << inputPointer->GetSpacing() << " Size="<< inputPointer->GetLargestPossibleRegion().GetSize(); \
        vns::Loggers::GetInstance()->SendMessageToLog(message.str(), vns::LoggerBase::DEBUG,  __FILE__, __FUNCTION__, __LINE__); \
    }

#define vnsLogImageInfoDebugMacro( x , inputPointer) \
    if( vns::Loggers::GetInstance()->GetStdCoutLogger()->GetMinLevel() <= vns::LoggerBase::DEBUG) \
    { \
        inputPointer->UpdateOutputInformation(); \
        std::ostringstream message; \
        message << x << std::setprecision(20) << ": Origin=" << inputPointer->GetOrigin() << " Spacing=" << inputPointer->GetSpacing() << " Size="<< inputPointer->GetLargestPossibleRegion().GetSize(); \
        vns::Loggers::GetInstance()->SendMessageToLog(message.str(), vns::LoggerBase::DEBUG,  __FILE__, __FUNCTION__, __LINE__); \
    }

#define vnsLogImageConstInfoDebugMacro( x , inputPointer, InputImageType) \
    if( vns::Loggers::GetInstance()->GetStdCoutLogger()->GetMinLevel() <= vns::LoggerBase::DEBUG) \
    { \
        InputImageType * l_NonConstinputPointer = const_cast<InputImageType*>(inputPointer); \
        l_NonConstinputPointer->UpdateOutputInformation(); \
        std::ostringstream message; \
        message << x << std::setprecision(20) << ": Origin=" << l_NonConstinputPointer->GetOrigin() << " Spacing=" << l_NonConstinputPointer->GetSpacing() << " Size="<< l_NonConstinputPointer->GetLargestPossibleRegion().GetSize(); \
        vns::Loggers::GetInstance()->SendMessageToLog(message.str(), vns::LoggerBase::DEBUG,  __FILE__, __FUNCTION__, __LINE__); \
    }

#define vnsLogSuperDebugMacro( x )  \
    if( vns::Loggers::GetInstance()->GetStdCoutLogger()->GetMinLevel() <= vns::LoggerBase::SUPER_DEBUG) \
    { \
        std::ostringstream message; message << x; \
        vns::Loggers::GetInstance()->SendMessageToLog(message.str(), vns::LoggerBase::SUPER_DEBUG,  __FILE__, __FUNCTION__, __LINE__); \
    }

#define vnsLogExtendedDebugMacro( x ) \
    if( vns::Loggers::GetInstance()->GetStdCoutLogger()->GetMinLevel() <= vns::LoggerBase::DEBUG) \
    { \
        std::ostringstream message;  \
        message  << x << "\n" \
                 << this->GetNameOfClass() << " (" << this << ") \n\n"; \
        vns::Loggers::GetInstance()->SendMessageToLog(message.str(), vns::LoggerBase::DEBUG,  __FILE__, __FUNCTION__, __LINE__); \
    }

namespace vns
{
    /** \class Loggers
     *  \brief Class Loggers is meant for centrally managing loggers.
     *
     *
     *
     * \author Hee-Su Kim, Compute Science Dept. Kyungpook National University,
     *                     ISIS Center, Georgetown University.
     *
     *
     *  \ingroup OSSystemObjects LoggingObjects
     *
     */

    class Loggers : public itk::Object
    {

    public:

        typedef Loggers Self;
        typedef itk::Object Superclass;
        typedef itk::SmartPointer<Self> Pointer;
        typedef itk::SmartPointer<const Self> ConstPointer;

        /** Run-time type information (and related methods). */
        itkTypeMacro( Loggers, itk::Object )

        /** Get the unique instance of type manager */
        static Pointer
        GetInstance();

        typedef Logger::LevelType LevelType;

        typedef Logger::OutputType OutputType;

        // FA : 311 sur probleme muti-thread        typedef ThreadLogger LoggerType;
        typedef Logger LoggerType;

        typedef LoggerType::Pointer LoggerPointer;
        typedef std::string LoggerNameType;
        typedef itk::StdStreamLogOutput StdStreamLogOutputType;
        typedef StdStreamLogOutputType::Pointer StdStreamLogOutputPointer;

        /** configure the loggers */
        void Initialize(const std::string & processor_name);


        /** Return the Stdcout logger */
        LoggerType*
        GetStdCoutLogger()
        {
            return (GetLogger("STDCOUT"));
        }
        /** Return the Stdcerr logger */
        LoggerType*
        GetStdCerrLogger()
        {
            return (GetLogger("STDCERR"));
        }

        /** Set the Min level */
        void
        SetMinLevel(const LevelType level);

        /** Set the Max level */
        void
        SetMaxLevel(const LevelType level);

        /** Set the logger name */
        void
        SetName(const std::string & name);

        template<class T>
        static std::string
        PrintList(const std::string & comment, const T & list)
        {
            std::ostringstream message;
            const unsigned int nb_values = list.size();
            typename T::const_iterator it = list.begin();
            message << "----------------------------------------------------" << std::endl;
            message << comment << " with '" << nb_values << "' :";
            while (it != list.end())
            {
                message << (*it) << " ";
                ++it;
            }
            message << std::endl;
            return message.str();
        }
        template<class T>
            static std::string
            PrintList(const std::string & comment, T & list)
            {
                std::ostringstream message;
                const unsigned int nb_values = list.size();
                typename T::const_iterator it = list.begin();
                message << "----------------------------------------------------" << std::endl;
                message << comment << " with '" << nb_values << "' :";
                while (it != list.end())
                {
                    message << (*it) << " ";
                    ++it;
                }
                message << std::endl;
                return message.str();
            }
        template<class T>
            static std::string
            PrintMap(const std::string & comment, const T & map)
            {
                std::ostringstream message;
                const unsigned int nb_values = map.size();
                typename T::const_iterator it = map.begin();
                message << "----------------------------------------------------" << std::endl;
                message << comment << " with '" << nb_values << "' :";
                while (it != map.end())
                {
                    message << " [" << it->first << "] = " << it->second << std::endl;
                    ++it;
                }
                return message.str();
            }
        template<class T>
            static std::string
            PrintMap(const std::string & comment, T & map)
            {
                std::ostringstream message;
                const unsigned int nb_values = map.size();
                typename T::const_iterator it = map.begin();
                message << "----------------------------------------------------" << std::endl;
                message << comment << " with '" << nb_values << "' :";
                while (it != map.end())
                {
                    message << " [" << it->first << "] = " << it->second << std::endl;
                    ++it;
                }
                return message.str();
            }

        static void
        SendMessageToLog(const std::string & msg, const LoggerBase::LevelType & level, const char *file, const char *function, const unsigned long line)
        {
            if (Instance->GetStdCoutLogger()->GetMinLevel() <= level /*vns::LoggerBase::INFO*/)
            {
                std::ostringstream message;
                message << msg;
                if ((level <= vns::LoggerBase::DEBUG) || (level == vns::LoggerBase::ERROR_FATAL))
                {
                    message << "  [" << itksys::SystemTools::GetFilenameName(file) << ":" << function << ":" << line << "]";
                }
                Instance->GetStdCoutLogger()->Write(level, message.str());
            }
        }
    protected:

        /** Constructor */
        Loggers()
        {
        }

        /** Destructor */
        virtual
        ~Loggers()
        {
        }

        /** Get a logger */
        LoggerType*
        GetLogger(const LoggerNameType &name);

        /** Print contents of a Loggers */
        void
        PrintSelf(std::ostream &os, itk::Indent indent) const;

    private:
        /** New is private due to the Singleton pattern */
        itkNewMacro (Self)

        /** The instance singleton */
        static Pointer Instance;

        typedef std::map<LoggerNameType, LoggerPointer> ContainerType;
        ContainerType m_LoggerSet;

        StdStreamLogOutputPointer m_StdCoutLogOutput;
        StdStreamLogOutputPointer m_StdCerrLogOutput;


    };
// class Logger

}
// namespace vns

#endif  // __vnsLoggers_h
