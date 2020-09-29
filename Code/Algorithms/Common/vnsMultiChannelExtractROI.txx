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
 * VERSION : 1.0.0 : FA : LAIG-FA-MAC-1988-CNES : 21 octobre 2016 : Correction qualite (code mort)          *
 * VERSION : 4-0-0 : FA : LAIG-FA-MAC-120653-CS : 3 avril 2014 : Correction de règles de qualité			*
 * VERSION : 1-0-0 : <TypeFT> : <NumFT> : 29 novembre 2010 : Creation
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id$
 *                                                                                                          *
 ************************************************************************************************************/

#ifndef __vnsMultiChannelExtractROI_txx
#define __vnsMultiChannelExtractROI_txx

#include "otbMultiChannelExtractROI.h"

#include "itkImageRegionIterator.h"
#include "itkImageRegionConstIterator.h"
#include "itkObjectFactory.h"
#include "itkExtractImageFilterRegionCopier.h"

namespace vns
{

    /**
     *
     */
    template<class TInputPixelType, class TOutputPixelType>
        MultiChannelExtractROI<TInputPixelType, TOutputPixelType>::MultiChannelExtractROI() :
                otb::ExtractROIBase<otb::VectorImage<TInputPixelType, 2>, otb::VectorImage<TOutputPixelType, 2> >(), m_FirstChannel(0), m_LastChannel(
                        0), m_ChannelsKind(0)
        {
            ClearChannels();
        }

    /**
     *
     */
    template<class TInputPixelType, class TOutputPixelType>
        void
        MultiChannelExtractROI<TInputPixelType, TOutputPixelType>::SetChannel(unsigned int channel)
        {
            if (m_ChannelsKind == 1)
            {
                itkExceptionMacro(<< "m_Channels already set using channels interval.");
            }
            // Add the channel to the list of channels
            m_Channels.push_back(channel);
            if (m_ChannelsKind == 0)
            {
                m_ChannelsKind = 2;
            }
            this->Modified();
        }

    template<class TInputPixelType, class TOutputPixelType>
        void
        MultiChannelExtractROI<TInputPixelType, TOutputPixelType>::SetFirstChannel(unsigned int id)
        {
            if (m_ChannelsKind == 2)
            {
                itkExceptionMacro(<< "m_Channels already set using SetChannels method.");
            }
            m_FirstChannel = id;
            if (m_ChannelsKind == 0)
            {
                m_ChannelsKind = 1;
            }
            this->Modified();
        }

    template<class TInputPixelType, class TOutputPixelType>
        void
        MultiChannelExtractROI<TInputPixelType, TOutputPixelType>::SetLastChannel(unsigned int id)
        {
            if (m_ChannelsKind == 2)
            {
                itkExceptionMacro(<< "m_Channels already set using SetChannels method.");
            }
            m_LastChannel = id;
            if (m_ChannelsKind == 0)
            {
                m_ChannelsKind = 1;
            }
            this->Modified();
        }

    /**
     *
     */
    template<class TInputPixelType, class TOutputPixelType>
        void
        MultiChannelExtractROI<TInputPixelType, TOutputPixelType>::ClearChannels(void)
        {
            // Clean all members
            m_FirstChannel = 0;
            m_LastChannel = 0;
            m_Channels.clear();
            m_ChannelsKind = 0;
            m_ChannelsWorks.clear();
        }

    /**
     *
     */
    template<class TInputPixelType, class TOutputPixelType>
        void
        MultiChannelExtractROI<TInputPixelType, TOutputPixelType>::PrintSelf(std::ostream& os, itk::Indent indent) const
        {
            Superclass::PrintSelf(os, indent);
        }

    template<class TInputPixelType, class TOutputPixelType>
        void
        MultiChannelExtractROI<TInputPixelType, TOutputPixelType>::ChannelsReInitialization()
        {
            // The following conditions can be gathered but we'd loose in comprehension
            m_ChannelsWorks.clear();
            // First passage in the method:
            if (m_Channels.empty() == true)
            {
                // - User SetFirst/LastChannel()
                if (m_ChannelsKind == 1)
                {
                    this->SetChannelsWorkWithLimits();
                }
                else
                {
                    // - User called SetChannels()
                    if (m_Channels.empty() == true && m_ChannelsKind == 2)
                    {
                        m_ChannelsWorks = m_Channels;
                    }
                }
            }
            // Second passage in the method: Update already donne
            else
            {
                // - User SetFirst/LastChannel()
                if (m_ChannelsKind == 1)
                {
                    m_Channels.clear();
                    this->SetChannelsWorkWithLimits();
                }
                else
                {
                    // - User called SetChannels()
                    if (m_ChannelsKind == 2)
                    {
                        m_ChannelsWorks = m_Channels;
                    }
                }
            }
        }

    template<class TInputPixelType, class TOutputPixelType>
        void
        MultiChannelExtractROI<TInputPixelType, TOutputPixelType>::SetChannelsWorkWithLimits()
        {
            // Checks the validity of the channels
            if ((m_FirstChannel == 0) || (m_LastChannel == 0))
            {
                itkExceptionMacro(
                        << "otb::ExtractImageFilter::GenerateOutputInformation " << "Channels must reside into [1...] " << typeid(itk::ImageBase<InputImageDimension>*).name());
            }
            // Checks that the first channel is under the second channel
            if (m_FirstChannel > m_LastChannel)
            {
                itkExceptionMacro(
                        << "otb::ExtractImageFilter::GenerateOutputInformation " << "FirstChannel is greater than LastChannel" << typeid(itk::ImageBase<InputImageDimension>*).name());
            }

            // Loops between the first and the second channel
            for (unsigned int channel = m_FirstChannel; channel <= m_LastChannel; channel++)
            {
                // add the channel to the list of works channels
                m_ChannelsWorks.push_back(channel);
            }

            // Update the channel member
            m_Channels = m_ChannelsWorks;
        }

    /**
     * ExtractImageFilter can produce an image which is a different resolution
     * than its input image.  As such, ExtractImageFilter needs to provide an
     * implementation for GenerateOutputInformation() in order to inform
     * the pipeline execution model.  The original documentation of this
     * method is below.
     *
     * \sa ProcessObject::GenerateOutputInformaton()
     */
    template<class TInputPixelType, class TOutputPixelType>
        void
        MultiChannelExtractROI<TInputPixelType, TOutputPixelType>::GenerateOutputInformation()
        {
            // Call to the superclass implementation
            Superclass::GenerateOutputInformation();
            this->ChannelsReInitialization();

            typename Superclass::InputImageConstPointer inputPtr = this->GetInput();
            typename Superclass::OutputImagePointer outputPtr = this->GetOutput();

            unsigned int nbComponentsPerPixel = inputPtr->GetNumberOfComponentsPerPixel();
            // Used to know if the user has set m_First/LastChannel (1) or m_Channels (3)
            if (m_ChannelsKind != 0)
            {
                // Test if the asked channels index exists in the input image
                ChannelsType m_BadChannels;
                m_BadChannels.clear();
                for (unsigned int i = 0; i < m_ChannelsWorks.size(); ++i)
                {
                    if ((m_ChannelsWorks[i] < 1) || (m_ChannelsWorks[i] > nbComponentsPerPixel))
                    {
                        bool isInsideBadChannels = false;
                        for (unsigned int j = 0; j < m_BadChannels.size(); ++j)
                        {
                            if (m_BadChannels[j] == m_ChannelsWorks[i])
                            {
                                isInsideBadChannels = true;
                            }
                        }
                        if (!isInsideBadChannels)
                        {
                            m_BadChannels.push_back(m_ChannelsWorks[i]);
                        }
                    }
                }
                // Checks the validity of the bad channels string
                if (m_BadChannels.empty() == false)
                {
                    std::ostringstream oss;
                    oss << "otb::ExtractImageFilter::GenerateOutputInformation : ";
                    oss << "Channel(s) [ ";
                    for (unsigned int i = 0; i < m_BadChannels.size(); ++i)
                    {
                        oss << m_BadChannels[i] << " ";
                    }
                    oss << "] not authorized.";
                    oss << " Each channel index has to be in [1," << nbComponentsPerPixel << "].";
                    itkExceptionMacro(<< oss.str().c_str());
                }
                nbComponentsPerPixel = m_ChannelsWorks.size();
            }

            // initialize the number of channels of the output image
            outputPtr->SetNumberOfComponentsPerPixel(nbComponentsPerPixel);
        }

    template<class TInputPixelType, class TOutputPixelType>
        void
        MultiChannelExtractROI<TInputPixelType, TOutputPixelType>::ThreadedGenerateData(const OutputImageRegionType& outputRegionForThread,
                itk::ThreadIdType /*threadId*/)
        {
            itkDebugMacro(<< "Actually executing");
            // Get the input and output pointers
            typename Superclass::InputImageConstPointer inputPtr = this->GetInput();
            typename Superclass::OutputImagePointer outputPtr = this->GetOutput();

            // support progress methods/callbacks

            // Define the portion of the input to walk for this thread
            InputImageRegionType inputRegionForThread;
            this->CallCopyOutputRegionToInputRegion(inputRegionForThread, outputRegionForThread);

            // Define the iterators.
            typedef itk::ImageRegionIterator<OutputImageType> OutputIterator;
            typedef itk::ImageRegionConstIterator<InputImageType> InputIterator;

            OutputIterator outIt(outputPtr, outputRegionForThread);
            InputIterator inIt(inputPtr, inputRegionForThread);

            outIt.GoToBegin();
            inIt.GoToBegin();

            // if default behaviour
            // if the user has set m_Channels (0)
            if (m_ChannelsKind == 0)
            {
                // walk the output region, and sample the input image
                while (!outIt.IsAtEnd())
                {
                    outIt.Set(inIt.Get());
                    ++outIt;
                    ++inIt;
                }
            }
            // Specific behaviour
            // if the user has set m_First/LastChannel (1)
            else
            {
                // Number of channels actually processed
                const unsigned int NbChannelsWorks(m_ChannelsWorks.size());
                ChannelsType l_ChannelsWorksMinusOne;

                // For each channel
                for (unsigned int nbChannels = 0; nbChannels < NbChannelsWorks; ++nbChannels)
                {
                    // Get the number of the channel starting with the index 0
                    l_ChannelsWorksMinusOne.push_back(m_ChannelsWorks[nbChannels] - 1);
                }

                // For each pixel
                while (!outIt.IsAtEnd())
                {
                    OutputImagePixelType pixelOutput;
                    pixelOutput.Reserve(outputPtr->GetVectorLength());
                    const InputImagePixelType & pixelInput(inIt.Get());
                    unsigned int channelOut = 0;
                    // Extract the channels selected by the user
                    for (unsigned int nbChannels = 0; nbChannels < NbChannelsWorks; ++nbChannels)
                    {
                        pixelOutput[channelOut] = static_cast<OutputValueType>(pixelInput[l_ChannelsWorksMinusOne[nbChannels]]);
                        ++channelOut;
                    }
                    outIt.Set(pixelOutput);
                    ++outIt;
                    ++inIt;
                }
            }
        }

} // end namespace otb

#endif
