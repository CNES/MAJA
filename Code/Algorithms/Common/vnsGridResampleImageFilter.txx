/*=========================================================================

 Program:   ORFEO Toolbox
 Language:  C++
 Date:      $Date$
 Version:   $Revision$


 Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
 See OTBCopyright.txt for details.


 This software is distributed WITHOUT ANY WARRANTY; without even
 the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 PURPOSE.  See the above copyright notices for more information.

 =========================================================================*/
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
 * VERSION : 2.1.0 : DM : LAIG-DM-MAJA-157814-CS : 18 mai 2017 : Refactoring, menage pour ameliorer qualite *
 * VERSION : 2.1.0 : FA : LAIG-FA-MAJA-157185-CS : 9 mai 2017 : Correction de règles qualité                *
 * VERSION : 1.0.0 : FA : LAIG-FA-MAC-1988-CNES : 21 octobre 2016 : Correction qualite (code mort)          *
 * VERSION : 1.0.0 : FA : LAIG-FA-MAC-148399-CS : 19 octobre 2016 : Audit code - Correction qualite         *
 * VERSION : 5-1-0 : FA : LAIG-FA-MAC-145739-CS : 25 juillet 2016 : Audit code - Correction warining        *
 * VERSION : 5-1-0 : FA : LAIG-FA-MAC-144674-CS : 26 mai 2016 : Modifications cosmetique et menage, etc...  *
 * VERSION : 5-0-0 : DM : LAIG-DM-MAC-1638-CNES : 1 mars 2016 : Nouveau filtre reechantillonnage issu OTB   *
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id: vnsCloudMaskingImageFilter.h 11465 2016-01-28 15:51:04Z tfeuvrie $
 *                                                                                                          *
 ************************************************************************************************************/

#ifndef __vnsGridResampleImageFilter_txx
#define __vnsGridResampleImageFilter_txx

#include "vnsGridResampleImageFilter.h"


#include "itkNumericTraits.h"
#include "itkImageScanlineIterator.h"
#include "itkContinuousIndex.h"

namespace vns
{

    template<typename TInputImage, typename TOutputImage, typename TInterpolatorPrecision>
        GridResampleImageFilter<TInputImage, TOutputImage, TInterpolatorPrecision>::GridResampleImageFilter() :
                m_OutputStartIndex(), m_OutputSize(), m_OutputOrigin(), m_OutputSpacing(), m_EdgePaddingValue(), m_CheckOutputBounds(true), m_Interpolator(), m_ReachableOutputRegion(), m_InterpolatorRadius(0)
        {
            // Set linear interpolator as default
            m_Interpolator = dynamic_cast<InterpolatorType *>(DefaultInterpolatorType::New().GetPointer());

            // Initialize EdgePaddingValue
            m_EdgePaddingValue = itk::NumericTraits<OutputPixelType>::ZeroValue(m_EdgePaddingValue);

            // Initialize origin and spacing
            m_OutputOrigin.Fill(0.);
            m_OutputSpacing.Fill(1.);
            m_OutputStartIndex.Fill(0);
            m_OutputSize.Fill(0);
        }

    /** Import output parameters from a given image */
    template<typename TInputImage, typename TOutputImage, typename TInterpolatorPrecision>
        void
        GridResampleImageFilter<TInputImage, TOutputImage, TInterpolatorPrecision>::SetOutputParametersFromImage(
                const ImageBaseType * image)
        {
            this->SetOutputOrigin(image->GetOrigin());
            this->SetOutputSpacing(image->GetSignedSpacing());
            this->SetOutputStartIndex(image->GetLargestPossibleRegion().GetIndex());
            this->SetOutputSize(image->GetLargestPossibleRegion().GetSize());
        }

    template<typename TInputImage, typename TOutputImage, typename TInterpolatorPrecision>
        void
        GridResampleImageFilter<TInputImage, TOutputImage, TInterpolatorPrecision>::GenerateOutputInformation()
        {
            // call the superclass' implementation of this method
            Superclass::GenerateOutputInformation();

            // get pointers to the input and output
            OutputImageType *outputPtr = this->GetOutput();
            if (!outputPtr)
            {
                return;
            }

            // Fill output image data
            typename TOutputImage::RegionType outputLargestPossibleRegion;
            outputLargestPossibleRegion.SetSize(m_OutputSize);
            outputLargestPossibleRegion.SetIndex(m_OutputStartIndex);

            outputPtr->SetLargestPossibleRegion(outputLargestPossibleRegion);
            outputPtr->SetSignedSpacing(m_OutputSpacing);
            outputPtr->SetOrigin(m_OutputOrigin);

        }

    template<typename TInputImage, typename TOutputImage, typename TInterpolatorPrecision>
        void
        GridResampleImageFilter<TInputImage, TOutputImage, TInterpolatorPrecision>::GenerateInputRequestedRegion()
        {
            // call the superclass's implementation of this method
            Superclass::GenerateInputRequestedRegion();

            // Check for input
            if (!this->GetInput())
            {
                return;
            }

            // get pointers to the input and output
            typename TInputImage::Pointer inputPtr = const_cast<TInputImage *>(this->GetInput());

            // check for output
            OutputImageType *outputPtr = this->GetOutput();
            if (!outputPtr)
            {
                return;
            }

            typename TOutputImage::RegionType outputRequestedRegion = outputPtr->GetRequestedRegion();

            IndexType outULIndex;
            IndexType outLRIndex;

            typedef itk::ContinuousIndex<double, TInputImage::ImageDimension> ContinuousIndexType;

            ContinuousIndexType inULCIndex;
            ContinuousIndexType inLRCIndex;

            // Get output image requested region corners as Index
            outULIndex = outputRequestedRegion.GetIndex();
            outLRIndex = outULIndex + outputRequestedRegion.GetSize();
            outLRIndex[0] -= 1;
            outLRIndex[1] -= 1;

            // Transform to physiscal points
            PointType outULPoint;
            PointType outLRPoint;
            outputPtr->TransformIndexToPhysicalPoint(outULIndex, outULPoint);
            outputPtr->TransformIndexToPhysicalPoint(outLRIndex, outLRPoint);

            // Transform to input image Index
            inputPtr->TransformPhysicalPointToContinuousIndex(outULPoint, inULCIndex);
            inputPtr->TransformPhysicalPointToContinuousIndex(outLRPoint, inLRCIndex);

            SizeType inSize;
            IndexType inULIndex;
            IndexType inLRIndex;

            // Reorder index properly and compute size
            for (unsigned int dim = 0; dim < ImageDimension; ++dim)
            {
                if (inULCIndex[dim] > inLRCIndex[dim])
                {
                    // swap
                    typename ContinuousIndexType::ValueType tmp(inULCIndex[dim]);
                    inULCIndex[dim] = inLRCIndex[dim];
                    inLRCIndex[dim] = tmp;
                }

                // Ensure correct rounding of coordinates

                inULIndex[dim] = static_cast<IndexValueType>(vcl_floor(inULCIndex[dim]));
                inLRIndex[dim] = static_cast<IndexValueType>(vcl_ceil(inLRCIndex[dim]));
                // Note THOMAS
                // centered pixel : OK
                // if footprint : KO
//                double l_OutputOrigin = m_OutputOrigin[dim];
//                double l_inputOrigin = inputOrigin[dim];
//                double l_inputSpacing = inputSpacing[dim];
//                unsigned int margin = vcl_ceil(vcl_abs( (l_OutputOrigin - l_inputOrigin ) / l_inputSpacing));
//                inULIndex[dim] -= margin;

                inSize[dim] = static_cast<typename SizeType::SizeValueType>(inLRIndex[dim] - inULIndex[dim]) + 1;
            }

            // Build the input requested region
            typename TInputImage::RegionType inputRequestedRegion;
            inputRequestedRegion.SetIndex(inULIndex);
            inputRequestedRegion.SetSize(inSize);

            // Compute the padding due to the interpolator
//            unsigned int interpolatorRadius =
//                    otb::StreamingTraits<typename Superclass::InputImageType>::CalculateNeededRadiusForInterpolator(
//                            this->GetInterpolator());
            inputRequestedRegion.PadByRadius(m_InterpolatorRadius); //interpolatorRadius);

            // crop the input requested region at the input's largest possible region
            if (inputRequestedRegion.Crop(inputPtr->GetLargestPossibleRegion()))
            {
                inputPtr->SetRequestedRegion(inputRequestedRegion);
            }
            else
            {
            	// Specific case when the input requested is out of the output one so request an empty zone
            	inULIndex[0] = inputPtr->GetLargestPossibleRegion().GetIndex()[0];
            	inULIndex[1] = inputPtr->GetLargestPossibleRegion().GetIndex()[1];
            	inSize[0] = 0;
            	inSize[1] = 0;
            	inputRequestedRegion.SetIndex(inULIndex);
            	inputRequestedRegion.SetSize(inSize);

            	// store what we tried to request (prior to trying to crop)
                inputPtr->SetRequestedRegion(inputRequestedRegion);
            }

        }

    template<typename TInputImage, typename TOutputImage, typename TInterpolatorPrecision>
        void
        GridResampleImageFilter<TInputImage, TOutputImage, TInterpolatorPrecision>::BeforeThreadedGenerateData()
        {
            if (!m_Interpolator)
            {
                itkExceptionMacro(<< "Interpolator not set");
            }

            // Connect input image to interpolator
            m_Interpolator->SetInputImage(this->GetInput());

            unsigned int nComponents = itk::DefaultConvertPixelTraits<OutputPixelType>::GetNumberOfComponents(m_EdgePaddingValue);

            if (nComponents == 0)
            {

                // Build a default value of the correct number of components
                OutputPixelComponentType zeroComponent = itk::NumericTraits<OutputPixelComponentType>::ZeroValue(zeroComponent);

                nComponents = this->GetInput()->GetNumberOfComponentsPerPixel();

                itk::NumericTraits<OutputPixelType>::SetLength(m_EdgePaddingValue, nComponents);
                for (unsigned int n = 0; n < nComponents; n++)
                {
                    OutputPixelConvertType::SetNthComponent(n, m_EdgePaddingValue, zeroComponent);
                }
            }

            // Compute ReachableOutputRegion
            // InputImage buffered region corresponds to a region of the ouptut
            // image. Computing it beforehand allows to save IsInsideBuffer
            // calls in the interpolation loop

            // Compute the padding due to the interpolator

            IndexType inUL = this->GetInput()->GetBufferedRegion().GetIndex();
            IndexType inLR = this->GetInput()->GetBufferedRegion().GetIndex() + this->GetInput()->GetBufferedRegion().GetSize();
            inLR[0] -= 1;
            inLR[1] -= 1;

            // We should take interpolation radius into account here, but this
            // does not match the IsInsideBuffer method
            // unsigned int interpolatorRadius =
            // StreamingTraits<typename Superclass::InputImageType>::CalculateNeededRadiusForInterpolator(this->GetInterpolator());
            // inUL[0]+=interpolatorRadius;
            // inUL[1]+=interpolatorRadius;
            // inLR[0]-=interpolatorRadius;
            // inLR[1]-=interpolatorRadius;

            PointType inULp;
            PointType inLRp;
            this->GetInput()->TransformIndexToPhysicalPoint(inUL, inULp);
            this->GetInput()->TransformIndexToPhysicalPoint(inLR, inLRp);

            inULp -= 0.5 * this->GetInput()->GetSignedSpacing();
            inLRp += 0.5 * this->GetInput()->GetSignedSpacing();

            ContinuousInputIndexType outUL;
            ContinuousInputIndexType outLR;
            this->GetOutput()->TransformPhysicalPointToContinuousIndex(inULp, outUL);
            this->GetOutput()->TransformPhysicalPointToContinuousIndex(inLRp, outLR);

            IndexType outputIndex;
            // This needs to take into account negative spacing
            outputIndex[0] = static_cast<IndexValueType>(vcl_ceil(std::min(outUL[0],outLR[0])));
            outputIndex[1] = static_cast<IndexValueType>(vcl_ceil(std::min(outUL[1],outLR[1])));

            SizeType outputSize;
            outputSize[0] = static_cast<SizeValueType>(vcl_floor(std::max(outUL[0],outLR[0])) - outputIndex[0] + 1);
            outputSize[1] = static_cast<SizeValueType>(vcl_floor(std::max(outUL[1],outLR[1])) - outputIndex[1] + 1);

            m_ReachableOutputRegion.SetIndex(outputIndex);
            m_ReachableOutputRegion.SetSize(outputSize);
        }

    template<typename TInputImage, typename TOutputImage, typename TInterpolatorPrecision>
        void
        GridResampleImageFilter<TInputImage, TOutputImage, TInterpolatorPrecision>::ThreadedGenerateData(
                const OutputImageRegionType& outputRegionForThread, itk::ThreadIdType /*threadId*/)
        {
            // Get the output pointers
            OutputImageType *outputPtr = this->GetOutput();

            // Get this input pointers
            const InputImageType *inputPtr = this->GetInput();

            // Min/max values of the output pixel type AND these values
            // represented as the output type of the interpolator
            const OutputPixelComponentType minValue = itk::NumericTraits<OutputPixelComponentType>::NonpositiveMin();
            const OutputPixelComponentType maxValue = itk::NumericTraits<OutputPixelComponentType>::max();

            const InterpolatorComponentType minOutputValue = static_cast<InterpolatorComponentType>(minValue);
            const InterpolatorComponentType maxOutputValue = static_cast<InterpolatorComponentType>(maxValue);

            // Iterator on the output region for current thread
            OutputImageRegionType regionToCompute = outputRegionForThread;
            bool cropSucceed = regionToCompute.Crop(m_ReachableOutputRegion);

            // Fill thread buffer
            itk::ImageScanlineIterator<OutputImageType> outItFull(outputPtr, outputRegionForThread);
            outItFull.GoToBegin();
            while (!outItFull.IsAtEnd())
            {
                while (!outItFull.IsAtEndOfLine())
                {
                    outItFull.Set(m_EdgePaddingValue);
                    ++outItFull;
                }
                outItFull.NextLine();
            }

            if (!cropSucceed)
                return;

            itk::ImageScanlineIterator<OutputImageType> outIt(outputPtr, regionToCompute);

            // Temporary variables for loop
            PointType outPoint;
            ContinuousInputIndexType inCIndex;
            InterpolatorOutputType interpolatorValue; //(this->GetOutput()->GetNumberOfComponentsPerPixel());
            OutputPixelType outputValue; //(this->GetOutput()->GetNumberOfComponentsPerPixel());
            double delta;

            assert(inputPtr->GetSignedSpacing()[0] != 0 && "Null spacing will cause division by zero.");
            delta = outputPtr->GetSignedSpacing()[0] / inputPtr->GetSignedSpacing()[0];

            // Iterate through the output region
            outIt.GoToBegin();

            while (!outIt.IsAtEnd())
            {
                // Map output index to input continuous index
                outputPtr->TransformIndexToPhysicalPoint(outIt.GetIndex(), outPoint);
                inputPtr->TransformPhysicalPointToContinuousIndex(outPoint, inCIndex);

                while (!outIt.IsAtEndOfLine())
                {
                    // Interpolate
                    interpolatorValue = m_Interpolator->EvaluateAtContinuousIndex(inCIndex);

                    // Cast and check bounds
                    outputValue = this->CastPixelWithBoundsChecking(interpolatorValue, minOutputValue, maxOutputValue);

                    // Set output value
                    outIt.Set(outputValue);

                    // move one pixel forward
                    ++outIt;

                    // Update input position
                    inCIndex[0] += delta;
                }

                // Move to next line
                outIt.NextLine();
            }

        }
    template<typename TInputImage, typename TOutputImage, typename TInterpolatorPrecision>
        void
        GridResampleImageFilter<TInputImage, TOutputImage, TInterpolatorPrecision>::AfterThreadedGenerateData()
        {
            // Disconnect input image from the interpolator
            m_Interpolator->SetInputImage(ITK_NULLPTR);
        }

    template<typename TInputImage, typename TOutputImage, typename TInterpolatorPrecision>
        itk::ModifiedTimeType
        GridResampleImageFilter<TInputImage, TOutputImage, TInterpolatorPrecision>::GetMTime(void) const
        {
            itk::ModifiedTimeType latestTime = itk::Object::GetMTime();

            if (m_Interpolator)
            {
                if (latestTime < m_Interpolator->GetMTime())
                {
                    latestTime = m_Interpolator->GetMTime();
                }
            }

            return latestTime;
        }

    template<typename TInputImage, typename TOutputImage, typename TInterpolatorPrecision>
        void
        GridResampleImageFilter<TInputImage, TOutputImage, TInterpolatorPrecision>::PrintSelf(std::ostream& os, itk::Indent indent) const
        {
            Superclass::PrintSelf(os, indent);

            os << indent << "EdgePaddingValue: " << static_cast<typename itk::NumericTraits<OutputPixelType>::PrintType>(m_EdgePaddingValue)
                    << std::endl;
            os << indent << "OutputStartIndex: " << m_OutputStartIndex << std::endl;
            os << indent << "OutputSize: " << m_OutputSize << std::endl;
            os << indent << "OutputOrigin: " << m_OutputOrigin << std::endl;
            os << indent << "OutputSpacing: " << m_OutputSpacing << std::endl;
            os << indent << "Interpolator: " << m_Interpolator.GetPointer() << std::endl;
            os << indent << "CheckOutputBounds: " << (m_CheckOutputBounds ? "On" : "Off") << std::endl;
        }

} // namespace otb

#endif
