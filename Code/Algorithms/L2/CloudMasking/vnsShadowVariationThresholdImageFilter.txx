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
 * VERSION : 1-0-0-3 : DM : 357 : 02 sep. 2011 : pas de correlation avec la date courante                     *
 * VERSION : 1-0-0 : <TypeFT> : <NumFT> : 7 juin 2010 : Creation                                                           
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id$
 *                                                                                                          *
 ************************************************************************************************************/
#ifndef __vnsShadowVariationThresholdImageFilter_txx
#define __vnsShadowVariationThresholdImageFilter_txx

#include "vnsShadowVariationThresholdImageFilter.h"

#include "itkImageRegionIterator.h"
#include "itkImageRegionConstIterator.h"

namespace vns
{

    /** Constructor */
    template<class TInputImage, class TInputMask, class TOutputImage>
        ShadowVariationThresholdImageFilter<TInputImage, TInputMask, TOutputImage>::ShadowVariationThresholdImageFilter()
        {
            // Input/Output initialization
            this->SetNumberOfIndexedInputs(2);
            this->SetNumberOfRequiredInputs(2);
            this->SetNumberOfIndexedOutputs(1);

            this->SetNthOutput(0, OutputImageType::New());

            // Parameters initialization
            m_NoData = static_cast<RealNoDataType>(0);
            m_ThresholdValue = .4;

        }

    /** Destructor */
    template<class TInputImage, class TInputMask, class TOutputImage>
        ShadowVariationThresholdImageFilter<TInputImage, TInputMask, TOutputImage>::~ShadowVariationThresholdImageFilter()
        {
        }

    //****************************** ThreadedGenerateData ********************************/
    /** ThreadedGenerateData method */
    template<class TInputImage, class TInputMask, class TOutputImage>
        void
        ShadowVariationThresholdImageFilter<TInputImage, TInputMask, TOutputImage>::ThreadedGenerateData(
                const OutputImageRegionType& outputRegionForThread, itk::ThreadIdType /*threadId*/)
        {

            // Get pointers to input images
            InputImageConstPointer l_ReflRatioPtr = this->GetReflRatio();
            InputMaskPointer l_IPCLDOutShadPtr = this->GetIPCLDOutShadInput();

            // Get the output mask
            OutputImagePointer l_IPCLDShadVarPtr = this->GetIPCLDShadVarOutput();

            // Define the input image region iterators
            itk::ImageRegionConstIterator<InputImageType> l_ReflRatioIt(l_ReflRatioPtr, outputRegionForThread);
            itk::ImageRegionIterator<InputMaskType> l_IPCLDOutShadIt(l_IPCLDOutShadPtr, outputRegionForThread);

            // Define the output image iterators
            // (Images not used for the correlation)
            itk::ImageRegionIterator<OutputImageType> l_IPCLDShadVarIt(l_IPCLDShadVarPtr, outputRegionForThread);

            // Iterator initializations
            l_ReflRatioIt.GoToBegin();
            l_IPCLDOutShadIt.GoToBegin();
            l_IPCLDShadVarIt.GoToBegin();

            InputMaskPixelType l_InputOneValue = static_cast<InputMaskPixelType>(1);
            InputImagePixelType l_InputNoDataValue = static_cast<InputImagePixelType>(m_NoData);
            OutputImagePixelType l_OutOneValue = static_cast<OutputImagePixelType>(1);
            OutputImagePixelType l_OutZeroValue = itk::NumericTraits<OutputImagePixelType>::Zero;

            // Pixel loop
            while (l_IPCLDShadVarIt.IsAtEnd() == false)
            {

                // Get the input pixel values
                const InputImagePixelType l_ReflRatioValue = l_ReflRatioIt.Get();
                const InputMaskPixelType l_IPCLDOutShadValue = l_IPCLDOutShadIt.Get();

                // Get the no_data mask values

                // Shadows are only detected in the outside clouds shadow zone
                // Detection not activated for refl = no_data
                if ((l_IPCLDOutShadValue == l_InputOneValue) && (l_ReflRatioValue != l_InputNoDataValue))
                {
                    if (l_ReflRatioValue < m_ThresholdValue)
                    {
                        l_IPCLDShadVarIt.Set(l_OutOneValue);

                    } // end if l_ReflRatio < l_Threshold
                    else
                    {
                        // Output mask initialization
                        l_IPCLDShadVarIt.Set(l_OutZeroValue);
                    }
                } // end if
                else
                {
                    // Output mask initialization
                    l_IPCLDShadVarIt.Set(l_OutZeroValue);
                }

                ++l_ReflRatioIt;
                ++l_IPCLDOutShadIt;
                ++l_IPCLDShadVarIt;

            } // end pixel loop

        }

    /** PrintSelf method */
    template<class TInputImage, class TInputMask, class TOutputImage>
        void
        ShadowVariationThresholdImageFilter<TInputImage, TInputMask, TOutputImage>::PrintSelf(std::ostream& os, itk::Indent indent) const
        {
            Superclass::PrintSelf(os, indent);
            os << "m_NoData: " << m_NoData << std::endl;
            os << "m_ThresholdValue: " << m_ThresholdValue << std::endl;
        }

} // End namespace vns

#endif /* __vnsShadowVariationThresholdImageFilter_txx */
