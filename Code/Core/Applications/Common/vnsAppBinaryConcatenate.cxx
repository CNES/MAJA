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
#include "vnsBinaryConcatenationVectorImageFilter.h"
#include <string>

namespace vns
{

namespace Wrapper
{

using namespace otb::Wrapper;

class BinaryConcatenate : public Application
{
public:
	/** Standard class typedefs. */
	typedef BinaryConcatenate             Self;
	typedef otb::Wrapper::Application     Superclass;
	typedef itk::SmartPointer<Self>       Pointer;
	typedef itk::SmartPointer<const Self> ConstPointer;

	/** Standard macro */
	itkNewMacro(Self);

	itkTypeMacro(BinaryConcatenate, otb::Wrapper::Application);

	/** Some convenient typedefs. */
    // Integer data
	typedef UInt8VectorImageType MaskVectorImageType;
	typedef MaskVectorImageType::ConstPointer MaskVectorConstPointerType;
    typedef UInt16ImageType UnsignedImageType;
    typedef UInt16ImageType::Pointer UnsignedImagePointerType;

    typedef BinaryConcatenationVectorImageFilter<MaskVectorImageType, UnsignedImageType> BinaryConcatenationVectorImageFilterType;
    typedef BinaryConcatenationVectorImageFilterType::Pointer BinaryConcatenationVectorImageFilterPointer;

private:
	void DoInit()
	{
		SetName("BinaryConcatenate");
		SetDescription("BinaryConcatenate algo.");
		Loggers::GetInstance()->Initialize(GetName());
		// Documentation
		SetDocLongDescription("This application concatenate each image of a vector image into one");
		SetDocLimitations("None");
		SetDocAuthors("MAJA-Team");
		SetDocSeeAlso("MAJA Doc");

		AddDocTag("Statistics");

		AddParameter(ParameterType_InputImage,  "im",   "vectorimage");
		AddParameter(ParameterType_OutputImage, "out", "image");
		SetParameterDescription("out","output image");
		SetParameterOutputImagePixelType("out", ImagePixelType_uint16);

		AddRAMParameter("ram");
		SetDefaultParameterInt("ram",2048);

	}

	void DoUpdateParameters()
	{

	}


	void DoExecute()
	{
		// Init filters
		m_concatenater = BinaryConcatenationVectorImageFilterType::New();

		//Get Image
		MaskVectorConstPointerType l_im = this->GetParameterUInt8VectorImage("im");
		m_concatenater->SetInput(l_im);
		SetParameterOutputImage<UnsignedImageType>("out",m_concatenater->GetOutput());

	}


	/** Filters declaration */
	BinaryConcatenationVectorImageFilterPointer m_concatenater;
};

}
}

OTB_APPLICATION_EXPORT(vns::Wrapper::BinaryConcatenate)
