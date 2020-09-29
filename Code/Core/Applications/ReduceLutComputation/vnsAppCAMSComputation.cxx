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
 * VERSION : 1-0-0 : <TypeFT> : <NumFT> : 15 nov. 2017 : Creation
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id$
 *                                                                                                          *
 ************************************************************************************************************/

#include "otbWrapperApplication.h"
#include "otbWrapperApplicationFactory.h"
#include "vnsUtilities.h"
#include "vnsDate.h"
#include "vnsCAMSFileHandler.h"
#include "vnsSimpleCAMSMapXMLFileHandler.h"

namespace vns
{
namespace Wrapper
{

using namespace otb::Wrapper;

class CAMSComputation : public Application
{
public:
	/** Standard class typedefs. */
	typedef CAMSComputation        Self;
	typedef otb::Wrapper::Application     Superclass;
	typedef itk::SmartPointer<Self>       Pointer;
	typedef itk::SmartPointer<const Self> ConstPointer;

	/** Standard macro */
	itkNewMacro(Self);

	itkTypeMacro(CAMSComputation, otb::Wrapper::Application);

	/** Some convenient typedefs. */


private:
	void DoInit()
	{
		SetName("CAMSComputation");
		SetDescription("CAMSComputation algo.");
		Loggers::GetInstance()->Initialize(GetName());
		// Documentation
		SetDocLongDescription("This application computes the directional correction coeffs");
		SetDocLimitations("None");
		SetDocAuthors("MAJA-Team");
		SetDocSeeAlso("MAJA Doc");

		AddDocTag("Statistics");

		AddParameter(ParameterType_String,  "camsdescription",   "XML file for CAMS description");
		SetParameterDescription("camsdescription", "File describing CAMS parameters");
		AddParameter(ParameterType_StringList, "models","models used to compute");
		AddParameter(ParameterType_StringList, "rhsampling","Sampling of the relative humidity");
		AddParameter(ParameterType_Float,  "limaod","limaod");
		AddParameter(ParameterType_Float,  "lat","latitude of point to extract");
		AddParameter(ParameterType_Float,  "lon","longitude of point to extract");
		AddParameter(ParameterType_Float,  "datejulian","date to extract in julian day");
		MandatoryOff("datejulian");
		AddParameter(ParameterType_String,  "dateutc","date to extract in UTC format");
		MandatoryOff("dateutc");

		AddParameter(ParameterType_Group,  "before","CAMS data before the date");
		MandatoryOff("before");
		AddParameter(ParameterType_Float,  "before.datejulian","CAMS julian date of before");
		AddParameter(ParameterType_String,  "before.dateutc","CAMS julian date of before in UTC format");
		AddParameter(ParameterType_InputFilename,  "before.aotfile","AOT of before");
		AddParameter(ParameterType_InputFilename,  "before.mrfile","MR of before");
		AddParameter(ParameterType_InputFilename,  "before.rhfile","RH of before");
		AddParameter(ParameterType_StringList, "before.modellevels","model levels used to compute");
		AddParameter(ParameterType_Int,  "before.nbnointerpol","Number of values that can not be interpolated");
		MandatoryOff("before.nbnointerpol");

		AddParameter(ParameterType_Group,  "after","CAMS data after the date");
		MandatoryOff("after");
		AddParameter(ParameterType_Float,  "after.datejulian","CAMS julian date of after");
		AddParameter(ParameterType_String,  "after.dateutc","CAMS julian date of after in UTC format");
		AddParameter(ParameterType_InputFilename,  "after.aotfile","AOT of after");
		AddParameter(ParameterType_InputFilename,  "after.mrfile","MR of after");
		AddParameter(ParameterType_InputFilename,  "after.rhfile","RH of after");
		AddParameter(ParameterType_StringList, "after.modellevels","model levels used to compute");
		AddParameter(ParameterType_Int,  "after.nbnointerpol","Number of values that can not be interpolated");
		MandatoryOff("after.nbnointerpol");



		AddParameter(ParameterType_Float, "rh", "Output sampling used for RH");
		SetParameterRole("rh", Role_Output);
		AddParameter(ParameterType_String, "proportions", "Output proportions for models");
		SetParameterRole("proportions", Role_Output);
		AddParameter(ParameterType_OutputImage, "sumofaot", "Sum of AOT on models");


		AddRAMParameter("ram");
		SetDefaultParameterInt("ram",2048);

	}

	void DoUpdateParameters()
	{

	}


	void DoExecute()
	{
		//Get Params
		const Utilities::ListOfUIntegers & l_RHSamplings = Utilities::StringListAsUnsigned(
						this->GetParameterStringList("rhsampling"));
		const std::vector<std::string> & l_Models = this->GetParameterStringList("models");
		const double l_limAOT = this->GetParameterFloat("limaod");

		//Read the xml file
		SimpleCAMSMapXMLFileHandler::Pointer l_CAMSXMLHandler = SimpleCAMSMapXMLFileHandler::New();
		l_CAMSXMLHandler->LoadFile(this->GetParameterAsString("camsdescription"));

		//Instanciate cams handler
		CAMSFileHandler::Pointer l_camsHandler = CAMSFileHandler::New();
		l_camsHandler->SetListOfModels(l_Models);
		l_camsHandler->SetRHSampling(l_RHSamplings);
		l_camsHandler->SetLimAOD(l_limAOT);
		l_camsHandler->SetRHTab(l_CAMSXMLHandler->GetRhTab());

		//Convert xml extinction map to algorithm format
		CAMSFileHandler::ExtinctionMapType l_CamsExtAlgo;
		const SimpleCAMSMapXMLFileHandler::CAMSMapXml & l_CamsXMLExt = l_CAMSXMLHandler->GetExtinctionMap();
		for (SimpleCAMSMapXMLFileHandler::CAMSMapXml::const_iterator itCams = l_CamsXMLExt.begin();
				itCams != l_CamsXMLExt.end();itCams++)
		{
			for (std::vector<SimpleCAMSMapXMLFileHandler::ExtinctionCoeff>::const_iterator itCoeffs = itCams->second.extinctionCoeffs.begin();
					itCoeffs != itCams->second.extinctionCoeffs.end();itCoeffs++)
			{
				l_CamsExtAlgo[itCams->first + itCoeffs->Name] = vns::CAMSFileHandler::ExtinctionValue(itCoeffs->Description, itCoeffs->Values,
							                            		itCams->second.RHDep);
			}
		}

		l_camsHandler->SetExtinctionMap(l_CamsExtAlgo);


		//Get the point to extract
		CAMSFileHandler::CornerType l_corner;
		l_corner.Latitude = this->GetParameterFloat("lat");
		l_corner.Longitude = this->GetParameterFloat("lon");
		//Get the date to extract
		double l_julianDate = 0;
		if (this->HasValue("datejulian"))
		{
			l_julianDate = this->GetParameterFloat("datejulian");
		} else if (this->HasValue("dateutc"))
		{
			l_julianDate = Date::GetJulianDayAsDouble(Date::GetTmFromDateUTC(this->GetParameterString("dateutc")));
		} else {
			vnsExceptionDataMacro("No date given to extract");
		}


		//Get the CAMS file
		//Before
		if ( (this->HasValue("before.datejulian") || this->HasValue("before.dateutc") ) && this->HasValue("before.aotfile")
				&& this->HasValue("before.mrfile") && this->HasValue("before.rhfile"))
		{
			const std::string l_beforeAOT = this->GetParameterString("before.aotfile");
			const std::string l_beforeMR = this->GetParameterString("before.mrfile");
			const std::string l_beforeRH = this->GetParameterString("before.rhfile");
			const Utilities::ListOfDoubles & l_ModelLevels = Utilities::StringListAsDouble(
											this->GetParameterStringList("before.modellevels"));

			unsigned int l_nbInterpol = 4;
			if (HasValue("after.nbnointerpol"))
			{
				l_nbInterpol = this->GetParameterInt("after.nbnointerpol");
			}

			double l_beforeDate = 0;
			if (this->HasValue("before.datejulian"))
			{
				l_beforeDate = this->GetParameterFloat("before.datejulian");
			} else if (this->HasValue("before.dateutc"))
			{
				l_beforeDate = Date::GetJulianDayAsDouble(Date::GetTmFromDateUTC(this->GetParameterString("before.dateutc")));
			}

			if (l_beforeDate > l_julianDate)
			{
				vnsExceptionDataMacro("Before CAMS is not before the requested date");
			}

			//Set in the handler
			l_camsHandler->SetBeforeCAMS(l_beforeAOT,l_beforeMR,l_beforeRH,l_ModelLevels, l_nbInterpol, l_beforeDate);
		}
		//After
		if ( ( this->HasValue("after.datejulian") || this->HasValue("after.dateutc") ) && this->HasValue("after.aotfile")
				&& this->HasValue("after.mrfile") && this->HasValue("after.rhfile"))
		{
			const std::string l_afterAOT = this->GetParameterString("after.aotfile");
			const std::string l_afterMR = this->GetParameterString("after.mrfile");
			const std::string l_afterRH = this->GetParameterString("after.rhfile");
			const Utilities::ListOfDoubles & l_ModelLevels = Utilities::StringListAsDouble(
														this->GetParameterStringList("after.modellevels"));
			unsigned int l_nbInterpol = 4;
			if (HasValue("after.nbnointerpol"))
			{
				l_nbInterpol = this->GetParameterInt("after.nbnointerpol");
			}

			double l_afterDate = 0;
			if (this->HasValue("after.datejulian"))
			{
				l_afterDate = this->GetParameterFloat("after.datejulian");
			} else if (this->HasValue("after.dateutc"))
			{
				l_afterDate = Date::GetJulianDayAsDouble(Date::GetTmFromDateUTC(this->GetParameterString("after.dateutc")));
			}

			if (l_afterDate < l_julianDate)
			{
				vnsExceptionDataMacro("After CAMS is not after the requested date");
			}
			//Set in the handler
			l_camsHandler->SetAfterCAMS(l_afterAOT,l_afterMR,l_afterRH,l_ModelLevels, l_nbInterpol, l_afterDate);
		}

		l_camsHandler->ExtractCAMSData(l_corner, l_julianDate);

		if (l_camsHandler->isValid())
		{
			this->SetParameterFloat("rh", l_camsHandler->GetOutSamplingRH());
			typedef CAMSFileHandler::AOTImageType AOTImageType;
			this->SetParameterOutputImage<AOTImageType>("sumofaot", l_camsHandler->GetAOTImage());
			std::ostringstream l_outPropsStr;
			for (CAMSFileHandler::ProportionMapType::const_iterator itProp = l_camsHandler->GetOutProportions().begin();
					itProp != l_camsHandler->GetOutProportions().end();++itProp)
			{
				l_outPropsStr << std::setprecision(15)<<itProp->first << "="<<itProp->second << ";";
			}
			this->SetParameterString("proportions", l_outPropsStr.str());
		}



	}


	/** Filters declaration */

};

}
}


OTB_APPLICATION_EXPORT(vns::Wrapper::CAMSComputation)
