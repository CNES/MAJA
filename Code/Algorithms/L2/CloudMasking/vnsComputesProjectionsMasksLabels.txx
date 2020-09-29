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
 * VERSION : 5-0-0 : DM : LAIG-DM-MAC-982-CNES : 5 avr. 2016 : Creation                                     *
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id:
 *                                                                                                          *
 ************************************************************************************************************/
#ifndef __vnsComputesProjectionsMasksLabels_txx
#define __vnsComputesProjectionsMasksLabels_txx

#include "vnsComputesProjectionsMasksLabels.h"

#include "otbImageFileWriter.h"
#include "itkTimeProbe.h"

namespace vns
{

    // Constructor
    template<class TRatioImage, class TLabelImage, class TMaskImage, class TGRIDImage, class TCLAImage, class TDTMImage>
        ComputesProjectionsMasksLabels<TRatioImage, TLabelImage, TMaskImage, TGRIDImage, TCLAImage, TDTMImage>::ComputesProjectionsMasksLabels()
        {

            // Parameters initialization
            m_CLDLabelsCount = static_cast<LabelImagePixelType>(0);

            m_NoData = static_cast<RealNoDataType>(0);

            m_DeltaHMax = 0;
            m_DeltaHMin = 0;
            m_DeltaHStep = 0;
            m_SOLH1 = 0.;
            m_VIEHRef = 0.;
            m_DarkeningThreshold = 0.;

            // Filters initialization
            m_IPCLAInput = CLAImageType::New();
            m_IPCLDSubLabelInput = LabelImageType::New();

            m_DTMInput = DTMImageType::New();

            m_IPVIEInput = GRIDImageType::New();
            m_IPSOL1Input = GRIDImageType::New();

            // Init L1 image spacing
            m_CLDandDarkFusionFilter = MaskFusionFilterType::New();

            m_KeepsCloudLabelFilterPointerTypeList = KeepsCloudLabelFilterPointerTypeList::New();
            m_ShadowMaskProjectionFilterList = ShadowMaskProjectionFilterPointerTypeList::New();
            m_KeepsCloudLabelFilterPointerTypeList->Clear();
            m_ShadowMaskCleaningFilterList = LabelMaskingFilterListType::New();

            m_LabeledShadowZoneOutputImageList = LabelImageListType::New();
            m_LabeledShadowZoneDarkenedThresholdOutputImageList = LabelImageListType::New();
            m_DarkeningThresholdFilterList = DarkeningThresholdFilterListType::New();
        }

    // Destructor
    template<class TRatioImage, class TLabelImage, class TMaskImage, class TGRIDImage, class TCLAImage, class TDTMImage>
        ComputesProjectionsMasksLabels<TRatioImage, TLabelImage, TMaskImage, TGRIDImage, TCLAImage, TDTMImage>::~ComputesProjectionsMasksLabels()
        {
        }

    //================================== GenerateData ==================================/
    // Main computation method.
    template<class TRatioImage, class TLabelImage, class TMaskImage, class TGRIDImage, class TCLAImage, class TDTMImage>
        void
        ComputesProjectionsMasksLabels<TRatioImage, TLabelImage, TMaskImage, TGRIDImage, TCLAImage, TDTMImage>::UpdateWithCaching()
        {
            // Load the DTM image
            m_I3D.Load(this->GetDTMInput());

            if (m_DeltaHStep == 0)
            {
                vnsStaticExceptionBusinessMacro("The step used for the altitude excursion (Delta_H_Step or Local_Delat_H_Step) is null\n");
            }

            const unsigned short l_NumberOfAltitudes(static_cast<unsigned short>(((m_DeltaHMax - m_DeltaHMin) / m_DeltaHStep) + 1));

            vnsLogDebugMacro("m_DeltaHMax: "<<m_DeltaHMax<<" m_DeltaHMin: "<<m_DeltaHMin<<" m_DeltaHStep: "<<m_DeltaHStep<<" => l_NumberOfAltitudes: "<<l_NumberOfAltitudes)

            //            4) Research of potential shadow zones (for each cloud label)
            //                The idea is to get the whole zone in which shadows may lie
            //                For each cloud label lab
            this->GetIPCLDSubLabelInput()->UpdateOutputInformation();

            vnsLogDebugMacro("m_CLDLabelsCount= "<<m_CLDLabelsCount);

            // Prepare mask from darkening and CLD -> ignored pixel are at one
            MaskImagePixelType one = static_cast<MaskImagePixelType>(1);
            m_CLDandDarkFusionFilter->SetInput1(this->GetCLDInput());
            m_CLDandDarkFusionFilter->SetInput2(this->GetDarkeningInput());
            m_CLDandDarkFusionFilter->GetFunctor().SetBackgroundValue(m_NoData);
            m_CLDandDarkFusionFilter->GetFunctor().SetReplaceValue(one);
            m_CLDandDarkFusionFilter->UpdateOutputInformation();

            LabelImagePixelType l_LabelImagePixelValueZero = static_cast<LabelImagePixelType>(0);
            // Label = 0 is background, start at 1
            vnsLogDebugMacro("Start loop for each CLD labels (nb = "<<m_CLDLabelsCount<<") and for each altitude (nb = "<<l_NumberOfAltitudes<<")...")
            itk::TimeProbe l_clock;
            l_clock.Start();
            for (LabelImagePixelType label = 1; label < (m_CLDLabelsCount + 1); label++)
            {

                vnsLogSuperDebugMacro("Starts processing the label '"<<label<<"' ...")

                // Map of label concatenated (merge) by altitudes
                // potential_shadows_zone.first  = position index pixels
                //                       .second = label
                LabelMapType potential_shadows_zone;

                potential_shadows_zone.clear();

                //KeepsCloudLabelFilterPointerType l_tot;
                typename KeepsCloudLabelFilterType::Pointer l_KeepsCloudLabelFilterType = KeepsCloudLabelFilterType::New();
                m_KeepsCloudLabelFilterPointerTypeList->PushBack(l_KeepsCloudLabelFilterType);

                // From the Labels image GetIPCLDSubLabelInput, generate image with only the Label "l" and 0 for others
                l_KeepsCloudLabelFilterType->SetInput(this->GetIPCLDSubLabelInput());
                l_KeepsCloudLabelFilterType->GetFunctor().SetKeepedValue(label);
                l_KeepsCloudLabelFilterType->GetFunctor().SetDefaultValue(l_LabelImagePixelValueZero);

                // Compute criterion for each cloud at each altitude
                for (unsigned short n = 0; n < l_NumberOfAltitudes; n++)
                {

                    const short l_DeltaH = m_DeltaHMin + n * m_DeltaHStep;
                    vnsLogSuperDebugMacro("---------- Altitude "<<l_DeltaH)

                    // used to defined the viewing direction in I3D (X,Y in the grid and Z=SOLH1)
                    typename ShadowProjectionFilterType::Pointer l_ShadowProjectionFilter = ShadowProjectionFilterType::New();
                    l_ShadowProjectionFilter->SetIPSOLInput(m_IPSOL1Input);
                    l_ShadowProjectionFilter->SetSOLHRef(m_SOLH1);
                    // Get the location of shadows
                    l_ShadowProjectionFilter->SetL2CLDSubLabelInput(l_KeepsCloudLabelFilterType->GetOutput());
                    l_ShadowProjectionFilter->SetDTMInput(this->GetDTMInput());
                    l_ShadowProjectionFilter->SetIPVIEInput(this->GetIPVIEInput());
                    l_ShadowProjectionFilter->SetDeltaH(static_cast<double>(l_DeltaH));

                    l_ShadowProjectionFilter->SetI3D(&m_I3D);

                    l_ShadowProjectionFilter->SetVIEHRef(m_VIEHRef);

                    l_ShadowProjectionFilter->Update();

                    // Get the map containing the list of shadow pixels and their associated labels
                    // shadows_zone_n.first  = position index pixels
                    //               .second = label
                    LabelMapType shadows_zone_n = l_ShadowProjectionFilter->GetShadowMap();

                    // Update l_LabelAltitudesShadowMap with the label detected for the current altitude
                    typename LabelMapType::iterator iter = shadows_zone_n.begin();
                    while (iter != shadows_zone_n.end())
                    {
                        potential_shadows_zone.insert((*iter));
                        ++iter;
                    }

                    // Free memory (necessary because 50 Go RAM with TV LANDSAT8 Backward !!)
//                    l_CLAConstantFilter = ITK_NULLPTR;
                    l_ShadowProjectionFilter = ITK_NULLPTR;

                } // end of altitude excursion loop

                // Generate the shadow mask with the shadow map (previously generated)
                typename ShadowMaskProjectionFilterType::Pointer l_ShadowMaskProjectionFilter = ShadowMaskProjectionFilterType::New();
                m_ShadowMaskProjectionFilterList->PushBack(l_ShadowMaskProjectionFilter);
                this->GetIPCLDSubLabelInput()->UpdateOutputInformation();

                l_ShadowMaskProjectionFilter->SetSize(this->GetIPCLDSubLabelInput()->GetLargestPossibleRegion().GetSize());
                l_ShadowMaskProjectionFilter->SetSpacing(this->GetIPCLDSubLabelInput()->GetSignedSpacing());
                l_ShadowMaskProjectionFilter->SetOrigin(this->GetIPCLDSubLabelInput()->GetOrigin());
                // Persistent filter
                l_ShadowMaskProjectionFilter->SetShadowMap(potential_shadows_zone);

                        // potential_shadows_zone
                LabelMaskingFilterPointerType  l_ShadowMaskCleaningFilter = LabelMaskingFilterType::New();
                l_ShadowMaskCleaningFilter->SetInput1(l_ShadowMaskProjectionFilter->GetOutput());
                l_ShadowMaskCleaningFilter->SetInput2(m_CLDandDarkFusionFilter->GetOutput());
                l_ShadowMaskCleaningFilter->GetFunctor().SetBackgroundValue(static_cast<MaskImagePixelType>(1));
                l_ShadowMaskCleaningFilter->GetFunctor().SetReplaceValue(static_cast<LabelImagePixelType>(0));
                l_ShadowMaskCleaningFilter->UpdateOutputInformation();
                m_ShadowMaskCleaningFilterList->PushBack(l_ShadowMaskCleaningFilter);


                m_LabeledShadowZoneOutputImageList->PushBack(l_ShadowMaskCleaningFilter->GetOutput());
                // Set for the label, the number of pixels detected in projection geo = size of the map (= nb first element)
                m_MapOfNbPixelsForEachLabel[label] = potential_shadows_zone.size();
                // Apply darkening threshold of the labeled image
                // find where an actual darkening is observed in the potential shadow zone
                // shadows_by_lab[lab] = where( potential_shadows_zone[lab] and  darkening < threshold)
                vnsLogSuperDebugMacro(
                        "Start DarkeningThreshold labeled image with label '"<<label<<"', darkening threshold '"<<m_DarkeningThreshold<<"' and l_LabelImagePixelValueZero '"<<l_LabelImagePixelValueZero<<"'...")
                typename DarkeningThresholdFilterType::Pointer l_DarkeningThresholdFilter = DarkeningThresholdFilterType::New();
                m_DarkeningThresholdFilterList->PushBack(l_DarkeningThresholdFilter);
                l_DarkeningThresholdFilter->SetInput1(l_ShadowMaskCleaningFilter->GetOutput());
                l_DarkeningThresholdFilter->SetInput2(this->GetDarkeningInput());
                l_DarkeningThresholdFilter->GetFunctor().SetLabelPixelValue(label);
                l_DarkeningThresholdFilter->GetFunctor().SetDarkeningThresholPixelValue(m_DarkeningThreshold);
                l_DarkeningThresholdFilter->GetFunctor().SetOutsidePixelValue(l_LabelImagePixelValueZero);
                // Launches update for images in memory
//                l_DarkeningThresholdFilter->Update();
                l_DarkeningThresholdFilter->UpdateOutputInformation();
                // shadows_by_lab[lab] = where( potential_shadows_zone[lab] and  darkening < threshold)
                m_LabeledShadowZoneDarkenedThresholdOutputImageList->PushBack(l_DarkeningThresholdFilter->GetOutput());
            } // end of labels loop
            l_clock.Stop();
            vnsLogDebugMacro("Start loop for each CLD labels (nb="<<m_CLDLabelsCount<<") and for each altitude (nb="<<l_NumberOfAltitudes<<") done (time is "<<vns::Utilities::GetStringFormatedTime(l_clock.GetTotal())<<").")
        }

    // PrintSelf method
    template<class TRatioImage, class TLabelImage, class TMaskImage, class TGRIDImage, class TCLAImage, class TDTMImage>
        void
        ComputesProjectionsMasksLabels<TRatioImage, TLabelImage, TMaskImage, TGRIDImage, TCLAImage, TDTMImage>::PrintSelf(std::ostream& os,
                itk::Indent indent) const
        {
            Superclass::PrintSelf(os, indent);

            os << "m_NoData:    " << m_NoData << std::endl;

            os << "m_DeltaHMax:    " << m_DeltaHMax << std::endl;
            os << "m_DeltaHMin:    " << m_DeltaHMin << std::endl;
            os << "m_DeltaHStep:    " << m_DeltaHStep << std::endl;

            os << "m_SOLH1:    " << m_SOLH1 << std::endl;

            os << "m_VIEHRef:    " << m_VIEHRef << std::endl;

        }

} // End namespace vns

#endif /* __vnsComputesProjectionsMasksLabels_txx */
