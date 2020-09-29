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
#include "otbMultiChannelExtractROI.h"
#include "otbMultiToMonoChannelExtractROI.h"
#include "otbWrapperListViewParameter.h"
#include "vnsMacro.h"

#define CAST_AND_EXTRACT_VECTOR_IMAGE_BASE(Tin, Tout, image_base)     \
{                                                \
	Tin* img = dynamic_cast<Tin*>(image_base);         \
	\
	if (img)                                       \
	{                                              \
		DoExtractVector<Tin, Tout>(img); \
		return;                                      \
		\
	}                                              \
} \


#define CAST_AND_EXTRACT_IMAGE_BASE(Tin, Tout, image_base)     \
{                                                \
	Tin* img = dynamic_cast<Tin*>(image_base);         \
	\
	if (img)                                       \
	{                                              \
		DoExtract<Tin, Tout>(img); \
		return;                                      \
		\
	}                                              \
} \


namespace vns
{
namespace Wrapper
{
using namespace otb::Wrapper;

class ExtractChannels : public Application
{
public:
  /** Standard class typedefs. */
  typedef ExtractChannels               Self;
  typedef Application                   Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Standard macro */
  itkNewMacro(Self);

  itkTypeMacro(ExtractChannels, otb::Application);


protected:
  ExtractChannels()
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
    AddParameter(ParameterType_ListView, "cl", "Output Image channels");
    SetParameterDescription("cl", "Channels to write in the output image.");

    AddRAMParameter();

    SetOfficialDocLink();
  }

  void DoUpdateParameters() override
  {
    if (HasValue("in"))
    {
      ImageBaseType* inImage = GetParameterImageBase("in",0);
      unsigned int       nbComponents = inImage->GetNumberOfComponentsPerPixel();
      ListViewParameter* clParam      = dynamic_cast<ListViewParameter*>(GetParameterByKey("cl"));
      // Update the values of the channels to be selected if nbComponents is changed
      if (clParam != nullptr && clParam->GetNbChoices() != nbComponents)
      {
        ClearChoices("cl");
        for (unsigned int idx = 0; idx < nbComponents; ++idx)
        {
          std::ostringstream key, item;
          key << "cl.channel" << idx + 1;
          item << "Channel" << idx + 1;
          AddChoice(key.str(), item.str());
        }
      }

    }

  }

  template <typename TInputImage, typename TOutputImage>
  void DoExtractVector(TInputImage* in)
  {
	  //MultiChannels
	  typedef otb::MultiChannelExtractROI<typename TInputImage::InternalPixelType, typename TOutputImage::InternalPixelType> ExtractROIFilterType;
	  typename ExtractROIFilterType::Pointer extractROIFilter = ExtractROIFilterType::New();
	  extractROIFilter->SetInput(in);
	  for (unsigned int idx = 0; idx < GetSelectedItems("cl").size(); ++idx)
	  {
		  extractROIFilter->SetChannel(GetSelectedItems("cl")[idx] + 1);
	  }
	  SetParameterOutputImage<TOutputImage>("out", extractROIFilter->GetOutput());
	  m_ExtractFilter = extractROIFilter;
  }


  template <typename TInputImage, typename TOutputImage>
  void DoExtract(TInputImage* in)
  {
	  //MonoChannel
	  typedef otb::MultiToMonoChannelExtractROI<typename TInputImage::InternalPixelType, typename TOutputImage::InternalPixelType> MonoChannelExtractROIFilterType;
	  typename MonoChannelExtractROIFilterType::Pointer extractROIFilter = MonoChannelExtractROIFilterType::New();
	  extractROIFilter->SetInput(in);
	  extractROIFilter->SetChannel(GetSelectedItems("cl")[0] + 1);
	  SetParameterOutputImage<TOutputImage>("out", extractROIFilter->GetOutput());
	  m_ExtractFilter = extractROIFilter;
  }



  void DoExecute() override
  {
	  // Get input image pointers
	  ImageBaseType* l_inPtr = GetParameterImageBase("in",0);
	  // Guess the image type
	  std::string className(l_inPtr->GetNameOfClass());
	  vnsLogDebugMacro("Resampling needed");
		if (className == "VectorImage") {
			if (GetSelectedItems("cl").size() == 1)
			{
				vnsLogDebugMacro("SingleImage");
				CAST_AND_EXTRACT_IMAGE_BASE(DoubleVectorImageType,DoubleImageType,l_inPtr);
				SetParameterOutputImagePixelType("out",ImagePixelType_float);
				CAST_AND_EXTRACT_IMAGE_BASE(FloatVectorImageType,FloatImageType,l_inPtr);
				SetParameterOutputImagePixelType("out",ImagePixelType_uint8);
				CAST_AND_EXTRACT_IMAGE_BASE(UInt8VectorImageType, UInt8ImageType,l_inPtr);
				SetParameterOutputImagePixelType("out",ImagePixelType_uint16);
				CAST_AND_EXTRACT_IMAGE_BASE(UInt16VectorImageType, UInt16ImageType,l_inPtr);
			} else {
				vnsLogDebugMacro("VectorImage");
				CAST_AND_EXTRACT_VECTOR_IMAGE_BASE(DoubleVectorImageType,DoubleVectorImageType,l_inPtr);
				SetParameterOutputImagePixelType("out",ImagePixelType_float);
				CAST_AND_EXTRACT_VECTOR_IMAGE_BASE(FloatVectorImageType,FloatVectorImageType,l_inPtr);
				SetParameterOutputImagePixelType("out",ImagePixelType_uint8);
				CAST_AND_EXTRACT_VECTOR_IMAGE_BASE(UInt8VectorImageType, UInt8VectorImageType,l_inPtr);
				SetParameterOutputImagePixelType("out",ImagePixelType_uint16);
				CAST_AND_EXTRACT_VECTOR_IMAGE_BASE(UInt16VectorImageType, UInt16VectorImageType,l_inPtr);
			}
		} else {
			vnsExceptionDataMacro("Unsuported image type : only vector supported");
		}
		vnsExceptionDataMacro("Unsuported image type");


  }

private:
  itk::ProcessObject::Pointer m_ExtractFilter;
};
}
}

OTB_APPLICATION_EXPORT(vns::Wrapper::ExtractChannels)
