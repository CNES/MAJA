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
 * VERSION : 5-0-0 : DM : LAIG-DM-MAC-982-CNES : 5 avr. 2016 : Creation                                       *
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id:
 *                                                                                                          *
 ************************************************************************************************************/
#ifndef __vnsImageListToImageListApplyMaskWithFunctorFilter_h
#define __vnsImageListToImageListApplyMaskWithFunctorFilter_h

#include "otbImageListToImageListFilter.h"
#include "itkBinaryFunctorImageFilter.h"
#include "itkMacro.h"

namespace vns
{
    /** \class ImageListToImageListApplyMaskWithFunctorFilter
     *  \brief This filter applies the functor given as template to the input image list.
     *
     *  It produces one output image in the output list for one input image in the output list.
     *
     *  It uses the mask image used by the itk::BinaryFunctorImageFilter filter.
     *
     *  The functor can be initialized via the GetFunctor() accessor.
     *
     *  It also support streaming when the templated filter support it.
     *
     * \ingroup Streamed
     */
    template<class TInputImageList, class TMaskImage, class TOutputImageList, class TApplyMaskFunctorType>
        class ImageListToImageListApplyMaskWithFunctorFilter : public otb::ImageListToImageListFilter<typename TInputImageList::ImageType,
                typename TOutputImageList::ImageType>
        {
        public:
            /** Standard typedefs */
            typedef ImageListToImageListApplyMaskWithFunctorFilter Self;
            typedef otb::ImageListToImageListFilter<typename TInputImageList::ImageType, typename TOutputImageList::ImageType> Superclass;
            typedef itk::SmartPointer<Self> Pointer;
            typedef itk::SmartPointer<const Self> ConstPointer;

            /** Type macro */
            itkNewMacro (Self);

            /** Creation through object factory macro */
            itkTypeMacro(ImageListToImageListApplyMaskWithFunctorFilter, ImageListToImageListFilter);

            /** Template parameters typedefs */
            typedef TInputImageList InputImageListType;
            typedef typename InputImageListType::Pointer InputImageListPointerType;
            typedef typename InputImageListType::ImageType InputImageType;

            typedef TMaskImage MaskImageType;

            typedef TOutputImageList OutputImageListType;
            typedef typename OutputImageListType::Pointer OutputImageListPointerType;
            typedef typename OutputImageListType::ImageType OutputImageType;

            typedef itk::BinaryFunctorImageFilter<typename TInputImageList::ImageType, TMaskImage, typename TOutputImageList::ImageType,
                    TApplyMaskFunctorType> FilterType;
            typedef typename FilterType::Pointer FilterPointerType;

            itkGetMacro(OutputIndex, unsigned int)
            itkSetMacro(OutputIndex, unsigned int)

            virtual void
            SetInputMaskImage(const MaskImageType * mask_image)
            {
                m_BinaryFunctorImageFilter->SetInput2(mask_image);
            }

            typename FilterType::FunctorType & GetFunctor()
            {
                return m_BinaryFunctorImageFilter->GetFunctor();
            }

            /** Generate output information for the ImageList and for each image
             in the list. */
            virtual void
            GenerateOutputInformation(void);

            /** Generate input requested region for each image in the list. */
            virtual void
            GenerateInputRequestedRegion(void);

        protected:
            /** Main computation method */
            virtual void
            GenerateData(void);
            /** Constructor */
            ImageListToImageListApplyMaskWithFunctorFilter();
            /** Destructor */
            virtual
            ~ImageListToImageListApplyMaskWithFunctorFilter()
            {
            }
            /**PrintSelf method */
            virtual void
            PrintSelf(std::ostream& os, itk::Indent indent) const;

        private:
            ImageListToImageListApplyMaskWithFunctorFilter(const Self &); //purposely not implemented
            void
            operator =(const Self&); //purposely not implemented

            /** The processing filter */
            FilterPointerType m_BinaryFunctorImageFilter;
            /** The index of the output of the filter to use */
            unsigned int m_OutputIndex;
        };
}
// End namespace vns
#ifndef VNS_MANUAL_INSTANTIATION
#include "vnsImageListToImageListApplyMaskWithFunctorFilter.txx"
#endif

#endif
