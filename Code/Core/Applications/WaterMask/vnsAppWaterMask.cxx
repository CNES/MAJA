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
#include "itkDiscreteGaussianImageFilter.h"
#include "vnsPossibleWaterFlagImageFilter.h"
#include "vnsSRTMWaterMaskImproveImageFilter.h"
#include "itkMultiplyImageFilter.h"


#include <string>

namespace vns
{
namespace Wrapper
{

using namespace otb::Wrapper;

class WaterMask : public Application
{
public:
	/** Standard class typedefs. */
	typedef WaterMask              Self;
	typedef otb::Wrapper::Application     Superclass;
	typedef itk::SmartPointer<Self>       Pointer;
	typedef itk::SmartPointer<const Self> ConstPointer;

	/** Standard macro */
	itkNewMacro(Self);

	itkTypeMacro(WaterMask, otb::Wrapper::Application);

	/** Some convenient typedefs. */
	typedef DoubleVectorImageType InputImageType;
	typedef InputImageType::ConstPointer InputImageConstPointer;
	typedef InputImageType::RegionType RegionType;
	typedef InputImageType::PixelType InputImagePixelType;
	typedef InputImageType::SizeType SizeType;

	typedef UInt8ImageType InputMaskType;
	typedef InputMaskType::Pointer InputMaskPointer;
	typedef InputMaskType::ConstPointer InputMaskConstPointer;
	typedef InputMaskType::PixelType InputMaskPixelType;

	typedef UInt8VectorImageType VectorMaskType;
	typedef VectorMaskType::Pointer VectorMaskPointer;
	typedef VectorMaskType::ConstPointer VectorMaskConstPointer;
	typedef VectorMaskType::PixelType VectorMaskPixelType;

	typedef UInt8ImageType SRTMMaskType;
	typedef SRTMMaskType::Pointer SRTMMaskPointer;
	typedef SRTMMaskType::ConstPointer SRTMMaskConstPointer;
	typedef SRTMMaskType::PixelType SRTMMaskPixelType;
	typedef VNSRealImageType SRTMWeighImageType;
	//Typedef for SRTM smoothing
	typedef itk::DiscreteGaussianImageFilter<SRTMMaskType, SRTMWeighImageType> GaussianSRTMMaskFilterType;
	typedef GaussianSRTMMaskFilterType::Pointer                       GaussianSRTMMaskFilterPointer;
	typedef itk::MultiplyImageFilter<SRTMWeighImageType, SRTMMaskType, SRTMWeighImageType> SRTMMultiplyFilterType;
	typedef SRTMMultiplyFilterType::Pointer                       SRTMMultiplyFilterPointer;

	typedef VNSRealImageType ImageType;
	typedef ImageType::Pointer ImageTypePointer;
	typedef ImageType::ConstPointer ImageTypeConstPointer;
	typedef ImageType::PixelType ImageTypePixelType;

	typedef UInt8ImageType OutputImageType;
	typedef OutputImageType::Pointer OutputImagePointer;
	typedef OutputImageType::PixelType OutputImagePixelType;


	/** Filters typedef */
	typedef PossibleWaterFlagImageFilter<InputImageType, InputMaskType, ImageType, InputMaskType> PossibleWaterFlagFilterType;
	typedef PossibleWaterFlagFilterType::Pointer PossibleWaterFlagPointerFilterType;

	typedef SRTMWaterMaskImproveImageFilter<InputMaskType, VectorMaskType, SRTMWeighImageType, OutputImageType> SRTMFilterType;
	typedef SRTMFilterType::Pointer SRTMPointerFilterType;


private:
	void DoInit()
	{
		SetName("WaterMask");
		SetDescription("WaterMask algo.");
		Loggers::GetInstance()->Initialize(GetName());
		// Documentation
		SetDocLongDescription("This application computes the Water mask");
		SetDocLimitations("None");
		SetDocAuthors("MAJA-Team");
		SetDocSeeAlso("MAJA Doc");

		AddDocTag("Statistics");

		AddParameter(ParameterType_InputImage,  "tocr",   "tocr image");
		SetParameterDescription("tocr", "TOCR image at coarse all bands");
		AddParameter(ParameterType_InputImage,  "edg",   "EDG image");
		SetParameterDescription("edg", "Image used as edge at coarse");
		AddParameter(ParameterType_InputImage,  "cldsum",   "Cloud Sum");
		SetParameterDescription("cldsum", "Cloud sum at coarse");
		AddParameter(ParameterType_InputImage,  "dtmshd",   "DTM shadow");
		SetParameterDescription("dtmshd", "DTM Shadow at coarse");
		AddParameter(ParameterType_InputImage,  "srtmwat",   "SRTM water");
		SetParameterDescription("srtmwat", "SRTM Water mask at coarse");
		AddParameter(ParameterType_InputImage,  "demslc",   "DEM_SLC image");
		SetParameterDescription("demslc", "Image used as DEM_SLC at coarse");
		AddParameter(ParameterType_InputImage,  "l2rcr",   "L2 RCR");
		SetParameterDescription("l2rcr", "L2 RCR (optional)");
		MandatoryOff("l2rcr");
		AddParameter(ParameterType_InputImage,  "l2pwa",   "L2 PWA");
		SetParameterDescription("l2pwa", "L2 PWA (optional)");
		MandatoryOff("l2pwa");
		AddParameter(ParameterType_InputImage,  "l2twa",   "L2 TWA");
		SetParameterDescription("l2twa", "L2 TWA (optional)");
		MandatoryOff("l2twa");

		//Set parameters
		// Init mode parameter accessors
		AddParameter(ParameterType_Bool,  "initmode","InitMode");
		// Sunglint flag parameter accessors
		AddParameter(ParameterType_Bool,  "sunglintflag","SunglintFlag");
		// Red band index accessors for the TOCR
		AddParameter(ParameterType_Int, "redbandtocr", "Red Band Index in RCR");
		// NIR band index accessors for TOCR
		AddParameter(ParameterType_Int,  "nirbandtocr","NIR Band Index in TOCR");
		// NIR band index accessors for RCR
		AddParameter(ParameterType_Int,  "nirbandrcr", "NIR Band Index in RCR");
		// Water NDVI threshold accessors
		AddParameter(ParameterType_Float,  "waterndvithreshold","WaterNDVIThreshold");
		// Water reflectance variation threshold accessors
		AddParameter(ParameterType_Float,  "waterreflectancevariationthreshold","WaterReflectanceVariationThreshold");
		// Maximum sunglint reflectance accessors
		AddParameter(ParameterType_Float,  "maximumsunglintreflectance", "MaximumSunglintReflectance");
		// SRTM sigma for gaussian accessors
		AddParameter(ParameterType_Float,  "srtmgaussiansigma", "SRTMGaussianSigma");
		// Bonus SRTM water mask accessors
		AddParameter(ParameterType_Float,  "srtmpercent", "SRTMPercent");
		// Number of water mask accessors
		AddParameter(ParameterType_Int,  "watermasknumber", "WaterMaskNumber");
		// Accessors of minimum percent to declare a pixel as water
		AddParameter(ParameterType_Float,  "minpercentpossiblewater","MinPercentPossibleWater");
		// DEM slope threshold accessors
		AddParameter(ParameterType_Float,  "waterslopethreshold", "WaterSlopeThreshold");
		// Red reflectance threshold accessors
		AddParameter(ParameterType_Float,  "waterredreflectancethreshold", "WaterRedReflectanceThreshold");
		// No data
		AddParameter(ParameterType_Float,  "reall2nodata", "RealL2NoData");




		AddParameter(ParameterType_OutputImage, "was", "Water mask");
		SetParameterDescription("was","Water mask");
		SetParameterOutputImagePixelType("was", ImagePixelType_uint8);
		AddParameter(ParameterType_OutputImage, "pwa", "Possible Water mask");
		SetParameterDescription("pwa","Possible Water Mask");
		SetParameterOutputImagePixelType("pwa", ImagePixelType_uint8);
		AddParameter(ParameterType_OutputImage, "twa", "Tested Water mask");
		SetParameterDescription("twa","Tested Water mask");
		SetParameterOutputImagePixelType("twa", ImagePixelType_uint8);

		AddRAMParameter("ram");
		SetDefaultParameterInt("ram",2048);

	}

	void DoUpdateParameters()
	{

	}


	void DoExecute()
	{
		// Init filters
		m_PossibleWaterFlagFilter = PossibleWaterFlagFilterType::New();
		m_SRTMFilter = SRTMFilterType::New();
		m_SRTMGaussianFilter = GaussianSRTMMaskFilterType::New();
		m_SRTMMultiplyFilter = SRTMMultiplyFilterType::New();
		// Load inputs
		InputImageConstPointer l_IPTOCRPtr = this->GetParameterDoubleVectorImage("tocr");
		InputMaskConstPointer l_IPEDGPtr =  this->GetParameterUInt8Image("edg");
		InputMaskConstPointer l_IPSHDSubPtr = this->GetParameterUInt8Image("dtmshd");
		SRTMMaskConstPointer l_SRTMWATPtr = this->GetParameterUInt8Image("srtmwat");
		InputMaskConstPointer l_L2CLDSubSummaryPtr = this->GetParameterUInt8Image("cldsum");
		ImageTypeConstPointer l_SlopeSubPtr = this->GetParameterDoubleImage("demslc");

		// Initializations of images of the composite
		InputImageConstPointer l_L2inRCRPtr;
		VectorMaskConstPointer l_L2inPWAPtr;
		VectorMaskConstPointer l_L2inTWAPtr;

		//Get parameters
		const bool l_InitMode = IsParameterEnabled("initmode");
		const bool l_SunglintFlag = IsParameterEnabled("sunglintflag");
		const unsigned int l_RedBandTOCR = GetParameterInt("redbandtocr");
		const unsigned int l_NIRBandTOCR = GetParameterInt("nirbandtocr");
		const unsigned int l_NIRBandRCR = GetParameterInt("nirbandrcr");
		const double l_WaterNDVIThreshold = GetParameterFloat("waterndvithreshold");
		const double l_WaterReflectanceVariationThreshold = GetParameterFloat("waterreflectancevariationthreshold");
		const double l_MaximumSunglintReflectance = GetParameterFloat("maximumsunglintreflectance");
		const double l_SRTMGaussianSigma = GetParameterFloat("srtmgaussiansigma");
		const double l_SRTMPercent = GetParameterFloat("srtmpercent");
		const unsigned int l_WaterMaskNumber = GetParameterInt("watermasknumber");
		const double l_MinPercentPossibleWater = GetParameterFloat("minpercentpossiblewater");
		const double l_WaterSlopeThreshold = GetParameterFloat("waterslopethreshold");
		const double l_WaterRedReflectanceThreshold= GetParameterFloat("waterredreflectancethreshold");
		const double l_NoData = GetParameterFloat("reall2nodata");

		// Check inputs
		// In nominal mode
		if (l_InitMode == false)
		{

			// Get the input images of the composite (of date D-1)
			l_L2inRCRPtr = this->GetParameterDoubleVectorImage("l2rcr");
			l_L2inPWAPtr = this->GetParameterUInt8VectorImage("l2pwa");
			l_L2inTWAPtr = this->GetParameterUInt8VectorImage("l2twa");

			// L2 input products (composite) exist only in standard mode
			m_PossibleWaterFlagFilter->SetL2inRCRInput(l_L2inRCRPtr);
			m_PossibleWaterFlagFilter->SetNIRBandRCR(l_NIRBandRCR);

			m_SRTMFilter->SetL2inPWAInput(l_L2inPWAPtr);
			m_SRTMFilter->SetL2inTWAInput(l_L2inTWAPtr);

		}

		// Compute the possible water flag for image D by detecting low NDVI
		// Set input images
		m_PossibleWaterFlagFilter->SetIPTOCRInput(l_IPTOCRPtr);
		m_PossibleWaterFlagFilter->SetIPEDGInput(l_IPEDGPtr);
		m_PossibleWaterFlagFilter->SetL2CLDSubSummaryInput(l_L2CLDSubSummaryPtr);
		m_PossibleWaterFlagFilter->SetIPSHDSubInput(l_IPSHDSubPtr);
		m_PossibleWaterFlagFilter->SetSlopeSubInput(l_SlopeSubPtr);

		// Set the parameters
		m_PossibleWaterFlagFilter->SetInitMode(l_InitMode);
		m_PossibleWaterFlagFilter->SetSunglintFlag(l_SunglintFlag);
		m_PossibleWaterFlagFilter->SetRedBandTOCR(l_RedBandTOCR);
		m_PossibleWaterFlagFilter->SetNIRBandTOCR(l_NIRBandTOCR);
		m_PossibleWaterFlagFilter->SetWaterNDVIThreshold(l_WaterNDVIThreshold);
		m_PossibleWaterFlagFilter->SetWaterReflectanceVariationThreshold(l_WaterReflectanceVariationThreshold);
		m_PossibleWaterFlagFilter->SetMaximumSunglintReflectance(l_MaximumSunglintReflectance);
		m_PossibleWaterFlagFilter->SetWaterSlopeThreshold(l_WaterSlopeThreshold);
		m_PossibleWaterFlagFilter->SetWaterRedReflectanceThreshold(l_WaterRedReflectanceThreshold);
		m_PossibleWaterFlagFilter->SetRealL2NoData(l_NoData);

		// Compute the final water flag from the possible water flags
		//Filter the input SRTM with a gaussian
		m_SRTMGaussianFilter->SetInput(l_SRTMWATPtr);
		m_SRTMGaussianFilter->SetVariance(l_SRTMGaussianSigma * l_SRTMGaussianSigma);
		m_SRTMGaussianFilter->SetUseImageSpacingOff();
		m_SRTMGaussianFilter->SetMaximumError(CONST_EPSILON_20);
		m_SRTMGaussianFilter->SetMaximumKernelWidth(static_cast<unsigned int>(40 * l_SRTMGaussianSigma));
		m_SRTMMultiplyFilter->SetInput1(m_SRTMGaussianFilter->GetOutput());
		m_SRTMMultiplyFilter->SetInput2(l_SRTMWATPtr);

		// Set input images
		m_SRTMFilter->SetIPPWAInput(m_PossibleWaterFlagFilter->GetIPPWAOutput());
		m_SRTMFilter->SetIPTWAInput(m_PossibleWaterFlagFilter->GetIPTWAOutput());
		m_SRTMFilter->SetSRTMWATInput(m_SRTMMultiplyFilter->GetOutput());

		// Set the parameters
		m_SRTMFilter->SetInitMode(l_InitMode);
		m_SRTMFilter->SetSRTMPercent(l_SRTMPercent);
		m_SRTMFilter->SetMinPercentPossibleWater(l_MinPercentPossibleWater);
		m_SRTMFilter->SetWaterMaskNumber(l_WaterMaskNumber);

		//Set the outputs
		SetParameterOutputImage<OutputImageType>("was",m_SRTMFilter->GetL2WASOutput());
		SetParameterOutputImage<VectorMaskType>("pwa",m_SRTMFilter->GetL2PWAOutput());
		SetParameterOutputImage<VectorMaskType>("twa",m_SRTMFilter->GetL2TWAOutput());



	}


	/** Filters declaration */
	PossibleWaterFlagPointerFilterType m_PossibleWaterFlagFilter;
	SRTMPointerFilterType m_SRTMFilter;
	GaussianSRTMMaskFilterPointer m_SRTMGaussianFilter;
	SRTMMultiplyFilterPointer m_SRTMMultiplyFilter;

};

}
}

OTB_APPLICATION_EXPORT(vns::Wrapper::WaterMask)
