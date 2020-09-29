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
 * VERSION : 1-0-0 : <TypeFT> : <NumFT> : 9 juil 2010 : Creation
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id$
 *                                                                                                          *
 ************************************************************************************************************/
#ifndef __vnsShadowMaskProjection_h
#define __vnsShadowMaskProjection_h

#include "itkImageSource.h"
#include "itkNumericTraits.h"

#include "vnsMacro.h"

namespace vns
{
    /** \class ShadowMaskProjection
     * \brief This class generates a mask of shadow pixels that contains the label value
     *  instead of generating a binary mask (0 or 1).
     *  A map that contains the list of shadow pixel and their associated clous label value
     *  is used to generate this final shadow mask. This map is producted by the
     *  vns::ShadowProjectionImageFilter.
     *
     * \author CS Systemes d'Information
     *
     * \sa ImageToImageFilter
     *
     * \ingroup L2
     *
     */
    template<typename TOutputImage>
        class ShadowMaskProjection : public itk::ImageSource<TOutputImage>
        {
        public:
            /** Standard class typedefs. */
            typedef ShadowMaskProjection Self;
            typedef itk::ImageSource<TOutputImage> Superclass;
            typedef itk::SmartPointer<Self> Pointer;
            typedef itk::SmartPointer<const Self> ConstPointer;

            /** Typedef for the output image PixelType. */
            typedef typename TOutputImage::PixelType OutputImagePixelType;

            /** Typedef to describe the output image region type. */
            typedef typename TOutputImage::RegionType OutputImageRegionType;

            /** Run-time type information (and related methods). */
            itkTypeMacro(ShadowMaskProjection,ImageSource)

            /** Method for creation through the object factory. */
            itkNewMacro(Self)

            /** Basic types from the OutputImageType */
            typedef typename TOutputImage::Pointer OutputImagePointer;
            typedef typename TOutputImage::SizeType SizeType;
            typedef typename TOutputImage::IndexType IndexType;
            typedef typename TOutputImage::SpacingType SpacingType;
            typedef typename TOutputImage::PointType PointType;
            typedef typename SizeType::SizeValueType SizeValueType;
            typedef SizeValueType SizeValueArrayType[TOutputImage::ImageDimension];
            typedef typename TOutputImage::SpacingValueType SpacingValueType;
            typedef SpacingValueType SpacingValueArrayType[TOutputImage::ImageDimension];
            typedef typename TOutputImage::PointValueType PointValueType;
            typedef PointValueType PointValueArrayType[TOutputImage::ImageDimension];

            /** Result map typedef */
            typedef unsigned long LinearIndex;
            typedef unsigned short LabelImagePixelType;
            typedef typename std::map<LinearIndex, LabelImagePixelType> LabelMapType;

            /** Set/Get size of the output image */
            itkSetMacro( Size, SizeType )

            virtual void
            SetSize(SizeValueArrayType sizeArray);
            virtual const SizeValueType *
            GetSize() const;

            /** Set/Get spacing of the output image */
            itkSetMacro( Spacing, SpacingType )

            virtual void
            SetSpacing(SpacingValueArrayType spacingArray);
            virtual const SpacingValueType *
            GetSpacing() const;

            /** Set/Get origin of the output image */
            itkSetMacro( Origin, PointType )

            virtual void
            SetOrigin(PointValueArrayType originArray);
            virtual const PointValueType *
            GetOrigin() const;

            // Reference altitude for sun direction accessors
            vnsSetMacro(ShadowMap, LabelMapType)
            vnsGetConstMacro(ShadowMap, LabelMapType)

            /** Get the shadow mask for cloud within the image */
            TOutputImage *
            GetIPCLDShadOutput()
            {
                return static_cast<TOutputImage*>(this->GetOutput(0));
            }

        protected:
            ShadowMaskProjection();
            virtual
            ~ShadowMaskProjection();
            void
            PrintSelf(std::ostream& os, itk::Indent indent) const;

            virtual void
            ThreadedGenerateData(const OutputImageRegionType& outputRegionForThread, itk::ThreadIdType threadId);
            virtual void
            GenerateOutputInformation();

        private:
            ShadowMaskProjection(const ShadowMaskProjection&); //purposely not implemented
            void
            operator=(const ShadowMaskProjection&); //purposely not implemented

            SizeType m_Size;    //size of the output image
            SpacingType m_Spacing; //spacing
            PointType m_Origin;  //origin

            // The following variables are deprecated, and provided here just for
            // backward compatibility. It use is discouraged.
            mutable PointValueArrayType m_OriginArray;
            mutable SpacingValueArrayType m_SpacingArray;

            /** Output map declaration */
            LabelMapType m_ShadowMap;

        };

} // end namespace vns

#ifndef VNS_MANUAL_INSTANTIATION
#include "vnsShadowMaskProjection.txx"
#endif

#endif // __vnsShadowMaskProjection_h
