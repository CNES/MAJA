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
#include "vnsVectorLookUpTable.h"
#include "vnsUtilities.h"
#include "vnsMultiTemporalAOTMethodThresholdsGenerator.h"
#include "vnsMultiTemporalComputeAOTImageFilter.h"
#include "vnsMultiSpectralComputeAOTImageFilter.h"
#include "vnsSpectroTemporalComputeAOTImageFilter.h"
#include "itkBinaryBallStructuringElement.h"
#include "itkBinaryDilateImageFilter.h"
#include "vnsSimpleAOTParamsXMLFileHandler.h"
#include "vnsSimpleLutXMLFileHandler.h"
#include "vnsEqualValueThresholdFunctor.h"
#include "vnsSimpleLutMapXMLFileHandler.h"
#include "otbGenericRSTransform.h"
#include "itkImageFileReader.h"
#include "itkBinaryMorphologicalOpeningImageFilter.h"
#include "vnsStreamingConditionalStatisticsImageFilter.h"
#include "vnsChangeValueWithMaskFunctor.h"
#include "vnsReplaceValueWithImageFunctor.h"
#include "itkBinaryBallStructuringElement.h"
#include "itkBinaryFunctorImageFilter.h"
#include "itkUnaryFunctorImageFilter.h"
#include "vnsAtomicRegistry.h"


namespace vns
{
enum
{
	multitemporal,
	multispectral,
	spectrotemporal
};

namespace Wrapper
{

using namespace otb::Wrapper;

class AOTEstimation : public Application
{
public:
	/** Standard class typedefs. */
	typedef AOTEstimation Self;
	typedef otb::Wrapper::Application     Superclass;
	typedef itk::SmartPointer<Self>       Pointer;
	typedef itk::SmartPointer<const Self> ConstPointer;

	/** Standard macro */
	itkNewMacro(Self);

	itkTypeMacro(AOTEstimation, otb::Wrapper::Application);

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

	/** Input AOT image typedef */
	typedef DoubleImageType AOTImageType;
	typedef AOTImageType::ConstPointer AOTImageConstPointerType;
	typedef AOTImageType::Pointer AOTImagePointerType;
	typedef AOTImageType::PixelType AOTPixelType;

	typedef UInt8ImageType InputMaskType;
	typedef InputMaskType::Pointer InputMaskPointerType;
	typedef InputMaskType::ConstPointer InputMaskConstPointerType;
	typedef InputMaskType::PixelType InputMaskPixelType;

	typedef UInt8VectorImageType InputVectorMaskType;
	typedef InputVectorMaskType::Pointer InputVectorMaskPointerType;
	typedef InputVectorMaskType::ConstPointer InputVectorMaskConstPointerType;
	typedef InputVectorMaskType::PixelType InputVectorMaskPixelType;

	typedef DoubleImageType InputDTMType;
	typedef InputDTMType::Pointer InputDTMPointerType;
	typedef InputDTMType::ConstPointer InputDTMConstPointerType;
	typedef InputDTMType::PixelType InputDTMPixelType;
	typedef InputDTMType::InternalPixelType InputDTMInternalPixelType;

	typedef Int32ImageType InputDateType;
	typedef InputDateType::Pointer InputDatePointerType;
	typedef InputDateType::ConstPointer InputDateConstPointerType;
	typedef InputDateType::PixelType InputDatePixelType;
	typedef InputDateType::InternalPixelType InputDateInternalPixelType;
	typedef InputDateInternalPixelType DateType;

	typedef VNSLUT3DType LUTType;
	typedef LUTType::Pointer LUTPointerType;
	typedef LUTType::ConstPointer LUTConstPointerType;
	typedef LUTType::PixelType LUTPixelType;
	typedef LUTType::ParameterValuesType ParameterValuesType;
	typedef itk::ImageFileReader<LUTType> ImageFileReaderType;
	typedef std::map<unsigned int, LUTPointerType> LUTMapType;

	/**LutXml handler typedef */
	typedef SimpleLutXMLFileHandler LutXMLHandlerType;
	typedef SimpleLutMapXMLFileHandler LutMapXMLHandlerType;
	typedef LutXMLHandlerType::Pointer LutXMLHandlerPointer;
	typedef LutMapXMLHandlerType::Pointer LutMapXMLHandlerPointer;
	typedef DoubleImageType OutputImageType;
	typedef otb::ObjectList<OutputImageType> OutputImageListType;
	typedef OutputImageListType::Pointer OutputImageListPointerType;
	typedef OutputImageType::Pointer OutputImagePointerType;
	typedef OutputImageType::ConstPointer OutputImageConstPointerType;
	typedef OutputImageType::RegionType OutputImageRegionType;
	typedef OutputImageType::InternalPixelType OutputImagePixelType;
	typedef OutputImageType::InternalPixelType NoDataType;

	typedef UInt8ImageType OutputMaskType;
	typedef otb::ObjectList<OutputMaskType> OutputMaskListType;
	typedef OutputMaskListType::Pointer OutputMaskListPointerType;
	typedef OutputMaskType::Pointer OutputMaskPointerType;
	typedef OutputMaskType::ConstPointer OutputMaskConstPointerType;
	typedef OutputMaskType::PixelType OutputMaskPixelType;
	typedef OutputMaskType::RegionType OutputMaskRegionType;


	/** Filters typedef */
    /** Interpolator typedef. */
    //Projection transform type
    typedef otb::GenericRSTransform<> GEOTransformType;
    typedef itk::LinearInterpolateImageFunction<AOTImageType> AOTInterpolatorType;
    typedef AOTInterpolatorType::ContinuousIndexType AOTContinuousIndexType;

	typedef MultiTemporalAOTMethodThresholdsGenerator<LUTType> MTThreholdsFilterType;
	typedef MTThreholdsFilterType::Pointer MTThreholdsFilterPointerType;

	typedef MultiTemporalComputeAOTImageFilter<InputImageType, InputMaskType, InputVectorMaskType, InputDateType, InputDTMType, LUTType,
			OutputImageType, OutputMaskType> MTComputeAOTImageFilterType;
	typedef MTComputeAOTImageFilterType::Pointer MTComputeAOTImageFilterPointerType;

	typedef MultiSpectralComputeAOTImageFilter<InputImageType, InputMaskType, InputVectorMaskType, InputDTMType, LUTType,
			OutputImageType, OutputMaskType> MSComputeAOTImageFilterType;
	typedef MSComputeAOTImageFilterType::Pointer MSComputeAOTImageFilterPointerType;

	typedef SpectroTemporalComputeAOTImageFilter<InputImageType, InputMaskType, InputVectorMaskType, InputDateType, InputDTMType,
			LUTType, OutputImageType, OutputMaskType> STComputeAOTImageFilterType;
	typedef STComputeAOTImageFilterType::Pointer STComputeAOTImageFilterPointerType;

	typedef itk::BinaryBallStructuringElement<OutputMaskPixelType, 2> KernelType;
	typedef itk::BinaryDilateImageFilter<OutputMaskType, OutputMaskType, KernelType> DilateImageFilterType;
	typedef DilateImageFilterType::Pointer DilateImageFilterPointerType;

	typedef itk::BinaryMorphologicalOpeningImageFilter<OutputMaskType, OutputMaskType, KernelType> BinaryMorphologicalOpeningImageFilterType;
	typedef BinaryMorphologicalOpeningImageFilterType::Pointer BinaryMorphologicalOpeningImageFilterPointerType;

	typedef Functor::ChangeValueWithMaskFunctor<OutputImagePixelType, OutputMaskPixelType, OutputImagePixelType> FunctorType;
	typedef itk::BinaryFunctorImageFilter<OutputImageType, OutputMaskType, OutputImageType, FunctorType> BinaryFunctorImageFilterType;
	typedef BinaryFunctorImageFilterType::Pointer BinaryFunctorImageFilterPointerType;

    // Reflectance threshold type
    typedef Functor::ReplaceValueWithImageFunctor<OutputImagePixelType, OutputImagePixelType> AOTSelectionFunctorType;
    typedef itk::BinaryFunctorImageFilter<OutputImageType, OutputImageType,OutputImageType,AOTSelectionFunctorType> AOTSelectionFilterType;
    typedef AOTSelectionFilterType::Pointer AOTSelectionPointerFilterType;

    typedef vns::Functor::EqualValueThresholdFunctor<OutputMaskPixelType, OutputMaskPixelType> OneBandEqualFunctor;
    typedef itk::UnaryFunctorImageFilter<OutputMaskType, OutputMaskType, OneBandEqualFunctor> OneBandEqualFilterType;
    typedef OneBandEqualFilterType::Pointer OneBandFilterPointerType;

	typedef StreamingConditionalStatisticsImageFilter<OutputImageType> StreamingConditionalStatisticsImageFilterType;
	typedef StreamingConditionalStatisticsImageFilterType::Pointer StreamingConditionalStatisticsImageFilterPointerType;


private:
	void DoInit()
	{
		SetName("AOTEstimation");
		SetDescription("Generate the AOTEstimation.");
		Loggers::GetInstance()->Initialize(GetName());
		// Documentation
		SetDocLongDescription("This application computes the AOT");
		SetDocLimitations("None");
		SetDocAuthors("MAJA-Team");
		SetDocSeeAlso("MAJA Doc");

		AddDocTag("Statistics");

		//Current input
		AddParameter(ParameterType_InputImage,  "toac",   "TOAC image");
		SetParameterDescription("toac", "TOAC image at coarse all bands");
		AddParameter(ParameterType_InputImage,  "sat",   "SAT image");
		SetParameterDescription("sat", "SAT image at coarse");
		AddParameter(ParameterType_InputImage,  "was",   "WAS image");
		SetParameterDescription("was", "WAS image at coarse");
		AddParameter(ParameterType_InputImage,  "dtm",   "dtm image");
		SetParameterDescription("dtm", "dtm of the image");
		AddParameter(ParameterType_InputImage,  "edg",   "edg image");
		SetParameterDescription("edg", "EDG mask");
		AddParameter(ParameterType_InputImage,  "shd",   "shadow image");
		SetParameterDescription("edg", "EDG mask");
		AddParameter(ParameterType_InputImage,  "snow",   "snow image");
		SetParameterDescription("snow", "SNOW mask");

		//L2 inputs
		AddParameter(ParameterType_InputImage,  "l2ndt",   "L2 nodata");
		SetParameterDescription("l2ndt", "L2 nodata");
		MandatoryOff("l2ndt");
		AddParameter(ParameterType_InputImage,  "l2pxd",   "L2 pxd");
		SetParameterDescription("l2pxd", "L2 pxd");
		MandatoryOff("l2pxd");
		AddParameter(ParameterType_InputImage,  "l2rtc",   "L2 rtc");
		SetParameterDescription("l2rtc", "L2 rtc");
		MandatoryOff("l2rtc");
		AddParameter(ParameterType_InputImage,  "l2rta",   "L2 rta");
		SetParameterDescription("l2rta", "L2 rta");
		MandatoryOff("l2rta");

		AddParameter(ParameterType_String,  "lutheader",   "Lut Header");
		SetParameterDescription("lutheader", "Lut Header");
		AddParameter(ParameterType_String,  "lutimage",   "Lut image");
		SetParameterDescription("lutimage", "Lut image");
		AddParameter(ParameterType_InputImage,  "cld",   "CLD image");
		SetParameterDescription("cld", "CLD image at coarse");
		AddParameter(ParameterType_Bool, "init","init mode");

		//Choose the mode
		AddParameter(ParameterType_Choice,"mode", "compute mode");
		AddChoice("mode.multitemporal", "MultiTemporel");
		AddChoice("mode.multispectral", "MultiSpectral");
		AddChoice("mode.spectrotemporal", "Spectro Temporal");

		//Cams related parameters
		AddParameter(ParameterType_Group, "cams","Flag to use cams or not");
		MandatoryOff("cams");
		AddParameter(ParameterType_InputImage,  "cams.aot",   "AOT from CAMS image");
		SetParameterDescription("cams.aot", "AOT from CAMS image");
		AddParameter(ParameterType_Float, "cams.altmean","Mean alt for cams computation");
		AddParameter(ParameterType_Float, "cams.nodatatau","No data to use for cams");

		//All params are in one XML file
		AddParameter(ParameterType_InputFilename,"xmlparamsfile", "XMl containing all the parameters");
		AddParameter(ParameterType_Group,"temporal", "multitemporal mode");
		MandatoryOff("temporal");
		AddParameter(ParameterType_InputFilename, "temporal.lutmap","");
		AddParameter(ParameterType_Float, "temporal.altitudemean","");

		//Common params
		AddParameter(ParameterType_Float, "reall2nodata","");
		AddParameter(ParameterType_Float, "jday","");
		AddParameter(ParameterType_Float, "darkestaot","");

		AddRAMParameter("ram");
		SetDefaultParameterInt("ram",2048);

		AddParameter(ParameterType_OutputImage, "aotcomputed", "aotcomputed");
		AddParameter(ParameterType_OutputImage, "aotmask", "aotmask");
		AddParameter(ParameterType_OutputImage, "aotcams", "aotcams");
		MandatoryOff("aotcams");
		AddParameter(ParameterType_Float, "aotmean", "aotmean");
		SetParameterRole("aotmean", Role_Output);
		AddParameter(ParameterType_String, "aotvalid", "aotvalid");
		SetParameterRole("aotvalid", Role_Output);

	}

	void DoUpdateParameters()
	{

	}


	void DoExecute()
	{
		// Init filters
		m_MTThresholdsFilter = MTThreholdsFilterType::New();
		m_MTComputeAOTImageFilter = MTComputeAOTImageFilterType::New();
		m_MSComputeAOTImageFilter = MSComputeAOTImageFilterType::New();
		m_STComputeAOTImageFilter = STComputeAOTImageFilterType::New();
		m_DilateWaterMaskFilter = DilateImageFilterType::New();
		m_DilateSnowMaskFilter = DilateImageFilterType::New();
		m_BinaryMorphologicalOpeningImageFilter = BinaryMorphologicalOpeningImageFilterType::New();
		m_BinaryFunctorImageFilter = BinaryFunctorImageFilterType::New();
		m_StreamingConditionalStatisticsImageFilter = StreamingConditionalStatisticsImageFilterType::New();
		//AOT Altitude cams functor
		m_AOTSelectionFilter = AOTSelectionFilterType::New();

		//Get image pointers
		InputMaskConstPointerType lL2CLDSub = GetParameterUInt8Image("cld");
		InputMaskConstPointerType lIPSHDSub = GetParameterUInt8Image("shd");
		InputVectorMaskConstPointerType lIPSAT =  GetParameterUInt8VectorImage("sat");
		InputMaskConstPointerType lL2WASSub = GetParameterUInt8Image("was");
		InputMaskConstPointerType lIPEDG = GetParameterUInt8Image("edg");
		InputMaskConstPointerType lIPSNWSub = GetParameterUInt8Image("snow");
		// Get the input images pointers
		InputImageConstPointerType lIPTOACSub = GetParameterDoubleVectorImage("toac");
		// Get the dtm
		InputDTMConstPointerType lDTMSub = GetParameterDoubleImage("dtm");
		// CAMS Image (optional)
		AOTImagePointerType lAOTCamsPtr;

		//Load the xml file
		SimpleAOTParamsXMLFileHandler::Pointer lAOTParamsXMLHandler = SimpleAOTParamsXMLFileHandler::New();
		lAOTParamsXMLHandler->LoadFile(this->GetParameterString("xmlparamsfile"));

		//Cams Params
		bool l_useCams = this->HasValue("cams.aot");
		double l_KPondsCams = 0;
		double l_HeightScale = 2000;
		double l_NoDataTau = 0;
		double l_AltMean = 0;
		AOTImagePointerType l_AOTCamsSub;
		if (l_useCams)
		{
			if (!IsParameterEnabled("aotcams"))
			{
				vnsExceptionDataMacro("aotcams parameter must be set in cas of cams usage !!!");
			}
			l_KPondsCams = lAOTParamsXMLHandler->GetAOTKPondCAMS();
			vnsLogDebugDoubleValueMacro(l_KPondsCams);
			l_HeightScale = lAOTParamsXMLHandler->GetAOTHeightScale();
			vnsLogDebugDoubleValueMacro(l_HeightScale);
			l_NoDataTau = GetParameterFloat("cams.nodatatau");
			vnsLogDebugDoubleValueMacro(l_NoDataTau);
			l_AltMean = GetParameterFloat("cams.altmean");
			vnsLogDebugDoubleValueMacro(l_AltMean);
			lAOTCamsPtr = GetParameterDoubleImage("cams.aot");
			lAOTCamsPtr->UpdateOutputInformation();
			lAOTCamsPtr->SetRequestedRegionToLargestPossibleRegion();
			lAOTCamsPtr->Update();
		}

		InputMaskConstPointerType lL2inNDT;
		InputImageConstPointerType lL2inRTA;
		InputImageConstPointerType lL2inRTC;
		InputDateConstPointerType lL2inPXD;

		//Lut
		//Load the lut image
		// ---------------------------------------------------------------------------------------------
		m_Reader = ImageFileReaderType::New();
		m_Reader->SetFileName(GetParameterString("lutimage"));
		// ---------------------------------------------------------------------------------------------
		// Load the LUT
		m_Reader->Update();
		//Lut
		LUTPointerType l_LUT_JDay = m_Reader->GetOutput();

		//Load the lut header
		const std::string l_GIPPFilename = GetParameterString("lutheader");
		LutXMLHandlerPointer l_LUTXMLHandler = LutXMLHandlerType::New();
		// Load the XML file and check with the schema
		l_LUTXMLHandler->LoadFile(l_GIPPFilename);
		const SimpleLutXMLContainer& l_lutxml = l_LUTXMLHandler->GetLutContainer();

		// Read informations from the GIPP file
		const Utilities::ListOfDoubles& l_GIP_L2TOCR_AltitudeIndexes = l_lutxml.GetAltitude_Indexes();
		const Utilities::ListOfDoubles& l_GIP_L2TOCR_AOTIndexes = l_lutxml.GetAOT_Indexes();
		const Utilities::ListOfDoubles& l_GIP_L2TOCR_TOAReflectanceIndexes = l_lutxml.GetTOA_Reflectance_Indexes();

		vnsLogDebugMacro("  l_GIP_L2TOCR_AltitudeIndexes: "<<l_GIP_L2TOCR_AltitudeIndexes.size());
		vnsLogDebugMacro("  l_GIP_L2TOCR_AOTIndexes: "<<l_GIP_L2TOCR_AOTIndexes.size());
		vnsLogDebugMacro("  l_GIP_L2TOCR_TOAReflectanceIndexes: "<<l_GIP_L2TOCR_TOAReflectanceIndexes.size());

		/* parameters are added one by one to the LUT */
		ParameterValuesType l_TOCRParam4;
		l_TOCRParam4.ParameterName = "Altitude_Indexes";
		l_TOCRParam4.ParameterValues = l_GIP_L2TOCR_AltitudeIndexes;
		l_LUT_JDay->AddParameterValues(l_TOCRParam4);

		ParameterValuesType l_TOCRParam5;
		l_TOCRParam5.ParameterName = "AOT_Indexes";
		l_TOCRParam5.ParameterValues = l_GIP_L2TOCR_AOTIndexes;
		l_LUT_JDay->AddParameterValues(l_TOCRParam5);

		ParameterValuesType l_TOCRParam6;
		l_TOCRParam6.ParameterName = "TOA_Reflectance_Indexes";
		l_TOCRParam6.ParameterValues = l_GIP_L2TOCR_TOAReflectanceIndexes;
		l_LUT_JDay->AddParameterValues(l_TOCRParam6);


		//GetParams
		//Choose mode
		Utilities::AOTMethodEnumType l_AOTMethod;
		// Get Mode
		switch ( GetParameterInt("mode") )
		{
		case multitemporal:
			l_AOTMethod = Utilities::MULTITEMPORAL;
			break;
		case multispectral:
			l_AOTMethod = Utilities::MULTISPECTRAL;
			break;
		case spectrotemporal:
			l_AOTMethod = Utilities::SPECTROTEMPORAL;
			break;
		}
		//Multi Temp params
		Utilities::ListOfUIntegers l_MTAOTbandsD;
		Utilities::ListOfUIntegers l_MTAOTbandsReflectanceCompositeDm1;
		Utilities::ListOfUIntegers l_MTAOTbandsLUTCompositeDm1;
		unsigned int l_VARBandD = 0;
		unsigned int l_VARBandDm1 = 0;
		unsigned int l_STDEVBand = 0;
		double l_RelativeVarMax = 0;
		double l_StdNoise = 0;
		double l_MinStdDevThreshold = 0;
		double l_AOTinitialDm1 = 0;
		double l_Weq2 = 0;
		double l_FirstAOT = 0;
		double l_SecondAOT = 0;
		double l_TOAReflMin = 0;
		double l_TOAReflMax = 0;
		double l_TOAReflStep = 0;
		double l_AltitudeMean = 0;
		double l_MinDiffThreholds = 0;
		double l_JDay = 0;
		double l_MaxAge = 0;
		LUTMapType l_lutMap;
		if ((l_AOTMethod == Utilities::MULTITEMPORAL) || (l_AOTMethod == Utilities::SPECTROTEMPORAL))
		{
			l_MTAOTbandsD = lAOTParamsXMLHandler->GetMTAOTBandCodeD();
			l_MTAOTbandsReflectanceCompositeDm1 = lAOTParamsXMLHandler->GetMTAOTBandReflectanceCompositeDm1();
			l_MTAOTbandsLUTCompositeDm1 = lAOTParamsXMLHandler->GetMTAOTBandLUTCompositeDm1();

			l_VARBandD = lAOTParamsXMLHandler->GetVarBandCodeD();
			vnsLogDebugDoubleValueMacro(l_VARBandD);
			l_VARBandDm1 = lAOTParamsXMLHandler->GetVarBandCodeDm1();
			vnsLogDebugDoubleValueMacro(l_VARBandDm1);
			l_STDEVBand = lAOTParamsXMLHandler->GetStdevBandCode();
			vnsLogDebugDoubleValueMacro(l_STDEVBand);
			l_RelativeVarMax = lAOTParamsXMLHandler->GetRelativeVarMax();
			vnsLogDebugDoubleValueMacro(l_RelativeVarMax);
			l_StdNoise = lAOTParamsXMLHandler->GetStdNoise();
			vnsLogDebugDoubleValueMacro(l_StdNoise);
			l_MinStdDevThreshold = lAOTParamsXMLHandler->GetStdevMin();
			vnsLogDebugDoubleValueMacro(l_MinStdDevThreshold);
			l_AOTinitialDm1 = lAOTParamsXMLHandler->GetAOTinitialDm1();
			vnsLogDebugDoubleValueMacro(l_AOTinitialDm1);
			l_Weq2 = lAOTParamsXMLHandler->GetWMTequation2();
			vnsLogDebugDoubleValueMacro(l_Weq2);
			l_FirstAOT = lAOTParamsXMLHandler->GetFirstAOT();
			vnsLogDebugDoubleValueMacro(l_FirstAOT);
			l_SecondAOT = lAOTParamsXMLHandler->GetSecondAOT();
			vnsLogDebugDoubleValueMacro(l_SecondAOT);
			l_TOAReflMin = lAOTParamsXMLHandler->GetTOAReflectanceMin();
			vnsLogDebugDoubleValueMacro(l_TOAReflMin);
			l_TOAReflMax = lAOTParamsXMLHandler->GetTOAReflectanceMax();
			vnsLogDebugDoubleValueMacro(l_TOAReflMax);
			l_TOAReflStep = lAOTParamsXMLHandler->GetTOAReflectanceStep();
			vnsLogDebugDoubleValueMacro(l_TOAReflStep);
			l_AltitudeMean = GetParameterFloat("temporal.altitudemean");
			vnsLogDebugDoubleValueMacro(l_AltitudeMean);
			l_MinDiffThreholds = lAOTParamsXMLHandler->GetMinDifferenceThreshold();
			vnsLogDebugDoubleValueMacro(l_MinDiffThreholds);
			l_JDay = GetParameterFloat("jday");
			vnsLogDebugDoubleValueMacro(l_JDay);
			l_MaxAge = lAOTParamsXMLHandler->GetAOTMaxAge();
			vnsLogDebugDoubleValueMacro(l_MaxAge);

			//Load the lut header
			if(!HasValue("temporal.lutmap"))
			{
				vnsExceptionDataMacro("No file given for temporal.lutmap");
			}
			const std::string l_GIPPFilename = GetParameterString("temporal.lutmap");
			LutMapXMLHandlerPointer l_LUTMAPXMLHandler = LutMapXMLHandlerType::New();
			// Load the XML file and check with the schema
			l_LUTMAPXMLHandler->LoadFile(l_GIPPFilename);
			const SimpleLutMapXMLFileHandler::LutMapXml& l_lutmapxml = l_LUTMAPXMLHandler-> GetLutMapContainer();
			SimpleLutMapXMLFileHandler::LutMapXml::const_iterator it = l_lutmapxml.begin();
			for (;it != l_lutmapxml.end();++it)
			{
				const SimpleLutXMLContainer& l_lut = (*it).second;
				// Read informations from the GIPP file
				const Utilities::ListOfDoubles& l_GIP_L2TOCR_AltitudeIndexes = l_lutxml.GetAltitude_Indexes();
				const Utilities::ListOfDoubles& l_GIP_L2TOCR_AOTIndexes = l_lutxml.GetAOT_Indexes();
				const Utilities::ListOfDoubles& l_GIP_L2TOCR_TOAReflectanceIndexes = l_lutxml.GetTOA_Reflectance_Indexes();

				vnsLogDebugMacro("  l_GIP_L2TOCR_AltitudeIndexes: "<<l_GIP_L2TOCR_AltitudeIndexes.size());
				vnsLogDebugMacro("  l_GIP_L2TOCR_AOTIndexes: "<<l_GIP_L2TOCR_AOTIndexes.size());
				vnsLogDebugMacro("  l_GIP_L2TOCR_TOAReflectanceIndexes: "<<l_GIP_L2TOCR_TOAReflectanceIndexes.size());

				//Lut
				//Load the lut image
				// ---------------------------------------------------------------------------------------------
				if( l_lut.GetListOfPackaged_DBL_Files().size() != 1)
				{
					vnsExceptionDataMacro("Only one file required for each lut");
				}

				m_Reader = ImageFileReaderType::New();
				m_Reader->SetFileName(l_lut.GetListOfPackaged_DBL_Files()[0]);
				// ---------------------------------------------------------------------------------------------
				// Load the LUT
				m_Reader->Update();
				//Lut
				LUTPointerType l_LUT = m_Reader->GetOutput();

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

				//Put the lut map
				l_lutMap[it->first] = l_LUT;
			}

		} // end if MULTITEMPORAL or SPECTROTEMPORAL

		Utilities::ListOfUIntegers l_MSAOTbands;
		unsigned int l_RedChannel = 0;
		unsigned int l_NDVIAOTBand1 = 0;
		unsigned int l_NDVIAOTBand2 = 0;
		double l_RedTOAThreshold = 0;
		double l_NDVIThreshold = 0;
		double l_SlopeMS = 0;
		double l_YInterceptMS = 0;
		double l_SlopeNDVI = 0;
		double l_YInterceptNDVI = 0;
		double l_WeqMS = 0;

		if ((l_AOTMethod == Utilities::MULTISPECTRAL) || (l_AOTMethod == Utilities::SPECTROTEMPORAL))
		{
			l_MSAOTbands = lAOTParamsXMLHandler->GetMSAOTBandCode();
			l_RedChannel = lAOTParamsXMLHandler->GetMSAOTRedBandCode();
			vnsLogDebugDoubleValueMacro(l_RedChannel);
			l_NDVIAOTBand1 = lAOTParamsXMLHandler->GetNDVIAOTBand1Code();
			vnsLogDebugDoubleValueMacro(l_NDVIAOTBand1);
			l_NDVIAOTBand2 = lAOTParamsXMLHandler->GetNDVIAOTBand2Code();
			vnsLogDebugDoubleValueMacro(l_NDVIAOTBand2);
			l_RedTOAThreshold = lAOTParamsXMLHandler->GetRedTOAThreshold();
			vnsLogDebugDoubleValueMacro(l_RedTOAThreshold);
			l_NDVIThreshold = lAOTParamsXMLHandler->GetNDVIThreshold();
			vnsLogDebugDoubleValueMacro(l_NDVIThreshold);
			l_SlopeMS = lAOTParamsXMLHandler->GetMSSlope();
			vnsLogDebugDoubleValueMacro(l_SlopeMS);
			l_YInterceptMS = lAOTParamsXMLHandler->GetMSYIntercept();
			vnsLogDebugDoubleValueMacro(l_YInterceptMS);
			l_SlopeNDVI = lAOTParamsXMLHandler->GetNDVISlope();
			vnsLogDebugDoubleValueMacro(l_SlopeNDVI);
			l_YInterceptNDVI = lAOTParamsXMLHandler->GetNDVIYIntercept();
			vnsLogDebugDoubleValueMacro(l_YInterceptNDVI);
			l_WeqMS = lAOTParamsXMLHandler->GetWMS();
			vnsLogDebugDoubleValueMacro(l_WeqMS);
		}

		//SpectroTemp params
		double l_MTWeightThreshold = 0;
		double l_ACoeff = 0;
		double l_BCoeff = 0;
		if (l_AOTMethod == Utilities::SPECTROTEMPORAL)
		{
			l_MTWeightThreshold = lAOTParamsXMLHandler->GetMTWeightThreshold();
			vnsLogDebugDoubleValueMacro(l_MTWeightThreshold);
			l_ACoeff = lAOTParamsXMLHandler->GetACoefficient();
			vnsLogDebugDoubleValueMacro(l_ACoeff);
			l_BCoeff = lAOTParamsXMLHandler->GetBCoefficient();
			vnsLogDebugDoubleValueMacro(l_BCoeff);
		}

		//Commons params
		const bool l_InitMode = GetParameterInt("init");
		vnsLogDebugMacro("========init: " << l_InitMode);
		unsigned int l_MinSamplingInterval = lAOTParamsXMLHandler->GetSamplingIntervalMin();
		vnsLogDebugDoubleValueMacro(l_MinSamplingInterval);
		unsigned int l_MaxSamplingInterval = lAOTParamsXMLHandler->GetSamplingIntervalMax();
		vnsLogDebugDoubleValueMacro(l_MaxSamplingInterval);
		unsigned int l_IncrementNDistance = lAOTParamsXMLHandler->GetIncrementNDistance();
		vnsLogDebugDoubleValueMacro(l_IncrementNDistance);
		unsigned int l_Radius = lAOTParamsXMLHandler->GetHalfWindowSize();
		vnsLogDebugDoubleValueMacro(l_Radius);
		unsigned int l_StepAOT = lAOTParamsXMLHandler->GetStepAOT();
		vnsLogDebugDoubleValueMacro(l_StepAOT);
		unsigned int l_MinNumberOfPixels = lAOTParamsXMLHandler->GetNPixMin();
		vnsLogDebugDoubleValueMacro(l_MinNumberOfPixels);
		double l_FTol = lAOTParamsXMLHandler->GetFTolerance();
		vnsLogDebugDoubleValueMacro(l_FTol);
		double l_GTol = lAOTParamsXMLHandler->GetGTolerance();
		vnsLogDebugDoubleValueMacro(l_GTol);
		double l_XTol = lAOTParamsXMLHandler->GetXTolerance();
		vnsLogDebugDoubleValueMacro(l_XTol);
		double l_Epsilon_Function = lAOTParamsXMLHandler->GetEpsilonFunction();
		vnsLogDebugDoubleValueMacro(l_Epsilon_Function);
		unsigned int l_Max_Iterations = lAOTParamsXMLHandler->GetMaxIterations();
		vnsLogDebugDoubleValueMacro(l_Max_Iterations);
		double l_AOTinitialD = lAOTParamsXMLHandler->GetAOTinitialD();
		vnsLogDebugDoubleValueMacro(l_AOTinitialD);
		double l_Wdark = lAOTParamsXMLHandler->GetWdark();
		vnsLogDebugDoubleValueMacro(l_Wdark);
		double l_Wbounds = lAOTParamsXMLHandler->GetWbounds();
		vnsLogDebugDoubleValueMacro(l_Wbounds);
		double l_AOTmin = lAOTParamsXMLHandler->GetAOTmin();
		vnsLogDebugDoubleValueMacro(l_AOTmin);
		double l_RealL2NoData = GetParameterFloat("reall2nodata");
		vnsLogDebugDoubleValueMacro(l_RealL2NoData);
		double l_SEMaskDilatationRadius  = lAOTParamsXMLHandler->GetSEMaskDilatationRadius();
		vnsLogDebugDoubleValueMacro(l_SEMaskDilatationRadius);
		double l_SEValidOATRadius  = lAOTParamsXMLHandler->GetSEValidAOTRadius();
		vnsLogDebugDoubleValueMacro(l_SEValidOATRadius);
		double l_DarkestPixelAOT = GetParameterFloat("darkestaot");
		vnsLogDebugDoubleValueMacro(l_DarkestPixelAOT);

		if (l_InitMode == false)
		{
		    vnsLogDebugMacro("====== init False");
			if (!HasValue("l2ndt") || !HasValue("l2rta") || !HasValue("l2rtc")
					|| !HasValue("l2pxd"))
			{
				vnsLogDebugMacro("l2ndt : "<<HasValue("l2ndt"))
				vnsLogDebugMacro("l2rtc : "<<HasValue("l2rtc"))
				vnsLogDebugMacro("l2rta : "<<HasValue("l2rta"))
				vnsLogDebugMacro("l2pxd : "<<HasValue("l2pxd"))
				vnsExceptionDataMacro("One L2in input is missing");
			}
			// Get input images of date D-1
			lL2inNDT = GetParameterUInt8Image("l2ndt");
			lL2inRTA = GetParameterDoubleVectorImage("l2rta");
			lL2inRTC = GetParameterDoubleVectorImage("l2rtc");
			// Get the date plan
			lL2inPXD = GetParameterInt32Image("l2pxd");
		}

		if( l_AOTMethod == Utilities::MULTITEMPORAL || l_AOTMethod == Utilities::SPECTROTEMPORAL)
		{
			m_MTThresholdsFilter->SetFirstAOT(l_FirstAOT);
			m_MTThresholdsFilter->SetSecondAOT(l_SecondAOT);
			m_MTThresholdsFilter->SetTOAReflMin(l_TOAReflMin);
			m_MTThresholdsFilter->SetTOAReflMax(l_TOAReflMax);
			m_MTThresholdsFilter->SetTOAReflStep(l_TOAReflStep);
			m_MTThresholdsFilter->SetAltitudeMean(l_AltitudeMean);
			m_MTThresholdsFilter->SetMinDiffThreholds(l_MinDiffThreholds);
			m_MTThresholdsFilter->SetMTAOTBands(l_MTAOTbandsD);
			m_MTThresholdsFilter->SetLUT(l_LUT_JDay);
			// Compute the TOA reflectance thresholds used in the selection of the pixels for the local inversion
			m_MTThresholdsFilter->ComputeReflectanceThresholds();
		}


		// Link DilateImageFilter : dilate the mask
		vnsLogDebugMacro("SEMaskDilatationRadius : "<< l_SEMaskDilatationRadius);
		KernelType lKernel;
		lKernel.SetRadius(l_SEMaskDilatationRadius);
		lKernel.CreateStructuringElement();
		// Dilate water mask
		m_DilateWaterMaskFilter->SetInput(lL2WASSub);
		m_DilateWaterMaskFilter->SetDilateValue(1);
		m_DilateWaterMaskFilter->SetKernel(lKernel);

		// Dilate Snow mask
		m_DilateSnowMaskFilter->SetInput(lIPSNWSub);
		m_DilateSnowMaskFilter->SetDilateValue(1);
		m_DilateSnowMaskFilter->SetKernel(lKernel);

		if(l_useCams)
		{
			vnsLogDebugMacro("Using CAMS in AOT estimation");
			 // L2Coarse area
			AreaType l_AreaToL2CoarseResolution;
			l_AreaToL2CoarseResolution.Origin = lL2CLDSub->GetOrigin();
			l_AreaToL2CoarseResolution.Spacing = lL2CLDSub->GetSignedSpacing();
			l_AreaToL2CoarseResolution.Size = lL2CLDSub->GetLargestPossibleRegion().GetSize();
			// Expand AOT from CAMS
			l_AOTCamsSub = AOTImageType::New();
			l_AOTCamsSub->SetRegions(lL2CLDSub->GetLargestPossibleRegion());
			l_AOTCamsSub->SetOrigin(lL2CLDSub->GetOrigin());
			l_AOTCamsSub->SetSignedSpacing(lL2CLDSub->GetSignedSpacing());
			l_AOTCamsSub->SetProjectionRef(lL2CLDSub->GetProjectionRef());
			l_AOTCamsSub->Allocate(true);
			// Build wgs ref
			OGRSpatialReference oSRS;
			oSRS.SetWellKnownGeogCS("WGS84");
			char * wgsRef = NULL;
			oSRS.exportToWkt(&wgsRef);
			// Instanciate Image->WGS transform
			GEOTransformType::Pointer img2wgs = GEOTransformType::New();
			img2wgs->SetOutputProjectionRef(lAOTCamsPtr->GetProjectionRef());
			//Set output transform info
			img2wgs->SetInputProjectionRef(lL2CLDSub->GetProjectionRef());
			img2wgs->SetInputOrigin(lL2CLDSub->GetOrigin());
			img2wgs->SetInputSpacing(lL2CLDSub->GetSignedSpacing());
			img2wgs->InstantiateTransform();
			GEOTransformType::InputPointType inPts;
			for (unsigned int i = 0; i < l_AreaToL2CoarseResolution.Size[0];i++)
			{
				for (unsigned int j = 0; j < l_AreaToL2CoarseResolution.Size[1];j++)
				{
					// Compute Lat,Long coordinate for the point
					// ---------------------------------------------------------------------------------------------
					inPts[0] = i;
					inPts[1] = j;
					//Get the transformed point
					GEOTransformType::OutputPointType l_pointWGS = img2wgs->TransformPoint(inPts);
					//AOT interpolate at this lat/long
					AOTContinuousIndexType l_contindex;
					AOTImageType::PointType l_point;
					l_point[0] = l_pointWGS[0];
					//Cyclic rotation on longitude
					if (l_pointWGS[0] < 0)
					{
						l_point[0] = 360 +l_pointWGS[0];
					}
					l_point[1] = l_pointWGS[1];
					lAOTCamsPtr->TransformPhysicalPointToContinuousIndex(l_point, l_contindex);
					AOTInterpolatorType::Pointer interp = AOTInterpolatorType::New();
					interp->SetInputImage(lAOTCamsPtr);
					AOTPixelType l_outPix = static_cast<AOTPixelType> (interp->EvaluateAtContinuousIndex(l_contindex));
					AOTImageType::IndexType inIdx;
					inIdx[0] = i;
					inIdx[1] = j;
					l_AOTCamsSub->SetPixel(inIdx, l_outPix);
				}
			}
		}


		// =================================================================================================================
		// Inversion : MULTI TEMPORAL AOT estimation
		// =================================================================================================================
		if (l_AOTMethod == Utilities::MULTITEMPORAL)
		{
			vnsLogDebugMacro("AOT Estimation : MULTITEMPORAL METHOD");
			//Common Param
			m_MTComputeAOTImageFilter->SetMinSamplingInterval(l_MinSamplingInterval);
			m_MTComputeAOTImageFilter->SetMaxSamplingInterval(l_MaxSamplingInterval);
			m_MTComputeAOTImageFilter->SetIncrementNDistance(l_IncrementNDistance);
			m_MTComputeAOTImageFilter->SetRadius(l_Radius);
			m_MTComputeAOTImageFilter->SetStepAOT(l_StepAOT);
			m_MTComputeAOTImageFilter->SetMinNumberOfPixels(l_MinNumberOfPixels);
			m_MTComputeAOTImageFilter->SetF_Tolerance(l_FTol);
			m_MTComputeAOTImageFilter->SetG_Tolerance(l_GTol);
			m_MTComputeAOTImageFilter->SetX_Tolerance(l_XTol);
			m_MTComputeAOTImageFilter->SetEpsilon_Function(l_Epsilon_Function);
			m_MTComputeAOTImageFilter->SetMax_Iterations(l_Max_Iterations);
			m_MTComputeAOTImageFilter->SetAOTinitialD(l_AOTinitialD);
			m_MTComputeAOTImageFilter->SetLUT(l_LUT_JDay);
			m_MTComputeAOTImageFilter->SetLUTMap(l_lutMap);
			m_MTComputeAOTImageFilter->SetWdark(l_Wdark);
			m_MTComputeAOTImageFilter->SetWbounds(l_Wbounds);
			m_MTComputeAOTImageFilter->SetNoData(l_RealL2NoData);
			m_MTComputeAOTImageFilter->SetAOTmin(l_AOTmin);
			m_MTComputeAOTImageFilter->SetL2CLDSub(lL2CLDSub);
			m_MTComputeAOTImageFilter->SetIPSHDSub(lIPSHDSub);
			m_MTComputeAOTImageFilter->SetIPSAT(lIPSAT);
			m_MTComputeAOTImageFilter->SetL2WASSub(/*m_DilateWaterMaskFilterOutput);//*/m_DilateWaterMaskFilter->GetOutput());
			m_MTComputeAOTImageFilter->SetIPEDG(lIPEDG);
			m_MTComputeAOTImageFilter->SetIPSNWSub(m_DilateSnowMaskFilter->GetOutput());
			m_MTComputeAOTImageFilter->SetL2inNDT(lL2inNDT);
			m_MTComputeAOTImageFilter->SetIPTOACSub(lIPTOACSub);

			//CAMS Parameters
			if (l_useCams)
			{
				m_MTComputeAOTImageFilter->SetAOTCamsSub(l_AOTCamsSub);
				m_MTComputeAOTImageFilter->SetKPondCAMS(l_KPondsCams);
				m_MTComputeAOTImageFilter->SetMeanAltitude(l_AltMean);
				m_MTComputeAOTImageFilter->SetHeightScale(l_HeightScale);
				m_MTComputeAOTImageFilter->SetUseCams(l_useCams);
			}
			//Temporal input
			m_MTComputeAOTImageFilter->SetL2inRTA(lL2inRTA);
			m_MTComputeAOTImageFilter->SetL2inRTC(lL2inRTC);
			m_MTComputeAOTImageFilter->SetL2inPXD(lL2inPXD);

			//Specifiq params
			m_MTComputeAOTImageFilter->SetAOTbandsD(l_MTAOTbandsD);
			m_MTComputeAOTImageFilter->SetAOTbandsReflectanceCompositeDm1(l_MTAOTbandsReflectanceCompositeDm1);
			m_MTComputeAOTImageFilter->SetAOTbandsLUTCompositeDm1(l_MTAOTbandsLUTCompositeDm1);
			m_MTComputeAOTImageFilter->SetRelativeVarMax(l_RelativeVarMax);
			m_MTComputeAOTImageFilter->SetVARBandD(l_VARBandD);
			m_MTComputeAOTImageFilter->SetVARBandDm1(l_VARBandDm1);
			m_MTComputeAOTImageFilter->SetSTDEVBand(l_STDEVBand);
			m_MTComputeAOTImageFilter->SetStdNoise(l_StdNoise);
			m_MTComputeAOTImageFilter->SetMinStdDevThreshold(l_MinStdDevThreshold);
			m_MTComputeAOTImageFilter->SetAOTinitialDm1(l_AOTinitialDm1);
			m_MTComputeAOTImageFilter->SetWeq2(l_Weq2);
			m_MTComputeAOTImageFilter->SetJDay(l_JDay);
			m_MTComputeAOTImageFilter->SetMaxAge(l_MaxAge);
			m_MTComputeAOTImageFilter->SetDarkestPixelAOT(l_DarkestPixelAOT);
			m_MTComputeAOTImageFilter->SetDTMSub(lDTMSub);


			m_MTComputeAOTImageFilter->SetLowReflectanceThresholds(m_MTThresholdsFilter->GetLowReflectanceThreshold());
			m_MTComputeAOTImageFilter->SetHighReflectanceThresholds(m_MTThresholdsFilter->GetHighReflectanceThreshold());

			m_MTComputeAOTImageFilter->UpdateOutputInformation();

			m_BinaryMorphologicalOpeningImageFilter->SetInput(m_MTComputeAOTImageFilter->GetOutputMask());
			m_BinaryFunctorImageFilter->SetInput1(m_MTComputeAOTImageFilter->GetIPAOT());

			SetParameterOutputImage<OutputMaskType>("aotmask",m_MTComputeAOTImageFilter->GetOutputMask());

			if (l_useCams)
			{
				SetParameterOutputImage<OutputImageType>("aotcams",m_MTComputeAOTImageFilter->GetIPAOTCams());
				m_AOTSelectionFilter->SetInput(1,m_MTComputeAOTImageFilter->GetIPAOTCams());
			}

		} // end MULTITEMPORAL METHOD

		// =================================================================================================================
		// Inversion : MULTI SPECTRAL AOT estimation
		// =================================================================================================================

		else if (l_AOTMethod == Utilities::MULTISPECTRAL)
		{
			vnsLogDebugMacro("AOT Estimation : MULTISPECTRAL METHOD");
			//Common Param
			m_MSComputeAOTImageFilter->SetMinSamplingInterval(l_MinSamplingInterval);
			m_MSComputeAOTImageFilter->SetMaxSamplingInterval(l_MaxSamplingInterval);
			m_MSComputeAOTImageFilter->SetIncrementNDistance(l_IncrementNDistance);
			m_MSComputeAOTImageFilter->SetRadius(l_Radius);
			m_MSComputeAOTImageFilter->SetStepAOT(l_StepAOT);
			m_MSComputeAOTImageFilter->SetMinNumberOfPixels(l_MinNumberOfPixels);
			m_MSComputeAOTImageFilter->SetG_Tolerance(l_GTol);
			m_MSComputeAOTImageFilter->SetF_Tolerance(l_FTol);
			m_MSComputeAOTImageFilter->SetX_Tolerance(l_XTol);
			m_MSComputeAOTImageFilter->SetEpsilon_Function(l_Epsilon_Function);
			m_MSComputeAOTImageFilter->SetMax_Iterations(l_Max_Iterations);
			m_MSComputeAOTImageFilter->SetAOTinitialD(l_AOTinitialD);
			m_MSComputeAOTImageFilter->SetLUT(l_LUT_JDay);
			m_MSComputeAOTImageFilter->SetWdark(l_Wdark);
			m_MSComputeAOTImageFilter->SetWbounds(l_Wbounds);
			m_MSComputeAOTImageFilter->SetAOTmin(l_AOTmin);
			m_MSComputeAOTImageFilter->SetNoData(l_RealL2NoData);
			m_MSComputeAOTImageFilter->SetL2CLDSub(lL2CLDSub);
			m_MSComputeAOTImageFilter->SetIPSHDSub(lIPSHDSub);
			m_MSComputeAOTImageFilter->SetIPSAT(lIPSAT);
			m_MSComputeAOTImageFilter->SetL2WASSub(m_DilateWaterMaskFilter->GetOutput());
			m_MSComputeAOTImageFilter->SetIPEDG(lIPEDG);
			m_MSComputeAOTImageFilter->SetIPSNWSub(m_DilateSnowMaskFilter->GetOutput());
			m_MSComputeAOTImageFilter->SetIPTOACSub(lIPTOACSub);
			//Specifiq params
			m_MSComputeAOTImageFilter->SetAOTbands(l_MSAOTbands);
			m_MSComputeAOTImageFilter->SetRedChannel(l_RedChannel);
			m_MSComputeAOTImageFilter->SetNDVIAOTBand1(l_NDVIAOTBand1);
			m_MSComputeAOTImageFilter->SetNDVIAOTBand2(l_NDVIAOTBand2);
			m_MSComputeAOTImageFilter->SetRedTOAThreshold(l_RedTOAThreshold);
			m_MSComputeAOTImageFilter->SetNDVIThreshold(l_NDVIThreshold);
			m_MSComputeAOTImageFilter->SetSlopeMS(l_SlopeMS);
			m_MSComputeAOTImageFilter->SetYInterceptMS(l_YInterceptMS);
			m_MSComputeAOTImageFilter->SetSlopeNDVI(l_SlopeNDVI);
			m_MSComputeAOTImageFilter->SetYInterceptNDVI(l_YInterceptNDVI);
			m_MSComputeAOTImageFilter->SetWeq1(l_WeqMS);

			//CAMS Parameters
			if (l_useCams)
			{
				m_MSComputeAOTImageFilter->SetAOTCamsSub(l_AOTCamsSub);
				m_MSComputeAOTImageFilter->SetKPondCAMS(l_KPondsCams);
				m_MSComputeAOTImageFilter->SetMeanAltitude(l_AltMean);
				m_MSComputeAOTImageFilter->SetHeightScale(l_HeightScale);
				m_MSComputeAOTImageFilter->SetUseCams(l_useCams);
			}

			// Set input parameters for LMCostFunction
			m_MSComputeAOTImageFilter->SetDTMSub(lDTMSub);
			m_MSComputeAOTImageFilter->SetDarkestPixelAOT(l_DarkestPixelAOT);

			m_MSComputeAOTImageFilter->UpdateOutputInformation();

			m_BinaryMorphologicalOpeningImageFilter->SetInput(m_MSComputeAOTImageFilter->GetOutputMask());
			m_BinaryFunctorImageFilter->SetInput1(m_MSComputeAOTImageFilter->GetIPAOT());

			SetParameterOutputImage<OutputMaskType>("aotmask",m_MSComputeAOTImageFilter->GetOutputMask());

			if (l_useCams)
			{
				SetParameterOutputImage<OutputImageType>("aotcams",m_MSComputeAOTImageFilter->GetIPAOTCams());
				m_AOTSelectionFilter->SetInput(1,m_MSComputeAOTImageFilter->GetIPAOTCams());
			}
		}

		// =================================================================================================================
		// Inversion : SPECTRO TEMPORAL METHOD
		// =================================================================================================================
		else // (m_AOTMethod == SPECTROTEMPORAL)
		{
			vnsLogDebugMacro("AOT Estimation : SPECTROTEMPORAL METHOD");
			//Common params
			m_STComputeAOTImageFilter->SetMaxSamplingInterval(l_MaxSamplingInterval);
			m_STComputeAOTImageFilter->SetMinSamplingInterval(l_MinSamplingInterval);
			m_STComputeAOTImageFilter->SetIncrementNDistance(l_IncrementNDistance);
			m_STComputeAOTImageFilter->SetRadius(l_Radius);
			m_STComputeAOTImageFilter->SetStepAOT(l_StepAOT);
			m_STComputeAOTImageFilter->SetMinNumberOfPixels(l_MinNumberOfPixels);
			m_STComputeAOTImageFilter->SetF_Tolerance(l_FTol);
			m_STComputeAOTImageFilter->SetG_Tolerance(l_GTol);
			m_STComputeAOTImageFilter->SetX_Tolerance(l_XTol);
			m_STComputeAOTImageFilter->SetEpsilon_Function(l_Epsilon_Function);
			m_STComputeAOTImageFilter->SetMax_Iterations(l_Max_Iterations);
			m_STComputeAOTImageFilter->SetAOTinitialD(l_AOTinitialD);
			m_STComputeAOTImageFilter->SetLUT(l_LUT_JDay);
			m_STComputeAOTImageFilter->SetLUTMap(l_lutMap);
			m_STComputeAOTImageFilter->SetWdark(l_Wdark);
			m_STComputeAOTImageFilter->SetWbounds(l_Wbounds);
			m_STComputeAOTImageFilter->SetAOTmin(l_AOTmin);
			m_STComputeAOTImageFilter->SetNoData(l_RealL2NoData);
			m_STComputeAOTImageFilter->SetL2CLDSub(lL2CLDSub);
			m_STComputeAOTImageFilter->SetIPSHDSub(lIPSHDSub);
			m_STComputeAOTImageFilter->SetIPSAT(lIPSAT);
			m_STComputeAOTImageFilter->SetL2WASSub(m_DilateWaterMaskFilter->GetOutput());
			m_STComputeAOTImageFilter->SetIPSNWSub(m_DilateSnowMaskFilter->GetOutput());
			m_STComputeAOTImageFilter->SetIPEDG(lIPEDG);
			m_STComputeAOTImageFilter->SetL2inNDT(lL2inNDT);
			m_STComputeAOTImageFilter->SetIPTOACSub(lIPTOACSub);

			//Temporal input
			m_STComputeAOTImageFilter->SetL2inRTA(lL2inRTA);
			m_STComputeAOTImageFilter->SetL2inRTC(lL2inRTC);
			m_STComputeAOTImageFilter->SetL2inPXD(lL2inPXD);

			//Specifiq params
			m_STComputeAOTImageFilter->SetMSAOTbands(l_MSAOTbands);
			m_STComputeAOTImageFilter->SetMTAOTbandsD(l_MTAOTbandsD);
			m_STComputeAOTImageFilter->SetMTAOTbandsReflectanceCompositeDm1(l_MTAOTbandsReflectanceCompositeDm1);
			m_STComputeAOTImageFilter->SetMTAOTbandsLUTCompositeDm1(l_MTAOTbandsLUTCompositeDm1);
			m_STComputeAOTImageFilter->SetRelativeVarMax(l_RelativeVarMax);
			m_STComputeAOTImageFilter->SetVARBandD(l_VARBandD);
			m_STComputeAOTImageFilter->SetVARBandDm1(l_VARBandDm1);
			m_STComputeAOTImageFilter->SetSTDEVBand(l_STDEVBand);
			m_STComputeAOTImageFilter->SetStdNoise(l_StdNoise);
			m_STComputeAOTImageFilter->SetMinStdDevThreshold(l_MinStdDevThreshold);
			m_STComputeAOTImageFilter->SetAOTinitialDm1(l_AOTinitialDm1);
			m_STComputeAOTImageFilter->SetWeq2(l_Weq2);
			m_STComputeAOTImageFilter->SetRedChannel(l_RedChannel);
			m_STComputeAOTImageFilter->SetNDVIAOTBand1(l_NDVIAOTBand1);
			m_STComputeAOTImageFilter->SetNDVIAOTBand2(l_NDVIAOTBand2);
			m_STComputeAOTImageFilter->SetRedTOAThreshold(l_RedTOAThreshold);
			m_STComputeAOTImageFilter->SetNDVIThreshold(l_NDVIThreshold);
			m_STComputeAOTImageFilter->SetSlopeMS(l_SlopeMS);
			m_STComputeAOTImageFilter->SetYInterceptMS(l_YInterceptMS);
			m_STComputeAOTImageFilter->SetSlopeNDVI(l_SlopeNDVI);
			m_STComputeAOTImageFilter->SetYInterceptNDVI(l_YInterceptNDVI);
			m_STComputeAOTImageFilter->SetWeqMS(l_WeqMS);
			m_STComputeAOTImageFilter->SetJDay(l_JDay);
			m_STComputeAOTImageFilter->SetMaxAge(l_MaxAge);
			m_STComputeAOTImageFilter->SetMTWeightThreshold(l_MTWeightThreshold);
			m_STComputeAOTImageFilter->SetACoefficient(l_ACoeff);
			m_STComputeAOTImageFilter->SetBCoefficient(l_BCoeff);
			m_STComputeAOTImageFilter->SetDarkestPixelAOT(l_DarkestPixelAOT);
			m_STComputeAOTImageFilter->SetDTMSub(lDTMSub);
			m_STComputeAOTImageFilter->SetLowReflectanceThresholds(m_MTThresholdsFilter->GetLowReflectanceThreshold());
			m_STComputeAOTImageFilter->SetHighReflectanceThresholds(m_MTThresholdsFilter->GetHighReflectanceThreshold());

			//CAMS Parameters
			if (l_useCams)
			{
				m_STComputeAOTImageFilter->SetAOTCamsSub(l_AOTCamsSub);
				m_STComputeAOTImageFilter->SetKPondCAMS(l_KPondsCams);
				m_STComputeAOTImageFilter->SetMeanAltitude(l_AltMean);
				m_STComputeAOTImageFilter->SetHeightScale(l_HeightScale);
				m_STComputeAOTImageFilter->SetUseCams(l_useCams);
			}

			m_BinaryMorphologicalOpeningImageFilter->SetInput(m_STComputeAOTImageFilter->GetOutputMask());
			m_BinaryFunctorImageFilter->SetInput1(m_STComputeAOTImageFilter->GetIPAOT());

			if (l_useCams)
			{
				SetParameterOutputImage<OutputImageType>("aotcams",m_STComputeAOTImageFilter->GetIPAOTCams());
				m_AOTSelectionFilter->SetInput(1,m_STComputeAOTImageFilter->GetIPAOTCams());
			}

		}

		//Set outputs
		// Opening to suppress isolated small group of pixels
		vnsLogDebugMacro("SEValidAOTRadius : "<< l_SEValidOATRadius);
		KernelType lKernel2;
		lKernel2.SetRadius(l_SEValidOATRadius);
		lKernel2.CreateStructuringElement();

		m_BinaryMorphologicalOpeningImageFilter->SetKernel(lKernel2);
		m_BinaryMorphologicalOpeningImageFilter->SetForegroundValue(1);
		//Mask inverted
		m_AOTMaskInvertFilter = OneBandEqualFilterType::New();
		m_AOTMaskInvertFilter->SetInput(m_BinaryMorphologicalOpeningImageFilter->GetOutput());
		m_AOTMaskInvertFilter->GetFunctor().SetThresholdValue(0);
		// Link BinaryFunctorImageFilter, replace 0 values by NoData
		m_BinaryFunctorImageFilter->SetInput2(m_BinaryMorphologicalOpeningImageFilter->GetOutput());
		m_BinaryFunctorImageFilter->GetFunctor().SetDefaultValue(l_RealL2NoData);
		m_BinaryFunctorImageFilter->GetFunctor().SetBackgroundValue(0);
		// Link StreamingConditionalStatisticsVectorImageFilter : compute the mean value
		m_StreamingConditionalStatisticsImageFilter->SetInput(m_BinaryFunctorImageFilter->GetOutput());
		m_StreamingConditionalStatisticsImageFilter->SetEnableVariance(false);
		m_StreamingConditionalStatisticsImageFilter->SetExcludedValue(l_RealL2NoData);
		m_StreamingConditionalStatisticsImageFilter->GetStreamer()->SetAutomaticAdaptativeStreaming(GetParameterInt("ram"));
		m_StreamingConditionalStatisticsImageFilter->Update();

		// Get the boolean "has valid pixel" in StreamingConditionalStatisticsVect.Im.Filter
		const bool lHasValidPixel = (m_StreamingConditionalStatisticsImageFilter->GetNbOfValidValues() != 0);
		SetParameterOutputImagePixelType("aotcomputed", ImagePixelType_float);
		if (l_useCams)
		{
			m_AOTSelectionFilter->SetInput(0,m_BinaryFunctorImageFilter->GetOutput());
			m_AOTSelectionFilter->GetFunctor().SetReplacedValue(l_RealL2NoData);
			SetParameterOutputImage<OutputImageType>("aotcomputed",m_AOTSelectionFilter->GetOutput());
		} else {
			SetParameterOutputImage<OutputImageType>("aotcomputed",m_BinaryFunctorImageFilter->GetOutput());
		}
		SetParameterOutputImage<OutputMaskType>("aotmask",m_AOTMaskInvertFilter->GetOutput());
		SetParameterOutputImagePixelType("aotmask", ImagePixelType_uint8);
		SetParameterFloat("aotmean", m_StreamingConditionalStatisticsImageFilter->GetMean());
		SetParameterString("aotvalid", vns::Utilities::BoolToString(lHasValidPixel));

	}

private:
	/** Filters declaration */
	MTThreholdsFilterPointerType m_MTThresholdsFilter;
	MTComputeAOTImageFilterPointerType m_MTComputeAOTImageFilter;
	MSComputeAOTImageFilterPointerType m_MSComputeAOTImageFilter;
	STComputeAOTImageFilterPointerType m_STComputeAOTImageFilter;
	DilateImageFilterPointerType m_DilateWaterMaskFilter;
	DilateImageFilterPointerType m_DilateSnowMaskFilter;
	ImageFileReaderType::Pointer m_Reader;
	BinaryFunctorImageFilterPointerType m_BinaryFunctorImageFilter;
	BinaryMorphologicalOpeningImageFilterPointerType m_BinaryMorphologicalOpeningImageFilter;
	StreamingConditionalStatisticsImageFilterPointerType m_StreamingConditionalStatisticsImageFilter;
    //Invert the AOT computed mask
    OneBandFilterPointerType m_AOTMaskInvertFilter;
    //Replace AOT to CAMS based on mask
    AOTSelectionPointerFilterType m_AOTSelectionFilter;
    AtomicRegistry<itk::MetaImageIOFactory> m_MetaRegistry;
};

}
}

OTB_APPLICATION_EXPORT(vns::Wrapper::AOTEstimation)
