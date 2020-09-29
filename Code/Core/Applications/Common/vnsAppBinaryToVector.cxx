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
#include "vnsBinaryToVectorImageFilter.h"
#include <string>

namespace vns
{

namespace Wrapper
{

using namespace otb::Wrapper;

class BinaryToVector : public Application
{
public:
	/** Standard class typedefs. */
	typedef BinaryToVector             Self;
	typedef otb::Wrapper::Application     Superclass;
	typedef itk::SmartPointer<Self>       Pointer;
	typedef itk::SmartPointer<const Self> ConstPointer;

	/** Standard macro */
	itkNewMacro(Self);

	itkTypeMacro(BinaryToVector, otb::Wrapper::Application);

	/** Some convenient typedefs. */
    // Integer data
	typedef UInt8VectorImageType MaskVectorImageType;
	typedef MaskVectorImageType::ConstPointer MaskVectorConstPointerType;
    typedef UInt16ImageType UnsignedImageType;
    typedef UInt16ImageType::Pointer UnsignedImagePointerType;

    typedef BinaryToVectorImageFilter<UnsignedImageType,MaskVectorImageType> BinaryToVectorImageFilterType;
    typedef BinaryToVectorImageFilterType::Pointer BinaryToVectorImageFilterPointer;

private:
	void DoInit()
	{
		SetName("BinaryToVector");
		SetDescription("BinaryToVector algo.");
		Loggers::GetInstance()->Initialize(GetName());
		// Documentation
		SetDocLongDescription("This application separate each bit plan into a vector image");
		SetDocLimitations("None");
		SetDocAuthors("MAJA-Team");
		SetDocSeeAlso("MAJA Doc");

		AddDocTag("Statistics");

		AddParameter(ParameterType_InputImage,  "im",   "binaryimage");
		AddParameter(ParameterType_OutputImage, "out", "vectorimage");
		AddParameter(ParameterType_Int,"nbcomp","Component per pixels");
		SetParameterDescription("out","output image");
		SetParameterOutputImagePixelType("out", ImagePixelType_uint8);

		AddRAMParameter("ram");
		SetDefaultParameterInt("ram",2048);

	}

	void DoUpdateParameters()
	{

	}


	void DoExecute()
	{
		// Init filters
		m_vector = BinaryToVectorImageFilterType::New();
		m_vector->SetReleaseDataFlag(true);
		m_vector->SetReleaseDataBeforeUpdateFlag(true);
		//Get Image
		UnsignedImagePointerType l_im = this->GetParameterUInt16Image("im");
		m_vector->SetInput(l_im);
		m_vector->SetNumberOfComponentsPerPixel(this->GetParameterInt("nbcomp"));
		SetParameterOutputImage<MaskVectorImageType>("out",m_vector->GetOutput());

	}


	/** Filters declaration */
	BinaryToVectorImageFilterPointer m_vector;
};

}
}

OTB_APPLICATION_EXPORT(vns::Wrapper::BinaryToVector)
