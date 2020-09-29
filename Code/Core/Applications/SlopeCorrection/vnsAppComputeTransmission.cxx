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
#include "vnsSimpleLutXMLFileHandler.h"
#include "itkImageFileReader.h"
#include "vnsLookUpTableFileReader.h"
#include "vnsComputeTransmissionImageFilter.h"

namespace vns
{
namespace Wrapper
{

using namespace otb::Wrapper;

class ComputeTransmission : public Application
{
public:
	/** Standard class typedefs. */
	typedef ComputeTransmission Self;
	typedef otb::Wrapper::Application     Superclass;
	typedef itk::SmartPointer<Self>       Pointer;
	typedef itk::SmartPointer<const Self> ConstPointer;

	/** Standard macro */
	itkNewMacro(Self);

	itkTypeMacro(ComputeTransmission, otb::Wrapper::Application);

	/** Image typedef */
	typedef VNSMaskImageType InputMaskType;
	typedef InputMaskType::Pointer InputMaskPointerType;
	typedef InputMaskType::ConstPointer InputMaskConstPointerType;
	typedef VNSMaskImageType OutputMaskType;
	typedef OutputMaskType::Pointer OutputMaskPointerType;
	typedef OutputMaskType::ConstPointer OutputMaskConstPointerType;
	typedef OutputMaskType::PixelType OutputMaskPixelType;

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

	typedef VNSRealVectorImageType OutputVectorImageType;
	typedef OutputVectorImageType::Pointer OutputVectorImagePointerType;
	typedef OutputVectorImageType::ConstPointer OutputVectorImageConstPointerType;
	typedef OutputVectorImageType::RegionType OutputVectorImageRegionType;

	typedef VNSLUT3DType LUTType;
	typedef LUTType::Pointer LUTPointerType;
	typedef LUTType::PixelType LUTPixelType;
	typedef LUTType::ParameterValuesType ParameterValuesType;
	/**LutXml handler typedef */
	typedef SimpleLutXMLFileHandler LutXMLHandlerType;
	typedef LutXMLHandlerType::Pointer LutXMLHandlerPointer;
	//LookupTable Reader
	typedef LookUpTableFileReader<LUTType> LookUpTableReaderType;
	typedef LookUpTableReaderType::Pointer LookUpTableReaderPointer;

	//Compute transmission typedef
	typedef ComputeTransmissionImageFilter<InputImageType, InputMaskType, InputDTMType, LUTType, OutputVectorImageType>
	ComputeTransmissionImageFilterType;
	typedef ComputeTransmissionImageFilterType::Pointer ComputeTransmissionImageFilterPointerType;

private:
	void DoInit()
	{
		SetName("ComputeTransmission");
		SetDescription("Generate the Transmission.");
		Loggers::GetInstance()->Initialize(GetName());
		// Documentation
		SetDocLongDescription("This application computes the ComputeTransmission");
		SetDocLimitations("None");
		SetDocAuthors("MAJA-Team");
		SetDocSeeAlso("MAJA Doc");

		AddDocTag("Statistics");

		AddParameter(ParameterType_InputFilename,  "luttdir",   "LutTDIR");
		SetParameterDescription("luttdir", "Lut TDIR");
		AddParameter(ParameterType_InputFilename,  "luttdif",   "LutTDIF");
		SetParameterDescription("luttdif", "Lut TDIF");
		AddParameter(ParameterType_InputImage,  "aot",   "AOT image");
		SetParameterDescription("aot", "AOT image at coarse");
		AddParameter(ParameterType_InputImage,  "dtm",   "DTM image");
		SetParameterDescription("dtm", "Image used as DTM at coarse");
		AddParameter(ParameterType_InputImage,  "edg",   "EDG image");
		SetParameterDescription("edg", "Image used as EDG at coarse");

		AddParameter(ParameterType_Float, "thetas", "Thetas");

		AddParameter(ParameterType_OutputImage, "tdir", "Computed TDIR");
		SetParameterDescription("tdir","Computed TDIR");
		SetParameterOutputImagePixelType("tdir", ImagePixelType_double);
		AddParameter(ParameterType_OutputImage, "tdif", "Computed TDIF");
		SetParameterDescription("tdif","Computed TDIF");
		SetParameterOutputImagePixelType("tdif", ImagePixelType_double);

		AddRAMParameter("ram");
		SetDefaultParameterInt("ram",2048);

	}

	void DoUpdateParameters()
	{

	}


	void DoExecute()
	{
		// Init filters
		m_ComputeTransmission = ComputeTransmissionImageFilterType::New();
		m_lookUpTableTDIRReader = LookUpTableReaderType::New();
		m_lookUpTableTDIFReader = LookUpTableReaderType::New();
		//GetImagePointers
		InputImageConstPointerType lIPAOTSubPtr = this->GetParameterDoubleImage("aot");
		InputDTMConstPointerType lDTMSubPtr = this->GetParameterDoubleImage("dtm");
		InputMaskConstPointerType lIPEDGPtr = this->GetParameterUInt8Image("edg");

		//Params
		const double l_ThetaS = this->GetParameterFloat("thetas");

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
		ParameterValuesType l_DIRTParam1;
		l_DIRTParam1.ParameterName = "Solar_Zenith_Angle_Indexes";
		l_DIRTParam1.ParameterValues = l_GIP_L2DIRT_ZenithalAngleIndexes;
		m_lookUpTableTDIRReader->AddParameterValues(l_DIRTParam1);
		ParameterValuesType l_DIRTParam2;
		l_DIRTParam2.ParameterName = "Altitude_Indexes";
		l_DIRTParam2.ParameterValues = l_GIP_L2DIRT_AltitudeIndexes;
		m_lookUpTableTDIRReader->AddParameterValues(l_DIRTParam2);
		ParameterValuesType l_DIRTParam3;
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
		ParameterValuesType l_DIFTParam1;
		l_DIFTParam1.ParameterName = "Solar_Zenith_Angle_Indexes";
		l_DIFTParam1.ParameterValues = l_GIP_L2DIFT_ZenithalAngleIndexes;
		m_lookUpTableTDIFReader->AddParameterValues(l_DIFTParam1);
		ParameterValuesType l_DIFTParam2;
		l_DIFTParam2.ParameterName = "Altitude_Indexes";
		l_DIFTParam2.ParameterValues = l_GIP_L2DIFT_AltitudeIndexes;
		m_lookUpTableTDIFReader->AddParameterValues(l_DIFTParam2);
		ParameterValuesType l_DIFTParam3;
		l_DIFTParam3.ParameterName = "AOT_Indexes";
		l_DIFTParam3.ParameterValues = l_GIP_L2DIFT_AOTIndexes;
		m_lookUpTableTDIFReader->AddParameterValues(l_DIFTParam3);
		// Generate the LUT
		m_lookUpTableTDIFReader->GenerateLUT();
		m_LUTTDIF = m_lookUpTableTDIFReader->GetLUT();

		const unsigned int lNbOfBands = m_LUTTDIF->GetNumberOfComponentsPerPixel();

		// Init the ComputeTransmission filter
		m_ComputeTransmission->SetIPAOTSub(lIPAOTSubPtr);
		m_ComputeTransmission->SetIPEDG(lIPEDGPtr);
		m_ComputeTransmission->SetDTMSub(lDTMSubPtr);
		m_ComputeTransmission->SetNbOfBands(lNbOfBands);
		m_ComputeTransmission->SetThetaS(l_ThetaS);
		m_ComputeTransmission->SetLUT_Tdir(m_LUTTDIR);
		m_ComputeTransmission->SetLUT_Tdif(m_LUTTDIF);
		//Set the output
		SetParameterOutputImage<OutputVectorImageType>("tdir",m_ComputeTransmission->GetTdir());
		SetParameterOutputImage<OutputVectorImageType>("tdif",m_ComputeTransmission->GetTdif());
	}

	/** Filters declaration */
	ComputeTransmissionImageFilterPointerType m_ComputeTransmission;
	/** LUT */
	LUTPointerType m_LUTTDIR;
	LUTPointerType m_LUTTDIF;
	LookUpTableReaderPointer m_lookUpTableTDIRReader;
	LookUpTableReaderPointer m_lookUpTableTDIFReader;


};

}
}

OTB_APPLICATION_EXPORT(vns::Wrapper::ComputeTransmission)
