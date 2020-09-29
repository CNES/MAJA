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
#include <string>
#include "vnsCloudMaskExtensionCorrelationGenerator.h"
#include "vnsTernaryLogicalOrFunctor.h"
#include "itkConnectedComponentImageFilter.h"
#include "vnsValidateMaskFromLabelMapAndValidityMapImageFilter.h"
#include "vnsTernaryLogicalOrFunctor.h"
#include "vnsStreamingThresholdFromLabelImageFilter.h"
#include "itkTernaryFunctorImageFilter.h"


namespace vns
{
namespace Wrapper
{

using namespace otb::Wrapper;

class CloudExtension : public Application
{
public:
	/** Standard class typedefs. */
	typedef CloudExtension           Self;
	typedef otb::Wrapper::Application     Superclass;
	typedef itk::SmartPointer<Self>       Pointer;
	typedef itk::SmartPointer<const Self> ConstPointer;

	/** Standard macro */
	itkNewMacro(Self);

	itkTypeMacro(CloudExtension, otb::Wrapper::Application);

	/** Some convenient typedefs. */
	//typedef TInputImage InputImageType;
	typedef DoubleVectorImageType InputImageType;
	typedef InputImageType::ConstPointer InputImageConstPointer;
	typedef InputImageType::Pointer InputImagePointer;
	typedef InputImageType::RegionType RegionType;
	typedef InputImageType::PixelType InputImagePixelType;
	typedef InputImageType::SizeType SizeType;
	typedef InputImageType::InternalPixelType InputInternalPixelType;
	//typedef TInputMask InputMaskType;
	typedef UInt8ImageType InputMaskType;
	typedef InputMaskType::Pointer InputMaskPointer;
	typedef InputMaskType::ConstPointer InputMaskConstPointer;
	typedef InputMaskType::PixelType InputMaskPixelType;
	//typedef TVectorMask VectorMaskType;
	typedef UInt8VectorImageType VectorMaskType;
	typedef VectorMaskType::Pointer VectorMaskPointer;
	typedef VectorMaskType::ConstPointer VectorMaskConstPointer;
	typedef VectorMaskType::PixelType VectorMaskPixelType;
	typedef VectorMaskType::InternalPixelType VectorMaskInternalPixelType;
	//typedef TDateImage DateImageType;
	typedef Int32ImageType DateImageType;
	typedef DateImageType::Pointer DateImagePointer;
	typedef DateImageType::ConstPointer DateImageConstPointer;
	typedef DateImageType::PixelType DateImagePixelType;

	// Label filter type
	typedef unsigned short LabelPixelType;
	typedef otb::Image<LabelPixelType, InputImageType::ImageDimension> LabelImageType;
	typedef LabelImageType::Pointer LabelImagePointer;
	typedef LabelImageType::PixelType LabelImagePixelType;

	typedef itk::ConnectedComponentImageFilter<InputMaskType, LabelImageType> LabelFilterType;
	typedef LabelFilterType::Pointer LabelPointerFilterType;

	// Ternary logical or funtor type
	typedef Functor::TernaryLogicalOrFunctor<InputMaskPixelType, InputMaskPixelType, InputMaskPixelType, InputMaskPixelType> TernaryLogicalFunctorType;
	typedef itk::TernaryFunctorImageFilter<InputMaskType, InputMaskType, InputMaskType, InputMaskType, TernaryLogicalFunctorType> TernaryLogicalFilterType;
	typedef TernaryLogicalFilterType::Pointer TernaryLogicalPointerFilterType;
	// Extension of clouds filter type
	typedef CloudMaskExtensionCorrelationGenerator<InputImageType, InputMaskType, LabelImageType, InputMaskType> CloudMaskExtensionFilterType;
	typedef CloudMaskExtensionFilterType::Pointer CloudMaskExtensionPointerFilterType;
	// Correlation discarded pixel post filtering
	typedef StreamingThresholdFromLabelImageFilter<InputMaskType, LabelImageType> DiscardedCloudPixelFilterType;
	typedef DiscardedCloudPixelFilterType::Pointer DiscardedCloudPixelFilterPointerType;

	// Extension of cloud PostProcessing
	typedef ValidateMaskFromLabelMapAndValidityMapImageFilter<InputMaskType, LabelImageType, InputMaskType> ValidateMaskFromLabelMapAndValidityMapImageFilterType;
	typedef ValidateMaskFromLabelMapAndValidityMapImageFilterType::Pointer ValidateMaskFromLabelMapAndValidityMapImageFilterPointerType;


private:
	void DoInit()
	{
		SetName("CloudExtension");
		SetDescription("Cloud Extension algo.");
		Loggers::GetInstance()->Initialize(GetName());
		// Documentation
		SetDocLongDescription("This application computes the cloud extension for the product");
		SetDocLimitations("None");
		SetDocAuthors("MAJA-Team");
		SetDocSeeAlso("MAJA Doc");

		AddDocTag("Statistics");
		//Input Images
		AddParameter(ParameterType_InputImage,  "tocr",   "tocr image");
		SetParameterDescription("tocr", "TOCR image at coarse all bands");
		AddParameter(ParameterType_InputImage,  "edg",   "EDG image");
		SetParameterDescription("edg", "Image used as edge at coarse");
		AddParameter(ParameterType_InputImage,  "cldext",   "EXT image");
		SetParameterDescription("cldext", "Image used as cld ext");
		AddParameter(ParameterType_InputImage,  "cldrefl",   "CLD Refl image");
		SetParameterDescription("cldrefl", "Image used as cldrefl");
		AddParameter(ParameterType_InputImage,  "cldreflvar",   "CLD ReflVar image");
		SetParameterDescription("cldreflvar", "Image used as refl var at coarse");

		//Previous L2
		AddParameter(ParameterType_InputImage,  "l2rcr",   "L2 RCR");
		SetParameterDescription("l2rcr", "L2 RCR");
		AddParameter(ParameterType_InputImage,  "l2sto",   "L2 STO");
		SetParameterDescription("l2sto", "L2 STO");
		AddParameter(ParameterType_InputImage,  "l2ndt",   "L2 NDT");
		SetParameterDescription("l2ndt", "L2 NDT");


		//Set parameters
		// cirrus flag parameter accessors
		AddParameter(ParameterType_Float,  "nodata","NoData");
		AddParameter(ParameterType_Int,  "correlbandtocr","CorrelBandTOCR");
		AddParameter(ParameterType_Int,  "correlbandrcr","CorrelBandRCR");
		AddParameter(ParameterType_Int,  "jday","JDay");
		AddParameter(ParameterType_Float,  "minpixelcorrelation","MinPixelCorrelation");
		AddParameter(ParameterType_Float,  "correlthreshold","CorrelThreshold");
		AddParameter(ParameterType_Float,  "ncorrel","NCorrel");
		AddParameter(ParameterType_Float,  "discardedratiomin","discardedratiomin");
		AddParameter(ParameterType_StringList, "stolistofdates","STOListOfDates");


		AddParameter(ParameterType_OutputImage, "ext", "Extensio");
		SetParameterDescription("ext","Cloud Extension");
		SetParameterOutputImagePixelType("ext", ImagePixelType_uint8);

		AddRAMParameter("ram");
		SetDefaultParameterInt("ram",2048);

	}

	void DoUpdateParameters()
	{

	}


	void DoExecute()
	{
		// Filter initialization
		// Label filter
		m_IPCLDExtLabelFilter = LabelFilterType::New();
		m_CloudMaskExtensionFilter = CloudMaskExtensionFilterType::New();
		// DM 982
		m_TmpCloudSynthFilter = TernaryLogicalFilterType::New();
		m_TmpCloudLabelFilter = LabelFilterType::New();
		// CLD pixel discarding filter
		m_CloudMaskExtensionDiscardedPixelValidationFilter = DiscardedCloudPixelFilterType::New();
		m_CloudMaskExtensionPostProcessingFilter = ValidateMaskFromLabelMapAndValidityMapImageFilterType::New();

		vnsLogDebugMacro("CloudExtension start");
		//Constant gen ask
		// Load inputs
		InputImageConstPointer l_IPTOCRPtr = this->GetParameterDoubleVectorImage("tocr");
		InputMaskConstPointer l_IPEDGPtr =  this->GetParameterUInt8Image("edg");
		InputImageConstPointer l_L2inRCRPtr = this->GetParameterDoubleVectorImage("l2rcr");
		InputImageConstPointer l_L2inSTOPtr = this->GetParameterDoubleVectorImage("l2sto");
		InputMaskConstPointer l_L2inNDTPtr = this->GetParameterUInt8Image("l2ndt");
		InputMaskConstPointer l_IPCLDExt = this->GetParameterUInt8Image("cldext");
		InputMaskConstPointer l_IPCLDRefl = this->GetParameterUInt8Image("cldrefl");
		InputMaskConstPointer l_IPCLDReflVar = this->GetParameterUInt8Image("cldreflvar");
		//Get parameters
		const double l_NoData = GetParameterFloat("nodata");
		const unsigned int l_CorrelBandTOCR = GetParameterInt("correlbandtocr");
		//Non init optional
		unsigned int l_CorrelBandRCR= GetParameterInt("correlbandrcr");
		unsigned int l_JDay = GetParameterInt("jday");
		Utilities::ListOfDoubles l_STOListOfDates;
		double l_MinPixelCorrelation= GetParameterFloat("minpixelcorrelation");
		double l_CorrelThreshold= GetParameterFloat("correlthreshold");
		double l_NCorrel = GetParameterFloat("ncorrel");
		const std::vector<std::string> l_ListOfDatesStr = GetParameterStringList("stolistofdates");
		for (unsigned int i = 0; i < l_ListOfDatesStr.size();i++)
		{
			l_STOListOfDates.push_back(Utilities::AsDouble(l_ListOfDatesStr[i]));
		}
		double l_DiscardedPixelsRatioMin = GetParameterFloat("discardedratiomin");

		// Label the mask of the extension of clouds
		m_IPCLDExtLabelFilter->SetInput(l_IPCLDExt);
		m_IPCLDExtLabelFilter->Update();
		// Cloud mask extension test
		// Input images
		m_CloudMaskExtensionFilter->SetIPTOCRInput(l_IPTOCRPtr);
		m_CloudMaskExtensionFilter->SetL2inRCRInput(l_L2inRCRPtr);
		m_CloudMaskExtensionFilter->SetL2inSTOInput(l_L2inSTOPtr);
		m_CloudMaskExtensionFilter->SetIPCLDReflInput(l_IPCLDRefl); //m_ReflectanceThresholdFilterOutputReader->GetOutput());
		m_CloudMaskExtensionFilter->SetIPCLDReflVarInput(l_IPCLDReflVar);
		m_CloudMaskExtensionFilter->SetIPCLDExtInput(l_IPCLDExt);
		m_CloudMaskExtensionFilter->SetIPCLDExtLabelInput(m_IPCLDExtLabelFilter->GetOutput());
		m_CloudMaskExtensionFilter->SetIPEDGInput(l_IPEDGPtr);
		m_CloudMaskExtensionFilter->SetL2inNDTInput(l_L2inNDTPtr);

		m_CloudMaskExtensionFilter->SetLabelCount(m_IPCLDExtLabelFilter->GetObjectCount());
		m_CloudMaskExtensionFilter->SetNoData(l_NoData);
		// 4-1-0 : sensor mixing
		m_CloudMaskExtensionFilter->SetCorrelBandTOCR(l_CorrelBandTOCR);
		m_CloudMaskExtensionFilter->SetCorrelBandRCR(l_CorrelBandRCR);
		m_CloudMaskExtensionFilter->SetMinPixelCorrelation(l_MinPixelCorrelation);
		m_CloudMaskExtensionFilter->SetCorrelThreshold(l_CorrelThreshold);
		typedef itk::Size<InputImageType::ImageDimension> RadiusSizeType;
		RadiusSizeType l_Correl;
		l_Correl.Fill(l_NCorrel);
		m_CloudMaskExtensionFilter->SetNCorrel(l_Correl);
		// LAIG-FA-MAC-1180-CNES : no correlation if the current date is equal to the STO one
		m_CloudMaskExtensionFilter->SetJDay(l_JDay);
		m_CloudMaskExtensionFilter->SetSTOListOfDates(l_STOListOfDates);
		m_CloudMaskExtensionFilter->UpdateWithCaching();

		// Correlation test post-processing (5.0.0-DM982)
		// -> TernaryLogicalFilter (refl || reflvar || ext): total cloud mask
		m_TmpCloudSynthFilter->SetInput1(l_IPCLDRefl); //m_ReflectanceThresholdFilterOutputReader->GetOutput());
		m_TmpCloudSynthFilter->SetInput2(l_IPCLDReflVar);
		m_TmpCloudSynthFilter->SetInput3(m_CloudMaskExtensionFilter->GetIPCLDExtOutput());
		// -> Total cloud mask to label map
		m_TmpCloudLabelFilter->SetInput(m_TmpCloudSynthFilter->GetOutput());
		// Compute statistics over labels
		m_CloudMaskExtensionDiscardedPixelValidationFilter->SetInput(m_CloudMaskExtensionFilter->GetIPCLDExtCorelFlagOutput());
		m_CloudMaskExtensionDiscardedPixelValidationFilter->SetInputLabelImage(m_TmpCloudLabelFilter->GetOutput());
		m_CloudMaskExtensionDiscardedPixelValidationFilter->SetLowerValidityThreshold(l_DiscardedPixelsRatioMin);
		m_CloudMaskExtensionDiscardedPixelValidationFilter->Update();
		// Filter Potentially discarded pixels (from Correlation in ext step)
		m_CloudMaskExtensionPostProcessingFilter->SetMaskInput(m_CloudMaskExtensionFilter->GetIPCLDExtOutput());
		m_CloudMaskExtensionPostProcessingFilter->SetExtCorrelFlagInput(m_CloudMaskExtensionFilter->GetIPCLDExtCorelFlagOutput());
		m_CloudMaskExtensionPostProcessingFilter->SetLabelInput(m_TmpCloudLabelFilter->GetOutput());
		m_CloudMaskExtensionPostProcessingFilter->SetValidityMap(
				m_CloudMaskExtensionDiscardedPixelValidationFilter->GetLabelValidityStatusMap());

		SetParameterOutputImage<InputMaskType>("ext",m_CloudMaskExtensionPostProcessingFilter->GetOutput());


	}


	/** Filters declaration */
	LabelPointerFilterType m_IPCLDExtLabelFilter;
	LabelPointerFilterType m_LabelFilter;
	DiscardedCloudPixelFilterPointerType m_CloudMaskExtensionDiscardedPixelValidationFilter;
	ValidateMaskFromLabelMapAndValidityMapImageFilterPointerType m_CloudMaskExtensionPostProcessingFilter;
	TernaryLogicalPointerFilterType m_TmpCloudSynthFilter;
	LabelPointerFilterType m_TmpCloudLabelFilter;
	CloudMaskExtensionPointerFilterType m_CloudMaskExtensionFilter;
};

}
}

OTB_APPLICATION_EXPORT(vns::Wrapper::CloudExtension)
