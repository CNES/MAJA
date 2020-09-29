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
#include "vnsConstantImageSource.h"
#include "vnsMacro.h"
#include "vnsLoggers.h"
#include <string>

namespace vns
{
namespace Wrapper
{

using namespace otb::Wrapper;

class ConstantImage : public Application
{
public:
	/** Standard class typedefs. */
	typedef ConstantImage                 Self;
	typedef otb::Wrapper::Application     Superclass;
	typedef itk::SmartPointer<Self>       Pointer;
	typedef itk::SmartPointer<const Self> ConstPointer;

	/** Standard macro */
	itkNewMacro(Self);

	itkTypeMacro(ConstantImage, otb::Wrapper::Application);

	/** Some convenient typedefs. */
	typedef DoubleImageType ImageType;
	typedef ImageType::ConstPointer ImageConstPointer;
	typedef ImageType::Pointer ImagePointer;
	typedef ImageType::PixelType PixelType;

	typedef ConstantImageSource<ImageType> RealConstantImageSourceFilterType;
    typedef typename RealConstantImageSourceFilterType::Pointer RealConstantImageSourceFilterPointer;

private:
	void DoInit()
	{
		SetName("ConstantImage");
		SetDescription("ConstantImage algo.");
		Loggers::GetInstance()->Initialize(GetName());
		// Documentation
		SetDocLongDescription("This applicatio create a constant image");
		SetDocLimitations("None");
		SetDocAuthors("MAJA-Team");
		SetDocSeeAlso("MAJA Doc");

		AddDocTag("Statistics");

		AddParameter(ParameterType_InputImage,  "dtm",   "DTM");
		SetParameterDescription("dtm", "Image used as reference grid");
		AddParameter(ParameterType_Float,  "value", "constant value");
		SetParameterDescription("value", "constant value of the output");
		AddRAMParameter("ram");
		SetDefaultParameterInt("ram",2048);

		AddParameter(ParameterType_OutputImage, "out", "image");
		SetParameterDescription("out","output image");

	}

	void DoUpdateParameters()
	{
	}


	void DoExecute()
	{
		vns::Loggers::GetInstance()->Initialize(GetName());
		// Init filters
		m_ConstImageFilter = RealConstantImageSourceFilterType::New();
		m_ConstImageFilter->SetReleaseDataBeforeUpdateFlag(true);
		m_ConstImageFilter->SetReleaseDataFlag(true);
		vnsLogDebugMacro("Number of threads : "<<itk::MultiThreader::GetGlobalDefaultNumberOfThreads());
		//Get dtm for reference size
		ImagePointer l_dtmPtr = GetParameterDoubleImage("dtm");
		l_dtmPtr->UpdateOutputInformation();
		//Set Parameters

		//Output area
		Area l_outArea;
		l_outArea.Origin = l_dtmPtr->GetOrigin();
		l_outArea.Spacing = l_dtmPtr->GetSignedSpacing();
		l_outArea.Size = l_dtmPtr->GetLargestPossibleRegion().GetSize();
        const double constant_val = GetParameterFloat("value");

        m_ConstImageFilter->SetSize(l_outArea.Size);
        m_ConstImageFilter->SetSpacing(l_outArea.Spacing);
        m_ConstImageFilter->SetOrigin(l_outArea.Origin);

        m_ConstImageFilter->SetConstantValue(constant_val);
        m_ConstImageFilter->SetProjectionRef(l_dtmPtr->GetProjectionRef());

        SetParameterOutputImage<ImageType>("out",m_ConstImageFilter->GetOutput());
	}


	/** Filters declaration */
	RealConstantImageSourceFilterPointer m_ConstImageFilter;

};

}
}

OTB_APPLICATION_EXPORT(vns::Wrapper::ConstantImage)
