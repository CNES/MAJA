/*
 * Copyright (C) 2005-2019 Centre National d'Etudes Spatiales (CNES)
 *
 * This file is part of Orfeo Toolbox
 *
 *     https://www.orfeo-toolbox.org/
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
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
#include "otbMultiToMonoChannelExtractROI.h"
#include "otbWrapperListViewParameter.h"
#include "vnsMacro.h"

namespace vns
{
namespace Wrapper
{
using namespace otb::Wrapper;

class ExtractOneChannel : public Application
{
public:
  /** Standard class typedefs. */
  typedef ExtractOneChannel               Self;
  typedef Application                   Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Standard macro */
  itkNewMacro(Self);

  itkTypeMacro(ExtractChannels, otb::Application);

  /** Filters typedef */
  typedef otb::MultiToMonoChannelExtractROI<DoubleVectorImageType::InternalPixelType, DoubleVectorImageType::InternalPixelType> ExtractROIFilterType;

  typedef ExtractROIFilterType::InputImageType ImageType;
  typedef ExtractROIFilterType::OutputImageType OutputImageType;

protected:
  ExtractOneChannel()
  {
  }

private:
  void DoInit() override
  {
    SetName("ExtractChannels");
    SetDescription("Extract channels defined by the user.");

    // Documentation
    SetDocLongDescription(
        "This application extracts a list of channels with "
        "user parameters. ");
    SetDocLimitations("None");
    SetDocAuthors("MAJA-OTB-Team");
    SetDocSeeAlso(" ");

    AddDocTag(Tags::Manip);

    // Set parameter input
    AddParameter(ParameterType_InputImage, "in", "Input Image");
    SetParameterDescription("in", "Image to be processed.");
    AddParameter(ParameterType_OutputImage, "out", "Output Image");
    SetParameterDescription("out", "Region of interest from the input image");
    SetParameterOutputImagePixelType("out",ImagePixelType_double);

    // Channelist Parameters
    AddParameter(ParameterType_Int, "cl", "Output Image channel");
    SetParameterDescription("cl", "Channel to write in the output image.");

    AddRAMParameter();

    SetOfficialDocLink();
  }

  void DoUpdateParameters() override
  {
  }

  void DoExecute() override
  {
    ImageType* inImage = GetParameterDoubleVectorImage("in");
    inImage->UpdateOutputInformation();

    m_extractROIFilter = ExtractROIFilterType::New();
    m_extractROIFilter->SetInput(inImage);

    const unsigned int channel_idx = GetParameterInt("cl");
    if (channel_idx == 0)
    {
    	vnsExceptionDataMacro("Channels starts at 1");
    }
    if (channel_idx <= inImage->GetNumberOfComponentsPerPixel())
    {
    	  m_extractROIFilter->SetChannel(channel_idx);
    } else {
    	vnsExceptionDataMacro("The requested channel is more than the number of components of input");
    }

    SetParameterOutputImage<OutputImageType>("out", m_extractROIFilter->GetOutput());

  }
  ExtractROIFilterType::Pointer m_extractROIFilter;

};
}
}

OTB_APPLICATION_EXPORT(vns::Wrapper::ExtractOneChannel)
