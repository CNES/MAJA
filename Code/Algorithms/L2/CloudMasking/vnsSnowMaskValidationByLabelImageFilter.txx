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
 * VERSION : 1.0.0 : FA : LAIG-FA-MAC-148399-CS : 27 octobre 2016 : Audit code - Correction qualite         *
 * VERSION : 1.0.0 : FA : LAIG-FA-MAC-1988-CNES : 21 octobre 2016 : Correction qualite (code mort)          *
 * VERSION : 5-0-0 : FA : LAIG-FA-MAC-143764-CS : 12 avril 2016 : Mise en forme du code (indentation, etc.) *
 * VERSION : 4-0-0 : DM : LAIG-DM-MAC-517-CNES : 6 janvier 2014 : Améliorations algorithmiques (Spec v2.1)	*
 * 															Création de l'algorithme SnowMaskValidation		*
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id: vnsSnowMaskValidationByLabelImageFilter.txx 5800 2013-05-30 17:24:16Z tfeuvrie $
 *                                                                                                          *
 ************************************************************************************************************/
#ifndef __vnsSnowMaskValidationByLabelImageFilter_txx
#define __vnsSnowMaskValidationByLabelImageFilter_txx

#include "vnsSnowMaskValidationByLabelImageFilter.h"

#include "vnsMath.h"
#include "itkImageRegionConstIteratorWithIndex.h"

namespace vns
{

    template<class TMaskType>
        PersistentSnowMaskValidationByLabelImageFilter<TMaskType>::PersistentSnowMaskValidationByLabelImageFilter() :
                m_NumberOfPixels(), m_SnowPixels(), m_IsValid(false)
        {
            this->SetNumberOfRequiredInputs(4);
            this->SetNumberOfIndexedInputs(4);
            this->SetNumberOfIndexedOutputs(3);

            // first output is a copy of the image, DataObject created by the
            // superclass
            //
            // allocate the data objects for the outputs which are
            // just decorators around pixel types
            //this->itk::ProcessObject::SetNthOutput(0, this->MakeOutput(0).GetPointer());
            typename ULObjectType::Pointer outputUL = static_cast<ULObjectType*>(this->MakeOutput(1).GetPointer());
            this->itk::ProcessObject::SetNthOutput(1, outputUL.GetPointer());

            typename ULObjectType::Pointer outputUL1 = static_cast<ULObjectType*>(this->MakeOutput(2).GetPointer());
            this->itk::ProcessObject::SetNthOutput(2, outputUL1.GetPointer());
        }

    template<class TMaskType>
        itk::DataObject::Pointer
        PersistentSnowMaskValidationByLabelImageFilter<TMaskType>::MakeOutput(unsigned int output)
        {
            // According to the output id, create the valid data
            switch (output)
                {
            case 0:
                {
                    return static_cast<itk::DataObject*>(TMaskType::New().GetPointer());
                }
            case 1:
                {
                    return static_cast<itk::DataObject*>(ULObjectType::New().GetPointer());
                }
            case 2:
                {
                    return static_cast<itk::DataObject*>(ULObjectType::New().GetPointer());
                }
            default:
                {
                    // might as well make an image
                    return static_cast<itk::DataObject*>(TMaskType::New().GetPointer());
                }
                }
        }

    template<class TMaskType>
        void
        PersistentSnowMaskValidationByLabelImageFilter<TMaskType>::CheckInputs()
        {
            // Get the Inpu tpointers
            MaskPointerType snwBorderMaskPtr = this->GetSNWLabelBorderMask();
            MaskPointerType edgMaskPtr = this->GetIPEDGMask();
            MaskPointerType cldReflMaskPtr = this->GetIPCLDReflMask();
            MaskPointerType cldReflVarMaskPtr = this->GetIPCLDReflVarMask();

            // Checks the validity of the inputs pointers
            if ((snwBorderMaskPtr.IsNull() == true) || (edgMaskPtr.IsNull() == true) || (cldReflMaskPtr.IsNull() == true)
                    || (cldReflVarMaskPtr.IsNull() == true))
            {
                itkExceptionMacro(<< "At least one input is missing.");
            }

            // Check the input images size at L2 Coarse Resolution
            vnsChecking4ImagesSizeMacro(snwBorderMaskPtr, edgMaskPtr, cldReflMaskPtr, cldReflVarMaskPtr)

        }

    template<class TMaskType>
        void
        PersistentSnowMaskValidationByLabelImageFilter<TMaskType>::ThreadedGenerateData(const MaskRegionType& outputRegionForThread,
                itk::ThreadIdType threadId)
        {
            // CHecks the inputs images size
            CheckInputs();

            // Grab the inputs
            MaskPointerType snwBorderMaskPtr = const_cast<MaskType *>(this->GetSNWLabelBorderMask());
            MaskPointerType edgMaskPtr = const_cast<MaskType *>(this->GetIPEDGMask());
            MaskPointerType cldReflMaskPtr = const_cast<MaskType *>(this->GetIPCLDReflMask());
            MaskPointerType cldReflVarMaskPtr = const_cast<MaskType *>(this->GetIPCLDReflVarMask());

            // Support progress methods/callbacks

            // Iterators
            itk::ImageRegionConstIteratorWithIndex<MaskType> itSnwBorder(snwBorderMaskPtr, outputRegionForThread);
            itk::ImageRegionConstIteratorWithIndex<MaskType> itEdg(edgMaskPtr, outputRegionForThread);
            itk::ImageRegionConstIteratorWithIndex<MaskType> itCldRefl(cldReflMaskPtr, outputRegionForThread);
            itk::ImageRegionConstIteratorWithIndex<MaskType> itCldReflVar(cldReflVarMaskPtr, outputRegionForThread);

            // Set the iterators to the begenning
            itSnwBorder.GoToBegin();
            itEdg.GoToBegin();
            itCldRefl.GoToBegin();
            itCldReflVar.GoToBegin();

            // do the work
            while ((itSnwBorder.IsAtEnd() == false))
            {
                // Get the snow pixel
                const MaskPixelType snwPix = itSnwBorder.Get();

                // Get the mask values
                const MaskPixelType edgPix = itEdg.Get();
                const MaskPixelType cldReflPix = itCldRefl.Get();
                const MaskPixelType cldReflVarPix = itCldReflVar.Get();

                // If the snow border pixel in the label is also cloudy or edge
                if (snwPix > 0)
                {
                    m_SnowPixels[threadId] = m_SnowPixels[threadId] + 1;
                    if ((cldReflPix > 0) || (cldReflVarPix > 0) || (edgPix > 0))
                    {
                        m_NumberOfPixels[threadId] = m_NumberOfPixels[threadId] + 1;
                    }
                }
                // Increments the iterators
                ++itSnwBorder;

                ++itEdg;
                ++itCldRefl;
                ++itCldReflVar;
            }
        }

    template<class TMaskType>
        void
        PersistentSnowMaskValidationByLabelImageFilter<TMaskType>::AllocateOutputs()
        {
            // This is commented to prevent the streaming of the whole image for the first stream strip
            // It shall not cause any problem because the output image of this filter is not intended to be used.
            //InputImagePointer image = const_cast< TInputImage * >( this->GetInput() );
            //this->GraftOutput( image );
            // Nothing that needs to be allocated for the remaining outputs
        }

    template<class TMaskType>
        void
        PersistentSnowMaskValidationByLabelImageFilter<TMaskType>::GenerateOutputInformation()
        {
            Superclass::GenerateOutputInformation();

            // This is the part that allows to do threading.
            if (this->GetInput())
            {
                // Copy information
                this->GetOutput()->CopyInformation(this->GetInput());
                this->GetOutput()->SetLargestPossibleRegion(this->GetInput()->GetLargestPossibleRegion());

                if (this->GetOutput()->GetRequestedRegion().GetNumberOfPixels() == 0)
                {
                    this->GetOutput()->SetRequestedRegion(this->GetOutput()->GetLargestPossibleRegion());
                }
            }
        }

    template<class TMaskType>
        void
        PersistentSnowMaskValidationByLabelImageFilter<TMaskType>::Synthetize(void)
        {
            //// Here we fuse all the found maps.

            unsigned int numberOfThreads = this->GetNumberOfThreads();
            unsigned long totalSnowPixels(0);
            unsigned long totalNumberOfPixels(0);

            // Loop over the number of threads
            for (unsigned int threadId = 0; threadId < numberOfThreads; threadId++)
            {
                // Get the total number of snow pixels and valid pixels
                totalSnowPixels = totalSnowPixels + static_cast<unsigned long>(m_SnowPixels[threadId]);
                totalNumberOfPixels = totalNumberOfPixels + static_cast<unsigned long>(m_NumberOfPixels[threadId]);
            }

            // if all (lab_border in (Cloud_mask_sub.refl or Cloud_mask_sub.refl_var or edge_mask_sub)) then
            // the pixels of the label are not snowy
            if ((totalSnowPixels == totalNumberOfPixels) && (totalSnowPixels > 0))
            {
                m_IsValid = true;
            }

            // Grab the outputs
            typename ULObjectType::Pointer outputValidPixels = static_cast<ULObjectType*>(this->itk::ProcessObject::GetOutput(1));
            typename ULObjectType::Pointer outputSnowPixels = static_cast<ULObjectType*>(this->itk::ProcessObject::GetOutput(2));

            // The values to the outputs
            outputValidPixels->Set(totalNumberOfPixels);
            outputSnowPixels->Set(totalSnowPixels);

        }

    template<class TMaskType>
        void
        PersistentSnowMaskValidationByLabelImageFilter<TMaskType>::Reset(void)
        {
            MaskPointerType snwBorderMaskPtr = const_cast<MaskType *>(this->GetSNWLabelBorderMask());
            MaskPointerType edgMaskPtr = const_cast<MaskType *>(this->GetIPEDGMask());
            MaskPointerType cldReflMaskPtr = const_cast<MaskType *>(this->GetIPCLDReflMask());
            MaskPointerType cldReflVarMaskPtr = const_cast<MaskType *>(this->GetIPCLDReflVarMask());

            // update the input image information
            snwBorderMaskPtr->UpdateOutputInformation();
            edgMaskPtr->UpdateOutputInformation();
            cldReflMaskPtr->UpdateOutputInformation();
            cldReflVarMaskPtr->UpdateOutputInformation();

            // Load the number of thread
            unsigned int numberOfThreads = this->GetNumberOfThreads();
            unsigned long ulvalue(0);

            // Initialize the output values
            m_NumberOfPixels = ULVectorType(numberOfThreads, ulvalue);
            m_SnowPixels = ULVectorType(numberOfThreads, ulvalue);
            m_IsValid = false;

        }

    template<class TMaskType>
        void
        PersistentSnowMaskValidationByLabelImageFilter<TMaskType>::PrintSelf(std::ostream& os, itk::Indent indent) const
        {
            Superclass::PrintSelf(os, indent);

            // Compute the number of threads
            unsigned int lNumberOfThreads = this->GetNumberOfThreads();

            // Display the sum of value per label and par thread
            os << "====== Number of valid pixel by thread ======" << std::endl;
            for (unsigned int threadId = 0; threadId < lNumberOfThreads; threadId++)
            {
                unsigned long nbPix = m_NumberOfPixels[threadId];
                os << indent << nbPix << std::endl;
            }
        }

    /** PrintSelf method */
    template<class TMaskType>
        void
        SnowMaskValidationByLabelImageFilter<TMaskType>::PrintSelf(std::ostream& os, itk::Indent indent) const
        {
            //Superclass::PrintSelf(os, indent);

            // Display the number of valid and snow pixels in the label
            os << "====== Number of valid pixels ======" << indent << (int) this->GetNumberOfValidPixels() << std::endl;
            os << "====== Number of snow pixels ======" << indent << (int) this->GetSnowPixels() << std::endl;

            os << std::endl;
        }

} //end of namespace

#endif /*__vnsSnowMaskValidationByLabelImageFilter_txx*/
