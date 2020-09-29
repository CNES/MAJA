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
/*_____________________________________________________________________________

 Program:
 ___  ___     ____  ____  ____
 / __)(__ \   (_  _)(  _ \( ___)
 \__ \ / _/    _)(_  )___/ )__)
 (___/(____)  (____)(__)  (__)


 Group:           CS Systemes d'Information  (France)
 GMV  (Spain)
 GAEL Systems  (France)

 SW Component:    IDP-SC
 Author:          CS Systemes d'Information

 Language:        C++
 _____________________________________________________________________________

 HISTORY

 VERSION : 01.00.00 : <AR/CR> : <JIRA #> : <Date> : Creation of the file.

 END-HISTORY
 _____________________________________________________________________________

 $Id$
 _____________________________________________________________________________*/

#include <cstdlib>
#include <ctime>

#include "vnsXMLFileHandler.h"

int
vnsXmlFileHandlerTemplateTest(int /*argc*/, char * argv[])
{
    vns::XMLFileHandler::Pointer handler = vns::XMLFileHandler::New();
    handler->LoadFile(argv[1], pugi::parse_full);

    vns::XMLFileHandler::Pointer handler2 = vns::XMLFileHandler::New();
    handler2->LoadFile(argv[2], pugi::parse_full);

    std::string result2 = handler->GetStringValue("/Earth_Explorer_Header/Fixed_Header/Mission");

    std::cout << "result= " << result2 << std::endl;

    handler->SetStringValue("//File_Class", "OPER");

    std::string attribute = handler->GetStringAttribute("//Image_Information/List_of_Bands", "count");//[[0-9]]$");
    std::cout << "attribute '" << attribute << "'" << std::endl;
    //    std::vector<std::string> listvalue = handler->GetStringListValue("//Image_Information/List_of_Bands/Band");//[[0-9]]$");
    std::vector<std::string> listvalue = handler->GetStringArrayValue("//Image_Information/List_of_Bands");
    for (std::vector<std::string>::const_iterator it = listvalue.begin(); it != listvalue.end(); it++)
    {
        std::cout << "Band: '" << *it << "'" << std::endl;

    }

    handler->SetStringAttribute("/Earth_Explorer_Header", "xsi:schemaLocation", "../../../../Schemas/Venus/toto.xsd");
//    ="http://eop-cfi.esa.int/CFI ../../../../Schemas/Venus/VE_VSC_PDTIMG_ImageProduct.xsd" xsi:type="PDTIMG_Header_Type"
    pugi::xml_document m_Doc;

    pugi::xpath_node xnode;
    pugi::xml_parse_result result = m_Doc.load_file(argv[1], pugi::parse_full);
    xnode = m_Doc.select_single_node("//Image_Information/List_of_Bands");

    std::cout << "root count: '" << xnode.node().attribute("count").as_string() << "'" << std::endl;
    pugi::xml_node tool;
    pugi::xml_node tool2;
    tool = xnode.node();
    std::cout << "Tool: '" << tool.child_value() << "'" << std::endl;
    std::cout << "Tool: '" << tool.text() << "'" << std::endl;
    std::string toto("6");
    tool2 = tool.find_child_by_attribute("sn", toto.c_str() /*"10"*/);
    std::cout << "Tool2: '" << tool2.child_value() << "'" << std::endl;
    std::cout << "Tool2: '" << tool2.text() << "'" << std::endl;
    std::cout << "Tool2 name: '" << tool2.name() << "'" << std::endl;
    //tool2.name();

    std::vector<std::string> list = handler->GetStringListValueSortedByAttribut("//Image_Information/List_of_Bands", "/Band", "", "count", "sn", 1 /* offset */);
    std::cout << "nb value: '" << list.size() << std::endl;
    for (unsigned int i = 0; i < list.size(); i++)
    {
        std::cout << "value: '" << list.at(i) << std::endl;
    }


    handler->SetStringListValueSortedByAttribut(list,"//Image_Information/TestField", "Band", "count", "sn", 1 /*offset*/);
    handler2->ReplaceNode(handler,"//List_of_Band_Central_Wavelength");

    handler->SaveFile(argv[3]);
    return EXIT_SUCCESS;
}
