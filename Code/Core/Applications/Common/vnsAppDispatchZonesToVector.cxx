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
#include "vnsMacro.h"
#include "vnsDispatchZoneVectorImageFilter.h"
#include <string>

namespace vns
{

namespace Wrapper
{

using namespace otb::Wrapper;

class DispatchZonesToVector : public Application
{
public:
	/** Standard class typedefs. */
	typedef DispatchZonesToVector             Self;
	typedef otb::Wrapper::Application     Superclass;
	typedef itk::SmartPointer<Self>       Pointer;
	typedef itk::SmartPointer<const Self> ConstPointer;

	/** Standard macro */
	itkNewMacro(Self);

	itkTypeMacro(DispatchZonesToVector, otb::Wrapper::Application);

	/** Some convenient typedefs. */
    // Integer data
	typedef UInt8VectorImageType MaskVectorImageType;
	typedef MaskVectorImageType::ConstPointer MaskVectorConstPointerType;
    typedef FloatVectorImageType InputImageType;
    typedef InputImageType::Pointer InputImagePointerType;

    typedef DispatchZoneVectorImageFilter<InputImageType,MaskVectorImageType> DispatchZoneVectorImageFilterType;
    typedef DispatchZoneVectorImageFilterType::Pointer DispatchZoneVectorImageFilterPointer;

private:
	void DoInit()
	{
		SetName("DispatchZonesToVector");
		SetDescription("DispatchZonesToVector algo.");
		Loggers::GetInstance()->Initialize(GetName());
		// Documentation
		SetDocLongDescription("This application put in the output the maximum value of the input image mask that has the bit for band b set");
		SetDocLimitations("None");
		SetDocAuthors("MAJA-Team");
		SetDocSeeAlso("MAJA Doc");

		AddDocTag("Statistics");


		AddParameter(ParameterType_InputImageList,  "il",   "Images");
		SetParameterDescription("il", "Images to write");
		AddParameter(ParameterType_OutputImage, "out", "vectorimage");
		AddParameter(ParameterType_Int,"nbcomp","Component per pixels");
		AddParameter(ParameterType_StringList, "outvals","values to put in each input image");
		AddParameter(ParameterType_StringList, "outindexes","index of input bit to the output bands");
		MandatoryOff("outindexes");
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
		m_vector = DispatchZoneVectorImageFilterType::New();
		const std::vector<unsigned int> l_out_vals = vns::Utilities::StringListAsUnsigned(this->GetParameterStringList("outvals"));
		std::vector<unsigned char> l_out_c;
		for (unsigned int j = 0; j < l_out_vals.size();j++)
			l_out_c.push_back(static_cast<unsigned char>(l_out_vals[j]));
		const FloatVectorImageListType* l_in_images = this->GetParameterImageList("il");
		for (unsigned int i =0 ; i<l_in_images->Size();i++)
		{
			m_vector->SetInput(i,l_in_images->GetNthElement(i));
		}
		m_vector->SetNumberOfComponentsPerPixel(this->GetParameterInt("nbcomp"));
		m_vector->SetImageOutputValueList(l_out_c);
		//Construct the bitweights for detectors/band conversion
		std::vector<std::vector<unsigned char> > m_BitWeights;
		std::vector<unsigned char> m_BitWeight;
		std::vector<unsigned int> l_out_idx;
		if(HasValue("outindexes"))
		{
			l_out_idx= vns::Utilities::StringListAsUnsigned(this->GetParameterStringList("outindexes"));
			if (l_out_idx.size() != this->GetParameterInt("nbcomp")*l_in_images->Size())
			{
				vnsExceptionBusinessMacro("Not the correction number of bit->band indexes, must be nbcomp*nbImages");
			}
			for (unsigned int j = 0; j < l_in_images->Size(); j++)
			{
				for (unsigned int i = 0; i < this->GetParameterInt("nbcomp"); i++)
				{
					if (l_out_idx[j*this->GetParameterInt("nbcomp")+i] == 0)
					{
						m_BitWeight.push_back(0);
					}
					else
					{
						m_BitWeight.push_back(
							static_cast<unsigned char> (
									vcl_pow(2., static_cast<double> (l_out_idx[j*this->GetParameterInt("nbcomp")+i]-1))));
					}
				}
				m_BitWeights.push_back(m_BitWeight);
				m_BitWeight.clear();
			}
		}
		else
		{
			for (unsigned int i = 0; i < this->GetParameterInt("nbcomp"); i++)
			{
				m_BitWeight.push_back(static_cast<unsigned char> (vcl_pow(2., static_cast<double> (i))));
			}
			for (unsigned int i = 0; i < l_in_images->Size(); i++)
			{
				m_BitWeights.push_back(m_BitWeight);
			}
		}
		m_vector->SetBitWeights(m_BitWeights);
		SetParameterOutputImagePixelType("out",ImagePixelType_uint8);
		SetParameterOutputImage<MaskVectorImageType>("out",m_vector->GetOutput());
	}


	/** Filters declaration */
	DispatchZoneVectorImageFilterPointer m_vector;
};

}
}

OTB_APPLICATION_EXPORT(vns::Wrapper::DispatchZonesToVector)
