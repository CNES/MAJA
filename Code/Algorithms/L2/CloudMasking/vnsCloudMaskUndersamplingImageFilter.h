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
 * VERSION : 5-0-0 : FA : LAIG-FA-MAC-1619-CNES : 21 avril 2016 : Modification pour corriger l'anomalie     *
 * VERSION : 5-0-0 : FA : LAIG-FA-MAC-143764-CS : 12 avril 2016 : Mise en forme du code (indentation, etc.) *
 * VERSION : 1-0-0-3 : DM : 331 : 04 oct. 2011 : Cirrus Flag, calcul fait à L2CoarseResolution                *
 * VERSION : 1-0-0 : <TypeFT> : <NumFT> : 23 avr. 2010 : Creation                                                           
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id$
 *                                                                                                          *
 ************************************************************************************************************/
#ifndef __vnsCloudMaskUndersamplingImageFilter_h
#define __vnsCloudMaskUndersamplingImageFilter_h

#include "itkImageToImageFilter.h"
#include "itkNeighborhoodIterator.h"

#include "vnsMacro.h"
#include "vnsCloudMaskUndersamplingWithoutResamplingImageFilter.h"
#include "vnsGridResampleImageFilter.h"

namespace vns
{
    /** \class  CloudMaskUndersamplingImageFilter
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
    class CloudMaskUndersamplingImageFilter : public itk::ProcessObject
    {
    public:
        /** Standard class typedefs. */
        typedef CloudMaskUndersamplingImageFilter Self;
        typedef itk::ProcessObject Superclass;
        typedef itk::SmartPointer<Self> Pointer;
        typedef itk::SmartPointer<const Self> ConstPointer;

        /** Type macro */
        itkNewMacro(Self)
        /** Creation through object factory macro */
        itkTypeMacro(CloudMaskUndersamplingImageFilter, ProcessObject )

        /** Some convenient typedefs. */
        typedef VNSRealImageType InputImageType;
        typedef InputImageType::ConstPointer InputImageConstPointer;
        typedef InputImageType::RegionType InputImageRegionType;
        typedef InputImageType::PixelType InputImagePixelType;
        typedef InputImageType::SpacingType InputImageSpacingType;
        typedef InputImageType::IndexType IndexType;
        typedef InputImageType::SizeType SizeType;
        typedef InputImageType::SizeValueType SizeValueType;

        typedef VNSRealImageType OutputImageType;
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


        typedef CloudMaskUndersamplingWithoutResamplingImageFilter<InputImageType, OutputImageType, OutputMaskType> CloudMaskUndersamplingWithoutResamplingImageFilterType;
        typedef GridResampleImageFilter<InputImageType, OutputImageType> GridResampleImageFilterCLAType;
        typedef GridResampleImageFilter<InputImageType, OutputMaskType> GridResampleImageFilterCLDAltType;

        // No_data pixel value of L1 product accessors
        itkSetMacro(L1NoData, RealNoDataType)
        itkGetConstReferenceMacro(L1NoData, RealNoDataType)

        vnsMemberAndDirectSetAndGetConstReferenceMacro(AreaToL2CoarseResolution, AreaType)

        /** Set the L1 stereoscopic cloud altitude image */
        vnsSetGetInputMacro( L1CLA, InputImageType, 0)

        /** Get the cloud altitude at L2_coarse_resolution */
        itkGetObjectMacro(IPCLAOutput,OutputImageType)
        /** Get the mask of the pixels for which the cloud altitude image is not null */
        itkGetObjectMacro(IPCLDAltOutput,OutputMaskType)

        /** Generate data method */
        virtual void
        UpdateWithCaching();


    protected:
        /** Constructor */
        CloudMaskUndersamplingImageFilter();
        /** Destructor */
        virtual
        ~CloudMaskUndersamplingImageFilter()
        {
        }
        /** PrintSelf method */
        virtual void
        PrintSelf(std::ostream& os, itk::Indent indent) const;

    private:
        CloudMaskUndersamplingImageFilter(const Self&); //purposely not implemented
        void
        operator=(const Self&); //purposely not implemented

        /** No_data value of product L1 declaration */
        RealNoDataType m_L1NoData;

        /** Outputs */
        OutputImagePointer m_IPCLAOutput;
        OutputMaskPointer m_IPCLDAltOutput;


        // CloudUndersampling without resampling
        CloudMaskUndersamplingWithoutResamplingImageFilterType::Pointer m_CloudMaskUndersamplingWithoutResamplingImageFilter;
        GridResampleImageFilterCLAType::Pointer m_ResampleImageFilterCLA;
        GridResampleImageFilterCLDAltType::Pointer  m_ResampleImageFilterCLDAlt;


    };

} // End namespace vns

#endif /* __vnsCloudMaskUndersamplingImageFilter_h */
