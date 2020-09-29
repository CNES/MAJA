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
 * VERSION : 1.0.0 : FA : LAIG-FA-MAC-148399-CS : 27 octobre 2016 : Audit code - Correction qualite         *
 * VERSION : 1.0.0 : FA : LAIG-FA-MAC-1989-CNES : 13 octobre 2016 : Correction qualite (FIX ME)             *
 * VERSION : 5-0-0 : DM : LAIG-DM-MAC-982-CNES : 5 avr. 2016 : Creation                                     *
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id:
 *                                                                                                          *
 ************************************************************************************************************/
#ifndef __ShadowVariationThresholdValueEstimator_txx
#define __ShadowVariationThresholdValueEstimator_txx
#include "vnsShadowVariationThresholdValueEstimator.h"

#include "itkImageRegionIterator.h"
#include "itkImageRegionConstIteratorWithIndex.h"
#include "otbMacro.h"

namespace vns
{

    template<class TInputImage>
        PersistentShadowVariationThresholdValueEstimator<TInputImage>::PersistentShadowVariationThresholdValueEstimator() :
                m_ThreadHistogramList(), m_Size(), m_HistogramMin(0), m_HistogramMax(10), m_NoDataValue(
                        itk::NumericTraits<InternalPixelType>::Zero), m_ShadVarPercentile(0.), m_ShadVarDarkeningMax(0.4)
        {
            // first output is a copy of the image, DataObject created by
            // superclass
            //
            // allocate the data objects for the outputs which are
            // just decorators around pixel types and histogram list
            HistogramListPointerType output = static_cast<HistogramListType*>(this->MakeOutput(1).GetPointer());
            this->itk::ProcessObject::SetNthOutput(1, output.GetPointer());

            m_Size.SetSize(1);
            m_HistogramMin.SetSize(1);
            m_HistogramMax.SetSize(1);

            m_Size.Fill(1000);
            m_HistogramMin.Fill(itk::NumericTraits<InternalPixelType>::Zero);
            m_HistogramMax.Fill(10);

        }

    template<class TInputImage>
        itk::DataObject::Pointer
        PersistentShadowVariationThresholdValueEstimator<TInputImage>::MakeOutput(unsigned int output)
        {
            switch (output)
                {
            case 0:
                return static_cast<itk::DataObject*>(TInputImage::New().GetPointer());
            case 1:
                return static_cast<itk::DataObject*>(HistogramListType::New().GetPointer());
            default:
                return static_cast<itk::DataObject*>(TInputImage::New().GetPointer());
                }
        }

    template<class TInputImage>
        void
        PersistentShadowVariationThresholdValueEstimator<TInputImage>::GenerateOutputInformation()
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

    template<class TInputImage>
        void
        PersistentShadowVariationThresholdValueEstimator<TInputImage>::AllocateOutputs()
        {
            // This is commented to prevent the streaming of the whole image for the first stream strip
            // It shall not cause any problem because the output image of this filter is not intended to be used.
            //InputImagePointer image = const_cast< TInputImage * >( this->GetInput() );
            //this->GraftOutput( image );
            // Nothing that needs to be allocated for the remaining outputs
        }

    template<class TInputImage>
        void
        PersistentShadowVariationThresholdValueEstimator<TInputImage>::Reset()
        {
            TInputImage * inputPtr = const_cast<TInputImage *>(this->GetInput());
            inputPtr->UpdateOutputInformation();

            unsigned int numberOfThreads = this->GetNumberOfThreads();
            unsigned int numberOfComponents = inputPtr->GetNumberOfComponentsPerPixel();
            if (m_Size.Size() != numberOfComponents)
            {
                m_Size.SetSize(numberOfComponents);
                m_Size.Fill(1000);
            }

            bool clipBins = false;

            // if histogram Min and Max have the wrong size : set to default [0, 1000]
            if (m_HistogramMin.Size() != numberOfComponents || m_HistogramMax.Size() != numberOfComponents)
            {
                m_HistogramMin.SetSize(numberOfComponents);
                m_HistogramMax.SetSize(numberOfComponents);

                m_HistogramMin.Fill(itk::NumericTraits<InternalPixelType>::Zero);
                m_HistogramMax.Fill(10);
            }

            // Setup output histogram
            HistogramListType* outputHisto = this->GetHistogramListOutput();
            outputHisto->Clear();
            itkAssertInDebugAndIgnoreInReleaseMacro(m_Size.Size() == numberOfComponents);
            for (unsigned int k = 0; k < numberOfComponents; ++k)
            {
                typename HistogramType::MeasurementVectorType bandMin;
                typename HistogramType::MeasurementVectorType bandMax;
                bandMin.SetSize(1);
                bandMax.SetSize(1);
                bandMin.Fill(m_HistogramMin[k]);
                bandMax.Fill(m_HistogramMax[k]);

                typename HistogramType::Pointer histogram = HistogramType::New();
//                histogram->SetClipBinsAtEnds(clipBins);
                histogram->SetClipBinsAtEnds(true);

                typename HistogramType::SizeType size;
                size.SetSize(1);
                size.Fill(m_Size[k]);
                histogram->SetMeasurementVectorSize(1);
                histogram->Initialize(size, bandMin, bandMax);

                outputHisto->PushBack(histogram);
            }

            // Setup HistogramLists for each thread
            m_ThreadHistogramList.clear();
            for (unsigned int i = 0; i < numberOfThreads; ++i)
            {
                HistogramListPointerType histoList = HistogramListType::New();
                histoList->Clear();
                for (unsigned int k = 0; k < numberOfComponents; ++k)
                {
                    typename HistogramType::MeasurementVectorType bandMin;
                    typename HistogramType::MeasurementVectorType bandMax;
                    bandMin.SetSize(1);
                    bandMax.SetSize(1);
                    bandMin.Fill(m_HistogramMin[k]);
                    bandMax.Fill(m_HistogramMax[k]);

                    typename HistogramType::Pointer histogram = HistogramType::New();
                    histogram->SetClipBinsAtEnds(clipBins);

                    typename HistogramType::SizeType size;
                    size.SetSize(1);
                    size.Fill(m_Size[k]);
                    histogram->SetMeasurementVectorSize(1);
                    histogram->Initialize(size, bandMin, bandMax);

                    histoList->PushBack(histogram);
                }
                m_ThreadHistogramList.push_back(histoList);
            }

        }

    template<class TInputImage>
        void
        PersistentShadowVariationThresholdValueEstimator<TInputImage>::Synthetize()
        {
            HistogramListType* outputHisto = this->GetHistogramListOutput();

            const int numberOfThreads = this->GetNumberOfThreads();
            unsigned int numberOfComponents = this->GetInput()->GetNumberOfComponentsPerPixel();

            // copy histograms to output
            for (int i = 0; i < numberOfThreads; ++i)
            {
                itkAssertInDebugAndIgnoreInReleaseMacro(numberOfComponents == m_ThreadHistogramList[i]->Size());
                for (unsigned int j = 0; j < numberOfComponents; ++j)
                {
                    HistogramType* outHisto = outputHisto->GetNthElement(j);
                    HistogramType* threadHisto = m_ThreadHistogramList[i]->GetNthElement(j);

                    typename HistogramType::Iterator iterOutput = outHisto->Begin();
                    typename HistogramType::Iterator iterThread = threadHisto->Begin();

                    while (iterOutput != outHisto->End() && iterThread != threadHisto->End())
                    {
                        iterOutput.SetFrequency(iterOutput.GetFrequency() + iterThread.GetFrequency());

                        ++iterOutput;
                        ++iterThread;
                    }
                }
            }
        }

    template<class TInputImage>
        void
        PersistentShadowVariationThresholdValueEstimator<TInputImage>::ThreadedGenerateData(const RegionType& outputRegionForThread,
                itk::ThreadIdType threadId)
        {
            /**
             * Grab the input
             */
            InputImagePointer inputPtr = const_cast<TInputImage *>(this->GetInput());
            typename HistogramType::IndexType index;

            itk::ImageRegionConstIteratorWithIndex<TInputImage> it(inputPtr, outputRegionForThread);
            it.GoToBegin();

            // do the work
            while (!it.IsAtEnd())
            {
                PixelType vectorValue = it.Get();

                bool skipSampleNoData = false;

                unsigned int itComp = 0;
                while (itComp < vectorValue.GetSize())
                {
                    if (vnsInfOrEqualDoubleMacro(vectorValue[itComp], 0, CONST_EPSILON))
                    {
                        skipSampleNoData = true;
                        itComp++;
                    }
                    else
                    {
                        skipSampleNoData = false;
                        break;
                    }
                }

                if (!skipSampleNoData)
                {
                    itkAssertInDebugAndIgnoreInReleaseMacro(vectorValue.GetSize() == m_ThreadHistogramList[threadId]->Size());
                    for (unsigned int j = 0; j < vectorValue.GetSize(); ++j)
                    {
                        typename HistogramType::MeasurementVectorType value;
                        value.SetSize(1);
                        value.Fill(vectorValue[j]);

                        m_ThreadHistogramList[threadId]->GetNthElement(j)->GetIndex(value, index);
                        if (!m_ThreadHistogramList[threadId]->GetNthElement(j)->IsIndexOutOfBounds(index))
                        {
                            // if the measurement vector is out of bound then
                            // the GetIndex method has returned an index set to the max size of
                            // the invalid dimension - even if the hvector is less than the minimum
                            // bin value.
                            // If the index isn't valid, we don't increase the frequency.
                            // See the comments in Histogram->GetIndex() for more info.
                            m_ThreadHistogramList[threadId]->GetNthElement(j)->IncreaseFrequencyOfIndex(index, 1);
                        }
                        else
                        {
                            vnsLogDebugMacro(m_ThreadHistogramList[threadId]->GetNthElement(j)->IsIndexOutOfBounds(index));
                        }
                    }
                }

                ++it;
            }
        }

    template<class TImage>
        void
        PersistentShadowVariationThresholdValueEstimator<TImage>::PrintSelf(std::ostream& os, itk::Indent indent) const
        {
            Superclass::PrintSelf(os, indent);

            os << indent << "Histogram minimum: " << this->GetHistogramMin() << std::endl;
            os << indent << "Histogram maximum: " << this->GetHistogramMax() << std::endl;
            os << indent << "Number of bins: " << m_Size[0] << std::endl;
            os << indent << "NoData value: " << this->GetNoDataValue() << std::endl;
        }

} // end namespace otb
#endif
