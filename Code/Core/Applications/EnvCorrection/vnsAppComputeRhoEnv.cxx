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
#include "vnsChangeValueWithMaskVectorFunctor.h"
#include "otbConvolutionImageFilter.h"
#include "vnsConvolutionVectorImageFilter.h"

namespace vns
{
namespace Wrapper
{

using namespace otb::Wrapper;

class ComputeRhoEnv : public Application
{
public:
	/** Standard class typedefs. */
	typedef ComputeRhoEnv Self;
	typedef otb::Wrapper::Application     Superclass;
	typedef itk::SmartPointer<Self>       Pointer;
	typedef itk::SmartPointer<const Self> ConstPointer;

	/** Standard macro */
	itkNewMacro(Self);

	itkTypeMacro(ComputeRhoEnv, otb::Wrapper::Application);

	/** Image typedef */
	typedef VNSRealVectorImageType InputVectorImageType;
	typedef InputVectorImageType::ConstPointer InputVectorImageConstPointerType;
	typedef InputVectorImageType::Pointer InputVectorImagePointerType;
	typedef InputVectorImageType::PixelType InputVectorImagePixelType;
	typedef InputVectorImageType::SizeType SizeType;
	typedef VNSMaskImageType InputMaskType;
	typedef InputMaskType::Pointer InputMaskPointerType;
	typedef InputMaskType::ConstPointer InputMaskConstPointerType;
	typedef InputMaskType::PixelType   InputMaskPixelType;
	typedef VNSRealVectorImageType OutputVectorImageType;
	typedef OutputVectorImageType::Pointer OutputVectorImagePointerType;
	typedef OutputVectorImageType::ConstPointer OutputVectorImageConstPointerType;
	typedef OutputVectorImageType::RegionType OutputVectorImageRegionType;
	typedef OutputVectorImageType::InternalPixelType OutputVectorImageInternalPixelType;
	typedef OutputVectorImageInternalPixelType NoDataType;
	typedef OutputVectorImageType::IndexType OutputVectorImageIndexType;
	typedef OutputVectorImageType::SizeType OutputVectorImageSizeType;
	typedef OutputVectorImageType::PixelType OutputVectorPixelType;
	typedef OutputVectorImageType::InternalPixelType OutputVectorInternalPixelType;

	//Compute rhoenv
	typedef ConvolutionVectorImageFilter<InputVectorImageType, InputMaskType, OutputVectorImageType> ConvolutionVectorImageFilterType;
	typedef ConvolutionVectorImageFilterType::Pointer ConvolutionVectorImageFilterPointerType;
	typedef ConvolutionVectorImageFilterType::ArrayType ArrayType;

	typedef Functor::ChangeValueWithMaskVectorFunctor<OutputVectorPixelType, InputMaskPixelType, OutputVectorPixelType>
	ChangeValueVectorFunctorType;
	typedef itk::BinaryFunctorImageFilter<OutputVectorImageType, InputMaskType, OutputVectorImageType,
			ChangeValueVectorFunctorType> ChangeValueWithMaskVectorImageFilterType;
	typedef ChangeValueWithMaskVectorImageFilterType::Pointer ChangeValueWithMaskVectorImageFilterPointer;

private:
	void DoInit()
	{
		SetName("ComputeRhoEnv");
		SetDescription("Generate the RhoEnv.");
		Loggers::GetInstance()->Initialize(GetName());
		// Documentation
		SetDocLongDescription("This application computes the RhoEnv");
		SetDocLimitations("None");
		SetDocAuthors("MAJA-Team");
		SetDocSeeAlso("MAJA Doc");

		AddDocTag("Statistics");

		AddParameter(ParameterType_InputImage,  "toc",   "TOC image");
		SetParameterDescription("toc", "TOC image at coarse");
		AddParameter(ParameterType_InputImage,  "edg",   "EDG image");
		SetParameterDescription("edg", "Image used as EDG at coarse");

		AddParameter(ParameterType_Float, "radius", "Radius");
		AddParameter(ParameterType_Float, "filtercoeff", "FilterCoeff");

		AddParameter(ParameterType_OutputImage, "rhoenv", "Computed rhoenv");
		SetParameterDescription("rhoenv","Computed rhoenv");
		SetParameterOutputImagePixelType("rhoenv", ImagePixelType_double);

		AddRAMParameter("ram");
		SetDefaultParameterInt("ram",2048);

	}

	void DoUpdateParameters()
	{

	}


	void DoExecute()
	{
		// Init filters
		m_Convolution = ConvolutionVectorImageFilterType::New();
		m_VectorChangeValueWithMaskFilter = ChangeValueWithMaskVectorImageFilterType::New();
		//GetImagePointers
		InputVectorImageConstPointerType lIPTOCSubPtr = this->GetParameterDoubleVectorImage("toc");
		InputMaskConstPointerType lIPEDGPtr = this->GetParameterUInt8Image("edg");

		//Params
		const double l_radius = this->GetParameterFloat("radius");
		const double l_coeff = this->GetParameterFloat("filtercoeff");
		SizeType l_EnvironmentCorrectionRadius;
        l_EnvironmentCorrectionRadius[0] = l_radius;
        l_EnvironmentCorrectionRadius[1] = l_radius;
        const unsigned int l_EnvCorSize = (2 * l_radius + 1) * (2 * l_radius + 1);
        ArrayType l_EnvironmentCorrectionfilterCoeffs;
        l_EnvironmentCorrectionfilterCoeffs.SetSize(l_EnvCorSize);
        l_EnvironmentCorrectionfilterCoeffs.Fill(l_coeff);

		// 1 - Convolution produces rho env sub
		// WARNING :to obtain exactly the same result that the CNES reference
		// we have to set 0 for the NoData value of the convolution.
		// But in fact, we should create a stricter mask in order not to use the blurred
		// values due to the expand filter. (Frontier between the data and noData values)
		// Such a mask could be created by thresholding
		m_Convolution->SetRadius(l_EnvironmentCorrectionRadius);
		m_Convolution->SetFilter(l_EnvironmentCorrectionfilterCoeffs);
		m_Convolution->SetNoData(0.);
		m_Convolution->SetInput(lIPTOCSubPtr);
		m_Convolution->SetMask(lIPEDGPtr);
		m_Convolution->UpdateOutputInformation();
		// Apply the edge mask to the rho_env image
		m_VectorChangeValueWithMaskFilter->SetInput1(m_Convolution->GetOutput());
		m_VectorChangeValueWithMaskFilter->SetInput2(lIPEDGPtr);
		m_VectorChangeValueWithMaskFilter->GetFunctor().SetBackgroundMaskValue(0);
		const unsigned int lPixSize = m_Convolution->GetOutput()->GetNumberOfComponentsPerPixel();
		InputVectorImagePixelType lChangeValue;
		lChangeValue.SetSize(lPixSize);
		lChangeValue.Fill(10000);
		vnsLogDebugMacro("lChangeValue Size = "<<lChangeValue.GetSize())
		m_VectorChangeValueWithMaskFilter->GetFunctor().SetChange(lChangeValue);
		m_VectorChangeValueWithMaskFilter->UpdateOutputInformation();



		//Set the output
		SetParameterOutputImage<OutputVectorImageType>("rhoenv",m_VectorChangeValueWithMaskFilter->GetOutput());
	}

	/** Filters declaration */
	ConvolutionVectorImageFilterPointerType m_Convolution;
	ChangeValueWithMaskVectorImageFilterPointer m_VectorChangeValueWithMaskFilter;

};

}
}

OTB_APPLICATION_EXPORT(vns::Wrapper::ComputeRhoEnv)
