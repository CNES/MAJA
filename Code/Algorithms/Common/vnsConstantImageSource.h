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
 * VERSION : 5-0-0 : FA : LAIG-FA-MAC-143764-CS : 25 avril 2016 : Ajout methode pour positionner parametres *
 *                                                                                 avec une image en entree *
 * VERSION : 3-2-0 : FA : LAIG-FA-MAC-701-CNES : 15 novembre 2013 : Correction information géographique     *
 * 								Ajout de la projection dans l'image de sortie								*
 * VERSION : 1-0-0 : <TypeFT> : <NumFT> : 20 mai 2010 : Creation
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id$
 *                                                                                                          *
 ************************************************************************************************************/
#ifndef __vnsConstantImageSource_h
#define __vnsConstantImageSource_h

#include "itkImageSource.h"
#include "itkNumericTraits.h"

#include "itkMetaDataDictionary.h"
#include "itkImageBase.h"

namespace vns
{

    /** \class ConstantImageSource
     * \brief Generate an n-dimensional image of specifics pixel values.
     *
     * ConstantImageSource generates an image of specific pixel values.
     *
     * \ingroup DataSources Multithreaded
     */
    template<typename TOutputImage>
        class ConstantImageSource : public itk::ImageSource<TOutputImage>
        {
        public:
            /** Standard class typedefs. */
            typedef ConstantImageSource Self;
            typedef itk::ImageSource<TOutputImage> Superclass;
            typedef itk::SmartPointer<Self> Pointer;
            typedef itk::SmartPointer<const Self> ConstPointer;

            /** Typedef for the output image PixelType. */
            typedef typename TOutputImage::PixelType OutputImagePixelType;

            /** Typedef to describe the output image region type. */
            typedef typename TOutputImage::RegionType OutputImageRegionType;

            /** Run-time type information (and related methods). */
            itkTypeMacro(ConstantImageSource,ImageSource)

            /** Method for creation through the object factory. */
            itkNewMacro(Self)

            typedef itk::ImageBase<TOutputImage::ImageDimension> ImageBaseType;

            /** Basic types from the OutputImageType */
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

            /** Set/Get size of the output image */
            itkSetMacro( Size, SizeType )

            virtual void
            SetSize(SizeValueArrayType sizeArray);
            virtual const SizeValueType *
            GetSize() const;

            /** Set/Get spacing of the output image */
            itkSetMacro( Spacing, SpacingType )

            virtual void
            SetSignedSpacing(SpacingType spacingArray);
            virtual const SpacingValueType *
            GetSignedSpacing() const;

            /** Set/Get origin of the output image */
            itkSetMacro( Origin, PointType )

            virtual void
            SetOrigin(PointValueArrayType originArray);
            virtual const PointValueType *
            GetOrigin() const;

            virtual void
            SetOriginSpacingSizeFromImage(const TOutputImage * image);

            /** Set the constant possible pixel value. By default, it is
             * NumericTraits<TOutputImage::PixelType>::min(). */
            itkSetClampMacro(ConstantValue, OutputImagePixelType,
                    itk::NumericTraits<OutputImagePixelType>::NonpositiveMin(),
                    itk::NumericTraits<OutputImagePixelType>::max())

            /** Get the constant pixel values. */
            itkGetConstMacro(ConstantValue, OutputImagePixelType)

            /** Set/Get the reference projection of the output image */
            virtual void
            SetProjectionRef(std::string);
            virtual const std::string
            GetProjectionRef() const;

        protected:
            ConstantImageSource();
            virtual
            ~ConstantImageSource();
            void
            PrintSelf(std::ostream& os, itk::Indent indent) const;

            virtual void
            ThreadedGenerateData(const OutputImageRegionType& outputRegionForThread, itk::ThreadIdType threadId);
            virtual void
            GenerateOutputInformation();

        private:
            ConstantImageSource(const ConstantImageSource&); //purposely not implemented
            void
            operator=(const ConstantImageSource&); //purposely not implemented

            SizeType m_Size;    //size of the output image
            SpacingType m_Spacing; //spacing
            PointType m_Origin;  //origin
            std::string m_ProjectionRef; // coordinate system

            typename TOutputImage::PixelType m_ConstantValue; // constant value

            // The following variables are deprecated, and provided here just for
            // backward compatibility. It use is discouraged.
            mutable PointValueArrayType m_OriginArray;
            mutable SpacingValueArrayType m_SpacingArray;
        };

} // end namespace vns

#ifndef VNS_MANUAL_INSTANTIATION
#include "vnsConstantImageSource.txx"
#endif

#endif // __vnsConstantImageSource_h
