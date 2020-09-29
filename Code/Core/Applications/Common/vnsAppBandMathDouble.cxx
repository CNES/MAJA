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
 * VERSION : 1-0-0 : <TypeFT> : <NumFT> : 15 juil. 2010 : Creation
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id$
 *                                                                                                          *
 ************************************************************************************************************/

#include "otbWrapperApplication.h"
#include "otbWrapperApplicationFactory.h"

#include "otbBandMathImageFilter.h"
#include "otbMultiToMonoChannelExtractROI.h"
#include "otbObjectList.h"
#include "otbClampImageFilter.h"
#include "vnsMacro.h"

namespace vns
{

namespace Wrapper
{

#define CLAMP_BANDMATH_IMAGE_IF(Out, In, image_base)       \
  {                                               \
    In* in_image = dynamic_cast<In*>(image_base); \
    if (in_image)                                 \
		outIm = Cast<In, Out>(in_image);        \
  }

#define CLAMP_BANDMATH_IMAGE_VECTOR_BASE(T, image_base)                   \
    T* outIm =nullptr;                                                    \
    CLAMP_BANDMATH_IMAGE_IF(T, UInt8VectorImageType, image_base);         \
    CLAMP_BANDMATH_IMAGE_IF(T, Int16VectorImageType, image_base);         \
    CLAMP_BANDMATH_IMAGE_IF(T, UInt16VectorImageType, image_base);        \
    CLAMP_BANDMATH_IMAGE_IF(T, FloatVectorImageType, image_base);         \
    CLAMP_BANDMATH_IMAGE_IF(T, DoubleVectorImageType, image_base);        \

#define CLAMP_BANDMATH_IMAGE_BASE(T, image_base)                          \
  	T* outIm =nullptr;                                                     \
	CLAMP_BANDMATH_IMAGE_IF(T, UInt8ImageType, image_base);               \
    CLAMP_BANDMATH_IMAGE_IF(T, Int16ImageType, image_base);               \
    CLAMP_BANDMATH_IMAGE_IF(T, UInt16ImageType, image_base);              \
    CLAMP_BANDMATH_IMAGE_IF(T, FloatImageType, image_base);               \
    CLAMP_BANDMATH_IMAGE_IF(T, DoubleImageType, image_base);              \




using namespace otb::Wrapper;

class BandMathDouble : public Application
{

public:
  /** Standard class typedefs. */
  typedef BandMathDouble                Self;
  typedef Application                   Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Standard macro */
  itkNewMacro(Self);

  itkTypeMacro(BandMathDouble, otb::Application);

  typedef otb::MultiToMonoChannelExtractROI<DoubleVectorImageType::InternalPixelType, DoubleImageType::PixelType> ExtractROIFilterType;
  typedef otb::ObjectList<ExtractROIFilterType>    ExtractROIFilterListType;
  typedef otb::BandMathImageFilter<DoubleImageType> BandMathImageFilterType;


private:
  void DoInit() override
  {
    SetName("BandMathDouble");

    SetDescription(
        "Outputs a monoband image which is the result of a mathematical operation "
        "on several multi-band images.");


    SetDocLongDescription(
        "This application performs a mathematical operation on several multi-band "
        "images and outputs the result into a monoband image. The given expression"
        " is computed at each pixel position. Evaluation of the "
        "mathematical formula is done by the muParser library.\n\n"

        "The formula can be written using:\n\n"
        "* numerical values ( 2.3, -5, 3.1e4, ...)\n"
        "* variables containing pixel values (e.g. ``im2b3`` is the pixel value"
        " in 2nd image, 3rd band)\n"
        "* binary operators:\n\n"

        "  * ``+`` addition, ``-`` subtraction, ``*`` multiplication, ``/`` division\n"
        "  * ``^`` raise x to the power of y\n"
        "  * ``<`` less than, ``>`` greater than, ``<=`` less or equal, ``>=`` greater or equal\n"
        "  * ``==`` equal, ``!=`` not equal\n"
#ifdef OTB_MUPARSER_HAS_CXX_LOGICAL_OPERATORS
        "  * ``||`` logical or, ``&&`` logical and\n"
        "  * if-then-else operator: ``(condition ? value_true : value_false)``\n"
#else
        "  * ``or`` logical or, ``and`` logical and\n"
        "* if-then-else operator: ``if(condition;value_true;value_false)``\n"
#endif
        "* functions: ``exp()``, ``log()``, ``sin()``, ``cos()``, ``min()``, ``max()``, ...\n\n"

        "The full list of features and operators is available on the muParser website [1].");

    SetDocLimitations("None");
    SetDocAuthors("OTB-Team");
    SetDocSeeAlso("[1] http://beltoforion.de/article.php?a=muparser");
    AddDocTag(Tags::Manip);

    AddParameter(ParameterType_InputImageList, "il", "Input image list");
    SetParameterDescription("il", "Image list of operands to the mathematical expression.");

    AddParameter(ParameterType_OutputImage, "out", "Output Image");
    SetParameterDescription("out",
                            "Output image which is the result of the mathematical expressions on input"
                            " image list operands.");

    AddParameter(ParameterType_String, "exp", "Expression");
    SetParameterDescription("exp", "The muParser mathematical expression to apply on input images.");

    AddRAMParameter();

    // Doc example parameter settings
    SetDocExampleParameterValue("il", "verySmallFSATSW_r.tif verySmallFSATSW_nir.tif verySmallFSATSW.tif");
    SetDocExampleParameterValue("out", "apTvUtBandMathOutput.tif");
    SetDocExampleParameterValue("exp", "\"cos( im1b1 ) > cos( im2b1 ) ? im3b1 : im3b2\"");

    SetOfficialDocLink();
  }

  void DoUpdateParameters() override
  {
    // Check if the expression is correctly set
    if (HasValue("il"))
    {
      LiveCheck();
    }
  }

  template <typename TInputImage,typename TOutputImage>
  TOutputImage* Cast(TInputImage* image)
  {
	  /** Input clamping */
	  using InputClampImageFilter = otb::ClampImageFilter<TInputImage, TOutputImage>;

	  /** Input-image clamp filter. */
	  typename InputClampImageFilter::Pointer icif = InputClampImageFilter::New();
	  icif->SetInput(image);
	  icif->UpdateOutputInformation();

	  m_InputCasters->PushBack(icif);

	  return icif->GetOutput();
  }


  void LiveCheck()
  {
    // Initialize a bandMath Filter first
    m_ChannelExtractorList = ExtractROIFilterListType::New();
    m_Filter               = BandMathImageFilterType::New();
    m_InputCasters         = otb::ObjectList<itk::ProcessObject>::New();

    unsigned int                      nbInputs = GetNumberOfElementsInParameterInputImageList("il");
    unsigned int                      imageId = 0, bandId = 0;

    for (unsigned int i = 0; i < GetNumberOfElementsInParameterInputImageList("il"); i++)
    {
    	ImageBaseType* imgBase = GetParameterImageBase("il", i);
    	imgBase->UpdateOutputInformation();

    	// Guess the image type
    	std::string className(imgBase->GetNameOfClass());
    	if (className == "VectorImage") {
    		CLAMP_BANDMATH_IMAGE_VECTOR_BASE(DoubleVectorImageType,imgBase);
    		if (outIm)
    		{
    			for (unsigned int j = 0; j < outIm->GetNumberOfComponentsPerPixel(); j++)
    			{
    				std::ostringstream tmpParserVarName;
    				tmpParserVarName << "im" << imageId + 1 << "b" << j + 1;

    				m_ExtractROIFilter = ExtractROIFilterType::New();
    				m_ExtractROIFilter->SetInput(outIm);
    				m_ExtractROIFilter->SetChannel(j + 1);
    				m_ExtractROIFilter->GetOutput()->UpdateOutputInformation();
    				m_ChannelExtractorList->PushBack(m_ExtractROIFilter);
    				m_Filter->SetNthInput(bandId, m_ChannelExtractorList->Back()->GetOutput(), tmpParserVarName.str());
    				bandId++;
    			}
    		} else {
    			vnsExceptionDataMacro("Wrong type of input image, only double vector supported")
    		}
    	} else {
    		CLAMP_BANDMATH_IMAGE_BASE(DoubleImageType,imgBase);
    		if (outIm)
    		{
    			std::ostringstream tmpParserVarName;
    			tmpParserVarName << "im" << imageId + 1 << "b" << 1;
    			m_Filter->SetNthInput(bandId, outIm, tmpParserVarName.str());
    			bandId++;
    		}
    		else {
    			vnsExceptionDataMacro("Wrong type of input image, only double supported")
    		}
    	}
    	imageId++;
    }

    otb::Parser::Pointer dummyParser = otb::Parser::New();
    std::vector<double>  dummyVars;
    std::string          success = "The expression is Valid";
    SetParameterDescription("exp", success);
    std::ostringstream failure;

    if (HasValue("exp"))
    {
      // Setup the dummy parser
      for (unsigned int i = 0; i < bandId; i++)
      {
        dummyVars.push_back(1.);
        dummyParser->DefineVar(m_Filter->GetNthInputName(i), &(dummyVars.at(i)));
      }
      dummyParser->SetExpr(GetParameterString("exp"));

      // Check the expression
      try
      {
        dummyParser->Eval();
      }
      catch (itk::ExceptionObject& err)
      {
        // Change the parameter description to be able to have the
        // parser errors in the tooltip
        SetParameterDescription("exp", err.GetDescription());
      }
    }
  }

  void DoExecute() override
  {
    // checking the input images list validity
    const unsigned int nbImages = GetNumberOfElementsInParameterInputImageList("il");

    if (nbImages == 0)
    {
      itkExceptionMacro("No input Image set...; please set at least one input image");
    }

    m_ChannelExtractorList = ExtractROIFilterListType::New();
    m_Filter               = BandMathImageFilterType::New();
    m_InputCasters         = otb::ObjectList<itk::ProcessObject>::New();

    unsigned int bandId  = 0;
    unsigned int imageId = 0;

    for (unsigned int i = 0; i < GetNumberOfElementsInParameterInputImageList("il"); i++)
    {
    	ImageBaseType* imgBase = GetParameterImageBase("il", i);
    	imgBase->UpdateOutputInformation();
    	otbAppLogINFO(<< "Image #" << i + 1 << " has " << imgBase->GetNumberOfComponentsPerPixel() << " components" << std::endl);
    	// Guess the image type
    	std::string className(imgBase->GetNameOfClass());
    	if (className == "VectorImage") {
    		CLAMP_BANDMATH_IMAGE_VECTOR_BASE(DoubleVectorImageType,imgBase);
    		if (outIm)
    		{
    			for (unsigned int j = 0; j < outIm->GetNumberOfComponentsPerPixel(); j++)
    			{
    				std::ostringstream tmpParserVarName;
    				tmpParserVarName << "im" << imageId + 1 << "b" << j + 1;
    				m_ExtractROIFilter = ExtractROIFilterType::New();
    				m_ExtractROIFilter->SetInput(outIm);
    				m_ExtractROIFilter->SetChannel(j + 1);
    				m_ExtractROIFilter->GetOutput()->UpdateOutputInformation();
    				m_ChannelExtractorList->PushBack(m_ExtractROIFilter);
    				m_Filter->SetNthInput(bandId, m_ChannelExtractorList->Back()->GetOutput(), tmpParserVarName.str());
    				bandId++;
    			}
    		} else {
    			vnsExceptionDataMacro("Wrong type of input image, only double vector supported")
    		}
    	} else {
    		CLAMP_BANDMATH_IMAGE_BASE(DoubleImageType,imgBase);
    		if (outIm)
    		{
    			std::ostringstream tmpParserVarName;
    			tmpParserVarName << "im" << imageId + 1 << "b" << 1;
    			m_Filter->SetNthInput(bandId, outIm, tmpParserVarName.str());
    			bandId++;
    		}
    		else {
    			vnsExceptionDataMacro("Wrong type of input image, only double supported")
    		}
    	}
    	imageId++;
    }

    m_Filter->SetExpression(GetParameterString("exp"));

    // Set the output image
    SetParameterOutputImage("out", m_Filter->GetOutput());
  }

  ExtractROIFilterType::Pointer     m_ExtractROIFilter;
  ExtractROIFilterListType::Pointer m_ChannelExtractorList;
  otb::ObjectList<itk::ProcessObject>::Pointer m_InputCasters;
  BandMathImageFilterType::Pointer  m_Filter;
};

} // namespace Wrapper
} // namespace otb

OTB_APPLICATION_EXPORT(vns::Wrapper::BandMathDouble)
