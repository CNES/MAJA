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
 * VERSION : 1.0.0 : FA : LAIG-FA-MAC-148399-CS : 24 octobre 2016 : Audit code - Correction qualite         *
 * VERSION : 1.0.0 : FA : LAIG-FA-MAC-1988-CNES : 20 octobre 2016 : Correction qualite (code mort)          *
 * VERSION : 1.0.0 : FA : LAIG-FA-MAC-1989-CNES : 13 octobre 2016 : Correction qualite (FIX ME)             *
 * VERSION : 5-1-0 : FA : LAIG-FA-MAC-145739-CS : 27 juin 2016 : Audit code - Correction du code            *
 * VERSION : 5-1-0 : FA : LAIG-FA-MAC-144674-CS : 26 mai 2016 : Modifications cosmetique et menage, etc...  *
 * VERSION : 5-1-0 : FA : LAIG-FA-MAC-144776-CS : 10 mai 2016 : Correction recherche multibaseline          *
 * VERSION : 4-0-0 : FA : LAIG-FA-MAC-117040-CS : 13 mars 2014 : Modifications mineures                     *
 * VERSION : 3-1-0 : DM : LAIG-DM-MAC-614-CNES : 28 octobre 2013 : Mise a jur suite creation plugin LANDSAT *
 * VERSION : 2-1-0 : FA : LAIG-FA-MAC-354-CNES : 8 juin 2012 : Correction qualite (regle ou metrique)       *
 *                                                                                                          *
 * VERSION : 1-0-0 : <TypeFT> : <NumFT> : 17 decembre 2010 : Creation
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id$
 *                                                                                                          *
 ************************************************************************************************************/
#include "vnsTestHelper.h"
#include "otbMacro.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include <cctype>
#include <itksys/SystemTools.hxx>

#include "otbImage.h"
#include "otbVectorImage.h"
#include "itkImageFileReader.h"
#include "otbImageFileReader.h"
#include "vnsSystem.h"
#include "vnsUtilities.h"
#include "vnsPerfoXMLFileHandler.h"

#define ITK_TEST_DIMENSION_MAX 6

// OGR Headers files
#include "cpl_conv.h"
#include "cpl_string.h"
#include "cpl_multiproc.h"
#include "ogr_api.h"

#define otbPrintDiff(comment, refStr, testStr) \
  std::cout << "   ----    '" << (comment) << "' checking   ---------------------------" << std::endl; \
  std::cout << "   Base << " << (refStr) << std::endl; \
  std::cout << "   Test >> " << (testStr) << std::endl;

#define otbCheckStringValue(Comment, RefStr, TestStr, count, report) \
    { \
    std::string strRefStr(""); if ((RefStr) != NULL) strRefStr = (RefStr); \
    std::string strTestStr(""); if (TestStr != NULL) strTestStr = (TestStr); \
    if (strRefStr != strTestStr) \
      { \
      (count)++; \
      if (report) \
        { \
        otbPrintDiff((Comment), strRefStr, strTestStr); \
        } \
      } \
    }
#define otbCheckValue(Comment, Ref, Test, count, report) \
    { \
    if ((Ref) != (Test)) \
      { \
      (count)++; \
      if (report) \
        { \
        otbPrintDiff((Comment), (Ref), (Test)); \
        } \
      } \
    }

namespace vns
{

    int
    TestHelper::RegressionTestCheckPerformances(const unsigned int p_CheckPerfosRamMax, const unsigned int p_CheckPerfosTimeMax,
            const std::string & p_CheckPerfosXmlOutptuResult)
    {
        int result = 0;
        PerfoXMLFileHandler::Pointer l_XMLHandler = PerfoXMLFileHandler::New();
        l_XMLHandler->LoadFile(p_CheckPerfosXmlOutptuResult);

        // ---------------------------------------------------------------------------------------------------
        // p_CheckPerfosRamMax in Mo (MB)
        std::ostringstream osram;
        const unsigned long l_RamPeakInMB = Utilities::AsUnsignedLong(l_XMLHandler->GetRamVmPeakValue()) / 1024L;
        osram << "Check RAM " << l_RamPeakInMB << " (real value: " << l_XMLHandler->GetRamVmPeakValue() << " ("
                << l_XMLHandler->GetRamVmPeakUnit() << ") with the reference MAX time authorized : " << p_CheckPerfosRamMax << " MB.";

        if (l_RamPeakInMB <= p_CheckPerfosRamMax)
        {
            std::cout << osram.str() << "  => Passed" << std::endl;
        }
        else
        {
            std::cout << osram.str() << "  => FAILED" << std::endl;
            result++;
        }

        // ---------------------------------------------------------------------------------------------------
        // l_CheckPerfosTimeMax in minutes
        const unsigned int l_TimeInMinutes = static_cast<unsigned int>(l_XMLHandler->GetTimeValue() / 60.);

        std::ostringstream ostime;
        ostime << "Check time " << l_TimeInMinutes << " (real value: " << l_XMLHandler->GetTimeValue() << " ("
                << l_XMLHandler->GetTimeUnit() << ") with the reference MAX time authorized : " << p_CheckPerfosTimeMax << " mn.";

        if (l_TimeInMinutes <= p_CheckPerfosTimeMax)
        {
            std::cout << ostime.str() << "  => Passed" << std::endl;
        }
        else
        {
            std::cout << ostime.str() << "  => FAILED" << std::endl;
            result++;
        }

        // Non regression test for files
        return result;
    }

    int
    TestHelper::RegressionTestAllBinaryFiles(const StringList& baselineFilenamesBinary, const StringList& testFilenamesBinary,
            const StringMap& p_AbsenceBaselineFilenames)
    {
        int result = 0;
        // Non regression test for binary files
        if ((baselineFilenamesBinary.size() > 0) && (testFilenamesBinary.size() > 0))
        {
            // Creates iterators on baseline filenames vector and test filenames vector
            StringListIt itbaselineFilenames = baselineFilenamesBinary.begin();
            StringListIt itTestFilenames = testFilenamesBinary.begin();

            // For each couple of baseline and test file, do the comparison
            for (; (itbaselineFilenames != baselineFilenamesBinary.end()) && (itTestFilenames != testFilenamesBinary.end());
                    ++itbaselineFilenames, ++itTestFilenames)
            {
                std::string baselineFilenameBinary = (*itbaselineFilenames);
                std::string testFilenameBinary = (*itTestFilenames);

                std::map<std::string, int> baselines = this->RegressionTestbaselines(const_cast<char*>(baselineFilenameBinary.c_str()));
                std::map<std::string, int>::reverse_iterator baseline = baselines.rbegin();
                int multiResult = 1;
                std::cout << "Number of baseline files: " << baselines.size() << std::endl;
                while (baseline != baselines.rend() && (multiResult != 0))
                {
                    std::cout << "Testing non-regression on file: " << (baseline->first).c_str() << std::endl;
                    this->ReportErrorsOff();
                    baseline->second = this->RegressionTestBinaryFile(testFilenameBinary.c_str(), (baseline->first).c_str());

                    multiResult = baseline->second;
                    ++baseline;
                }
                if (multiResult != 0)
                {
                    baseline = baselines.rbegin();
                    this->ReportErrorsOn();
                    baseline->second = this->RegressionTestBinaryFile(testFilenameBinary.c_str(), (baseline->first).c_str());
                }
                result = result + multiResult;
            }
        }
        if (p_AbsenceBaselineFilenames.empty() == false)
        {
            std::cout << "##################################################### " << std::endl;
            std::cout << " There are '" << p_AbsenceBaselineFilenames.size()
                    << "' files present in the output directory and not in the Baseline directory." << std::endl;
            StringMapIt it = p_AbsenceBaselineFilenames.begin();
            while (it != p_AbsenceBaselineFilenames.end())
            {
                std::cout << " File : " << itksys::SystemTools::GetFilenameName(it->second) << std::endl;
                std::cout << "         cp " << it->second << " " << it->first << "; " << std::endl;
                ++it;
            }
            result += p_AbsenceBaselineFilenames.size();
            std::cout << "#####################################################   FAILED   " << std::endl;
        }
        // Non regression test for files
        return result;
    }
    int
    TestHelper::RegressionTestAllMetaData(const StringList& baselineFilenamesMetaData, const StringList& testFilenamesMetaData)
    {
        int result = 0;
        // Non-regression test for metadata.
        if ((baselineFilenamesMetaData.size() > 0) && (testFilenamesMetaData.size() > 0))
        {
            // Creates iterators on baseline filenames vector and test filenames vector
            StringListIt itbaselineFilenames = baselineFilenamesMetaData.begin();
            StringListIt itTestFilenames = testFilenamesMetaData.begin();
            // For each couple of baseline and test file, do the comparison
            for (; (itbaselineFilenames != baselineFilenamesMetaData.end()) && (itTestFilenames != testFilenamesMetaData.end());
                    ++itbaselineFilenames, ++itTestFilenames)
            {
                std::string baselineFilenameImage = (*itbaselineFilenames);
                std::string testFilenameImage = (*itTestFilenames);

                std::map<std::string, int> baselines = this->RegressionTestbaselines(const_cast<char*>(baselineFilenameImage.c_str()));
                std::map<std::string, int>::reverse_iterator baseline = baselines.rbegin();
                int multiResult = 1;
                while (baseline != baselines.rend() && (multiResult != 0))
                {
                    this->ReportErrorsOff();
                    baseline->second = this->RegressionTestMetaData(testFilenameImage.c_str(), (baseline->first).c_str(),
                            m_ToleranceDiffValue);

                    multiResult = baseline->second;
                    ++baseline;
                }
                if (multiResult != 0)
                {
                    std::cout << "Number of baseline images: " << baselines.size() << std::endl;
                    std::cout << "Testing non-regression on image: " << (baseline->first).c_str() << std::endl;
                    baseline = baselines.rbegin();
                    this->ReportErrorsOn();
                    baseline->second = this->RegressionTestMetaData(testFilenameImage.c_str(), (baseline->first).c_str(),
                            m_ToleranceDiffValue);
                }
                result = result + multiResult;
            }
        }
        return result;
    }

    int
    TestHelper::RegressionTestAllAsciiFiles(const StringList& baselineFilenamesAscii, const StringList& testFilenamesAscii,
            const StringMap& p_AbsenceBaselineFilenames, const StringList& ignoredLines, const std::string & baseOutputdir)
    {
        int result = 0;
        //Store all the baseline update commande lines
        std::string l_fullUpdateCommand;

        // --------------------------------------------------------------------------------
        // --------------------------------------------------------------------------------
        // Non regression test for ascii files
        if ((baselineFilenamesAscii.size() > 0) && (testFilenamesAscii.size() > 0))
        {
            std::cout << "-------------  Start compare ASCII    -------------" << std::endl;

            // --------------------------------------------------------------------------------
            // --------------------------------------------------------------------------------
            // Creates iterators on baseline filenames vector and test filenames vector
            StringListIt itbaselineFilenames = baselineFilenamesAscii.begin();
            StringListIt itTestFilenames = testFilenamesAscii.begin();
            StringListIt itIgnoredLines = ignoredLines.begin();

            // --------------------------------------------------------------------------------
            // --------------------------------------------------------------------------------
            // Warning message
            if (ignoredLines.size() > 0)
            {
                std::cout << "The lines containing the expressions \n";
                for (; itIgnoredLines != ignoredLines.end(); ++itIgnoredLines)
                {
                    std::cout << " - '" << (*itIgnoredLines) << "' \n";
                }
                std::cout << "are not considered" << std::endl;
            }

            // --------------------------------------------------------------------------------
            // For each couple of baseline and test file, do the comparison
            for (; (itbaselineFilenames != baselineFilenamesAscii.end()) && (itTestFilenames != testFilenamesAscii.end());
                    ++itbaselineFilenames, ++itTestFilenames)
            {
                // --------------------------------------------------------------------------------
                this->ReportErrorsOff();
                // --------------------------------------------------------------------------------
                std::string baselineFilenameAscii = (*itbaselineFilenames);
                std::string testFilenameAscii = (*itTestFilenames);

                // --------------------------------------------------------------------------------
                std::string baseOutputdir2 = baseOutputdir;
                // --------------------------------------------------------------------------------
                if (baseOutputdir2.empty() == true)
                {
                    // --------------------------------------------------------------------------------
                    // Si repertoire non fournis alors
                    // Recupere le repertoire ou est le fichier de test, pour pouvoir le passer en parametre output_directory
                    baseOutputdir2 = itksys::SystemTools::GetFilenamePath(testFilenameAscii);
                }

                // --------------------------------------------------------------------------------
                std::map<std::string, int> baselines = this->RegressionTestbaselines(const_cast<char*>(baselineFilenameAscii.c_str()));
                std::map<std::string, int>::reverse_iterator baseline = baselines.rbegin();
                // --------------------------------------------------------------------------------
                int multiResult = 1;
                // --------------------------------------------------------------------------------
                // Loops
                while (baseline != baselines.rend() && (multiResult != 0))
                {
                    // --------------------------------------------------------------------------------
                    if (m_Verbose)
                    {
                        std::cout
                                << "-------   START      ---------------------------------------------------------------------------------------------------------------------------------------------"
                                << std::endl;
                        std::cout << "Testing the file                                  : " << testFilenameAscii << std::endl;
                        std::cout << "Number of baseline files                          : " << baselines.size() << std::endl;
                        std::cout << "Testing the non-regression on ascii file          : "
                                << itksys::SystemTools::GetFilenameName(baselineFilenameAscii) << std::endl;
                        std::cout << "Testing the non-regression on ascii file (multi)  : " << baseline->first << std::endl;
                    }
                    // --------------------------------------------------------------------------------
                    baseline->second = this->RegressionTestAsciiFile(testFilenameAscii.c_str(), (baseline->first).c_str(), m_Epsilon,
                            ignoredLines, baseOutputdir2, l_fullUpdateCommand);

                    // --------------------------------------------------------------------------------
                    if (m_Verbose)
                    {
                        std::cout << "Testing the non-regression on ascii file: "
                                << itksys::SystemTools::GetFilenameName(baselineFilenameAscii) << std::endl;
                        std::cout
                                << "-------   END      ---------------------------------------------------------------------------------------------------------------------------------------------"
                                << std::endl;
                        std::cout
                                << "--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------"
                                << std::endl;
                    }
                    // --------------------------------------------------------------------------------
                    multiResult = baseline->second;
                    ++baseline;
                }
                // --------------------------------------------------------------------------------
                if (multiResult != 0)
                {
                    std::cout << "#####################################################      "
                            << itksys::SystemTools::GetFilenameName(testFilenameAscii) << "    FAILED" << std::endl;
                    // --------------------------------------------------------------------------------
                    if (m_Verbose == false)
                    {
                        // --------------------------------------------------------------------------------
                        std::cout
                                << "-------   START      ---------------------------------------------------------------------------------------------------------------------------------------------"
                                << std::endl;
                        // --------------------------------------------------------------------------------
                        std::cout << "Number of baseline files: " << baselines.size() << std::endl;
                        // --------------------------------------------------------------------------------
                        std::cout << "Testing the non-regression on ascii file: "
                                << itksys::SystemTools::GetFilenameName(baselineFilenameAscii) << std::endl;
                        // --------------------------------------------------------------------------------
                        std::cout << "Testing non-regression on file: " << (baseline->first).c_str() << std::endl;
                    }
                    // --------------------------------------------------------------------------------
                    baseline = baselines.rbegin();
                    this->ReportErrorsOn();
                    // --------------------------------------------------------------------------------
                    baseline->second = this->RegressionTestAsciiFile(testFilenameAscii.c_str(), (baseline->first).c_str(), m_Epsilon,
                            ignoredLines, baseOutputdir2, l_fullUpdateCommand);
                    // --------------------------------------------------------------------------------
                    if (m_Verbose == false)
                    {
                        // --------------------------------------------------------------------------------
                        std::cout << "Testing the non-regression on ascii file: "
                                << itksys::SystemTools::GetFilenameName(baselineFilenameAscii) << std::endl;
                        // --------------------------------------------------------------------------------
                        std::cout
                                << "-------   END      ---------------------------------------------------------------------------------------------------------------------------------------------"
                                << std::endl;
                        // --------------------------------------------------------------------------------
                        std::cout
                                << "--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------"
                                << std::endl;
                    }
                }
                // --------------------------------------------------------------------------------
                else
                {
                    std::cout << "#####################################################      "
                            << itksys::SystemTools::GetFilenameName(testFilenameAscii) << "    SUCCESS" << std::endl;
                }
                // --------------------------------------------------------------------------------
                result += multiResult;
            }
        }
        if (p_AbsenceBaselineFilenames.empty() == false)
        {
            std::cout << "##################################################### " << std::endl;
            std::cout << " There are '" << p_AbsenceBaselineFilenames.size()
                    << "' files present in the output directory and not in the Baseline directory." << std::endl;
            StringMapIt it = p_AbsenceBaselineFilenames.begin();
            while (it != p_AbsenceBaselineFilenames.end())
            {
                std::cout << " File : " << itksys::SystemTools::GetFilenameName(it->second) << std::endl;
                std::cout << "         cp " << it->second << " " << it->first << "; " << std::endl;
                ++it;
            }
            result += p_AbsenceBaselineFilenames.size();
            std::cout << "#####################################################   FAILED   " << std::endl;
        }
        if (l_fullUpdateCommand.empty() == false)
        {
            std::cout << "##################################################### " << std::endl;
            std::cout << "Full ASCII Update Command Line : " << std::endl;
            std::cout << l_fullUpdateCommand << std::endl;
            std::cout << "##################################################### " << std::endl;
        }

        return result;
    }
    bool
    TestHelper::IsAnImageFilename(const std::string & p_filename)
    {
        const std::string l_Extensions(".jpg .JPG .tif .TIF .png .PNG .JP2 .jp2 .mha");
        const std::string l_FileExtension = itksys::SystemTools::GetFilenameExtension(p_filename);
        if ((l_FileExtension.empty() == false)
                && (l_Extensions.find(itksys::SystemTools::GetFilenameLastExtension(p_filename)) != std::string::npos))
        {
            return true;
        }
        return false;
    }
    bool
    TestHelper::IsAMAJAConfigFilename(const std::string & p_filename)
    {
        // MAJAUserConfigSystem.xml
        // MAJAUserConfig_<PLUGINNAME>.xml
        // MAJAAdminConfigSystem.xml
        // MAJAAdminConfig_<PLUGINNAME>.xml
        const std::string l_FilenameName = itksys::SystemTools::GetFilenameWithoutExtension(p_filename);
        if (l_FilenameName.find("MAJAUserConfig") != std::string::npos)
        {
            return true;
        }
        else if (l_FilenameName.find("MAJAAdminConfig") != std::string::npos)
        {
            return true;
        }
        else
        {
            return false;
        }
        return false;
    }
    bool
    TestHelper::IsAnAsciiFilename(const std::string & p_filename)
    {
        const std::string l_Extensions(".txt .log .LOG .hdr .HDR .EEF .xml .XML .xsl");
        const std::string l_FileExtension = itksys::SystemTools::GetFilenameExtension(p_filename);
        if ((l_FileExtension.empty() == false)
                && (l_Extensions.find(itksys::SystemTools::GetFilenameLastExtension(p_filename)) != std::string::npos))
        {
            return true;
        }
        return false;
    }
    bool
    TestHelper::IsAnBinaryFilename(const std::string & p_filename)
    {
        const std::string l_Extensions(".DBL .DBL.BIN .zip .dat");
        const std::string l_FileExtension = itksys::SystemTools::GetFilenameExtension(p_filename);
        if ((l_FileExtension.empty() == false)
                && (l_Extensions.find(itksys::SystemTools::GetFilenameLastExtension(p_filename)) != std::string::npos))
        {
            return true;
        }
        return false;
    }
    bool
    TestHelper::IsAMultiBaselineFilename(const std::string & p_filename)
    {
        // Filename : ..../.../toto.titi.tif
        // Filename : ..../.../toto.titi.1.tif
        //            => it's the same baseline file to be compared
        // Get toto.titi.1.tif
        const std::string l_FilenameName = itksys::SystemTools::GetFilenameName(p_filename);
        // Get .tif 
        const std::string l_LastExtension = itksys::SystemTools::GetFilenameLastExtension(l_FilenameName);
        // Get toto.titi.1
        const std::string l_FilenameWithoutLastExtension = itksys::SystemTools::GetFilenameWithoutLastExtension(l_FilenameName);

        const std::string l_LastExtension2 = itksys::SystemTools::GetFilenameLastExtension(l_FilenameWithoutLastExtension);
        //std::cout <<" Find <"<<l_FilenameName<<"> is mutli baseline ? l_LastExtension2 = "<<l_LastExtension2 <<std::endl;
        if (l_LastExtension2.empty() == false)
        {
            // remove '.'
            const std::string l_ExtentionWithoutDot = Utilities::ReplaceAllSubStrings(l_LastExtension2, ".", "");
            if (Utilities::IsANumber(l_ExtentionWithoutDot) == true)
            {
                //std::cout <<" Find <"<<l_FilenameName<<"> contains a Number -> mutli baseline TRUE" <<std::endl;
                return true;
            }
            // Check if is a ...rhelX. .... filename
            // Check also if .rhel5[6/7].
            // ex : try to detect  toto.tif.rhel5.tif
            // ex : try to detect  toto.jpg.rhel6.jpg

            // Get filename
            std::string checkRHELextension = l_LastExtension + ".rhel5" + l_LastExtension;
            //std::cout <<" Find <"<<checkRHELextension<<"> in <"<<l_FilenameName<<"> ?" <<std::endl;
            if (l_FilenameName.find(checkRHELextension) != std::string::npos)
            {
                //std::cout <<" Find <"<<checkRHELextension<<"> in <"<<l_FilenameName<<"> TRUE" <<std::endl;
                return true;
            }
            checkRHELextension = l_LastExtension + ".rhel6" + l_LastExtension;
            //std::cout <<" Find <"<<checkRHELextension<<"> in <"<<l_FilenameName<<"> ?" <<std::endl;
            if (l_FilenameName.find(checkRHELextension) != std::string::npos)
            {
                //std::cout <<" Find <"<<checkRHELextension<<"> in <"<<l_FilenameName<<"> TRUE" <<std::endl;
                return true;
            }
            checkRHELextension = l_LastExtension + ".rhel7" + l_LastExtension;
            //std::cout <<" Find <"<<checkRHELextension<<"> in <"<<l_FilenameName<<"> ?" <<std::endl;
            if (l_FilenameName.find(checkRHELextension) != std::string::npos)
            {
                //std::cout <<" Find <"<<checkRHELextension<<"> in <"<<l_FilenameName<<"> TRUE" <<std::endl;
                return true;
            }
        }
        return false;
    }

    bool
    TestHelper::IsHaveMultiBaselineFilename(const std::string & p_filename)
    {
        // Filename : ..../.../toto.titi.tif
        // Filename : ..../.../toto.titi.1.tif
        //            => it's the same baseline file to be compared
        // Get toto.titi.1.tif
        const std::string l_FilenameName = itksys::SystemTools::GetFilenameName(p_filename);
        // Get .tif
        const std::string l_LastExtension = itksys::SystemTools::GetFilenameLastExtension(l_FilenameName);

        if (l_LastExtension.empty() == false)
        {

            const std::string lMultiBaselineFileNameREHL5 = p_filename + ".rhel5" + l_LastExtension;
            const std::string lMultiBaselineFileNameREHL6 = p_filename + ".rhel6" + l_LastExtension;
            const std::string lMultiBaselineFileNameREHL7 = p_filename + ".rhel7" + l_LastExtension;
            // If one of multibase line exist
            if ((itksys::SystemTools::FileExists(lMultiBaselineFileNameREHL5.c_str()) == true)
                    || (itksys::SystemTools::FileExists(lMultiBaselineFileNameREHL6.c_str()) == true)
                    || (itksys::SystemTools::FileExists(lMultiBaselineFileNameREHL7.c_str()) == true))
            {
                vnsLogDebugMacro(" - The image ref filename  <"<<p_filename<<"> have a 'xxxx.rhelx.xxx multibaseline filename.")
                return true;
            }
        }
        return false;
    }

    bool
    TestHelper::IsAMaskBaselineFilename(const std::string & p_filename)
    {
        // Filename : ..../.../toto.titi.tif
        // Filename : ..../.../toto.titi.mask.tif
        // or         ..../.../toto.titi.mask.jpg
        // or         ..../.../toto.titi.mask.png
        //            => it's the same baseline file to be compared
        // Get toto.titi.1.tif
        const std::string l_FilenameName = itksys::SystemTools::GetFilenameName(p_filename);
        // Get toto.titi.1
        const std::string l_FilenameWithoutLastExtension = itksys::SystemTools::GetFilenameWithoutLastExtension(l_FilenameName);

        const std::string l_LastExtension = itksys::SystemTools::GetFilenameLastExtension(l_FilenameWithoutLastExtension);
        if (l_LastExtension.empty() == false)
        {
            if (l_LastExtension == ".mask")
            {
                return true;
            }
        }
        return false;
    }

    bool
    TestHelper::IsFilenameMatchWithKey(const std::string & p_filename, const StringList & selectedFilenamesWithKeyMatching)
    {
        const unsigned int l_NbKey = selectedFilenamesWithKeyMatching.size();
        if (l_NbKey == 0)
        {
            return true;
        }
        unsigned int i(0);
        bool l_match(false);
        const std::string l_Filename = itksys::SystemTools::GetFilenameName(p_filename);
        while ((l_match == false) && (i < l_NbKey))
        {
            if (l_Filename.find(selectedFilenamesWithKeyMatching[i]) != std::string::npos)
            {
                l_match = true;
            }
            i++;
        }
        return l_match;
    }

    bool
    TestHelper::IsFilenameNoMatchWithKey(const std::string & p_filename, const StringList & excludedFilenamesWithKeyMatching)
    {
        const unsigned int l_NbKey = excludedFilenamesWithKeyMatching.size();
        if (l_NbKey == 0)
        {
            return true;
        }
        unsigned int i(0);
        bool l_nomatch(true);
        const std::string l_Filename = itksys::SystemTools::GetFilenameName(p_filename);
        while ((l_nomatch == true) && (i < l_NbKey))
        {
            if (l_Filename.find(excludedFilenamesWithKeyMatching[i]) != std::string::npos)
            {
                l_nomatch = false;
            }
            i++;
        }
        return l_nomatch;
    }
    void
    TestHelper::RegressionGetListOfAllImagesFilesFromDirectory(StringList& p_baselineFilenames, StringList& p_testFilenames,
            const std::string & p_ReferenceInputDirectory, const std::string & p_ReferenceOutDirectory,
            const std::string & p_ReferenceOutDirectory2, const StringList & selectedFilenamesWithKeyMatchingImage,
            const bool p_recursively)
    {
        Utilities::PrintVector("Images : selected filenames with key matching", selectedFilenamesWithKeyMatchingImage, std::cout);
        const Utilities::ListOfStrings ListOfFilenames = System::GetListOfAllsFilesFromDirectory(p_ReferenceInputDirectory, p_recursively);
        const unsigned int l_NbFiles = ListOfFilenames.size();
        vnsLogDebugMacro("Nb files in the sub directory: "<<l_NbFiles)
        for (unsigned int i = 0; i < l_NbFiles; i++)
        {
            const std::string l_Filename = ListOfFilenames[i];
//            std::cout <<" l_Filename"<<l_Filename<<std::endl;

            if (IsAnImageFilename(l_Filename) == true)
            {
//                std::cout <<" IsImage OK"<<std::endl;
                if (IsFilenameMatchWithKey(l_Filename, selectedFilenamesWithKeyMatchingImage) == true)
                {
//                    std::cout <<" IsFilenameMatchWithKey OK"<<std::endl;
//                    std::cout <<" IsAMultiBaselineFilename(l_Filename)"<< vns::Utilities::BoolToLowerString(IsAMultiBaselineFilename(l_Filename))<<std::endl;
//                    std::cout <<" IsAMaskBaselineFilename(l_Filename)"<< vns::Utilities::BoolToLowerString(IsAMaskBaselineFilename(l_Filename))<<std::endl;
                    if ((IsAMultiBaselineFilename(l_Filename) == false) && (IsAMaskBaselineFilename(l_Filename) == false))
                    {
                        std::cout << " IsAMultiBaselineFilename false and  IsAMaskBaselineFilename false -> OK" << std::endl;
                        p_baselineFilenames.push_back(l_Filename);
                        // replace p_ReferenceInputImagesDirectory by p_ReferenceOutImagesDirectory to compute the output filename directory
                        // Find in the file is in p_ReferenceOutDirectory (Input dir of the TV or p_ReferenceOutDirectory2 output dir on the TV)
                        std::string l_Outfilename = Utilities::RenameString(l_Filename, p_ReferenceInputDirectory, p_ReferenceOutDirectory);
                        if (itksys::SystemTools::FileExists(l_Outfilename.c_str()) == true)
                        {
                            p_testFilenames.push_back(l_Outfilename);
                        }
                        else
                        {
                            l_Outfilename = Utilities::RenameString(l_Filename, p_ReferenceInputDirectory, p_ReferenceOutDirectory2);
                            p_testFilenames.push_back(l_Outfilename);
                        }
                        vnsLogDebugMacro(
                                " - Add the image ref filename  <"<<l_Filename<<"> and compare with the selected output filename <"<<l_Outfilename<<">.")
                    }
                }
            }
        }
        if (p_testFilenames.empty() == true)
        {
            vnsLogErrorMacro("The list of images filename of the baseline is empty !!")
        }
    }

    Utilities::ListOfStrings
    TestHelper::ExcludeGlobalFilenamesWithKeyMatching(const std::string & p_ReferenceInputDirectory,
            const Utilities::ListOfStrings & p_InputListOfFilenames, const StringList & p_ExcludeSudDirectories)
    {
        const unsigned int l_NbKey = p_ExcludeSudDirectories.size();
        vnsLogDebugMacro("Nb excluded sub directories: "<<l_NbKey)
        if (l_NbKey == 0)
        {
            return p_InputListOfFilenames;
        }
        Utilities::ListOfStrings l_NewFiltredFilenames;
        // Get Base name without ref

        const unsigned int l_NbInputFilenames = p_InputListOfFilenames.size();
        for (unsigned int i = 0; i < l_NbInputFilenames; i++)
        {
            // Remove root dir
            const std::string l_BaseFilename = Utilities::ReplaceAllSubStrings(p_InputListOfFilenames[i], p_ReferenceInputDirectory, "");
            unsigned int j(0);
            bool l_nomatch(true);
            while ((l_nomatch == true) && (j < l_NbKey))
            {
                if (l_BaseFilename.find(p_ExcludeSudDirectories[j]) != std::string::npos)
                {
                    l_nomatch = false;
                    vnsLogDebugMacro(
                            "Check the filename <"<< p_InputListOfFilenames[i] << "> with root base dir <"<<p_ReferenceInputDirectory<<"> MATCHES with excluded keyword <"<<p_ExcludeSudDirectories[j]<<">")
                }
                j++;
            }
            if (l_nomatch == true)
            {
                l_NewFiltredFilenames.push_back(p_InputListOfFilenames[i]);
            }
        }
        return l_NewFiltredFilenames;

    }
    TestHelper::StringMap
    TestHelper::RegressionGetListOfAllAbsenceImageFilesFromDirectory(const std::string & p_ReferenceInputDirectory,
            const std::string & p_ReferenceOutDirectory, const StringList & selectedFilenamesWithKeyMatching,
            const StringList & excludedGlobalFilenamesWithKeyMatching, const bool p_recursively)
    {
        StringMap l_AbsenceBaselineFilenames;
        // -------------------------------------------------------------------------------------------------------
        // Read all file the output dir and check if all files are present in the baseline
        // p_ReferenceOutDirectory2 == the output directory of the TV
        const Utilities::ListOfStrings l_RefOutputListOfFilenames = System::GetListOfAllsFilesFromDirectory(p_ReferenceOutDirectory,
                p_recursively);
        const Utilities::ListOfStrings l_OutputListOfFilenames = ExcludeGlobalFilenamesWithKeyMatching(p_ReferenceOutDirectory,
                l_RefOutputListOfFilenames, excludedGlobalFilenamesWithKeyMatching);

        const unsigned int l_OutputNbFiles = l_OutputListOfFilenames.size();
        vnsLogSuperDebugMacro("Nb output files in the sub directory: "<<l_OutputNbFiles)
        for (unsigned int i = 0; i < l_OutputNbFiles; i++)
        {
            const std::string l_Filename = l_OutputListOfFilenames[i];
            if (IsAnImageFilename(l_Filename) == true)
            {
                if (IsFilenameMatchWithKey(l_Filename, selectedFilenamesWithKeyMatching) == true)
                {
                    if ((IsAMultiBaselineFilename(l_Filename) == false) && (IsAMaskBaselineFilename(l_Filename) == false))
                    {
                        const std::string l_Referencefilename = Utilities::RenameString(l_Filename, p_ReferenceOutDirectory,
                                p_ReferenceInputDirectory);
                        // If toto.tif, check if the baseline toto.tif.rhelX.tif exist. If not exist, the file is absent in the baseline
                        if (IsHaveMultiBaselineFilename(l_Referencefilename) == false)
                        {
                            if (itksys::SystemTools::FileExists(l_Referencefilename.c_str()) == false)
                            {
                                l_AbsenceBaselineFilenames[l_Referencefilename] = l_Filename;
                                vnsLogDebugMacro(
                                        " - The image ref filename  <"<<l_Referencefilename<<"> is not present in the Baseline directory of the test.")
                            }
                        }
                    }
                }
            }
        }
        return l_AbsenceBaselineFilenames;
    }

    void
    TestHelper::RegressionGetListOfAllAsciiFilesFromDirectory(StringList& p_baselineFilenames, StringList& p_testFilenames,
            const std::string & p_ReferenceInputDirectory, const std::string & p_ReferenceOutDirectory,
            const std::string & p_ReferenceOutDirectory2, const StringList & selectedFilenamesWithKeyMatchingAscii,
            const StringList & excludedFilenamesWithKeyMatchingAscii, const bool p_recursively)
    {
        Utilities::PrintVector("ASCII : selected filenames with key matching", selectedFilenamesWithKeyMatchingAscii, std::cout);
        Utilities::PrintVector("ASCII : excluded filenames with key matching", excludedFilenamesWithKeyMatchingAscii, std::cout);
        const Utilities::ListOfStrings ListOfFilenames = System::GetListOfAllsFilesFromDirectory(p_ReferenceInputDirectory, p_recursively);
        const unsigned int l_NbFiles = ListOfFilenames.size();
        vnsLogDebugMacro("Nb files in the sub directory: "<<l_NbFiles)
        for (unsigned int i = 0; i < l_NbFiles; i++)
        {
            const std::string l_Filename = ListOfFilenames[i];
            if (IsAnAsciiFilename(l_Filename) == true)
            {
                if ((IsFilenameMatchWithKey(l_Filename, selectedFilenamesWithKeyMatchingAscii) == true)
                        && (IsFilenameNoMatchWithKey(l_Filename, excludedFilenamesWithKeyMatchingAscii) == true))
                {
                    // Exclude Configuration file present in the Baseline, because in the output directory, there are i the .maja-temporary-dir and removed after processing
                    // TODO : improve by creating the configuration file in the outputdir
                    if (IsAMAJAConfigFilename(l_Filename) == false)
                    {
                        // replace p_ReferenceInputImagesDirectory by p_ReferenceOutImagesDirectory to compute the output filename directory
                        // Find in the file is in p_ReferenceOutDirectory (Input dir of the TV or p_ReferenceOutDirectory2 output dir on the TV)
                        p_baselineFilenames.push_back(l_Filename);
                        std::string l_Outfilename = Utilities::RenameString(l_Filename, p_ReferenceInputDirectory, p_ReferenceOutDirectory);
                        if (itksys::SystemTools::FileExists(l_Outfilename.c_str()) == true)
                        {
                            p_testFilenames.push_back(l_Outfilename);
                        }
                        else
                        {
                            l_Outfilename = Utilities::RenameString(l_Filename, p_ReferenceInputDirectory, p_ReferenceOutDirectory2);
                            p_testFilenames.push_back(l_Outfilename);
                        }
                        vnsLogDebugMacro(
                                " - Add the ascii ref filename  <"<<l_Filename<<"> and compare with the selected output filename <"<<l_Outfilename<<">.")
                    }
                }
            }
        }
        if (p_testFilenames.empty() == true)
        {
            vnsLogErrorMacro("The list of images filename of the baseline is empty !!")
        }
    }

    TestHelper::StringMap
    TestHelper::RegressionGetListOfAllAbsenceAsciiFilesFromDirectory(const std::string & p_ReferenceInputDirectory,
            const std::string & p_ReferenceOutDirectory, const StringList & selectedFilenamesWithKeyMatching,
            const StringList & excludedFilenamesWithKeyMatching, const StringList & excludedGlobalFilenamesWithKeyMatching,
            const bool p_recursively)
    {
        StringMap l_AbsenceBaselineFilenames;
        // -------------------------------------------------------------------------------------------------------
        // Read all file the output dir and check if all files are present in the baseline
        // p_ReferenceOutDirectory2 == the output directory of the TV
        const Utilities::ListOfStrings l_RefOutputListOfFilenames = System::GetListOfAllsFilesFromDirectory(p_ReferenceOutDirectory,
                p_recursively);
        const Utilities::ListOfStrings l_OutputListOfFilenames = ExcludeGlobalFilenamesWithKeyMatching(p_ReferenceOutDirectory,
                l_RefOutputListOfFilenames, excludedGlobalFilenamesWithKeyMatching);
        const unsigned int l_OutputNbFiles = l_OutputListOfFilenames.size();
        vnsLogSuperDebugMacro("Nb output files in the sub directory: "<<l_OutputNbFiles)
        for (unsigned int i = 0; i < l_OutputNbFiles; i++)
        {
            const std::string l_Filename = l_OutputListOfFilenames[i];
            if (IsAnAsciiFilename(l_Filename) == true)
            {
                if ((IsFilenameMatchWithKey(l_Filename, selectedFilenamesWithKeyMatching) == true)
                        && (IsFilenameNoMatchWithKey(l_Filename, excludedFilenamesWithKeyMatching) == true))
                {

                    const std::string l_Referencefilename = Utilities::RenameString(l_Filename, p_ReferenceOutDirectory,
                            p_ReferenceInputDirectory);
                    if (itksys::SystemTools::FileExists(l_Referencefilename.c_str()) == false)
                    {
                        l_AbsenceBaselineFilenames[l_Referencefilename] = l_Filename;
                        vnsLogDebugMacro(
                                " - The ascii ref filename  <"<<l_Referencefilename<<"> is not present in the Baseline directory of the test.")
                    }
                }
            }
        }
        return l_AbsenceBaselineFilenames;
    }

    void
    TestHelper::RegressionGetListOfAllBinaryFilesFromDirectory(StringList& p_baselineFilenames, StringList& p_testFilenames,
            const std::string & p_ReferenceInputDirectory, const std::string & p_ReferenceOutDirectory,
            const std::string & p_ReferenceOutDirectory2, const StringList & selectedFilenamesWithKeyMatchingAscii,
            const bool p_recursively)
    {
        Utilities::PrintVector("BINARY : selected filenames with key matching", selectedFilenamesWithKeyMatchingAscii, std::cout);
        const Utilities::ListOfStrings ListOfFilenames = System::GetListOfAllsFilesFromDirectory(p_ReferenceInputDirectory, p_recursively);
        const unsigned int l_NbFiles = ListOfFilenames.size();
        vnsLogDebugMacro("Nb files in the sub directory: "<<l_NbFiles)
        for (unsigned int i = 0; i < l_NbFiles; i++)
        {
            const std::string l_Filename = ListOfFilenames[i];
            if (IsAnBinaryFilename(l_Filename) == true)
            {
                if (IsFilenameMatchWithKey(l_Filename, selectedFilenamesWithKeyMatchingAscii) == true)
                {
                    p_baselineFilenames.push_back(l_Filename);
                    // replace p_ReferenceInputImagesDirectory by p_ReferenceOutImagesDirectory to compute the output filename directory
                    // Find in the file is in p_ReferenceOutDirectory (Input dir of the TV or p_ReferenceOutDirectory2 output dir on the TV)
                    std::string l_Outfilename = Utilities::RenameString(l_Filename, p_ReferenceInputDirectory, p_ReferenceOutDirectory);
                    if (itksys::SystemTools::FileExists(l_Outfilename.c_str()) == true)
                    {
                        p_testFilenames.push_back(l_Outfilename);
                    }
                    else
                    {
                        l_Outfilename = Utilities::RenameString(l_Filename, p_ReferenceInputDirectory, p_ReferenceOutDirectory2);
                        p_testFilenames.push_back(l_Outfilename);
                    }
                    vnsLogDebugMacro(
                            " - Add the binary ref filename  <"<<l_Filename<<"> and compare with the selected output filename <"<<l_Outfilename<<">.")
                }
            }
        }
        if (p_testFilenames.empty() == true)
        {
            vnsLogErrorMacro("The list of images filename of the baseline is empty !!")
        }
    }

    TestHelper::StringMap
    TestHelper::RegressionGetListOfAllAbsenceBinaryFilesFromDirectory(const std::string & p_ReferenceInputDirectory,
            const std::string & p_ReferenceOutDirectory, const StringList & selectedFilenamesWithKeyMatching,
            const StringList & excludedGlobalFilenamesWithKeyMatching, const bool p_recursively)
    {
        StringMap l_AbsenceBaselineFilenames;
        // -------------------------------------------------------------------------------------------------------
        // Read all file the output dir and check if all files are present in the baseline
        // p_ReferenceOutDirectory2 == the output directory of the TV
        const Utilities::ListOfStrings l_RefOutputListOfFilenames = System::GetListOfAllsFilesFromDirectory(p_ReferenceOutDirectory,
                p_recursively);
        const Utilities::ListOfStrings l_OutputListOfFilenames = ExcludeGlobalFilenamesWithKeyMatching(p_ReferenceOutDirectory,
                l_RefOutputListOfFilenames, excludedGlobalFilenamesWithKeyMatching);
        const unsigned int l_OutputNbFiles = l_OutputListOfFilenames.size();
        vnsLogSuperDebugMacro("Nb output files in the sub directory: "<<l_OutputNbFiles)
        for (unsigned int i = 0; i < l_OutputNbFiles; i++)
        {
            const std::string l_Filename = l_OutputListOfFilenames[i];
            if (IsAnBinaryFilename(l_Filename) == true)
            {
                if (IsFilenameMatchWithKey(l_Filename, selectedFilenamesWithKeyMatching) == true)
                {
                    const std::string l_Referencefilename = Utilities::RenameString(l_Filename, p_ReferenceOutDirectory,
                            p_ReferenceInputDirectory);
                    if (itksys::SystemTools::FileExists(l_Referencefilename.c_str()) == false)
                    {
                        l_AbsenceBaselineFilenames[l_Referencefilename] = l_Filename;
                        vnsLogDebugMacro(
                                " - The binary ref filename  <"<<l_Referencefilename<<"> is not present in the Baseline directory of the test.")
                    }
                }
            }
        }
        return l_AbsenceBaselineFilenames;
    }

    int
    TestHelper::RegressionTestAllImages(const StringList& baselineFilenamesImage, const StringList& testFilenamesImage,
            const StringMap& p_AbsenceBaselineFilenames, const std::string & baseOutputdir)
    {
        int result = 0;
        //Store all the baseline update commande lines
        std::string l_fullUpdateCommand;
        // --------------------------------------------------------------------------------
        // Non regression test for images
        if ((baselineFilenamesImage.size() > 0) && (testFilenamesImage.size() > 0))
        {
            // --------------------------------------------------------------------------------
            std::cout << "-------------  Start compare IMAGES    -------------" << std::endl;
            // --------------------------------------------------------------------------------
            // Creates iterators on baseline filenames vector and test filenames vector
            StringListIt itbaselineFilenames = baselineFilenamesImage.begin();
            StringListIt itTestFilenames = testFilenamesImage.begin();
            int cpt(1);
            // --------------------------------------------------------------------------------
            // For each couple of baseline and test file, do the comparison
            for (; (itbaselineFilenames != baselineFilenamesImage.end()) && (itTestFilenames != testFilenamesImage.end());
                    ++itbaselineFilenames, ++itTestFilenames)
            {
                // --------------------------------------------------------------------------------
                // --------------------------------------------------------------------------------
                this->ReportErrorsOff();
                std::string baselineFilenameImage = (*itbaselineFilenames);
                std::string testFilenameImage = (*itTestFilenames);

                // --------------------------------------------------------------------------------
                std::string baseOutputdir2 = baseOutputdir;
                // --------------------------------------------------------------------------------
                if (baseOutputdir2.empty() == true)
                {
                    // --------------------------------------------------------------------------------
                    // Si repertoire non fournis alors
                    // Recupere le repertoire ou est le fichier de test, pour pouvoir le passer en parametre output_directory
                    baseOutputdir2 = itksys::SystemTools::GetFilenamePath(testFilenameImage);
                }

                // --------------------------------------------------------------------------------
                std::map<std::string, int> baselines = this->RegressionTestbaselines(const_cast<char*>(baselineFilenameImage.c_str()));
                std::map<std::string, int>::reverse_iterator baseline = baselines.rbegin();

                // --------------------------------------------------------------------------------
                // Read the extension.
                // If .mha extension file, then use ITK reader and writer filters
                // Otherwise, use OTB reader and writer filters
                const std::string l_Extension = itksys::SystemTools::GetFilenameLastExtension(baselineFilenameImage);
                bool l_UseOTBReadWriteFilters = true;
                if (l_Extension.find("mha") != std::string::npos)
                {
                    l_UseOTBReadWriteFilters = false;
                }

                // --------------------------------------------------------------------------------
                int multiResult = 1;
                int binchecksum(0);
                // --------------------------------------------------------------------------------
                bool check_origin(true);
                bool check_spacing(true);
                bool check_pixel_componenttype(true);
                // --------------------------------------------------------------------------------
                while (baseline != baselines.rend() && (multiResult != 0))
                {
                    // --------------------------------------------------------------------------------
                    if (m_Verbose)
                    {
                        // --------------------------------------------------------------------------------
                        std::cout
                                << "-------   START      ---------------------------------------------------------------------------------------------------------------------------------------------"
                                << std::endl;
                        // --------------------------------------------------------------------------------
                        std::cout << "Number of baseline images: " << baselines.size() << std::endl;
                        // --------------------------------------------------------------------------------
                        std::cout << "Testing the non-regression on image: "
                                << itksys::SystemTools::GetFilenameName((baseline->first).c_str()) << std::endl;
                        // --------------------------------------------------------------------------------
                        std::cout << baselineFilenameImage << std::endl;
                    }
                    // --------------------------------------------------------------------------------
                    if (l_UseOTBReadWriteFilters == true)
                    {
                        baseline->second = this->RegressionTestImage<otb::ImageFileReader<otb::VectorImage<double, 2> >,
                                otb::ImageFileWriter<otb::VectorImage<unsigned char, 2> > >(testFilenameImage.c_str(),
                                (baseline->first).c_str(), m_ToleranceDiffValue, baseOutputdir2, binchecksum, check_origin, check_spacing,
                                check_pixel_componenttype, l_fullUpdateCommand);
                    }
                    else
                    {
                        baseline->second = this->RegressionTestImage<itk::ImageFileReader<otb::VectorImage<double, 2> >,
                                itk::ImageFileWriter<otb::VectorImage<unsigned char, 2> > >(testFilenameImage.c_str(),
                                (baseline->first).c_str(), m_ToleranceDiffValue, baseOutputdir2, binchecksum, check_origin, check_spacing,
                                check_pixel_componenttype, l_fullUpdateCommand);
                    }
                    // --------------------------------------------------------------------------------
                    if (m_Verbose)
                    {
                        // --------------------------------------------------------------------------------
                        std::cout << "Testing the non-regression on image: "
                                << itksys::SystemTools::GetFilenameName((baseline->first).c_str()) << std::endl;
                        // --------------------------------------------------------------------------------
                        std::cout
                                << "-------   END      ---------------------------------------------------------------------------------------------------------------------------------------------"
                                << std::endl;
                    }
                    // --------------------------------------------------------------------------------
                    multiResult = baseline->second;
                    ++baseline;
                }
                // --------------------------------------------------------------------------------
                if (multiResult != 0)
                {
                    // --------------------------------------------------------------------------------
                    std::string msg = "#####################################################      "
                            + itksys::SystemTools::GetFilenameName(testFilenameImage) + "    FAILED";
                    // --------------------------------------------------------------------------------
                    if ((check_origin == false) || (check_spacing == false))
                    {
                        msg += "      Note(s): ";
                    }
                    // --------------------------------------------------------------------------------
                    if (check_pixel_componenttype == false)
                    {
                        msg += " PixelType different ;";
                    }
                    // --------------------------------------------------------------------------------
                    if (check_origin == false)
                    {
                        msg += " Origin different ;";
                    }
                    // --------------------------------------------------------------------------------
                    if (check_spacing == false)
                    {
                        msg += " Spacing different";
                    }
                    // --------------------------------------------------------------------------------
                    std::cout << msg << std::endl;

                    // --------------------------------------------------------------------------------
                    if (m_Verbose == false)
                    {
                        // --------------------------------------------------------------------------------
                        std::cout
                                << "-------   START      ---------------------------------------------------------------------------------------------------------------------------------------------"
                                << std::endl;
                        // --------------------------------------------------------------------------------
                        std::cout << "Number of baseline images: " << baselines.size() << std::endl;
                        // --------------------------------------------------------------------------------
                        std::cout << "Testing the non-regression on image: " << itksys::SystemTools::GetFilenameName(baselineFilenameImage)
                                << std::endl;
                        // --------------------------------------------------------------------------------
                        std::cout << baselineFilenameImage << std::endl;
                    }
                    // --------------------------------------------------------------------------------
                    baseline = baselines.rbegin();
                    // --------------------------------------------------------------------------------
                    this->ReportErrorsOn();
                    // --------------------------------------------------------------------------------
                    // --------------------------------------------------------------------------------
                    if (l_UseOTBReadWriteFilters == true)
                    {
                        baseline->second = this->RegressionTestImage<otb::ImageFileReader<otb::VectorImage<double, 2> >,
                                otb::ImageFileWriter<otb::VectorImage<unsigned char, 2> > >(testFilenameImage.c_str(),
                                (baseline->first).c_str(), m_ToleranceDiffValue, baseOutputdir2, binchecksum, check_origin, check_spacing,
                                check_pixel_componenttype, l_fullUpdateCommand);
                    }
                    else
                    {
                        baseline->second = this->RegressionTestImage<itk::ImageFileReader<otb::VectorImage<double, 2> >,
                                itk::ImageFileWriter<otb::VectorImage<unsigned char, 2> > >(testFilenameImage.c_str(),
                                (baseline->first).c_str(), m_ToleranceDiffValue, baseOutputdir2, binchecksum, check_origin, check_spacing,
                                check_pixel_componenttype, l_fullUpdateCommand);

                    }

                    // --------------------------------------------------------------------------------
                    if (m_Verbose == false)
                    {
                        // --------------------------------------------------------------------------------
                        std::cout << "Testing the non-regression on image: " << itksys::SystemTools::GetFilenameName(baselineFilenameImage)
                                << std::endl;
                        // --------------------------------------------------------------------------------
                        std::cout
                                << "-------   END      ---------------------------------------------------------------------------------------------------------------------------------------------"
                                << std::endl;
                    }
                }
                // --------------------------------------------------------------------------------
                else
                {
                    // --------------------------------------------------------------------------------
                    std::string msg = "#####################################################      "
                            + itksys::SystemTools::GetFilenameName(testFilenameImage) + "    SUCCESS";
                    // --------------------------------------------------------------------------------
                    if ((binchecksum != 0) || (check_origin == false) || (check_spacing == false))
                    {
                        msg += "      Note(s): ";
                    }

                    // --------------------------------------------------------------------------------
                    if (binchecksum != 0)
                    {
                        msg += " Binary different ;";
                    }
                    // --------------------------------------------------------------------------------
                    if (check_origin == false)
                    {
                        msg += " Origin different ;";
                    }
                    // --------------------------------------------------------------------------------
                    if (check_spacing == false)
                    {
                        msg += " Spacing different";
                    }
                    // --------------------------------------------------------------------------------
                    std::cout << msg << std::endl;
                }
                cpt++;
                // --------------------------------------------------------------------------------
                result += multiResult;
            }
        }
        if (p_AbsenceBaselineFilenames.empty() == false)
        {
            std::cout << "##################################################### " << std::endl;
            std::cout << " There are '" << p_AbsenceBaselineFilenames.size()
                    << "' files present in the output directory and not in the Baseline directory." << std::endl;
            StringMapIt it = p_AbsenceBaselineFilenames.begin();
            while (it != p_AbsenceBaselineFilenames.end())
            {
                std::cout << " File : " << itksys::SystemTools::GetFilenameName(it->second) << std::endl;
                std::cout << "         cp " << it->second << " " << it->first << "; " << std::endl;
                ++it;
            }
            result += p_AbsenceBaselineFilenames.size();
            std::cout << "#####################################################   FAILED   " << std::endl;
        }
        if (l_fullUpdateCommand.empty() == false)
        {
            std::cout << "##################################################### " << std::endl;
            std::cout << "Full IMAGE Update Command Line : " << std::endl;
            std::cout << l_fullUpdateCommand << std::endl;
            std::cout << "##################################################### " << std::endl;
        }

        return result;
    }
    int
    TestHelper::RegressionTestAsciiFile(const char * testAsciiFileName, const char * baselineAsciiFileName, const double epsilon,
            std::vector<std::string> ignoredLines, const std::string & baseOutputdir, std::string & fullUpdateCommandLine) const
    {
        // ---------------------------------------------------------------------------------------------
        std::string strtestAsciiFileName(testAsciiFileName);
        std::string strbaselineAsciiFileName(baselineAsciiFileName);

        // ---------------------------------------------------------------------------------------------
        if (m_ReportErrors == 0)
        {
            const System::ReturnStatus result = System::CallCommandBasic(
                    "diff -b " + strtestAsciiFileName + " " + strbaselineAsciiFileName + "  > /dev/null 2> /dev/null");
            if (result.first == 0)
            {
                return 0;
            }
        }
        else
        {
            const System::ReturnStatus result = System::CallCommandBasic("diff -b " + strtestAsciiFileName + " " + strbaselineAsciiFileName + " 1>&2");
            if (result.first == 0)
            {
                std::cout << "================================================================" << std::endl;
                std::cout << "diff -b OK for file : " << testAsciiFileName << std::endl;
            }
        }
        // ---------------------------------------------------------------------------------------------
        std::ifstream fluxfiletest(testAsciiFileName);
        std::ifstream fluxfileref(baselineAsciiFileName);

// ---------------------------------------------------------------------------------------------
//  std::string diffAsciiFileName(testAsciiFileName);
        std::string diffAsciiFileName(baseOutputdir);
        diffAsciiFileName += "/" + itksys::SystemTools::GetFilenameName(testAsciiFileName);
        diffAsciiFileName += ".diff.txt";

// ---------------------------------------------------------------------------------------------
        std::ofstream fluxfilediff;
        if (m_ReportErrors)
        {
            fluxfilediff.open(diffAsciiFileName.c_str());
        }

// ---------------------------------------------------------------------------------------------
        std::string strfiletest;
        std::string strfileref;
// ---------------------------------------------------------------------------------------------

        int nbdiff(0);
        int numLine(1);
// ---------------------------------------------------------------------------------------------
        if (!fluxfiletest)
        {
            if ((m_ReportErrors) || (m_Verbose == true))
            {
                std::cout << "Impossible to open the test ASCII file <" << testAsciiFileName << ">." << std::endl;
                std::cout << " ---------------    Remove baseline : " << std::endl;
                std::cout << "  If this file is obsolete, remove the Baseline :" << std::endl;
                std::cout << "  rm " << strbaselineAsciiFileName << "; " << std::endl;
            }
            return 1;
        }
        if (!fluxfileref)
        {
            std::cout << " ---------------    Create baseline : " << std::endl;
            std::cout << " cp " << testAsciiFileName << " " << baselineAsciiFileName << "; " << std::endl;
            itkGenericExceptionMacro(<< "Impossible to open the baseline ASCII file <"<<baselineAsciiFileName<<">.")
        }
// ---------------------------------------------------------------------------------------------
//fill up a vector of string, in which each element is one line of the file
        std::vector<std::string> listLineFileRef;
        std::vector<std::string> listLineFileTest;
// ---------------------------------------------------------------------------------------------
        while (std::getline(fluxfileref, strfileref))
        {
            listLineFileRef.push_back(strfileref);
        }
        fluxfileref.close();

// ---------------------------------------------------------------------------------------------
        while (std::getline(fluxfiletest, strfiletest))
        {
            listLineFileTest.push_back(strfiletest);
        }
        fluxfiletest.close();

// ---------------------------------------------------------------------------------------------
        if (m_IgnoreLineOrder)
        {
            std::sort(listLineFileRef.begin(), listLineFileRef.end());
            std::sort(listLineFileTest.begin(), listLineFileTest.end());
        }

//These are to save up the differences
        std::vector<std::string> listStrDiffLineFileRef;
        std::vector<std::string> listStrDiffLineFileTest;

// ---------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------
        std::vector<std::string>::iterator itRef = listLineFileRef.begin();
        std::vector<std::string>::iterator itTest = listLineFileTest.begin();
        while ((itRef != listLineFileRef.end()) && (itTest != listLineFileTest.end()))
        {
            strfileref = *itRef;
            strfiletest = *itTest;

            // ---------------------------------------------------------------------------------------------
            //Check is the current line should be ignored
            bool ignoreCurrentLineRef = false;
            bool ignoreCurrentLineTest = false;
            if (ignoredLines.size() > 0)
            {
                // ---------------------------------------------------------------------------------------------
                for (std::vector<std::string>::iterator itIgnoredLines = ignoredLines.begin(); itIgnoredLines != ignoredLines.end();
                        ++itIgnoredLines)
                {
                    std::string ignoredLinesAscii = (*itIgnoredLines);
                    std::string::size_type loc = strfileref.find(ignoredLinesAscii);
                    if (loc != std::string::npos)
                    {
                        ignoreCurrentLineRef = true;
                    }
                    loc = strfiletest.find(ignoredLinesAscii);
                    if (loc != std::string::npos)
                    {
                        ignoreCurrentLineTest = true;
                    }
                }
            }

            // ---------------------------------------------------------------------------------------------
            //Compare the lines only if none is supposed to be ignored
            //Note: the iterator increment will take care of moving only the
            //ignored one if the order does not matter
            bool differenceFoundInCurrentLine = false;
            if ((!ignoreCurrentLineRef) && (!ignoreCurrentLineTest))
            {
                //       std::cout << "Ref line:  " << strfileref << std::endl;
                //       std::cout << "Test line: " << strfiletest << std::endl;
                differenceFoundInCurrentLine = CompareLines(strfileref, strfiletest, nbdiff, fluxfilediff, numLine, listStrDiffLineFileRef,
                        listStrDiffLineFileTest, epsilon);
            }

            // ---------------------------------------------------------------------------------------------
            // ---------------------------------------------------------------------------------------------
            // ---------------------------------------------------------------------------------------------
            // ---------------------------------------------------------------------------------------------
            if (m_IgnoreLineOrder)
            {
                if (ignoreCurrentLineRef)
                {
                    ++itRef;
                }
                if (ignoreCurrentLineTest)
                {
                    ++itTest;
                }
                // ---------------------------------------------------------------------------------------------
                if ((!ignoreCurrentLineRef) && (!ignoreCurrentLineTest))
                {
                    if (differenceFoundInCurrentLine)
                    {
                        if (*itRef > *itTest)
                        {
                            fluxfilediff << "Additional line in test file: " << " : " << *itTest << std::endl;
                            ++itTest;
                        }
                        else
                        {
                            fluxfilediff << "Additional line in ref file: " << " : " << *itTest << std::endl;
                            ++itRef;
                        }
                    }
                    else
                    {
                        ++itRef;
                        ++itTest;
                    }
                }
            }
            else
            {
                ++itRef;
                ++itTest;
            }

        }

// ---------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------
// Here, the line by line comparison is finished and at least one (ref or test) is at the end
// we simply output the content of the other one

// ---------------------------------------------------------------------------------------------
        while (itRef != listLineFileRef.end())
        {
            strfileref = *itRef;
            std::stringstream buffstreamRef;
            std::string strRef = "";
            buffstreamRef << strfileref;
            buffstreamRef >> strRef;
            fluxfilediff << "Additional line in ref file: " << numLine << " : " << strRef << std::endl;
            nbdiff++;
            if (m_ReportErrors)
            {
                listStrDiffLineFileRef.push_back(strfileref);
            }
            ++itRef;
        }

// ---------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------
        while (itTest != listLineFileTest.end())
        {
            strfiletest = *itTest;
            std::stringstream buffstreamTest;
            std::string strTest = "";
            buffstreamTest << strfiletest;
            buffstreamTest >> strTest;
            fluxfilediff << "Additional line in test file: " << numLine << " : " << strTest << std::endl;
            nbdiff++;
            if (m_ReportErrors)
            {
                listStrDiffLineFileTest.push_back(strfiletest);
            }
            ++itTest;
        }

// ---------------------------------------------------------------------------------------------
        if (m_ReportErrors)
        {
            fluxfilediff.close();
        }

// ---------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------
        if (nbdiff != 0 && m_ReportErrors)
        {
            std::cout << "= = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =" << std::endl;
            std::cout << "<DartMeasurement name=\"ASCIIFileError\" type=\"numeric/int\">";
            std::cout << nbdiff;
            std::cout << "</DartMeasurement>" << std::endl;
            std::cout << "================================================================" << std::endl;
            std::cout << "baseline ASCII File : " << baselineAsciiFileName << std::endl;
            std::cout << "Test ASCII File     : " << testAsciiFileName << std::endl;
            std::cout << "Diff ASCII File     : " << diffAsciiFileName << std::endl;
            std::cout << "Tolerance value     : " << epsilon << std::endl;
            std::cout << "Tolerance max check : " << m_EpsilonBoundaryChecking << std::endl;
            std::cout << "Command diff :" << std::endl;
            std::cout << " sdiff -b " << testAsciiFileName << " " << baselineAsciiFileName << std::endl;
            std::cout << " meld " << testAsciiFileName << " " << baselineAsciiFileName << " &" << std::endl;
            std::cout << "Command update baseline :" << std::endl;
            std::cout << " cp " << testAsciiFileName << " " << baselineAsciiFileName << "; " << std::endl;
            fullUpdateCommandLine += std::string("cp ") + testAsciiFileName + " " + baselineAsciiFileName + ";";

            // ---------------------------------------------------------------------------------------------
            // Get min num line diff
            const unsigned int numLineDiff = std::min(listStrDiffLineFileRef.size(), listStrDiffLineFileTest.size());
            std::cout << "Nb lines differents : " << numLineDiff << std::endl;
            // ---------------------------------------------------------------------------------------------
            for (unsigned int i = 0; i < numLineDiff; ++i)
            {
                std::cout << "   -------------------------------" << std::endl;
                std::cout << "   Base << " << listStrDiffLineFileRef[i] << std::endl;
                std::cout << "   Test >> " << listStrDiffLineFileTest[i] << std::endl;
            }
            // ---------------------------------------------------------------------------------------------
            for (unsigned int i = numLineDiff; i < listStrDiffLineFileRef.size(); ++i)
            {
                std::cout << "   -------------------------------" << std::endl;
                std::cout << "   Base << " << listStrDiffLineFileRef[i] << std::endl;
            }
            // ---------------------------------------------------------------------------------------------
            for (unsigned int i = numLineDiff; i < listStrDiffLineFileTest.size(); ++i)
            {
                std::cout << "   -------------------------------" << std::endl;
                std::cout << "   Test >> " << listStrDiffLineFileTest[i] << std::endl;
            }
            // ---------------------------------------------------------------------------------------------
            std::cout << " => - - - - - - - - - Diff baseline command:  - - - - - - - - - " << std::endl;
            std::cout << " meld " << testAsciiFileName << " " << baselineAsciiFileName << " &" << std::endl;
            std::cout << " => - - - - - - - - - Update baseline command:  - - - - - - - - - " << std::endl;
            std::cout << "cp " << testAsciiFileName << " " << baselineAsciiFileName << std::endl;
        }
// ---------------------------------------------------------------------------------------------
        if (nbdiff != 0)
        {
            return 1;
        }
// ---------------------------------------------------------------------------------------------
        else
        {
            return 0;
        }
    }

    /******************************************/
    /******************************************/
    /******************************************/
    int
    TestHelper::RegressionTestBinaryFile(const char * testBinaryFileName, const char * baselineBinaryFileName) const
    {
// ---------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------
        std::ifstream fluxfiletest(testBinaryFileName, std::ifstream::binary);
        std::ifstream fluxfileref(baselineBinaryFileName, std::ifstream::binary);
        if (fluxfiletest.fail())
        {
            itkGenericExceptionMacro(<< "Impossible to open the test binary file <" << testBinaryFileName << ">.");
        }
        if (!fluxfileref)
        {
            std::cout << " ---------------    Create baseline : " << std::endl;
            std::cout << " cp " << testBinaryFileName << " " << baselineBinaryFileName << "; " << std::endl;
            itkGenericExceptionMacro(<< "Impossible to open the baseline binary file <" << baselineBinaryFileName << ">.");
        }
// ---------------------------------------------------------------------------------------------
        bool different(false);
// Calcul de taille fichier de test
        fluxfiletest.seekg(0, fluxfiletest.end);
        std::streampos length_test = fluxfiletest.tellg();
        fluxfiletest.seekg(0, fluxfiletest.beg);
// Calcul de taille fichier de ref
        fluxfileref.seekg(0, fluxfileref.end);
        std::streampos length_ref = fluxfileref.tellg();
        fluxfileref.seekg(0, fluxfileref.beg);

// ---------------------------------------------------------------------------------------------
        if ((length_test != length_ref))
        {
            different = true;
            if (m_ReportErrors)
            {
                std::cout << "Fichiers de tailles differentes (ref:" << length_ref << " ; test:" << length_test << ")" << std::endl;
            }
        }

// ---------------------------------------------------------------------------------------------
//Lectures des fichiers par block de 'buffer_size'
        const unsigned int buffer_size(1023);
        char buffer1[buffer_size + 1];
        char buffer2[buffer_size + 1];

// ---------------------------------------------------------------------------------------------
        while ((fluxfiletest.good() && fluxfileref.good()) && (different == false))
        {
            fluxfiletest.read(buffer1, buffer_size);
            fluxfileref.read(buffer2, buffer_size);
            buffer1[fluxfiletest.gcount()] = '\0';
            buffer2[fluxfileref.gcount()] = '\0';
            if (memcmp(buffer1, buffer2, fluxfiletest.gcount() - 1) != 0)
            {
                different = true;
            }
        }
        if (m_Verbose == true)
        {
            std::cout << " Binary different:    " << different << std::endl;
        }

// ---------------------------------------------------------------------------------------------
        fluxfiletest.close();
        fluxfileref.close();

// ---------------------------------------------------------------------------------------------
        if (different == true && m_ReportErrors)
        {
            std::cout << "<DartMeasurement name=\"BINARYFileError\" type=\"numeric/int\">";
            std::cout << "</DartMeasurement>" << std::endl;
        }
// ---------------------------------------------------------------------------------------------
        if (different == true)
        {
            return 1;
        }
// ---------------------------------------------------------------------------------------------
        else
        {
            return 0;
        }
    }

    int
    TestHelper::RegressionTestMetaData(const char *testImageFilename, const char *baselineImageFilename,
            const double /*toleranceDiffPixelImage*/) const
    {
// ---------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------
// Use the factory mechanism to read the test and baseline files and convert them to double
        typedef otb::Image<double, ITK_TEST_DIMENSION_MAX> ImageType;
        typedef otb::ImageFileReader<ImageType> ReaderType;

// ---------------------------------------------------------------------------------------------
// Read the baseline file
        ReaderType::Pointer baselineReader = ReaderType::New();
        baselineReader->SetFileName(baselineImageFilename);
        try
        {
            baselineReader->GenerateOutputInformation();
        }
        catch (itk::ExceptionObject& e)
        {
            itkGenericExceptionMacro(<< "Exception detected while reading " << baselineImageFilename << " : " << e.GetDescription());
        }

        // ---------------------------------------------------------------------------------------------
        // ---------------------------------------------------------------------------------------------
        // Read the baseline file
        ReaderType::Pointer testReader = ReaderType::New();
        testReader->SetFileName(testImageFilename);
        try
        {
            testReader->GenerateOutputInformation();
        }
        catch (itk::ExceptionObject& e)
        {
            itkGenericExceptionMacro(<< "Exception detected while reading " << baselineImageFilename << " : " << e.GetDescription());
        }

        unsigned int errcount = 0;
        // The sizes of the baseline and test image must match
        ImageType::SizeType baselineSize;
        baselineSize = baselineReader->GetOutput()->GetLargestPossibleRegion().GetSize();
        ImageType::SizeType testSize;
        testSize = testReader->GetOutput()->GetLargestPossibleRegion().GetSize();

        // ---------------------------------------------------------------------------------------------
        // ---------------------------------------------------------------------------------------------
        if (baselineSize != testSize)
        {
            std::cerr << "The size of the baseline image and Test image do not match!" << std::endl;
            std::cerr << "baseline image: " << baselineImageFilename << " has size " << baselineSize << std::endl;
            std::cerr << "Test image:     " << testImageFilename << " has size " << testSize << std::endl;
            errcount++;
        }
// ---------------------------------------------------------------------------------------------
        ImageType::Pointer blImPtr = baselineReader->GetOutput();
        ImageType::Pointer testImPtr = testReader->GetOutput();

// test orgin
        if (blImPtr->GetOrigin() != testImPtr->GetOrigin())
        {
            std::cerr << "The origin of the baseline image and Test image do not match!" << std::endl;
            std::cerr << "baseline image: " << baselineImageFilename << " has origin " << blImPtr->GetOrigin() << std::endl;
            std::cerr << "Test image:     " << testImageFilename << " has origin " << testImPtr->GetOrigin() << std::endl;
            errcount++;
        }

// test spacing
        if (blImPtr->GetSpacing() != testImPtr->GetSpacing())
        {
            std::cerr << "The spacing of the baseline image and Test image do not match!" << std::endl;
            std::cerr << "baseline image: " << baselineImageFilename << " has spacing " << blImPtr->GetSpacing() << std::endl;
            std::cerr << "Test image:     " << testImageFilename << " has spacing " << testImPtr->GetSpacing() << std::endl;
            errcount++;
        }

// test projection reference
        if (blImPtr->GetProjectionRef().compare(testImPtr->GetProjectionRef()) != 0)
        {
            std::cerr << "The projection reference of the baseline image and Test image do not match!" << std::endl;
            std::cerr << "baseline image: " << baselineImageFilename << " has projection reference " << blImPtr->GetProjectionRef()
                    << std::endl;
            std::cerr << "Test image:     " << testImageFilename << " has projection reference " << testImPtr->GetProjectionRef()
                    << std::endl;
            errcount++;
        }

// test Geographic transform
        if (blImPtr->GetGeoTransform() != testImPtr->GetGeoTransform())
        {
            std::cerr << "The geographic transform of the baseline image and Test image do not match!" << std::endl;
            std::cerr << "baseline image: " << baselineImageFilename << " has geographic transform "
                    << VectorToString(blImPtr->GetGeoTransform()) << std::endl;
            std::cerr << "Test image:     " << testImageFilename << " has geographic transform "
                    << VectorToString(testImPtr->GetGeoTransform()) << std::endl;
            errcount++;
        }

// test upper left corner
        if (blImPtr->GetUpperLeftCorner() != testImPtr->GetUpperLeftCorner())
        {
            std::cerr << "The upper left corner of the baseline image and Test image do not match!" << std::endl;
            std::cerr << "baseline image: " << baselineImageFilename << " has upper left corner "
                    << VectorToString(blImPtr->GetUpperLeftCorner()) << std::endl;
            std::cerr << "Test image:     " << testImageFilename << " has upper left corner "
                    << VectorToString(testImPtr->GetUpperLeftCorner()) << std::endl;
            errcount++;
        }

// test upper right corner
        if (blImPtr->GetUpperRightCorner() != testImPtr->GetUpperRightCorner())
        {
            std::cerr << "The upper right corner of the baseline image and Test image do not match!" << std::endl;
            std::cerr << "baseline image: " << baselineImageFilename << " has upper right corner "
                    << VectorToString(blImPtr->GetUpperRightCorner()) << std::endl;
            std::cerr << "Test image:     " << testImageFilename << " has upper right corner "
                    << VectorToString(testImPtr->GetUpperRightCorner()) << std::endl;
            errcount++;
        }

// test lower left corner
        if (blImPtr->GetLowerLeftCorner() != testImPtr->GetLowerLeftCorner())
        {
            std::cerr << "The lower left corner  of the baseline image and Test image do not match!" << std::endl;
            std::cerr << "baseline image: " << baselineImageFilename << " has lower left corner "
                    << VectorToString(blImPtr->GetLowerLeftCorner()) << std::endl;
            std::cerr << "Test image:     " << testImageFilename << " has lower left corner "
                    << VectorToString(testImPtr->GetLowerLeftCorner()) << std::endl;
            errcount++;
        }

// test lower right corner
        if (blImPtr->GetLowerRightCorner() != testImPtr->GetLowerRightCorner())
        {
            std::cerr << "The lower right of the baseline image and Test image do not match!" << std::endl;
            std::cerr << "baseline image: " << baselineImageFilename << " has lower right corner "
                    << VectorToString(blImPtr->GetLowerRightCorner()) << std::endl;
            std::cerr << "Test image:     " << testImageFilename << " has lower right corner "
                    << VectorToString(testImPtr->GetLowerRightCorner()) << std::endl;
            errcount++;
        }

// test gcp projection
        if (blImPtr->GetGCPProjection().compare(testImPtr->GetGCPProjection()) != 0)
        {
            std::cerr << "The gcp projection of the baseline image and Test image do not match!" << std::endl;
            std::cerr << "baseline image: " << baselineImageFilename << " has gcp projection " << blImPtr->GetGCPProjection() << std::endl;
            std::cerr << "Test image:     " << testImageFilename << " has gcp projection " << testImPtr->GetGCPProjection() << std::endl;
            errcount++;
        }

// test gcp count
        if (blImPtr->GetGCPCount() != testImPtr->GetGCPCount())
        {
            std::cerr << "The gcp count of the baseline image and Test image do not match!" << std::endl;
            std::cerr << "baseline image: " << baselineImageFilename << " has gcp count " << blImPtr->GetGCPCount() << std::endl;
            std::cerr << "Test image:     " << testImageFilename << " has gcp count " << testImPtr->GetGCPCount() << std::endl;
            errcount++;
        }
        else
        {
            for (unsigned int i = 0; i < blImPtr->GetGCPCount(); ++i)
            {
                if ((blImPtr->GetGCPId(i).compare(testImPtr->GetGCPId(i)) != 0)
                        || (blImPtr->GetGCPInfo(i).compare(testImPtr->GetGCPInfo(i)) != 0)
                        || (blImPtr->GetGCPRow(i) != testImPtr->GetGCPRow(i)) || (blImPtr->GetGCPCol(i) != testImPtr->GetGCPCol(i))
                        || (blImPtr->GetGCPX(i) != testImPtr->GetGCPX(i)) || (blImPtr->GetGCPY(i) != testImPtr->GetGCPY(i))
                        || (blImPtr->GetGCPZ(i) != testImPtr->GetGCPZ(i)))
                {
                    std::cerr << "The GCP number " << i << " of the baseline image and Test image do not match!" << std::endl;
                    std::cerr << "baseline image: " << baselineImageFilename << " has gcp number " << i << " (" << "id: "
                            << blImPtr->GetGCPId(i) << ", " << "info: " << blImPtr->GetGCPInfo(i) << ", " << "row: "
                            << blImPtr->GetGCPRow(i) << ", " << "col: " << blImPtr->GetGCPCol(i) << ", " << "X: " << blImPtr->GetGCPX(i)
                            << ", " << "Y: " << blImPtr->GetGCPY(i) << ", " << "Z: " << blImPtr->GetGCPZ(i) << ")" << std::endl;
                    std::cerr << "Test image:     " << testImageFilename << " has gcp  number " << i << " (" << "id: "
                            << testImPtr->GetGCPId(i) << ", " << "info: " << testImPtr->GetGCPInfo(i) << ", " << "row: "
                            << testImPtr->GetGCPRow(i) << ", " << "col: " << testImPtr->GetGCPCol(i) << ", " << "X: "
                            << testImPtr->GetGCPX(i) << ", " << "Y: " << testImPtr->GetGCPY(i) << ", " << "Z: " << testImPtr->GetGCPZ(i)
                            << ")" << std::endl;
                    errcount++;
                }
            }
        }
        if (errcount > 0)
        {
            std::cout << "<DartMeasurement name=\"MetadataError\" type=\"numeric/int\">";
            std::cout << errcount;
            std::cout << "</DartMeasurement>" << std::endl;
        }
        return errcount;

    }

//
// Generate all of the possible baselines
// The possible baselines are generated fromn the baselineFilename using the following algorithm:
// 1) strip the suffix
// 2) append a digit _x
// 3) append the original suffix.
// It the file exists, increment x and continue
//
    std::map<std::string, int>
    TestHelper::RegressionTestbaselines(char *baselineFilename) const
    {
//
// Generate all of the possible baselines
// The possible baselines are generated fromn the baselineFilename using the following algorithm:
// 1) strip the suffix
// 2) append a digit _x
// 3) append the original suffix.
// It the file exists, increment x and continue
//
// ---------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------
        std::map<std::string, int> baselines;
        baselines[std::string(baselineFilename)] = 0;

        std::string originalBaseline(baselineFilename);

        int x = 0;
        std::string::size_type suffixPos = originalBaseline.rfind(".");
        std::string suffix;
        if (suffixPos != std::string::npos)
        {
            suffix = originalBaseline.substr(suffixPos, originalBaseline.length());
            originalBaseline.erase(suffixPos, originalBaseline.length());
        }
        while (++x)
        {
            std::ostringstream filename;
            filename << originalBaseline << "." << x << suffix;
            std::ifstream filestream(filename.str().c_str());
            if (!filestream)
            {
                break;
            }
            baselines[filename.str()] = 0;
            filestream.close();
        }

        // Check also baseline with .rhel5[6/7].
        // ex : for            toto.tif
        //      try to detect  toto.tif.rhel5.tif
        // ex : for            toto.jpg
        //      try to detect  toto.jpg.rhel6.jpg

        // Get the extension of the baseline filename toto.tif -> .tif
        vnsLogSuperDebugMacro("Find the multi baseline file for the reference filename <"<<baselineFilename<<">...")
        std::string strBaselineFilename(baselineFilename);
        const std::string l_LastExtension = itksys::SystemTools::GetFilenameLastExtension(strBaselineFilename);

        //vnsLogDebugValueMacro(l_LastExtension)
        if (l_LastExtension.empty() == false)
        {
            std::string checkRHELmultibaselineFilename = strBaselineFilename + ".rhel5" + l_LastExtension;
            //vnsLogDebugValueMacro(checkRHELmultibaselineFilename)
            if (itksys::SystemTools::FileExists(checkRHELmultibaselineFilename.c_str()) == true)
            {
                baselines[checkRHELmultibaselineFilename] = 0;
            }
            checkRHELmultibaselineFilename = strBaselineFilename + ".rhel7" + l_LastExtension;
            //vnsLogDebugValueMacro(checkRHELmultibaselineFilename)
            if (itksys::SystemTools::FileExists(checkRHELmultibaselineFilename.c_str()) == true)
            {
                baselines[checkRHELmultibaselineFilename] = 0;
            }
            checkRHELmultibaselineFilename = strBaselineFilename + ".rhel6" + l_LastExtension;
            //vnsLogDebugValueMacro(checkRHELmultibaselineFilename)
            if (itksys::SystemTools::FileExists(checkRHELmultibaselineFilename.c_str()) == true)
            {
                baselines[checkRHELmultibaselineFilename] = 0;
            }
        }

        return baselines;
    }

// Regression Testing Code

    bool
    TestHelper::isNumber(int i) const
    {
        return ((i > 47) && (i < 58));
    }

    bool
    TestHelper::isHexaNumber(int i) const
    {
        return (((i > 47) && (i < 58)) || ((i > 96) && (i < 103)));
    }

    bool
    TestHelper::isPoint(int i) const
    {
        return (i == 46);
    }

    bool
    TestHelper::isMinusSign(int i) const
    {
        return (i == 45);
    }

    bool
    TestHelper::isNumeric(std::string str) const
    {
// ---------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------
        int nbOfPoints = 0;
        int nbOfNumbers = 0;
        unsigned int i = 0;
        bool result = true;

        while ((i < str.size()) && (result == true))
        {
            int number = str[i];

            if (isPoint(number))
            {
                ++nbOfPoints;
            }
            if (isNumber(number))
            {
                ++nbOfNumbers;
            }
            if ((!isNumber(number) && !isPoint(number) && !isMinusSign(number)) || (isMinusSign(number) && (i != 0)))
            {
                result = false;
            }

            ++i;
        }
        if ((str.size() == 0) || (nbOfPoints > 1) /*||(nbOfNumbers==0)*/)
        {
            result = false;
        }

        return result;
    }

    bool
    TestHelper::isScientificNumeric(std::string str) const
    {
// ---------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------
        int number(0);
        number = str[0];
        bool pointDetected(false);
        bool eDetected(false);
        bool signDetected(false);

// Analyse first character (+, -, 0...9)
        unsigned int cpt(0);
        if ((str[0] != '+') && (str[0] != '-') && (!isNumber(number)))
        {
            return false;
        }
        if ((str[0] == '+') || (str[0] == '-'))
        {
            cpt++;
            signDetected = true;
        }

        while (cpt < str.size())
        {
            if (str[cpt] == '.')
            {
                // Exit false if two points or if e already detected
                if ((pointDetected == true) || (eDetected == true))
                {
                    return false;
                }
                pointDetected = true;
            }
            else if ((str[cpt] == 'e') || (str[cpt] == 'E'))
            {
                // Exit false if two e
                if (eDetected == true)
                {
                    return false;
                }
                eDetected = true;
            }
            else if ((str[cpt] == '-') || (str[cpt] == '+'))
            {
                // Exit false if already sign with no E detected
                if ((signDetected == true) && (eDetected == false))
                {
                    return false;
                }
                signDetected = true;
            }
            else
            {
                number = str[cpt];
                if (!isNumber(number))
                {
                    return false;
                }
            }
            cpt++;
        }

        return true;
    }

    bool
    TestHelper::isHexaPointerAddress(std::string str) const
    {
// ---------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------
        unsigned int size(0);
        bool result(false);
        unsigned int start(0);
//If (0xadresss)
        if ((str[0] == 40) && (str[str.size() - 1] == 41) && (str[1] == 48) && (str[2] == 120))
        {
            result = true;
            start = 3;
            size = str.size() - 1;
        }
//If 0xadresss
        else if ((str[0] == 48) && (str[1] == 120))
        {
            result = true;
            start = 2;
            size = str.size();
        }
// if 01adress
        else if ((str[0] == 48) && (str[1] == 49))
        {
            result = true;
            start = 2;
            size = str.size();
        }
        else
        {
            result = false;
        }

        unsigned int i(start);
        while (result && (i < size))
        {
            result = result && isHexaNumber(str[i]);
            ++i;
        }
        return result;
    }

    bool
    TestHelper::CompareLines(std::string strfileref, std::string strfiletest, int& nbdiff, std::ofstream& fluxfilediff, int& numLine,
            std::vector<std::string>& listStrDiffLineFileRef, std::vector<std::string>& listStrDiffLineFileTest, double epsilon) const
    {
// ---------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------
        std::stringstream buffstreamRef, buffstreamTest;
        buffstreamRef << strfileref;
        buffstreamTest << strfiletest;
//Number of differences in the current line
        bool differenceFoundInCurrentLine = false;

        enum TypeEtat
        {
            ETAT_NUM, ETAT_CHAR
        };

        TypeEtat etatPrec(ETAT_NUM), etatCour(ETAT_NUM);

        while (buffstreamRef.peek() != EOF)
        {
            std::string strRef = "";
            std::string strTest = "";

            std::string strNumRef = "";
            std::string strCharRef = "";
            std::string strNumTest = "";
            std::string strCharTest = "";

            buffstreamRef >> strRef;
            buffstreamTest >> strTest;

            // MACCS 5.1.0 (disable warning): bool chgt = false;
            std::string charTmpRef = "";
            std::string charTmpTest = "";

            // ---------------------------------------------------------------------------------------------
            // ---------------------------------------------------------------------------------------------
            if (!isHexaPointerAddress(strRef))
            {
                //Analyse if strRef contains scientific value (ex: "-142.124e-012")
                if (isScientificNumeric(strRef))
                {
                    if (!isScientificNumeric(strTest))
                    {
                        if (m_ReportErrors)
                        {
                            fluxfilediff << "Diff at line " << numLine << " : compare numeric value with no numeric value (" << strRef
                                    << strRef << " != " << strTest << ")" << std::endl;
                            differenceFoundInCurrentLine = true;
                        }
                        nbdiff++;

                    }
                    // TODO: nettoyer le test de comparaison entre deux valeurs numériques.
                    // dans le cas où on teste deux valeurs (eg 0.0 et 0.1), si la référence est inférieur à m_EpsilonBoundaryChecking (1e-30),
                    // alors on ne trace pas d'erreur alors que l'on devrait
                    else if ((strRef != strTest)                        //&& (vcl_abs(atof(strRef.c_str())) > m_EpsilonBoundaryChecking)
                    && (vcl_abs(atof(strRef.c_str()) - atof(strTest.c_str())) > epsilon * vcl_abs(atof(strRef.c_str())))) //epsilon as relative error
                    {
                        if (m_ReportErrors)
                        {
                            fluxfilediff << "Diff at line " << numLine << " : vcl_abs ( (" << strRef << ") - (" << strTest << ") ) > " << epsilon << std::endl;
                            differenceFoundInCurrentLine = true;
                        }
                        nbdiff++;
                    }
                }
                else
                {
                    unsigned int i = 0;
                    while (i < strRef.size())
                    {
                        charTmpRef = strRef[i];

                        if (i < strTest.size())
                        {
                            charTmpTest = strTest[i];
                        }

                        if (isNumeric(charTmpRef))
                        {
                            etatCour = ETAT_NUM;
                        }
                        else
                        {
                            etatCour = ETAT_CHAR;
                        }

                        // ---------------------------------------------------------------------------------------------
                        // "reference" state initialization.
                        if (i == 0)
                        {
                            etatPrec = etatCour;
                        }

                        // ---------------------------------------------------------------------------------------------
                        // Case where there's a number after characteres.
                        if ((etatCour == ETAT_NUM) && (etatPrec == ETAT_CHAR))
                        {
                            if (strCharRef != strCharTest)
                            {
                                if (m_ReportErrors)
                                {
                                    fluxfilediff << "Diff at line " << numLine << " : " << strCharRef << " != " << strCharTest << std::endl;
                                    differenceFoundInCurrentLine = true;
                                }
                                nbdiff++;
                            }

                            strCharRef = "";
                            strCharTest = "";
                            strNumRef = charTmpRef;
                            strNumTest = charTmpTest;
                            // MACCS 5.1.0 (disable warning): chgt = true;
                        }
                        // ---------------------------------------------------------------------------------------------
                        // Case where there's a character after numbers.
                        else if ((etatCour == ETAT_CHAR) && (etatPrec == ETAT_NUM))
                        {

                            if ((strNumRef != strNumTest) //&& (vcl_abs(atof(strNumRef.c_str())) > m_EpsilonBoundaryChecking)
                            && (vcl_abs(atof(strNumRef.c_str()) - atof(strNumTest.c_str())) > epsilon * vcl_abs(atof(strNumRef.c_str()))))//epsilon as relative error
                            {
                                if (m_ReportErrors)
                                {
                                    fluxfilediff << "Diff at line " << numLine << " : vcl_abs ( (" << strNumRef << ") - (" << strNumTest << ") ) > " << epsilon << std::endl;
                                    differenceFoundInCurrentLine = true;
                                }
                                nbdiff++;
                            }

                            strNumRef = "";
                            strNumTest = "";
                            strCharRef = charTmpRef;
                            strCharTest = charTmpTest;
                            // MACCS 5.1.0 (disable warning): chgt = true;
                        }
                        else if (etatCour == etatPrec)
                        {
                            if (etatCour == ETAT_CHAR)
                            {
                                strCharRef = strCharRef + charTmpRef;
                                strCharTest = strCharTest + charTmpTest;
                            }
                            else
                            {
                                strNumRef = strNumRef + charTmpRef;
                                strNumTest = strNumTest + charTmpTest;
                            }
                        }

                        etatPrec = etatCour;
                        ++i;
                    }

                    // ---------------------------------------------------------------------------------------------
                    // ---------------------------------------------------------------------------------------------
                    // ---------------------------------------------------------------------------------------------
                    // ---------------------------------------------------------------------------------------------
                    // Simpliest case : string characters or numeric value between 2 separators
                    // or checks end of strings (if chgt=false)
                    {
                        if (strNumRef != "")
                        {

                            if ((strNumRef != strNumTest) //&& (vcl_abs(atof(strNumRef.c_str())) > m_EpsilonBoundaryChecking)
                            && (vcl_abs(atof(strNumRef.c_str()) - atof(strNumTest.c_str())) > epsilon * vcl_abs(atof(strNumRef.c_str()))))//epsilon as relative error
                            {
                                if (m_ReportErrors)
                                {
                                    fluxfilediff << "Diff at line " << numLine << " : vcl_abs( (" << strNumRef << ") - (" << strNumTest << ") ) > " << epsilon << std::endl;
                                    differenceFoundInCurrentLine = true;
                                }
                                nbdiff++;
                            }
                        }
                        else
                        {
                            if (strCharRef != strCharTest)
                            {
                                if (m_ReportErrors)
                                {
                                    fluxfilediff << "Diff at line " << numLine << " : " << strCharRef << " != " << strCharTest << std::endl;
                                    differenceFoundInCurrentLine = true;
                                }
                                nbdiff++;
                            }
                        }
                    }
                }
            }
            else
            {
                if (m_ReportErrors)
                {
                    fluxfilediff << "Pointer address found at line " << numLine << " : " << strRef << " -> comparison skipped." << std::endl;
                }
            }
        }

        numLine++;
        //Store alls differences lines
        if (differenceFoundInCurrentLine && m_ReportErrors)
        {
            listStrDiffLineFileRef.push_back(strfileref);
            listStrDiffLineFileTest.push_back(strfiletest);
        }

        return differenceFoundInCurrentLine;
    }

    std::string
    TestHelper::VectorToString(otb::MetaDataKey::VectorType vector) const
    {
// ---------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------
        std::stringstream oss;
        oss.str("");
        otb::MetaDataKey::VectorType::iterator it = vector.begin();
        oss << "[";
        while (it != vector.end())
        {
            oss << (*it);
            ++it;
            if (it == vector.end())
            {
                oss << "]";
                break;
            }
            else
            {
                oss << ", ";
            }
        }
        return oss.str();
    }

}
