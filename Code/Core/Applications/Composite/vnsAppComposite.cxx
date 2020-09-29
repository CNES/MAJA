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
#include "vnsCompositeImageFilter.h"
#include "vnsConstantImageSource.h"
#include <string>

namespace vns
{
namespace Wrapper
{

using namespace otb::Wrapper;

class Composite : public Application
{
public:
	/** Standard class typedefs. */
	typedef Composite              Self;
	typedef otb::Wrapper::Application     Superclass;
	typedef itk::SmartPointer<Self>       Pointer;
	typedef itk::SmartPointer<const Self> ConstPointer;

	/** Standard macro */
	itkNewMacro(Self);

	itkTypeMacro(Composite, otb::Wrapper::Application);

	/** Some convenient typedefs. */
	typedef DoubleVectorImageType InputImageType;
	typedef InputImageType::ConstPointer InputImageConstPointer;
	typedef InputImageType::Pointer InputImagePointer;

	typedef UInt8ImageType InputMaskType;
	typedef InputMaskType::Pointer InputMaskPointer;
	typedef InputMaskType::ConstPointer InputMaskConstPointer;

	typedef Int16ImageType DateImageType;
	typedef DateImageType::Pointer DateImagePointer;
	typedef DateImageType::ConstPointer DateImageConstPointer;

	typedef UInt8VectorImageType VectorMaskType;
	typedef VectorMaskType::Pointer VectorMaskPointer;
	typedef VectorMaskType::ConstPointer VectorMaskConstPointer;

	typedef DoubleImageType AOTImageType;
	typedef AOTImageType::Pointer AOTImagePointer;
	typedef AOTImageType::ConstPointer AOTImageConstPointer;

	typedef DoubleVectorImageType OutputImageType;
	typedef OutputImageType::Pointer OutputImagePointer;

	typedef Utilities::ListOfUIntegers ListOfUIntegers;
	/** AOTImageType constant image filter type */
	typedef ConstantImageSource<AOTImageType> AOTConstantImageSourceFilterType;

private:
	void DoInit()
	{
		SetName("Composite");
		SetDescription("Composite algo.");
		Loggers::GetInstance()->Initialize(GetName());
		// Documentation
		SetDocLongDescription("This application computes the composite for the product");
		SetDocLimitations("None");
		SetDocAuthors("MAJA-Team");
		SetDocSeeAlso("MAJA Doc");

		AddDocTag("Statistics");
		//Input Images
		AddParameter(ParameterType_InputImage,  "tocr",   "tocr image");
		SetParameterDescription("tocr", "TOCR image at coarse all bands");
		AddParameter(ParameterType_InputImage,  "toac",   "toac image");
		SetParameterDescription("toac", "TOAC image at coarse all bands");
		AddParameter(ParameterType_InputImage,  "toc",   "toc image");
		SetParameterDescription("toc", "TOC image at coarse all bands");
		AddParameter(ParameterType_InputImage,  "edg",   "EDG image");
		SetParameterDescription("edg", "Image used as edge at coarse");
		AddParameter(ParameterType_InputImage,  "cldsum",   "Cloud Sum");
		SetParameterDescription("cldsum", "Cloud sum at coarse");
		AddParameter(ParameterType_InputImage,  "sat",   "SAT");
		SetParameterDescription("sat", "Saturated");
		AddParameter(ParameterType_InputImage,  "snow",   "SnowMask");
		SetParameterDescription("snow", "Snow mask at coarse");
		MandatoryOff("snow");
		AddParameter(ParameterType_InputImage,  "aot",   "AOT");
		SetParameterDescription("aot", "AOT at coarse");
		MandatoryOff("aot");
		//Previous L2
		AddParameter(ParameterType_InputImage,  "l2rta",   "L2 RTA");
		SetParameterDescription("l2rta", "L2 RTA (optional)");
		MandatoryOff("l2rta");
		AddParameter(ParameterType_InputImage,  "l2rcr",   "L2 RCR");
		SetParameterDescription("l2rcr", "L2 RCR (optional)");
		MandatoryOff("l2rcr");
		AddParameter(ParameterType_InputImage,  "l2rtc",   "L2 RTC");
		SetParameterDescription("l2rtc", "L2 RTC (optional)");
		MandatoryOff("l2rtc");
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
		AddParameter(ParameterType_Bool,  "cirrusflag","CirrusFlag");
		// Hotspot flag parameter accessors
		AddParameter(ParameterType_Bool,  "hotspotflag","HotSpotFlag");
		// rain flag parameter accessors
		AddParameter(ParameterType_Bool,  "rainflag","RainFlag");
		AddParameter(ParameterType_StringList, "bandlist", "Composite Band List");
		MandatoryOff("bandlist");
		AddParameter(ParameterType_Int, "stobandnumber", "StoBandNumber");
		AddParameter(ParameterType_Int, "stobandindex", "StoBandIndex");
		AddParameter(ParameterType_Int, "aotradius", "AOTRadius");
		AddParameter(ParameterType_Float, "maxaot", "maxaot");
		AddParameter(ParameterType_Float, "minaot", "minaot");
		AddParameter(ParameterType_Int, "sigmaaot", "sigmaaot");
		AddParameter(ParameterType_Float, "defaultaot", "default aot");
		MandatoryOff("defaultaot");
		AddParameter(ParameterType_Int, "maxage", "maxage");
		AddParameter(ParameterType_Int, "jday", "jday");
		// No data Real
		AddParameter(ParameterType_Float,  "reall2nodata", "RealL2NoData");
		// No data
		AddParameter(ParameterType_Float,  "l2nodata", "L2NoData");


		AddParameter(ParameterType_OutputImage, "rta", "rta");
		SetParameterDescription("rta","rta image");
		SetParameterOutputImagePixelType("rta", ImagePixelType_double);
		AddParameter(ParameterType_OutputImage, "rcr", "RCR");
		SetParameterDescription("rcr","RCR Image");
		SetParameterOutputImagePixelType("rcr", ImagePixelType_double);
		AddParameter(ParameterType_OutputImage, "rtc", "RTC");
		SetParameterDescription("rtc","RTC");
		SetParameterOutputImagePixelType("rtc", ImagePixelType_double);
		AddParameter(ParameterType_OutputImage, "sto", "STO");
		SetParameterDescription("sto","STO");
		SetParameterOutputImagePixelType("sto", ImagePixelType_double);
		AddParameter(ParameterType_OutputImage, "pxd", "PXD");
		SetParameterDescription("pxd","PXD");
		SetParameterOutputImagePixelType("pxd", ImagePixelType_int16);
		AddParameter(ParameterType_OutputImage, "ndt", "NDT");
		SetParameterDescription("ndt","NDT");
		SetParameterOutputImagePixelType("ndt", ImagePixelType_uint8);


		AddRAMParameter("ram");
		SetDefaultParameterInt("ram",2048);
	}

	void DoUpdateParameters()
	{

	}


	void DoExecute()
	{
		// Filter initialization
		m_CompositeImage = CompositeImageFilter::New();
		m_AOTDefaultConstantImageFilter = AOTConstantImageSourceFilterType::New();
		// Load inputs
		InputImagePointer l_IPTOCRPtr = this->GetParameterDoubleVectorImage("tocr");
		InputImagePointer l_IPTOACPtr = this->GetParameterDoubleVectorImage("toac");
		InputImagePointer l_IPTOCPtr = this->GetParameterDoubleVectorImage("toc");
		InputMaskPointer l_IPEDGPtr =  this->GetParameterUInt8Image("edg");
		VectorMaskPointer l_IPSAT = this->GetParameterUInt8VectorImage("sat");
		InputMaskPointer l_IPCLDSum = this->GetParameterUInt8Image("cldsum");
		AOTImagePointer l_IPAOT;
		InputMaskPointer l_IPSNW;
		InputImagePointer l_L2RTA;
    	InputImagePointer l_L2RCR;
        InputImagePointer l_L2RTC;
        InputImagePointer l_L2STO;
        DateImagePointer l_L2PXD;
        InputMaskPointer l_L2NDT;

		//Get parameters
		const bool l_InitMode = GetParameterInt("initmode");
		const bool l_RainFlag = GetParameterInt("rainflag");
		const bool l_CirrusFlag = GetParameterInt("cirrusflag");
		const bool l_HotspotFlag = GetParameterInt("hotspotflag");
		/** Real L2 No_Data parameter */
		const RealNoDataType l_RealL2NoData = GetParameterFloat("reall2nodata");
		/** L2 No_Data parameter */
		const NoDataType l_L2NoData = GetParameterFloat("l2nodata");
		// HighAOTImageFilter parameters
		/** Coefficient for the threshold to detect high AOT */
		const unsigned short l_CoefSigmaAOT = GetParameterInt("sigmaaot");
		/** Std max under which the std of the AOT image is not taking into
		 * account in the calculation of the high AOT threshold
		 */
		const double l_MinimumHighAOTStd = GetParameterFloat("minaot");
		// CompositeImageUpdateImageFilter parameters
		/** AOT Threshold parameter */
		const double l_MaxAOT = GetParameterFloat("maxaot");
		/** Maximum age for reflectancein the composite image */
		const unsigned short l_MaxAge = GetParameterInt("maxage");
		/** Acquisition date in julian days */
		const short l_JDay = GetParameterInt("jday");
		/** Composite bands to use */
		Utilities::ListOfUIntegers l_BandIdx;
		if(HasValue("bandlist"))
		{
			const std::vector<std::string> l_BandIdxStr = GetParameterStringList("bandlist");
			for (unsigned int i = 0; i < l_BandIdxStr.size();i++)
			{
				l_BandIdx.push_back(Utilities::AsUnsignedInt(l_BandIdxStr[i]));
			}
		}
		/** Number of bands of the STO images */
		const unsigned int l_STOBandNumber = GetParameterInt("stobandnumber");
		/** RCR band index set in the STO image */
		const unsigned int l_STOBandIndex = GetParameterInt("stobandindex");
		/** SE AOT radius parameter */
		const unsigned long l_SEAOTRadius = GetParameterInt("aotradius");


		//SetImages
        m_CompositeImage->SetIPTOACInput(l_IPTOACPtr);
        m_CompositeImage->SetIPTOCRInput(l_IPTOCRPtr);
        m_CompositeImage->SetIPTOCInput(l_IPTOCPtr);
        m_CompositeImage->SetIPEDGInput(l_IPEDGPtr);
        m_CompositeImage->SetIPCLDSumInput(l_IPCLDSum);
        m_CompositeImage->SetIPSATSubInput(l_IPSAT);

        if (HasValue("snow"))
        {
        	l_IPSNW = this->GetParameterUInt8Image("snow");
        	m_CompositeImage->SetIPSNWSubInput(l_IPSNW);
        }

        // Same as : if ((p_mode.IsInit() == true) && (l_AOTMethod == Utilities::MULTITEMPORAL)) or image is too cloudy or snowy
        if( !HasValue("aot")) {
        	/** AOT default value parameter */
        	const double l_DefaultAOT = GetParameterFloat("defaultaot");
        	m_AOTDefaultConstantImageFilter->SetConstantValue(l_DefaultAOT);
        	m_AOTDefaultConstantImageFilter->SetSpacing(l_IPTOCRPtr->GetSpacing());
        	m_AOTDefaultConstantImageFilter->SetOrigin(l_IPTOCRPtr->GetOrigin());
        	m_AOTDefaultConstantImageFilter->SetSize(l_IPTOCPtr->GetLargestPossibleRegion().GetSize());
        	m_CompositeImage->SetIPAOTSubInput(m_AOTDefaultConstantImageFilter->GetOutput());
        }
        else
        {
        	l_IPAOT = this->GetParameterDoubleImage("aot");
        	m_CompositeImage->SetIPAOTSubInput(l_IPAOT);
        }
        if (!l_InitMode)
        {
    		l_L2RTA = this->GetParameterDoubleVectorImage("l2rta");
        	l_L2RCR = this->GetParameterDoubleVectorImage("l2rcr");
            l_L2RTC = this->GetParameterDoubleVectorImage("l2rtc");
            l_L2STO = this->GetParameterDoubleVectorImage("l2sto");
            l_L2PXD = this->GetParameterInt16Image("l2pxd");
            l_L2NDT = this->GetParameterUInt8Image("l2ndt");
        	m_CompositeImage->SetL2inRTAInput(l_L2RTA);
            m_CompositeImage->SetL2inRCRInput(l_L2RCR);
            m_CompositeImage->SetL2inRTCInput(l_L2RTC);
            m_CompositeImage->SetL2inSTOInput(l_L2STO);
            m_CompositeImage->SetL2inPXDInput(l_L2PXD);
            m_CompositeImage->SetL2inNDTInput(l_L2NDT);
        }

        // Set parameters
        // --------------------------------------------------
        m_CompositeImage->SetListOfBandIndexForComposite(l_BandIdx);
        m_CompositeImage->SetInitMode(l_InitMode);
        m_CompositeImage->SetRealL2NoData(l_RealL2NoData);
        m_CompositeImage->SetL2NoData(l_L2NoData);
        m_CompositeImage->SetCoefSigmaAOT(l_CoefSigmaAOT);
        m_CompositeImage->SetMinimumHighAOTStd(l_MinimumHighAOTStd);
        m_CompositeImage->SetMaxAOT(l_MaxAOT);
        m_CompositeImage->SetMaxAge(l_MaxAge);
        m_CompositeImage->SetHotspotFlag(l_HotspotFlag);
        m_CompositeImage->SetCirrusFlag(l_CirrusFlag);
        m_CompositeImage->SetRainFlag(l_RainFlag);
        m_CompositeImage->SetJDay(static_cast<short>(l_JDay));
        m_CompositeImage->SetSTOBandNumber(l_STOBandNumber);
        m_CompositeImage->SetSTOBandIndex(l_STOBandIndex);
        m_CompositeImage->SetSEAOTRadius(l_SEAOTRadius);
        m_CompositeImage->UpdateWithCaching();

		//Set the outputs
		SetParameterOutputImage<OutputImageType>("rta",m_CompositeImage->GetL2RTAOutput());
		SetParameterOutputImage<OutputImageType>("rcr",m_CompositeImage->GetL2RCROutput());
		SetParameterOutputImage<OutputImageType>("rtc",m_CompositeImage->GetL2RTCOutput());
		SetParameterOutputImage<OutputImageType>("sto",m_CompositeImage->GetL2STOOutput());
		SetParameterOutputImage<DateImageType>("pxd",m_CompositeImage->GetL2PXDOutput());
		SetParameterOutputImage<InputMaskType>("ndt",m_CompositeImage->GetL2NDTOutput());



	}


	/** Filters declaration */
	CompositeImageFilter::Pointer m_CompositeImage;
	AOTConstantImageSourceFilterType::Pointer m_AOTDefaultConstantImageFilter;
};

}
}

OTB_APPLICATION_EXPORT(vns::Wrapper::Composite)
