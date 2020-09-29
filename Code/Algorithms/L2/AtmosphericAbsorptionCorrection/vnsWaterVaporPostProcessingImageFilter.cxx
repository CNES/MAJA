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
 *                                        __  __    __     ____   __                                        *
 *                                       (  \/  )  /__\   (_  _) /__\                                       *
 *                                        )    (  /(__)\ .-_)(  /(__)\                                      *
 *                                       (_/\/\_)(__)(__)\____)(__)(__)                                     *
 *                                                                                                          *
 *                                                                                                          *
 ************************************************************************************************************
 *                                                                                                          *
 * Author: CS Systemes d'Information (France)                                                              *
 *                                                                                                          *
 ************************************************************************************************************
 * HISTORIQUE                                                                                               *
 *                                                                                                          *
 * VERSION : 3.0.0 : DM : LAIG-DM-MAJA-2368-CNES : 27 juillet 2017 : Seuillage WaterPP sur bande B8/B9      *
 * VERSION : 2.1.1 : FA : LAIG-FA-MAJA-2443-CNES : 18 sept 2017 : Pas de IWC en sortie de muscate           *
 * VERSION : 2.0.0 : DM : LAIG-DM-MAJA-153436-CS : 13 avril 2017 : Refactoring de classe doublons           *
 * VERSION : 2.0.0 : FA : LAIG-FA-MAJA-152790-CS : 21 fevrier 2017 : Correction qualite taux de commentaire *
 * VERSION : 1.0.0 : DM : LAIG-DM-MAC-1754-CNES : 24 nov 2016 : Creation                                      *
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id$                                                                                                     *
 *                                                                                                          *
 ************************************************************************************************************/
#include "vnsWaterVaporPostProcessingImageFilter.h"

namespace vns
{
    // Constructor
    WaterVaporPostProcessingImageFilter::WaterVaporPostProcessingImageFilter() :
            m_NoData(0.), m_DefaultWaterVapor(2.0), m_UseDefaultConstantWaterAmount(true), m_WaterVaporReferenceBandCode(99),
			m_WaterVaporReflectanceThresholdRef(-999.), m_WaterVaporBandCode(99), m_WaterVaporReflectanceThreshold(-999.),
					m_WaterVaporInitWindowRadius(99), m_WaterVaporMaxWindowRadius(99), m_WaterVaporSEValidRadius(99)
    {
        // Set the number of inputs
        this->SetNumberOfIndexedInputs(4);
        this->SetNumberOfRequiredInputs(1);

        // Instanciate filters
        m_MultiToMonoChannelExtractFilterRef = MultiToMonoChannelExtractFilterType::New();
        m_ThresholdFilterRef = ThresholdFilterType::New();
        m_MultiToMonoChannelExtractFilter = MultiToMonoChannelExtractFilterType::New();
        m_ThresholdFilter = ThresholdFilterType::New();
        m_VapourApplyMaskFilter = ApplyMaskFilterType::New();
        m_CLDSumApplyMaskFilter = ApplyMaskFilterType::New();
        m_DilateFilter = DilateFilterType::New();
        m_VAPMaskingFilter = VAPMaskingFilterType::New();
        // Init filter streaming conditional statistics
        m_StreamingConditionalStatisticsImageFilter = StreamingConditionalStatisticsImageFilterType::New();
        // Init gap filing filter
        m_GapFillingImageFilter = GapFillingImageFilterType::New();
        // Init Expand filters
        m_ExpandList = PadAndResampleImageFilterListType::New();
        m_ExpandVAPFillMaskList = ExpandThresholdMaskFilterListType::New();
        // Init water amount filters
        m_WaterAmountValueConstantImageFilterList = VAPImageConstantImageSourceFilterListType::New();
        m_WaterAmountValueConstantMaskFilterList = VAPMaskConstantImageSourceFilterListType::New();

        // Init VAP Outputs
        m_L2VAPImageListOutput = VAPImageListType::New();
        m_L2VAPFillMaskListOutput = MaskImageListType::New();
        // Init VAP Sub Outputs
        m_VAPImageSubOutput = VAPImageType::New();
        m_VAPFillMaskSubOutput = MaskImageType::New();
    }

    // Destructor
    WaterVaporPostProcessingImageFilter::~WaterVaporPostProcessingImageFilter()
    {
    }

    void
    WaterVaporPostProcessingImageFilter::UpdateWithCaching()
    {
        // Get Inputs
        DTMImageListConstPointerType l_DTMList = this->GetDTMInput();
        if (l_DTMList.IsNull() == true)
        {
            vnsExceptionBusinessMacro("DTM input is null...");
        }

        // Get the number of resolution of the L2 product
        const unsigned int l_nbRes = l_DTMList->Size();

        // Constant water amount value
        if (m_UseDefaultConstantWaterAmount == true)
        {
            // For each L2 resolution
            for (unsigned int l_Res = 0; l_Res < l_nbRes; l_Res++)
            {
                // L2Coarse area
                AreaType l_AreaToL2Resolution;
                l_AreaToL2Resolution.Origin = l_DTMList->GetNthElement(l_Res)->GetOrigin();
                l_AreaToL2Resolution.Spacing = l_DTMList->GetNthElement(l_Res)->GetSpacing();
                l_AreaToL2Resolution.Size = l_DTMList->GetNthElement(l_Res)->GetLargestPossibleRegion().GetSize();

                // Generate a constant image with the constant value set in the GIP_L2COMM
                VAPImageConstantImageSourceFilterType::Pointer l_VAPConstantImageFilter = VAPImageConstantImageSourceFilterType::New();
                // Set constant image spatial properties
                l_VAPConstantImageFilter->SetSize(l_AreaToL2Resolution.Size);
                l_VAPConstantImageFilter->SetSpacing(l_AreaToL2Resolution.Spacing);
                l_VAPConstantImageFilter->SetOrigin(l_AreaToL2Resolution.Origin);
                // Define the constant pixel to be used
                VAPImageType::PixelType VAPpixel;
                VAPpixel = static_cast<VAPImageType::PixelType>(m_DefaultWaterVapor);
                l_VAPConstantImageFilter->SetConstantValue(VAPpixel);
                l_VAPConstantImageFilter->UpdateOutputInformation();
                // Store the filter
                m_WaterAmountValueConstantImageFilterList->PushBack(l_VAPConstantImageFilter);
                // Store the resulting image
                m_L2VAPImageListOutput->PushBack(l_VAPConstantImageFilter->GetOutput());

                // Generate the mask associated with the constant image
                VAPMaskConstantImageSourceFilterType::Pointer l_VAPConstantMaskFilter = VAPMaskConstantImageSourceFilterType::New();
                // Set the mask spatial properties
                l_VAPConstantMaskFilter->SetSize(l_AreaToL2Resolution.Size);
                l_VAPConstantMaskFilter->SetSpacing(l_AreaToL2Resolution.Spacing);
                l_VAPConstantMaskFilter->SetOrigin(l_AreaToL2Resolution.Origin);
                // Define the constant pixel to be used
                MaskImageType::PixelType MaskPixel;
                MaskPixel = static_cast<MaskImageType::PixelType>(0);
                l_VAPConstantMaskFilter->SetConstantValue(MaskPixel);
                l_VAPConstantMaskFilter->UpdateOutputInformation();
                // Store the filter
                m_WaterAmountValueConstantMaskFilterList->PushBack(l_VAPConstantMaskFilter);
                // Store the resulting Mask
                m_L2VAPFillMaskListOutput->PushBack(l_VAPConstantMaskFilter->GetOutput());
            }
        }
        // Estimated water vapor post processing
        else
        {
            // Retrieve inputs
            IPTOCRImageConstPointerType l_IPTOCRSub = this->GetIPTOCRInput();
            VAPImageConstPointerType l_VAPSub = this->GetVAPSubInput();
            MaskImageConstPointerType l_CLDSub = this->GetCLDSubInput();

            // Check Inputs
            if (l_IPTOCRSub.IsNull() == true)
            {
                vnsExceptionBusinessMacro("TOCR input at L2 coarse is null...");
            }
            if (l_VAPSub.IsNull() == true)
            {
                vnsExceptionBusinessMacro("VAP input at L2 coarse is null...");
            }
            if (l_CLDSub.IsNull() == true)
            {
                vnsExceptionBusinessMacro("CLD input is null...");
            }

            // Extract Ref Band
            m_MultiToMonoChannelExtractFilterRef->SetInput(l_IPTOCRSub);
            // In the MultiToMonoChannelExtract filter, the first band index is 1 and not 0
            m_MultiToMonoChannelExtractFilterRef->SetChannel(m_WaterVaporReferenceBandCode + 1);
            // Binary Threshold on ref band
            m_ThresholdFilterRef->SetInput(m_MultiToMonoChannelExtractFilterRef->GetOutput());
            m_ThresholdFilterRef->GetFunctor().SetThresholdValue(m_WaterVaporReflectanceThresholdRef);
            m_ThresholdFilterRef->GetFunctor().SetInsideValue(1);
            m_ThresholdFilterRef->GetFunctor().SetOutputValue(0);

            // Extract vapour Band
            m_MultiToMonoChannelExtractFilter->SetInput(l_IPTOCRSub);
            // In the MultiToMonoChannelExtract filter, the first band index is 1 and not 0
            m_MultiToMonoChannelExtractFilter->SetChannel(m_WaterVaporBandCode + 1);
            // Binary Threshold on ref band
            m_ThresholdFilter->SetInput(m_MultiToMonoChannelExtractFilter->GetOutput());
            m_ThresholdFilter->GetFunctor().SetThresholdValue(m_WaterVaporReflectanceThreshold);
            m_ThresholdFilter->GetFunctor().SetInsideValue(1);
            m_ThresholdFilter->GetFunctor().SetOutputValue(0);
            //Apply vapour on ref binary
            m_VapourApplyMaskFilter->SetInput1(m_ThresholdFilterRef->GetOutput());
            m_VapourApplyMaskFilter->SetInput2(m_ThresholdFilter->GetOutput());
            m_VapourApplyMaskFilter->GetFunctor().SetReplaceValue(static_cast<MaskImagePixelType>(1));
            m_VapourApplyMaskFilter->GetFunctor().SetBackgroundValue(static_cast<MaskImagePixelType>(1));

            // Apply CLD
            m_CLDSumApplyMaskFilter->SetInput1(m_VapourApplyMaskFilter->GetOutput());
            m_CLDSumApplyMaskFilter->SetInput2(l_CLDSub);
            m_CLDSumApplyMaskFilter->GetFunctor().SetReplaceValue(static_cast<MaskImagePixelType>(1));
            m_CLDSumApplyMaskFilter->GetFunctor().SetBackgroundValue(static_cast<MaskImagePixelType>(1));

            // Dilate
            MaskImagePixelType l_DilateValue(1);
            StructuringElementType l_SECloud;
            l_SECloud.SetRadius(m_WaterVaporSEValidRadius); // 3x3 structuring element
            l_SECloud.CreateStructuringElement();
            m_DilateFilter->SetKernel(l_SECloud);
            m_DilateFilter->SetInput(m_CLDSumApplyMaskFilter->GetOutput());
            m_DilateFilter->SetDilateValue(l_DilateValue);

            // Patch input VAPSub with NoData
            m_VAPMaskingFilter->SetInput1(l_VAPSub);
            m_VAPMaskingFilter->SetInput2(m_DilateFilter->GetOutput());
            m_VAPMaskingFilter->GetFunctor().SetBackgroundValue(static_cast<MaskImagePixelType>(1));
            m_VAPMaskingFilter->GetFunctor().SetReplaceValue(static_cast<VAPImagePixelType>(m_NoData));

            // VAP Statistics estimation
            m_StreamingConditionalStatisticsImageFilter->SetInput(m_VAPMaskingFilter->GetOutput());
            m_StreamingConditionalStatisticsImageFilter->SetExcludedValue(m_NoData);
            m_StreamingConditionalStatisticsImageFilter->SetEnableVariance(false);
            m_StreamingConditionalStatisticsImageFilter->Update();
            const bool l_HasValidPixel = (m_StreamingConditionalStatisticsImageFilter->GetNbOfValidValues() != 0);

            vnsLogDebugMacro("HasValidPixel used in VAP Gap Filling: " << l_HasValidPixel)
            vnsLogDebugMacro("Mean Value used in VAP Gap Filling: " << m_StreamingConditionalStatisticsImageFilter->GetMean())

            // Gap Filling filter
            m_GapFillingImageFilter->SetInputImage(m_VAPMaskingFilter->GetOutput());
            m_GapFillingImageFilter->SetNoData(m_NoData);
            m_GapFillingImageFilter->SetDefaultValue(m_DefaultWaterVapor);
            m_GapFillingImageFilter->SetMeanValue(m_StreamingConditionalStatisticsImageFilter->GetMean());
            m_GapFillingImageFilter->SetHasValidPixel(l_HasValidPixel);
            m_GapFillingImageFilter->SetInitWindowRadius(m_WaterVaporInitWindowRadius);
            m_GapFillingImageFilter->SetMaxWindowRadius(m_WaterVaporMaxWindowRadius);
            MaskImagePointerType l_FilledFlagImage = m_GapFillingImageFilter->GetOutputMask();
            m_GapFillingImageFilter->UpdateOutputInformation();

            m_VAPImageSubOutput = m_GapFillingImageFilter->GetOutputFilledImage();
            m_VAPFillMaskSubOutput = m_GapFillingImageFilter->GetOutputMask();

            // For each L2 resolution, resample:
            for (unsigned int l_Res = 0; l_Res < l_nbRes; l_Res++)
            {
                // L2 area
                // Set spatial properties
                AreaType l_AreaToL2Resolution;
                l_AreaToL2Resolution.Origin = l_DTMList->GetNthElement(l_Res)->GetOrigin();
                l_AreaToL2Resolution.Spacing = l_DTMList->GetNthElement(l_Res)->GetSpacing();
                l_AreaToL2Resolution.Size = l_DTMList->GetNthElement(l_Res)->GetLargestPossibleRegion().GetSize();

                // Resample to l_Res resolution
                PadAndResampleImageFilterPointerType l_ExpandVAP = PadAndResampleImageFilterType::New();
                vnsImagePadAndResamplerMacro("L2VAPGenerator", l_ExpandVAP, m_GapFillingImageFilter->GetOutputFilledImage(),
                        ResamplerHelper::RB_INTERPOLATOR_LINEAR, l_AreaToL2Resolution)
                // And store filter and output
                m_ExpandList->PushBack(l_ExpandVAP);
                m_L2VAPImageListOutput->PushBack(l_ExpandVAP->GetOutput());

                // Resample to l_Res resolution
                ExpandThresholdMaskFilterPointerType l_ExpandVAPFillMaskFilter = ExpandThresholdMaskFilterType::New();
                l_ExpandVAPFillMaskFilter->SetInput0(m_GapFillingImageFilter->GetOutputMask());
                l_ExpandVAPFillMaskFilter->SetEdgePaddingValue(0);
                l_ExpandVAPFillMaskFilter->SetAreaToOutputImageResolution(l_AreaToL2Resolution);
                l_ExpandVAPFillMaskFilter->UpdateWithCaching();
                // And store filter and output
                m_ExpandVAPFillMaskList->PushBack(l_ExpandVAPFillMaskFilter);
                m_L2VAPFillMaskListOutput->PushBack(l_ExpandVAPFillMaskFilter->GetOutput0());
            }
        }
    }

    void
    WaterVaporPostProcessingImageFilter::PrintSelf(std::ostream& os, itk::Indent indent) const
    {
        Superclass::PrintSelf(os, indent);
        os << "m_NoData: " << m_NoData << std::endl;
        os << "m_DefaultWaterVapor: " << m_DefaultWaterVapor << std::endl;
        os << "m_UseDefaultConstantWaterAmount: " << m_UseDefaultConstantWaterAmount << std::endl;
        os << "m_WaterVaporReferenceBandCodem: " << m_WaterVaporReferenceBandCode << std::endl;
        os << "m_WaterVaporReflectanceThreshold: " << m_WaterVaporReflectanceThreshold << std::endl;
        os << "m_WaterVaporInitWindowRadius: " << m_WaterVaporInitWindowRadius << std::endl;
        os << "m_WaterVaporMaxWindowRadius: " << m_WaterVaporMaxWindowRadius << std::endl;
        os << "m_WaterVaporSEValidRadius: " << m_WaterVaporSEValidRadius << std::endl;
    }

} // End namespace vns
