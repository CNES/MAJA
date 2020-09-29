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
 * VERSION : 1.0.0 : FA : LAIG-FA-MAC-148399-CS : 11 octobre 2016 : Audit code - Correction qualite         *
 * VERSION : 5-1-0 : FA : LAIG-FA-MAC-144674-CS : 9 mai 2016 : Mise en forme                                *
 * VERSION : 5-0-0 : FA : LAIG-FA-MAC-143764-CS : 19 avril 2016 : Mise en forme du code (indentation, etc.) *
 * VERSION : 4-8-0 : FA : LAIG-FA-MAC-142832-CS : 10 mars 2016 : S2 Correction pour la lecture du           *
 *             "Mean_Viewing_Incidence_Angle_List" trié par le "band id" (GPP et PDGS) de règles de qualite *
 * VERSION : 4-7-0 : FA : LAIG-FA-MAC-1477-CNES : 7 decembre 2015 : Correction partielle des messages       *
 *                                                                    d'erreur non significatif dans le log *
 * VERSION : 4-0-0 : FA : LAIG-FA-MAC-117040-CS : 13 mars 2014 : Modifications mineures                     *
 * VERSION : 3-1-0 : DM : LAIG-DM-MAC-614-CNES : 10 octobre 2013 : Creation du plugin LANDSAT 5 et 7        *
 * 						creation d'une methode qui puisse retourner une string vide							*
 * VERSION : 3-0-0 : DM : LAIG-DM-MAC-239-CNES : 10 mai 2013 : Creation pour la prise en compte de la DM du  *
 *                                                            MACCS generique (PM LAIG-NT-MAC-0239-CNES)    *
 * VERSION : 1-0-0 : <TypeFT> : <NumFT> : 15 fevrier 2010 : Creation
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id$
 *                                                                                                          *
 ************************************************************************************************************/

#ifndef vns_XMLFILEHANDLER_INCLUDED
#define vns_XMLFILEHANDLER_INCLUDED

#include "itkObject.h"
#include "itkObjectFactory.h"
#include "pugixml.hpp"
#include <iostream>
#include <string>
#include <vector>

#include "vnsUtilities.h"

namespace vns
{

    class XMLFileHandler : public itk::Object
    {
    public:
        typedef XMLFileHandler Self;
        typedef itk::Object Superclass;
        typedef itk::SmartPointer<Self> Pointer;
        typedef itk::SmartPointer<const Self> ConstPointer;

        /** Run-time type information (and related methods). */
        itkTypeMacro( XMLFileHandler, itk::Object )

        /** New macro for creation of through a Smart Pointer */
        itkNewMacro( Self )

        typedef class BoundingBox
        {
        public:
            unsigned int xmin;
            unsigned int ymin;
            unsigned int xmax;
            unsigned int ymax;
        }
        BoundingBoxType;

        typedef class PointXY
        {
        public:
            unsigned int x;
            unsigned int y;
        } PointXYType;

        typedef class PointSignedXY
        {
        public:
            int x;
            int y;
        } PointSignedXYType;

        typedef Utilities::ListOfStrings ListOfStrings;
        typedef Utilities::ListOfFilenames ListOfFilenames;
        typedef Utilities::ListOfIntegers ListOfIntegers;
        typedef Utilities::ListOfFloats ListOfFloats;
        typedef Utilities::ListOfDoubles ListOfDoubles;
        typedef Utilities::ListOfUIntegers ListOfUIntegers;

        typedef std::map<std::string, std::string> MapStringType;

#define vnsSetGetStringValueWithXpathMacro(name, xpath) \
            /* Set ##name method */ \
            virtual void Set##name (const std::string & _arg) \
            { \
                this->SetStringValue(xpath, _arg); \
            } \
            /* Get ##name method */ \
            virtual std::string Get##name () const \
            { \
                return this->GetStringValue(xpath); \
            }
#define vnsSetGetStringAttributeWithXpathMacro(name, xpath, attr) \
            /* Set ##name method */ \
            virtual void Set##name (const std::string & _arg) \
            { \
                this->SetStringAttribute(xpath, attr, _arg ); \
            } \
            /* Get ##name method */ \
            virtual std::string Get##name () const \
            { \
                return this->GetStringAttribute(xpath, attr); \
            }
#define vnsSetGetUIntValueWithXpathMacro(name, xpath) \
            /* Set ##name method */ \
            virtual void Set##name (const unsigned int & _arg) \
            { \
                this->SetUIntValue(xpath,  _arg ); \
            } \
            /* Get ##name method */ \
            virtual unsigned int Get##name () const \
            { \
                return this->GetUIntValue(xpath); \
            }
#define vnsSetGetDoubleValueWithXpathMacro(name, xpath) \
            /* Set ##name method */ \
            virtual void Set##name (const double & _arg) \
            { \
                this->SetDoubleValue(xpath, _arg); \
            } \
            /* Get ##name method */ \
            virtual double Get##name () const \
            { \
                return this->GetDoubleValue(xpath); \
            }

        /** Get the xml filename **/
        itkGetConstReferenceMacro(Filename, std::string)

        virtual void
        LoadFile(const std::string& filename, unsigned int options = pugi::parse_full /*parse_default*/);

        static bool
        CanLoadFile(const std::string& filename, unsigned int options = pugi::parse_full /*parse_default*/);

        virtual void
        LoadFileAndCheck(const std::string& filename, const std::string& schemaLocationDirectoy,
                unsigned int options = pugi::parse_full /*parse_default*/);
        /*
         * Save in a new xml filename
         */
        void
        SaveFile(const std::string& filename, std::string indent = "    ", unsigned int flags = pugi::format_default,
                pugi::xml_encoding encoding = pugi::encoding_auto);

        void
        SaveFileAndCheck(const std::string& filename, const std::string& schemaLocationDirectoy, std::string indent = "    ",
                unsigned int flags = pugi::format_default, pugi::xml_encoding encoding = pugi::encoding_auto);
        /*
         * Update the input xml filename
         */
        void
        SaveFile();

        /*
         * Set/Get the xsi:schemaLocation attribut
         */
        std::string
        GetSchemaLocation() const
        {
            const std::string lXsdFilename = GetSingleNode("//@xsi:schemaLocation").attribute().as_string();
            if (lXsdFilename.empty() == true)
            {
                vnsExceptionDataMacro("Error while reading the schemaLocation attribute in the XmlFilename '"<< m_Filename<<"' !")
            }
            return lXsdFilename;
        }

        /*
         * Test node is valid
         */
        bool
        NodeIsValid(const std::string& RootXPath) const
        {
            bool l_IsValid(true);
            // Ex:
            //  RootXPath   = //Image_Information/List_of_Bands
            // ---------------------------------------------------------------------------------------------
            try
            {
                // ---------------------------------------------------------------------------------------------
                // Selects the node with the root path
                pugi::xpath_node xnode = m_Doc.select_single_node(RootXPath.c_str());
                if (xnode == 0)
                {
                    l_IsValid = false;
                }
            }
            // ---------------------------------------------------------------------------------------------
            // throws an exception id error
            catch (const pugi::xpath_exception& /*e*/)
            {
                l_IsValid = false;
            }
            catch (...)
            {
                l_IsValid = false;
            }
            return l_IsValid;
        }

        pugi::xpath_node
        GetSingleNode(const std::string& RootXPath) const;

        static pugi::xpath_node
        GetSingleNode(Pointer Doc, const std::string& RootXPath);
        /*
         * Get list of nodes
         */
        pugi::xpath_node_set
        GetListOfNodes(const std::string& RootXPath) const;

        /*
         * Accessors part return default constructed if doesn't exist
         */

        double
        GetDoubleValue(const std::string& XPath) const;

        float
        GetFloatValue(const std::string& XPath) const;

        int
        GetIntValue(const std::string& XPath) const;

        unsigned int
        GetUIntValue(const std::string& XPath) const;

        short
        GetShortValue(const std::string& XPath) const;

        unsigned short
        GetUShortValue(const std::string& XPath) const;

        unsigned char
        GetUCharValue(const std::string& XPath) const;

        std::string
        GetStringValue(const std::string& XPath) const;

        std::string
        GetStringValueOrEmpty(const std::string& XPath) const;

        bool
        GetBoolValue(const std::string& XPath) const;

        double
        GetDoubleAttribute(const std::string& XPath, const std::string& Attribute) const;

        float
        GetFloatAttribute(const std::string& XPath, const std::string& Attribute) const;

        int
        GetIntAttribute(const std::string& XPath, const std::string& Attribute) const;

        unsigned int
        GetUIntAttribute(const std::string& XPath, const std::string& Attribute) const;

        std::string
        GetStringAttribute(const std::string& XPath, const std::string& Attribute) const;

        bool
        GetBoolAttribute(const std::string& XPath, const std::string& Attribute) const;

        unsigned int
        GetNumberOfNodes(const std::string & path) const;

        ListOfUIntegers
        GetAttributeValuesAsIntFromNodes(const std::string & path, const std::string & attributeName) const;

        ListOfStrings
        GetAttributeValuesAsStringFromNodes(const std::string & path, const std::string & attributeName) const;

        ListOfDoubles
        GetDoubleListFromNodes(const std::string & path) const;
        ListOfStrings
        GetStringListFromNodes(const std::string & path) const;

        ListOfDoubles
        GetDoubleListValue(const std::string& XPath) const;

        ListOfIntegers
        GetIntListValue(const std::string& XPath) const;

        ListOfUIntegers
        GetUIntListValue(const std::string& XPath) const;

        ListOfStrings
        GetStringListValue(const std::string& XPath) const;

        void
        SetStringListValue(const ListOfStrings& ListOfValues, const std::string& RootXPath, const std::string& SubNodeName);

        void
        SetFilenameListValue(const ListOfFilenames& ListOfValues, const std::string& RootXPath, const std::string& SubNodeName);

        ListOfDoubles
        GetDoubleArrayValue(const std::string& XPath) const;

        ListOfFloats
        GetFloatArrayValue(const std::string& XPath) const;

        ListOfIntegers
        GetIntArrayValue(const std::string& XPath) const;

        ListOfStrings
        GetStringArrayValue(const std::string& XPath) const;

        /*
         * Setters, insertion if doesn't exist
         */
        void
        SetDoubleValue(const std::string& XPath, double avalue);

        void
        SetFloatValue(const std::string& XPath, float avalue);

        void
        SetIntValue(const std::string& XPath, int avalue);

        void
        SetUShortValue(const std::string& XPath, unsigned short avalue);

        void
        SetUIntValue(const std::string& XPath, unsigned int avalue);

        void
        SetIntArrayValue(const std::string& XPath, const ListOfIntegers & array);
        void
        SetUIntArrayValue(const std::string& XPath, const ListOfUIntegers & array);
        void
        SetFloatArrayValue(const std::string& XPath, const ListOfFloats & array);
        void
        SetDoubleArrayValue(const std::string& XPath, const ListOfDoubles & array);

        void
        SetStringValue(const std::string& XPath, std::string avalue);

        void
        SetBoolValue(const std::string& XPath, bool avalue);

        /*  void SetDoubleListValue(const std::string& XPath, ListOfDoubles list);

         void SetIntListValue(const std::string& XPath, ListOfDoubles list);

         void SetStringListValue(const std::string& XPath, ListOfDoubles list);

         void SetDoubleArrayValue(const std::string& XPath, ListOfDoubles array, unsigned int width, unsigned int height);

         void SetIntArrayValue(const std::string& XPath, ListOfDoubles array, unsigned int width, unsigned int height);

         void SetStringArrayValue(const std::string& XPath, ListOfDoubles array, unsigned int width, unsigned int height); */

        void
        SetDoubleAttribute(const std::string& XPath, const std::string& Attribute, double avalue);

        void
        SetFloatAttribute(const std::string& XPath, const std::string& Attribute, float avalue);

        void
        SetIntAttribute(const std::string& XPath, const std::string& Attribute, int avalue);

        void
        SetUIntAttribute(const std::string& XPath, const std::string& Attribute, unsigned int avalue);

        void
        SetStringAttribute(const std::string& XPath, const std::string& Attribute, std::string avalue);

        void
        SetBoolAttribute(const std::string& XPath, const std::string& Attribute, bool avalue);

        /*
         * Node editing
         */
        void
        InsertChild(const std::string& XPath, const std::string& Name);
        void
        InsertSibling(const std::string& XPath, const std::string& Name);
        void
        CopyToChild(const std::string& XPathSource, const std::string& XPathDestination);

        static void
        CopyToChild(Pointer DocSource, const std::string& XPathSource, Pointer DocDestination, const std::string& XPathDestination);
        void
        CopyToSibling(const std::string& XPathSource, const std::string& XPathDestination);

        static void
        CopiesToChild(Pointer DocSource, const std::string& XPathSource, Pointer DocDestination, const std::string& XPathDestination);

        static void
        CopyToSibling(Pointer DocSource, const std::string& XPathSource, Pointer DocDestination, const std::string& XPathDestination);

        static void
        RemoveNode(Pointer Doc, const std::string& XPath);
        void
        RemoveNode(const std::string& XPath)
        {
            RemoveNode(this, XPath);
        }

        static void
        ReplaceNode(Pointer DocSource, const std::string& XPathSource, Pointer DocDestination, const std::string& XPathDestination);

        void
        ReplaceNode(Pointer DocSource, const std::string& XPath)
        {
            ReplaceNode(DocSource, XPath, this, XPath);
        }

        void
        ReplaceNode(Pointer DocSource, const std::string& XPathSource, const std::string& XPathDestination)
        {
            ReplaceNode(DocSource, XPathSource, this, XPathDestination);
        }

        void
        RemoveChild(const std::string& XPath, const std::string& Name);
        void
        RemoveAttribute(const std::string& XPath, const std::string& Name);

        ListOfStrings
        GetStringListValueSortedByAttribut(const std::string & RootXPath, const std::string & SubNodeNameBeforeAttribut,
                const std::string & SubNodeNameAfterAttribut /* = "" */, const std::string & RootNodeAttributName /* "count" */,
                const std::string & RootSubNodeAttributName /* = "sn"*/, unsigned int OffsetAttributValue /* = 0/1 */) const;

        template<class T>
            std::vector<T>
            GetListValueSortedByAttribut(const std::string & RootXPath, const std::string & SubNodeNameBeforeAttribut,
                    const std::string & SubNodeNameAfterAttribut /* = "" */, const std::string & RootNodeAttributName /* "count" */,
                    const std::string & RootSubNodeAttributName /* = "sn"*/, unsigned int OffsetAttributValue /* = 0/1 */) const
            {
                // ---------------------------------------------------------------------------------------------
                const ListOfStrings l_ListOfStrings = this->GetStringListValueSortedByAttribut(RootXPath, SubNodeNameBeforeAttribut,
                        SubNodeNameAfterAttribut, RootNodeAttributName, RootSubNodeAttributName, OffsetAttributValue);
                // ---------------------------------------------------------------------------------------------
                const unsigned int l_NbListOfStrings = l_ListOfStrings.size();
                std::vector<T> l_List;
                // ---------------------------------------------------------------------------------------------
                // Start to "1"
                for (unsigned int i = 0; i < l_NbListOfStrings; i++)
                {
                    // ---------------------------------------------------------------------------------------------
                    std::istringstream iss(l_ListOfStrings[i]);
                    // ---------------------------------------------------------------------------------------------
                    if (iss.good())
                    {
                        // ---------------------------------------------------------------------------------------------
                        T tmp;
                        iss >> tmp;
                        l_List.push_back(tmp);
                    }
                }
                // ---------------------------------------------------------------------------------------------
                return l_List;
            }

        ListOfStrings
        GetStringListFilenamesSortedByAttribut(const std::string& RootXPath, const std::string& SubNodeNameBeforeAttribut,
                const std::string & SubNodeNameAfterAttribut /*= ""*/, const bool check_file_exists/*  = true */,
                const std::string & RootNodeAttributName/*= "count"*/, const std::string & RootSubNodeAttributName/*= "sn"*/,
                unsigned int OffsetAttributValue /* = 0/1 */) const;

        void
        SetStringListValueSortedByAttribut(const ListOfStrings& ListOfValues, const std::string& RootXPath, const std::string& SubNodeName,
                const std::string & RootNodeAttributName /*= "count"*/, const std::string & RootSubNodeAttributName /*= "sn"*/,
                const unsigned int OffsetAttributValue /* = 0/1 */, MapStringType p_attributs = MapStringType());

        template<class L>
            void
            SetListValueSortedByAttribut(const L& ListOfValues, const std::string& RootXPath, const std::string& SubNodeName,
                    std::string RootNodeAttributName, std::string RootSubNodeAttributName, const unsigned int OffsetAttributValue /* = 0/1 */
                    , MapStringType p_attributs)
            {
                // --------------------------------------------------------------
                // Convert a list of integers in a list of string
                // --------------------------------------------------------------
                const unsigned int l_NbListOfValues = ListOfValues.size();
                ListOfStrings l_ListOfStrings;
                // Start to "1"
                for (unsigned int i = 0; i < l_NbListOfValues; i++)
                {

                    std::stringstream oss;
                    oss << ListOfValues[i];
                    if (oss.good())
                    {
                        std::string tmp;
                        oss >> tmp;
                        l_ListOfStrings.push_back(tmp);
                    }
                }
                // Call the global method used to set value in the xml file
                this->SetStringListValueSortedByAttribut(l_ListOfStrings, RootXPath, SubNodeName, RootNodeAttributName,
                        RootSubNodeAttributName, OffsetAttributValue, p_attributs);
            }

        //Check a xml file
        void
        CheckXmlWithXsd(const std::string & lSchemaLocationDirectory);

        // Static function to check a xm file
        static void
        CheckXmlWithXsd(const std::string & lXmlFilename, const std::string & pSchemaLocationDirectory);

        //Static function apply style sheet
        static void
        TranslateXsl(const std::string & sourceFilename, const std::string & lXslStyleSheetFilename);

        void
        RenameNode(const std::string &XPath, const std::string & newName);

    protected:
        /** Constructeur */
        XMLFileHandler()
        {
        }

        /** Destructeur */
        virtual
        ~XMLFileHandler()
        {
        }

        /** Indentation used to format writing xml file */
        static const std::string m_XMLIndentation;

        pugi::xml_document m_Doc;

        /** Print contents of a XMLFileHandler */
        virtual void
        PrintSelf(std::ostream &os, itk::Indent indent) const;

        pugi::xml_text
        GetXmlTextValue(const std::string& XPath) const;
        pugi::xml_attribute
        GetXmlAttributeValue(const std::string& XPath, const std::string& Attribute) const;

    private:
        std::string m_Filename;
    };

}

#endif
