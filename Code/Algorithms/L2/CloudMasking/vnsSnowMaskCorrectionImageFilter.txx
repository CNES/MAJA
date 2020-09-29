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
 * VERSION : 1.0.0 : FA : LAIG-FA-MAC-148399-CS : 03 novembre 2016 : Audit code - Correction qualite        *
 * VERSION : 4-0-0 : DM : LAIG-DM-MAC-517-CNES : 6 janvier 2014 : Améliorations algorithmiques (Spec v2.1)	*
 * 															Création de l'algorithme SnowMaskCorrection		*
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id: vnsSnowMaskCorrectionImageFilter.txx 5749 2013-05-29 14:44:02Z tfeuvrie $
 *                                                                                                          *
 ************************************************************************************************************/
#ifndef __vnsSnowMaskCorrectionImageFilter_txx
#define __vnsSnowMaskCorrectionImageFilter_txx

#include "vnsSnowMaskCorrectionImageFilter.h"
#include "otbImageFileWriter.h"
#include "vnsUtilities.h"

namespace vns
{

    // Constructor
    template<class TMaskType>
        SnowMaskCorrectionImageFilter<TMaskType>::SnowMaskCorrectionImageFilter() :
                m_DilateRadius(0), m_OutputMaskValue(0)
        {

            // Input/Output initialization
            this->SetNumberOfIndexedInputs(4);
            this->SetNumberOfRequiredInputs(2);
            this->SetNumberOfIndexedOutputs(2);

            // Filters initialization
            m_LabelFilterPtr = LabelizeFilterType::New();
            m_SnowLabelCorrFilterPtr = SnowMaskCorrectionByLabelImageFilterType::New();

            m_LabelReader = ReaderLabelType::New();

            m_SnowMaskLabelFilterList = SnowMaskCorrectionByLabelFilterListType::New();

            // Outputs initialization
            m_IPCLDReflSubOutput = MaskType::New();
            m_IPSNWSubOutput = MaskType::New();

        }

    // Destructor
    template<class TMaskType>
        SnowMaskCorrectionImageFilter<TMaskType>::~SnowMaskCorrectionImageFilter()
        {
        }

    template<class TMaskType>
        void
        SnowMaskCorrectionImageFilter<TMaskType>::CheckInputs()
        {

            // Grab all inputs
            MaskPointer lIPSNWSubPtr = this->GetIPSNWSubInput();
            MaskPointer lIPEDGSubPtr = this->GetIPEDGSubInput();
            MaskPointer lIPCLDReflSubPtr = this->GetIPCLDReflSubInput();
            MaskPointer lIPCLDReflVarSubPtr = this->GetIPCLDReflVarSubInput();

            lIPSNWSubPtr->UpdateOutputInformation();
            lIPEDGSubPtr->UpdateOutputInformation();
            lIPCLDReflSubPtr->UpdateOutputInformation();
            lIPCLDReflVarSubPtr->UpdateOutputInformation();

            // Checks the validity of all input pointers
            if ((lIPSNWSubPtr.IsNull() == true) || (lIPEDGSubPtr.IsNull() == true) || (lIPCLDReflSubPtr.IsNull() == true)
                    || (lIPCLDReflVarSubPtr.IsNull() == true))
            {
                itkExceptionMacro(<< "At least one input is missing.");
            }

            // Check the input images size at L2 Coarse Resolution
            vnsChecking4ImagesSizeMacro(lIPSNWSubPtr, lIPEDGSubPtr, lIPCLDReflSubPtr, lIPCLDReflVarSubPtr)

        }

    //================================== GenerateData ==================================/
    // Main computation method.
    template<class TMaskType>
        void
        SnowMaskCorrectionImageFilter<TMaskType>::UpdateWithCaching()
        {
            CheckInputs();

            MaskPointer lIPSNWSubPtr = this->GetIPSNWSubInput();
            MaskPointer lIPEDGSubPtr = this->GetIPEDGSubInput();
            MaskPointer lIPCLDReflSubPtr = this->GetIPCLDReflSubInput();
            MaskPointer lIPCLDReflVarSubPtr = this->GetIPCLDReflVarSubInput();

            lIPSNWSubPtr->UpdateOutputInformation();
            lIPEDGSubPtr->UpdateOutputInformation();
            lIPCLDReflSubPtr->UpdateOutputInformation();
            lIPCLDReflVarSubPtr->UpdateOutputInformation();

            SnowMaskCorrectionByLabelImageFilterPointer l_SnowMaskLabelFilterPtr;

            // Snow labelling
            m_LabelFilterPtr->SetInput(lIPSNWSubPtr);
            m_LabelFilterPtr->UpdateOutputInformation();

            // Use a specific Caching Macro (that call an otbImageFileWriter) because using StreamingImageFileWriter generates
            // which each stream is the first input image one.
            m_LabelFilterPtr->Update();

            const unsigned long l_NumberOfLabels(m_LabelFilterPtr->GetObjectCount());
            vnsLogDebugMacro("Number of input objects = " << l_NumberOfLabels)

            // Start at index 1, 0 is the background
            for (unsigned int lLabel = 1; lLabel <= l_NumberOfLabels; lLabel++)
            {
                // Call the snow mask correction by label filter and update the output masks at each iteration
                l_SnowMaskLabelFilterPtr = SnowMaskCorrectionByLabelImageFilterType::New();

                l_SnowMaskLabelFilterPtr->SetSNWLabelInput(m_LabelFilterPtr->GetOutput());
                l_SnowMaskLabelFilterPtr->SetIPEDGSubInput(lIPEDGSubPtr);
                l_SnowMaskLabelFilterPtr->SetIPCLDReflSubInput(lIPCLDReflSubPtr);
                l_SnowMaskLabelFilterPtr->SetIPCLDReflVarSubInput(lIPCLDReflVarSubPtr);

                // First iteration => update the input snow and cloud reflectance masks
                if (m_SnowMaskLabelFilterList->Size() == 0)
                {
                    l_SnowMaskLabelFilterPtr->SetUpdateIPSNWSubInput(lIPSNWSubPtr);
                    l_SnowMaskLabelFilterPtr->SetUpdateIPCLDReflSubInput(lIPCLDReflSubPtr);
                }
                // Then update the previous updated snow and cloud reflectance masks
                else
                {
                    l_SnowMaskLabelFilterPtr->SetUpdateIPSNWSubInput(m_SnowMaskLabelFilterList->Back()->GetIPSNWSubOutput());
                    l_SnowMaskLabelFilterPtr->SetUpdateIPCLDReflSubInput(m_SnowMaskLabelFilterList->Back()->GetIPCLDReflSubOutput());
                }

                l_SnowMaskLabelFilterPtr->SetDilateRadius(m_DilateRadius);
                l_SnowMaskLabelFilterPtr->SetOutputMaskValue(m_OutputMaskValue);
                l_SnowMaskLabelFilterPtr->SetLabel(lLabel);

                l_SnowMaskLabelFilterPtr->UpdateWithCaching();

                m_SnowMaskLabelFilterList->PushBack(l_SnowMaskLabelFilterPtr);

            } // end label loop

            // If the input snow mask is not empty
            if (l_NumberOfLabels != 0)
            {
                m_IPCLDReflSubOutput = m_SnowMaskLabelFilterList->Back()->GetIPCLDReflSubOutput();
                m_IPSNWSubOutput = m_SnowMaskLabelFilterList->Back()->GetIPSNWSubOutput();
            }
            // The input masks are not updated
            else
            {
                m_IPCLDReflSubOutput = lIPCLDReflSubPtr;
                m_IPSNWSubOutput = lIPSNWSubPtr;
            }

        }

    // PrintSelf method
    template<class TMaskType>
        void
        SnowMaskCorrectionImageFilter<TMaskType>::PrintSelf(std::ostream& os, itk::Indent indent) const
        {
            Superclass::PrintSelf(os, indent);

        }

} // End namespace vns

#endif /* __vnsSnowMaskCorrectionImageFilter_txx */
