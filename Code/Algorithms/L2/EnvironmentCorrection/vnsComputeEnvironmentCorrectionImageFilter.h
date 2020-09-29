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
 * VERSION : 2.0.0 : DM : LAIG-DM-MAJA-153436-CS : 13 avril 2017 : Classe ThesholdVector dispo dans l'otb   *
 * VERSION : 1.0.0 : FA : LAIG-FA-MAC-148399-CS : 07 novembre 2016 : Audit code - Correction qualite        *
 * VERSION : 1.0.0 : FA : LAIG-FA-MAC-1988-CNES : 21 octobre 2016 : Correction qualite (code mort)          *
 * VERSION : 5-1-0 : FA : LAIG-FA-MAC-145739-CS : 27 juin 2016 : Audit code - Supp de la macro ITK_EXPORT   *
 * VERSION : 5-1-0 : FA : LAIG-FA-MAC-144674-CS : 2 mai 2016 : Correction warning, qualite, etc             *
 * VERSION : 5-0-0 : FA : LAIG-FA-MAC-143764-CS : 19 avril 2016 : Mise en forme du code (indentation, etc.) *
 * VERSION : 2-0-0 : DM : LAIG-DM-MAC-146-CNES : 07 novembre 2011 : Modification pour prise en compte            *
 *                                                            d'evolutions liees a MACCS                    *
 * VERSION : 1-0-0 : 07 novembre 2011 : Creation
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id:
 *                                                                                                          *
 ************************************************************************************************************/
#ifndef __vnsComputeEnvironmentCorrectionImageFilter_h
#define __vnsComputeEnvironmentCorrectionImageFilter_h

#include "itkProcessObject.h"
#include "vnsNaryEnvironmentCorrectionImageFilter.h"
#include "vnsInterpolateForEnvCorrectionImageFilter.h"
#include "otbImageFileReader.h"
#include "itkAddImageFilter.h"
#include "otbThresholdVectorImageFilter.h"
#include "vnsDedicatedPadAndFastResampleImageFilter.h"
#include "vnsComputeFinalResultEnvironmentCorrectionImageFilter.h"

namespace vns
{
    /** \class  ComputeEnvironmentCorrectionImageFilter
     * \brief This class implements the algorithms to compute the reflectance image after Environment correction.
     *
     * Note that the size of the output images are expanded at L2_resolution an eventually rearranged
     * with ResizeVectorImageFilter.
     *
     * \author CS Systemes d'Information
     *
     * \sa ImageToImageFilter
     *
     * \ingroup L2
     *
     */
    template<class TInputVectorImage, class TOutputVectorImage, class TInputMask>
        class ComputeEnvironmentCorrectionImageFilter : public itk::ProcessObject
        {
        public:
            /** Standard class typedefs. */
            typedef ComputeEnvironmentCorrectionImageFilter Self;
            typedef itk::ProcessObject Superclass;
            typedef itk::SmartPointer<Self> Pointer;
            typedef itk::SmartPointer<const Self> ConstPointer;

            /** Type macro */
            itkNewMacro(Self)
            /** Creation through object factory macro */
            itkTypeMacro(ComputeEnvironmentCorrectionImageFilter, ProcessObject )

            /** Some convenient typedefs. */
            typedef TInputVectorImage InputVectorImageType;
            typedef typename InputVectorImageType::ConstPointer InputVectorImageConstPointerType;
            typedef typename InputVectorImageType::Pointer InputVectorImagePointerType;
            typedef typename InputVectorImageType::RegionType InputVectorImageRegionType;
            typedef typename InputVectorImageType::PixelType InputVectorImagePixelType;
            typedef typename InputVectorImageType::InternalPixelType InternalPixelType;
            typedef typename InputVectorImageType::IndexType IndexType;
            typedef typename InputVectorImageType::SizeType SizeType;
            typedef typename InputVectorImageType::OffsetType OffsetType;

            typedef TOutputVectorImage OutputVectorImageType;
            typedef typename OutputVectorImageType::Pointer OutputVectorImagePointerType;
            typedef typename OutputVectorImageType::ConstPointer OutputVectorImageConstPointerType;
            typedef typename OutputVectorImageType::RegionType OutputVectorImageRegionType;
            typedef typename OutputVectorImageType::InternalPixelType OutputVectorImageInternalPixelType;
            typedef OutputVectorImageInternalPixelType NoDataType;
            typedef typename OutputVectorImageType::IndexType OutputVectorImageIndexType;
            typedef typename OutputVectorImageType::SizeType OutputVectorImageSizeType;
            typedef typename OutputVectorImageType::PixelType OutputVectorPixelType;
            typedef typename OutputVectorImageType::InternalPixelType OutputVectorInternalPixelType;

            typedef TInputMask InputMaskType;
            typedef typename InputMaskType::Pointer InputMaskPointerType;
            typedef typename InputMaskType::ConstPointer InputMaskConstPointerType;

            /** Filters */
            typedef NaryEnvironmentCorrectionImageFilter<InputVectorImageType, OutputVectorImageType> NaryEnvironmentCorrectionImageFilterType;
            typedef typename NaryEnvironmentCorrectionImageFilterType::Pointer NaryEnvironmentCorrectionImageFilterPointerType;

            typedef ComputeFinalResultEnvironmentCorrectionImageFilter<InputVectorImageType, InputMaskType, OutputVectorImageType> ComputeFinalResultEnvironmentCorrectionImageFilterType;
            typedef typename ComputeFinalResultEnvironmentCorrectionImageFilterType::Pointer ComputeFinalResultEnvironmentCorrectionImageFilterPointerType;

            typedef DedicatedPadAndFastResampleImageFilter<OutputVectorImageType, OutputVectorImageType> DedicatedPadAndFastResampleImageFilterType;
            typedef typename DedicatedPadAndFastResampleImageFilterType::Pointer DedicatedPadAndFastResampleImageFilterPointerType;

            typedef itk::AddImageFilter<OutputVectorImageType, OutputVectorImageType> AddImageFilterType;
            typedef typename AddImageFilterType::Pointer AddImageFilterPointerType;

            typedef otb::ThresholdVectorImageFilter<OutputVectorImageType> ThresholdVectorImageFilterType;
            typedef typename ThresholdVectorImageFilterType::Pointer ThresholdVectorImageFilterPointer;

            /** Inputs */
            vnsSetGetInputRawMacro( IPTOC, InputVectorImageType, 0)
            vnsSetGetInputRawMacro( RhoEnvSub, InputVectorImageType, 1)
            vnsSetGetInputRawMacro( LUT_Tdir, OutputVectorImageType, 2)
            vnsSetGetInputRawMacro( LUT_Tdif, OutputVectorImageType, 3)
            vnsSetGetInputRawMacro( LUT_Satm, OutputVectorImageType, 4)
            vnsSetGetInputRawMacro( L2EDG, InputMaskType, 5)

            /** Outputs */
            // RhoEnv output at L2_resolution
            itkGetObjectMacro(RhoEnv,OutputVectorImageType)

            // L2SRE output at L2_resolution
            itkGetObjectMacro(L2SRE,OutputVectorImageType)

            // NoData
            itkSetMacro(NoData,NoDataType)

            /** Generate data method */
            virtual void
            UpdateWithCaching();

        protected:
            /** Constructor */
            ComputeEnvironmentCorrectionImageFilter();
            /** Destructor */
            virtual
            ~ComputeEnvironmentCorrectionImageFilter();

            /** PrintSelf method */
            virtual void
            PrintSelf(std::ostream& os, itk::Indent indent) const;

        private:
            ComputeEnvironmentCorrectionImageFilter(const Self&); //purposely not implemented
            void
            operator=(const Self&); //purposely not implemented

            /** Validate the presence of all inputs and their sizes. If at least one input
             * is missing, throws an exception. */
            void
            CheckInputs();

            // Expand images from L2 coarse resolution to L2 resolution
            DedicatedPadAndFastResampleImageFilterPointerType m_ExpandTdir;
            DedicatedPadAndFastResampleImageFilterPointerType m_ExpandTdif;
            DedicatedPadAndFastResampleImageFilterPointerType m_ExpandSatm;
            DedicatedPadAndFastResampleImageFilterPointerType m_ExpandRhoEnv;

            ComputeFinalResultEnvironmentCorrectionImageFilterPointerType m_ComputeFinalResultEnvironmentCorrection;

            ThresholdVectorImageFilterPointer m_ThresholdVectorImageFilter;

            /** Outputs */
            OutputVectorImagePointerType m_RhoEnv;
            OutputVectorImagePointerType m_L2SRE;

            NoDataType m_NoData;
        };

} // End namespace vns
#ifndef VNS_MANUAL_INSTANTIATION
#include "vnsComputeEnvironmentCorrectionImageFilter.txx"
#endif

#endif /* __vnsComputeEnvironmentCorrectionImageFilter_h */

