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
 * VERSION : 1.0.0 : FA : LAIG-FA-MAC-1988-CNES : 21 octobre 2016 : Correction qualite (code mort)          *
 * VERSION : 5-0-0 : DM : LAIG-DM-MAC-982-CNES : 8 janvier 2016 : Modification de l’algorithme de detection **
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id: vnsStreamingThresholdFromLabelImageFilter.txx 10095 2016-01-15 09:42:27Z abricier $                 *
 *                                                                                                          *
 ************************************************************************************************************/
#ifndef __vnsStreamingThresholdFromLabelImageFilter_txx
#define __vnsStreamingThresholdFromLabelImageFilter_txx
#include "vnsStreamingThresholdFromLabelImageFilter.h"

#include "itkInputDataObjectIterator.h"
#include "itkImageRegionIterator.h"
#include "itkProgressReporter.h"
#include "otbMacro.h"
#include "vnsUtilities.h"

namespace vns
{

    template<class TInputMask, class TLabelImage>
        PersistentStreamingThresholdFromLabelImageFilter<TInputMask, TLabelImage>::PersistentStreamingThresholdFromLabelImageFilter() :
                m_LowerValidityThreshold(-1000.)
        {
            // first output is a copy of the image, DataObject created by
            // superclass
            //
            // allocate the data objects for the outputs which are
            // just decorators around pixel types
            typename LabelValidityMapObjectType::Pointer output = static_cast<LabelValidityMapObjectType*>(this->MakeOutput(1).GetPointer());
            this->itk::ProcessObject::SetNthOutput(1, output.GetPointer());

            this->Reset();

        }

    template<class TInputMask, class TLabelImage>
        typename itk::DataObject::Pointer
        PersistentStreamingThresholdFromLabelImageFilter<TInputMask, TLabelImage>::MakeOutput(unsigned int output)
        {
            switch (output)
                {
            case 0:
                return static_cast<itk::DataObject*>(TInputMask::New().GetPointer());
            case 1:
                return static_cast<itk::DataObject*>(LabelValidityMapObjectType::New().GetPointer());
            default:
                // might as well make an image
                return static_cast<itk::DataObject*>(TInputMask::New().GetPointer());
                }
        }

    template<class TInputMask, class TLabelImage>
        void
        PersistentStreamingThresholdFromLabelImageFilter<TInputMask, TLabelImage>::GenerateOutputInformation()
        {
            Superclass::GenerateOutputInformation();

            if (this->GetInput())
            {
                this->GetOutput()->CopyInformation(this->GetInput());
                this->GetOutput()->SetLargestPossibleRegion(this->GetInput()->GetLargestPossibleRegion());

                if (this->GetOutput()->GetRequestedRegion().GetNumberOfPixels() == 0)
                {
                    this->GetOutput()->SetRequestedRegion(this->GetOutput()->GetLargestPossibleRegion());
                }
            }
        }

    template<class TInputMask, class TLabelImage>
        void
        PersistentStreamingThresholdFromLabelImageFilter<TInputMask, TLabelImage>::AllocateOutputs()
        {
            // This is commented to prevent the streaming of the whole image for the first stream strip
            // It shall not cause any problem because the output image of this filter is not intended to be used.
            //InputImagePointer image = const_cast< TInputImage * >( this->GetInput() );
            //this->GraftOutput( image );
            // Nothing that needs to be allocated for the remaining outputs
        }

    template<class TInputMask, class TLabelImage>
        void
        PersistentStreamingThresholdFromLabelImageFilter<TInputMask, TLabelImage>::Synthetize()
        {
            typename LabelPopulationMapType::iterator it;

            for (it = m_LabelValidPopulation.begin(); it != m_LabelValidPopulation.end(); ++it)
            {
                if ((double(it->second) / double(m_LabelTotalPopulation[it->first])) > m_LowerValidityThreshold)
                {
                    m_LabelValidityStatusMap[it->first] = true;
                }
            }
        }

    template<class TInputMask, class TLabelImage>
        void
        PersistentStreamingThresholdFromLabelImageFilter<TInputMask, TLabelImage>::Reset()
        {
            m_LabelTotalPopulation.clear();
            m_LabelValidPopulation.clear();
            m_LabelValidityStatusMap.clear();
        }

    template<class TInputMask, class TLabelImage>
        void
        PersistentStreamingThresholdFromLabelImageFilter<TInputMask, TLabelImage>::GenerateInputRequestedRegion()
        {
            // The Requested Regions of all the inputs are set to their Largest Possible Regions
            this->itk::ProcessObject::GenerateInputRequestedRegion();

            // Iteration over all the inputs of the current filter (this)
            for (itk::InputDataObjectIterator it(this); !it.IsAtEnd(); ++it)
            {
                // Check whether the input is an image of the appropriate dimension
                // dynamic_cast of all the input images as itk::ImageBase objects
                // in order to pass the if ( input ) test whatever
                // the inputImageType (vectorImage or labelImage)
                ImageBaseType * input = dynamic_cast<ImageBaseType *>(it.GetInput());

                if (input)
                {
                    // Use the function object RegionCopier to copy the output region
                    // to the input.  The default region copier has default implementations
                    // to handle the cases where the input and output are the same
                    // dimension, the input a higher dimension than the output, and the
                    // input a lower dimension than the output.
                    InputImageRegionType inputRegion;
                    this->CallCopyOutputRegionToInputRegion(inputRegion, this->GetOutput()->GetRequestedRegion());
                    input->SetRequestedRegion(inputRegion);
                }
            }
        }

    template<class TInputMask, class TLabelImage>
        void
        PersistentStreamingThresholdFromLabelImageFilter<TInputMask, TLabelImage>::GenerateData()
        {
            /**
             * Grab the input
             */
            InputMaskPointer inputPtr = const_cast<TInputMask *>(this->GetInput());
            LabelImagePointer labelInputPtr = const_cast<TLabelImage *>(this->GetInputLabelImage());

            itk::ImageRegionConstIterator<TInputMask> inIt(inputPtr, inputPtr->GetRequestedRegion());
            itk::ImageRegionConstIterator<TLabelImage> labelIt(labelInputPtr, labelInputPtr->GetRequestedRegion());

            MaskPixelType flag;
            LabelPixelType label;
            // do the work
            for (inIt.GoToBegin(), labelIt.GoToBegin(); !inIt.IsAtEnd() && !labelIt.IsAtEnd(); ++inIt, ++labelIt)
            {
                flag = inIt.Get();
                label = labelIt.Get();
//                if (m_LabelTotalPopulation.count(label) <= 0) //add new element to the map
                if (m_LabelTotalPopulation.find(label) == m_LabelTotalPopulation.end()) //add new element to the map
                {
                    m_LabelTotalPopulation[label] = 1;
                    m_LabelValidPopulation[label] = flag;
                    m_LabelValidityStatusMap[label] = false;
                }
                else
                {
                    m_LabelTotalPopulation[label] += 1;
                    m_LabelValidPopulation[label] += flag;
                }

            }
        }

    template<class TInputMask, class TLabelImage>
        void
        PersistentStreamingThresholdFromLabelImageFilter<TInputMask, TLabelImage>::PrintSelf(std::ostream& os, itk::Indent indent) const
        {
            Superclass::PrintSelf(os, indent);
        }

} // end namespace vns
#endif
