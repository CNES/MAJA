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
 * VERSION : 5-0-0 : FA : LAIG-FA-MAC-143764-CS : 12 avril 2016 : Mise en forme du code (indentation, etc.) *
 * VERSION : 4-1-2 : FA : LAIG-FA-MAC-1180-CNES : 19 mars 2015 : Pas de corrélation si date courante = date STO	*
 * VERSION : 1-0-0 : <TypeFT> : <NumFT> : 25 mai 2010 : Creation                                                           
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id$
 *                                                                                                          *
 ************************************************************************************************************/
#ifndef __vnsCloudMaskExtensionCorrelationGenerator_txx
#define __vnsCloudMaskExtensionCorrelationGenerator_txx

#include "vnsCloudMaskExtensionCorrelationGenerator.h"

#include "vnsLoggers.h"

namespace vns
{

    /** Constructor */
    template<class TInputImage, class TInputMask, class TLabelImage, class TOutputImage>
        CloudMaskExtensionCorrelationGenerator<TInputImage, TInputMask, TLabelImage, TOutputImage>::CloudMaskExtensionCorrelationGenerator() :
                m_NoData(0.), m_CorrelBandTOCR(0), m_CorrelBandRCR(0), m_LabelCount(0), m_CorrelThreshold(0.), m_MinPixelCorrelation(0), m_JDay(
                        0.)
        {
            // Input/Output initialization
            this->SetNumberOfIndexedInputs(9);
            this->SetNumberOfRequiredInputs(9);

            //    this->SetNumberOfRequiredOutputs(1);
            //    this->SetNumberOfIndexedOutputs(1);

            // Parameters initialization
            m_NCorrel.Fill(3);

            // Output image initialization
            m_IPCLDExtOutput = OutputImageType::New();
            m_IPCLDExtCorelFlagOutput = OutputImageType::New();

            // Filters initialization
            m_DetectedCloudFilter = CloudFilterType::New();
            m_CloudMaskExtensionFilter = CloudMaskExtensionFilterType::New();

            m_CloudFoundMap.clear();

        }

    /** Destructor */
    template<class TInputImage, class TInputMask, class TLabelImage, class TOutputImage>
        CloudMaskExtensionCorrelationGenerator<TInputImage, TInputMask, TLabelImage, TOutputImage>::~CloudMaskExtensionCorrelationGenerator()
        {
        }

    //====================== Check the input images =============================//
    template<class TInputImage, class TInputMask, class TLabelImage, class TOutputImage>
        void
        CloudMaskExtensionCorrelationGenerator<TInputImage, TInputMask, TLabelImage, TOutputImage>::CheckInputs()
        {

            if ((this->GetIPCLDExtInput() == NULL) || (this->GetIPTOCRInput() == NULL) || (this->GetL2inRCRInput() == NULL)
                    || (this->GetL2inSTOInput() == NULL) || (this->GetIPCLDReflInput() == NULL) || (this->GetIPCLDReflVarInput() == NULL)
                    || (this->GetIPCLDExtLabelInput() == NULL) || (this->GetIPEDGInput() == NULL) || (this->GetL2inNDTInput() == NULL))
            {
                vnsExceptionBusinessMacro("CloudMaskExtensionCorrelationGenerator: Internal error: At least one of the input is null.");
            }

        }

    //============================ Generate the output image ===============================//
    template<class TInputImage, class TInputMask, class TLabelImage, class TOutputImage>
        void
        CloudMaskExtensionCorrelationGenerator<TInputImage, TInputMask, TLabelImage, TOutputImage>::UpdateWithCaching()
        {

            this->CheckInputs();

            dynamic_cast<InputMaskType *>(itk::ProcessObject::GetInput(4))->UpdateOutputInformation();
            dynamic_cast<InputMaskType *>(itk::ProcessObject::GetInput(5))->UpdateOutputInformation();
            dynamic_cast<LabelImageType*>(itk::ProcessObject::GetInput(6))->UpdateOutputInformation();

            // Check if the cloud mask extension pixel has also been detected by the reflectance
            // and the variation reflectance tests :
            // pixels set to one in the cloud mask extension and in the
            // reflectance threshold cloud mask or in the reflectance variation threshold one

            m_DetectedCloudFilter->SetIPCLDRefl(this->GetIPCLDReflInput());
            m_DetectedCloudFilter->SetIPCLDReflVar(this->GetIPCLDReflVarInput());
            m_DetectedCloudFilter->SetIPCLDExtLabel(this->GetIPCLDExtLabelInput());

            m_DetectedCloudFilter->SetExcludedLabelValue(0);
            m_DetectedCloudFilter->SetInsideValue(1);

            // Persistent filter
            m_DetectedCloudFilter->Update();

            // Get the output map of cloud already detected
            m_CloudFoundMap = m_DetectedCloudFilter->GetCloudFound();

            m_CloudMaskExtensionFilter->SetIPCLDExtInput(this->GetIPCLDExtInput());
            m_CloudMaskExtensionFilter->SetIPTOCRInput(this->GetIPTOCRInput());
            m_CloudMaskExtensionFilter->SetL2inRCRInput(this->GetL2inRCRInput());
            m_CloudMaskExtensionFilter->SetL2inSTOInput(this->GetL2inSTOInput());
            m_CloudMaskExtensionFilter->SetIPCLDReflInput(this->GetIPCLDReflInput());
            m_CloudMaskExtensionFilter->SetIPCLDReflVarInput(this->GetIPCLDReflVarInput());
            m_CloudMaskExtensionFilter->SetIPCLDExtLabelInput(this->GetIPCLDExtLabelInput());
            m_CloudMaskExtensionFilter->SetIPEDGInput(this->GetIPEDGInput());
            m_CloudMaskExtensionFilter->SetL2inNDTInput(this->GetL2inNDTInput());

            // Set parameters
            m_CloudMaskExtensionFilter->SetNoData(m_NoData);
            m_CloudMaskExtensionFilter->SetLabelCount(m_LabelCount);
            m_CloudMaskExtensionFilter->SetCorrelBandTOCR(m_CorrelBandTOCR);
            m_CloudMaskExtensionFilter->SetCorrelBandRCR(m_CorrelBandRCR);
            m_CloudMaskExtensionFilter->SetNCorrel(m_NCorrel);
            m_CloudMaskExtensionFilter->SetCorrelThreshold(m_CorrelThreshold);
            m_CloudMaskExtensionFilter->SetMinPixelCorrelation(m_MinPixelCorrelation);
            m_CloudMaskExtensionFilter->SetJDay(m_JDay);

            // Set the map of cloud already detected
            m_CloudMaskExtensionFilter->SetCloudFoundMap(m_CloudFoundMap);
            m_CloudMaskExtensionFilter->UpdateOutputInformation();

            // Get the output image of the filter
            m_IPCLDExtOutput = m_CloudMaskExtensionFilter->GetIPCLDExtOutput();
            m_IPCLDExtCorelFlagOutput = m_CloudMaskExtensionFilter->GetIPCLDExtCorelFlagOutput();
        }

    /** PrintSelf method */
    template<class TInputImage, class TInputMask, class TLabelImage, class TOutputImage>
        void
        CloudMaskExtensionCorrelationGenerator<TInputImage, TInputMask, TLabelImage, TOutputImage>::PrintSelf(std::ostream& os,
                itk::Indent indent) const
        {
            Superclass::PrintSelf(os, indent);
            os << "m_NoData:                    " << m_NoData << std::endl;
            os << "m_LabelCount:                " << m_LabelCount << std::endl;
            os << "m_CorrelBandTOCR:            " << m_CorrelBandTOCR << std::endl;
            os << "m_CorrelBandRCR:             " << m_CorrelBandRCR << std::endl;
            os << "m_NCorrel:                   " << m_NCorrel << std::endl;
            os << "m_CorrelThreshold:           " << m_CorrelThreshold << std::endl;
            os << "m_MinPixelCorrelation:       " << m_MinPixelCorrelation << std::endl;
            os << "m_JDay:                      " << m_JDay << std::endl;
            Utilities::PrintList("m_STOListOfDates", m_STOListOfDates, os);
        }

} // End namespace vns

#endif /* __vnsCloudMaskExtensionCorrelationGenerator_txx */
