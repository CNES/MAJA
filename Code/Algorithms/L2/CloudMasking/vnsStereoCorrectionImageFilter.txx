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
 * VERSION : 2.0.0 : DM : LAIG-DM-MAJA-153436-CS : 13 avril 2017 : Refactoring de classe doublons           *
 * VERSION : 1.0.0 : FA : LAIG-FA-MAC-148399-CS : 27 octobre 2016 : Audit code - Correction qualite         *
 * VERSION : 5-0-0 : FA : LAIG-FA-MAC-143764-CS : 12 avril 2016 : Mise en forme du code (indentation, etc.) *
 * VERSION : 4-0-0 : DM : LAIG-DM-MAC-517-CNES : 16 janvier 2014 : Améliorations algorithmiques (Spec v2.1)	*
 * 													Ajout d'un commentaire									*
 * VERSION : 1-0-0 : <TypeFT> : <NumFT> : 31 mai 2010 : Creation                                                           
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id$
 *                                                                                                          *
 ************************************************************************************************************/
#ifndef __vnsStereoCorrectionImageFilter_txx
#define __vnsStereoCorrectionImageFilter_txx

#include "vnsStereoCorrectionImageFilter.h"

namespace vns
{

    /** Constructor */
    template<class TInputImage, class TInputMask, class TLabelImage, class TOutputImage>
        StereoCorrectionImageFilter<TInputImage, TInputMask, TLabelImage, TOutputImage>::StereoCorrectionImageFilter()
        {
            // Input/Output initialization
            this->SetNumberOfIndexedInputs(2);
            this->SetNumberOfRequiredInputs(2);
            this->SetNumberOfIndexedOutputs(0);

            // Filters initialization
            m_LabelFilter = LabelFilterType::New();
            m_LabelStatFilter = LabelStatFilterType::New();
            m_ChangeValueFilter = ChangeValueFilterType::New();
            m_ChangeLabelStatFilter = LabelStatFilterType::New();

            m_StereoAltitudeFilter = StereoAltitudeCorrectionImageFilterType::New();

            m_CLADefaultConstantImageFilter = ConstantFilterType::New();

            m_Threshold = ThresholderType::New();

            // Parameters initialization
            m_ExcludedValue = 0;
            m_DefaultValue = 0;
            m_KsigmaAltitude = 0;
            m_DefaultAltitude = 0;

            // Image for caching
            m_IPCLAOutput = OutputImageType::New();
            m_IPCLDAllLabelOutput = LabelImageType::New();

        }

    /** Destructor */
    template<class TInputImage, class TInputMask, class TLabelImage, class TOutputImage>
        StereoCorrectionImageFilter<TInputImage, TInputMask, TLabelImage, TOutputImage>::~StereoCorrectionImageFilter()
        {
        }

    /** Main computation method. */
    template<class TInputImage, class TInputMask, class TLabelImage, class TOutputMask>
        void
        StereoCorrectionImageFilter<TInputImage, TInputMask, TLabelImage, TOutputMask>::UpdateWithCaching()
        {

            // TODO : important optimisation . ajouter parametres : si NON Venus (m_CloudMaskingKnownCloudsAltitude == false), alors faire directement l'image  m_DefaultAltitude
            // TODO : du coup parametre sksigma_altitude non utilise pour les capteurs autres que VENUS ??

            // Store the list of cloud labels for which all the cloud altitude values are null
            std::vector<unsigned long> l_LabelVector;
            std::vector<unsigned long> l_OKLabelVector;

            // get pointers to the input and output images
            InputImageConstPointer l_IPinCLAPtr = const_cast<InputImageType *>(this->GetIPCLAInput());
            InputMaskConstPointer l_IPCLDAllPtr = const_cast<InputMaskType *>(this->GetIPCLDAllInput());

            // All clouds mask labels
            m_LabelFilter->SetInput(l_IPCLDAllPtr);
            m_LabelFilter->SetBackgroundValue(0);

            // Compute label statistics
            m_LabelStatFilter->SetInput(l_IPinCLAPtr);
            m_LabelStatFilter->SetLabelInput(m_LabelFilter->GetOutput());
            // No Persitente Filter
            m_LabelStatFilter->Update();

            //Get the number of labels
            const unsigned long l_NumberOfObjects(m_LabelStatFilter->GetNumberOfObjects());
            vnsLogDebugMacro("Number of input objects = " << l_NumberOfObjects);

            // Find pixels within label image for which cloud altitude is not null
            m_ChangeValueFilter->SetInput1(m_LabelFilter->GetOutput());
            m_ChangeValueFilter->SetInput2(l_IPinCLAPtr);
            m_ChangeValueFilter->GetFunctor().SetBackgroundValue(static_cast<InputImagePixelType>(m_ExcludedValue));
            m_ChangeValueFilter->GetFunctor().SetReplaceValue(static_cast<LabelImagePixelType>(m_DefaultValue));

            m_IPCLDAllLabelOutput = m_ChangeValueFilter->GetOutput();

            // Compute label statistics after changing value
            m_ChangeLabelStatFilter->SetInput(l_IPinCLAPtr);
            m_ChangeLabelStatFilter->SetLabelInput(m_IPCLDAllLabelOutput);
            // No Persitente Filter
            m_ChangeLabelStatFilter->Update();

            const unsigned long l_NumberOfLabels(m_ChangeLabelStatFilter->GetNumberOfLabels());
            const unsigned long l_NumberOfObjects2(m_ChangeLabelStatFilter->GetNumberOfObjects());

            vnsLogDebugMacro("Number of labels with altitude not null = " << l_NumberOfLabels);
            vnsLogDebugMacro("Number of objects with altitude not null = " << l_NumberOfObjects2);

            // Start at index 1, 0 is the background
            for (unsigned int l_Label = 1; l_Label < l_NumberOfObjects; l_Label++)
            {
                if (m_ChangeLabelStatFilter->HasLabel(l_Label) == true)
                {
                    l_OKLabelVector.push_back(l_Label);
                }
                else
                {
                    l_LabelVector.push_back(l_Label);
                }
            }

            vnsLogDebugMacro("l_OKLabelVector size = " << l_OKLabelVector.size());
            vnsLogDebugMacro("l_LabelVector size = " << l_LabelVector.size());

            const unsigned int l_OKLabelVectorSize(l_OKLabelVector.size());

            // If clouds have been detected by stereoscopy
            if (l_OKLabelVectorSize >= 1)
            {
                // Construct a list of mean and sigma per label
                std::vector<double> l_Means;
                std::vector<double> l_Sigmas;

                for (unsigned int l = 0; l < l_OKLabelVectorSize; l++)
                {
                    l_Means.push_back(m_ChangeLabelStatFilter->GetMean(l_OKLabelVector.at(l)));
                    l_Sigmas.push_back(m_ChangeLabelStatFilter->GetSigma(l_OKLabelVector.at(l)));
                }

                m_StereoAltitudeFilter->SetIPCLAInput(l_IPinCLAPtr);
                m_StereoAltitudeFilter->SetIPCLDAllLabelInput(m_LabelFilter->GetOutput());
                m_StereoAltitudeFilter->SetKsigmaAltitude(m_KsigmaAltitude);
                m_StereoAltitudeFilter->SetLabelsMean(l_Means);
                m_StereoAltitudeFilter->SetLabelsSigma(l_Sigmas);
                m_StereoAltitudeFilter->SetLabels(l_OKLabelVector);
                m_StereoAltitudeFilter->Update();

                // Get the average of the cloud altitude for all the labels
                const double l_IPinCLAMean = static_cast<double>(m_StereoAltitudeFilter->GetLabelsAltitudeMean());
                vnsLogDebugMacro("Cloud altitude mean = " << l_IPinCLAMean);

                // Get the number of pixels that have a label null in the mask

                const unsigned long l_LabelVectorSize(l_LabelVector.size());

                vnsLogDebugMacro("Number of label discarded <=> altitude null = " << l_LabelVectorSize);

                if (l_LabelVectorSize > 1)
                {
                    vnsLogDebugMacro("Average altitude within the image = " << l_IPinCLAMean);

                    OutputImagePointer l_IPCLAOutput = m_StereoAltitudeFilter->GetIPCLAOutput();
                    // Loop over labels for which cloud altitude is null
                    for (unsigned long i = 0; i < l_LabelVectorSize; i++)
                    {

                        // Find pixels within label image for which cloud altitude is not null
                        CLAChangeValueFilterPointer l_CLAChangeValueFilter = CLAChangeValueFilterType::New();
                        if (i == 0)
                        {
                            l_CLAChangeValueFilter->SetInput1(m_StereoAltitudeFilter->GetIPCLAOutput());
                        }
                        else
                        {
                            l_CLAChangeValueFilter->SetInput1(m_CLAChangeValueFilter.at(i - 1)->GetOutput());
                        }
                        LabelImagePixelType l_Label = static_cast<LabelImagePixelType>(l_LabelVector[i]);
                        l_CLAChangeValueFilter->SetInput2(m_LabelFilter->GetOutput());
                        l_CLAChangeValueFilter->GetFunctor().SetBackgroundValue(static_cast<LabelImagePixelType>(l_Label));
                        l_CLAChangeValueFilter->GetFunctor().SetReplaceValue(static_cast<OutputImagePixelType>(l_IPinCLAMean));

                        m_CLAChangeValueFilter.push_back(l_CLAChangeValueFilter);

                    } // end label loop

                    // Link output
                    m_IPCLAOutput = m_CLAChangeValueFilter.at(l_LabelVectorSize - 1)->GetOutput();

                } // end if l_LabelVectorSize > 1
                else
                {
                    // Link output
                	m_IPCLAOutput =  m_StereoAltitudeFilter->GetIPCLAOutput();
                }

            } // end if l_NumberOfLabels > 1

            // If no cloud has been detected by stereoscopy
            else
            {
                // Set the altitude of the pixels not cloudy (the background) to zero
                // and the other ones to the m_DefaultAltitude value
                m_Threshold->SetInput(m_LabelFilter->GetOutput());
                m_Threshold->SetInsideValue(static_cast<OutputImagePixelType>(0));
                m_Threshold->SetOutsideValue(static_cast<OutputImagePixelType>(m_DefaultAltitude));
                m_Threshold->SetUpperThreshold(static_cast<LabelImagePixelType>(0));
                m_Threshold->SetLowerThreshold(itk::NumericTraits<LabelImagePixelType>::NonpositiveMin());

                // Link output
                m_IPCLAOutput = m_Threshold->GetOutput();

            } // end If no cloud has been detected by stereoscopy

        }

    /** PrintSelf method */
    template<class TInputImage, class TInputMask, class TLabelImage, class TOutputImage>
        void
        StereoCorrectionImageFilter<TInputImage, TInputMask, TLabelImage, TOutputImage>::PrintSelf(std::ostream& os,
                itk::Indent indent) const
        {
            Superclass::PrintSelf(os, indent);
            os << "m_ExcludedValue:   " << m_ExcludedValue << std::endl;
            os << "m_DefaultValue:   " << m_DefaultValue << std::endl;
            os << "m_DefaultAltitude:   " << m_DefaultAltitude << std::endl;
            os << "m_KsigmaAltitude:   " << m_KsigmaAltitude << std::endl;
        }

} // End namespace vns

#endif /* __vnsStereoCorrectionImageFilter_txx */

