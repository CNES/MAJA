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
#include "vnsLoggers.h"
#include "itkMeanImageFilter.h"
#include "itkDiscreteGaussianImageFilter.h"
#include "itkGradientAnisotropicDiffusionImageFilter.h"


namespace vns
{
namespace Wrapper
{

using namespace otb::Wrapper;

enum
{
  Smoothing_Mean,
  Smoothing_Gaussian,
  Smoothing_Anisotropic
};

typedef otb::Image<DoubleVectorImageType::InternalPixelType, 2>  ImageType;

class SmoothingBis : public otb::Wrapper::Application
{
public:
  /** Standard class typedefs. */
  typedef SmoothingBis                      Self;
  typedef Application                   Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Standard macro */
  itkNewMacro(Self);

  itkTypeMacro(SmoothingBis, otb::Application);

private:
  void DoInit()
  {
    SetName("SmoothingBis");
    SetDescription("Apply a smoothing filter to an image");
    Loggers::GetInstance()->Initialize(GetName());
    SetDocLongDescription("This application applies smoothing filter to an image."
      " Either gaussian, mean, or anisotropic diffusion are available.");
    SetDocLimitations("None");
    SetDocAuthors("OTB-Team");
    SetDocSeeAlso(" ");

    AddDocTag(Tags::Filter);

    AddParameter(ParameterType_InputImage,  "in",   "Input Image");
    SetParameterDescription("in", "Input image to smooth.");
    AddParameter(ParameterType_OutputImage, "out",  "Output Image");
    SetParameterDescription("out", "Output smoothed image.");

    AddRAMParameter();

    AddParameter(ParameterType_Choice,      "type", "Smoothing Type");
    SetParameterDescription("type", "Smoothing kernel to apply");

    AddChoice("type.mean",     "Mean");

    AddParameter(ParameterType_Radius, "type.mean.radius", "Radius");
    SetParameterDescription("type.mean.radius", "Mean radius (in pixels)");
    SetDefaultParameterInt("type.mean.radius", 2);

    AddChoice("type.gaussian", "Gaussian");

    AddParameter(ParameterType_Float, "type.gaussian.radius", "Radius");
    SetParameterDescription("type.gaussian.radius", "Gaussian radius (in pixels)");
    SetDefaultParameterFloat("type.gaussian.radius", 2.0);

    AddParameter(ParameterType_Float, "type.gaussian.maxkernelwidth", "MaxKernelWidth");
    SetParameterDescription("type.gaussian.maxkernelwidth", "Maximum gaussian kernel width (in pixels)");
    SetDefaultParameterFloat("type.gaussian.maxkernelwidth", 8.0);

    AddParameter(ParameterType_Float, "type.gaussian.maxerror", "MaxError");
    SetParameterDescription("type.gaussian.maxerror", "Maximum gaussian error");
    SetDefaultParameterFloat("type.gaussian.maxerror", 1e-15);

    AddChoice("type.anidif",   "Anisotropic Diffusion");

    AddParameter(ParameterType_Float,  "type.anidif.timestep", "Time Step");
    SetParameterDescription("type.anidif.timestep", "Diffusion equation time step");

    AddParameter(ParameterType_Int,  "type.anidif.nbiter", "Nb Iterations");
    SetParameterDescription("type.anidif.nbiter", "Number of iterations");

    AddParameter(ParameterType_Float,  "type.anidif.conductance", "Conductance");
    SetParameterDescription("type.anidif.nbiter", "Controls the sensitivity of the conductance term");

    SetDefaultParameterFloat("type.anidif.timestep",   0.125);
    SetDefaultParameterInt("type.anidif.nbiter",     10);
    SetDefaultParameterInt("type.anidif.conductance",     1.);

    SetParameterString("type", "anidif", false);

    // Doc example parameter settings
    SetExampleComment("Image smoothing using a mean filter.", 0);
    SetDocExampleParameterValue("in", "Romania_Extract.tif");
    SetDocExampleParameterValue("out", "smoothedImage_mean.png uchar");
    SetDocExampleParameterValue("type", "mean");

    unsigned int exId = AddExample( "Image smoothing using an anisotropic diffusion filter." );
    SetDocExampleParameterValue("in", "Romania_Extract.tif", exId);
    SetDocExampleParameterValue("out", "smoothedImage_ani.png float", exId);
    SetDocExampleParameterValue("type", "anidif", exId);
    SetDocExampleParameterValue("type.anidif.timestep", "0.1", exId);
    SetDocExampleParameterValue("type.anidif.nbiter", "5", exId);
    SetDocExampleParameterValue("type.anidif.conductance", "1.5", exId);

	AddRAMParameter("ram");
	SetDefaultParameterInt("ram",2048);

    SetOfficialDocLink();
  }

  void DoUpdateParameters()
  {
    // Nothing to do here : all parameters are independent
  }

  void DoExecute()
  {
    GetLogger()->Debug("Entering DoExecute\n");


    DoubleImageType::Pointer inImage = GetParameterDoubleImage("in");

    switch ( GetParameterInt("type") )
      {
      case Smoothing_Mean:
        {
        GetLogger()->Debug("Using mean");

        typedef itk::MeanImageFilter<ImageType, ImageType>         MeanFilterType;

        MeanFilterType::Pointer filter
          = MeanFilterType::New();
        filter->SetInput(inImage);

        MeanFilterType::InputSizeType radius;
        radius.Fill( GetParameterInt("type.mean.radius") );
        filter->SetRadius(radius);
        filter->UpdateOutputInformation();
        m_FilterRef = filter;
        SetParameterOutputImage("out", filter->GetOutput());
        }
        break;
      case Smoothing_Gaussian:
        {
        GetLogger()->Debug("Using gaussian");

        typedef itk::DiscreteGaussianImageFilter<ImageType, ImageType>  DiscreteGaussianFilterType;

        DiscreteGaussianFilterType::Pointer filter
          = DiscreteGaussianFilterType::New();
        filter->SetInput(inImage);

        double radius = GetParameterFloat("type.gaussian.radius");
        double variance = radius * radius;
        filter->SetVariance(variance);
        filter->SetUseImageSpacing(false);
        const double maxkernelwidth = GetParameterFloat("type.gaussian.maxkernelwidth");
        filter->SetMaximumKernelWidth(maxkernelwidth);
        const double maxerror = GetParameterFloat("type.gaussian.maxerror");
        filter->SetMaximumError(maxerror);
        filter->UpdateOutputInformation();
        m_FilterRef = filter;
        SetParameterOutputImage("out", filter->GetOutput());
        }
        break;
      case Smoothing_Anisotropic:
        {
        GetLogger()->Debug("Using anisotropic diffusion");

        typedef itk::GradientAnisotropicDiffusionImageFilter<ImageType, ImageType>  GradientAnisotropicDiffusionFilterType;

        GradientAnisotropicDiffusionFilterType::Pointer filter
          = GradientAnisotropicDiffusionFilterType::New();
        filter->SetInput(inImage);

        const int aniDifNbIter = GetParameterInt("type.anidif.nbiter");
        filter->SetNumberOfIterations(static_cast<unsigned int>(aniDifNbIter));

        const float aniDifTimeStep = GetParameterFloat("type.anidif.timestep");
        filter->SetTimeStep(static_cast<double>(aniDifTimeStep));

        filter->SetConductanceParameter(GetParameterFloat("type.anidif.conductance"));
        filter->SetUseImageSpacing(false);
        filter->UpdateOutputInformation();
        
        m_FilterRef = filter;
        SetParameterOutputImage("out", filter->GetOutput());
        }
        break;
      }
  }

  itk::LightObject::Pointer m_FilterRef;
};


}
}

OTB_APPLICATION_EXPORT(vns::Wrapper::SmoothingBis)

