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
 * VERSION : 5-0-0 : DM : LAIG-DM-MAC-982-CNES : 8 janvier 2016 : Modification de l’algorithme de detection **
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id: vnsStreamingThresholdFromLabelImageFilter.h 10095 2016-01-15 09:42:27Z abricier $                   *
 *                                                                                                          *
 ************************************************************************************************************/
#ifndef __vnsStreamingThresholdFromLabelImageFilter_h
#define __vnsStreamingThresholdFromLabelImageFilter_h

#include "otbPersistentImageFilter.h"
#include "itkNumericTraits.h"
#include "itkArray.h"
#include "itkSimpleDataObjectDecorator.h"
#include "otbPersistentFilterStreamingDecorator.h"
#include "vnsMacro.h"

namespace vns
{

    /** \class PersistentStreamingThresholdFromLabelImageFilter
     * \brief Returns a validity map including for each label the result of a test regarding
     * the total population of the label and a pixel subset from the label also at true in the input mask.
     *
     */
    template<class TInputMask, class TLabelImage>
        class PersistentStreamingThresholdFromLabelImageFilter : public otb::PersistentImageFilter<TInputMask, TInputMask>
        {
        public:
            /** Standard Self typedef */
            typedef PersistentStreamingThresholdFromLabelImageFilter Self;
            typedef otb::PersistentImageFilter<TInputMask, TInputMask> Superclass;
            typedef itk::SmartPointer<Self> Pointer;
            typedef itk::SmartPointer<const Self> ConstPointer;

            /** Method for creation through the object factory. */
            itkNewMacro(Self)

            /** Runtime information support. */
            itkTypeMacro(PersistentStreamingThresholdFromLabelImageFilter, PersistentImageFilter)

            /** Image related typedefs. */
            typedef TInputMask InputMaskType;
            typedef typename TInputMask::Pointer InputMaskPointer;
            typedef TLabelImage LabelImageType;
            typedef typename TLabelImage::Pointer LabelImagePointer;

            typedef typename InputMaskType::PixelType MaskPixelType;
            typedef typename LabelImageType::PixelType LabelPixelType;
            typedef std::map<LabelPixelType, double> LabelPopulationMapType;
            typedef std::map<LabelPixelType, bool> LabelValidityMapType;

            /** Smart Pointer type to a DataObject. */
            typedef typename itk::DataObject::Pointer DataObjectPointer;

            itkStaticConstMacro(InputImageDimension, unsigned int, TInputMask::ImageDimension);
            typedef itk::ImageBase<InputImageDimension> ImageBaseType;
            typedef typename ImageBaseType::RegionType InputImageRegionType;

            /** Type of DataObjects used for scalar outputs */
            typedef itk::SimpleDataObjectDecorator<LabelValidityMapType> LabelValidityMapObjectType;

            /** Set input label image */
            vnsSetGetInputRawMacro(Input , InputMaskType , 0)
            /** Set input label image */
            vnsSetGetInputRawMacro(InputLabelImage, LabelImageType , 1)

            /** Set/Get validity lower threshold */
            itkSetMacro(LowerValidityThreshold, double);
            itkGetConstReferenceMacro(LowerValidityThreshold, double);

            /** Return the computed Mean for each label in the input label image */
            itkGetConstMacro(LabelValidityStatusMap, LabelValidityMapType)

            /** Make a DataObject of the correct type to be used as the specified
             * output. */
            virtual DataObjectPointer MakeOutput(unsigned int idx);

            /** Pass the input through unmodified. Do this by Grafting in the
             *  AllocateOutputs method.
             */
            void AllocateOutputs();

            virtual void GenerateOutputInformation();

            void Synthetize(void);

            void Reset(void);

            /** Due to heterogeneous input template GenerateInputRequestedRegion must be reimplemented using explicit cast **/
            /** This new implementation is inspired by the one of itk::ImageToImageFilter **/
            void GenerateInputRequestedRegion();

        protected:
            PersistentStreamingThresholdFromLabelImageFilter();
            virtual ~PersistentStreamingThresholdFromLabelImageFilter()
            {}
            void PrintSelf(std::ostream& os, itk::Indent indent) const;

            /** GenerateData. */
            void GenerateData();

        private:
            PersistentStreamingThresholdFromLabelImageFilter(const Self &); //purposely not implemented
            void operator =(const Self&);//purposely not implemented

            LabelPopulationMapType m_LabelTotalPopulation;
            LabelPopulationMapType m_LabelValidPopulation;
            LabelValidityMapType m_LabelValidityStatusMap;
            double m_LowerValidityThreshold;
        }; // end of class

        /*===========================================================================*/

        /** \class StreamingThresholdFromLabelImageFilter
         * \brief
         *
         */

    template<class TInputMask, class TLabelImage>
        class StreamingThresholdFromLabelImageFilter : public otb::PersistentFilterStreamingDecorator<
                PersistentStreamingThresholdFromLabelImageFilter<TInputMask, TLabelImage> >
        {
        public:
            /** Standard Self typedef */
            typedef StreamingThresholdFromLabelImageFilter Self;
            typedef otb::PersistentFilterStreamingDecorator<PersistentStreamingThresholdFromLabelImageFilter<TInputMask, TLabelImage> > Superclass;
            typedef itk::SmartPointer<Self> Pointer;
            typedef itk::SmartPointer<const Self> ConstPointer;

            /** Type macro */
            itkNewMacro(Self)

            /** Creation through object factory macro */
            itkTypeMacro(StreamingThresholdFromLabelImageFilter, PersistentFilterStreamingDecorator)

            typedef TInputMask MaskType;
            typedef TLabelImage LabelImageType;

            typedef typename Superclass::FilterType::LabelValidityMapType LabelValidityMapType;
            typedef typename Superclass::FilterType::LabelValidityMapObjectType LabelValidityMapObjectType;

            typedef typename Superclass::FilterType::LabelPopulationMapType LabelPopulationMapType;

            /** Set input Mask */
            void
            SetInput(const MaskType * input)
            {
                this->GetFilter()->SetInput(input);
            }

            /** Get input Mask */
            const MaskType *
            GetInput()
            {
                return this->GetFilter()->GetInput();
            }

            /** Set input label image (monoband) */
            void
            SetInputLabelImage(const LabelImageType * input)
            {
                this->GetFilter()->SetInputLabelImage(input);
            }

            /** Get input label image (monoband) */
            const LabelImageType *
            GetInputLabelImage()
            {
                return this->GetFilter()->GetInputLabelImage();
            }

            /** Set input Validity Threshold */
            void
            SetLowerValidityThreshold(const double t)
            {
                this->GetFilter()->SetLowerValidityThreshold(t);
            }

            /** Return the computed Mean for each label */
            LabelValidityMapType
            GetLabelValidityStatusMap() const
            {
                return this->GetFilter()->GetLabelValidityStatusMap();
            }

        protected:
            /** Constructor */
            StreamingThresholdFromLabelImageFilter()
            {
            }
            /** Destructor */
            virtual
            ~StreamingThresholdFromLabelImageFilter()
            {
            }

        private:
            StreamingThresholdFromLabelImageFilter(const Self &); //purposely not implemented
            void
            operator =(const Self&); //purposely not implemented
        };

} // end namespace otb

#ifndef VNS_MANUAL_INSTANTIATION
#include "vnsStreamingThresholdFromLabelImageFilter.txx"
#endif

#endif
