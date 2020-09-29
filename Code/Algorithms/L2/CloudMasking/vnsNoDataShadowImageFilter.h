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
 * VERSION : 4-0-0 : FA : LAIG-FA-MAC-120653-CS : 7 mai 2014 : Correction de règles qualité ou commentaires *
 * VERSION : 1-0-0 : <TypeFT> : <NumFT> : 9 juil 2010 : Creation
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id: vnsNoDataShadowMapUpdating.h 2406 2011-01-19 15:38:23Z tfeuvrie $
 *                                                                                                          *
 ************************************************************************************************************/
#ifndef __vnsNoDataShadowMapUpdating_h
#define __vnsNoDataShadowMapUpdating_h

#include "itkNumericTraits.h"
#include "itkSimpleDataObjectDecorator.h"

#include "otbPersistentImageFilter.h"
#include "otbPersistentFilterStreamingDecorator.h"

#include "vnsMacro.h"

namespace vns
{
    /** \class PersistentNoDataShadowImageFilter
     * \brief This class generates a list that contains the shadow labels for which
     * no altitude has been found and for which their projection for the altitude
     * obtained at the first global iteration was in the no data zone in the composite image.
     *
     * The ThreadedGeberateData method computes the values for each thread.
     * The Synthetize method gathers every thread values.
     *
     * The input image is the difference image between the current image and the composite one
     * for the shadow band.
     * The output are a list of labels for which the cloud altitude will not be updated in
     * the ShadowMaskDetermination class and the total number of labels.
     *
     *
     * \author CS Systemes d'Information
     *
     * \sa ImageToImageFilter
     *
     * \ingroup L2
     *
     */
    template<class TInputImage>
        class PersistentNoDataShadowImageFilter : public otb::PersistentImageFilter<TInputImage, TInputImage>
        {
        public:
            /** Standard class typedefs. */
            typedef PersistentNoDataShadowImageFilter Self;
            typedef otb::PersistentImageFilter<TInputImage, TInputImage> Superclass;
            typedef itk::SmartPointer<Self> Pointer;
            typedef itk::SmartPointer<const Self> ConstPointer;

            /** Type macro */
            itkNewMacro(Self)
            /** Creation through object factory macro */
            itkTypeMacro(PersistentNoDataShadowImageFilter, PersistentImageFilter )

            /** Some convenient typedefs. */
            typedef TInputImage InputImageType;
            typedef typename InputImageType::Pointer InputImagePointerType;
            typedef typename InputImageType::ConstPointer InputImageConstPointer;
            typedef typename InputImageType::PixelType InputImagePixelType;
            typedef typename InputImageType::InternalPixelType InputImageInternalPixelType;
            typedef typename InputImageType::RegionType InputImageRegionType;

            typedef typename InputImageType::IndexType IndexType;

            /** Type to use for computations. */
            typedef typename itk::NumericTraits<InputImageInternalPixelType>::RealType RealType;

            /** Result map typedef */
            typedef unsigned long LinearIndex;
            typedef unsigned short LabelType;
            typedef typename std::map<LinearIndex, LabelType> LabelMapType;

            /** vector of map typedefs */
            typedef std::list<LabelType> LabelListType;

            /** vector of list typedefs */
            typedef std::vector<LabelListType> LabelVectorListType;

            /** Smart Pointer type to a DataObject. */
            typedef typename itk::DataObject::Pointer DataObjectPointer;
            typedef itk::SimpleDataObjectDecorator<LabelListType> LabelListObjectType;
            typedef itk::SimpleDataObjectDecorator<unsigned long> ULObjectType;

            /** Getters/Setters **/
            vnsSetGetImageMacro( Diff, InputImageType, 0)

            // Reference altitude for sun direction accessors
            vnsSetMacro(ShadowMap, LabelMapType)
            vnsGetConstMacro(ShadowMap, LabelMapType)

            const LabelListObjectType *
            GetLabelKOList() const
            {
                return static_cast<const LabelListObjectType*>(this->itk::ProcessObject::GetOutput(1));
            }
            const ULObjectType *
            GetNumberOfLabels() const
            {
                return static_cast<const ULObjectType*>(this->itk::ProcessObject::GetOutput(2));
            }
            /** Make a DataObject of the correct type to be used as the specified
             * output.
             */
            virtual DataObjectPointer
            MakeOutput(unsigned int idx);

            /** Real NoData value accessor */
            vnsSetMacro(NoDataValue, InputImagePixelType)
            vnsGetMacro(NoDataValue, InputImagePixelType)

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
            PersistentNoDataShadowImageFilter();
            /** Destructor */
            virtual
            ~PersistentNoDataShadowImageFilter()
            {
            }

            /** Multi-thread version GenerateData. */
            virtual void
            ThreadedGenerateData(const InputImageRegionType& outputRegionForThread, itk::ThreadIdType threadId);

            /** PrintSelf method */
            virtual void
            PrintSelf(std::ostream& os, itk::Indent indent) const;

        private:
            PersistentNoDataShadowImageFilter(const Self&); //purposely not implemented
            void
            operator=(const Self&); //purposely not implemented

            bool
            FindLabel(LabelListType & pLabelList, LabelType & pLabel);

            LabelMapType m_ShadowMap;
            LabelVectorListType m_LabelKOVList;
            LabelVectorListType m_LabelVList;

            InputImagePixelType m_NoDataValue;

        };
    // end of class PersistentNoDataShadowImageFilter

    /**===========================================================================*/

    /** \class  NoDataShadowImageFilter
     * \brief This class generates a list that contains the shadow labels for which
     * no altitude has been found and for which their projection for the altitude
     * obtained at the first global iteration was in the no data zone in the composite image.
     *
     * The input image is the difference image between the current image and the composite one
     * for the shadow band.
     * The output are a list of labels for which the cloud altitude will not be updated in
     * the ShadowMaskDetermination class and the total number of labels.
     *
     * \author CS Systemes d'Information
     *
     * \sa vnsPersistentNoDataShadowImageFilter
     * \sa otbPersistentImageFilter
     * \sa otbPersistentFilterStreamingDecorator
     *
     * \ingroup L2
     *
     */

    template<class TInputImage>
        class NoDataShadowImageFilter : public otb::PersistentFilterStreamingDecorator<
                PersistentNoDataShadowImageFilter<TInputImage> >
        {
        public:
            /** Standard Self typedef */
            typedef NoDataShadowImageFilter Self;
            typedef otb::PersistentFilterStreamingDecorator<PersistentNoDataShadowImageFilter<TInputImage> > Superclass;
            typedef itk::SmartPointer<Self> Pointer;
            typedef itk::SmartPointer<const Self> ConstPointer;

            /** Type macro */
            itkNewMacro(Self)

            /** Creation through object factory macro */
            itkTypeMacro(NoDataShadowImageFilter,PersistentFilterStreamingDecorator)

            typedef typename Superclass::FilterType SuperFilterType;
            typedef typename SuperFilterType::RealType RealType;

            /** Containers typdefs from PersistentNoDataShadowImageFilter */
            typedef typename SuperFilterType::LabelMapType LabelMapType;
            typedef typename SuperFilterType::LabelListType LabelListType;

            /** Some convenient typedefs. */

            typedef TInputImage InputImageType;
            typedef typename InputImageType::Pointer InputImagePointerType;
            typedef typename InputImageType::ConstPointer InputImageConstPointer;
            typedef typename InputImageType::PixelType InputImagePixelType;

            /** Getters/Setters **/
            void
            SetDiffImage(const InputImageType * pDiffImage)
            {
                this->GetFilter()->SetDiffImage(pDiffImage);
            }

            InputImageType*
            GetDiffImage()
            {
                return this->GetFilter()->GetDiffImage();
            }

            /** Set the shadow map that contains for each shadowed pixel the number of label */
            void
            SetShadowMap(const LabelMapType & pMap)
            {
                this->GetFilter()->SetShadowMap(pMap);
            }
            LabelMapType
            GetShadowMap()
            {
                return this->GetFilter()->GetShadowMap();
            }

            /** Set the real no_data value contained in the input image.*/
            void
            SetRealNoDataValue(const InputImagePixelType & pixVal)
            {
                this->GetFilter()->SetNoDataValue(pixVal);
            }
            InputImagePixelType
            GetRealNoDataValue()
            {
                return this->GetFilter()->GetNoDataValue();
            }

            // Outputs
            const LabelListType
            GetLabelKOListOutput() const
            {
                return this->GetFilter()->GetLabelKOList()->Get();
            }
            unsigned long
            GetNumberOfLabelsOutput() const
            {
                return this->GetFilter()->GetNumberOfLabels()->Get();
            }

        protected:
            /** Constructor */
            NoDataShadowImageFilter()
            {
            }
            /** Destructor */
            virtual
            ~NoDataShadowImageFilter()
            {
            }

            /** PrintSelf method */
            virtual void
            PrintSelf(std::ostream& os, itk::Indent indent) const;

        private:
            NoDataShadowImageFilter(const Self&); //purposely not implemented
            void
            operator=(const Self&); //purposely not implemented

        };

} // End namespace vns

#ifndef VNS_MANUAL_INSTANTIATION
#include "vnsNoDataShadowImageFilter.txx"
#endif

#endif // __vnsNoDataShadowMapUpdating_h
