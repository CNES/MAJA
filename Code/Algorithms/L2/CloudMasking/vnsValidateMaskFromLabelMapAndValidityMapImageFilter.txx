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
 * VERSION : 5-0-0 : DM : LAIG-DM-MAC-982-CNES : 8 janvier 2016 : Modification de l’algorithme de detection **
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id: vnsValidateMaskFromLabelMapAndValidityMapImageFilter.txx 10095 2016-01-15 09:42:27Z abricier $            *
 *                                                                                                          *
 ************************************************************************************************************/
#ifndef __vnsValidateMaskFromLabelMapAndValidityMapImageFilter_txx
#define __vnsValidateMaskFromLabelMapAndValidityMapImageFilter_txx

#include "vnsValidateMaskFromLabelMapAndValidityMapImageFilter.h"

namespace vns
{

    /** Constructor */
    template<class TInputMask, class TLabelImage, class TOutputImage>
        ValidateMaskFromLabelMapAndValidityMapImageFilter<TInputMask, TLabelImage, TOutputImage>::ValidateMaskFromLabelMapAndValidityMapImageFilter()
        {
            // Input/Output initialization
            this->SetNumberOfIndexedInputs(3);
            this->SetNumberOfRequiredInputs(2);
            this->SetNumberOfIndexedOutputs(1);
            this->SetNumberOfRequiredOutputs(1);
            this->SetNthOutput(0, InputMaskType::New());
        }

    /** Destructor */
    template<class TInputMask, class TLabelImage, class TOutputImage>
        ValidateMaskFromLabelMapAndValidityMapImageFilter<TInputMask, TLabelImage, TOutputImage>::~ValidateMaskFromLabelMapAndValidityMapImageFilter()
        {
        }

    //======================= ThreadedGenerateData =================================/
    /** ThreadedGenerateData method */
    template<class TInputMask, class TLabelImage, class TOutputImage>
        void
        ValidateMaskFromLabelMapAndValidityMapImageFilter<TInputMask, TLabelImage, TOutputImage>::ThreadedGenerateData(
                const OutputImageRegionType& outputRegionForThread, itk::ThreadIdType /*threadId*/)
        {
            // Get pointers to input images
            InputMaskConstPointer l_InputPtr = this->GetMaskInput();
            InputMaskConstPointer l_ExtCorrelFlagInputPtr = this->GetExtCorrelFlagInput();
            LabelImageConstPointer l_InputLabelMapPtr = this->GetLabelInput();

            // Get the output mask
            OutputImagePointer l_OutputPtr = this->GetOutput();

            // Define the input image region iterators
            itk::ImageRegionConstIterator<InputMaskType> l_InputIt(l_InputPtr, outputRegionForThread);
            itk::ImageRegionConstIterator<LabelImageType> l_InputLabelMapIt(l_InputLabelMapPtr, outputRegionForThread);

            // Define the output image iterators
            // (Images not used for the correlation)
            itk::ImageRegionIterator<OutputImageType> l_OutputIt(l_OutputPtr, outputRegionForThread);

            // Set the image region iterators to the same region of the neighborhood iterators
            l_InputIt.GoToBegin();
            l_InputLabelMapIt.GoToBegin();
            l_OutputIt.GoToBegin();

            const InputMaskPixelType l_InputOneValue = static_cast<InputMaskPixelType>(1);
            const OutputImagePixelType l_OutputOneValue = static_cast<OutputImagePixelType>(1);
            const OutputImagePixelType l_OutputZeroValue = static_cast<OutputImagePixelType>(0);

            vnsLogSuperDebugMacro("ValidityMap.size: " << m_ValidityMap.size())
            if (m_ValidityMap.size() == 0) // No validation/invalidation to apply
            {
                while (l_InputIt.IsAtEnd() == false)
                {
                    l_OutputIt.Set(static_cast<OutputImagePixelType>(l_InputIt.Get()));
                    ++l_InputIt;
                    ++l_OutputIt;
                }
            }
            // If l_ExtCorrelFlagInputPtr NOT set
            else if (l_ExtCorrelFlagInputPtr.IsNull() == true)
            {
                vnsLogSuperDebugMacro("l_EnableExtCorrelFlagInput: false")
                // Pixel loop
                while (l_InputLabelMapIt.IsAtEnd() == false)
                {
                    const LabelImagePixelType l_InputLabelMapValue = l_InputLabelMapIt.Get();
                    itkAssertInDebugAndIgnoreInReleaseMacro(m_ValidityMap.find(l_InputLabelMapValue) != m_ValidityMap.end());
                    if (l_InputIt.Get() == l_InputOneValue)
                    {
                        if ((m_ValidityMap.count(l_InputLabelMapValue) <= 0) || (m_ValidityMap.at(l_InputLabelMapValue) == true)) // fixed last minute spec modifications
                        {
                            l_OutputIt.Set(l_OutputZeroValue);
                        }
                        else
                        {
                            l_OutputIt.Set(l_OutputOneValue);
                        }
                    }
                    else
                    {
                        l_OutputIt.Set(l_OutputZeroValue);
                    }
                    ++l_InputIt;
                    ++l_InputLabelMapIt;
                    ++l_OutputIt;
                } // end pixel loop
            }
            else
            {
                vnsLogSuperDebugMacro("l_EnableExtCorrelFlagInput: true")

                itk::ImageRegionConstIterator<InputMaskType> l_ExtCorrelFlagInputIt(l_ExtCorrelFlagInputPtr, outputRegionForThread);
                l_ExtCorrelFlagInputIt.GoToBegin();
                // Pixel loop
                while (l_InputLabelMapIt.IsAtEnd() == false)
                {
                    const LabelImagePixelType l_InputLabelMapValue = l_InputLabelMapIt.Get();
                    itkAssertInDebugAndIgnoreInReleaseMacro(m_ValidityMap.find(l_InputLabelMapValue) != m_ValidityMap.end());
                    if (l_InputIt.Get() == l_InputOneValue)
                    {
                        //if ((m_ValidityMap.count(l_InputLabelMapValue) <= 0) || (m_ValidityMap.at(l_InputLabelMapValue) == true)) // fixed last minute spec modifications
                        if ((m_ValidityMap.find(l_InputLabelMapValue) != m_ValidityMap.end())
                                && (m_ValidityMap.at(l_InputLabelMapValue) == true) && (l_ExtCorrelFlagInputIt.Get() == l_InputOneValue))
                        {
                            l_OutputIt.Set(l_OutputZeroValue);
                        }
                        else
                        {
                            l_OutputIt.Set(l_OutputOneValue);
                        }
                    }
                    else
                    {
                        l_OutputIt.Set(l_OutputZeroValue);
                    }
                    ++l_InputIt;
                    ++l_ExtCorrelFlagInputIt;
                    ++l_InputLabelMapIt;
                    ++l_OutputIt;
                } // end pixel loop
            }
        }

    /** PrintSelf method */
    template<class TInputMask, class TLabelImage, class TOutputImage>
        void
        ValidateMaskFromLabelMapAndValidityMapImageFilter<TInputMask, TLabelImage, TOutputImage>::PrintSelf(std::ostream& os,
                itk::Indent indent) const
        {
            Superclass::PrintSelf(os, indent);
        }

} // End namespace vns

#endif

