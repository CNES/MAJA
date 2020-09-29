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
#include "vnsSimpleLutMapXMLFileHandler.h"
#include "vnsComputeScatteringCorrectionImageFilter.h"
#include "vnsAtomicRegistry.h"


namespace vns
{
namespace Wrapper
{

using namespace otb::Wrapper;

class ScatteringCorrection : public Application
{
public:
	/** Standard class typedefs. */
	typedef ScatteringCorrection Self;
	typedef otb::Wrapper::Application     Superclass;
	typedef itk::SmartPointer<Self>       Pointer;
	typedef itk::SmartPointer<const Self> ConstPointer;

	/** Standard macro */
	itkNewMacro(Self);

	itkTypeMacro(ScatteringCorrection, otb::Wrapper::Application);

	/** Image typedef */
	/** TOAC input image typedefs. */
	typedef VNSRealVectorImageType TOACImageType;
	typedef TOACImageType::ConstPointer TOACImageConstPointer;
	typedef TOACImageType::PixelType TOACImagePixelType;
	typedef TOACImageType::InternalPixelType TOACImageInternalPixelType;

	/** AOT input image typedefs. */
	typedef VNSRealImageType AOTImageType;
	typedef AOTImageType::ConstPointer AOTImageConstPointer;
	typedef AOTImageType::PixelType AOTImagePixelType;

	/** Altitude input image typedefs. */
	typedef VNSDTMImageType DTMImageType;
	typedef DTMImageType::ConstPointer DTMImageConstPointer;
	typedef DTMImageType::PixelType DTMImagePixelType;
	typedef DTMImageType::InternalPixelType DTMImageInternalPixelType;

	/** Edge input image typedefs. */
	typedef VNSMaskImageType InputMaskType;
	typedef InputMaskType::ConstPointer InputMaskConstPointer;
	typedef InputMaskType::PixelType InputMaskPixelType;
	typedef InputMaskType::InternalPixelType InputMaskInternalPixelType;

	/** Zone/Detector input mask typedefs. */
	typedef VNSMaskVectorImageType VectorMaskType;
	typedef VectorMaskType::ConstPointer VectorMaskConstPointer;
	typedef VectorMaskType::PixelType VectorMaskPixelType;
	typedef VectorMaskType::InternalPixelType VectorMaskInternalPixelType;

	/** LUT image typedef */
	typedef VNSLUT3DType LUTType;
	typedef LUTType::Pointer LUTPointer;
	typedef LUTType::ConstPointer LUTConstPointer;
	typedef LUTType::PixelType LUTPixelType;
	typedef LUTType::PointType LUTPointType;
	typedef LUTType::ParameterValuesType ParameterValuesType;
	typedef LUTPointType::ValueType LUTPointValueType;
	typedef std::map<unsigned int, LUTPointer> LUTMapType;
	typedef SimpleLutMapXMLFileHandler LutMapXMLHandlerType;
	typedef LutMapXMLHandlerType::Pointer LutMapXMLHandlerPointer;
	typedef itk::ImageFileReader<LUTType> ImageFileReaderType;
	typedef otb::ObjectList<ImageFileReaderType> ImageFileReaderListType;
	typedef ImageFileReaderListType::Pointer ImageFileReaderListPointer;
	/** Output image typedefs. */
	typedef VNSRealVectorImageType OutputImageType;
	typedef OutputImageType::Pointer OutputImagePointer;
	typedef OutputImageType::ConstPointer OutputImageConstPointer;
	typedef OutputImageType::PixelType OutputImagePixelType;
	typedef OutputImageType::InternalPixelType OutputImageInternalPixelType;
	typedef OutputImageType::RegionType OutputImageRegionType;

	/** ScatteringCorrectionImageFilter typedef */
	typedef ComputeScatteringCorrectionImageFilter<TOACImageType, AOTImageType, DTMImageType, InputMaskType, VectorMaskType,
			OutputImageType, LUTType> ComputeScatteringCorrectionImageFilterType;
	typedef ComputeScatteringCorrectionImageFilterType::Pointer ComputeScatteringCorrectionImageFilterPointer;


private:
	void DoInit()
	{
		SetName("ScatteringCorrection");
		SetDescription("Generate the ScatteringCorrection.");
		Loggers::GetInstance()->Initialize(GetName());
		// Documentation
		SetDocLongDescription("This application computes the ScatteringCorrection");
		SetDocLimitations("None");
		SetDocAuthors("MAJA-Team");
		SetDocSeeAlso("MAJA Doc");

		AddDocTag("Statistics");

		AddParameter(ParameterType_InputFilename,  "lutmap",   "LutMap");
		SetParameterDescription("lutmap", "Reduce LUT Map per zone");
		AddParameter(ParameterType_InputImage,  "toac",   "TOAC image");
		SetParameterDescription("toac", "TOA image");
		AddParameter(ParameterType_InputImage,  "aot",   "AOT image");
		SetParameterDescription("aot", "aot image");
		AddParameter(ParameterType_InputImage,  "edg",   "EDG image");
		SetParameterDescription("edg", "Image used as edge");
		AddParameter(ParameterType_InputImage,  "dtm",   "DTM image");
		SetParameterDescription("dtm", "Image used as DTM");
		AddParameter(ParameterType_InputImage,  "anglezone",   "AngleZone image");
		SetParameterDescription("anglezone", "anglezone image");
		MandatoryOff("anglezone");

		AddParameter(ParameterType_Float, "l2nodata","NoData value");
		AddParameter(ParameterType_StringList, "l2bandincoarse","l2bandincoarse");

		AddParameter(ParameterType_OutputImage, "tocr", "Corrected TOA");
		SetParameterOutputImagePixelType("tocr", ImagePixelType_double);
		SetParameterDescription("tocr","Corrected TOA");

		AddRAMParameter("ram");
		SetDefaultParameterInt("ram",2048);

	}

	void DoUpdateParameters()
	{

	}


	void DoExecute()
	{
		// Init filters
		m_ScatteringFilter =  ComputeScatteringCorrectionImageFilterType::New();
		m_Readers = ImageFileReaderListType::New();
		// Load optional inputs if the product L2 is generated at L2 resolutions
		TOACImageConstPointer l_TOAC = this->GetParameterDoubleVectorImage("toac");
		AOTImageConstPointer l_AOT = this->GetParameterDoubleImage("aot");
		InputMaskConstPointer l_EDG = this->GetParameterUInt8Image("edg");
		DTMImageConstPointer l_DTM = this->GetParameterDoubleImage("dtm");
		// optional input (if angle zones are taken into account)
		VectorMaskConstPointer l_AngleZone;
		LUTMapType l_LUTMap;


		//Load the lut header
		const std::string l_GIPPFilename = GetParameterString("lutmap");
		LutMapXMLHandlerPointer l_LUTMAPXMLHandler = LutMapXMLHandlerType::New();
		// Load the XML file and check with the schema
		l_LUTMAPXMLHandler->LoadFile(l_GIPPFilename);
		const SimpleLutMapXMLFileHandler::LutMapXml& l_lutmapxml = l_LUTMAPXMLHandler-> GetLutMapContainer();
		SimpleLutMapXMLFileHandler::LutMapXml::const_iterator it = l_lutmapxml.begin();
		for (;it != l_lutmapxml.end();++it)
		{

			const SimpleLutXMLContainer& l_lutxml = (*it).second;
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
			if( l_lutxml.GetListOfPackaged_DBL_Files().size() != 1)
			{
				vnsExceptionDataMacro("Only one file required for each lut");
			}

			ImageFileReaderType::Pointer l_Reader = ImageFileReaderType::New();
			l_Reader->SetFileName(l_lutxml.GetListOfPackaged_DBL_Files()[0]);
			// ---------------------------------------------------------------------------------------------
			// Load the LUT
			l_Reader->Update();
			//Lut
			LUTPointer l_LUT = l_Reader->GetOutput();

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
			l_LUTMap[it->first] = l_LUT;
			m_Readers->PushBack(l_Reader);
		}


		//Params
		const double l_RealL2NoData = GetParameterFloat("l2nodata");

		m_ScatteringFilter = ComputeScatteringCorrectionImageFilterType::New();
		m_ScatteringFilter->SetTOACInput(l_TOAC);
		m_ScatteringFilter->SetEDGInput(l_EDG);
		m_ScatteringFilter->SetDTMInput(l_DTM);
		m_ScatteringFilter->SetAOTInput(l_AOT);
		m_ScatteringFilter->SetLUTMap(l_LUTMap);

		// For wide field sensor like SENTINEL2, the variation of viewing angles within the image is taken into account
		if (HasValue("anglezone"))
		{
			l_AngleZone = this->GetParameterUInt8VectorImage("anglezone");
			m_ScatteringFilter->SetAngleZoneMaskInput(l_AngleZone);
		}

		// Get the list of band id at L2 coarse resolution that corresponds to band codes at L2 resolution (SENTINEL2 - R1 : 1, 2, 3, 7, ...)
		Utilities::ListOfUIntegers l_ListOfL2CoarseBandId =
				vns::Utilities::StringListAsUnsigned(GetParameterStringList("l2bandincoarse"));

		m_ScatteringFilter->SetListOfL2CoarseBandId(l_ListOfL2CoarseBandId);

		m_ScatteringFilter->SetNoData(l_RealL2NoData);
		// Get the output image of surface reflectances at L2 resolution
		m_ScatteringFilter->GetOutput()->UpdateOutputInformation();
		m_ScatteringFilter->UpdateOutputInformation();

		//Set the output
		SetParameterOutputImage<OutputImageType>("tocr",m_ScatteringFilter->GetOutput());



	}

	/** Filters declaration */
	ComputeScatteringCorrectionImageFilterPointer m_ScatteringFilter;
	ImageFileReaderListPointer m_Readers;

  AtomicRegistry<itk::MetaImageIOFactory> m_MetaRegistry;

};

}
}

OTB_APPLICATION_EXPORT(vns::Wrapper::ScatteringCorrection)
