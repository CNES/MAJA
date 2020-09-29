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
#include "vnsLookUpTableExtractor.h"
#include "vnsLookUpTableFileReader.h"
#include "vnsWaterAmountImageGenerator.h"

namespace vns
{
namespace Wrapper
{

using namespace otb::Wrapper;

class WaterAmountGeneratorFilter : public Application
{
public:
	/** Standard class typedefs. */
	typedef WaterAmountGeneratorFilter Self;
	typedef otb::Wrapper::Application     Superclass;
	typedef itk::SmartPointer<Self>       Pointer;
	typedef itk::SmartPointer<const Self> ConstPointer;

	/** Standard macro */
	itkNewMacro(Self);

	itkTypeMacro(WaterAmountGeneratorFilter, otb::Application);

	/** Image typedef */
	/** WATV Look Up Table typedef */
	typedef VNSLUTMonoBand3DType GIP_L2WATV_LookUpTableType;
	typedef VNSLUTMonoBand1DType GIP_L2WATV_ReducedLookUpTableType;
	typedef LookUpTableExtractor<GIP_L2WATV_LookUpTableType, GIP_L2WATV_ReducedLookUpTableType> GIP_L2WATV_LookUpTableExtractorType;
	typedef LookUpTableFileReader<GIP_L2WATV_LookUpTableType> GIP_L2WATV_LookUpTableReaderType;
	typedef GIP_L2WATV_LookUpTableReaderType::Pointer GIP_L2WATV_LookUpTableReaderPointer;
	/** TOA input image typedefs. */
	typedef DoubleVectorImageType TOAImageType;
	typedef TOAImageType::ConstPointer TOAImageConstPointer;
	/** LUT image typedef */
	typedef VNSLUTMonoBand1DType LUTType;
	typedef LUTType::Pointer LUTPointer;
	typedef LUTType::ConstPointer LUTConstPointer;
	/** Vapor amount output image typedefs. */
	typedef VNSRealImageType VAPImageType;
	typedef VAPImageType::Pointer VAPImagePointer;
	typedef VAPImageType::ConstPointer VAPImageConstPointer;
	/** Water amount image generator typedefs */
	typedef WaterAmountImageGenerator<TOAImageType, VAPImageType, LUTType> WaterAmountImageGeneratorType;
	typedef WaterAmountImageGeneratorType::Pointer WaterAmountImageGeneratorPointer;
	/** Utils typedef */
	typedef Utilities::ListOfDoubles ListOfDoubles;
	typedef Utilities::ListOfFilenames ListOfFilenames;

	/**LutXml handler typedef */
	typedef SimpleLutXMLFileHandler LutXMLHandlerType;
	typedef LutXMLHandlerType::Pointer LutXMLHandlerPointer;

private:
	void DoInit()
	{
		SetName("WaterAmountGeneratorFilter");
		SetDescription("Generate the water amount.");
		Loggers::GetInstance()->Initialize(GetName());
		// Documentation
		SetDocLongDescription("This application computes the water amount");
		SetDocLimitations("None");
		SetDocAuthors("MAJA-Team");
		SetDocSeeAlso("MAJA Doc");

		AddDocTag("Statistics");

		AddParameter(ParameterType_InputImage,  "toa",   "TOA image");
		SetParameterDescription("toa", "TOA image at coarse all bands");
		AddParameter(ParameterType_String,  "gipwatv",   "WATV Gipp");
		SetParameterDescription("gipwatv", "WATV gipp");
		AddParameter(ParameterType_Int, "watervaporreferencebandid","WaterVapor ref Band");
		AddParameter(ParameterType_Int, "watervaporbandid","WaterVapor Band");
		AddParameter(ParameterType_Float, "vapnodatavalue","NoData for WaterAmount");
		AddParameter(ParameterType_Float, "watervapormaxthreshold","Max Water Amount");
		AddParameter(ParameterType_Float,"solarzenith","solarzenith");
		AddParameter(ParameterType_Float,"viewingzenith","viewingzenith");
		AddParameter(ParameterType_OutputImage, "wateramount", "Water Amount image");
		SetParameterDescription("wateramount","water amount");
		SetParameterOutputImagePixelType("wateramount", ImagePixelType_double);

		AddRAMParameter("ram");
		SetDefaultParameterInt("ram",2048);
	}

	void DoUpdateParameters()
	{

	}


	void DoExecute()
	{
		// Init filters
		m_WaterAmountGenerator = WaterAmountImageGeneratorType::New();
		//GetParams
		TOAImageConstPointer l_toa = this->GetParameterDoubleVectorImage("toa");
		//Load the LUT
		//--------------------------------------------
		const std::string l_GIPPFilename = GetParameterString("gipwatv");
		LutXMLHandlerPointer l_LUTXMLHandler = LutXMLHandlerType::New();
		// Load the XML file and check with the schema
		l_LUTXMLHandler->LoadFile(l_GIPPFilename);
		const SimpleLutXMLContainer& l_lutxml = l_LUTXMLHandler->GetLutContainer();

		// Initialize the LookUpTable reader
		//--------------------------------------------
		GIP_L2WATV_LookUpTableReaderPointer l_GIP_L2WATV_LookUpTableReader = GIP_L2WATV_LookUpTableReaderType::New();

		// Read informations from the GIPP file
		const ListOfDoubles& l_GIP_L2WATV_SolarZenithalAngleIndexes = l_lutxml.GetSolar_Zenith_Angle_Indexes();
		const ListOfDoubles& l_GIP_L2WATV_ViewZenithalAngleIndexes = l_lutxml.GetView_Zenith_Angle_Indexes();
		const ListOfDoubles& l_GIP_L2WATV_TOAReflectanceIndexes = l_lutxml.GetReflectance_Ratio_Indexes();

		vnsLogDebugMacro("=================================================================")
		vnsLogDebugMacro("  l_GIP_L2WATV_SolarZenithalAngleIndexes: "<<l_GIP_L2WATV_SolarZenithalAngleIndexes.size())
		vnsLogDebugMacro("  l_GIP_L2WATV_ViewZenithalAngleIndexes: "<<l_GIP_L2WATV_ViewZenithalAngleIndexes.size())
		vnsLogDebugMacro("  l_GIP_L2WATV_TOAReflectanceIndexes: "<<l_GIP_L2WATV_TOAReflectanceIndexes.size())

		// Read the files
		const ListOfFilenames& l_GIP_L2WATV_ListOfFilenames = l_lutxml.GetListOfPackaged_DBL_Files();

		if (l_GIP_L2WATV_ListOfFilenames.size() != 1)
		{
			vnsExceptionChainMacro("On the GIPP 'GIP_L2WATV', only one filename is require. Found "<<l_GIP_L2WATV_ListOfFilenames.size())
		}

		const std::string l_GIP_L2WATV_Filename = l_GIP_L2WATV_ListOfFilenames.at(0);
		vnsLogDebugMacro("  l_GIP_L2WATV_Filename: "<<l_GIP_L2WATV_Filename)

		// Initialize the LookUpTable reader
		//--------------------------------------------
		// Set the Band filename
		l_GIP_L2WATV_LookUpTableReader->AddBandFilename(l_GIP_L2WATV_Filename);

		// parameters are added one by one to the LUT
		GIP_L2WATV_LookUpTableType::ParameterValuesType l_watvParam1;
		l_watvParam1.ParameterName = "Solar_Zenith_Angle_Indexes";
		l_watvParam1.ParameterValues = l_GIP_L2WATV_SolarZenithalAngleIndexes;
		l_GIP_L2WATV_LookUpTableReader->AddParameterValues(l_watvParam1);

		GIP_L2WATV_LookUpTableType::ParameterValuesType l_watvParam2;
		l_watvParam2.ParameterName = "View_Zenith_Angle_Indexes";
		l_watvParam2.ParameterValues = l_GIP_L2WATV_ViewZenithalAngleIndexes;
		l_GIP_L2WATV_LookUpTableReader->AddParameterValues(l_watvParam2);

		GIP_L2WATV_LookUpTableType::ParameterValuesType l_watvParam3;
		l_watvParam3.ParameterName = "TOA_Reflectance_Indexes";
		l_watvParam3.ParameterValues = l_GIP_L2WATV_TOAReflectanceIndexes;
		l_GIP_L2WATV_LookUpTableReader->AddParameterValues(l_watvParam3);

		// Generate the LUT
		//--------------------------------------------
		l_GIP_L2WATV_LookUpTableReader->GenerateLUT();

		//Extract Lut for angles
		// --------------------------------------------------
		// Generate the WATV Reduce Look Up Table
		// Reduce the solar angle.
		GIP_L2WATV_LookUpTableExtractorType::ExtractPointType point;
		point[0] = GetParameterFloat("solarzenith");
		point[1] = GetParameterFloat("viewingzenith");

		LUTPointer l_GIP_L2WATV_ReducedLookUpTable = GIP_L2WATV_LookUpTableExtractorType::ExtractLUT(l_GIP_L2WATV_LookUpTableReader->GetLUT(), point);

		// Set the image of the TOA reflectance used to compute the water vapour
		m_WaterAmountGenerator->SetIPTOAInput(l_toa);
		m_WaterAmountGenerator->SetLUT(l_GIP_L2WATV_ReducedLookUpTable);
		m_WaterAmountGenerator->SetWaterVaporReferenceBandId(GetParameterInt("watervaporreferencebandid"));
		m_WaterAmountGenerator->SetWaterVaporBandId(GetParameterInt("watervaporbandid"));
		m_WaterAmountGenerator->SetVAPNodataValue(GetParameterFloat("vapnodatavalue"));
		m_WaterAmountGenerator->SetMaxThreshold(GetParameterFloat("watervapormaxthreshold"));

		//Set outputs
		SetParameterOutputImage<VAPImageType>("wateramount",m_WaterAmountGenerator->GetOutput());
	}

	/** Filters declaration */
	WaterAmountImageGeneratorPointer m_WaterAmountGenerator;
	/** LUT H2O */
	LUTType::Pointer m_LUT;

};

}
}

OTB_APPLICATION_EXPORT(vns::Wrapper::WaterAmountGeneratorFilter)
