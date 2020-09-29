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
 * VERSION : 4-0-0 : FA : LAIG-FA-MAC-117040-CS : 13 mars 2014 : Modifications mineures                     *
 * VERSION : 1-0-0 : <TypeFT> : <NumFT> : 27 août 2010 : Creation                                                           
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id$
 *                                                                                                          *
 ************************************************************************************************************/
#ifndef __vnsHighAOTImageFilter_txx
#define __vnsHighAOTImageFilter_txx

#include "vnsHighAOTImageFilter.h"
#include "vnsMacro.h"
#include "vnsMath.h"

namespace vns
{

    // Constructor
    template<class TInputImage, class TInputMask, class TOutputImage>
        HighAOTImageFilter<TInputImage, TInputMask, TOutputImage>::HighAOTImageFilter():
           m_CoefSigmaAOT(0), m_MinimumHighAOTStd(0.), m_AOTMean(0.), m_AOTStd(0.)
        {
            // Filter initializations
            m_BinaryLogicalOrImageFilter = BinaryLogicalOrFilterType::New();
            m_HighAOTFilter = HighAOTFilterType::New();
        }

    // Destructor
    template<class TInputImage, class TInputMask, class TOutputImage>
        HighAOTImageFilter<TInputImage, TInputMask, TOutputImage>::~HighAOTImageFilter()
        {
        }


    //====================== Main computation method ======================/
    template<class TInputImage, class TInputMask, class TOutputImage>
        void
        HighAOTImageFilter<TInputImage, TInputMask, TOutputImage>::GenerateData()
        {

            InputImagePixelType l_HighAOTThreshold;

            // Get pointers to the input and output images
            InputImageConstPointer l_IPAOTSubPtr = this->GetIPAOTSubInput();
            InputMaskConstPointer l_IPEDGPtr = this->GetIPEDGInput();
            InputMaskConstPointer l_L2CLDSubPtr = this->GetL2CLDSubInput();


            // Logical addition of the two input masks
            m_BinaryLogicalOrImageFilter->SetInput1(l_IPEDGPtr);
            m_BinaryLogicalOrImageFilter->SetInput2(l_L2CLDSubPtr);

            // Compute the high AOT threshold value
            // If the std is enough high to catch high AOT pixels
            if (m_AOTStd*m_CoefSigmaAOT > m_MinimumHighAOTStd)
            {
                l_HighAOTThreshold =
                        static_cast<InputImagePixelType>(m_AOTMean + m_CoefSigmaAOT * m_AOTStd);
            }
            // If the std is too low
            else
            {
                l_HighAOTThreshold =
                        static_cast<InputImagePixelType>(m_AOTMean + m_MinimumHighAOTStd);
            }

            // Determination of high AOT pixels
            m_HighAOTFilter->SetInput1(l_IPAOTSubPtr);
            m_HighAOTFilter->SetInput2(m_BinaryLogicalOrImageFilter->GetOutput());

     //       m_HighAOTFilter->GetFunctor().SetOutputSize(l_IPAOTSubPtr->GetNumberOfComponentsPerPixel());
            m_HighAOTFilter->GetFunctor().SetHighAOTThresholdValue(l_HighAOTThreshold);
            m_HighAOTFilter->GetFunctor().SetInsideValue(1);
            m_HighAOTFilter->GetFunctor().SetOutputValue(0);

            // Set the output mask of high AOT pixels
            m_HighAOTFilter->GraftOutput(this->GetOutput());
            m_HighAOTFilter->Update();
            this->GraftOutput(m_HighAOTFilter->GetOutput());

        }

    // PrintSelf method
    template<class TInputImage, class TInputMask, class TOutputImage>
        void
        HighAOTImageFilter<TInputImage, TInputMask, TOutputImage>::PrintSelf(std::ostream& os,
                itk::Indent indent) const
        {
            Superclass::PrintSelf(os, indent);

            os << "m_CoefSigmaAOT:               "<< m_CoefSigmaAOT <<std::endl;
            os << "m_MinimumHighAOTStd:          "<< m_MinimumHighAOTStd <<std::endl;
            os << "m_AOTMean:                    "<< m_AOTMean <<std::endl;
            os << "m_AOTStd:                     "<< m_AOTStd <<std::endl;
        }

} // End namespace vns

#endif /* __vnsHighAOTImageFilter_txx */
