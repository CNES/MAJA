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
 * VERSION : 5-1-0 : FA : LAIG-FA-MAC-144719-CS : 17 mai 2016 : Correction warning et contrôle qualité      *
 * VERSION : 5-0-0 : FA : LAIG-FA-MAC-143764-CS : 19 avril 2016 : Mise en forme du code (indentation, etc.) *
 * VERSION : 4-6-4 : FA : LAIG-FA-MAC-1426-CNES : 30 novembre 2015 : Valeurs < 0 FRE : correction poutr TVA *
 * VERSION : 4-6-2 : FA : LAIG-FA-MAC-1426-CNES : 10 novembre 2015 : Valeurs très négatives                 *
 *                                                                   sur les plans FRE                      *
 * VERSION : 4-0-0 : FA : LAIG-FA-MAC-120653-CS : 3 avril 2014 : Correction de règles de qualité			*
 * VERSION : 2-0-0 : DM : LAIG-DM-MAC-146-CNES : 07 novembre 2011 : Modification pour prise en compte       *
 *                                                            d'evolutions liees a MACCS                    *
 * VERSION : 1-0-0 : 07 novembre 2011 : Creation                                                            *
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id:
 *                                                                                                          *
 ************************************************************************************************************/
#ifndef __vnsComputeEnvironmentCorrectionImageFilter_txx
#define __vnsComputeEnvironmentCorrectionImageFilter_txx

#include "vnsComputeEnvironmentCorrectionImageFilter.h"
#include "vnsMacro.h"
#include "vnsLoggers.h"
#include "vnsResamplerHelper.h"
#include "vnsDedicatedPadAndFastResampleImageFilter.h"

namespace vns
{

    //
    //******************* Constructor  ****************
    //
    template<class TInputVectorImage, class TOutputVectorImage, class TInputMask>
        ComputeEnvironmentCorrectionImageFilter<TInputVectorImage, TOutputVectorImage, TInputMask>::ComputeEnvironmentCorrectionImageFilter()
        {

            // Input/Output initialization
            this->SetNumberOfRequiredInputs(6);
            this->SetNumberOfIndexedInputs(6);

            m_RhoEnv = OutputVectorImageType::New();
            m_L2SRE = OutputVectorImageType::New();

            // Instantiate Filters
            // Expand filters used to resample the image at L2 resolution
            m_ExpandTdir = DedicatedPadAndFastResampleImageFilterType::New();
            m_ExpandTdir->SetReleaseDataFlag(true);
            m_ExpandTdif = DedicatedPadAndFastResampleImageFilterType::New();
            m_ExpandTdif->SetReleaseDataFlag(true);
            m_ExpandSatm = DedicatedPadAndFastResampleImageFilterType::New();
            m_ExpandSatm->SetReleaseDataFlag(true);
            m_ExpandRhoEnv = DedicatedPadAndFastResampleImageFilterType::New();
            m_ExpandRhoEnv->SetReleaseDataFlag(true);

            m_ComputeFinalResultEnvironmentCorrection = ComputeFinalResultEnvironmentCorrectionImageFilterType::New();
            m_ComputeFinalResultEnvironmentCorrection->SetReleaseDataFlag(true);
            m_ThresholdVectorImageFilter = ThresholdVectorImageFilterType::New();
            m_ThresholdVectorImageFilter->SetReleaseDataFlag(true);

            // Default No_Data value
            m_NoData = 0;

        }

    //
    //******************* Destructor  ****************
    //
    template<class TInputVectorImage, class TOutputVectorImage, class TInputMask>
        ComputeEnvironmentCorrectionImageFilter<TInputVectorImage, TOutputVectorImage, TInputMask>::~ComputeEnvironmentCorrectionImageFilter()
        {
        }

    //
    //******************* BeforeThreadedGenerateData  ****************
    //
    template<class TInputVectorImage, class TOutputVectorImage, class TInputMask>
        void
        ComputeEnvironmentCorrectionImageFilter<TInputVectorImage, TOutputVectorImage, TInputMask>::CheckInputs()
        {

            InputVectorImageConstPointerType l_IPTOCPtr = this->GetIPTOC();
            InputMaskConstPointerType l_L2EDGPtr = this->GetL2EDG();
            InputVectorImageConstPointerType l_RhoEnvSubPtr = this->GetRhoEnvSub();
            OutputVectorImageConstPointerType l_LUT_TdirPtr = this->GetLUT_Tdir();
            OutputVectorImageConstPointerType l_LUT_TdifPtr = this->GetLUT_Tdif();
            OutputVectorImageConstPointerType l_LUT_SatmPtr = this->GetLUT_Satm();

            if ((l_RhoEnvSubPtr.IsNull() == true) || (l_LUT_TdirPtr.IsNull() == true) || (l_LUT_TdifPtr.IsNull() == true)
                    || (l_LUT_SatmPtr.IsNull() == true) || (l_IPTOCPtr.IsNull() == true))
            {
                itkExceptionMacro(<< "At least one input is missing.");
            }

        }

//
//******************* GenerateData  ****************
//
    template<class TInputVectorImage, class TOutputVectorImage, class TInputMask>
        void
        ComputeEnvironmentCorrectionImageFilter<TInputVectorImage, TOutputVectorImage, TInputMask>::UpdateWithCaching()
        {

            // Validate the presence of all inputs and their sizes
            CheckInputs();

            // Load Inputs
            InputVectorImageConstPointerType l_IPTOCPtr = this->GetIPTOC();
            InputVectorImageConstPointerType l_RhoEnvSubPtr = this->GetRhoEnvSub();
            OutputVectorImageConstPointerType l_LUT_TdirPtr = this->GetLUT_Tdir();
            OutputVectorImageConstPointerType l_LUT_TdifPtr = this->GetLUT_Tdif();
            OutputVectorImageConstPointerType l_LUT_SatmPtr = this->GetLUT_Satm();

            InputMaskConstPointerType l_L2EDGPtr = this->GetL2EDG();

            // L2Coarse area
            AreaType l_AreaToL2Resolution;
            l_AreaToL2Resolution.Origin = l_IPTOCPtr->GetOrigin();
            l_AreaToL2Resolution.Spacing = l_IPTOCPtr->GetSpacing();
            l_AreaToL2Resolution.Size = l_IPTOCPtr->GetLargestPossibleRegion().GetSize();

            vnsImagePadAndFastDedicatedResamplerMacro("ExpandTdir", /*DedicatedPadAndFastResampleImageFilterType, */m_ExpandTdir,
                    l_LUT_TdirPtr /* Input */,ResamplerHelper::RB_INTERPOLATOR_MAJA_FAST_VECTOR_LINEAR /* Interpolator */,
                    l_AreaToL2Resolution)
            //, l_edgeVal /* edgePaddingValue */)
            vnsImagePadAndFastDedicatedResamplerMacro("ExpandTdif", /*DedicatedPadAndFastResampleImageFilterType, */m_ExpandTdif,
                    l_LUT_TdifPtr /* Input */,ResamplerHelper::RB_INTERPOLATOR_MAJA_FAST_VECTOR_LINEAR /* Interpolator */,
                    l_AreaToL2Resolution)
            //, l_edgeVal /* edgePaddingValue */)
            vnsImagePadAndFastDedicatedResamplerMacro("ExpandSatm", /*DedicatedPadAndFastResampleImageFilterType, */m_ExpandSatm,
                    l_LUT_SatmPtr /* Input */,ResamplerHelper::RB_INTERPOLATOR_MAJA_FAST_VECTOR_LINEAR /* Interpolator */,
                    l_AreaToL2Resolution)
            //, l_edgeVal /* edgePaddingValue */)
            vnsImagePadAndFastDedicatedResamplerMacro("ExpandRhoEnv", /*DedicatedPadAndFastResampleImageFilterType, */m_ExpandRhoEnv,
                    l_RhoEnvSubPtr /* Input */,ResamplerHelper::RB_INTERPOLATOR_MAJA_FAST_VECTOR_LINEAR /* Interpolator */,
                    l_AreaToL2Resolution)
            //, l_edgeVal /* edgePaddingValue */)

            //Threshold to avoid blur effect at edge border.
            // FA 1426: Change threshold from 1 to 2 to prevent RhoEnv=-10 in output if RhoEnv=1.03
            //         and outside value to threshold value
            m_ThresholdVectorImageFilter->SetInput(m_ExpandRhoEnv->GetOutput());
            m_ThresholdVectorImageFilter->SetUpper(2);
            m_ThresholdVectorImageFilter->SetOutsideValue(m_NoData);

            // 4 - Resize the expanded images to the correct output size

            // MACCS 4.3 - optimization - try to remove this caching
            m_RhoEnv =  m_ThresholdVectorImageFilter->GetOutput();
            // 6 - Compute the final result
            vnsLogDebugMacro(
                    "ComputeEnvironmentCorrectionImageFilter::UpdateWithCaching: l_IPTOCPtr->GetNumberOfComponentsPerPixel() : "<<l_IPTOCPtr->GetNumberOfComponentsPerPixel())

            m_ComputeFinalResultEnvironmentCorrection->SetTocImage(l_IPTOCPtr);
            m_ComputeFinalResultEnvironmentCorrection->SetRhoEnvImage(m_RhoEnv);
            m_ComputeFinalResultEnvironmentCorrection->SetTdirImage(m_ExpandTdir->GetOutput());
            m_ComputeFinalResultEnvironmentCorrection->SetTdifImage(m_ExpandTdif->GetOutput());
            m_ComputeFinalResultEnvironmentCorrection->SetSatmImage(m_ExpandSatm->GetOutput());
            m_ComputeFinalResultEnvironmentCorrection->SetEdgImage(l_L2EDGPtr);
            m_ComputeFinalResultEnvironmentCorrection->SetNoData(m_NoData);

            m_L2SRE = m_ComputeFinalResultEnvironmentCorrection->GetOutput();

        }

//
//******************* Printself method  ****************
//
    template<class TInputVectorImage, class TOutputVectorImage, class TInputMask>
        void
        ComputeEnvironmentCorrectionImageFilter<TInputVectorImage, TOutputVectorImage, TInputMask>::PrintSelf(std::ostream& os,
                itk::Indent indent) const
        {
            this->Superclass::PrintSelf(os, indent);
        }

} // End namespace vns

#endif /* __vnsComputeEnvironmentCorrectionImageFilter_txx */
