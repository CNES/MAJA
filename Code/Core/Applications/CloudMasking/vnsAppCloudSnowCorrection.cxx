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
#include "vnsConstantImageSource.h"
#include "vnsSnowMaskCorrectionImageFilter.h"
#include <string>

namespace vns
{
namespace Wrapper
{

using namespace otb::Wrapper;

class CloudSnowCorrection : public Application
{
public:
	/** Standard class typedefs. */
	typedef CloudSnowCorrection           Self;
	typedef otb::Wrapper::Application     Superclass;
	typedef itk::SmartPointer<Self>       Pointer;
	typedef itk::SmartPointer<const Self> ConstPointer;

	/** Standard macro */
	itkNewMacro(Self);

	itkTypeMacro(CloudSnowCorrection, otb::Wrapper::Application);

	/** Some convenient typedefs. */
	//typedef TInputMask InputMaskType;
	typedef UInt8ImageType InputMaskType;
	typedef InputMaskType::Pointer InputMaskPointer;
	typedef InputMaskType::ConstPointer InputMaskConstPointer;
	typedef InputMaskType::PixelType InputMaskPixelType;
	// Snow mask correction image filter
	typedef SnowMaskCorrectionImageFilter<InputMaskType> SnowMaskCorrectionFilterType;
	typedef SnowMaskCorrectionFilterType::Pointer SnowMaskCorrectionPointerFilterType;


private:
	void DoInit()
	{
		SetName("CloudSnowCorrection");
		SetDescription("Cloud Snow Correction algo.");
		Loggers::GetInstance()->Initialize(GetName());
		// Documentation
		SetDocLongDescription("This application computes the cloud SnowCorrection on the reflectance mask");
		SetDocLimitations("None");
		SetDocAuthors("MAJA-Team");
		SetDocSeeAlso("MAJA Doc");

		AddDocTag("Statistics");
		//Input Images
		AddParameter(ParameterType_InputImage,  "snowin",   "SnowMask");
		SetParameterDescription("snowin", "Snow mask at coarse");
		AddParameter(ParameterType_InputImage,  "edg",   "EDG image");
		SetParameterDescription("edg", "Image used as edge at coarse");
		AddParameter(ParameterType_InputImage,  "cldrefl",   "CLD Refl image");
		SetParameterDescription("cldrefl", "Image used as cldrefl");
		AddParameter(ParameterType_InputImage,  "cldreflvar",   "CLD ReflVar image");
		SetParameterDescription("cldreflvar", "Image used as cldreflvar");

		//Set parameters
		AddParameter(ParameterType_Int,  "seradius","seradius");

		AddParameter(ParameterType_OutputImage, "refl", "Reflectance");
		SetParameterDescription("refl","Reflectance");
		SetParameterOutputImagePixelType("refl", ImagePixelType_uint8);
		AddParameter(ParameterType_OutputImage, "snow", "Snow Mask");
		SetParameterDescription("snow","SnowMask");
		SetParameterOutputImagePixelType("snow", ImagePixelType_uint8);

		AddRAMParameter("ram");
		SetDefaultParameterInt("ram",2048);

	}

	void DoUpdateParameters()
	{

	}


	void DoExecute()
	{
		// Filter initialization
		m_SnowMaskCorrectionFilter = SnowMaskCorrectionFilterType::New();
		// Load inputs
		InputMaskConstPointer l_IPEDGPtr =  this->GetParameterUInt8Image("edg");
		InputMaskConstPointer l_IPSNWSubPtr =  this->GetParameterUInt8Image("snowin");
		InputMaskConstPointer l_IPCLDRefl = this->GetParameterUInt8Image("cldrefl");
		InputMaskConstPointer l_IPCLDReflVar = this->GetParameterUInt8Image("cldreflvar");
		//Get parameters
		const double l_SEInputMasksRadius = GetParameterInt("seradius");

		m_SnowMaskCorrectionFilter->SetIPSNWSubInput(l_IPSNWSubPtr);
		m_SnowMaskCorrectionFilter->SetIPEDGSubInput(l_IPEDGPtr);
		m_SnowMaskCorrectionFilter->SetIPCLDReflSubInput(l_IPCLDRefl);
		m_SnowMaskCorrectionFilter->SetIPCLDReflVarSubInput(l_IPCLDReflVar);
		// the cloud_mask_sub.refl_var is set above
		m_SnowMaskCorrectionFilter->SetDilateRadius(l_SEInputMasksRadius);
		m_SnowMaskCorrectionFilter->SetOutputMaskValue(static_cast<InputMaskPixelType>(1));

		m_SnowMaskCorrectionFilter->UpdateWithCaching();

		// ========> Get the corrected snow output mask
		SetParameterOutputImage<InputMaskType>("refl",m_SnowMaskCorrectionFilter->GetIPCLDReflSubOutput());
		SetParameterOutputImage<InputMaskType>("snow",m_SnowMaskCorrectionFilter->GetIPSNWSubOutput());

	}


	/** Filters declaration */
	SnowMaskCorrectionPointerFilterType m_SnowMaskCorrectionFilter;

};

}
}

OTB_APPLICATION_EXPORT(vns::Wrapper::CloudSnowCorrection)
