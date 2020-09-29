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
 * VERSION : 1-0-0 : <TypeFT> : <NumFT> : 15 juil. 2010 : Creation
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id$
 *                                                                                                          *
 ************************************************************************************************************/

#include "otbWrapperApplication.h"
#include "otbWrapperApplicationFactory.h"
#include "vnsStreamingConditionalStatisticsImageFilter.h"
#include <string>

namespace vns
{
namespace Wrapper
{

using namespace otb::Wrapper;

class Stats : public Application
{
public:
	/** Standard class typedefs. */
	typedef Stats             Self;
	typedef otb::Wrapper::Application     Superclass;
	typedef itk::SmartPointer<Self>       Pointer;
	typedef itk::SmartPointer<const Self> ConstPointer;

	/** Standard macro */
	itkNewMacro(Self);

	itkTypeMacro(Stats, otb::Wrapper::Application);

	/** Some convenient typedefs. */
	typedef DoubleImageType ImageType;
	typedef ImageType::ConstPointer ImageConstPointer;
	typedef ImageType::Pointer ImagePointer;
	typedef ImageType::PixelType PixelType;

	typedef UInt8ImageType MaskType;
	typedef MaskType::Pointer MaskPointer;
	typedef MaskType::ConstPointer MaskConstPointer;

	typedef StreamingConditionalStatisticsImageFilter<ImageType,MaskType> StreamingConditionalStatisticsImageFilterType;
	typedef StreamingConditionalStatisticsImageFilterType::Pointer StreamingConditionalStatisticsImageFilterPointerType;

private:
	void DoInit()
	{
		SetName("Stats");
		SetDescription("Stats algo.");
		Loggers::GetInstance()->Initialize(GetName());
		// Documentation
		SetDocLongDescription("This application computes the Solar angle");
		SetDocLimitations("None");
		SetDocAuthors("MAJA-Team");
		SetDocSeeAlso("MAJA Doc");

		AddDocTag("Statistics");
		AddParameter(ParameterType_InputImage, "im", "image");

		AddParameter(ParameterType_Float,  "exclude",   "Excluded value");
		MandatoryOff("exclude");

		AddParameter(ParameterType_InputImage,  "mask",   "masking image");
		MandatoryOff("mask");
		AddParameter(ParameterType_Float,  "maskforeground",   "Mask Foreround value");
		MandatoryOff("maskforeground");


		AddParameter(ParameterType_Float, "mean",  "MEAN");
		SetParameterDescription("mean", "Mean");
		SetParameterRole("mean", Role_Output);

		AddParameter(ParameterType_Float, "sum",  "sum");
		SetParameterDescription("sum", "sum");
		SetParameterRole("sum", Role_Output);

		AddParameter(ParameterType_Int, "validcount",  "validcount");
		SetParameterDescription("validcount", "Valid pixels");
		SetParameterRole("validcount", Role_Output);

		AddParameter(ParameterType_Int, "excludedcount",  "excludedcount");
		SetParameterDescription("excludedcount", "excluded pixels");
		SetParameterRole("excludedcount", Role_Output);

		AddParameter(ParameterType_Float, "min",  "minimum");
		SetParameterDescription("min", "minimum");
		SetParameterRole("min", Role_Output);

		AddParameter(ParameterType_Float, "max",  "maximum");
		SetParameterDescription("max", "maximum");
		SetParameterRole("max", Role_Output);

		AddParameter(ParameterType_Float, "stdv",  "standard deviation");
		SetParameterDescription("stdv", "standard deviation");
		SetParameterRole("stdv", Role_Output);

		AddRAMParameter("ram");
		SetDefaultParameterInt("ram",2048);

	}

	void DoUpdateParameters()
	{

	}


	void DoExecute()
	{
		// Init filters
		m_StatImage = StreamingConditionalStatisticsImageFilterType::New();
		// Get input image pointers
		ImagePointer l_inPtr = GetParameterDoubleImage("im");
		m_StatImage->SetInput(l_inPtr);
		MaskPointer l_mask;
		if(HasValue("mask"))
		{
			l_mask = GetParameterUInt8Image("mask");
			if (HasValue("maskforeground"))
			{
				const int l_for = GetParameterInt("maskforeground");
				m_StatImage->SetMaskForegroundValue(l_for);
			}
		}

		double l_excluded = -999;
		if(HasValue("exclude"))
		{
			l_excluded = GetParameterFloat("exclude");
		}
		if(HasValue("mask"))
		{
			m_StatImage->SetMaskInput(l_mask);
		}
		if(HasValue("exclude"))
		{
			m_StatImage->SetExcludedValue(l_excluded);
			m_StatImage->SetEnableExcludeValue(true);
		} else {
			m_StatImage->SetEnableExcludeValue(false);
		}
		m_StatImage->SetEnableVariance(true);
		m_StatImage->SetEnableMinMax(true);
		m_StatImage->SetEnableFirstOrderStats(true);
		m_StatImage->GetStreamer()->SetAutomaticAdaptativeStreaming(GetParameterInt("ram"));
		m_StatImage->Update();

		SetParameterFloat("mean",  m_StatImage->GetMean());

		SetParameterFloat("sum",  m_StatImage->GetSum());

		SetParameterInt("validcount",  m_StatImage->GetNbOfValidValues());

		SetParameterInt("excludedcount",  m_StatImage->GetNbOfExcludedValues());

		SetParameterFloat("min", m_StatImage->GetMinimum() );

		SetParameterFloat("max",  m_StatImage->GetMaximum());

		SetParameterFloat("stdv",  m_StatImage->GetStandardDeviation());

	}


	/** Filters declaration */
	StreamingConditionalStatisticsImageFilterPointerType m_StatImage;

};

}
}

OTB_APPLICATION_EXPORT(vns::Wrapper::Stats)
