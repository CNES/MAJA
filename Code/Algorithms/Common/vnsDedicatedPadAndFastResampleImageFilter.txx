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
 * VERSION : 1.0.0 : FA : LAIG-FA-MAC-148399-CS : 14 octobre 2016 : Audit code - Correction qualite         *
 * VERSION : 4-4-1 : FA : LAIG-FA-MAC-127944-CS : 9 juillet 2015 : Correction warning et contrôle qualité   *
 * 														sur le code source C++   							*
 * VERSION : 4-4-0 : FA : LAIG-FA-MAC-1256-CNES : 1 juillet 2015 : Amelioration des performances            *
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id: vnsDedicatedPadAndFastResampleImageFilter.txx 9591 2015-03-20 17:29:33Z tfeuvrie $
 *                                                                                                          *
 ************************************************************************************************************/
#ifndef __vnsDedicatedPadAndFastResampleImageFilter_txx
#define __vnsDedicatedPadAndFastResampleImageFilter_txx

#include "vnsDedicatedPadAndFastResampleImageFilter.h"
#include "vnsMacro.h"
#include "vnsLoggers.h"
#include <iomanip>
#include "itkLinearInterpolateImageFunction.h"
#include "otbBCOInterpolateImageFunction.h"
#include "vnsLinearInterpolateImageFunction.h"
#include "vnsFastLinearInterpolateImageFunction.h"
#include "vnsFastVectorLinearWithRadiusInterpolateImageFunction.h"

namespace vns
{

// Constructor
    template<class TInputImage, class TOutputImage>
        DedicatedPadAndFastResampleImageFilter<TInputImage, TOutputImage>::DedicatedPadAndFastResampleImageFilter()

        {
            m_InterpolatorValue = ResamplerHelper::RB_INTERPOLATOR_LINEAR;
            // Input/Output initialization
            this->SetNumberOfIndexedInputs(1);
            this->SetNumberOfRequiredInputs(1);
            m_Pader = PadFilterType::New();
            m_Resampler = ResamplerFilterType::New();
            m_Resampler->SetReleaseDataFlag(true);
            m_Pader->SetReleaseDataFlag(true);

        }

// UpdateWithCaching method
    template<class TInputImage, class TOutputImage>
        void
        DedicatedPadAndFastResampleImageFilter<TInputImage, TOutputImage>::UpdateWithCaching()
        {

            InputImageType * l_NonConstinputPointer = const_cast<InputImageType*>(this->GetInput0());
            const InputImageType * inputPointer = this->GetInput0();

            typedef itk::LinearInterpolateImageFunction<InputImageType, double> LinearInterpolatorType;
            typedef otb::BCOInterpolateImageFunction<InputImageType, double> BCOInterpolatorType;
            typedef LinearInterpolateImageFunction<InputImageType, double> MACCSLinearWithRadiusInterpolatorType;
            typedef FastLinearInterpolateImageFunction<InputImageType, double> MACCSFastLinearInterpolatorType;

            unsigned int radius(0);

            // Interpolator set-up
            switch (m_InterpolatorValue)
                {
            case ResamplerHelper::RB_INTERPOLATOR_LINEAR:
                {
                    typename LinearInterpolatorType::Pointer f_interpolator = LinearInterpolatorType::New();
                    vnsLogDebugMacro("RB_INTERPOLATOR_LINEAR")
                    m_Resampler->SetInterpolator(f_interpolator);
                    break;
                }
                // Used for the VENUS subsampling (5m to 10m)
            case ResamplerHelper::RB_INTERPOLATOR_BCO:
                {
                    typename BCOInterpolatorType::Pointer f_interpolator = BCOInterpolatorType::New();
                    radius = ResamplerHelper::ComputeInterpolatorRadius(inputPointer->GetSpacing(), m_AreaToOutputImageResolution.Spacing,
                            m_InterpolatorValue);
                    vnsLogDebugMacro("RB_INTERPOLATOR_BCO - Radius = "<<radius)
                    f_interpolator->SetRadius(radius);
                    m_Resampler->SetInterpolator(f_interpolator);
                    break;
                }
            case ResamplerHelper::RB_INTERPOLATOR_LINEAR_WITH_RADIUS:
                {
                    typename MACCSLinearWithRadiusInterpolatorType::Pointer f_interpolator = MACCSLinearWithRadiusInterpolatorType::New();
                    radius = ResamplerHelper::ComputeInterpolatorRadius(inputPointer->GetSpacing(), m_AreaToOutputImageResolution.Spacing,
                            m_InterpolatorValue);
                    vnsLogDebugMacro("RB_INTERPOLATOR_LINEAR_WITH_RADIUS - Radius = "<<radius)
                    f_interpolator->SetRadius(radius);
                    m_Resampler->SetInterpolator(f_interpolator);
                    break;
                }
                // same as the RB_INTERPOLATOR_LINEAR (itk) but faster
            case ResamplerHelper::RB_INTERPOLATOR_MAJA_FAST_LINEAR:
                {
                    typename MACCSFastLinearInterpolatorType::Pointer f_interpolator = MACCSFastLinearInterpolatorType::New();
                    m_Resampler->SetInterpolator(f_interpolator);
                    break;
                }
            case ResamplerHelper::RB_INTERPOLATOR_MAJA_FAST_VECTOR_LINEAR:
                {
                    vnsLogDebugMacro("RB_INTERPOLATOR_MAJA_FAST_VECTOR_LINEAR")
                    // Nothing to do.
                    break;
                }
            case ResamplerHelper::RB_INTERPOLATOR_MAJA_FAST_VECTOR_LINEAR_WITH_RADIUS:
                {
                    radius = ResamplerHelper::ComputeInterpolatorRadius(inputPointer->GetSpacing(), m_AreaToOutputImageResolution.Spacing,
                            m_InterpolatorValue);
                    vnsLogDebugMacro("RB_INTERPOLATOR_MAJA_FAST_VECTOR_LINEAR_WITH_RADIUS - Radius = "<<radius)
                    m_Resampler->GetFastLinearWithRadiusInterpolator()->SetRadius(radius);
                    break;
                }
            case ResamplerHelper::NOT_INTERPOLATOR_INITIALIZED:
            default:
                {
                    vnsStaticExceptionBusinessMacro("The interpolator is not initialized.")
                }
                }

            // Fixed padding value : Set to 3
            typename PadFilterType::SizeType l_PadUpperBound;
            l_PadUpperBound.Fill(4);
            vnsLogDebugMacro("DedicatedPadAndFastResampleImageFilter - Pad value : "<<l_PadUpperBound)
            m_Pader->SetInput(inputPointer);
            m_Pader->SetPadUpperBound(l_PadUpperBound);
            m_Pader->SetPadLowerBound(l_PadUpperBound);
            m_Resampler->SetInput(m_Pader->GetOutput());

            m_Resampler->SetInterpolatorValue(m_InterpolatorValue);
            m_Resampler->SetAreaToOutputImageResolution(m_AreaToOutputImageResolution);

            m_OutputResampler = m_Resampler->GetOutput();
            vnsLogDebugMacro("UpdateWithCaching done ")

        }

} // End namespace vns

#endif /* __vnsDedicatedPadAndFastResampleImageFilter_txx */
