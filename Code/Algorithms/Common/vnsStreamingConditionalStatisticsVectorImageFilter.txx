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
 * VERSION : 2.0.0 : FA : LAIG-FA-MAJA-153190-CS : 3 fevrier 2017 : Correction de "runtime error"  levés     *
 *                                              par MAJA compilé avec les options de sanaisation de clang++ *
 * VERSION : 1.0.0 : FA : LAIG-FA-MAC-148399-CS : 27 octobre 2016 : Audit code - Correction qualite         *
 * VERSION : 1.0.0 : FA : LAIG-FA-MAC-1988-CNES : 21 octobre 2016 : Correction qualite (code mort)          *
 * VERSION : 4-4-0 : FA : LAIG-FA-MAC-127944-CS : 3 juin 2015 : Correction code pour la qualite             *
 * VERSION : 4-0-0 : FA : LAIG-FA-MAC-120653-CS : 3 avril 2014 : Correction de règles de qualité			*
 * VERSION : 3-2-0 : DM : LAIG-DM-MAC-697-CNES : 8 novembre 2013 : Mise à jour du plugin SENTINEL2          *
 * 					Action interne 98664. Prise en compte  des modifications apportées à la classe   		*
 * 					otbStreamingStatisticsVectorImageFilter v3.13   										*
 * VERSION : 1-0-0 : <TypeFT> : <NumFT> : 1 avr. 2010 : Creation                                                           
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id$
 *                                                                                                          *
 ************************************************************************************************************/
#ifndef __vnsStreamingConditionalStatisticsVectorImageFilter_txx
#define __vnsStreamingConditionalStatisticsVectorImageFilter_txx

#include "vnsStreamingConditionalStatisticsVectorImageFilter.h"

#include "itkImageRegionIterator.h"
#include "itkImageRegionConstIteratorWithIndex.h"
#include "itkNumericTraits.h"

#include "vnsMacro.h"
#include "vnsMath.h"
#include "vnsLoggers.h"
#include "vnsUtilities.h"

namespace vns
{

    template<class TInputImage, class TPrecision>
        PersistentStreamingConditionalStatisticsVectorImageFilter<TInputImage, TPrecision>::PersistentStreamingConditionalStatisticsVectorImageFilter() :
                m_EnableMinMax(true), m_EnableFirstOrderStats(true), m_EnableSecondOrderStats(true), m_UseUnbiasedEstimator(false), m_IsValid(
                        false)
        {
            // first output is a copy of the image, DataObject created by
            // superclass

            // allocate the data objects for the outputs which are
            // just decorators around vector/matrix types
            for (unsigned int i = 1; i < 12; ++i)
            {
                this->itk::ProcessObject::SetNthOutput(i, this->MakeOutput(i).GetPointer());
            }
        }

    template<class TInputImage, class TPrecision>
        itk::DataObject::Pointer
        PersistentStreamingConditionalStatisticsVectorImageFilter<TInputImage, TPrecision>::MakeOutput(unsigned int output)
        {
            switch (output)
                {
            // Input image
            case 0:
                return static_cast<itk::DataObject*>(TInputImage::New().GetPointer());
                // 1 and 2 outputs : pixel objects
            case 1:
            case 2:
                // min/max
                return static_cast<itk::DataObject*>(PixelObjectType::New().GetPointer());
                // 3 and 4 outputs : Real pixel objects
            case 3:
            case 4:
                // mean / sum
                return static_cast<itk::DataObject*>(RealPixelObjectType::New().GetPointer());
                // 5 and 6 outputs : Matrix objects
            case 5:
            case 6:
                // covariance / correlation
                return static_cast<itk::DataObject*>(MatrixObjectType::New().GetPointer());
                // 7, 8 and 9 outputs : Real objects
            case 7:
            case 8:
            case 9:
                // component mean, component covariance, component correlation
                return static_cast<itk::DataObject*>(RealObjectType::New().GetPointer());
                // 10 and 11 output : UI objects
            case 10:
            case 11:
                return static_cast<itk::DataObject*>(UIObjectType::New().GetPointer());
            default:
                // might as well make an image
                return static_cast<itk::DataObject*>(TInputImage::New().GetPointer());
                }
        }

    template<class TInputImage, class TPrecision>
        typename PersistentStreamingConditionalStatisticsVectorImageFilter<TInputImage, TPrecision>::PixelObjectType*
        PersistentStreamingConditionalStatisticsVectorImageFilter<TInputImage, TPrecision>::GetMinimumOutput()
        {
            return static_cast<PixelObjectType*>(this->itk::ProcessObject::GetOutput(1));
        }

    template<class TInputImage, class TPrecision>
        const typename PersistentStreamingConditionalStatisticsVectorImageFilter<TInputImage, TPrecision>::PixelObjectType*
        PersistentStreamingConditionalStatisticsVectorImageFilter<TInputImage, TPrecision>::GetMinimumOutput() const
        {
            return static_cast<const PixelObjectType*>(this->itk::ProcessObject::GetOutput(1));
        }

    template<class TInputImage, class TPrecision>
        typename PersistentStreamingConditionalStatisticsVectorImageFilter<TInputImage, TPrecision>::PixelObjectType*
        PersistentStreamingConditionalStatisticsVectorImageFilter<TInputImage, TPrecision>::GetMaximumOutput()
        {
            return static_cast<PixelObjectType*>(this->itk::ProcessObject::GetOutput(2));
        }

    template<class TInputImage, class TPrecision>
        const typename PersistentStreamingConditionalStatisticsVectorImageFilter<TInputImage, TPrecision>::PixelObjectType*
        PersistentStreamingConditionalStatisticsVectorImageFilter<TInputImage, TPrecision>::GetMaximumOutput() const
        {
            return static_cast<const PixelObjectType*>(this->itk::ProcessObject::GetOutput(2));
        }

    template<class TInputImage, class TPrecision>
        typename PersistentStreamingConditionalStatisticsVectorImageFilter<TInputImage, TPrecision>::RealPixelObjectType*
        PersistentStreamingConditionalStatisticsVectorImageFilter<TInputImage, TPrecision>::GetMeanOutput()
        {
            return static_cast<RealPixelObjectType*>(this->itk::ProcessObject::GetOutput(3));
        }

    template<class TInputImage, class TPrecision>
        const typename PersistentStreamingConditionalStatisticsVectorImageFilter<TInputImage, TPrecision>::RealPixelObjectType*
        PersistentStreamingConditionalStatisticsVectorImageFilter<TInputImage, TPrecision>::GetMeanOutput() const
        {
            return static_cast<const RealPixelObjectType*>(this->itk::ProcessObject::GetOutput(3));
        }

    template<class TInputImage, class TPrecision>
        typename PersistentStreamingConditionalStatisticsVectorImageFilter<TInputImage, TPrecision>::RealPixelObjectType*
        PersistentStreamingConditionalStatisticsVectorImageFilter<TInputImage, TPrecision>::GetSumOutput()
        {
            return static_cast<RealPixelObjectType*>(this->itk::ProcessObject::GetOutput(4));
        }

    template<class TInputImage, class TPrecision>
        const typename PersistentStreamingConditionalStatisticsVectorImageFilter<TInputImage, TPrecision>::RealPixelObjectType*
        PersistentStreamingConditionalStatisticsVectorImageFilter<TInputImage, TPrecision>::GetSumOutput() const
        {
            return static_cast<const RealPixelObjectType*>(this->itk::ProcessObject::GetOutput(4));
        }

    template<class TInputImage, class TPrecision>
        typename PersistentStreamingConditionalStatisticsVectorImageFilter<TInputImage, TPrecision>::MatrixObjectType*
        PersistentStreamingConditionalStatisticsVectorImageFilter<TInputImage, TPrecision>::GetCorrelationOutput()
        {
            return static_cast<MatrixObjectType*>(this->itk::ProcessObject::GetOutput(5));
        }

    template<class TInputImage, class TPrecision>
        const typename PersistentStreamingConditionalStatisticsVectorImageFilter<TInputImage, TPrecision>::MatrixObjectType*
        PersistentStreamingConditionalStatisticsVectorImageFilter<TInputImage, TPrecision>::GetCorrelationOutput() const
        {
            return static_cast<const MatrixObjectType*>(this->itk::ProcessObject::GetOutput(5));
        }

    template<class TInputImage, class TPrecision>
        typename PersistentStreamingConditionalStatisticsVectorImageFilter<TInputImage, TPrecision>::MatrixObjectType*
        PersistentStreamingConditionalStatisticsVectorImageFilter<TInputImage, TPrecision>::GetCovarianceOutput()
        {
            return static_cast<MatrixObjectType*>(this->itk::ProcessObject::GetOutput(6));
        }

    template<class TInputImage, class TPrecision>
        const typename PersistentStreamingConditionalStatisticsVectorImageFilter<TInputImage, TPrecision>::MatrixObjectType*
        PersistentStreamingConditionalStatisticsVectorImageFilter<TInputImage, TPrecision>::GetCovarianceOutput() const
        {
            return static_cast<const MatrixObjectType*>(this->itk::ProcessObject::GetOutput(6));
        }

    template<class TInputImage, class TPrecision>
        typename PersistentStreamingConditionalStatisticsVectorImageFilter<TInputImage, TPrecision>::RealObjectType*
        PersistentStreamingConditionalStatisticsVectorImageFilter<TInputImage, TPrecision>::GetComponentMeanOutput()
        {
            return static_cast<RealObjectType*>(this->itk::ProcessObject::GetOutput(7));
        }

    template<class TInputImage, class TPrecision>
        const typename PersistentStreamingConditionalStatisticsVectorImageFilter<TInputImage, TPrecision>::RealObjectType*
        PersistentStreamingConditionalStatisticsVectorImageFilter<TInputImage, TPrecision>::GetComponentMeanOutput() const
        {
            return static_cast<const RealObjectType*>(this->itk::ProcessObject::GetOutput(7));
        }

    template<class TInputImage, class TPrecision>
        typename PersistentStreamingConditionalStatisticsVectorImageFilter<TInputImage, TPrecision>::RealObjectType*
        PersistentStreamingConditionalStatisticsVectorImageFilter<TInputImage, TPrecision>::GetComponentCorrelationOutput()
        {
            return static_cast<RealObjectType*>(this->itk::ProcessObject::GetOutput(8));
        }

    template<class TInputImage, class TPrecision>
        const typename PersistentStreamingConditionalStatisticsVectorImageFilter<TInputImage, TPrecision>::RealObjectType*
        PersistentStreamingConditionalStatisticsVectorImageFilter<TInputImage, TPrecision>::GetComponentCorrelationOutput() const
        {
            return static_cast<const RealObjectType*>(this->itk::ProcessObject::GetOutput(8));
        }

    template<class TInputImage, class TPrecision>
        typename PersistentStreamingConditionalStatisticsVectorImageFilter<TInputImage, TPrecision>::RealObjectType*
        PersistentStreamingConditionalStatisticsVectorImageFilter<TInputImage, TPrecision>::GetComponentCovarianceOutput()
        {
            return static_cast<RealObjectType*>(this->itk::ProcessObject::GetOutput(9));
        }

    template<class TInputImage, class TPrecision>
        const typename PersistentStreamingConditionalStatisticsVectorImageFilter<TInputImage, TPrecision>::RealObjectType*
        PersistentStreamingConditionalStatisticsVectorImageFilter<TInputImage, TPrecision>::GetComponentCovarianceOutput() const
        {
            return static_cast<const RealObjectType*>(this->itk::ProcessObject::GetOutput(9));
        }

    template<class TInputImage, class TPrecision>
        typename PersistentStreamingConditionalStatisticsVectorImageFilter<TInputImage, TPrecision>::UIObjectType*
        PersistentStreamingConditionalStatisticsVectorImageFilter<TInputImage, TPrecision>::GetNbOfValidValuesOutput()
        {
            return static_cast<UIObjectType*>(this->itk::ProcessObject::GetOutput(10));
        }

    template<class TInputImage, class TPrecision>
        const typename PersistentStreamingConditionalStatisticsVectorImageFilter<TInputImage, TPrecision>::UIObjectType*
        PersistentStreamingConditionalStatisticsVectorImageFilter<TInputImage, TPrecision>::GetNbOfValidValuesOutput() const
        {
            return static_cast<const UIObjectType*>(this->itk::ProcessObject::GetOutput(10));
        }

    template<class TInputImage, class TPrecision>
        typename PersistentStreamingConditionalStatisticsVectorImageFilter<TInputImage, TPrecision>::UIObjectType*
        PersistentStreamingConditionalStatisticsVectorImageFilter<TInputImage, TPrecision>::GetNbOfExcludedValuesOutput()
        {
            return static_cast<UIObjectType*>(this->itk::ProcessObject::GetOutput(11));
        }

    template<class TInputImage, class TPrecision>
        const typename PersistentStreamingConditionalStatisticsVectorImageFilter<TInputImage, TPrecision>::UIObjectType*
        PersistentStreamingConditionalStatisticsVectorImageFilter<TInputImage, TPrecision>::GetNbOfExcludedValuesOutput() const
        {
            return static_cast<const UIObjectType*>(this->itk::ProcessObject::GetOutput(11));
        }

    template<class TInputImage, class TPrecision>
        void
        PersistentStreamingConditionalStatisticsVectorImageFilter<TInputImage, TPrecision>::GenerateOutputInformation()
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

    template<class TInputImage, class TPrecision>
        void
        PersistentStreamingConditionalStatisticsVectorImageFilter<TInputImage, TPrecision>::AllocateOutputs()
        {
            // This is commented to prevent the streaming of the whole image for the first stream strip
            // It shall not cause any problem because the output image of this filter is not intended to be used.
            //InputImagePointer image = const_cast< TInputImage * >( this->GetInput() );
            //this->GraftOutput( image );
            // Nothing that needs to be allocated for the remaining outputs
        }

    template<class TInputImage, class TPrecision>
        void
        PersistentStreamingConditionalStatisticsVectorImageFilter<TInputImage, TPrecision>::Reset()
        {
            // Grab the input
            TInputImage * inputPtr = const_cast<TInputImage *>(this->GetInput());
            inputPtr->UpdateOutputInformation();

            // Get the number of threads
            const unsigned int numberOfThreads = this->GetNumberOfThreads();
            // Get the number of component
            const unsigned int numberOfComponent = inputPtr->GetNumberOfComponentsPerPixel();

            // Class parameters
            m_NumberOfValidPixels.SetSize(numberOfThreads);
            m_NumberOfValidPixels.Fill(itk::NumericTraits<long>::Zero);

            this->GetNbOfValidValuesOutput()->Set(itk::NumericTraits<long>::Zero);
            this->GetNbOfExcludedValuesOutput()->Set(itk::NumericTraits<long>::Zero);

            // If the Min and Max values are computed
            if (m_EnableMinMax)
            {
                PixelType tempPixel;
                tempPixel.SetSize(numberOfComponent);

                // Initialization of the min and max values
                tempPixel.Fill(itk::NumericTraits < InternalPixelType > ::max());
                this->GetMinimumOutput()->Set(tempPixel);

                tempPixel.Fill(itk::NumericTraits < InternalPixelType > ::NonpositiveMin());
                this->GetMaximumOutput()->Set(tempPixel);

                // Initialization of the values per thread
                PixelType tempTemporiesPixel;
                tempTemporiesPixel.SetSize(numberOfComponent);
                tempTemporiesPixel.Fill(itk::NumericTraits < InternalPixelType > ::max());
                m_ThreadMin = std::vector < PixelType > (numberOfThreads, tempTemporiesPixel);

                tempTemporiesPixel.Fill(itk::NumericTraits < InternalPixelType > ::NonpositiveMin());
                m_ThreadMax = std::vector < PixelType > (numberOfThreads, tempTemporiesPixel);
            }

            if (m_EnableSecondOrderStats)
            {
                m_EnableFirstOrderStats = true;
            }

            // Initialization of the first order statistics
            if (m_EnableFirstOrderStats)
            {
                RealPixelType zeroRealPixel;
                zeroRealPixel.SetSize(numberOfComponent);
                zeroRealPixel.Fill(itk::NumericTraits < PrecisionType > ::ZeroValue());
                this->GetMeanOutput()->Set(zeroRealPixel);
                this->GetSumOutput()->Set(zeroRealPixel);
                m_ThreadFirstOrderAccumulators.resize(numberOfThreads);
                std::fill(m_ThreadFirstOrderAccumulators.begin(), m_ThreadFirstOrderAccumulators.end(), zeroRealPixel);

                RealType zeroReal = itk::NumericTraits < RealType > ::ZeroValue();
                m_ThreadFirstOrderComponentAccumulators.resize(numberOfThreads);
                std::fill(m_ThreadFirstOrderComponentAccumulators.begin(), m_ThreadFirstOrderComponentAccumulators.end(), zeroReal);

            }

            // Initialization of the first order statistics
            if (m_EnableSecondOrderStats)
            {
                MatrixType zeroMatrix;
                zeroMatrix.SetSize(numberOfComponent, numberOfComponent);
                zeroMatrix.Fill(itk::NumericTraits < PrecisionType > ::Zero);
                this->GetCovarianceOutput()->Set(zeroMatrix);
                this->GetCorrelationOutput()->Set(zeroMatrix);

                m_ThreadSecondOrderAccumulators.resize(numberOfThreads);
                std::fill(m_ThreadSecondOrderAccumulators.begin(), m_ThreadSecondOrderAccumulators.end(), zeroMatrix);

                RealType zeroReal = itk::NumericTraits < RealType > ::ZeroValue();
                m_ThreadSecondOrderComponentAccumulators.resize(numberOfThreads);
                std::fill(m_ThreadSecondOrderComponentAccumulators.begin(), m_ThreadSecondOrderComponentAccumulators.end(), zeroReal);
            }

            // Boolean used to indicate if the number of pixels used to compute the statistics is not null
            m_IsValid = false;

        }

    template<class TInputImage, class TPrecision>
        void
        PersistentStreamingConditionalStatisticsVectorImageFilter<TInputImage, TPrecision>::Synthetize()
        {
            TInputImage * inputPtr = const_cast<TInputImage *>(this->GetInput());
            const unsigned int nbPixels = inputPtr->GetLargestPossibleRegion().GetNumberOfPixels();
            const unsigned int numberOfComponent = inputPtr->GetNumberOfComponentsPerPixel();

            // Initializations
            PixelType minimum;
            minimum.SetSize(numberOfComponent);
            minimum.Fill(itk::NumericTraits < InternalPixelType > ::max());
            PixelType maximum;
            maximum.SetSize(numberOfComponent);
            maximum.Fill(itk::NumericTraits < InternalPixelType > ::NonpositiveMin());

            RealPixelType streamFirstOrderAccumulator(numberOfComponent);
            streamFirstOrderAccumulator.Fill(itk::NumericTraits < PrecisionType > ::Zero);
            MatrixType streamSecondOrderAccumulator(numberOfComponent, numberOfComponent);
            streamSecondOrderAccumulator.Fill(itk::NumericTraits < PrecisionType > ::Zero);

            RealType streamFirstOrderComponentAccumulator = itk::NumericTraits < RealType > ::Zero;
            RealType streamSecondOrderComponentAccumulator = itk::NumericTraits < RealType > ::Zero;

            long nbValidPixels(0);

            // Accumulate results from all threads
            const unsigned int numberOfThreads = this->GetNumberOfThreads();
            for (unsigned int threadId = 0; threadId < numberOfThreads; ++threadId)
            {
                if (m_EnableMinMax)
                {
                    const PixelType& threadMin = m_ThreadMin[threadId];
                    const PixelType& threadMax = m_ThreadMax[threadId];

                    // Search the min and max value of all the threads and for each spectral band
                    for (unsigned int j = 0; j < numberOfComponent; ++j)
                    {
                        if (threadMin[j] < minimum[j])
                        {
                            minimum[j] = threadMin[j];
                        }
                        if (threadMax[j] > maximum[j])
                        {
                            maximum[j] = threadMax[j];
                        }
                    }
                    nbValidPixels += m_NumberOfValidPixels[threadId];
                }

                if (m_EnableFirstOrderStats)
                {
                    streamFirstOrderAccumulator += m_ThreadFirstOrderAccumulators[threadId];
                    streamFirstOrderComponentAccumulator += m_ThreadFirstOrderComponentAccumulators[threadId];
                }

                if (m_EnableSecondOrderStats)
                {
                    streamSecondOrderAccumulator += m_ThreadSecondOrderAccumulators[threadId];
                    streamSecondOrderComponentAccumulator += m_ThreadSecondOrderComponentAccumulators[threadId];
                }
            }

            // Set null statistics if the number of valid pixels (not equal to the input excluded value) is null
            if (nbValidPixels == 0)
            {
                m_IsValid = false;

                vnsLogWarningMacro("PersistentStreamingConditionalStatisticsVectorImageFilter::Synthetize."
                        "No pixel is valid. Return null statistics");

                // Set default values
                minimum.Fill(itk::NumericTraits < InternalPixelType > ::Zero);
                maximum.Fill(itk::NumericTraits < InternalPixelType > ::Zero);
                this->GetMinimumOutput()->Set(minimum);
                this->GetMaximumOutput()->Set(maximum);
                this->GetSumOutput()->Set(static_cast<RealPixelType>(streamFirstOrderAccumulator));
                this->GetMeanOutput()->Set(static_cast<RealPixelType>(streamFirstOrderAccumulator));

                MatrixType cor;
                cor.SetSize(numberOfComponent, numberOfComponent);
                cor.Fill(itk::NumericTraits < RealType > ::Zero);

                this->GetCovarianceOutput()->Set(cor);
            }
            // Set the outputs
            else
            {
                m_IsValid = true;

                // Final calculations
                if (m_EnableMinMax)
                {
                    this->GetMinimumOutput()->Set(minimum);
                    this->GetMaximumOutput()->Set(maximum);
                }

                if (m_EnableFirstOrderStats)
                {
                    // Compute the sum, the mean and the mean for all the components tacking into account the valid pixels (and not the excluded one)
                    this->GetComponentMeanOutput()->Set(streamFirstOrderComponentAccumulator / (nbValidPixels * numberOfComponent)); //(nbPixels * numberOfComponent));

                    this->GetMeanOutput()->Set(streamFirstOrderAccumulator / static_cast<RealType>(nbValidPixels)); //nbPixels);
                    this->GetSumOutput()->Set(streamFirstOrderAccumulator);
                }

                if (m_EnableSecondOrderStats)
                {

                    MatrixType cor = streamSecondOrderAccumulator / nbValidPixels; //nbPixels;
                    this->GetCorrelationOutput()->Set(cor);

                    const RealPixelType& mean = this->GetMeanOutput()->Get();
                    double regul = static_cast<double>(nbValidPixels) / (nbValidPixels - 1); //(nbPixels) / (nbPixels - 1);

                    if (!m_UseUnbiasedEstimator)
                    {
                        regul = 1;
                    }

                    // Compute the covariance
                    MatrixType cov = cor;
                    for (unsigned int r = 0; r < numberOfComponent; ++r)
                    {
                        for (unsigned int c = 0; c < numberOfComponent; ++c)
                        {
                            cov(r, c) = regul * (cov(r, c) - mean[r] * mean[c]);
                        }
                    }

                    this->GetCovarianceOutput()->Set(cov);

                    this->GetComponentMeanOutput()->Set(streamFirstOrderComponentAccumulator / (nbValidPixels * numberOfComponent)); //(nbPixels * numberOfComponent));
                    this->GetComponentCorrelationOutput()->Set(streamSecondOrderComponentAccumulator / (nbValidPixels * numberOfComponent)); //(nbPixels * numberOfComponent));
                    this->GetComponentCovarianceOutput()->Set((nbValidPixels * numberOfComponent) / (nbValidPixels * numberOfComponent - 1)
                    //(nbPixels * numberOfComponent) / (nbPixels * numberOfComponent - 1)
                            * (this->GetComponentCorrelation() - (this->GetComponentMean() * this->GetComponentMean())));
                }

                // Set the number of used pixels to compute statistics and the number of excluded values
                this->GetNbOfValidValuesOutput()->Set(nbValidPixels);
                this->GetNbOfExcludedValuesOutput()->Set(nbPixels - nbValidPixels);

            } // if some pixels valid

        }

    template<class TInputImage, class TPrecision>
        void
        PersistentStreamingConditionalStatisticsVectorImageFilter<TInputImage, TPrecision>::ThreadedGenerateData(
                const RegionType& outputRegionForThread, itk::ThreadIdType threadId)
        {
            // Support progress methods/callbacks

            // Grab the input
            InputImagePointer inputPtr = const_cast<TInputImage *>(this->GetInput());
            itk::ImageRegionConstIteratorWithIndex < TInputImage > it(inputPtr, outputRegionForThread);

            for (it.GoToBegin(); !it.IsAtEnd(); ++it) //MACCS 4.3 - disable progress : progress.CompletedPixel())
            {
                const PixelType& vectorValue = it.Get();
                bool isExcluded = false;

                // If a pixel is set to the excluded value in one band, it is excluded
                // for all bands
                unsigned int j = 0;
                const unsigned int l_Size = vectorValue.GetSize();

                // Band loop
                while ((j < l_Size) && (isExcluded == false))
                {
                    InternalPixelType value = vectorValue[j];
                    // Check with excluded value
                    if (vnsEqualsDoubleMacro(static_cast<double>(value), static_cast<double>(m_ExcludedValue)) == true)
                    {
                        isExcluded = true;
                    }
                    j++;
                }

                // Statistics not computed for the excluded value
                if (isExcluded == false)
                {
                    // Compute min and max for the current thread
                    if (m_EnableMinMax)
                    {
                        for (unsigned int j = 0; j < vectorValue.GetSize(); ++j)
                        {
                            if (vectorValue[j] < m_ThreadMin[threadId][j])
                            {
                                m_ThreadMin[threadId][j] = vectorValue[j];
                            }
                            if (vectorValue[j] >  m_ThreadMax[threadId][j])
                            {
                                m_ThreadMax[threadId][j] = vectorValue[j];
                            }
                        }
                    }

                    // Compute the sum used to calculate the mean values for this thread
                    if (m_EnableFirstOrderStats)
                    {
                        m_ThreadFirstOrderAccumulators[threadId] += vectorValue;

                        for (unsigned int i = 0; i < vectorValue.GetSize(); ++i)
                        {
                            m_ThreadFirstOrderComponentAccumulators[threadId] += vectorValue[i];
                        }
                    }

                    // Compute the squared sum used to calculate the covariance matrix
                    if (m_EnableSecondOrderStats)
                    {
                        MatrixType& threadSecondOrder = m_ThreadSecondOrderAccumulators[threadId];
                        for (unsigned int r = 0; r < threadSecondOrder.Rows(); ++r)
                        {
                            for (unsigned int c = 0; c < threadSecondOrder.Cols(); ++c)
                            {
                                threadSecondOrder(r, c) += vectorValue[r] * vectorValue[c];
                            }
                        }
                        m_ThreadSecondOrderComponentAccumulators[threadId] += vectorValue.GetSquaredNorm();
                    }

                    // Count the number of valid pixels per thread
                    m_NumberOfValidPixels[threadId]++;

                } // end if excluded value
            }
        }

    template<class TImage, class TPrecision>
        void
        PersistentStreamingConditionalStatisticsVectorImageFilter<TImage, TPrecision>::PrintSelf(std::ostream& os, itk::Indent indent) const
        {
            Superclass::PrintSelf(os, indent);

            os << indent << "Min: " << this->GetMinimumOutput()->Get() << std::endl;
            os << indent << "Max: " << this->GetMaximumOutput()->Get() << std::endl;
            os << indent << "Mean: " << this->GetMeanOutput()->Get() << std::endl;
            os << indent << "Covariance: " << this->GetCovarianceOutput()->Get() << std::endl;
            os << indent << "Correlation: " << this->GetCorrelationOutput()->Get() << std::endl;
            os << indent << "Component Mean: " << this->GetComponentMeanOutput()->Get() << std::endl;
            os << indent << "Component Covariance: " << this->GetComponentCovarianceOutput()->Get() << std::endl;
            os << indent << "Component Correlation: " << this->GetComponentCorrelationOutput()->Get() << std::endl;
            os << indent << "UseUnbiasedEstimator: " << Utilities::BoolToLowerString(this->m_UseUnbiasedEstimator) << std::endl;
            os << indent << "ValidVal: " << this->GetNbOfValidValuesOutput()->Get() << std::endl;
            os << indent << "ExcludedVal: " << this->GetNbOfExcludedValuesOutput()->Get() << std::endl;
        }

} // end namespace otb
#endif
