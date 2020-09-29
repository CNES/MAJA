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
#include "vnsCountCirrusPixelGenerator.h"
#include "vnsThresholdImageFunctor.h"
#include "vnsStreamingConditionalStatisticsImageFilter.h"

#include <string>

namespace vns
{
namespace Wrapper
{

using namespace otb::Wrapper;

class CirrusFlag : public Application
{
public:
	/** Standard class typedefs. */
	typedef CirrusFlag              Self;
	typedef otb::Wrapper::Application     Superclass;
	typedef itk::SmartPointer<Self>       Pointer;
	typedef itk::SmartPointer<const Self> ConstPointer;

	/** Standard macro */
	itkNewMacro(Self);

	itkTypeMacro(CirrusFlag, otb::Wrapper::Application);

	/** Some convenient typedefs. */
	typedef DoubleImageType InputImageType;
	typedef InputImageType::PixelType InputPixelType;
	typedef InputImageType::InternalPixelType InputInternalPixelType;
	typedef InputImageType::Pointer InputImagePointer;
	typedef InputImageType::ConstPointer InputImageConstPointer;

	typedef UInt8ImageType InputMaskType;
	typedef InputMaskType::Pointer InputMaskPointer;
	typedef InputMaskType::ConstPointer InputMaskConstPointer;

	/** Filters typedefs. */
	typedef CountCirrusPixelGenerator<InputImageType, InputMaskType> CountCirrusFilterType;
	typedef CountCirrusFilterType::Pointer CountCirrusFilterPointerType;

	typedef Functor::ThresholdImageFunctor<InputPixelType, InputPixelType> ThresholdFunctorType;
	typedef itk::UnaryFunctorImageFilter<InputImageType, InputImageType, ThresholdFunctorType> ThresholdFilterType;
	typedef ThresholdFilterType::Pointer ThresholdFilterPointerType;

	typedef StreamingConditionalStatisticsImageFilter<InputImageType, InputImageType> StatisticsFilterType;
	typedef StatisticsFilterType::Pointer StatisticsFilterPointerType;

	/** Filters typedefs */


private:
	void DoInit()
	{
		SetName("CirrusFlag");
		SetDescription("CirrusFlag algo.");
		Loggers::GetInstance()->Initialize(GetName());
		// Documentation
		SetDocLongDescription("This application computes the cirrus mask");
		SetDocLimitations("None");
		SetDocAuthors("MAJA-Team");
		SetDocSeeAlso("MAJA Doc");

		AddDocTag("Statistics");

		//Input images
		AddParameter(ParameterType_InputImage,  "cla",   "cla image");
		SetParameterDescription("cla", "Cloud level altitude image");
		AddParameter(ParameterType_InputImage,  "edg",   "EDG image");
		SetParameterDescription("edg", "Image used as edge at coarse");
		//algo params
		AddParameter(ParameterType_Int,  "altthreshold",  "Altitude threshold");
		SetParameterDescription("altthreshold","Threshold for altitude");
		SetParameterRole("altthreshold", Role_Output);
		AddParameter(ParameterType_Float,  "minpercentcirrus",  "Minimum percent of cirrus");
		SetParameterDescription("minpercentcirrus","Minimum percent of cirrus");
		SetParameterRole("minpercentcirrus", Role_Output);
		AddParameter(ParameterType_Float,  "minpercentcloud",  "Minimum percent of cloud");
		SetParameterDescription("minpercentcloud","Minimum percent of cloud");
		AddParameter(ParameterType_Float,  "minstdcirrus",  "Minimum of cirrus standard deviation");
		SetParameterDescription("minstdcirrus","Minimum  of cirrus standard deviation");
		SetParameterRole("minstdcirrus", Role_Output);
		AddParameter(ParameterType_Float,  "minpercentstdcirrus",  "Minimum percent of cirrus standard deviation");
		SetParameterDescription("minpercentstdcirrus","Minimum percent of cirrus standard deviation");
		SetParameterRole("minpercentstdcirrus", Role_Output);
		AddParameter(ParameterType_Float,  "nodata","NoData for L1");




		//Output flag
		AddParameter(ParameterType_String, "cirrusflag",  "CirrusFlag");
		SetParameterDescription("cirrusflag", "Is the image cirrus enought");
		SetParameterRole("cirrusflag", Role_Output);
		AddParameter(ParameterType_Float, "mean",  "Mean");
		SetParameterRole("mean", Role_Output);
		AddParameter(ParameterType_Float, "cirrusproportion",  "Cirrusproportion");
		SetParameterRole("cirrusproportion", Role_Output);
		AddParameter(ParameterType_Float, "cirrusstd",  "Cirrusstd");
		SetParameterRole("cirrusstd", Role_Output);
		AddParameter(ParameterType_Float, "cloudrelativestd",  "Cloudrelativestd");
		SetParameterRole("cloudrelativestd", Role_Output);
		AddParameter(ParameterType_Float, "cloudpixelnumber",  "Cloudpixelnumber");
		SetParameterRole("cloudpixelnumber", Role_Output);
		AddParameter(ParameterType_Float, "cirruspixelnumber",  "Cirruspixelnumber");
		SetParameterRole("cirruspixelnumber", Role_Output);
		AddParameter(ParameterType_Float, "validpixelnumber",  "Validpixelnumber");
		SetParameterRole("validpixelnumber", Role_Output);
		AddParameter(ParameterType_Float, "minimum",  "Minimum");
		SetParameterRole("minimum", Role_Output);
		AddParameter(ParameterType_Float, "maximum",  "Maximum");
		SetParameterRole("maximum", Role_Output);
		AddParameter(ParameterType_Float, "sum",  "Sum");
		SetParameterRole("sum", Role_Output);
		AddParameter(ParameterType_Float, "variance",  "Variance");
		SetParameterRole("variance", Role_Output);
		AddParameter(ParameterType_Float, "standardeviation",  "Standardeviation");
		SetParameterRole("standardeviation", Role_Output);


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
		m_StatisticsFilter = StatisticsFilterType::New();


		// Get input image pointers
		InputImagePointer l_L1CLA = this->GetParameterDoubleImage("cla");
		InputMaskConstPointer l_EDGSub = this->GetParameterUInt8Image("edg");


		vnsLogDebugMacro("CirrusFlagGenerator::GenerateData() - Start")

		std::string l_CirrusFlag = "false";
		// Count all cirrus clouds
		// Call the vnsCountCirrusPixelGenerator
		m_CountCirrusFilter->SetInput(l_L1CLA);
		m_CountCirrusFilter->SetAltitudeThreshold(this->GetParameterInt("altthreshold"));
		m_CountCirrusFilter->SetL1NoData(this->GetParameterFloat("nodata"));
		m_CountCirrusFilter->SetEDGSub(l_EDGSub);
		//TODO Add a virtual writer
		m_CountCirrusFilter->Update();


		// Get the results of the vnsCountCirrusPixelGenerator
		const unsigned long l_CirrusPixelNumber = m_CountCirrusFilter->GetCirrusPixelNumber();
		const unsigned long l_CloudPixelNumber = m_CountCirrusFilter->GetCloudPixelNumber();
		const unsigned long l_ValidPixelNumber = m_CountCirrusFilter->GetValidPixelNumber();

		// Display the number of cirrus pixels in debug mode
		vnsLogDebugMacro("l_CirrusPixelNumber: "<<l_CirrusPixelNumber);
		vnsLogDebugMacro("l_CloudPixelNumber: "<<l_CloudPixelNumber);
		vnsLogDebugMacro("l_ValidPixelNumber: "<<l_ValidPixelNumber);

		if (l_ValidPixelNumber == 0)
		{
			vnsLogWarningMacro("CirrusFlagGenerator::GenerateData(). The number of valid pixels is null");
			// Cirrus flag is set to false
			this->SetParameterString( "cirrusflag", l_CirrusFlag , false);
			this->SetParameterFloat( "cirrusstd", 0.0);
			this->SetParameterFloat( "cloudrelativestd", 0.0);
		}
		else
		{
			// Threshold the cloud altitude (> 0) and generate a mask used to compute statistics
			// Statistics are computed if the pixel of the mask is equal to 0
			m_ThresholdFilter->SetInput(l_L1CLA);
			m_ThresholdFilter->GetFunctor().SetThresholdValue(0);
			m_ThresholdFilter->GetFunctor().SetInsideValue(1);
			m_ThresholdFilter->GetFunctor().SetOutputValue(0);

			// Cast the input image to a vector image
			// Compute image statistics with an input mask
			m_StatisticsFilter->SetInput(l_L1CLA);
			m_StatisticsFilter->SetEnableVariance(true);
			m_StatisticsFilter->SetMaskInput(m_ThresholdFilter->GetOutput());
			//TODO add a virtual writer
			m_StatisticsFilter->Update();


			// Statistics used in debug mode
			vnsLogDebugMacro("CirrusFlagGenerator::GenerateData() Minimum: " <<m_StatisticsFilter->GetMinimumOutput()->Get());
			vnsLogDebugMacro("CirrusFlagGenerator::GenerateData() Maximum: "<< m_StatisticsFilter->GetMaximumOutput()->Get());
			vnsLogDebugMacro("CirrusFlagGenerator::GenerateData() Sum: " << m_StatisticsFilter->GetSum());
			vnsLogDebugMacro("CirrusFlagGenerator::GenerateData() Mean: " << m_StatisticsFilter->GetMean());
			vnsLogDebugMacro("CirrusFlagGenerator::GenerateData() Variance: " << m_StatisticsFilter->GetVariance());
			vnsLogDebugMacro("CirrusFlagGenerator::GenerateData() StandarDeviation: " << m_StatisticsFilter->GetStandardDeviation());

			// Proportion of cirrus within the image
			double l_CirrusProportion = l_CirrusPixelNumber * 100. / l_ValidPixelNumber;
			double l_CloudProportion = l_CloudPixelNumber * 100. / l_ValidPixelNumber;

			// Relative standard deviation for cirrus
			// Get the first element of the matrix while the input image of the statistics filter
			// is an otb::image and not an otb::VectorImage
			const double l_CloudMean = m_StatisticsFilter->GetMean(); //[0];
			double l_CloudStd = m_StatisticsFilter->GetStandardDeviation();
			vnsLogDebugMacro("CirrusFlagGenerator l_CloudMean : "<<l_CloudMean);
			vnsLogDebugMacro("CirrusFlagGenerator m_CloudStd : "<<l_CloudStd);
			vnsLogDebugMacro("CirrusFlagGenerator m_StatisticsFilter->GetIsValid() : "<<m_StatisticsFilter->GetIsValid());

			// No pixel is valid. Returns null Statistics
			if (m_StatisticsFilter->GetIsValid() == false)
			{
				// Cirrus flag is set to false
				this->SetParameterString( "cirrusflag", l_CirrusFlag , false);
				this->SetParameterFloat( "cirrusstd", 0.0);
				this->SetParameterFloat( "cloudrelativestd", 0.0);
			}
			else
			{
				// Check if the mean value is not null before computing the "CloudRstd" value
				if(vnsIsZeroDoubleWithEpsilonMacro(l_CloudMean, CONST_EPSILON_20))
				{
					vnsExceptionBusinessMacro("CirrusFlagGenerator failed. The cloud statistics of L1 CLA image is null")
				}
				const double l_CloudRstd = l_CloudStd / l_CloudMean;
				const double l_MinRstdCirrus = this->GetParameterFloat("minstdcirrus");
				const double l_MinPercentCloud = this->GetParameterFloat("minpercentcloud");
				const double l_MinPercentCirrus = this->GetParameterFloat("minpercentcirrus");
				const double l_MinPercentStdCirrus = this->GetParameterFloat("minpercentstdcirrus");

				// Parameters used to raise the cirrus flag and displayed in debug mode
				vnsLogDebugMacro("m_CloudProportion: " << l_CloudProportion << " <> " << l_MinPercentCloud);
				vnsLogDebugMacro("m_CirrusProportion: " << l_CirrusProportion << " <> " << l_MinPercentCirrus);
				vnsLogDebugMacro("m_CloudRstd: " << l_CloudRstd << " <> " << l_MinRstdCirrus);
				vnsLogDebugMacro("m_CloudStd: " << l_CloudStd << " <> " << l_MinPercentStdCirrus);

				// If the proportion of cirrus within the image is too high
				// or if the standart deviation is high, the cirrus flag is raised
				if (l_CloudProportion > l_MinPercentCloud)
				{
					if ((l_CirrusProportion > l_MinPercentCirrus) || (l_CloudRstd > l_MinRstdCirrus)
							|| (l_CloudStd > l_MinPercentStdCirrus))
					{
						l_CirrusFlag = "true";
					}
				}
			    this->SetParameterFloat( "cirrusstd", l_CloudStd);
			    this->SetParameterFloat( "cloudrelativestd", l_CloudRstd);
			 }
		     this->SetParameterFloat( "cirrusproportion", l_CirrusProportion);

		}


		// Set the output flag
		this->SetParameterString( "cirrusflag", l_CirrusFlag , false);
		this->SetParameterInt( "altthreshold",this->GetParameterInt("altthreshold"));
		this->SetParameterFloat( "minpercentcirrus",this->GetParameterInt("minpercentcirrus"));
		this->SetParameterFloat( "minstdcirrus",this->GetParameterInt("minstdcirrus"));
		this->SetParameterFloat( "minpercentstdcirrus",this->GetParameterInt("minpercentstdcirrus"));
		this->SetParameterFloat( "mean", m_StatisticsFilter->GetMean());
		this->SetParameterFloat( "cloudpixelnumber", l_CloudPixelNumber);
		this->SetParameterFloat( "cirruspixelnumber", l_CirrusPixelNumber);
		this->SetParameterFloat( "validpixelnumber", l_ValidPixelNumber);
		this->SetParameterFloat( "minimum", m_StatisticsFilter->GetMinimumOutput()->Get());
		this->SetParameterFloat( "maximum",  m_StatisticsFilter->GetMaximumOutput()->Get());
		this->SetParameterFloat( "sum", m_StatisticsFilter->GetSum());
		this->SetParameterFloat( "variance", m_StatisticsFilter->GetVariance());
		this->SetParameterFloat( "standardeviation",  m_StatisticsFilter->GetStandardDeviation());
	}


	/** Filters declaration */
	CountCirrusFilterPointerType m_CountCirrusFilter;
	ThresholdFilterPointerType m_ThresholdFilter;
	StatisticsFilterPointerType m_StatisticsFilter;


};

}
}

OTB_APPLICATION_EXPORT(vns::Wrapper::CirrusFlag)
