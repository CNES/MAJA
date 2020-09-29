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
#include "vnsDirectionalCorrectionCompute.h"
#include "vnsUtilities.h"

namespace vns
{
namespace Wrapper
{

using namespace otb::Wrapper;

class DirectionnalCorrection : public Application
{
public:
	/** Standard class typedefs. */
	typedef DirectionnalCorrection        Self;
	typedef otb::Wrapper::Application     Superclass;
	typedef itk::SmartPointer<Self>       Pointer;
	typedef itk::SmartPointer<const Self> ConstPointer;

	/** Standard macro */
	itkNewMacro(Self);

	itkTypeMacro(DirectionnalCorrection, otb::Wrapper::Application);

	/** Some convenient typedefs. */
    //Directional correction compute
    typedef DirectionalCorrectionCompute DirectionalCorrectionComputeType;
private:
	void DoInit()
	{
		SetName("DirectionnalCorrection");
		SetDescription("DirectionnalCorrection algo.");
		Loggers::GetInstance()->Initialize(GetName());
		// Documentation
		SetDocLongDescription("This application computes the directional correction coeffs");
		SetDocLimitations("None");
		SetDocAuthors("MAJA-Team");
		SetDocSeeAlso("MAJA Doc");

		AddDocTag("Statistics");

		AddParameter(ParameterType_StringList, "coeffsr", "CoeffsR");
		AddParameter(ParameterType_StringList, "coeffsv", "CoeffsV");
		AddParameter(ParameterType_Group, "solar","Solar angles");
		AddParameter(ParameterType_Float, "solar.zenith","Solar zenith");
		AddParameter(ParameterType_Float, "solar.azimuth","Solar azimuth");
		AddParameter(ParameterType_Group, "viewing","Mean Viewing angles");
		AddParameter(ParameterType_StringList, "viewing.zenith","Viewing zenith");
		AddParameter(ParameterType_StringList, "viewing.azimuth","Viewing azimuth");
		AddParameter(ParameterType_Float,  "refzenith","refzenith");
		AddParameter(ParameterType_Float,  "refazimuth","refazimuth");

		AddParameter(ParameterType_String, "coeffs", "correction coeffs");
		SetParameterRole("coeffs", Role_Output);

		AddRAMParameter("ram");
		SetDefaultParameterInt("ram",2048);

	}

	void DoUpdateParameters()
	{

	}


	void DoExecute()
	{
		//Get Params
		//Correction coeff
		Utilities::ListOfDoubles m_CoefsR = Utilities::StringListAsDouble(
				this->GetParameterStringList("coeffsr"));
		Utilities::ListOfDoubles m_CoefsV= Utilities::StringListAsDouble(
				this->GetParameterStringList("coeffsv"));
		double m_SolarZenith = GetParameterFloat("solar.zenith");
		double m_SolarAzimuth = GetParameterFloat("solar.azimuth");
		Utilities::ListOfDoubles m_MeanViewingZenithAngles = Utilities::StringListAsDouble(
				this->GetParameterStringList("viewing.zenith"));
		Utilities::ListOfDoubles m_MeanViewingAzimuthAngles = Utilities::StringListAsDouble(
				this->GetParameterStringList("viewing.azimuth"));
		double m_RefZenith = GetParameterFloat("refzenith");
		double m_RefAzimuth = GetParameterFloat("refazimuth");
		const unsigned int nb_MeanViewingAngles = m_MeanViewingZenithAngles.size();
		//Verify consitency of input
		if (m_CoefsR.size() != m_MeanViewingZenithAngles.size()){
			vnsExceptionAlgorithmsProcessingMacro("Internal error: the number of CoefR '"<<m_CoefsR.size()<<
					"' is different than the ViewingAnglesZenithPerBand'"<<m_MeanViewingZenithAngles.size()<<"'!")
		}
		if (m_CoefsV.size() != m_MeanViewingZenithAngles.size()){
			vnsExceptionAlgorithmsProcessingMacro("Internal error: the number of CoefV '"<<m_CoefsV.size()<<
					"' is different than the ViewingAnglesZenithPerBand'"<<m_MeanViewingZenithAngles.size()<<"'!")
		}
		//Compute the directional correction coeffs
		Utilities::ListOfDoubles coefs = DirectionalCorrectionComputeType::ComputeCorrectionCoeffs(m_SolarZenith,m_SolarAzimuth,
				m_RefZenith,m_RefAzimuth,m_MeanViewingZenithAngles, m_MeanViewingAzimuthAngles, m_CoefsR,m_CoefsV);
		for (unsigned int bandNum = 0; bandNum < nb_MeanViewingAngles; bandNum++)
		{
			vnsLogDebugMacro("Direction Correction Coeff band "<<bandNum<<" = "<<std::setprecision(20)<<coefs.at(bandNum));
		}

		std::string l_String("");
		if (coefs.size() > 0)
		{
			for (unsigned int i = 0; i < coefs.size() - 1; i++)
			{
				std::ostringstream oss;
				oss << std::setprecision(12)<<coefs[i];
				l_String = l_String + oss.str() + " ";
			}
			std::ostringstream oss;
			oss << coefs[coefs.size() - 1];
			l_String = l_String + oss.str().c_str();

		}
		SetParameterString("coeffs",l_String);
	}


	/** Filters declaration */

};

}
}


OTB_APPLICATION_EXPORT(vns::Wrapper::DirectionnalCorrection)
