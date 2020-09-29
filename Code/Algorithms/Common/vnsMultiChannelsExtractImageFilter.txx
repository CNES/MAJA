/*=========================================================================

 Program:   ORFEO Toolbox
 Language:  C++
 Date:      $Date$
 Version:   $Revision$


 Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
 See OTBCopyright.txt for details.


 This software is distributed WITHOUT ANY WARRANTY; without even
 the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 PURPOSE.  See the above copyright notices for more information.

 =========================================================================*/

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
 * VERSION : 1.0.0 : FA : LAIG-FA-MAC-1988-CNES : 21 octobre 2016 : Correction qualite (code mort)          *
 * VERSION : 1-0-0 : <TypeFT> : <NumFT> : 1 avr. 2010 : Creation                                                           
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id: vnsStreamingConditionalStatisticsImageFilter.txx 10384 2015-06-05 16:23:02Z tfeuvrie $
 *                                                                                                          *
 ************************************************************************************************************/

#ifndef __otbMultiChannelsExtractImageFilter_txx
#define __otbMultiChannelsExtractImageFilter_txx

#include "vnsMultiChannelsExtractImageFilter.h"

#include "itkImageRegionIterator.h"
#include "itkImageRegionConstIterator.h"
#include "itkObjectFactory.h"
#include "itkExtractImageFilterRegionCopier.h"

namespace vns
{

    /**
     *
     */
    template<class TInputImage, class TOutputImage>
        MultiChannelsExtractImageFilter<TInputImage, TOutputImage>::MultiChannelsExtractImageFilter() :
                itk::ImageToImageFilter<TInputImage, TOutputImage>()
        {
            ClearChannels();
        }

    /**
     *
     */
    template<class TInputImage, class TOutputImage>
        void
        MultiChannelsExtractImageFilter<TInputImage, TOutputImage>::SetChannel(unsigned int channel)
        {
            m_Channels.push_back(channel);
            this->Modified();
        }

    /**
     *
     */
    template<class TInputImage, class TOutputImage>
        void
        MultiChannelsExtractImageFilter<TInputImage, TOutputImage>::ClearChannels(void)
        {
            m_Channels.clear();
            this->Modified();
        }

    /**
     *
     */
    template<class TInputImage, class TOutputImage>
        void
        MultiChannelsExtractImageFilter<TInputImage, TOutputImage>::PrintSelf(std::ostream& os, itk::Indent indent) const
        {
            Superclass::PrintSelf(os, indent);
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
    template<class TInputImage, class TOutputImage>
        void
        MultiChannelsExtractImageFilter<TInputImage, TOutputImage>::GenerateOutputInformation()
        {
            // Call to the superclass implementation
            Superclass::GenerateOutputInformation();

            typename Superclass::InputImageConstPointer inputPtr = this->GetInput();
            typename Superclass::OutputImagePointer outputPtr = this->GetOutput();

            unsigned int nbComponentsPerPixel = inputPtr->GetNumberOfComponentsPerPixel();

            // Test if the asked channels index exists in the input image
            ChannelsType m_BadChannels;
            m_BadChannels.clear();
            // m_Channels contains the list of the channels (integer) that
            // will be extracted from the input image
            for (unsigned int i = 0; i < m_Channels.size(); ++i)
            {
                if ((m_Channels[i] < 1) || (m_Channels[i] > nbComponentsPerPixel))
                {
                	// test if the bad channel is already in the list
                    bool isInsideBadChannels = false;
                    for (unsigned int j = 0; j < m_BadChannels.size(); ++j)
                    {
                        if (m_BadChannels[j] == m_Channels[i])
                        {
                            isInsideBadChannels = true;
                        }

                    }
                    // Add the channel to the list of bad channel
                    if (!isInsideBadChannels)
                    {
                        m_BadChannels.push_back(m_Channels[i]);
                    }
                }
            }
            // If some channels don't exist in the input image, launch an exception
            if (m_BadChannels.empty() == false)
            {
            	// Display the bad channel
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
            nbComponentsPerPixel = m_Channels.size();

            // initialize the number of channels of the output image
            outputPtr->SetNumberOfComponentsPerPixel(nbComponentsPerPixel);
        }

    template<class TInputImage, class TOutputImage>
        void
        MultiChannelsExtractImageFilter<TInputImage, TOutputImage>::ThreadedGenerateData(const OutputImageRegionType& outputRegionForThread,
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

            const unsigned int lNbChannels = m_Channels.size();
            /** List of channels actually being processed [0...[ */
            ChannelsType l_ChannelsWorks;
            // Deduces - 1
            for (unsigned int nbChannels = 0; nbChannels < lNbChannels; ++nbChannels)
            {
                l_ChannelsWorks.push_back( m_Channels[nbChannels] - 1);
            }

            // Pixel loop
            InputImagePixelType pixelInput;
            while (!outIt.IsAtEnd())
            {
                OutputImagePixelType pixelOutput;
                pixelOutput.Reserve(outputPtr->GetVectorLength());
                pixelInput = inIt.Get();
                // Get the value of the pixel for each extracted band
                for (unsigned int nbChannels = 0; nbChannels < lNbChannels; ++nbChannels)
                {
                    pixelOutput[nbChannels] = static_cast<OutputValueType>(pixelInput[l_ChannelsWorks[nbChannels]]);
                }
                outIt.Set(pixelOutput);
                ++outIt;
                ++inIt;
            }
        }

} // end namespace otb

#endif
