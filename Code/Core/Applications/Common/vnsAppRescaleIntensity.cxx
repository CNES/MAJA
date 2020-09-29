/*
 * Copyright (C) 2005-2017 Centre National d'Etudes Spatiales (CNES)
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
#include "itksys/SystemTools.hxx"


#include "otbStreamingMinMaxVectorImageFilter.h"
#include "otbVectorRescaleIntensityImageFilter.h"
#include "otbExtendedFilenameToWriterOptions.h"

#include "otbImageFileWriter.h"

#include "vnsMacro.h"
#include "vnsUtilities.h"
#include <string>


//#include "vnsPadAndResampleImageFilter.h"
//#include "vnsBinaryThresholdVectorImageFilter.h"
//#include "otbVectorImageToImageListFilter.h"
//#include "vnsBinaryThresholdVectorImageFilter.h"
//#include <string>




namespace vns
{

namespace Wrapper
{

using namespace otb::Wrapper;

class RescaleIntensity : public Application
{
public:
  /** Standard class typedefs. */
  typedef RescaleIntensity                       Self;
  typedef Application                   Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Standard macro */
  itkNewMacro(Self);

  itkTypeMacro(RescaleIntensity, otb::Application);

  typedef UInt16VectorImageType MaskVectorImageType ;

  /** Filters typedef */
  typedef otb::StreamingMinMaxVectorImageFilter<FloatVectorImageType>  MinMaxFilterType;
  typedef otb::VectorRescaleIntensityImageFilter<FloatVectorImageType> RescaleImageFilterType;

  typedef otb::ImageFileWriter<FloatVectorImageType> ImageFileWriterType;

  typedef FloatVectorImageType::PixelType InputPixelType;


private:
  void DoInit() ITK_OVERRIDE
  {
    SetName("RescaleIntensity");
    SetDescription("Rescale the image between two given values.");
    Loggers::GetInstance()->Initialize(GetName());
    SetDocLongDescription("This application scales the given image pixel intensity between two given values.\n"
                                  "By default min (resp. max) value is set to 0 (resp. 255).\n"
                                  "Input minimum and maximum values is automatically computed for all image bands.");
    SetDocLimitations("None");
    SetDocAuthors("OTB-Team");
    SetDocSeeAlso(" ");

    AddDocTag(Tags::Deprecated);
    AddDocTag(Tags::Manip);
    AddParameter(ParameterType_InputImage,  "in",   "Input Image");
    SetParameterDescription( "in", "The image to scale." );
    AddParameter(ParameterType_OutputImage, "out",  "Output Image");
    SetParameterDescription( "out" , "The rescaled image filename." );

    AddRAMParameter();

    AddParameter(ParameterType_Float,      "outmin", "Output min value");
    AddParameter(ParameterType_Float,      "outmax", "Output max value");
    SetDefaultParameterFloat("outmin", 0.0);
    SetParameterDescription( "outmin", "Minimum value of the output image." );
    SetDefaultParameterFloat("outmax", 255.0);
    SetParameterDescription( "outmax", "Maximum value of the output image." );


    AddParameter(ParameterType_StringList,  "inmin", "inmin");
	SetParameterDescription("inmin", "Minimum value of the input image.");

	AddParameter(ParameterType_StringList,  "inmax", "inmax");
	SetParameterDescription("inmax", "Maximmum value of the input image.");


    MandatoryOff("outmin");
    MandatoryOff("outmax");

    MandatoryOff("inmax");
    MandatoryOff("inmin");

    SetOfficialDocLink();
  }

  void DoUpdateParameters() ITK_OVERRIDE
  {
    // Nothing to do here for the parameters : all are independent
  }

  void DoExecute() ITK_OVERRIDE
  {
    FloatVectorImageType::Pointer inImage = GetParameterImage("in");


    if (!HasValue("inmin") || !HasValue("inmax"))
    otbAppLogDEBUG( << "Starting Min/Max computation" )
    m_MinMaxFilter = MinMaxFilterType::New();
    m_MinMaxFilter->SetInput( inImage );
    m_MinMaxFilter->GetStreamer()->SetAutomaticAdaptativeStreaming(GetParameterInt("ram"));

    AddProcess(m_MinMaxFilter->GetStreamer(), "Min/Max computing");
    m_MinMaxFilter->Update();

    m_RescaleFilter = RescaleImageFilterType::New();
    m_RescaleFilter->SetInput( inImage );
    m_RescaleFilter->SetAutomaticInputMinMaxComputation(false);

    FloatVectorImageType::PixelType inMin, inMax;

    if(HasValue("inmin"))
    {
        inMin.SetSize( inImage->GetNumberOfComponentsPerPixel() );
        Utilities::ListOfDoubles l_inmin = Utilities::StringListAsDouble(
				this->GetParameterStringList("inmin"));

        for(unsigned int i=0; i < l_inmin.size() ; i++)
        {
            inMin.Fill(l_inmin.at(i));
        }
        vnsLogDebugMacro("inMin: " << inMin);
        m_RescaleFilter->SetInputMinimum( inMin );
    }
    else
    {
        vnsLogDebugMacro("Min/Max computation done : min=" << m_MinMaxFilter->GetMinimum())
        m_RescaleFilter->SetInputMinimum(m_MinMaxFilter->GetMinimum() );
    }

    if(HasValue("inmax"))
    {
        inMax.SetSize( inImage->GetNumberOfComponentsPerPixel() );
        Utilities::ListOfDoubles l_inmax = Utilities::StringListAsDouble(
				this->GetParameterStringList("inmax"));
        for(unsigned int i=0; i<l_inmax.size(); i++)
        {
          inMax.Fill(l_inmax.at(i));
        }
        vnsLogDebugMacro("inMax: " << inMax);
        m_RescaleFilter->SetInputMaximum( inMax );
    }
    else
    {
       vnsLogDebugMacro( "Min/Max computation done : max=" << m_MinMaxFilter->GetMaximum())
       m_RescaleFilter->SetInputMaximum( m_MinMaxFilter->GetMaximum() );
    }

    FloatVectorImageType::PixelType outMin, outMax;
    outMin.SetSize( inImage->GetNumberOfComponentsPerPixel() );
    outMax.SetSize( inImage->GetNumberOfComponentsPerPixel() );
    outMin.Fill( GetParameterFloat("outmin") );
    outMax.Fill( GetParameterFloat("outmax") );

    m_RescaleFilter->SetOutputMinimum( outMin );
    m_RescaleFilter->SetOutputMaximum( outMax );
    m_RescaleFilter->UpdateOutputInformation();

    std::string outFile = GetParameterString("out");
    std::string extension ;
    extension = itksys::SystemTools::GetFilenameLastExtension(outFile) ;

    if(extension == ".jpg" || extension==".jpeg")
    {
        vnsLogDebugMacro("jpeg output extension");
        SetParameterString("out", outFile+Utilities::GetExtendedFilenameWriteImageFileJpeg() );
    }

    SetParameterOutputImage("out", m_RescaleFilter->GetOutput());
  }

  RescaleImageFilterType::Pointer m_RescaleFilter;
  ImageFileWriterType::Pointer m_Writer;
  MinMaxFilterType::Pointer       m_MinMaxFilter;

};

}
}

OTB_APPLICATION_EXPORT(vns::Wrapper::RescaleIntensity)
