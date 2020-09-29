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
 * VERSION : 5-1-0 : FA : LAIG-FA-MAC-144719-CS : 17 mai 2016 : Correction warning et contrôle qualité      *
 * VERSION : 5-1-0 : FA : LAIG-FA-MAC-1721-CNES : 3 mai 2016 : Test si fichier /proc/self/x present         *
 * VERSION : 5-0-0 : DM : LAIG-DM-MAC-143544-CS : 5 avril 2016 : Performance : ajout d'informations         *
 * VERSION : 4-4-1 : FA : LAIG-FA-MAC-135450-CS : 8 juillet 2015 : Creation pour mecanisme mesures perfos   *
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id: vnsResourceProbeHandler.cxx 6307 2013-10-28 18:23:14Z cruffel $
 *                                                                                                          *
 ************************************************************************************************************/
#include "vnsResourceProbeHandler.h"

#include "itksys/SystemInformation.hxx"
#include "vnsPerfoXMLFileHandler.h"

namespace vns
{
    ResourceProbeHandler::Pointer ResourceProbeHandler::Instance = NULL;

    const std::string ResourceProbeHandler::m_STEP_MAJA_PROCESSING = "MACCS_PROCESSING";
    const std::string ResourceProbeHandler::m_STEP_PRE_PROCESSING = "PRE_PROCESSING";
    const std::string ResourceProbeHandler::m_STEP_SCIENTIFIC_ROCESSING = "SCIENTIFIC_ROCESSING";
    const std::string ResourceProbeHandler::m_STEP_POST_PROCESSING = "POST_PROCESSING";

    const std::string ResourceProbeHandler::m_FILENAME_PROC_SELF_STAT = "/proc/self/stat";
    const std::string ResourceProbeHandler::m_FILENAME_PROC_SELF_STATUS = "/proc/self/status";
    const std::string ResourceProbeHandler::m_FILENAME_PROC_SELF_LIMITS = "/proc/self/limits";
    const std::string ResourceProbeHandler::m_FILENAME_PROC_SELF_IO = "/proc/self/io";
    const std::string ResourceProbeHandler::m_FILENAME_PROC_SELF_SCHED = "/proc/self/sched";
    const std::string ResourceProbeHandler::m_FILENAME_PROC_SELF_SCHEDSTAT = "/proc/self/schedstat";

    /** Constructor */
    ResourceProbeHandler::ResourceProbeHandler() :
            m_InternalActivatedResourceProbe(false), m_ActivatedResourceProbeInLog(false), m_ActivatedResourceProbeReport(false), m_DiscSizeOfTemporaryDirectory(
                    0), m_DiscSizeOfCachingDirectory(0), m_ExitCode(0)

    {

    }

    /** Destructor */
    ResourceProbeHandler::~ResourceProbeHandler()
    {
    }

    void
    ResourceProbeHandler::ReportToLog(LoggerBase::LevelType level) const
    {
        // -----------------------------------------------------------------------
        if (m_ActivatedResourceProbeReport == true)
        {
            // -----------------------------------------------------------------------
            ResourceProbeBaseType l_Prob = this->m_MapOfResourceProbe.at(m_STEP_MAJA_PROCESSING);
            // -----------------------------------------------------------------------
            const std::string l_SeparatorLog(
                    "==============================================================================================================");
            // -----------------------------------------------------------------------
            const std::string l_SeparatorLog2(
                    "- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -");
            // -----------------------------------------------------------------------
            vnsLogMacro(level, l_SeparatorLog)
            vnsLogMacro(level, l_SeparatorLog)
            vnsLogMacro(level, l_SeparatorLog)
            vnsLogMacro(level, " B E G I N    P E R F O R M A N C E S   I N F O R M A T I O N")
            vnsLogMacro(level, l_SeparatorLog)
            vnsLogMacro(level,
                    "PERFORMANCES: " << m_STEP_MAJA_PROCESSING << " : Time   : " << l_Prob.time.GetMean() << " ("<<l_Prob.time.GetUnit()<<")  = " << vns::Utilities::GetStringFormatedTime(l_Prob.time.GetMean())<<".")
            vnsLogMacro(level,
                    "PERFORMANCES: Exit Code : " << m_ExitCode<<".")
            vnsLogMacro(level,
                    "PERFORMANCES: " << m_STEP_MAJA_PROCESSING << " : Memory : " << l_Prob.memory.GetMean() << " ("<<l_Prob.memory.GetUnit()<<")  = " << vns::Utilities::GetStringFormatedMemory(l_Prob.memory.GetMean())<<".")

            vnsLogMacro(level,
                    "PERFORMANCES: SPACE DISC SIZE : CACHING DIRECTORY            " << vns::Utilities::GetStringFormatedSpaceDisk(m_DiscSizeOfCachingDirectory) << " ("<<m_DiscSizeOfCachingDirectory<<").")
            vnsLogMacro(level,
                    "PERFORMANCES: SPACE DISC SIZE : TOTAL TEMPORARY DIRECTORY    " << vns::Utilities::GetStringFormatedSpaceDisk(m_DiscSizeOfTemporaryDirectory) << " ("<<m_DiscSizeOfTemporaryDirectory<<").")
            // -----------------------------------------------------------------------
            vnsLogMacro(level, l_SeparatorLog2)
            // -----------------------------------------------------------------------
            vnsLogMacro(level, GetAllsProbes())
            // -----------------------------------------------------------------------
            vnsLogMacro(level, "RAM: "<< this->GetProcRawVMPeak())

            // -----------------------------------------------------------------------
            std::string l_ProcSelf = m_FILENAME_PROC_SELF_STAT;
            // -----------------------------------------------------------------------
            if (itksys::SystemTools::FileExists(l_ProcSelf) == true)
            {
                // -----------------------------------------------------------------------
                std::ostringstream os;
                vnsLogMacro(level, l_SeparatorLog2)
                os << l_ProcSelf << " file content: \n" << System::ReadAsciiFile(l_ProcSelf);
                vnsLogMacro(level, os.str())
            }
            // -----------------------------------------------------------------------
            l_ProcSelf = m_FILENAME_PROC_SELF_STATUS;
            // -----------------------------------------------------------------------
            if (itksys::SystemTools::FileExists(l_ProcSelf) == true)
            {
                // -----------------------------------------------------------------------
                std::ostringstream os;
                vnsLogMacro(level, l_SeparatorLog2)
                os << l_ProcSelf << " file content: \n" << System::ReadAsciiFile(l_ProcSelf);
                vnsLogMacro(level, os.str())
            }
            // -----------------------------------------------------------------------
            l_ProcSelf = m_FILENAME_PROC_SELF_LIMITS;
            // -----------------------------------------------------------------------
            if (itksys::SystemTools::FileExists(l_ProcSelf) == true)
            {
                // -----------------------------------------------------------------------
                std::ostringstream os;
                vnsLogMacro(level, l_SeparatorLog2)
                os << l_ProcSelf << " file content: \n" << System::ReadAsciiFile(l_ProcSelf);
                vnsLogMacro(level, os.str())
            }
            // -----------------------------------------------------------------------
            l_ProcSelf = m_FILENAME_PROC_SELF_IO;
            // -----------------------------------------------------------------------
            if (itksys::SystemTools::FileExists(l_ProcSelf) == true)
            {
                // -----------------------------------------------------------------------
                std::ostringstream os;
                vnsLogMacro(level, l_SeparatorLog2)
                os << l_ProcSelf << " file content: \n" << System::ReadAsciiFile(l_ProcSelf);
                vnsLogMacro(level, os.str())
            }
            // -----------------------------------------------------------------------
            l_ProcSelf = m_FILENAME_PROC_SELF_SCHEDSTAT;
            // -----------------------------------------------------------------------
            if (itksys::SystemTools::FileExists(l_ProcSelf) == true)
            {
                // -----------------------------------------------------------------------
                std::ostringstream os;
                vnsLogMacro(level, l_SeparatorLog2)
                os << l_ProcSelf << " file content: \n" << System::ReadAsciiFile(l_ProcSelf);
                vnsLogMacro(level, os.str())
            }
            // -----------------------------------------------------------------------
            l_ProcSelf = m_FILENAME_PROC_SELF_SCHED;
            // -----------------------------------------------------------------------
            if (itksys::SystemTools::FileExists(l_ProcSelf) == true)
            {
                // -----------------------------------------------------------------------
                std::ostringstream os;
                vnsLogMacro(level, l_SeparatorLog2)
                os << l_ProcSelf << " file content: \n" << System::ReadAsciiFile(l_ProcSelf);
                vnsLogMacro(level, os.str())
            }
            // -----------------------------------------------------------------------
            vnsLogMacro(level, l_SeparatorLog)
            vnsLogMacro(level, " E N D    P E R F O R M A N C E S   I N F O R M A T I O N")
            vnsLogMacro(level, l_SeparatorLog)
            vnsLogMacro(level, l_SeparatorLog)
            vnsLogMacro(level, l_SeparatorLog)
        }
    }

    void
    ResourceProbeHandler::ReportToXml(const std::string & xml_file, const std::string & output_directory) const
    {
        // -----------------------------------------------------------------------
        if (m_ActivatedResourceProbeReport == true)
        {
            // -----------------------------------------------------------------------
            PerfoXMLFileHandler::Pointer l_XMLHandler = PerfoXMLFileHandler::New();
            vnsLogDebugMacro("Start write PERFO XML report ...")
            // -----------------------------------------------------------------------
            const std::string l_OutputXmlFilename = output_directory + "/" + itksys::SystemTools::GetFilenameName(xml_file);
            // -----------------------------------------------------------------------
            l_XMLHandler->LoadFile(xml_file);

            // -----------------------------------------------------------------------
            ResourceProbeBaseType l_Prob = this->m_MapOfResourceProbe.at(m_STEP_MAJA_PROCESSING);

            // -----------------------------------------------------------------------
            vnsLogDebugMacro("PERFO write Prob ...")
            l_XMLHandler->SetTimeUnit(l_Prob.time.GetUnit());
            l_XMLHandler->SetTimeValue(l_Prob.time.GetMean());
            l_XMLHandler->SetExitCode(m_ExitCode);
            // -----------------------------------------------------------------------
            l_XMLHandler->SetTimeAsString(vns::Utilities::GetStringFormatedTime(l_Prob.time.GetMean()));
            l_XMLHandler->SetMemoryUnit(l_Prob.memory.GetUnit());
            l_XMLHandler->SetMemoryValue(l_Prob.memory.GetMean());

            // -----------------------------------------------------------------------
            vnsLogDebugMacro("PERFO write NbThreads ...")
            // -----------------------------------------------------------------------
            l_XMLHandler->SetNbOfThreads(itk::MultiThreader::GetGlobalDefaultNumberOfThreads());

            // -----------------------------------------------------------------------
            vnsLogDebugMacro("PERFO write SystemInformation ...")
            // -----------------------------------------------------------------------
            l_XMLHandler->SetSystemInformation(GetSystemInformation());
            vnsLogDebugMacro("PERFO write AllsProbes ...")
            l_XMLHandler->SetAllsProbes(GetAllsProbes());

            // -----------------------------------------------------------------------
            vnsLogDebugMacro("PERFO write /proc files ...")
            // -----------------------------------------------------------------------
            if (itksys::SystemTools::FileExists(m_FILENAME_PROC_SELF_STAT) == true)
            {
                // -----------------------------------------------------------------------
                l_XMLHandler->SetProcStat(System::ReadAsciiFile(m_FILENAME_PROC_SELF_STAT));
            }
            // -----------------------------------------------------------------------
            if (itksys::SystemTools::FileExists(m_FILENAME_PROC_SELF_STATUS) == true)
            {
                // -----------------------------------------------------------------------
                l_XMLHandler->SetProcStatus(System::ReadAsciiFile(m_FILENAME_PROC_SELF_STATUS));
            }
            // -----------------------------------------------------------------------
            if (itksys::SystemTools::FileExists(m_FILENAME_PROC_SELF_LIMITS) == true)
            {
                // -----------------------------------------------------------------------
                l_XMLHandler->SetProcLimits(System::ReadAsciiFile(m_FILENAME_PROC_SELF_LIMITS));
            }
            // -----------------------------------------------------------------------
            if (itksys::SystemTools::FileExists(m_FILENAME_PROC_SELF_IO) == true)
            {
                // -----------------------------------------------------------------------
                l_XMLHandler->SetProcIo(System::ReadAsciiFile(m_FILENAME_PROC_SELF_IO));
            }
            // -----------------------------------------------------------------------
            if (itksys::SystemTools::FileExists(m_FILENAME_PROC_SELF_SCHEDSTAT) == true)
            {
                // -----------------------------------------------------------------------
                l_XMLHandler->SetProcSchedstat(System::ReadAsciiFile(m_FILENAME_PROC_SELF_SCHEDSTAT));
            }
            // -----------------------------------------------------------------------
            if (itksys::SystemTools::FileExists(m_FILENAME_PROC_SELF_SCHED) == true)
            {
                // -----------------------------------------------------------------------
                l_XMLHandler->SetProcSched(System::ReadAsciiFile(m_FILENAME_PROC_SELF_SCHED));
            }
            // -----------------------------------------------------------------------
            vnsLogDebugMacro("PERFO write Space disc size ...")
            l_XMLHandler->SetSpaceDiscCachingDirectory(Utilities::GetStringFormatedSpaceDisk(m_DiscSizeOfCachingDirectory));
            l_XMLHandler->SetSpaceDiscTotalTemporaryDirectory(Utilities::GetStringFormatedSpaceDisk(m_DiscSizeOfTemporaryDirectory));

            // -----------------------------------------------------------------------
            vnsLogDebugMacro("PERFO write PEAK ...")
            // -----------------------------------------------------------------------
            std::streampos vm_peak_value(0);
            std::string vm_peak_unit("");
            // -----------------------------------------------------------------------
            this->GetProcVMPeak(vm_peak_value, vm_peak_unit);

            // -----------------------------------------------------------------------
            l_XMLHandler->SetRamVmPeakValue(Utilities::AsString(static_cast<unsigned long>(vm_peak_value)));
            l_XMLHandler->SetRamVmPeakUnit(vm_peak_unit);
            l_XMLHandler->SetRamVmPeakInMb(static_cast<unsigned int>(GetProcVMPeakInMbUnit(vm_peak_value, vm_peak_unit)));

            // -----------------------------------------------------------------------
            vnsLogDebugMacro("PERFO save xml file ...")
            // -----------------------------------------------------------------------
            l_XMLHandler->SaveFile(l_OutputXmlFilename);
        }
    }

    std::string
    ResourceProbeHandler::GetSystemInformation() const
    {
        // -----------------------------------------------------------------------
        itksys::SystemInformation mySys;
        // -----------------------------------------------------------------------
        mySys.RunCPUCheck();
        mySys.RunOSCheck();
        mySys.RunMemoryCheck();

        // -----------------------------------------------------------------------
        std::string l_Value("---------- System Information ----------\n");
        // -----------------------------------------------------------------------
        l_Value += (std::string) ("VendorString:                 ") + (std::string) (mySys.GetVendorString()) + "\n";
        l_Value += (std::string) ("VendorID:                     ") + (std::string) (mySys.GetVendorID()) + "\n";
        l_Value += (std::string) ("TypeID:                       ") + (std::string) (mySys.GetTypeID()) + "\n";
        l_Value += (std::string) ("FamilyID:                     ") + (std::string) (mySys.GetFamilyID()) + "\n";
        l_Value += (std::string) ("ModelID:                      ") + (std::string) (mySys.GetModelID()) + "\n";
        // -----------------------------------------------------------------------
        l_Value += (std::string) ("SteppingCode:                 ") + mySys.GetSteppingCode() + "\n";
        l_Value += (std::string) ("ExtendedProcessorName:        ") + (std::string) (mySys.GetExtendedProcessorName()) + "\n";
        l_Value += (std::string) ("DoesCPUSupportCPUID:          ") + Utilities::BoolToLowerString(mySys.DoesCPUSupportCPUID()) + "\n";
        // -----------------------------------------------------------------------
        l_Value += (std::string) ("ProcessorSerialNumber:        ") + (std::string) (mySys.GetProcessorSerialNumber()) + "\n";
        l_Value += (std::string) ("ProcessorCacheSize:           ") + Utilities::AsString(mySys.GetProcessorCacheSize()) + "\n";
        l_Value += (std::string) ("LogicalProcessorsPerPhysical: ") + Utilities::AsString(mySys.GetLogicalProcessorsPerPhysical()) + "\n";
        l_Value += (std::string) ("ProcessorClockFrequency:      ") + Utilities::DoubleToString(mySys.GetProcessorClockFrequency()) + "\n";
        l_Value += (std::string) ("ProcessorAPICID:              ") + Utilities::AsString(mySys.GetProcessorAPICID()) + "\n";

        // -----------------------------------------------------------------------
        l_Value += (std::string) ("OSName:                       ") + (std::string) (mySys.GetOSName()) + "\n";
        l_Value += (std::string) ("Hostname:                     ") + (std::string) (mySys.GetHostname()) + "\n";
        // -----------------------------------------------------------------------
        l_Value += (std::string) ("OSRelease:                    ") + (std::string) (mySys.GetOSRelease()) + "\n";
        l_Value += (std::string) ("OSVersion:                    ") + (std::string) (mySys.GetOSVersion()) + "\n";
        l_Value += (std::string) ("OSPlatform:                   ") + (std::string) (mySys.GetOSPlatform()) + "\n";

        // -----------------------------------------------------------------------
        l_Value += (std::string) ("Is64Bits:                     ") + Utilities::BoolToLowerString(mySys.Is64Bits()) + "\n";
        // -----------------------------------------------------------------------
        l_Value += (std::string) ("NumberOfLogicalCPU:           ") + Utilities::AsString(mySys.GetNumberOfLogicalCPU()) + "\n";
        // -----------------------------------------------------------------------
        l_Value += (std::string) ("NumberOfPhysicalCPU:          ") + Utilities::AsString(mySys.GetNumberOfPhysicalCPU()) + "\n";

        // -----------------------------------------------------------------------
        // Retrieve memory information in megabyte.
        l_Value += (std::string) ("TotalVirtualMemory:           ") + Utilities::AsString((unsigned int) mySys.GetTotalVirtualMemory())
                + "\n";
        // -----------------------------------------------------------------------
        l_Value += (std::string) ("AvailableVirtualMemory:       ") + Utilities::AsString((unsigned int) mySys.GetAvailableVirtualMemory())
                + "\n";
        // -----------------------------------------------------------------------
        l_Value += (std::string) ("TotalPhysicalMemory:          ") + Utilities::AsString((unsigned int) mySys.GetTotalPhysicalMemory())
                + "\n";
        // -----------------------------------------------------------------------
        l_Value += (std::string) ("AvailablePhysicalMemory:      ") + Utilities::AsString((unsigned int) mySys.GetAvailablePhysicalMemory())
                + "\n";
        // -----------------------------------------------------------------------
        l_Value += (std::string) ("HostMemoryUsed:               ") + Utilities::AsString((unsigned long) mySys.GetHostMemoryUsed()) + "\n";
        // -----------------------------------------------------------------------
        l_Value += (std::string) ("ProcMemoryUsed:               ") + Utilities::AsString((unsigned long) mySys.GetProcMemoryUsed()) + "\n";
        // -----------------------------------------------------------------------
        l_Value += (std::string) ("HostMemoryTotal:              ") + Utilities::AsString((unsigned long) mySys.GetHostMemoryTotal())
                + "\n";
        // -----------------------------------------------------------------------
        return l_Value;
    }

    std::string
    ResourceProbeHandler::GetAllsProbes() const
    {
        // -----------------------------------------------------------------------
        std::ostringstream os;
        // -----------------------------------------------------------------------
        MapOfResourceProbeType::const_iterator probe = m_MapOfResourceProbe.begin();
        // -----------------------------------------------------------------------
        MapOfResourceProbeType::const_iterator end = m_MapOfResourceProbe.end();

        // -----------------------------------------------------------------------
        if (probe == end)
        {
            // -----------------------------------------------------------------------
            os << "No times probes have been created" << std::endl;
            return "";
        }

        // -----------------------------------------------------------------------
        os << "Resource probe information" << std::endl;
        os.width(20);
        // -----------------------------------------------------------------------
        os << " Probe Tag ";
        // -----------------------------------------------------------------------
        os.width(20);
        os << probe->second.time.GetType() << " (" << probe->second.time.GetUnit() << ")";
        // -----------------------------------------------------------------------
        os.width(20);
        os << probe->second.memory.GetType() << " (" << probe->second.memory.GetUnit() << ")";
        // -----------------------------------------------------------------------
        os.width(20);
        os << probe->second.memory.GetType() << " (unit auto)";
        os << std::endl;

        // -----------------------------------------------------------------------
        while (probe != end)
        {
            // -----------------------------------------------------------------------
            os.width(20);
            // -----------------------------------------------------------------------
            os << probe->first << "  ";
            os.width(15);
            // -----------------------------------------------------------------------
            os << probe->second.time.GetMean();
            os.width(15);
            // -----------------------------------------------------------------------
            os << probe->second.memory.GetMean();
            os.width(15);
            // -----------------------------------------------------------------------
            os << vns::Utilities::GetStringFormatedMemory(probe->second.memory.GetMean());
            os << std::endl;
            ++probe;
        }
        // -----------------------------------------------------------------------
        return os.str();
    }

    /** PrintSelf method */
    void
    ResourceProbeHandler::PrintSelf(std::ostream& os, itk::Indent indent) const
    {
        Superclass::PrintSelf(os, indent);

        // -----------------------------
        // Return some processing paraemters
        // -----------------------------
        os << "m_ActivatedResourceProbeInLog:                      " << Utilities::BoolToLowerString(m_ActivatedResourceProbeInLog)
                << std::endl;
        os << "m_ActivatedResourceProbeReport:                     " << Utilities::BoolToLowerString(m_ActivatedResourceProbeReport)
                << std::endl;
        os << "m_InternalActivatedResourceProbe:                   " << Utilities::BoolToLowerString(m_InternalActivatedResourceProbe)
                << std::endl;
        os << "m_OutputDirectoryFilename:                          " << m_OutputDirectoryFilename << std::endl;
        os << "m_XmlTemplatePerfos:                                " << m_XmlTemplatePerfos << std::endl;

    }

}
// End namespace vns
