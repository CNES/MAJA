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
 * VERSION : 1-0-0-3 : DM : 357 : 31 août. 2011 : pas de correlation avec la date courante                    *
 * VERSION : 1-0-0 : <TypeFT> : <NumFT> : 7 juin 2010 : Creation                                                           
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id$
 *                                                                                                          *
 ************************************************************************************************************/
#ifndef __vnsShadowVariationThresholdImageFilter_h
#define __vnsShadowVariationThresholdImageFilter_h

#include "itkImageToImageFilter.h"
#include "itkImageRegionConstIterator.h"
#include "vnsMacro.h"
#include "vnsUtilities.h"

namespace vns
{
    /** \class  ShadowVariationThresholdImageFilter
     * \brief This class detects shadows caused by clouds outside the image.
     *
     * WWhen a ratio of the reflectance of date D and the composite image of date D-1 is below
     * a threshold (for the red band), the pixel is considered as a shadow.
     * The threshold increase with time delay between the image acquisition and the reference
     * reflectance date.
     * The detection is not activated above water surfaces to avoid false detection of shadows.
     * It is only activated in the zone where clouds outside the image could cast shadows within image.
     *
     * The input images are otb::VectorImage (images of reflectance).
     * The input mask and the image of date are otb::Image.
     * The output mask is an otb::Image.
     *
     * \author CS Systemes d'Information
     *
     * \sa ImageToImageFilter
     *
     * \ingroup L2
     *
     */
    template<class TInputImage, class TInputMask, class TOutputImage>
        class ShadowVariationThresholdImageFilter : public itk::ImageToImageFilter<TInputImage, TOutputImage>
        {
        public:
            /** Standard class typedefs. */
            typedef ShadowVariationThresholdImageFilter Self;
            typedef itk::ImageToImageFilter<TInputImage, TOutputImage> Superclass;
            typedef itk::SmartPointer<Self> Pointer;
            typedef itk::SmartPointer<const Self> ConstPointer;

            /** Type macro */
            itkNewMacro(Self)

            /** Creation through object factory macro */
            itkTypeMacro(ShadowVariationThresholdImageFilter, ImageToImageFilter )

            /** Some convenient typedefs. */
            typedef typename Superclass::InputImageType InputImageType;
            typedef typename InputImageType::ConstPointer InputImageConstPointer;
            typedef typename InputImageType::Pointer InputImagePointer;
            typedef typename InputImageType::RegionType RegionType;
            typedef typename InputImageType::PixelType InputImagePixelType;
            typedef typename InputImageType::SizeType SizeType;
            typedef typename InputImageType::InternalPixelType InputInternalPixelType;

            typedef TInputMask InputMaskType;
            typedef typename InputMaskType::Pointer InputMaskPointer;
            typedef typename InputMaskType::ConstPointer InputMaskConstPointer;
            typedef typename InputMaskType::PixelType InputMaskPixelType;

            typedef typename Superclass::OutputImageType OutputImageType;
            typedef typename OutputImageType::Pointer OutputImagePointer;
            typedef typename OutputImageType::RegionType OutputImageRegionType;
            typedef typename OutputImageType::PixelType OutputImagePixelType;

            // No_data pixel value accessors
            itkSetMacro(NoData, RealNoDataType)
            itkGetConstReferenceMacro(NoData, RealNoDataType)

            // CorrelThreshold parameter accesors
            itkSetMacro(ThresholdValue, double)
            itkGetConstReferenceMacro(ThresholdValue, double)

            /** Set the image of the surface reflectance ratio*/
            vnsSetGetInputRawMacro( ReflRatio, InputImageType ,0)
            /** Set the outside cloud shadow mask */
            vnsSetGetInputRawMacro( IPCLDOutShadInput , InputMaskType , 1)

            /** Get the shadows mask from clouds outside image */
            OutputImageType*
            GetIPCLDShadVarOutput()
            {
                return static_cast<OutputImageType*>(this->itk::ProcessObject::GetOutput(0));
            }

        protected:
            /** Constructor */
            ShadowVariationThresholdImageFilter();
            /** Destructor */
            virtual
            ~ShadowVariationThresholdImageFilter();

            /** Multi-thread version GenerateData. */
            void
            ThreadedGenerateData(const OutputImageRegionType& outputRegionForThread, itk::ThreadIdType threadId);

            /** PrintSelf method */
            virtual void
            PrintSelf(std::ostream& os, itk::Indent indent) const;

        private:
            ShadowVariationThresholdImageFilter(const Self&); //purposely not implemented
            void
            operator=(const Self&); //purposely not implemented

            /** No_data value */
            RealNoDataType m_NoData;

            /** Declaration of the threshold on reflectance ratio */
            double m_ThresholdValue;

        };

} // End namespace vns
#ifndef VNS_MANUAL_INSTANTIATION
#include "vnsShadowVariationThresholdImageFilter.txx"
#endif

#endif /* __vnsShadowVariationThresholdImageFilter_h */
