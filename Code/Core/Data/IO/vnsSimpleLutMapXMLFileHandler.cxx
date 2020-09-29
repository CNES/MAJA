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
 * SimpleSMACXMLFileHandler.cpp
 *
 *  Created on: 26 oct. 2017
 *      Author: besquis
 */

#include <vnsSimpleLutMapXMLFileHandler.h>

#include "otb_tinyxml.h"

namespace vns
{
SimpleLutMapXMLFileHandler::SimpleLutMapXMLFileHandler()
{

}

SimpleLutMapXMLFileHandler::~SimpleLutMapXMLFileHandler() {

}

void SimpleLutMapXMLFileHandler::LoadFile(const std::string& filename) {

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
	n_DOCMain = handle.FirstChild("LUTMap").Element();

	if(!n_DOCMain)
	{
		return;
	}
	for( TiXmlElement* n_DOC = n_DOCMain->FirstChildElement("LUT"); n_DOC != ITK_NULLPTR;
			n_DOC = n_DOC->NextSiblingElement() )
	{
		SimpleLutXMLContainer l_Container;
		int l_lutKey;
		n_DOC->QueryIntAttribute("index",&l_lutKey);

		TiXmlElement *n_IdxNode = n_DOC->FirstChildElement("Indexes");
		if(!n_IdxNode)
		{
			vnsExceptionDataMacro("No Indexes in the lut xml file");
		}

		TiXmlElement *n_Node = n_IdxNode->FirstChildElement("Altitude_Indexes");
		if(n_Node)
		{
			l_Container.SetAltitude_Indexes( Utilities::SplitStringAsDouble(n_Node->GetText()));
		}
		n_Node = n_IdxNode->FirstChildElement("AOT_Indexes");
		if(n_Node)
		{
			l_Container.SetAOT_Indexes(Utilities::SplitStringAsDouble(n_Node->GetText()));
		}
		n_Node = n_IdxNode->FirstChildElement("TOA_Reflectance_Indexes");
		if(n_Node)
		{
			l_Container.SetTOA_Reflectance_Indexes( Utilities::SplitStringAsDouble(n_Node->GetText()));
		}
		n_Node = n_IdxNode->FirstChildElement("Reflectance_Ratio_Indexes");
		if(n_Node)
		{
			l_Container.SetReflectance_Ratio_Indexes( Utilities::SplitStringAsDouble(n_Node->GetText()));
		}
		n_Node = n_IdxNode->FirstChildElement("View_Zenith_Angle_Indexes");
		if(n_Node)
		{
			l_Container.SetView_Zenith_Angle_Indexes( Utilities::SplitStringAsDouble(n_Node->GetText()));
		}
		n_Node = n_IdxNode->FirstChildElement("Solar_Zenith_Angle_Indexes");
		if(n_Node)
		{
			l_Container.SetSolar_Zenith_Angle_Indexes( Utilities::SplitStringAsDouble(n_Node->GetText()));
		}
		n_Node = n_IdxNode->FirstChildElement("Relative_Azimuth_Angle_Indexes");
		if(n_Node)
		{
			l_Container.SetRelative_Azimuth_Angle_Indexes( Utilities::SplitStringAsDouble(n_Node->GetText()) );
		}
		n_Node = n_IdxNode->FirstChildElement("Zenithal_Angle_Indexes");
		if(n_Node)
		{
			l_Container.SetZenithal_Angle_Indexes( Utilities::SplitStringAsDouble(n_Node->GetText()) );
		}

		n_Node = n_DOC->FirstChildElement("List_Of_Files");
		if(n_Node)
		{
			ListOfStrings l_ListOfFiles;
			for( TiXmlElement* n_Parameter = n_Node->FirstChildElement("Relative_File_Path"); n_Parameter != ITK_NULLPTR;
					n_Parameter = n_Parameter->NextSiblingElement() )
			{
				l_ListOfFiles.push_back(n_Parameter->GetText());
			}
			const std::string l_RootBasePath = itksys::SystemTools::GetFilenamePath(filename);
			Utilities::RelativeToAbsoluteFilename(l_RootBasePath, l_ListOfFiles);
			Utilities::CheckingExistenceFilenames(l_ListOfFiles);
			l_Container.SetListOfPackaged_DBL_Files(l_ListOfFiles);
		}

		m_MapContainer[l_lutKey] = l_Container;
	}
	fclose(fp);

}

}
