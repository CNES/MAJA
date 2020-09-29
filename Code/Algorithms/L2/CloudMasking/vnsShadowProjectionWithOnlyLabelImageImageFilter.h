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
 * VERSION : 5-0-0 : DM : LAIG-DM-MAC-982-CNES : 5 avr. 2016 : Creation                                       *
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id:
 *                                                                                                          *
 ************************************************************************************************************/
#ifndef __vnsShadowProjectionWithOnlyLabelImageImageFilter_h
#define __vnsShadowProjectionWithOnlyLabelImageImageFilter_h

#include "itkImageToImageFilter.h"

#include "otbPersistentImageFilter.h"
#include "otbPersistentFilterStreamingDecorator.h"
#include "itkSimpleDataObjectDecorator.h"

#include "vnsDEMDataManager.h"

#include "vnsMacro.h"

namespace vns
{
    /** \class  PersistentShadowProjectionWithOnlyLabelImageImageFilter
     * \brief This class computes the location of the shadows of the clouds detected at
     * level 1. The output is a map that contains the label value for
     * each shadow pixel.
     *
     * The ThreadedGeberateData method computes the values for each thread.
     * The Synthetize method gathers every thread values.
     *
     * Input images have to be otbImage.
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

    template<class TLabelImage, class TDTMImage, class TGRIDImage, class TCLAImage>
        class PersistentShadowProjectionWithOnlyLabelImageImageFilter : public otb::PersistentImageFilter<TLabelImage,
                TLabelImage>
        {
        public:
            /** Standard class typedefs. */
            typedef PersistentShadowProjectionWithOnlyLabelImageImageFilter Self;
            typedef otb::PersistentImageFilter<TLabelImage, TLabelImage> Superclass;
            typedef itk::SmartPointer<Self> Pointer;
            typedef itk::SmartPointer<const Self> ConstPointer;

            /** Type macro */
            itkNewMacro(Self)

            /** Creation through object factory macro */
            itkTypeMacro(PersistentShadowProjectionWithOnlyLabelImageImageFilter, PersistentImageFilter )

            /** Some convenient typedefs. */
            typedef typename Superclass::InputImageType InputMaskType;
            typedef typename InputMaskType::ConstPointer InputMaskConstPointer;
            typedef typename InputMaskType::Pointer InputMaskPointer;

            typedef TLabelImage LabelImageType;
            typedef typename LabelImageType::Pointer LabelImagePointer;
            typedef typename LabelImageType::ConstPointer LabelImageConstPointer;
            typedef typename LabelImageType::PixelType LabelImagePixelType;
            typedef typename LabelImageType::RegionType LabelImageRegionType;
            typedef typename LabelImageType::SizeType SizeType;

            typedef TDTMImage DTMImageType;
            typedef typename DTMImageType::Pointer DTMImagePointer;
            typedef typename DTMImageType::ConstPointer DTMImageConstPointer;
            typedef typename DTMImageType::PixelType DTMImagePixelType;

            typedef TGRIDImage GRIDImageType;
            typedef typename GRIDImageType::Pointer GRIDImagePointer;
            typedef typename GRIDImageType::ConstPointer GRIDImageConstPointer;
            typedef typename GRIDImageType::PixelType GRIDImagePixelType;
            typedef typename GRIDImageType::IndexType IndexType;
            typedef typename GRIDImageType::PointType PointType;
            typedef typename GRIDImageType::SpacingType SpacingType;

            typedef TCLAImage CLAImageType;
            typedef typename CLAImageType::Pointer CLAImagePointer;
            typedef typename CLAImageType::ConstPointer CLAImageConstPointer;
            typedef typename CLAImageType::PixelType CLAImagePixelType;

            typedef DEMDataManager<DTMImageType> Intersector3DType;

            typedef unsigned long LinearIndex;

            /** Result map typedef */
            typedef typename std::map<LinearIndex, LabelImagePixelType> LabelMapType;

            /** Smart Pointer type to a DataObject. */
            typedef typename itk::DataObject::Pointer DataObjectPointer;
            typedef itk::SimpleDataObjectDecorator<LabelMapType> MapLabelObjectType;

            /** vector of map typedefs */
            typedef std::vector<LabelMapType> LabelVectorMapType;

            // Reference altitude for solar direction accessors
            vnsMemberAndSetAndGetConstReferenceMacro(SOLHRef, double)

            // Delta H
            vnsMemberAndSetAndGetConstReferenceMacro(DeltaH, double)


            /** Set the image of label of the cloud mask */
            vnsSetGetInputMacro( L2CLDSubLabel , LabelImageType , 0)
            /** Set the DTM image */
            vnsSetGetInputMacro( DTM , DTMImageType, 1)
            /** Set the viewing direction grid */
            vnsSetGetInputMacro( IPVIE , GRIDImageType, 2)
            /** Set the solar direction grid */
            vnsSetGetInputMacro( IPSOL , GRIDImageType, 3 )
            /** Set the cloud altitude image */
//            vnsSetGetInputMacro( IPCLA , CLAImageType , 4)

            // L2 Coarse Resolution accessors
            vnsMemberAndSetAndGetConstReferenceMacro(L2CoarseResolution, unsigned int)

            // Reference altitude for viewing direction accessors
            virtual void
            SetVIEHRef(const double viefref)
            {
                if (vnsEqualsDoubleMacro(viefref, 0.) == true)
                {
                    vnsExceptionBusinessMacro("The reference altitude "<<viefref<<" for viewing direction is null !");
                }
                m_InvVIEHRef = 1. / viefref;
                this->Modified();
            }

            itkGetConstReferenceMacro(InvVIEHRef, double)

            /** Intersector 3D object accessor */
            virtual void
            SetI3D(Intersector3DType* i3d)
            {
                m_I3D = i3d;
                if (m_I3D == NULL)
                {
                    vnsExceptionBusinessMacro("I3D pointer is NULL!");
                }
            }

            const MapLabelObjectType*
            GetShadowMap() const;

            /** Make a DataObject of the correct type to be used as the specified
             * output.
             */
            virtual DataObjectPointer
            MakeOutput(unsigned int idx);

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
            PersistentShadowProjectionWithOnlyLabelImageImageFilter();
            /** Destructor */
            virtual
            ~PersistentShadowProjectionWithOnlyLabelImageImageFilter()
            {
            }

            /** Multi-thread version GenerateData. */
            virtual void
            ThreadedGenerateData(const LabelImageRegionType& outputRegionForThread, itk::ThreadIdType threadId);

            /** PrintSelf method */
            virtual void
            PrintSelf(std::ostream& os, itk::Indent indent) const;

        private:
            PersistentShadowProjectionWithOnlyLabelImageImageFilter(const Self&); //purposely not implemented
            void
            operator=(const Self&); //purposely not implemented

            /** Intersector 3D declaration */
            Intersector3DType* m_I3D;

            /** Reference altitude for viewing direction declaration */
            double m_InvVIEHRef;

            LabelVectorMapType m_ShadowVectorMap;

        };
    // end of class PersistentShadowProjectionWithOnlyLabelImageImageFilter

    /**===========================================================================*/

    /** \class  ShadowProjectionWithOnlyLabelImageImageFilter
     * \brief This class computes the location of the shadows of the clouds detected at
     * level 1 using stereoscopy. The output is a map that contains the label value for
     * each shadow pixel.
     *
     * The cloud mask provides the apparent position of clouds for band B5.
     * To compute the shadow projection, it is necessary to compute the real cloud position.
     * The shadow projection is computed with Intersection3D, taking into account the DTM.
     *
     * All the input images are otb::Image except the direction grids which are otb::VectorImage.
     *
     *
     * \author CS Systemes d'Information
     *
     * \sa vnsPersistentShadowProjectionWithOnlyLabelImageImageFilter
     * \sa otbPersistentImageFilter
     * \sa otbPersistentFilterStreamingDecorator
     *
     * \ingroup L2
     *
     */

    template<class TLabelImage, class TDTMImage, class TGRIDImage, class TCLAImage>
        class ShadowProjectionWithOnlyLabelImageImageFilter : public otb::PersistentFilterStreamingDecorator<
                PersistentShadowProjectionWithOnlyLabelImageImageFilter<TLabelImage, TDTMImage, TGRIDImage, TCLAImage> >
        {
        public:
            /** Standard Self typedef */
            typedef ShadowProjectionWithOnlyLabelImageImageFilter Self;
            typedef otb::PersistentFilterStreamingDecorator<
                    PersistentShadowProjectionWithOnlyLabelImageImageFilter<TLabelImage, TDTMImage, TGRIDImage, TCLAImage> > Superclass;
            typedef itk::SmartPointer<Self> Pointer;
            typedef itk::SmartPointer<const Self> ConstPointer;

            /** Type macro */
            itkNewMacro(Self)

            /** Creation through object factory macro */
            itkTypeMacro(ShadowProjectionWithOnlyLabelImageImageFilter,PersistentFilterStreamingDecorator)

            typedef typename Superclass::FilterType StatFilterType;

            /** Some convenient typedefs. */
            typedef TLabelImage LabelImageType;
            typedef typename LabelImageType::Pointer LabelImagePointer;
            typedef typename LabelImageType::ConstPointer LabelImageConstPointer;
            typedef typename LabelImageType::PixelType LabelImagePixelType;
            typedef typename LabelImageType::RegionType RegionType;
            typedef typename LabelImageType::SizeType SizeType;

            typedef TDTMImage DTMImageType;
            typedef typename DTMImageType::Pointer DTMImagePointer;
            typedef typename DTMImageType::ConstPointer DTMImageConstPointer;
            typedef typename DTMImageType::PixelType DTMImagePixelType;

            typedef TGRIDImage GRIDImageType;
            typedef typename GRIDImageType::Pointer GRIDImagePointer;
            typedef typename GRIDImageType::ConstPointer GRIDImageConstPointer;
            typedef typename GRIDImageType::PixelType GRIDImagePixelType;
            typedef typename GRIDImageType::IndexType IndexType;
            typedef typename GRIDImageType::PointType PointType;
            typedef typename GRIDImageType::SpacingType SpacingType;

            typedef TCLAImage CLAImageType;
            typedef typename CLAImageType::Pointer CLAImagePointer;
            typedef typename CLAImageType::ConstPointer CLAImageConstPointer;
            typedef typename CLAImageType::PixelType CLAImagePixelType;

            typedef DEMDataManager<DTMImageType> Intersector3DType;

            /** Containers typdefs from PersistentShadowProjectionWithOnlyLabelImageImageFilter */
            typedef typename StatFilterType::LabelMapType LabelMapType;

            /** Getters/Setters **/
            void
            SetL2CLDSubLabelInput(const LabelImageType * image)
            {
                this->GetFilter()->SetL2CLDSubLabelInput(image);
            }
            LabelImageType*
            GetL2CLDSubLabelInput()
            {
                return this->GetFilter()->GetL2CLDSubLabelInput();
            }

            void
            SetDTMInput(const DTMImageType * image)
            {
                this->GetFilter()->SetDTMInput(image);
            }
            DTMImageType*
            GetDTMInput()
            {
                return this->GetFilter()->GetDTMInput();
            }

            void
            SetIPVIEInput(const GRIDImageType * image)
            {
                this->GetFilter()->SetIPVIEInput(image);
            }
            GRIDImageType*
            GetIPVIEInput()
            {
                return this->GetFilter()->GetIPVIEInput();
            }

            void
            SetIPSOLInput(const GRIDImageType * image)
            {
                this->GetFilter()->SetIPSOLInput(image);
            }
            GRIDImageType*
            GetSOLInput()
            {
                return this->GetFilter()->GetIPSOLInput();
            }

//            void
//            SetIPCLAInput(const CLAImageType * image)
//            {
//                this->GetFilter()->SetIPCLAInput(image);
//            }
            CLAImageType*
            GetIPCLAInput()
            {
                return this->GetFilter()->GetIPCLAInput();
            }

            /** Set the viewing reference altitude.*/
            void
            SetVIEHRef(const double & val)
            {
                this->GetFilter()->SetVIEHRef(val);
            }
            double
            GetVIEHRef()
            {
                return this->GetFilter()->GetVIEHRef();
            }
            /** Set the viewing reference altitude.*/
            void
            SetSOLHRef(const double & val)
            {
                this->GetFilter()->SetSOLHRef(val);
            }
            double
            GetSOLHRef()
            {
                return this->GetFilter()->GetSOLHRef();
            }
            /** Set the viewing reference altitude.*/
            void
            SetDeltaH(const double & val)
            {
                this->GetFilter()->SetDeltaH(val);
            }
            double
            GetDeltaH()
            {
                return this->GetFilter()->GetDeltaH();
            }
            /** Set L2 Coarse Resolution.*/
            void
            SetL2CoarseResolution(const unsigned int & res)
            {
                this->GetFilter()->SetL2CoarseResolution(res);
            }
            unsigned int
            GetL2CoarseResolution()
            {
                return this->GetFilter()->GetL2CoarseResolution();
            }

            /** Intersector 3D object accessor */
            virtual void
            SetI3D(Intersector3DType* i3d)
            {
                this->GetFilter()->SetI3D(i3d);
            }

            // Outputs
            const LabelMapType
            GetShadowMap() const
            {
                return this->GetFilter()->GetShadowMap()->Get();
            }

            // L1 spacing accessors
            void
            SetL1Spacing(SpacingType sp)
            {
                this->GetFilter()->SetL1Spacing(sp);
                this->Modified();
            }

            SpacingType
            GetL1Spacing()
            {
                return this->GetFilter()->GetL1Spacing();
            }

        protected:
            /** Constructor */
            ShadowProjectionWithOnlyLabelImageImageFilter()
            {
            }

            /** Destructor */
            virtual
            ~ShadowProjectionWithOnlyLabelImageImageFilter()
            {
            }

            /** PrintSelf method */
            virtual void
            PrintSelf(std::ostream& os, itk::Indent indent) const;

        private:
            ShadowProjectionWithOnlyLabelImageImageFilter(const Self&); //purposely not implemented
            void
            operator=(const Self&); //purposely not implemented

        };

} // End namespace vns

#ifndef VNS_MANUAL_INSTANTIATION
#include "vnsShadowProjectionWithOnlyLabelImageImageFilter.txx"
#endif

#endif /* __vnsShadowProjectionWithOnlyLabelImageImageFilter_h */
