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
#include "vnsShadowVariationThresholdValueEstimator.h"
#include "vnsMaskingImageFunctor.h"
#include "vnsShadowVariationThresholdImageFilter.h"
#include "vnsShadowVariationPixelValidationImageFilter.h"
#include "vnsShadowVariationCorrelationImageFilter.h"
#include "vnsOutsideCloudsShadowMaskGenerator.h"
#include "otbImageToVectorImageCastFilter.h"
#include "itkConnectedComponentImageFilter.h"
#include "vnsConditionalRatioImageFilter.h"
#include "vnsStreamingThresholdFromLabelImageFilter.h"
#include "vnsValidateMaskFromLabelMapAndValidityMapImageFilter.h"

namespace vns
{

namespace Wrapper
{

using namespace otb::Wrapper;

class CloudShadVar : public Application
{
public:
	/** Standard class typedefs. */
	typedef CloudShadVar           Self;
	typedef otb::Wrapper::Application     Superclass;
	typedef itk::SmartPointer<Self>       Pointer;
	typedef itk::SmartPointer<const Self> ConstPointer;

	/** Standard macro */
	itkNewMacro(Self);

	itkTypeMacro(CloudShadVar, otb::Wrapper::Application);

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

	//Label filter
	typedef unsigned short LabelPixelType;
	typedef otb::Image<LabelPixelType, InputImageType::ImageDimension> LabelImageType;
	typedef LabelImageType::Pointer LabelImagePointer;
	typedef LabelImageType::PixelType LabelImagePixelType;
	typedef itk::ConnectedComponentImageFilter<InputMaskType, LabelImageType> LabelFilterType;
	typedef LabelFilterType::Pointer LabelPointerFilterType;
	//typedef TCLAImage CLAImageType;
	typedef VNSRealImageType CLAImageType;
	typedef CLAImageType::Pointer CLAImagePointer;
	typedef CLAImageType::ConstPointer CLAImageConstPointer;
	typedef CLAImageType::PixelType CLAImagePixelType;
	//typedef TGRIDImage GRIDImageType;
	typedef VNSRealVectorImageType GRIDImageType;
	typedef GRIDImageType::Pointer GRIDImagePointer;
	typedef GRIDImageType::ConstPointer GRIDImageConstPointer;
	typedef GRIDImageType::PixelType GRIDImagePixelType;
	typedef GRIDImageType::IndexType IndexType;
	typedef GRIDImageType::PointType PointType;
	typedef GRIDImageType::SpacingType SpacingType;
	//typedef TDTMImage DTMImageType;
	typedef VNSDTMImageType DTMImageType;
	typedef DTMImageType::Pointer DTMImagePointer;
	typedef DTMImageType::ConstPointer DTMImageConstPointer;
	typedef DTMImageType::PixelType DTMImagePixelType;

	// Shadow from clouds outside the image
	typedef OutsideCloudsShadowMaskGenerator<InputMaskType, DTMImageType, GRIDImageType, InputMaskType> OutsideCloudsShadowMaskFilterType;
	typedef OutsideCloudsShadowMaskFilterType::Pointer OutsideCloudsShadowMaskPointerFilterType;

	// Shadow variation threshold
	typedef otb::Image<double, 2> RatioImageType;
	typedef RatioImageType::Pointer RatioImagePointer;
	typedef ConditionalRatioImageFilter<InputImageType, InputMaskType, RatioImageType> RatioFilterType;
	typedef RatioFilterType::Pointer RatioFilterPointerType;
	typedef Functor::MaskingImageFunctor<double, InputMaskPixelType, double> RatioMaskingFunctorType;
	typedef itk::BinaryFunctorImageFilter<RatioImageType, InputMaskType, RatioImageType, RatioMaskingFunctorType> ReflRatioMaskingFilterType;
	typedef ReflRatioMaskingFilterType::Pointer ReflRatioMaskingFilterPointerType;
	typedef otb::ImageFileReader<RatioImageType> ReaderRatioImageType;
	typedef ReaderRatioImageType::Pointer ReaderRatioPointer;
	typedef otb::ImageToVectorImageCastFilter<RatioImageType, InputImageType> RatioImageListToVectorImageFilterType;
	typedef RatioImageListToVectorImageFilterType::Pointer RatioImageListToVectorImageFilterPointerType;
	typedef ShadowVariationThresholdValueEstimator<InputImageType> ShadowVariationThresholdValueEstimatorFilterType;
	typedef ShadowVariationThresholdValueEstimatorFilterType::Pointer ShadowVariationThresholdValueEstimatorFilterPointerType;
	typedef ShadowVariationThresholdImageFilter<RatioImageType, InputMaskType, InputMaskType> ShadowVariationThresholdFilterType;
	typedef ShadowVariationThresholdFilterType::Pointer ShadowVariationThresholdPointerFilterType;
	typedef ShadowVariationPixelValidationImageFilter<InputMaskType, LabelImageType> ShadowVariationPixelValidationImageFilterType;
	typedef ShadowVariationPixelValidationImageFilterType::Pointer ShadowVariationPixelValidationImageFilterPointerType;
	typedef StreamingThresholdFromLabelImageFilter<InputMaskType, LabelImageType> DiscardedCloudPixelFilterType;
	typedef DiscardedCloudPixelFilterType::Pointer DiscardedCloudPixelFilterPointerType;
	typedef ShadowVariationCorrelationImageFilter<InputImageType, InputMaskType, InputMaskType> ShadowVariationCorrelationImageFilterType;
	typedef ShadowVariationCorrelationImageFilterType::Pointer ShadowVariationCorrelationImageFilterPointerType;
    typedef Functor::MaskingImageFunctor<InputMaskPixelType, InputMaskPixelType, InputMaskPixelType> MaskMaskingFunctorType;
    typedef itk::BinaryFunctorImageFilter<InputMaskType, InputMaskType, InputMaskType, MaskMaskingFunctorType> MaskMaskingFilterType;
    typedef MaskMaskingFilterType::Pointer MaskMaskingFilterPointerType;
    typedef ValidateMaskFromLabelMapAndValidityMapImageFilter<InputMaskType, LabelImageType, InputMaskType> ValidateMaskFromLabelMapAndValidityMapImageFilterType;
    typedef ValidateMaskFromLabelMapAndValidityMapImageFilterType::Pointer ValidateMaskFromLabelMapAndValidityMapImageFilterPointerType;



private:
	void DoInit()
	{
		SetName("CloudShadVar");
		SetDescription("Cloud shadvar algo.");
		Loggers::GetInstance()->Initialize(GetName());
		// Documentation
		SetDocLongDescription("This application computes the cloud shadow variation mask");
		SetDocLimitations("None");
		SetDocAuthors("MAJA-Team");
		SetDocSeeAlso("MAJA Doc");

		AddDocTag("Statistics");
		//Input Images
		AddParameter(ParameterType_InputImage,  "tocr",   "tocr image");
		SetParameterDescription("tocr", "TOCR image at coarse all bands");
		AddParameter(ParameterType_InputImage,  "edg",   "EDG image");
		SetParameterDescription("edg", "Image used as edge at coarse");
		AddParameter(ParameterType_InputImage,  "cldall",   "CLD all image");
		SetParameterDescription("cldall", "Image used as cld all no ext");
		AddParameter(ParameterType_InputImage,  "cldshad",   "CLD shadow");
		SetParameterDescription("cldshad", "Image used as cld shadow");
		AddParameter(ParameterType_InputImage,  "vie",   "VIE");
		SetParameterDescription("vie", "Viewing angle image at coarse");
		AddParameter(ParameterType_InputImage,  "dtm",   "dtm image");
		SetParameterDescription("dtm", "dtm of the image");
		AddParameter(ParameterType_InputImage,  "sol",   "SOLAR angles image");
		SetParameterDescription("sol", "Image used as solar angle");
		//Previous L2
		AddParameter(ParameterType_InputImage,  "l2rcr",   "L2 RCR");
		SetParameterDescription("l2rcr", "L2 RCR");
		AddParameter(ParameterType_InputImage,  "l2was",   "L2 WAS");
		SetParameterDescription("l2was", "L2 WAS");
		AddParameter(ParameterType_InputImage,  "l2ndt",   "L2 NDT");
		SetParameterDescription("l2ndt", "L2 NDT");
		AddParameter(ParameterType_InputImage,  "l2shad",   "L2 Shadow");
		SetParameterDescription("l2shad", "L2 Shadow");
		AddParameter(ParameterType_InputImage,  "l2shadvar",   "L2 Shadow Variation");
		SetParameterDescription("l2shadvar", "L2 Shadow Variation");
		AddParameter(ParameterType_InputImage,  "l2sto",   "L2 STO");
		SetParameterDescription("l2sto", "L2 STO");
		//Set parameters
		AddParameter(ParameterType_Bool,  "initmode","InitMode");
		AddParameter(ParameterType_Int,  "shadbandtocr","ShadBandTOCR");
		AddParameter(ParameterType_Int,  "shadbandrcr","ShadBandRCR");
		AddParameter(ParameterType_Float, "viehref","viehref");
		AddParameter(ParameterType_Float, "solhref","solhref");
		AddParameter(ParameterType_Float,  "nodata","NoData");
		AddParameter(ParameterType_Float, "maxdarkening","maxdarkening");
		AddParameter(ParameterType_Float, "minpercentile","minpercentile");
		AddParameter(ParameterType_Float, "maxcla","maxcla");
		AddParameter(ParameterType_Float, "compositelabelratio","compositelabelratio");
		AddParameter(ParameterType_Float,  "minpixelcorrelation","MinPixelCorrelation");
		AddParameter(ParameterType_Float,  "correlthreshold","CorrelThreshold");
		AddParameter(ParameterType_Float,  "ncorrel","NCorrel");
		AddParameter(ParameterType_StringList, "stolistofdates","STOListOfDates");
		AddParameter(ParameterType_Int,  "jday","JDay");
		//Output
		AddParameter(ParameterType_OutputImage, "shadvar", "Shadvar");
		SetParameterDescription("shadvar","Cloud Shadows variation");
		SetParameterOutputImagePixelType("shadvar", ImagePixelType_uint8);

		AddRAMParameter("ram");
		SetDefaultParameterInt("ram",2048);

	}

	void DoUpdateParameters()
	{

	}


	void DoExecute()
	{
		// Filter initialization
		m_OutsideCloudShadowMaskFilter = OutsideCloudsShadowMaskFilterType::New();
		m_ReflRatioFilter = RatioFilterType::New();
		m_ReflRatioMaskingFilter = ReflRatioMaskingFilterType::New();
		m_ReflRatioMaskedToVectorImage = RatioImageListToVectorImageFilterType::New();
		m_ShadVarThresholdValueEstimatorFilter = ShadowVariationThresholdValueEstimatorFilterType::New();
		m_ShadowVariationThresholdFilter = ShadowVariationThresholdFilterType::New();
		m_ShadVarLabelFilter = LabelFilterType::New();
		m_ShadowVariationMaskCompositeCombinaisonFilter = MaskMaskingFilterType::New();
		m_ShadowVariationPixelValidationFilter = DiscardedCloudPixelFilterType::New();
		m_ShadowVariationPixelDiscardingFilter = ValidateMaskFromLabelMapAndValidityMapImageFilterType::New();
		m_ShadowVariationCorrelationFilter = ShadowVariationCorrelationImageFilterType::New();


		vnsLogDebugMacro("CloudShadVar start");
		//Constant gen ask
		// Load inputs
		InputImageConstPointer l_IPTOCRPtr = this->GetParameterDoubleVectorImage("tocr");
		InputMaskConstPointer l_IPEDGPtr =  this->GetParameterUInt8Image("edg");
		InputMaskConstPointer l_IPCLDAll = this->GetParameterUInt8Image("cldall");
		InputMaskConstPointer l_IPCLDShad = this->GetParameterUInt8Image("cldshad");
		InputMaskConstPointer l_L2inShadVarPtr = this->GetParameterUInt8Image("l2shadvar");
		InputMaskConstPointer l_L2inShadowPtr = this->GetParameterUInt8Image("l2shad");
		InputImageConstPointer l_L2inSTOPtr = this->GetParameterDoubleVectorImage("l2sto");
		InputMaskConstPointer l_L2inWASPtr = this->GetParameterUInt8Image("l2was");
		InputImageConstPointer l_L2inRCRPtr = this->GetParameterDoubleVectorImage("l2rcr");
		InputMaskConstPointer l_L2inNDTPtr = this->GetParameterUInt8Image("l2ndt");
		InputImageConstPointer l_IPSOL =  this->GetParameterDoubleVectorImage("sol");
		DTMImageConstPointer l_DTMPtr = GetParameterDoubleImage("dtm");
		GRIDImageConstPointer l_IPVIEPtr = this->GetParameterDoubleVectorImage("vie");
		//Get parameters
		const double m_NoData = GetParameterFloat("nodata");
		const double m_VIEHRef = GetParameterFloat("viehref");
		const unsigned int m_ShadowBandTOCR = GetParameterInt("shadbandtocr");
		const unsigned int m_ShadowBandRCR = GetParameterInt("shadbandrcr");
		const double m_ProjShadDarkeningMax = GetParameterFloat("maxdarkening");
		const double m_DarkeningMinPercentile = GetParameterFloat("minpercentile");
		const double m_MaxCLA = GetParameterFloat("maxcla");
		const double m_SOLHref = GetParameterFloat("solhref");
		const double m_ShadVarCompositeLabelRatio = GetParameterFloat("compositelabelratio");
		double m_MinPixelCorrelation = GetParameterFloat("minpixelcorrelation");
		double m_CorrelThreshold= GetParameterFloat("correlthreshold");
		double m_NCorrel = GetParameterFloat("ncorrel");
		unsigned int m_JDay = GetParameterInt("jday");
		Utilities::ListOfDoubles m_STOListOfDates;
		const std::vector<std::string> l_ListOfDatesStr = GetParameterStringList("stolistofdates");
		for (unsigned int i = 0; i < l_ListOfDatesStr.size();i++)
		{
			m_STOListOfDates.push_back(Utilities::AsDouble(l_ListOfDatesStr[i]));
		}

		// Projection of clouds outside the image
        m_OutsideCloudShadowMaskFilter->SetIPSOLInput(l_IPSOL);
        m_OutsideCloudShadowMaskFilter->SetSOLHRef(m_SOLHref);
		m_OutsideCloudShadowMaskFilter->SetIPEDGInput(l_IPEDGPtr);
		m_OutsideCloudShadowMaskFilter->SetDTMInput(l_DTMPtr);
		m_OutsideCloudShadowMaskFilter->SetIPVIEInput(l_IPVIEPtr);
		m_OutsideCloudShadowMaskFilter->SetMaxCLA(m_MaxCLA);
		m_OutsideCloudShadowMaskFilter->SetVIEHRef(m_VIEHRef);
		m_OutsideCloudShadowMaskFilter->UpdateOutputInformation();

		// Radiometric criterion to detect shadow in the zone
		m_ReflRatioFilter->SetIPTOCRInput(l_IPTOCRPtr);
		m_ReflRatioFilter->SetL2inRCRInput(l_L2inRCRPtr);
		m_ReflRatioFilter->SetL2CLDSubInput(l_IPCLDAll);
		m_ReflRatioFilter->SetIPEDGInput(l_IPEDGPtr);
		m_ReflRatioFilter->SetL2inWASInput(l_L2inWASPtr);
		m_ReflRatioFilter->SetNoData(m_NoData);
		m_ReflRatioFilter->SetShadowBandTOCR(m_ShadowBandTOCR);
		m_ReflRatioFilter->SetShadowBandRCR(m_ShadowBandRCR);

		m_ReflRatioMaskingFilter->SetInput1(m_ReflRatioFilter->GetOutput());
		m_ReflRatioMaskingFilter->SetInput2(l_IPCLDShad);
		m_ReflRatioMaskingFilter->GetFunctor().SetBackgroundValue(static_cast<InputMaskPixelType>(0));
		m_ReflRatioMaskingFilter->GetFunctor().SetReplaceValue(static_cast<InputInternalPixelType>(0));
		m_ReflRatioMaskedToVectorImage->SetInput(m_ReflRatioMaskingFilter->GetOutput());

		// Radiometric Threshold Estimation
		PersistentShadowVariationThresholdValueEstimator<InputImageType>::CountVectorType l_NumberOfBins(1); // one component only
		l_NumberOfBins[0] = 1000;
		m_ShadVarThresholdValueEstimatorFilter->SetReflRatio(m_ReflRatioMaskedToVectorImage->GetOutput());
		m_ShadVarThresholdValueEstimatorFilter->SetNoDataValue(m_NoData);
		m_ShadVarThresholdValueEstimatorFilter->SetShadVarPercentile(m_DarkeningMinPercentile);   //XML Handler
		m_ShadVarThresholdValueEstimatorFilter->SetShadVarDarkeningMax(m_ProjShadDarkeningMax); //XML Handler
		m_ShadVarThresholdValueEstimatorFilter->Update();
		vnsLogDebugValueMacro(m_ShadVarThresholdValueEstimatorFilter->GetThresholdValue())
		// Radiometric Thresholding
		//m_ShadowVariationThresholdFilter->SetIPTOCRInput(l_IPTOCRPtr);
		m_ShadowVariationThresholdFilter->SetReflRatio(m_ReflRatioFilter->GetOutput());
		m_ShadowVariationThresholdFilter->SetNoData(m_NoData);
		m_ShadowVariationThresholdFilter->SetThresholdValue(m_ShadVarThresholdValueEstimatorFilter->GetThresholdValue());
		m_ShadowVariationThresholdFilter->SetIPCLDOutShadInput(m_OutsideCloudShadowMaskFilter->GetIPCLDOutShadOutput());

		// Labelisation of ShadVarTemp
		m_ShadVarLabelFilter->SetInput(m_ShadowVariationThresholdFilter->GetIPCLDShadVarOutput());

		// ShadVarTemp post-processing (over labels)
		m_ShadowVariationMaskCompositeCombinaisonFilter->SetInput1(l_L2inShadVarPtr);                //D-1 shadvar
		m_ShadowVariationMaskCompositeCombinaisonFilter->SetInput2(l_L2inShadowPtr);                //D-1 shadow
		m_ShadowVariationMaskCompositeCombinaisonFilter->GetFunctor().SetBackgroundValue(static_cast<InputMaskPixelType>(1));
		m_ShadowVariationMaskCompositeCombinaisonFilter->GetFunctor().SetReplaceValue(static_cast<InputMaskPixelType>(0));

		m_ShadowVariationPixelValidationFilter->SetInput(m_ShadowVariationMaskCompositeCombinaisonFilter->GetOutput());
		m_ShadowVariationPixelValidationFilter->SetInputLabelImage(m_ShadVarLabelFilter->GetOutput());
		m_ShadowVariationPixelValidationFilter->SetLowerValidityThreshold(m_ShadVarCompositeLabelRatio);
		m_ShadowVariationPixelValidationFilter->Update();

		/// Filtering
		m_ShadowVariationPixelDiscardingFilter->SetMaskInput(m_ShadowVariationThresholdFilter->GetIPCLDShadVarOutput());
		m_ShadowVariationPixelDiscardingFilter->SetLabelInput(m_ShadVarLabelFilter->GetOutput());
		m_ShadowVariationPixelDiscardingFilter->SetValidityMap(m_ShadowVariationPixelValidationFilter->GetLabelValidityStatusMap());
		// No ExtCorrelFlagInput in this case
		m_ShadowVariationPixelDiscardingFilter->SetExtCorrelFlagInput(ITK_NULLPTR);
		m_ShadowVariationPixelDiscardingFilter->Update();

		// ShadVarTemp correlation
		m_ShadowVariationCorrelationFilter->SetIPCLDShadVarInput(m_ShadowVariationPixelDiscardingFilter->GetOutput()); //ShadowVariationPixelDiscardingFilterOutput);
		m_ShadowVariationCorrelationFilter->SetIPTOCRInput(l_IPTOCRPtr);
		m_ShadowVariationCorrelationFilter->SetL2inRCRInput(l_L2inRCRPtr);
		m_ShadowVariationCorrelationFilter->SetL2inSTOInput(l_L2inSTOPtr);
		m_ShadowVariationCorrelationFilter->SetIPEDGInput(l_IPEDGPtr);
		m_ShadowVariationCorrelationFilter->SetL2inNDTInput(l_L2inNDTPtr);
		m_ShadowVariationCorrelationFilter->SetL2inWASInput(l_L2inWASPtr);
		m_ShadowVariationCorrelationFilter->SetNoData(m_NoData);

		m_ShadowVariationCorrelationFilter->SetCorrelBandTOCR(m_ShadowBandTOCR); //m_CorrelBandTOCR);
		m_ShadowVariationCorrelationFilter->SetCorrelBandRCR(m_ShadowBandRCR); //m_CorrelBandRCR);
		m_ShadowVariationCorrelationFilter->SetJDay(m_JDay);
		m_ShadowVariationCorrelationFilter->SetSTOListOfDates(m_STOListOfDates);
		m_ShadowVariationCorrelationFilter->SetMinPixelCorrelation(m_MinPixelCorrelation);
		m_ShadowVariationCorrelationFilter->SetCorrelThreshold(m_CorrelThreshold);
		m_ShadowVariationCorrelationFilter->SetNCorrel(m_NCorrel);

		//SetOutput
		SetParameterOutputImage<InputMaskType>("shadvar",m_ShadowVariationCorrelationFilter->GetIPCLDShadVarOutput());


	}


	/** Filters declaration */
	OutsideCloudsShadowMaskPointerFilterType m_OutsideCloudShadowMaskFilter;
	RatioFilterPointerType m_ReflRatioFilter;
	ReflRatioMaskingFilterPointerType m_ReflRatioMaskingFilter;
	RatioImageListToVectorImageFilterPointerType m_ReflRatioMaskedToVectorImage;
	ShadowVariationThresholdValueEstimatorFilterPointerType m_ShadVarThresholdValueEstimatorFilter;
	ShadowVariationThresholdPointerFilterType m_ShadowVariationThresholdFilter;
	LabelPointerFilterType m_ShadVarLabelFilter;
	MaskMaskingFilterPointerType m_ShadowVariationMaskCompositeCombinaisonFilter;
	DiscardedCloudPixelFilterPointerType m_ShadowVariationPixelValidationFilter;
	ValidateMaskFromLabelMapAndValidityMapImageFilterPointerType m_ShadowVariationPixelDiscardingFilter;
	ShadowVariationCorrelationImageFilterPointerType m_ShadowVariationCorrelationFilter;


};

}
}

OTB_APPLICATION_EXPORT(vns::Wrapper::CloudShadVar)
