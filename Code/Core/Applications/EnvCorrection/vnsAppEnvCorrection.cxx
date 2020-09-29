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
#include "vnsComputeEnvironmentCorrectionImageFilter.h"
#include "vnsUtilities.h"

namespace vns
{
namespace Wrapper
{

using namespace otb::Wrapper;

class EnvCorrection : public Application
{
public:
	/** Standard class typedefs. */
	typedef EnvCorrection Self;
	typedef otb::Wrapper::Application     Superclass;
	typedef itk::SmartPointer<Self>       Pointer;
	typedef itk::SmartPointer<const Self> ConstPointer;

	/** Standard macro */
	itkNewMacro(Self);

	itkTypeMacro(EnvCorrection, otb::Wrapper::Application);

	/** Image typedef */
	typedef VNSRealVectorImageType InputVectorImageType;
    typedef InputVectorImageType::ConstPointer InputVectorImageConstPointerType;
    typedef InputVectorImageType::Pointer InputVectorImagePointerType;

	typedef VNSMaskImageType InputMaskType;
	typedef InputMaskType::Pointer InputMaskPointerType;
	typedef InputMaskType::ConstPointer InputMaskConstPointerType;
	typedef VNSMaskImageType OutputMaskType;
	typedef OutputMaskType::Pointer OutputMaskPointerType;
	typedef OutputMaskType::ConstPointer OutputMaskConstPointerType;
	typedef OutputMaskType::PixelType OutputMaskPixelType;

	typedef VNSRealVectorImageType OutputVectorImageType;
	typedef OutputVectorImageType::Pointer OutputVectorImagePointerType;
	typedef OutputVectorImageType::ConstPointer OutputVectorImageConstPointerType;
	typedef OutputVectorImageType::RegionType OutputVectorImageRegionType;

	typedef ComputeEnvironmentCorrectionImageFilter<InputVectorImageType, OutputVectorImageType, InputMaskType> ComputeEnvironmentCorrectionImageFilterType;
    typedef ComputeEnvironmentCorrectionImageFilterType::Pointer ComputeEnvironmentCorrectionImageFilterPointer;


private:
	void DoInit()
	{
		SetName("EnvCorrection");
		SetDescription("Generate the EnvCorrection.");
		Loggers::GetInstance()->Initialize(GetName());
		// Documentation
		SetDocLongDescription("This application computes the EnvCorrection");
		SetDocLimitations("None");
		SetDocAuthors("MAJA-Team");
		SetDocSeeAlso("MAJA Doc");

		AddDocTag("Statistics");

		AddParameter(ParameterType_InputImage,  "tdir",   "TDIR");
		SetParameterDescription("tdir", "TDIR");
		AddParameter(ParameterType_InputImage,  "tdif",   "TDIF");
		SetParameterDescription("tdif", "TDIF");
		AddParameter(ParameterType_InputImage,  "albd",   "ALBD");
		SetParameterDescription("albd", "Albedo");
		AddParameter(ParameterType_InputImage,  "toc",   "TOC image");
		SetParameterDescription("toc", "TOC image at coarse");
		AddParameter(ParameterType_InputImage,  "edg",   "EDG image");
		SetParameterDescription("edg", "Image used as EDG at coarse");
		AddParameter(ParameterType_InputImage,  "rhoenvsub",   "RhoEnv image");
		SetParameterDescription("rhoenvsub", "Image used as RhoEnv at coarse");


		AddParameter(ParameterType_Float, "nodata", "NoData");

		AddParameter(ParameterType_OutputImage, "sre", "Computed SRE");
		SetParameterDescription("sre","Computed SRE");
		SetParameterOutputImagePixelType("sre", ImagePixelType_double);
		AddParameter(ParameterType_OutputImage, "rhoenv", "Computed RhoEnv");
		SetParameterDescription("rhoenv","Computed rhoenv");
		SetParameterOutputImagePixelType("rhoenv", ImagePixelType_double);

		AddRAMParameter("ram");
		SetDefaultParameterInt("ram",2048);
	}

	void DoUpdateParameters()
	{

	}


	void DoExecute()
	{
		// Init filters
		m_ComputeEnvironmentCorrectionImageFilter = ComputeEnvironmentCorrectionImageFilterType::New();
		m_ComputeEnvironmentCorrectionImageFilter->SetReleaseDataBeforeUpdateFlag(true);
		m_ComputeEnvironmentCorrectionImageFilter->SetReleaseDataFlag(true);
		//Get image pointers
		InputVectorImageConstPointerType l_RhoEnvPtr = this->GetParameterDoubleVectorImage("rhoenvsub");
		InputVectorImageConstPointerType l_TdirPtr = this->GetParameterDoubleVectorImage("tdir");
		InputVectorImageConstPointerType l_TdifPtr = this->GetParameterDoubleVectorImage("tdif");
		InputVectorImageConstPointerType l_SatmPtr = this->GetParameterDoubleVectorImage("albd");
		InputVectorImageConstPointerType l_IPTOCPtr = this->GetParameterDoubleVectorImage("toc");
		InputMaskConstPointerType l_L2EDGPtr = this->GetParameterUInt8Image("edg");

        //GetParams
		const double l_NoData = this->GetParameterFloat("nodata");

		//Configure filter
        m_ComputeEnvironmentCorrectionImageFilter->SetRhoEnvSub(l_RhoEnvPtr);
        m_ComputeEnvironmentCorrectionImageFilter->SetLUT_Tdir(l_TdirPtr);
        m_ComputeEnvironmentCorrectionImageFilter->SetLUT_Tdif(l_TdifPtr);
        m_ComputeEnvironmentCorrectionImageFilter->SetLUT_Satm(l_SatmPtr);
        m_ComputeEnvironmentCorrectionImageFilter->SetIPTOC(l_IPTOCPtr);
        m_ComputeEnvironmentCorrectionImageFilter->SetL2EDG(l_L2EDGPtr);
        m_ComputeEnvironmentCorrectionImageFilter->SetNoData(l_NoData);
        m_ComputeEnvironmentCorrectionImageFilter->UpdateWithCaching();
		//Set the output
		SetParameterOutputImage<OutputVectorImageType>("sre",m_ComputeEnvironmentCorrectionImageFilter->GetL2SRE());
		SetParameterOutputImage<OutputVectorImageType>("rhoenv",m_ComputeEnvironmentCorrectionImageFilter->GetRhoEnv());


	}

	/** Filters declaration */
	ComputeEnvironmentCorrectionImageFilterPointer m_ComputeEnvironmentCorrectionImageFilter;


};

}
}

OTB_APPLICATION_EXPORT(vns::Wrapper::EnvCorrection)
