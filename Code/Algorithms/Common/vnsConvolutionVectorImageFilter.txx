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
 * VERSION : 4-0-0 : FA : LAIG-FA-MAC-354-CNES : 10 mars 2014 : Correction qualité sur règles de codage     *
 * VERSION : 1-0-0 : <TypeFT> : <NumFT> : 7 sept. 2010 : Creation                                                           
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id$
 *                                                                                                          *
 ************************************************************************************************************/
#ifndef __vnsConvolutionVectorImageFilter_txx
#define __vnsConvolutionVectorImageFilter_txx
#include "vnsConvolutionVectorImageFilter.h"

#include "itkConstNeighborhoodIterator.h"
#include "itkNeighborhoodInnerProduct.h"
#include "itkImageRegionIterator.h"
#include "itkNeighborhoodAlgorithm.h"
#include "itkOffset.h"
#include "itkConstantBoundaryCondition.h"

#include "otbMacro.h"
#include "vnsMacro.h"

namespace vns
{

    template<class TInputVectorImage, class TMask, class TOutputVectorImage, class TBoundaryCondition>
        ConvolutionVectorImageFilter<TInputVectorImage, TMask, TOutputVectorImage, TBoundaryCondition>::ConvolutionVectorImageFilter()
        {
            // Input/Output initialization
            this->SetNumberOfRequiredInputs(2);
            this->SetNumberOfIndexedInputs(2);
            this->SetNumberOfIndexedOutputs(1);

            m_Radius.Fill(1);
            m_Filter.SetSize(1);
            m_Filter.Fill(1);
            //m_NormalizeFilter = false;
            m_NoData = static_cast<OutputVectorInternalPixelType>(0);
        }

    template<class TInputVectorImage, class TMask, class TOutputVectorImage, class TBoundaryCondition>
        void
        ConvolutionVectorImageFilter<TInputVectorImage, TMask, TOutputVectorImage, TBoundaryCondition>::GenerateInputRequestedRegion()
                throw (itk::InvalidRequestedRegionError)
        {
            // call the superclass' implementation of this method
            Superclass::GenerateInputRequestedRegion();

            // get pointers to the input and output
            typename Superclass::InputImagePointer inputPtr = const_cast<TInputVectorImage *>(this->GetInput());
            MaskPointer maskPtr = const_cast<TMask *>(this->GetMask());
            typename Superclass::OutputImagePointer outputPtr = this->GetOutput();

            if (!inputPtr || !maskPtr || !outputPtr)
            {
                return;
            }

            // get a copy of the input requested region (should equal the output
            // requested region)
            typename TInputVectorImage::RegionType inputRequestedRegion;
            inputRequestedRegion = inputPtr->GetRequestedRegion();

            // pad the input requested region by the operator radius
            inputRequestedRegion.PadByRadius(m_Radius);

            otbMsgDevMacro(<<"Padding by " << m_Radius )
            otbMsgDevMacro(<<"Region is now " << inputRequestedRegion.GetIndex() << ", "<< inputRequestedRegion.GetSize() );

            // crop the input requested region at the input's largest possible region
            if (inputRequestedRegion.Crop(inputPtr->GetLargestPossibleRegion()))
            {
                inputPtr->SetRequestedRegion(inputRequestedRegion);
            }
            else
            {
                // Couldn't crop the region (requested region is outside the largest
                // possible region).  Throw an exception.

                // store what we tried to request (prior to trying to crop)
                inputPtr->SetRequestedRegion(inputRequestedRegion);

                // build an exception
                itk::InvalidRequestedRegionError e(__FILE__, __LINE__);
                e.SetLocation(ITK_LOCATION);
                e.SetDescription("Requested region is (at least partially) outside the largest possible region.");
                e.SetDataObject(inputPtr);
                throw e;
            }

            // get a copy of the input requested region (should equal the output
            // requested region)
            typename TMask::RegionType maskRequestedRegion;
            maskRequestedRegion = maskPtr->GetRequestedRegion();

            // pad the input requested region by the operator radius
            maskRequestedRegion.PadByRadius(m_Radius);

            // crop the input requested region at the input's largest possible region
            if (maskRequestedRegion.Crop(maskPtr->GetLargestPossibleRegion()))
            {
                maskPtr->SetRequestedRegion(maskRequestedRegion);
            }
            else
            {
                // Couldn't crop the region (requested region is outside the largest
                // possible region).  Throw an exception.

                // store what we tried to request (prior to trying to crop)
                maskPtr->SetRequestedRegion(maskRequestedRegion);

                // build an exception
                itk::InvalidRequestedRegionError e(__FILE__, __LINE__);
                e.SetLocation(ITK_LOCATION);
                e.SetDescription("Requested region is (at least partially) outside the largest possible region.");
                e.SetDataObject(inputPtr);
                throw e;
            }

            return;
        }

    template<class TInputVectorImage, class TMask, class TOutputVectorImage, class TBoundaryCondition>
        void
        ConvolutionVectorImageFilter<TInputVectorImage, TMask, TOutputVectorImage, TBoundaryCondition>::ThreadedGenerateData(
                const OutputVectorImageRegionType& outputRegionForThread, itk::ThreadIdType /*threadId*/)
        {
            unsigned int i = 0;

            // Allocate output
            typename OutputVectorImageType::Pointer output = this->GetOutput();
            // Get inputs
            typename InputVectorImageType::ConstPointer input = this->GetInput();
            typename MaskType::ConstPointer maskPtr = this->GetMask();

            // Init the number of pixels
            const unsigned int lNbOfBands = input->GetNumberOfComponentsPerPixel();

            // support progress methods/callbacks

            // Copy region
            InputVectorImageRegionType inputRegionForThread;
            this->CallCopyOutputRegionToInputRegion(inputRegionForThread, outputRegionForThread);
            MaskRegionType maskRegionForThread;
            this->CallCopyOutputRegionToInputRegion(maskRegionForThread, outputRegionForThread);

            // Init the region iterators
            itk::ConstNeighborhoodIterator<InputVectorImageType, BoundaryConditionType> inputIt(m_Radius, input, inputRegionForThread);
            itk::ConstNeighborhoodIterator<MaskType, MaskBoundaryConditionType> inputMaskIt(m_Radius, maskPtr, maskRegionForThread);
            itk::ImageRegionIterator<OutputVectorImageType> outputIt(output, outputRegionForThread);

            // Init iterator to begin
            inputIt.GoToBegin();
            inputMaskIt.GoToBegin();
            outputIt.GoToBegin();

            // Computes the size of neighborhoood
            const unsigned int neighborhoodSize = inputIt.Size();

            // Init the default value
            OutputVectorPixelType lOutPixDefaultPix;
            lOutPixDefaultPix.SetSize(lNbOfBands);
            lOutPixDefaultPix.Fill(m_NoData);

            // Init the sum values
            InputRealType lSum[lNbOfBands];
            InputRealType lSumPond[lNbOfBands];
            InputRealType lSumCoef[lNbOfBands];
            InputRealType lSumCoefNoData = itk::NumericTraits<InputRealType>::Zero;
            unsigned int lNbValidPix[lNbOfBands];
            // Loop
            while (!inputIt.IsAtEnd())
            {
                // if the pixel in the mask is equal to 0
                if (inputMaskIt.GetCenterPixel() == 0)
                {
                    // Init the output pixel
                    OutputVectorPixelType lOutPix;
                    lOutPix.SetSize(lNbOfBands);
                    memset(lSum, 0, lNbOfBands * sizeof(InputRealType));
                    memset(lSumPond, 0, lNbOfBands * sizeof(InputRealType));
                    memset(lSumCoef, 0, lNbOfBands * sizeof(InputRealType));
                    memset(lNbValidPix, 0, lNbOfBands * sizeof(unsigned int));
                    lSumCoefNoData = itk::NumericTraits<InputRealType>::Zero;
                    for (i = 0; i < neighborhoodSize; i++)
                    {
                        // Sum all valid values ( corresponding to a non NoData value )
                        const InputRealType FilterElement = m_Filter.GetElement(i);
                        const InputVectorPixelType InputPixel = inputIt.GetPixel(i);
                        if (inputMaskIt.GetPixel(i) == 0)
                        {
                            for (unsigned int b = 0; b < lNbOfBands; b++)
                            {
                                lSumPond[b] = lSumPond[b] + static_cast<InputRealType>((InputPixel[b]) * FilterElement);
                                lSumCoef[b] = lSumCoef[b] + FilterElement;
                                lSum[b] = lSum[b] + static_cast<InputRealType>(InputPixel[b]);
                                lNbValidPix[b] = lNbValidPix[b] + 1;
                            }
                        }
                        else
                        {
                            lSumCoefNoData = lSumCoefNoData + FilterElement;
                        }
                    }

                    // Loop for the bands
                    for (unsigned int b = 0; b < lNbOfBands; b++)
                    {
                        // Manage "NoData Edges"
                        InputRealType lLocalMean = lSum[b] / static_cast<InputRealType>(lNbValidPix[b]);
                        InputRealType lLocalPond = (lSumPond[b] + lLocalMean * lSumCoefNoData)
                                / static_cast<InputRealType>(lSumCoef[b] + lSumCoefNoData);

                        lOutPix[b] = static_cast<OutputVectorInternalPixelType>(lLocalPond);
                    }
                    // Set the output pixel value
                    outputIt.Set(lOutPix);
                }
                else
                {
                    // Set the output pixel value to default value
                    outputIt.Set(lOutPixDefaultPix);
                }
                // Increment the iterators
                ++inputIt;
                ++inputMaskIt;
                ++outputIt;
            }
        }

    /**
     * Standard "PrintSelf" method
     */
    template<class TInputVectorImage, class TMask, class TOutput, class TBoundaryCondition>
        void
        ConvolutionVectorImageFilter<TInputVectorImage, TMask, TOutput, TBoundaryCondition>::PrintSelf(std::ostream& os,
                itk::Indent indent) const
        {
            Superclass::PrintSelf(os, indent);
            os << indent << "Radius: " << m_Radius << std::endl;

        }

} // end namespace vns

#endif
