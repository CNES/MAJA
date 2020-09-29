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
#include "vnsMultiplyByScalarVectorImageFilter.h"
#include "otbMultiplyByScalarImageFilter.h"
#include "vnsLoggers.h"
#include <string>

namespace vns
{

namespace Wrapper
{

using namespace otb::Wrapper;

class MultiplyByScalar: public Application
{
public:
	/** Standard class typedefs. */
	typedef MultiplyByScalar  	 		  Self;
	typedef otb::Wrapper::Application     Superclass;
	typedef itk::SmartPointer<Self>       Pointer;
	typedef itk::SmartPointer<const Self> ConstPointer;

	/** Standard macro */
	itkNewMacro(Self);

	itkTypeMacro(MultiplyByScalar, otb::Wrapper::Application);

	/** Some convenient typedefs. */
	typedef DoubleVectorImageType VectorImageType;
	typedef vns::MultiplyByScalarVectorImageFilter<VectorImageType, VectorImageType> RealToRealScalarVectorImageFilterType;
	typedef DoubleImageType ImageType;
	typedef otb::MultiplyByScalarImageFilter<ImageType, ImageType> RealToRealScalarImageFilterType;


private:
	void DoInit()
	{
		SetName("MultiplyByScalar");
		SetDescription("Multiply by scalar algo.");
		Loggers::GetInstance()->Initialize(GetName());
		// Documentation
		SetDocLongDescription("This application multiply each image of a vector image by a factor");
		SetDocLimitations("None");
		SetDocAuthors("MAJA-Team");
		SetDocSeeAlso("MAJA Doc");

		AddDocTag("Statistics");

		AddParameter(ParameterType_InputImage,  "im",   "vectorimage");
		AddParameter(ParameterType_Float, "coef","Coeff to multiply");
		AddParameter(ParameterType_OutputImage, "out", "image");
		SetParameterOutputImagePixelType("out", ImagePixelType_double);
		SetParameterDescription("out","output image");

		AddRAMParameter("ram");
		SetDefaultParameterInt("ram",2048);

	}

	void DoUpdateParameters()
	{

	}


	void DoExecute()
	{


		ImageBaseType* l_baseIm = this->GetParameterImageBase("im",0);
		// Guess the image type
		std::string className(l_baseIm->GetNameOfClass());

		if (className == "VectorImage") {
			// Init filters
			m_vector_filter = RealToRealScalarVectorImageFilterType::New();
			//Get Image
			VectorImageType::ConstPointer l_im = this->GetParameterDoubleVectorImage("im");
			const double l_coeff = this->GetParameterFloat("coef");
			m_vector_filter->SetInput(l_im);
			m_vector_filter->SetCoeff(l_coeff);
			SetParameterOutputImage<VectorImageType>("out",m_vector_filter->GetOutput());
		} else {
			// Init filters
			m_filter = RealToRealScalarImageFilterType::New();
			//Get Image
			ImageType::ConstPointer l_im = this->GetParameterDoubleImage("im");
			const double l_coeff = this->GetParameterFloat("coef");
			m_filter->SetInput(l_im);
			m_filter->SetCoef(l_coeff);
			SetParameterOutputImage<ImageType>("out",m_filter->GetOutput());
		}

	}


	/** Filters declaration */
	RealToRealScalarVectorImageFilterType::Pointer m_vector_filter;
	RealToRealScalarImageFilterType::Pointer m_filter;
};

}
}

OTB_APPLICATION_EXPORT(vns::Wrapper::MultiplyByScalar)
