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
#include "vnsIncidenceAngleFunctor.h"
#include "itkBinaryFunctorImageFilter.h"
#include <string>
#include "otbMath.h"

namespace vns
{
namespace Wrapper
{

using namespace otb::Wrapper;

class IncidenceAngle : public Application
{
public:
	/** Standard class typedefs. */
	typedef IncidenceAngle             Self;
	typedef otb::Wrapper::Application     Superclass;
	typedef itk::SmartPointer<Self>       Pointer;
	typedef itk::SmartPointer<const Self> ConstPointer;

	/** Standard macro */
	itkNewMacro(Self);

	itkTypeMacro(IncidenceAngle, otb::Wrapper::Application);

	typedef DoubleImageType InputSimpleImageType;
	typedef InputSimpleImageType::Pointer InputSimpleImagePointer;
	typedef InputSimpleImageType::ConstPointer InputSimpleImageConstPointer;
	typedef InputSimpleImageType::PixelType InputSimpleImagePixelType;

	typedef DoubleImageType InternalIncidenceAngleImageType;
	typedef InternalIncidenceAngleImageType::Pointer InternalIncidenceAngleImageTypePointer;
	typedef InternalIncidenceAngleImageType::PixelType InternalIncidenceAnglePixelType;

	typedef Functor::IncidenceAngleFunctor< InputSimpleImagePixelType, InputSimpleImagePixelType, InternalIncidenceAnglePixelType > IncidenceAngleFunctorType;
	typedef itk::BinaryFunctorImageFilter< InputSimpleImageType, InputSimpleImageType, InternalIncidenceAngleImageType, IncidenceAngleFunctorType> IncidenceAngleFilterType;
	typedef IncidenceAngleFilterType::Pointer IncidenceAngleFilterPointer;


private:
	void DoInit()
	{
		SetName("IncidenceAngle");
		SetDescription("IncidenceAngle algo.");
		Loggers::GetInstance()->Initialize(GetName());
		// Documentation
		SetDocLongDescription("This application computes the incidence angle");
		SetDocLimitations("None");
		SetDocAuthors("MAJA-Team");
		SetDocSeeAlso("MAJA Doc");

		AddDocTag("Statistics");

		AddParameter(ParameterType_InputImage,  "demasc",   "DEM_ASC image");
		SetParameterDescription("demasc", "Image used as DEM_ASC");
		AddParameter(ParameterType_InputImage,  "demslc",   "DEM_SLC image");
		SetParameterDescription("demslc", "Image used as DEM_SLC");

		AddParameter(ParameterType_Float,  "thetas",  "solar azimuth");
		SetParameterDescription("thetas","Solar azimuth for computation in Rad");
		AddParameter(ParameterType_Float,  "phis",  "solar zenith");
		SetParameterDescription("phis","Solar zenith for computation in Rad");

		AddParameter(ParameterType_OutputImage, "angles", "incidence angles");
		SetParameterDescription("angles","incidence angles");
		SetParameterOutputImagePixelType("angles", ImagePixelType_double);

		AddRAMParameter("ram");
		SetDefaultParameterInt("ram",2048);
	}

	void DoUpdateParameters()
	{

	}


	void DoExecute()
	{
		// Init filters
		m_IncidenceAngleFilter = IncidenceAngleFilterType::New();

		// Get input image pointers
		InputSimpleImagePointer l_asc = this->GetParameterDoubleImage("demasc");
		InputSimpleImagePointer l_slc = this->GetParameterDoubleImage("demslc");

		//Set Parameters
		m_IncidenceAngleFilter->SetInput1(l_asc);
		m_IncidenceAngleFilter->SetInput2(l_slc);
		m_IncidenceAngleFilter->GetFunctor().SetPhiS(this->GetParameterFloat("phis")*otb::CONST_PI_180);
		m_IncidenceAngleFilter->GetFunctor().SetThetaS(this->GetParameterFloat("thetas")*otb::CONST_PI_180);
		SetParameterOutputImage<InputSimpleImageType>("angles",m_IncidenceAngleFilter->GetOutput());

	}


	/** Filters declaration */
	IncidenceAngleFilterPointer m_IncidenceAngleFilter;


};

}
}

OTB_APPLICATION_EXPORT(vns::Wrapper::IncidenceAngle)
