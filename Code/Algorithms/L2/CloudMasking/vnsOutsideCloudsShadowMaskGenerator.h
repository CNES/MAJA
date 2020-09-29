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
 * VERSION : 1-0-0 : <TypeFT> : <NumFT> : 29 juin 2010 : Creation                                                           
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id$
 *                                                                                                          *
 ************************************************************************************************************/
#ifndef __vnsOutsideCloudsShadowMaskGenerator_h
#define __vnsOutsideCloudsShadowMaskGenerator_h

#include "itkImageToImageFilter.h"

#include "vnsDEMDataManager.h"
#include "vnsMacro.h"

namespace vns
{
    /** \class  OutsideCloudsShadowMaskGenerator
     * \brief This class find the zone where the clouds lying outside the image
     * could cast shadow.
     *
     * This class uses intersector3D to project the input image and get the real
     * position of clouds.
     *
     * The input images are the edge mask, the solar and viewing grids and the DTM.
     *
     * \author CS Systemes d'Information
     *
     * \sa ImageToImageFilter
     *
     * \ingroup L2
     *
     */
    template<class TInputMask, class TDTMImage, class TGRIDImage, class TOutputImage>
        class OutsideCloudsShadowMaskGenerator : public itk::ImageToImageFilter<TInputMask, TOutputImage>
        {
        public:
            /** Standard class typedefs. */
            typedef OutsideCloudsShadowMaskGenerator Self;
            typedef itk::ImageToImageFilter<TInputMask, TOutputImage> Superclass;
            typedef itk::SmartPointer<Self> Pointer;
            typedef itk::SmartPointer<const Self> ConstPointer;

            /** Type macro */
            itkNewMacro(Self)
            /** Creation through object factory macro */
            itkTypeMacro(OutsideCloudsShadowMaskGenerator, ImageToImageFilter )

            /** Some convenient typedefs. */
            typedef typename Superclass::InputImageType InputMaskType;
            typedef typename InputMaskType::ConstPointer InputMaskConstPointer;
            typedef typename InputMaskType::RegionType RegionType;
            typedef typename InputMaskType::PixelType InputMaskPixelType;
            typedef typename InputMaskType::SizeType SizeType;

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

            typedef typename Superclass::OutputImageType OutputImageType;
            typedef typename OutputImageType::Pointer OutputImagePointer;
            typedef typename OutputImageType::RegionType OutputImageRegionType;
            typedef typename OutputImageType::PixelType OutputImagePixelType;

            typedef DEMDataManager<DTMImageType> Intersector3DType;


            // Max cloud altitude accessors
            itkSetMacro(MaxCLA, double)
            itkGetConstReferenceMacro(MaxCLA, double)

            // Reference altitude for viewing direction accessors
            itkSetMacro(VIEHRef, double)
            itkGetConstReferenceMacro(VIEHRef, double)

            // Reference altitude for sun direction accessors
            itkSetMacro(SOLHRef, double)
            itkGetConstReferenceMacro(SOLHRef, double)

            /** Set the edges mask (No_Data) */
            vnsSetGetInputMacro( IPEDG , InputMaskType , 0)
            /** Set the DTM image */
            vnsSetGetInputMacro( DTM , DTMImageType , 1)
            /** Set the viewing direction grid */
            vnsSetGetInputMacro( IPVIE , GRIDImageType , 3 )
            /** Set the solar direction grid */
            vnsSetGetInputMacro( IPSOL , GRIDImageType , 2 )

            /** Get the outside clouds shadow mask */
            OutputImageType*
            GetIPCLDOutShadOutput()
            {
                return static_cast<OutputImageType*>(this->GetOutput(0));
            }

        protected:
            /** Constructor */
            OutsideCloudsShadowMaskGenerator();
            /** Destructor */
            virtual
            ~OutsideCloudsShadowMaskGenerator();

            /** Load the DTM and get the translation with the projected center point */
            virtual void
            GenerateOutputInformation();

            /** Multi-thread version GenerateData. */
            void
            ThreadedGenerateData(const OutputImageRegionType& outputRegionForThread, itk::ThreadIdType threadId);

            /** PrintSelf method */
            virtual void
            PrintSelf(std::ostream& os, itk::Indent indent) const;

        private:
            OutsideCloudsShadowMaskGenerator(const Self&); //purposely not implemented
            void
            operator=(const Self&); //purposely not implemented

            /** Reference altitude for viewing direction declaration */
            double m_VIEHRef;
            /** Reference altitude for solar direction declaration */
            double m_SOLHRef;
            /** Maximum cloud altitude declaration */
            double m_MaxCLA;

            // Intersector 3D
            Intersector3DType m_I3D;

            IndexType m_Translation;

        };

} // End namespace vns
#ifndef VNS_MANUAL_INSTANTIATION
#include "vnsOutsideCloudsShadowMaskGenerator.txx"
#endif

#endif /* __vnsOutsideCloudsShadowMaskGenerator_h */
