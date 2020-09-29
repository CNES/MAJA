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
 * VERSION : 2.0.0 : DM : LAIG-DM-MAJA-2149-CNES : 24 fevrier 2017 : Correction Cirrus - création           *
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id$
 *                                                                                                          *
 ************************************************************************************************************/
#ifndef __vnsCirrusMultiMeanStatisticsVectorImageFilter_h
#define __vnsCirrusMultiMeanStatisticsVectorImageFilter_h

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

    /** \class CirrusMultiMeanStatisticsVectorImageFilter
     * \brief Compute the various mean and pixel count using streaming excluding
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
    template<class TInputImage1, class TInputImage2, class TInputMask, class TPrecision>
    class PersistentCirrusMultiMeanStatisticsVectorImageFilter : public otb::PersistentImageFilter<TInputImage1,
    TInputImage1>
    {
    public:
        /** Standard Self typedef */
        typedef PersistentCirrusMultiMeanStatisticsVectorImageFilter Self;
        typedef otb::PersistentImageFilter<TInputImage1, TInputImage1> Superclass;
        typedef itk::SmartPointer<Self> Pointer;
        typedef itk::SmartPointer<const Self> ConstPointer;

        /** Method for creation through the object factory. */
        itkNewMacro(Self)

        /** Runtime information support. */
        itkTypeMacro(PersistentCirrusMultiMeanStatisticsVectorImageFilter, PersistentImageFilter)

        /** Image related typedefs. */
        typedef TInputImage1 ImageType;
        typedef typename ImageType::Pointer InputImagePointer;
        typedef typename ImageType::RegionType RegionType;
        typedef typename ImageType::SizeType SizeType;
        typedef typename ImageType::IndexType IndexType;
        typedef typename ImageType::PixelType PixelType;
        typedef typename ImageType::InternalPixelType InternalPixelType;
        typedef TInputImage2 L1ImageType;
        typedef typename L1ImageType::Pointer L1InputImagePointer;
        typedef typename L1ImageType::RegionType L1RegionType;
        typedef typename L1ImageType::SizeType L1SizeType;
        typedef typename L1ImageType::IndexType L1IndexType;
        typedef typename L1ImageType::PixelType L1PixelType;
        typedef typename L1ImageType::InternalPixelType L1InternalPixelType;

        typedef TInputMask MaskType;
        typedef typename TInputMask::Pointer InputMaskPointer;
        typedef typename TInputMask::PixelType MaskPixelType;

        typedef TPrecision PrecisionType;
        typedef PrecisionType RealType;

        /** Image related typedefs. */
        itkStaticConstMacro(ImageDimension, unsigned int, TInputImage1::ImageDimension);

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

        /** Set the input cirrus mask*/
        vnsSetGetInputMacro(CirrusMask, MaskType, 1)

        /** Set the input thin mask*/
        vnsSetGetInputMacro(ThinCirrusMask, MaskType, 2)

        /** Set the input not cirrus mask*/
        vnsSetGetInputMacro(NonCirrusMask, MaskType, 3)

        /** Set the input L1TOA cirrus band */
        vnsSetGetInputMacro(L1TOACirrus,L1ImageType, 4)

        itkSetMacro(CirrusForegroundValue, MaskPixelType);
        itkGetMacro(CirrusForegroundValue, MaskPixelType);

        itkSetMacro(ThinCirrusForegroundValue, MaskPixelType);
        itkGetMacro(ThinCirrusForegroundValue, MaskPixelType);

        itkSetMacro(NonCirrusForegroundValue, MaskPixelType);
        itkGetMacro(NonCirrusForegroundValue, MaskPixelType);

        /** Return the computed Mean. */
        RealType GetL1ThinCirrusMean() const
        {
            return this->GetL1ThinCirrusMeanOutput()->Get();
        }
        RealObjectType* GetL1ThinCirrusMeanOutput();
        const RealObjectType* GetL1ThinCirrusMeanOutput() const;

        RealPixelType GetL2ThinCirrusMean() const
        {
            return this->GetL2ThinCirrusMeanOutput()->Get();
        }
        RealPixelObjectType* GetL2ThinCirrusMeanOutput();
        const RealPixelObjectType* GetL2ThinCirrusMeanOutput() const;

        RealPixelType GetL2NonCirrusMean() const
        {
            return this->GetL2NonCirrusMeanOutput()->Get();
        }
        RealPixelObjectType* GetL2NonCirrusMeanOutput();
        const RealPixelObjectType* GetL2NonCirrusMeanOutput() const;

        /** Return the number of valid values. */
        unsigned int GetNbOfValidCirrusValues() const
        {
            return this->GetNbOfValidCirrusValuesOutput()->Get();
        }
        UIObjectType* GetNbOfValidCirrusValuesOutput();
        const UIObjectType* GetNbOfValidCirrusValuesOutput() const;

        unsigned int GetNbOfValidThinCirrusValues() const
        {
            return this->GetNbOfValidThinCirrusValuesOutput()->Get();
        }
        UIObjectType* GetNbOfValidThinCirrusValuesOutput();
        const UIObjectType* GetNbOfValidThinCirrusValuesOutput() const;

        unsigned int GetNbOfValidNonCirrusValues() const
        {
            return this->GetNbOfValidNonCirrusValuesOutput()->Get();
        }
        UIObjectType* GetNbOfValidNonCirrusValuesOutput();
        const UIObjectType* GetNbOfValidNonCirrusValuesOutput() const;

        /** Check the validity of the statistics */
        itkGetConstMacro(IsValid,bool)

        /** Make a DataObject of the correct type to be used as the specified
         * output.
         */
        virtual DataObjectPointer MakeOutput(unsigned int idx);

        virtual void Reset(void);

        virtual void Synthetize(void);

    protected:
        PersistentCirrusMultiMeanStatisticsVectorImageFilter();

        virtual ~PersistentCirrusMultiMeanStatisticsVectorImageFilter()
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
        PersistentCirrusMultiMeanStatisticsVectorImageFilter(const Self &); //purposely not implemented
        void operator =(const Self&);//purposely not implemented

        /* use an unbiased estimator to compute the covariance */
        std::vector<RealType>      m_ThreadFirstOrderL1ThinCirrusAccumulators;
        std::vector<RealPixelType> m_ThreadFirstOrderL2ThinCirrusAccumulators;
        std::vector<RealPixelType> m_ThreadFirstOrderL2NonCirrusAccumulators;

        /** Number of valid pixels per thread */
        itk::Array<long> m_NumberOfCirrusValidPixels;
        itk::Array<long> m_NumberOfThinCirrusValidPixels;
        itk::Array<long> m_NumberOfNonCirrusValidPixels;
        /** The foreground value is the valid value in the mask */
        MaskPixelType m_CirrusForegroundValue;
        MaskPixelType m_ThinCirrusForegroundValue;
        MaskPixelType m_NonCirrusForegroundValue;
        /** Check is the statistics are valid */
        bool m_IsValid;

    }; // end of class PersistentCirrusMultiMeanStatisticsVectorImageFilter

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

    template<class TInputImage1, class TInputImage2, class TInputMask,
    class TPrecision = typename itk::NumericTraits<typename TInputImage1::InternalPixelType>::RealType>
    class CirrusMultiMeanStatisticsVectorImageFilter : public otb::PersistentFilterStreamingDecorator<
    PersistentCirrusMultiMeanStatisticsVectorImageFilter<TInputImage1,TInputImage2, TInputMask, TPrecision> >
    {
    public:
        /** Standard Self typedef */
        typedef CirrusMultiMeanStatisticsVectorImageFilter Self;
        typedef otb::PersistentFilterStreamingDecorator<
                PersistentCirrusMultiMeanStatisticsVectorImageFilter<TInputImage1, TInputImage2, TInputMask, TPrecision> > Superclass;
        typedef itk::SmartPointer<Self> Pointer;
        typedef itk::SmartPointer<const Self> ConstPointer;

        /** Type macro */
        itkNewMacro(Self)

        /** Creation through object factory macro */
        itkTypeMacro(CirrusMultiMeanStatisticsVectorImageFilter, PersistentFilterStreamingDecorator)

        typedef TInputImage1 InputImageType;
        typedef TInputImage2 L1InputImageType;
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
        SetCirrusMaskInput(const TInputMask * lMask)
        {
            this->GetFilter()->SetCirrusMaskInput(const_cast<TInputMask *>(lMask));
        }

        void
        SetThinCirrusMaskInput(const TInputMask * lMask)
        {
            this->GetFilter()->SetThinCirrusMaskInput(const_cast<TInputMask *>(lMask));
        }

        void
        SetNonCirrusMaskInput(const TInputMask * lMask)
        {
            this->GetFilter()->SetNonCirrusMaskInput(const_cast<TInputMask *>(lMask));
        }

        /** Set L1TOA cirrus image*/
        void
        SetL1TOACirrusInput(L1InputImageType * input)
        {
            this->GetFilter()->SetL1TOACirrusInput(input);
        }

        /** Set the foreground (valid) value mask */
        void
        SetCirrusForegroundValue(const MaskPixelType & lMaskVal)
        {
            this->GetFilter()->SetCirrusForegroundValue(lMaskVal);
        }

        /** Get the foreground (valid) value mask */
        MaskPixelType
        GetCirrusForegroundValue()
        {
            return this->GetFilter()->GetCirrusForegroundValue();
        }

        /** Set the foreground (valid) value mask */
        void
        SetThinCirrusForegroundValue(const MaskPixelType & lMaskVal)
        {
            this->GetFilter()->SetThinCirrusForegroundValue(lMaskVal);
        }

        /** Get the foreground (valid) value mask */
        MaskPixelType
        GetThinCirrusForegroundValue()
        {
            return this->GetFilter()->GetThinCirrusForegroundValue();
        }
        /** Set the foreground (valid) value mask */
        void
        SetNonCirrusForegroundValue(const MaskPixelType & lMaskVal)
        {
            this->GetFilter()->SetNonCirrusForegroundValue(lMaskVal);
        }

        /** Get the foreground (valid) value mask */
        MaskPixelType
        GetNonCirrusForegroundValue()
        {
            return this->GetFilter()->GetNonCirrusForegroundValue();
        }

        /** Return the computed Mean. */
        RealType
        GetL1ThinCirrusMean() const
        {
            return this->GetFilter()->GetL1ThinCirrusMeanOutput()->Get();
        }
        RealObjectType*
        GetL1ThinCirrusMeanOutput()
        {
            return this->GetFilter()->GetL1ThinCirrusMeanOutput();
        }
        const RealObjectType*
        GetL1ThinCirrusMeanOutput() const
        {
            return this->GetFilter()->GetL1ThinCirrusMeanOutput();
        }

        RealPixelType
        GetL2ThinCirrusMean() const
        {
            return this->GetFilter()->GetL2ThinCirrusMeanOutput()->Get();
        }
        RealPixelObjectType*
        GetL2ThinCirrusMeanOutput()
        {
            return this->GetFilter()->GetL2ThinCirrusMeanOutput();
        }
        const RealPixelObjectType*
        GetL2ThinCirrusMeanOutput() const
        {
            return this->GetFilter()->GetL2ThinCirrusMeanOutput();
        }

        RealPixelType
        GetL2NonCirrusMean() const
        {
            return this->GetFilter()->GetL2NonCirrusMeanOutput()->Get();
        }
        RealPixelObjectType*
        GetL2NonCirrusMeanOutput()
        {
            return this->GetFilter()->GetL2NonCirrusMeanOutput();
        }
        const RealPixelObjectType*
        GetL2NonCirrusMeanOutput() const
        {
            return this->GetFilter()->GetL2NonCirrusMeanOutput();
        }

        /** Return the number of valid values. */
        unsigned int
        GetNbOfValidCirrusValues() const
        {
            return this->GetFilter()->GetNbOfValidCirrusValuesOutput()->Get();
        }
        UIObjectType*
        GetNbOfValidCirrusValuesOutput()
        {
            return this->GetFilter()->GetNbOfValidCirrusValues();
        }
        const UIObjectType*
        GetNbOfValidCirrusValueOutput() const
        {
            return this->GetFilter()->GetNbOfValidCirrusValues();
        }
        unsigned int
        GetNbOfValidThinCirrusValues() const
        {
            return this->GetFilter()->GetNbOfValidThinCirrusValuesOutput()->Get();
        }
        UIObjectType*
        GetNbOfValidThinCirrusValuesOutput()
        {
            return this->GetFilter()->GetNbOfValidThinCirrusValues();
        }
        const UIObjectType*
        GetNbOfValidThinCirrusValueOutput() const
        {
            return this->GetFilter()->GetNbOfValidThinCirrusValues();
        }
        unsigned int
        GetNbOfValidNonCirrusValues() const
        {
            return this->GetFilter()->GetNbOfValidNonCirrusValuesOutput()->Get();
        }
        UIObjectType*
        GetNbOfValidNonCirrusValuesOutput()
        {
            return this->GetFilter()->GetNbOfValidNonCirrusValues();
        }
        const UIObjectType*
        GetNbOfValidNonCirrusValueOutput() const
        {
            return this->GetFilter()->GetNbOfValidNonCirrusValues();
        }


        bool
        GetIsValid() const
        {
            return this->GetFilter()->GetIsValid();
        }

    protected:
        /** Constructor */
        CirrusMultiMeanStatisticsVectorImageFilter()
    {
    }

        /** Destructor */
        virtual
        ~CirrusMultiMeanStatisticsVectorImageFilter()
        {
        }

    private:
        CirrusMultiMeanStatisticsVectorImageFilter(const Self &); //purposely not implemented
        void
        operator =(const Self&); //purposely not implemented

    };

} // end namespace otb

#ifndef VNS_MANUAL_INSTANTIATION
#include <vnsCirrusMultiMeanStatisticsVectorImageFilter.txx>
#endif

#endif /* __vnsMultiMeanStatisticsVectorImageFilter_h */
