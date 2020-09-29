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
#include "vnsMacro.h"
#include "vnsCloudMaskUndersamplingWithoutResamplingImageFilter.h"
#include "vnsCloudMaskUndersamplingInterpolateImageFunction.h"
#include "vnsGridResampleImageFilter.h"
#include "vnsResamplerHelper.h"

#include <string>

namespace vns
{
enum
{
	interp_cloudcla,
	interp_cloudclalt
};

namespace Wrapper
{

using namespace otb::Wrapper;

class CloudMaskUndersampling : public Application
{
public:
	/** Standard class typedefs. */
	typedef CloudMaskUndersampling        Self;
	typedef otb::Wrapper::Application     Superclass;
	typedef itk::SmartPointer<Self>       Pointer;
	typedef itk::SmartPointer<const Self> ConstPointer;

	/** Standard macro */
	itkNewMacro(Self);

	itkTypeMacro(CloudMaskUndersampling, otb::Wrapper::Application);

	/** Some convenient typedefs. */
	typedef DoubleImageType ImageType;
	typedef ImageType::ConstPointer ImageConstPointer;
	typedef ImageType::Pointer ImagePointer;
	typedef ImageType::PixelType PixelType;
	typedef ImageType::SpacingType SpacingType;

	typedef UInt8ImageType MaskType;
	typedef MaskType::Pointer MaskPointer;
	typedef MaskType::ConstPointer MaskConstPointer;


    typedef CloudMaskUndersamplingWithoutResamplingImageFilter<ImageType, ImageType, MaskType> CloudMaskUndersamplingWithoutResamplingImageFilterType;
    typedef GridResampleImageFilter<ImageType, ImageType> GridResampleImageFilterCLAType;
    typedef GridResampleImageFilter<ImageType, MaskType> GridResampleImageFilterCLDAltType;


private:
	void DoInit()
	{
		SetName("CloudMaskUndersampling");
		SetDescription("CloudMaskUndersampling algo.");
		Loggers::GetInstance()->Initialize(GetName());
		// Documentation
		SetDocLongDescription("This application resample the cloud according to the dtm");
		SetDocLimitations("None");
		SetDocAuthors("MAJA-Team");
		SetDocSeeAlso("MAJA Doc");

		AddDocTag("Statistics");

		AddParameter(ParameterType_InputImage,  "dtm",   "DTM");
		SetParameterDescription("dtm", "Image used as reference grid");
		AddParameter(ParameterType_InputImage,  "im",   "image");
		SetParameterDescription("im", "Image to be resampled");
		AddParameter(ParameterType_Float,  "nodata", "nodata");
		SetParameterDescription("nodata", "nodata to use in CLA resampling");


		AddRAMParameter("ram");
		SetDefaultParameterInt("ram",2048);

		AddParameter(ParameterType_OutputImage, "cla", "cloud image");
		SetParameterDescription("cla","cloud output image");
		AddParameter(ParameterType_OutputImage, "clalt", "cloud altitude image");
		SetParameterDescription("clalt","cloud output image");

	}

	void DoUpdateParameters()
	{

	}


	void DoExecute()
	{
		// Init filters
		m_ResampleImageFilterCLA = GridResampleImageFilterCLAType::New();
		m_ResampleImageFilterCLDAlt = GridResampleImageFilterCLDAltType::New();

		// Get input image pointers
		ImagePointer l_inPtr = GetParameterDoubleImage("im");
		l_inPtr->UpdateOutputInformation();
		bool l_isVec = true;

		ImagePointer l_dtmPtr = GetParameterDoubleImage("dtm");
		l_dtmPtr->UpdateOutputInformation();
		const double l_nodata = GetParameterFloat("nodata");
		Area l_outArea;
		l_outArea.Origin = l_dtmPtr->GetOrigin();
		l_outArea.Spacing = l_dtmPtr->GetSpacing();
		l_outArea.Size = l_dtmPtr->GetLargestPossibleRegion().GetSize();


		const SpacingType l_InputSpacing = l_inPtr->GetSpacing();
		vnsLogDebugMacro("L1CLA Spacing value: "<< l_InputSpacing)

		const double l_SpacingRatio = vcl_abs(static_cast<double>(l_outArea.Spacing[0]) / static_cast<double>(l_InputSpacing[0]));
		vnsLogDebugValueMacro(l_SpacingRatio)

		if (vnsEqualsDoubleMacro(l_SpacingRatio,1.) == true)
		{
			m_CloudMaskUndersamplingWithoutResamplingImageFilter = CloudMaskUndersamplingWithoutResamplingImageFilterType::New();
			m_CloudMaskUndersamplingWithoutResamplingImageFilter->SetL1CLAInput(l_inPtr);
			m_CloudMaskUndersamplingWithoutResamplingImageFilter->SetNoData(l_nodata);

			// Set the output image pointer of the filter
			SetParameterOutputImagePixelType("cla", ImagePixelType_double);
			SetParameterOutputImage<ImageType>("cla",m_CloudMaskUndersamplingWithoutResamplingImageFilter->GetIPCLAOutput());
			SetParameterOutputImage<MaskType>("clalt",m_CloudMaskUndersamplingWithoutResamplingImageFilter->GetIPCLDAltOutput());

		}
		else
		{
			unsigned int l_InterpolatorRadius(0);
			// CLA computes
			m_ResampleImageFilterCLA->SetInput(l_inPtr);
			m_ResampleImageFilterCLA->SetOutputOrigin(l_outArea.Origin);
			m_ResampleImageFilterCLA->SetOutputSpacing(l_outArea.Spacing);
			m_ResampleImageFilterCLA->SetOutputSize(l_outArea.Size);
			typedef CloudMaskUndersamplingCLAInterpolateImageFunction<ImageType, double> CLAInterpolatorType;
			CLAInterpolatorType::Pointer f_interpolator_cla = CLAInterpolatorType::New();
			// Compute interpolator radius
			l_InterpolatorRadius = ResamplerHelper::ComputeInterpolatorRadius(l_InputSpacing, l_outArea.Spacing,
					ResamplerHelper::RB_INTERPOLATOR_MAJA_CLOUD_UNDERSAMPLING_CLA);
			vnsLogDebugMacro("RB_INTERPOLATOR_MAJA_CLOUD_UNDERSAMPLING_CLA - Radius = "<<l_InterpolatorRadius)
			f_interpolator_cla->SetRadius(l_InterpolatorRadius);

			// Set NoData
			f_interpolator_cla->SetNoData(l_nodata);

			// Set interpolator to the resampler filter
			m_ResampleImageFilterCLA->SetInterpolator(f_interpolator_cla);
			// Set the InterpolatorRadius to the resampler filter
			m_ResampleImageFilterCLA->SetInterpolatorRadius(l_InterpolatorRadius);

			// CLDAlt computes
			m_ResampleImageFilterCLDAlt->SetInput(l_inPtr);
			m_ResampleImageFilterCLDAlt->SetOutputOrigin(l_outArea.Origin);
			m_ResampleImageFilterCLDAlt->SetOutputSpacing(l_outArea.Spacing);
			m_ResampleImageFilterCLDAlt->SetOutputSize(l_outArea.Size);
			typedef CloudMaskUndersamplingCLDAltInterpolateImageFunction<ImageType, double> CLDAltInterpolatorType;
			CLDAltInterpolatorType::Pointer f_interpolator_cldalt = CLDAltInterpolatorType::New();
			// Compute interpolator radius
			l_InterpolatorRadius = ResamplerHelper::ComputeInterpolatorRadius(l_InputSpacing, l_outArea.Spacing,
					ResamplerHelper::RB_INTERPOLATOR_MAJA_CLOUD_UNDERSAMPLING_CLD_ALT);
			vnsLogDebugMacro("RB_INTERPOLATOR_MAJA_CLOUD_UNDERSAMPLING_CLDAlt - Radius = "<<l_InterpolatorRadius)
			f_interpolator_cldalt->SetRadius(l_InterpolatorRadius);

			// Set NoData
			f_interpolator_cldalt->SetNoData(l_nodata);

			// Set SuccesOutputValue FailureOutputValue
			f_interpolator_cldalt->SetSuccesOutputValue(1);
			f_interpolator_cldalt->SetFailureOutputValue(0);

			// Set interpolator to the resampler filter
			m_ResampleImageFilterCLDAlt->SetInterpolator(f_interpolator_cldalt);
			// Set the InterpolatorRadius to the resampler filter
			m_ResampleImageFilterCLDAlt->SetInterpolatorRadius(l_InterpolatorRadius);

			// Set the output image pointer of the filter
			SetParameterOutputImagePixelType("cla", ImagePixelType_double);
			SetParameterOutputImage<ImageType>("cla",m_ResampleImageFilterCLA->GetOutput());
			SetParameterOutputImage<MaskType>("clalt",m_ResampleImageFilterCLDAlt->GetOutput());

		}




	}


	/** Filters declaration */
    // CloudUndersampling without resampling
    CloudMaskUndersamplingWithoutResamplingImageFilterType::Pointer m_CloudMaskUndersamplingWithoutResamplingImageFilter;
    GridResampleImageFilterCLAType::Pointer m_ResampleImageFilterCLA;
    GridResampleImageFilterCLDAltType::Pointer  m_ResampleImageFilterCLDAlt;

};

}
}

OTB_APPLICATION_EXPORT(vns::Wrapper::CloudMaskUndersampling)
