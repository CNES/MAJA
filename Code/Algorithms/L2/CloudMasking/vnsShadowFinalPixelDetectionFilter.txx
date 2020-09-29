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
 * VERSION : 2.0.0 : DM : LAIG-DM-MAJA-153436-CS : 13 avril 2017 : Refactoring de classe doublons           *
 * VERSION : 1.0.0 : FA : LAIG-FA-MAC-148399-CS : 03 novembre 2016 : Audit code - Correction qualite        *
 * VERSION : 5-1-1 : FA : LAIG-FA-MAC-1858-CNES : 18 aout 2016 : Ombres nuage incohérentes S2 MUSCATE       *
 * VERSION : 5-0-0 : DM : LAIG-DM-MAC-982-CNES : 5 avr. 2016 : Creation                                     *
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id:
 *                                                                                                          *
 ************************************************************************************************************/
#ifndef __vnsShadowFinalPixelDetectionFilter_txx
#define __vnsShadowFinalPixelDetectionFilter_txx

#include "vnsShadowFinalPixelDetectionFilter.h"

namespace vns
{

    // Constructor
    template<class TRatioImage, class TLabelImage,class TMaskImage>
        ShadowFinalPixelDetectionFilter<TRatioImage, TLabelImage, TMaskImage>::ShadowFinalPixelDetectionFilter() : m_ShadowCloudRatio(0.), m_ProjShadDarkeningMax(0.), m_NoData(0.)
        {
        m_InputMaskList = LabelImageListType::New();
        m_InputDarkening =  RatioImageType::New();

        m_OutputMaskImageList = MaskImageListType::New();

        // Filters initialization

        m_MaskingImageFilterList = MaskingFilterListType::New();
        m_DarkeningMaskedImageListList = DarkeningImageListListType::New();
        m_DarkeningMaskedToVectorImageList = DarkeningImageListToVectorImageFilterListType::New();
        m_ShadowThresholdValueEstimatorFilterList = ThresholdValueEstimatorListType::New();
        m_DarkeningThresholdFilterList = ThresholdingFilterListType::New();

        }

    // Destructor
    template<class TRatioImage, class TLabelImage,class TMaskImage>
        ShadowFinalPixelDetectionFilter<TRatioImage, TLabelImage, TMaskImage>::~ShadowFinalPixelDetectionFilter()
        {
        }

    //================================== GenerateData ==================================/
    // Main computation method.
    template<class TRatioImage, class TLabelImage,class TMaskImage>
    void
    ShadowFinalPixelDetectionFilter<TRatioImage, TLabelImage, TMaskImage>::UpdateWithCaching()
    {

        double shadowCloudRatio = this->GetShadowCloudRatio();

        //Label Loop
        for (typename NbPixelsForEachLabelMapType::const_iterator it=this->GetRejectedPixelsNumberByLabel().begin(); it!=this->GetRejectedPixelsNumberByLabel().end(); ++it)
        {
            unsigned int curLabel = it->first;
            double curNbCLD = this->GetCLDPixelNbMap().at(curLabel);
            const double curPercentileThreshold = shadowCloudRatio * curNbCLD / it->second;

            // Extract Darkening associated with current label
            MaskingFilterPointerType l_MaskingImageFilter = MaskingFilterType::New();
            l_MaskingImageFilter->SetInput1(this->GetInputDarkening());
            itkAssertInDebugAndIgnoreInReleaseMacro(curLabel > 0);
            itkAssertInDebugAndIgnoreInReleaseMacro(curLabel-1 < this->GetInputMaskList()->Size());
            l_MaskingImageFilter->SetInput2(this->GetInputMaskList()->GetNthElement(int(curLabel) -1));
            l_MaskingImageFilter->GetFunctor().SetKeptValue(static_cast<LabelImagePixelType>(curLabel));
            l_MaskingImageFilter->GetFunctor().SetDefaultValue(static_cast<RatioImagePixelType>(this->GetNoData()));
            l_MaskingImageFilter->UpdateOutputInformation();
            m_MaskingImageFilterList->PushBack(l_MaskingImageFilter);

            DarkeningImageListPointerType l_DarkeningMaskedImageList = DarkeningImageListType::New();
            l_DarkeningMaskedImageList->PushBack(l_MaskingImageFilter->GetOutput());
            m_DarkeningMaskedImageListList->PushBack(l_DarkeningMaskedImageList);

            DarkeningImageListToVectorImageFilterPointerType l_DarkeningMaskedToVectorImage = DarkeningImageListToVectorImageFilterType::New();
            l_DarkeningMaskedToVectorImage->SetInput(l_DarkeningMaskedImageList);
            m_DarkeningMaskedToVectorImageList->PushBack(l_DarkeningMaskedToVectorImage);

            // Compute threshold associated with current label
            ThresholdValueEstimatorFilterPointerType l_ShadowThresholdValueEstimatorFilter = ThresholdValueEstimatorFilterType::New();
            l_ShadowThresholdValueEstimatorFilter->SetReflRatio(l_DarkeningMaskedToVectorImage->GetOutput());
            l_ShadowThresholdValueEstimatorFilter->SetNoDataValue(this->GetNoData());
            l_ShadowThresholdValueEstimatorFilter->SetShadVarPercentile(curPercentileThreshold);
            l_ShadowThresholdValueEstimatorFilter->SetShadVarDarkeningMax(this->GetProjShadDarkeningMax());
            l_ShadowThresholdValueEstimatorFilter->Update();
            m_ShadowThresholdValueEstimatorFilterList->PushBack(l_ShadowThresholdValueEstimatorFilter);
            m_ThresholdVector.push_back(l_ShadowThresholdValueEstimatorFilter->GetThresholdValue());
            vnsLogDebugMacro("curLabel: " << curLabel << " - used Percentile = " << curPercentileThreshold << "/ DarkeningThresholdValue = " << l_ShadowThresholdValueEstimatorFilter->GetThresholdValue());

            // apply pixel thresholding
            ThresholdingFilterPointerType l_DarkeningThresholdFilter = ThresholdingFilterType::New();
            l_DarkeningThresholdFilter->SetInput(l_MaskingImageFilter->GetOutput());
            l_DarkeningThresholdFilter->GetFunctor().SetMaximumThresholdValue(l_ShadowThresholdValueEstimatorFilter->GetThresholdValue());
            l_DarkeningThresholdFilter->GetFunctor().SetNoDataValue(this->GetNoData());
            l_DarkeningThresholdFilter->GetFunctor().SetOutsideValue(static_cast<MaskImagePixelType>(0));
            l_DarkeningThresholdFilter->GetFunctor().SetInsideValue(static_cast<MaskImagePixelType>(1));
//            l_DarkeningThresholdFilter->Update();
            m_DarkeningThresholdFilterList->PushBack(l_DarkeningThresholdFilter);
            m_OutputMaskImageList->PushBack(l_DarkeningThresholdFilter->GetOutput());

        }// end of labels loop

    }

    // PrintSelf method
    template<class TRatioImage, class TLabelImage,class TMaskImage>
        void
        ShadowFinalPixelDetectionFilter<TRatioImage, TLabelImage, TMaskImage>::PrintSelf(std::ostream& os,
                itk::Indent indent) const
        {
            Superclass::PrintSelf(os, indent);

            os << "m_NoData:    " << m_NoData << std::endl;
        }

} // End namespace vns

#endif /* __vnsShadowFinalPixelDetectionFilter_txx */
