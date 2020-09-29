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
#include "vnsCirrusMultiMeanStatisticsVectorImageFilter.h"
#include "vnsValueCountImageFilter.h"
#include "otbMultiToMonoChannelExtractROI.h"
#include "vnsMaskingImageFunctor.h"
#include "itkBinaryFunctorImageFilter.h"
#include "itkOrImageFilter.h"
#include "vnsThresholdImageFunctor.h"
#include <string>

namespace vns
{
namespace Wrapper
{

using namespace otb::Wrapper;

class GammaCompute : public Application
{
public:
	/** Standard class typedefs. */
	typedef GammaCompute               Self;
	typedef otb::Wrapper::Application     Superclass;
	typedef itk::SmartPointer<Self>       Pointer;
	typedef itk::SmartPointer<const Self> ConstPointer;

	/** Standard macro */
	itkNewMacro(Self);

	itkTypeMacro(GammaCompute, otb::Wrapper::Application);

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
	//TOA typedef
	typedef otb::MultiToMonoChannelExtractROI<TOAImageInternalPixelType, TOAImageInternalPixelType> MultiToMonoChannelExtractFilterType;
	typedef MultiToMonoChannelExtractFilterType::Pointer                                            MultiToMonoChannelExtractFilterPointerType;

	//MASK typedef
	//Boolean operator
	typedef Functor::MaskingImageFunctor<InputMaskPixelType,InputMaskPixelType,InputMaskPixelType> MaskApplyFunctorType;
	typedef itk::BinaryFunctorImageFilter<InputMaskType,InputMaskType, InputMaskType, MaskApplyFunctorType> MaskApplyFilterType;
	typedef MaskApplyFilterType::Pointer                                                    MaskApplyFilterPointerType;
	typedef itk::OrImageFilter<InputMaskType, InputMaskType, InputMaskType>                 OrFilterType;
	typedef OrFilterType::Pointer                                                           OrFilterPointerType;
	typedef Functor::ThresholdImageFunctor<InputMaskPixelType,InputMaskPixelType>           ThresholdImageFunctorType;
	typedef itk::UnaryFunctorImageFilter<InputMaskType,InputMaskType,ThresholdImageFunctorType> InvertMaskFilterType;
	typedef InvertMaskFilterType::Pointer                                                       InvertMaskFilterPointerType;

	//Statistics filter
	typedef vns::CirrusMultiMeanStatisticsVectorImageFilter<TOAImageType,ImageType, InputMaskType> MultiMeanStatsFilterType;
	typedef MultiMeanStatsFilterType::Pointer MultiMeanPointer;

	//Pixel count
	typedef vns::ValueCountImageFilter<InputMaskType> PixelCountFilterType;
	typedef PixelCountFilterType::Pointer             PixelCountFilterPointerType;

private:
	void DoInit()
	{
		SetName("GammaCompute");
		SetDescription("GammaCompute algo.");
		Loggers::GetInstance()->Initialize(GetName());
		// Documentation
		SetDocLongDescription("This application computes the gamma");
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

		AddParameter(ParameterType_StringList, "bandlist", "Gamma Band List");
		AddParameter(ParameterType_InputImage,  "cloudcirrus","CloudCirrus Image");
		AddParameter(ParameterType_InputImage,  "cloudall","CloudAll  Image");
		AddParameter(ParameterType_InputImage,  "cloudrefl","CloudRefl  Image");
		AddParameter(ParameterType_Int,  "cirrusminpixinlist","CirrusMinPixInList");


		AddParameter(ParameterType_Int, "cirrusflag",  "CirrusFlag");
		SetParameterDescription("cirrusflag", "Is the image cirrus enought");
		SetParameterRole("cirrusflag", Role_Output);
		AddParameter(ParameterType_Float, "gamma",  "gamma");
		SetParameterDescription("gamma", "gamma of the image");
		SetParameterRole("gamma", Role_Output);

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
		m_nodataCirrusFilter = MaskApplyFilterType::New();
		m_thinCirrusFilter = MaskApplyFilterType::New();
		m_nodataThinCirrusFilter = MaskApplyFilterType::New();
		m_NonCirrusOrFilter = OrFilterType::New();
		m_multiMean = MultiMeanStatsFilterType::New();

		// Get input image pointers
		ImagePointer l_L1CIRRUS = this->GetParameterDoubleImage("l2cirrus");
		TOAImagePointer l_L2TOA = this->GetParameterDoubleVectorImage("l2toa");
		InputMaskConstPointer l_L2EDG = this->GetParameterUInt8Image("l2edg");
		DTMImageConstPointer l_L2DTM = this->GetParameterDoubleImage("l2dtm");

		//Get Params
		InputMaskConstPointer l_CloudCirrus = GetParameterUInt8Image("cloudcirrus");
		InputMaskConstPointer l_CloudAll = GetParameterUInt8Image("cloudall");
		InputMaskConstPointer l_CloudRefl = GetParameterUInt8Image("cloudrefl");
		const unsigned int l_CirrusMinPixInList = GetParameterInt("cirrusminpixinlist");
		unsigned int l_CirrusCorrectionFlag = 0;
		/** Gamma bands to use */
		const std::vector<std::string> l_BandIdxStr = GetParameterStringList("bandlist");
		Utilities::ListOfUIntegers l_BandIdx;
		for (unsigned int i = 0; i < l_BandIdxStr.size();i++)
		{
			l_BandIdx.push_back(Utilities::AsUnsignedInt(l_BandIdxStr[i]));
		}
		//Process
		vnsLogDebugMacro("Starting cirrus mask cre");
		if (l_BandIdx.empty())
		{
			vnsLogDebugMacro("No band given to compute, resuming");
		}

		//Apply EDG on cirrus mask to remove nodata
		m_nodataCirrusFilter->SetInput1(l_CloudCirrus);
		m_nodataCirrusFilter->SetInput2(l_L2EDG);
		m_nodataCirrusFilter->GetFunctor().SetBackgroundValue(static_cast<InputMaskType::PixelType>(1));
		m_nodataCirrusFilter->GetFunctor().SetReplaceValue(static_cast<InputMaskType::PixelType>(0));
		SetParameterOutputImage<InputMaskType>("mask",m_nodataCirrusFilter->GetOutput());
		// Generate Thin Cirrus mask from CLD.cirrus and CLD.refl
		m_thinCirrusFilter->SetInput1(l_CloudCirrus);
		m_thinCirrusFilter->SetInput2(l_CloudRefl);
		m_thinCirrusFilter->GetFunctor().SetBackgroundValue(static_cast<InputMaskType::PixelType>(1));
		m_thinCirrusFilter->GetFunctor().SetReplaceValue(static_cast<InputMaskType::PixelType>(0));
		//Apply EDG on cirrus mask to remove nodata
		m_nodataThinCirrusFilter->SetInput1(m_thinCirrusFilter->GetOutput());
		m_nodataThinCirrusFilter->SetInput2(l_L2EDG);
		m_nodataThinCirrusFilter->GetFunctor().SetBackgroundValue(static_cast<InputMaskType::PixelType>(1));
		m_nodataThinCirrusFilter->GetFunctor().SetReplaceValue(static_cast<InputMaskType::PixelType>(0));
		// Generate NonCirrus Mask from CLD.all and EDG
		m_NonCirrusOrFilter->SetInput1(l_CloudAll);
		m_NonCirrusOrFilter->SetInput2(l_L2EDG);

		//Pipeline the various mean computation
		m_multiMean->SetInput(l_L2TOA);
		m_multiMean->SetL1TOACirrusInput(l_L1CIRRUS);
		m_multiMean->SetCirrusMaskInput(m_nodataCirrusFilter->GetOutput());
		m_multiMean->SetCirrusForegroundValue(static_cast<InputMaskType::PixelType>(1));
		m_multiMean->SetThinCirrusMaskInput(m_nodataThinCirrusFilter->GetOutput());
		m_multiMean->SetThinCirrusForegroundValue(static_cast<InputMaskType::PixelType>(1));
		m_multiMean->SetNonCirrusMaskInput(m_NonCirrusOrFilter->GetOutput());
		m_multiMean->SetNonCirrusForegroundValue(static_cast<InputMaskType::PixelType>(0));

		//Update stats filter only on the requested resolution for test
		vnsLogDebugMacro("Updating multi stats");
		m_multiMean->GetStreamer()->SetAutomaticStrippedStreaming(this->GetParameterInt("ram"));
		m_multiMean->Update();

		//Get nb pixels for each mask
		const unsigned int nbPixCirrus = m_multiMean->GetNbOfValidCirrusValues();
		vnsLogDebugValueMacro(nbPixCirrus)
		const unsigned int nbPixThin = m_multiMean->GetNbOfValidThinCirrusValues();
		vnsLogDebugValueMacro(nbPixThin)
		const unsigned int nbPixNon = m_multiMean->GetNbOfValidNonCirrusValues();
		vnsLogDebugValueMacro(nbPixNon)

		if (nbPixCirrus < l_CirrusMinPixInList || nbPixThin < l_CirrusMinPixInList || nbPixNon < l_CirrusMinPixInList)
		{
			vnsLogDebugMacro("There is not enought cirrus pixel for cirrus Corrections");
			//Not enought pixels to work with
			l_CirrusCorrectionFlag = 0;
		}
		else
		{
			//Correction activated
			l_CirrusCorrectionFlag = 1;
			vnsLogDebugMacro("Cirrus Corrections enabled")
		}
		//Get the mean cirrus for the ref resolution
		const double l_MeanCirrus = m_multiMean->GetL1ThinCirrusMean();
		vnsLogDebugValueMacro(l_MeanCirrus);
		std::vector<double> l_GammaVector;
		if (!l_BandIdx.empty())
		{
			const TOAImagePixelType l_NonMean = m_multiMean->GetL2NonCirrusMean();
			vnsLogDebugValueMacro(l_NonMean);
			const TOAImagePixelType l_ThinMean = m_multiMean->GetL2ThinCirrusMean();
			vnsLogDebugValueMacro(l_ThinMean);
			TOAImagePixelType l_gamma;
			l_gamma.SetSize(l_NonMean.GetSize());
			for (unsigned int b = 0; b < l_gamma.GetSize(); b++)
			{
				if (l_ThinMean[b] - l_NonMean[b] != 0)
					l_gamma[b] = l_MeanCirrus / (l_ThinMean[b] - l_NonMean[b]);
				else
					l_gamma[b] = 0;
			}
			for (unsigned int b = 0; b < l_BandIdx.size(); b++)
			{
				const unsigned int l_band = l_BandIdx[b];
				if (l_band > l_gamma.GetSize())
				{
					vnsLogWarningMacro("Index of band exceed the number of band, ignored for computation")
				} else
				{
					l_GammaVector.push_back(static_cast<double>(l_gamma[l_band]));
					vnsLogDebugMacro("Gamma for band " + vns::Utilities::AsString(l_band) + ": " + vns::Utilities::DoubleToString(l_gamma[l_band]));
				}
			}
		}
		//Compute GammaMean
		double l_GammaMean = std::accumulate(l_GammaVector.begin(), l_GammaVector.end(), 0.0);
		if (l_GammaVector.size()!=0)
		{
			l_GammaMean /= l_GammaVector.size();
		}

		vnsLogDebugValueMacro(l_GammaMean);

		// Set the output
		SetParameterInt("cirrusflag",l_CirrusCorrectionFlag);
		SetParameterFloat("gamma",l_GammaMean);

		m_multiMean = NULL;

	}


	/** Filters declaration */
	MaskApplyFilterPointerType m_nodataCirrusFilter;
	MaskApplyFilterPointerType m_thinCirrusFilter;
	MaskApplyFilterPointerType m_nodataThinCirrusFilter;
	OrFilterPointerType m_NonCirrusOrFilter;
	MultiMeanPointer m_multiMean;


};

}
}


OTB_APPLICATION_EXPORT(vns::Wrapper::GammaCompute)
