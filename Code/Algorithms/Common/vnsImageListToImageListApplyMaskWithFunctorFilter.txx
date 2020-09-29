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
#ifndef __vnsImageListToImageListApplyMaskWithFunctorFilter_txx
#define __vnsImageListToImageListApplyMaskWithFunctorFilter_txx

#include "vnsImageListToImageListApplyMaskWithFunctorFilter.h"

namespace vns
{
    /**
     * Constructor
     */
    template<class TInputImageList, class TMaskImage, class TOutputImageList, class TApplyMaskFunctorType>
        ImageListToImageListApplyMaskWithFunctorFilter<TInputImageList, TMaskImage, TOutputImageList, TApplyMaskFunctorType>::ImageListToImageListApplyMaskWithFunctorFilter()
        {
            m_BinaryFunctorImageFilter = FilterType::New();
            m_OutputIndex = 0;
        }
    /** Generate output information for the ImageList and for each image
     in the List. */
    template<class TInputImageList, class TMaskImage, class TOutputImageList, class TApplyMaskFunctorType>
        void
        ImageListToImageListApplyMaskWithFunctorFilter<TInputImageList, TMaskImage, TOutputImageList, TApplyMaskFunctorType>::GenerateOutputInformation()
        {
            // Retrieving input/output pointers
            InputImageListPointerType inputPtr = this->GetInput();
            OutputImageListPointerType outputPtr = this->GetOutput();
            if (outputPtr)
            {
                if (outputPtr->Size() != inputPtr->Size())
                {
                    // in this case, clear the list
                    outputPtr->Clear();
                    typename InputImageListType::ConstIterator inputListIt = inputPtr->Begin();
                    while (inputListIt != inputPtr->End())
                    {
                        outputPtr->PushBack(OutputImageType::New());
                        ++inputListIt;
                    }
                }

                // For each input image
                typename InputImageListType::ConstIterator inputListIt = inputPtr->Begin();
                typename OutputImageListType::Iterator outputListIt = outputPtr->Begin();

                // Start loop
                while (inputListIt != inputPtr->End() && outputListIt != outputPtr->End())
                {
                    // Create the output image and set its information
                    m_BinaryFunctorImageFilter->SetInput(inputListIt.Get());
                    m_BinaryFunctorImageFilter->UpdateOutputInformation();
                    outputListIt.Get()->CopyInformation(m_BinaryFunctorImageFilter->GetOutput(m_OutputIndex));
                    outputListIt.Get()->SetLargestPossibleRegion(
                            m_BinaryFunctorImageFilter->GetOutput(m_OutputIndex)->GetLargestPossibleRegion());
                    ++inputListIt;
                    ++outputListIt;
                }
            }
        }
    /** Generate input requested region for each image in the List. */
    template<class TInputImageList, class TMaskImage, class TOutputImageList, class TApplyMaskFunctorType>
        void
        ImageListToImageListApplyMaskWithFunctorFilter<TInputImageList, TMaskImage, TOutputImageList, TApplyMaskFunctorType>::GenerateInputRequestedRegion()
        {
            // Retrieving input/output pointers
            InputImageListPointerType inputPtr = this->GetInput();
            OutputImageListPointerType outputPtr = this->GetOutput();

            // For each input image and corresponding output image
            typename InputImageListType::ConstIterator inputListIt = inputPtr->Begin();
            typename OutputImageListType::Iterator outputListIt = outputPtr->Begin();

            // Use the filter to generate input requested region
            while (inputListIt != inputPtr->End() && outputListIt != outputPtr->End())
            {
                m_BinaryFunctorImageFilter->SetInput(inputListIt.Get());
                m_BinaryFunctorImageFilter->GetOutput(m_OutputIndex)->SetRequestedRegion(outputListIt.Get()->GetRequestedRegion());
                m_BinaryFunctorImageFilter->PropagateRequestedRegion(outputListIt.Get());
                ++inputListIt;
                ++outputListIt;
            }
        }
    /** Main computation method */
    template<class TInputImageList, class TMaskImage, class TOutputImageList, class TApplyMaskFunctorType>
        void
        ImageListToImageListApplyMaskWithFunctorFilter<TInputImageList, TMaskImage, TOutputImageList, TApplyMaskFunctorType>::GenerateData()
        {
            // Retrieving input/output pointers
            InputImageListPointerType inputPtr = this->GetInput();
            OutputImageListPointerType outputPtr = this->GetOutput();

            // For each input image and corresponding output image
            typename InputImageListType::ConstIterator inputListIt = inputPtr->Begin();
            typename OutputImageListType::Iterator outputListIt = outputPtr->Begin();
            unsigned int counter = 0;

            // Start loop
            while (inputListIt != inputPtr->End() && outputListIt != outputPtr->End())
            {
                m_BinaryFunctorImageFilter->SetInput1(inputListIt.Get());
                m_BinaryFunctorImageFilter->GetOutput(m_OutputIndex)->SetRequestedRegion(outputListIt.Get()->GetRequestedRegion());
                m_BinaryFunctorImageFilter->Update();
                outputPtr->SetNthElement(counter, static_cast<OutputImageType *>(m_BinaryFunctorImageFilter->GetOutput(m_OutputIndex)));
                outputListIt.Get()->DisconnectPipeline();
                ++inputListIt;
                ++outputListIt;
                ++counter;
            }
        }
    /**
     * PrintSelf Method
     */
    template<class TInputImageList, class TMaskImage, class TOutputImageList, class TApplyMaskFunctorType>
        void
        ImageListToImageListApplyMaskWithFunctorFilter<TInputImageList, TMaskImage, TOutputImageList, TApplyMaskFunctorType>::PrintSelf(
                std::ostream& os, itk::Indent indent) const
        {
            Superclass::PrintSelf(os, indent);
        }
} // End namespace otb
#endif
