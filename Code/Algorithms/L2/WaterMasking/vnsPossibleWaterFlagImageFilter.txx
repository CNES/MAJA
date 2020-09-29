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
 * VERSION : 5-1-0 : DM : LAIG-DM-MAC-1514-CNES : 29 juillet 2016 : Ajout NoData                            *
 * VERSION : 4-0-0 : DM : LAIG-DM-MAC-517-CNES : 19 décembre 2013 : Améliorations algorithmiques			*
 * 										Ajout d'un seuillage sur la pente et la réflectance dans le rouge   *
 * VERSION : 1-0-0 : <TypeFT> : <NumFT> : 20 juil. 2010 : Creation                                                           
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id$
 *                                                                                                          *
 ************************************************************************************************************/
#ifndef __vnsPossibleWaterFlagImageFilter_txx
#define __vnsPossibleWaterFlagImageFilter_txx

#include "vnsPossibleWaterFlagImageFilter.h"
#include "vnsMath.h"

namespace vns
{

    // Constructor
    template<class TInputImage, class TInputMask, class TImage, class TOutputImage>
        PossibleWaterFlagImageFilter<TInputImage, TInputMask, TImage, TOutputImage>::PossibleWaterFlagImageFilter() :
                m_RealL2NoData(0.), m_InitMode(false), m_SunglintFlag(false), m_RedBandTOCR(0), m_NIRBandTOCR(0), m_NIRBandRCR(0), m_WaterNDVIThreshold(
                        0.), m_WaterReflectanceVariationThreshold(0.), m_MaximumSunglintReflectance(0.), m_WaterSlopeThreshold(0.), m_WaterRedReflectanceThreshold(
                        0.)
        {

            // Input/Output initialization
            this->SetNumberOfIndexedInputs(6);
            this->SetNumberOfRequiredInputs(5);
            this->SetNumberOfIndexedOutputs(2);

            this->SetNthOutput(0, OutputImageType::New());
            this->SetNthOutput(1, OutputImageType::New());

        }

    // Destructor
    template<class TInputImage, class TInputMask, class TImage, class TOutputImage>
        PossibleWaterFlagImageFilter<TInputImage, TInputMask, TImage, TOutputImage>::~PossibleWaterFlagImageFilter()
        {
        }

    //====================== Main computation method ======================/
    template<class TInputImage, class TInputMask, class TImage, class TOutputImage>
        void
        PossibleWaterFlagImageFilter<TInputImage, TInputMask, TImage, TOutputImage>::ThreadedGenerateData(
                const OutputImageRegionType& outputRegionForThread, itk::ThreadIdType /*threadId*/)
        {
            InputImageInternalPixelType l_IPTOCRValue;
            InputImageInternalPixelType l_L2inRCRValue;

            // Load inputs
            InputImageConstPointer l_IPTOCRPtr = this->GetIPTOCRInput();
            InputMaskConstPointer l_IPEDGPtr = this->GetIPEDGInput();
            InputMaskConstPointer l_L2CLDSubSummaryPtr = this->GetL2CLDSubSummaryInput();
            InputMaskConstPointer l_IPSHDSubPtr = this->GetIPSHDSubInput();

            InputImageConstPointer l_L2inRCRPtr;

            ImageTypeConstPointer l_SlopeSubPtr = this->GetSlopeSubInput();

            // Load outputs
            OutputImagePointer l_IPPWAPtr = this->GetIPPWAOutput();
            OutputImagePointer l_IPTWAPtr = this->GetIPTWAOutput();

            // Prepare the threaded region input local iterators
            InputImageConstIterator l_IPTOCRIt(l_IPTOCRPtr, outputRegionForThread);
            MaskConstIterator l_IPEDGIt(l_IPEDGPtr, outputRegionForThread);
            MaskConstIterator l_L2CLDSubSummaryIt(l_L2CLDSubSummaryPtr, outputRegionForThread);
            MaskConstIterator l_IPSHDSubIt(l_IPSHDSubPtr, outputRegionForThread);
            ImageConstIterator l_SlopeSubIt(l_SlopeSubPtr, outputRegionForThread);

            InputImageConstIterator l_L2inRCRIt;

            // Initialization to the beginning
            l_IPTOCRIt.GoToBegin();
            l_IPEDGIt.GoToBegin();
            l_L2CLDSubSummaryIt.GoToBegin();
            l_IPSHDSubIt.GoToBegin();

            // Prepare the threaded region output local iterator
            OutputMaskIterator l_IPPWAIt(l_IPPWAPtr, outputRegionForThread);
            OutputMaskIterator l_IPTWAIt(l_IPTWAPtr, outputRegionForThread);

            // Initialization to the beginning
            l_IPPWAIt.GoToBegin();
            l_IPTWAIt.GoToBegin();

            if (m_InitMode == false)
            {

                // Get the L2 in products if init_mode = false
                l_L2inRCRPtr = this->GetL2inRCRInput();

                l_L2inRCRIt = InputImageConstIterator(l_L2inRCRPtr, outputRegionForThread);

                // Initialization to the beginning
                l_L2inRCRIt.GoToBegin();

                // Loop over the input and output images
                while ((l_IPTOCRIt.IsAtEnd() == false) && (l_L2inRCRIt.IsAtEnd() == false) && (l_IPEDGIt.IsAtEnd() == false)
                        && (l_IPSHDSubIt.IsAtEnd() == false) && (l_L2CLDSubSummaryIt.IsAtEnd() == false)
                        && (l_IPPWAIt.IsAtEnd() == false) && (l_IPTWAIt.IsAtEnd() == false))
                {

                    // Initialization
                    l_IPPWAIt.Set(static_cast<OutputImagePixelType>(0));
                    l_IPTWAIt.Set(static_cast<OutputImagePixelType>(0));

                    // If the pixel is not cloudy, no_data or cast shadow for date D
                    // and if the DEM slope is lower a specific threshold
                    if ((l_IPEDGIt.Get() == 0) && (l_L2CLDSubSummaryIt.Get() == 0) && (l_IPSHDSubIt.Get() == 0)
                            && (static_cast<double>(l_SlopeSubIt.Get()) <= m_WaterSlopeThreshold))
                    {

                        // Detection of low NDVI
                        LowNDVIDetection(l_IPTOCRIt, l_IPPWAIt, l_IPTWAIt);

                        if (m_SunglintFlag == true)
                        {
                            l_L2inRCRValue = l_L2inRCRIt.Get()[m_NIRBandRCR];
                            if ( vnsIsNotNoDataMacro(l_L2inRCRValue, m_RealL2NoData) == true)
                            {
                                l_IPTOCRValue = l_IPTOCRIt.Get()[m_NIRBandTOCR];

                                // Compute relative variation
                                const double l_Index = static_cast<double>(2. * vcl_abs(l_IPTOCRValue - l_L2inRCRValue)
                                / (l_IPTOCRValue + l_L2inRCRValue));

                                if ((l_Index > m_WaterReflectanceVariationThreshold) && (l_L2inRCRValue < m_MaximumSunglintReflectance))
                                {
                                    l_IPPWAIt.Set(static_cast<OutputImagePixelType>(1));
                                }

                            } // end test no_data D-1

                        } // end if init_mode false and sunglint flag raised

                    } // end if not cloudy, no_data and cast shadow

                    ++l_IPTOCRIt;
                    ++l_L2inRCRIt;
                    ++l_IPEDGIt;
                    ++l_L2CLDSubSummaryIt;
                    ++l_IPSHDSubIt;
                    ++l_IPPWAIt;
                    ++l_IPTWAIt;
                    ++l_SlopeSubIt;

                } // end loop pixel

            }
            // Init_mode is true
            else
            {

                // Loop over the input and output images
                while ((l_IPTOCRIt.IsAtEnd() == false) && (l_IPEDGIt.IsAtEnd() == false) && (l_L2CLDSubSummaryIt.IsAtEnd() == false)
                        && (l_IPSHDSubIt.IsAtEnd() == false) && (l_IPPWAIt.IsAtEnd() == false) && (l_IPTWAIt.IsAtEnd() == false))
                {
                    // Initialization
                    l_IPPWAIt.Set(static_cast<OutputImagePixelType>(0));
                    l_IPTWAIt.Set(static_cast<OutputImagePixelType>(0));

                    // If the pixel is not cloudy, no_data or cast shadow for date D
                    // and if the DEM slope is lower a specific threshold
                    if ((l_IPEDGIt.Get() == 0) && (l_L2CLDSubSummaryIt.Get() == 0) && (l_IPSHDSubIt.Get() == 0)
                            && (static_cast<double>(l_SlopeSubIt.Get()) <= m_WaterSlopeThreshold))
                    {

                        // Detection of low NDVI
                        LowNDVIDetection(l_IPTOCRIt, l_IPPWAIt, l_IPTWAIt);

                    } // end if not cloudy, no_data and cast shadow

                    ++l_IPTOCRIt;
                    ++l_IPEDGIt;
                    ++l_L2CLDSubSummaryIt;
                    ++l_IPSHDSubIt;
                    ++l_IPPWAIt;
                    ++l_IPTWAIt;
                    ++l_SlopeSubIt;

                } //end while

            } // end else (m_InitMode == false)

        }

    template<class TInputImage, class TInputMask, class TImage, class TOutputImage>
        void
        PossibleWaterFlagImageFilter<TInputImage, TInputMask, TImage, TOutputImage>::LowNDVIDetection(
                const InputImageConstIterator & p_IPTOCRIt, OutputMaskIterator & p_IPPWAIt, OutputMaskIterator & p_IPTWAIt)
        {
            // The pixel is tested
            p_IPTWAIt.Set(static_cast<OutputImagePixelType>(1));

            // Compute NDVI indice
            itk::VariableLengthVector<double> v(2);
            v[0] = p_IPTOCRIt.Get()[m_RedBandTOCR];
            v[1] = p_IPTOCRIt.Get()[m_NIRBandTOCR];
            m_NDVIFunctor.SetBandIndex(CommonBandNames::RED, 1);
            m_NDVIFunctor.SetBandIndex(CommonBandNames::NIR, 2);
            const InputImageInternalPixelType l_NDVIValue = m_NDVIFunctor(v);

            // A pixel is declared as "possible water" if :
            // - it is dark and not located within a topographic shadow
            // - the red reflectance of pixels with low enough NDVI is lower than a specific threshold
            if ((l_NDVIValue < m_WaterNDVIThreshold) && (p_IPTOCRIt.Get()[m_RedBandTOCR] < m_WaterRedReflectanceThreshold))
            {
                p_IPPWAIt.Set(static_cast<OutputImagePixelType>(1));
            }

        }

    // PrintSelf method
    template<class TInputImage, class TInputMask, class TImage, class TOutputImage>
        void
        PossibleWaterFlagImageFilter<TInputImage, TInputMask, TImage, TOutputImage>::PrintSelf(std::ostream& os, itk::Indent indent) const
        {
            Superclass::PrintSelf(os, indent);
            os << "m_InitMode :     " << m_InitMode << std::endl;
            os << "m_SunglintFlag :     " << m_SunglintFlag << std::endl;
            os << "m_RedBandTOCR :          " << m_RedBandTOCR << std::endl;
            os << "m_NIRBandTOCR :          " << m_NIRBandTOCR << std::endl;
            os << "m_NIRBandRCR :          " << m_NIRBandRCR << std::endl;
            os << "m_WaterNDVIThreshold :     " << m_WaterNDVIThreshold << std::endl;
            os << "m_WaterSlopeThreshold :     " << m_WaterSlopeThreshold << std::endl;
            os << "m_WaterRedReflectanceThreshold :     " << m_WaterRedReflectanceThreshold << std::endl;
            os << "m_WaterReflectanceVariationThreshold :     " << m_WaterReflectanceVariationThreshold << std::endl;
            os << "m_MaximumSunglintReflectance :     " << m_MaximumSunglintReflectance << std::endl;

        }

} // End namespace vns

#endif /* __vnsPossibleWaterFlagImageFilter_txx */
