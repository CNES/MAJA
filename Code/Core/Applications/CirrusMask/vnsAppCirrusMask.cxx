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
#include "itkBinaryFunctorImageFilter.h"
#include "vnsThresholdImageFunctor.h"
#include "otbMultiToMonoChannelExtractROI.h"
#include "vnsReflectanceThresholdImageFunctor.h"
#include "vnsCountCirrusPixelGenerator.h"

#include <string>

namespace vns
{
namespace Wrapper
{

using namespace otb::Wrapper;

class CirrusMask : public Application
{
public:
	/** Standard class typedefs. */
	typedef CirrusMask              Self;
	typedef otb::Wrapper::Application     Superclass;
	typedef itk::SmartPointer<Self>       Pointer;
	typedef itk::SmartPointer<const Self> ConstPointer;

	/** Standard macro */
	itkNewMacro(Self);

	itkTypeMacro(CirrusMask, otb::Wrapper::Application);

	/** Some convenient typedefs. */
	typedef DoubleVectorImageType InputImageType;
	typedef InputImageType::PixelType InputPixelType;
	typedef InputImageType::InternalPixelType InputInternalPixelType;
	typedef InputImageType::Pointer InputImagePointer;
	typedef InputImageType::ConstPointer InputImageConstPointer;

	typedef DoubleImageType InputDTMImageType;
	typedef InputDTMImageType::PixelType InputDTMPixelType;
	typedef InputDTMImageType::InternalPixelType InputDTMInternalPixelType;
	typedef InputDTMImageType::Pointer InputDTMImagePointer;
	typedef InputDTMImageType::ConstPointer InputDTMImageConstPointer;

	typedef UInt8ImageType InputMaskType;
	typedef InputMaskType::Pointer InputMaskPointer;
	typedef InputMaskType::ConstPointer InputMaskConstPointer;

	typedef UInt8ImageType OutputMaskType;
	typedef OutputMaskType::PixelType MaskPixelType;
	typedef OutputMaskType::Pointer OutputMaskPointer;

	/** Filters typedefs. */
	typedef otb::MultiToMonoChannelExtractROI<InputInternalPixelType, InputInternalPixelType> MultiToMonoChannelExtractFilterType;
	typedef MultiToMonoChannelExtractFilterType::Pointer MultiToMonoChannelExtractFilterPointer;
	typedef MultiToMonoChannelExtractFilterType::OutputImageType MonoChannelImageType;

	typedef Functor::ReflectanceThresholdImageFunctor<InputInternalPixelType, InputDTMInternalPixelType, MaskPixelType> ThresholdFunctorType;
	typedef itk::BinaryFunctorImageFilter<MonoChannelImageType, InputDTMImageType, OutputMaskType, ThresholdFunctorType> ThresholdFilterType;
	typedef ThresholdFilterType::Pointer ThresholdFilterPointer;

	typedef CountCirrusPixelGenerator<OutputMaskType, InputMaskType> CountCirrusFilterType;
	typedef CountCirrusFilterType::Pointer CountCirrusFilterPointer;
	/** Filters typedefs */


private:
	void DoInit()
	{
		SetName("CirrusMask");
		SetDescription("CirrusMask algo.");
		Loggers::GetInstance()->Initialize(GetName());
		// Documentation
		SetDocLongDescription("This application computes the cirrus mask");
		SetDocLimitations("None");
		SetDocAuthors("MAJA-Team");
		SetDocSeeAlso("MAJA Doc");

		AddDocTag("Statistics");

		AddParameter(ParameterType_InputImage,  "toa",   "toa image");
		SetParameterDescription("toa", "TOA image at coarse all bands");
		AddParameter(ParameterType_InputImage,  "edg",   "EDG image");
		SetParameterDescription("edg", "Image used as edge at coarse");
		AddParameter(ParameterType_InputImage,  "dtm",   "DTM image");
		SetParameterDescription("dtm", "Image used as DTM at coarse");

		AddParameter(ParameterType_Int,  "bandidx",   "Cirrus Band Index");
		SetParameterDescription("bandidx", "Cirrus band index to compute cirrus");

		AddParameter(ParameterType_Float,  "threshoff",  "Threshold offset");
		SetParameterDescription("threshoff","Threshold Offset for computation");
		AddParameter(ParameterType_Float,  "threshgain",  "Threshold gain");
		SetParameterDescription("threshgain","Threshold Gain for computation");

		AddParameter(ParameterType_Float,  "altref",  "Altitude reference");
		SetParameterDescription("altref","Cirrus altitude reference");

		AddParameter(ParameterType_Float,  "minpercent",  "Min cirrus percent");
		SetParameterDescription("minpercent","Minimum percentage to consider cirrus flag");

		AddParameter(ParameterType_String, "cirrusflag",  "CirrusFlag");
		SetParameterDescription("cirrusflag", "Is the image cirrus enought");
		SetParameterRole("cirrusflag", Role_Output);

		AddParameter(ParameterType_OutputImage, "mask", "cirrus mask");
		SetParameterDescription("mask","cirrus mask");
		SetParameterOutputImagePixelType("mask", ImagePixelType_uint8);

		AddRAMParameter("ram");
		SetDefaultParameterInt("ram",2048);
	}

	void DoUpdateParameters()
	{

	}


	void DoExecute()
	{
		// Init filters
		m_CountCirrusFilter = CountCirrusFilterType::New();
		m_ThresholdFilter = ThresholdFilterType::New();
		m_MultiToMonoChannelExtractFilter = MultiToMonoChannelExtractFilterType::New();

		// Get input image pointers
		InputImageConstPointer l_toa = this->GetParameterDoubleVectorImage("toa");
		InputMaskConstPointer l_edg = this->GetParameterUInt8Image("edg");
		InputDTMImageConstPointer l_dtm = this->GetParameterDoubleImage("dtm");

		m_MultiToMonoChannelExtractFilter->SetInput(l_toa);
		// In the MultiToMonoChannelExtract filter, the first band index is 1 and not 0
		m_MultiToMonoChannelExtractFilter->SetChannel(this->GetParameterInt("bandidx")+1);

		// Threshold the cirrus band (1.38µm)
		m_ThresholdFilter->SetInput1(m_MultiToMonoChannelExtractFilter->GetOutput());
		m_ThresholdFilter->SetInput2(l_dtm);
		m_ThresholdFilter->GetFunctor().SetThresholdGain(this->GetParameterFloat("threshgain"));
		m_ThresholdFilter->GetFunctor().SetThresholdOffset(this->GetParameterFloat("threshoff"));
		m_ThresholdFilter->GetFunctor().SetAltRef(this->GetParameterFloat("altref"));
		m_ThresholdFilter->GetFunctor().SetInsideValue(0);
		m_ThresholdFilter->GetFunctor().SetOutputValue(1);

		SetParameterOutputImage<OutputMaskType>("mask",m_ThresholdFilter->GetOutput());
		// Count all cirrus clouds
		// Call the vnsCountCirrusPixelGenerator
		m_CountCirrusFilter->SetInput(m_ThresholdFilter->GetOutput());
		m_CountCirrusFilter->SetAltitudeThreshold(static_cast<CountCirrusFilterType::InternalPixelType>(0.5));
		m_CountCirrusFilter->SetEDGSub(l_edg);
		m_CountCirrusFilter->Update();

		// Get the results of the vnsCountCirrusPixelGenerator
		const unsigned long l_ValidPixelNumber = m_CountCirrusFilter->GetValidPixelNumber();

		// Display the number of cirrus pixels in debug mode
		vnsLogDebugMacro("l_ValidPixelNumber: "<<l_ValidPixelNumber);

		if (l_ValidPixelNumber == 0)
		{
			vnsLogWarningMacro("CirrusMask: The number of valid pixels is null");
		}
		else
		{
			const unsigned long l_CirrusPixelNumber = m_CountCirrusFilter->GetCirrusPixelNumber();
			vnsLogDebugMacro("l_CirrusPixelNumber: "<<l_CirrusPixelNumber);
			// Proportion of cirrus within the image
			double l_CirrusProportion = l_CirrusPixelNumber * 100. / l_ValidPixelNumber;

			// Parameters used to raise the cirrus flag and displayed in debug mode
			vnsLogDebugMacro("l_CirrusProportion: " << l_CirrusProportion << " <> " << this->GetParameterFloat("minpercent"))
			// If the proportion of cirrus within the image is too high, the cirrus flag is raised
			// Initialization
			std::string l_CirrusFlag = "false";
			if (l_CirrusProportion > this->GetParameterFloat("minpercent"))
			{
				l_CirrusFlag = "true";
			}

			// Set the output flag
			this->SetParameterString( "cirrusflag", l_CirrusFlag , false);
		}

	}


	/** Filters declaration */
	MultiToMonoChannelExtractFilterPointer m_MultiToMonoChannelExtractFilter;
	CountCirrusFilterPointer m_CountCirrusFilter;
	ThresholdFilterPointer m_ThresholdFilter;


};

}
}

OTB_APPLICATION_EXPORT(vns::Wrapper::CirrusMask)
