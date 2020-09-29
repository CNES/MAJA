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
#include "vnsChangeValueFunctor.h"
#include "itkUnaryFunctorImageFilter.h"
#include "vnsLoggers.h"
#include "vnsMacro.h"
#include "vnsUtilities.h"
#include <string>

namespace vns
{

namespace Wrapper
{

using namespace otb::Wrapper;

class ChangeValues : public Application
{
public:
	/** Standard class typedefs. */
	typedef ChangeValues	              Self;
	typedef otb::Wrapper::Application     Superclass;
	typedef itk::SmartPointer<Self>       Pointer;
	typedef itk::SmartPointer<const Self> ConstPointer;

	/** Standard macro */
	itkNewMacro(Self);

	itkTypeMacro(ChangeValues, otb::Wrapper::Application);

	/** Some convenient typedefs. */
	typedef DoubleImageType ImageType;
	typedef ImageType::InternalPixelType ImagePixelType;
	//Change value typedef
	typedef Functor::ChangeValue<ImagePixelType, ImagePixelType> ChangeValueFunctorType;
	typedef itk::UnaryFunctorImageFilter<ImageType, ImageType, ChangeValueFunctorType> ChangeValueImageFilterType;
	typedef typename ChangeValueImageFilterType::Pointer ChangeValueImageFilterPointer;

private:
	void DoInit()
	{
		SetName("ChangeValues");
		SetDescription("Change certain values for other in the input.");
		Loggers::GetInstance()->Initialize(GetName());
		// Documentation
		SetDocLongDescription("This application changes some value for others as specified");
		SetDocLimitations("None");
		SetDocAuthors("MAJA-Team");
		SetDocSeeAlso("MAJA Doc");

		AddDocTag("Statistics");

		AddParameter(ParameterType_InputImage,  "im",   "single band image");
		AddParameter(ParameterType_StringList, "invals","values to replace");
		AddParameter(ParameterType_StringList, "outvals","values to put instead");
		AddParameter(ParameterType_OutputImage, "out", "image");
		SetParameterDescription("out","output image");

		AddRAMParameter("ram");
		SetDefaultParameterInt("ram",2048);

	}

	void DoUpdateParameters()
	{

	}


	void DoExecute()
	{
		// Init filters
		m_filter = ChangeValueImageFilterType::New();

		//Get Image
		ImageType::ConstPointer l_im = this->GetParameterDoubleImage("im");
		const std::vector<double> l_in_vals = vns::Utilities::StringListAsDouble(this->GetParameterStringList("invals"));
		const std::vector<double> l_out_vals = vns::Utilities::StringListAsDouble(this->GetParameterStringList("outvals"));
		if (l_in_vals.size() != l_out_vals.size())
		{
			vnsExceptionDataMacro("Not the same number of values for in and out values");
		}
		m_filter->SetInput(l_im);
		m_filter->GetFunctor().SetChangeVector(l_in_vals, l_out_vals);
		SetParameterOutputImage<ImageType>("out",m_filter->GetOutput());

	}


	/** Filters declaration */
	ChangeValueImageFilterType::Pointer m_filter;
};

}
}

OTB_APPLICATION_EXPORT(vns::Wrapper::ChangeValues)
