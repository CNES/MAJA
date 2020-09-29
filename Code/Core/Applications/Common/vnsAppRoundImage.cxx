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
#include "vnsRoundVectorImageFunctor.h"
#include "vnsRoundImageFunctor.h"
#include "otbUnaryFunctorVectorImageFilter.h"
#include "itkUnaryFunctorImageFilter.h"
#include <string>
#include "vnsLoggers.h"
#include "vnsMacro.h"

namespace vns
{

namespace Wrapper
{

using namespace otb::Wrapper;

class RoundImage : public Application
{
public:
	/** Standard class typedefs. */
	typedef RoundImage			 		  Self;
	typedef otb::Wrapper::Application     Superclass;
	typedef itk::SmartPointer<Self>       Pointer;
	typedef itk::SmartPointer<const Self> ConstPointer;

	/** Standard macro */
	itkNewMacro(Self);

	itkTypeMacro(RoundImage, otb::Wrapper::Application);

	/** Some convenient typedefs. */
	typedef DoubleVectorImageType InputImageType;

	typedef vns::Functor::RoundVectorImage<InputImageType::PixelType, InputImageType::PixelType> RoundVectorImageFunctor;
	typedef otb::UnaryFunctorVectorImageFilter<InputImageType, InputImageType, RoundVectorImageFunctor> RealToRealRoundVectorImageFilterType;

	typedef vns::Functor::RoundImage<DoubleImageType::PixelType, DoubleImageType::PixelType> RoundImageFunctor;
	typedef itk::UnaryFunctorImageFilter<DoubleImageType, DoubleImageType, RoundImageFunctor> RealToRealRoundImageFilterType;


private:
	void DoInit()
	{
		SetName("RoundImage");
		SetDescription("Round the image to the closest int value.");
		Loggers::GetInstance()->Initialize(GetName());
		// Documentation
		SetDocLongDescription("If one band equal threshold the band equal value");
		SetDocLimitations("None");
		SetDocAuthors("MAJA-Team");
		SetDocSeeAlso("MAJA Doc");

		AddDocTag("Mask");

		AddParameter(ParameterType_InputImage,  "im",   "image");
		AddParameter(ParameterType_OutputImage, "out", "image");
		SetParameterOutputImagePixelType("out", ImagePixelType_double);
		SetParameterDescription("out","output image");
		AddParameter(ParameterType_Float,"coef","multiplication coeff");
		MandatoryOff("coef");
		AddRAMParameter("ram");
		SetDefaultParameterInt("ram", 2048);

	}

	void DoUpdateParameters()
	{
	}


	void DoExecute()
	{
		// Get input image pointers
		ImageBaseType* l_inPtr = GetParameterImageBase("im",0);
		// Guess the image type
		std::string className(l_inPtr->GetNameOfClass());


		if (className == "VectorImage") {
			// Init filters
			DoubleVectorImageType::ConstPointer l_im = this->GetParameterDoubleVectorImage("im");
			m_filter = RealToRealRoundVectorImageFilterType::New();
			m_filter->SetInput(l_im);
			if(HasValue("coef"))
			{
				m_filter->GetFunctor().SetCoef(this->GetParameterFloat("coef"));
			}
			SetParameterOutputImage<InputImageType>("out", m_filter->GetOutput());
		} else {
			// Init filters
			DoubleImageType::ConstPointer l_im = this->GetParameterDoubleImage("im");
			m_single_filter = RealToRealRoundImageFilterType::New();
			m_single_filter->SetInput(l_im);
			if(HasValue("coef"))
			{
				m_single_filter->GetFunctor().SetCoef(this->GetParameterFloat("coef"));
			}
			SetParameterOutputImage<DoubleImageType>("out", m_single_filter->GetOutput());
		}
	}

	/** Filters declaration */
	 RealToRealRoundVectorImageFilterType::Pointer m_filter;
	 RealToRealRoundImageFilterType::Pointer m_single_filter;
};

}
}

OTB_APPLICATION_EXPORT(vns::Wrapper::RoundImage)
