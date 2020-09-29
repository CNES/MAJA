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
#include "vnsInterpolateForEnvCorrectionImageFilter.h"
#include "vnsLookUpTableFileReader.h"
#include "vnsSimpleLutXMLFileHandler.h"


namespace vns
{
namespace Wrapper
{

using namespace otb::Wrapper;

class InterpolateForEnvCorr : public Application
{
public:
	/** Standard class typedefs. */
	typedef InterpolateForEnvCorr Self;
	typedef otb::Wrapper::Application     Superclass;
	typedef itk::SmartPointer<Self>       Pointer;
	typedef itk::SmartPointer<const Self> ConstPointer;

	/** Standard macro */
	itkNewMacro(Self);

	itkTypeMacro(InterpolateForEnvCorr, otb::Wrapper::Application);

	/** Image typedef */
	typedef VNSRealVectorImageType InputVectorImageType;
	typedef InputVectorImageType::ConstPointer InputVectorImageConstPointerType;
	typedef InputVectorImageType::Pointer InputVectorImagePointerType;
	typedef InputVectorImageType::PixelType InputVectorImagePixelType;
	typedef InputVectorImageType::SizeType SizeType;
	typedef VNSMaskImageType InputMaskType;
	typedef InputMaskType::Pointer InputMaskPointerType;
	typedef InputMaskType::ConstPointer InputMaskConstPointerType;
	typedef InputMaskType::PixelType   InputMaskPixelType;
	typedef VNSRealVectorImageType OutputVectorImageType;
	typedef OutputVectorImageType::Pointer OutputVectorImagePointerType;
	typedef OutputVectorImageType::ConstPointer OutputVectorImageConstPointerType;
	typedef OutputVectorImageType::RegionType OutputVectorImageRegionType;
	typedef OutputVectorImageType::InternalPixelType OutputVectorImageInternalPixelType;
	typedef OutputVectorImageInternalPixelType NoDataType;
	typedef OutputVectorImageType::IndexType OutputVectorImageIndexType;
	typedef OutputVectorImageType::SizeType OutputVectorImageSizeType;
	typedef OutputVectorImageType::PixelType OutputVectorPixelType;
	typedef OutputVectorImageType::InternalPixelType OutputVectorInternalPixelType;
	typedef VNSRealImageType InputImageType;
	typedef InputImageType::Pointer InputImagePointerType;
	typedef InputImageType::ConstPointer InputImageConstPointerType;
	typedef InputImageType::PixelType InputImagePixelType;
	typedef InputImageType::InternalPixelType InputImageInternalPixelType;
	typedef VNSDTMImageType InputDTMType;
	typedef InputDTMType::Pointer InputDTMPointerType;
	typedef InputDTMType::ConstPointer InputDTMConstPointerType;
	typedef InputDTMType::PixelType InputDTMPixelType;
	typedef InputDTMType::InternalPixelType InputDTMInternalPixelType;
	typedef VNSLUT3DType LUT3DType;
	typedef LUT3DType::Pointer LUT3DPointerType;
	typedef LUT3DType::PixelType LUT3DPixelType;
	typedef LUT3DType::ParameterValuesType ParameterValues3DType;
	typedef VNSLUT2DType LUT2DType;
	typedef LUT2DType::Pointer LUT2DPointerType;
	typedef LUT2DType::PixelType LUT2DPixelType;
	typedef LUT2DType::ParameterValuesType ParameterValues2DType;
	/**LutXml handler typedef */
	typedef SimpleLutXMLFileHandler LutXMLHandlerType;
	typedef LutXMLHandlerType::Pointer LutXMLHandlerPointer;
	//LookupTable Reader
	typedef LookUpTableFileReader<LUT3DType> LookUpTable3DReaderType;
	typedef LookUpTable3DReaderType::Pointer LookUpTable3DReaderPointer;
	typedef LookUpTableFileReader<LUT2DType> LookUpTable2DReaderType;
	typedef LookUpTable2DReaderType::Pointer LookUpTable2DReaderPointer;
	//Interpolate for env corr
	typedef InterpolateForEnvCorrectionImageFilter<InputImageType, InputMaskType, InputDTMType, LUT3DType, LUT2DType,
			OutputVectorImageType> InterpolateForEnvCorrectionImageFilterType;
	typedef InterpolateForEnvCorrectionImageFilterType::Pointer InterpolateForEnvCorImageFilterPointerType;


private:
	void DoInit()
	{
		SetName("InterpolateForEnvCorr");
		SetDescription("Generate the TDIR TDIF And Albedo for the environment correction.");
		Loggers::GetInstance()->Initialize(GetName());
		// Documentation
		SetDocLongDescription("This application computes the TDIR TDIF And Albedo for the environment correction");
		SetDocLimitations("None");
		SetDocAuthors("MAJA-Team");
		SetDocSeeAlso("MAJA Doc");

		AddDocTag("Statistics");

		AddParameter(ParameterType_InputFilename,  "luttdir",   "LutTDIR");
		SetParameterDescription("luttdir", "Lut TDIR");
		AddParameter(ParameterType_InputFilename,  "luttdif",   "LutTDIF");
		SetParameterDescription("luttdif", "Lut TDIF");
		AddParameter(ParameterType_InputFilename,  "lutalbd",   "LutAlbedo");
		SetParameterDescription("lutalbd", "Lut Albedo");
		AddParameter(ParameterType_InputImage,  "aot",   "AOT image");
		SetParameterDescription("aot", "AOT image at coarse");
		AddParameter(ParameterType_InputImage,  "dtm",   "DTM image");
		SetParameterDescription("dtm", "Image used as DTM at coarse");
		AddParameter(ParameterType_InputImage,  "edg",   "EDG image");
		SetParameterDescription("edg", "Image used as EDG at coarse");

		AddParameter(ParameterType_StringList, "thetav", "ThetaV");


		AddParameter(ParameterType_OutputImage, "tdir", "Computed TDIR");
		SetParameterDescription("tdir","Computed TDIR");
		SetParameterOutputImagePixelType("tdir", ImagePixelType_double);
		AddParameter(ParameterType_OutputImage, "tdif", "Computed TDIF");
		SetParameterDescription("tdif","Computed TDIF");
		SetParameterOutputImagePixelType("tdif", ImagePixelType_double);
		AddParameter(ParameterType_OutputImage, "albd", "Computed Albedo");
		SetParameterDescription("albd","Computed Albedo");
		SetParameterOutputImagePixelType("albd", ImagePixelType_double);

		AddRAMParameter("ram");
		SetDefaultParameterInt("ram",2048);
	}

	void DoUpdateParameters()
	{

	}


	void DoExecute()
	{
		// Init filters
		m_InterpolateForEnvCor = InterpolateForEnvCorrectionImageFilterType::New();
		m_lookUpTableTDIRReader = LookUpTable3DReaderType::New();
		m_lookUpTableTDIFReader = LookUpTable3DReaderType::New();
		m_lookUpTableALBDReader = LookUpTable2DReaderType::New();
		//GetImagePointers
		InputImageConstPointerType lIPAOTSubPtr = this->GetParameterDoubleImage("aot");
		InputDTMConstPointerType lDTMSubPtr = this->GetParameterDoubleImage("dtm");
		InputMaskConstPointerType lIPEDGPtr = this->GetParameterUInt8Image("edg");


		//Get param
		const Utilities::ListOfDoubles l_ThetaV = Utilities::StringListAsDouble(
					this->GetParameterStringList("thetav"));
		//Load luts
		const std::string l_TDIRFilename = GetParameterString("luttdir");
		LutXMLHandlerPointer l_LUTXMLHandler = LutXMLHandlerType::New();
		// Load the XML file and check with the schema
		l_LUTXMLHandler->LoadFile(l_TDIRFilename);
		const SimpleLutXMLContainer& l_lutxml = l_LUTXMLHandler->GetLutContainer();
		// Read informations from the GIPP file
		Utilities::ListOfDoubles l_GIP_L2DIRT_ZenithalAngleIndexes = l_lutxml.GetZenithal_Angle_Indexes();
		Utilities::ListOfDoubles l_GIP_L2DIRT_AltitudeIndexes = l_lutxml.GetAltitude_Indexes();
		Utilities::ListOfDoubles l_GIP_L2DIRT_AOTIndexes = l_lutxml.GetAOT_Indexes();
		// Read the files
		Utilities::ListOfFilenames l_GIP_L2DIRT_ListOfFilenames = l_lutxml.GetListOfPackaged_DBL_Files();
		vnsLogDebugMacro("=================================================================")
		vnsLogDebugMacro("  l_GIP_L2DIRT_ListOfFilenames number of filenames: "<<l_GIP_L2DIRT_ListOfFilenames.size())
		vnsLogDebugMacro("  l_GIP_L2DIRT_ZenithalAngleIndexes: "<<l_GIP_L2DIRT_ZenithalAngleIndexes.size())
		vnsLogDebugMacro("  l_GIP_L2DIRT_AltitudeIndexes: "<<l_GIP_L2DIRT_AltitudeIndexes.size())
		vnsLogDebugMacro("  l_GIP_L2DIRT_AOTIndexes: "<<l_GIP_L2DIRT_AOTIndexes.size())
		// Get the number of file that matches with the number of band
		unsigned int fileNumber = l_GIP_L2DIRT_ListOfFilenames.size();
		// For each spectral band, add the filename of the LUT to the reader
		for (unsigned int file = 0; file < fileNumber; file++)
		{
			// Set the filename of each band
			m_lookUpTableTDIRReader->AddBandFilename(l_GIP_L2DIRT_ListOfFilenames[file]);
		}
		// parameters are added one by one to the LUT
		ParameterValues3DType l_DIRTParam1;
		l_DIRTParam1.ParameterName = "Solar_Zenith_Angle_Indexes";
		l_DIRTParam1.ParameterValues = l_GIP_L2DIRT_ZenithalAngleIndexes;
		m_lookUpTableTDIRReader->AddParameterValues(l_DIRTParam1);
		ParameterValues3DType l_DIRTParam2;
		l_DIRTParam2.ParameterName = "Altitude_Indexes";
		l_DIRTParam2.ParameterValues = l_GIP_L2DIRT_AltitudeIndexes;
		m_lookUpTableTDIRReader->AddParameterValues(l_DIRTParam2);
		ParameterValues3DType l_DIRTParam3;
		l_DIRTParam3.ParameterName = "AOT_Indexes";
		l_DIRTParam3.ParameterValues = l_GIP_L2DIRT_AOTIndexes;
		m_lookUpTableTDIRReader->AddParameterValues(l_DIRTParam3);
		// Generate the LUT
		m_lookUpTableTDIRReader->GenerateLUT();
		m_LUTTDIR = m_lookUpTableTDIRReader->GetLUT();

		//TDIF load
		const std::string l_TDIFFilename = GetParameterString("luttdif");
		l_LUTXMLHandler = LutXMLHandlerType::New();
		// Load the XML file and check with the schema
		l_LUTXMLHandler->LoadFile(l_TDIFFilename);
		const SimpleLutXMLContainer& l_lutxml2 = l_LUTXMLHandler->GetLutContainer();
		// Read informations from the GIPP file
		Utilities::ListOfDoubles l_GIP_L2DIFT_ZenithalAngleIndexes = l_lutxml2.GetZenithal_Angle_Indexes();
		Utilities::ListOfDoubles l_GIP_L2DIFT_AltitudeIndexes = l_lutxml2.GetAltitude_Indexes();
		Utilities::ListOfDoubles l_GIP_L2DIFT_AOTIndexes = l_lutxml2.GetAOT_Indexes();
		// Read the files
		Utilities::ListOfFilenames l_GIP_L2DIFT_ListOfFilenames = l_lutxml2.GetListOfPackaged_DBL_Files();
		vnsLogDebugMacro("=================================================================")
		vnsLogDebugMacro("  l_GIP_L2DIFT_ListOfFilenames number of filenames: "<<l_GIP_L2DIFT_ListOfFilenames.size())
		vnsLogDebugMacro("  l_GIP_L2DIFT_ZenithalAngleIndexes: "<<l_GIP_L2DIFT_ZenithalAngleIndexes.size())
		vnsLogDebugMacro("  l_GIP_L2DIFT_AltitudeIndexes: "<<l_GIP_L2DIFT_AltitudeIndexes.size())
		vnsLogDebugMacro("  l_GIP_L2DIFT_AOTIndexes: "<<l_GIP_L2DIFT_AOTIndexes.size())
		// Get the number of file that matches with the number of band
		fileNumber = l_GIP_L2DIFT_ListOfFilenames.size();
		// For each spectral band, add the filename of the LUT to the reader
		for (unsigned int file = 0; file < fileNumber; file++)
		{
			// Set the filename of each band
			m_lookUpTableTDIFReader->AddBandFilename(l_GIP_L2DIFT_ListOfFilenames[file]);
		}
		// parameters are added one by one to the LUT
		ParameterValues3DType l_DIFTParam1;
		l_DIFTParam1.ParameterName = "Solar_Zenith_Angle_Indexes";
		l_DIFTParam1.ParameterValues = l_GIP_L2DIFT_ZenithalAngleIndexes;
		m_lookUpTableTDIFReader->AddParameterValues(l_DIFTParam1);
		ParameterValues3DType l_DIFTParam2;
		l_DIFTParam2.ParameterName = "Altitude_Indexes";
		l_DIFTParam2.ParameterValues = l_GIP_L2DIFT_AltitudeIndexes;
		m_lookUpTableTDIFReader->AddParameterValues(l_DIFTParam2);
		ParameterValues3DType l_DIFTParam3;
		l_DIFTParam3.ParameterName = "AOT_Indexes";
		l_DIFTParam3.ParameterValues = l_GIP_L2DIFT_AOTIndexes;
		m_lookUpTableTDIFReader->AddParameterValues(l_DIFTParam3);
		// Generate the LUT
		m_lookUpTableTDIFReader->GenerateLUT();
		m_LUTTDIF = m_lookUpTableTDIFReader->GetLUT();

		//ALBD load
		const std::string l_ALBDFilename = GetParameterString("lutalbd");
		l_LUTXMLHandler = LutXMLHandlerType::New();
		// Load the XML file and check with the schema
		l_LUTXMLHandler->LoadFile(l_ALBDFilename);
		const SimpleLutXMLContainer& l_lutxml3 = l_LUTXMLHandler->GetLutContainer();
		// Read informations from the GIPP file
		// Read informations from the GIPP file
		Utilities::ListOfDoubles l_GIP_L2ALBD_AltitudeIndexes = l_lutxml3.GetAltitude_Indexes();
		Utilities::ListOfDoubles l_GIP_L2ALBD_AOTIndexes = l_lutxml3.GetAOT_Indexes();
		Utilities::ListOfFilenames l_GIP_L2ALBD_ListOfFilenames = l_lutxml3.GetListOfPackaged_DBL_Files();
		vnsLogDebugMacro("=================================================================")
		vnsLogDebugMacro("  l_GIP_L2ALBD_ListOfFilenames number of filenames: "<<l_GIP_L2ALBD_ListOfFilenames.size())
		vnsLogDebugMacro("  l_GIP_L2ALBD_AltitudeIndexes: "<<l_GIP_L2ALBD_AltitudeIndexes.size())
		vnsLogDebugMacro("  l_GIP_L2ALBD_AOTIndexes: "<<l_GIP_L2ALBD_AOTIndexes.size())
		 // Get the number of file that matches with the number of band
		fileNumber = l_GIP_L2ALBD_ListOfFilenames.size();
		// For each spectral band, add the filename of the LUT to the reader
		for (unsigned int file = 0; file < fileNumber; file++)
		{
			// Set the filename of each band
			m_lookUpTableALBDReader->AddBandFilename(l_GIP_L2ALBD_ListOfFilenames[file]);
		}
		// parameters are added one by one to the LUT
		ParameterValues2DType l_ALBDParam2;
		l_ALBDParam2.ParameterName = "Altitude_Indexes";
		l_ALBDParam2.ParameterValues = l_GIP_L2ALBD_AltitudeIndexes;
		m_lookUpTableALBDReader->AddParameterValues(l_ALBDParam2);

		ParameterValues2DType l_ALBDParam3;
		l_ALBDParam3.ParameterName = "AOT_Indexes";
		l_ALBDParam3.ParameterValues = l_GIP_L2ALBD_AOTIndexes;
		m_lookUpTableALBDReader->AddParameterValues(l_ALBDParam3);
		// Generate the LUT
		m_lookUpTableALBDReader->GenerateLUT();
		m_LUTALBD = m_lookUpTableALBDReader->GetLUT();
		//Get number of bands
		const unsigned int lNbOfBands = m_LUTTDIF->GetNumberOfComponentsPerPixel();

		// 2 - Interpolations with AOTSub and the different LUTs
		m_InterpolateForEnvCor->SetLUT_Satm(m_LUTALBD);
		m_InterpolateForEnvCor->SetLUT_Tdif(m_LUTTDIF);
		m_InterpolateForEnvCor->SetLUT_Tdir(m_LUTTDIR);
		m_InterpolateForEnvCor->SetThetaV(l_ThetaV);
		m_InterpolateForEnvCor->SetIPAOTSub(lIPAOTSubPtr);
		m_InterpolateForEnvCor->SetIPEDG(lIPEDGPtr);
		m_InterpolateForEnvCor->SetDTMSub(lDTMSubPtr);
		m_InterpolateForEnvCor->SetNbOfBands(lNbOfBands);

		//Set the output
		SetParameterOutputImage<OutputVectorImageType>("tdir",m_InterpolateForEnvCor->GetTdir());
		SetParameterOutputImage<OutputVectorImageType>("tdif",m_InterpolateForEnvCor->GetTdif());
		SetParameterOutputImage<OutputVectorImageType>("albd",m_InterpolateForEnvCor->GetSatm());
	}

	/** Filters declaration */
	InterpolateForEnvCorImageFilterPointerType m_InterpolateForEnvCor;
	LUT3DPointerType m_LUTTDIR;
	LUT3DPointerType m_LUTTDIF;
	LUT2DPointerType m_LUTALBD;
	LookUpTable3DReaderPointer m_lookUpTableTDIRReader;
	LookUpTable3DReaderPointer m_lookUpTableTDIFReader;
	LookUpTable2DReaderPointer m_lookUpTableALBDReader;

};

}
}

OTB_APPLICATION_EXPORT(vns::Wrapper::InterpolateForEnvCorr)
