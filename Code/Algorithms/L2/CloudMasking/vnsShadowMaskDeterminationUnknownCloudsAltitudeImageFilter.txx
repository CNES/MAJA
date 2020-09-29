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
 * VERSION : 1.0.0 : FA : LAIG-FA-MAC-1990-CNES : 13 octobre 2016 : Correction qualite (TO DO)              *
 * VERSION : 1.0.0 : FA : LAIG-FA-MAC-1988-CNES : 13 octobre 2016 : Correction qualite (code mort)          *
 * VERSION : 5-1-0 : FA : LAIG-FA-MAC-145739-CS : 25 juillet 2016 : Audit code - Correction warning         *
 * VERSION : 5-0-0 : DM : LAIG-DM-MAC-982-CNES : 5 avr. 2016 : Creation                                     *
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id:
 *                                                                                                          *
 ************************************************************************************************************/
#ifndef __vnsShadowMaskDeterminationUnknownCloudsAltitudeImageFilter_txx
#define __vnsShadowMaskDeterminationUnknownCloudsAltitudeImageFilter_txx

#include "vnsShadowMaskDeterminationUnknownCloudsAltitudeImageFilter.h"

#include "otbImageFileWriter.h"
#include "vnsMacro.h"

namespace vns
{

    // Constructor
    template<class TInputImage, class TInputMask, class TLabelImage, class TGRIDImage, class TCLAImage, class TDTMImage, class TOutputImage>
        ShadowMaskDeterminationUnknownCloudsAltitudeImageFilter<TInputImage, TInputMask, TLabelImage, TGRIDImage, TCLAImage, TDTMImage,
                TOutputImage>::ShadowMaskDeterminationUnknownCloudsAltitudeImageFilter() :
                m_ShadowBandTOCR(100), m_ShadowBandRCR(100), m_NoData(0.), m_DeltaHMax(0), m_DeltaHMin(0), m_DeltaHStep(
                        0), m_SOLH1(0.), m_SOLHRef(0.), m_VIEHRef(0.), m_CloudCoverMultFactor(0.), m_ShadowCloudRatioMax(0.), m_ShadowCloudRatioMin(
                        0.), m_ShadowCloudRatioPixNb(0), m_ProjShadDarkeningMax(0.), m_DarkeningMinPercentile(0.)
        {
            // Parameters initialization
            m_IPTOCRInput = InputImageType::New();
            m_L2inRCRInput = InputImageType::New();
            m_IPEDGInput = InputMaskType::New();
            m_L2inNDTInput = InputMaskType::New();
            m_L2inWASInput = InputMaskType::New();
            m_L2CLDSubInput = InputMaskType::New();
            m_IPCLAInput = CLAImageType::New();

            m_DTMInput = DTMImageType::New();

            m_IPVIEInput = GRIDImageType::New();
            m_IPSOLInput = GRIDImageType::New();

            m_IPCLDShadOutput = OutputImageType::New();
            m_IPCLAOutput = CLAImageType::New();

            m_DarkeningFilter = DarkeningFilterType::New();
            m_DarkeningFilterOutputReader = ReaderDarkeningImageType::New();
            m_DarkeningImageList = DarkeningImageListType::New();
            m_DarkeningImageListToVectorImage = DarkeningImageListToVectorImageFilterType::New();
            m_ShadowThresholdValueEstimatorFilter = ShadowVariationThresholdValueEstimatorFilterType::New();
            m_DilatedCloudMaskAllLabelFilter = LabelFilterType::New();
            m_LabelMaskingFilter = LabelMaskingFilterType::New();
            m_ComputeCLDPixNbMap = StreamingStatisticsMapFromLabelImageFilterType::New();
            m_ComputesProjectionsMasksLabelsFilter = ComputesProjectionsMasksLabelsType::New();

            m_LabelCountFilterList_ShadByLab = LabelCountFilterListType::New();
            m_ShadowGeomMaskByRatioFilter = ByLabelCloudShadowFilterType::New();

            m_ShadowRadiomMaskFilter = LabelImageListMaskingFilterType::New();
            m_LabelCountFilterList_ShadByLab_Updated = LabelCountFilterListType::New();
            m_ShadowRadiomMaskByRatioFilter = ByLabelCloudShadowFilterType::New();

            m_ShadowFinalPixelDetection = ShadowFinalPixelDetectionFilterType::New();
            m_FlatteningFilter = MaskListFlatteningFilterType::New();

            m_ShadowLogicalSummaryFilter = TernaryLogicalFilterType::New();
            m_IPCLDShadConstantFilter = ConstantFilterType::New();

        }

    // Destructor
    template<class TInputImage, class TInputMask, class TLabelImage, class TGRIDImage, class TCLAImage, class TDTMImage, class TOutputImage>
        ShadowMaskDeterminationUnknownCloudsAltitudeImageFilter<TInputImage, TInputMask, TLabelImage, TGRIDImage, TCLAImage, TDTMImage,
                TOutputImage>::~ShadowMaskDeterminationUnknownCloudsAltitudeImageFilter()
        {
        }

    //================================== GenerateData ==================================/
    // Main computation method.
    template<class TInputImage, class TInputMask, class TLabelImage, class TGRIDImage, class TCLAImage, class TDTMImage, class TOutputImage>
        void
        ShadowMaskDeterminationUnknownCloudsAltitudeImageFilter<TInputImage, TInputMask, TLabelImage, TGRIDImage, TCLAImage, TDTMImage,
                TOutputImage>::UpdateWithCaching()
        {
            m_IPCLDShadConstantFilter->SetSize(this->GetL2CLDSubInput()->GetLargestPossibleRegion().GetSize());
            m_IPCLDShadConstantFilter->SetSpacing(this->GetL2CLDSubInput()->GetSignedSpacing());
            m_IPCLDShadConstantFilter->SetOrigin(this->GetL2CLDSubInput()->GetOrigin());
            m_IPCLDShadConstantFilter->SetConstantValue(static_cast<InputMaskPixelType>(0));

            // compute cloud cover
            ValueCountFilterPointerType l_CLDCountFilter = ValueCountFilterType::New();
            l_CLDCountFilter->SetImage(this->GetL2CLDSubInput());
            l_CLDCountFilter->SetPixelValue(static_cast<typename InputMaskType::PixelType>(1));
            l_CLDCountFilter->Update();
            const unsigned long l_CLDPixNb(l_CLDCountFilter->GetValueCount());
            vnsLogDebugValueMacro(l_CLDPixNb)

            // Compute Nodata number of pixels
            ValueCountFilterPointerType l_IPEDGCount = ValueCountFilterType::New();
            l_IPEDGCount->SetImage(this->GetIPEDGInput());
            l_IPEDGCount->SetPixelValue(static_cast<typename InputMaskType::PixelType>(1));
            l_IPEDGCount->Update();

            // -> Number of pixel to NoData is :
            const unsigned long l_NoDataNumber(l_IPEDGCount->GetValueCount());
            vnsLogDebugValueMacro(l_NoDataNumber)

            // Number of pixels of the image
            const unsigned long ImageNbPixels(
                    this->GetL2CLDSubInput()->GetLargestPossibleRegion().GetSize()[0]
                            * this->GetL2CLDSubInput()->GetLargestPossibleRegion().GetSize()[1]);
            vnsLogDebugValueMacro(ImageNbPixels)

            if (l_CLDPixNb > 0) // Cloud cover not empty
            {
                // cloud cover in ratio / use number of valid pixels (NoData pixel are excluded)
                const double l_CloudCover(static_cast<double>(l_CLDPixNb) / static_cast<double>(ImageNbPixels - l_NoDataNumber));

                // compute the percentile threshold value
                const double l_PercentileThreshold(
                        std::max(this->GetDarkeningMinPercentile(), std::min(this->GetCloudCoverMultFactor() * l_CloudCover, 1.)));

                vnsLogDebugValueMacro(l_CloudCover)
                vnsLogDebugValueMacro(l_PercentileThreshold)

                // cloud mask to label image based on dilated mask
                m_DilatedCloudMaskAllLabelFilter->SetInput(this->GetL2CLDDilatedSubInput());

                // Reduce labelMap to undilated cloud mask
                m_LabelMaskingFilter->SetInput1(m_DilatedCloudMaskAllLabelFilter->GetOutput());
                m_LabelMaskingFilter->SetInput2(this->GetL2CLDSubInput());
                m_LabelMaskingFilter->GetFunctor().SetBackgroundValue(static_cast<InputMaskPixelType>(0));
                m_LabelMaskingFilter->GetFunctor().SetReplaceValue(static_cast<LabelImagePixelType>(0));
                m_LabelMaskingFilter->UpdateOutputInformation();
                vnsLogDebugMacro("Filtered Label Map");

                // Compute Label population map
                m_ComputeCLDPixNbMap->SetInput(m_LabelMaskingFilter->GetOutput());
                m_ComputeCLDPixNbMap->Update();
                vnsLogDebugMacro(
                        "m_ComputeCLDPixNbMap->GetLabelPopulationMap count " << m_ComputeCLDPixNbMap->GetLabelPopulationMap().size());
                vnsLogDebugMacro("DilatedCloudMaskAllLabel count " << m_DilatedCloudMaskAllLabelFilter->GetObjectCount());

                for (unsigned int i = 0; i < m_ComputeCLDPixNbMap->GetLabelPopulationMap().size(); i++)
                {
                    vnsLogSuperDebugMacro("m_ComputeCLDPixNbMap[" << i << "]: " << m_ComputeCLDPixNbMap->GetLabelPopulationMap().at(i));
                }

                // compute darkening
                vnsLogDebugMacro("m_ShadowBandTOCR: " << m_ShadowBandTOCR);
                vnsLogDebugMacro("m_ShadowBandRCR : " << m_ShadowBandRCR);
                m_DarkeningFilter->SetIPTOCRInput(this->GetIPTOCRInput());
                m_DarkeningFilter->SetL2inRCRInput(this->GetL2inRCRInput());
                m_DarkeningFilter->SetL2CLDSubInput(this->GetL2CLDSubInput());
                m_DarkeningFilter->SetIPEDGInput(this->GetIPEDGInput());
                m_DarkeningFilter->SetL2inWASInput(this->GetL2inWASInput());
                // CF Mail Morgan du 20/04/2016. spec algo ST-169-CloudMasking-1130
                // and L2_in.R_surf_ray_sub(Shadow_band,l,p)!= No_data
                // m_DarkeningFilter->SetL2inNDTInput(this->GetL2inNDTInput());
                m_DarkeningFilter->SetShadowBandTOCR(m_ShadowBandTOCR);
                m_DarkeningFilter->SetShadowBandRCR(m_ShadowBandRCR);
                m_DarkeningFilter->SetNoData(m_NoData);

                // Call the "caching" not to call the previous filter for each stream
                //vnsCachingRealDataSingleImageMacro(m_DarkeningFilterOutputReader, m_DarkeningFilter->GetOutput(),
                //        "ShadowMaskDetDifferenceRefl")

                // compute the threshold value
                // TODO : supprimer list et utiliser le filtre CastImageToVectorImage filter
                m_DarkeningImageList->PushBack(m_DarkeningFilter->GetOutput());
                m_DarkeningImageListToVectorImage->SetInput(m_DarkeningImageList);
                typename PersistentShadowVariationThresholdValueEstimator<InputImageType>::CountVectorType l_NumberOfBins(1); // one component only
                l_NumberOfBins[0] = 1000;
                m_ShadowThresholdValueEstimatorFilter->SetNumberOfBins(l_NumberOfBins);
                //m_ShadowThresholdValueEstimatorFilter->SetHistogramMax(m_DarkeningMinMaxFilter->GetMaximum());    //Use default value
                //m_ShadowThresholdValueEstimatorFilter->SetHistogramMin(0.);                                       //Use default value
                m_ShadowThresholdValueEstimatorFilter->SetReflRatio(m_DarkeningImageListToVectorImage->GetOutput());
                m_ShadowThresholdValueEstimatorFilter->SetNoDataValue(0);
                m_ShadowThresholdValueEstimatorFilter->SetShadVarPercentile(l_PercentileThreshold);
                m_ShadowThresholdValueEstimatorFilter->SetShadVarDarkeningMax(this->GetProjShadDarkeningMax());
                m_ShadowThresholdValueEstimatorFilter->Update();
                vnsLogDebugMacro("l_PercentileThreshold : " << l_PercentileThreshold);
                vnsLogDebugMacro(
                        "m_ShadowThresholdValueEstimatorFilter->GetThresholdValue() : " << m_ShadowThresholdValueEstimatorFilter->GetThresholdValue());

                // Projection
                m_ComputesProjectionsMasksLabelsFilter->SetCLDLabelsCount(m_DilatedCloudMaskAllLabelFilter->GetObjectCount());
                m_ComputesProjectionsMasksLabelsFilter->SetNoData(m_NoData);
                m_ComputesProjectionsMasksLabelsFilter->SetDeltaHMax(m_DeltaHMax);
                m_ComputesProjectionsMasksLabelsFilter->SetDeltaHMin(m_DeltaHMin);
                m_ComputesProjectionsMasksLabelsFilter->SetDeltaHStep(m_DeltaHStep);
                m_ComputesProjectionsMasksLabelsFilter->SetSOLH1(m_SOLH1);
                m_ComputesProjectionsMasksLabelsFilter->SetVIEHRef(m_VIEHRef);
                m_ComputesProjectionsMasksLabelsFilter->SetDarkeningThreshold(m_ShadowThresholdValueEstimatorFilter->GetThresholdValue());
                // Inputs images
                m_ComputesProjectionsMasksLabelsFilter->SetIPCLDSubLabelInput(m_LabelMaskingFilter->GetOutput());
                m_ComputesProjectionsMasksLabelsFilter->SetIPVIEInput(m_IPVIEInput);
                m_ComputesProjectionsMasksLabelsFilter->SetIPSOL1Input(m_IPSOLInput);
                m_ComputesProjectionsMasksLabelsFilter->SetIPCLAInput(m_IPCLAInput);
                m_ComputesProjectionsMasksLabelsFilter->SetDTMInput(m_DTMInput);
                m_ComputesProjectionsMasksLabelsFilter->SetEDGInput(m_IPEDGInput);
                m_ComputesProjectionsMasksLabelsFilter->SetCLDInput(this->GetL2CLDSubInput());
                m_ComputesProjectionsMasksLabelsFilter->SetDarkeningInput(m_DarkeningFilter->GetOutput());

                m_ComputesProjectionsMasksLabelsFilter->UpdateWithCaching();

                // Generate population map for labels in LabeledShadowZoneDarkened (shadow_by_lab)
                LabelImagePixelType l_curLabel = 0;
                const unsigned int l_NbLabeledShadowZoneDarkenedThresholdOutputImageList(
                        m_ComputesProjectionsMasksLabelsFilter->GetLabeledShadowZoneDarkenedThresholdOutputImageList()->Size());
                vnsLogDebugValueMacro(l_NbLabeledShadowZoneDarkenedThresholdOutputImageList)

                for (unsigned int i = 0; i < l_NbLabeledShadowZoneDarkenedThresholdOutputImageList; i++)
                {
                    const unsigned int l_curImageIndex = i;
                    l_curLabel = l_curImageIndex + 1;

                    LabelCountFilterPointerType l_CountFilter = LabelCountFilterType::New();
                    l_CountFilter->SetImage(
                            m_ComputesProjectionsMasksLabelsFilter->GetLabeledShadowZoneDarkenedThresholdOutputImageList()->GetNthElement(
                                    l_curImageIndex));
                    l_CountFilter->SetPixelValue(static_cast<LabelImagePixelType>(l_curLabel));
                    l_CountFilter->Update();
                    m_LabelCountFilterList_ShadByLab->PushBack(l_CountFilter);
                    if (l_CountFilter->GetValueCount() > 0)
                    {
                        m_ShadowByLabPixelsNumberMap[l_curLabel] = l_CountFilter->GetValueCount();
                        vnsLogSuperDebugMacro("ShadowByLabPixelsNumberMap[" << l_curLabel << "] = " << l_CountFilter->GetValueCount());
                        vnsLogSuperDebugMacro(
                                "PotentialShadowZoneNbPixelsNumberMap[" << l_curLabel << "]: " << m_ComputesProjectionsMasksLabelsFilter->GetMapOfNbPixelsForEachLabel().at(l_curLabel));
                    }
                    else
                    {
                        vnsLogSuperDebugMacro("No pixel in ShadowByLabPixelsNumberMap[" << l_curLabel << "]");
                        vnsLogSuperDebugMacro(
                                "PotentialShadowZoneNbPixelsNumberMap[" << l_curLabel << "]: " << m_ComputesProjectionsMasksLabelsFilter->GetMapOfNbPixelsForEachLabel().at(l_curLabel));
                    }
                }

                // Label selection 1 (first pass)
                if (m_ShadowByLabPixelsNumberMap.size() > 0) // Is first pass necessary?
                {
                    vnsLogDebugMacro("First Pass")
                    m_ShadowGeomMaskByRatioFilter->SetInput(
                            m_ComputesProjectionsMasksLabelsFilter->GetLabeledShadowZoneDarkenedThresholdOutputImageList());
                    m_ShadowGeomMaskByRatioFilter->SetShadowCloudRatioMin(this->GetShadowCloudRatioMin());
                    m_ShadowGeomMaskByRatioFilter->SetShadowCloudRatioMax(this->GetShadowCloudRatioMax());
                    m_ShadowGeomMaskByRatioFilter->SetShadowCloudRatioPixNumberThreshold(this->GetShadowCloudRatioPixNb());
                    m_ShadowGeomMaskByRatioFilter->SetShadowPixelsNumberMap(m_ShadowByLabPixelsNumberMap);
                    m_ShadowGeomMaskByRatioFilter->SetCloudPixelsNumberMap(m_ComputeCLDPixNbMap->GetLabelPopulationMap());

                    m_ShadowLogicalSummaryFilter->SetInput1(m_ShadowGeomMaskByRatioFilter->GetOutput());

                    // Removing detected shadowy pixel from shadowy pixel to be verified during 2nd pass
                    InputMaskPixelType l_ExcludedValue = static_cast<InputMaskPixelType>(1);
                    LabelImagePixelType l_DefaultValue = static_cast<LabelImagePixelType>(0);
                    m_ShadowRadiomMaskFilter->SetInput(
                            m_ComputesProjectionsMasksLabelsFilter->GetLabeledShadowZoneDarkenedThresholdOutputImageList());
                    m_ShadowRadiomMaskFilter->SetInputMaskImage(m_ShadowGeomMaskByRatioFilter->GetOutput());
                    m_ShadowRadiomMaskFilter->GetFunctor().SetBackgroundValue(l_ExcludedValue);
                    m_ShadowRadiomMaskFilter->GetFunctor().SetReplaceValue(l_DefaultValue);
                    m_ShadowRadiomMaskFilter->Update();

                    // Generated up-to-date population map for rejected labels in LabeledShadowZoneDarkened
                    LabelImagePixelSetType l_RejectedLabelSet = m_ShadowGeomMaskByRatioFilter->GetRejectedLabelList();
                    LabelImagePixelType l_curLabel = 0;

                    vnsLogDebugMacro("RejectedLabelList().size = " << l_RejectedLabelSet.size());
                    for (typename LabelImagePixelSetType::const_iterator itRejected = l_RejectedLabelSet.begin();
                            itRejected != l_RejectedLabelSet.end(); ++itRejected)
                    {
                        vnsLogSuperDebugMacro("The label " << *itRejected << " has been rejected from first pass");
                        l_curLabel = *itRejected;
                        const unsigned int l_curImageIndex = l_curLabel - 1;

                        LabelCountFilterPointerType l_CountFilter = LabelCountFilterType::New();
                        l_CountFilter->SetImage(m_ShadowRadiomMaskFilter->GetOutput()->GetNthElement(l_curImageIndex));
                        l_CountFilter->SetPixelValue(static_cast<LabelImagePixelType>(l_curLabel));
                        l_CountFilter->Update();
                        m_LabelCountFilterList_ShadByLab_Updated->PushBack(l_CountFilter);

                        if (l_CountFilter->GetValueCount() > 0)
                        {
                            m_ShadowByLabPixelsNumberMap_Updated[l_curLabel] = l_CountFilter->GetValueCount();
                            vnsLogSuperDebugMacro(
                                    "ShadowByLabPixelsNumberMap_Updated[" << l_curLabel << "] = " << l_CountFilter->GetValueCount() << " (image " << l_curImageIndex << " in the list)");
                        }
                        else
                        {
                            vnsLogSuperDebugMacro(
                                    "No remaining pixel in ShadowByLabPixelsNumberMap_Updated[" << l_curLabel << "]" << " (image " << l_curImageIndex << " in the list)");
                        }
                    }
                }
                else
                {
                    m_ShadowLogicalSummaryFilter->SetInput1(m_IPCLDShadConstantFilter->GetOutput());
                }

                // Label selection 2 (second pass)
                if (m_ShadowByLabPixelsNumberMap_Updated.size() > 0) // Is Second pass necessary?
                {
                    vnsLogDebugMacro("Second Pass")
                    m_ShadowRadiomMaskByRatioFilter->SetInput(m_ShadowRadiomMaskFilter->GetOutput());
                    m_ShadowRadiomMaskByRatioFilter->SetShadowCloudRatioMin(this->GetShadowCloudRatioMin()); // ShadowCloudRatioMin is used for every tested label
                    m_ShadowRadiomMaskByRatioFilter->SetShadowCloudRatioMax(this->GetShadowCloudRatioMin()); // Min pour le parametere Max car traitement de reprise des labels rejetes
                    m_ShadowRadiomMaskByRatioFilter->SetShadowCloudRatioPixNumberThreshold(this->GetShadowCloudRatioPixNb());
                    m_ShadowRadiomMaskByRatioFilter->SetShadowPixelsNumberMap(m_ShadowByLabPixelsNumberMap_Updated);
                    m_ShadowRadiomMaskByRatioFilter->SetCloudPixelsNumberMap(m_ComputeCLDPixNbMap->GetLabelPopulationMap());
                    m_ShadowRadiomMaskByRatioFilter->Update();
                    m_ShadowLogicalSummaryFilter->SetInput2(m_ShadowRadiomMaskByRatioFilter->GetOutput());
                }
                else
                {
                    m_ShadowLogicalSummaryFilter->SetInput2(m_IPCLDShadConstantFilter->GetOutput());
                }

                // Final pixel selection (third pass)

                std::map<LabelImagePixelType, unsigned long> l_ShadowByLabPixelsNumberMap_StillRejected;
                LabelImagePixelSetType l_StillRejectedLabelSet = m_ShadowRadiomMaskByRatioFilter->GetRejectedLabelList();
                for (typename std::map<LabelImagePixelType, unsigned long>::const_iterator itRejected =
                        m_ShadowByLabPixelsNumberMap_Updated.begin(); itRejected != m_ShadowByLabPixelsNumberMap_Updated.end();
                        ++itRejected)
                {
                    if (l_StillRejectedLabelSet.find(itRejected->first) != l_StillRejectedLabelSet.end())
                    {
                        vnsLogSuperDebugMacro("The label " << itRejected->first << " has been rejected from second pass");
                        l_ShadowByLabPixelsNumberMap_StillRejected.insert(*itRejected);
                    }
                }

                if (l_StillRejectedLabelSet.size() > 0) // Is Third pass necessary?
                {
                    vnsLogDebugMacro("Third Pass")
                    m_ShadowFinalPixelDetection->SetRejectedPixelsNumberByLabel(l_ShadowByLabPixelsNumberMap_StillRejected);
                    m_ShadowFinalPixelDetection->SetCLDPixelNbMap(m_ComputeCLDPixNbMap->GetLabelPopulationMap());
                    m_ShadowFinalPixelDetection->SetShadowCloudRatio(this->GetShadowCloudRatioMin());
                    m_ShadowFinalPixelDetection->SetInputDarkening(m_DarkeningFilter->GetOutput());
                    m_ShadowFinalPixelDetection->SetInputMaskList(m_ShadowRadiomMaskFilter->GetOutput());
                    m_ShadowFinalPixelDetection->SetProjShadDarkeningMax(this->GetProjShadDarkeningMax());
                    m_ShadowFinalPixelDetection->SetNoData(m_NoData);
                    m_ShadowFinalPixelDetection->UpdateWithCaching();

                    // Flattening detected shadow pixel mask list
                    m_FlatteningFilter->SetInput(m_ShadowFinalPixelDetection->GetOutputMaskImageList());
                    m_ShadowLogicalSummaryFilter->SetInput3(m_FlatteningFilter->GetOutput());
                }
                else
                {
                    m_ShadowLogicalSummaryFilter->SetInput3(m_IPCLDShadConstantFilter->GetOutput());
                }

                m_IPCLDShadOutput = m_ShadowLogicalSummaryFilter->GetOutput();
            }
            else // Cloud cover empty
            {
                vnsLogDebugMacro("No Cloud Detected -> Shadow mask empty");
                m_IPCLDShadOutput = m_IPCLDShadConstantFilter->GetOutput();
            }

            m_IPCLAOutput = this->GetIPCLAInput();

        }

    // PrintSelf method
    template<class TInputImage, class TInputMask, class TLabelImage, class TGRIDImage, class TCLAImage, class TDTMImage, class TOutputImage>
        void
        ShadowMaskDeterminationUnknownCloudsAltitudeImageFilter<TInputImage, TInputMask, TLabelImage, TGRIDImage, TCLAImage, TDTMImage,
                TOutputImage>::PrintSelf(std::ostream& os, itk::Indent indent) const
        {
            Superclass::PrintSelf(os, indent);

            // Display the parameters
            os << "m_ShadowBandTOCR    " << m_ShadowBandTOCR << std::endl;
            os << "m_ShadowBandRCR    " << m_ShadowBandRCR << std::endl;
            os << "m_NoData:    " << m_NoData << std::endl;
            // Parameter used for the altitude refinement
            os << "m_DeltaHMax:    " << m_DeltaHMax << std::endl;
            os << "m_DeltaHMin:    " << m_DeltaHMin << std::endl;
            os << "m_DeltaHStep:    " << m_DeltaHStep << std::endl;
            os << "m_SOLH1:    " << m_SOLH1 << std::endl;
            os << "m_SOLHRef:    " << m_SOLHRef << std::endl;
            os << "m_VIEHRef:    " << m_VIEHRef << std::endl;
            os << "m_CloudCoverMultFactor:    " << m_CloudCoverMultFactor << std::endl;
            os << "m_ShadowCloudRatioMax:    " << m_ShadowCloudRatioMax << std::endl;
            os << "m_ShadowCloudRatioMin:    " << m_ShadowCloudRatioMin << std::endl;
            os << "m_ShadowCloudRatioPixNb:    " << m_ShadowCloudRatioPixNb << std::endl;
            os << "m_ProjShadDarkeningMax:    " << m_ProjShadDarkeningMax << std::endl;
            os << "m_DarkeningMinPercentile:    " << m_DarkeningMinPercentile << std::endl;

        }

} // End namespace vns

#endif /* __vnsShadowMaskDeterminationUnknownCloudsAltitudeImageFilter_txx */
