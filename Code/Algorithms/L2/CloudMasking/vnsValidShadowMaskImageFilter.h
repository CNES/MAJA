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
 * VERSION : 5-0-0 : FA : LAIG-FA-MAC-143764-CS : 26 avril 2016 : Mise en forme du code (indentation, etc.) *
 * VERSION : 4-0-0 : DM : LAIG-DM-MAC-517-CNES : 13 mars 2014 : Améliorations algorithmiques                *
 * VERSION : 1-0-0-3 : DM : 357 : 19 sept. 2011 : Calcule la somme des reflectances par label	                *
 *                                                                                                          *
 * VERSION : 1-0-0 : <TypeFT> : <NumFT> : 7 juin 2010 : Creation                                                           
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id$
 *                                                                                                          *
 ************************************************************************************************************/
#ifndef __vnsValidShadowMaskImageFilter_h
#define __vnsValidShadowMaskImageFilter_h

#include "vnsMacro.h"
#include "otbPersistentImageFilter.h"
#include "otbPersistentFilterStreamingDecorator.h"
#include "itkSimpleDataObjectDecorator.h"
#include <map>

namespace vns
{
    /** \class  PersistentValidShadowMaskImageFilter
     * \brief This class implements the computation of sums of valid pixel (and their number).
     * Those sums are computed per label.
     *
     * The ThreadedGeberateData method computes the values for each thread.
     * The Synthetize method gathers every thread values.
     * The output are two maps which keys are the label value:
     *
     * \begin{itemize}
     * \item For sum : std::map<TInputLabel::PixelType, double>
     * \item For valid pixel : std::map<TInputLabel::PixelType, unsigned int>
     * \end{itemize}
     *
     * Input images have to be otbImage. All pixels in InputDiff image that are at m_ExcludedValue which
     * value in label input image is m_ExcludedLabelValue will be discard wall be discard.
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

    template<class TInputDiff, class TInputImage, class TInputLabel>
        class PersistentValidShadowMaskImageFilter : public otb::PersistentImageFilter<TInputDiff, TInputDiff>
        {
        public:
            /** Standard class typedefs. */
            typedef PersistentValidShadowMaskImageFilter Self;
            typedef otb::PersistentImageFilter<TInputDiff, TInputDiff> Superclass;
            typedef itk::SmartPointer<Self> Pointer;
            typedef itk::SmartPointer<const Self> ConstPointer;

            /** Type macro */
            itkNewMacro(Self)

            /** Creation through object factory macro */
            itkTypeMacro(PersistentValidShadowMaskImageFilter, PersistentImageFilter )

            /** Some convenient typedefs. */
            typedef TInputLabel InputLabelType;
            typedef typename InputLabelType::Pointer InputLabelPointerType;
            typedef typename InputLabelType::ConstPointer InputLabelConstPointer;
            typedef typename InputLabelType::PixelType InputLabelPixelType;
            typedef typename InputLabelType::RegionType InputImageRegionType;

            typedef TInputDiff InputDiffImageType;
            typedef typename InputDiffImageType::Pointer InputDiffPointerType;
            typedef typename InputDiffImageType::ConstPointer InputDiffConstPointer;
            typedef typename InputDiffImageType::PixelType InputDiffPixelType;
            typedef typename InputDiffImageType::InternalPixelType InputDiffInternalPixelType;

            typedef TInputImage ReflectanceImageType;
            typedef typename ReflectanceImageType::ConstPointer ReflectanceImageConstPointer;
            typedef typename ReflectanceImageType::Pointer ReflectanceImagePointer;
            typedef typename ReflectanceImageType::PixelType ReflectanceImagePixelType;

            /** Type to use for computations. */
            typedef typename itk::NumericTraits<InputDiffInternalPixelType>::RealType RealType;

            /** Result map typedef */
            typedef typename std::map<InputLabelPixelType, double> DoubleMapType;
            typedef typename std::map<InputLabelPixelType, unsigned int> UIMapType;

            /** Smart Pointer type to a DataObject. */
            typedef typename itk::DataObject::Pointer DataObjectPointer;
            typedef itk::SimpleDataObjectDecorator<DoubleMapType> MapDoubleObjectType;
            typedef itk::SimpleDataObjectDecorator<UIMapType> MapUIObjectType;

            /** vector of map typedefs */
            typedef std::vector<DoubleMapType> DoubleVectorMapType;
            typedef std::vector<UIMapType> UIVectorMapType;

            vnsSetGetInputRawMacro(IPCLDShadLabel , InputLabelType, 1)
            vnsSetGetInputMacro( IPDiff, InputDiffImageType, 0)
            vnsSetGetInputMacro( Refl, ReflectanceImageType, 2)

            const MapDoubleObjectType*
            GetSumOfPixels() const
            {
                return static_cast<const MapDoubleObjectType*>(this->itk::ProcessObject::GetOutput(1));
            }

            const MapUIObjectType*
            GetNumberOfValidPixels() const
            {
                return static_cast<const MapUIObjectType*>(this->itk::ProcessObject::GetOutput(2));
            }

            const MapDoubleObjectType*
            GetReflSum() const
            {
                return static_cast<const MapDoubleObjectType*>(this->itk::ProcessObject::GetOutput(3));
            }

            /** Make a DataObject of the correct type to be used as the specified
             * output.
             */
            virtual DataObjectPointer
            MakeOutput(unsigned int idx);

            /** Excluded value accessor */
            vnsSetMacro(ExcludedValue, InputDiffPixelType)
            vnsGetMacro(ExcludedValue, InputDiffPixelType)

            /** Excluded shadow mask value */
            vnsSetMacro(ExcludedLabelValue, InputLabelPixelType)
            vnsGetMacro(ExcludedLabelValue, InputLabelPixelType)

            // Shadow band accessors
            itkSetMacro(ShadowBand, unsigned int)
            itkGetConstReferenceMacro(ShadowBand, unsigned int)

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
            PersistentValidShadowMaskImageFilter();
            /** Destructor */
            virtual
            ~PersistentValidShadowMaskImageFilter()
            {
            }

            /** Multi-thread version GenerateData. */
            virtual void
            ThreadedGenerateData(const InputImageRegionType& outputRegionForThread, itk::ThreadIdType threadId);

            /** PrintSelf method */
            virtual void
            PrintSelf(std::ostream& os, itk::Indent indent) const;

        private:
            PersistentValidShadowMaskImageFilter(const Self&); //purposely not implemented
            void
            operator=(const Self&); //purposely not implemented

            DoubleVectorMapType m_SumList;
            UIVectorMapType m_NumberList;
            DoubleVectorMapType m_ReflSum;
            InputDiffPixelType m_ExcludedValue;
            InputLabelPixelType m_ExcludedLabelValue;
            /** Shadow band declaration */
            unsigned int m_ShadowBand;

            // Outputs
            /*  double m_SumList     <=> GetOutput(1)
             *  bool   m_NumberList  <=> GetOutput(2)
             */

        };
    // end of class PersistentValidShadowMaskImageFilterImageFilter

    /**===========================================================================*/

    /** \class  ValidShadowMaskImageFilter
     * \brief This class implements the computation of sums of valid pixel (and their number). Those sums are label per label.
     *
     * \begin{itemize}
     * \item For sum : std::map<TInputLabel::PixelType, double>
     * \item For valid pixel : std::map<TInputLabel::PixelType, unsigned int>
     * \end{itemize}
     *
     * Input images of difference have to be otbImage. All pixels in InputDiff image that are at m_ExcludedValue which
     * value in label input image is m_ExcludedLabelValue will be discard.
     *
     * \author CS Systemes d'Information
     *
     * \sa vnsPersistentValidShadowMaskImageFilter
     * \sa otbPersistentImageFilter
     * \sa otbPersistentFilterStreamingDecorator
     *
     * \ingroup L2
     *
     */

    template<class TInputDiff, class TInputImage, class TInputLabel>
        class ValidShadowMaskImageFilter : public otb::PersistentFilterStreamingDecorator<
                PersistentValidShadowMaskImageFilter<TInputDiff, TInputImage, TInputLabel> >
        {
        public:
            /** Standard Self typedef */
            typedef ValidShadowMaskImageFilter Self;
            typedef otb::PersistentFilterStreamingDecorator<PersistentValidShadowMaskImageFilter<TInputDiff, TInputImage, TInputLabel> > Superclass;
            typedef itk::SmartPointer<Self> Pointer;
            typedef itk::SmartPointer<const Self> ConstPointer;

            /** Type macro */
            itkNewMacro(Self)

            /** Creation through object factory macro */
            itkTypeMacro(ValidShadowMaskImageFilter,PersistentFilterStreamingDecorator)

            typedef typename Superclass::FilterType StatFilterType;
            typedef typename StatFilterType::RealType RealType;

            /** Containers typdefs from PersistentValidShadowMaskImageFilter */
            typedef typename StatFilterType::DoubleMapType DoubleMapType;
            typedef typename StatFilterType::UIMapType UIMapType;

            /** Some convenient typedefs. */
            typedef TInputLabel InputLabelType;
            typedef typename InputLabelType::ConstPointer InputLabelConstPointerType;
            typedef typename InputLabelType::Pointer InputLabelPointerType;
            typedef typename InputLabelType::PixelType InputLabelPixelType;
            typedef TInputDiff InputDiffImageType;
            typedef typename InputDiffImageType::Pointer InputDiffImagePointerType;
            typedef typename InputDiffImageType::ConstPointer InputDiffImageConstPointer;
            typedef typename InputDiffImageType::PixelType InputDiffImagePixelType;
            typedef TInputImage ReflectanceImageType;
            typedef typename ReflectanceImageType::ConstPointer ReflectanceImageConstPointer;
            typedef typename ReflectanceImageType::Pointer ReflectanceImagePointer;
            typedef typename ReflectanceImageType::PixelType ReflectanceImagePixelType;

            /** Getters/Setters **/
            void
            SetIPCLDShadLabel(const InputLabelType * pShadLabel)
            {
                this->GetFilter()->SetIPCLDShadLabel(pShadLabel);
            }

            InputLabelType*
            GetIPCLDShadLabel()
            {
                return this->GetFilter()->GetIPCLDShadLabel();
            }

            void
            SetIPDiffInput(const InputDiffImageType * pDiff)
            {
                this->GetFilter()->SetIPDiffInput(pDiff);
            }

            InputDiffImageType*
            GetIPDiffInput()
            {
                return this->GetFilter()->GetIPDiffInput();
            }

            void
            SetReflInput(const ReflectanceImageType * pRefl)
            {
                this->GetFilter()->SetReflInput(pRefl);
            }

            ReflectanceImageType*
            GetReflInput()
            {
                return this->GetFilter()->GetReflInput();
            }

            /** Set the diff image excluded value.*/
            void
            SetExcludedValue(const InputDiffImagePixelType & pixVal)
            {
                this->GetFilter()->SetExcludedValue(pixVal);
            }
            InputDiffImagePixelType
            GetExcludedValue()
            {
                return this->GetFilter()->GetExcludedValue();
            }

            /** Set the shadow mask excluded value.*/
            void
            SetExcludedLabelValue(const InputLabelPixelType & pixVal)
            {
                this->GetFilter()->SetExcludedLabelValue(pixVal);
            }
            InputLabelPixelType
            GetExcludedLabelValue()
            {
                return this->GetFilter()->GetExcludedLabelValue();
            }

            /** Set the shadow band.*/
            void
            SetShadowBand(const unsigned int & pBand)
            {
                this->GetFilter()->SetShadowBand(pBand);
            }
            unsigned int
            GetShadowBand()
            {
                return this->GetFilter()->GetShadowBand();
            }

            // Outputs
            const DoubleMapType
            GetSumOfPixels() const
            {
                return this->GetFilter()->GetSumOfPixels()->Get();
            }

            const UIMapType
            GetNumberOfValidPixels() const
            {
                return this->GetFilter()->GetNumberOfValidPixels()->Get();
            }
            const DoubleMapType
            GetReflSum() const
            {
                return this->GetFilter()->GetReflSum()->Get();
            }

        protected:
            /** Constructor */
            ValidShadowMaskImageFilter()
            {
            }

            /** Destructor */
            virtual
            ~ValidShadowMaskImageFilter()
            {
            }

            /** PrintSelf method */
            virtual void
            PrintSelf(std::ostream& os, itk::Indent indent) const;

        private:
            ValidShadowMaskImageFilter(const Self&); //purposely not implemented
            void
            operator=(const Self&); //purposely not implemented

        };

} // End namespace vns

#ifndef VNS_MANUAL_INSTANTIATION
#include "vnsValidShadowMaskImageFilter.txx"
#endif

#endif /* __vnsValidShadowMaskImageFilter_h */
