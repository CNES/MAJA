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
#include "itkBinaryBallStructuringElement.h"
#include "vnsShadowMaskDeterminationImageFilter.h"
#include "vnsShadowMaskDeterminationUnknownCloudsAltitudeImageFilter.h"
#include "itkBinaryDilateImageFilter.h"
#include "vnsConstantImageSource.h"
#include "vnsStereoCorrectionImageFilter.h"

namespace vns
{

enum
{
	withalt,
	withoutalt
};

namespace Wrapper
{

using namespace otb::Wrapper;

class CloudShadow : public Application
{
public:
	/** Standard class typedefs. */
	typedef CloudShadow           Self;
	typedef otb::Wrapper::Application     Superclass;
	typedef itk::SmartPointer<Self>       Pointer;
	typedef itk::SmartPointer<const Self> ConstPointer;

	/** Standard macro */
	itkNewMacro(Self);

	itkTypeMacro(CloudShadow, otb::Wrapper::Application);

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

	// Stereoscopic altitude correction
	typedef StereoCorrectionImageFilter<CLAImageType, InputMaskType, LabelImageType, CLAImageType> StereoCorrectionFilterType;
	typedef StereoCorrectionFilterType::Pointer StereoCorrectionPointerFilterType;
	// Shadow mask determination
	typedef ShadowMaskDeterminationImageFilter<InputImageType, InputMaskType, LabelImageType, GRIDImageType, CLAImageType,
			DTMImageType, InputMaskType> ShadowMaskDeterminationFilterType;
	typedef ShadowMaskDeterminationFilterType::Pointer ShadowMaskDeterminationPointerFilterType;
	// DM982
	typedef ShadowMaskDeterminationUnknownCloudsAltitudeImageFilter<InputImageType, InputMaskType, LabelImageType,
			GRIDImageType, CLAImageType, DTMImageType, InputMaskType>                   ShadowMaskDeterminationWithoutAltitudeFilterType;
	typedef ShadowMaskDeterminationWithoutAltitudeFilterType::Pointer          ShadowMaskDeterminationWithoutAltitudePointerFilterType;
	// Dilatation of all cloud masks
	typedef itk::BinaryBallStructuringElement<InputMaskPixelType, InputMaskType::ImageDimension> StructuringElementType;
	typedef itk::BinaryDilateImageFilter<InputMaskType, InputMaskType, StructuringElementType> DilateFilterType;
	typedef DilateFilterType::Pointer DilatePointerFilterType;

	//Constant mask in case no alt init
	typedef ConstantImageSource<InputMaskType> ConstantFilterType;
	typedef ConstantFilterType::Pointer ConstantPointerFilterType;


private:
	void DoInit()
	{
		SetName("CloudShadow");
		SetDescription("Cloud shadow algo.");
		Loggers::GetInstance()->Initialize(GetName());
		// Documentation
		SetDocLongDescription("This application computes the cloud shadow mask");
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
		AddParameter(ParameterType_InputImage,  "cla",   "CLA");
		SetParameterDescription("cla", "Cla image");
		AddParameter(ParameterType_InputImage,  "vie",   "VIE");
		SetParameterDescription("vie", "VIE image");
		AddParameter(ParameterType_InputImage,  "dtm",   "dtm image");
		SetParameterDescription("dtm", "dtm of the image");
		//Previous L2
		AddParameter(ParameterType_InputImage,  "l2rcr",   "L2 RCR");
		SetParameterDescription("l2rcr", "L2 RCR");
		MandatoryOff("l2rcr");
		AddParameter(ParameterType_InputImage,  "l2was",   "L2 WAS");
		SetParameterDescription("l2was", "L2 WAS");
		MandatoryOff("l2was");
		AddParameter(ParameterType_InputImage,  "l2ndt",   "L2 NDT");
		SetParameterDescription("l2ndt", "L2 NDT");
		MandatoryOff("l2ndt");

		//Set parameters
		AddParameter(ParameterType_Bool,  "initmode","InitMode");
		AddParameter(ParameterType_Group, "sol1", "SOL1 parameters");
		AddParameter(ParameterType_InputImage,  "sol1.in",   "SOL1 image");
		SetParameterDescription("sol1.in", "Image used as solar angle 1");
		AddParameter(ParameterType_Float,  "sol1.h",   "SOL1 height");
		SetParameterDescription("sol1.h", "Height used to compute solar angle");
		AddParameter(ParameterType_Float,  "solhref",  "solar height ref");
		SetParameterDescription("solhref","solar height ref");
		AddParameter(ParameterType_Int,  "defaultalt","defaultalt");
		AddParameter(ParameterType_Int,  "ksigma","ksigma");

		AddParameter(ParameterType_Int,  "deltahmax","deltahmax");
		AddParameter(ParameterType_Int,  "deltahmin","deltahmin");
		AddParameter(ParameterType_Int,  "deltahstep","deltahstep");
		AddParameter(ParameterType_Int,  "shadbandtocr","ShadBandTOCR");
		AddParameter(ParameterType_Int,  "shadbandrcr","ShadBandRCR");
		AddParameter(ParameterType_Float, "viehref","viehref");
		AddParameter(ParameterType_Float,  "nodata","NoData");

		//Choice on algo
		AddParameter(ParameterType_Choice,"algo","algo");

		//With alt spec
		AddChoice("algo.withalt","With altitude");
		AddParameter(ParameterType_Bool,"algo.withalt.refinement","refinement");

		AddParameter(ParameterType_Float, "algo.withalt.absnbpixthresh","absnbpixthresh");
		MandatoryOff("algo.withalt.absnbpixthresh");

		AddParameter(ParameterType_Float, "algo.withalt.threshdiffim","threshdiffim");
		MandatoryOff("algo.withalt.threshdiffim");

		AddParameter(ParameterType_Group, "algo.withalt.sol2", "SOL2 parameters");
		AddParameter(ParameterType_InputImage,  "algo.withalt.sol2.in",   "SOL2 image");
		MandatoryOff("algo.withalt.sol2.in");

		SetParameterDescription("algo.withalt.sol2.in", "Image used as solar angle 2");
		AddParameter(ParameterType_Float,  "algo.withalt.sol2.h",   "SOL2 height");
		SetParameterDescription("algo.withalt.sol2.h", "Height used to compute solar angle");
		MandatoryOff("algo.withalt.sol2.h");


		//No Alt spec
		AddChoice("algo.withoutalt", "Without alt");
		AddParameter(ParameterType_Float,"algo.withoutalt.cloudcovermult","cloudcovermult");
		AddParameter(ParameterType_Float,"algo.withoutalt.shadratiomax","shadratiomax");
		AddParameter(ParameterType_Float,"algo.withoutalt.shadratiomin","shadratiomin");
		AddParameter(ParameterType_Int,"algo.withoutalt.shadratiopixnb","shadratiopixnb");
		AddParameter(ParameterType_Float,"algo.withoutalt.maxdarkening","maxdarkening");
		AddParameter(ParameterType_Float,"algo.withoutalt.minpercentile","minpercentile");
		AddParameter(ParameterType_Int,"algo.withoutalt.seradius","seradius");


		//Output
		AddParameter(ParameterType_OutputImage, "shadow", "Shadows");
		SetParameterDescription("shadow","Cloud Shadows");
		SetParameterOutputImagePixelType("shadow", ImagePixelType_uint8);
		AddParameter(ParameterType_OutputImage, "l2cla", "l2cla");
		SetParameterDescription("l2cla","l2cla");
		SetParameterOutputImagePixelType("l2cla", ImagePixelType_double);
		MandatoryOff("l2cla");

		AddRAMParameter("ram");
		SetDefaultParameterInt("ram",2048);
	}

	void DoUpdateParameters()
	{

	}


	void DoExecute()
	{
		// Filter initialization
		m_IPCLDAllNoExtensionDilateFilter = DilateFilterType::New();
		m_LabelFilter = LabelFilterType::New();
		m_StereoCorrectionFilter = StereoCorrectionFilterType::New();
		m_ShadowMaskDeterminationFilter = ShadowMaskDeterminationFilterType::New();
		m_ShadowMaskDeterminationWithoutCloudAltitudeFilter = ShadowMaskDeterminationWithoutAltitudeFilterType::New();
		m_ConstantMaskFilter = ConstantFilterType::New();
		vnsLogDebugMacro("CloudShadow start");
		//Constant gen ask
		// Load inputs
		InputImageConstPointer l_IPTOCRPtr = this->GetParameterDoubleVectorImage("tocr");
		InputMaskConstPointer l_IPEDGPtr =  this->GetParameterUInt8Image("edg");
		InputMaskConstPointer l_IPCLDAll = this->GetParameterUInt8Image("cldall");
		CLAImagePointer l_L1CLAPtr = this->GetParameterDoubleImage("cla");
		InputMaskConstPointer l_L2inWASPtr;
		InputImageConstPointer l_L2inRCRPtr;
		InputMaskConstPointer l_L2inNDTPtr;
		DTMImageConstPointer l_DTMPtr = GetParameterDoubleImage("dtm");
		GRIDImageConstPointer l_IPSOL1Ptr = this->GetParameterDoubleVectorImage("sol1.in");
		GRIDImageConstPointer l_IPVIEPtr = this->GetParameterDoubleVectorImage("vie");
		//Get parameters
		const double l_NoData = GetParameterFloat("nodata");
		const unsigned short m_KsigmaAltitude = static_cast<unsigned short>(this->GetParameterInt("ksigma"));
		const short m_DefaultAltitude = static_cast<short>(this->GetParameterInt("defaultalt"));
		const double l_SOLHRef = this->GetParameterFloat("solhref");
		const double l_SOLH1 = this->GetParameterFloat("sol1.h");
		const short m_DeltaHMax = static_cast<short>(this->GetParameterInt("deltahmax"));
		const short m_DeltaHMin = static_cast<short>(this->GetParameterInt("deltahmin"));
		const short m_DeltaHStep = static_cast<short>(this->GetParameterInt("deltahstep"));
		const bool m_InitMode = GetParameterInt("initmode");
		const unsigned int m_ShadowBandTOCR = GetParameterInt("shadbandtocr");
		const unsigned int m_ShadowBandRCR = GetParameterInt("shadbandrcr");
		const double m_VIEHRef = GetParameterFloat("viehref");
		bool m_CloudMaskingKnownCloudsAltitude = false;

		// Get Mode
		switch ( GetParameterInt("algo") )
		{
		case withalt:
			m_CloudMaskingKnownCloudsAltitude = true;
			break;
		case withoutalt:
			m_CloudMaskingKnownCloudsAltitude = false;
			break;
		default:
			m_CloudMaskingKnownCloudsAltitude = false;
			break;
		}


		if (m_InitMode == false)
		{
			if(!HasValue("l2rcr"))
			{
				vnsExceptionDataMacro("No input l2rcr");
			}
			l_L2inRCRPtr = this->GetParameterDoubleVectorImage("l2rcr");
			if(!HasValue("l2ndt"))
			{
				vnsExceptionDataMacro("No input l2ndt");
			}
			l_L2inNDTPtr = this->GetParameterUInt8Image("l2ndt");
			if(!HasValue("l2was"))
			{
				vnsExceptionDataMacro("No input l2was");
			}
			l_L2inWASPtr = this->GetParameterUInt8Image("l2was");
		}


		vnsLogDebugMacro("Object count " << m_LabelFilter->GetObjectCount());

		// Assign an altitude to each pixel within each label
		m_StereoCorrectionFilter->SetIPCLAInput(l_L1CLAPtr);
		m_StereoCorrectionFilter->SetIPCLDAllInput(l_IPCLDAll);

		m_StereoCorrectionFilter->SetExcludedValue(static_cast<CLAImagePixelType>(0));
		m_StereoCorrectionFilter->SetDefaultValue(static_cast<LabelImagePixelType>(0));
		m_StereoCorrectionFilter->SetKsigmaAltitude(m_KsigmaAltitude);
		m_StereoCorrectionFilter->SetDefaultAltitude(m_DefaultAltitude);

		m_StereoCorrectionFilter->UpdateWithCaching();

		// Projection of clouds shadows and iteration on altitude
		if (m_CloudMaskingKnownCloudsAltitude == true) //TODO: clean up nonVenus specific code
		{
			GRIDImagePointer l_IPSOL2Ptr;
			unsigned short l_SOLH2 = 0;
			const bool m_RefinementOption = IsParameterEnabled("algo.withalt.refinement");
			const double m_AbsNbpixThreshold = GetParameterFloat("algo.withalt.absnbpixthresh");
			const double m_ThresholdDiffImage = GetParameterFloat("algo.withalt.threshdiffim");

			// Labellization of clouds
			m_LabelFilter->SetInput(l_IPCLDAll);

			//Shadow filter init
			m_ShadowMaskDeterminationFilter->SetIPTOCRInput(l_IPTOCRPtr);
			m_ShadowMaskDeterminationFilter->SetIPEDGInput(l_IPEDGPtr);
			m_ShadowMaskDeterminationFilter->SetL2CLDSubInput(l_IPCLDAll);
			m_ShadowMaskDeterminationFilter->SetIPVIEInput(l_IPVIEPtr);
			m_ShadowMaskDeterminationFilter->SetIPSOL1Input(l_IPSOL1Ptr);
			if (HasValue("algo.withalt.sol2.in"))
			{
				l_IPSOL2Ptr = this->GetParameterDoubleVectorImage("algo.withalt.sol2.in");
				l_SOLH2 = static_cast<unsigned short>(this->GetParameterInt("algo.withalt.sol2.h"));
				m_ShadowMaskDeterminationFilter->SetIPSOL2Input(l_IPSOL2Ptr);
			}
			m_ShadowMaskDeterminationFilter->SetIPCLAInput(m_StereoCorrectionFilter->GetIPCLAOutput());
			m_ShadowMaskDeterminationFilter->SetDTMInput(l_DTMPtr);
			m_ShadowMaskDeterminationFilter->SetL2CLDSubLabelInput(m_LabelFilter->GetOutput());

			// In init mode, the composite product is not available
			if (m_InitMode == false)
			{
				m_ShadowMaskDeterminationFilter->SetL2inRCRInput(l_L2inRCRPtr);
				m_ShadowMaskDeterminationFilter->SetL2inNDTInput(l_L2inNDTPtr);
				m_ShadowMaskDeterminationFilter->SetL2inWASInput(l_L2inWASPtr);
			}

			// parameters
			m_ShadowMaskDeterminationFilter->SetNoData(l_NoData);
			m_ShadowMaskDeterminationFilter->SetInitMode(m_InitMode);
			m_ShadowMaskDeterminationFilter->SetRefinementOption(m_RefinementOption);
			m_ShadowMaskDeterminationFilter->SetDeltaHMax(m_DeltaHMax);
			m_ShadowMaskDeterminationFilter->SetDeltaHMin(m_DeltaHMin);
			m_ShadowMaskDeterminationFilter->SetDeltaHStep(m_DeltaHStep);
			m_ShadowMaskDeterminationFilter->SetSOLH1(l_SOLH1);
			m_ShadowMaskDeterminationFilter->SetSOLH2(l_SOLH2);
			m_ShadowMaskDeterminationFilter->SetSOLHRef(l_SOLHRef);
			m_ShadowMaskDeterminationFilter->SetVIEHRef(m_VIEHRef);
			m_ShadowMaskDeterminationFilter->SetAbsNbpixThreshold(m_AbsNbpixThreshold);
			m_ShadowMaskDeterminationFilter->SetThresholdDiffImage(m_ThresholdDiffImage);
			m_ShadowMaskDeterminationFilter->SetShadowBandTOCR(m_ShadowBandTOCR);
			m_ShadowMaskDeterminationFilter->SetShadowBandRCR(m_ShadowBandRCR);

			// Call the main of the ShadowMaskDeterminationFilter
			m_ShadowMaskDeterminationFilter->UpdateWithCaching();

			// ========> Set the cloud altitude output mask
			SetParameterOutputImage<InputMaskType>("shadow", m_ShadowMaskDeterminationFilter->GetIPCLDShadOutput());
			if(!HasValue("l2cla"))
			{
				vnsExceptionDataMacro("No input l2cla filename given");
			}
			SetParameterOutputImage<CLAImageType>("l2cla", m_ShadowMaskDeterminationFilter->GetIPCLAOutput());

		}
		else //Not Venus -> DM982:
		{
			// In init mode, the composite product is not available
			if (m_InitMode == false)
			{
				const double m_CloudCoverMultFactor = GetParameterFloat("algo.withoutalt.cloudcovermult");
				const double m_ShadowCloudRatioMax = GetParameterFloat("algo.withoutalt.shadratiomax");
				const double m_ShadowCloudRatioMin = GetParameterFloat("algo.withoutalt.shadratiomin");
				const unsigned int m_ShadowCloudRatioPixNb = GetParameterInt("algo.withoutalt.shadratiopixnb");
				const double m_ProjShadDarkeningMax = GetParameterFloat("algo.withoutalt.maxdarkening");
				const double m_DarkeningMinPercentile = GetParameterFloat("algo.withoutalt.minpercentile");
				const int m_SECloudRadius =  GetParameterInt("algo.withoutalt.seradius");

				 // Dilatation of all cloud mask
				InputInternalPixelType l_DilateValue(1);
				StructuringElementType l_SECloud;
				l_SECloud.SetRadius(m_SECloudRadius); // 3x3 structuring element
				l_SECloud.CreateStructuringElement();
				m_IPCLDAllNoExtensionDilateFilter->SetKernel(l_SECloud);
				m_IPCLDAllNoExtensionDilateFilter->SetInput(l_IPCLDAll);
				m_IPCLDAllNoExtensionDilateFilter->SetDilateValue(l_DilateValue);

				//Shadow filter
				m_ShadowMaskDeterminationWithoutCloudAltitudeFilter->SetNoData(l_NoData);
				m_ShadowMaskDeterminationWithoutCloudAltitudeFilter->SetDeltaHMin(m_DeltaHMin);
				m_ShadowMaskDeterminationWithoutCloudAltitudeFilter->SetDeltaHMax(m_DeltaHMax);
				m_ShadowMaskDeterminationWithoutCloudAltitudeFilter->SetDeltaHStep(m_DeltaHStep);
				m_ShadowMaskDeterminationWithoutCloudAltitudeFilter->SetShadowBandTOCR(m_ShadowBandTOCR);
				m_ShadowMaskDeterminationWithoutCloudAltitudeFilter->SetShadowBandRCR(m_ShadowBandRCR);
				m_ShadowMaskDeterminationWithoutCloudAltitudeFilter->SetSOLH1(l_SOLH1);
				m_ShadowMaskDeterminationWithoutCloudAltitudeFilter->SetVIEHRef(m_VIEHRef);
				m_ShadowMaskDeterminationWithoutCloudAltitudeFilter->SetCloudCoverMultFactor(m_CloudCoverMultFactor);
				m_ShadowMaskDeterminationWithoutCloudAltitudeFilter->SetShadowCloudRatioMax(m_ShadowCloudRatioMax);
				m_ShadowMaskDeterminationWithoutCloudAltitudeFilter->SetShadowCloudRatioMin(m_ShadowCloudRatioMin);
				m_ShadowMaskDeterminationWithoutCloudAltitudeFilter->SetShadowCloudRatioPixNb(m_ShadowCloudRatioPixNb);
				m_ShadowMaskDeterminationWithoutCloudAltitudeFilter->SetProjShadDarkeningMax(m_ProjShadDarkeningMax);
				m_ShadowMaskDeterminationWithoutCloudAltitudeFilter->SetDarkeningMinPercentile(m_DarkeningMinPercentile);
				m_ShadowMaskDeterminationWithoutCloudAltitudeFilter->SetL2inRCRInput(l_L2inRCRPtr);
				m_ShadowMaskDeterminationWithoutCloudAltitudeFilter->SetL2inNDTInput(l_L2inNDTPtr);
				m_ShadowMaskDeterminationWithoutCloudAltitudeFilter->SetL2inWASInput(l_L2inWASPtr);
				m_ShadowMaskDeterminationWithoutCloudAltitudeFilter->SetIPTOCRInput(l_IPTOCRPtr);
				m_ShadowMaskDeterminationWithoutCloudAltitudeFilter->SetL2CLDSubInput(l_IPCLDAll);
				m_ShadowMaskDeterminationWithoutCloudAltitudeFilter->SetIPEDGInput(l_IPEDGPtr);
				m_ShadowMaskDeterminationWithoutCloudAltitudeFilter->SetL2CLDDilatedSubInput(m_IPCLDAllNoExtensionDilateFilter->GetOutput());
				m_ShadowMaskDeterminationWithoutCloudAltitudeFilter->SetIPVIEInput(l_IPVIEPtr);
				m_ShadowMaskDeterminationWithoutCloudAltitudeFilter->SetIPSOLInput(l_IPSOL1Ptr);
				m_ShadowMaskDeterminationWithoutCloudAltitudeFilter->SetIPCLAInput(m_StereoCorrectionFilter->GetIPCLAOutput());
				m_ShadowMaskDeterminationWithoutCloudAltitudeFilter->SetDTMInput(l_DTMPtr);
				// Call the main of the ShadowMaskDeterminationFilter
				m_ShadowMaskDeterminationWithoutCloudAltitudeFilter->UpdateWithCaching();
				SetParameterOutputImage<InputMaskType>("shadow",m_ShadowMaskDeterminationWithoutCloudAltitudeFilter->GetIPCLDShadOutput());
			} else
			{
				vnsLogWarningMacro("Can not run without altitude in InitMode, constant mask produced");
				// In init mode, the reflectance variation threshold test is not performed
				// and the water mask of the composite is not available
				// A constant mask is generate to proceed the treatments
				// This mask is also used when an option is not set and a simulated image has to be used
				m_ConstantMaskFilter->SetSize(l_IPEDGPtr->GetLargestPossibleRegion().GetSize());
				m_ConstantMaskFilter->SetSignedSpacing(l_IPEDGPtr->GetSignedSpacing());
				m_ConstantMaskFilter->SetOrigin(l_IPEDGPtr->GetOrigin());
				m_ConstantMaskFilter->SetConstantValue(static_cast<InputMaskPixelType>(0));
				SetParameterOutputImage<InputMaskType>("shadow",m_ConstantMaskFilter->GetOutput());

			}
		}

		//SetOutput

	}


	/** Filters declaration */
	LabelPointerFilterType m_LabelFilter;
	StereoCorrectionPointerFilterType m_StereoCorrectionFilter;
	ShadowMaskDeterminationPointerFilterType m_ShadowMaskDeterminationFilter;
	ShadowMaskDeterminationWithoutAltitudePointerFilterType	m_ShadowMaskDeterminationWithoutCloudAltitudeFilter;
	DilatePointerFilterType m_IPCLDAllNoExtensionDilateFilter;
	ConstantPointerFilterType m_ConstantMaskFilter;
};

}
}

OTB_APPLICATION_EXPORT(vns::Wrapper::CloudShadow)
