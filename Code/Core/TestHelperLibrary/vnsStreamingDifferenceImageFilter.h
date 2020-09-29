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

#ifndef __vnsStreamingDifferenceImageFilter_h
#define __vnsStreamingDifferenceImageFilter_h

#include "otbPersistentImageFilter.h"
#include "otbPersistentFilterStreamingDecorator.h"
#include "itkSimpleDataObjectDecorator.h"
#include "itkNumericTraits.h"
#include "itkArray.h"

namespace vns
{

    /** \class PersistentDifferenceImageFilter
     * \brief Implements comparison between two images in persistent version.
     *
     * This filter is used by the testing system to compute the difference between
     * a valid image and an image produced by the test. The comparison value is
     * computed by visiting all the pixels in the baseline images and comparing
     * their values with the pixel values in the neighborhood of the homologous
     * pixel in the other image.
     *
     * \ingroup IntensityImageFilters   Multithreaded
     */
    template<class TInputImage, class TOutputImage>
        class PersistentDifferenceImageFilter : public itk::ImageToImageFilter<TInputImage, TOutputImage>
        {
        public:
            /** Standard class typedefs. */
            typedef PersistentDifferenceImageFilter Self;
            typedef itk::ImageToImageFilter<TInputImage, TOutputImage> Superclass;
            typedef itk::SmartPointer<Self> Pointer;
            typedef itk::SmartPointer<const Self> ConstPointer;

            /** Method for creation through the object factory. */
            itkNewMacro(Self)

            /** Run-time type information (and related methods). */
            itkTypeMacro(PersistentDifferenceImageFilter, ImageToImageFilter)

            /** Some convenient typedefs. */
            typedef TInputImage InputImageType;
            typedef TOutputImage OutputImageType;
            typedef typename OutputImageType::PixelType OutputPixelType;
            typedef typename OutputImageType::RegionType OutputImageRegionType;
            typedef typename itk::NumericTraits<OutputPixelType>::RealType RealType;
            typedef typename itk::NumericTraits<RealType>::AccumulateType AccumulateType;
            typedef typename RealType::RealValueType ScalarRealType;

            /** Set the valid image input.  This will be input 0.  */
            virtual void
            SetValidInput(const InputImageType* validImage);

            /** Set the test image input.  This will be input 1.  */
            virtual void
            SetTestInput(const InputImageType* testImage);

            /** Set/Get the maximum distance away to look for a matching pixel.
             Default is 0. */
            itkSetMacro(ToleranceRadius, int)
            itkGetMacro(ToleranceRadius, int)

            /** Set/Get the minimum threshold for pixels to be different (relative).
             Default is 0. */
            itkSetMacro(DifferenceThreshold, ScalarRealType)
            itkGetMacro(DifferenceThreshold, ScalarRealType)

            /** Get parameters of the difference image after execution.  */
            itkGetMacro(MeanDifference, RealType)
            itkGetMacro(TotalDifference, RealType)
            itkGetMacro(NumberOfPixelsWithDifferences, unsigned long)

            void
            Synthetize(void);
            void
            Reset(void);

        protected:
            PersistentDifferenceImageFilter();
            virtual
            ~PersistentDifferenceImageFilter()
            {
            }

            void
            PrintSelf(std::ostream& os, itk::Indent indent) const;

            /** DifferenceImageFilter can be implemented as a multithreaded
             * filter.  Therefore, this implementation provides a
             * ThreadedGenerateData() routine which is called for each
             * processing thread. The output image data is allocated
             * automatically by the superclass prior to calling
             * ThreadedGenerateData().  ThreadedGenerateData can only write to
             * the portion of the output image specified by the parameter
             * "outputRegionForThread"
             *
             * \sa ImageToImageFilter::ThreadedGenerateData(),
             *     ImageToImageFilter::GenerateData()  */
            void
            ThreadedGenerateData(const OutputImageRegionType& threadRegion, itk::ThreadIdType threadId);

            virtual void
            GenerateOutputInformation();

            ScalarRealType m_DifferenceThreshold;
            RealType m_MeanDifference;
            RealType m_TotalDifference;
            unsigned long m_NumberOfPixelsWithDifferences;
            int m_ToleranceRadius;

            std::vector<RealType> m_ThreadDifferenceSum;
            itk::Array<unsigned long> m_ThreadNumberOfPixels;
            itk::Array<unsigned long> m_ThreadNumberOfDifferentPixels;

        private:
            PersistentDifferenceImageFilter(const Self &); //purposely not implemented
            void
            operator =(const Self&); //purposely not implemented
        };

    template<class TInputImage, class TOutputImage>
        class StreamingDifferenceImageFilter : public otb::PersistentFilterStreamingDecorator<
                PersistentDifferenceImageFilter<TInputImage, TOutputImage> >
        {
        public:
            /** Standard Self typedef */
            typedef StreamingDifferenceImageFilter Self;
            typedef otb::PersistentFilterStreamingDecorator<PersistentDifferenceImageFilter<TInputImage, TOutputImage> > Superclass;
            typedef itk::SmartPointer<Self> Pointer;
            typedef itk::SmartPointer<const Self> ConstPointer;

            /** Type macro */
            itkNewMacro(Self)

            /** Creation through object factory macro */
            itkTypeMacro(StreamingDIfferenceImageFilter, PersistentFilterStreamingDecorator)

            typedef typename Superclass::FilterType CompareFilterType;
            typedef typename CompareFilterType::OutputPixelType PixelType;
            typedef typename CompareFilterType::RealType RealType;
            typedef typename RealType::RealValueType ScalarRealType;
            typedef TInputImage InputImageType;
            typedef TOutputImage OutputImageType;

            /** Type of DataObjects used for scalar outputs */
            typedef itk::SimpleDataObjectDecorator<RealType> RealObjectType;
            typedef itk::SimpleDataObjectDecorator<PixelType> PixelObjectType;

            /** Connect one the first operands. */
            void
            SetTestInput(InputImageType * input)
            {
                this->GetFilter()->SetTestInput(input);
            }

            /** Connect one the second operands. */
            void
            SetValidInput(InputImageType * input)
            {
                this->GetFilter()->SetValidInput(input);
            }

            /** Set/Get the maximum distance away to look for a matching pixel.
             Default is 0. */
            int
            GetToleranceRadius() const
            {
                return this->GetFilter()->GetToleranceRadius();
            }

            void
            SetToleranceRadius(const int aTol)
            {
                this->GetFilter()->SetToleranceRadius(aTol);
            }

            /** Set/Get the minimum threshold for pixels to be different (relative).
             Default is 0. */
            ScalarRealType
            GetDifferenceThreshold() const
            {
                return this->GetFilter()->GetDifferenceThreshold();
            }

            void
            SetDifferenceThreshold(const ScalarRealType aTol)
            {
                this->GetFilter()->SetDifferenceThreshold(aTol);
            }

            /** Get parameters of the difference image after execution.  */
            RealType
            GetMeanDifference()
            {
                return this->GetFilter()->GetMeanDifference();
            }
            RealType
            GetTotalDifference()
            {
                return this->GetFilter()->GetTotalDifference();
            }
            unsigned long
            GetNumberOfPixelsWithDifferences()
            {
                return this->GetFilter()->GetNumberOfPixelsWithDifferences();
            }
            OutputImageType *
            GetOutput()
            {
                return this->GetFilter()->GetOutput();
            }
            const OutputImageType *
            GetOutput() const
            {
                return this->GetFilter()->GetOutput();
            }

        protected:
            /** Constructor */
            StreamingDifferenceImageFilter()
            {
            }

            /** Destructor */
            ~StreamingDifferenceImageFilter()
            {
            }

        private:
            StreamingDifferenceImageFilter(const Self &); //purposely not implemented
            void
            operator =(const Self&); //purposely not implemented
        };

} // end namespace vns

#ifndef VNS_MANUAL_INSTANTIATION
#include <vnsStreamingDifferenceImageFilter.txx>
#endif

#endif
