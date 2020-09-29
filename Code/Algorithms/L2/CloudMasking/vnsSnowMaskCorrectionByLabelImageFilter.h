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
 * VERSION : 2.1.0 : DM : LAIG-DM-MAJA-157814-CS : 17 mai 2017 : Refactoring plus de unary                  *
 * VERSION : 5-1-0 : FA : LAIG-FA-MAC-145739-CS : 27 juin 2016 : Audit code - Supp de la macro ITK_EXPORT   *
 * VERSION : 5-0-0 : FA : LAIG-FA-MAC-143764-CS : 12 avril 2016 : Mise en forme du code (indentation, etc.) *
 * VERSION : 4-0-0 : DM : LAIG-DM-MAC-517-CNES : 6 janvier 2014 : Améliorations algorithmiques (Spec v2.1)	*
 * 															Création de l'algorithme SnowMaskCorrectionByLabel		*
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id: vnsSnowMaskCorrectionByLabelImageFilter.h 3545 2011-11-29 14:21:48Z cruffel $
 *                                                                                                          *
 ************************************************************************************************************/
#ifndef __vnsSnowMaskCorrectionByLabelImageFilter_h
#define __vnsSnowMaskCorrectionByLabelImageFilter_h

#include "vnsMaskUpdatingFunctor.h"
#include "itkProcessObject.h"

#include "itkUnaryFunctorImageFilter.h"
#include "itkBinaryBallStructuringElement.h"
#include "itkBinaryDilateImageFilter.h"
#include "itkXorImageFilter.h"

#include "vnsEqualValueThresholdFunctor.h"
#include "vnsSnowMaskValidationByLabelImageFilter.h"
#include "vnsMacro.h"

#include "otbImage.h"

namespace vns
{
    /** \class  SnowMaskCorrectionByLabelImageFilter
     * \brief This class updates for a given label the snow and the cloud reflectance masks to minimize the false detections of snow.
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
    template<class TInputMask, class TInputLabel, class TOutputMask>
        class SnowMaskCorrectionByLabelImageFilter : public itk::ProcessObject
        {
        public:
            /** Standard class typedefs. */
            typedef SnowMaskCorrectionByLabelImageFilter Self;
            typedef itk::ProcessObject Superclass;
            typedef itk::SmartPointer<Self> Pointer;
            typedef itk::SmartPointer<const Self> ConstPointer;

            /** Type macro */
            itkNewMacro(Self)

            /** Creation through object factory macro */
            itkTypeMacro(SnowMaskCorrectionByLabelImageFilter, ProcessObject )

            /** Some convenient typedefs. */
            typedef TInputMask InputMaskType;
            typedef typename InputMaskType::Pointer InputMaskPointer;
            typedef typename InputMaskType::ConstPointer InputMaskConstPointer;
            typedef typename InputMaskType::RegionType RegionType;
            typedef typename InputMaskType::PixelType InputMaskPixelType;

            typedef TInputLabel InputLabelType;
            typedef typename InputLabelType::Pointer InputLabelPointer;
            typedef typename InputLabelType::RegionType InputLabelRegionType;
            typedef typename InputLabelType::PixelType InputLabelPixelType;

            typedef TOutputMask OutputMaskType;
            typedef typename OutputMaskType::Pointer OutputMaskPointer;
            typedef typename OutputMaskType::RegionType OutputMaskRegionType;
            typedef typename OutputMaskType::PixelType OutputMaskPixelType;

            /** Internal types */
            // Conversion of the snow label image in a mask
            typedef Functor::EqualValueThresholdFunctor<InputLabelPixelType, InputMaskPixelType> EqualValueThresholdFunctorType;
            typedef itk::UnaryFunctorImageFilter<InputLabelType, InputMaskType, EqualValueThresholdFunctorType> EqualValueThresholdImageFilterType;
            typedef typename EqualValueThresholdImageFilterType::Pointer EqualValueThresholdPointer;

            // Dilatation of all cloud masks
            typedef itk::BinaryBallStructuringElement<InputMaskPixelType, InputMaskType::ImageDimension> StructuringElementType;
            typedef itk::BinaryDilateImageFilter<InputMaskType, InputMaskType, StructuringElementType> DilateFilterType;
            typedef typename DilateFilterType::Pointer DilateFilterPointer;

            // Generate the border of the dilated snow mask
            typedef itk::XorImageFilter<InputMaskType, InputMaskType, InputMaskType> XorFilterType;
            typedef typename XorFilterType::Pointer XorFilterPointer;

            // Test if all the border of a label is surrounded by cloud
            typedef SnowMaskValidationByLabelImageFilter<InputMaskType> SnowMaskLabelImageFilterType;
            typedef typename SnowMaskLabelImageFilterType::Pointer SnowMaskLabelImageFilterPointer;

            // Update the snow and cloud masks
            typedef Functor::MaskUpdatingImageFunctor<InputMaskPixelType, OutputMaskPixelType> UpdateMaskFunctorType;
            typedef itk::BinaryFunctorImageFilter<InputMaskType, OutputMaskType, InputMaskType, UpdateMaskFunctorType> UpdateMaskFilterType;
            typedef typename UpdateMaskFilterType::Pointer UpdateMaskFilterPointer;

            // input label
            itkSetMacro(Label, unsigned short)

            // radius of the stucturing element use for dilatation
            itkSetMacro(DilateRadius, unsigned short)

            // output mask value
            itkSetMacro(OutputMaskValue, OutputMaskPixelType)

            /** Set the snow mask at L2 coarse resolution */
            vnsMemberSetGetObjectMacro(SNWLabelInput, InputLabelType)

            /** Set the edge mask at L2 coarse resolution */
            vnsMemberSetGetObjectMacro(IPEDGSubInput, InputMaskType)

            /** Set the cloud reflectance mask at L2 coarse resolution */
            vnsMemberSetGetObjectMacro(IPCLDReflSubInput, InputMaskType)

            /** Set the cloud reflectance variation mask at L2 coarse resolution */
            vnsMemberSetGetObjectMacro(IPCLDReflVarSubInput, InputMaskType)

            /** Set the snow mask that will be updated at L2 coarse resolution */
            vnsMemberSetGetObjectMacro(UpdateIPSNWSubInput, InputMaskType)

            /** Set the cloud reflectance mask that will be updated at L2 coarse resolution */
            vnsMemberSetGetObjectMacro(UpdateIPCLDReflSubInput, InputMaskType)

            /** Get the updated IPCLDRefSubl mask */
            vnsMemberGetObjectMacro(IPCLDReflSubOutput, OutputMaskType)

            /** Get the updated IPSNWSub mask */
            vnsMemberGetObjectMacro(IPSNWSubOutput, OutputMaskType)

            virtual void
            UpdateWithCaching();

        protected:
            /** Constructor */
            SnowMaskCorrectionByLabelImageFilter();
            /** Destructor */
            virtual
            ~SnowMaskCorrectionByLabelImageFilter();

            /** PrintSelf method */
            virtual void
            PrintSelf(std::ostream& os, itk::Indent indent) const;

        private:
            SnowMaskCorrectionByLabelImageFilter(const Self&); //purposely not implemented
            void
            operator=(const Self&); //purposely not implemented

            /** Validate the presence of all inputs and their sizes. If at least one input
             * is missing, throws an exception. */
            void
            CheckInputs();

            /** Filters declaration */
            EqualValueThresholdPointer m_EqualValueThresholdFilterPtr;
            DilateFilterPointer m_DilateFilterPtr;
            XorFilterPointer m_XorFilterPtr;
            SnowMaskLabelImageFilterPointer m_SnowLabelCorrFilterPtr;
            UpdateMaskFilterPointer m_UpdateCLDMaskFilterPtr;
            UpdateMaskFilterPointer m_UpdateSNWMaskFilterPtr;

            /** Parameters declaration */
            unsigned short m_Label;
            unsigned short m_DilateRadius;
            OutputMaskPixelType m_OutputMaskValue;

        };

} // End namespace vns
#ifndef VNS_MANUAL_INSTANTIATION
#include "vnsSnowMaskCorrectionByLabelImageFilter.txx"
#endif

#endif /* __vnsSnowMaskCorrectionByLabelImageFilter_h */
