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
 * VERSION : 1.0.0 : FA : LAIG-FA-MAC-148399-CS : 26 octobre 2016 : Audit code - Correction qualite         *
 * VERSION : 5-0-0 : DM : LAIG-DM-MAC-982-CNES : 8 janvier 2016 : Modification de l’algorithme de detection **
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id$
 *                                                                                                          *
 ************************************************************************************************************/

#ifndef __vnsFlattenCloudMaskImageListFilter_txx
#define __vnsFlattenCloudMaskImageListFilter_txx

#include "vnsFlattenCloudMaskImageListFilter.h"

#include <limits>
#include "itkImageRegionIterator.h"
#include "vnsImageListPixelIterator.h"

namespace vns
{

    template<typename TInputMask, typename TOutputMask>
        FlattenCloudMaskImageListFilter<TInputMask, TOutputMask>::FlattenCloudMaskImageListFilter()
        {
            this->SetNumberOfRequiredOutputs(1);
            this->SetNthOutput(0, OutputMaskType::New());
        }

    template<typename TInputMask, typename TOutputMask>
        void
        FlattenCloudMaskImageListFilter<TInputMask, TOutputMask>::GenerateOutputInformation(void)
        {
            InputMaskListPointerType inputPtr = this->GetInput();
            OutputMaskPointerType outputPtr = this->GetOutput();
            if (inputPtr->Size() > 0)
            {
                typename InputMaskListType::ConstIterator inputListIt = inputPtr->Begin();
                outputPtr->CopyInformation(inputListIt.Get());
                outputPtr->SetLargestPossibleRegion(inputListIt.Get()->GetLargestPossibleRegion());
            }
        }

    template<typename TInputMask, typename TOutputMask>
        void
        FlattenCloudMaskImageListFilter<TInputMask, TOutputMask>::GenerateInputRequestedRegion(void)
        {
            InputMaskListPointerType inputPtr = this->GetInput();
            OutputMaskPointerType outputPtr = this->GetOutput();
            itkAssertInDebugAndIgnoreInReleaseMacro(inputPtr.IsNotNull());
            itkAssertInDebugAndIgnoreInReleaseMacro(outputPtr.IsNotNull());

            typename OutputMaskType::RegionType requestedRegion = outputPtr->GetRequestedRegion();

            for (typename InputMaskListType::ConstIterator b = inputPtr->Begin(); b != inputPtr->End(); ++b)
            {
                b.Get()->SetRequestedRegion(requestedRegion);
            }
        }

    /*virtual*/
    template<typename TInputMask, typename TOutputMask>
        void
        FlattenCloudMaskImageListFilter<TInputMask, TOutputMask>::PrintSelf(std::ostream& os, itk::Indent indent) const
        {
            Superclass::PrintSelf(os, indent);
        }

    /*virtual*/
    template<typename TInputMask, typename TOutputMask>
        void
        FlattenCloudMaskImageListFilter<TInputMask, TOutputMask>::GenerateData()
        {
            BeforeGenerateData();
            DoGenerateData();
        }

    template<typename TInputMask, typename TOutputMask>
        void
        FlattenCloudMaskImageListFilter<TInputMask, TOutputMask>::BeforeGenerateData()
        {
            typename InputMaskListType::Pointer ImageList = this->GetInput();
            typename OutputMaskType::Pointer output = this->GetOutput();

            // 1- check invariants
            itkAssertInDebugAndIgnoreInReleaseMacro(ImageList.IsNotNull());
            itkAssertInDebugAndIgnoreInReleaseMacro(ImageList->Size() > 0);
            itkAssertInDebugAndIgnoreInReleaseMacro(output.IsNotNull());

            // 2- Allocate
            output->SetBufferedRegion(output->GetRequestedRegion());
            output->Allocate();
        }

    template<typename TInputMask, typename TOutputMask>
        void
        FlattenCloudMaskImageListFilter<TInputMask, TOutputMask>::DoGenerateData()
        {
            typedef itk::ImageRegionIterator<InputMaskType> InputMaskIteratorType;
            typedef ImageListPixelIterator<InputMaskIteratorType> InputMaskListIteratorType;
            typedef typename InputMaskListIteratorType::PixelReferenceType InputPixelType;
            typedef itk::ImageRegionIterator<OutputMaskType> OutputMaskIteratorType;
            typedef typename OutputMaskIteratorType::PixelType OutputPixelType;

            typename InputMaskListType::Pointer maskList = this->GetInput();
            typename OutputMaskType::Pointer output = this->GetOutput();

            itkAssertInDebugAndIgnoreInReleaseMacro(maskList.IsNotNull());
            itkAssertInDebugAndIgnoreInReleaseMacro(output.IsNotNull());

            const std::size_t N = maskList->Size();
            InputMaskListIteratorType inIt(*maskList, output->GetRequestedRegion());
            OutputMaskIteratorType outIt(output, output->GetRequestedRegion());

            // Loop
            for (; !inIt.IsAtEnd(); ++inIt, ++outIt)
            {
                itkAssertInDebugAndIgnoreInReleaseMacro(
                        !outIt.IsAtEnd() && "input and output shall have the same number of pixels by contruction");

                InputPixelType const& pixel = *inIt;
                bool isSet = false;
                for (std::size_t i = 0; i != N; ++i)
                {
                    // TODO: extract a function!
                    typename InputPixelType::SubPixelType v = pixel[i];
                    if (v > 0)
                    {
                        isSet = true;
                        break; // no need to loop again!
                    }
                }
                const OutputPixelType outPix = isSet ? 1 : 0;
                outIt.Set(outPix);
            }
            itkAssertInDebugAndIgnoreInReleaseMacro(
                    outIt.IsAtEnd() && "input and output shall have the same number of pixels by contruction");
        }
}

#endif /* FlattenCloudMaskImageListFilter */
