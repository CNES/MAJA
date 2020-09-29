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
 *																											*
 * VERSION : 1.0.0 : FA : LAIG-FA-MAC-1988-CNES : 21 octobre 2016 : Correction qualite (code mort)          *
 * VERSION : 1.0.0 : FA : LAIG-FA-MAC-148399-CS : 14 octobre 2016 : Audit code - Correction qualite         *
 * VERSION : 1.0.0 : FA : LAIG-FA-MAC-1990-CNES : 13 octobre 2016 : Correction qualite (TO DO)              *
 * VERSION : 5-0-0 : FA : LAIG-FA-MAC-1619-CNES : 21 avril 2016 : Modification pour corriger l'anomalie     *
 * VERSION : 4-8-0 : DM : LAIG-DM-MAC-1638-CNES : 1 mars 2016 : Nouveau filtre reechantillonnage issu OTB   *
 * VERSION : 4-3-0 : FA : LAIG-FA-MAC-1243-CNES : 26 mai 2015 : Correction du calcul du padding necessaire  *
 *																											*
 * VERSION : 1-0-0 : <TypeFT> : <NumFT> : 11 janv. 2011 : Creation                                                           
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id: vnsPadAndResampleImageFilter.txx 9591 2015-03-20 17:29:33Z tfeuvrie $
 *                                                                                                          *
 ************************************************************************************************************/
#ifndef __vnsPadAndResampleImageFilter_txx
#define __vnsPadAndResampleImageFilter_txx

#include "vnsPadAndResampleImageFilter.h"
#include "itkLinearInterpolateImageFunction.h"
#include "otbBCOInterpolateImageFunction.h"
#include "vnsLinearInterpolateImageFunction.h"
#include "vnsFastLinearInterpolateImageFunction.h"
#include "vnsCloudMaskUndersamplingInterpolateImageFunction.h"
#include "vnsMacro.h"
#include "vnsLoggers.h"
#include <iomanip>

namespace vns
{

// Constructor
    template<class TInputImage, class TOutputImage>
        PadAndResampleImageFilter<TInputImage, TOutputImage>::PadAndResampleImageFilter() :
                m_Interpolator(ResamplerHelper::RB_INTERPOLATOR_BCO), m_PadRadius(4)
        {
            // Input/Output initialization
            this->SetNumberOfIndexedInputs(1);
            this->SetNumberOfRequiredInputs(1);
            m_Pader = PadFilterType::New();
            m_Pader->SetReleaseDataFlag(true);
            m_Resampler = ResamplerFilterType::New();
            m_Resampler->SetReleaseDataFlag(true);
            m_OutputResampler = OutputImageType::New();

        }

// UpdateWithCaching method
    template<class TInputImage, class TOutputImage>
        void
        PadAndResampleImageFilter<TInputImage, TOutputImage>::UpdateWithCaching()
        {
            InputImageType * l_NonConstinputPointer = const_cast<InputImageType*>(this->GetInput0());
            const InputImageType * inputPointer = this->GetInput0();

            // Compute the padding radius
            // TODO : calcul automatique du pas (reprendre code commente dans version précedente (supprimé pour éviter code mort)
            //        Include the interpolator radius into the padding computation

            unsigned int l_InterpolatorRadius(0);

            // Interpolator set-up
            switch (m_Interpolator)
                {
            case ResamplerHelper::RB_INTERPOLATOR_LINEAR:
                {
                    typedef itk::LinearInterpolateImageFunction<InputImageType, double> LinearInterpolatorType;
                    typename LinearInterpolatorType::Pointer f_interpolator = LinearInterpolatorType::New();
                    vnsLogDebugMacro("RB_INTERPOLATOR_LINEAR")
                    m_Resampler->SetInterpolator(f_interpolator);
                    l_InterpolatorRadius = 1;
                    break;
                }
            case ResamplerHelper::RB_INTERPOLATOR_BCO:
                {
                    typedef otb::BCOInterpolateImageFunction<InputImageType, double> BCOInterpolatorType;
                    typename BCOInterpolatorType::Pointer f_interpolator = BCOInterpolatorType::New();
                    l_InterpolatorRadius = ResamplerHelper::ComputeInterpolatorRadius(inputPointer->GetSignedSpacing(),
                            m_AreaToOutputImageResolution.Spacing, m_Interpolator);
                    vnsLogDebugMacro("RB_INTERPOLATOR_BCO - Radius = "<<l_InterpolatorRadius)
                    f_interpolator->SetRadius(l_InterpolatorRadius);
                    m_Resampler->SetInterpolator(f_interpolator);
                    break;
                }
            case ResamplerHelper::RB_INTERPOLATOR_LINEAR_WITH_RADIUS:
                {
                    typedef LinearInterpolateImageFunction<InputImageType, double> MAJALinearWithRadiusInterpolatorType;
                    typename MAJALinearWithRadiusInterpolatorType::Pointer f_interpolator = MAJALinearWithRadiusInterpolatorType::New();
                    l_InterpolatorRadius = ResamplerHelper::ComputeInterpolatorRadius(inputPointer->GetSignedSpacing(),
                            m_AreaToOutputImageResolution.Spacing, m_Interpolator);
                    vnsLogDebugMacro("RB_INTERPOLATOR_LINEAR_WITH_RADIUS - Radius = "<<l_InterpolatorRadius)
                    f_interpolator->SetRadius(l_InterpolatorRadius);
                    m_Resampler->SetInterpolator(f_interpolator);
                    break;
                }
            case ResamplerHelper::RB_INTERPOLATOR_MAJA_FAST_LINEAR:
                {
                    typedef FastLinearInterpolateImageFunction<InputImageType, double> MAJAFastLinearInterpolatorType;
                    typename MAJAFastLinearInterpolatorType::Pointer f_interpolator = MAJAFastLinearInterpolatorType::New();
                    m_Resampler->SetInterpolator(f_interpolator);
                    l_InterpolatorRadius = 1;
                    break;
                }
            case ResamplerHelper::RB_INTERPOLATOR_MAJA_CLOUD_UNDERSAMPLING_CLA:
                {
                    typedef CloudMaskUndersamplingCLAInterpolateImageFunction<InputImageType, double> CLAInterpolatorType;
                    typename CLAInterpolatorType::Pointer f_interpolator = CLAInterpolatorType::New();
                    l_InterpolatorRadius = ResamplerHelper::ComputeInterpolatorRadius(inputPointer->GetSignedSpacing(),
                            m_AreaToOutputImageResolution.Spacing, m_Interpolator);
                    vnsLogDebugMacro("RB_INTERPOLATOR_MAJA_CLOUD_UNDERSAMPLING_CLA - Radius = "<<l_InterpolatorRadius)
                    f_interpolator->SetRadius(l_InterpolatorRadius);
                    m_Resampler->SetInterpolator(f_interpolator);
                    break;
                }
            case ResamplerHelper::RB_INTERPOLATOR_MAJA_CLOUD_UNDERSAMPLING_CLD_ALT:
                {
                    typedef CloudMaskUndersamplingCLDAltInterpolateImageFunction<InputImageType, double> CLDAltInterpolatorType;
                    typename CLDAltInterpolatorType::Pointer f_interpolator = CLDAltInterpolatorType::New();
                    l_InterpolatorRadius = ResamplerHelper::ComputeInterpolatorRadius(inputPointer->GetSignedSpacing(),
                            m_AreaToOutputImageResolution.Spacing, m_Interpolator);
                    vnsLogDebugMacro("RB_INTERPOLATOR_MAJA_CLOUD_UNDERSAMPLING_CLD_ALT - Radius = "<<l_InterpolatorRadius)
                    f_interpolator->SetRadius(l_InterpolatorRadius);
                    m_Resampler->SetInterpolator(f_interpolator);
                    break;
                }
            case ResamplerHelper::NOT_INTERPOLATOR_INITIALIZED:
            default:
                {
                    vnsStaticExceptionBusinessMacro("The interpolator is not initialized.")
                }
                }

            // Set to 3

            if ( m_PadRadius > 0) {
            	typename PadFilterType::SizeType l_PadUpperBound;
            	l_PadUpperBound.Fill(m_PadRadius);
            	vnsLogDebugMacro("PadAndResampleImageFilter - Pad value : "<<l_PadUpperBound)
            	m_Pader->SetInput(inputPointer);
            	m_Pader->SetPadUpperBound(l_PadUpperBound);
            	m_Resampler->SetInput(m_Pader->GetOutput());
            } else {
            	m_Resampler->SetInput(inputPointer);
            }

            m_Resampler->SetOutputOrigin(m_AreaToOutputImageResolution.Origin);
            m_Resampler->SetOutputSpacing(m_AreaToOutputImageResolution.Spacing);
            m_Resampler->SetOutputSize(m_AreaToOutputImageResolution.Size);
            m_Resampler->SetInterpolatorRadius(l_InterpolatorRadius);

            m_OutputResampler = m_Resampler->GetOutput();

        }

} // End namespace vns

#endif /* __vnsPadAndResampleImageFilter_txx */
