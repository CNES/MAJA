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
 * VERSION : 5-1-0 : FA : LAIG-FA-MAC-145739-CS : 27 juin 2016 : Audit code - Supp de la macro ITK_EXPORT   *
 * VERSION : 4-0-0 : FA : LAIG-FA-MAC-120653-CS : 7 mai 2014 : Correction de règles qualité ou commentaires *
 * VERSION : 1-0-0-3 : DM : 331 : 04 oct. 2011 : Cirrus Flag, calcul fait à L2CoarseResolution                *
 * VERSION : 1-0-0-3 : DM : 357 : 25 juil. 2011 : Add a the computation of cloud pixels.                      *
 * VERSION : 1-0-0 : <TypeFT> : <NumFT> : 9 avr. 2010 : Creation                                                           
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id$
 *                                                                                                          *
 ************************************************************************************************************/
#ifndef __vnsCountCirrusPixelGenerator_h
#define __vnsCountCirrusPixelGenerator_h

#include "vnsMacro.h"

#include "otbPersistentImageFilter.h"
#include "otbPersistentFilterStreamingDecorator.h"

#include "itkNumericTraits.h"
#include "itkArray.h"
#include "itkArray2D.h"
#include "itkSimpleDataObjectDecorator.h"
#include "itkImageRegionSplitter.h"
#include "itkVariableSizeMatrix.h"
#include "itkVariableLengthVector.h"

namespace vns
{
    /** \class  CountCirrusPixelGenerator
     * \brief This class counts cirrus pixels. Its detects high altitude pixels
     * in the stereoscopic cloud mask contained in the level 1 product.
     *
     * The input image is an otb::Image.
     *
     * \author CS Systemes d'Information
     *
     * \sa CirrusFlagGenerator
     *
     * \ingroup L2
     *
     */
    template<class TInputImage, class TInputMask>
        class PersistentCountCirrusPixelGenerator : public otb::PersistentImageFilter<TInputImage,
                TInputImage>
        {
        public:
            /** Standard Self typedef */
            typedef PersistentCountCirrusPixelGenerator Self;
            typedef otb::PersistentImageFilter<TInputImage, TInputImage> Superclass;
            typedef itk::SmartPointer<Self> Pointer;
            typedef itk::SmartPointer<const Self> ConstPointer;

            /** Method for creation through the object factory. */
            itkNewMacro(Self)


            /** Runtime information support. */
            itkTypeMacro(PersistentCountCirrusPixelGenerator,PersistentImageFilter)


            /** Image related typedefs. */
            typedef TInputImage ImageType;
            typedef typename TInputImage::Pointer InputImagePointer;
            typedef typename TInputImage::RegionType RegionType;
            typedef typename TInputImage::SizeType SizeType;
            typedef typename TInputImage::IndexType IndexType;
            typedef typename TInputImage::PixelType PixelType;
            typedef typename TInputImage::InternalPixelType InternalPixelType;

            typedef TInputMask InputMaskType;
            typedef typename InputMaskType::Pointer InputMaskPointer;
            typedef typename InputMaskType::PixelType MaskPixelType;

            itkStaticConstMacro(InputImageDimension, unsigned int, TInputImage::ImageDimension);

            /** Image related typedefs. */
            itkStaticConstMacro(ImageDimension, unsigned int, TInputImage::ImageDimension );

            /** Type to use for computations. */
            typedef typename itk::NumericTraits<InternalPixelType>::RealType RealType;

            /** Smart Pointer type to a DataObject. */
            typedef typename itk::DataObject::Pointer DataObjectPointer;

            /** Type of DataObjects used for scalar outputs */
            typedef typename std::vector<unsigned long> ArrayULongPixelType;
            typedef itk::SimpleDataObjectDecorator<unsigned long> ULongPixelObjectType;

            /** Set the excluded value in the statistic computation. */
            itkSetMacro(AltitudeThreshold, InternalPixelType)

            /** Get the excluded value in the statistic computation. */
            itkGetConstReferenceMacro(AltitudeThreshold, InternalPixelType)


            /** Set the no_data value to detect valid pixel. */
            itkSetMacro(L1NoData, RealNoDataType)

            /** Get the no_data value to detect valid pixel. */
            itkGetConstReferenceMacro(L1NoData, RealNoDataType)

            /** Main input*/
            vnsSetGetInputMacro( Image, ImageType, 0)
			/** EdgSub input*/
            vnsSetGetInputMacro( EDGSub, InputMaskType, 1)

            /** Return the number of cirrus pixels. */
            unsigned long
            GetCirrusPixelNumber() const
            {
                return this->GetCirrusPixelNumberOutput()->Get();
            }

            ULongPixelObjectType*
            GetCirrusPixelNumberOutput();
            const ULongPixelObjectType*
            GetCirrusPixelNumberOutput() const;

            /** Return the number of valid pixels. */
            unsigned long
            GetValidPixelNumber() const
            {
                return this->GetValidPixelNumberOutput()->Get();
            }

            ULongPixelObjectType*
            GetValidPixelNumberOutput();
            const ULongPixelObjectType*
            GetValidPixelNumberOutput() const;
            
            /** Return the number of cloud pixels. */
            unsigned long
            GetCloudPixelNumber() const
            {
                return this->GetCloudPixelNumberOutput()->Get();
            }

            ULongPixelObjectType*
            GetCloudPixelNumberOutput();
            const ULongPixelObjectType*
            GetCloudPixelNumberOutput() const;

            /** Make a DataObject of the correct type to be used as the specified
             * output.
             */
            virtual DataObjectPointer
            MakeOutput(unsigned int idx);

            /** Pass the input through unmodified. Do this by Grafting in the
             *  AllocateOutputs method.
             */
            virtual void
            AllocateOutputs();
            virtual void
            GenerateOutputInformation();
            virtual void
            Synthetize(void);
            virtual void
            Reset(void);

        protected:
            PersistentCountCirrusPixelGenerator();
            virtual
            ~PersistentCountCirrusPixelGenerator()
            {
            }

            virtual void
            PrintSelf(std::ostream& os, itk::Indent indent) const;
            /** Multi-thread version GenerateData. */
            virtual void
            ThreadedGenerateData(const RegionType& outputRegionForThread, itk::ThreadIdType threadId);

        private:
            PersistentCountCirrusPixelGenerator(const Self&); //purposely not implemented
            void
            operator=(const Self&); //purposely not implemented

            ArrayULongPixelType m_ThreadCirrus;
            ArrayULongPixelType m_ThreadCloud;
            ArrayULongPixelType m_ThreadValid;

            InternalPixelType m_AltitudeThreshold;

            RealNoDataType m_L1NoData;

        }; // end of class PersistentConditionalStatisticsVectorImageFilter

    /**===========================================================================*/

    /** \class CountCirrusPixelGenerator
     * \brief This class streams the whole input image through the PersistentCountCirrusPixelGenerator.
     *
     * This way, it allows to compute the first order global statistics of this image. It calls the
     * Reset() method of the PersistentCountCirrusPixelGenerator before streaming the image and the
     * Synthetize() method of the PersistentCountCirrusPixelGenerator after having streamed the image
     * to compute the statistics. The accessor on the results are wrapping the accessors of the
     * internal PersistentCountCirrusPixelGenerator.
     *
     * \sa PersistentCountCirrusPixelGenerator
     *
     * \ingroup L2
     */

    template<class TInputImage, class TInputMask>
        class CountCirrusPixelGenerator : public otb::PersistentFilterStreamingDecorator<
                PersistentCountCirrusPixelGenerator<TInputImage,TInputMask> >
        {
        public:
            /** Standard Self typedef */
            typedef CountCirrusPixelGenerator Self;
            typedef otb::PersistentFilterStreamingDecorator<PersistentCountCirrusPixelGenerator<TInputImage,TInputMask> >
                    Superclass;
            typedef itk::SmartPointer<Self> Pointer;
            typedef itk::SmartPointer<const Self> ConstPointer;

            /** Type macro */
            itkNewMacro(Self)


            /** Creation through object factory macro */
            itkTypeMacro(CountCirrusPixelGenerator,PersistentFilterStreamingDecorator)


            typedef typename Superclass::FilterType StatFilterType;
            typedef typename StatFilterType::PixelType PixelType;
            typedef typename StatFilterType::InternalPixelType InternalPixelType;
            typedef typename StatFilterType::RealType RealType;
            typedef typename StatFilterType::ArrayULongPixelType ArrayULongPixelType;

            /** Type of DataObjects used for scalar outputs */
            typedef typename StatFilterType::ULongPixelObjectType ULongPixelObjectType;

            void
            SetInput(TInputImage * input)
            {
                this->GetFilter()->SetImageInput(input);
            }
            TInputImage *
            GetInput()
            {
                return this->GetFilter()->GetImageInput();
            }

            void
            SetEDGSub(const TInputMask * EDGMask)
            {
                this->GetFilter()->SetEDGSubInput(EDGMask);
            }

            /** Set the altitude threshold of the filter.*/
            void
            SetAltitudeThreshold(const InternalPixelType val)
            {
                this->GetFilter()->SetAltitudeThreshold(val);
            }
            /** Get the altitude threshold of the filter.*/
            InternalPixelType
            GetAltitudeThreshold() const
            {
                return this->GetFilter()->GetAltitudeThreshold();
            }

            /** Set the no_data value of L1 product.*/
            void
            SetL1NoData(const RealNoDataType val)
            {
                this->GetFilter()->SetL1NoData(val);
            }

            /** Return the number of cirrus pixels. */
            unsigned long
            GetCirrusPixelNumber() const
            {
                return this->GetFilter()->GetCirrusPixelNumberOutput()->Get();
            }

            ULongPixelObjectType*
            GetCirrusPixelNumberOutput()
            {
                return this->GetFilter()->GetCirrusPixelNumberOutput();
            }

            const ULongPixelObjectType*
            GetCirrusPixelNumberOutput() const
            {
                return this->GetFilter()->GetCirrusPixelNumberOutput();
            }


            /** Return the number of valid pixels. */
            unsigned long
            GetValidPixelNumber() const
            {
                return this->GetFilter()->GetValidPixelNumberOutput()->Get();
            }

            ULongPixelObjectType*
            GetValidPixelNumberOutput()
            {
                return this->GetFilter()->GetValidPixelNumberOutput();
            }

            const ULongPixelObjectType*
            GetValidPixelNumberOutput() const
            {
                return this->GetFilter()->GetValidPixelNumberOutput();
            }

            /** Return the number of cloud pixels. */
            unsigned long
            GetCloudPixelNumber() const
            {
                return this->GetFilter()->GetCloudPixelNumberOutput()->Get();
            }

            ULongPixelObjectType*
            GetCloudPixelNumberOutput()
            {
                return this->GetFilter()->GetCloudPixelNumberOutput();
            }

            const ULongPixelObjectType*
            GetCloudPixelNumberOutput() const
            {
                return this->GetFilter()->GetCloudPixelNumberOutput();
            }

        protected:
            /** Constructor */
            CountCirrusPixelGenerator()
            {
            }

            /** Destructor */
            virtual
            ~CountCirrusPixelGenerator()
            {
            }


        private:
            CountCirrusPixelGenerator(const Self&); //purposely not implemented
            void
            operator=(const Self&); //purposely not implemented


        };

} // end namespace vns

#ifndef VNS_MANUAL_INSTANTIATION
#include "vnsCountCirrusPixelGenerator.txx"
#endif

#endif /* __vnsCountCirrusPixelGenerator_h */
