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
 *
 * VERSION : 1.0.0 : FA : LAIG-FA-MAC-148399-CS : 19 octobre 2016 : Audit code - Correction qualite         *
 * VERSION : 5-1-0 : DM : LAIG-DM-MAC-1769-CNES : 6 juillet 2016 : Implémentation écart type checktools     *
 * VERSION : 4-4-0 : FA : LAIG-FA-MAC-127944-CS : 3 juin 2015 : Correction code pour la qualite             *
 * VERSION : 4-0-0 : FA : LAIG-FA-MAC-120653-CS : 3 avril 2014 : Correction de règles de qualité			*
 * VERSION : 3-2-0 : DM : LAIG-DM-MAC-697-CNES : 8 novembre 2013 : Mise à jour du plugin SENTINEL2          *
 * 					Action interne 98664. Prise en compte  des modifications apportées à la classe   		*
 * 					otbStreamingStatisticsVectorImageFilter v3.13   										*
 * VERSION : 1-0-0-3 : FT : 65466 : 26 sept. 2011 : Test if the number of valid pixel is not null             *
 * VERSION : 1-0-0 : <TypeFT> : <NumFT> : 19 avr. 2010 : Creation                                                           
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id$
 *                                                                                                          *
 ************************************************************************************************************/
#ifndef __vnsStreamingMaskConditionalStatisticsVectorImageFilter_txx
#define __vnsStreamingMaskConditionalStatisticsVectorImageFilter_txx

#include "vnsStreamingMaskConditionalStatisticsVectorImageFilter.h"

#include "itkImageRegionIterator.h"
#include "itkImageRegionConstIteratorWithIndex.h"
#include "itkNumericTraits.h"

#include "vnsMacro.h"
#include "vnsMath.h"
#include "vnsLoggers.h"
#include "vnsUtilities.h"

namespace vns
{

    template<class TInputImage, class TInputMask, class TPrecision>
        PersistentStreamingMaskConditionalStatisticsVectorImageFilter<TInputImage, TInputMask, TPrecision>::PersistentStreamingMaskConditionalStatisticsVectorImageFilter() :
                m_EnableMinMax(true), m_EnableExcludeValue(false), m_EnableFirstOrderStats(true), m_EnableStandardDeviation(false), m_EnableSecondOrderStats(
                        true), m_UseUnbiasedEstimator(false), m_ExcludeValue(static_cast<RealType>(0)), m_ForegroundValue(
                        static_cast<MaskPixelType>(0)), m_IsValid(false)
        {
            // first output is a copy of the image, DataObject created by
            // superclass

            // allocate the data objects for the outputs which are
            // just decorators around vector/matrix types
            for (unsigned int i = 1; i < 16; ++i)
            {
                this->itk::ProcessObject::SetNthOutput(i, this->MakeOutput(i).GetPointer());
            }
        }

    template<class TInputImage, class TInputMask, class TPrecision>
        itk::DataObject::Pointer
        PersistentStreamingMaskConditionalStatisticsVectorImageFilter<TInputImage, TInputMask, TPrecision>::MakeOutput(unsigned int output)
        {
            switch (output)
                {
            case 0:
                return static_cast<itk::DataObject*>(TInputImage::New().GetPointer());
                // 1 and 2 outputs : pixel objects
            case 1:
            case 2:
                // min/max
                return static_cast<itk::DataObject*>(PixelObjectType::New().GetPointer());
                // 3 , 4 and 5 outputs : Real pixel objects
            case 3:
            case 4:
            case 5:
            case 14:
            case 15:
                // mean / sum / standard deviation / nbValidPerBand / nbExcludedPerBand
                return static_cast<itk::DataObject*>(RealPixelObjectType::New().GetPointer());
                // 6 and 7 outputs : Matrix objects
            case 6:
            case 7:
                // covariance / correlation
                return static_cast<itk::DataObject*>(MatrixObjectType::New().GetPointer());
                // 8, 9, 10 and 11 outputs : Real objects
            case 8:
            case 9:
            case 10:
            case 11:
                // component mean, standard deviation, component covariance, component correlation
                return static_cast<itk::DataObject*>(RealObjectType::New().GetPointer());
                // 12 and 13 output : UI objects
            case 12:
            case 13:
                return static_cast<itk::DataObject*>(UIObjectType::New().GetPointer());
            default:
                // might as well make an image
                return static_cast<itk::DataObject*>(TInputImage::New().GetPointer());
                }
        }

    template<class TInputImage, class TInputMask, class TPrecision>
        typename PersistentStreamingMaskConditionalStatisticsVectorImageFilter<TInputImage, TInputMask, TPrecision>::PixelObjectType*
        PersistentStreamingMaskConditionalStatisticsVectorImageFilter<TInputImage, TInputMask, TPrecision>::GetMinimumOutput()
        {
            return static_cast<PixelObjectType*>(this->itk::ProcessObject::GetOutput(1));
        }

    template<class TInputImage, class TInputMask, class TPrecision>
        const typename PersistentStreamingMaskConditionalStatisticsVectorImageFilter<TInputImage, TInputMask, TPrecision>::PixelObjectType*
        PersistentStreamingMaskConditionalStatisticsVectorImageFilter<TInputImage, TInputMask, TPrecision>::GetMinimumOutput() const
        {
            return static_cast<const PixelObjectType*>(this->itk::ProcessObject::GetOutput(1));
        }

    template<class TInputImage, class TInputMask, class TPrecision>
        typename PersistentStreamingMaskConditionalStatisticsVectorImageFilter<TInputImage, TInputMask, TPrecision>::PixelObjectType*
        PersistentStreamingMaskConditionalStatisticsVectorImageFilter<TInputImage, TInputMask, TPrecision>::GetMaximumOutput()
        {
            return static_cast<PixelObjectType*>(this->itk::ProcessObject::GetOutput(2));
        }

    template<class TInputImage, class TInputMask, class TPrecision>
        const typename PersistentStreamingMaskConditionalStatisticsVectorImageFilter<TInputImage, TInputMask, TPrecision>::PixelObjectType*
        PersistentStreamingMaskConditionalStatisticsVectorImageFilter<TInputImage, TInputMask, TPrecision>::GetMaximumOutput() const
        {
            return static_cast<const PixelObjectType*>(this->itk::ProcessObject::GetOutput(2));
        }

    template<class TInputImage, class TInputMask, class TPrecision>
        typename PersistentStreamingMaskConditionalStatisticsVectorImageFilter<TInputImage, TInputMask, TPrecision>::RealPixelObjectType*
        PersistentStreamingMaskConditionalStatisticsVectorImageFilter<TInputImage, TInputMask, TPrecision>::GetMeanOutput()
        {
            return static_cast<RealPixelObjectType*>(this->itk::ProcessObject::GetOutput(3));
        }

    template<class TInputImage, class TInputMask, class TPrecision>
        const typename PersistentStreamingMaskConditionalStatisticsVectorImageFilter<TInputImage, TInputMask, TPrecision>::RealPixelObjectType*
        PersistentStreamingMaskConditionalStatisticsVectorImageFilter<TInputImage, TInputMask, TPrecision>::GetMeanOutput() const
        {
            return static_cast<const RealPixelObjectType*>(this->itk::ProcessObject::GetOutput(3));
        }

    template<class TInputImage, class TInputMask, class TPrecision>
        typename PersistentStreamingMaskConditionalStatisticsVectorImageFilter<TInputImage, TInputMask, TPrecision>::RealPixelObjectType*
        PersistentStreamingMaskConditionalStatisticsVectorImageFilter<TInputImage, TInputMask, TPrecision>::GetSumOutput()
        {
            return static_cast<RealPixelObjectType*>(this->itk::ProcessObject::GetOutput(4));
        }

    template<class TInputImage, class TInputMask, class TPrecision>
        const typename PersistentStreamingMaskConditionalStatisticsVectorImageFilter<TInputImage, TInputMask, TPrecision>::RealPixelObjectType*
        PersistentStreamingMaskConditionalStatisticsVectorImageFilter<TInputImage, TInputMask, TPrecision>::GetSumOutput() const
        {
            return static_cast<const RealPixelObjectType*>(this->itk::ProcessObject::GetOutput(4));
        }

    template<class TInputImage, class TInputMask, class TPrecision>
        typename PersistentStreamingMaskConditionalStatisticsVectorImageFilter<TInputImage, TInputMask, TPrecision>::RealPixelObjectType*
        PersistentStreamingMaskConditionalStatisticsVectorImageFilter<TInputImage, TInputMask, TPrecision>::GetStandardDeviationOutput()
        {
            return static_cast<RealPixelObjectType*>(this->itk::ProcessObject::GetOutput(5));
        }

    template<class TInputImage, class TInputMask, class TPrecision>
        const typename PersistentStreamingMaskConditionalStatisticsVectorImageFilter<TInputImage, TInputMask, TPrecision>::RealPixelObjectType*
        PersistentStreamingMaskConditionalStatisticsVectorImageFilter<TInputImage, TInputMask, TPrecision>::GetStandardDeviationOutput() const
        {
            return static_cast<const RealPixelObjectType*>(this->itk::ProcessObject::GetOutput(5));
        }

    template<class TInputImage, class TInputMask, class TPrecision>
        typename PersistentStreamingMaskConditionalStatisticsVectorImageFilter<TInputImage, TInputMask, TPrecision>::MatrixObjectType*
        PersistentStreamingMaskConditionalStatisticsVectorImageFilter<TInputImage, TInputMask, TPrecision>::GetCorrelationOutput()
        {
            return static_cast<MatrixObjectType*>(this->itk::ProcessObject::GetOutput(6));
        }

    template<class TInputImage, class TInputMask, class TPrecision>
        const typename PersistentStreamingMaskConditionalStatisticsVectorImageFilter<TInputImage, TInputMask, TPrecision>::MatrixObjectType*
        PersistentStreamingMaskConditionalStatisticsVectorImageFilter<TInputImage, TInputMask, TPrecision>::GetCorrelationOutput() const
        {
            return static_cast<const MatrixObjectType*>(this->itk::ProcessObject::GetOutput(6));
        }

    template<class TInputImage, class TInputMask, class TPrecision>
        typename PersistentStreamingMaskConditionalStatisticsVectorImageFilter<TInputImage, TInputMask, TPrecision>::MatrixObjectType*
        PersistentStreamingMaskConditionalStatisticsVectorImageFilter<TInputImage, TInputMask, TPrecision>::GetCovarianceOutput()
        {
            return static_cast<MatrixObjectType*>(this->itk::ProcessObject::GetOutput(7));
        }

    template<class TInputImage, class TInputMask, class TPrecision>
        const typename PersistentStreamingMaskConditionalStatisticsVectorImageFilter<TInputImage, TInputMask, TPrecision>::MatrixObjectType*
        PersistentStreamingMaskConditionalStatisticsVectorImageFilter<TInputImage, TInputMask, TPrecision>::GetCovarianceOutput() const
        {
            return static_cast<const MatrixObjectType*>(this->itk::ProcessObject::GetOutput(7));
        }

    template<class TInputImage, class TInputMask, class TPrecision>
        typename PersistentStreamingMaskConditionalStatisticsVectorImageFilter<TInputImage, TInputMask, TPrecision>::RealObjectType*
        PersistentStreamingMaskConditionalStatisticsVectorImageFilter<TInputImage, TInputMask, TPrecision>::GetComponentMeanOutput()
        {
            return static_cast<RealObjectType*>(this->itk::ProcessObject::GetOutput(8));
        }

    template<class TInputImage, class TInputMask, class TPrecision>
        const typename PersistentStreamingMaskConditionalStatisticsVectorImageFilter<TInputImage, TInputMask, TPrecision>::RealObjectType*
        PersistentStreamingMaskConditionalStatisticsVectorImageFilter<TInputImage, TInputMask, TPrecision>::GetComponentMeanOutput() const
        {
            return static_cast<const RealObjectType*>(this->itk::ProcessObject::GetOutput(8));
        }

    template<class TInputImage, class TInputMask, class TPrecision>
        typename PersistentStreamingMaskConditionalStatisticsVectorImageFilter<TInputImage, TInputMask, TPrecision>::RealObjectType*
        PersistentStreamingMaskConditionalStatisticsVectorImageFilter<TInputImage, TInputMask, TPrecision>::GetComponentStandardDeviationOutput()
        {
            return static_cast<RealObjectType*>(this->itk::ProcessObject::GetOutput(9));
        }

    template<class TInputImage, class TInputMask, class TPrecision>
        const typename PersistentStreamingMaskConditionalStatisticsVectorImageFilter<TInputImage, TInputMask, TPrecision>::RealObjectType*
        PersistentStreamingMaskConditionalStatisticsVectorImageFilter<TInputImage, TInputMask, TPrecision>::GetComponentStandardDeviationOutput() const
        {
            return static_cast<const RealObjectType*>(this->itk::ProcessObject::GetOutput(9));
        }

    template<class TInputImage, class TInputMask, class TPrecision>
        typename PersistentStreamingMaskConditionalStatisticsVectorImageFilter<TInputImage, TInputMask, TPrecision>::RealObjectType*
        PersistentStreamingMaskConditionalStatisticsVectorImageFilter<TInputImage, TInputMask, TPrecision>::GetComponentCorrelationOutput()
        {
            return static_cast<RealObjectType*>(this->itk::ProcessObject::GetOutput(10));
        }

    template<class TInputImage, class TInputMask, class TPrecision>
        const typename PersistentStreamingMaskConditionalStatisticsVectorImageFilter<TInputImage, TInputMask, TPrecision>::RealObjectType*
        PersistentStreamingMaskConditionalStatisticsVectorImageFilter<TInputImage, TInputMask, TPrecision>::GetComponentCorrelationOutput() const
        {
            return static_cast<const RealObjectType*>(this->itk::ProcessObject::GetOutput(10));
        }

    template<class TInputImage, class TInputMask, class TPrecision>
        typename PersistentStreamingMaskConditionalStatisticsVectorImageFilter<TInputImage, TInputMask, TPrecision>::RealObjectType*
        PersistentStreamingMaskConditionalStatisticsVectorImageFilter<TInputImage, TInputMask, TPrecision>::GetComponentCovarianceOutput()
        {
            return static_cast<RealObjectType*>(this->itk::ProcessObject::GetOutput(11));
        }

    template<class TInputImage, class TInputMask, class TPrecision>
        const typename PersistentStreamingMaskConditionalStatisticsVectorImageFilter<TInputImage, TInputMask, TPrecision>::RealObjectType*
        PersistentStreamingMaskConditionalStatisticsVectorImageFilter<TInputImage, TInputMask, TPrecision>::GetComponentCovarianceOutput() const
        {
            return static_cast<const RealObjectType*>(this->itk::ProcessObject::GetOutput(11));
        }

    template<class TInputImage, class TInputMask, class TPrecision>
        typename PersistentStreamingMaskConditionalStatisticsVectorImageFilter<TInputImage, TInputMask, TPrecision>::UIObjectType*
        PersistentStreamingMaskConditionalStatisticsVectorImageFilter<TInputImage, TInputMask, TPrecision>::GetNbOfValidValuesOutput()
        {
            return static_cast<UIObjectType*>(this->itk::ProcessObject::GetOutput(12));
        }

    template<class TInputImage, class TInputMask, class TPrecision>
        const typename PersistentStreamingMaskConditionalStatisticsVectorImageFilter<TInputImage, TInputMask, TPrecision>::UIObjectType*
        PersistentStreamingMaskConditionalStatisticsVectorImageFilter<TInputImage, TInputMask, TPrecision>::GetNbOfValidValuesOutput() const
        {
            return static_cast<const UIObjectType*>(this->itk::ProcessObject::GetOutput(12));
        }

    template<class TInputImage, class TInputMask, class TPrecision>
        typename PersistentStreamingMaskConditionalStatisticsVectorImageFilter<TInputImage, TInputMask, TPrecision>::UIObjectType*
        PersistentStreamingMaskConditionalStatisticsVectorImageFilter<TInputImage, TInputMask, TPrecision>::GetNbOfExcludedValuesOutput()
        {
            return static_cast<UIObjectType*>(this->itk::ProcessObject::GetOutput(13));
        }

    template<class TInputImage, class TInputMask, class TPrecision>
        const typename PersistentStreamingMaskConditionalStatisticsVectorImageFilter<TInputImage, TInputMask, TPrecision>::UIObjectType*
        PersistentStreamingMaskConditionalStatisticsVectorImageFilter<TInputImage, TInputMask, TPrecision>::GetNbOfExcludedValuesOutput() const
        {
            return static_cast<const UIObjectType*>(this->itk::ProcessObject::GetOutput(13));
        }

    template<class TInputImage, class TInputMask, class TPrecision>
        typename PersistentStreamingMaskConditionalStatisticsVectorImageFilter<TInputImage, TInputMask, TPrecision>::RealPixelObjectType*
        PersistentStreamingMaskConditionalStatisticsVectorImageFilter<TInputImage, TInputMask, TPrecision>::GetNbOfValidValuesPerBandOutput()
        {
            return static_cast<RealPixelObjectType*>(this->itk::ProcessObject::GetOutput(14));
        }

    template<class TInputImage, class TInputMask, class TPrecision>
        const typename PersistentStreamingMaskConditionalStatisticsVectorImageFilter<TInputImage, TInputMask, TPrecision>::RealPixelObjectType*
        PersistentStreamingMaskConditionalStatisticsVectorImageFilter<TInputImage, TInputMask, TPrecision>::GetNbOfValidValuesPerBandOutput() const
        {
            return static_cast<const RealPixelObjectType*>(this->itk::ProcessObject::GetOutput(14));
        }

    template<class TInputImage, class TInputMask, class TPrecision>
        typename PersistentStreamingMaskConditionalStatisticsVectorImageFilter<TInputImage, TInputMask, TPrecision>::RealPixelObjectType*
        PersistentStreamingMaskConditionalStatisticsVectorImageFilter<TInputImage, TInputMask, TPrecision>::GetNbOfExcludedValuesPerBandOutput()
        {
            return static_cast<RealPixelObjectType*>(this->itk::ProcessObject::GetOutput(15));
        }

    template<class TInputImage, class TInputMask, class TPrecision>
        const typename PersistentStreamingMaskConditionalStatisticsVectorImageFilter<TInputImage, TInputMask, TPrecision>::RealPixelObjectType*
        PersistentStreamingMaskConditionalStatisticsVectorImageFilter<TInputImage, TInputMask, TPrecision>::GetNbOfExcludedValuesPerBandOutput() const
        {
            return static_cast<const RealPixelObjectType*>(this->itk::ProcessObject::GetOutput(15));
        }

    template<class TInputImage, class TInputMask, class TPrecision>
        void
        PersistentStreamingMaskConditionalStatisticsVectorImageFilter<TInputImage, TInputMask, TPrecision>::GenerateOutputInformation()
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

    template<class TInputImage, class TInputMask, class TPrecision>
        void
        PersistentStreamingMaskConditionalStatisticsVectorImageFilter<TInputImage, TInputMask, TPrecision>::AllocateOutputs()
        {
            // This is commented to prevent the streaming of the whole image for the first stream strip
            // It shall not cause any problem because the output image of this filter is not intended to be used.
            //InputImagePointer image = const_cast< TInputImage * >( this->GetInput() );
            //this->GraftOutput( image );
            // Nothing that needs to be allocated for the remaining outputs
        }

    template<class TInputImage, class TInputMask, class TPrecision>
        void
        PersistentStreamingMaskConditionalStatisticsVectorImageFilter<TInputImage, TInputMask, TPrecision>::Reset()
        {
            TInputImage * inputPtr = const_cast<TInputImage *>(this->GetInput());
            inputPtr->UpdateOutputInformation();

            unsigned int numberOfThreads = this->GetNumberOfThreads();
            unsigned int numberOfComponent = inputPtr->GetNumberOfComponentsPerPixel();

            //Zero values
            RealPixelType zeroRealPixel;
            zeroRealPixel.SetSize(numberOfComponent);
            zeroRealPixel.Fill(itk::NumericTraits<PrecisionType>::ZeroValue());
            RealType zeroReal = itk::NumericTraits<RealType>::ZeroValue();

            // Class parameters
            m_NumberOfValidPixels.SetSize(numberOfThreads);
            m_NumberOfValidPixels.Fill(itk::NumericTraits<long>::Zero);
            m_NumberOfValidPixelsPerBand.resize(numberOfThreads);
            std::fill(m_NumberOfValidPixelsPerBand.begin(), m_NumberOfValidPixelsPerBand.end(), zeroRealPixel);

            // Initialize the output values
            this->GetNbOfValidValuesOutput()->Set(itk::NumericTraits<long>::Zero);
            this->GetNbOfExcludedValuesOutput()->Set(itk::NumericTraits<long>::Zero);

            // If the Min and Max values are computed
            if (m_EnableMinMax)
            {
                PixelType tempPixel;
                tempPixel.SetSize(numberOfComponent);

                // Initialization of the min and max values
                tempPixel.Fill(itk::NumericTraits<InternalPixelType>::max());
                this->GetMinimumOutput()->Set(tempPixel);

                tempPixel.Fill(itk::NumericTraits<InternalPixelType>::NonpositiveMin());
                this->GetMaximumOutput()->Set(tempPixel);

                // Initialization of the values per thread
                PixelType tempTemporiesPixel;
                tempTemporiesPixel.SetSize(numberOfComponent);
                tempTemporiesPixel.Fill(itk::NumericTraits<InternalPixelType>::max());
                m_ThreadMin = std::vector<PixelType>(numberOfThreads, tempTemporiesPixel);

                tempTemporiesPixel.Fill(itk::NumericTraits<InternalPixelType>::NonpositiveMin());
                m_ThreadMax = std::vector<PixelType>(numberOfThreads, tempTemporiesPixel);
            }

            if (m_EnableSecondOrderStats)
            {
                m_EnableFirstOrderStats = true;
            }

            // Initialization of the first order statistics

            if (m_EnableFirstOrderStats)
            {
                // Set the mean and sum values to the output image
                this->GetMeanOutput()->Set(zeroRealPixel);
                this->GetSumOutput()->Set(zeroRealPixel);
                m_ThreadFirstOrderAccumulators.resize(numberOfThreads);
                std::fill(m_ThreadFirstOrderAccumulators.begin(), m_ThreadFirstOrderAccumulators.end(), zeroRealPixel);
                m_ThreadFirstOrderComponentAccumulators.resize(numberOfThreads);
                std::fill(m_ThreadFirstOrderComponentAccumulators.begin(), m_ThreadFirstOrderComponentAccumulators.end(), zeroReal);
            }
            // Initialization of the standard deviation
            if (m_EnableStandardDeviation)
            {
                m_ThreadFirstOrderSquareAccumulators.resize(numberOfThreads);
                std::fill(m_ThreadFirstOrderSquareAccumulators.begin(), m_ThreadFirstOrderSquareAccumulators.end(), zeroRealPixel);
                m_ThreadFirstOrderSquareComponentAccumulators.resize(numberOfThreads);
                std::fill(m_ThreadFirstOrderSquareComponentAccumulators.begin(), m_ThreadFirstOrderSquareComponentAccumulators.end(),
                        zeroReal);
            }
            if (m_EnableSecondOrderStats)
            {
                MatrixType zeroMatrix;
                zeroMatrix.SetSize(numberOfComponent, numberOfComponent);
                zeroMatrix.Fill(itk::NumericTraits<PrecisionType>::Zero);
                // Set the Covariance and correlation value to the output
                this->GetCovarianceOutput()->Set(zeroMatrix);
                this->GetCorrelationOutput()->Set(zeroMatrix);

                m_ThreadSecondOrderAccumulators.resize(numberOfThreads);
                std::fill(m_ThreadSecondOrderAccumulators.begin(), m_ThreadSecondOrderAccumulators.end(), zeroMatrix);

                m_ThreadSecondOrderComponentAccumulators.resize(numberOfThreads);
                std::fill(m_ThreadSecondOrderComponentAccumulators.begin(), m_ThreadSecondOrderComponentAccumulators.end(), zeroReal);
            }

            // Boolean used to indicate if the number of pixels used to compute the statistics is not null
            m_IsValid = false;

        }

    template<class TInputImage, class TInputMask, class TPrecision>
        void
        PersistentStreamingMaskConditionalStatisticsVectorImageFilter<TInputImage, TInputMask, TPrecision>::Synthetize()
        {
            TInputImage * inputPtr = const_cast<TInputImage *>(this->GetInput());
            const unsigned int nbPixels = inputPtr->GetLargestPossibleRegion().GetNumberOfPixels();
            const unsigned int numberOfComponent = inputPtr->GetNumberOfComponentsPerPixel();
            RealPixelType nbPixelsPerBand(numberOfComponent);
            nbPixelsPerBand.Fill(inputPtr->GetLargestPossibleRegion().GetNumberOfPixels());
            // Initializations
            PixelType minimum;
            minimum.SetSize(numberOfComponent);
            minimum.Fill(itk::NumericTraits<InternalPixelType>::max());
            PixelType maximum;
            maximum.SetSize(numberOfComponent);
            maximum.Fill(itk::NumericTraits<InternalPixelType>::NonpositiveMin());

            RealPixelType streamFirstOrderAccumulator(numberOfComponent);
            streamFirstOrderAccumulator.Fill(itk::NumericTraits<PrecisionType>::Zero);
            RealPixelType streamFirstOrderSquareAccumulator(numberOfComponent);
            streamFirstOrderSquareAccumulator.Fill(itk::NumericTraits<PrecisionType>::Zero);
            MatrixType streamSecondOrderAccumulator(numberOfComponent, numberOfComponent);
            streamSecondOrderAccumulator.Fill(itk::NumericTraits<PrecisionType>::Zero);

            RealType streamFirstOrderComponentAccumulator = itk::NumericTraits<RealType>::Zero;
            RealType streamFirstOrderSquareComponentAccumulator = itk::NumericTraits<RealType>::Zero;
            RealType streamSecondOrderComponentAccumulator = itk::NumericTraits<RealType>::Zero;

            long nbValidPixels(0);
            RealPixelType nbValidPixelsPerBand(numberOfComponent);
            nbValidPixelsPerBand.Fill(itk::NumericTraits<RealType>::Zero);
            unsigned long totalValidPixelsPerBand(0);

            // Accumulate results from all threads
            const unsigned int numberOfThreads = this->GetNumberOfThreads();
            for (unsigned int threadId = 0; threadId < numberOfThreads; ++threadId)
            {
                //Valid pixel per band
                nbValidPixelsPerBand += m_NumberOfValidPixelsPerBand[threadId];
                nbValidPixels += m_NumberOfValidPixels[threadId];
                for (unsigned int j = 0; j < numberOfComponent; ++j)
                {
                    totalValidPixelsPerBand += static_cast<unsigned long>(m_NumberOfValidPixelsPerBand[threadId][j]);
                }
                // Min and max research option set to true
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

                }
                if (m_EnableFirstOrderStats)
                {
                    streamFirstOrderAccumulator += m_ThreadFirstOrderAccumulators[threadId];
                    streamFirstOrderComponentAccumulator += m_ThreadFirstOrderComponentAccumulators[threadId];
                }
                if (m_EnableStandardDeviation)
                {
                    streamFirstOrderSquareAccumulator += m_ThreadFirstOrderSquareAccumulators[threadId];
                    streamFirstOrderSquareComponentAccumulator += m_ThreadFirstOrderSquareComponentAccumulators[threadId];
                }

                if (m_EnableSecondOrderStats)
                {
                    streamSecondOrderAccumulator += m_ThreadSecondOrderAccumulators[threadId];
                    streamSecondOrderComponentAccumulator += m_ThreadSecondOrderComponentAccumulators[threadId];
                }
            }
            // Test if all the pixels are invalid => return null statistics
            if ((nbValidPixels == 0) || (totalValidPixelsPerBand == 0))
            {
                m_IsValid = false;

                vnsLogWarningMacro(
                        "PersistentMaskConditionalStatisticsVectorImageFilter::Synthetize." "No pixel is valid (Not masked nb valid pixels="<<nbValidPixels<<" and not NoData nb valids pixels total per band="<<totalValidPixelsPerBand<<"). Return null statistics");

                // Set default values
                minimum.Fill(itk::NumericTraits<InternalPixelType>::Zero);
                maximum.Fill(itk::NumericTraits<InternalPixelType>::Zero);
                this->GetMinimumOutput()->Set(minimum);
                this->GetMaximumOutput()->Set(maximum);
                this->GetSumOutput()->Set(static_cast<RealPixelType>(streamFirstOrderAccumulator));
                this->GetMeanOutput()->Set(static_cast<RealPixelType>(streamFirstOrderAccumulator));
                this->GetStandardDeviationOutput()->Set(static_cast<RealPixelType>(streamFirstOrderAccumulator));

                MatrixType cor;
                cor.SetSize(numberOfComponent, numberOfComponent);
                cor.Fill(itk::NumericTraits<RealType>::Zero);

                this->GetCovarianceOutput()->Set(cor);
            }
            // If some pixels are valid => Set the outputs
            else
            {
                //This flag is always true if a single pixel from a single band is correct
                m_IsValid = true;

                // Global calculations band conditional
                if (m_EnableMinMax)
                {
                    this->GetMinimumOutput()->Set(minimum);
                    this->GetMaximumOutput()->Set(maximum);
                }
                if (m_EnableFirstOrderStats)
                {
                    // Compute the sum, the mean and the mean for all the components tacking into account the valid pixels (and not the excluded one)
                    this->GetComponentMeanOutput()->Set(streamFirstOrderComponentAccumulator / totalValidPixelsPerBand); //(nbPixels * numberOfComponent));
                }
                if (m_EnableStandardDeviation)
                {
                    RealType stdv = streamFirstOrderSquareComponentAccumulator / static_cast<double>(totalValidPixelsPerBand);
                    stdv -= vcl_pow(streamFirstOrderComponentAccumulator / static_cast<double>(totalValidPixelsPerBand),2);
                    this->GetComponentStandardDeviationOutput()->Set(static_cast<RealType>(std::sqrt(vcl_abs(stdv)))); //(nbPixels * numberOfComponent));
                }
                // Per Band case

                RealPixelType l_Mean(numberOfComponent);
                l_Mean.Fill(itk::NumericTraits<PrecisionType>::Zero);
                RealPixelType l_Sum(numberOfComponent);
                l_Sum.Fill(itk::NumericTraits<PrecisionType>::Zero);
                RealPixelType l_Stdv(numberOfComponent);
                l_Stdv.Fill(itk::NumericTraits<PrecisionType>::Zero);

                for (unsigned int j = 0; j < numberOfComponent; ++j)
                {
                    if (nbValidPixelsPerBand[j] != 0)
                    {
                        if (m_EnableFirstOrderStats)
                        {
                            // Compute the sum, the mean and the mean for all the components tacking into account the valid pixels (and not the excluded one)
                            l_Sum[j] = streamFirstOrderAccumulator[j];
                            l_Mean[j] = streamFirstOrderAccumulator[j] / static_cast<RealType>(nbValidPixelsPerBand[j]); //nbPixels);
                        }
                        if (m_EnableStandardDeviation)
                        {
                            RealType stdvPix = streamFirstOrderSquareAccumulator[j] / static_cast<RealType>(nbValidPixelsPerBand[j]);
                            stdvPix -= static_cast<RealType>(vcl_pow(streamFirstOrderAccumulator[j] / static_cast<double>(nbValidPixelsPerBand[j]),2));
                            stdvPix = static_cast<RealType>(std::sqrt(vcl_abs(stdvPix)));
                            l_Stdv[j] = stdvPix;
                        }
                    }
                }
                this->GetMeanOutput()->Set(l_Mean); //nbPixels);
                this->GetSumOutput()->Set(l_Sum);
                this->GetStandardDeviationOutput()->Set(l_Stdv);


                if (m_EnableSecondOrderStats)
                {

                    MatrixType cor = streamSecondOrderAccumulator / nbValidPixels; //nbPixels;
                    this->GetCorrelationOutput()->Set(cor);

                    const RealPixelType& mean = this->GetMeanOutput()->Get();
                    double regul(0.);
                    if (nbValidPixels == 1)
                    {
                        vnsLogWarningMacro(
                                "PersistentMaskConditionalStatisticsVectorImageFilter::Synthetize: Only one pixel valid computed on this image!")
                        regul = 1.;
                    }
                    else
                    {
                        regul = static_cast<double>(nbValidPixels) / (nbValidPixels - 1); //(nbPixels) / (nbPixels - 1);
                    }

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
                    if ((nbValidPixels * numberOfComponent) == 1)
                    {
                        this->GetComponentCovarianceOutput()->Set(itk::NumericTraits<RealType>::Zero);
                    }
                    else
                    {
                        this->GetComponentCovarianceOutput()->Set(
                                (nbValidPixels * numberOfComponent) / (nbValidPixels * numberOfComponent - 1)
                                //(nbPixels * numberOfComponent) / (nbPixels * numberOfComponent - 1)
                                        * (this->GetComponentCorrelation() - (this->GetComponentMean() * this->GetComponentMean())));
                    }
                }
            } // end if nbValidPixels

            // Set the number of used pixels to compute statistics and the number of excluded values
            this->GetNbOfValidValuesPerBandOutput()->Set(nbValidPixelsPerBand);
            this->GetNbOfExcludedValuesPerBandOutput()->Set(nbPixelsPerBand - nbValidPixelsPerBand);
            this->GetNbOfValidValuesOutput()->Set(nbValidPixels);
            this->GetNbOfExcludedValuesOutput()->Set(nbPixels - nbValidPixels);
        }

    template<class TInputImage, class TInputMask, class TPrecision>
        void
        PersistentStreamingMaskConditionalStatisticsVectorImageFilter<TInputImage, TInputMask, TPrecision>::ThreadedGenerateData(
                const RegionType& outputRegionForThread, itk::ThreadIdType threadId)
        {

            // Support progress methods/callbacks

            // Grab the input image
            InputImagePointer inputPtr = const_cast<TInputImage *>(this->GetInput());
            // Get the input mask
            InputMaskPointer maskPtr = dynamic_cast<TInputMask *>(this->itk::ProcessObject::GetInput(1));

            itk::ImageRegionConstIteratorWithIndex<TInputImage> it(inputPtr, outputRegionForThread);
            itk::ImageRegionConstIteratorWithIndex<TInputMask> maskIt(maskPtr, outputRegionForThread);

            maskIt.GoToBegin();

            const double l_DoubleForegroundValue = static_cast<double>(m_ForegroundValue);

            for (it.GoToBegin(); !it.IsAtEnd(); ++it) //MACCS 4.3 - disable progress : progress.CompletedPixel())
            {

                // Get the values of the mask pixels used for the selection of the pixels
                const MaskPixelType& maskValue = maskIt.Get();

                // Statistics computed if the pixel of the mask is equal to m_ForegroundValue
                if (vnsEqualsDoubleMacro(static_cast<double>(maskValue), l_DoubleForegroundValue))
                {

                    const PixelType& vectorValue = it.Get();
                    const unsigned int l_NbValues = vectorValue.GetSize();
                    //Count good values
                    for (unsigned int j = 0; j < l_NbValues; ++j)
                    {
                        if ((m_EnableExcludeValue == false) || vnsIsNotNoDataMacro(vectorValue[j], m_ExcludeValue))
                        {
                            m_NumberOfValidPixelsPerBand[threadId][j]++;
                            // Compute min and max for the current thread
                            if (m_EnableMinMax)
                            {
                                // Initialize variables per thread
                                PixelType& threadMin = m_ThreadMin[threadId];
                                PixelType& threadMax = m_ThreadMax[threadId];

                                if (vectorValue[j] < threadMin[j])
                                {
                                    threadMin[j] = vectorValue[j];
                                }
                                if (vectorValue[j] > threadMax[j])
                                {
                                    threadMax[j] = vectorValue[j];
                                }
                            }

                            // Compute the sum used to calculate the mean values for this thread
                            if (m_EnableFirstOrderStats)
                            {
                                RealPixelType& threadFirstOrder = m_ThreadFirstOrderAccumulators[threadId];
                                RealType& threadFirstOrderComponent = m_ThreadFirstOrderComponentAccumulators[threadId];

                                threadFirstOrder[j] += vectorValue[j];
                                threadFirstOrderComponent += vectorValue[j];

                            }

                            if (m_EnableStandardDeviation)
                            {
                                RealPixelType& threadFirstOrderSquare = m_ThreadFirstOrderSquareAccumulators[threadId];
                                RealType& threadFirstOrderSquareComponent = m_ThreadFirstOrderSquareComponentAccumulators[threadId];

                                const InternalPixelType tmp = vectorValue[j];
                                threadFirstOrderSquareComponent += tmp * tmp;
                                threadFirstOrderSquare[j] += (tmp * tmp);
                            }
                        }
                    }

                    // Compute the squared sum used to calculate the covariance matrix
                    if (m_EnableSecondOrderStats)
                    {
                        MatrixType& threadSecondOrder = m_ThreadSecondOrderAccumulators[threadId];
                        RealType& threadSecondOrderComponent = m_ThreadSecondOrderComponentAccumulators[threadId];

                        for (unsigned int r = 0; r < threadSecondOrder.Rows(); ++r)
                        {
                            for (unsigned int c = 0; c < threadSecondOrder.Cols(); ++c)
                            {
                                threadSecondOrder(r, c) += vectorValue[r] * vectorValue[c];
                            }
                        }
                        threadSecondOrderComponent += vectorValue.GetSquaredNorm();
                    }

                    // Count the number of valid pixels per thread
                    m_NumberOfValidPixels[threadId]++;

                } // end if excluded value

                ++maskIt;
            }
        }

    template<class TImage, class TInputMask, class TPrecision>
        void
        PersistentStreamingMaskConditionalStatisticsVectorImageFilter<TImage, TInputMask, TPrecision>::PrintSelf(std::ostream& os,
                itk::Indent indent) const
        {
            Superclass::PrintSelf(os, indent);

            os << indent << "Min: " << this->GetMinimumOutput()->Get() << std::endl;
            os << indent << "Max: " << this->GetMaximumOutput()->Get() << std::endl;
            os << indent << "Mean: " << this->GetMeanOutput()->Get() << std::endl;
            os << indent << "Standard deviation: " << this->GetStandardDeviationOutput()->Get() << std::endl;
            os << indent << "Covariance: " << this->GetCovarianceOutput()->Get() << std::endl;
            os << indent << "Correlation: " << this->GetCorrelationOutput()->Get() << std::endl;
            os << indent << "Component Mean: " << this->GetComponentMeanOutput()->Get() << std::endl;
            os << indent << "Component standard deviation: " << this->GetComponentStandardDeviationOutput()->Get() << std::endl;
            os << indent << "Component Covariance: " << this->GetComponentCovarianceOutput()->Get() << std::endl;
            os << indent << "Component Correlation: " << this->GetComponentCorrelationOutput()->Get() << std::endl;
            os << indent << "UseUnbiasedEstimator: " << Utilities::BoolToLowerString(this->m_UseUnbiasedEstimator) << std::endl;
            os << indent << "ValidVal: " << this->GetNbOfValidValuesOutput()->Get() << std::endl;
            os << indent << "ExcludedVal: " << this->GetNbOfExcludedValuesOutput()->Get() << std::endl;
        }

} // end namespace otb
#endif
