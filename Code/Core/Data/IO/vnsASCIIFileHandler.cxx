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
 * VERSION : 5-1-0 : FA : LAIG-FA-MAC-145739-CS : 6 juillet 2016 : Audit code - atoi, atof, atol            *
 * VERSION : 5-1-0 : FA : LAIG-FA-MAC-144719-CS : 17 mai 2016 : Correction warning et contrôle qualité      *
 * VERSION : 5-0-0 : FA : LAIG-FA-MAC-143764-CS : 12 avril 2016 : Mise en forme du code (indentation, etc.) *
 * VERSION : 4-4-0 : DM : LAIG-DM-MAC-1284-CNES : 27 mai 2015 : Création du plug-in Landsat8 natif          *
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id$                                                                                                     *
 *                                                                                                          *
 ************************************************************************************************************/
#include "vnsASCIIFileHandler.h"
#include "vnsLoggers.h"
#include "vnsMacro.h"
#include "itksys/SystemTools.hxx"
#include "vnsUtilities.h"

namespace vns
{

    /** Constructor */
    ASCIIFileHandler::ASCIIFileHandler() : m_Filename(""), m_IsLoaded(false)
    {
        m_LinesOfFiles.clear();
    }

    /** Destructor */
    ASCIIFileHandler::~ASCIIFileHandler()
    {
    }

    /** PrintSelf method */
    void
    ASCIIFileHandler::PrintSelf(std::ostream& os, itk::Indent indent) const
    {
        this->Superclass::PrintSelf(os, indent);
        os << indent << "FilePath : " << this->m_Filename << std::endl;
        os << indent << "Parameters List : " << std::endl;

        for (std::list<std::string>::const_iterator it = m_LinesOfFiles.begin(); it != m_LinesOfFiles.end(); ++it)
        {
            os << indent << " -> " << *it << std::endl;
        }
    }

    void
    ASCIIFileHandler::LoadFile(const std::string& filename)
    {
        // ---------------------------------------------------------------------------------------------
        m_Filename = filename;

        // ---------------------------------------------------------------------------------------------
        // If already opened, exit
        if (m_IsLoaded)
        {
            vnsExceptionMacro("vns::ASCIIFileHandler : file already loaded !" << m_Filename)
        }

        // ---------------------------------------------------------------------------------------------
        vnsLogSuperDebugMacro("vns::ASCIIFileHandler::Load : Starting loading file " << m_Filename)
        if (CanHandle(filename) != true)
        {
            vnsLogWarningMacro("vns::ASCIIFileHandler : Cannot read file " << m_Filename)
            return;
        }

        // ---------------------------------------------------------------------------------------------
        GenerateOutputInformation();

        // ---------------------------------------------------------------------------------------------
        // If load succeeded
        m_IsLoaded = true;

        vnsLogSuperDebugMacro("vns::ASCIIFileHandler::LoadFile : Successfully Loaded file " << m_Filename)
    }

    bool
    ASCIIFileHandler::CanHandle(const std::string & name)
    {
        // ---------------------------------------------------------------------------------------------
        vnsLogSuperDebugMacro("vns::ASCIIFileHandler::CanHandle starting")

        // ---------------------------------------------------------------------------------------------
        // If file does not exist, return f
        if (itksys::SystemTools::FileExists(name.c_str()) == false)
        {
            return false;
        }
        // ---------------------------------------------------------------------------------------------
        // Checks if the file is a txt
        return true;
    }

    void
    ASCIIFileHandler::GenerateOutputInformation()
    {
        // ---------------------------------------------------------------------------------------------
        std::ifstream inputstream;

        // ---------------------------------------------------------------------------------------------
        // Open the file
        inputstream.open(m_Filename.c_str());
        // ---------------------------------------------------------------------------------------------
        // Checks if the file is opened
        if (inputstream.fail())
        {
            vnsExceptionDataMacro("ASCIIFileHandler: Enable to open the file "<<m_Filename<<".")
        }

        // ---------------------------------------------------------------------------------------------
        m_LinesOfFiles.clear();
        std::string line("");

        // ---------------------------------------------------------------------------------------------
        // Loops on the input file
        while (std::getline(inputstream, line))
        {
            if (line.empty() == false)
            {
                // ---------------------------------------------------------------------------------------------
                // Add the line in the list
                m_LinesOfFiles.push_back(line);
            }
        }
        // ---------------------------------------------------------------------------------------------
        // Close the file
        inputstream.close();
        // ---------------------------------------------------------------------------------------------
        // Checks if the list is empty
        if (m_LinesOfFiles.empty() == true)
        {
            vnsExceptionDataMacro("ASCIIFileHandler: The file "<<m_Filename<<" is empty !")
        }
    }

    std::string
    ASCIIFileHandler::GetRawParameterValue(const std::string& ParamName)
    {
        // ---------------------------------------------------------------------------------------------
        const std::string l_ParamName(ParamName);
        // ---------------------------------------------------------------------------------------------
        // Read the file
        for (std::list<std::string>::const_iterator i = m_LinesOfFiles.begin(); i != m_LinesOfFiles.end(); ++i)
        {
            // ---------------------------------------------------------------------------------------------
            // Get the line
            std::string l_Line(*i);
            // ---------------------------------------------------------------------------------------------
            const std::string::size_type pos = l_Line.find(l_ParamName.c_str());
            // ---------------------------------------------------------------------------------------------
            /** If found */
            if (pos != std::string::npos)
            {
                // ---------------------------------------------------------------------------------------------
                // Get the parameter value in the line
                l_Line.replace(pos, l_ParamName.size(), "");
                vnsLogSuperDebugMacro(
                        "vns::ASCIIFileHandler::GetRawParameterValue : read value <" << l_Line << "> for parameter <" << l_ParamName<<">.")
                return l_Line;
            }
        }

        // ---------------------------------------------------------------------------------------------
        vnsLogSuperDebugMacro("vns::ASCIIFileHandler::GetParameterValue : no value for parameter : <" << l_ParamName<<">.")
        return "";
    }

    std::string
    ASCIIFileHandler::GetParameterValue(const std::string& ParamName)
    {
        // ---------------------------------------------------------------------------------------------
        // Read the file
        std::string l_Line = GetRawParameterValue(ParamName);

        // ---------------------------------------------------------------------------------------------
        //Remove blank spaces
        std::string::iterator end_pos = std::remove(l_Line.begin(), l_Line.end(), ' ');
        l_Line.erase(end_pos, l_Line.end());

        // ---------------------------------------------------------------------------------------------
        //Remove double quotes
        end_pos = std::remove(l_Line.begin(), l_Line.end(), '"');
        l_Line.erase(end_pos, l_Line.end());

        // ---------------------------------------------------------------------------------------------
        //remove '='
        end_pos = std::remove(l_Line.begin(), l_Line.end(), '=');
        l_Line.erase(end_pos, l_Line.end());

        // ---------------------------------------------------------------------------------------------
        vnsLogSuperDebugMacro(
                "vns::ASCIIFileHandler::GetParameterValue : read value <" << l_Line << "> for parameter <" << ParamName<<">.")

        return l_Line;
    }

    std::string
    ASCIIFileHandler::GetStringValue(const std::string& ParamName)
    {
        return GetParameterValue(ParamName);
    }

    double
    ASCIIFileHandler::GetDoubleValue(const std::string& ParamName)
    {
        const std::string retval = GetParameterValue(ParamName);
        return Utilities::AsDouble(retval);
    }

    float
    ASCIIFileHandler::GetFloatValue(const std::string& ParamName)
    {
        const std::string retval = GetParameterValue(ParamName);
        return Utilities::AsFloat(retval);
    }

    int
    ASCIIFileHandler::GetIntValue(const std::string& ParamName)
    {
        const std::string retval = GetParameterValue(ParamName);
        return Utilities::AsInt(retval);
    }

    unsigned int
    ASCIIFileHandler::GetUIntValue(const std::string& ParamName)
    {
        const std::string retval = GetParameterValue(ParamName);
        return Utilities::AsUnsignedInt(retval);
    }

} // End namespace vns
