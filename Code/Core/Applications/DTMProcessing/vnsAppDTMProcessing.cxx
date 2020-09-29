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
#include "vnsDTMProcessingImageFilter.h"
#include <string>

namespace vns
{
namespace Wrapper
{

using namespace otb::Wrapper;

class DTMProcessing : public Application
{
public:
	/** Standard class typedefs. */
	typedef DTMProcessing              Self;
	typedef otb::Wrapper::Application     Superclass;
	typedef itk::SmartPointer<Self>       Pointer;
	typedef itk::SmartPointer<const Self> ConstPointer;

	/** Standard macro */
	itkNewMacro(Self);

	itkTypeMacro(DTMProcessing, otb::Wrapper::Application);

	/** Filters typedef */
	/** Some convenient typedefs. */
	typedef DoubleVectorImageType GRIDImageType;
	typedef GRIDImageType::ConstPointer GRIDImageConstPointer;
	typedef GRIDImageType::Pointer GRIDImagePointer;
	typedef GRIDImageType::PixelType GRIDImagePixelType;

	typedef DoubleImageType DTMImageType;
	typedef DTMImageType::Pointer DTMImagePointer;
	typedef DTMImageType::ConstPointer DTMImageConstPointer;
	typedef DTMImageType::PixelType DTMImagePixelType;

	typedef UInt8ImageType InputMaskType;
	typedef InputMaskType::Pointer InputMaskPointer;
	typedef InputMaskType::ConstPointer InputMaskConstPointer;
	typedef InputMaskType::PixelType InputMaskPixelType;
	typedef InputMaskType::InternalPixelType InputMaskInternalPixelType;

	typedef UInt8ImageType OutputImageType;
	typedef OutputImageType::Pointer OutputImagePointer;
	typedef OutputImageType::PixelType OutputImagePixelType;
	typedef OutputImageType::RegionType OutputImageRegionType;

	typedef DEMDataManager<DTMImageType> Intersector3DType;

	/** Filters typedefs */
	typedef DTMProcessingImageFilter<GRIDImageType, DTMImageType, InputMaskType, OutputImageType> DTMProcessingFilterType;
	typedef DTMProcessingFilterType::Pointer DTMProcessingPointer;


private:
	void DoInit()
	{
		SetName("DTMProcessing");
		SetDescription("DTMProcessing algo.");
		Loggers::GetInstance()->Initialize(GetName());

		// Documentation
		SetDocLongDescription("This application computes the hidden and shadows using solar angles and DTM");
		SetDocLimitations("None");
		SetDocAuthors("MAJA-Team");
		SetDocSeeAlso("MAJA Doc");

		AddDocTag("Statistics");

		AddParameter(ParameterType_InputImage,  "solar",   "Solar image");
		SetParameterDescription("solar", "Image used as solar angle");
		AddParameter(ParameterType_InputImage,  "viedtm",   "VIEDTM image");
		SetParameterDescription("viedtm", "Image used as VIEDTM");
		AddParameter(ParameterType_InputImage,  "edg",   "EDG image");
		SetParameterDescription("edg", "Image used as edge");
		AddParameter(ParameterType_InputImage,  "dtm",   "DTM image");
		SetParameterDescription("dtm", "Image used as DTM");

		AddParameter(ParameterType_Float,  "distthreshold",  "dist threshold");
		SetParameterDescription("distthreshold","Dist threshold for computation");

		AddParameter(ParameterType_Int,  "solhref",  "solar height ref");
		SetParameterDescription("solhref","solar height ref");
		AddParameter(ParameterType_Int,  "viehref",  "viewing height ref");
		SetParameterDescription("viehref","viewing height ref");

		AddParameter(ParameterType_OutputImage, "hid", "Hidden mask image");
		SetParameterDescription("hid","hidden mask image");
		SetParameterOutputImagePixelType("hid", ImagePixelType_uint8);
		AddParameter(ParameterType_OutputImage, "shd", "Shadow mask image");
		SetParameterDescription("shd","shadow mask image");
		SetParameterOutputImagePixelType("shd", ImagePixelType_uint8);

		AddRAMParameter("ram");
		SetDefaultParameterInt("ram",2048);

		m_L2CoarseResolution = 1;


	}

	void DoUpdateParameters()
	{

	}


	void DoExecute()
	{
		vnsLogDebugMacro("Number of thread used : "<<itk::MultiThreader::GetGlobalDefaultNumberOfThreads())

		// Init filters
		m_DTMProcessingFilter = DTMProcessingFilterType::New();

		// Get input image pointers
		GRIDImageType::Pointer l_solar = this->GetParameterDoubleVectorImage("solar");
		InputMaskType::Pointer l_edg = this->GetParameterUInt8Image("edg");
		GRIDImageType::Pointer l_viedtm = this->GetParameterDoubleVectorImage("viedtm");
		DTMImageType::Pointer l_dtm = this->GetParameterDoubleImage("dtm");

		// Set viewing and solar reference altitudes
		// --------------------------------------------------
		const unsigned short l_VIEHRef = static_cast<unsigned short>(this->GetParameterInt("viehref"));
		const unsigned short l_SOLHRef = static_cast<unsigned short>(this->GetParameterInt("solhref"));
		const double l_DistanceThreshold = this->GetParameterFloat("distthreshold");
		//Load I3D
		m_I3D.Load(l_dtm);

		m_DTMProcessingFilter->SetIPSOLInput(l_solar);
		m_DTMProcessingFilter->SetSOLHRef(l_SOLHRef);
		// =====================================================
		// Generate the hidden faces and the cast shadows masks
		// =====================================================
		m_DTMProcessingFilter->SetDTMInput(l_dtm);
		m_DTMProcessingFilter->SetIPVIEInput(l_viedtm);
		m_DTMProcessingFilter->SetIPEDGInput(l_edg);

		m_DTMProcessingFilter->SetI3D(&m_I3D);

		m_DTMProcessingFilter->SetL2CoarseResolution(m_L2CoarseResolution);
		m_DTMProcessingFilter->SetVIEHRef(l_VIEHRef);

		m_DTMProcessingFilter->SetDistanceThreshold(l_DistanceThreshold);

		SetParameterOutputImage<OutputImageType>("hid",m_DTMProcessingFilter->GetIPHIDOutput());
		SetParameterOutputImage<OutputImageType>("shd",m_DTMProcessingFilter->GetIPSHDOutput());
	}

	// Intersector 3D
	Intersector3DType m_I3D;

	/** Filters declaration */
	DTMProcessingPointer m_DTMProcessingFilter;
	/** L2 coarse resolution declaration */
	unsigned int m_L2CoarseResolution;

};

}
}

OTB_APPLICATION_EXPORT(vns::Wrapper::DTMProcessing)
