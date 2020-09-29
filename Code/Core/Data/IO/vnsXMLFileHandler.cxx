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
 * VERSION : 2.0.0 : DM : LAIG-DM-MAJA-2240-CNES : 30 mars 2017 : Retrait de log à l'execution de commande  *
 * VERSION : 1.0.0 : FA : LAIG-FA-MAC-148399-CS : 25 octobre 2016 : Audit code - Correction qualite         *
 * VERSION : 1.0.0 : FA : LAIG-FA-MAC-1990-CNES : 13 octobre 2016 : Correction qualite (TO DO)              *
 * VERSION : 5-1-0 : FA : LAIG-FA-MAC-145739-CS : 5 aout 2016 : Audit code - Correction du code             *
 * VERSION : 5-1-0 : FA : LAIG-FA-MAC-144674-CS : 9 mai 2016 : Mise en forme                                *
 * VERSION : 5-0-0 : FA : LAIG-FA-MAC-143764-CS : 12 avril 2016 : Mise en forme du code (indentation, etc.) *
 * VERSION : 4-8-0 : FA : LAIG-FA-MAC-142832-CS : 10 mars 2016 : S2 Correction pour la lecture du           *
 *             "Mean_Viewing_Incidence_Angle_List" trié par le "band id" (GPP et PDGS) de règles de qualite *
 * VERSION : 4-7-0 : FA : LAIG-FA-MAC-1477-CNES : 7 decembre 2015 : Correction partielle des messages       *
 *                                                                    d'erreur non significatif dans le log *
 * VERSION : 4-0-0 : FA : LAIG-FA-MAC-354-CNES : 10 mars 2014 : Correction qualité sur règles de codage     *
 * VERSION : 3-1-0 : DM : LAIG-DM-MAC-614-CNES : 28 octobre 2013 : Mise a jur suite creation plugin LANDSAT *
 * VERSION : 3-0-0 : DM : LAIG-DM-MAC-239-CNES : 10 mai 2013 : Creation pour la prise en compte de la DM du  *
 *                                                            MACCS generique (PM LAIG-NT-MAC-0239-CNES)    *
 * VERSION : 1-0-0 : <TypeFT> : <NumFT> : 15 fevrier 2010 : Creation
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id$
 *                                                                                                          *
 ************************************************************************************************************/

#include "vnsXMLFileHandler.h"
#include <sstream>
#include "vnsMacro.h"
#include "vnsMath.h"
#include "vnsSystem.h"
#include "vnsUtilities.h"
#include "vnsLoggers.h"
#include "itksys/SystemTools.hxx"
#include "itkNumericTraits.h"

namespace vns
{

    const std::string XMLFileHandler::m_XMLIndentation = "    ";

    void
    XMLFileHandler::LoadFile(const std::string& filename, unsigned int options)
    {
        // ---------------------------------------------------------------------------------------------
        vnsLogDebugMacro("Start loading the XML file <" << filename<<"> ... (XMLFileHandler::LoadFile)")
        // ---------------------------------------------------------------------------------------------
        // Load the xml file with the doc pugixml variable
        pugi::xml_parse_result result = m_Doc.load_file(filename.c_str(), options);
        // ---------------------------------------------------------------------------------------------
        // Checks the result
        if (result)
        {
            m_Filename = filename;
        }
        // ---------------------------------------------------------------------------------------------
        // Throws an error
        else
        {
            vnsExceptionDataMacro(
                    "XMLFileHandler::LoadFile: Error while loading the file [" << filename << "]. Parsed with errors\n Error description: " << result.description() << "\n Error offset: " << result.offset);
        }

    }

    bool
    XMLFileHandler::CanLoadFile(const std::string& filename, unsigned int options)
    {
        // ---------------------------------------------------------------------------------------------
        // Load the xml file with the doc pugixml variable
        pugi::xml_document l_Doc;
        pugi::xml_parse_result result = l_Doc.load_file(filename.c_str(), options);
        // ---------------------------------------------------------------------------------------------
        // Checks the result
        if (result)
        {
            vnsLogSuperDebugMacro("Can open the XML file <" << filename<<"> ... (XMLFileHandler::CanLoadFile)")
            return true;
        }
        // ---------------------------------------------------------------------------------------------
        // Throws an error
        else
        {
            vnsLogSuperDebugMacro("Can NOT open the XML file <" << filename<<"> ... (XMLFileHandler::CanLoadFile)")
            return false;
        }
    }

    void
    XMLFileHandler::LoadFileAndCheck(const std::string& filename, const std::string& schemaLocationDirectoy, unsigned int options)
    {
        // ---------------------------------------------------------------------------------------------
        // Laod the file
        LoadFile(filename, options);
        // ---------------------------------------------------------------------------------------------
        // Check the file with the schema
        CheckXmlWithXsd(schemaLocationDirectoy);
    }

    void
    XMLFileHandler::CheckXmlWithXsd(const std::string & lXmlFilename, const std::string & pSchemaLocationDirectory)
    {
        // ---------------------------------------------------------------------------------------------
        vnsLogDebugMacro(
                "Start validation of the XML file <" << lXmlFilename<<"> with the schema location <" << pSchemaLocationDirectory<<">... (XMLFileHandler::CheckXmlWithXsd)");
        // ---------------------------------------------------------------------------------------------
        Pointer handler = New();
        // ---------------------------------------------------------------------------------------------
        // Load the file
        handler->LoadFile(lXmlFilename);
        // ---------------------------------------------------------------------------------------------
        // Checks the file with the schema
        handler->CheckXmlWithXsd(pSchemaLocationDirectory);
    }
    void
    XMLFileHandler::CheckXmlWithXsd(const std::string & lSchemaLocationDirectory)
    {
        // --------------------------------------
        // Get the schema location
        std::string lXsdFilename = GetSingleNode("//@xsi:schemaLocation").attribute().as_string();
        if (lXsdFilename.empty() == true)
        {
            lXsdFilename = GetSingleNode("//@xsi:noNamespaceSchemaLocation").attribute().as_string();
            if (lXsdFilename.empty() == true)
            {
                vnsExceptionDataMacro(
                        "Error while reading the attribute 'xsi:schemaLocation' or 'xsi:noNamespaceSchemaLocation' in the xml file "<<m_Filename<<". Impossible to check the file with its schema!")
            }
        }
        // --------------------------------------
        // Ex: lXsdFilename = xsi:schemaLocation="http://eop-cfi.esa.int/CFI ../../Schemas/Venus/VE_GIP_CKQLTL_QuicklookTool.xsd"
        // Extract the name of the Xsd file
        const Utilities::ListOfStrings listString = Utilities::SplitFilename(lXsdFilename, ' ', false /* bool without_extension */);
        // --------------------------------------
        // Get the Latest value
        std::string lTheXsdFilename("");
        const unsigned int nbStrings = listString.size();
        // --------------------------------------
        // Loop under strings
        for (unsigned int cpt = 0; cpt < nbStrings; cpt++)
        {
            lTheXsdFilename = listString[cpt];
            itksys::SystemTools::ConvertToUnixSlashes(lTheXsdFilename); // to be sur (because header from CNES data are generated from Window platform)
            const std::string extension = itksys::SystemTools::GetFilenameLastExtension(lTheXsdFilename);
            if (extension.find("xsd") != std::string::npos)
            {
                lTheXsdFilename = itksys::SystemTools::GetFilenameName(lTheXsdFilename);
            }

        }
        // --------------------------------------
        vnsLogDebugMacro("The Xsd Filename <" << lTheXsdFilename<<"> had been detected in the xml file <" << m_Filename<<">.");

        // --------------------------------------
        // Get the full path of the schemalocation
        const std::string lFukllXsdFilename = Utilities::JoinFilenames(lSchemaLocationDirectory, lTheXsdFilename);
        // --------------------------------------
        // Check the Xsd to the xml file
        const System::ReturnStatus result = System::CheckXmlWithXsd(m_Filename.c_str(), lFukllXsdFilename.c_str());
        if (result.first != 0)
        {
            vnsExceptionDataMacro("The xml file <"<<m_Filename<<"> is not conform with its schema <"<<lFukllXsdFilename<<"> !, Log of execution :"<<result.second)
        }
        // --------------------------------------
        vnsLogDebugMacro("The XML file <" << m_Filename<<"> is conform with its schema.");
    }

    void
    XMLFileHandler::TranslateXsl(const std::string & sourceFilename, const std::string & lXslStyleSheetFilename)
    {
        // ---------------------------------------------------------------------------------------------
        // Translate the stylesheet
        const System::ReturnStatus result = System::TranslateXsl(sourceFilename, lXslStyleSheetFilename);
        // ---------------------------------------------------------------------------------------------
        // Checks the result and throws an exception if error
        if (result.first != 0)
        {
            vnsStaticExceptionDataMacro(
                    "Error while applying the stylesheet <"<<lXslStyleSheetFilename<<"> on the xml file <"<<sourceFilename<<"> !, Log of execution :"<<result.second)
        }
    }

    void
    XMLFileHandler::SaveFile(const std::string& filename, std::string indent, unsigned int flags, pugi::xml_encoding encoding)
    {
        // ---------------------------------------------------------------------------------------------
        vnsLogDebugMacro("Save the xml file <"<<filename<<">...")
        // ---------------------------------------------------------------------------------------------
        // Save the file with the doc pugi xml method
        const bool l_return = m_Doc.save_file(filename.c_str(), indent.c_str(), flags, encoding);
        // ---------------------------------------------------------------------------------------------
        // Checks the result and throws an exception if error
        if (l_return == false)
        {
            vnsExceptionDataMacro("Error while saving the xml file "<<filename<<"!")
        }
    }

    void
    XMLFileHandler::SaveFileAndCheck(const std::string& filename, const std::string& schemaLocationDirectoy, std::string indent,
            unsigned int flags, pugi::xml_encoding encoding)
    {
        // ---------------------------------------------------------------------------------------------
        // Save the file
        SaveFile(filename, indent, flags, encoding);
        // ---------------------------------------------------------------------------------------------
        // Check the file with the schema
        CheckXmlWithXsd(filename, schemaLocationDirectoy);
    }

    void
    XMLFileHandler::SaveFile()
    {
        // ---------------------------------------------------------------------------------------------
        // Save the file with the doc pugi xml method
        const bool l_return = m_Doc.save_file(m_Filename.c_str());
        // ---------------------------------------------------------------------------------------------
        // Checks the result and throws an exception if error
        if (l_return == false)
        {
            vnsExceptionDataMacro("Error while saving the xml file "<<m_Filename<<"!")
        }
    }

    pugi::xpath_node
    XMLFileHandler::GetSingleNode(const std::string& RootXPath) const
    {
        // Ex:
        //  RootXPath   = //Image_Information/List_of_Bands
        // ---------------------------------------------------------------------------------------------
        pugi::xpath_node xnode;
        try
        {
            // ---------------------------------------------------------------------------------------------
            // Selects the node with the root path
            xnode = m_Doc.select_single_node(RootXPath.c_str());
            if (xnode == 0)
            {
                vnsLogWarningMacro(
                        "ATTENTION: XMLFileHandler::GetSingleNode: Impossible to read the XPath <"<<RootXPath<<"> from the Xml filename <"<<m_Filename<<"> !")
            }
        }
        // ---------------------------------------------------------------------------------------------
        // throws an exception id error
        catch (const pugi::xpath_exception& e)
        {
            vnsExceptionDataMacro(
                    "XMLFileHandler::GetSingleNode: PugiXml error while reading the xml node '"<<RootXPath<<"' in the xml file "<<m_Filename<<"! Details: "<<e.what())
        }
        catch (...)
        {
            vnsExceptionDataMacro(
                    "XMLFileHandler::GetSingleNode: Other error while writing the xml node '"<<RootXPath<<"' in the xml file "<<m_Filename<<"! ")
        }

        return xnode;
    }
    pugi::xpath_node
    XMLFileHandler::GetSingleNode(Pointer Doc, const std::string& RootXPath)
    {
        // Ex:
        //  RootXPath   = //Image_Information/List_of_Bands
        // ---------------------------------------------------------------------------------------------
        pugi::xpath_node xnode;
        try
        {
            // ---------------------------------------------------------------------------------------------
            // Selects the node with the root path
            xnode = Doc->m_Doc.select_single_node(RootXPath.c_str());
            if (xnode == 0)
            {
                vnsLogWarningMacro(
                        "ATTENTION: XMLFileHandler::GetSingleNode: Impossible to read the XPath <"<<RootXPath<<"> from the Xml filename <"<< Doc->m_Filename<<"> !")
            }
        }
        // ---------------------------------------------------------------------------------------------
        // throws an exception id error
        catch (const pugi::xpath_exception& e)
        {
            vnsStaticExceptionDataMacro(
                    "Error while reading the xml node '"<<RootXPath<<"' in the xml file "<< Doc->m_Filename<<"! Details: "<<e.what())
        }

        return xnode;
    }
    pugi::xpath_node_set
    XMLFileHandler::GetListOfNodes(const std::string& RootXPath) const
    {
        // Ex:
        //  RootXPath   = //Image_Information/List_of_Bands

        // ---------------------------------------------------------------------------------------------
        pugi::xpath_node_set xnode;
        try
        {
            // ---------------------------------------------------------------------------------------------
            // Selects the node with the root path
            xnode = m_Doc.select_nodes(RootXPath.c_str());
        }
        catch (const pugi::xpath_exception& e)
        {
            vnsExceptionDataMacro(
                    "Error while reading the xml nodes '"<<RootXPath<<"' in the xml file "<<m_Filename<<"! Details: "<<e.what())
        }

        return xnode;
    }

    /**
     Accessors
     */
    double
    XMLFileHandler::GetDoubleValue(const std::string& XPath) const
    {
        // ---------------------------------------------------------------------------------------------
        // returns the value read from the XPath in the dedicated format
        return GetXmlTextValue(XPath).as_double(0.); //-123456789.);
//        TODO       const double l_Min = itk::NumericTraits<double>::min();
//         double l_value = GetXmlTextValue(XPath).as_double(l_Min); //-123456789.);
//         if vnsDifferentDoubleWithEpsilonMacro(l_value, l_Min, CONST_EPSILON_20)
//         {
//         vnsExceptionDataMacro("Error while reading the double value xml node '"<<XPath<<"' in the xml file "<<m_Filename<<"!")
//         }
//         return l_value;
    }

    float
    XMLFileHandler::GetFloatValue(const std::string& XPath) const
    {
        // ---------------------------------------------------------------------------------------------
        // returns the value read from the XPath in the dedicated format
        return GetXmlTextValue(XPath).as_float(0.); //-123456789.);
        /* TODO        const float l_Min = itk::NumericTraits<float>::min();
         float l_value = GetXmlTextValue(XPath).as_float(l_Min);
         if vnsDifferentDoubleWithEpsilonMacro(l_value, l_Min, CONST_EPSILON_20)
         {
         vnsExceptionDataMacro("Error while reading the float value xml node '"<<XPath<<"' in the xml file "<<m_Filename<<"!")
         }
         return l_value; */
    }

    int
    XMLFileHandler::GetIntValue(const std::string& XPath) const
    {
        // ---------------------------------------------------------------------------------------------
        // returns the value read from the XPath in the dedicated format
        const int l_value = GetXmlTextValue(XPath).as_int(2147483647);
        if (l_value == 2147483647)
        {
            vnsExceptionDataMacro("Error while reading the integer value xml node '"<<XPath<<"' in the xml file "<<m_Filename<<"!")
        }
        // ---------------------------------------------------------------------------------------------
        return l_value;
    }

    unsigned int
    XMLFileHandler::GetUIntValue(const std::string& XPath) const
    {
        // ---------------------------------------------------------------------------------------------
        // returns the value read from the XPath in the dedicated format
        const unsigned int l_value = GetXmlTextValue(XPath).as_uint(4294967295);
        if (l_value == 4294967295)
        {
            vnsExceptionDataMacro("Error while reading the unsigned integer value xml node '"<<XPath<<"' in the xml file "<<m_Filename<<"!")
        }
        // ---------------------------------------------------------------------------------------------
        // returns the value read from the XPath in the dedicated format
        return l_value;
    }

    short
    XMLFileHandler::GetShortValue(const std::string& XPath) const
    {
        // ---------------------------------------------------------------------------------------------
        return static_cast<short>(this->GetIntValue(XPath));
    }

    unsigned short
    XMLFileHandler::GetUShortValue(const std::string& XPath) const
    {
        // ---------------------------------------------------------------------------------------------
        return static_cast<unsigned short>(this->GetUIntValue(XPath));
    }

    unsigned char
    XMLFileHandler::GetUCharValue(const std::string& XPath) const
    {
        // ---------------------------------------------------------------------------------------------
        return static_cast<unsigned char>(this->GetUIntValue(XPath));
    }

    pugi::xml_text
    XMLFileHandler::GetXmlTextValue(const std::string& XPath) const
    {
        // ---------------------------------------------------------------------------------------------
        // Selects the node with the root path
        pugi::xpath_node xnode = GetSingleNode(XPath);

        // ---------------------------------------------------------------------------------------------
        // returns the value
        return xnode.node().text();
    }

    std::string
    XMLFileHandler::GetStringValueOrEmpty(const std::string& XPath) const
    {
        // ---------------------------------------------------------------------------------------------
        vnsLogSuperDebugMacro("Get the string value for to the xpath '"<<XPath<<"' in the xml file "<<m_Filename<<"...")
        return GetXmlTextValue(XPath).as_string("");
    }

    std::string
    XMLFileHandler::GetStringValue(const std::string& XPath) const
    {
        // ---------------------------------------------------------------------------------------------
        vnsLogSuperDebugMacro("Get the string value for to the xpath '"<<XPath<<"' in the xml file "<<m_Filename<<"...")
        const std::string l_string = GetXmlTextValue(XPath).as_string("NO_READING_VALUE");
        // ---------------------------------------------------------------------------------------------
        // Checks if the value exists
        if (l_string == "NO_READING_VALUE")
        {
            vnsExceptionDataMacro("Error while reading the value xml node '"<<XPath<<"' in the xml file "<<m_Filename<<"!")
        }
        // ---------------------------------------------------------------------------------------------
        // returns the value
        return l_string;
    }

    bool
    XMLFileHandler::GetBoolValue(const std::string& XPath) const
    {
        // ---------------------------------------------------------------------------------------------
        return Utilities::AsBool(GetStringValue(XPath));
    }

    pugi::xml_attribute
    XMLFileHandler::GetXmlAttributeValue(const std::string& XPath, const std::string& Attribute) const
    {
        // ---------------------------------------------------------------------------------------------
        // Selects the node with the root path
        pugi::xpath_node xnode = GetSingleNode(XPath);

        // ---------------------------------------------------------------------------------------------
        // returns the attributes value
        return xnode.node().attribute(Attribute.c_str());
    }

    double
    XMLFileHandler::GetDoubleAttribute(const std::string& XPath, const std::string& Attribute) const
    {
        // ---------------------------------------------------------------------------------------------
        return GetXmlAttributeValue(XPath, Attribute).as_double();
    }

    float
    XMLFileHandler::GetFloatAttribute(const std::string& XPath, const std::string& Attribute) const
    {
        // ---------------------------------------------------------------------------------------------
        return GetXmlAttributeValue(XPath, Attribute).as_float();
    }

    int
    XMLFileHandler::GetIntAttribute(const std::string& XPath, const std::string& Attribute) const
    {
        // ---------------------------------------------------------------------------------------------
        return GetXmlAttributeValue(XPath, Attribute).as_int();
    }

    unsigned int
    XMLFileHandler::GetUIntAttribute(const std::string& XPath, const std::string& Attribute) const
    {
        // ---------------------------------------------------------------------------------------------
        return GetXmlAttributeValue(XPath, Attribute).as_uint();
    }

    std::string
    XMLFileHandler::GetStringAttribute(const std::string& XPath, const std::string& Attribute) const
    {
        // ---------------------------------------------------------------------------------------------
        const std::string l_string = GetXmlAttributeValue(XPath, Attribute).as_string("NO_READING_VALUE");
        // ---------------------------------------------------------------------------------------------
        // Checks if the value exists
        if (l_string == "NO_READING_VALUE")
        {
            vnsExceptionDataMacro(
                    "Error while reading the xml string attribute '"<<Attribute<<"' from the node '"<<XPath<<"' in the xml file "<<m_Filename<<"!")
        }
        // ---------------------------------------------------------------------------------------------
        return l_string;
    }

    bool
    XMLFileHandler::GetBoolAttribute(const std::string& XPath, const std::string& Attribute) const
    {
        // ---------------------------------------------------------------------------------------------
        return GetXmlAttributeValue(XPath, Attribute).as_bool();
    }

    unsigned int
    XMLFileHandler::GetNumberOfNodes(const std::string & path) const
    {
        // ---------------------------------------------------------------------------------------------
        // Get all the nodes from path
        pugi::xpath_node_set nodes = this->GetListOfNodes(path);
        return nodes.size();
    }

    XMLFileHandler::ListOfUIntegers
    XMLFileHandler::GetAttributeValuesAsIntFromNodes(const std::string & path, const std::string & attributeName) const
    {
        // ---------------------------------------------------------------------------------------------
        ListOfUIntegers attributes;

        // ---------------------------------------------------------------------------------------------
        // Get all the nodes from path
        pugi::xpath_node_set nodes = this->GetListOfNodes(path);
        const unsigned short nbNodes = nodes.size(); // one node for each attribute

        // ---------------------------------------------------------------------------------------------
        // For each node
        for (unsigned short n = 0; n < nbNodes; n++)
        {
            // ---------------------------------------------------------------------------------------------
            pugi::xml_node xnode = nodes[n].node();
            pugi::xml_attribute attribute = xnode.attribute(attributeName.c_str());
            attributes.push_back(attribute.as_int());
        }
        // ---------------------------------------------------------------------------------------------
        return attributes;
    }

    XMLFileHandler::ListOfDoubles
    XMLFileHandler::GetDoubleListFromNodes(const std::string & path) const
    {
        // ---------------------------------------------------------------------------------------------
        ListOfDoubles valueList;
        // ---------------------------------------------------------------------------------------------
        // Get all the nodes from path
        pugi::xpath_node_set nodes = this->GetListOfNodes(path);
        const unsigned short nbNodes = nodes.size(); // one node for each attribute

        // ---------------------------------------------------------------------------------------------
        // For each node
        for (unsigned short n = 0; n < nbNodes; n++)
        {
            valueList.push_back(nodes[n].node().text().as_double());
        }
        // ---------------------------------------------------------------------------------------------
        return valueList;
    }

    XMLFileHandler::ListOfStrings
    XMLFileHandler::GetStringListFromNodes(const std::string & path) const
    {
        // ---------------------------------------------------------------------------------------------
        ListOfStrings valueList;
        // ---------------------------------------------------------------------------------------------
        // Get all the nodes from path
        pugi::xpath_node_set nodes = this->GetListOfNodes(path);
        const unsigned short nbNodes = nodes.size(); // one node for each attribute

        // ---------------------------------------------------------------------------------------------
        // For each node
        for (unsigned short n = 0; n < nbNodes; n++)
        {
            valueList.push_back(nodes[n].node().text().as_string());
        }
        return valueList;
    }

    XMLFileHandler::ListOfStrings
    XMLFileHandler::GetAttributeValuesAsStringFromNodes(const std::string & path, const std::string & attributeName) const
    {
        // ---------------------------------------------------------------------------------------------
        ListOfStrings attributes;

        // ---------------------------------------------------------------------------------------------
        // Get all the nodes from path
        pugi::xpath_node_set nodes = this->GetListOfNodes(path);
        const unsigned short nbNodes = nodes.size(); // one node for each attribute

        // ---------------------------------------------------------------------------------------------
        // For each node
        for (unsigned short n = 0; n < nbNodes; n++)
        {
            pugi::xml_node xnode = nodes[n].node();
            pugi::xml_attribute attribute = xnode.attribute(attributeName.c_str());
            attributes.push_back(attribute.as_string());
        }
        // ---------------------------------------------------------------------------------------------
        return attributes;
    }

    XMLFileHandler::ListOfDoubles
    XMLFileHandler::GetDoubleListValue(const std::string& XPath) const
    {
        // ---------------------------------------------------------------------------------------------
        // Selects the node with the root path
        pugi::xpath_node xnode = GetSingleNode(XPath);

        // Check if the node exist or if it's no empty
        if (xnode.node().empty() == true)
        {
            // ---------------------------------------------------------------------------------------------
            // returns the default list
            vnsLogDebugMacro(
                    "Attention: XMLFileHandler::GetDoubleListValue. Empty value for the XPath <"<<XPath<<"> for the Xml filename <"<<m_Filename<<">!")
            return ListOfDoubles();
        }
        // ---------------------------------------------------------------------------------------------
        std::istringstream iss(xnode.node().child_value());
        ListOfDoubles result;
        // ---------------------------------------------------------------------------------------------
        // Loops on the stringstream
        while (iss.good())
        {
            double tmp;
            iss >> tmp;
            result.push_back(tmp);
        }
        // ---------------------------------------------------------------------------------------------
        // returns the formated list of values
        return result;
    }

    XMLFileHandler::ListOfIntegers
    XMLFileHandler::GetIntListValue(const std::string& XPath) const
    {
        // ---------------------------------------------------------------------------------------------
        // Selects the node with the root path
        pugi::xpath_node xnode = GetSingleNode(XPath);
        // ---------------------------------------------------------------------------------------------
        // Check if the node exist or if it's no empty
        if (xnode.node().empty() == true)
        {
            vnsLogDebugMacro(
                    "Attention: XMLFileHandler::GetIntListValue. Empty value for the XPath <"<<XPath<<"> for the Xml filename <"<<m_Filename<<">!")
            return ListOfIntegers();
        }
        std::istringstream iss(xnode.node().child_value());
        ListOfIntegers result;
        // ---------------------------------------------------------------------------------------------
        // Loops on the stringstream
        while (iss.good())
        {
            int tmp;
            iss >> tmp;
            result.push_back(tmp);
        }
        // ---------------------------------------------------------------------------------------------
        // Returns the result to the dedicated type
        return result;
    }
    XMLFileHandler::ListOfUIntegers
    XMLFileHandler::GetUIntListValue(const std::string& XPath) const
    {
        // ---------------------------------------------------------------------------------------------
        // Selects the node with the root path
        pugi::xpath_node xnode = GetSingleNode(XPath);
        // ---------------------------------------------------------------------------------------------
        // Check if the node exist or if it's no empty
        if (xnode.node().empty() == true)
        {
            vnsLogDebugMacro(
                    "Attention: XMLFileHandler::GetIntListValue. Empty value for the XPath <"<<XPath<<"> for the Xml filename <"<<m_Filename<<">!")
            return ListOfUIntegers();
        }
        std::istringstream iss(xnode.node().child_value());
        ListOfUIntegers result;
        // ---------------------------------------------------------------------------------------------
        // Loops on the stringstream
        while (iss.good())
        {
            unsigned int tmp;
            iss >> tmp;
            result.push_back(tmp);
        }
        // ---------------------------------------------------------------------------------------------
        // Returns the result to the dedicated type
        return result;
    }

    XMLFileHandler::ListOfStrings
    XMLFileHandler::GetStringListValue(const std::string& XPath) const
    {
        // ---------------------------------------------------------------------------------------------
        // Selects the node with the root path
        pugi::xpath_node xnode = GetSingleNode(XPath);

        // Check if the node exist or if it's no empty
        if (xnode.node().empty() == true)
        {
            vnsLogDebugMacro(
                    "Attention: XMLFileHandler::GetStringListValue. Empty value for the XPath <"<<XPath<<"> for the Xml filename <"<<m_Filename<<">!")
            return ListOfStrings();
        }

        std::istringstream iss(xnode.node().child_value());
        ListOfStrings result;
        while (iss.good())
        {
            std::string tmp;
            iss >> tmp;
            result.push_back(tmp);
        }
        // ---------------------------------------------------------------------------------------------
        // Returns the result to the dedicated type
        return result;
    }
    void
    XMLFileHandler::SetFilenameListValue(const ListOfFilenames& ListOfValues, const std::string& RootXPath, const std::string& SubNodeName)
    {
        ListOfStrings lListOfStrings;
        const unsigned int nbListInput(ListOfValues.size());
        for (unsigned int i = 0; i < nbListInput; i++)
        {
            lListOfStrings.push_back(ListOfValues[i].c_str());
        }
        this->SetStringListValue(lListOfStrings, RootXPath, SubNodeName);
    }

    void
    XMLFileHandler::SetStringListValue(const ListOfStrings& ListOfValues, const std::string& RootXPath, const std::string& SubNodeName)
    {
        // Ex:
        //  RootXPath   = //Image_Information/List_of_Bands
        //  SubNodeName = Band
        // RootXPath must be exist

        // ---------------------------------------------------------------------------------------------
        // Selects the node with the root path
        pugi::xpath_node xnode = GetSingleNode(RootXPath);

        const unsigned int l_count = ListOfValues.size();
        pugi::xml_node main_node = xnode.node();
        for (unsigned int i = 0; i < l_count; i++)
        {
            // ---------------------------------------------------------------------------------------------
            // Set the value to the dedicated node
            pugi::xml_node tool = main_node.append_child(SubNodeName.c_str());
            tool.append_child(pugi::node_pcdata).text().set(ListOfValues.at(i).c_str());
        }

    }

    XMLFileHandler::ListOfStrings
    XMLFileHandler::GetStringListValueSortedByAttribut(const std::string& RootXPath, const std::string& SubNodeNameBeforeAttribut,
            const std::string & SubNodeNameAfterAttribut, const std::string & RootNodeAttributName, const std::string & RootSubNodeAttributName,
            unsigned int OffsetAttributValue) const
    {
        // Read the count value
        // Read the number of Node
        // Check number of nodes and count

        // ex :     RootXPath                   = "/Earth_Explorer_Header/Variable_Header/Specific_Product_Header/DBL_Organization/List_of_Packaged_DBL_Files"
        //          SubNodeNameBeforeAttribut   = "/Packaged_DBL_File"
        //          SubNodeNameAfterAttribut    = "/Relative_File_Path"

        // Read filesname in the order of the attribute 'sn'

        const unsigned int l_NumberOfNodes = this->GetNumberOfNodes(RootXPath + SubNodeNameBeforeAttribut);
        // If the RootNodeAttributName attribut doesn't exist in the XML file, so set the RootNodeAttributName = ""
        unsigned int l_Count = l_NumberOfNodes;
        if (RootNodeAttributName.empty() == false)
        {
            l_Count = this->GetIntAttribute(RootXPath, RootNodeAttributName);
            if (l_Count != l_NumberOfNodes)
            {
                vnsExceptionDataMacro(
                        "In the XML file <"<<this->GetFilename()<<">, in the <"<<RootXPath<<"> node, the 'count' value ("<<l_Count<<") is different than the number of sub-nodes ("<<l_NumberOfNodes<<") !!")
            }
        }
        ListOfStrings l_ListOfStrings;
        // Start to "1"
        for (unsigned int i = OffsetAttributValue; i < (l_Count + OffsetAttributValue); i++)
        {
            std::string xpath = RootXPath + SubNodeNameBeforeAttribut + "[@" + RootSubNodeAttributName + "='" + Utilities::AsString(i)
                    + "']" + SubNodeNameAfterAttribut;
            l_ListOfStrings.push_back(this->GetStringValue(xpath));
        }
        return l_ListOfStrings;
    }

    XMLFileHandler::ListOfStrings
    XMLFileHandler::GetStringListFilenamesSortedByAttribut(const std::string & RootXPath, const std::string & SubNodeNameBeforeAttribut,
            const std::string & SubNodeNameAfterAttribut, const bool check_file_exists, const std::string & RootNodeAttributName,
            const std::string & RootSubNodeAttributName, const unsigned int OffsetAttributValue) const
    {
        // --------------------------------------------------------------
        // Call the GetStringListValueSortedByAttribut method
        const ListOfStrings l_ListOfStrings = this->GetStringListValueSortedByAttribut(RootXPath, SubNodeNameBeforeAttribut,
                SubNodeNameAfterAttribut, RootNodeAttributName, RootSubNodeAttributName, OffsetAttributValue);

        // --------------------------------------------------------------
        // Check that each retrieved value is a file
        if (check_file_exists == true)
        {
            const unsigned int l_NbStrings = l_ListOfStrings.size();
            for (unsigned int i = 0; i < l_NbStrings; i++)
            {
                if (itksys::SystemTools::FileExists(l_ListOfStrings[i].c_str()) == false)
                {
                    vnsExceptionDataMacro(
                            "In the XML file <"<<this->GetFilename()<<">, in the <"<<RootXPath<<">' node, the file <"<<l_ListOfStrings[i]<<"> doesn't exist !!")
                }
            }
        }
        // --------------------------------------------------------------
        // Return the list of filenames
        return l_ListOfStrings;
    }

    void
    XMLFileHandler::SetStringListValueSortedByAttribut(const ListOfStrings& ListOfValues, const std::string& RootXPath,
            const std::string& SubNodeName, const std::string & RootNodeAttributName, const std::string & RootSubNodeAttributName,
            const unsigned int OffsetAttributValue, MapStringType p_attributs)
    {
        // Ex:
        //  RootXPath   = //Image_Information/List_of_Bands
        //  SubNodeName = Band
        // RootXPath must be exist

        // ---------------------------------------------------------------------------------------------
        // Selects the node with the root path
        pugi::xpath_node xnode = GetSingleNode(RootXPath);

        const unsigned int l_count = ListOfValues.size();
        pugi::xml_node main_node = xnode.node();
        pugi::xml_attribute attrib = main_node.attribute(RootNodeAttributName.c_str());
        // ---------------------------------------------------------------------------------------------
        // If the attribute variable not empty, set the dedicated value
        if (attrib.empty() != true)
        {
            attrib.set_value(l_count);
        }
        else
        {
            main_node.append_attribute(RootNodeAttributName.c_str()) = l_count;
        }
        for (unsigned int i = 0; i < l_count; i++)
        {
            // ---------------------------------------------------------------------------------------------
            // Set the value to the dedicated node
            pugi::xml_node tool = main_node.append_child(SubNodeName.c_str());
            //pugi::xml_attribute attrib = tool.attribute(RootSubNodeAttributName.c_str());
            tool.append_attribute(RootSubNodeAttributName.c_str()) = i + OffsetAttributValue;
            // ---------------------------------------------------------------------------------------------
            // Loops on creates additionnals attributs
            if (p_attributs.empty() == false)
            {
                for (MapStringType::const_iterator it = p_attributs.begin(); it != p_attributs.end(); ++it)
                {
                    tool.append_attribute(it->first.c_str()) = it->second.c_str();
                }
            }
            tool.append_child(pugi::node_pcdata).text().set(ListOfValues.at(i).c_str());
        }

    }

    XMLFileHandler::ListOfDoubles
    XMLFileHandler::GetDoubleArrayValue(const std::string& XPath) const
    {
        // ---------------------------------------------------------------------------------------------
        // Selects the node with the root path
        pugi::xpath_node xnode = GetSingleNode(XPath);

        ListOfDoubles result;
        // ---------------------------------------------------------------------------------------------
        // Loops on all child node of the main node 'xnode'
        pugi::xml_node tool = xnode.node().first_child();
        while(tool != 0)
        {
            // ---------------------------------------------------------------------------------------------
            // Get the value of the child into a string value
            // Checks the size -1 value of the string
            // If the last character is empty, resize the string
            std::string value = tool.child_value();
            const unsigned int l_size_moins_1 = value.size() - 1;
            if (value[l_size_moins_1] == ' ')
            {
                value.resize(l_size_moins_1);
            }
            // ---------------------------------------------------------------------------------------------
            // Converts the string to the dedicated type of the output result value
            std::istringstream iss(value);
            while (iss.good())
            {
                double tmp;
                iss >> tmp;
                result.push_back(tmp);
            }
            // Increment tool variable
            tool = tool.next_sibling();
        }
        // ---------------------------------------------------------------------------------------------
        // Returns the result to the dedicated type
        return result;
    }

    XMLFileHandler::ListOfFloats
    XMLFileHandler::GetFloatArrayValue(const std::string& XPath) const
    {
        // ---------------------------------------------------------------------------------------------
        // Selects the node with the root path
        pugi::xpath_node xnode = GetSingleNode(XPath);

        ListOfFloats result;
        // ---------------------------------------------------------------------------------------------
        // Loops on all child node of the main node 'xnode'
        pugi::xml_node tool = xnode.node().first_child();
        while(tool != 0)
        {
            // ---------------------------------------------------------------------------------------------
            // Get the value of the child into a string value
            // Checks the size -1 value of the string
            // If the last character is empty, resize the string
            std::string value = tool.child_value();
            const unsigned int l_size_moins_1 = value.size() - 1;
            if (value[l_size_moins_1] == ' ')
            {
                value.resize(l_size_moins_1);
            }
            // ---------------------------------------------------------------------------------------------
            // Converts the string to the dedicated type of the output result value
            std::istringstream iss(value);
            while (iss.good())
            {
                float tmp;
                iss >> tmp;
                result.push_back(tmp);
            }
            // Increment tool variable
            tool = tool.next_sibling();
        }
        // ---------------------------------------------------------------------------------------------
        // Returns the result to the dedicated type
        return result;
    }

    XMLFileHandler::ListOfIntegers
    XMLFileHandler::GetIntArrayValue(const std::string& XPath) const
    {
        // ---------------------------------------------------------------------------------------------
        // Selects the node with the root path
        pugi::xpath_node xnode = GetSingleNode(XPath);

        ListOfIntegers result;
        // ---------------------------------------------------------------------------------------------
        // Loops on all child node of the main node 'xnode'
        pugi::xml_node tool = xnode.node().first_child();
        while(tool != 0)
        {
            // ---------------------------------------------------------------------------------------------
            // Get the value of the child into a string value
            // Checks the size -1 value of the string
            // If the last character is empty, resize the string
            std::string value = tool.child_value();
            const unsigned int l_size_moins_1 = value.size() - 1;
            if (value[l_size_moins_1] == ' ')
            {
                value.resize(l_size_moins_1);
            }
            // ---------------------------------------------------------------------------------------------
            // Converts the string to the dedicated type of the output result value
            std::istringstream iss(value);
            while (iss.good())
            {
                int tmp;
                iss >> tmp;
                result.push_back(tmp);
            }
            // Increment tool variable
            tool = tool.next_sibling();
        }
        // ---------------------------------------------------------------------------------------------
        // Returns the result to the dedicated type
        return result;
    }

    XMLFileHandler::ListOfStrings
    XMLFileHandler::GetStringArrayValue(const std::string& XPath) const
    {
        // ---------------------------------------------------------------------------------------------
        // Selects the node with the root path
        pugi::xpath_node xnode = GetSingleNode(XPath);

        ListOfStrings result;
        // ---------------------------------------------------------------------------------------------
        // Loops on all child node of the main node 'xnode'
        pugi::xml_node tool = xnode.node().first_child();
        while(tool != 0)
        {
            // ---------------------------------------------------------------------------------------------
            // Get the value of the child into a string value
            // Checks the size -1 value of the string
            // If the last character is empty, resize the string
            std::string value = tool.child_value();
            const unsigned int l_size_moins_1 = value.size() - 1;
            if (value[l_size_moins_1] == ' ')
            {
                value.resize(l_size_moins_1);
            }
            // ---------------------------------------------------------------------------------------------
            // Converts the string to the dedicated type of the output result value
            std::istringstream iss(value);
            while (iss.good())
            {
                std::string tmp;
                iss >> tmp;
                result.push_back(tmp);
            }
            // Increment tool variable
            tool = tool.next_sibling();
        }
        // ---------------------------------------------------------------------------------------------
        // Returns the result to the dedicated type
        return result;
    }

    void
    XMLFileHandler::SetDoubleValue(const std::string& XPath, double avalue)
    {
        // ---------------------------------------------------------------------------------------------
        // Selects the node with the root path
        pugi::xpath_node xnode = GetSingleNode(XPath);

        // ---------------------------------------------------------------------------------------------
        // Get the xml_text of the dedicated node (text() method)
        pugi::xml_text text = xnode.node().text();
        // ---------------------------------------------------------------------------------------------
        // If the xml_text variable is valid, set the dedicated value
        if (text)
        {
            text.set(avalue);
        }
        else
        {
            // ---------------------------------------------------------------------------------------------
            // Set the value to the dedicated node
            xnode.node().append_child(pugi::node_pcdata).text().set(avalue);
        }

        return;
    }

    void
    XMLFileHandler::SetFloatValue(const std::string& XPath, float avalue)
    {
        // ---------------------------------------------------------------------------------------------
        // Selects the node with the root path
        pugi::xpath_node xnode = GetSingleNode(XPath);

        // ---------------------------------------------------------------------------------------------
        // Get the xml_text of the dedicated node (text() method)
        pugi::xml_text text = xnode.node().text();
        // ---------------------------------------------------------------------------------------------
        // If the xml_text variable is valid, set the dedicated value
        if (text)
        {
            text.set(avalue);
        }
        else
        {
            // ---------------------------------------------------------------------------------------------
            // Set the value to the dedicated node
            xnode.node().append_child(pugi::node_pcdata).text().set(avalue);
        }

        return;
    }

    void
    XMLFileHandler::SetIntValue(const std::string& XPath, int avalue)
    {
        // ---------------------------------------------------------------------------------------------
        // Selects the node with the root path
        pugi::xpath_node xnode = GetSingleNode(XPath);

        // ---------------------------------------------------------------------------------------------
        // Get the xml_text of the dedicated node (text() method)
        pugi::xml_text text = xnode.node().text();
        // ---------------------------------------------------------------------------------------------
        // If the xml_text variable is valid, set the dedicated value
        if (text)
        {
            text.set(avalue);
        }
        else
        {
            // ---------------------------------------------------------------------------------------------
            // Set the value to the dedicated node
            xnode.node().append_child(pugi::node_pcdata).text().set(avalue);
        }

        return;
    }

    void
    XMLFileHandler::SetUShortValue(const std::string& XPath, unsigned short avalue)
    {
        this->SetUIntValue(XPath, static_cast<unsigned short>(avalue));
    }

    void
    XMLFileHandler::SetUIntValue(const std::string& XPath, unsigned int avalue)
    {
        // ---------------------------------------------------------------------------------------------
        // Selects the node with the root path
        pugi::xpath_node xnode = GetSingleNode(XPath);

        // ---------------------------------------------------------------------------------------------
        // Get the xml_text of the dedicated node (text() method)
        pugi::xml_text text = xnode.node().text();
        // ---------------------------------------------------------------------------------------------
        // If the xml_text variable is valid, set the dedicated value
        if (text)
        {
            text.set(avalue);
        }
        else
        {
            // ---------------------------------------------------------------------------------------------
            // Set the value to the dedicated node
            xnode.node().append_child(pugi::node_pcdata).text().set(avalue);
        }
        return;
    }

    void
    XMLFileHandler::SetIntArrayValue(const std::string& XPath, const ListOfIntegers & array)
    {
        const std::string l_String = Utilities::GetStringFromSdtListOfValues(array);
        this->SetStringValue(XPath, l_String);
    }
    void
    XMLFileHandler::SetUIntArrayValue(const std::string& XPath, const ListOfUIntegers & array)
    {
        const std::string l_String = Utilities::GetStringFromSdtListOfValues(array);
        this->SetStringValue(XPath, l_String);
    }
    void
    XMLFileHandler::SetFloatArrayValue(const std::string& XPath, const ListOfFloats & array)
    {
        const std::string l_String = Utilities::GetStringFromSdtListOfValues(array);
        this->SetStringValue(XPath, l_String);
    }
    void
    XMLFileHandler::SetDoubleArrayValue(const std::string& XPath, const ListOfDoubles & array)
    {
        const std::string l_String = Utilities::GetStringFromSdtListOfValues(array);
        this->SetStringValue(XPath, l_String);
    }

    void
    XMLFileHandler::SetStringValue(const std::string& XPath, std::string avalue)
    {
        // --------------------------------------
        const std::string lXPath(XPath);
        const std::string lavalue(avalue);

        // --------------------------------------
        vnsLogSuperDebugMacro("Set the string value '"<<avalue<<"' to the xpath '"<<XPath<<"' in the xml file "<<m_Filename<<"...")
        // --------------------------------------
        // ---------------------------------------------------------------------------------------------
        // Selects the node with the root path
        pugi::xpath_node xnode = GetSingleNode(lXPath);

        // --------------------------------------
        // If valid node
        if (xnode)
        {
            // ---------------------------------------------------------------------------------------------
            // Get the xml_text of the dedicated node (text() method)
            pugi::xml_text text = xnode.node().text();
            // --------------------------------------
            // Write the value in existing node
            // ---------------------------------------------------------------------------------------------
            // If the xml_text variable is valid, set the dedicated value
            if (text)
            {
                text.set(lavalue.c_str());
            }
            // --------------------------------------
            // Or create the node with the correct value
            else
            {
                // ---------------------------------------------------------------------------------------------
                // Set the value to the dedicated node
                xnode.node().append_child(pugi::node_pcdata).text().set(lavalue.c_str());
            }
        }
        else
        {
            vnsExceptionDataMacro("Error while writing the xml node '"<<lXPath<<"' in the xml file "<<m_Filename<<"! ")
        }
    }

    void
    XMLFileHandler::SetBoolValue(const std::string& XPath, bool avalue)
    {
        // ---------------------------------------------------------------------------------------------
        // Selects the node with the root path
        pugi::xpath_node xnode = GetSingleNode(XPath);

        // ---------------------------------------------------------------------------------------------
        // Get the xml_text of the dedicated node (text() method)
        pugi::xml_text text = xnode.node().text();
        // ---------------------------------------------------------------------------------------------
        // If the xml_text variable is valid, set the dedicated value
        if (text)
        {
            text.set(avalue);
        }
        else
        {
            // ---------------------------------------------------------------------------------------------
            // Set the value to the dedicated node
            xnode.node().append_child(pugi::node_pcdata).text().set(avalue);
        }
        return;
    }

    void
    XMLFileHandler::SetDoubleAttribute(const std::string& XPath, const std::string& Attribute, double avalue)
    {
        // ---------------------------------------------------------------------------------------------
        // Selects the node with the root path
        pugi::xpath_node xnode = GetSingleNode(XPath);

        // ---------------------------------------------------------------------------------------------
        // Get the xml_attribute of the dedicated node (attribute() method)
        pugi::xml_attribute attrib = xnode.node().attribute(Attribute.c_str());
        // ---------------------------------------------------------------------------------------------
        // If the attribute variable not empty, set the dedicated value
        if (attrib.empty() != true)
        {
            attrib.set_value(avalue);
        }
        else
        {
            // ---------------------------------------------------------------------------------------------
            // else, adds the attribute to the node with the dedicated value
            xnode.node().append_attribute(Attribute.c_str()) = avalue;
        }
    }

    void
    XMLFileHandler::SetFloatAttribute(const std::string& XPath, const std::string& Attribute, float avalue)
    {
        // ---------------------------------------------------------------------------------------------
        // Selects the node with the root path
        pugi::xpath_node xnode = GetSingleNode(XPath);

        // ---------------------------------------------------------------------------------------------
        // Get the xml_attribute of the dedicated node (attribute() method)
        pugi::xml_attribute attrib = xnode.node().attribute(Attribute.c_str());
        // ---------------------------------------------------------------------------------------------
        // If the attribute variable not empty, set the dedicated value
        if (attrib.empty() != true)
        {
            attrib.set_value(avalue);
        }
        else
        {
            // ---------------------------------------------------------------------------------------------
            // else, adds the attribute to the node with the dedicated value
            xnode.node().append_attribute(Attribute.c_str()) = avalue;
        }
    }

    void
    XMLFileHandler::SetIntAttribute(const std::string& XPath, const std::string& Attribute, int avalue)
    {
        // ---------------------------------------------------------------------------------------------
        // Selects the node with the root path
        pugi::xpath_node xnode = GetSingleNode(XPath);

        // ---------------------------------------------------------------------------------------------
        // Get the xml_attribute of the dedicated node (attribute() method)
        pugi::xml_attribute attrib = xnode.node().attribute(Attribute.c_str());
        // ---------------------------------------------------------------------------------------------
        // If the attribute variable not empty, set the dedicated value
        if (attrib.empty() != true)
        {
            attrib.set_value(avalue);
        }
        else
        {
            // ---------------------------------------------------------------------------------------------
            // else, adds the attribute to the node with the dedicated value
            xnode.node().append_attribute(Attribute.c_str()) = avalue;
        }
    }

    void
    XMLFileHandler::SetUIntAttribute(const std::string& XPath, const std::string& Attribute, unsigned int avalue)
    {
        // ---------------------------------------------------------------------------------------------
        // Selects the node with the root path
        pugi::xpath_node xnode = GetSingleNode(XPath);

        // ---------------------------------------------------------------------------------------------
        // Get the xml_attribute of the dedicated node (attribute() method)
        pugi::xml_attribute attrib = xnode.node().attribute(Attribute.c_str());
        // ---------------------------------------------------------------------------------------------
        // If the attribute variable not empty, set the dedicated value
        if (attrib.empty() != true)
        {
            attrib.set_value(avalue);
        }
        else
        {
            // ---------------------------------------------------------------------------------------------
            // else, adds the attribute to the node with the dedicated value
            xnode.node().append_attribute(Attribute.c_str()) = avalue;
        }
    }

    void
    XMLFileHandler::SetStringAttribute(const std::string& XPath, const std::string& Attribute, std::string avalue)
    {
        // ---------------------------------------------------------------------------------------------
        // Selects the node with the root path
        pugi::xpath_node xnode = GetSingleNode(XPath);

        // ---------------------------------------------------------------------------------------------
        // Get the xml_attribute of the dedicated node (attribute() method)
        pugi::xml_attribute attrib = xnode.node().attribute(Attribute.c_str());
        // ---------------------------------------------------------------------------------------------
        // If the attribute variable not empty, set the dedicated value
        if (attrib.empty() != true)
        {
            attrib.set_value(avalue.c_str());
        }
        else
        {
            // ---------------------------------------------------------------------------------------------
            // else, adds the attribute to the node with the dedicated value
            xnode.node().append_attribute(Attribute.c_str()) = avalue.c_str();
        }
    }

    void
    XMLFileHandler::SetBoolAttribute(const std::string& XPath, const std::string& Attribute, bool avalue)
    {
        // ---------------------------------------------------------------------------------------------
        // Selects the node with the root path
        pugi::xpath_node xnode = GetSingleNode(XPath);

        // ---------------------------------------------------------------------------------------------
        // Get the xml_attribute of the dedicated node (attribute() method)
        pugi::xml_attribute attrib = xnode.node().attribute(Attribute.c_str());
        // ---------------------------------------------------------------------------------------------
        // If the attribute variable not empty, set the dedicated value
        if (attrib.empty() != true)
        {
            attrib.set_value(avalue);
        }
        else
        {
            // ---------------------------------------------------------------------------------------------
            // else, adds the attribute to the node with the dedicated value
            xnode.node().append_attribute(Attribute.c_str()) = avalue;
        }
    }

    void
    XMLFileHandler::InsertChild(const std::string& XPath, const std::string& Name)
    {
        // ---------------------------------------------------------------------------------------------
        // Selects the node with the root path
        pugi::xpath_node xnode = GetSingleNode(XPath);

        // ---------------------------------------------------------------------------------------------
        // Set the value to the dedicated node
        xnode.node().append_child(Name.c_str());

    }

    void
    XMLFileHandler::InsertSibling(const std::string& XPath, const std::string& Name)
    {
        // ---------------------------------------------------------------------------------------------
        // Selects the node with the root path
        pugi::xpath_node xnode = GetSingleNode(XPath);

        // ---------------------------------------------------------------------------------------------
        // Set the value to the dedicated node
        xnode.node().parent().append_child(Name.c_str());
    }

    void
    XMLFileHandler::RemoveNode(Pointer Doc, const std::string& XPath)
    {
        pugi::xpath_node xnode = GetSingleNode(Doc, XPath);
        try
        {
            xnode.node().parent().remove_child(xnode.node());
        }
        catch (const pugi::xpath_exception& e)
        {
            vnsStaticExceptionDataMacro("Error while deleting the xml node '"<<XPath<<"'! Details: "<<e.what())
            return;
        }
    }

    void
    XMLFileHandler::ReplaceNode(Pointer DocSource, const std::string& XPathSource, Pointer DocDestination,
            const std::string& XPathDestination)
    {

        // ---------------------------------------------------------------------------------------------
        // Selects the node with the root path
        pugi::xpath_node xnodesource = GetSingleNode(DocSource, XPathSource);
        pugi::xpath_node xnodedest = GetSingleNode(DocDestination, XPathDestination);

        // Insert new node after
        // <List_of_Band_Central_Wavelength />

        xnodedest.node().parent().insert_copy_after(xnodesource.node(), xnodedest.node());
        // <List_of_Band_Central_Wavelength />
        // <List_of_Band_Central_Wavelength count="12">
        //    <Band_Central_Wavelength sk="B01" unit="nanometer">415</Band_Central_Wavelength>
        //      <Band_Central_Wavelength sk="B02" unit="nanometer">443</Band_Central_Wavelength>
        // </List_of_Band_Central_Wavelength>

        //Delete previous template node
        //        xnodesource = DocSource.select_single_node(XPathSource.c_str());

        //RemoveChild()XPathSource
        //        xnodedest = DocDestination.select_single_node(XPathDestination.c_str());
        //xnodedest.node().parent().remove_child(xnodedest.node().child_value());
        try
        {
            xnodedest.node().parent().remove_child(xnodedest.node());
        }
        catch (const pugi::xpath_exception& e)
        {
            vnsStaticExceptionDataMacro(
                    "Error while delete the xml first node '"<<XPathDestination<<"' after copy sibling node in the xml file! Details: "<<e.what())
            return;
        }
        // <List_of_Band_Central_Wavelength count="12">
        //    <Band_Central_Wavelength sk="B01" unit="nanometer">415</Band_Central_Wavelength>
        //      <Band_Central_Wavelength sk="B02" unit="nanometer">443</Band_Central_Wavelength>
        // </List_of_Band_Central_Wavelength>

    }

    void
    XMLFileHandler::CopyToChild(const std::string& XPathSource, const std::string& XPathDestination)
    {
        // ---------------------------------------------------------------------------------------------
        pugi::xpath_node xnodesource = GetSingleNode(XPathSource);

        // ---------------------------------------------------------------------------------------------
        pugi::xpath_node xnodedest = GetSingleNode(XPathDestination);

        // ---------------------------------------------------------------------------------------------
        // Copy the node
        xnodedest.node().append_copy(xnodesource.node());
    }
    void
    XMLFileHandler::CopyToChild(Pointer DocSource, const std::string& XPathSource, Pointer DocDestination,
            const std::string& XPathDestination)
    {
        pugi::xpath_node xnodesource = GetSingleNode(DocSource, XPathSource);
        pugi::xpath_node xnodedest = GetSingleNode(DocDestination, XPathDestination);

        xnodedest.node().append_copy(xnodesource.node());
    }

    void
    XMLFileHandler::CopiesToChild(Pointer DocSource, const std::string& XPathSource, Pointer DocDestination,
            const std::string& XPathDestination)
    {
        // ---------------------------------------------------------------------------------------------
        pugi::xpath_node_set xnodessource = DocSource->GetListOfNodes(XPathSource);
        const unsigned int nb_xnodessource = xnodessource.size();

        // ---------------------------------------------------------------------------------------------
        pugi::xpath_node xnodedest = GetSingleNode(DocDestination, XPathDestination);

        // ---------------------------------------------------------------------------------------------
        // Loops and copy nodes
        for (unsigned int i = 0; i < nb_xnodessource; i++)
        {
            xnodedest.node().append_copy(xnodessource[i].node());
        }
    }
    void
    XMLFileHandler::CopyToSibling(const std::string& XPathSource, const std::string& XPathDestination)
    {
        pugi::xpath_node xnodesource = GetSingleNode(XPathSource);
        pugi::xpath_node xnodedest = GetSingleNode(XPathDestination);

        xnodedest.node().parent().insert_copy_after(xnodesource.node(), xnodedest.node());
    }
    void
    XMLFileHandler::CopyToSibling(Pointer DocSource, const std::string& XPathSource, Pointer DocDestination,
            const std::string& XPathDestination)
    {
        pugi::xpath_node xnodesource = GetSingleNode(DocSource, XPathSource);
        pugi::xpath_node xnodedest = GetSingleNode(DocDestination, XPathDestination);

        xnodedest.node().parent().insert_copy_after(xnodesource.node(), xnodedest.node());
    }

    void
    XMLFileHandler::RemoveChild(const std::string& XPath, const std::string& Name)
    {
        pugi::xpath_node xnode = GetSingleNode(XPath);

        xnode.node().remove_child(Name.c_str());
    }

    void
    XMLFileHandler::RemoveAttribute(const std::string& XPath, const std::string& Name)
    {
        pugi::xpath_node xnode = GetSingleNode(XPath);

        xnode.node().remove_attribute(Name.c_str());
    }
    void
    XMLFileHandler::RenameNode(const std::string &XPath, const std::string & newName)
    {
        pugi::xpath_node xnode = GetSingleNode(XPath);

        pugi::xml_node node = xnode.node();
        const bool l_Result = node.set_name(newName.c_str());
        if (l_Result == false)
        {
            vnsExceptionDataMacro("Error while renaming the Node name from the XPath '"<<XPath<<"' to the new name '"<<newName<<"'!")
            return;

        }
    }
    /** Print contents of a XMLFileHandler */
    void
    XMLFileHandler::PrintSelf(std::ostream &os, itk::Indent indent) const
    {
        // ---------------------------------------------------------------------------------------------
        Superclass::PrintSelf(os, indent);

        // ---------------------------------------------------------------------------------------------
        os << indent << "Filename : " << Utilities::GetFilenameWithoutNumbersOfPrefixSubDirectory(m_Filename, 1) << std::endl;
    }

}

