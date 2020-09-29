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
#include "otbMultiToMonoChannelExtractROI.h"
#include "itkUnaryFunctorImageFilter.h"
#include "vnsSupOrEqualThresholdImageFunctor.h"
#include "itkBinaryFunctorImageFilter.h"
#include "itkBinaryDilateImageFilter.h"
#include "itkBinaryBallStructuringElement.h"
#include "vnsStreamingConditionalStatisticsImageFilter.h"
#include "vnsGapFillingImageFilter.h"
#include "vnsMaskingImageFunctor.h"

namespace vns
{
namespace Wrapper
{

using namespace otb::Wrapper;

class WaterVaporPostPro : public Application
{
public:
	/** Standard class typedefs. */
	typedef WaterVaporPostPro             Self;
	typedef otb::Wrapper::Application     Superclass;
	typedef itk::SmartPointer<Self>       Pointer;
	typedef itk::SmartPointer<const Self> ConstPointer;

	/** Standard macro */
	itkNewMacro(Self);

	itkTypeMacro(WaterVaporPostPro, otb::Wrapper::Application);

	/** Some convenient typedefs. */
	/** IPTOCR input image typedefs. */
	typedef VNSRealVectorImageType IPTOCRImageType;
	typedef IPTOCRImageType::ConstPointer IPTOCRImageConstPointerType;
	typedef IPTOCRImageType::RegionType RegionType;
	typedef IPTOCRImageType::PixelType IPTOCRImagePixelType;
	typedef IPTOCRImageType::InternalPixelType IPTOCRImageInternalPixelType;
	/** DTM input image typedefs. */
	typedef VNSDTMImageType DTMImageType;
	typedef otb::ObjectList<DTMImageType> DTMImageListType;
	typedef DTMImageListType::ConstPointer DTMImageListConstPointerType;
	typedef DTMImageType::ConstPointer DTMImageConstPointerType;
	/** Vapor amount image typedefs. */
	typedef VNSRealImageType VAPImageType;
	typedef otb::ObjectList<VAPImageType> VAPImageListType;
	typedef VAPImageListType::Pointer VAPImageListPointerType;
	typedef VAPImageType::Pointer VAPImagePointerType;
	typedef VAPImageType::ConstPointer VAPImageConstPointerType;
	typedef VAPImageType::PixelType VAPImagePixelType;
	typedef VAPImageType::InternalPixelType VAPImageInternalPixelType;
	/** CLD Mask typedefs. */
	typedef VNSMaskImageType MaskImageType;
	typedef otb::ObjectList<MaskImageType> MaskImageListType;
	typedef MaskImageListType::Pointer MaskImageListPointerType;
	typedef MaskImageType::Pointer MaskImagePointerType;
	typedef MaskImageType::ConstPointer MaskImageConstPointerType;
	typedef MaskImageType::RegionType MaskImageRegionType;
	typedef MaskImageType::PixelType MaskImagePixelType;
	typedef MaskImageType::InternalPixelType MaskImageInternalPixelType;

	/** Filters typedefs. */
	typedef otb::MultiToMonoChannelExtractROI<IPTOCRImageInternalPixelType, IPTOCRImageInternalPixelType> MultiToMonoChannelExtractFilterType;
	typedef MultiToMonoChannelExtractFilterType::Pointer MultiToMonoChannelExtractFilterPointerType;
	typedef MultiToMonoChannelExtractFilterType::OutputImageType MonoChannelImageType;
	typedef MonoChannelImageType::PixelType MonoChannelImagePixelType;

	typedef Functor::SupOrEqualThresholdImageFunctor<MonoChannelImagePixelType, MaskImageInternalPixelType> ThresholdFunctorType;
	typedef itk::UnaryFunctorImageFilter<MonoChannelImageType, MaskImageType, ThresholdFunctorType> ThresholdFilterType;
	typedef ThresholdFilterType::Pointer ThresholdFilterPointerType;

	typedef Functor::MaskingImageFunctor<MaskImagePixelType, MaskImagePixelType, MaskImagePixelType> ApplyMaskFunctorType;
	typedef itk::BinaryFunctorImageFilter<MaskImageType, MaskImageType, MaskImageType, ApplyMaskFunctorType> ApplyMaskFilterType;
	typedef ApplyMaskFilterType::Pointer ApplyMaskFilterPointerType;

	typedef itk::BinaryBallStructuringElement<MaskImagePixelType, MaskImageType::ImageDimension> StructuringElementType;
	typedef itk::BinaryDilateImageFilter<MaskImageType, MaskImageType, StructuringElementType> DilateFilterType;
	typedef DilateFilterType::Pointer DilateFilterPointerType;

	typedef Functor::MaskingImageFunctor<VAPImagePixelType, MaskImagePixelType, VAPImagePixelType> VAPMaskingFunctorType;
	typedef itk::BinaryFunctorImageFilter<VAPImageType, MaskImageType, VAPImageType, VAPMaskingFunctorType> VAPMaskingFilterType;
	typedef VAPMaskingFilterType::Pointer VAPMaskingFilterPointerType;

	typedef StreamingConditionalStatisticsImageFilter<VAPImageType> StreamingConditionalStatisticsImageFilterType;
	typedef StreamingConditionalStatisticsImageFilterType::Pointer StreamingConditionalStatisticsImageFilterPointerType;

	typedef GapFillingImageFilter<VAPImageType, VAPImageType, MaskImageType> GapFillingImageFilterType;
	typedef GapFillingImageFilterType::Pointer GapFillingImageFilterPointerType;

private:
	void DoInit()
	{
		SetName("WaterVaporPostPro");
		SetDescription("WaterVaporPostPro algo.");
		Loggers::GetInstance()->Initialize(GetName());
		// Documentation
		SetDocLongDescription("This application computes the WaterVaporPostPro");
		SetDocLimitations("None");
		SetDocAuthors("MAJA-Team");
		SetDocSeeAlso("MAJA Doc");

		AddDocTag("Statistics");

		AddParameter(ParameterType_InputImage,  "tocr",   "tocr image");
		SetParameterDescription("tocr", "TOCR image at coarse");
		AddParameter(ParameterType_InputImage,  "vapin",   "VAP image");
		SetParameterDescription("vapin", "Image used as VAP");
		AddParameter(ParameterType_InputImage,  "cldsum",   "CLD SUM image");
		SetParameterDescription("cldsum", "Image used as CLD SUM");

		AddParameter(ParameterType_Float,  "defaultwatervapor","defaultwatervapor");
		AddParameter(ParameterType_Float,  "threshold","threshold");
		AddParameter(ParameterType_Float,  "thresholdref","thresholdref");
		AddParameter(ParameterType_Int,  "referencebandcode","referencebandcode");
		AddParameter(ParameterType_Int,  "bandcode","bandcode");
		AddParameter(ParameterType_Float,  "nodata","nodata");
		AddParameter(ParameterType_Int,  "sevalidradius","sevalidradius");
		AddParameter(ParameterType_Int,  "initwindowradius","initwindowradius");
		AddParameter(ParameterType_Int,  "maxwindowradius","maxwindowradius");


		AddParameter(ParameterType_OutputImage, "mask", "gap filling mask");
		SetParameterDescription("mask","gap filling mask");
		SetParameterOutputImagePixelType("mask", ImagePixelType_uint8);
		AddParameter(ParameterType_OutputImage, "vap", "vap image");
		SetParameterDescription("vap","vap image");
		SetParameterOutputImagePixelType("vap", ImagePixelType_double);

		AddRAMParameter("ram");
		SetDefaultParameterInt("ram",2048);

	}

	void DoUpdateParameters()
	{

	}


	void DoExecute()
	{
		itk::MultiThreader::SetGlobalDefaultNumberOfThreads(8);

		// Init filters
		m_MultiToMonoChannelExtractFilterRef = MultiToMonoChannelExtractFilterType::New();
		m_ThresholdFilterRef = ThresholdFilterType::New();
		m_MultiToMonoChannelExtractFilter = MultiToMonoChannelExtractFilterType::New();
		m_ThresholdFilter = ThresholdFilterType::New();
		m_VapourApplyMaskFilter = ApplyMaskFilterType::New();
		m_CLDSumApplyMaskFilter = ApplyMaskFilterType::New();
		m_DilateFilter = DilateFilterType::New();
		m_VAPMaskingFilter = VAPMaskingFilterType::New();
		// Init filter streaming conditional statistics
		m_StreamingConditionalStatisticsImageFilter = StreamingConditionalStatisticsImageFilterType::New();
		// Init gap filing filter
		m_GapFillingImageFilter = GapFillingImageFilterType::New();

		// Get input image pointers
		IPTOCRImageConstPointerType l_IPTOCRSub = GetParameterDoubleVectorImage("tocr");
		VAPImageConstPointerType l_VAPSub = GetParameterDoubleImage("vapin");
		MaskImageConstPointerType l_CLDSub = GetParameterUInt8Image("cldsum");
		//Get Params
		const double m_DefaultWaterVapor = GetParameterFloat("defaultwatervapor");
		const double m_WaterVaporReflectanceThreshold =  GetParameterFloat("threshold");
		const double m_WaterVaporReflectanceThresholdRef =  GetParameterFloat("thresholdref");
		const unsigned int m_WaterVaporReferenceBandCode = GetParameterInt("referencebandcode");
		const unsigned int m_WaterVaporBandCode = GetParameterInt("bandcode");
		const double m_NoData =  GetParameterFloat("nodata");
		const unsigned int m_WaterVaporSEValidRadius = GetParameterInt("sevalidradius");
		const unsigned int m_WaterVaporInitWindowRadius = GetParameterInt("initwindowradius");
		const unsigned int m_WaterVaporMaxWindowRadius = GetParameterInt("maxwindowradius");
		//Process
		// Check Inputs
		if (l_IPTOCRSub.IsNull() == true)
		{
			vnsExceptionBusinessMacro("TOCR input at L2 coarse is null...");
		}
		if (l_VAPSub.IsNull() == true)
		{
			vnsExceptionBusinessMacro("VAP input at L2 coarse is null...");
		}
		if (l_CLDSub.IsNull() == true)
		{
			vnsExceptionBusinessMacro("CLD input is null...");
		}

		// Extract Ref Band
		m_MultiToMonoChannelExtractFilterRef->SetInput(l_IPTOCRSub);
		// In the MultiToMonoChannelExtract filter, the first band index is 1 and not 0
		m_MultiToMonoChannelExtractFilterRef->SetChannel(m_WaterVaporReferenceBandCode + 1);
		// Binary Threshold on ref band
		m_ThresholdFilterRef->SetInput(m_MultiToMonoChannelExtractFilterRef->GetOutput());
		m_ThresholdFilterRef->GetFunctor().SetThresholdValue(m_WaterVaporReflectanceThresholdRef);
		m_ThresholdFilterRef->GetFunctor().SetInsideValue(1);
		m_ThresholdFilterRef->GetFunctor().SetOutputValue(0);

		// Extract vapour Band
		m_MultiToMonoChannelExtractFilter->SetInput(l_IPTOCRSub);
		// In the MultiToMonoChannelExtract filter, the first band index is 1 and not 0
		m_MultiToMonoChannelExtractFilter->SetChannel(m_WaterVaporBandCode + 1);
		// Binary Threshold on ref band
		m_ThresholdFilter->SetInput(m_MultiToMonoChannelExtractFilter->GetOutput());
		m_ThresholdFilter->GetFunctor().SetThresholdValue(m_WaterVaporReflectanceThreshold);
		m_ThresholdFilter->GetFunctor().SetInsideValue(1);
		m_ThresholdFilter->GetFunctor().SetOutputValue(0);
		//Apply vapour on ref binary
		m_VapourApplyMaskFilter->SetInput1(m_ThresholdFilterRef->GetOutput());
		m_VapourApplyMaskFilter->SetInput2(m_ThresholdFilter->GetOutput());
		m_VapourApplyMaskFilter->GetFunctor().SetReplaceValue(static_cast<MaskImagePixelType>(1));
		m_VapourApplyMaskFilter->GetFunctor().SetBackgroundValue(static_cast<MaskImagePixelType>(1));

		// Apply CLD
		m_CLDSumApplyMaskFilter->SetInput1(m_VapourApplyMaskFilter->GetOutput());
		m_CLDSumApplyMaskFilter->SetInput2(l_CLDSub);
		m_CLDSumApplyMaskFilter->GetFunctor().SetReplaceValue(static_cast<MaskImagePixelType>(1));
		m_CLDSumApplyMaskFilter->GetFunctor().SetBackgroundValue(static_cast<MaskImagePixelType>(1));

		// Dilate
		MaskImagePixelType l_DilateValue(1);
		StructuringElementType l_SECloud;
		l_SECloud.SetRadius(m_WaterVaporSEValidRadius); // 3x3 structuring element
		l_SECloud.CreateStructuringElement();
		m_DilateFilter->SetKernel(l_SECloud);
		m_DilateFilter->SetInput(m_CLDSumApplyMaskFilter->GetOutput());
		m_DilateFilter->SetDilateValue(l_DilateValue);

		// Patch input VAPSub with NoData
		m_VAPMaskingFilter->SetInput1(l_VAPSub);
		m_VAPMaskingFilter->SetInput2(m_DilateFilter->GetOutput());
		m_VAPMaskingFilter->GetFunctor().SetBackgroundValue(static_cast<MaskImagePixelType>(1));
		m_VAPMaskingFilter->GetFunctor().SetReplaceValue(static_cast<VAPImagePixelType>(m_NoData));

		// VAP Statistics estimation
		m_StreamingConditionalStatisticsImageFilter->SetInput(m_VAPMaskingFilter->GetOutput());
		m_StreamingConditionalStatisticsImageFilter->SetExcludedValue(m_NoData);
		m_StreamingConditionalStatisticsImageFilter->SetEnableVariance(false);
		m_StreamingConditionalStatisticsImageFilter->Update();
		const bool l_HasValidPixel = (m_StreamingConditionalStatisticsImageFilter->GetNbOfValidValues() != 0);

		vnsLogDebugMacro("HasValidPixel used in VAP Gap Filling: " << l_HasValidPixel);
		vnsLogDebugMacro("Mean Value used in VAP Gap Filling: " << m_StreamingConditionalStatisticsImageFilter->GetMean());

		// Gap Filling filter
		m_GapFillingImageFilter->SetInputImage(m_VAPMaskingFilter->GetOutput());
		m_GapFillingImageFilter->SetNoData(m_NoData);
		m_GapFillingImageFilter->SetDefaultValue(m_DefaultWaterVapor);
		m_GapFillingImageFilter->SetMeanValue(m_StreamingConditionalStatisticsImageFilter->GetMean());
		m_GapFillingImageFilter->SetHasValidPixel(l_HasValidPixel);
		m_GapFillingImageFilter->SetInitWindowRadius(m_WaterVaporInitWindowRadius);
		m_GapFillingImageFilter->SetMaxWindowRadius(m_WaterVaporMaxWindowRadius);

		SetParameterOutputImage<VAPImageType>("vap",m_GapFillingImageFilter->GetOutputFilledImage());
		SetParameterOutputImage<MaskImageType>("mask",m_GapFillingImageFilter->GetOutputMask());
	}


	/** Filters declaration */
	MultiToMonoChannelExtractFilterPointerType m_MultiToMonoChannelExtractFilterRef;
	ThresholdFilterPointerType m_ThresholdFilterRef;
	MultiToMonoChannelExtractFilterPointerType m_MultiToMonoChannelExtractFilter;
	ThresholdFilterPointerType m_ThresholdFilter;
	ApplyMaskFilterPointerType m_VapourApplyMaskFilter;
	ApplyMaskFilterPointerType m_CLDSumApplyMaskFilter;
	DilateFilterPointerType m_DilateFilter;
	VAPMaskingFilterPointerType m_VAPMaskingFilter;
	StreamingConditionalStatisticsImageFilterPointerType m_StreamingConditionalStatisticsImageFilter;
	GapFillingImageFilterPointerType m_GapFillingImageFilter;

};

}
}


OTB_APPLICATION_EXPORT(vns::Wrapper::WaterVaporPostPro)
