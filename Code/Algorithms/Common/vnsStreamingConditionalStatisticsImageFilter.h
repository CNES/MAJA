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
 * VERSION : 3-2-0 : DM : LAIG-DM-MAC-697-CNES : 8 novembre 2013 : Mise à jour du plugin SENTINEL2          *
 * 					Action interne 98664. Prise en compte  des modifications apportées à la classe   		*
 * 					otbStreamingStatisticsVectorImageFilter v3.13   										*
 * VERSION : 1-0-0 : <TypeFT> : <NumFT> : 1 avr. 2010 : Creation
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id: vnsStreamingConditionalStatisticsImageFilter.h 9316 2015-01-12 08:25:12Z tfeuvrie $
 *                                                                                                          *
 ************************************************************************************************************/
#ifndef __vnsStreamingConditionalStatisticsImageFilter_h
#define __vnsStreamingConditionalStatisticsImageFilter_h

#include "otbPersistentImageFilter.h"
#include "otbPersistentFilterStreamingDecorator.h"

#include "otbImage.h"
#include "itkNumericTraits.h"
#include "itkArray.h"
#include "itkSimpleDataObjectDecorator.h"
#include "itkImageRegionSplitter.h"
#include "vnsLoggers.h"

namespace vns
{

    /** \class PersistentStreamingConditionalStatisticsImageFilter
     * \brief Compute covariance & correlation of a large image using streaming excluding
     * the pixels equal to a value specified by the input parameter m_ExcludedValue.
     *
     *  This filter persists its temporary data. It means that if you Update it n times on n different
     * requested regions, the output statistics will be the statitics of the whole set of n regions.
     *
     * To reset the temporary data, one should call the Reset() function.
     *
     * To get the statistics once the regions have been processed via the pipeline, use the Synthetize() method.
     *
     * Note that if a pixel is equal to m_ExcludedValue in one band, it is considered equal to
     * m_ExcludedValue in all bands.
     *
     * \sa PersistentImageFilter
     * \sa PersistentConditionalStatisticsVectorImageFilter
     *
     * \ingroup L2
     *
     */
    template<class TInputImage, class TInputMask, class TPrecision>
        class PersistentStreamingConditionalStatisticsImageFilter : public otb::PersistentImageFilter<TInputImage, TInputImage>
        {
        public:
            /** Standard Self typedef */
            typedef PersistentStreamingConditionalStatisticsImageFilter Self;
            typedef otb::PersistentImageFilter<TInputImage, TInputImage> Superclass;
            typedef itk::SmartPointer<Self> Pointer;
            typedef itk::SmartPointer<const Self> ConstPointer;

            /** Method for creation through the object factory. */
            itkNewMacro(Self)
            ;

            /** Runtime information support. */
            itkTypeMacro(PersistentStreamingConditionalStatisticsImageFilter, PersistentImageFilter)
            ;

            /** Image related typedefs. */
            typedef TInputImage ImageType;
            typedef typename ImageType::Pointer InputImagePointer;
            typedef typename ImageType::RegionType RegionType;
            typedef typename ImageType::SizeType SizeType;
            typedef typename ImageType::IndexType IndexType;
            typedef typename ImageType::PixelType PixelType;
            typedef typename ImageType::InternalPixelType InternalPixelType;

            typedef TInputMask MaskType;
            typedef typename TInputMask::Pointer InputMaskPointer;
            typedef typename TInputMask::PixelType MaskPixelType;

            typedef TPrecision PrecisionType;
            typedef PrecisionType RealType;

            /** Image related typedefs. */
            itkStaticConstMacro(ImageDimension, unsigned int, TInputImage::ImageDimension);

            /** Smart Pointer type to a DataObject. */
            typedef typename itk::DataObject::Pointer DataObjectPointer;

            /** Type to use for computations. */
            typedef RealType RealPixelType;

            /** Type of DataObjects used for outputs */
            typedef itk::SimpleDataObjectDecorator<RealType> RealObjectType;
            typedef itk::SimpleDataObjectDecorator<IndexType> IndexObjectType;
            typedef itk::SimpleDataObjectDecorator<PixelType> PixelObjectType;
            typedef itk::SimpleDataObjectDecorator<RealPixelType> RealPixelObjectType;

            // Used to count the number of valid pixels
            typedef itk::SimpleDataObjectDecorator<unsigned long> UIObjectType;

            /** Set the input mask*/
            void SetMaskInput(const MaskType * lMask)
            {
                this->SetNthInput(1, const_cast<MaskType *> (lMask));
                m_UseMaskInput = true;
                vnsLogDebugMacro("UseMaskInput: "<< m_UseMaskInput)
            }

            itkSetMacro(MaskForegroundValue, MaskPixelType);
            itkGetMacro(MaskForegroundValue, MaskPixelType);

            /** Set the excluded value in the statistic computation. */
            itkSetMacro(ExcludedValue, InternalPixelType);
            /** Get the excluded value in the statistic computation. */
            itkGetConstReferenceMacro(ExcludedValue, InternalPixelType);

            /** Return the computed Min */
            PixelType GetMinimum() const
            {
                return this->GetMinOutput()->Get();
            }
            PixelObjectType* GetMinimumOutput();
            const PixelObjectType* GetMinimumOutput() const;

            /** Return the computed Max index */
            PixelType GetMaximum() const
            {
                return this->GetMaxOutput()->Get();
            }
            PixelObjectType* GetMaximumOutput();
            const PixelObjectType* GetMaximumOutput() const;

            /** Return the computed Mean. */
            RealPixelType GetMean() const
            {
                return this->GetMeanOutput()->Get();
            }
            RealPixelObjectType* GetMeanOutput();
            const RealPixelObjectType* GetMeanOutput() const;

            /** Return the computed Sum. */
            RealPixelType GetSum() const
            {
                return this->GetSumOutput()->Get();
            }
            RealPixelObjectType* GetSumOutput();
            const RealPixelObjectType* GetSumOutput() const;

            /** Return the computed Variance. */
            RealPixelType GetVariance() const
            {
                return this->GetVarianceOutput()->Get();
            }
            RealPixelObjectType* GetVarianceOutput();
            const RealPixelObjectType* GetVarianceOutput() const;

            /** Return the computed Variance. */
            RealPixelType GetStandardDeviation() const
            {
                return this->GetStandardDeviationOutput()->Get();
            }
            RealPixelObjectType* GetStandardDeviationOutput();
            const RealPixelObjectType* GetStandardDeviationOutput() const;

            /** Return the number of valid values. */
            unsigned long GetNbOfValidValues() const
            {
                return this->GetNbOfValidValuesOutput()->Get();
            }
            UIObjectType* GetNbOfValidValuesOutput();
            const UIObjectType* GetNbOfValidValuesOutput() const;

            /** Return the number of excluded values. */
            unsigned int GetNbOfExcludedValues() const
            {
                return this->GetNbOfExcludedValuesOutput()->Get();
            }
            UIObjectType* GetNbOfExcludedValuesOutput();
            const UIObjectType* GetNbOfExcludedValuesOutput() const;

            /** Check the validity of the statistics */
            itkGetConstMacro(IsValid,bool)

            /** Make a DataObject of the correct type to be used as the specified
             * output.
             */
            virtual DataObjectPointer MakeOutput(unsigned int idx);

            virtual void Reset(void);

            virtual void Synthetize(void);

            itkSetMacro(EnableMinMax, bool);
            itkGetMacro(EnableMinMax, bool);

            itkSetMacro(EnableExcludeValue, bool);
            itkGetMacro(EnableExcludeValue, bool);

            itkSetMacro(EnableFirstOrderStats, bool);
            itkGetMacro(EnableFirstOrderStats, bool);

            itkSetMacro(EnableVariance, bool);
            itkGetMacro(EnableVariance, bool);

            itkSetMacro(UseUnbiasedEstimator, bool);
            itkGetMacro(UseUnbiasedEstimator, bool);

        protected:
            PersistentStreamingConditionalStatisticsImageFilter();

            virtual ~PersistentStreamingConditionalStatisticsImageFilter()
            {}

            /** Pass the input through unmodified. Do this by Grafting in the
             *  AllocateOutputs method.
             */
            virtual void AllocateOutputs();

            virtual void GenerateOutputInformation();

            virtual void PrintSelf(std::ostream& os, itk::Indent indent) const;

            /** Multi-thread version GenerateData. */
            void ThreadedGenerateData(const RegionType& outputRegionForThread, itk::ThreadIdType threadId);

        private:
            PersistentStreamingConditionalStatisticsImageFilter(const Self &); //purposely not implemented
            void operator =(const Self&);//purposely not implemented

            bool m_EnableMinMax;
            bool m_EnableFirstOrderStats;
            bool m_EnableVariance;
            bool m_EnableExcludeValue;

            /* use an unbiased estimator to compute the covariance */
            bool m_UseUnbiasedEstimator;

            std::vector<PixelType> m_ThreadMin;
            std::vector<PixelType> m_ThreadMax;
            std::vector<RealPixelType> m_ThreadFirstOrderAccumulators;
            std::vector<RealPixelType> m_ThreadSumOfSquaresAccumulators;

            itk::Array<unsigned long> m_NumberOfValidPixels;
            InternalPixelType m_ExcludedValue;

            /** The foreground value is the valid value in the mask */
            MaskPixelType m_MaskForegroundValue;

            /** Check is the statistics are valid */
            bool m_IsValid;

            // Enable/Disable MaskInput
            bool m_UseMaskInput;

        }; // end of class PersistentStreamingConditionalStatisticsImageFilter

        /**===========================================================================*/

        /** \class StreamingConditionalStatisticsImageFilter
         * \brief This class streams the whole input image through the PersistentStatisticsImageFilter.
         *
         * This way, it allows to compute the first and second order global statistics of this image. It calls the
         * Reset() method of the PersistentStreamingConditionalStatisticsImageFilter before streaming the image and the
         * Synthetize() method of the PersistentStreamingConditionalStatisticsImageFilter after having streamed the image
         * to compute the statistics. The accessor on the results are wrapping the accessors of the
         * internal PersistentStreamingConditionalStatisticsImageFilter.
         *
         * \sa PersistentStreamingConditionalStatisticsImageFilter
         * \sa PersistentImageFilter
         * \sa PersistentFilterStreamingDecorator
         * \sa StreamingImageVirtualWriter
         * \ingroup Streamed
         * \ingroup Multithreaded
         * \ingroup MathematicalStatisticsImageFilters
         */

    template<class TInputImage, class TInputMask = otb::Image<unsigned char>, class TPrecision = typename itk::NumericTraits<
            typename TInputImage::InternalPixelType>::RealType>
        class StreamingConditionalStatisticsImageFilter : public otb::PersistentFilterStreamingDecorator<
                PersistentStreamingConditionalStatisticsImageFilter<TInputImage, TInputMask, TPrecision> >
        {
        public:
            /** Standard Self typedef */
            typedef StreamingConditionalStatisticsImageFilter Self;
            typedef otb::PersistentFilterStreamingDecorator<
                    PersistentStreamingConditionalStatisticsImageFilter<TInputImage, TInputMask, TPrecision> > Superclass;
            typedef itk::SmartPointer<Self> Pointer;
            typedef itk::SmartPointer<const Self> ConstPointer;

            /** Type macro */
            itkNewMacro(Self)
            ;

            /** Creation through object factory macro */
            itkTypeMacro(StreamingConditionalStatisticsImageFilter, PersistentFilterStreamingDecorator)
            ;

            typedef TInputImage InputImageType;
            typedef typename Superclass::FilterType StatFilterType;

            typedef typename TInputMask::PixelType MaskPixelType;

            /** Type of DataObjects used for outputs */
            typedef typename StatFilterType::PixelType PixelType;
            typedef typename StatFilterType::PixelObjectType PixelObjectType;
            typedef typename StatFilterType::InternalPixelType InternalPixelType;
            typedef typename StatFilterType::RealType RealType;
            typedef typename StatFilterType::RealObjectType RealObjectType;
            typedef typename StatFilterType::RealPixelType RealPixelType;
            typedef typename StatFilterType::RealPixelObjectType RealPixelObjectType;

            // Used to count the number of valid pixels
            typedef typename StatFilterType::UIObjectType UIObjectType;

            void
            SetInput(InputImageType * input)
            {
                this->GetFilter()->SetInput(input);
            }
            void
            SetInput(const InputImageType * input)
            {
                this->GetFilter()->SetInput(input);
            }
            const InputImageType *
            GetInput()
            {
                return this->GetFilter()->GetInput();
            }

            /** Set the mask */
            void
            SetMaskInput(const TInputMask * lMask)
            {
                this->GetFilter()->SetMaskInput(const_cast<TInputMask *>(lMask));
            }
            void
            SetMaskInput(TInputMask * lMask)
            {
                this->GetFilter()->SetMaskInput(const_cast<TInputMask *>(lMask));
            }

            /** Return the computed Mean. */
            PixelType
            GetMinimum() const
            {
                return this->GetFilter()->GetMinimumOutput()->Get();
            }
            PixelObjectType*
            GetMinimumOutput()
            {
                return this->GetFilter()->GetMinimumOutput();
            }
            const PixelObjectType*
            GetMinimumOutput() const
            {
                return this->GetFilter()->GetMinimumOutput();
            }

            /** Return the computed Mean. */
            PixelType
            GetMaximum() const
            {
                return this->GetFilter()->GetMaximumOutput()->Get();
            }
            PixelObjectType*
            GetMaximumOutput()
            {
                return this->GetFilter()->GetMaximumOutput();
            }
            const PixelObjectType*
            GetMaximumOutput() const
            {
                return this->GetFilter()->GetMaximumOutput();
            }

            /** Return the computed Mean. */
            RealPixelType
            GetMean() const
            {
                return this->GetFilter()->GetMeanOutput()->Get();
            }
            RealPixelObjectType*
            GetMeanOutput()
            {
                return this->GetFilter()->GetMeanOutput();
            }
            const RealPixelObjectType*
            GetMeanOutput() const
            {
                return this->GetFilter()->GetMeanOutput();
            }

            /** Return the computed Mean. */
            RealPixelType
            GetSum() const
            {
                return this->GetFilter()->GetSumOutput()->Get();
            }
            RealPixelObjectType*
            GetSumOutput()
            {
                return this->GetFilter()->GetSumOutput();
            }
            const RealPixelObjectType*
            GetSumOutput() const
            {
                return this->GetFilter()->GetSumOutput();
            }

            /** Return the computed Mean. */
            RealPixelType
            GetVariance() const
            {
                return this->GetFilter()->GetVarianceOutput()->Get();
            }
            RealPixelObjectType*
            GetVarianceOutput()
            {
                return this->GetFilter()->GetVarianceOutput();
            }
            const RealPixelObjectType*
            GetVarianceOutput() const
            {
                return this->GetFilter()->GetVarianceOutput();
            }

            /** Return the computed Mean. */
            RealPixelType
            GetStandardDeviation() const
            {
                return this->GetFilter()->GetStandardDeviationOutput()->Get();
            }
            RealPixelObjectType*
            GetStandardDeviationOutput()
            {
                return this->GetFilter()->GetStandardDeviationOutput();
            }
            const RealPixelObjectType*
            GetStandardDeviationOutput() const
            {
                return this->GetFilter()->GetStandardDeviationOutput();
            }

            /** Set the excluded value of the filter.*/
            void
            SetExcludedValue(const InternalPixelType val)
            {
                this->GetFilter()->SetExcludedValue(val);
            }
            /** Get the excluded value of the filter.*/
            InternalPixelType
            GetExcludedValue() const
            {
                return this->GetFilter()->GetExcludedValue();
            }
            /** Set the foreground (valid) value mask */
            void
            SetMaskForegroundValue(const MaskPixelType & lMaskVal)
            {
                this->GetFilter()->SetMaskForegroundValue(lMaskVal);
            }

            /** Get the foreground (valid) value mask */
            MaskPixelType
            GetMaskForegroundValue()
            {
                return this->GetFilter()->GetMaskForegroundValue();
            }

            /** Return the number of valid values. */
            unsigned long
            GetNbOfValidValues() const
            {
                return this->GetFilter()->GetNbOfValidValuesOutput()->Get();
            }
            UIObjectType*
            GetNbOfValidValuesOutput()
            {
                return this->GetFilter()->GetNbOfValidValues();
            }
            const UIObjectType*
            GetNbOfValidValueOutput() const
            {
                return this->GetFilter()->GetNbOfValidValues();
            }

            /** Return the number of excluded values. */
            unsigned int
            GetNbOfExcludedValues() const
            {
                return this->GetFilter()->GetNbOfExcludedValuesOutput()->Get();
            }
            UIObjectType*
            GetNbOfExcludedValuesOutput()
            {
                return this->GetFilter()->GetNbOfExcludedValues();
            }
            const UIObjectType*
            GetNbOfExcludedValueOutput() const
            {
                return this->GetFilter()->GetNbOfExcludedValues();
            }

            bool
            GetIsValid() const
            {
                return this->GetFilter()->GetIsValid();
            }

            otbSetObjectMemberMacro(Filter, EnableMinMax, bool)
            ;otbGetObjectMemberMacro(Filter, EnableMinMax, bool)
            ;

            otbSetObjectMemberMacro(Filter, EnableVariance, bool)
            ;otbGetObjectMemberMacro(Filter, EnableVariance, bool)
            ;

            otbSetObjectMemberMacro(Filter, EnableExcludeValue, bool)
            ;otbGetObjectMemberMacro(Filter, EnableExcludeValue, bool)
            ;

            otbSetObjectMemberMacro(Filter, EnableFirstOrderStats, bool)
            ;otbGetObjectMemberMacro(Filter, EnableFirstOrderStats, bool)
            ;

            otbSetObjectMemberMacro(Filter, UseUnbiasedEstimator, bool)
            ;otbGetObjectMemberMacro(Filter, UseUnbiasedEstimator, bool)
            ;

        protected:
            /** Constructor */
            StreamingConditionalStatisticsImageFilter()
            {
            }

            /** Destructor */
            virtual
            ~StreamingConditionalStatisticsImageFilter()
            {
            }

        private:
            StreamingConditionalStatisticsImageFilter(const Self &); //purposely not implemented
            void
            operator =(const Self&); //purposely not implemented

            virtual void
            PrintSelf(std::ostream& os, itk::Indent indent) const
            {
                os << indent << this->GetFilter();
            }

        };

}
// end namespace otb

#ifndef VNS_MANUAL_INSTANTIATION
#include "vnsStreamingConditionalStatisticsImageFilter.txx"
#endif

#endif /* __vnsStreamingConditionalStatisticsImageFilter_h */
