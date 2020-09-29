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
#include "vnsSnowMaskDeterminationImageFilter.h"
#include "itkBinaryBallStructuringElement.h"
#include "itkBinaryMorphologicalClosingImageFilter.h"
#include "itkOrImageFilter.h"
#include "vnsLoggers.h"

#include <string>

namespace vns
{
namespace Wrapper
{

using namespace otb::Wrapper;

class SnowMask : public Application
{
public:
	/** Standard class typedefs. */
	typedef SnowMask              Self;
	typedef otb::Wrapper::Application     Superclass;
	typedef itk::SmartPointer<Self>       Pointer;
	typedef itk::SmartPointer<const Self> ConstPointer;

	/** Standard macro */
	itkNewMacro(Self);

	itkTypeMacro(SnowMask, otb::Wrapper::Application);

	/** Some convenient typedefs. */
	typedef DoubleVectorImageType InputImageType;
	typedef InputImageType::ConstPointer InputImageConstPointer;
	typedef InputImageType::RegionType InputImageRegionType;
	typedef InputImageType::PixelType InputImagePixelType;
	typedef InputImageType::SizeType SizeType;
	typedef VNSMaskImageType OutputMaskType;
	typedef OutputMaskType::Pointer OutputMaskPointer;
	typedef OutputMaskType::PixelType OutputMaskPixelType;

	typedef DoubleImageType InputSimpleImageType;
	typedef InputSimpleImageType::Pointer InputSimpleImagePointer;
	typedef InputSimpleImageType::ConstPointer InputSimpleImageConstPointer;
	typedef InputSimpleImageType::PixelType InputSimpleImagePixelType;

	typedef UInt8ImageType InputMaskType;
	typedef InputMaskType::Pointer InputMaskPointer;
	typedef InputMaskType::ConstPointer InputMaskConstPointer;

	typedef DoubleImageType InternalIncidenceAngleImageType;
	typedef InternalIncidenceAngleImageType::Pointer InternalIncidenceAngleImageTypePointer;
	typedef InternalIncidenceAngleImageType::PixelType InternalIncidenceAnglePixelType;

	typedef itk::ImageRegionConstIterator<InputMaskType> InputMaskConstIteratorType;
	typedef itk::ImageRegionConstIterator<InputImageType> InputImageConstIteratorType;
	typedef itk::ImageRegionIterator<OutputMaskType> OutputMaskIteratorType;

	typedef SnowMaskDeterminationImageFilter<InputImageType, InputMaskType, InternalIncidenceAngleImageType, OutputMaskType> SnowMaskDeterminationImageFilterType;
	typedef SnowMaskDeterminationImageFilterType::Pointer SnowMaskDeterminationImageFilterPointer;

	typedef itk::BinaryBallStructuringElement<OutputMaskPixelType, 2> KernelType;
	typedef itk::BinaryMorphologicalClosingImageFilter<OutputMaskType, OutputMaskType, KernelType>
	BinaryMorphologicalClosingImageFilterType;
	typedef BinaryMorphologicalClosingImageFilterType::Pointer BinaryMorphologicalClosingImageFilterPointer;

	typedef itk::OrImageFilter<InputMaskType, InputMaskType, InputMaskType> OrFilterType;
	typedef OrFilterType::Pointer OrFilterPointer;


private:
	void DoInit()
	{
		SetName("SnowMask");
		SetDescription("SnowMask algo.");
		Loggers::GetInstance()->Initialize(GetName());
		// Documentation
		SetDocLongDescription("This application computes the snow mask");
		SetDocLimitations("None");
		SetDocAuthors("MAJA-Team");
		SetDocSeeAlso("MAJA Doc");

		AddDocTag("Statistics");

		AddParameter(ParameterType_InputImage,  "tocr",   "tocr image");
		SetParameterDescription("tocr", "TOCR image at coarse all bands");
		AddParameter(ParameterType_InputImage,  "edg",   "EDG image");
		SetParameterDescription("edg", "Image used as edge at coarse");
		AddParameter(ParameterType_InputImage,  "angles",   "Incidence Angle image");
		SetParameterDescription("angles", "Incidence Angle image coarse");
		MandatoryOff("angles");
		AddParameter(ParameterType_InputImage,  "cirrusmask",   "Cirrus Mask image");
		SetParameterDescription("cirrusmask", "Cirrus mask image coarse (optional)");
		MandatoryOff("cirrusmask");
		AddParameter(ParameterType_InputImage,  "watermask",   "Water Mask image");
		SetParameterDescription("watermask", "L2 Water mask image coarse(optional)");
		MandatoryOff("watermask");


		AddParameter(ParameterType_Int,  "refband",   "Ref Band Index");
		SetParameterDescription("refband", "Reference band index to compute snow");
		AddParameter(ParameterType_Int,  "absband",   "Absorbing Band Index");
		SetParameterDescription("absband", "Absorbing band index to compute snow");
		AddParameter(ParameterType_Int,  "redband",   "Red Band Index");
		SetParameterDescription("redband", "Red band index to compute snow");

		AddParameter(ParameterType_Int,  "seradius",   "SERadius");
		SetParameterDescription("seradius", "SERadius for morphomath");
		AddParameter(ParameterType_Float,  "ndsithresh",  "NDSI Threshold");
		SetParameterDescription("ndsithresh","NDSI Threshold");
		AddParameter(ParameterType_Float,  "redthresh",  "Red Threshold");
		SetParameterDescription("redthresh","Red Threshold");
		AddParameter(ParameterType_Float,  "swirthresh",  "SWIR Threshold");
		SetParameterDescription("swirthresh","SWIR Threshold");
		MandatoryOff("swirthresh");
		AddParameter(ParameterType_Float,  "thetas",  "solar azimuth");
		SetParameterDescription("thetas","Solar azimuth for computation in Rad");
		MandatoryOff("thetas");
		AddParameter(ParameterType_Float,  "mincos",  "Min cos");
		SetParameterDescription("mincos","mincos computation");
		MandatoryOff("mincos");

		AddParameter(ParameterType_OutputImage, "mask", "snow mask");
		SetParameterDescription("mask","snow mask");
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
		m_SnowMaskDeterminationImageFilter = SnowMaskDeterminationImageFilterType::New();
		m_BinaryClosingImageFilter = BinaryMorphologicalClosingImageFilterType::New();
		m_OrFilter = OrFilterType::New();
		// Get input image pointers
		InputImageConstPointer l_tocr = this->GetParameterDoubleVectorImage("tocr");
		InputMaskConstPointer l_edg = this->GetParameterUInt8Image("edg");
		InternalIncidenceAngleImageTypePointer l_angles = ITK_NULLPTR;
		InputMaskConstPointer l_cirrus;
		InputMaskConstPointer l_water;

		//GetParameters
		const int l_refband = GetParameterInt("refband");
		const int l_absband = GetParameterInt("absband");
		const int l_redband = GetParameterInt("redband");

		const int l_seradius = GetParameterInt("seradius");
		const double l_ndsithreshold = GetParameterFloat("ndsithresh");
		const double l_redthreshold = GetParameterFloat("redthresh");

		// Set the Cirrus mask pointer
		if (!HasValue("cirrusmask"))
		{
			m_SnowMaskDeterminationImageFilter->SetIPNotValidMaskSubInput(l_edg);
		}
		else
		{
			l_cirrus = this->GetParameterUInt8Image("cirrusmask");
			m_OrFilter->SetInput1(l_edg);
			m_OrFilter->SetInput2(l_cirrus);
			m_SnowMaskDeterminationImageFilter->SetIPNotValidMaskSubInput(m_OrFilter->GetOutput());
		}

		if (!HasValue("watermask"))
		{
			m_SnowMaskDeterminationImageFilter->SetUseWaterMask(false);
		}
		else
		{
			l_water = this->GetParameterUInt8Image("watermask");
			m_SnowMaskDeterminationImageFilter->SetL2inWASInput(l_water);
			m_SnowMaskDeterminationImageFilter->SetUseWaterMask(true);
		}

		// Initializes the now mask determination filter
		m_SnowMaskDeterminationImageFilter->SetIPTOCRInput(l_tocr);
		if (HasValue("angles"))
		{
			l_angles = this->GetParameterDoubleImage("angles");
			m_SnowMaskDeterminationImageFilter->SetIncidenceAngleInput(l_angles);
			m_SnowMaskDeterminationImageFilter->SetUseIncidencesAngles(true);
			const double l_thetas = GetParameterFloat("thetas");
			const double l_mincos = GetParameterFloat("mincos");
			m_SnowMaskDeterminationImageFilter->SetMinCosI(l_mincos);
			m_SnowMaskDeterminationImageFilter->SetThetaS(l_thetas*otb::CONST_PI_180);
		}
		else
		{
			m_SnowMaskDeterminationImageFilter->SetUseIncidencesAngles(false);
		}
		//m_SnowMaskDeterminationImageFilter->SetL2inWASInput(l_WaterMaskPtr);
		// Set the bands values
		m_SnowMaskDeterminationImageFilter->SetRefBand(l_refband);
		m_SnowMaskDeterminationImageFilter->SetAbsBand(l_absband);
		m_SnowMaskDeterminationImageFilter->SetRedBand(l_redband);
		// Set the threshold values
		m_SnowMaskDeterminationImageFilter->SetNDSIThreshold(l_ndsithreshold);
		m_SnowMaskDeterminationImageFilter->SetRedThreshold(l_redthreshold);
		if (HasValue("swirthresh"))
		{
			m_SnowMaskDeterminationImageFilter->SetHasSWIR(true);
			const double l_swirthreshold = GetParameterFloat("swirthresh");
			m_SnowMaskDeterminationImageFilter->SetSWIRThreshold(l_swirthreshold);
		} else
		{
			vnsLogDebugMacro("No SWIR band available, deactivating the SWIR threshold test")
			m_SnowMaskDeterminationImageFilter->SetHasSWIR(false);
		}

		// Set the thetaS value


		// Initialize the Kernel of the Binary closing filter
		KernelType l_Kernel;
		l_Kernel.SetRadius(l_seradius);
		l_Kernel.CreateStructuringElement();
		m_BinaryClosingImageFilter->SetInput(m_SnowMaskDeterminationImageFilter->GetOutput());
		m_BinaryClosingImageFilter->SetKernel(l_Kernel);
		m_BinaryClosingImageFilter->SetForegroundValue(1);

		// Initialize the output
		SetParameterOutputImage<OutputMaskType>("mask",m_BinaryClosingImageFilter->GetOutput());


	}


	/** Filters declaration */
	SnowMaskDeterminationImageFilterPointer m_SnowMaskDeterminationImageFilter;
	BinaryMorphologicalClosingImageFilterPointer m_BinaryClosingImageFilter;
	OrFilterPointer m_OrFilter;


};

}
}

OTB_APPLICATION_EXPORT(vns::Wrapper::SnowMask)
