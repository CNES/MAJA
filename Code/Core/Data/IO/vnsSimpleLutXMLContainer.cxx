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

#include <vnsSimpleLutXMLContainer.h>

namespace vns
{
SimpleLutXMLContainer::SimpleLutXMLContainer()
{

}

SimpleLutXMLContainer::~SimpleLutXMLContainer() {

}

vns::SimpleLutXMLContainer& vns::SimpleLutXMLContainer::operator=(const vns::SimpleLutXMLContainer& other)
{
	if (this != &other)
	{
		m_ZenithalIndx = other.m_ZenithalIndx;
		m_RelAziIndx = other.m_RelAziIndx;
		m_SolZenithIndx = other.m_SolZenithIndx;
		m_VieZenithAngleIndx = other.m_VieZenithAngleIndx;
		m_ReflRatioIdx = other.m_ReflRatioIdx;
		m_TOAIdx = other.m_TOAIdx;
		m_AOTIndx = other.m_AOTIndx;
		m_AtlIndx = other.m_AtlIndx;
		m_ListOfFiles = other.m_ListOfFiles;
	}
	return *this;

}

void SimpleLutXMLContainer::SetAltitude_Indexes(const ListOfDoubles& list)
{
	m_AtlIndx = list;
}
void SimpleLutXMLContainer::SetAOT_Indexes(const ListOfDoubles& list)
{
	m_AOTIndx = list;
}
void SimpleLutXMLContainer::SetTOA_Reflectance_Indexes(const ListOfDoubles& list)
{
	m_TOAIdx = list;
}
void SimpleLutXMLContainer::SetReflectance_Ratio_Indexes(const ListOfDoubles& list)
{
	m_ReflRatioIdx = list;
}
void SimpleLutXMLContainer::SetView_Zenith_Angle_Indexes(const ListOfDoubles& list)
{
	m_VieZenithAngleIndx = list;
}
void SimpleLutXMLContainer::SetSolar_Zenith_Angle_Indexes(const ListOfDoubles& list)
{
	m_SolZenithIndx = list;
}
void SimpleLutXMLContainer::SetRelative_Azimuth_Angle_Indexes(const ListOfDoubles& list)
{
	m_RelAziIndx = list;
}
void SimpleLutXMLContainer::SetZenithal_Angle_Indexes(const ListOfDoubles& list)
{
	m_ZenithalIndx = list;
}
void SimpleLutXMLContainer::SetListOfPackaged_DBL_Files(const ListOfStrings& list)
{
	m_ListOfFiles = list;
}

}
