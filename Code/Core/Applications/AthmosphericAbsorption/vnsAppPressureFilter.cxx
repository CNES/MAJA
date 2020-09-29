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
#include "vnsPressureFunctor.h"
#include "itkUnaryFunctorImageFilter.h"
#include "vnsLoggers.h"

namespace vns
{
namespace Wrapper
{

using namespace otb::Wrapper;

class PressureFilter : public Application
{
public:
	/** Standard class typedefs. */
	typedef PressureFilter              Self;
	typedef otb::Wrapper::Application     Superclass;
	typedef itk::SmartPointer<Self>       Pointer;
	typedef itk::SmartPointer<const Self> ConstPointer;

	/** Standard macro */
	itkNewMacro(Self);

	itkTypeMacro(PressureFilter, otb::Application);

	/** Image typedef */
	/** Altitude input image typedefs. */
	typedef DoubleImageType DTMImageType;
	typedef otb::ObjectList<DTMImageType> DTMImageListType;
	typedef DTMImageListType::ConstPointer DTMImageListConstPointer;
	typedef DTMImageType::ConstPointer DTMImageConstPointer;
	typedef DTMImageType::PixelType DTMImagePixelType;
	typedef DTMImageType::InternalPixelType DTMImageInternalPixelType;
	typedef DoubleImageType TempImageType;
	typedef TempImageType::PixelType TempImagePixelType;

	/** Filters typedef */
    /** Pressure image filter typedefs */
    typedef Functor::PressureFunctor<DTMImagePixelType, TempImagePixelType> PressureFunctorType;
    typedef itk::UnaryFunctorImageFilter<DTMImageType, TempImageType, PressureFunctorType> PressureFilterType;
    typedef PressureFilterType::Pointer PressureFilterPointer;

private:
	void DoInit()
	{
		SetName("PressureFilter");
		SetDescription("Generate the pressure data from DTM.");
		Loggers::GetInstance()->Initialize(GetName());
		// Documentation
		SetDocLongDescription("This application computes the pressure from the dtm");
		SetDocLimitations("None");
		SetDocAuthors("MAJA-Team");
		SetDocSeeAlso("MAJA Doc");

		AddDocTag("Statistics");

		AddParameter(ParameterType_InputImage,  "dtm",   "DTM image");
		SetParameterDescription("dtm", "Image used as DTM");

		AddParameter(ParameterType_OutputImage, "pressure", "Pressure image");
		SetParameterDescription("pressure","Pressure image");
		SetParameterOutputImagePixelType("pressure", ImagePixelType_double);

		AddRAMParameter("ram");
		SetDefaultParameterInt("ram",2048);

	}

	void DoUpdateParameters()
	{

	}


	void DoExecute()
	{
		// Init filters
		m_PressureFilter = PressureFilterType::New();
		m_PressureFilter->SetInput(this->GetParameterDoubleImage("dtm"));

		SetParameterOutputImage<TempImageType>("pressure",m_PressureFilter->GetOutput());
	}

	/** Filters declaration */
	PressureFilterPointer m_PressureFilter;


};

}
}

OTB_APPLICATION_EXPORT(vns::Wrapper::PressureFilter)
