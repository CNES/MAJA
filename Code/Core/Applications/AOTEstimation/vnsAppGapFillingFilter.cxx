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
#include "vnsGapFillingImageFilter.h"

namespace vns
{
namespace Wrapper
{

using namespace otb::Wrapper;

class GapFilling : public Application
{
public:
	/** Standard class typedefs. */
	typedef GapFilling              Self;
	typedef otb::Wrapper::Application     Superclass;
	typedef itk::SmartPointer<Self>       Pointer;
	typedef itk::SmartPointer<const Self> ConstPointer;

	/** Standard macro */
	itkNewMacro(Self);

	itkTypeMacro(GapFilling, otb::Application);

	/** Image typedef */
	typedef DoubleImageType OutputImageType;
	typedef OutputImageType::Pointer OutputImagePointerType;
	typedef OutputImageType::ConstPointer OutputImageConstPointerType;
	typedef OutputImageType::InternalPixelType NoDataType;

	typedef UInt8ImageType OutputMaskType;
	typedef OutputMaskType::Pointer OutputMaskPointerType;
	typedef OutputMaskType::ConstPointer OutputMaskConstPointerType;
	typedef OutputMaskType::PixelType OutputMaskPixelType;
	typedef OutputMaskType::RegionType OutputMaskRegionType;

	/** Filters typedef */
	typedef GapFillingImageFilter<OutputImageType, OutputImageType, OutputMaskType> GapFillingImageFilterType;
	typedef GapFillingImageFilterType::Pointer GapFillingImageFilterPointerType;

private:
	void DoInit()
	{
		SetName("GapFilling");
		SetDescription("Do the gap filling on an image.");
		Loggers::GetInstance()->Initialize(GetName());
		// Documentation
		SetDocLongDescription("This application do the gap filling on an image");
		SetDocLimitations("None");
		SetDocAuthors("MAJA-Team");
		SetDocSeeAlso("MAJA Doc");

		AddDocTag("Statistics");

		AddParameter(ParameterType_InputImage,  "im",   "image");
		SetParameterDescription("im", "Image");
		AddParameter(ParameterType_Int, "initwindowsradius","");
		AddParameter(ParameterType_Int, "maxwindowsradius","");
		AddParameter(ParameterType_Float, "reall2nodata","");
		AddParameter(ParameterType_Float, "mean","");
		AddParameter(ParameterType_Float, "defaultvalue","");
		AddParameter(ParameterType_Bool, "hasvalidpixels","");
		AddParameter(ParameterType_OutputImage, "out", "Out image");
		SetParameterDescription("out", "Gapfilled image");
		SetParameterOutputImagePixelType("out", ImagePixelType_double);
		AddParameter(ParameterType_OutputImage, "outmask", "Out mask");
		SetParameterDescription("outmask", "Gapfilled mask");
		SetParameterOutputImagePixelType("outmask", ImagePixelType_uint8);

		AddRAMParameter("ram");
		SetDefaultParameterInt("ram",2048);


	}

	void DoUpdateParameters()
	{

	}


	void DoExecute()
	{
		// Init filters
		m_GapFillingImageFilter = GapFillingImageFilterType::New();

		//GetImage
		OutputImagePointerType l_im = GetParameterDoubleImage("im");
		m_GapFillingImageFilter->SetInputImage(l_im);
		m_GapFillingImageFilter->SetMeanValue(GetParameterFloat("mean"));
		m_GapFillingImageFilter->SetDefaultValue(GetParameterFloat("defaultvalue"));
		m_GapFillingImageFilter->SetInitWindowRadius(GetParameterInt("initwindowsradius"));
		m_GapFillingImageFilter->SetMaxWindowRadius(GetParameterInt("maxwindowsradius"));
		m_GapFillingImageFilter->SetNoData(GetParameterFloat("reall2nodata"));
		m_GapFillingImageFilter->SetHasValidPixel(GetParameterInt("hasvalidpixels"));
		SetParameterOutputImage<OutputImageType>("out",m_GapFillingImageFilter->GetOutputFilledImage());
		SetParameterOutputImage<OutputMaskType>("outmask",m_GapFillingImageFilter->GetOutputMask());
	}

	/** Filters declaration */
	GapFillingImageFilterPointerType m_GapFillingImageFilter;


};

}
}

OTB_APPLICATION_EXPORT(vns::Wrapper::GapFilling)
