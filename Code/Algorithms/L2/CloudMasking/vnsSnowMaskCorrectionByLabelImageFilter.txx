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
 * VERSION : 1.0.0 : FA : LAIG-FA-MAC-148399-CS : 03 novembre 2016 : Audit code - Correction qualite        *
 * VERSION : 5-0-0 : FA : LAIG-FA-MAC-143764-CS : 12 avril 2016 : Mise en forme du code (indentation, etc.) *
 * VERSION : 4-0-0 : DM : LAIG-DM-MAC-517-CNES : 6 janvier 2014 : Améliorations algorithmiques (Spec v2.1)	*
 * 															Création de l'algorithme SnowMaskCorrectionByLabel		*
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id: vnsSnowMaskCorrectionByLabelImageFilter.txx 5749 2013-05-29 14:44:02Z tfeuvrie $
 *                                                                                                          *
 ************************************************************************************************************/
#ifndef __vnsSnowMaskCorrectionByLabelImageFilter_txx
#define __vnsSnowMaskCorrectionByLabelImageFilter_txx

#include "vnsSnowMaskCorrectionByLabelImageFilter.h"
#include "otbImageFileWriter.h"
#include "vnsUtilities.h"

namespace vns
{

    // Constructor
    template<class TInputMask, class TInputLabel, class TOutputMask>
        SnowMaskCorrectionByLabelImageFilter<TInputMask, TInputLabel, TOutputMask>::SnowMaskCorrectionByLabelImageFilter() :
                m_Label(0), m_DilateRadius(0), m_OutputMaskValue(0)
        {

            // Input/Output initialization
            this->SetNumberOfIndexedInputs(6);
            this->SetNumberOfRequiredInputs(6);
            this->SetNumberOfIndexedOutputs(2);

            // Filters initialization
            m_EqualValueThresholdFilterPtr = EqualValueThresholdImageFilterType::New();
            m_DilateFilterPtr = DilateFilterType::New();
            m_XorFilterPtr = XorFilterType::New();
            m_SnowLabelCorrFilterPtr = SnowMaskLabelImageFilterType::New();
            m_UpdateCLDMaskFilterPtr = UpdateMaskFilterType::New();
            m_UpdateSNWMaskFilterPtr = UpdateMaskFilterType::New();

            // Outputs initialization
            m_IPCLDReflSubOutput = OutputMaskType::New();
            m_IPSNWSubOutput = OutputMaskType::New();

        }

    // Destructor
    template<class TInputMask, class TInputLabel, class TOutputMask>
        SnowMaskCorrectionByLabelImageFilter<TInputMask, TInputLabel, TOutputMask>::~SnowMaskCorrectionByLabelImageFilter()
        {
        }

    template<class TInputMask, class TInputLabel, class TOutputMask>
        void
        SnowMaskCorrectionByLabelImageFilter<TInputMask, TInputLabel, TOutputMask>::CheckInputs()
        {

            // Grab all inputs
            InputLabelPointer lSNWLabelPtr = this->GetSNWLabelInput();
            InputMaskPointer lIPEDGSubPtr = this->GetIPEDGSubInput();
            InputMaskPointer lIPCLDReflSubPtr = this->GetIPCLDReflSubInput();
            InputMaskPointer lIPCLDReflVarSubPtr = this->GetIPCLDReflVarSubInput();
            InputMaskPointer lUpdateSNWPtr = this->GetUpdateIPSNWSubInput();
            InputMaskPointer lUpdateCLDReflPtr = this->GetUpdateIPCLDReflSubInput();

            // Update output information of all inputs
            lSNWLabelPtr->UpdateOutputInformation();
            lIPEDGSubPtr->UpdateOutputInformation();
            lIPCLDReflSubPtr->UpdateOutputInformation();
            lIPCLDReflVarSubPtr->UpdateOutputInformation();
            lUpdateSNWPtr->UpdateOutputInformation();
            lUpdateCLDReflPtr->UpdateOutputInformation();

            // Checks the validity of all inputs pointers
            if ((lSNWLabelPtr.IsNull() == true) || (lIPEDGSubPtr.IsNull() == true) || (lIPCLDReflSubPtr.IsNull() == true)
                    || (lIPCLDReflVarSubPtr.IsNull() == true) || (lUpdateSNWPtr.IsNull() == true) || (lUpdateCLDReflPtr.IsNull() == true))
            {
                itkExceptionMacro(<< "At least one input is missing.");
            }

            // Check the input images size at L2 Coarse Resolution
            vnsChecking6ImagesSizeMacro(lSNWLabelPtr, lIPEDGSubPtr, lIPCLDReflSubPtr, lIPCLDReflVarSubPtr, lUpdateSNWPtr, lUpdateCLDReflPtr)

        }

    //================================== GenerateData ==================================/
    // Main computation method.
    template<class TInputMask, class TInputLabel, class TOutputMask>
        void
        SnowMaskCorrectionByLabelImageFilter<TInputMask, TInputLabel, TOutputMask>::UpdateWithCaching()
        {
            CheckInputs();

            InputLabelPointer lSNWLabelPtr = this->GetSNWLabelInput();
            InputMaskPointer lIPEDGSubPtr = this->GetIPEDGSubInput();
            InputMaskPointer lIPCLDReflSubPtr = this->GetIPCLDReflSubInput();
            InputMaskPointer lIPCLDReflVarSubPtr = this->GetIPCLDReflVarSubInput();
            InputMaskPointer lUpdateSNWPtr = this->GetUpdateIPSNWSubInput();
            InputMaskPointer lUpdateCLDReflPtr = this->GetUpdateIPCLDReflSubInput();

            //lab_mask = where(labels_snow == lab)
            m_EqualValueThresholdFilterPtr->SetInput(lSNWLabelPtr);
            m_EqualValueThresholdFilterPtr->GetFunctor().SetThresholdValue(m_Label);
            m_EqualValueThresholdFilterPtr->GetFunctor().SetEqualValue(1);
            m_EqualValueThresholdFilterPtr->GetFunctor().SetOutsideValue(0);

            //lab_mask_dilated = binary_dilation(lab_mask)
            InputMaskPixelType l_DilateValue(1);
            StructuringElementType lStructuringElement;
            lStructuringElement.SetRadius(m_DilateRadius); // 3x3 structuring element
            lStructuringElement.CreateStructuringElement();

            m_DilateFilterPtr->SetKernel(lStructuringElement);
            m_DilateFilterPtr->SetInput(m_EqualValueThresholdFilterPtr->GetOutput());
            m_DilateFilterPtr->SetDilateValue(l_DilateValue);

            m_DilateFilterPtr->UpdateOutputInformation();

            // Get the border of the dilated snow label
            //lab_border = where(lab_mask == 0 and lab_mask_dilated == 1)
            // A=0 B=0 A XOR B = 0
            // A=0 B=1 A XOR B = 1
            // A=1 B=1 A XOR B = 0
            m_XorFilterPtr->SetInput1(m_EqualValueThresholdFilterPtr->GetOutput());
            m_XorFilterPtr->SetInput2(m_DilateFilterPtr->GetOutput());

            // Test if the current snow label is a false detection
            //if all (lab_border in (Cloud_mask_sub.refl or Cloud_mask_sub.refl_var or edge_mask_sub))
            m_SnowLabelCorrFilterPtr->SetSNWLabelBorderMask(m_XorFilterPtr->GetOutput());
            m_SnowLabelCorrFilterPtr->SetIPEDGMask(lIPEDGSubPtr);
            m_SnowLabelCorrFilterPtr->SetIPCLDReflMask(lIPCLDReflSubPtr);
            m_SnowLabelCorrFilterPtr->SetIPCLDReflVarMask(lIPCLDReflVarSubPtr);

            m_SnowLabelCorrFilterPtr->Update();

            if (m_SnowLabelCorrFilterPtr->GetIsValid() == true)
            {

                // then:
                // discard snow detection for all pixels in the label
                // Snow_mask_sub[where lab_mask == 1] = 0
                m_UpdateSNWMaskFilterPtr->SetInput1(lUpdateSNWPtr);
                m_UpdateSNWMaskFilterPtr->SetInput2(m_EqualValueThresholdFilterPtr->GetOutput());
                m_UpdateSNWMaskFilterPtr->GetFunctor().SetThresholdRefMaskValue(0);
                m_UpdateSNWMaskFilterPtr->GetFunctor().SetOutputMaskValue(0);

                m_UpdateSNWMaskFilterPtr->GetOutput()->UpdateOutputInformation();
                m_UpdateSNWMaskFilterPtr->UpdateOutputInformation();

                // Cloud_mask_sub.refl [where lab_mask == 1] = 1
                m_UpdateCLDMaskFilterPtr->SetInput1(lUpdateCLDReflPtr);
                m_UpdateCLDMaskFilterPtr->SetInput2(m_EqualValueThresholdFilterPtr->GetOutput());
                m_UpdateCLDMaskFilterPtr->GetFunctor().SetThresholdRefMaskValue(0);
                m_UpdateCLDMaskFilterPtr->GetFunctor().SetOutputMaskValue(m_OutputMaskValue);

                m_UpdateCLDMaskFilterPtr->GetOutput()->UpdateOutputInformation();
                m_UpdateCLDMaskFilterPtr->UpdateOutputInformation();

                m_IPCLDReflSubOutput = m_UpdateCLDMaskFilterPtr->GetOutput();
                m_IPSNWSubOutput = m_UpdateSNWMaskFilterPtr->GetOutput();

            } // end if IsValid
            else
            {
                m_IPCLDReflSubOutput = lUpdateCLDReflPtr;
                m_IPSNWSubOutput = lUpdateSNWPtr;
            }

        }

    // PrintSelf method
    template<class TInputMask, class TInputLabel, class TOutputMask>
        void
        SnowMaskCorrectionByLabelImageFilter<TInputMask, TInputLabel, TOutputMask>::PrintSelf(std::ostream& os, itk::Indent indent) const
        {
            Superclass::PrintSelf(os, indent);

        }

} // End namespace vns

#endif /* __vnsSnowMaskCorrectionByLabelImageFilter_txx */
