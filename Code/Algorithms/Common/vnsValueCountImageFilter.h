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
 * VERSION : 5-0-0 : FA : LAIG-FA-MAC-143764-CS : 12 avril 2016 : Mise en forme du code (indentation, etc.) *
 * VERSION : 4-4-0 : FA : LAIG-FA-MAC-133882-CS : 3 juin 2015 : Correction code pour la qualite             *
 * VERSION : 1-0-0 : <TypeFT> : <NumFT> : 21 sept. 2010 : Creation                                                           
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id$
 *                                                                                                          *
 ************************************************************************************************************/
#ifndef __vnsValueCountImageFilter_h
#define __vnsValueCountImageFilter_h

#include "vnsMacro.h"
#include "otbPersistentImageFilter.h"
#include "otbPersistentFilterStreamingDecorator.h"
#include "itkSimpleDataObjectDecorator.h"

namespace vns
{
    /** \class  PersistentValueCountImageFilter
     * \brief This class counts the number of pixels equal to a given value
     * in an input image.
     *
     * The ThreadedGeberateData method counts the values for each thread.
     * The Synthetize method gathers every thread values.
     * The output is a scalar that contains the number of pixel equal to this value.
     *
     * Input image has to be otbImage.
     *
     * \author CS Systemes d'Information
     *
     * \sa Object
     * \sa otbPersistentImageFilter
     * \sa otbPersistentFilterStreamingDecorator
     *
     * \ingroup L2
     *
     */

    template<class TInputImage, class TOutputImage = TInputImage>
        class PersistentValueCountImageFilter : public otb::PersistentImageFilter<TInputImage, TOutputImage>
        {
        public:
            /** Standard class typedefs. */
            typedef PersistentValueCountImageFilter Self;
            typedef otb::PersistentImageFilter<TInputImage, TOutputImage> Superclass;
            typedef itk::SmartPointer<Self> Pointer;
            typedef itk::SmartPointer<const Self> ConstPointer;

            /** Type macro */
            itkNewMacro(Self)

            /** Creation through object factory macro */
            itkTypeMacro(PersistentValueCountImageFilter, PersistentImageFilter )

            /** Some convenient typedefs. */
            typedef TInputImage InputImageType;
            typedef typename InputImageType::Pointer InputImagePointerType;
            typedef typename InputImageType::ConstPointer InputImageConstPointer;
            typedef typename InputImageType::PixelType InputImagePixelType;
            typedef typename InputImageType::RegionType InputImageRegionType;

            typedef unsigned long CountType;

            /** Type to use for computations. */
            typedef typename itk::NumericTraits<InputImagePixelType>::RealType RealType;

            /** Smart Pointer type to a DataObject. */
            typedef typename itk::DataObject::Pointer DataObjectPointer;
            typedef itk::SimpleDataObjectDecorator<CountType> CountValueObjectType;

            /** vector of ULong typedefs */
            typedef std::vector<CountType> VectorCountType;

            /** Getters/Setters **/
            // In VENµS, the input image is the mask of No_Data (L2inNDT)
            vnsSetGetInputMacro(Image, InputImageType, 0)

            const CountValueObjectType *
            GetValueCount() const
            {
                return static_cast<const CountValueObjectType*>(this->itk::ProcessObject::GetOutput(1));
            }

            /** Make a DataObject of the correct type to be used as the specified
             * output.
             */
            virtual DataObjectPointer
            MakeOutput(unsigned int idx);

            /** pixel value accessor */
            vnsSetMacro(PixelValue, InputImagePixelType)
            vnsGetMacro(PixelValue, InputImagePixelType)

            /** Pass the input through unmodified. Do this by Grafting in the
             *  AllocateOutputs method.
             */
            virtual void
            AllocateOutputs(void);
            virtual void
            GenerateOutputInformation(void);
            virtual void
            Synthetize(void);
            virtual void
            Reset(void);

        protected:
            /** Constructor */
            PersistentValueCountImageFilter();
            /** Destructor */
            virtual
            ~PersistentValueCountImageFilter()
            {
            }

            /** Multi-thread version GenerateData. */
            virtual void
            ThreadedGenerateData(const InputImageRegionType& outputRegionForThread, itk::ThreadIdType threadId);

            /** PrintSelf method */
            virtual void
            PrintSelf(std::ostream& os, itk::Indent indent) const;

        private:
            PersistentValueCountImageFilter(const Self&); //purposely not implemented
            void
            operator=(const Self&); //purposely not implemented

            InputImagePixelType m_PixelValue;
            VectorCountType m_CountValueByThread;

        };
    // end of class PersistentConditionalStatisticsVectorImageFilter

    /**===========================================================================*/

    /** \class  ValueCountImageFilter
     * \brief This class counts the number of pixels equal to a given value
     * in an input image.
     *
     * The output is a scalar that contains the number of pixel equal to this value.
     *
     * Input image has to be otbImage.
     *
     * \author CS Systemes d'Information
     *
     * \sa vnsPersistentValueCountImageFilter
     * \sa otbPersistentImageFilter
     * \sa otbPersistentFilterStreamingDecorator
     *
     * \ingroup L2
     *
     */

    template<class TInputImage>
        class ValueCountImageFilter : public otb::PersistentFilterStreamingDecorator<PersistentValueCountImageFilter<TInputImage> >
        {
        public:
            /** Standard Self typedef */
            typedef ValueCountImageFilter Self;
            typedef otb::PersistentFilterStreamingDecorator<PersistentValueCountImageFilter<TInputImage> > Superclass;
            typedef itk::SmartPointer<Self> Pointer;
            typedef itk::SmartPointer<const Self> ConstPointer;

            /** Type macro */
            itkNewMacro(Self)

            /** Creation through object factory macro */
            itkTypeMacro(ValueCountImageFilter,PersistentFilterStreamingDecorator)

            typedef typename Superclass::FilterType CountValueType;
            typedef typename CountValueType::RealType RealType;

            /** Some convenient typedefs. */
            typedef TInputImage InputImageType;
            typedef typename InputImageType::Pointer InputImagePointerType;
            typedef typename InputImageType::ConstPointer InputImageConstPointer;
            typedef typename InputImageType::PixelType InputImagePixelType;
            typedef typename InputImageType::RegionType InputImageRegionType;

            typedef unsigned long CountType;

            /** Getters/Setters **/
            void
            SetImage(const InputImageType * pImage)
            {
                this->GetFilter()->SetImageInput(pImage);
            }

            InputImageType*
            GetImage()
            {
                return this->GetFilter()->GetImageInput();
            }

            /** Set the pixel value.*/
            void
            SetPixelValue(const InputImagePixelType & pixVal)
            {
                this->GetFilter()->SetPixelValue(pixVal);
            }
            InputImagePixelType
            GetPixelValue()
            {
                return this->GetFilter()->GetPixelValue();
            }

            // Outputs
            CountType
            GetValueCount() const
            {
                return this->GetFilter()->GetValueCount()->Get();
            }

        protected:
            /** Constructor */
            ValueCountImageFilter()
            {
            }
            /** Destructor */
            virtual
            ~ValueCountImageFilter()
            {
            }
            /** PrintSelf method */
            virtual void
            PrintSelf(std::ostream& os, itk::Indent indent) const;

        private:
            ValueCountImageFilter(const Self&); //purposely not implemented
            void
            operator=(const Self&); //purposely not implemented

        };

} // End namespace vns

#ifndef VNS_MANUAL_INSTANTIATION
#include "vnsValueCountImageFilter.txx"
#endif

#endif /* __vnsValueCountImageFilter_h */
