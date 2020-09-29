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
 * VERSION : 5-1-0 : FA : LAIG-FA-MAC-144674-CS : 26 mai 2016 : Modifications cosmetique et menage, etc...  *
 * VERSION : 5-0-0 : FA : LAIG-FA-MAC-143764-CS : 12 avril 2016 : Mise en forme du code (indentation, etc.) *
 * VERSION : 4-0-0 : DM : LAIG-DM-MAC-517-CNES : 16 janvier 2014 : Améliorations algorithmiques (Spec v2.1)	*
 * VERSION : 1-0-0-3 : DM : 357 : 19 sept. 2011 : modification du calcul du Shadow_Criterion                  *
 * VERSION : 1-0-0-1 : 61916 : 15 juin 2011 : Correction bug, variable m_SOLHRef non init                     *
 * VERSION : 1-0-0 : <TypeFT> : <NumFT> : 18 juin 2010 : Creation                                                           
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id$
 *                                                                                                          *
 ************************************************************************************************************/
#ifndef __vnsShadowMaskDeterminationImageFilter_txx
#define __vnsShadowMaskDeterminationImageFilter_txx

#include "vnsShadowMaskDeterminationImageFilter.h"

#include "otbImageFileWriter.h"
#include "vnsMacro.h"

namespace vns
{

    // Constructor
    template<class TInputImage, class TInputMask, class TLabelImage, class TGRIDImage, class TCLAImage, class TDTMImage, class TOutputImage>
        ShadowMaskDeterminationImageFilter<TInputImage, TInputMask, TLabelImage, TGRIDImage, TCLAImage, TDTMImage, TOutputImage>::ShadowMaskDeterminationImageFilter() :
                m_InitMode(false), m_RefinementOption(true), m_DeltaHMax(0), m_DeltaHMin(0), m_DeltaHStep(0), m_SOLH1(
                        0.), m_SOLH2(0.), m_VIEHRef(0.), m_SOLHRef(0.), m_AbsNbpixThreshold(0), m_ThresholdDiffImage(0), m_ShadowBandTOCR(
                        0), m_ShadowBandRCR(0)
        {

            // Parameters initialization
            m_NoData = static_cast<RealNoDataType>(0);

            // Filters initialization
            m_DifferenceFilter = DifferenceFilterType::New();
            m_AltitudeRefinementFilterList = AltitudeRefinementFilterListType::New();
            m_AltitudeCorrectionFilterList = AltCorrFilterListType::New();

            m_ImageUpdatingFilterList = UpdatingFilterListType::New();
            m_NoDataShadowFilter = NoDataShadowImageFilterType::New();
            m_CloudLabelRemovalFilter = CloudLabelRemovalFilterType::New();
            m_LastCLAUpdatingFilter = UpdatingWithMaskFilterType::New();

            m_SolarFilter = SolarFilterType::New();
            m_ShadowProjectionFilter = ShadowProjectionFilterType::New();
            m_FinalShadowProjectionFilter = ShadowProjectionFilterType::New();

            m_FinalShadowMaskProjectionFilter = ShadowMaskProjectionFilterType::New();
            m_FinalSolarFilter = SolarFilterType::New();
            m_ThresholdFilter = ThresholdFilterType::New();

            m_IPTOCRInput = InputImageType::New();
            m_L2inRCRInput = InputImageType::New();
            m_IPEDGInput = InputMaskType::New();
            m_L2inNDTInput = InputMaskType::New();
            m_L2inWASInput = InputMaskType::New();
            m_L2CLDSubInput = InputMaskType::New();
            m_IPCLAInput = CLAImageType::New();
            m_L2CLDSubLabelInput = LabelImageType::New();

            m_DTMInput = DTMImageType::New();

            m_IPVIEInput = GRIDImageType::New();
            m_IPSOL1Input = GRIDImageType::New();
            //m_IPSOL2Input = GRIDImageType::New();

            m_IPCLDShadOutput = OutputImageType::New();
            m_IPCLAOutput = CLAImageType::New();

        }

    // Destructor
    template<class TInputImage, class TInputMask, class TLabelImage, class TGRIDImage, class TCLAImage, class TDTMImage, class TOutputImage>
        ShadowMaskDeterminationImageFilter<TInputImage, TInputMask, TLabelImage, TGRIDImage, TCLAImage, TDTMImage, TOutputImage>::~ShadowMaskDeterminationImageFilter()
        {
        }

    //================================== GenerateData ==================================/
    // Main computation method.
    template<class TInputImage, class TInputMask, class TLabelImage, class TGRIDImage, class TCLAImage, class TDTMImage, class TOutputImage>
        void
        ShadowMaskDeterminationImageFilter<TInputImage, TInputMask, TLabelImage, TGRIDImage, TCLAImage, TDTMImage, TOutputImage>::UpdateWithCaching()
        {
            // Load the DTM image
            m_I3D.Load(this->GetDTMInput());

            // m_AbsNbpixThreshold == 0 not authorized if init mode false and refinement option true
            if ((m_InitMode == false) && (m_RefinementOption == true) && (m_AbsNbpixThreshold == 0))
            {
                vnsExceptionBusinessMacro("Init Mode False - Refinement Option True - Abs_Nbpix_Threshold is null.");
            }

            // Conversion of parameters that are in percent
            m_ThresholdDiffImage = m_ThresholdDiffImage / static_cast<double>(100.);

            // STANDARD MODE and REFINEMENT OF CLOUD ALTITUDE

            if ((m_InitMode == false) && (m_RefinementOption == true))
            {
                // Get the number of altitudes
                if (m_DeltaHStep == 0)
                {
                    vnsExceptionBusinessMacro("m_deltaHStep is null.");
                }

                // Compute the difference between the current image and the composite image
                // this value should be negative when a shadow affects the image of date D
                m_DifferenceFilter->SetIPTOCRInput(this->GetIPTOCRInput());
                m_DifferenceFilter->SetL2inRCRInput(this->GetL2inRCRInput());
                m_DifferenceFilter->SetL2CLDSubInput(this->GetL2CLDSubInput());
                m_DifferenceFilter->SetIPEDGInput(this->GetIPEDGInput());
                m_DifferenceFilter->SetL2inWASInput(this->GetL2inWASInput());
                m_DifferenceFilter->SetL2inNDTInput(this->GetL2inNDTInput());

                m_DifferenceFilter->SetNoData(m_NoData);
                m_DifferenceFilter->SetShadowBandTOCR(m_ShadowBandTOCR);
                m_DifferenceFilter->SetShadowBandRCR(m_ShadowBandRCR);

                // MACCS 5.0
                // Global refinement : Find an altitude for each cloud label
                // For sensor other than Venµs, there is only one label at this stage
                GlobalRefinementProcess();


                m_IPCLAOutput = m_AltitudeCorrectionFilterList->Back()->GetOutput();

            } // end if m_InitMode == false || m_RefinementOption == true

            // INIT MODE or NO REFINEMENT OF CLOUD ALTITUDE

            else
            {
                m_IPCLAOutput = this->GetIPCLAInput();
            }

            // ===============================================================================
            // Final projection of clouds
            // ===============================================================================
            FinalCloudProjection();

        }

    // Global refinement process method
    template<class TInputImage, class TInputMask, class TLabelImage, class TGRIDImage, class TCLAImage, class TDTMImage, class TOutputImage>
        void
        ShadowMaskDeterminationImageFilter<TInputImage, TInputMask, TLabelImage, TGRIDImage, TCLAImage, TDTMImage, TOutputImage>::GlobalRefinementProcess()
        {
            vnsLogDebugMacro("======> Global refinement ");

            AltitudeRefinementFilterPointerType l_AltitudeRefinementFilter = AltitudeRefinementFilterType::New();
            AltCorrFilterPointerType l_AltitudeCorrectionFilter = AltCorrFilterType::New();

            l_AltitudeRefinementFilter->SetIPCLDSubInput(this->GetL2CLDSubInput());
            l_AltitudeRefinementFilter->SetIPCLDSubLabelInput(this->GetL2CLDSubLabelInput());
            l_AltitudeCorrectionFilter->SetInput2(this->GetL2CLDSubLabelInput());

            // GLOBAL REFINEMENT : Call the altitude refinement algorithm
            l_AltitudeRefinementFilter->SetL2inRCRInput(this->GetL2inRCRInput());
            l_AltitudeRefinementFilter->SetDiffInput(m_DifferenceFilter->GetOutput());
            l_AltitudeRefinementFilter->SetIPVIEInput(this->GetIPVIEInput());
            l_AltitudeRefinementFilter->SetIPSOL1Input(this->GetIPSOL1Input());
            if (m_IPSOL2Input.IsNull() == false)
            {
                l_AltitudeRefinementFilter->SetIPSOL2Input(this->GetIPSOL2Input());
            }
            l_AltitudeRefinementFilter->SetIPCLAInput(this->GetIPCLAInput());
            l_AltitudeRefinementFilter->SetDTMInput(this->GetDTMInput());
            l_AltitudeRefinementFilter->SetI3D(&m_I3D);

            // Set parameters
            l_AltitudeRefinementFilter->SetNoData(m_NoData);
            l_AltitudeRefinementFilter->SetDeltaHMax(m_DeltaHMax);
            l_AltitudeRefinementFilter->SetDeltaHMin(m_DeltaHMin);
            l_AltitudeRefinementFilter->SetDeltaHStep(m_DeltaHStep);
            l_AltitudeRefinementFilter->SetSOLH1(m_SOLH1);
            l_AltitudeRefinementFilter->SetSOLH2(m_SOLH2);
            l_AltitudeRefinementFilter->SetVIEHRef(m_VIEHRef);
            l_AltitudeRefinementFilter->SetSOLHRef(m_SOLHRef);
            l_AltitudeRefinementFilter->SetAbsNbpixThreshold(m_AbsNbpixThreshold);
            l_AltitudeRefinementFilter->SetThresholdDiffImage(m_ThresholdDiffImage);
            l_AltitudeRefinementFilter->SetShadowBand(m_ShadowBandRCR); // m_ShadowBandRCR because only by the L2inRCRInput data

            l_AltitudeRefinementFilter->UpdateWithCaching();

            m_AltitudeRefinementFilterList->PushBack(l_AltitudeRefinementFilter);

            // Correction of the altitude after the global refinement
            // The image of label is set at the beginning of the "iter" loop
            // h_stereo_sub + alt_global_correction = h_stereo_sub + (Delta_H.min + n_optim*Delta_H.step)
            l_AltitudeCorrectionFilter->SetInput1(this->GetIPCLAInput());
            l_AltitudeCorrectionFilter->GetFunctor().SetMap(l_AltitudeRefinementFilter->GetAltitudeCorrectionMap());

            m_AltitudeCorrectionFilterList->PushBack(l_AltitudeCorrectionFilter);
        }


    // Final projection of the cloud with last estimation of the cloud altitude
    template<class TInputImage, class TInputMask, class TLabelImage, class TGRIDImage, class TCLAImage, class TDTMImage, class TOutputImage>
        void
        ShadowMaskDeterminationImageFilter<TInputImage, TInputMask, TLabelImage, TGRIDImage, TCLAImage, TDTMImage, TOutputImage>::FinalCloudProjection()
        {

            vnsLogDebugMacro("======> Final cloud projection ");

            LabelMapType l_ShadowMap;

            if (m_IPSOL2Input.IsNull() == true)
            {
                m_FinalShadowProjectionFilter->SetIPSOLInput(m_IPSOL1Input);
                m_FinalShadowProjectionFilter->SetSOLHRef(m_SOLH1);
                m_IPSOL1Input->UpdateOutputInformation();
            }
            else
            {
                // Estimate the solar direction for each cloud taking into
                // account the corrected altitude
                m_FinalSolarFilter->SetInput1(this->GetIPSOL1Input());
                m_FinalSolarFilter->SetInput2(this->GetIPSOL2Input());
                m_FinalSolarFilter->SetInput3(m_IPCLAOutput);

                m_FinalSolarFilter->GetFunctor().SetH1(m_SOLH1);
                m_FinalSolarFilter->GetFunctor().SetH2(m_SOLH2);
                m_FinalSolarFilter->GetFunctor().SetSOLHRef(m_SOLHRef);

                m_FinalShadowProjectionFilter->SetIPSOLInput(m_FinalSolarFilter->GetOutput());
                m_FinalShadowProjectionFilter->SetSOLHRef(m_SOLHRef); //m_IPCLAOutput);

                m_FinalSolarFilter->GetOutput()->UpdateOutputInformation();
            }
            // Projection of each cloud at each corrected altitude for
            // obtaining the final shadow mask
            this->GetL2CLDSubInput()->UpdateOutputInformation();
            this->GetL2CLDSubLabelInput()->UpdateOutputInformation();
            this->GetDTMInput()->UpdateOutputInformation();
            this->GetIPVIEInput()->UpdateOutputInformation();
            m_IPCLAOutput->UpdateOutputInformation();

            m_FinalShadowProjectionFilter->SetL2CLDSubInput(this->GetL2CLDSubInput());
            m_FinalShadowProjectionFilter->SetL2CLDSubLabelInput(this->GetL2CLDSubLabelInput());
            m_FinalShadowProjectionFilter->SetDTMInput(this->GetDTMInput());
            m_FinalShadowProjectionFilter->SetIPVIEInput(this->GetIPVIEInput());
            m_FinalShadowProjectionFilter->SetIPCLAInput(m_IPCLAOutput);
            m_FinalShadowProjectionFilter->SetI3D(&m_I3D);
            m_FinalShadowProjectionFilter->SetVIEHRef(m_VIEHRef);

            // Persistent filter update
            m_FinalShadowProjectionFilter->Update();

            // Get the map containing the list of shadow pixels and their associated labels
            l_ShadowMap = m_FinalShadowProjectionFilter->GetShadowMap();

            // Generate the shadow mask with the shadow map
            m_FinalShadowMaskProjectionFilter->SetSize(this->GetL2CLDSubInput()->GetLargestPossibleRegion().GetSize());
            m_FinalShadowMaskProjectionFilter->SetSpacing(this->GetL2CLDSubInput()->GetSignedSpacing());
            m_FinalShadowMaskProjectionFilter->SetOrigin(this->GetL2CLDSubInput()->GetOrigin());
            m_FinalShadowMaskProjectionFilter->SetShadowMap(l_ShadowMap);

            // Threshold the shadow mask that contains label values to generate
            // a binary shadow mask
            m_ThresholdFilter->SetInput(m_FinalShadowMaskProjectionFilter->GetOutput());
            m_ThresholdFilter->GetFunctor().SetThresholdValue(0);
            m_ThresholdFilter->GetFunctor().SetInsideValue(0);
            m_ThresholdFilter->GetFunctor().SetOutputValue(1);

            m_IPCLDShadOutput = m_ThresholdFilter->GetOutput();
        }

    // PrintSelf method
    template<class TInputImage, class TInputMask, class TLabelImage, class TGRIDImage, class TCLAImage, class TDTMImage, class TOutputImage>
        void
        ShadowMaskDeterminationImageFilter<TInputImage, TInputMask, TLabelImage, TGRIDImage, TCLAImage, TDTMImage, TOutputImage>::PrintSelf(
                std::ostream& os, itk::Indent indent) const
        {
            Superclass::PrintSelf(os, indent);

            // Display the parameters
            os << "m_NoData:    " << m_NoData << std::endl;
            os << "m_InitMode:    " << m_InitMode << std::endl;
            os << "m_RefinementOption:    " << m_RefinementOption << std::endl;

            // Parameter used for the altitude refinement
            os << "m_DeltaHMax:    " << m_DeltaHMax << std::endl;
            os << "m_DeltaHMin:    " << m_DeltaHMin << std::endl;
            os << "m_DeltaHStep:    " << m_DeltaHStep << std::endl;

            // Different solar altitudes
            os << "m_SOLH1:    " << m_SOLH1 << std::endl;
            os << "m_SOLH2:    " << m_SOLH2 << std::endl;
            os << "m_SOLHRef:    " << m_SOLHRef << std::endl;

            // Viewing reference altitude
            os << "m_VIEHRef:    " << m_VIEHRef << std::endl;

            os << "m_AbsNbpixThreshold:    " << m_AbsNbpixThreshold << std::endl;
            os << "m_ThresholdDiffImage:    " << m_ThresholdDiffImage << std::endl;

            os << "m_ShadowBandTOCR:    " << m_ShadowBandTOCR << std::endl;
            os << "m_ShadowBandRCR:    " << m_ShadowBandRCR << std::endl;
        }

} // End namespace vns

#endif /* __vnsShadowMaskDeterminationImageFilter_txx */
