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
#include "itkSignedMaurerDistanceMapImageFilter.h"
#include "vnsCirrusReflectanceFunctor.h"
#include "vnsCirrusCorrectionApplyImageFilter.h"
#include "vnsNormalizeAndThresholdFunctor.h"

#include <string>

namespace vns
{
namespace Wrapper
{

using namespace otb::Wrapper;

class CirrusCorrectionApply : public Application
{
public:
	/** Standard class typedefs. */
	typedef CirrusCorrectionApply         Self;
	typedef otb::Wrapper::Application     Superclass;
	typedef itk::SmartPointer<Self>       Pointer;
	typedef itk::SmartPointer<const Self> ConstPointer;

	/** Standard macro */
	itkNewMacro(Self);

	itkTypeMacro(CirrusCorrectionApply, otb::Wrapper::Application);

	/** Some convenient typedefs. */
	/** Input image typedefs. */
	typedef VNSRealVectorImageType                  TOAImageType;
	typedef TOAImageType::Pointer                   TOAImagePointer;
	typedef TOAImageType::ConstPointer              TOAImageConstPointer;
	typedef TOAImageType::PixelType                 TOAImagePixelType;
	typedef TOAImageType::InternalPixelType         TOAImageInternalPixelType;

	typedef VNSRealImageType                        ImageType;
	typedef ImageType::Pointer                      ImagePointer;
	typedef ImageType::ConstPointer                 ImageConstPointer;

	typedef VNSMaskVectorImageType                  InputMaskVectorType;
	typedef InputMaskVectorType::InternalPixelType  MaskInternalPixelType;
	typedef InputMaskVectorType::Pointer            InputMaskVectorPointer;
	typedef InputMaskVectorType::ConstPointer       InputMaskVectorConstPointer;

	typedef VNSMaskImageType                        InputMaskType;
	typedef InputMaskType::PixelType                InputMaskPixelType;
	typedef InputMaskType::Pointer                  InputMaskPointer;
	typedef InputMaskType::ConstPointer             InputMaskConstPointer;

	/** Altitude input image typedefs. */
	typedef VNSDTMImageType DTMImageType;
	typedef DTMImageType::ConstPointer DTMImageConstPointer;
	typedef DTMImageType::PixelType DTMImagePixelType;
	typedef DTMImageType::InternalPixelType DTMImageInternalPixelType;

	/** Output image typedefs. */
	typedef VNSRealVectorImageType                  OutputImageType;
	typedef OutputImageType::Pointer                OutputImagePointer;
	typedef OutputImageType::ConstPointer           OutputImageConstPointer;
	typedef OutputImageType::PixelType              OutputImagePixelType;
	typedef OutputImageType::InternalPixelType      OutputImageInternalPixelType;
	typedef OutputImageType::RegionType             OutputImageRegionType;


	/** Filters typedefs. */
	//Maurer Distance
	typedef itk::SignedMaurerDistanceMapImageFilter<InputMaskType, ImageType> SignedMaurerDistanceFilterType;
	typedef SignedMaurerDistanceFilterType::Pointer SignedMaurerDistancePointerType;

	//Normalize distance filter
	typedef Functor::NormalizeAndThresholdFunctor<ImageType::PixelType,ImageType::PixelType> NormalizeAndThresholdFunctorType;
	typedef itk::UnaryFunctorImageFilter<ImageType,ImageType,NormalizeAndThresholdFunctorType> NormalizeAndThresholdFilterType;
	typedef NormalizeAndThresholdFilterType::Pointer NormalizeAndThresholdFilterPointerType;

	//Cirrus reflectance computation
	typedef Functor::CirrusReflectanceFunctor<DTMImagePixelType, ImageType::PixelType, ImageType::PixelType> CirrusReflectanceFunctorType;
	typedef itk::BinaryFunctorImageFilter<DTMImageType,ImageType, ImageType,
			CirrusReflectanceFunctorType> CirrusReflectanceImageFilterType;
	typedef CirrusReflectanceImageFilterType::Pointer CirrusReflectanceImageFilterPointer;

	//Cirrus correction apply
	typedef CirrusCorrectionApplyImageFilter<TOAImageType, ImageType,InputMaskType,ImageType,TOAImageType >
	CirrusCorrectionApplyFilterType;
	typedef CirrusCorrectionApplyFilterType::Pointer CirrusCorrectionApplyFilterPointerType;


private:
	void DoInit()
	{
		SetName("CirrusCorrectionApply");
		SetDescription("CirrusCorrectionApply algo.");
		Loggers::GetInstance()->Initialize(GetName());
		// Documentation
		SetDocLongDescription("This application apply the cirrus correction");
		SetDocLimitations("None");
		SetDocAuthors("MAJA-Team");
		SetDocSeeAlso("MAJA Doc");

		AddDocTag("Statistics");

		AddParameter(ParameterType_InputImage,  "l2toa",   "toa image");
		SetParameterDescription("l2toa", "TOA image");
		AddParameter(ParameterType_InputImage,  "l2edg",   "EDG image");
		SetParameterDescription("l2edg", "Image used as edge");
		AddParameter(ParameterType_InputImage,  "l2dtm",   "DTM image");
		SetParameterDescription("l2dtm", "Image used as DTM");
		AddParameter(ParameterType_InputImage,  "l2cirrus", "Cirrus image");
		SetParameterDescription("l2cirrus", "Image used as cirrus");
		AddParameter(ParameterType_InputImage,  "cirrusmask", "Cirrus mask");
		SetParameterDescription("cirrusmask", "Image used as cirrus mask");

		AddParameter(ParameterType_Int, "resolution","Resolution in meters");
		AddParameter(ParameterType_Float, "maxdistance","max distance in meters");
		AddParameter(ParameterType_Float, "thresholdgain","Threshold gain for Reflectance");
		AddParameter(ParameterType_Float, "thresholdoffset","Threshold offset for Reflectance");
		AddParameter(ParameterType_Float, "thresholdfactor","Threshold factor for Reflectance");

		AddParameter(ParameterType_Float, "gammamean","Gamma mean value");
		AddParameter(ParameterType_Float, "swirgammafactor","Swir gamma factor");
		AddParameter(ParameterType_Float, "maxreflectance", "Cirrus max reflectance");
		AddParameter(ParameterType_StringList,  "bandcodelist",   "List of band");
		AddParameter(ParameterType_StringList,  "swirbandcodelist",   "List of swir band");

		AddParameter(ParameterType_OutputImage, "correctedtoa", "Corrected toa image");
		SetParameterOutputImagePixelType("correctedtoa", ImagePixelType_double);

		AddRAMParameter("ram");
		SetDefaultParameterInt("ram",2048);
	}

	void DoUpdateParameters()
	{

	}


	void DoExecute()
	{
		// Init filters
		m_MaurerDist = SignedMaurerDistanceFilterType::New();
		m_normalizer = NormalizeAndThresholdFilterType::New();
		m_CirrusReflCenter = CirrusReflectanceImageFilterType::New();
		m_CirrusReflEdge = CirrusReflectanceImageFilterType::New();
		m_CirrusApply = CirrusCorrectionApplyFilterType::New();

		//Get inputs
		ImagePointer l_L2CIRRUS = this->GetParameterDoubleImage("l2cirrus");
		TOAImagePointer l_L2TOA = this->GetParameterDoubleVectorImage("l2toa");
		InputMaskConstPointer l_L2EDG = this->GetParameterUInt8Image("l2edg");
		DTMImageConstPointer l_L2DTM = this->GetParameterDoubleImage("l2dtm");
		InputMaskConstPointer l_CirrusMask = this->GetParameterUInt8Image("cirrusmask");


		//Get parameters
		const unsigned int l_resMeters = this->GetParameterInt("resolution");
		const double l_CirrusMaxDistance = this->GetParameterFloat("maxdistance");
		const double l_CirrusThresholdGain = this->GetParameterFloat("thresholdgain");
		const double l_CirrusThresholdOffset = this->GetParameterFloat("thresholdoffset");
		const double l_CirrusThresholdFactor = this->GetParameterFloat("thresholdfactor");

		const double l_GammaMean = this->GetParameterFloat("gammamean");
		const double l_SWIRGammaFactor = this->GetParameterFloat("swirgammafactor");
		const double l_CirrusMaxReflectance = this->GetParameterFloat("maxreflectance");
		vns::Utilities::ListOfStrings l_bandcodelist=this->GetParameterStringList("bandcodelist");
		vns::Utilities::ListOfStrings l_swirbandcodelist=this->GetParameterStringList("swirbandcodelist");

		// Start correction

		//Compute dist
		m_MaurerDist->SetInput(l_CirrusMask);
		m_MaurerDist->SetSquaredDistance(false);
		m_MaurerDist->SetUseImageSpacing(false);
		m_MaurerDist->SetInsideIsPositive(false);
		m_MaurerDist->SetBackgroundValue(static_cast<InputMaskType::PixelType>(1));

		//Normalize dist with max distance
		m_normalizer->GetFunctor().SetNormalizeFactor(l_CirrusMaxDistance / l_resMeters);
		m_normalizer->SetInput(m_MaurerDist->GetOutput());

		//Compute Center cirrus reflectance
		m_CirrusReflCenter->GetFunctor().SetMaskThresholdGain(l_CirrusThresholdGain);
		m_CirrusReflCenter->GetFunctor().SetMaskThresholdOffset(l_CirrusThresholdOffset);
		m_CirrusReflCenter->GetFunctor().SetCirrusThresholdFactor(l_CirrusThresholdFactor);
		m_CirrusReflCenter->SetInput1(l_L2DTM);
		m_CirrusReflCenter->SetInput2(l_L2CIRRUS);

		//Compute Edge cirrus reflectance
		m_CirrusReflEdge->GetFunctor().SetMaskThresholdGain(l_CirrusThresholdGain);
		m_CirrusReflEdge->GetFunctor().SetMaskThresholdOffset(l_CirrusThresholdOffset);
		//For edge the factor is set to 1
		m_CirrusReflEdge->GetFunctor().SetCirrusThresholdFactor(1.0);
		m_CirrusReflEdge->SetInput1(l_L2DTM);
		m_CirrusReflEdge->SetInput2(l_L2CIRRUS);

		//Apply the correction
		m_CirrusApply->SetReflCenterInput(m_CirrusReflCenter->GetOutput());
		m_CirrusApply->SetReflEdgeInput(m_CirrusReflEdge->GetOutput());
		m_CirrusApply->SetTOAInput(l_L2TOA);
		m_CirrusApply->SetCirrusDistInput(m_normalizer->GetOutput());
		m_CirrusApply->SetCirrusMaskInput(l_CirrusMask);
		m_CirrusApply->SetForegroundMaskValue(static_cast<InputMaskType::PixelType>(1));

		m_CirrusApply->SetGamma(l_GammaMean);
		m_CirrusApply->SetSWIRGammaFactor(l_SWIRGammaFactor);
		m_CirrusApply->SetMaxReflectance(l_CirrusMaxReflectance);
		m_CirrusApply->SetListOfBandCode(l_bandcodelist);
		m_CirrusApply->SetListOfSWIRBandCode(l_swirbandcodelist);

		// Set the output
		SetParameterOutputImage<TOAImageType>("correctedtoa",m_CirrusApply->GetOutput());

	}


	/** Filters declaration */
	SignedMaurerDistancePointerType m_MaurerDist;
	NormalizeAndThresholdFilterPointerType m_normalizer;
	CirrusReflectanceImageFilterPointer m_CirrusReflCenter;
	CirrusReflectanceImageFilterPointer m_CirrusReflEdge;
	CirrusCorrectionApplyFilterPointerType m_CirrusApply;


};

}
}


OTB_APPLICATION_EXPORT(vns::Wrapper::CirrusCorrectionApply)
