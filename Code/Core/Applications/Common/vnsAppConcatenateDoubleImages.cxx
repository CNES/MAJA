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

#include "otbImageListToVectorImageFilter.h"
#include "otbMultiToMonoChannelExtractROI.h"
#include "otbImageList.h"
#include "vnsMacro.h"

namespace vns
{
namespace Wrapper
{

using namespace otb::Wrapper;

class ConcatenateDoubleImages : public Application
{
public:
  /** Standard class typedefs. */
  typedef ConcatenateDoubleImages             Self;
  typedef Application                   Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Standard macro */
  itkNewMacro(Self);

  itkTypeMacro(ConcatenateDoubleImages, otb::Application);

  /** Filters typedef */
  typedef otb::ImageList<DoubleImageType> ImageListType;
  typedef otb::ImageListToVectorImageFilter<ImageListType, DoubleVectorImageType>                               ListConcatenerFilterType;
  typedef otb::MultiToMonoChannelExtractROI<DoubleVectorImageType::InternalPixelType, DoubleImageType::PixelType> ExtractROIFilterType;
  typedef otb::ObjectList<ExtractROIFilterType> ExtractROIFilterListType;

private:
  void DoInit() override
  {
    SetName("ConcatenateImages");
    SetDescription("Concatenate a list of images of the same size into a single multi-channel image.");

    // Documentation
    SetDocLongDescription(
        "Concatenate a list of images of the same size into a single multi-channel image. "
        "It reads the input image list (single or multi-channel) "
        "and generates a single multi-channel image. The channel order is the same as the list.");
    SetDocLimitations("All input images must have the same size.");
    SetDocAuthors("OTB-Team");
    SetDocSeeAlso("Rescale application, DynamicConvert, SplitImage");

    AddDocTag(Tags::Manip);
    AddDocTag("Concatenation");
    AddDocTag("Multi-channel");

    AddParameter(ParameterType_InputImageList, "il", "Input images list");
    SetParameterDescription("il", "The list of images to concatenate, must have the same size.");

    AddParameter(ParameterType_OutputImage, "out", "Output Image");
    SetParameterOutputImagePixelType("out", ImagePixelType_double);
    SetParameterDescription("out", "The concatenated output image.");

    AddRAMParameter();

    // Doc example parameter settings
    SetDocExampleParameterValue("il", "GomaAvant.png GomaApres.png");
    SetDocExampleParameterValue("out", "otbConcatenateImages.tif");

    SetOfficialDocLink();
  }

  void DoUpdateParameters() override
  {
    // Nothing to do here for the parameters : all are independent
  }

  void DoExecute() override
  {
    m_Concatener    = ListConcatenerFilterType::New();
    m_ExtractorList = ExtractROIFilterListType::New();
    m_ImageList     = ImageListType::New();
    ImageBaseType* imgBase = GetParameterImageBase("il", 0);
    if (GetNumberOfElementsInParameterInputImageList("il") == 0)
    {
      itkExceptionMacro("No input Image set...");
    }
    // Split each input vector image into image
    // and generate an mono channel image list
    for (unsigned int i = 0; i < GetNumberOfElementsInParameterInputImageList("il"); i++)
    {
    	ImageBaseType* imgBase = GetParameterImageBase("il", i);
    	imgBase->UpdateOutputInformation();
    	// Guess the image type
    	std::string className(imgBase->GetNameOfClass());
    	if (className == "VectorImage") {
    		DoubleVectorImageType * vectIm = dynamic_cast<DoubleVectorImageType*>(imgBase);
    		if (vectIm)
    		{
    			for (unsigned int j = 0; j < vectIm->GetNumberOfComponentsPerPixel(); j++)
    			{
    				ExtractROIFilterType::Pointer extractor = ExtractROIFilterType::New();
    				extractor->SetInput(vectIm);
    				extractor->SetChannel(j + 1);
    				extractor->UpdateOutputInformation();
    				m_ExtractorList->PushBack(extractor);
    				m_ImageList->PushBack(extractor->GetOutput());
    			}
    		} else {
    			vnsExceptionDataMacro("Wrong type of input image, only double supported")
    		}
    	} else {
    		DoubleImageType * double_Im = dynamic_cast<DoubleImageType*>(imgBase);
    		if (double_Im)
    		{
    			m_ImageList->PushBack(double_Im);
    		}
    		else {
    			vnsExceptionDataMacro("Wrong type of input image, only double supported")
    		}
    	}
    }

    m_Concatener->SetInput(m_ImageList);
    SetParameterOutputImage("out", m_Concatener->GetOutput());
  }

private:
  ListConcatenerFilterType::Pointer m_Concatener;
  ExtractROIFilterListType::Pointer m_ExtractorList;
  ImageListType::Pointer            m_ImageList;
};
}
}

OTB_APPLICATION_EXPORT(vns::Wrapper::ConcatenateDoubleImages)
