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
 * VERSION : 1-0-0 : <TypeFT> : <NumFT> : 8 juin 2010 : Creation                                                           
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id$
 *                                                                                                          *
 ************************************************************************************************************/
#ifndef __vnsConditionalDifferenceImageFilter_h
#define __vnsConditionalDifferenceImageFilter_h

#include "itkImageToImageFilter.h"
#include  "vnsMacro.h"

namespace vns
{
    /** \class  ConditionalDifferenceImageFilter
     * \brief This class computes the difference between the current image of reflectance and
     * the composite image.The value should be negative when a shadow affects the image
     * of date D.
     *
     * \author CS Systemes d'Information
     *
     * \sa ImageToImageFilter
     *
     * \ingroup L2
     *
     */
    template<class TInputImage, class TInputMask, class TOutputImage>
        class ConditionalDifferenceImageFilter : public itk::ImageToImageFilter<TInputImage, TOutputImage>
        {
        public:
            /** Standard class typedefs. */
            typedef ConditionalDifferenceImageFilter Self;
            typedef itk::ImageToImageFilter<TInputImage, TOutputImage> Superclass;
            typedef itk::SmartPointer<Self> Pointer;
            typedef itk::SmartPointer<const Self> ConstPointer;

            /** Type macro */
            itkNewMacro(Self)
            /** Creation through object factory macro */
            itkTypeMacro(ConditionalDifferenceImageFilter, ImageToImageFilter )

            /** Some convenient typedefs. */
            typedef typename Superclass::InputImageType InputImageType;
            typedef typename InputImageType::ConstPointer InputImageConstPointer;
            typedef typename InputImageType::RegionType RegionType;
            typedef typename InputImageType::PixelType InputImagePixelType;
            typedef typename InputImageType::InternalPixelType InputInternalPixelType;
            typedef typename InputImageType::SizeType SizeType;

            typedef TInputMask InputMaskType;
            typedef typename InputMaskType::Pointer InputMaskPointer;
            typedef typename InputMaskType::ConstPointer InputMaskConstPointer;
            typedef typename InputMaskType::PixelType InputMaskPixelType;

            typedef typename Superclass::OutputImageType OutputImageType;
            typedef typename OutputImageType::Pointer OutputImagePointer;
            typedef typename OutputImageType::RegionType OutputImageRegionType;
            typedef typename OutputImageType::PixelType OutputImagePixelType;

            // No_Data value accessors
            itkSetMacro(NoData, RealNoDataType)
            itkGetConstReferenceMacro(NoData, RealNoDataType)

            // Shadow band accessors for the TOCR
            itkSetMacro(ShadowBandTOCR, unsigned int)
            itkGetConstReferenceMacro(ShadowBandTOCR, unsigned int)

            // Shadow band accessors for the RCR
            itkSetMacro(ShadowBandRCR, unsigned int)
            itkGetConstReferenceMacro(ShadowBandRCR, unsigned int)

            /** Set the image of the surface reflectance corrected by absorption and rayleigh */
            vnsSetGetInputMacro( IPTOCR, InputImageType, 0)
            /** Set the image of the surface reflectance corrected by absorption and rayleigh
             * of the composite product */
            vnsSetGetInputMacro( L2inRCR, InputImageType, 1)
            /** Set the edge mask of the input product */
            vnsSetGetInputMacro( IPEDG, InputMaskType, 2)
            /** Set the no_data mask of the composite product */
            vnsSetGetInputMacro( L2inNDT,InputMaskType , 3)
            /** Set the water mask of the composite product */
            vnsSetGetInputMacro(L2inWAS , InputMaskType, 4)
            /** Set the cloud mask product */
            vnsSetGetInputMacro( L2CLDSub, InputMaskType, 5)

        protected:
            /** Constructor */
            ConditionalDifferenceImageFilter();
            /** Destructor */
            virtual
            ~ConditionalDifferenceImageFilter();

            /** Multi-thread version GenerateData. */
            void
            ThreadedGenerateData(const OutputImageRegionType& outputRegionForThread, itk::ThreadIdType threadId);

            /** PrintSelf method */
            virtual void
            PrintSelf(std::ostream& os, itk::Indent indent) const;

        private:
            ConditionalDifferenceImageFilter(const Self&); //purposely not implemented
            void
            operator=(const Self&); //purposely not implemented

            /** Parameter declaration */
            /** No_Data value declaration */
            RealNoDataType m_NoData;
            /** Shadow band declaration for TOCR */
            unsigned int m_ShadowBandTOCR;
            /** Shadow band declaration for RCR */
            unsigned int m_ShadowBandRCR;

        };

} // End namespace vns
#ifndef VNS_MANUAL_INSTANTIATION
#include "vnsConditionalDifferenceImageFilter.txx"
#endif

#endif /* __vnsConditionalDifferenceImageFilter_h */
