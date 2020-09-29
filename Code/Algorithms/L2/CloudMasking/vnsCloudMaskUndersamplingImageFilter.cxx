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
 * VERSION : 1.0.0 : FA : LAIG-FA-MAC-148399-CS : 03 novembre 2016 : Audit code - Correction qualite        *
 * VERSION : 1.0.0 : FA : LAIG-FA-MAC-1988-CNES : 21 octobre 2016 : Correction qualite (code mort)          *
 * VERSION : 5-0-0 : FA : LAIG-FA-MAC-1619-CNES : 21 avril 2016 : Modification pour corriger l'anomalie     *
 * VERSION : 5-0-0 : FA : LAIG-FA-MAC-143764-CS : 12 avril 2016 : Mise en forme du code (indentation, etc.) *
 * VERSION : 1-0-0-3 : DM : 331 : 04 oct. 2011 : Cirrus Flag, calcul fait à L2CoarseResolution                *
 * VERSION : 1-0-0-3 : FA : 294 : 14 septembre 2011 : Prise en compte remarque SGC CNES VE-FT-294.            *
 * VERSION : 1-0-0-1 : FA : 244 : 27 juin 2011 : Sous echantillonnage du plan d'altitude des nuages           *
 *                                                                                                          *
 * VERSION : 1-0-0 : <TypeFT> : <NumFT> : 23 avr. 2010 : Creation                                                           
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id$
 *                                                                                                          *
 ************************************************************************************************************/
#include "vnsCloudMaskUndersamplingImageFilter.h"

#include "vnsLoggers.h"
#include "vnsCloudMaskUndersamplingInterpolateImageFunction.h"
#include "vnsResamplerHelper.h"

namespace vns
{

    /** Constructor */
    CloudMaskUndersamplingImageFilter::CloudMaskUndersamplingImageFilter()
    {

        // Input/Output initialization
        this->SetNumberOfIndexedInputs(1);
        this->SetNumberOfRequiredInputs(1);

        // Parameters initialization
        m_L1NoData = static_cast<RealNoDataType>(0);

        m_IPCLAOutput = OutputImageType::New();
        m_IPCLDAltOutput = OutputMaskType::New();
        m_CloudMaskUndersamplingWithoutResamplingImageFilter = CloudMaskUndersamplingWithoutResamplingImageFilterType::New();
        m_ResampleImageFilterCLA = GridResampleImageFilterCLAType::New();
        m_ResampleImageFilterCLDAlt = GridResampleImageFilterCLDAltType::New();

    }

    /** Main */
    void
    CloudMaskUndersamplingImageFilter::UpdateWithCaching()
    {

        // Check if necessary to resample image or not !!
        const InputImageSpacingType l_InputSpacing = this->GetL1CLAInput()->GetSpacing();
        vnsLogDebugMacro("L1CLA Spacing value: "<< l_InputSpacing)

        const double l_SpacingRatio = vcl_abs(static_cast<double>(m_AreaToL2CoarseResolution.Spacing[0]) / static_cast<double>(l_InputSpacing[0]));
        vnsLogDebugValueMacro(l_SpacingRatio)

        if (vnsEqualsDoubleMacro(l_SpacingRatio,1.) == true)
        {
            m_CloudMaskUndersamplingWithoutResamplingImageFilter->SetL1CLAInput(this->GetL1CLAInput());
            m_CloudMaskUndersamplingWithoutResamplingImageFilter->SetNoData(m_L1NoData);

            // Set the output image pointer of the filter
            m_IPCLAOutput = m_CloudMaskUndersamplingWithoutResamplingImageFilter->GetIPCLAOutput();
            m_IPCLDAltOutput = m_CloudMaskUndersamplingWithoutResamplingImageFilter->GetIPCLDAltOutput();

        }
        else
        {
            unsigned int l_InterpolatorRadius(0);
            // CLA computes
            m_ResampleImageFilterCLA->SetInput(this->GetL1CLAInput());
            m_ResampleImageFilterCLA->SetOutputOrigin(m_AreaToL2CoarseResolution.Origin);
            m_ResampleImageFilterCLA->SetOutputSpacing(m_AreaToL2CoarseResolution.Spacing);
            m_ResampleImageFilterCLA->SetOutputSize(m_AreaToL2CoarseResolution.Size);
            typedef CloudMaskUndersamplingCLAInterpolateImageFunction<InputImageType, double> CLAInterpolatorType;
            CLAInterpolatorType::Pointer f_interpolator_cla = CLAInterpolatorType::New();
            // Compute interpolator radius
            l_InterpolatorRadius = ResamplerHelper::ComputeInterpolatorRadius(l_InputSpacing, m_AreaToL2CoarseResolution.Spacing,
                    ResamplerHelper::RB_INTERPOLATOR_MAJA_CLOUD_UNDERSAMPLING_CLA);
            vnsLogDebugMacro("RB_INTERPOLATOR_MAJA_CLOUD_UNDERSAMPLING_CLA - Radius = "<<l_InterpolatorRadius)
            f_interpolator_cla->SetRadius(l_InterpolatorRadius);

            // Set NoData
            f_interpolator_cla->SetNoData(m_L1NoData);

            // Set interpolator to the resampler filter
            m_ResampleImageFilterCLA->SetInterpolator(f_interpolator_cla);
            // Set the InterpolatorRadius to the resampler filter
            m_ResampleImageFilterCLA->SetInterpolatorRadius(l_InterpolatorRadius);

            // CLDAlt computes
            m_ResampleImageFilterCLDAlt->SetInput(this->GetL1CLAInput());
            m_ResampleImageFilterCLDAlt->SetOutputOrigin(m_AreaToL2CoarseResolution.Origin);
            m_ResampleImageFilterCLDAlt->SetOutputSpacing(m_AreaToL2CoarseResolution.Spacing);
            m_ResampleImageFilterCLDAlt->SetOutputSize(m_AreaToL2CoarseResolution.Size);
            typedef CloudMaskUndersamplingCLDAltInterpolateImageFunction<InputImageType, double> CLDAltInterpolatorType;
            CLDAltInterpolatorType::Pointer f_interpolator_cldalt = CLDAltInterpolatorType::New();
            // Compute interpolator radius
            l_InterpolatorRadius = ResamplerHelper::ComputeInterpolatorRadius(l_InputSpacing, m_AreaToL2CoarseResolution.Spacing,
                    ResamplerHelper::RB_INTERPOLATOR_MAJA_CLOUD_UNDERSAMPLING_CLD_ALT);
            vnsLogDebugMacro("RB_INTERPOLATOR_MAJA_CLOUD_UNDERSAMPLING_CLDAlt - Radius = "<<l_InterpolatorRadius)
            f_interpolator_cldalt->SetRadius(l_InterpolatorRadius);

            // Set NoData
            f_interpolator_cldalt->SetNoData(m_L1NoData);

            // Set SuccesOutputValue FailureOutputValue
            f_interpolator_cldalt->SetSuccesOutputValue(1);
            f_interpolator_cldalt->SetFailureOutputValue(0);

            // Set interpolator to the resampler filter
            m_ResampleImageFilterCLDAlt->SetInterpolator(f_interpolator_cldalt);
            // Set the InterpolatorRadius to the resampler filter
            m_ResampleImageFilterCLDAlt->SetInterpolatorRadius(l_InterpolatorRadius);

            // Set the output image pointer of the filter
            m_IPCLAOutput = m_ResampleImageFilterCLA->GetOutput();
            m_IPCLDAltOutput = m_ResampleImageFilterCLDAlt->GetOutput();
        }

    }

    /** PrintSelf method */

    void
    CloudMaskUndersamplingImageFilter::PrintSelf(std::ostream& os, itk::Indent indent) const
    {
        Superclass::PrintSelf(os, indent);
        os << "m_L1NoData:               " << m_L1NoData << std::endl;
    }

} // End namespace vns
