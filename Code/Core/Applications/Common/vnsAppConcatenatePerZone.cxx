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
 * VERSION : 1-0-0 : <TypeFT> : <NumFT> : 15 juil. 2010 : Creation
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id$
 *                                                                                                          *
 ************************************************************************************************************/
#include "otbWrapperApplication.h"
#include "otbWrapperApplicationFactory.h"
#include "vnsConcatenatePerZoneVectorImageFilter.h"
#include <string>

namespace vns
{

namespace Wrapper
{

using namespace otb::Wrapper;

class ConcatenatePerZone : public Application
{
public:
	/** Standard class typedefs. */
	typedef ConcatenatePerZone             Self;
	typedef otb::Wrapper::Application     Superclass;
	typedef itk::SmartPointer<Self>       Pointer;
	typedef itk::SmartPointer<const Self> ConstPointer;

	/** Standard macro */
	itkNewMacro(Self);

	itkTypeMacro(ConcatenatePerZone, otb::Wrapper::Application);

	/** Some convenient typedefs. */
	typedef FloatImageType ImageType;
	typedef ImageType::ConstPointer ImageConstPointer;
	typedef ImageType::Pointer ImagePointer;
	typedef ImageType::PixelType PixelType;
	typedef FloatVectorImageType                      VectorImageType;
	typedef VectorImageType::ConstPointer     VectorImageConstPointer;
	typedef VectorImageType::Pointer          VectorImagePointer;
    typedef ImageType::SizeType SizeType;
    typedef ImageType::SpacingType SpacingType;
    typedef ImageType::PointType PointType;
    typedef UInt8ImageType MaskType;
    typedef MaskType::Pointer MaskPointer;
    typedef MaskType::ConstPointer MaskConstPointer;

    typedef ConcatenatePerZoneVectorImageFilter<VectorImageType, MaskType, VectorImageType> ConcatenatePerZoneVectorImageFilterType;
    typedef ConcatenatePerZoneVectorImageFilterType::Pointer ConcatenatePerZoneVectorImageFilterPointer;
private:
	void DoInit()
	{
		SetName("ConcatenatePerZone");
		SetDescription("ConcatenatePerZone algo.");
		Loggers::GetInstance()->Initialize(GetName());
		// Documentation
		SetDocLongDescription("This application concatenate images according to the zone in the mask");
		SetDocLimitations("None");
		SetDocAuthors("MAJA-Team");
		SetDocSeeAlso("MAJA Doc");

		AddDocTag("Statistics");

		AddParameter(ParameterType_InputImage,  "mask",   "Zone mask");
		SetParameterDescription("mask", "Image used as mask of zone");
		AddParameter(ParameterType_InputImageList,  "il",   "Image Files");
		SetParameterDescription("il", "Image files to concatenates");
		AddParameter(ParameterType_StringList,  "zonelist",   "List of zone");
		SetParameterDescription("zonelist", "List of zone in the order of image list");

		AddParameter(ParameterType_OutputImage, "out", "image");
		SetParameterDescription("out","output image");

		AddRAMParameter("ram");
		SetDefaultParameterInt("ram",2048);

	}

	void DoUpdateParameters()
	{

	}


	void DoExecute()
	{
		// Init filters
		m_ConcatenatePerZoneFilter = ConcatenatePerZoneVectorImageFilterType::New();
	    //Get Params
		const Utilities::ListOfUIntegers  listOfZone =
							vns::Utilities::StringListAsUnsigned(GetParameterStringList("zonelist"));
		// Get input image pointers
		FloatVectorImageListType::Pointer inList = GetParameterImageList("il");
		MaskPointer l_mask = GetParameterUInt8Image("mask");
		// Concatenate all the images
		m_ConcatenatePerZoneFilter->SetInputList(inList);
		m_ConcatenatePerZoneFilter->SetInputMask(l_mask);
		m_ConcatenatePerZoneFilter->SetListOfZones(listOfZone);

		//Set Output
		SetParameterOutputImage<VectorImageType>("out",m_ConcatenatePerZoneFilter->GetOutput());

	}


	/** Filters declaration */
	ConcatenatePerZoneVectorImageFilterPointer m_ConcatenatePerZoneFilter;
};

}
}

OTB_APPLICATION_EXPORT(vns::Wrapper::ConcatenatePerZone)
