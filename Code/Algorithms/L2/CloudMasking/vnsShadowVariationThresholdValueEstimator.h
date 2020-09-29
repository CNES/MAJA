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
 * VERSION : 5-1-0 : FA : LAIG-FA-MAC-145739-CS : 27 juin 2016 : Audit code - Supp de la macro ITK_EXPORT   *
 * VERSION : 5-1-0 : FA : LAIG-FA-MAC-145739-CS : 28 juin 2016 : Audit code - Correction du code            *
 * VERSION : 5-0-0 : DM : LAIG-DM-MAC-982-CNES : 5 avr. 2016 : Creation                                     *
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id:
 *                                                                                                          *
 ************************************************************************************************************/
#ifndef __vnsShadowVariationThresholdValueEstimator_h
#define __vnsShadowVariationThresholdValueEstimator_h

#include "otbPersistentImageFilter.h"
#include "otbPersistentFilterStreamingDecorator.h"

#include "otbObjectList.h"
#include "itkStatisticsAlgorithm.h"
#include "itkNumericTraits.h"
#include "itkHistogram.h"
#include "itkMacro.h"
#include "vnsMacro.h"
#include "vnsMath.h"
#include "vnsUtilities.h"

namespace vns
{

    /** \class PersistentShadowVariationThresholdValueEstimator
     * \brief Compute the histogram of a large image using streaming
     *
     *  This filter persists its temporary data. It means that if you Update it n times on n different
     * requested regions, the output histogram will be the histogram of the whole set of n regions.
     *
     * To reset the temporary data, one should call the Reset() function.
     *
     * To get the histogram once the regions have been processed via the pipeline, use the Synthetize() method.
     *
     * The threshold is then computed on demand.
     *
     * \sa PersistentImageFilter
     * \ingroup Streamed
     * \ingroup Multithreaded
     * \ingroup MathematicalStatisticsImageFilters
     *
     */
    template<class TInputImage>
        class PersistentShadowVariationThresholdValueEstimator : public otb::PersistentImageFilter<TInputImage, TInputImage>
        {
        public:
            /** Standard Self typedef */
            typedef PersistentShadowVariationThresholdValueEstimator Self;
            typedef otb::PersistentImageFilter<TInputImage, TInputImage> Superclass;
            typedef itk::SmartPointer<Self> Pointer;
            typedef itk::SmartPointer<const Self> ConstPointer;

            /** Method for creation through the object factory. */
            itkNewMacro(Self)

            /** Runtime information support. */
            itkTypeMacro(PersistentShadowVariationThresholdValueEstimator, otb::PersistentImageFilter)

            /** Image related typedefs. */
            typedef TInputImage ImageType;
            typedef typename TInputImage::Pointer InputImagePointer;
            typedef typename TInputImage::RegionType RegionType;
            typedef typename TInputImage::SizeType SizeType;
            typedef typename TInputImage::IndexType IndexType;
            typedef typename TInputImage::PixelType PixelType;
            typedef typename TInputImage::InternalPixelType InternalPixelType;

            itkStaticConstMacro(InputImageDimension, unsigned int, TInputImage::ImageDimension);

            /** Image related typedefs. */
            itkStaticConstMacro(ImageDimension, unsigned int, TInputImage::ImageDimension);

            /** Type to use for computations. */
            typedef typename itk::NumericTraits<InternalPixelType>::RealType RealType;
            typedef itk::VariableLengthVector<RealType> RealPixelType;

            /** Smart Pointer type to a DataObject. */
            typedef typename itk::DataObject::Pointer DataObjectPointer;

            /** Types for histogram */
            typedef itk::Statistics::DenseFrequencyContainer2 DFContainerType;

            typedef
            typename itk::NumericTraits<InternalPixelType>::RealType
            HistogramMeasurementRealType;

            typedef
            itk::Statistics::Histogram<HistogramMeasurementRealType, DFContainerType>
            HistogramType;

            typedef itk::VariableLengthVector< unsigned int > CountVectorType;

            typedef PixelType MeasurementVectorType;
            typedef otb::ObjectList<HistogramType> HistogramListType;
            typedef typename HistogramListType::Pointer HistogramListPointerType;
            typedef typename std::vector<HistogramListPointerType> ArrayHistogramListType;

            /** Set the no data value. These value are ignored in histogram
             *  computation if NoDataFlag is On
             */
            itkSetMacro(NoDataValue, InternalPixelType);

            /** Set the no data value. These value are ignored in histogram
             *  computation if NoDataFlag is On
             */
            itkGetConstReferenceMacro(NoDataValue, InternalPixelType);

            inline void SetNumberOfBins( unsigned int i, CountVectorType::ValueType size )
            {
                m_Size[ i ] = size;
            }

            inline void SetNumberOfBins( const CountVectorType& size )
            {
                m_Size = size;
            }

            /** Return the computed histogram list */
            HistogramListType* GetHistogramListOutput()
            {
                return static_cast<HistogramListType*>(this->itk::ProcessObject::GetOutput(1));
            }
            const HistogramListType* GetHistogramListOutput() const
            {
                return static_cast<const HistogramListType*>(this->itk::ProcessObject::GetOutput(1));
            }

            vnsMemberAndSetAndGetConstReferenceMacro(HistogramMinValue, InternalPixelType)

            vnsMemberAndSetAndGetConstReferenceMacro(HistogramMaxValue, InternalPixelType)

            /** Get the minimum values for histograms */
            itkGetConstReferenceMacro(HistogramMin,MeasurementVectorType);

            /** Set the minimum values for histograms */
            itkSetMacro(HistogramMin,MeasurementVectorType);

            /** Get the maximum values for histograms */
            itkGetConstReferenceMacro(HistogramMax,MeasurementVectorType);

            /** Set the maximum values for histograms */
            itkSetMacro(HistogramMax,MeasurementVectorType);

            // ShadVarPercentile accessors
            itkSetMacro(ShadVarPercentile, double);
            itkGetConstReferenceMacro(ShadVarPercentile, double);

            // ShadVarMaxDarkening accessors
            itkSetMacro(ShadVarDarkeningMax, double);
            itkGetConstReferenceMacro(ShadVarDarkeningMax, double);

            /** Make a DataObject of the correct type to be used as the specified
             * output.
             */
            virtual DataObjectPointer MakeOutput(unsigned int idx);

            /** Pass the input through unmodified. Do this by Grafting in the
             *  AllocateOutputs method.
             */
            virtual void AllocateOutputs();
            virtual void GenerateOutputInformation();
            virtual void Synthetize(void);
            virtual void Reset(void);

        protected:
            PersistentShadowVariationThresholdValueEstimator();
            virtual ~PersistentShadowVariationThresholdValueEstimator()
            {}
            virtual void PrintSelf(std::ostream& os, itk::Indent indent) const;
            /** Multi-thread version GenerateData. */
            void ThreadedGenerateData(const RegionType& outputRegionForThread, itk::ThreadIdType threadId);

        private:
            PersistentShadowVariationThresholdValueEstimator(const Self &); //purposely not implemented
            void operator =(const Self&);//purposely not implemented

            ArrayHistogramListType m_ThreadHistogramList;
            CountVectorType m_Size;
            MeasurementVectorType m_HistogramMin;
            MeasurementVectorType m_HistogramMax;
            InternalPixelType m_NoDataValue;

            // Threshold computation parameters
            double m_ShadVarPercentile;
            double m_ShadVarDarkeningMax;

        }; // end of class PersistentStatisticsVectorImageFilter

        /**===========================================================================*/

        /** \class ShadowVariationThresholdValueEstimator
         * \brief This class streams the whole input image through the PersistentShadowVariationThresholdValueEstimator.
         *
         * This way, it allows to compute the min/max of this image. It calls the
         * Reset() method of the PersistentShadowVariationThresholdValueEstimator before streaming the image and the
         * Synthetize() method of the PersistentShadowVariationThresholdValueEstimator after having streamed the image
         * to compute the statistics. The accessor on the results are wrapping the accessors of the
         * internal PersistentMinMaxImageFilter.
         *
         * \sa PersistentStatisticsVectorImageFilter
         * \sa PersistentImageFilter
         * \sa PersistentFilterStreamingDecorator
         * \sa StreamingImageVirtualWriter
         * \ingroup Streamed
         * \ingroup Multithreaded
         * \ingroup MathematicalStatisticsImageFilters
         */

    template<class TInputImage>
        class ShadowVariationThresholdValueEstimator : public otb::PersistentFilterStreamingDecorator<
                PersistentShadowVariationThresholdValueEstimator<TInputImage> >
        {
        public:
            /** Standard Self typedef */
            typedef ShadowVariationThresholdValueEstimator Self;
            typedef otb::PersistentFilterStreamingDecorator<PersistentShadowVariationThresholdValueEstimator<TInputImage> > Superclass;
            typedef itk::SmartPointer<Self> Pointer;
            typedef itk::SmartPointer<const Self> ConstPointer;

            /** Type macro */
            itkNewMacro(Self)

            /** Creation through object factory macro */
            itkTypeMacro(ShadowVariationThresholdValueEstimator, otb::PersistentFilterStreamingDecorator)

            typedef TInputImage InputImageType;
            typedef typename Superclass::FilterType InternalFilterType;

            /** Types needed for histograms */
            typedef typename InternalFilterType::HistogramType HistogramType;
            typedef typename InternalFilterType::HistogramListType HistogramListType;

            typedef typename InternalFilterType::InternalPixelType InternalPixelType;
            typedef typename PersistentShadowVariationThresholdValueEstimator<TInputImage>::CountVectorType CountVectorType;

            void
            SetReflRatio(InputImageType * input)
            {
                this->GetFilter()->SetInput(input);
            }
            const InputImageType *
            GetReflRatio()
            {
                return this->GetFilter()->GetInput();
            }

            void
            SetNoDataValue(InternalPixelType val)
            {
                this->GetFilter()->SetNoDataValue(val);
            }

            void
            SetHistogramMin(InternalPixelType val)
            {
                this->GetFilter()->SetHistogramMinValue(val);
            }

            void
            SetHistogramMax(InternalPixelType val)
            {
                this->GetFilter()->SetHistogramMaxValue(val);
            }

            /** Return the computed histogram */
            HistogramListType*
            GetHistogramList()
            {
                return this->GetFilter()->GetHistogramListOutput();
            }

            void
            SetShadVarPercentile(double p)
            {
                this->GetFilter()->SetShadVarPercentile(p);
            }

            double
            GetShadVarPercentile()
            {
                return this->GetFilter()->GetShadVarPercentile();
            }

            void
            SetShadVarDarkeningMax(double p)
            {
                this->GetFilter()->SetShadVarDarkeningMax(p);
            }
            double
            GetShadVarDarkeningMax()
            {
                return this->GetFilter()->GetShadVarDarkeningMax();
            }

            void
            SetNumberOfBins(const CountVectorType& size)
            {
                this->GetFilter()->SetNumberOfBins(size);
            }

            // return threshold value
            double
            GetThresholdValue()
            {
                //    HistogramType outputHisto = this->GetHistogramList()->GetNthElement(0);
                itkAssertInDebugAndIgnoreInReleaseMacro(this->GetHistogramList()->Size() > 0);

                // LAIG-FA-MAC-145739-CS : report from clang sanitize. Check the TotalFrequency to avoid / divided by zero in Quantile method
                const double l_TotalFrequency = this->GetHistogramList()->GetNthElement(0)->GetTotalFrequency();
                if (vnsIsZeroDoubleMacro(l_TotalFrequency) == true)
                {
                    // return default value ShadVarDarkeningMax
                    return this->GetShadVarDarkeningMax();
                }
                else
                {
                    const double l_Quantile = this->GetHistogramList()->GetNthElement(0)->Quantile(0, this->GetShadVarPercentile());
                    vnsLogDebugMacro(
                            "ShadVarQuantile: " << l_Quantile << " - IsNan ? : "<<Utilities::BoolToLowerString(std::isnan(l_Quantile)) << " - ShadVarDarkeningMax: " << this->GetShadVarDarkeningMax())
                    if (std::isnan(l_Quantile) == true)
                    {
                        // return default value ShadVarDarkeningMax
                        return this->GetShadVarDarkeningMax();
                    }
                    else
                    {
                        return std::min(l_Quantile, this->GetShadVarDarkeningMax());
                    }
                }
            }

        protected:
            /** Constructor */
            ShadowVariationThresholdValueEstimator()
            {
            }

            /** Destructor */
            virtual
            ~ShadowVariationThresholdValueEstimator()
            {
            }

        private:
            ShadowVariationThresholdValueEstimator(const Self &); //purposely not implemented
            void
            operator =(const Self&); //purposely not implemented
        };

} // End namespace vns
#ifndef VNS_MANUAL_INSTANTIATION
#include "vnsShadowVariationThresholdValueEstimator.txx"
#endif

#endif /* __vnsShadowVariationThresholdValueEstimator_h */
