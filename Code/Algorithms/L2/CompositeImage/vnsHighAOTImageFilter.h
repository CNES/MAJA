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
 * VERSION : 1-0-0 : <TypeFT> : <NumFT> : 27 août 2010 : Creation                                                           
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id$
 *                                                                                                          *
 ************************************************************************************************************/
#ifndef __vnsHighAOTImageFilter_h
#define __vnsHighAOTImageFilter_h

#include "itkImageToImageFilter.h"
#include "itkBinaryFunctorImageFilter.h"

#include "vnsBinaryLogicalOrFunctor.h"
#include "vnsHighAOTFunctor.h"

#include "otbVectorImage.h"
#include "vnsMacro.h"

namespace vns
{
    /** \class  HighAOTImageFilter
     * \brief This class implements .
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
    template<class TInputImage, class TInputMask, class TOutputImage>
        class HighAOTImageFilter : public itk::ImageToImageFilter<TInputImage, TOutputImage>
        {
        public:
            /** Standard class typedefs. */
            typedef HighAOTImageFilter Self;
            typedef itk::ImageToImageFilter<TInputImage, TOutputImage> Superclass;
            typedef itk::SmartPointer<Self> Pointer;
            typedef itk::SmartPointer<const Self> ConstPointer;

            /** Type macro */
            itkNewMacro(Self)
            /** Creation through object factory macro */
            itkTypeMacro(HighAOTImageFilter, ImageToImageFilter )

            /** Some convenient typedefs. */
            typedef typename Superclass::InputImageType InputImageType;
            typedef typename InputImageType::ConstPointer InputImageConstPointer;
            typedef typename InputImageType::RegionType RegionType;
            typedef typename InputImageType::PixelType InputImagePixelType;
            typedef typename InputImageType::SizeType SizeType;

            /** Input mask typedefs. */
            typedef TInputMask InputMaskType;
            typedef typename InputMaskType::ConstPointer InputMaskConstPointer;
            typedef typename InputMaskType::PixelType InputMaskPixelType;
            typedef typename InputMaskType::InternalPixelType InputMaskInternalPixelType;

            /** Output mask typedefs */
            typedef typename Superclass::OutputImageType OutputImageType;
            typedef typename OutputImageType::Pointer OutputImagePointer;
            typedef typename OutputImageType::PixelType OutputImagePixelType;
            typedef typename OutputImageType::RegionType OutputImageRegionType;

            /** Filter typedefs */
            // Ternary logical or funtor type
            typedef Functor::BinaryLogicalOrFunctor<InputMaskPixelType, InputMaskPixelType, InputMaskPixelType> BinaryLogicalOrFunctorType;

            typedef itk::BinaryFunctorImageFilter<InputMaskType, InputMaskType, InputMaskType, BinaryLogicalOrFunctorType> BinaryLogicalOrFilterType;

            typedef typename BinaryLogicalOrFilterType::Pointer BinaryLogicalOrPointerFilterType;

            typedef vns::Functor::HighAOTFunctor<InputImagePixelType, InputMaskPixelType, OutputImagePixelType> FunctorType;
            typedef itk::BinaryFunctorImageFilter<InputImageType, InputMaskType, OutputImageType, FunctorType> HighAOTFilterType;

            typedef typename HighAOTFilterType::Pointer HighAOTPointerFilterType;

            // Maximum high AOT std accessors
            itkSetMacro(AOTMean, double)
            itkGetConstReferenceMacro(AOTMean, double)

            // Maximum high AOT std accessors
            itkSetMacro(AOTStd, double)
            itkGetConstReferenceMacro(AOTStd, double)

            // Coef sigma AOT accessors
            itkSetMacro(CoefSigmaAOT, unsigned short)
            itkGetConstReferenceMacro(CoefSigmaAOT, unsigned short)


            // Maximum high AOT std accessors
            itkSetMacro(MinimumHighAOTStd, double)
            itkGetConstReferenceMacro(MinimumHighAOTStd, double)


            /** Set the AOT sub image */
            vnsSetGetInputMacro( IPAOTSub, InputImageType, 0)
            /** Set the edges mask sub */
            vnsSetGetInputMacro( IPEDG, InputMaskType, 1)
            /** Set the cloud mask sub */
            vnsSetGetInputMacro( L2CLDSub, InputMaskType, 2)

        protected:
            /** Constructor */
            HighAOTImageFilter();
            /** Destructor */
            virtual
            ~HighAOTImageFilter();
            /** PrintSelf method */
            virtual void
            PrintSelf(std::ostream& os, itk::Indent indent) const;

            /** Composite Filter method. */
            virtual void
            GenerateData();

        private:
            HighAOTImageFilter(const Self&); //purposely not implemented
            void
            operator=(const Self&); //purposely not implemented

            /** Coefficient for the threshold to detect high AOT */
            unsigned short m_CoefSigmaAOT;
            /** Std max under which the std of the AOT image is not taking into
             * account in the calculation of the high AOT threshold
             */
            double m_MinimumHighAOTStd;

            /** AOT mean parameter declaration */
            double m_AOTMean;
            /** AOT standard deviation aprameter declaration */
            double m_AOTStd;

            BinaryLogicalOrPointerFilterType m_BinaryLogicalOrImageFilter;
            HighAOTPointerFilterType m_HighAOTFilter;

        };

} // End namespace vns
#ifndef VNS_MANUAL_INSTANTIATION
#include "vnsHighAOTImageFilter.txx"
#endif

#endif /* __vnsHighAOTImageFilter_h */
