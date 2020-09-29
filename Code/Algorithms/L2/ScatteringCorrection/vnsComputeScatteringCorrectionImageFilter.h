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
 * VERSION : 5-1-0 : FA : LAIG-FA-MAC-144674-CS : 23 mai 2016 : Correction refactoring, menage, etc...      *
 * VERSION : 2-0-0 : DM : LAIG-DM-MAC-146-CNES : 08 novembre 2011 : Modification pour prise en compte            *
 * VERSION : 1-0-0 : <TypeFT> : <NumFT> : 8 avr. 2010 : Creation                                                           
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id$
 *                                                                                                          *
 ************************************************************************************************************/
#ifndef __vnsComputeScatteringCorrectionImageFilter_h
#define __vnsComputeScatteringCorrectionImageFilter_h

#include "vnsMacro.h"
#include "itkImageToImageFilter.h"

#include "vnsVectorLookUpTable.h"
#include "vnsUtilities.h"
#include "otbImage.h"
#include "otbObjectList.h"
#include <map>


namespace vns
{
    /** \class  ComputeScatteringCorrectionImageFilter
     * \brief This class computes the surface reflectance using the atmospheric LUT..
     *
     * Inputs are:
     * \begin{itemize}
     *    \item TOAC image (otb::VectorImage)
     *    \item AOT image (otb::Image)
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
    template<class TTOACInputImage, class TAOTInputImage, class TDTMInputImage, class TInputMask, class TVectorMask, class TOutputImage,
            class TLUTInput = vns::VectorLookUpTable<double, 3> >
        class ComputeScatteringCorrectionImageFilter : public itk::ImageToImageFilter<TTOACInputImage, TOutputImage>
        {
        public:
            /** Standard class typedefs. */
            typedef ComputeScatteringCorrectionImageFilter Self;
            typedef itk::ImageToImageFilter<TTOACInputImage, TOutputImage> Superclass;
            typedef itk::SmartPointer<Self> Pointer;
            typedef itk::SmartPointer<const Self> ConstPointer;

            /** Type macro */
            itkNewMacro(Self)

            /** Creation through object factory macro */
            itkTypeMacro(ComputeScatteringCorrectionImageFilter, itk::ImageToImageFilter)

            /** Some convenient typedefs. */
            /** TOAC input image typedefs. */
            typedef TTOACInputImage TOACImageType;
            typedef typename TOACImageType::ConstPointer TOACImageConstPointer;
            typedef typename TOACImageType::PixelType TOACImagePixelType;
            typedef typename TOACImageType::InternalPixelType TOACImageInternalPixelType;

            /** AOT input image typedefs. */
            typedef TAOTInputImage AOTImageType;
            typedef typename AOTImageType::ConstPointer AOTImageConstPointer;
            typedef typename AOTImageType::PixelType AOTImagePixelType;

            /** Altitude input image typedefs. */
            typedef TDTMInputImage DTMImageType;
            typedef typename DTMImageType::ConstPointer DTMImageConstPointer;
            typedef typename DTMImageType::PixelType DTMImagePixelType;
            typedef typename DTMImageType::InternalPixelType DTMImageInternalPixelType;

            /** Edge input image typedefs. */
            typedef TInputMask InputMaskType;
            typedef typename InputMaskType::ConstPointer InputMaskConstPointer;
            typedef typename InputMaskType::PixelType InputMaskPixelType;
            typedef typename InputMaskType::InternalPixelType InputMaskInternalPixelType;

            /** Angle zone input mask typedefs. */
            typedef TVectorMask VectorMaskType;
            typedef typename VectorMaskType::ConstPointer VectorMaskConstPointer;
            typedef typename VectorMaskType::PixelType VectorMaskPixelType;
            typedef typename VectorMaskType::InternalPixelType VectorMaskInternalPixelType;

            /** LUT image typedef */
            typedef TLUTInput LUTType;
            typedef typename LUTType::Pointer LUTPointer;
            typedef typename LUTType::ConstPointer LUTConstPointer;
            typedef typename LUTType::PixelType LUTPixelType;
            typedef typename LUTType::PointType LUTPointType;
            typedef typename LUTPointType::ValueType LUTPointValueType;
            typedef std::map<unsigned int, LUTPointer> LUTMapType;

            /** Output image typedefs. */
            typedef TOutputImage OutputImageType;
            typedef typename OutputImageType::Pointer OutputImagePointer;
            typedef typename OutputImageType::ConstPointer OutputImageConstPointer;
            typedef typename OutputImageType::PixelType OutputImagePixelType;
            typedef typename OutputImageType::InternalPixelType OutputImageInternalPixelType;
            typedef typename OutputImageType::RegionType OutputImageRegionType;

            /** Vector of string typedef. */
            //  typedef std::vector<std::string>  ListOfBandType;
            typedef Utilities::ListOfUIntegers ListOfBandIdType;

            /** Set the TOAC reflectance image */
            vnsSetGetInputMacro( TOAC, TOACImageType, 0)
            /** Set the AOT reflectance image */
            vnsSetGetInputMacro( AOT, AOTImageType, 1)
            /** Set the EDG image */
            vnsSetGetInputMacro( EDG, InputMaskType, 2)
            /** Set the DTM image */
            vnsSetGetInputMacro( DTM, DTMImageType, 3)
            /** Set the mask of the angle zone (mask of detector) */
            vnsSetGetInputMacro( AngleZoneMask, VectorMaskType , 4)

            /** Set the LUT image */
            void
            SetLUTMap(const LUTMapType& pLUTMap)
            {
                m_LUTMap = pLUTMap;
            }

            /** Set the List of band present in the input reflectance image */
            vnsSetMacro(ListOfL2CoarseBandId, ListOfBandIdType)

            /** No_data pixel value accessors. */
            itkSetMacro(NoData, RealNoDataType)
            ;itkGetConstReferenceMacro(NoData, RealNoDataType)
            ;

        protected:
            /** Constructor */
            ComputeScatteringCorrectionImageFilter();
            /** Destructor */
            virtual
            ~ComputeScatteringCorrectionImageFilter();
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
            LUTMapType m_LUTMap;

            /** Set the List of band present in the input reflectance image */
            ListOfBandIdType m_ListOfL2CoarseBandId;

            /** NoData value. */
            RealNoDataType m_NoData;

        };

} // End namespace vns
#ifndef VNS_MANUAL_INSTANTIATION
#include "vnsComputeScatteringCorrectionImageFilter.txx"
#endif

#endif /* __vnsComputeScatteringCorrectionImageFilter_h */
