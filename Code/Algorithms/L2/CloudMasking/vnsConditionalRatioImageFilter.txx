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
 * VERSION : 5-0-0 : DM : LAIG-DM-MAC-982-CNES : 5 avr. 2016 : Creation                                       *
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id:
 *                                                                                                          *
 ************************************************************************************************************/
#ifndef __vnsConditionalRatioImageFilter_txx
#define __vnsConditionalRatioImageFilter_txx

#include "vnsConditionalRatioImageFilter.h"

#include "itkImageRegionConstIterator.h"
#include "itkImageRegionIterator.h"

namespace vns
{

    /** Constructor */
    template<class TInputImage, class TInputMask, class TOutputImage>
        ConditionalRatioImageFilter<TInputImage, TInputMask, TOutputImage>::ConditionalRatioImageFilter()
        {
            // Input/Output initialization
            this->SetNumberOfIndexedInputs(5);
            this->SetNumberOfRequiredInputs(5);
            this->SetNumberOfIndexedOutputs(1);

            this->SetNthOutput(0, OutputImageType::New());

            // Parameters initialization
            m_NoData = static_cast<RealNoDataType> (0);
            m_ShadowBandTOCR = 0;
            m_ShadowBandRCR = 0;
        }

    /** Destructor */
    template<class TInputImage, class TInputMask, class TOutputImage>
        ConditionalRatioImageFilter<TInputImage, TInputMask, TOutputImage>::~ConditionalRatioImageFilter()
        {
        }

    //****************************** ThreadedGenerateData ********************************/
    /** ThreadedGenerateData method */
    template<class TInputImage, class TInputMask, class TOutputImage>
        void
        ConditionalRatioImageFilter<TInputImage, TInputMask, TOutputImage>::ThreadedGenerateData(
                const OutputImageRegionType& outputRegionForThread, itk::ThreadIdType /* threadId */)
        {
            // Get pointers to input images
            InputImageConstPointer l_IPTOCRPtr = this->GetIPTOCRInput();
            InputImageConstPointer l_L2inRCRPtr = this->GetL2inRCRInput();
            InputMaskConstPointer l_IPEDGPtr = this->GetIPEDGInput();
            //InputMaskConstPointer l_L2inNDTPtr = this->GetL2inNDTInput();
            InputMaskConstPointer l_L2inWASPtr = this->GetL2inWASInput();
            InputMaskPointer l_L2CLDSubPtr = this->GetL2CLDSubInput();

            // Get the output mask
            OutputImagePointer l_OutputPtr = this->GetOutput(0);

            // Define the input image iterators
            itk::ImageRegionConstIterator<InputImageType> l_IPTOCRIt(l_IPTOCRPtr,
                    outputRegionForThread);
            itk::ImageRegionConstIterator<InputImageType> l_L2inRCRIt(l_L2inRCRPtr,
                    outputRegionForThread);
            itk::ImageRegionConstIterator<InputMaskType> l_L2CLDSubIt(l_L2CLDSubPtr,
                    outputRegionForThread);
            itk::ImageRegionConstIterator<InputMaskType> l_IPEDGIt(l_IPEDGPtr,
                    outputRegionForThread);
            itk::ImageRegionConstIterator<InputMaskType> l_L2inWASIt(l_L2inWASPtr,
                    outputRegionForThread);
//            itk::ImageRegionConstIterator<InputMaskType> l_L2inNDTIt(l_L2inNDTPtr,
//                    outputRegionForThread);

            // Define the output image iterators
            itk::ImageRegionIterator<OutputImageType>
                    l_OutputIt(l_OutputPtr, outputRegionForThread);

            l_IPTOCRIt.GoToBegin();
            l_L2inRCRIt.GoToBegin();
            l_L2CLDSubIt.GoToBegin();
            l_IPEDGIt.GoToBegin();
            l_L2inWASIt.GoToBegin();
//            l_L2inNDTIt.GoToBegin();
            l_OutputIt.GoToBegin();

            // Pixel loop
            while (l_OutputIt.IsAtEnd() == false)
            {

                // Get values of reflectance for the current image and the composite image
                // in the shadow band
                const InputInternalPixelType l_IPTOCRValue = l_IPTOCRIt.Get()[m_ShadowBandTOCR];
                const InputInternalPixelType l_L2inRCRValue = l_L2inRCRIt.Get()[m_ShadowBandRCR];

                // Get the pixel values of the cloud, water and edges (No_Data) masks
                const InputMaskPixelType l_L2CLDSubValue = l_L2CLDSubIt.Get();
                const InputMaskPixelType l_IPEDGValue = l_IPEDGIt.Get();
                const InputMaskPixelType l_L2inWASValue = l_L2inWASIt.Get();
//                const InputMaskPixelType l_L2inNDTValue = l_L2inNDTIt.Get();

                // Test if the pixel is not cloudy for the date D, edges for the date D and D-1
                // or water for the date D-1
                if ((l_L2CLDSubValue == 0) && (l_IPEDGValue == 0) && (l_L2inWASValue == 0)
                        /* && (l_L2inNDTValue == 0) */ && !vnsEqualsDoubleMacro(l_L2inRCRValue, 0) && !vnsEqualsDoubleMacro(l_L2inRCRValue,m_NoData))
                {
                    // Compute difference between the current image of reflectance and
                    // the composite image
                    // The value should be negative when a shadow affects the image of date D
                    l_OutputIt.Set(static_cast<OutputImagePixelType>(l_IPTOCRValue / l_L2inRCRValue));
                }
                else
                {
                    l_OutputIt.Set(static_cast<OutputImagePixelType>(m_NoData));
                }

                ++l_IPTOCRIt;
                ++l_L2inRCRIt;
                ++l_L2CLDSubIt;
                ++l_IPEDGIt;
                ++l_L2inWASIt;
//                ++l_L2inNDTIt;
                ++l_OutputIt;
            }

        }

    /** PrintSelf method */
    template<class TInputImage, class TInputMask, class TOutputImage>
        void
        ConditionalRatioImageFilter<TInputImage, TInputMask, TOutputImage>::PrintSelf(
                std::ostream& os, itk::Indent indent) const
        {
            Superclass::PrintSelf(os, indent);
        }

} // End namespace vns

#endif /* __vnsConditionalRatioImageFilter_txx */
