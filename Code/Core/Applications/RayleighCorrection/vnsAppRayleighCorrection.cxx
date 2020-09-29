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
#include "vnsRayleighCorrectionImageFilter.h"
#include "itkImageFileReader.h"
#include "vnsAtomicRegistry.h"

namespace vns
{
namespace Wrapper
{

using namespace otb::Wrapper;

class RayleighCorrection : public Application
{
public:
	/** Standard class typedefs. */
	typedef RayleighCorrection Self;
	typedef otb::Wrapper::Application     Superclass;
	typedef itk::SmartPointer<Self>       Pointer;
	typedef itk::SmartPointer<const Self> ConstPointer;

	/** Standard macro */
	itkNewMacro(Self);

	itkTypeMacro(RayleighCorrection, otb::Wrapper::Application);

	/** Image typedef */
	typedef DoubleVectorImageType TOAImageType;
	typedef VNSMaskImageType                        InputMaskType;
	typedef InputMaskType::Pointer                  InputMaskPointer;
	typedef InputMaskType::ConstPointer             InputMaskConstPointer;
	typedef VNSMaskVectorImageType                  InputMaskVectorType;
	typedef InputMaskVectorType::Pointer            InputMaskVectorPointer;
	typedef InputMaskVectorType::ConstPointer       InputMaskVectorConstPointer;
	/** Altitude input image typedefs. */
	typedef VNSDTMImageType DTMImageType;
	typedef DTMImageType::ConstPointer DTMImageConstPointer;
	//Reduced output lut typedef
	typedef VNSLUT3DType ReducedLutType;
	typedef ReducedLutType::ConstPointer ReducedLutConstPointer;
	typedef ReducedLutType::Pointer ReducedLutPointer;
	typedef ReducedLutType::ParameterValuesType ParameterValuesType;
	/**LutXml handler typedef */
	typedef SimpleLutXMLFileHandler LutXMLHandlerType;
	typedef LutXMLHandlerType::Pointer LutXMLHandlerPointer;
	typedef itk::ImageFileReader<ReducedLutType> ImageFileReaderType;

private:
	void DoInit()
	{
		SetName("RayleighCorrection");
		SetDescription("Generate the RayleighCorrection.");
		Loggers::GetInstance()->Initialize(GetName());
		// Documentation
		SetDocLongDescription("This application computes the RayleighCorrection");
		SetDocLimitations("None");
		SetDocAuthors("MAJA-Team");
		SetDocSeeAlso("MAJA Doc");

		AddDocTag("Statistics");

		AddParameter(ParameterType_InputFilename,  "lutimage",   "Lut3D");
		SetParameterDescription("lutimage", "Reduce LUT");
		AddParameter(ParameterType_InputFilename,  "lutheader",   "Lut header");
		SetParameterDescription("lutheader", "Reduce LUT");
		AddParameter(ParameterType_InputImage,  "toa",   "TOA image");
		SetParameterDescription("toa", "TOA image at coarse all bands");
		AddParameter(ParameterType_InputImage,  "edg",   "EDG image");
		SetParameterDescription("edg", "Image used as edge at coarse");
		AddParameter(ParameterType_InputImage,  "dtm",   "DTM image");
		SetParameterDescription("dtm", "Image used as DTM at coarse");

		AddParameter(ParameterType_Float, "nodata","NoData value");
		AddParameter(ParameterType_Float, "defaultaot","Default AOT");

		AddParameter(ParameterType_OutputImage, "tocr", "Corrected TOA");
		SetParameterDescription("tocr","Corrected TOA");
		SetParameterOutputImagePixelType("tocr", ImagePixelType_double);

		AddRAMParameter("ram");
		SetDefaultParameterInt("ram",2048);

	}

	void DoUpdateParameters()
	{

	}


	void DoExecute()
	{
		// Init filters
		m_RayleighCorrection = RayleighCorrectionImageFilter::New();
		//GetImagePointers
		TOAImageType::Pointer l_toa = this->GetParameterDoubleVectorImage("toa");
		InputMaskConstPointer l_edg = this->GetParameterUInt8Image("edg");
		DTMImageConstPointer l_dtm = this->GetParameterDoubleImage("dtm");
		//GetParams
		const double l_nodata = GetParameterFloat("nodata");
		const double l_defaultAOT = GetParameterFloat("defaultaot");

		//Load the LUT
		//--------------------------------------------
		//Load the lut image
		// ---------------------------------------------------------------------------------------------
		m_Reader = ImageFileReaderType::New();
		m_Reader->SetFileName(GetParameterString("lutimage"));
		// ---------------------------------------------------------------------------------------------
		// Load the LUT
		m_Reader->Update();

		// ---------------------------------------------------------------------------------------------
		// Get pointer to the output LUT
		m_lut = m_Reader->GetOutput();

		//Load the lut header
		const std::string l_GIPPFilename = GetParameterString("lutheader");
		LutXMLHandlerPointer l_LUTXMLHandler = LutXMLHandlerType::New();
		// Load the XML file and check with the schema
		l_LUTXMLHandler->LoadFile(l_GIPPFilename);
		const SimpleLutXMLContainer& l_lutxml = l_LUTXMLHandler->GetLutContainer();

		// Read informations from the GIPP file
		const Utilities::ListOfDoubles l_GIP_L2TOCR_SolarZenithalAngleIndexes = l_lutxml.GetSolar_Zenith_Angle_Indexes();
		const Utilities::ListOfDoubles l_GIP_L2TOCR_ViewZenithalAngleIndexes = l_lutxml.GetView_Zenith_Angle_Indexes();
		const Utilities::ListOfDoubles l_GIP_L2TOCR_RelativeAzimuthAngleIndexes = l_lutxml.GetRelative_Azimuth_Angle_Indexes();
		const Utilities::ListOfDoubles l_GIP_L2TOCR_AltitudeIndexes = l_lutxml.GetAltitude_Indexes();
		const Utilities::ListOfDoubles l_GIP_L2TOCR_AOTIndexes = l_lutxml.GetAOT_Indexes();
		const Utilities::ListOfDoubles l_GIP_L2TOCR_TOAReflectanceIndexes = l_lutxml.GetTOA_Reflectance_Indexes();

		vnsLogDebugMacro("  l_GIP_L2TOCR_ViewZenithalAngleIndexes: "<<l_GIP_L2TOCR_ViewZenithalAngleIndexes.size());
		vnsLogDebugMacro("  l_GIP_L2TOCR_ViewZenithalAngleIndexes: "<<l_GIP_L2TOCR_ViewZenithalAngleIndexes.size());
		vnsLogDebugMacro("  l_GIP_L2TOCR_RelativeAzimuthAngleIndexes: "<<l_GIP_L2TOCR_RelativeAzimuthAngleIndexes.size());
		vnsLogDebugMacro("  l_GIP_L2TOCR_AltitudeIndexes: "<<l_GIP_L2TOCR_AltitudeIndexes.size());
		vnsLogDebugMacro("  l_GIP_L2TOCR_AOTIndexes: "<<l_GIP_L2TOCR_AOTIndexes.size());
		vnsLogDebugMacro("  l_GIP_L2TOCR_TOAReflectanceIndexes: "<<l_GIP_L2TOCR_TOAReflectanceIndexes.size());

		/* parameters are added one by one to the LUT */
		ParameterValuesType l_TOCRParam4;
		l_TOCRParam4.ParameterName = "Altitude_Indexes";
		l_TOCRParam4.ParameterValues = l_GIP_L2TOCR_AltitudeIndexes;
		m_lut->AddParameterValues(l_TOCRParam4);

		ParameterValuesType l_TOCRParam5;
		l_TOCRParam5.ParameterName = "AOT_Indexes";
		l_TOCRParam5.ParameterValues = l_GIP_L2TOCR_AOTIndexes;
		m_lut->AddParameterValues(l_TOCRParam5);

		ParameterValuesType l_TOCRParam6;
		l_TOCRParam6.ParameterName = "TOA_Reflectance_Indexes";
		l_TOCRParam6.ParameterValues = l_GIP_L2TOCR_TOAReflectanceIndexes;
		m_lut->AddParameterValues(l_TOCRParam6);


		vnsLogInfoMacro("'Rayleigh correction' processing ...")

        // Set the input reflectance image
        // --------------------------------------------------
        m_RayleighCorrection->SetIPTOACInput(l_toa);

        // Set the edge mask
        // --------------------------------------------------
        m_RayleighCorrection->SetIPEDGInput(l_edg);

        // Set the DTM input
        // --------------------------------------------------
        m_RayleighCorrection->SetDTMInput(l_dtm);

        // Set default aerosol value
        // --------------------------------------------------
        m_RayleighCorrection->SetDefaultAOT(l_defaultAOT);
        vnsLogInfoMacro("DefaultAOT: "<<l_defaultAOT);
        // Set the no data value
        // ----------------------
        m_RayleighCorrection->SetNoData(l_nodata);
        vnsLogInfoMacro("NoData: "<<l_nodata)
        m_RayleighCorrection->SetLUT(m_lut);

        //Set the output
        this->SetParameterOutputImage<TOAImageType>("tocr",m_RayleighCorrection->GetOutput());
	}

	/** Filters declaration */
	RayleighCorrectionImageFilter::Pointer m_RayleighCorrection;
	ImageFileReaderType::Pointer m_Reader;
	/** LUT */
	ReducedLutPointer m_lut;

  AtomicRegistry<itk::MetaImageIOFactory> m_MetaRegistry;
};

}
}

OTB_APPLICATION_EXPORT(vns::Wrapper::RayleighCorrection)
