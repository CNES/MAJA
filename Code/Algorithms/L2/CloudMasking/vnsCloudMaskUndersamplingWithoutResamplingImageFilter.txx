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
 * VERSION : 1.0.0 : FA : LAIG-FA-MAC-1988-CNES : 21 octobre 2016 : Correction qualite (code mort)          *
 * VERSION : 5-0-0 : FA : LAIG-FA-MAC-1619-CNES : 21 avril 2016 : Creation pour corriger l'anomalie         *
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id: vnsCloudMaskUndersamplingWithoutResamplingImageFilter.txx 11465 2016-01-28 15:51:04Z tfeuvrie $
 *                                                                                                          *
 ************************************************************************************************************/
#ifndef __vnsCloudMaskUndersamplingWithoutResamplingImageFilter_txx
#define __vnsCloudMaskUndersamplingWithoutResamplingImageFilter_txx

#include "vnsCloudMaskUndersamplingWithoutResamplingImageFilter.h"
#include "vnsMath.h"

#include "itkImageRegionIterator.h"
#include "itkImageRegionConstIterator.h"

namespace vns
{

    /** Constructor */
    template<class TInputImage, class TOutputImage, class TOutputMask>
        CloudMaskUndersamplingWithoutResamplingImageFilter<TInputImage, TOutputImage, TOutputMask>::CloudMaskUndersamplingWithoutResamplingImageFilter()
        {

            // Input/Output initialization
            this->SetNumberOfIndexedInputs(1);
            this->SetNumberOfRequiredInputs(1);
            this->SetNumberOfIndexedOutputs(2);

            this->SetNthOutput(0, OutputImageType::New());
            this->SetNthOutput(1, OutputMaskType::New());

            m_NoData = static_cast<RealNoDataType>(0);

        }

    /** Destructor */
    template<class TInputImage, class TOutputImage, class TOutputMask>
        CloudMaskUndersamplingWithoutResamplingImageFilter<TInputImage, TOutputImage, TOutputMask>::~CloudMaskUndersamplingWithoutResamplingImageFilter()
        {
        }

    //****************************** ThreadedGenerateData ********************************/
    /** ThreadedGenerateData method */
    template<class TInputImage, class TOutputImage, class TOutputMask>
        void
        CloudMaskUndersamplingWithoutResamplingImageFilter<TInputImage, TOutputImage, TOutputMask>::ThreadedGenerateData(
                const OutputImageRegionType& outputRegionForThread, itk::ThreadIdType /*threadId*/)
        {
            // get pointers to input images
            typename Superclass::InputImagePointer l_L1CLAPtr = this->GetL1CLAInput();

            // get pointers to output images
            OutputImagePointer l_IPCLAPtr = this->GetIPCLAOutput();
            OutputMaskPointer l_IPCLDAltPtr = this->GetIPCLDAltOutput();

            // Declaration of input image iterator
            itk::ImageRegionConstIterator<InputImageType> l_L1CLAIt(l_L1CLAPtr, outputRegionForThread);
            // Declaration of output image iterators
            itk::ImageRegionIterator<OutputImageType> l_IPCLAIt(l_IPCLAPtr, outputRegionForThread);
            itk::ImageRegionIterator<OutputMaskType> l_IPCLDAltIt(l_IPCLDAltPtr, outputRegionForThread);

            l_L1CLAIt.GoToBegin();
            l_IPCLAIt.GoToBegin();
            l_IPCLDAltIt.GoToBegin();

            while (l_L1CLAIt.IsAtEnd() == false)
            {

                l_IPCLAIt.Set(itk::NumericTraits<OutputImagePixelType>::Zero);
                l_IPCLDAltIt.Set(itk::NumericTraits<OutputMaskPixelType>::Zero);

                OutputImagePixelType l_L1CLAValue = l_L1CLAIt.Get();

                if ( vnsDifferentDoubleMacro(l_L1CLAValue,m_NoData) == true)
                {
                    l_IPCLAIt.Set(static_cast<OutputImagePixelType>(l_L1CLAValue));
                    l_IPCLDAltIt.Set(static_cast<OutputMaskPixelType>(1));

                }

                ++l_L1CLAIt;
                ++l_IPCLAIt;
                ++l_IPCLDAltIt;

            } // end loop over input iterator
        }

    /** PrintSelf method */
    template<class TInputImage, class TOutputImage, class TOutputMask>
        void
        CloudMaskUndersamplingWithoutResamplingImageFilter<TInputImage, TOutputImage, TOutputMask>::PrintSelf(std::ostream& os,
                itk::Indent indent) const
        {
            Superclass::PrintSelf(os, indent);
            os << "m_NoData:               " << m_NoData << std::endl;
        }

} // End namespace vns

#endif /* __vnsCloudMaskUndersamplingWithoutResamplingImageFilter_txx */

