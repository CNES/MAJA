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
 * SimpleSMACXMLFileHandler.h
 *
 *  Created on: 26 oct. 2017
 *      Author: besquis
 */
#ifndef CODE_CORE_DATA_IO_VNSSIMPLESMACXMLFILEHANDLER_H_
#define CODE_CORE_DATA_IO_VNSSIMPLESMACXMLFILEHANDLER_H_

#include "vnsUtilities.h"

namespace vns
{
class SimpleSMACXMLFileHandler : public itk::Object
{
public:
	typedef SimpleSMACXMLFileHandler Self;
	typedef itk::Object Superclass;
	typedef itk::SmartPointer<Self> Pointer;
	typedef itk::SmartPointer<const Self> ConstPointer;

	/** Run-time type information (and related methods). */
	itkTypeMacro( SimpleSMACXMLFileHandler, itk::Object )

	/** New macro for creation of through a Smart Pointer */
	itkNewMacro( Self )

	typedef Utilities::ListOfStrings ListOfStrings;
	typedef Utilities::ListOfDoubles ListOfDoubles;
	typedef Utilities::ListOfFilenames ListOfFilenames;

	void LoadFile(const std::string& filename);

	// Read the EarthExplorer field SMAC 'SMAC_NumberOfBands'
	unsigned int
	GetSMAC_NumberOfBands() const
	{
		return m_NumberOfBands;
	}
	// Read the EarthExplorer field SMAC 'SMAC_Coefficients'
	ListOfDoubles
	GetSMAC_Coefficients() const
	{
		return m_SMACCoeffs;
	}

protected:
	SimpleSMACXMLFileHandler();
	virtual ~SimpleSMACXMLFileHandler();
	unsigned int m_NumberOfBands;
	ListOfDoubles m_SMACCoeffs;
};

}
#endif /* CODE_CORE_DATA_IO_VNSSIMPLESMACXMLFILEHANDLER_H_ */
