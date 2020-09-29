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
#include "vnsSimpleSMACXMLFileHandler.h"
#include "vnsTOACorrectionImageFilter.h"
#include "vnsSmacCoefContainer.h"

namespace vns
{
namespace Wrapper
{

using namespace otb::Wrapper;

class TOACorrection : public Application
{
public:
	/** Standard class typedefs. */
	typedef TOACorrection Self;
	typedef otb::Wrapper::Application     Superclass;
	typedef itk::SmartPointer<Self>       Pointer;
	typedef itk::SmartPointer<const Self> ConstPointer;

	/** Standard macro */
	itkNewMacro(Self);

	itkTypeMacro(TOACorrection, otb::Wrapper::Application);

	/** Image typedef */
	/** TOA input image typedefs. */
	typedef DoubleVectorImageType TOAImageType;
	typedef TOAImageType::ConstPointer TOAImageConstPointer;

	/** Corrected TOA output image typedefs. */
	typedef DoubleVectorImageType TOACImageType;
	typedef TOACImageType::Pointer TOACImagePointer;
	typedef TOACImageType::ConstPointer TOACImageConstPointer;
	/** Vapor amount output image typedefs. */
	typedef DoubleImageType VAPImageType;
	typedef VAPImageType::Pointer VAPImagePointer;
	typedef VAPImageType::ConstPointer VAPImageConstPointer;

	/** TOA correction image filter typedefs */
	typedef TOACorrectionImageFilter<TOAImageType, VAPImageType, TOACImageType, VAPImageType> TOACorrectionImageFilterType;
	typedef TOACorrectionImageFilterType::Pointer TOACorrectionImageFilterPointer;
	typedef SmacCoefContainer SmacCoefContainerType;
	typedef TOACorrectionImageFilterType::SmacCoefContainerListType SmacCoefContainerListType;
	typedef std::vector<SmacCoefContainerListType> ListOfSmacCoefContainerListType;

	/** Utils typedef */
	typedef Utilities::ListOfDoubles ListOfDoubles;
	typedef Utilities::ListOfFilenames ListOfFilenames;

	/** Typedef on the SMAC reader */
	typedef SimpleSMACXMLFileHandler LutHandlerType;
	typedef LutHandlerType::Pointer LutHandlerPointer;

private:
	void DoInit()
	{
		SetName("TOACorrection");
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
		AddParameter(ParameterType_InputImage,  "pressure",   "Pressure image");
		SetParameterDescription("pressure", "Pressure of the image");
		AddParameter(ParameterType_InputImage,  "wateramount",   "wateramount image");
		SetParameterDescription("wateramount", "Water amount");
		MandatoryOff("wateramount");
		AddParameter(ParameterType_String,  "smac",   "SMAC Gipp");
		SetParameterDescription("smac", "SMAC gipp");
		AddParameter(ParameterType_Float, "wateramountdefault","wateramountdefault");
		AddParameter(ParameterType_Float, "ozoneamount","ozoneamount");
		AddParameter(ParameterType_Float, "thetas","thetas");
		AddParameter(ParameterType_StringList, "thetav","thetav");
		AddParameter(ParameterType_Float,"nodata","nodata");
		AddParameter(ParameterType_StringList, "bandlist", "Band index List");
		AddParameter(ParameterType_OutputImage, "toac", "Corrected TOA");
		SetParameterDescription("toac","corrected TOA");

		AddRAMParameter("ram");
		SetDefaultParameterInt("ram",2048);
	}

	void DoUpdateParameters()
	{

	}


	void DoExecute()
	{
		// Init filters
		m_TOACorrectionFilter = TOACorrectionImageFilterType::New();
		m_TOACorrectionFilter->SetReleaseDataFlag(true);
		//Get image pointers
		TOAImageConstPointer l_toa = this->GetParameterDoubleVectorImage("toa");
		VAPImagePointer l_wateramount;
		VAPImagePointer l_pressure = this->GetParameterDoubleImage("pressure");

		//GetParams
		const std::vector<std::string> l_BandIdxStr = GetParameterStringList("bandlist");
		Utilities::ListOfUIntegers l_BandIdx;
		for (unsigned int i = 0; i < l_BandIdxStr.size();i++)
		{
			l_BandIdx.push_back(Utilities::AsUnsignedInt(l_BandIdxStr[i]));
		}
		const std::vector<std::string> l_ThetaVStr = GetParameterStringList("thetav");
		Utilities::ListOfDoubles l_ThetaV;
		for (unsigned int i = 0; i < l_ThetaVStr.size();i++)
		{
			l_ThetaV.push_back(Utilities::AsDouble(l_ThetaVStr[i]));
		}

		//*************************************************************************************************************
		// Read the GIPP file "GIP_L2SMAC"
		//*************************************************************************************************************
		vnsLogInfoMacro("Starting SMAC reading ...");
		std::string l_GIPPFilename = GetParameterString("smac");

		// Load the xml file
		LutHandlerPointer l_LUTXMLHandler = LutHandlerType::New();
		l_LUTXMLHandler->LoadFile(l_GIPPFilename);
		SmacCoefContainerListType l_GIP_L2SMAC_ListOfCoefficientsPerBands;

		// Read the number of bands for coefficients SMAC from the GIPP file
		const unsigned int l_GIP_L2SMAC_NumberOfCoefficients = l_LUTXMLHandler->GetSMAC_NumberOfBands();

		// Read the coefficients SMAC from the GIPP file
		const ListOfDoubles l_GIP_L2SMAC_ListOfCoefficients = l_LUTXMLHandler->GetSMAC_Coefficients();
		// Check the number of Coefficients: 49 per Bands
		const unsigned int l_NumbersOfSmacCoefficientsContainer = SmacCoefContainer::m_NumbersOfSmacCoefficients;
		const unsigned int l_SmacNumberOfCoefficients = l_GIP_L2SMAC_NumberOfCoefficients * l_NumbersOfSmacCoefficientsContainer;
		if (l_GIP_L2SMAC_ListOfCoefficients.size() != l_SmacNumberOfCoefficients)
		{
			vnsExceptionChainMacro(
					"The GIP_L2SMAC GIPP file doesn't contain "<<l_GIP_L2SMAC_NumberOfCoefficients<<"x"<<l_NumbersOfSmacCoefficientsContainer<<"(="<<l_SmacNumberOfCoefficients<<") values but it contains "<<l_GIP_L2SMAC_ListOfCoefficients.size()<<" coefficients.");
		}

		// Allocate the smac coefficients values per band
		//ListOfDoubles l_InitList;
		SmacCoefContainerType l_InitList; // FixeArrray of size m_NumbersOfSmacCoefficients
		for (unsigned int cpt = 0; cpt < l_GIP_L2SMAC_NumberOfCoefficients; cpt++)
		{
			l_GIP_L2SMAC_ListOfCoefficientsPerBands.push_back(l_InitList);
		}
		unsigned int l_GlobalCpt(0);
		for (unsigned int SmacCoefficientsPerBand = 0; SmacCoefficientsPerBand < l_NumbersOfSmacCoefficientsContainer;
				SmacCoefficientsPerBand++)
		{
			for (unsigned int NumberOfBand = 0; NumberOfBand < l_GIP_L2SMAC_NumberOfCoefficients; NumberOfBand++)
			{
				vnsLogSuperDebugMacro(
						"l_GIP_L2SMAC_ListOfCoefficients size()="<<l_GIP_L2SMAC_ListOfCoefficients.size()<<" index l_GlobalCpt: "<<l_GlobalCpt<<" value = l_GIP_L2SMAC_ListOfCoefficients.at(l_GlobalCpt)"<<l_GIP_L2SMAC_ListOfCoefficients.at(l_GlobalCpt))
		                            		l_GIP_L2SMAC_ListOfCoefficientsPerBands[NumberOfBand][SmacCoefficientsPerBand] =
		                            				l_GIP_L2SMAC_ListOfCoefficients[l_GlobalCpt];
				// Increment the compter
				l_GlobalCpt++;
			}
		}
		// Check the copy of the values
		for (unsigned int NumberOfBand = 0; NumberOfBand < l_GIP_L2SMAC_NumberOfCoefficients; NumberOfBand++)
		{
			if (l_GIP_L2SMAC_ListOfCoefficientsPerBands[NumberOfBand].Size() != SmacCoefContainer::m_NumbersOfSmacCoefficients)
			{
				vnsExceptionChainMacro(
						"Internal error when copy the SMAC coefficient from the GIPP_L2SMAC to list of vector per band !")
			}
		}

		//Get the sub part of subs
		SmacCoefContainerListType l_ListOfCoefficients;
		for (unsigned int l_BandId = 0; l_BandId < l_BandIdx.size(); l_BandId++)
		{
			l_ListOfCoefficients.push_back(l_GIP_L2SMAC_ListOfCoefficientsPerBands[l_BandIdx[l_BandId]]);
			vnsLogDebugMacro("l_GIP_L2SMAC_ListOfCoefficientsPerBands band : " << l_BandIdx[l_BandId] )
			vnsLogDebugMacro(l_GIP_L2SMAC_ListOfCoefficientsPerBands[l_BandIdx[l_BandId]])
		}



		// Compute the TOA reflectances after correction of the absorbing atmospheric molecules
		if (HasValue("wateramount"))
		{
			l_wateramount = this->GetParameterDoubleImage("wateramount");
			m_TOACorrectionFilter->SetWaterInput(l_wateramount);
		}
		// The spectral bands are not available in the input product or the user chooses to use a constant value
		else
		{
			m_TOACorrectionFilter->SetWaterAmountDefaultValue(this->GetParameterFloat("wateramountdefault"));
		}

		//Set filter parameters
		m_TOACorrectionFilter->SetTOAInput(l_toa);
		m_TOACorrectionFilter->SetPressureInput(l_pressure);
		m_TOACorrectionFilter->SetSmacCoefContainerList(l_ListOfCoefficients);
		m_TOACorrectionFilter->SetOzoneAmount(this->GetParameterFloat("ozoneamount"));
		m_TOACorrectionFilter->SetThetaS(this->GetParameterFloat("thetas"));
		m_TOACorrectionFilter->SetThetaV(l_ThetaV);
		m_TOACorrectionFilter->SetL2NoData(this->GetParameterFloat("nodata"));


		//Set outputs
		SetParameterOutputImagePixelType("toac", ImagePixelType_double);
		SetParameterOutputImage<TOACImageType>("toac",m_TOACorrectionFilter->GetOutput());
	}

	/** Filters declaration */
	/** TOA correction image filter */
	TOACorrectionImageFilterPointer m_TOACorrectionFilter;

};

}
}

OTB_APPLICATION_EXPORT(vns::Wrapper::TOACorrection)
