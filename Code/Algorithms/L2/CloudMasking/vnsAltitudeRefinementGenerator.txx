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
 * VERSION : 5-0-0 : FA : LAIG-FA-MAC-143764-CS : 12 avril 2016 : Mise en forme du code (indentation, etc.) *
 * VERSION : 4-0-0 : DM : LAIG-DM-MAC-517-CNES : 16 janvier 2014 : Améliorations algorithmiques (Spec v2.1)	*
 * VERSION : 4-0-0 : <TypeFT> : <NumFT> : 16 janvier 2014 : Creation
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id: vnsAltitudeRefinementGenerator.txx 5749 2013-05-29 14:44:02Z tfeuvrie $
 *                                                                                                          *
 ************************************************************************************************************/
#ifndef __vnsAltitudeRefinementGenerator_txx
#define __vnsAltitudeRefinementGenerator_txx

#include "vnsAltitudeRefinementGenerator.h"

#include "otbImageFileWriter.h"

namespace vns
{

    // Constructor
    template<class TInputImage, class TDiffImage, class TInputMask, class TLabelImage, class TGRIDImage, class TCLAImage, class TDTMImage>
        AltitudeRefinementGenerator<TInputImage, TDiffImage, TInputMask, TLabelImage, TGRIDImage, TCLAImage, TDTMImage>::AltitudeRefinementGenerator() :
                m_LevelMap(), m_ShadowCriterionMap(), m_AltitudeCorrectionMap()
        {

            // Parameters initialization
            m_NoData = static_cast<RealNoDataType>(0);

            m_DeltaHMax = 0;
            m_DeltaHMin = 0;
            m_DeltaHStep = 0;
            m_SOLH1 = 0.;
            m_SOLH2 = 0.;
            m_VIEHRef = 0.;
            m_SOLHRef = 0.;
            m_AbsNbpixThreshold = 0;
            m_ThresholdDiffImage = 0.;
            m_ShadowBand = 0;

            // Filters initialization
            m_SolarFilter = SolarFilterType::New();
            m_ShadowProjectionFilter = ShadowProjectionFilterType::New();
            m_ShadowMaskProjectionFilter = ShadowMaskProjectionFilterType::New();
            m_AddConstantFilter = AddConstantFilterType::New();

            m_ShadowCriterionFilterList = ShadowCriterionGeneratorListType::New();

            m_FinalSolarFilter = SolarFilterType::New();


            m_L2inRCRInput = InputImageType::New();

            m_IPCLDSubInput = InputMaskType::New();
            m_IPCLAInput = CLAImageType::New();
            m_IPCLDSubLabelInput = LabelImageType::New();

            m_DTMInput = DTMImageType::New();

            m_IPVIEInput = GRIDImageType::New();
            m_IPSOL1Input = GRIDImageType::New();
            //m_IPSOL2Input = GRIDImageType::New();

            // Init L1 image spacing
            m_I3D = NULL;

        }

    // Destructor
    template<class TInputImage, class TDiffImage, class TInputMask, class TLabelImage, class TGRIDImage, class TCLAImage, class TDTMImage>
        AltitudeRefinementGenerator<TInputImage, TDiffImage, TInputMask, TLabelImage, TGRIDImage, TCLAImage, TDTMImage>::~AltitudeRefinementGenerator()
        {
        }

    //================================== GenerateData ==================================/
    // Main computation method.
    template<class TInputImage, class TDiffImage, class TInputMask, class TLabelImage, class TGRIDImage, class TCLAImage, class TDTMImage>
        void
        AltitudeRefinementGenerator<TInputImage, TDiffImage, TInputMask, TLabelImage, TGRIDImage, TCLAImage, TDTMImage>::UpdateWithCaching()
        {
            LabelMapType l_ShadowMap;

            if (m_DeltaHStep == 0)
            {
                vnsStaticExceptionBusinessMacro("The step used for the altitude excursion (Delta_H_Step or Local_Delat_H_Step) is null\n");
            }

            const unsigned short l_NumberOfAltitudes(static_cast<unsigned short>(((m_DeltaHMax - m_DeltaHMin) / m_DeltaHStep) + 1));

            vnsLogDebugMacro("m_DeltaHMax: "<<m_DeltaHMax<<" m_DeltaHMin: "<<m_DeltaHMin<<" => l_NumberOfAltitudes: "<<l_NumberOfAltitudes)

            ShadowCriterionFilterPointerType l_ShadowCriterionFilter;

            // Compute radiometric criterion for each cloud at each altitude
            for (unsigned short n = 0; n < l_NumberOfAltitudes; n++)
            {

                const short l_DeltaH = m_DeltaHMin + n * m_DeltaHStep;
                vnsLogDebugMacro("oooooooo Altitude "<<l_DeltaH);

                // Add the excursion altitude to the cloud altitude
                // estimated by stereoscopy
                m_AddConstantFilter->SetInput(this->GetIPCLAInput());
                m_AddConstantFilter->SetConstant(l_DeltaH);

                if (m_IPSOL2Input.IsNull() == true)
                {
                    // used to defined the viewing direction in I3D (X,Y in the grid and Z=SOLH1)
                    m_ShadowProjectionFilter->SetIPSOLInput(m_IPSOL1Input);
                    m_ShadowProjectionFilter->SetSOLHRef(m_SOLH1);
                }
                else
                {
                    // Estimate the solar direction for each cloud taking into
                    // account the tested altitude
                    m_SolarFilter->SetInput1(this->GetIPSOL1Input());
                    m_SolarFilter->SetInput2(this->GetIPSOL2Input());
                    m_SolarFilter->SetInput3(m_AddConstantFilter->GetOutput());

                    m_SolarFilter->GetFunctor().SetH1(m_SOLH1);
                    m_SolarFilter->GetFunctor().SetH2(m_SOLH2);
                    m_SolarFilter->GetFunctor().SetSOLHRef(m_SOLHRef);

                    m_ShadowProjectionFilter->SetIPSOLInput(m_SolarFilter->GetOutput());
                    m_ShadowProjectionFilter->SetSOLHRef(m_SOLHRef);
                }
                // Get the location of shadows
                m_ShadowProjectionFilter->SetL2CLDSubInput(this->GetIPCLDSubInput());
                m_ShadowProjectionFilter->SetL2CLDSubLabelInput(this->GetIPCLDSubLabelInput());
                m_ShadowProjectionFilter->SetDTMInput(this->GetDTMInput());
                m_ShadowProjectionFilter->SetIPVIEInput(this->GetIPVIEInput());
                m_ShadowProjectionFilter->SetIPCLAInput(m_AddConstantFilter->GetOutput());

                m_ShadowProjectionFilter->SetI3D(m_I3D);

                m_ShadowProjectionFilter->SetVIEHRef(m_VIEHRef);

                m_ShadowProjectionFilter->Update();

                // Get the map containing the list of shadow pixels and their associated labels
                l_ShadowMap = m_ShadowProjectionFilter->GetShadowMap();

                // Generate the shadow mask with the shadow map (previously generated)
                this->GetIPCLDSubInput()->UpdateOutputInformation();
                m_ShadowMaskProjectionFilter->SetSize(this->GetIPCLDSubInput()->GetLargestPossibleRegion().GetSize());
                m_ShadowMaskProjectionFilter->SetSpacing(this->GetIPCLDSubInput()->GetSignedSpacing());
                m_ShadowMaskProjectionFilter->SetOrigin(this->GetIPCLDSubInput()->GetOrigin());

                // Persistent filter
                m_ShadowMaskProjectionFilter->SetShadowMap(l_ShadowMap);

                // Get the shadow criterion
                l_ShadowCriterionFilter = ShadowCriterionFilterType::New();

                // First excursion => initialize the shadow criterion map
                if (m_ShadowCriterionFilterList->Size() == 0)
                {
                    l_ShadowCriterionFilter->SetShadowCriterionMap(m_ShadowCriterionMap);
                    l_ShadowCriterionFilter->SetLevelMap(m_LevelMap);
                }
                // Next excursion => update the existing shadow criterion map
                else
                {
                    l_ShadowCriterionFilter->SetShadowCriterionMap(m_ShadowCriterionFilterList->Back()->GetShadowCriterionMap());
                    l_ShadowCriterionFilter->SetLevelMap(m_ShadowCriterionFilterList->Back()->GetLevelMap());
                }

                l_ShadowCriterionFilter->SetL2inRCRInput(this->GetL2inRCRInput());
                l_ShadowCriterionFilter->SetDiffInput(this->GetDiffInput());
                l_ShadowCriterionFilter->SetIPCLDShadLabelInput(m_ShadowMaskProjectionFilter->GetOutput());

                l_ShadowCriterionFilter->SetNoData(m_NoData);
                l_ShadowCriterionFilter->SetLevelIndex(n);
                l_ShadowCriterionFilter->SetAbsNbpixThreshold(m_AbsNbpixThreshold);
                l_ShadowCriterionFilter->SetShadowBand(m_ShadowBand);

                l_ShadowCriterionFilter->UpdateWithCaching();

                // The filter pointer is local => we have to keep the pointer in a list
                m_ShadowCriterionFilterList->PushBack(l_ShadowCriterionFilter);

            } // end of altitude excursion loop

            // Get the last updated shadow criterion map
            m_ShadowCriterionMap = m_ShadowCriterionFilterList->Back()->GetShadowCriterionMap();
            m_LevelMap = m_ShadowCriterionFilterList->Back()->GetLevelMap();

            // For each cloud label, select the altitude for which the shadow criterion is
            // lower than a fixed threshold

            m_AltitudeCorrectionMap[0] = 0;
            typename DoubleMapType::iterator l_ShadCriterionIt = m_ShadowCriterionMap.begin();
            typename UIMapType::iterator l_LevelIt = m_LevelMap.begin();

            while (l_ShadCriterionIt != m_ShadowCriterionMap.end())
            {
                if (l_ShadCriterionIt->second < -m_ThresholdDiffImage)
                {
                    // Alt_correction(label) = Delta_H.min + n_optim*Delta_H.step
                    m_AltitudeCorrectionMap[l_ShadCriterionIt->first] = static_cast<double>(m_DeltaHMin)
                            + static_cast<double>(l_LevelIt->second) * static_cast<double>(m_DeltaHStep);
                }
                else
                {
                    m_AltitudeCorrectionMap[l_ShadCriterionIt->first] = 0.;
                }
                vnsLogDebugMacro(
                        "==> Final altitude : Label "<<l_ShadCriterionIt->first<< " - Criterion "<<l_ShadCriterionIt->second <<" => "<<m_AltitudeCorrectionMap[l_ShadCriterionIt->first]);

                l_ShadCriterionIt++;
                l_LevelIt++;
            }

        }

    // PrintSelf method
    template<class TInputImage, class TDiffImage, class TInputMask, class TLabelImage, class TGRIDImage, class TCLAImage, class TDTMImage>
        void
        AltitudeRefinementGenerator<TInputImage, TDiffImage, TInputMask, TLabelImage, TGRIDImage, TCLAImage, TDTMImage>::PrintSelf(
                std::ostream& os, itk::Indent indent) const
        {
            Superclass::PrintSelf(os, indent);

            os << "m_NoData:    " << m_NoData << std::endl;

            os << "m_DeltaHMax:    " << m_DeltaHMax << std::endl;
            os << "m_DeltaHMin:    " << m_DeltaHMin << std::endl;
            os << "m_DeltaHStep:    " << m_DeltaHStep << std::endl;

            os << "m_SOLH1:    " << m_SOLH1 << std::endl;
            os << "m_SOLH2:    " << m_SOLH2 << std::endl;
            os << "m_SOLHRef:    " << m_SOLHRef << std::endl;

            os << "m_VIEHRef:    " << m_VIEHRef << std::endl;

            os << "m_AbsNbpixThreshold:    " << m_AbsNbpixThreshold << std::endl;
            os << "m_ThresholdDiffImage:    " << m_ThresholdDiffImage << std::endl;

            os << "m_ShadowBand:    " << m_ShadowBand << std::endl;
        }

} // End namespace vns

#endif /* __vnsAltitudeRefinementGenerator_txx */
