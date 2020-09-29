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
 * VERSION : 4-0-0 : FA : LAIG-FA-MAC-120653-CS : 7 mai 2014 : Correction de règles qualité ou commentaires *
 * VERSION : 1-0-0 : <TypeFT> : <NumFT> : 27 oct. 2010 : Creation                                                           
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id$
 *                                                                                                          *
 ************************************************************************************************************/
#ifndef __vnsStereoAltitudeCorrectionImageFilter_txx
#define __vnsStereoAltitudeCorrectionImageFilter_txx

#include "vnsStereoAltitudeCorrectionImageFilter.h"

#include "itkImageRegionConstIterator.h"
#include "itkImageRegionIterator.h"
#include "vnsLoggers.h"
#include "vnsMath.h"

namespace vns
{

    // Constructor
    template<class TInputImage, class TLabelImage, class TOutputImage>
        StereoAltitudeCorrectionImageFilter<TInputImage, TLabelImage, TOutputImage>::StereoAltitudeCorrectionImageFilter() :
                m_KsigmaAltitude(0), m_LabelsAltitudeMean(0), m_NumberOfLabels(0)
        {

            // Input/Output initialization
            this->SetNumberOfIndexedInputs(2);
            this->SetNumberOfRequiredInputs(2);
            this->SetNumberOfIndexedOutputs(1);

        }

    // Destructor
    template<class TInputImage, class TLabelImage, class TOutputImage>
        StereoAltitudeCorrectionImageFilter<TInputImage, TLabelImage, TOutputImage>::~StereoAltitudeCorrectionImageFilter()
        {
        }

    template<class TInputImage, class TLabelImage, class TOutputImage>
        void
        StereoAltitudeCorrectionImageFilter<TInputImage, TLabelImage, TOutputImage>::BeforeThreadedGenerateData()
        {
            // Calls the BeforeThreadedGenerateData method of the superclass
            Superclass::BeforeThreadedGenerateData();

            // Initialize the number of labels
            m_NumberOfLabels = m_Labels.size();

            //vnsLogSuperDebugMacro("Number of cloud labels "<<m_NumberOfLabels);

            // Initialize the tab of the sum and count altitudes for each threads
            m_LabelsAltitudeSumThreads = std::vector<double>(this->GetNumberOfThreads(), 0.);
            m_LabelsAltitudeCountThreads = std::vector<unsigned long>(this->GetNumberOfThreads(), 0);

            // Checks the size
            if (m_LabelsMean.size() != m_NumberOfLabels || m_LabelsSigma.size() != m_NumberOfLabels)
            {
                vnsExceptionBusinessMacro("Invalid mean or sigma list, size mismatch with label list.");
            }
        }

    //====================== Main computation method ======================/
    template<class TInputImage, class TLabelImage, class TOutputImage>
        void
        StereoAltitudeCorrectionImageFilter<TInputImage, TLabelImage, TOutputImage>::ThreadedGenerateData(
                const OutputImageRegionType& outputRegionForThread, itk::ThreadIdType threadId)
        {

            double l_MaxValue(0.);
            double l_MinValue(0.);

            // get pointers to the input and output images
            InputImageConstPointer l_IPinCLAPtr = const_cast<InputImageType *>(this->GetIPCLAInput());
            LabelImageConstPointer l_IPCLDAllLabelPtr = this->GetIPCLDAllLabelInput();

            OutputImagePointer l_IPCLAPtr = this->GetIPCLAOutput();

            // Define the input image iterators
            itk::ImageRegionConstIterator<InputImageType> l_IPinCLAIt(l_IPinCLAPtr, outputRegionForThread);

            itk::ImageRegionConstIterator<LabelImageType> l_IPCLDAllLabelIt(l_IPCLDAllLabelPtr, outputRegionForThread);

            // Define the output image iterators
            itk::ImageRegionIterator<OutputImageType> l_IPCLAIt(l_IPCLAPtr, outputRegionForThread);

            l_IPinCLAIt.GoToBegin();
            l_IPCLDAllLabelIt.GoToBegin();
            l_IPCLAIt.GoToBegin();

            // current cloud label value
            unsigned int l_LabelId = 0;
            std::vector<unsigned long>::iterator itLab;
            itLab = m_Labels.begin();
            std::vector<unsigned long>::iterator itLabStop;
            itLabStop = m_Labels.end();
            bool l_Found = false;

            // Pixel loop over the label region
            while (l_IPinCLAIt.IsAtEnd() == false)
            {
                const double l_IPinCLAValue = static_cast<double>(l_IPinCLAIt.Get());

                // Init with the input altitude value
                l_IPCLAIt.Set(static_cast<OutputImagePixelType>(l_IPinCLAValue));

                // If no cloud, copy input else
                if (m_NumberOfLabels != 0)
                {

                    // if it is a cloud
                    if (l_IPCLDAllLabelIt.Get() != 0)
                    {
                        // find the label index
                        itLab = m_Labels.begin();
                        l_Found = false;
                        l_LabelId = 0;
                        while ((itLab < itLabStop) && (l_Found == false))
                        {
                            // If the label is found
                            if ((*itLab) == l_IPCLDAllLabelIt.Get())
                            {
                                l_Found = true;
                            }
                            else
                            {
                                l_LabelId = l_LabelId + 1;
                                ++itLab;
                            }
                        }

                        if (l_Found == true)
                        {
                            // If the altitude of the cloud pixel is null,
                            // set the mean altitude of the cloud label
                            if (vnsEqualsDoubleMacro(l_IPinCLAValue,0.) == true)
                            {
                                l_IPCLAIt.Set(static_cast<OutputImagePixelType>(m_LabelsMean.at(l_LabelId)));
                            }
                            // To Discard outliers
                            else
                            {
                                // Compute min and max altitude value taking into account
                                // the standard deviation in the label
                                l_MaxValue = m_LabelsMean.at(l_LabelId) + m_KsigmaAltitude * m_LabelsSigma.at(l_LabelId);
                                l_MinValue = m_LabelsMean.at(l_LabelId) - m_KsigmaAltitude * m_LabelsSigma.at(l_LabelId);

                                // Bring back the value between the min and max altitude
                                if (l_IPinCLAValue > l_MaxValue)
                                {
                                    l_IPCLAIt.Set(static_cast<OutputImagePixelType>(l_MaxValue));
                                }
                                if (l_IPinCLAValue < l_MinValue)
                                {
                                    l_IPCLAIt.Set(static_cast<OutputImagePixelType>(l_MinValue));
                                }
                            } // end if input altitude is null

                        } // end if the label is found

                    } // end if label is not null

                } // end if number of label is not null

                if (l_IPinCLAValue != 0)
                {
                    // Sum the altitude values of all the pixels labeled in the image of labels
                    m_LabelsAltitudeSumThreads.at(threadId) = m_LabelsAltitudeSumThreads.at(threadId) + static_cast<double>(l_IPinCLAValue);
                    m_LabelsAltitudeCountThreads.at(threadId) = m_LabelsAltitudeCountThreads.at(threadId) + 1;
                }

                ++l_IPinCLAIt;
                ++l_IPCLDAllLabelIt;
                ++l_IPCLAIt;

            } // end pixel loop

        }

    // After threaded generate data
    template<class TInputImage, class TLabelImage, class TOutputImage>
        void
        StereoAltitudeCorrectionImageFilter<TInputImage, TLabelImage, TOutputImage>::AfterThreadedGenerateData()
        {
            // Calls the AfterThreadedGenerateData method of the superclass
            Superclass::AfterThreadedGenerateData();

            // Get the number of threads
            const unsigned int nbThreads = this->GetNumberOfThreads();

            double cumulSum(0.);
            double cumulCount(0.);
            // Loop over threads
            // Compute the sum and count for all threads
            for (unsigned int threadId = 0; threadId < nbThreads; threadId++)
            {
                cumulSum = cumulSum + static_cast<double>(m_LabelsAltitudeSumThreads.at(threadId));
                cumulCount = cumulCount + static_cast<double>(m_LabelsAltitudeCountThreads.at(threadId));
            }

            // If points have been computed
            if (cumulCount != 0)
            {
                m_LabelsAltitudeMean = cumulSum / cumulCount;
            }
            // Set the default value for the mean altitude
            else
            {
                m_LabelsAltitudeMean = 0.;
            }
        }

    // PrintSelf method
    template<class TInputImage, class TLabelImage, class TOutputImage>
        void
        StereoAltitudeCorrectionImageFilter<TInputImage, TLabelImage, TOutputImage>::PrintSelf(std::ostream& os, itk::Indent indent) const
        {
            Superclass::PrintSelf(os, indent);
            os << "m_KsigmaAltitude:   " << m_KsigmaAltitude << std::endl;
            os << "m_NumberOfLabels:   " << m_NumberOfLabels << std::endl;
            os << "m_LabelsMean:   ";
            for (unsigned int k = 0; k < m_NumberOfLabels; k++)
            {
                os << m_LabelsMean.at(k) << "  ";
            }
            os << std::endl;
            os << "m_LabelsSigma:   ";
            for (unsigned int k = 0; k < m_NumberOfLabels; k++)
            {
                os << m_LabelsSigma.at(k) << "  ";
            }
            os << std::endl;
            os << "m_LabelsAltitudeMean :   " << m_LabelsAltitudeMean << std::endl;

        }

} // End namespace vns

#endif /* __vnsStereoAltitudeCorrectionImageFilter_txx */
