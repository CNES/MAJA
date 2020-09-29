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
 * SimpleLUTXMLFileHandler.h
 *
 *  Created on: 26 oct. 2017
 *      Author: besquis
 */
#ifndef CODE_CORE_DATA_IO_VNSSIMPLELUTXMLCONTAINER_H_
#define CODE_CORE_DATA_IO_VNSSIMPLELUTXMLCONTAINER_H_

#include "vnsUtilities.h"


namespace vns
{
class SimpleLutXMLContainer
{
public:

	typedef Utilities::ListOfDoubles ListOfDoubles;
	typedef Utilities::ListOfStrings ListOfStrings;

	SimpleLutXMLContainer();
	virtual ~SimpleLutXMLContainer();

	vns::SimpleLutXMLContainer& operator=(const vns::SimpleLutXMLContainer&);

	//-----------------------------------------------------------------------------
	//-----------------------------------------------------------------------------
	//-----------------------------------------------------------------------------
	//-----------------------------------------------------------------------------
	// Look Up Table Header
	//-----------------------------------------------------------------------------
	// Read the EarthExplorer field LUT Index 'Altitude_Indexes'
	const ListOfDoubles&
	GetAltitude_Indexes() const
	{
		return m_AtlIndx;
	}
	void SetAltitude_Indexes(const ListOfDoubles& list);

	// Read the EarthExplorer field LUT Index 'AOT_Indexes'
	const ListOfDoubles&
	GetAOT_Indexes() const
	{
		return m_AOTIndx;
	}
	void SetAOT_Indexes(const ListOfDoubles& list);

	// Read the EarthExplorer field LUT Index 'TOA_Reflectance_Indexes'
	const ListOfDoubles&
	GetTOA_Reflectance_Indexes() const
	{
		return m_TOAIdx;
	}
	void SetTOA_Reflectance_Indexes(const ListOfDoubles& list);

	// Read the EarthExplorer field LUT Index 'Reflectance_Ratio_Indexes' for WATV
	const ListOfDoubles&
	GetReflectance_Ratio_Indexes() const
	{
		return m_ReflRatioIdx;
	}
	void SetReflectance_Ratio_Indexes(const ListOfDoubles& list);

	// Read the EarthExplorer field LUT Index 'View_Zenith_Angle_Indexes'
	const ListOfDoubles&
	GetView_Zenith_Angle_Indexes() const
	{
		return m_VieZenithAngleIndx;
	}
	void SetView_Zenith_Angle_Indexes(const ListOfDoubles& list);
	// Read the EarthExplorer field LUT Index 'Solar_Zenith_Angle_Indexes'
	const ListOfDoubles&
	GetSolar_Zenith_Angle_Indexes() const
	{
		return m_SolZenithIndx;
	}
	void SetSolar_Zenith_Angle_Indexes(const ListOfDoubles& list);

	// Read the EarthExplorer field LUT Index 'Relative_Azimuth_Angle_Indexes'
	const ListOfDoubles&
	GetRelative_Azimuth_Angle_Indexes() const
	{
		return m_RelAziIndx;
	}
	void SetRelative_Azimuth_Angle_Indexes(const ListOfDoubles& list);

	// Read the EarthExplorer field LUT Index 'Zenithal_Angle_Indexes'
	const ListOfDoubles&
	GetZenithal_Angle_Indexes() const
	{
		return m_ZenithalIndx;
	}
	void SetZenithal_Angle_Indexes(const ListOfDoubles& list);

	const ListOfStrings&
    GetListOfPackaged_DBL_Files() const
    {
    	return m_ListOfFiles;
    }
	void SetListOfPackaged_DBL_Files(const ListOfStrings& list);


protected:

	ListOfDoubles m_ZenithalIndx;
	ListOfDoubles m_RelAziIndx;
	ListOfDoubles m_SolZenithIndx;
	ListOfDoubles m_VieZenithAngleIndx;
	ListOfDoubles m_ReflRatioIdx;
	ListOfDoubles m_TOAIdx;
	ListOfDoubles m_AOTIndx;
	ListOfDoubles m_AtlIndx;
	ListOfStrings m_ListOfFiles;
};

}
#endif /* CODE_CORE_DATA_IO_VNSSIMPLELUTXMLCONTAINER_H_ */
