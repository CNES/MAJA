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
 * VERSION : 4-0-0 : DM : LAIG-DM-MAC-517-CNES : 6 janvier 2014 : Améliorations algorithmiques (Spec v2.1)	*
 * 															Création de l'algorithme SnowMaskCorrection		*
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id: vnsSnowMaskCorrectionImageFilter.h 3545 2011-11-29 14:21:48Z cruffel $
 *                                                                                                          *
 ************************************************************************************************************/
#ifndef __vnsSnowMaskCorrectionImageFilter_h
#define __vnsSnowMaskCorrectionImageFilter_h

#include "itkProcessObject.h"

#include "itkConnectedComponentImageFilter.h"

#include "vnsSnowMaskCorrectionByLabelImageFilter.h"

#include "vnsMacro.h"

#include "otbImage.h"
#include "otbImageFileReader.h"
#include "otbObjectList.h"

namespace vns
{
    /** \class  SnowMaskCorrectionImageFilter
     * \brief This class updates the snow and the cloud reflectance masks to minimize the false detections of snow.
     * If a snow zone is entirely surrounded by clouds, then the pixels inside the labels are cloudy instead of snowy.
     * To determine if a zone is surrounded by clouds, we dilate it, and look at the border of the dilated zone
     * (that is the pixels that belong to the dilated zone and not to the initial one). If all the pixels of that border are
     * classified as cloudy (or edge), then it is surrounded by clouds.
     *
     * All the input masks are otb::image.
     *
     * \author CS Systemes d'Information
     *
     * \ingroup L2
     *
     */
    template<class TMaskType>
        class SnowMaskCorrectionImageFilter : public itk::ProcessObject
        {
        public:
            /** Standard class typedefs. */
            typedef SnowMaskCorrectionImageFilter Self;
            typedef itk::ProcessObject Superclass;
            typedef itk::SmartPointer<Self> Pointer;
            typedef itk::SmartPointer<const Self> ConstPointer;

            /** Type macro */
            itkNewMacro(Self)

            /** Creation through object factory macro */
            itkTypeMacro(SnowMaskCorrectionImageFilter, ProcessObject )

            /** Some convenient typedefs. */
            typedef TMaskType MaskType;
            typedef typename MaskType::Pointer MaskPointer;
            typedef typename MaskType::ConstPointer MaskConstPointer;
            typedef typename MaskType::RegionType RegionType;
            typedef typename MaskType::PixelType MaskPixelType;

            /** Internal types */
            typedef unsigned short LabelPixelType;
            typedef otb::Image<LabelPixelType, MaskType::ImageDimension> LabelImageType;
            typedef typename LabelImageType::Pointer LabelImagePointer;
            typedef typename LabelImageType::PixelType LabelImagePixelType;

            /** Filters typedefs */
            typedef itk::ConnectedComponentImageFilter<MaskType, LabelImageType> LabelizeFilterType;
            typedef typename LabelizeFilterType::Pointer LabelizeFilterPointer;

            // Test if all the border of a label is surrounded by cloud
            typedef SnowMaskCorrectionByLabelImageFilter<MaskType, LabelImageType, MaskType> SnowMaskCorrectionByLabelImageFilterType;
            typedef typename SnowMaskCorrectionByLabelImageFilterType::Pointer SnowMaskCorrectionByLabelImageFilterPointer;

            typedef otb::ObjectList<SnowMaskCorrectionByLabelImageFilterType> SnowMaskCorrectionByLabelFilterListType;

            // Readers for caching
            typedef otb::ImageFileReader<LabelImageType> ReaderLabelType;
            typedef typename ReaderLabelType::Pointer ReaderLabelPointer;

            // radius of the stucturing element use for dilatation
            itkSetMacro(DilateRadius, unsigned short)

            // output mask value
            itkSetMacro(OutputMaskValue, MaskPixelType)

            /** Set the snow mask at L2 coarse resolution */
            vnsMemberSetGetObjectMacro(IPSNWSubInput, MaskType)

            /** Set the edge mask at L2 coarse resolution */
            vnsMemberSetGetObjectMacro(IPEDGSubInput, MaskType)

            /** Set the cloud reflectance mask at L2 coarse resolution */
            vnsMemberSetGetObjectMacro(IPCLDReflSubInput, MaskType)

            /** Set the cloud reflectance variation mask at L2 coarse resolution */
            vnsMemberSetGetObjectMacro(IPCLDReflVarSubInput, MaskType)

            /** Get the updated IPCLDRefSubl mask */
            vnsMemberGetObjectMacro(IPCLDReflSubOutput, MaskType)

            /** Get the updated IPSNWSub mask */
            vnsMemberGetObjectMacro(IPSNWSubOutput, MaskType)

            virtual void
            UpdateWithCaching();

        protected:
            /** Constructor */
            SnowMaskCorrectionImageFilter();
            /** Destructor */
            virtual
            ~SnowMaskCorrectionImageFilter();

            /** PrintSelf method */
            virtual void
            PrintSelf(std::ostream& os, itk::Indent indent) const;

        private:
            SnowMaskCorrectionImageFilter(const Self&); //purposely not implemented
            void
            operator=(const Self&); //purposely not implemented

            /** Validate the presence of all inputs and their sizes. If at least one input
             * is missing, throws an exception. */
            void
            CheckInputs();

            /** Filters declaration */
            LabelizeFilterPointer m_LabelFilterPtr;
            SnowMaskCorrectionByLabelImageFilterPointer m_SnowLabelCorrFilterPtr;

            ReaderLabelPointer m_LabelReader;

            typename SnowMaskCorrectionByLabelFilterListType::Pointer m_SnowMaskLabelFilterList;

            /** Parameters declaration */
            // Dilatation
            unsigned short m_DilateRadius;
            MaskPixelType m_OutputMaskValue;

        };

} // End namespace vns
#ifndef VNS_MANUAL_INSTANTIATION
#include "vnsSnowMaskCorrectionImageFilter.txx"
#endif

#endif /* __vnsSnowMaskCorrectionImageFilter_h */
