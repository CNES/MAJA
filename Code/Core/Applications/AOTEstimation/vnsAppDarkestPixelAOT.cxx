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
#include "vnsDarkestPixelAOTCalculator.h"
#include "vnsSimpleLutXMLFileHandler.h"
#include "itkImageFileReader.h"
#include "vnsVectorLookUpTable.h"
#include "vnsUtilities.h"
#include "vnsAtomicRegistry.h"

namespace vns
{
namespace Wrapper
{

using namespace otb::Wrapper;

class DarkestPixelAOT : public Application
{
public:
	/** Standard class typedefs. */
	typedef DarkestPixelAOT Self;
	typedef otb::Wrapper::Application     Superclass;
	typedef itk::SmartPointer<Self>       Pointer;
	typedef itk::SmartPointer<const Self> ConstPointer;

	/** Standard macro */
	itkNewMacro(Self);

	itkTypeMacro(DarkestPixelAOT, otb::Wrapper::Application);

	/** Image typedef */
	/** Some convenient typedefs. */
	typedef DoubleVectorImageType InputImageType;
	typedef InputImageType::ConstPointer InputImageConstPointerType;
	typedef InputImageType::Pointer InputImagePointerType;
	typedef InputImageType::PixelType InputImagePixelType;
	typedef InputImageType::InternalPixelType InternalPixelType;
	/** Image information : spacing, origin, region... typedef */
	typedef InputImageType::RegionType RegionType;
	typedef InputImageType::IndexType IndexType;
	typedef InputImageType::SizeType SizeType;
	typedef InputImageType::OffsetType OffsetType;
	typedef InputImageType::SpacingType SpacingType;
	typedef InputImageType::PointType OriginPointType;
	typedef InputImageType::DirectionType DirectionType;

	typedef UInt8ImageType InputMaskType;
	typedef InputMaskType::Pointer InputMaskPointerType;
	typedef InputMaskType::ConstPointer InputMaskConstPointerType;
	typedef InputMaskType::PixelType InputMaskPixelType;

	typedef DoubleImageType InputDTMType;
	typedef InputDTMType::Pointer InputDTMPointerType;
	typedef InputDTMType::ConstPointer InputDTMConstPointerType;
	typedef InputDTMType::PixelType InputDTMPixelType;
	typedef InputDTMType::InternalPixelType InputDTMInternalPixelType;

	typedef VNSLUT3DType LUTType;
	typedef LUTType::Pointer LUTPointerType;
	typedef LUTType::ConstPointer LUTConstPointerType;
	typedef LUTType::PixelType LUTPixelType;
	typedef LUTType::ParameterValuesType ParameterValuesType;
	typedef itk::ImageFileReader<LUTType> ImageFileReaderType;

	/**LutXml handler typedef */
	typedef SimpleLutXMLFileHandler LutXMLHandlerType;
	typedef LutXMLHandlerType::Pointer LutXMLHandlerPointer;

	typedef DarkestPixelAOTCalculator<InputImageType, InputDTMType, InputMaskType, LUTType> DarkestPixelAOTCalculatorType;
	typedef DarkestPixelAOTCalculatorType::Pointer DarkestPixelAOTCalculatorPointerType;


private:
	void DoInit()
	{
		SetName("DarkestPixelAOT");
		SetDescription("Generate the darkest pixel aot.");
		Loggers::GetInstance()->Initialize(GetName());
		// Documentation
		SetDocLongDescription("This application computes the darkest pixel aot");
		SetDocLimitations("None");
		SetDocAuthors("MAJA-Team");
		SetDocSeeAlso("MAJA Doc");

		AddDocTag("Statistics");

		AddParameter(ParameterType_InputImage,  "toac",   "TOAC image");
		SetParameterDescription("toac", "TOAC image at coarse all bands");
		AddParameter(ParameterType_InputImage,  "cld",   "CLD image");
		SetParameterDescription("cld", "CLD image at coarse");
		AddParameter(ParameterType_InputImage,  "dtm",   "dtm image");
		SetParameterDescription("dtm", "dtm of the image");
		AddParameter(ParameterType_InputImage,  "edg",   "edg image");
		SetParameterDescription("edg", "EDG mask");
		AddParameter(ParameterType_InputImage,  "shd",   "shadow image");
		SetParameterDescription("edg", "EDG mask");
		AddParameter(ParameterType_InputImage,  "l2ndt",   "L2 nodata");
		SetParameterDescription("l2ndt", "L2 nodata");
		MandatoryOff("l2ndt");
		AddParameter(ParameterType_InputImage,  "l2rtc",   "L2 rtc");
		MandatoryOff("l2rtc");
		SetParameterDescription("l2rtc", "L2 rtc");
		AddParameter(ParameterType_String,  "lutheader",   "Lut Header");
		SetParameterDescription("lutheader", "Lut Header");
		AddParameter(ParameterType_String,  "lutimage",   "Lut image");
		SetParameterDescription("lutimage", "Lut image");

		AddParameter(ParameterType_Bool, "init","init mode");
		AddParameter(ParameterType_Int, "darkestchanneltoac","darkestchanneltoac");
		AddParameter(ParameterType_Float, "thresholddarkpixel","thresholddarkpixel");
		AddParameter(ParameterType_Float, "darksurfacereflectance","darksurfacereflectance");
		AddParameter(ParameterType_Int, "darkestchannelrtc", "darkestchannelrtc");

		AddParameter(ParameterType_Float, "darkestaot","darkestaot");
		SetParameterRole("darkestaot",Role_Output);
		AddParameter(ParameterType_String, "aotcomputed", "aotcomputed");
		SetParameterRole("aotcomputed",Role_Output);

		AddRAMParameter("ram");
		SetDefaultParameterInt("ram",2048);

	}

	void DoUpdateParameters()
	{

	}


	void DoExecute()
	{
		// Init filters
		m_DarkestPixelAOTCalculator = DarkestPixelAOTCalculatorType::New();
		//Get image pointers
		InputMaskConstPointerType l_L2CLDSub = GetParameterUInt8Image("cld");
		InputMaskConstPointerType l_IPSHDSub = GetParameterUInt8Image("shd");
		InputMaskConstPointerType l_IPEDG = GetParameterUInt8Image("edg");
		InputImageConstPointerType l_IPTOACSub = GetParameterDoubleVectorImage("toac");
		// Get the dtm
		InputDTMConstPointerType l_DTMSub = GetParameterDoubleImage("dtm");
		//d-1
		InputMaskPointerType l_L2inNDT;
		InputImagePointerType l_L2inRTC;


		//GetParams
		const bool l_InitMode = GetParameterInt("init");
		const unsigned int l_DarkestChannelTOAC = GetParameterInt("darkestchanneltoac");
		const double l_ThresholdDarkPixel = GetParameterFloat("thresholddarkpixel");
		const double l_DarkSurfaceReflectance = GetParameterFloat("darksurfacereflectance");
		const unsigned int l_DarkestChannelRTC = GetParameterInt("darkestchannelrtc");
		//Compute
		if (l_InitMode == false)
		{
			l_L2inNDT = GetParameterUInt8Image("l2ndt");
			l_L2inRTC = GetParameterDoubleVectorImage("l2rtc");
			// Boolean that indicates if the composite product (date D-1) is available
			m_DarkestPixelAOTCalculator->SetL2inIsAvalaible(true);
			m_DarkestPixelAOTCalculator->SetL2inRTC(l_L2inRTC);
			m_DarkestPixelAOTCalculator->SetL2inEDG(l_L2inNDT);

		}
		else // (init mode true)
		{
			// Boolean that indicates if the composite product (date D-1) is available
			m_DarkestPixelAOTCalculator->SetL2inIsAvalaible(false);
		}

		//Load the lut image
		// ---------------------------------------------------------------------------------------------
		m_Reader = ImageFileReaderType::New();
		m_Reader->SetFileName(GetParameterString("lutimage"));
		// ---------------------------------------------------------------------------------------------
		// Load the LUT
		m_Reader->Update();
		//Lut
		LUTPointerType l_LUT = m_Reader->GetOutput();

		//Load the lut header
		const std::string l_GIPPFilename = GetParameterString("lutheader");
		LutXMLHandlerPointer l_LUTXMLHandler = LutXMLHandlerType::New();
		// Load the XML file and check with the schema
		l_LUTXMLHandler->LoadFile(l_GIPPFilename);
		const SimpleLutXMLContainer& l_lutxml = l_LUTXMLHandler->GetLutContainer();

		// Read informations from the GIPP file
		const Utilities::ListOfDoubles l_GIP_L2TOCR_AltitudeIndexes = l_lutxml.GetAltitude_Indexes();
		const Utilities::ListOfDoubles l_GIP_L2TOCR_AOTIndexes = l_lutxml.GetAOT_Indexes();
		const Utilities::ListOfDoubles l_GIP_L2TOCR_TOAReflectanceIndexes = l_lutxml.GetTOA_Reflectance_Indexes();

		vnsLogDebugMacro("  l_GIP_L2TOCR_AltitudeIndexes: "<<l_GIP_L2TOCR_AltitudeIndexes.size());
		vnsLogDebugMacro("  l_GIP_L2TOCR_AOTIndexes: "<<l_GIP_L2TOCR_AOTIndexes.size());
		vnsLogDebugMacro("  l_GIP_L2TOCR_TOAReflectanceIndexes: "<<l_GIP_L2TOCR_TOAReflectanceIndexes.size());

		/* parameters are added one by one to the LUT */
		ParameterValuesType l_TOCRParam4;
		l_TOCRParam4.ParameterName = "Altitude_Indexes";
		l_TOCRParam4.ParameterValues = l_GIP_L2TOCR_AltitudeIndexes;
		l_LUT->AddParameterValues(l_TOCRParam4);

		ParameterValuesType l_TOCRParam5;
		l_TOCRParam5.ParameterName = "AOT_Indexes";
		l_TOCRParam5.ParameterValues = l_GIP_L2TOCR_AOTIndexes;
		l_LUT->AddParameterValues(l_TOCRParam5);

		ParameterValuesType l_TOCRParam6;
		l_TOCRParam6.ParameterName = "TOA_Reflectance_Indexes";
		l_TOCRParam6.ParameterValues = l_GIP_L2TOCR_TOAReflectanceIndexes;
		l_LUT->AddParameterValues(l_TOCRParam6);





		// ================================================================
		// Estimate of a likely maximum AOT using the darkest pixel method
		// ================================================================

		// Link DarkestPixelAOTCalculator
		m_DarkestPixelAOTCalculator->SetDarkestChannelTOAC(l_DarkestChannelTOAC);
		m_DarkestPixelAOTCalculator->SetThresholdDarkPixel(l_ThresholdDarkPixel);
		m_DarkestPixelAOTCalculator->SetDarkSurfaceReflectance(l_DarkSurfaceReflectance);
		m_DarkestPixelAOTCalculator->SetLUT(l_LUT);
		m_DarkestPixelAOTCalculator->SetDarkestChannelRTC(l_DarkestChannelRTC);
		m_DarkestPixelAOTCalculator->SetIPTOACSub(l_IPTOACSub);
		m_DarkestPixelAOTCalculator->SetDTMSub(l_DTMSub);
		m_DarkestPixelAOTCalculator->SetL2CLDSub(l_L2CLDSub);
		m_DarkestPixelAOTCalculator->SetIPSHDSub(l_IPSHDSub);
		m_DarkestPixelAOTCalculator->SetIPEDG(l_IPEDG);

		//Launch compute
		m_DarkestPixelAOTCalculator->Update();

		const bool lDarkAOTComputed = m_DarkestPixelAOTCalculator->GetDarkestPixelAOTComputed();
		double lDarkestPixelAOT(0.);

		if (lDarkAOTComputed == true)
		{
			lDarkestPixelAOT = m_DarkestPixelAOTCalculator->GetDarkestPixelAOT();
			vnsLogDebugMacro("DarkestPixelAOT : "<< m_DarkestPixelAOTCalculator->GetDarkestPixelAOT());
			SetParameterFloat("darkestaot", lDarkestPixelAOT);
		}
		else
		{
			// Use the max value of AOT Dark (in the indexes of the LUT) when no AOTDarkPixel is found
			lDarkestPixelAOT = 1.0;

			LUTConstPointerType lLUT = m_DarkestPixelAOTCalculator->GetLUT();
			const unsigned int lParamValuesSize = lLUT->GetParametersValues().size();
			// TODO PERF: Extract GetParametersValues() to a local
			// variable! (a const& depending on GetParametersValues
			// return type)
			for (unsigned int iii = 0; iii < lParamValuesSize; iii++)
			{
				if (lLUT->GetParametersValues()[iii].ParameterName == "AOT_Indexes")
				{
					LUTType::ParameterValuesType::ParamValuesType val = lLUT->GetParametersValues()[iii].ParameterValues;
					lDarkestPixelAOT = static_cast<double>(*std::max_element(val.begin(), val.begin() + val.size()) + 0.1);
				}
			}
			vnsLogWarningMacro(
					"AOTEstimationImageFilter : No darkestPixel found," <<"The AOT computation will use the max AOT value for the DarkestPixelAOT (1.6).");

		}


		//Set outputs
		SetParameterFloat("darkestaot", lDarkestPixelAOT);
		SetParameterString("aotcomputed",vns::Utilities::BoolToLowerString(lDarkAOTComputed));
	}

	/** Filters declaration */
	DarkestPixelAOTCalculatorPointerType m_DarkestPixelAOTCalculator;
	ImageFileReaderType::Pointer m_Reader;

  AtomicRegistry<itk::MetaImageIOFactory> m_MetaRegistry;

};

}
}

OTB_APPLICATION_EXPORT(vns::Wrapper::DarkestPixelAOT)
