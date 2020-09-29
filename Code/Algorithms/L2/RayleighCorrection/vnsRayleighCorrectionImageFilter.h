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
 * VERSION : 1.0.0 : FA : LAIG-FA-MAC-1988-CNES : 21 octobre 2016 : Correction qualite (code mort)          *
 * VERSION : 5-1-0 : FA : LAIG-FA-MAC-145739-CS : 27 juin 2016 : Audit code - Supp de la macro ITK_EXPORT   *
 * VERSION : 5-0-0 : FA : LAIG-FA-MAC-143764-CS : 12 avril 2016 : Mise en forme du code (indentation, etc.) *
 * VERSION : 1-0-0 : <TypeFT> : <NumFT> : 19 juil. 2010 : Creation                                                           
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id$
 *                                                                                                          *
 ************************************************************************************************************/
#ifndef __vnsRayleighCorrectionImageFilter_h
#define __vnsRayleighCorrectionImageFilter_h

#include "vnsMacro.h"
#include "itkImageToImageFilter.h"

#include "vnsVectorLookUpTable.h"
#include "otbImage.h"


namespace vns
{
    /** \class  RayleighCorrectionImageFilter
     * \brief This class computes the surface reflectance using the atmospheric LUT
     * and a default value for the aerosol optical thickness.
     *
     * Inputs are:
     * \begin{itemize}
     *    \item TOAC image (otb::VectorImage)
     *    \item EDG image (otb::Image)
     *    \item DTM image (otb::Image)
     *    \item LUT (vns::VectorLookUpTable)
     *  \end{itemize}
     *
     * Output is TOC (or NSL) image (otb::VectorImage).
     *
     * \author CS Systemes d'Information
     *
     * \sa ImageToImageFilter
     *
     * \ingroup L2
     * \ingroup L3
     * \ingroup Checktool
     *
     */
        class RayleighCorrectionImageFilter : public itk::ImageToImageFilter<VNSRealVectorImageType, VNSRealVectorImageType>
        {
        public:
            /** Standard class typedefs. */
            typedef RayleighCorrectionImageFilter Self;
            typedef itk::ImageToImageFilter<VNSRealVectorImageType, VNSRealVectorImageType> Superclass;
            typedef itk::SmartPointer<Self> Pointer;
            typedef itk::SmartPointer<const Self> ConstPointer;

            /** Type macro */
            itkNewMacro(Self)

            /** Creation through object factory macro */
            itkTypeMacro(RayleighCorrectionImageFilter, itk::ImageToImageFilter)

            /** Some convenient typedefs. */
            /** TOAC input image typedefs. */
            typedef Superclass::InputImageType InputImageType;
            typedef InputImageType::ConstPointer InputImageConstPointer;
            typedef InputImageType::Pointer InputImagePointer;
            typedef InputImageType::PixelType InputImagePixelType;
            typedef InputImageType::InternalPixelType InputInternalPixelType;

            /** Edge input mask typedefs. */
            typedef VNSMaskImageType InputMaskType;
            typedef InputMaskType::ConstPointer InputMaskConstPointer;
            typedef InputMaskType::PixelType InputMaskPixelType;
            typedef InputMaskType::InternalPixelType InputMaskInternalPixelType;

            /** Altitude input image typedefs. */
            typedef VNSDTMImageType DTMImageType;
            typedef DTMImageType::ConstPointer DTMImageConstPointer;
            typedef DTMImageType::PixelType DTMImagePixelType;
            typedef DTMImageType::InternalPixelType DTMImageInternalPixelType;

            /** LUT image typedef */
            typedef VNSLUT3DType LUTType;
            typedef LUTType::Pointer LUTPointer;
            typedef LUTType::ConstPointer LUTConstPointer;
            typedef LUTType::PixelType LUTPixelType;
            typedef LUTType::InternalPixelType LUTInternalPixelType;
            typedef LUTType::PointType LUTPointType;
            typedef LUTPointType::ValueType LUTPointValueType;

            /** Output image typedefs. */
            typedef VNSRealVectorImageType OutputImageType;
            typedef OutputImageType::Pointer OutputImagePointer;
            typedef OutputImageType::ConstPointer OutputImageConstPointer;
            typedef OutputImageType::PixelType OutputImagePixelType;
            typedef OutputImageType::InternalPixelType OutputImageInternalPixelType;
            typedef OutputImageType::RegionType OutputImageRegionType;

            /** Set the TOAC reflectance image */
            vnsSetGetInputMacro( IPTOAC, InputImageType, 0)
            /** Set the edges mask */
            vnsSetGetInputMacro( IPEDG, InputMaskType, 1)
            /** Set the DTM image */
            vnsSetGetInputMacro( DTM, DTMImageType, 2)

            /** Set the LUT image */
            itkSetObjectMacro(LUT, LUTType)
            /** Get the DTM image */
            itkGetConstObjectMacro(LUT, LUTType)

            /**  Set default AOT value. */
            itkSetMacro(DefaultAOT, double)
            /**  Get default AOT value. */
            itkGetConstMacro(DefaultAOT, double)

            /** No_data pixel value accessors. */
            itkSetMacro(NoData, RealNoDataType)
            ;itkGetConstReferenceMacro(NoData, RealNoDataType)
            ;

        protected:
            /** Constructor */
            RayleighCorrectionImageFilter();
            /** Destructor */
            virtual
            ~RayleighCorrectionImageFilter();
            /** PrintSelf method */
            virtual void
            PrintSelf(std::ostream& os, itk::Indent indent) const;

            /** BeforeThreaded generate data. */
            virtual void
            BeforeThreadedGenerateData(void);

            /** Generate output information. */
            virtual void
            GenerateOutputInformation(void);

            /** Composite Filter method. */
            virtual void
            ThreadedGenerateData(const OutputImageRegionType& outputRegionForThread, itk::ThreadIdType threadId);

        private:
            void
            operator=(const Self&); //purposely not implemented

            /** Surface reflectance LUT */
            LUTType::Pointer m_LUT;
            /** Default AOT value */
            double m_DefaultAOT;
            /** NoData value. */
            RealNoDataType m_NoData;

        };

} // End namespace vns

#endif /* __vnsRayleighCorrectionImageFilter_h */
