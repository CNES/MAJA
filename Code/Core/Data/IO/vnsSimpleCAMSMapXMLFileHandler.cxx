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
/*
 * SimpleCAMSMapXMLFileHandler.cpp
 *
 *  Created on: 26 oct. 2017
 *      Author: besquis
 */

#include <vnsSimpleCAMSMapXMLFileHandler.h>

#include "otb_tinyxml.h"

namespace vns
{
SimpleCAMSMapXMLFileHandler::SimpleCAMSMapXMLFileHandler()
{

}

SimpleCAMSMapXMLFileHandler::~SimpleCAMSMapXMLFileHandler() {

}

void SimpleCAMSMapXMLFileHandler::LoadFile(const std::string& filename) {

	if(filename.empty())
	{
		std::cerr <<"Input XML Filename is empty" << std::endl;
	}

	// Open the xml file
	TiXmlDocument doc;

	//Use itksys::SystemTools::FOpen() and close it below because
	//TiXmlDocument::TiXmlFileOpen( ) is not exposed from tinyXML library. Even
	//though its available in the TiXmlDocument::SaveFile().
	FILE* fp =  itksys::SystemTools::Fopen(filename.c_str(), "rb");

	if (!doc.LoadFile(fp , TIXML_ENCODING_UTF8))
	{
	    fclose(fp);
		vnsExceptionDataMacro("Impossible to open "<<filename);
	}

	TiXmlHandle handle(&doc);

	TiXmlElement *n_DOCMain;
	n_DOCMain = handle.FirstChild("CAMS").Element();

	if(!n_DOCMain)
	{
		return;
	}
	//Get the model node
	TiXmlElement * n_ModelMain = n_DOCMain->FirstChildElement("ModelList");

	for( TiXmlElement* n_MOD = n_ModelMain->FirstChildElement("Model"); n_MOD != ITK_NULLPTR;
			n_MOD = n_MOD->NextSiblingElement() )
	{
		ExtinctionCoeffList l_Container;
		std::string l_ModelName;
		n_MOD->QueryStringAttribute("Name",&l_ModelName);

		TiXmlElement *n_rhdepNode = n_MOD->FirstChildElement("RhDep");
		if(!n_rhdepNode)
		{
			vnsExceptionDataMacro("No rhDep for model "+ l_ModelName +" in cams xml file");
		}
		std::string l_rhDep;
		l_rhDep = n_rhdepNode->GetText();
		bool l_rhDepBool = Utilities::AsBool(l_rhDep);
		l_Container.RHDep = l_rhDepBool;

		TiXmlElement * n_ExtinctionListMain = n_MOD->FirstChildElement("Extinction_Coefs_List");

		for( TiXmlElement* n_Extinction = n_ExtinctionListMain->FirstChildElement("Extinction_Coef"); n_Extinction != ITK_NULLPTR;
				n_Extinction = n_Extinction->NextSiblingElement() )
		{
			ExtinctionCoeff l_Coeff;
			l_Coeff.Name = n_Extinction->FirstChildElement("Name")->GetText();
			l_Coeff.Description = n_Extinction->FirstChildElement("Description")->GetText();
			l_Coeff.Values = Utilities::SplitStringAsDouble(n_Extinction->FirstChildElement("Values")->GetText());
			l_Container.extinctionCoeffs.push_back(l_Coeff);
		}


		m_MapContainer[l_ModelName] = l_Container;
	}

	//Get the rhTab
	this->m_RhTab = Utilities::SplitStringAsUnsigned(n_DOCMain->FirstChildElement("RhTab")->GetText());
	fclose(fp);

}

}
