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
 * VERSION : 4-4-0 : FA : LAIG-FA-MAC-1256-CNES : 1 juillet 2015 : Amelioration des performances            *
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id: vnsDedicatedFastResampleImageFilter.txx 2656 2011-02-15 10:25:38Z cvallade $
 *                                                                                                          *
 ************************************************************************************************************/
#ifndef __vnsDedicatedFastResampleImageFilter_txx
#define __vnsDedicatedFastResampleImageFilter_txx

#include "vnsDedicatedFastResampleImageFilter.h"
#include "itkImageRegionIteratorWithIndex.h"
#include "itkMacro.h"
#include "itkObjectFactory.h"
#include "itkNumericTraits.h"
#include "vnsMath.h"
#include "itkPoint.h"
#include "vnsLoggers.h"
#include "vnsResamplerHelper.h"

namespace vns
{

    /**
     * BeforeThreadedGenerateData
     */
    template<class TInputImage, class TOutputImage>
        void
        DedicatedFastResampleImageFilter<TInputImage, TOutputImage>::BeforeThreadedGenerateData()
        {

            // Checks the input and the Interpolator variables
            if ((!m_Interpolator) || (!this->GetInput()))
            {
                itkExceptionMacro(<< "Interpolator and/or Input not set");
            }
            m_VectorLinearInterpolator->SetInputImage(this->GetInput());
            m_FastLinearWithRadiusInterpolator->SetInputImage(this->GetInput());

        }

    /**
     * ThreadedGenerateData
     */
    template<class TInputImage, class TOutputImage>
        void
        DedicatedFastResampleImageFilter<TInputImage, TOutputImage>::ThreadedGenerateData(
                const OutputImageRegionType& outputRegionForThread, itk::ThreadIdType /*threadId*/)
        {
            // Get the input and output pointers
            OutputImagePointer outputPtr = this->GetOutput();

            // Load the number of channels
            const unsigned int l_Size = outputPtr->GetNumberOfComponentsPerPixel();

            OutputPixelType outPix;
            outPix.SetSize(l_Size);
            outPix.Fill(0);

            OutputPixelType outPixPaddingValue;
            outPixPaddingValue.SetSize(l_Size);
            outPixPaddingValue.Fill(0);

            // Iterator for walking the output
            typedef itk::ImageRegionIteratorWithIndex<TOutputImage> OutputIterator;

            OutputIterator outIt(outputPtr, outputRegionForThread);

            // Define a few indices that will be used to translate from an input
            // pixel to and output pixel
            typename TOutputImage::IndexType outputIndex;
            typename InterpolatorType::ContinuousIndexType inputIndex;

            PointType point;

            // Support progress methods/callbacks
            typedef typename OutputImageType::PixelType::ValueType ValueType;

            if (m_InterpolatorValue == ResamplerHelper::RB_INTERPOLATOR_MAJA_FAST_VECTOR_LINEAR)
            {
                // Walk the output region, and interpolate the input image
                while (outIt.IsAtEnd() == false)
                {
                    // Determine the index of the output pixel
                    outputIndex = outIt.GetIndex();
                    outputPtr->TransformIndexToPhysicalPoint(outputIndex, point);

                    // get the interpolated value
                    if (m_VectorLinearInterpolator->IsInsideBuffer(point))
                    {
                        typedef typename InterpolatorType::OutputType OutputType;
                        const OutputType interpolatedValue = m_VectorLinearInterpolator->Evaluate(point);
                        for (unsigned int k = 0; k < l_Size; k++)
                        {
                            outPix[k] = static_cast<ValueType>(interpolatedValue[k]);
                        }

                        outIt.Set(outPix);
                    }
                    else
                    {
                        outIt.Set(outPixPaddingValue);
                    }
                    ++outIt;
                }
            }
            else if (m_InterpolatorValue == ResamplerHelper::RB_INTERPOLATOR_MAJA_FAST_VECTOR_LINEAR_WITH_RADIUS)
            {
//                const PointType l_Origin = outputPtr->GetOrigin();
//                const SpacingType l_Spacing = outputPtr->GetSpacing();

                // Walk the output region, and interpolate the input image
                while (outIt.IsAtEnd() == false)
                {
                    // Determine the index of the output pixel
                    outputIndex = outIt.GetIndex();

// V 4.4 - optimization perfo - annule, ne gagne rien
                    outputPtr->TransformIndexToPhysicalPoint(outputIndex, point);

                    // get the interpolated value
                    if (m_FastLinearWithRadiusInterpolator->IsInsideBuffer(point))
                    {
                        typedef typename InterpolatorType::OutputType OutputType;
                        const OutputType interpolatedValue = m_FastLinearWithRadiusInterpolator->Evaluate(point);
                        for (unsigned int k = 0; k < l_Size; k++)
                        {
                            outPix[k] = static_cast<ValueType>(interpolatedValue[k]);
                        }

                        outIt.Set(outPix);
                    }
                    else
                    {
                        outIt.Set(outPixPaddingValue);
                    }
                    ++outIt;
                }
            }
            else
            {
                m_Interpolator->SetInputImage(this->GetInput());
                // Walk the output region, and interpolate the input image
                while (outIt.IsAtEnd() == false)
                {
                    // Determine the index of the output pixel
                    outputIndex = outIt.GetIndex();
                    outputPtr->TransformIndexToPhysicalPoint(outputIndex, point);

                    // get the interpolated value
                    if (m_Interpolator->IsInsideBuffer(point))
                    {
                        typedef typename InterpolatorType::OutputType OutputType;
                        const OutputType interpolatedValue = m_Interpolator->Evaluate(point);
                        for (unsigned int k = 0; k < l_Size; k++)
                        {
                            outPix[k] = static_cast<ValueType>(interpolatedValue[k]);
                        }

                        outIt.Set(outPix);
                    }
                    else
                    {
                        outIt.Set(outPixPaddingValue);
                    }
                    ++outIt;
                }

            }

        }

    /**
     * GenerateInputRequesteRegion
     */
    template<class TInputImage, class TOutputImage>
        void
        DedicatedFastResampleImageFilter<TInputImage, TOutputImage>::GenerateInputRequestedRegion()
        {
            // Call the superclass' implementation of this method
            Superclass::GenerateInputRequestedRegion();

            // Get pointers to the input and output
            InputImagePointer inputPtr = const_cast<TInputImage *>(this->GetInput());
            OutputImagePointer outputPtr = this->GetOutput();

            // Get the Output region
            const OutputIndexType& outputRequestedRegionStartIndex = outputPtr->GetRequestedRegion().GetIndex();
            const OutputIndexType& outputRequestedRegionEndIndex = outputPtr->GetRequestedRegion().GetUpperIndex();

            PointType outputRequestedRegionStartPoint;
            outputPtr->TransformIndexToPhysicalPoint(outputRequestedRegionStartIndex, outputRequestedRegionStartPoint);

            PointType outputRequestedRegionEndPoint;
            outputPtr->TransformIndexToPhysicalPoint(outputRequestedRegionEndIndex, outputRequestedRegionEndPoint);

            ContinuousIndexType inputRequestedRegionStartContinuousIndex;
            InputIndexType inputRequestedRegionStartIndex;
            ContinuousIndexType inputRequestedRegionEndContinuousIndex;
            InputIndexType inputRequestedRegionEndIndex;
            inputPtr->TransformPhysicalPointToContinuousIndex(outputRequestedRegionStartPoint, inputRequestedRegionStartContinuousIndex);
            inputPtr->TransformPhysicalPointToContinuousIndex(outputRequestedRegionEndPoint, inputRequestedRegionEndContinuousIndex);

            for (unsigned int dim = 0; dim < ImageDimension; dim++)
            {
                inputRequestedRegionStartIndex[dim] = itk::Math::Floor < IndexValueType > (inputRequestedRegionStartContinuousIndex[dim]);
                inputRequestedRegionEndIndex[dim] = itk::Math::Floor < IndexValueType > (inputRequestedRegionEndContinuousIndex[dim]);
            }

            InputSizeType inputFinalSize;
            InputIndexType inputFinalIndex;

            for (unsigned int dim = 0; dim < ImageDimension; ++dim)
            {
                inputFinalIndex[dim] = std::min(inputRequestedRegionStartIndex[dim], inputRequestedRegionEndIndex[dim]);
                inputFinalSize[dim] = std::max(inputRequestedRegionStartIndex[dim], inputRequestedRegionEndIndex[dim])
                        - inputRequestedRegionStartIndex[dim] + 1;
            }

            InputRegionType inputRequestedRegion;
            inputRequestedRegion.SetIndex(inputFinalIndex);
            inputRequestedRegion.SetSize(inputFinalSize);

            // Compute the padding due to the interpolator
            unsigned int interpolatorRadius = 4;

            // pad the input requested region by the operator radius
            inputRequestedRegion.PadByRadius(interpolatorRadius);

            // crop the input requested region at the input's largest possible region
            if (inputRequestedRegion.Crop(inputPtr->GetLargestPossibleRegion()))
            {
                inputPtr->SetRequestedRegion(inputRequestedRegion);
                //vnsLogSuperDebugMacro("InputRequested region computed final (Crop): "<<inputRequestedRegion)
            }
            else
            {

                inputFinalSize.Fill(0);
                inputRequestedRegion.SetSize(inputFinalSize);
                inputFinalIndex.Fill(0);
                inputRequestedRegion.SetIndex(inputFinalIndex);

                // store what we tried to request (prior to trying to crop)
                inputPtr->SetRequestedRegion(inputRequestedRegion);
//            typename ImageBaseType::RegionType inputRegion;
//            this->CallCopyOutputRegionToInputRegion( inputRegion, this->GetOutput()->GetRequestedRegion() );
//            input->SetRequestedRegion(inputRegion);
            }
        }
    /**
     * GenerateOutputInformation
     */
    template<class TInputImage, class TOutputImage>
        void
        DedicatedFastResampleImageFilter<TInputImage, TOutputImage>::GenerateOutputInformation()
        {
            // Call the superclass' implementation of this method
            Superclass::GenerateOutputInformation();

            // Get pointers to the input and output
            InputImagePointer inputPtr = const_cast<TInputImage *>(this->GetInput());
            OutputImagePointer outputPtr = this->GetOutput();

            if ((inputPtr.IsNull() == true) || (outputPtr.IsNull() == true))
            {
                return;
            }

            this->GetOutput()->SetNumberOfComponentsPerPixel(this->GetInput()->GetNumberOfComponentsPerPixel());

            typename TOutputImage::SpacingType outputSpacing;
            typename TOutputImage::SizeType outputSize;
            typename TOutputImage::IndexType outputStartIndex;
            typename TOutputImage::PointType outputOrigin;

            outputSpacing = m_AreaToOutputImageResolution.Spacing;
            outputSize = m_AreaToOutputImageResolution.Size;
            outputOrigin = m_AreaToOutputImageResolution.Origin;

            // Set the spacing and origin to the output image
            outputPtr->SetSpacing(outputSpacing);
            outputPtr->SetOrigin(outputOrigin);

            outputStartIndex.Fill(0);

//            // Set the largest possible region to the output image
            typename TOutputImage::RegionType outputLargestPossibleRegion;

            outputLargestPossibleRegion.SetSize(outputSize);
            outputLargestPossibleRegion.SetIndex(outputStartIndex);
            outputPtr->SetLargestPossibleRegion(outputLargestPossibleRegion);

        }

}
// end namespace vns

#endif
