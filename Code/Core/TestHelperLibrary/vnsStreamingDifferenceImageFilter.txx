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
 * VERSION : 2.0.0 : DM : LAIG-DM-MAJA-153436-CS : 13 mars 2017 : Refactoring pour ameliorer la qualite     *
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id$
 *                                                                                                          *
 ************************************************************************************************************/

#ifndef __vnsStreamingDifferenceImageFilter_txx
#define __vnsStreamingDifferenceImageFilter_txx

#include <vnsStreamingDifferenceImageFilter.h>

#include "itkConstNeighborhoodIterator.h"
#include "itkImageRegionIterator.h"
#include "itkNeighborhoodAlgorithm.h"
#include "itkProgressReporter.h"

namespace vns
{

//----------------------------------------------------------------------------
    template<class TInputImage, class TOutputImage>
        PersistentDifferenceImageFilter<TInputImage, TOutputImage>::PersistentDifferenceImageFilter()
        {
            // We require two inputs to execute.
            this->SetNumberOfRequiredInputs(2);

            // Set the default DifferenceThreshold.
            m_DifferenceThreshold = itk::NumericTraits<ScalarRealType>::Zero;

            // Set the default ToleranceRadius.
            m_ToleranceRadius = 0;

            // Initialize statistics about difference image.
            m_MeanDifference.SetSize(0);
            m_TotalDifference.SetSize(0);
            m_MeanDifference = itk::NumericTraits<RealType>::ZeroValue(m_MeanDifference);
            m_TotalDifference = itk::NumericTraits<AccumulateType>::ZeroValue(m_TotalDifference);
            m_NumberOfPixelsWithDifferences = 0;
        }

//----------------------------------------------------------------------------
    template<class TInputImage, class TOutputImage>
        void
        PersistentDifferenceImageFilter<TInputImage, TOutputImage>::PrintSelf(std::ostream& os, itk::Indent indent) const
        {
            this->Superclass::PrintSelf(os, indent);
            os << indent << "ToleranceRadius: " << m_ToleranceRadius << "\n";
            os << indent << "DifferenceThreshold: " << m_DifferenceThreshold << "\n";
            os << indent << "MeanDifference: " << m_MeanDifference << "\n";
            os << indent << "TotalDifference: " << m_TotalDifference << "\n";
            os << indent << "NumberOfPixelsWithDifferences: " << m_NumberOfPixelsWithDifferences << "\n";
        }

//----------------------------------------------------------------------------
    template<class TInputImage, class TOutputImage>
        void
        PersistentDifferenceImageFilter<TInputImage, TOutputImage>::SetValidInput(const InputImageType* validImage)
        {
            // The valid image should be input 0.
            this->SetInput(0, validImage);
        }

//----------------------------------------------------------------------------
    template<class TInputImage, class TOutputImage>
        void
        PersistentDifferenceImageFilter<TInputImage, TOutputImage>::SetTestInput(const InputImageType* testImage)
        {
            // The test image should be input 1.
            this->SetInput(1, testImage);
        }
    template<class TInputImage, class TOutputImage>
        void
        PersistentDifferenceImageFilter<TInputImage, TOutputImage>::GenerateOutputInformation()
        {
            Superclass::GenerateOutputInformation();
            if (this->GetInput(0)->GetNumberOfComponentsPerPixel() != this->GetInput(1)->GetNumberOfComponentsPerPixel())
            {
                itkExceptionMacro(
                        << "Image 1 has " << this->GetInput( 0)->GetNumberOfComponentsPerPixel() << " bands, whereas image 2 has " << this->GetInput( 1)->GetNumberOfComponentsPerPixel());
            }
            this->GetOutput()->SetNumberOfComponentsPerPixel(this->GetInput(0)->GetNumberOfComponentsPerPixel());
        }
//----------------------------------------------------------------------------
    template<class TInputImage, class TOutputImage>
        void
        PersistentDifferenceImageFilter<TInputImage, TOutputImage>::Reset()
        {
            int numberOfThreads = this->GetNumberOfThreads();

            m_MeanDifference.SetSize(this->GetInput(0)->GetNumberOfComponentsPerPixel());
            m_TotalDifference.SetSize(this->GetInput(0)->GetNumberOfComponentsPerPixel());

            // Initialize statistics about difference image.
            m_MeanDifference.Fill(itk::NumericTraits<ScalarRealType>::Zero);
            m_TotalDifference.Fill(itk::NumericTraits<ScalarRealType>::Zero);
            m_NumberOfPixelsWithDifferences = 0;
            // Resize the thread temporaries
            m_ThreadDifferenceSum.reserve(numberOfThreads);
            m_ThreadNumberOfDifferentPixels.SetSize(numberOfThreads);
            m_ThreadNumberOfPixels.SetSize(numberOfThreads);

            // Initialize the temporaries
            for (int i = 0; i < numberOfThreads; ++i)
            {
                m_ThreadDifferenceSum.push_back(m_TotalDifference);
            }
            m_ThreadNumberOfPixels.Fill(0);
            m_ThreadNumberOfDifferentPixels.Fill(0);
        }

//----------------------------------------------------------------------------
    template<class TInputImage, class TOutputImage>
        void
        PersistentDifferenceImageFilter<TInputImage, TOutputImage>::ThreadedGenerateData(const OutputImageRegionType& threadRegion,
                itk::ThreadIdType threadId)
        {
            typedef itk::ConstNeighborhoodIterator<InputImageType> SmartIterator;
            typedef itk::ImageRegionConstIterator<InputImageType> InputIterator;
            typedef itk::ImageRegionIterator<OutputImageType> OutputIterator;
            typedef itk::NeighborhoodAlgorithm::ImageBoundaryFacesCalculator<InputImageType> FacesCalculator;
            typedef typename FacesCalculator::RadiusType RadiusType;
            typedef typename FacesCalculator::FaceListType FaceListType;
            typedef typename FaceListType::iterator FaceListIterator;
            typedef typename InputImageType::PixelType InputPixelType;

            // Prepare standard boundary condition.
            itk::ZeroFluxNeumannBoundaryCondition<InputImageType> nbc;

            // Get a pointer to each image.
            const InputImageType* validImage = this->GetInput(0);
            const InputImageType* testImage = this->GetInput(1);
            OutputImageType* outputPtr = this->GetOutput();

            // Create a radius of pixels.
            RadiusType radius;
            if (m_ToleranceRadius > 0)
            {
                radius.Fill(m_ToleranceRadius);
            }
            else
            {
                radius.Fill(0);
            }

            // Find the data-set boundary faces.
            FacesCalculator boundaryCalculator;
            FaceListType faceList = boundaryCalculator(testImage, threadRegion, radius);

            // Support progress methods/callbacks.
            itk::ProgressReporter progress(this, threadId, threadRegion.GetNumberOfPixels());

            // Process the internal face and each of the boundary faces.
            for (FaceListIterator face = faceList.begin(); face != faceList.end(); ++face)
            {
                SmartIterator test(radius, testImage, *face); // Iterate over test image.
                InputIterator valid(validImage, *face);       // Iterate over valid image.
                OutputIterator out(outputPtr, *face);         // Iterate over output image.
                test.OverrideBoundaryCondition(&nbc);

                for (valid.GoToBegin(), test.GoToBegin(), out.GoToBegin(); !valid.IsAtEnd(); ++valid, ++test, ++out)
                {
                    // Get the current valid pixel.
                    InputPixelType t = valid.Get();

                    // Find the closest-valued pixel in the neighborhood of the test
                    // image.
                    OutputPixelType minimumDifference = itk::NumericTraits<OutputPixelType>::max(t);
                    unsigned int neighborhoodSize = test.Size();
                    for (unsigned int i = 0; i < neighborhoodSize; ++i)
                    {
                        // Use the RealType for the difference to make sure we get the
                        // sign.
                        RealType difference = static_cast<RealType>(t) - static_cast<RealType>(test.GetPixel(i));

                        for (unsigned int j = 0; j < difference.Size(); ++j)
                        {
                            if (difference[j] < 0)
                            {
                                difference[j] *= -1;
                            }
                            ScalarRealType d = static_cast<ScalarRealType>(difference[j]);

                            if (d < minimumDifference[j])
                            {
                                minimumDifference[j] = d;
                            }
                        }
                    }

                    //for complex and vector type. module might be better
                    //        ScalarRealType tMax=vcl_abs(t[0]);
                    ScalarRealType tMax = 0.01; //Avoiding the 0 case for neighborhood computing
                    // NB: still more restrictive than before for small values.
                    for (unsigned int j = 0; j < t.Size(); ++j)
                    {
                        if (vcl_abs(t[j]) > tMax) tMax = vcl_abs(t[j]);
                    }

                    // Check if difference is above threshold
                    // the threshold is interpreted as relative to the value
                    bool isDifferent = false;

                    for (unsigned int j = 0; j < minimumDifference.Size(); ++j)
                    {
                        if (minimumDifference[j] > m_DifferenceThreshold * tMax)
                        {
                            isDifferent = true;
                        }
                    }

                    if (isDifferent)
                    {
                        // Store the minimum difference value in the output image.
                        out.Set(minimumDifference);

                        // Update difference image statistics.
                        m_ThreadDifferenceSum[threadId] += minimumDifference;
                        m_ThreadNumberOfDifferentPixels[threadId]++;
                    }
                    else
                    {
                        // Difference is below threshold.
                        out.Set(itk::NumericTraits<OutputPixelType>::ZeroValue(minimumDifference));
                    }
                    //Increment number of pixels treated
                    m_ThreadNumberOfPixels[threadId]++;
                    // Update progress.
                    progress.CompletedPixel();
                }
            }

        }
//----------------------------------------------------------------------------
    template<class TInputImage, class TOutputImage>
        void
        PersistentDifferenceImageFilter<TInputImage, TOutputImage>::Synthetize()
        {
            // Set statistics about difference image.
            int numberOfThreads = this->GetNumberOfThreads();
            unsigned long l_NumberOfPixels = 0;
            for (int i = 0; i < numberOfThreads; ++i)
            {
                m_TotalDifference += m_ThreadDifferenceSum[i];
                m_NumberOfPixelsWithDifferences += m_ThreadNumberOfDifferentPixels[i];
                l_NumberOfPixels += m_ThreadNumberOfPixels[i];
            }

            // Calculate the mean difference.
            m_MeanDifference = m_TotalDifference / l_NumberOfPixels;
        }

} // end namespace vns

#endif
