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
 * VERSION : 5-0-0 : FA : LAIG-FA-MAC-143764-CS : 12 avril 2016 : Mise en forme du code (indentation, etc.) *
 * VERSION : 1-0-0-3 : DM : 331 : 04 oct. 2011 : Cirrus Flag, calcul fait à L2CoarseResolution                *
 * VERSION : 1-0-0 : <TypeFT> : <NumFT> : 23 avr. 2010 : Creation                                                           
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id: vnsCloudMaskUndersamplingImageFilterDeprecated.h 12010 2016-04-12 10:52:56Z tfeuvrie $
 *                                                                                                          *
 ************************************************************************************************************/
#ifndef __vnsCloudMaskUndersamplingImageFilterDeprecated_h
#define __vnsCloudMaskUndersamplingImageFilterDeprecated_h

#include "itkImageToImageFilter.h"
#include "itkNeighborhoodIterator.h"

#include "vnsMacro.h"

namespace vns
{
    /** \class  CloudMaskUndersamplingImageFilterDeprecated
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
    class CloudMaskUndersamplingImageFilterDeprecated : public itk::ImageToImageFilter<VNSRealImageType, VNSRealImageType>
    {
    public:
        /** Standard class typedefs. */
        typedef CloudMaskUndersamplingImageFilterDeprecated Self;
        typedef itk::ImageToImageFilter<VNSRealImageType, VNSRealImageType> Superclass;
        typedef itk::SmartPointer<Self> Pointer;
        typedef itk::SmartPointer<const Self> ConstPointer;

        /** Type macro */
        itkNewMacro(Self)
        /** Creation through object factory macro */
        itkTypeMacro(CloudMaskUndersamplingImageFilterDeprecated, ImageToImageFilter )

        /** Some convenient typedefs. */
        typedef Superclass::InputImageType InputImageType;
        typedef InputImageType::ConstPointer InputImageConstPointer;
        typedef InputImageType::RegionType InputImageRegionType;
        typedef InputImageType::PixelType InputImagePixelType;
        typedef InputImageType::IndexType IndexType;
        typedef InputImageType::SizeType SizeType;
        typedef InputImageType::SizeValueType SizeValueType;

        typedef Superclass::OutputImageType OutputImageType;
        typedef OutputImageType::Pointer OutputImagePointer;
        typedef InputImageType::RegionType OutputImageRegionType;
        typedef OutputImageType::PixelType OutputImagePixelType;
        typedef OutputImageType::SizeValueType OutputSizeValueType;

        typedef VNSMaskImageType OutputMaskType;
        typedef OutputMaskType::Pointer OutputMaskPointer;
        typedef OutputMaskType::RegionType OutputMaskRegionType;
        typedef OutputMaskType::PixelType OutputMaskPixelType;

        typedef itk::NeighborhoodIterator<InputImageType> NeighborhoodIteratorType;
        typedef NeighborhoodIteratorType::NeighborhoodType NeighborhoodType;

        // No_data pixel value of L1 product accessors
        itkSetMacro(L1NoData, RealNoDataType)
        itkGetConstReferenceMacro(L1NoData, RealNoDataType)

        // Cloud Mask Resolution accessors
//    itkSetMacro(CloudMaskResolution, unsigned short);
//    itkGetConstReferenceMacro(CloudMaskResolution, unsigned short);
//
//    // L2 Coarse Resolution accessors
//    itkSetMacro(L2CoarseResolution, unsigned short);
//    itkGetConstReferenceMacro(L2CoarseResolution, unsigned short);

        vnsMemberAndDirectSetAndGetConstReferenceMacro(AreaToCLDResolution, AreaType)

        vnsMemberAndDirectSetAndGetConstReferenceMacro(AreaToL2CoarseResolution, AreaType)

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

        /** Since input and output image are very likely to be of different size.
         * Region estimation functions has to be reimplemented
         */
        virtual void
        CallCopyOutputRegionToInputRegion(InputImageRegionType & destRegion, const OutputImageRegionType & srcRegion);
        virtual void
        CallCopyInputRegionToOutputRegion(OutputImageRegionType & destRegion, const InputImageRegionType & srcRegion);

        /** Output image region size is not of the same dimension as the input.
         * That is why GenerateOutputInformation has to be redefined.
         */
        virtual void
        GenerateOutputInformation();

    protected:
        /** Constructor */
        CloudMaskUndersamplingImageFilterDeprecated();
        /** Destructor */
        virtual
        ~CloudMaskUndersamplingImageFilterDeprecated()
        {
        }
        /** PrintSelf method */
        virtual void
        PrintSelf(std::ostream& os, itk::Indent indent) const;

        /**
         * Pad the input requested region by radius
         */
        virtual void
        GenerateInputRequestedRegion(void);
        /** Multi-thread version GenerateData. */
        void
        ThreadedGenerateData(const OutputImageRegionType& outputRegionForThread, itk::ThreadIdType threadId);

    private:
        CloudMaskUndersamplingImageFilterDeprecated(const Self&); //purposely not implemented
        void
        operator=(const Self&); //purposely not implemented

        /** Resolution parameters declaration */
//    unsigned short m_CloudMaskResolution;
//    unsigned short m_L2CoarseResolution;
        /** No_data value of product L1 declaration */
        RealNoDataType m_L1NoData;

        /** Undersampling factor declaration */
//    double m_SubsampleFactor;
        /** Radius declaration depending on the subsample factor */
        SizeType m_Radius;

    };

} // End namespace vns

#endif /* __vnsCloudMaskUndersamplingImageFilterDeprecated_h */
