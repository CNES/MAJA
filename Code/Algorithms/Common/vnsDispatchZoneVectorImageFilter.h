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
 * HISTORIQUE                                                                                               *
 *                                                                                                          *
 * VERSION : 5-1-0 : FA : LAIG-FA-MAC-145739-CS : 27 juin 2016 : Audit code - Supp de la macro ITK_EXPORT   *
 * VERSION : 2-1-0 : FA : LAIG-FA-MAC-354-CNES : 8 juin 2012 : Correction qualite (regle ou metrique)       *
 *                                                                                                          *
 * MOD : VERSION : 1-0-0  : 24 nov. 2011 : Creation                                                           
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id$
 *                                                                                                          *
 ************************************************************************************************************/
#ifndef __vnsDispatchZoneVectorImageFilter_h
#define __vnsDispatchZoneVectorImageFilter_h

#include "itkInPlaceImageFilter.h"
#include "vnsUtilities.h"
#include "vnsMacro.h"

namespace vns
{
    /** \class  ConcatenatePerZoneVectorImageFilter
     * \brief This class takes a list of vector image and a mask of zone.
     * for each zone, the output value is the value of the input image corresponding to the zone.
     *
     *
     *
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
    template<class TInputImage, class TOutputImage>
        class DispatchZoneVectorImageFilter : public itk::InPlaceImageFilter<TInputImage, TOutputImage>
        {
        public:
            /** Standard class typedefs. */
            typedef DispatchZoneVectorImageFilter Self;
            typedef itk::ImageToImageFilter<TInputImage, TOutputImage> Superclass;
            typedef itk::SmartPointer<Self> Pointer;
            typedef itk::SmartPointer<const Self> ConstPointer;

            /** Type macro */
            itkNewMacro(Self)
            /** Creation through object factory macro */
            itkTypeMacro(DispatchZoneVectorImageFilter, ImageToImageFilter )

            /** Some convenient typedefs. */
            typedef TInputImage InputImageType;
            typedef otb::ObjectList<InputImageType> InputImageListType;
            typedef typename InputImageListType::ConstPointer InputImageListConstPointer;
            typedef typename InputImageType::ConstPointer InputImageConstPointer;
            typedef typename InputImageType::RegionType RegionType;
            typedef typename InputImageType::PixelType InputImagePixelType;
            typedef typename InputImageType::InternalPixelType InputImageInternalPixelType;
            typedef typename InputImageType::SizeType SizeType;
            typedef typename InputImageType::SizeValueType SizeValueType;
            typedef TOutputImage OutputImageType;
            typedef typename OutputImageType::RegionType OutputImageRegionType;
            typedef typename OutputImageType::Pointer OutputImagePointer;
            typedef typename OutputImageType::PixelType OutputImagePixelType;
            typedef typename OutputImageType::InternalPixelType OutputImageInternalPixelType;
            typedef std::vector< OutputImageInternalPixelType >   NaryArrayType;
            typedef itk::ImageRegionConstIterator<InputImageType> InputImageConstIteratorType;
            typedef itk::ImageRegionIterator<OutputImageType> OutputImageIteratorType;

            itkSetMacro(NumberOfComponentsPerPixel, unsigned int)
            itkGetConstReferenceMacro(NumberOfComponentsPerPixel, unsigned int)
			itkSetMacro(ImageOutputValueList, std::vector<OutputImageInternalPixelType>)
			itkSetMacro(BitWeights,std::vector<std::vector<OutputImageInternalPixelType> >)

        protected:
            /** Constructor */
            DispatchZoneVectorImageFilter();
            /** Destructor */
            virtual
            ~DispatchZoneVectorImageFilter();
            /** PrintSelf method */
            virtual void
            PrintSelf(std::ostream& os, itk::Indent indent) const;

            /** Check the pixel type size */
            void BeforeThreadedGenerateData();
            /** Multi-thread version GenerateData. */
            void ThreadedGenerateData (const RegionType& outputRegionForThread, itk::ThreadIdType threadId);

            virtual void
            GenerateOutputInformation();

        private:
            DispatchZoneVectorImageFilter(const Self&); //purposely not implemented
            void
            operator=(const Self&); //purposely not implemented

            std::vector<OutputImageInternalPixelType> m_ImageOutputValueList;

            /** Number of value in the input image file (number of component by pixel for the output vector image) */
            unsigned int m_NumberOfComponentsPerPixel;

            /** Pixel byte weight */
            std::vector<std::vector<OutputImageInternalPixelType> > m_BitWeights;
        };

} // End namespace vns
#ifndef VNS_MANUAL_INSTANTIATION
#include "vnsDispatchZoneVectorImageFilter.txx"
#endif

#endif /* __vnsDispatchZoneVectorImageFilter_h */
