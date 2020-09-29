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
#include "itkTernaryFunctorImageFilter.h"
#include "vnsConstantImageSource.h"
#include "vnsSolarDirectionForCloudAltitudeFunctor.h"

#include <string>

namespace vns
{
namespace Wrapper
{

using namespace otb::Wrapper;

class SolarAngle : public Application
{
public:
	/** Standard class typedefs. */
	typedef SolarAngle             Self;
	typedef otb::Wrapper::Application     Superclass;
	typedef itk::SmartPointer<Self>       Pointer;
	typedef itk::SmartPointer<const Self> ConstPointer;

	/** Standard macro */
	itkNewMacro(Self);

	itkTypeMacro(SolarAngle, otb::Wrapper::Application);

	/** Some convenient typedefs. */
	typedef DoubleVectorImageType GRIDImageType;
	typedef GRIDImageType::ConstPointer GRIDImageConstPointer;
	typedef GRIDImageType::Pointer GRIDImagePointer;
	typedef GRIDImageType::PixelType GRIDImagePixelType;

	// Altitude image type
	typedef double AltPixelType;
	typedef otb::Image<AltPixelType, GRIDImageType::ImageDimension> AltImageType;
	typedef AltImageType::PixelType AltImagePixelType;

	typedef ConstantImageSource<AltImageType> AltConstantFilterType;
	typedef AltConstantFilterType::Pointer AltConstantFilterPointerType;
	typedef Functor::SolarDirectionForCloudAltitudeFunctor<GRIDImagePixelType, GRIDImagePixelType, AltImagePixelType, GRIDImagePixelType> FunctorType;
	typedef itk::TernaryFunctorImageFilter<GRIDImageType, GRIDImageType, AltImageType, GRIDImageType, FunctorType> SolarFilterType;
	typedef SolarFilterType::Pointer SolarFilterPointer;



private:
	void DoInit()
	{
		SetName("SolarAngle");
		SetDescription("SolarAngle algo.");
		Loggers::GetInstance()->Initialize(GetName());
		// Documentation
		SetDocLongDescription("This application computes the Solar angle");
		SetDocLimitations("None");
		SetDocAuthors("MAJA-Team");
		SetDocSeeAlso("MAJA Doc");

		AddDocTag("Statistics");
		AddParameter(ParameterType_Group, "sol1", "SOL1 parameters");
		AddParameter(ParameterType_InputImage,  "sol1.in",   "SOL1 image");
		SetParameterDescription("sol1.in", "Image used as solar angle 1");
		AddParameter(ParameterType_Int,  "sol1.h",   "SOL1 height");
		SetParameterDescription("sol1.h", "Height used to compute solar angle");

		AddParameter(ParameterType_Group, "sol2", "SOL2 parameters");
		MandatoryOff("sol2");
		AddParameter(ParameterType_InputImage,  "sol2.in",   "SOL2 image");
		MandatoryOff("sol2.in");
		SetParameterDescription("sol2.in", "Image used as solar angle 2");
		AddParameter(ParameterType_Int,  "sol2.h",   "SOL2 height");
		MandatoryOff("sol2.h");
		SetParameterDescription("sol2.h", "Height used to compute solar angle");
		AddParameter(ParameterType_Int,  "solhref",  "solar height ref");
		SetParameterDescription("solhref","solar height ref");
		AddParameter(ParameterType_OutputImage, "sol", "solar angle image");
		SetParameterDescription("sol","solar angle image");
		SetParameterOutputImagePixelType("sol", ImagePixelType_double);
		AddParameter(ParameterType_Int, "outhref", "wiewing height used ");
		SetParameterDescription("outhref","solar angle image");
		SetParameterRole("outhref", Role_Output);

		AddRAMParameter("ram");
		SetDefaultParameterInt("ram",2048);

	}

	void DoUpdateParameters()
	{

	}


	void DoExecute()
	{
		// Init filters
		m_SolarFilter = SolarFilterType::New();
		m_SOLHRefConstantFilter = AltConstantFilterType::New();
		// Get input image pointers
		GRIDImageType::Pointer l_sol1 = this->GetParameterDoubleVectorImage("sol1.in");
		GRIDImageType::Pointer l_sol2;

		// Set viewing and solar reference altitudes
		// --------------------------------------------------
		const unsigned short l_SOLHRef = static_cast<unsigned short>(this->GetParameterInt("solhref"));
		const unsigned short l_SOLH1 = static_cast<unsigned short>(this->GetParameterInt("sol1.h"));

		// =======================================================
		// Generate a constant image with solar reference altitude
		// =======================================================
		m_SOLHRefConstantFilter->SetSize(l_sol1->GetLargestPossibleRegion().GetSize());
		m_SOLHRefConstantFilter->SetSignedSpacing(l_sol1->GetSignedSpacing());
		m_SOLHRefConstantFilter->SetOrigin(l_sol1->GetOrigin());
		m_SOLHRefConstantFilter->SetConstantValue(static_cast<AltImagePixelType>(l_SOLHRef));

		// ===============================================================
		// Estimate the solar direction for each reference altitude
		// The reference altitude should be the higher altitude of the DTM
		// ===============================================================

		if (this->HasValue("sol2.in") && this->HasValue("sol2.h"))
		{
			l_sol2 = this->GetParameterDoubleVectorImage("sol2.in");
			const unsigned short l_SOLH2 = static_cast<unsigned short>(this->GetParameterInt("sol2.h"));
			m_SolarFilter->SetInput1(l_sol1);
			m_SolarFilter->SetInput2(l_sol2);
			m_SolarFilter->SetInput3(m_SOLHRefConstantFilter->GetOutput());

			m_SolarFilter->GetFunctor().SetH1(l_SOLH1);
			m_SolarFilter->GetFunctor().SetH2(l_SOLH2);
			m_SolarFilter->GetFunctor().SetSOLHRef(l_SOLHRef);

			SetParameterOutputImage<GRIDImageType>("sol",m_SolarFilter->GetOutput());
			SetParameterInt("outhref", l_SOLHRef);

		}
		else
		{
			SetParameterOutputImage<GRIDImageType>("sol",l_sol1);
			SetParameterInt("outhref", l_SOLH1);
		}


	}


	/** Filters declaration */
	SolarFilterPointer m_SolarFilter;
	AltConstantFilterPointerType m_SOLHRefConstantFilter;

};

}
}

OTB_APPLICATION_EXPORT(vns::Wrapper::SolarAngle)
