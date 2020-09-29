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
 * VERSION : 2.0.0 : DM : LAIG-DM-MAJA-153436-CS : 29 mars 2017 : Refactoring pour ameliorer la qualite     *
 * VERSION : 1.0.0 : FA : LAIG-FA-MAC-148399-CS : 26 octobre 2016 : Audit code - Correction qualite         *
 * VERSION : 5-1-0 : FA : LAIG-FA-MAC-145739-CS : 28 juin 2016 : Audit code - Correction du code / clang    *
 * VERSION : 5-0-0 : DM : LAIG-DM-MAC-143544-CS : 5 avril 2016 : Performance : ajout d'informations         *
 * VERSION : 4-4-1 : FA : LAIG-FA-MAC-135450-CS : 8 juillet 2015 : Creation pour mecanisme mesures perfos   *
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id: vnsResourceProbeHandler.h 5749 2013-05-29 14:44:02Z tfeuvrie $
 *                                                                                                          *
 ************************************************************************************************************/
#ifndef __vnsResourceProbeHandler_h
#define __vnsResourceProbeHandler_h

#include "itkObject.h"
#include "itkObjectFactory.h"
#include "vnsMacro.h"
#include "vnsLoggers.h"
#include "vnsLoggerBase.h"
#include "vnsUtilities.h"
#include "vnsSystem.h"
#include "vnsXMLFileHandler.h"
#include "itksys/SystemTools.hxx"
#include <map>
#include <string>
#include "itkTimeProbe.h"
#include "itkMemoryProbe.h"
#include "itkMultiThreader.h"
#include "vnsASCIIFileHandler.h"
#include "vnsConfigure.h"

namespace vns
{
    /** \class  ResourceProbeHandler
     * \brief This class provide resource probe management.
     *
     * \author CS Systemes d'Information
     *
     */

    class ResourceProbeHandler : public itk::Object
    {
    public:
        /** Standard class typedefs. */
        typedef ResourceProbeHandler Self;
        typedef itk::Object Superclass;
        typedef itk::SmartPointer<Self> Pointer;
        typedef itk::SmartPointer<const Self> ConstPointer;

        /** Creation through object factory macro */
        itkTypeMacro(ResourceProbeHandler, Object )

        /** Typedef */
        typedef std::string IdType;

        /** Get the unique instance of type manager */
        static const Self&
        GetInstance()
        {
            // If instance was NOT already created
            if (Instance.IsNull() == true)
            {
                // Log an error message
                vnsStaticExceptionDataMacro(
                        "ResourceProbeHandler::GetInstance(): Internal error. Cannot get ResourceProbeHandler Instance : instance was not yet created.")
            }
            return *Instance;
        }
        /** Get the unique instance of type manager */
        static Self&
        GetMutableInstance()
        {
            // If instance was NOT already created
            if (Instance.IsNull() == true)
            {
                // Log an error message
                vnsStaticExceptionDataMacro(
                        "ResourceProbeHandler::GetInstance(): Internal error. Cannot get ResourceProbeHandler Instance : instance was not yet created.")
            }
            return *Instance;
        }

        static Pointer
        GetPointerInstance()
        {
            // If instance was NOT already created
            if (Instance.IsNull() == true)
            {
                // Log an error message
                vnsStaticExceptionDataMacro(
                        "ResourceProbeHandler::GetInstance(): Internal error. Cannot get ResourceProbeHandler Instance : instance was not yet created.")
            }
            return Instance;
        }

        /** Load the application */
        static void
        Initialize(const bool active_perfo_in_log, const bool active_perfo_report, std::string output_directory = "./",
                std::string XmlTemplatePerfos = "NOT_INITIALIZED")
        {
            // If instance was already created
            if (Instance)
            {
                // Log an error message and exit
                vnsStaticExceptionDataMacro("Cannot create ResourceProbeHandler Instance : instance is already created.")
            }
            // --------------------------------------
            // Initialize the Instance
            Instance = New();

            // Initialize the application
            Instance->m_ActivatedResourceProbeInLog = active_perfo_in_log;
            Instance->m_ActivatedResourceProbeReport = active_perfo_report;
            Instance->m_InternalActivatedResourceProbe = false;
            if ((active_perfo_in_log == true) || (active_perfo_report == true))
            {
                Instance->m_InternalActivatedResourceProbe = true;
                vnsLogDebugMacro("The performances measurement are activated (InLog:" << Utilities::BoolToLowerString(Instance->m_ActivatedResourceProbeInLog)<<"; Report:"<< Utilities::BoolToLowerString(Instance->m_ActivatedResourceProbeReport)<<").")
            }
            Instance->m_OutputDirectoryFilename = output_directory;
            Instance->m_XmlTemplatePerfos = XmlTemplatePerfos;

        }

        /** ProcessorName identification come from the JobOrder as string value */
        vnsMemberAndGetConstReferenceMacro(InternalActivatedResourceProbe, bool)
        vnsMemberAndGetConstReferenceMacro(ActivatedResourceProbeInLog, bool)
        vnsMemberAndGetConstReferenceMacro(ActivatedResourceProbeReport, bool)
        vnsStringMemberAndGetConstReferenceMacro (XmlTemplatePerfos)vnsStringMemberAndGetConstReferenceMacro(OutputDirectoryFilename)
//
    vnsMemberAndSetAndGetConstReferenceMacro(DiscSizeOfTemporaryDirectory, System::DiscSizeType)
    vnsMemberAndSetAndGetConstReferenceMacro(DiscSizeOfCachingDirectory, System::DiscSizeType)
    vnsMemberAndSetAndGetConstReferenceMacro(ExitCode, int)

    void
    Start(const IdType & id_name)
    {
        if (m_InternalActivatedResourceProbe == true)
        {
            vnsLogDebugMacro("ResourceProbeHandler::Start("<<id_name<<") ...")
            MapOfResourceProbeType::iterator pos = m_MapOfResourceProbe.find(id_name);
            if (pos != this->m_MapOfResourceProbe.end())
            {
                vnsStaticExceptionDataMacro("The resource probe \"" << id_name << "\" already exist. It can not be overwrited.");
            }
            m_MapOfResourceProbe[id_name].Start();
            vnsLogDebugMacro("ResourceProbeHandler::Start("<<id_name<<") done.")
        }
    }
    void
    Stop(const IdType & id_name)
    {
        if (m_InternalActivatedResourceProbe == true)
        {
            vnsLogDebugMacro("ResourceProbeHandler::Stop("<<id_name<<") ...")
            MapOfResourceProbeType::iterator pos = m_MapOfResourceProbe.find(id_name);
            if (pos == this->m_MapOfResourceProbe.end())
            {
                vnsStaticExceptionDataMacro("The resource probe \"" << id_name << "\" does not exist. It can not be stopped.");
            }
            pos->second.Stop();
            vnsLogDebugMacro("ResourceProbeHandler::Stop("<<id_name<<") done.")
        }
    }

    void
    ReportAndClose()
    {
        if (m_InternalActivatedResourceProbe == true)
        {
            // ----------------------------------------------------------------------------------
            // To be sur, close all probes
            CloseAllsProbes();

            if (m_ActivatedResourceProbeReport == true)
            {
                // ----------------------------------------------------------------------------------
                // Report all information in the LOG
                ReportToLog(vns::LoggerBase::DEBUG);
                // ----------------------------------------------------------------------------------
                // Report dedicated information in the XML perfo file
                vnsLogDebugMacro("Template of the Xml perfo filename : <"<<m_XmlTemplatePerfos<<">.")
                ReportToXml(m_XmlTemplatePerfos, m_OutputDirectoryFilename);
            }
            // ----------------------------------------------------------------------------------
            // CLear map probes
            m_MapOfResourceProbe.clear();
        }
    }
    void
    ReportToLog(LoggerBase::LevelType level) const;

    void
    ReportToXml(const std::string & xml_file, const std::string & output_directory) const;

    std::string
    GetProcRawVMPeak() const
    {
        return this->GetProcRawValue("VmPeak:");
    }

    /** ProcessorName possible identification */
    static const std::string m_STEP_MAJA_PROCESSING;
    static const std::string m_STEP_PRE_PROCESSING;
    static const std::string m_STEP_SCIENTIFIC_ROCESSING;
    static const std::string m_STEP_POST_PROCESSING;

protected:
    /** Constructor */
    ResourceProbeHandler();
    /** Destructor */
    virtual
    ~ResourceProbeHandler();

    /** PrintSelf method */
    virtual void
    PrintSelf(std::ostream& os, itk::Indent indent) const;

private:
    ResourceProbeHandler(const Self&); //purposely not implemented
    void
    operator=(const Self&);//purposely not implemented
    /** Type macro */
    itkNewMacro(Self)

    /** The instance singleton */
    static Pointer Instance;

    std::string
    GetAllsProbes() const;
    std::string
    GetSystemInformation() const;

    typedef class ResourceProbeBase
    {
    public:
        void
        Start()
        {
            time.Start();
            memory.Start();
        }
        void
        Stop()
        {
            time.Stop();
            memory.Stop();
        }

        itk::TimeProbe time;
        itk::MemoryProbe memory;
    }ResourceProbeBaseType;
    typedef std::map<IdType, ResourceProbeBaseType> MapOfResourceProbeType;

    void
    CloseAllsProbes()
    {
        MapOfResourceProbeType::iterator probe = m_MapOfResourceProbe.begin();
        MapOfResourceProbeType::const_iterator end = m_MapOfResourceProbe.end();
        while (probe != end)
        {
            probe->second.Stop();
            ++probe;
        }
    }

    std::string
    GetProcRawValue(const std::string & vm_peak_keyword) const
    {
        ASCIIFileHandler::Pointer l_ASCIIHandler = ASCIIFileHandler::New();
        l_ASCIIHandler->LoadFile(m_FILENAME_PROC_SELF_STATUS);
        std::string l_String = l_ASCIIHandler->GetRawParameterValue(vm_peak_keyword);
        return Utilities::ReplaceAllSubStrings(l_String, "\t", " ");
    }

    void
    GetProcVMPeak(std::streampos & vm_peak_value, std::string & vm_peak_unit) const
    {
        const std::string l_String = this->GetProcRawValue("VmPeak:");
        Utilities::ListOfStrings l_ListStrings = Utilities::SplitString(l_String, ' ', true /*remove_empty*/);
        vm_peak_value = static_cast<std::streampos>(Utilities::AsUnsignedLong(l_ListStrings[0]));
        vm_peak_unit = l_ListStrings[1];
    }
    unsigned long
    GetProcVMPeakInMbUnit(const std::streampos & vm_peak_value, const std::string & unit) const
    {
        if (unit == "kB")
        {
            return static_cast<unsigned long>(vm_peak_value / 1024L);
        }
        return 0;
    }
    unsigned long
    GetProcVMPeakInGbUnit(const std::streampos & vm_peak_value, const std::string & unit) const
    {
        if (unit == "kB")
        {
            return static_cast<unsigned long>(vm_peak_value / (1024L * 1024L));
        }
        return 0;
    }

    MapOfResourceProbeType m_MapOfResourceProbe;

    static const std::string m_FILENAME_PROC_SELF_STAT;
    static const std::string m_FILENAME_PROC_SELF_STATUS;
    static const std::string m_FILENAME_PROC_SELF_LIMITS;
    static const std::string m_FILENAME_PROC_SELF_IO;
    static const std::string m_FILENAME_PROC_SELF_SCHED;
    static const std::string m_FILENAME_PROC_SELF_SCHEDSTAT;
};

}
// End namespace vns
#define vnsResourceProbeLogInfoRam() \
    if( vns::ResourceProbeHandler::GetInstance().GetActivatedResourceProbeInLog() == true ) \
    { \
        vnsLogInfoMacro("RAM: "<< vns::ResourceProbeHandler::GetInstance().GetProcRawVMPeak()) \
    }
#define vnsResourceProbeLogDebugRam() \
    if( vns::ResourceProbeHandler::GetInstance().GetActivatedResourceProbeInLog() == true ) \
    { \
        vnsLogDebugMacro("RAM: "<< vns::ResourceProbeHandler::GetInstance().GetProcRawVMPeak()) \
    }
#define vnsResourceProbeLogSuperDebugRam() \
    if( vns::ResourceProbeHandler::GetInstance().GetActivatedResourceProbeInLog() == true ) \
    { \
        vnsLogSuperDebugMacro("RAM: "<< vns::ResourceProbeHandler::GetInstance().GetProcRawVMPeak()) \
    }

#endif /* __vnsResourceProbeHandler_h */
