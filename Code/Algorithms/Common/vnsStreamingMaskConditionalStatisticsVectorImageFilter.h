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
 * VERSION : 5-1-0 : DM : LAIG-DM-MAC-1769-CNES : 6 juillet 2016 : Implémentation écart type checktools     *
 * VERSION : 5-1-0 : FA : LAIG-FA-MAC-145739-CS : 27 juin 2016 : Audit code - Supp de la macro ITK_EXPORT   *
 * VERSION : 5-1-0 : FA : LAIG-FA-MAC-144719-CS : 17 mai 2016 : Correction warning et contrôle qualité      *
 * VERSION : 3-2-0 : DM : LAIG-DM-MAC-697-CNES : 8 novembre 2013 : Mise à jour du plugin SENTINEL2          *
 * 					Action interne 98664. Prise en compte  des modifications apportées à la classe   		*
 * 					otbStreamingStatisticsVectorImageFilter v3.13   										*
 * VERSION : 1-0-0-3 : FA : 65300 : 21 sept. 2011 : Correction pour mise en forme qualité                     *
 * VERSION : 1-0-0 : <TypeFT> : <NumFT> : 19 avr. 2010 : Creation
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id$
 *                                                                                                          *
 ************************************************************************************************************/
#ifndef __vnsStreamingMaskValueConditionalStatisticsVectorImageFilter_h
#define __vnsStreamingMaskValueConditionalStatisticsVectorImageFilter_h

#include "otbPersistentImageFilter.h"
#include "otbPersistentFilterStreamingDecorator.h"

#include "itkNumericTraits.h"
#include "itkArray.h"
#include "itkSimpleDataObjectDecorator.h"
#include "itkImageRegionSplitter.h"
#include "itkVariableSizeMatrix.h"
#include "itkVariableLengthVector.h"
#include "vnsMacro.h"

namespace vns
{

    /** \class PersistentStreamingMaskConditionalStatisticsVectorImageFilter
     * \brief Compute covariance & correlation of a large image using streaming excluding
     * the pixels equals to 1 in the input mask.
     *
     *  This filter persists its temporary data. It means that if you Update it n times on n different
     * requested regions, the output statistics will be the statistics of the whole set of n regions.
     *
     * To reset the temporary data, one should call the Reset() function.
     *
     * To get the statistics once the regions have been processed via the pipeline, use the Synthetize() method.
     *
     * \sa PersistentImageFilter
     * \sa PersistentConditionalStatisticsVectorImageFilter
     *
     * \ingroup L2
     *
     */
    template<class TInputImage, class TInputMask, class TPrecision>
    class PersistentStreamingMaskConditionalStatisticsVectorImageFilter : public otb::PersistentImageFilter<TInputImage,
    TInputImage>
    {
    public:
        /** Standard Self typedef */
        typedef PersistentStreamingMaskConditionalStatisticsVectorImageFilter Self;
        typedef otb::PersistentImageFilter<TInputImage, TInputImage> Superclass;
        typedef itk::SmartPointer<Self> Pointer;
        typedef itk::SmartPointer<const Self> ConstPointer;

        /** Method for creation through the object factory. */
        itkNewMacro(Self)

        /** Runtime information support. */
        itkTypeMacro(PersistentStreamingMaskConditionalStatisticsVectorImageFilter, PersistentImageFilter)

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
        typedef itk::VariableSizeMatrix<PrecisionType> MatrixType;
        typedef itk::VariableLengthVector<PrecisionType> RealPixelType;

        /** Type of DataObjects used for outputs */
        typedef itk::SimpleDataObjectDecorator<RealType> RealObjectType;
        typedef itk::SimpleDataObjectDecorator<IndexType> IndexObjectType;
        typedef itk::SimpleDataObjectDecorator<PixelType> PixelObjectType;
        typedef itk::SimpleDataObjectDecorator<RealPixelType> RealPixelObjectType;
        typedef itk::SimpleDataObjectDecorator<MatrixType> MatrixObjectType;

        // Used to count the number of valid pixels
        typedef itk::SimpleDataObjectDecorator<unsigned int> UIObjectType;

        /** Set the input mask*/
        vnsSetGetInputMacro(Mask, MaskType, 1)

        itkSetMacro(ForegroundValue, MaskPixelType);
        itkGetMacro(ForegroundValue, MaskPixelType);

        itkSetMacro(ExcludeValue, RealType);
        itkGetMacro(ExcludeValue, RealType);

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

        /** Return the global mean of all the internal pixel values
         * (flattening the multispectral image as a 1D-vector) */
        RealType GetComponentMean() const
        {
            return this->GetComponentMeanOutput()->Get();
        }
        RealObjectType* GetComponentMeanOutput();
        const RealObjectType* GetComponentMeanOutput() const;

        /** Return the global mean of all the internal pixel values
         * (flattening the multispectral image as a 1D-vector) */
        RealType GetComponentStandardDeviation() const
        {
            return this->GetComponentStandardDeviationOutput()->Get();
        }
        RealObjectType* GetComponentStandardDeviationOutput();
        const RealObjectType* GetComponentStandardDeviationOutput() const;


        /** Return the global correlation of all the internal pixel values
         * (flattening the multispectral image as a 1D-vector) */
        RealType GetComponentCorrelation() const
        {
            return this->GetComponentCorrelationOutput()->Get();
        }
        RealObjectType* GetComponentCorrelationOutput();
        const RealObjectType* GetComponentCorrelationOutput() const;

        /** Return the global covariance of all the internal pixel values
         * (flattening the multispectral image as a 1D-vector) */
        RealType GetComponentCovariance() const
        {
            return this->GetComponentCovarianceOutput()->Get();
        }
        RealObjectType* GetComponentCovarianceOutput();
        const RealObjectType* GetComponentCovarianceOutput() const;

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

        /** Return the computed Standard deviation. */
        RealPixelType GetStandardDeviation() const
        {
            return this->GetStandardDeviationOutput()->Get();
        }
        RealPixelObjectType* GetStandardDeviationOutput();
        const RealPixelObjectType* GetStandardDeviationOutput() const;


        /** Return the computed Covariance. */
        MatrixType GetCorrelation() const
        {
            return this->GetCorrelation()->Get();
        }
        MatrixObjectType* GetCorrelationOutput();
        const MatrixObjectType* GetCorrelationOutput() const;

        /** Return the computed Covariance. */
        MatrixType GetCovariance() const
        {
            return this->GetCovarianceOutput()->Get();
        }
        MatrixObjectType* GetCovarianceOutput();
        const MatrixObjectType* GetCovarianceOutput() const;

        /** Return the number of valid values. */
        unsigned int GetNbOfValidValues() const
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

        /** Return the number of valid values  per band. */
        RealPixelType GetNbOfValidValuesPerBand() const
        {
            return this->GetNbOfValidValuesPerBandOutput()->Get();
        }
        RealPixelObjectType* GetNbOfValidValuesPerBandOutput();
        const RealPixelObjectType* GetNbOfValidValuesPerBandOutput() const;

        /** Return the number of excluded values per band. */
        RealPixelType GetNbOfExcludedValuesPerBand() const
        {
            return this->GetNbOfExcludedValuesPerBandOutput()->Get();
        }
        RealPixelObjectType* GetNbOfExcludedValuesPerBandOutput();
        const RealPixelObjectType* GetNbOfExcludedValuesPerBandOutput() const;


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

        itkSetMacro(EnableStandardDeviation, bool);
        itkGetMacro(EnableStandardDeviation, bool);

        itkSetMacro(EnableSecondOrderStats, bool);
        itkGetMacro(EnableSecondOrderStats, bool);

        itkSetMacro(UseUnbiasedEstimator, bool);
        itkGetMacro(UseUnbiasedEstimator, bool);

    protected:
        PersistentStreamingMaskConditionalStatisticsVectorImageFilter();

        virtual ~PersistentStreamingMaskConditionalStatisticsVectorImageFilter()
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
        PersistentStreamingMaskConditionalStatisticsVectorImageFilter(const Self &); //purposely not implemented
        void operator =(const Self&);//purposely not implemented

        bool m_EnableMinMax;
        bool m_EnableExcludeValue;
        bool m_EnableFirstOrderStats;
        bool m_EnableStandardDeviation;
        bool m_EnableSecondOrderStats;

        /* use an unbiased estimator to compute the covariance */
        bool m_UseUnbiasedEstimator;
        RealType m_ExcludeValue;
        std::vector<PixelType> m_ThreadMin;
        std::vector<PixelType> m_ThreadMax;
        std::vector<RealType> m_ThreadFirstOrderComponentAccumulators;
        std::vector<RealType> m_ThreadFirstOrderSquareComponentAccumulators;
        std::vector<RealType> m_ThreadSecondOrderComponentAccumulators;
        std::vector<RealPixelType> m_ThreadFirstOrderAccumulators;
        std::vector<RealPixelType> m_ThreadFirstOrderSquareAccumulators;
        std::vector<MatrixType> m_ThreadSecondOrderAccumulators;

        /** Number of valid pixels per thread */
        itk::Array<long> m_NumberOfValidPixels;
        /** Number of valid pixels per thread */
        std::vector<RealPixelType> m_NumberOfValidPixelsPerBand;
        /** The foreground value is the valid value in the mask */
        MaskPixelType m_ForegroundValue;
        /** Check is the statistics are valid */
        bool m_IsValid;

    }; // end of class PersistentStreamingMaskConditionalStatisticsVectorImageFilter

    /**===========================================================================*/

    /** \class StreamingMaskConditionalStatisticsVectorImageFilter
     * \brief This class streams the whole input image through the PersistentStatisticsImageFilter.
     *
     * This way, it allows to compute the first and second order global statistics of this image. It calls the
     * Reset() method of the PersistentStreamingMaskConditionalStatisticsVectorImageFilter before streaming the image and the
     * Synthetize() method of the PersistentStreamingMaskConditionalStatisticsVectorImageFilter after having streamed the image
     * to compute the statistics. The accessor on the results are wrapping the accessors of the
     * internal PersistentStreamingMaskConditionalStatisticsVectorImageFilter.
     *
     * \sa PersistentStreamingMaskConditionalStatisticsVectorImageFilter
     * \sa PersistentImageFilter
     * \sa PersistentFilterStreamingDecorator
     * \sa StreamingImageVirtualWriter
     * \ingroup Streamed
     * \ingroup Multithreaded
     * \ingroup MathematicalStatisticsImageFilters
     */

    template<class TInputImage, class TInputMask,
    class TPrecision = typename itk::NumericTraits<typename TInputImage::InternalPixelType>::RealType>
    class StreamingMaskConditionalStatisticsVectorImageFilter : public otb::PersistentFilterStreamingDecorator<
    PersistentStreamingMaskConditionalStatisticsVectorImageFilter<TInputImage, TInputMask, TPrecision> >
    {
    public:
        /** Standard Self typedef */
        typedef StreamingMaskConditionalStatisticsVectorImageFilter Self;
        typedef otb::PersistentFilterStreamingDecorator<
                PersistentStreamingMaskConditionalStatisticsVectorImageFilter<TInputImage, TInputMask, TPrecision> > Superclass;
        typedef itk::SmartPointer<Self> Pointer;
        typedef itk::SmartPointer<const Self> ConstPointer;

        /** Type macro */
        itkNewMacro(Self)

        /** Creation through object factory macro */
        itkTypeMacro(StreamingMaskConditionalStatisticsVectorImageFilter, PersistentFilterStreamingDecorator)

        typedef TInputImage InputImageType;
        typedef typename Superclass::FilterType StatFilterType;

        typedef typename TInputMask::PixelType MaskPixelType;

        /** Type of DataObjects used for outputs */
        typedef typename StatFilterType::PixelType PixelType;
        typedef typename StatFilterType::InternalPixelType InternalPixelType;
        typedef typename StatFilterType::RealType RealType;
        typedef typename StatFilterType::RealObjectType RealObjectType;
        typedef typename StatFilterType::RealPixelType RealPixelType;
        typedef typename StatFilterType::RealPixelObjectType RealPixelObjectType;
        typedef typename StatFilterType::MatrixType MatrixType;
        typedef typename StatFilterType::MatrixObjectType MatrixObjectType;

        // Used to count the number of valid pixels
        typedef typename StatFilterType::UIObjectType UIObjectType;

        void
        SetInput(InputImageType * input)
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

        /** Set the foreground (valid) value mask */
        void
        SetForegroundValue(const MaskPixelType & lMaskVal)
        {
            this->GetFilter()->SetForegroundValue(lMaskVal);
        }

        /** Get the foreground (valid) value mask */
        MaskPixelType
        GetForegroundValue()
        {
            return this->GetFilter()->GetForegroundValue();
        }

        /** Set the exclude non valid value for image */
        void
        SetExcludeValue(const RealType & lImVal)
        {
            this->GetFilter()->SetExcludeValue(lImVal);
        }

        /** Get the foreground (valid) value mask */
        RealType
        GetExcludeValue()
        {
            return this->GetFilter()->GetExcludeValue();
        }

        /** Return the computed Mean. */
        RealPixelType
        GetMinimum() const
        {
            return this->GetFilter()->GetMinimumOutput()->Get();
        }
        RealPixelObjectType*
        GetMinimumOutput()
        {
            return this->GetFilter()->GetMinimumOutput();
        }
        const RealPixelObjectType*
        GetMinimumOutput() const
        {
            return this->GetFilter()->GetMinimumOutput();
        }

        /** Return the computed Mean. */
        RealPixelType
        GetMaximum() const
        {
            return this->GetFilter()->GetMaximumOutput()->Get();
        }
        RealPixelObjectType*
        GetMaximumOutput()
        {
            return this->GetFilter()->GetMaximumOutput();
        }
        const RealPixelObjectType*
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

        /** Return the computed Standard deviation. */
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

        /** Return the computed Covariance. */
        MatrixType
        GetCovariance() const
        {
            return this->GetFilter()->GetCovarianceOutput()->Get();
        }
        MatrixObjectType*
        GetCovarianceOutput()
        {
            return this->GetFilter()->GetCovarianceOutput();
        }
        const MatrixObjectType*
        GetCovarianceOutput() const
        {
            return this->GetFilter()->GetCovarianceOutput();
        }

        /** Return the computed Covariance. */
        MatrixType
        GetCorrelation() const
        {
            return this->GetFilter()->GetCorrelationOutput()->Get();
        }
        MatrixObjectType*
        GetCorrelationOutput()
        {
            return this->GetFilter()->GetCorrelationOutput();
        }
        const MatrixObjectType*
        GetCorrelationOutput() const
        {
            return this->GetFilter()->GetCorrelationOutput();
        }

        /** Return the computed Mean. */
        RealType
        GetComponentMean() const
        {
            return this->GetFilter()->GetComponentMeanOutput()->Get();
        }
        RealObjectType*
        GetComponentMeanOutput()
        {
            return this->GetFilter()->GetComponentMeanOutput();
        }
        const RealObjectType*
        GetComponentMeanOutput() const
        {
            return this->GetFilter()->GetComponentMeanOutput();
        }

        /** Return the computed Standard deviation. */
        RealType
        GetComponentStandardDeviation() const
        {
            return this->GetFilter()->GetComponentStandardDeviationOutput()->Get();
        }
        RealObjectType*
        GetComponentStandardDeviationOutput()
        {
            return this->GetFilter()->GetComponentStandardDeviationOutput();
        }
        const RealObjectType*
        GetComponentStandardDeviationOutput() const
        {
            return this->GetFilter()->GetComponentStandardDeviationOutput();
        }

        /** Return the computed Covariance. */
        RealType
        GetComponentCovariance() const
        {
            return this->GetFilter()->GetComponentCovarianceOutput()->Get();
        }
        RealObjectType*
        GetComponentCovarianceOutput()
        {
            return this->GetFilter()->GetComponentCovarianceOutput();
        }
        const RealObjectType*
        GetComponentCovarianceOutput() const
        {
            return this->GetFilter()->GetComponentCovarianceOutput();
        }

        /** Return the computed Covariance. */
        RealType
        GetComponentCorrelation() const
        {
            return this->GetFilter()->GetComponentCorrelationOutput()->Get();
        }
        RealObjectType*
        GetComponentCorrelationOutput()
        {
            return this->GetFilter()->GetComponentCorrelationOutput();
        }
        const RealObjectType*
        GetComponentCorrelationOutput() const
        {
            return this->GetFilter()->GetComponentCorrelationOutput();
        }

        /** Return the number of valid values. */
        unsigned int
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

        /** Return the number of valid values. */
        RealPixelType
        GetNbOfValidValuesPerBand() const
        {
            return this->GetFilter()->GetNbOfValidValuesPerBandOutput()->Get();
        }
        RealPixelObjectType*
        GetNbOfValidValuesPerBandOutput()
        {
            return this->GetFilter()->GetNbOfValidValuesPerBandOutput();
        }
        const RealPixelObjectType*
        GetNbOfValidValuePerBandOutput() const
        {
            return this->GetFilter()->GetNbOfValidValuesPerBandOutput();
        }

        /** Return the number of excluded values. */
        RealPixelType
        GetNbOfExcludedValuesPerBand() const
        {
            return this->GetFilter()->GetNbOfExcludedValuesPerBandOutput()->Get();
        }
        RealPixelObjectType*
        GetNbOfExcludedValuesPerBandOutput()
        {
            return this->GetFilter()->GetNbOfExcludedValuesPerBandOutput();
        }
        const RealPixelObjectType*
        GetNbOfExcludedValuePerBandOutput() const
        {
            return this->GetFilter()->GetNbOfExcludedValuesPerBandOutput();
        }


        bool
        GetIsValid() const
        {
            return this->GetFilter()->GetIsValid();
        }

        otbSetObjectMemberMacro(Filter, EnableMinMax, bool)
        otbGetObjectMemberMacro(Filter, EnableMinMax, bool)

        otbSetObjectMemberMacro(Filter, EnableExcludeValue, bool)
        otbGetObjectMemberMacro(Filter, EnableExcludeValue, bool)

        otbSetObjectMemberMacro(Filter, EnableFirstOrderStats, bool)
        otbGetObjectMemberMacro(Filter, EnableFirstOrderStats, bool)

        otbSetObjectMemberMacro(Filter, EnableStandardDeviation, bool)
        otbGetObjectMemberMacro(Filter, EnableStandardDeviation, bool)

        otbSetObjectMemberMacro(Filter, EnableSecondOrderStats, bool)
        otbGetObjectMemberMacro(Filter, EnableSecondOrderStats, bool)

        otbSetObjectMemberMacro(Filter, UseUnbiasedEstimator, bool)
        otbGetObjectMemberMacro(Filter, UseUnbiasedEstimator, bool)

    protected:
        /** Constructor */
        StreamingMaskConditionalStatisticsVectorImageFilter()
        {
        }

        /** Destructor */
        virtual
        ~StreamingMaskConditionalStatisticsVectorImageFilter()
        {
        }

    private:
        StreamingMaskConditionalStatisticsVectorImageFilter(const Self &); //purposely not implemented
        void
        operator =(const Self&); //purposely not implemented

    };

} // end namespace otb

#ifndef VNS_MANUAL_INSTANTIATION
#include "vnsStreamingMaskConditionalStatisticsVectorImageFilter.txx"
#endif

#endif /* __vnsStreamingMaskConditionalStatisticsVectorImageFilter_h */
