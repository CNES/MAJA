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
#include "itkBinaryDilateImageFilter.h"
#include "itkBinaryErodeImageFilter.h"
#include "itkBinaryBallStructuringElement.h"
#include "itkBinaryFunctorImageFilter.h"
#include "itkOrImageFilter.h"
#include "vnsReflectanceThresholdFunctor.h"
#include "vnsReflectanceVariationCorrelationImageFilter.h"
#include "otbQuaternaryFunctorImageFilter.h"
#include "vnsConstantImageSource.h"
#include <string>

namespace vns
{
namespace Wrapper
{

using namespace otb::Wrapper;

class CloudReflectance : public Application
{
public:
	/** Standard class typedefs. */
	typedef CloudReflectance           Self;
	typedef otb::Wrapper::Application     Superclass;
	typedef itk::SmartPointer<Self>       Pointer;
	typedef itk::SmartPointer<const Self> ConstPointer;

	/** Standard macro */
	itkNewMacro(Self);

	itkTypeMacro(CloudReflectance, otb::Wrapper::Application);

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
	/** Filter Types */
	typedef itk::OrImageFilter<InputMaskType, InputMaskType, InputMaskType> OrFilterType;
	typedef OrFilterType::Pointer OrFilterPointerType;
	// Water mask erosion
	typedef itk::BinaryBallStructuringElement<InputMaskPixelType, InputMaskType::ImageDimension> StructuringElementType;
	typedef itk::BinaryErodeImageFilter<InputMaskType, InputMaskType, StructuringElementType> ErodeFilterType;
	typedef ErodeFilterType::Pointer ErodePointerFilterType;
	// Dilatation of all cloud masks
	typedef itk::BinaryDilateImageFilter<InputMaskType, InputMaskType, StructuringElementType> DilateFilterType;
	typedef DilateFilterType::Pointer DilatePointerFilterType;
	// Filters used to generate images constants
	typedef ConstantImageSource<InputMaskType> ConstantFilterType;
	typedef ConstantFilterType::Pointer ConstantPointerFilterType;
	// Reflectance threshold type
	typedef Functor::ReflectanceThresholdFunctor<InputImagePixelType, VectorMaskPixelType, InputMaskPixelType, InputMaskPixelType> ReflectanceThresholdFunctorType;
	typedef otb::QuaternaryFunctorImageFilter<InputImageType, VectorMaskType, InputMaskType, InputMaskType, InputMaskType,
			ReflectanceThresholdFunctorType> ReflectanceThresholdFilterType;
	typedef ReflectanceThresholdFilterType::Pointer ReflectanceThresholdPointerFilterType;
	// Reflectance variation threshold type
	typedef ReflectanceVariationCorrelationImageFilter<InputImageType, InputMaskType, DateImageType, InputMaskType> ReflectanceVariationCorrelationFilterType;
	typedef ReflectanceVariationCorrelationFilterType::Pointer ReflectanceVariationCorrelationPointerFilterType;

private:
	void DoInit()
	{
		SetName("CloudReflectance");
		SetDescription("Cloud Reflectance algo.");
		Loggers::GetInstance()->Initialize(GetName());
		// Documentation
		SetDocLongDescription("This application computes the cloud reflectance mask");
		SetDocLimitations("None");
		SetDocAuthors("MAJA-Team");
		SetDocSeeAlso("MAJA Doc");

		AddDocTag("Statistics");
		//Input Images
		AddParameter(ParameterType_InputImage,  "tocr",   "tocr image");
		SetParameterDescription("tocr", "TOCR image at coarse all bands");
		AddParameter(ParameterType_InputImage,  "edg",   "EDG image");
		SetParameterDescription("edg", "Image used as edge at coarse");
		AddParameter(ParameterType_InputImage,  "sat",   "SAT");
		SetParameterDescription("sat", "Saturated mask at coarse");
		AddParameter(ParameterType_InputImage,  "snow",   "SnowMask");
		SetParameterDescription("snow", "Snow mask at coarse");
		MandatoryOff("snow");
		//Previous L2
		AddParameter(ParameterType_InputImage,  "l2was",   "L2 WAS");
		SetParameterDescription("l2was", "L2 WAS (optional)");
		MandatoryOff("l2was");
		AddParameter(ParameterType_InputImage,  "l2rcr",   "L2 RCR");
		SetParameterDescription("l2rcr", "L2 RCR (optional)");
		MandatoryOff("l2rcr");
		AddParameter(ParameterType_InputImage,  "l2sto",   "L2 STO");
		SetParameterDescription("l2sto", "L2 STO (optional)");
		MandatoryOff("l2sto");
		AddParameter(ParameterType_InputImage,  "l2pxd",   "L2 PXD");
		SetParameterDescription("l2pxd", "L2 PXD (optional)");
		MandatoryOff("l2pxd");
		AddParameter(ParameterType_InputImage,  "l2ndt",   "L2 NDT");
		SetParameterDescription("l2ndt", "L2 NDT (optional)");
		MandatoryOff("l2ndt");

		//Set parameters
		// Init mode parameter accessors
		AddParameter(ParameterType_Bool,  "initmode","InitMode");
		// cirrus flag parameter accessors
		AddParameter(ParameterType_Int,  "foregroundvalue","ForegroundValue");
		AddParameter(ParameterType_Int,  "bluebandtocr","BlueBandTOCR");
		AddParameter(ParameterType_Int,  "redbandtocr","RedBandTOCR");
		AddParameter(ParameterType_Int,  "nirbandtocr","NIRBandTOCR");
		AddParameter(ParameterType_Int,  "reflectancewaterband","ReflectanceWaterBand");
		AddParameter(ParameterType_Float,  "bluereflectancethresholdvalue","BlueReflectanceThresholdValue");
		AddParameter(ParameterType_Float,  "redreflectancethresholdvalue","RedReflectanceThresholdValue");
		AddParameter(ParameterType_Float,  "nirredratiomaxthresholdvalue","NirRedRatioMaxThresholdValue");
		AddParameter(ParameterType_Float,  "nirredratiominthresholdvalue","NirRedRatioMinThresholdValue");
		AddParameter(ParameterType_Float,  "nodata","NoData");
		AddParameter(ParameterType_Int,  "correlbandtocr","CorrelBandTOCR");
		AddParameter(ParameterType_Float, "waterthreshold","waterthreshold");

		//non init mode
		AddParameter(ParameterType_Group,"nominal","NominalMode");
		MandatoryOff("nominal");
		AddParameter(ParameterType_Int,  "nominal.seradius","seradius");
		MandatoryOff("nominal.seradius");
		AddParameter(ParameterType_Int,  "nominal.bluebandrcr","BlueBandRCR");
		MandatoryOff("nominal.bluebandrcr");
		AddParameter(ParameterType_Int,  "nominal.redbandrcr","RedBandRCR");
		MandatoryOff("nominal.redbandrcr");
		AddParameter(ParameterType_Int,  "nominal.nirbandrcr","NIRBandRCR");
		MandatoryOff("nominal.nirbandrcr");
		AddParameter(ParameterType_Int,  "nominal.correlbandrcr","CorrelBandRCR");
		MandatoryOff("nominal.correlbandrcr");
		AddParameter(ParameterType_Int,  "nominal.jday","JDay");
		MandatoryOff("nominal.jday");
		AddParameter(ParameterType_Int,  "nominal.cloudforgettingduration","CloudForgettingDuration");
		MandatoryOff("nominal.cloudforgettingduration");
		AddParameter(ParameterType_Float,  "nominal.minvarthreshold","MinVarThreshold");
		MandatoryOff("nominal.minvarthreshold");
		AddParameter(ParameterType_Float,  "nominal.maxvarthreshold","MaxVarThreshold");
		MandatoryOff("nominal.maxvarthreshold");
		AddParameter(ParameterType_Float,  "nominal.knirblue","KNIRBlue");
		MandatoryOff("nominal.knirblue");
		AddParameter(ParameterType_Float,  "nominal.kredblue","KRedBlue");
		MandatoryOff("nominal.kredblue");
		AddParameter(ParameterType_Float,  "nominal.deltathreshold","DeltaThreshold");
		MandatoryOff("nominal.deltathreshold");
		AddParameter(ParameterType_Float,  "nominal.minpixelcorrelation","MinPixelCorrelation");
		MandatoryOff("nominal.minpixelcorrelation");
		AddParameter(ParameterType_Float,  "nominal.correlthreshold","CorrelThreshold");
		MandatoryOff("nominal.correlthreshold");
		AddParameter(ParameterType_Float,  "nominal.ncorrel","NCorrel");
		MandatoryOff("nominal.ncorrel");
		AddParameter(ParameterType_StringList, "nominal.stolistofdates","STOListOfDates");
		MandatoryOff("nominal.stolistofdates");
		AddParameter(ParameterType_Group,"swir","swir");
		AddParameter(ParameterType_Int,  "swir.bandtocr","SWIRBandTOCR");
		MandatoryOff("swir.bandtocr");
		AddParameter(ParameterType_Float,  "swir.nirswirratiomin","NirSwirRatioMinThresholdValue");
		MandatoryOff("swir.nirswirratiomin");


		AddParameter(ParameterType_OutputImage, "refl", "Reflectance");
		SetParameterDescription("refl","Reflectance");
		SetParameterOutputImagePixelType("refl", ImagePixelType_uint8);
		AddParameter(ParameterType_OutputImage, "reflvar", "ReflectanceVariation");
		SetParameterDescription("reflvar","ReflectanceVariation");
		SetParameterOutputImagePixelType("reflvar", ImagePixelType_uint8);
		MandatoryOff("reflvar");
		AddParameter(ParameterType_OutputImage, "reflext", "ReflectanceExtension");
		SetParameterDescription("reflext","ReflectanceExtension");
		SetParameterOutputImagePixelType("reflext", ImagePixelType_uint8);
		MandatoryOff("reflext");

		AddRAMParameter("ram");
		SetDefaultParameterInt("ram",2048);
	}

	void DoUpdateParameters()
	{

	}


	void DoExecute()
	{
		// Filter initialization
		m_OrFilter = OrFilterType::New();
		m_OrFilter2 = OrFilterType::New();
		// Threshold filter
		m_ReflectanceThresholdFilter = ReflectanceThresholdFilterType::New();
		m_ReflectanceVariationCorrelationFilter = ReflectanceVariationCorrelationFilterType::New();
		//Morpho
		m_DilateWaterMaskFilter = DilateFilterType::New();
		m_DilateSnowMaskFilter = DilateFilterType::New();
		m_ErodeWaterMaskFilter = ErodeFilterType::New();
		//Constant gen ask
		m_ConstantMaskFilter = ConstantFilterType::New();
		// Load inputs
		InputImageConstPointer l_IPTOCRPtr = this->GetParameterDoubleVectorImage("tocr");
		InputMaskConstPointer l_IPEDGPtr =  this->GetParameterUInt8Image("edg");
		VectorMaskConstPointer l_IPSATPtr = this->GetParameterUInt8VectorImage("sat");
		InputMaskConstPointer l_L2inWASPtr = ITK_NULLPTR;
		InputImageConstPointer l_L2inRCRPtr = ITK_NULLPTR;
		InputImageConstPointer l_L2inSTOPtr = ITK_NULLPTR;
		DateImageConstPointer l_L2inPXDPtr = ITK_NULLPTR;
		InputMaskConstPointer l_L2inNDTPtr = ITK_NULLPTR;
		InputMaskPointer l_IPSNW = ITK_NULLPTR;

		//Get parameters
		const bool l_InitMode = GetParameterInt("initmode");
		vnsLogInfoMacro("InitMode: "<<Utilities::BoolToLowerString(l_InitMode));
		const unsigned int m_ForegroundValue = GetParameterInt("foregroundvalue");
		const unsigned int m_BlueBandTOCR = GetParameterInt("bluebandtocr");
		const unsigned int m_RedBandTOCR = GetParameterInt("redbandtocr");
		const unsigned int m_NIRBandTOCR = GetParameterInt("nirbandtocr");
		const unsigned int m_ReflectanceWaterBand = GetParameterInt("reflectancewaterband");
		const double m_BlueReflectanceThresholdValue = GetParameterFloat("bluereflectancethresholdvalue");
		const double m_RedReflectanceThresholdValue = GetParameterFloat("redreflectancethresholdvalue");
		const double m_NirRedRatioMaxThresholdValue = GetParameterFloat("nirredratiomaxthresholdvalue");
		const double m_NirRedRatioMinThresholdValue = GetParameterFloat("nirredratiominthresholdvalue");
		const double m_NoData = GetParameterFloat("nodata");
		const unsigned int m_CorrelBandTOCR = GetParameterInt("correlbandtocr");
		const double l_WaterThreshold = GetParameterFloat("waterthreshold");
		//Non init optional
		unsigned int m_BlueBandRCR = 0;
		unsigned int m_RedBandRCR = 0;
		unsigned int m_NIRBandRCR = 0;
		unsigned int m_CorrelBandRCR = 0;
		unsigned int m_JDay = 0;
		Utilities::ListOfDoubles m_STOListOfDates;
		unsigned int m_CloudForgettingDuration = 0;
		double m_MinVarThreshold = 0;
		double m_MaxVarThreshold = 0;
		double m_KNIRBlue = 0;
		double m_KRedBlue = 0;
		double m_DeltaThreshold = 0;
		double m_MinPixelCorrelation = 0;
		double m_CorrelThreshold = 0;
		double m_NCorrel = 0;
		unsigned int m_SEInputMasksRadius = 0;

		//Swir optional
		unsigned int m_SWIRBandTOCR = 0;
		double m_NirSwirRatioMinThresholdValue = 0;
		if(HasValue("swir.bandtocr"))
		{
			m_SWIRBandTOCR = GetParameterInt("swir.bandtocr");
			m_NirSwirRatioMinThresholdValue = GetParameterFloat("swir.nirswirratiomin");
		}
		//SetImages from L2
		if(!l_InitMode)
		{
			if(!HasValue("l2was"))
			{
				vnsExceptionDataMacro("No input l2was");
			}
			l_L2inWASPtr = this->GetParameterUInt8Image("l2was");
			if(!HasValue("l2rcr"))
			{
				vnsExceptionDataMacro("No input l2rcr");
			}
			l_L2inRCRPtr = this->GetParameterDoubleVectorImage("l2rcr");
			if(!HasValue("l2sto"))
			{
				vnsExceptionDataMacro("No input l2sto");
			}
			l_L2inSTOPtr = this->GetParameterDoubleVectorImage("l2sto");
			if(!HasValue("l2pxd"))
			{
				vnsExceptionDataMacro("No input l2pxd");
			}
			l_L2inPXDPtr = this->GetParameterInt32Image("l2pxd");
			if(!HasValue("l2ndt"))
			{
				vnsExceptionDataMacro("No input l2ndt");
			}
			l_L2inNDTPtr = this->GetParameterUInt8Image("l2ndt");

			if(!HasValue("reflvar"))
			{
				vnsExceptionDataMacro("No input reflvar filename");
			}
			if(!HasValue("reflext"))
			{
				vnsExceptionDataMacro("No input reflext filename");
			}


			m_CloudForgettingDuration = GetParameterInt("nominal.cloudforgettingduration");
			m_MinVarThreshold = GetParameterFloat("nominal.minvarthreshold");
			m_MaxVarThreshold = GetParameterFloat("nominal.maxvarthreshold");
			m_KNIRBlue = GetParameterFloat("nominal.knirblue");
			m_KRedBlue = GetParameterFloat("nominal.kredblue");
			m_DeltaThreshold = GetParameterFloat("nominal.deltathreshold");
			m_MinPixelCorrelation = GetParameterFloat("nominal.minpixelcorrelation");
			m_CorrelThreshold = GetParameterFloat("nominal.correlthreshold");
			m_NCorrel = GetParameterFloat("nominal.ncorrel");
			m_SEInputMasksRadius = GetParameterInt("nominal.seradius");
			const std::vector<std::string> l_ListOfDatesStr = GetParameterStringList("nominal.stolistofdates");
			for (unsigned int i = 0; i < l_ListOfDatesStr.size();i++)
			{
				m_STOListOfDates.push_back(Utilities::AsDouble(l_ListOfDatesStr[i]));
			}
			m_BlueBandRCR = GetParameterInt("nominal.bluebandrcr");
			m_RedBandRCR = GetParameterInt("nominal.redbandrcr");
			m_NIRBandRCR = GetParameterInt("nominal.nirbandrcr");
			m_CorrelBandRCR = GetParameterInt("nominal.correlbandrcr");
			m_JDay = GetParameterInt("nominal.jday");
		}
		//Or filter to take into account the EDG mask and the Snow Mask in the same time.
		//This avoid the creation of a QuaternaryImageFunctor in case where snow mask is present
		//and a TernaryImageFunctor otherwise.
		m_ReflectanceThresholdFilter->SetInput1(l_IPTOCRPtr);
		m_ReflectanceThresholdFilter->SetInput2(l_IPSATPtr);
		if (HasValue("snow"))
		{
			l_IPSNW = this->GetParameterUInt8Image("snow");
			m_OrFilter->SetInput1(l_IPEDGPtr);
			m_OrFilter->SetInput2(l_IPSNW);
			m_ReflectanceThresholdFilter->SetInput3(m_OrFilter->GetOutput());
		}
		else
		{
			m_ReflectanceThresholdFilter->SetInput3(l_IPEDGPtr);
		}
		// In init mode, the reflectance variation threshold test is not performed
		// and the water mask of the composite is not available
		// A constant mask is generate to proceed the treatments
		// This mask is also used when an option is not set and a simulated image has to be used
		m_ConstantMaskFilter->SetSize(l_IPEDGPtr->GetLargestPossibleRegion().GetSize());
		m_ConstantMaskFilter->SetSignedSpacing(l_IPEDGPtr->GetSignedSpacing());
		m_ConstantMaskFilter->SetOrigin(l_IPEDGPtr->GetOrigin());
		m_ConstantMaskFilter->SetConstantValue(static_cast<InputMaskPixelType>(0));
		m_ConstantMaskFilter->UpdateOutputInformation();

		if (l_InitMode == false)
		{
			// Water mask erosion (FTs OGM 114432)
			StructuringElementType l_SEMaskErosion;
			l_SEMaskErosion.SetRadius(m_SEInputMasksRadius); // 3x3 structuring element
			l_SEMaskErosion.CreateStructuringElement();

			m_ErodeWaterMaskFilter->SetInput(l_L2inWASPtr);
			m_ErodeWaterMaskFilter->SetErodeValue(1);
			m_ErodeWaterMaskFilter->SetKernel(l_SEMaskErosion);
			m_ErodeWaterMaskFilter->UpdateOutputInformation();

			m_ReflectanceThresholdFilter->SetInput4(m_ErodeWaterMaskFilter->GetOutput());
		}
		else
		{
			// The Water_mask_sub(D-1) doesn't exist
			m_ReflectanceThresholdFilter->SetInput4(m_ConstantMaskFilter->GetOutput());
		}

		m_ReflectanceThresholdFilter->GetFunctor().SetForegroundValue(m_ForegroundValue);
		m_ReflectanceThresholdFilter->GetFunctor().SetBlueBand(m_BlueBandTOCR);
		m_ReflectanceThresholdFilter->GetFunctor().SetRedBand(m_RedBandTOCR);
		m_ReflectanceThresholdFilter->GetFunctor().SetNirBand(m_NIRBandTOCR);
		if (HasValue("swir.bandtocr"))
		{
			m_ReflectanceThresholdFilter->GetFunctor().SetSwirBand(m_SWIRBandTOCR);
			m_ReflectanceThresholdFilter->GetFunctor().SetNirSwirRatioMinThreshold(m_NirSwirRatioMinThresholdValue);
		}
		m_ReflectanceThresholdFilter->GetFunctor().SetWaterBand(m_ReflectanceWaterBand);
		m_ReflectanceThresholdFilter->GetFunctor().SetBlueReflectanceThreshold(m_BlueReflectanceThresholdValue);
		m_ReflectanceThresholdFilter->GetFunctor().SetRedReflectanceThreshold(m_RedReflectanceThresholdValue);
		m_ReflectanceThresholdFilter->GetFunctor().SetNirRedRatioMaxThreshold(m_NirRedRatioMaxThresholdValue);
		m_ReflectanceThresholdFilter->GetFunctor().SetNirRedRatioMinThreshold(m_NirRedRatioMinThresholdValue);
		m_ReflectanceThresholdFilter->GetFunctor().SetWaterReflectanceThreshold(l_WaterThreshold);
		if (l_InitMode == false)
		{

			// Definition of the structuring element used for the dilatation of the water and the snow masks
			StructuringElementType l_SEMask;
			l_SEMask.SetRadius(m_SEInputMasksRadius); // 3x3 structuring element
			l_SEMask.CreateStructuringElement();

			// dilatation of water and snow mask
			// Dilate water mask
			m_DilateWaterMaskFilter->SetInput(l_L2inWASPtr);
			m_DilateWaterMaskFilter->SetDilateValue(1);
			m_DilateWaterMaskFilter->SetKernel(l_SEMask);
			m_DilateWaterMaskFilter->UpdateOutputInformation();

			if (HasValue("snow"))
			{
				//Or filter to take into account the Water mask and the Snow Mask in the same time.
				// Dilate snow mask
				m_DilateSnowMaskFilter->SetInput(l_IPSNW);
				m_DilateSnowMaskFilter->SetDilateValue(1);
				m_DilateSnowMaskFilter->SetKernel(l_SEMask);
				m_DilateSnowMaskFilter->UpdateOutputInformation();
				m_OrFilter2->SetInput1(m_DilateWaterMaskFilter->GetOutput());
				m_OrFilter2->SetInput2(m_DilateSnowMaskFilter->GetOutput());
				m_ReflectanceVariationCorrelationFilter->SetConditionalMaskInput(m_OrFilter2->GetOutput());
			}
			else
			{
				m_ReflectanceVariationCorrelationFilter->SetConditionalMaskInput(m_DilateWaterMaskFilter->GetOutput());
			}

			// Reflectance variation threshold only in standard mode
			m_ReflectanceVariationCorrelationFilter->SetIPTOCRInput(l_IPTOCRPtr);
			m_ReflectanceVariationCorrelationFilter->SetL2inRCRInput(l_L2inRCRPtr);
			m_ReflectanceVariationCorrelationFilter->SetL2inSTOInput(l_L2inSTOPtr);

			m_ReflectanceVariationCorrelationFilter->SetL2inPXDInput(l_L2inPXDPtr);
			m_ReflectanceVariationCorrelationFilter->SetIPEDGInput(l_IPEDGPtr);
			m_ReflectanceVariationCorrelationFilter->SetL2inNDTInput(l_L2inNDTPtr);
			m_ReflectanceVariationCorrelationFilter->SetIPCLDReflInput(m_ReflectanceThresholdFilter->GetOutput());

			m_ReflectanceVariationCorrelationFilter->SetBlueBandTOCR(m_BlueBandTOCR);
			m_ReflectanceVariationCorrelationFilter->SetRedBandTOCR(m_RedBandTOCR);
			m_ReflectanceVariationCorrelationFilter->SetNIRBandTOCR(m_NIRBandTOCR);
			m_ReflectanceVariationCorrelationFilter->SetNoData(m_NoData);
			m_ReflectanceVariationCorrelationFilter->SetCorrelBandTOCR(m_CorrelBandTOCR);
			m_ReflectanceVariationCorrelationFilter->SetBlueBandRCR(m_BlueBandRCR);
			m_ReflectanceVariationCorrelationFilter->SetRedBandRCR(m_RedBandRCR);
			m_ReflectanceVariationCorrelationFilter->SetNIRBandRCR(m_NIRBandRCR);
			m_ReflectanceVariationCorrelationFilter->SetCorrelBandRCR(m_CorrelBandRCR);
			m_ReflectanceVariationCorrelationFilter->SetJDay(m_JDay);
			m_ReflectanceVariationCorrelationFilter->SetSTOListOfDates(m_STOListOfDates);
			m_ReflectanceVariationCorrelationFilter->SetForgettingDuration(m_CloudForgettingDuration);
			m_ReflectanceVariationCorrelationFilter->SetMinVarThreshold(m_MinVarThreshold);
			m_ReflectanceVariationCorrelationFilter->SetMaxVarThreshold(m_MaxVarThreshold);
			m_ReflectanceVariationCorrelationFilter->SetKNIRBlue(m_KNIRBlue);
			m_ReflectanceVariationCorrelationFilter->SetKRedBlue(m_KRedBlue);
			m_ReflectanceVariationCorrelationFilter->SetDeltaThreshold(m_DeltaThreshold);
			m_ReflectanceVariationCorrelationFilter->SetMinPixelCorrelation(m_MinPixelCorrelation);
			m_ReflectanceVariationCorrelationFilter->SetCorrelThreshold(m_CorrelThreshold);
			typedef itk::Size<InputImageType::ImageDimension> RadiusSizeType;
			RadiusSizeType l_Correl;
			l_Correl.Fill(m_NCorrel);
			m_ReflectanceVariationCorrelationFilter->SetNCorrel(l_Correl);
			SetParameterOutputImage<InputMaskType>("refl", m_ReflectanceVariationCorrelationFilter->GetIPCLDReflOutput());
			SetParameterOutputImage<InputMaskType>("reflvar", m_ReflectanceVariationCorrelationFilter->GetIPCLDReflVarOutput());
			SetParameterOutputImage<InputMaskType>("reflext", m_ReflectanceVariationCorrelationFilter->GetIPCLDExtOutput());

		} else
		{
			SetParameterOutputImage<InputMaskType>("refl", m_ReflectanceThresholdFilter->GetOutput());
			SetParameterOutputImage<InputMaskType>("reflvar", m_ReflectanceThresholdFilter->GetOutput());
			SetParameterOutputImage<InputMaskType>("reflext", m_ReflectanceThresholdFilter->GetOutput());
		}
	}


	/** Filters declaration */
	ReflectanceThresholdPointerFilterType m_ReflectanceThresholdFilter;
	ReflectanceVariationCorrelationPointerFilterType m_ReflectanceVariationCorrelationFilter;
	OrFilterPointerType m_OrFilter;
	OrFilterPointerType m_OrFilter2;
	DilatePointerFilterType m_DilateWaterMaskFilter;
	DilatePointerFilterType m_DilateSnowMaskFilter;
	ErodePointerFilterType m_ErodeWaterMaskFilter;
	ConstantPointerFilterType m_ConstantMaskFilter;

};

}
}

OTB_APPLICATION_EXPORT(vns::Wrapper::CloudReflectance)
