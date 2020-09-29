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
 * VERSION : 2.0.0 : DM : LAIG-DM-MAJA-153436-CS : 6 mars 2017 : Refactoring pour ameliorer la qualite      *
 * VERSION : 5-1-0 : FA : LAIG-FA-MAC-145739-CS : 27 juin 2016 : Audit code - Correction du code            *
 * VERSION : 5-0-0 : DM : LAIG-DM-MAC-143544-CS : 5 avril 2016 : Performance : ajout d'informations         *
 * VERSION : 4-4-1 : FA : LAIG-FA-MAC-135450-CS : 8 juillet 2015 : Creation pour mecanisme mesures perfos   *
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id: vnsPerfoXMLFileHandler.h 10034 2015-05-07 14:43:56Z abricier $
 *                                                                                                          *
 ************************************************************************************************************/

#ifndef vns_PerfoXMLFileHandler_INCLUDED
#define vns_PerfoXMLFileHandler_INCLUDED

#include "vnsXMLFileHandler.h"
#include "itkObjectFactory.h"
#include <string>
#include "vnsMacro.h"
#include "vnsUtilities.h"
#include <itksys/SystemTools.hxx>

namespace vns
{

    class PerfoXMLFileHandler : public XMLFileHandler
    {
    public:
        typedef PerfoXMLFileHandler Self;
        typedef XMLFileHandler Superclass;
        typedef itk::SmartPointer<Self> Pointer;
        typedef itk::SmartPointer<const Self> ConstPointer;

        /** Run-time type information (and related methods). */
        itkTypeMacro( PerfoXMLFileHandler, XMLFileHandler )

        /** New macro for creation of through a Smart Pointer */
        itkNewMacro( Self )

        //-----------------------------------------------------------------------------
        vnsSetGetDoubleValueWithXpathMacro(TimeValue, "/Perfos/Time")
        //-----------------------------------------------------------------------------
        vnsSetGetDoubleValueWithXpathMacro(ExitCode, "/Perfos/ExitCode")
        //-----------------------------------------------------------------------------
        vnsSetGetStringAttributeWithXpathMacro(TimeUnit, "/Perfos/Time", "unit")
        //-----------------------------------------------------------------------------
        vnsSetGetStringValueWithXpathMacro(TimeAsString, "/Perfos/Time_As_String")

        //-----------------------------------------------------------------------------
        vnsSetGetDoubleValueWithXpathMacro(MemoryValue, "/Perfos/Memory")
        //-----------------------------------------------------------------------------
        vnsSetGetStringAttributeWithXpathMacro(MemoryUnit, "/Perfos/Memory", "unit")

        //-----------------------------------------------------------------------------
        vnsSetGetStringValueWithXpathMacro(RamVmPeakValue, "/Perfos/Ram_VmPeak")

        //-----------------------------------------------------------------------------
        vnsSetGetStringAttributeWithXpathMacro(RamVmPeakUnit, "/Perfos/Ram_VmPeak", "unit")
        //-----------------------------------------------------------------------------
        vnsSetGetUIntValueWithXpathMacro(RamVmPeakInMb, "/Perfos/Ram_VmPeak_In_MB")

        //-----------------------------------------------------------------------------
        vnsSetGetUIntValueWithXpathMacro(NbOfThreads, "/Perfos/NbOfThreads")

        //-----------------------------------------------------------------------------
        vnsSetGetStringValueWithXpathMacro(SystemInformation, "/Perfos/SystemInformation")
        //-----------------------------------------------------------------------------
        vnsSetGetStringValueWithXpathMacro(AllsProbes, "/Perfos/AllsProbes")
        //-----------------------------------------------------------------------------
        vnsSetGetStringValueWithXpathMacro(ProcStat, "/Perfos/Procs/Proc[@name='stat']")
        //-----------------------------------------------------------------------------
        vnsSetGetStringValueWithXpathMacro(ProcStatus, "/Perfos/Procs/Proc[@name='status']")
        //-----------------------------------------------------------------------------
        vnsSetGetStringValueWithXpathMacro(ProcLimits, "/Perfos/Procs/Proc[@name='limits']")
        //-----------------------------------------------------------------------------
        vnsSetGetStringValueWithXpathMacro(ProcIo, "/Perfos/Procs/Proc[@name='io']")

        //-----------------------------------------------------------------------------
        vnsSetGetStringValueWithXpathMacro(ProcSched, "/Perfos/Procs/Proc[@name='sched']")
        //-----------------------------------------------------------------------------
        vnsSetGetStringValueWithXpathMacro(ProcSchedstat, "/Perfos/Procs/Proc[@name='schedstat']")

        vnsSetGetStringValueWithXpathMacro(SpaceDiscCachingDirectory ,"/Perfos/Space_Disc_Of_Temporary_Directory/Caching_Directory")
        vnsSetGetStringValueWithXpathMacro(SpaceDiscTotalTemporaryDirectory ,"/Perfos/Space_Disc_Of_Temporary_Directory/Total_Temporary_Directory")

    protected:
        /** Constructeur */
        PerfoXMLFileHandler()
        {
        }

        /** Destructeur */
        virtual
        ~PerfoXMLFileHandler()
        {
        }
        /** Print contents of a PerfoXMLFileHandler */
        virtual void
        PrintSelf(std::ostream &os, itk::Indent indent) const
        { // ---------------------------------------------------------------------------------------------
            Superclass::PrintSelf(os, indent);
        }
    private:

    };

}

#endif
