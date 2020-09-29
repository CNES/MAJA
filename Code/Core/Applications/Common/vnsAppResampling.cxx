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
#include "vnsPadAndResampleImageFilter.h"
#include "vnsBinaryThresholdVectorImageFilter.h"
#include "otbVectorImageToImageListFilter.h"
#include "itkBinaryThresholdImageFilter.h"
#include "vnsGDALLogInit.h"
#include <string>


#define CAST_AND_RES_VECTOR_IMAGE_BASE(Tin, Tout, image_base, out_thresh_type)     \
{                                                \
	Tin* img = dynamic_cast<Tin*>(image_base);         \
	\
	if (img)                                       \
	{                                              \
	DoResampleVector<Tin, Tout>(img, l_outArea, l_interpolator, l_padradius, out_thresh_type); \
	return;                                      \
	\
	}                                              \
} \


#define CAST_AND_RES_IMAGE_BASE(Tin, Tout, image_base, out_thresh_type)     \
{                                                \
	Tin* img = dynamic_cast<Tin*>(image_base);         \
	\
	if (img)                                       \
	{                                              \
	DoResample<Tin, Tout>(img, l_outArea, l_interpolator, l_padradius, out_thresh_type); \
	return;                                      \
	\
	}                                              \
} \

#define THRESHOLD_VECTOR_IMAGE_BASE(Tin, Tout, image_base, out_type, out_thresh_type)     \
{                                                \
	Tin* img = dynamic_cast<Tin*>(image_base);         \
	\
	if (img)                                       \
	{                                              \
	DoThresholdVector<Tin, Tout>(img, out_type, out_thresh_type); \
	return;                                      \
	\
	}                                              \
} \


#define THRESHOLD_IMAGE_BASE(Tin, Tout, image_base, out_type, out_thresh_type)     \
{                                                \
	Tin* img = dynamic_cast<Tin*>(image_base);         \
	\
	if (img)                                       \
	{                                              \
	DoThreshold<Tin, Tout>(img, out_type, out_thresh_type); \
	return;                                      \
	\
	}                                              \
} \


namespace vns
{
enum
{
	interp_linear,
	interp_bco,
	interp_linearradius,
	interp_fastlinear,
	interp_cloudcla,
	interp_cloudclalt
};

namespace Wrapper
{

using namespace otb::Wrapper;

class Resampling : public Application
{
public:
	/** Standard class typedefs. */
	typedef Resampling             Self;
	typedef otb::Wrapper::Application     Superclass;
	typedef itk::SmartPointer<Self>       Pointer;
	typedef itk::SmartPointer<const Self> ConstPointer;

	/** Standard macro */
	itkNewMacro(Self);

	itkTypeMacro(Resampling, otb::Wrapper::Application);

	/** Some convenient typedefs. */
	typedef DoubleVectorImageType VectorImageType;
	typedef VectorImageType::ConstPointer VectorImageConstPointer;
	typedef VectorImageType::Pointer VectorImagePointer;
	typedef VectorImageType::PixelType VectorPixelType;

	typedef DoubleImageType ImageType;
	typedef ImageType::ConstPointer ImageConstPointer;
	typedef ImageType::Pointer ImagePointer;
	typedef ImageType::PixelType PixelType;

	typedef UInt8VectorImageType MaskVectorType;
	typedef MaskVectorType::Pointer MaskVectorPointer;
	typedef MaskVectorType::ConstPointer MaskVectorConstPointer;

	typedef UInt8ImageType MaskType;
	typedef MaskType::Pointer MaskPointer;
	typedef MaskType::ConstPointer MaskConstPointer;


private:
	void DoInit()
	{
		SetName("Resampling");
		SetDescription("Resampling algo.");
		Loggers::GetInstance()->Initialize(GetName());
		GDALInit();
		// Documentation
		SetDocLongDescription("This application resample the image according to the dtm");
		SetDocLimitations("None");
		SetDocAuthors("MAJA-Team");
		SetDocSeeAlso("MAJA Doc");

		AddDocTag("Statistics");

		AddParameter(ParameterType_InputImage,  "dtm",   "DTM");
		SetParameterDescription("dtm", "Image used as reference grid");
		AddParameter(ParameterType_InputImage,  "im",   "image");
		SetParameterDescription("im", "Image to be resampled");
		AddParameter(ParameterType_Float,  "threshold", "Do threshold");
		SetParameterDescription("threshold", "Threshold the output image");
		MandatoryOff("threshold");

		AddParameter(ParameterType_Group,  "outareasize", "outareasize");
		SetParameterDescription("outareasize", "Size of the output image");
		MandatoryOff("outareasize");

		AddParameter(ParameterType_Int, "outareasize.x","outareasize.x" );
		SetParameterDescription("outareasize.x", "Size x");
		MandatoryOff("outareasize.x");

		AddParameter(ParameterType_Int, "outareasize.y","outareasize.y" );
		SetParameterDescription("outareasize.y", "Size y");
		MandatoryOff("outareasize.y");

		AddParameter(ParameterType_Float,  "padradius", "padradius");
		SetParameterDescription("padradius", "Radius to pad the input");
		MandatoryOff("padradius");


		AddParameter(ParameterType_Float,  "nodata", "nodata");
		SetParameterDescription("nodata", "nodata to use in CLA resampling");
		MandatoryOff("nodata");
		AddParameter(ParameterType_Choice, "interp", "Interpolator");
		AddChoice("interp.linear", "Linear Interpolation");
		AddChoice("interp.bco", "BCO Interpolation");
		AddChoice("interp.linearradius", "Linear with radius Interpolation");
		AddChoice("interp.fastlinear", "Fast Linear Interpolation");
		AddChoice("interp.cloudcla", "Cloud CLA special interpolation");
		AddChoice("interp.cloudclalt", "Cloud CLAlt special Interpolation");

		AddRAMParameter("ram");
		SetDefaultParameterInt("ram",2048);

		AddParameter(ParameterType_OutputImage, "out", "image");
		SetParameterOutputImagePixelType("out", ImagePixelType_double);
		SetParameterDescription("out","output image");

	}

	void DoUpdateParameters()
	{
	}

	template <typename TInputImage, typename TOutputImage>
	void DoResampleVector(TInputImage* in, const Area& l_outArea, const typename ResamplerHelper::InterpolatorType& l_interpolator,
			const unsigned int l_padradius, const ImagePixelType& out_thresh_type)
	{
		/** PadFilter used to manipulate real data */
		typedef PadAndResampleImageFilter<TInputImage, DoubleVectorImageType> PadAndResampleImageFilterType;
		typedef typename PadAndResampleImageFilterType::Pointer PadAndResampleImageFilterPointerType;

		typedef BinaryThresholdVectorImageFilter<DoubleVectorImageType, TOutputImage> BinaryThresholdVectorImageFilterType;
		typedef typename BinaryThresholdVectorImageFilterType::Pointer BinaryThresholdVectorImageFilterPointer;
		//Compute
		PadAndResampleImageFilterPointerType l_PadAndResampleFilter = PadAndResampleImageFilterType::New();
		l_PadAndResampleFilter->SetInput0(in);
		l_PadAndResampleFilter->SetInterpolator(l_interpolator);
		l_PadAndResampleFilter->SetReleaseDataFlag(true);
		l_PadAndResampleFilter->SetReleaseDataBeforeUpdateFlag(true);
		if(HasValue("outareasize.x") && HasValue("outareasize.y") )
		{
			vns::AreaType::SizeType l_outputSize;
			l_outputSize[0] = GetParameterInt("outareasize.x");
			l_outputSize[1] = GetParameterInt("outareasize.y");
			const AreaType l_area  = ResamplerHelper::ComputeResamplingCenteredImageAreaFromInputImageAreaAndSizeOutputImageCriteria(l_outArea, l_outputSize);
			l_PadAndResampleFilter->SetAreaToOutputImageResolution(l_area);
		}
		else
		{
			l_PadAndResampleFilter->SetAreaToOutputImageResolution(l_outArea);
		}

		l_PadAndResampleFilter->SetPadRadius(l_padradius);
		l_PadAndResampleFilter->UpdateWithCaching();
		l_PadAndResampleFilter->GetOutput()->UpdateOutputInformation();

		if(HasValue("threshold"))
		{
			BinaryThresholdVectorImageFilterPointer l_ThresholderMask = BinaryThresholdVectorImageFilterType::New();
			l_ThresholderMask->SetReleaseDataBeforeUpdateFlag(true);
			l_ThresholderMask->SetReleaseDataFlag(true);
			l_ThresholderMask->SetInput(l_PadAndResampleFilter->GetOutput());
			l_ThresholderMask->ThresholdAbove(GetParameterFloat("threshold"));
			l_ThresholderMask->SetInsideValue(0);
			l_ThresholderMask->SetOutsideValue(1);
			SetParameterOutputImagePixelType("out", out_thresh_type);
			SetParameterOutputImage<TOutputImage>("out",l_ThresholderMask->GetOutput());
			m_Thresholder = l_ThresholderMask;
			m_PadAndResampleFilter = l_PadAndResampleFilter;
		}
		else
		{
			SetParameterOutputImagePixelType("out", ImagePixelType_double);
			SetParameterOutputImage<DoubleVectorImageType>("out",l_PadAndResampleFilter->GetOutput());
			m_PadAndResampleFilter = l_PadAndResampleFilter;
		}
	}

	template <typename TInputImage, typename TOutputImage>
	void DoResample(TInputImage* in, const Area& l_outArea, const typename ResamplerHelper::InterpolatorType& l_interpolator,
			const unsigned int l_padradius, const ImagePixelType& out_thresh_type)
	{
		/** PadFilter used to manipulate real data */
		typedef PadAndResampleImageFilter<TInputImage, DoubleImageType> PadAndResampleImageFilterType;
		typedef typename PadAndResampleImageFilterType::Pointer PadAndResampleImageFilterPointerType;

		typedef itk::BinaryThresholdImageFilter<DoubleImageType, TOutputImage> BinaryThresholdImageFilterType;
		typedef typename BinaryThresholdImageFilterType::Pointer BinaryThresholdImageFilterPointer;
		//Compute
		PadAndResampleImageFilterPointerType l_PadAndResampleFilter = PadAndResampleImageFilterType::New();
		l_PadAndResampleFilter->SetInput0(in);
		l_PadAndResampleFilter->SetInterpolator(l_interpolator);
		l_PadAndResampleFilter->SetReleaseDataFlag(true);
		l_PadAndResampleFilter->SetReleaseDataBeforeUpdateFlag(true);
		if(HasValue("outareasize.x") && HasValue("outareasize.y") )
		{
			vns::AreaType::SizeType l_outputSize;
			l_outputSize[0] = GetParameterInt("outareasize.x");
			l_outputSize[1] = GetParameterInt("outareasize.y");
			const AreaType l_area  = ResamplerHelper::ComputeResamplingCenteredImageAreaFromInputImageAreaAndSizeOutputImageCriteria(l_outArea, l_outputSize);
			l_PadAndResampleFilter->SetAreaToOutputImageResolution(l_area);
		}
		else
		{
			l_PadAndResampleFilter->SetAreaToOutputImageResolution(l_outArea);
		}

		l_PadAndResampleFilter->SetPadRadius(l_padradius);
		l_PadAndResampleFilter->UpdateWithCaching();
		l_PadAndResampleFilter->GetOutput()->UpdateOutputInformation();

		if(HasValue("threshold"))
		{
			BinaryThresholdImageFilterPointer l_ThresholderMask = BinaryThresholdImageFilterType::New();
			l_ThresholderMask->SetReleaseDataBeforeUpdateFlag(true);
			l_ThresholderMask->SetReleaseDataFlag(true);
			l_ThresholderMask->SetInput(l_PadAndResampleFilter->GetOutput());
			l_ThresholderMask->SetUpperThreshold(GetParameterFloat("threshold"));
			l_ThresholderMask->SetInsideValue(0);
			l_ThresholderMask->SetOutsideValue(1);
			SetParameterOutputImagePixelType("out", out_thresh_type);
			SetParameterOutputImage<TOutputImage>("out",l_ThresholderMask->GetOutput());
			m_Thresholder = l_ThresholderMask;
			m_PadAndResampleFilter = l_PadAndResampleFilter;
		}
		else
		{
			SetParameterOutputImagePixelType("out", ImagePixelType_double);
			SetParameterOutputImage<DoubleImageType>("out",l_PadAndResampleFilter->GetOutput());
			m_PadAndResampleFilter = l_PadAndResampleFilter;
		}
	}

	template <typename TInputImage, typename TOutputImage>
		void DoThresholdVector(TInputImage* in, const ImagePixelType& out_type, const ImagePixelType& out_thresh_type)
	{
		if(HasValue("threshold"))
		{
			typedef BinaryThresholdVectorImageFilter<TInputImage, TOutputImage> BinaryThresholdVectorImageFilterType;
			typedef typename BinaryThresholdVectorImageFilterType::Pointer BinaryThresholdVectorImageFilterPointer;
			BinaryThresholdVectorImageFilterPointer l_VectorThresholder = BinaryThresholdVectorImageFilterType::New();
			l_VectorThresholder->SetInput(in);
			l_VectorThresholder->ThresholdAbove(GetParameterFloat("threshold"));
			l_VectorThresholder->SetInsideValue(0);
			l_VectorThresholder->SetOutsideValue(1);
			SetParameterOutputImagePixelType("out", out_thresh_type);
			SetParameterOutputImage<TOutputImage>("out",l_VectorThresholder->GetOutput());
			m_Thresholder = l_VectorThresholder;
		}
		else
		{
			SetParameterOutputImagePixelType("out", out_type);
			SetParameterOutputImage<TInputImage>("out", in);
		}
	}

	template <typename TInputImage, typename TOutputImage>
	void DoThreshold(TInputImage* in, const ImagePixelType& out_type, const ImagePixelType& out_thresh_type)
	{
		if(HasValue("threshold"))
		{
			typedef itk::BinaryThresholdImageFilter<TInputImage, TOutputImage> BinaryThresholdImageFilterType;
			typedef typename BinaryThresholdImageFilterType::Pointer BinaryThresholdImageFilterPointer;
			BinaryThresholdImageFilterPointer l_Thresholder = BinaryThresholdImageFilterType::New();
			l_Thresholder->SetInput(in);
			l_Thresholder->SetUpperThreshold(GetParameterFloat("threshold"));
			l_Thresholder->SetInsideValue(0);
			l_Thresholder->SetOutsideValue(1);
			SetParameterOutputImagePixelType("out", out_thresh_type);
			SetParameterOutputImage<TOutputImage>("out",l_Thresholder->GetOutput());
			m_Thresholder = l_Thresholder;
		}
		else
		{
			SetParameterOutputImagePixelType("out", out_type);
			SetParameterOutputImage<TInputImage>("out",in);
		}
	}


	void DoExecute()
	{
		vns::Loggers::GetInstance()->Initialize(GetName());
		// Init filters
		vnsLogDebugMacro("Number of threads : "<<itk::MultiThreader::GetGlobalDefaultNumberOfThreads())
		// Get input image pointers
		ImageBaseType* l_inPtr = GetParameterImageBase("im",0);
		// Guess the image type
		std::string className(l_inPtr->GetNameOfClass());
		l_inPtr->UpdateOutputInformation();
		bool l_isVec = true;

		ImagePointer l_dtmPtr = GetParameterDoubleImage("dtm");
		l_dtmPtr->UpdateOutputInformation();
		//Set Parameters
		unsigned int l_padradius = 0;
		if(HasValue("padradius"))
		{
			l_padradius = GetParameterInt("padradius");
		}

		Area l_outArea;
		l_outArea.Origin = l_dtmPtr->GetOrigin();
		l_outArea.Spacing = l_dtmPtr->GetSignedSpacing();
		l_outArea.Size = l_dtmPtr->GetLargestPossibleRegion().GetSize();


		//Choose interpolator
		ResamplerHelper::InterpolatorType l_interpolator;
		// Get Interpolator
		switch ( GetParameterInt("interp") )
		{
		case interp_linear:
			l_interpolator = ResamplerHelper::RB_INTERPOLATOR_LINEAR;
			break;
		case interp_bco:
			l_interpolator = ResamplerHelper::RB_INTERPOLATOR_BCO;
			break;
		case interp_linearradius:
			l_interpolator = ResamplerHelper::RB_INTERPOLATOR_LINEAR_WITH_RADIUS;
			break;
		case interp_fastlinear:
			l_interpolator = ResamplerHelper::RB_INTERPOLATOR_MAJA_FAST_LINEAR;
			break;
		case interp_cloudcla:
			l_interpolator = ResamplerHelper::RB_INTERPOLATOR_MAJA_CLOUD_UNDERSAMPLING_CLA;
			break;
		case interp_cloudclalt:
			l_interpolator = ResamplerHelper::RB_INTERPOLATOR_MAJA_CLOUD_UNDERSAMPLING_CLD_ALT;
			break;
		}



		const bool isSamePhysical = l_inPtr->GetOrigin().GetVnlVector().is_equal(l_dtmPtr->GetOrigin().GetVnlVector(), 1e-5) &&
				l_inPtr->GetLargestPossibleRegion() == l_dtmPtr->GetLargestPossibleRegion() &&
				l_inPtr->GetSpacing().GetVnlVector().is_equal(l_dtmPtr->GetSpacing().GetVnlVector(), 1e-5) &&
				l_inPtr->GetDirection().GetVnlMatrix().as_ref().is_equal(l_dtmPtr->GetDirection().GetVnlMatrix(), 1e-5);

		if( isSamePhysical && HasValue("outareasize.x")==false && HasValue("outareasize.y")==false)
		{
			vnsLogDebugMacro("No resampling needed");

			if (className == "VectorImage") {
				vnsLogDebugMacro("VectorImage");
				THRESHOLD_VECTOR_IMAGE_BASE(DoubleVectorImageType,UInt8VectorImageType,l_inPtr, ImagePixelType_double, ImagePixelType_uint8);
				THRESHOLD_VECTOR_IMAGE_BASE(FloatVectorImageType,UInt8VectorImageType,l_inPtr, ImagePixelType_float, ImagePixelType_uint8);
				THRESHOLD_VECTOR_IMAGE_BASE(UInt8VectorImageType, UInt8VectorImageType,l_inPtr, ImagePixelType_uint8, ImagePixelType_uint8);
				THRESHOLD_VECTOR_IMAGE_BASE(UInt16VectorImageType, UInt16VectorImageType,l_inPtr, ImagePixelType_uint16, ImagePixelType_uint16);
			} else {
				vnsLogDebugMacro("SingleImage");
				THRESHOLD_IMAGE_BASE(DoubleImageType,UInt8ImageType,l_inPtr, ImagePixelType_double, ImagePixelType_uint8);
				THRESHOLD_IMAGE_BASE(FloatImageType,UInt8ImageType,l_inPtr, ImagePixelType_float, ImagePixelType_uint8);
				THRESHOLD_IMAGE_BASE(UInt8ImageType, UInt8ImageType,l_inPtr, ImagePixelType_uint8, ImagePixelType_uint8);
				THRESHOLD_IMAGE_BASE(UInt16ImageType, UInt16ImageType,l_inPtr, ImagePixelType_uint16, ImagePixelType_uint16);
			}
		}
		else
		{
			vnsLogDebugMacro("Resampling needed");
			if (className == "VectorImage") {
				vnsLogDebugMacro("VectorImage");
				CAST_AND_RES_VECTOR_IMAGE_BASE(DoubleVectorImageType,UInt8VectorImageType,l_inPtr, ImagePixelType_uint8);
				CAST_AND_RES_VECTOR_IMAGE_BASE(FloatVectorImageType,UInt8VectorImageType,l_inPtr, ImagePixelType_uint8);
				CAST_AND_RES_VECTOR_IMAGE_BASE(UInt8VectorImageType, UInt8VectorImageType,l_inPtr, ImagePixelType_uint8);
				CAST_AND_RES_VECTOR_IMAGE_BASE(UInt16VectorImageType, UInt16VectorImageType,l_inPtr, ImagePixelType_uint16);
			} else {
				vnsLogDebugMacro("SingleImage");
				CAST_AND_RES_IMAGE_BASE(DoubleImageType,UInt8ImageType,l_inPtr, ImagePixelType_uint8);
				CAST_AND_RES_IMAGE_BASE(FloatImageType,UInt8ImageType,l_inPtr, ImagePixelType_uint8);
				CAST_AND_RES_IMAGE_BASE(UInt8ImageType, UInt8ImageType,l_inPtr, ImagePixelType_uint8);
				CAST_AND_RES_IMAGE_BASE(UInt16ImageType, UInt16ImageType,l_inPtr, ImagePixelType_uint16);
			}

			vnsExceptionDataMacro("Unsuported image type");
		}
	}


	/** Filters declaration */
	itk::ProcessObject::Pointer m_PadAndResampleFilter;
	itk::ProcessObject::Pointer m_Thresholder;

};

}
}

OTB_APPLICATION_EXPORT(vns::Wrapper::Resampling)
