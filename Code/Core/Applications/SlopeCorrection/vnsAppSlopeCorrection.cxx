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
#include "vnsInternalSlopeCorrectionImageFilter.h"
#include "vnsDedicatedPadAndFastResampleImageFilter.h"
#include "vnsUtilities.h"


namespace vns
{
namespace Wrapper
{

using namespace otb::Wrapper;

class SlopeCorrection : public Application
{
public:
	/** Standard class typedefs. */
	typedef SlopeCorrection Self;
	typedef otb::Wrapper::Application     Superclass;
	typedef itk::SmartPointer<Self>       Pointer;
	typedef itk::SmartPointer<const Self> ConstPointer;

	/** Standard macro */
	itkNewMacro(Self);

	itkTypeMacro(SlopeCorrection, otb::Wrapper::Application);

	/** Image typedef */
	typedef VNSRealVectorImageType InputVectorImageType;
	typedef InputVectorImageType::ConstPointer InputVectorImageConstPointerType;
	typedef InputVectorImageType::Pointer InputVectorImagePointerType;

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

	typedef otb::Image<double, 2> InternalIncidenceAngleImageType;
	typedef InternalIncidenceAngleImageType::Pointer InternalIncidenceAngleImageTypePointer;
	typedef InternalIncidenceAngleImageType::PixelType InternalIncidenceAnglePixelType;

	typedef InternalSlopeCorrectionImageFilter<InputVectorImageType, InputImageType, InternalIncidenceAngleImageType, OutputVectorImageType, OutputMaskType>
	InternalSlopeCorrectionImageFilterType;
	typedef InternalSlopeCorrectionImageFilterType::Pointer InternalSlopeCorrectionImageFilterPointerType;
	typedef DedicatedPadAndFastResampleImageFilter<InputVectorImageType, OutputVectorImageType> DedicatedPadAndFastResampleImageFilterType;
	typedef DedicatedPadAndFastResampleImageFilterType::Pointer DedicatedPadAndFastResampleImageFilterPointerType;

private:
	void DoInit()
	{
		SetName("SlopeCorrection");
		SetDescription("Generate the Transmission.");
		Loggers::GetInstance()->Initialize(GetName());
		// Documentation
		SetDocLongDescription("This application computes the SlopeCorrection");
		SetDocLimitations("None");
		SetDocAuthors("MAJA-Team");
		SetDocSeeAlso("MAJA Doc");

		AddDocTag("Statistics");

		AddParameter(ParameterType_InputImage,  "tdirsub",   "TDIR");
		SetParameterDescription("tdirsub", "TDIR at coarse res");
		AddParameter(ParameterType_InputImage,  "tdifsub",   "TDIF");
		SetParameterDescription("tdifsub", "TDIF at coarse res");
		AddParameter(ParameterType_InputImage,  "dtmasc",   "DTM ASC image");
		SetParameterDescription("dtmasc", "Image used as DTM ASC");
		AddParameter(ParameterType_InputImage,  "dtmslc",   "DTM SLC image");
		SetParameterDescription("dtmslc", "Image used as DTM SLC");
		AddParameter(ParameterType_InputImage,  "rhoenv",   "RhoEnv image");
		SetParameterDescription("rhoenv", "Image used as RhoEnv");
		AddParameter(ParameterType_InputImage,  "incangle",   "Incidence Angle image");
		SetParameterDescription("incangle", "Image used as incidence angles");
		AddParameter(ParameterType_InputImage,  "sre",   "SRE image");
		SetParameterDescription("sre", "Image used as SRE");

		AddParameter(ParameterType_Float, "thetas", "ThetaS");
		AddParameter(ParameterType_Float, "phis", "PhiS");
		AddParameter(ParameterType_StringList, "thetav", "ThetaV");
		AddParameter(ParameterType_StringList, "phiv", "PhiV");
		AddParameter(ParameterType_Float, "mincosi", "MinCosI");
		AddParameter(ParameterType_Float, "mincose", "MinCosE");
		AddParameter(ParameterType_Float, "mincosratio", "MinCosRatio");
		AddParameter(ParameterType_Float, "nodata", "NoData");

		AddParameter(ParameterType_OutputImage, "fre", "Computed FRE");
		SetParameterDescription("fre","Computed FRE");
		SetParameterOutputImagePixelType("fre", ImagePixelType_double);
		AddParameter(ParameterType_OutputImage, "tgs", "Computed TGS");
		SetParameterDescription("tgs","Computed TGS");
		SetParameterOutputImagePixelType("tgs", ImagePixelType_uint8);
		AddParameter(ParameterType_OutputImage, "stl", "Computed STL");
		SetParameterDescription("stl","Computed STL");
		SetParameterOutputImagePixelType("stl", ImagePixelType_uint8);

		AddRAMParameter("ram");
		SetDefaultParameterInt("ram",2048);
	}

	void DoUpdateParameters()
	{

	}


	void DoExecute()
	{
		// Init filters
		m_InternalSlopeCorrection = InternalSlopeCorrectionImageFilterType::New();
		m_InternalSlopeCorrection->SetReleaseDataFlag(true);
		m_ExpandTdif = DedicatedPadAndFastResampleImageFilterType::New();
		m_ExpandTdif->SetReleaseDataFlag(true);
		m_ExpandTdir = DedicatedPadAndFastResampleImageFilterType::New();
		m_ExpandTdir->SetReleaseDataFlag(true);
		//Get image pointers
		InputVectorImagePointerType l_L2SREPtr = this->GetParameterDoubleVectorImage("sre");
		l_L2SREPtr->UpdateOutputInformation();
		InputImageConstPointerType l_APtr = this->GetParameterDoubleImage("dtmasc");
		InputImageConstPointerType l_SPtr = this->GetParameterDoubleImage("dtmslc");
		InputVectorImageConstPointerType l_RhoEnvPtr = this->GetParameterDoubleVectorImage("rhoenv") ;
		InternalIncidenceAngleImageTypePointer l_IncidenceAnglePtr = this->GetParameterDoubleImage("incangle");
		InputVectorImageConstPointerType l_TdirPtr = this->GetParameterDoubleVectorImage("tdirsub");
		InputVectorImageConstPointerType l_TdifPtr = this->GetParameterDoubleVectorImage("tdifsub");
		//GetParams
		const double l_ThetaS = this->GetParameterFloat("thetas");
		const double l_PhiS = this->GetParameterFloat("phis");
		const double l_MinCosI = this->GetParameterFloat("mincosi");
		const double l_MinCosE = this->GetParameterFloat("mincose");
		const double l_MinCosRatio = this->GetParameterFloat("mincosratio");
		const double l_RealL2NoData = this->GetParameterFloat("nodata");

		Utilities::ListOfDoubles l_ThetaV = Utilities::StringListAsDouble(
				this->GetParameterStringList("thetav"));
		Utilities::ListOfDoubles l_PhiV = Utilities::StringListAsDouble(
				this->GetParameterStringList("phiv"));


		const unsigned int lSize_ThetaV = l_ThetaV.size();
		for(unsigned int i=0;i<lSize_ThetaV;i++)
		    {
		    l_ThetaV.at(i) = l_ThetaV.at(i)*otb::CONST_PI_180;
		    vnsLogDebugMacro(" SlopeCorr ThetaV : "<< l_ThetaV.at(i))
		    }

		const unsigned int lSize_PhiV = l_PhiV.size();
		for(unsigned int i=0;i<lSize_PhiV;i++)
		    {
		    l_PhiV.at(i) = l_PhiV.at(i)*otb::CONST_PI_180;
		    vnsLogDebugMacro(" SlopeCorr PhiV : "<< l_PhiV.at(i))
		    }


		//Resampling because the app is not vector friendly
		AreaType l_DEMAreaToL2Resolution;
		l_DEMAreaToL2Resolution.Origin = l_L2SREPtr->GetOrigin();
		l_DEMAreaToL2Resolution.Spacing = l_L2SREPtr->GetSpacing();
		l_DEMAreaToL2Resolution.Size = l_L2SREPtr->GetLargestPossibleRegion().GetSize();
		vnsImagePadAndFastDedicatedResamplerMacro("ExpandTdir", /* DedicatedPadAndFastResampleImageFilterType, */
				m_ExpandTdir,l_TdirPtr /* Input */,
				ResamplerHelper::RB_INTERPOLATOR_MAJA_FAST_VECTOR_LINEAR /* Interpolator */,l_DEMAreaToL2Resolution)
		//, l_edgeVal /* edgePaddingValue */)
		vnsImagePadAndFastDedicatedResamplerMacro("ExpandTdif", /* DedicatedPadAndFastResampleImageFilterType, */
				m_ExpandTdif,l_TdifPtr /* Input */,
				ResamplerHelper::RB_INTERPOLATOR_MAJA_FAST_VECTOR_LINEAR /* Interpolator */,l_DEMAreaToL2Resolution)

		//Configure filter
		// Init the SlopeCorrection list filters
		m_InternalSlopeCorrection->SetS(l_SPtr);
		m_InternalSlopeCorrection->SetA(l_APtr);
		m_InternalSlopeCorrection->SetIncidenceAngle(l_IncidenceAnglePtr);
		m_InternalSlopeCorrection->SetL2SRE(l_L2SREPtr);
		m_InternalSlopeCorrection->SetRhoEnv(l_RhoEnvPtr);
		m_InternalSlopeCorrection->SetTdir(m_ExpandTdir->GetOutput());
		m_InternalSlopeCorrection->SetTdif(m_ExpandTdif->GetOutput());
		// MACCS 4.3 - optimization
		//                m_InternalSlopeCorrectionList->GetNthElement(l_res)->SetT(m_AddTdirTdifList->GetNthElement(l_res)->GetOutput());
		m_InternalSlopeCorrection->SetThetaS(l_ThetaS*otb::CONST_PI_180);
		m_InternalSlopeCorrection->SetPhiS(l_PhiS*otb::CONST_PI_180);
		m_InternalSlopeCorrection->SetThetaV(l_ThetaV);
		m_InternalSlopeCorrection->SetPhiV(l_PhiV);
		m_InternalSlopeCorrection->SetMinCosI(l_MinCosI);
		m_InternalSlopeCorrection->SetMinCosE(l_MinCosE);
		m_InternalSlopeCorrection->SetMinCosRatio(l_MinCosRatio);
		m_InternalSlopeCorrection->SetNoData(l_RealL2NoData);

		//Set the output
		SetParameterOutputImage<OutputVectorImageType>("fre",m_InternalSlopeCorrection->GetL2FRE());
		SetParameterOutputImage<OutputMaskType>("stl",m_InternalSlopeCorrection->GetL2STL());
		SetParameterOutputImage<OutputMaskType>("tgs",m_InternalSlopeCorrection->GetL2TGS());

	}

	/** Filters declaration */
	InternalSlopeCorrectionImageFilterPointerType m_InternalSlopeCorrection;
	DedicatedPadAndFastResampleImageFilterPointerType m_ExpandTdir;
	DedicatedPadAndFastResampleImageFilterPointerType m_ExpandTdif;

};

}
}

OTB_APPLICATION_EXPORT(vns::Wrapper::SlopeCorrection)
