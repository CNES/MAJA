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
 * VERSION : 5-0-0 : FA : LAIG-FA-MAC-143764-CS : 12 avril 2016 : Mise en forme du code (indentation, etc.) *
 * VERSION : 4-0-0 : DM : LAIG-DM-MAC-517-CNES : 16 janvier 2014 : Améliorations algorithmiques (Spec v2.1)	*
 * 											Factorisation du calcul du critère de détection des ombres 		*
 * 											Correction : Calcul de la moyenne de l'assombrissement relatif	*
 * VERSION : 4-0-0 : <TypeFT> : <NumFT> : 6 janvier 2014 : Creation
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id: vnsShadowCriterionGenerator.txx 5749 2013-05-29 14:44:02Z tfeuvrie $
 *                                                                                                          *
 ************************************************************************************************************/
#ifndef __vnsShadowCriterionGenerator_txx
#define __vnsShadowCriterionGenerator_txx

#include "vnsShadowCriterionGenerator.h"

#include "vnsLoggers.h"

namespace vns
{

    // Constructor
    template<class TInputImage, class TDiffImage, class TLabelImage>
        ShadowCriterionGenerator<TInputImage, TDiffImage, TLabelImage>::ShadowCriterionGenerator() :
                m_LevelIndex(0), m_AbsNbpixThreshold(0), m_ShadowCriterionMap(), m_LevelMap()
        {
            // Parameters initialization
            m_NoData = static_cast<RealNoDataType>(0);

            // Filters initialization
            m_ValidShadowFilter = ValidShadowFilterType::New();

            m_L2inRCRInput = InputImageType::New();
            m_IPCLDShadLabelInput = LabelImageType::New();

        }

    // Destructor
    template<class TInputImage, class TDiffImage, class TLabelImage>
        ShadowCriterionGenerator<TInputImage, TDiffImage, TLabelImage>::~ShadowCriterionGenerator()
        {
        }

    template<class TInputImage, class TDiffImage, class TLabelImage>
        void
        ShadowCriterionGenerator<TInputImage, TDiffImage, TLabelImage>::SetShadowCriterionMap(DoubleMapType & map)
        {
            m_ShadowCriterionMap = map;
        }
    template<class TInputImage, class TDiffImage, class TLabelImage>
        typename ShadowCriterionGenerator<TInputImage, TDiffImage, TLabelImage>::DoubleMapType &
        ShadowCriterionGenerator<TInputImage, TDiffImage, TLabelImage>::GetShadowCriterionMap(void)
        {
            return m_ShadowCriterionMap;
        }

    template<class TInputImage, class TDiffImage, class TLabelImage>
        void
        ShadowCriterionGenerator<TInputImage, TDiffImage, TLabelImage>::SetLevelMap(UIMapType & map)
        {
            m_LevelMap = map;
        }
    template<class TInputImage, class TDiffImage, class TLabelImage>
        typename ShadowCriterionGenerator<TInputImage, TDiffImage, TLabelImage>::UIMapType &
        ShadowCriterionGenerator<TInputImage, TDiffImage, TLabelImage>::GetLevelMap(void)
        {
            return m_LevelMap;
        }

    // Main computation method.
    template<class TInputImage, class TDiffImage, class TLabelImage>
        void
        ShadowCriterionGenerator<TInputImage, TDiffImage, TLabelImage>::UpdateWithCaching()
        {
            LabelMapType l_ShadowMap;

            // Compute the number of pixels of each label and the number of pixel
            // of the associated shadow
            m_ValidShadowFilter->SetIPDiffInput(this->GetDiffInput());
            m_ValidShadowFilter->SetIPCLDShadLabel(this->GetIPCLDShadLabelInput());
            m_ValidShadowFilter->SetReflInput(this->GetL2inRCRInput());

            // The shadow band is directly set by the accessor
            m_ValidShadowFilter->SetExcludedValue(m_NoData);
            m_ValidShadowFilter->SetExcludedLabelValue(0);

            // Persistent filter
            m_ValidShadowFilter->Update();

            // Get the outputs of the ValidShadowFilter
            // For each label :
            // - the sum of the difference between the reflectance of the current
            // image and the composite one
            DoubleMapType l_SumOfDiffMap = m_ValidShadowFilter->GetSumOfPixels();

            // - the number of pixels of the associated shadow
            UIMapType l_NumberOfShadowPixelMap = m_ValidShadowFilter->GetNumberOfValidPixels();

            typename UIMapType::iterator l_NbShadIt = l_NumberOfShadowPixelMap.begin();

            // - the sum of reflectance (TOCR) for each shadow
            //DoubleMapType l_ReflSumMap =  m_ValidShadowFilter->GetReflSum();

            // Loop over shadows listed in the map
            while (l_NbShadIt != l_NumberOfShadowPixelMap.end())
            {

            	const unsigned int l_NumberOfShadowPixel = l_NbShadIt->second;
                const unsigned short l_Label = l_NbShadIt->first;

                std::ostringstream flux;
                flux << "==> Label " << l_Label << " - Number of shadow pixel " << l_NumberOfShadowPixel;

                // Compute radiometric detection only if the shadow pixel number is greater
                // than a threshold
                if (l_NumberOfShadowPixel >= m_AbsNbpixThreshold)
                {
                    // if (l_NumberOfShadowPixel == 0) corresponds to the case m_AbsNbpixThreshold==0, tested before
                    // Compute the mean of the relative darkening
                    const double l_ShadowCriterion = l_SumOfDiffMap[l_Label] / static_cast<double>(l_NumberOfShadowPixel);

                    typename DoubleMapType::iterator l_ShadCriterionIt = m_ShadowCriterionMap.find(l_Label);

                    // If the label doesn't exist yet
                    if (l_ShadCriterionIt == m_ShadowCriterionMap.end())
                    {

                        // Mean of the difference between the current image and the composite one
                        m_ShadowCriterionMap[l_Label] = l_ShadowCriterion;
                        // n = altitude level
                        m_LevelMap[l_Label] = m_LevelIndex;

                    }
                    // If the label already exist in the map, just add values
                    else
                    {

                        // Find the altitude for which the number of pixel in the label is greater
                        // than the threshold and the shadow criterion is minimal
                        // (search a minimum value because shadow criterion should be negative in
                        // case of shadow)
                        if (m_ShadowCriterionMap[l_Label] > l_ShadowCriterion)
                        {
                            m_ShadowCriterionMap[l_Label] = l_ShadowCriterion;
                            m_LevelMap[l_Label] = m_LevelIndex;
                        }

                    }

                    flux << " - Shadow criterion = " << l_ShadowCriterion;

                } // end if the number of valid pixel is sufficient

                vnsLogSuperDebugMacro(flux.str().c_str());

                l_NbShadIt++;

            } // end l_NumberOfShadowPixelMap loop

        }

    // PrintSelf method
    template<class TInputImage, class TDiffImage, class TLabelImage>
        void
        ShadowCriterionGenerator<TInputImage, TDiffImage, TLabelImage>::PrintSelf(std::ostream& os, itk::Indent indent) const
        {
            Superclass::PrintSelf(os, indent);

            os << "m_AbsNbpixThreshold:    " << m_AbsNbpixThreshold << std::endl;

            os << "m_ShadowBand:    " << static_cast<unsigned int>(m_ValidShadowFilter->GetShadowBand()) << std::endl;
        }

} // End namespace vns

#endif /* __vnsShadowCriterionGenerator_txx */
