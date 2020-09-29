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
 * VERSION : 1-0-0 : <TypeFT> : <NumFT> : 15 juil. 2010 : Creation
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id$
 *                                                                                                          *
 ************************************************************************************************************/

#include "otbWrapperApplication.h"
#include "otbWrapperApplicationFactory.h"
#include "vnsOzoneValueExtractor.h"
#include "itkObjectFactoryBase.h"
#include "vnsGRIBImageIOFactory.h"
#include <string>

namespace vns
{
namespace Wrapper
{

using namespace otb::Wrapper;

class OzoneExtractor : public Application
{
public:
	/** Standard class typedefs. */
	typedef OzoneExtractor             Self;
	typedef otb::Wrapper::Application     Superclass;
	typedef itk::SmartPointer<Self>       Pointer;
	typedef itk::SmartPointer<const Self> ConstPointer;

	/** Standard macro */
	itkNewMacro(Self);

	itkTypeMacro(OzoneExtractor, otb::Wrapper::Application);

	/** Some convenient typedefs. */
	/** Ozone typedef */
    typedef OzoneValueExtractor::PointType OzoneValueExtractorPointType;
    typedef OzoneValueExtractor::OzoneValueType OzoneValueExtractorValueType;


private:
	void DoInit()
	{
		SetName("OzoneExtractor");
		SetDescription("OzoneExtractor algo.");
		Loggers::GetInstance()->Initialize(GetName());
		// Documentation
		SetDocLongDescription("This application extract the ozone value from a meteo file");
		SetDocLimitations("None");
		SetDocAuthors("MAJA-Team");
		SetDocSeeAlso("MAJA Doc");

		AddDocTag("OzoneExtractor");
		AddParameter(ParameterType_String, "im", "meteo file");
		AddParameter(ParameterType_Float,  "lon",   "Longitude value");
		AddParameter(ParameterType_Float,  "lat",   "Latitude value");


		AddParameter(ParameterType_Float, "ozone",  "Ozone amount");
		SetParameterDescription("ozone", "Ozone amount");
		SetParameterRole("ozone", Role_Output);

		AddRAMParameter("ram");
		SetDefaultParameterInt("ram",2048);

	}

	void DoUpdateParameters()
	{

	}


	void DoExecute()
	{
		// Init filters
		//*************************************************************************************************************
		// Read data from "EXO_METDTA"
		//*************************************************************************************************************
		OzoneValueExtractorPointType point;
		// Compute Lat,Long coordinate of the central point of the image
		// Read the UsefulImageGeoCoverageCenterCorner data from the HEADER_IMAGE_INFORMATIONS registered file
		point[0] = GetParameterFloat("lon");
		point[1] = GetParameterFloat("lat");
		const std::string l_file = GetParameterString("im");
		vnsLogDebugMacro("FileName: "<<l_file);

		itk::ObjectFactoryBase::RegisterFactory( vns::GRIBImageIOFactory::New() );
		OzoneValueExtractorValueType l_OzoneAmountValue = OzoneValueExtractor::GetOzoneValue(l_file, point);

		SetParameterFloat("ozone",l_OzoneAmountValue);
	}


	/** Filters declaration */


};

}
}

OTB_APPLICATION_EXPORT(vns::Wrapper::OzoneExtractor)
