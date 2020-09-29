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
 * VERSION : 4-0-0 : FA : LAIG-FA-MAC-121054-CS : 24 avril 2014 : Creation suite à la correction de la      *
 *                                                          rasterisation des angles PDV et solaire pour S2 *
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id: vnsRawExpandVectorImageFilter.txx 2656 2011-02-15 10:25:38Z cvallade $
 *                                                                                                          *
 ************************************************************************************************************/
#ifndef __vnsRawExpandVectorImageFilter_txx
#define __vnsRawExpandVectorImageFilter_txx

#include "vnsRawExpandVectorImageFilter.h"
#include "itkImageRegionIteratorWithIndex.h"
#include "itkMacro.h"
#include "itkObjectFactory.h"
#include "itkNumericTraits.h"
#include "vnsMath.h"

namespace vns
{

    /**
     * Default constructor
     */
    template<class TInputImage, class TOutputImage>
        RawExpandVectorImageFilter<TInputImage, TOutputImage>::RawExpandVectorImageFilter(void)
        {
            // ---------------------------------------------------------------------------------------------
            // Set default factors to 1
            for (unsigned int j = 0; j < ImageDimension; j++)
            {
                m_ExpandFactors[j] = 1;
            }

            m_Interpolator = InterpolatorType::New();

            // ---------------------------------------------------------------------------------------------
            // Set default padding value to zero
            m_EdgePaddingValue.SetSize(1);
            m_EdgePaddingValue.Fill(1000); //TODO : faire un init plus propre !! itk::NumericTraits<OutputInternalPixelType>::Zero);

            m_IsSetEdgePaddingValue = false;
        }

    /**
     * Standard "PrintSelf" method
     */
    template<class TInputImage, class TOutputImage>
        void
        RawExpandVectorImageFilter<TInputImage, TOutputImage>::PrintSelf(std::ostream& os, itk::Indent indent) const
        {
            // ---------------------------------------------------------------------------------------------
            Superclass::PrintSelf(os, indent);

            unsigned int j;
            // ---------------------------------------------------------------------------------------------
            // Print the factor
            os << indent << "ExpandFactors: [";
            for (j = 0; j < ImageDimension - 1; j++)
            {
                os << m_ExpandFactors[j] << ", ";
            }
            os << m_ExpandFactors[j] << "]" << std::endl;

            // ---------------------------------------------------------------------------------------------
            // Print the Interpolator
            os << indent << "Interpolator: ";
            os << m_Interpolator.GetPointer() << std::endl;

            // ---------------------------------------------------------------------------------------------
            // Print the padding value
            os << indent << "EdgePaddingValue: " << static_cast<typename itk::NumericTraits<OutputPixelType>::PrintType>(m_EdgePaddingValue)
                    << std::endl;
            os << indent << "EdgePaddingValue: ";
            os << m_EdgePaddingValue << std::endl;
        }

    /**
     * Set expand factors from an array of unsigned int.
     */
    template<class TInputImage, class TOutputImage>
        void
        RawExpandVectorImageFilter<TInputImage, TOutputImage>::SetExpandFactors(const ExpandFactorsType factors[])
        {
            // ---------------------------------------------------------------------------------------------
            //Check the ExpandFactors is modified
            unsigned int j;
            for (j = 0; j < ImageDimension; j++)
            {
                if (factors[j] != m_ExpandFactors[j])
                {
                    break;
                }
            }
            // ---------------------------------------------------------------------------------------------
            // Update the expand factor if necessary
            if (j < ImageDimension)
            {
                this->Modified();
                for (j = 0; j < ImageDimension; j++)
                {
                    m_ExpandFactors[j] = factors[j];
                    // // MACCS adaptation for ExpandFactor in ]0;1]                    if (m_ExpandFactors[j] < 1)
                    if (m_ExpandFactors[j] < 0)
                    {
                        m_ExpandFactors[j] = 1;
                    }
                }
            }

        }

    /**
     * Set expand factors from a single unsigned int
     */
    template<class TInputImage, class TOutputImage>
        void
        RawExpandVectorImageFilter<TInputImage, TOutputImage>::SetExpandFactors(const ExpandFactorsType factor)
        {

            // ---------------------------------------------------------------------------------------------
            // Loop on factor and check if modified
            unsigned int j;
            for (j = 0; j < ImageDimension; j++)
            {
                if (factor != m_ExpandFactors[j])
                {
                    break;
                }
            }
            // ---------------------------------------------------------------------------------------------
            // If modified
            if (j < ImageDimension)
            {
                this->Modified();
                // ---------------------------------------------------------------------------------------------
                // Loop on the dimension and set the new factor values
                for (j = 0; j < ImageDimension; j++)
                {
                    m_ExpandFactors[j] = factor;
                    // // MACCS adaptation for ExpandFactor in ]0;1]                    if (m_ExpandFactors[j] < 1)
                    if (m_ExpandFactors[j] < 0)
                    {
                        m_ExpandFactors[j] = 1;
                    }
                }
            }

        }

    /**
     * BeforeThreadedGenerateData
     */
    template<class TInputImage, class TOutputImage>
        void
        RawExpandVectorImageFilter<TInputImage, TOutputImage>::BeforeThreadedGenerateData()
        {

            // ---------------------------------------------------------------------------------------------
            // Checks the input and the Interpolator variables
            if ((!m_Interpolator) || (!this->GetInput()))
            {
                itkExceptionMacro(<< "Interpolator and/or Input not set");
            }
            // ---------------------------------------------------------------------------------------------
            if (m_EdgePaddingValue.GetSize() != this->GetInput()->GetNumberOfComponentsPerPixel())
            {
                // ---------------------------------------------------------------------------------------------
                if (m_IsSetEdgePaddingValue)
                {
                    itkExceptionMacro(<< "Invalid m_EdgePaddingValue "<<m_EdgePaddingValue<<", size mismatch.");
                }
                // ---------------------------------------------------------------------------------------------
                else
                {
                    m_EdgePaddingValue.SetSize(this->GetInput()->GetNumberOfComponentsPerPixel());
                    m_EdgePaddingValue.Fill(itk::NumericTraits < OutputInternalPixelType > ::Zero);
                }
            }

            // ---------------------------------------------------------------------------------------------
            // Connect input image to interpolator
            m_Interpolator->SetInputImage(this->GetInput());

        }

    /**
     * ThreadedGenerateData
     */
    template<class TInputImage, class TOutputImage>
        void
        RawExpandVectorImageFilter<TInputImage, TOutputImage>::ThreadedGenerateData(const OutputImageRegionType& outputRegionForThread,
                itk::ThreadIdType /*threadId*/)
        {
        // ---------------------------------------------------------------------------------------------
            // Get the input and output pointers
            OutputImagePointer outputPtr = this->GetOutput();

            // ---------------------------------------------------------------------------------------------
            // Load the number of channels
            unsigned int l_Size(0);
            l_Size = outputPtr->GetNumberOfComponentsPerPixel();

            OutputPixelType outPix;
            outPix.SetSize(l_Size);
            // ---------------------------------------------------------------------------------------------
            // Iterator for walking the output
            typedef itk::ImageRegionIteratorWithIndex<TOutputImage> OutputIterator;

            OutputIterator outIt(outputPtr, outputRegionForThread);

            // ---------------------------------------------------------------------------------------------
            // Define a few indices that will be used to translate from an input
            // pixel to and output pixel
            typename TOutputImage::IndexType outputIndex;
            typename InterpolatorType::ContinuousIndexType inputIndex;

            // ---------------------------------------------------------------------------------------------
            // Support progress methods/callbacks

            // ---------------------------------------------------------------------------------------------
            // Walk the output region, and interpolate the input image
            while (outIt.IsAtEnd() == false)
            {
                // ---------------------------------------------------------------------------------------------
                // Determine the index of the output pixel
                outputIndex = outIt.GetIndex();

                // Determine the input pixel location associated with this output pixel.
                // Don't need to check for division by zero because the factors are
                // clamped to be minimum for 1.
                for (unsigned int j = 0; j < ImageDimension; j++)
                {
#if 1 // MACCS 4.2 ITK_USE_CENTERED_PIXEL_COORDINATES_CONSISTENTLY
                    inputIndex[j] = ( static_cast<double> (outputIndex[j]) + 0.5 ) / static_cast<double> (m_ExpandFactors[j]) - 0.5;
#else
                    inputIndex[j] = static_cast<double>(outputIndex[j]) / static_cast<double>(m_ExpandFactors[j]);
#endif
                }

                // ---------------------------------------------------------------------------------------------
                // interpolate value and write to output
                if (m_Interpolator->IsInsideBuffer(inputIndex))
                {
                    OutputPixelType output;
                    output.SetSize(l_Size);
                    InterpolatorOutputType interpVal = m_Interpolator->EvaluateAtContinuousIndex(inputIndex);
                    for (unsigned int l = 0; l < l_Size; l++)
                    {
                        output[l] = static_cast<OutputInternalPixelType>(interpVal[l] + CONST_EPSILON);
                    }
                    outIt.Set(output);
                }
                // ---------------------------------------------------------------------------------------------
                else
                {
                    outIt.Set(m_EdgePaddingValue);
                }
                // ---------------------------------------------------------------------------------------------
                ++outIt;
            }

        }

    /**
     * GenerateInputRequesteRegion
     */
    template<class TInputImage, class TOutputImage>
        void
        RawExpandVectorImageFilter<TInputImage, TOutputImage>::GenerateInputRequestedRegion()
        {
        // ---------------------------------------------------------------------------------------------
            // Call the superclass' implementation of this method
            Superclass::GenerateInputRequestedRegion();

            // ---------------------------------------------------------------------------------------------
            // Get pointers to the input and output
            InputImagePointer inputPtr = const_cast<TInputImage *>(this->GetInput());
            OutputImagePointer outputPtr = this->GetOutput();

            if ((inputPtr.IsNull() == true) || (outputPtr.IsNull() == true))
            {
                return;
            }

            // ---------------------------------------------------------------------------------------------
            // We need to compute the input requested region (size and start index)
            unsigned int i;
            const typename TOutputImage::SizeType& outputRequestedRegionSize = outputPtr->GetRequestedRegion().GetSize();
            const typename TOutputImage::IndexType& outputRequestedRegionStartIndex = outputPtr->GetRequestedRegion().GetIndex();

            typename TInputImage::SizeType inputRequestedRegionSize;
            typename TInputImage::IndexType inputRequestedRegionStartIndex;

            // ---------------------------------------------------------------------------------------------
            // inputRequestedSize = (outputRequestedSize / ExpandFactor) + 1)
            // The extra 1 above is to take care of edge effects when streaming.
            for (i = 0; i < TInputImage::ImageDimension; i++)
            {
#if 1 // MACCS 4.2 ITK_USE_CENTERED_PIXEL_COORDINATES_CONSISTENTLY
                inputRequestedRegionSize[i]
                = static_cast<long> ( vcl_ceil(static_cast<double> (outputRequestedRegionSize[i]) /
                                static_cast<double> (m_ExpandFactors[i]) ) + 1 );
#else
                // ---------------------------------------------------------------------------------------------
                // Add one more line, to take care of edge effects when streaming
                inputRequestedRegionSize[i] = static_cast<long>(vcl_ceil(
                static_cast<double>(outputRequestedRegionSize[i]) / static_cast<double>(m_ExpandFactors[i])) + 1 + 1);
#endif
                inputRequestedRegionStartIndex[i] = static_cast<long>(vcl_floor(
                static_cast<double>(outputRequestedRegionStartIndex[i]) / static_cast<double>(m_ExpandFactors[i])));
            }

            // ---------------------------------------------------------------------------------------------
            typename TInputImage::RegionType inputRequestedRegion;
            inputRequestedRegion.SetSize(inputRequestedRegionSize);
            inputRequestedRegion.SetIndex(inputRequestedRegionStartIndex);

            // ---------------------------------------------------------------------------------------------
            // Make sure the requested region is within largest possible.
            inputRequestedRegion.Crop(inputPtr->GetLargestPossibleRegion());

            // ---------------------------------------------------------------------------------------------
            // Set the input requested region.
            inputPtr->SetRequestedRegion(inputRequestedRegion);

        }

    /**
     * GenerateOutputInformation
     */
    template<class TInputImage, class TOutputImage>
        void
        RawExpandVectorImageFilter<TInputImage, TOutputImage>::GenerateOutputInformation()
        {
        // ---------------------------------------------------------------------------------------------
            // Call the superclass' implementation of this method
            Superclass::GenerateOutputInformation();

            // ---------------------------------------------------------------------------------------------
            // Get pointers to the input and output
            InputImagePointer inputPtr = const_cast<TInputImage *>(this->GetInput());
            OutputImagePointer outputPtr = this->GetOutput();

            if ((inputPtr.IsNull() == true) || (outputPtr.IsNull() == true))
            {
                return;
            }

            this->GetOutput()->SetNumberOfComponentsPerPixel(this->GetInput()->GetNumberOfComponentsPerPixel());

            // ---------------------------------------------------------------------------------------------
            // We need to compute the output spacing, the output image size, and the
            // output image start index
            const typename TInputImage::SpacingType& inputSpacing = inputPtr->GetSpacing();
            const typename TInputImage::SizeType& inputSize = inputPtr->GetLargestPossibleRegion().GetSize();
            const typename TInputImage::IndexType& inputStartIndex = inputPtr->GetLargestPossibleRegion().GetIndex();
            const typename TInputImage::PointType& inputOrigin = inputPtr->GetOrigin();

            typename TOutputImage::SpacingType outputSpacing;
            typename TOutputImage::SizeType outputSize;
            typename TOutputImage::IndexType outputStartIndex;
            typename TOutputImage::PointType outputOrigin;

            typename TInputImage::SpacingType inputOriginShift;

            // ---------------------------------------------------------------------------------------------
            for (unsigned int i = 0; i < TOutputImage::ImageDimension; i++)
            {
                outputSpacing[i] = inputSpacing[i] / (float) m_ExpandFactors[i];
                outputSize[i] = (unsigned long) ((ExpandFactorsType) inputSize[i] * m_ExpandFactors[i] + (ExpandFactorsType) 0.5);
                outputStartIndex[i] = (long) ((ExpandFactorsType) inputStartIndex[i] * m_ExpandFactors[i] + (ExpandFactorsType) 0.5);
#if 1 // MACCS 4.2 ITK_USE_CENTERED_PIXEL_COORDINATES_CONSISTENTLY
                const double fraction = (double)(m_ExpandFactors[i] - 1) / (double)m_ExpandFactors[i];
                inputOriginShift[i] = - ( inputSpacing[i] / 2.0 ) * fraction;
#endif
            }

#if 1 // MACCS 4.2 ITK_USE_CENTERED_PIXEL_COORDINATES_CONSISTENTLY
            const typename TInputImage::DirectionType inputDirection = inputPtr->GetDirection();
            const typename TOutputImage::SpacingType outputOriginShift = inputDirection * inputOriginShift;

            outputOrigin = inputOrigin + outputOriginShift;
#else
            outputOrigin = inputOrigin;
#endif

            // ---------------------------------------------------------------------------------------------
            // Set the spacing and origin to the output image
            outputPtr->SetSpacing(outputSpacing);
            outputPtr->SetOrigin(outputOrigin);

            // ---------------------------------------------------------------------------------------------
            // Set the largest possible region to the output image
            typename TOutputImage::RegionType outputLargestPossibleRegion;
            outputLargestPossibleRegion.SetSize(outputSize);
            outputLargestPossibleRegion.SetIndex(outputStartIndex);
            outputPtr->SetLargestPossibleRegion(outputLargestPossibleRegion);
        }

} // end namespace vns

#endif
