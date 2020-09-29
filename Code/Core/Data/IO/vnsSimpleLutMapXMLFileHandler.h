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
#ifndef CODE_CORE_DATA_IO_VNSSIMPLELUTMAPXMLFILEHANDLER_H_
#define CODE_CORE_DATA_IO_VNSSIMPLELUTMAPXMLFILEHANDLER_H_

#include "vnsUtilities.h"
#include "vnsSimpleLutXMLContainer.h"

namespace vns
{
class SimpleLutMapXMLFileHandler : public itk::Object
{
public:
	typedef SimpleLutMapXMLFileHandler Self;
	typedef itk::Object Superclass;
	typedef itk::SmartPointer<Self> Pointer;
	typedef itk::SmartPointer<const Self> ConstPointer;

	/** Run-time type information (and related methods). */
	itkTypeMacro( SimpleLutMapXMLFileHandler, itk::Object )

	/** New macro for creation of through a Smart Pointer */
	itkNewMacro( Self )

	typedef Utilities::ListOfDoubles ListOfDoubles;
	typedef Utilities::ListOfStrings ListOfStrings;
	typedef std::map<int,SimpleLutXMLContainer> LutMapXml;

	void LoadFile(const std::string& filename);

	const LutMapXml& GetLutMapContainer() const
	{
		return m_MapContainer;
	}

protected:
	SimpleLutMapXMLFileHandler();
	virtual ~SimpleLutMapXMLFileHandler();

	LutMapXml m_MapContainer;
};

}
#endif /* CODE_CORE_DATA_IO_VNSSIMPLESMACXMLFILEHANDLER_H_ */
