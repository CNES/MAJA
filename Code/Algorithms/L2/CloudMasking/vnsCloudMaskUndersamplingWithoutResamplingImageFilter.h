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
 * VERSION : 5-0-0 : FA : LAIG-FA-MAC-1619-CNES : 21 avril 2016 : Creation pour corriger l'anomalie         *
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id: vnsCloudMaskUndersamplingWithoutResamplingImageFilter.h 11465 2016-01-28 15:51:04Z tfeuvrie $
 *                                                                                                          *
 ************************************************************************************************************/
#ifndef __vnsCloudMaskUndersamplingWithoutResamplingImageFilter_h
#define __vnsCloudMaskUndersamplingWithoutResamplingImageFilter_h

#include "itkImageToImageFilter.h"
#include "itkNeighborhoodIterator.h"

#include "vnsMacro.h"

namespace vns
{
    /** \class  CloudMaskUndersamplingWithoutResamplingImageFilter
     * \brief This class undersamples the stereoscopic altitude image to L2_coarse_resolution.
     *
     * It is not a standard downsampling because the input image is not a continuous image.
     * This undersampling is only performed if the cloud_mask_resolution is lower than
     * the L2_coarse_resolution.
     *
     *
     * \author CS Systemes d'Information
     *
     * \sa ImageToImageFilter
     *
     * \ingroup L2
     *
     */
    template<class TInputImage, class TOutputImage, class TOutputMask>
        class CloudMaskUndersamplingWithoutResamplingImageFilter : public itk::ImageToImageFilter<TInputImage, TOutputImage>
        {
        public:
            /** Standard class typedefs. */
            typedef CloudMaskUndersamplingWithoutResamplingImageFilter Self;
            typedef itk::ImageToImageFilter<TInputImage, TOutputImage> Superclass;
            typedef itk::SmartPointer<Self> Pointer;
            typedef itk::SmartPointer<const Self> ConstPointer;

            /** Type macro */
            itkNewMacro(Self)
            /** Creation through object factory macro */
            itkTypeMacro(CloudMaskUndersamplingWithoutResamplingImageFilter, ImageToImageFilter )

            /** Some convenient typedefs. */
            typedef typename Superclass::InputImageType InputImageType;
            typedef typename InputImageType::ConstPointer InputImageConstPointer;
            typedef typename InputImageType::RegionType InputImageRegionType;
            typedef typename InputImageType::PixelType InputImagePixelType;
            typedef typename InputImageType::IndexType IndexType;
            typedef typename InputImageType::SizeType SizeType;
            typedef typename InputImageType::SizeValueType SizeValueType;

            typedef typename Superclass::OutputImageType OutputImageType;
            typedef typename OutputImageType::Pointer OutputImagePointer;
            typedef typename InputImageType::RegionType OutputImageRegionType;
            typedef typename OutputImageType::PixelType OutputImagePixelType;
            typedef typename OutputImageType::SizeValueType OutputSizeValueType;

            typedef TOutputMask OutputMaskType;
            typedef typename OutputMaskType::Pointer OutputMaskPointer;
            typedef typename OutputMaskType::RegionType OutputMaskRegionType;
            typedef typename OutputMaskType::PixelType OutputMaskPixelType;

            typedef itk::NeighborhoodIterator<InputImageType> NeighborhoodIteratorType;
            typedef typename NeighborhoodIteratorType::NeighborhoodType NeighborhoodType;

            // No_data pixel value of L1 product accessors
            itkSetMacro(NoData, RealNoDataType)
            itkGetConstReferenceMacro(NoData, RealNoDataType)

            /** Set the L1 stereoscopic cloud altitude image */
            vnsSetGetInputMacro( L1CLA, InputImageType, 0)

            /** Get the cloud altitude at L2_coarse_resolution */
            OutputImageType*
            GetIPCLAOutput()
            {
                return static_cast<OutputImageType*>(this->itk::ProcessObject::GetOutput(0));
            }

            /** Get the mask of the pixels for which the cloud altitude image is not null */
            OutputMaskType*
            GetIPCLDAltOutput()
            {
                return static_cast<OutputMaskType*>(this->itk::ProcessObject::GetOutput(1));
            }

        protected:
            /** Constructor */
            CloudMaskUndersamplingWithoutResamplingImageFilter();
            /** Destructor */
            virtual
            ~CloudMaskUndersamplingWithoutResamplingImageFilter();
            /** PrintSelf method */
            virtual void
            PrintSelf(std::ostream& os, itk::Indent indent) const;

            /** Multi-thread version GenerateData. */
            void
            ThreadedGenerateData(const OutputImageRegionType& outputRegionForThread, itk::ThreadIdType threadId);

        private:
            CloudMaskUndersamplingWithoutResamplingImageFilter(const Self&); //purposely not implemented
            void
            operator=(const Self&); //purposely not implemented

            /** No_data value of product L1 declaration */
            RealNoDataType m_NoData;

        };

} // End namespace vns
#ifndef VNS_MANUAL_INSTANTIATION
#include "vnsCloudMaskUndersamplingWithoutResamplingImageFilter.txx"
#endif

#endif /* __vnsCloudMaskUndersamplingWithoutResamplingImageFilter_h */
