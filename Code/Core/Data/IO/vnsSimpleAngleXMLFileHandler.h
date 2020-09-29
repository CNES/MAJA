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
#ifndef CODE_CORE_DATA_IO_VNSSIMPLEANGLEXMLFILEHANDLER_H_
#define CODE_CORE_DATA_IO_VNSSIMPLEANGLEXMLFILEHANDLER_H_

#include "vnsUtilities.h"

namespace vns
{
class SimpleAngleXMLFileHandler : public itk::Object
{
public:
	typedef SimpleAngleXMLFileHandler Self;
	typedef itk::Object Superclass;
	typedef itk::SmartPointer<Self> Pointer;
	typedef itk::SmartPointer<const Self> ConstPointer;

	/** Run-time type information (and related methods). */
	itkTypeMacro( SimpleAngleXMLFileHandler, itk::Object )

	/** New macro for creation of through a Smart Pointer */
	itkNewMacro( Self )

	typedef Utilities::ListOfDoubles ListOfDoubles;
	typedef Utilities::ListOfStrings ListOfStrings;

	void LoadFile(const std::string& filename);

	double GetAngleSpacingCol() const {
		return m_angleSpacingCol;
	}
	double GetAngleSpacingRow() const {
		return m_angleSpacingRow;
	}
	const ListOfStrings& GetAngleZenithList() const {
		return m_angleZenithList;
	}

	const ListOfStrings& GetAngleAzimuthList() const {
		return m_angleAzimuthList;
	}

protected:
	SimpleAngleXMLFileHandler();
	virtual ~SimpleAngleXMLFileHandler();

	double m_angleSpacingCol;
	double m_angleSpacingRow;
	ListOfStrings m_angleZenithList;
	ListOfStrings m_angleAzimuthList;
};

}
#endif /* CODE_CORE_DATA_IO_VNSSIMPLESMACXMLFILEHANDLER_H_ */
