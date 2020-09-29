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
#include "vnsListOfStringToImageSource.h"
#include "vnsSimpleAngleXMLFileHandler.h"
#include "vnsZenithAndAzimuthAnglesToGridFilter.h"
#include "vnsResamplerHelper.h"
#include <string>

namespace vns
{

namespace Wrapper
{

using namespace otb::Wrapper;

class AngleListToImage : public Application
{
public:
	/** Standard class typedefs. */
	typedef AngleListToImage             Self;
	typedef otb::Wrapper::Application     Superclass;
	typedef itk::SmartPointer<Self>       Pointer;
	typedef itk::SmartPointer<const Self> ConstPointer;

	/** Standard macro */
	itkNewMacro(Self);

	itkTypeMacro(AngleListToImage, otb::Wrapper::Application);

	/** Some convenient typedefs. */
	typedef DoubleImageType ImageType;
	typedef ImageType::ConstPointer ImageConstPointer;
	typedef ImageType::Pointer ImagePointer;
	typedef ImageType::PixelType PixelType;
	typedef DoubleVectorImageType                      VectorImageType;
	typedef VectorImageType::ConstPointer     VectorImageConstPointer;
	typedef VectorImageType::Pointer          VectorImagePointer;
    typedef ImageType::SizeType SizeType;
    typedef ImageType::SpacingType SpacingType;
    typedef ImageType::PointType PointType;

    typedef ListOfStringToImageSource<DoubleImageType> ListOfStringToImageSourceType;
    typedef ListOfStringToImageSourceType::Pointer ListOfStringToImageSourcePointer;
    typedef ZenithAndAzimuthAnglesToGridFilter<ImageType, ImageType, VectorImageType> ZenithAndAzimuthAnglesToGridFilterType;
    typedef ZenithAndAzimuthAnglesToGridFilterType::Pointer ZenithAndAzimuthAnglesToGridFilterPointer;
private:
	void DoInit()
	{
		SetName("AngleListToImage");
		SetDescription("AngleListToImage algo.");
		Loggers::GetInstance()->Initialize(GetName());
		// Documentation
		SetDocLongDescription("This application compute angle grid from angle list");
		SetDocLimitations("None");
		SetDocAuthors("MAJA-Team");
		SetDocSeeAlso("MAJA Doc");

		AddDocTag("Statistics");

		AddParameter(ParameterType_InputImage,  "dtm",   "DTM");
		SetParameterDescription("dtm", "Image used as reference grid");
		AddParameter(ParameterType_InputFilename,  "angles",   "XML Angle Files");
		SetParameterDescription("angles", "XML Angle Files");
		AddParameter(ParameterType_Bool,"extrapol","extrapol");


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
	    m_zenAnglesImageSource = ListOfStringToImageSourceType::New();
	    m_aziAnglesImageSource = ListOfStringToImageSourceType::New();
	    m_AnglesToGridFilter = ZenithAndAzimuthAnglesToGridFilterType::New();

	    //Get Params
	    const std::string l_AngleFilename = GetParameterString("angles");
	    SimpleAngleXMLFileHandler::Pointer l_XMLHandler = SimpleAngleXMLFileHandler::New();
	    // Load the XML file and check with the schema
	    l_XMLHandler->LoadFile(l_AngleFilename);
	    SpacingType l_AngleSpacing;
	    l_AngleSpacing[0] = l_XMLHandler->GetAngleSpacingCol();
	    // ATTENTION : * -1 for compatibility negative spacing
	    l_AngleSpacing[1] = -1 * l_XMLHandler->GetAngleSpacingRow();
	    const bool l_extrapolate = IsParameterEnabled("extrapol");

		// Get input image pointers
		ImagePointer l_dtmPtr = GetParameterDoubleImage("dtm");
		l_dtmPtr->UpdateOutputInformation();
		Area l_outArea;
		l_outArea.Origin = l_dtmPtr->GetOrigin();
		l_outArea.Spacing = l_dtmPtr->GetSignedSpacing();
		l_outArea.Size = l_dtmPtr->GetLargestPossibleRegion().GetSize();
		const std::string & projectionRef = l_dtmPtr->GetProjectionRef();
		const PointType l_SunAngleResolutionOrigin = ResamplerHelper::ComputeOrigin(
				l_outArea.Origin, l_dtmPtr->GetSignedSpacing(), l_AngleSpacing,
				ResamplerHelper::RB_MODE_UPPER_LEFT_CORNER_PIXEL);


		vnsLogDebugMacro("Angle Resolution Origin computed in 5000 resolution:  "<<l_SunAngleResolutionOrigin);

		try
		{
			m_zenAnglesImageSource->SetListOfString(l_XMLHandler->GetAngleZenithList(), l_extrapolate /* ActivateDataExtrapolation */);
		}
		catch (...)
		{
			vnsExceptionPluginBaseMacro("The size of the zenital solar angles grid of the product is incorrect!")
		}
		m_zenAnglesImageSource->SetOrigin(l_SunAngleResolutionOrigin);
		m_zenAnglesImageSource->SetSpacing(l_AngleSpacing);
		m_zenAnglesImageSource->SetProjectionRef(projectionRef);

		try
		{
			m_aziAnglesImageSource->SetListOfString(l_XMLHandler->GetAngleAzimuthList(), l_extrapolate /* ActivateDataExtrapolation */);
		}
		catch (...)
		{
			vnsExceptionPluginBaseMacro("The size of the azimuth solar angles grid of the product is incorrect!")
		}
		m_aziAnglesImageSource->SetOrigin(l_SunAngleResolutionOrigin);
		m_aziAnglesImageSource->SetSpacing(l_AngleSpacing);
		m_aziAnglesImageSource->SetProjectionRef(projectionRef);

		// Convert angle values (zenith, azimuth) in grid values (DS.X, DS.Y)
		m_AnglesToGridFilter->SetInput1(m_zenAnglesImageSource->GetOutput());
		m_AnglesToGridFilter->SetInput2(m_aziAnglesImageSource->GetOutput());
		SetParameterOutputImage<VectorImageType>("out",m_AnglesToGridFilter->GetOutput());

	}


	/** Filters declaration */
    ListOfStringToImageSourcePointer m_zenAnglesImageSource;
    ListOfStringToImageSourcePointer m_aziAnglesImageSource;
    ZenithAndAzimuthAnglesToGridFilterPointer m_AnglesToGridFilter;
};

}
}

OTB_APPLICATION_EXPORT(vns::Wrapper::AngleListToImage)
