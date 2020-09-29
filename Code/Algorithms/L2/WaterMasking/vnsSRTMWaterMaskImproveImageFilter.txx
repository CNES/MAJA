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
 * VERSION : 5-1-0 : DM : LAIG-DM-MAC-1514-CNES : 12 juillet 2016 : Amélioration algo ( SRTM loin des côtes)*
 * VERSION : 1-0-0 : <TypeFT> : <NumFT> : 22 juil. 2010 : Creation                                                           
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id$
 *                                                                                                          *
 ************************************************************************************************************/
#ifndef __vnsSRTMWaterMaskImproveImageFilter_txx
#define __vnsSRTMWaterMaskImproveImageFilter_txx

#include "vnsSRTMWaterMaskImproveImageFilter.h"

#include "itkImageRegionConstIterator.h"
#include "itkImageRegionIterator.h"

#include "vnsMath.h"
#include "vnsMacro.h"

namespace vns
{

    // Constructor
    template<class TInputImage, class TVectorMask, class TSRTMMask, class TOutputImage>
        SRTMWaterMaskImproveImageFilter<TInputImage, TVectorMask, TSRTMMask, TOutputImage>::SRTMWaterMaskImproveImageFilter() :
                m_InitMode(false), m_SRTMPercent(0.), m_WaterMaskNumber(0), m_MinPercentPossibleWater(0.)
        {

            // Input/Output initialization
            this->SetNumberOfIndexedInputs(5);
            this->SetNumberOfRequiredInputs(3);
            this->SetNumberOfIndexedOutputs(3);

            this->SetNthOutput(0, OutputImageType::New());
            this->SetNthOutput(1, VectorMaskType::New());
            this->SetNthOutput(2, VectorMaskType::New());

        }

    // Destructor
    template<class TInputImage, class TVectorMask, class TSRTMMask, class TOutputImage>
        SRTMWaterMaskImproveImageFilter<TInputImage, TVectorMask, TSRTMMask, TOutputImage>::~SRTMWaterMaskImproveImageFilter()
        {
        }

    //===============  Generate output information method ===================/
    template<class TInputMask, class TVectorMask, class TSRTMMask, class TOutputImage>
        void
        SRTMWaterMaskImproveImageFilter<TInputMask, TVectorMask, TSRTMMask, TOutputImage>::GenerateOutputInformation()
        {
            // Call superclass implementation
            Superclass::GenerateOutputInformation();

            // If Init mode, no possible water mask and tested pixel flag
            // has been generated before
            if (m_InitMode == true)
            {
                this->GetL2PWAOutput()->SetNumberOfComponentsPerPixel(1);
                this->GetL2TWAOutput()->SetNumberOfComponentsPerPixel(1);
            }
            else
            {

                unsigned int l_NumberOfComponents(0);

                // Check L2inPWA image availability
                if (!this->GetL2inPWAInput())
                {
                    vnsExceptionBusinessMacro(
                            "Waiting for a possible water mask of previous product, but no one specified. Please set one.");
                }

                // Get the number of possible water mask previously generated (input masks)
                l_NumberOfComponents = this->GetL2inPWAInput()->GetNumberOfComponentsPerPixel();

                // The number of output possible water mask depends on the
                // number of previous possible water mask generated since
                // init mode (D-1 to D-N_WM).
                // Then, the mask of date D is added to the previous ones.
                if (l_NumberOfComponents < m_WaterMaskNumber)
                {
                    this->GetL2PWAOutput()->SetNumberOfComponentsPerPixel(l_NumberOfComponents + 1);
                    this->GetL2TWAOutput()->SetNumberOfComponentsPerPixel(l_NumberOfComponents + 1);
                }
                // In this case, the older mask (D-N_WM) is replaced by the new one (date D)
                else
                {
                    this->GetL2PWAOutput()->SetNumberOfComponentsPerPixel(m_WaterMaskNumber);
                    this->GetL2TWAOutput()->SetNumberOfComponentsPerPixel(m_WaterMaskNumber);
                }
            }

        }

    //=============== BeforeThreaded generate data method ================/
    template<class TInputMask, class TVectorMask, class TSRTMMask, class TOutputImage>
        void
        SRTMWaterMaskImproveImageFilter<TInputMask, TVectorMask, TSRTMMask, TOutputImage>::BeforeThreadedGenerateData()
        {
            // Call superclass implementation
            Superclass::BeforeThreadedGenerateData();

            // Check IPPWA image availability
            if (!this->GetIPPWAInput())
            {
                vnsExceptionBusinessMacro("Waiting for a possible water mask for date D, but no one specified. Please set one.");
            }
            // Check IPTWA mask availability
            if (!this->GetIPTWAInput())
            {
                vnsExceptionBusinessMacro("Waiting for a tested pixel mask for date D, but no one specified. Please set one.");
            }

            if (m_InitMode == false)
            {
                // Check L2inPWA image availability
                if (!this->GetL2inPWAInput())
                {
                    vnsExceptionBusinessMacro(
                            "Waiting for a possible water mask of previous product, but no one specified. Please set one.");
                }
                // Check L2inTWA mask availability
                if (!this->GetL2inTWAInput())
                {
                    vnsExceptionBusinessMacro("Waiting for a tested pixel mask of previous product, but no one specified. Please set one.");
                }
            }

            // Conversion of parameters that are in percent
            m_SRTMPercent = m_SRTMPercent / static_cast<double>(100.);                // Initial parameter in percent
            m_MinPercentPossibleWater = m_MinPercentPossibleWater / static_cast<double>(100.);               // Initial parameter in percent

        }

    //====================== Main computation method ======================/
    template<class TInputMask, class TVectorMask, class TSRTMMask, class TOutputImage>
        void
        SRTMWaterMaskImproveImageFilter<TInputMask, TVectorMask, TSRTMMask, TOutputImage>::ThreadedGenerateData(
                const OutputImageRegionType& outputRegionForThread, itk::ThreadIdType /*threadId*/)
        {

            // Number of previous possible water flag (or tested pixel one) generated
            unsigned int l_PrevWaterMaskNumber(0);

            VectorMaskPixelType l_L2outPWAPix;
            VectorMaskPixelType l_L2outTWAPix;

            // Load inputs
            InputMaskConstPointer l_IPPWAPtr = this->GetIPPWAInput();
            InputMaskConstPointer l_IPTWAPtr = this->GetIPTWAInput();
            SRTMMaskConstPointer l_SRTMWATPtr = this->GetSRTMWATInput();

            VectorMaskConstPointer l_L2inPWAPtr;
            VectorMaskConstPointer l_L2inTWAPtr;

            // Load outputs
            OutputImagePointer l_L2WASPtr = this->GetL2WASOutput();
            VectorMaskPointer l_L2outPWAPtr = this->GetL2PWAOutput();
            VectorMaskPointer l_L2outTWAPtr = this->GetL2TWAOutput();

            // Prepare the threaded region input local iterators
            itk::ImageRegionConstIterator<InputMaskType> l_IPPWAIt(l_IPPWAPtr, outputRegionForThread);
            itk::ImageRegionConstIterator<InputMaskType> l_IPTWAIt(l_IPTWAPtr, outputRegionForThread);
            itk::ImageRegionConstIterator<SRTMMaskType> l_SRTMWATIt(l_SRTMWATPtr, outputRegionForThread);

            itk::ImageRegionConstIterator<VectorMaskType> l_L2inPWAIt;
            itk::ImageRegionConstIterator<VectorMaskType> l_L2inTWAIt;

            // Init to the beginning
            l_IPPWAIt.GoToBegin();
            l_IPTWAIt.GoToBegin();
            l_SRTMWATIt.GoToBegin();

            // Prepare the threaded region output local iterator
            itk::ImageRegionIterator<OutputImageType> l_L2WASIt(l_L2WASPtr, outputRegionForThread);
            itk::ImageRegionIterator<VectorMaskType> l_L2outPWAIt(l_L2outPWAPtr, outputRegionForThread);
            itk::ImageRegionIterator<VectorMaskType> l_L2outTWAIt(l_L2outTWAPtr, outputRegionForThread);

            // Init to the beginning
            l_L2WASIt.GoToBegin();
            l_L2outPWAIt.GoToBegin();
            l_L2outTWAIt.GoToBegin();

            // Progress reporter

            // In init mode, the possible water flag and the tested pixel flag
            // of previous images do not exist
            if (m_InitMode == true)
            {
                l_PrevWaterMaskNumber = 0;
            }
            else
            {
                l_L2inPWAPtr = this->GetL2inPWAInput();
                l_L2inTWAPtr = this->GetL2inTWAInput();

                l_L2inPWAIt = itk::ImageRegionConstIterator<VectorMaskType>(l_L2inPWAPtr, outputRegionForThread);
                l_L2inTWAIt = itk::ImageRegionConstIterator<VectorMaskType>(l_L2inTWAPtr, outputRegionForThread);

                l_L2inPWAIt.GoToBegin();
                l_L2inTWAIt.GoToBegin();

                l_PrevWaterMaskNumber = l_L2inPWAPtr->GetNumberOfComponentsPerPixel();

            }

            // Set the number of output masks
            const unsigned int l_WaterMaskNumber(l_L2outPWAPtr->GetNumberOfComponentsPerPixel());
            const unsigned int l_ConstPrevWaterMaskNumber(l_PrevWaterMaskNumber);

            // Allocate the output pixels
            l_L2outPWAPix.Reserve(l_WaterMaskNumber);
            l_L2outTWAPix.Reserve(l_WaterMaskNumber);

            // Loop over the output images
            while (l_IPPWAIt.IsAtEnd() == false)
            {
                // Initialization
                l_L2WASIt.Set(static_cast<OutputImagePixelType>(0));

                // Increase confidence index by a bonus depending on the filtered SRTM ( far from coast = more confident )
                const double l_BonusSRTM = m_SRTMPercent * l_SRTMWATIt.Get();

                // Count the number of times the pixel has been identified as water
                // in the D-N_WM days before

                // Possible water flag and tested pixel flag of date D
                unsigned int l_Sum = l_IPPWAIt.Get();
                unsigned int l_SumTests = l_IPTWAIt.Get();

                // Output possible water and tested pixel masks generation

                // Set the new mask value in the output masks (for date D)
                l_L2outPWAPix[0] = static_cast<VectorMaskInternalPixelType>(l_Sum);
                l_L2outTWAPix[0] = static_cast<VectorMaskInternalPixelType>(l_SumTests);

                if (m_InitMode == false)
                {

                    const VectorMaskPixelType & l_L2inPWAPix = l_L2inPWAIt.Get();
                    const VectorMaskPixelType & l_L2inTWAPix = l_L2inTWAIt.Get();
                    // Copy the previous masks in the output ones
                    for (unsigned int l_date = 1; l_date < l_WaterMaskNumber; l_date++)
                    {
                        l_L2outPWAPix[l_date] = l_L2inPWAPix[l_date - 1];
                        l_L2outTWAPix[l_date] = l_L2inTWAPix[l_date - 1];
                    }

                    // Final water mask generation (for date D)

                    // Previous date loop (0, 1 to N dates, 1 to N_WM dates)
                    for (unsigned int l_date = 0; l_date < l_ConstPrevWaterMaskNumber; l_date++)
                    {
                        l_Sum += l_L2inPWAPix[l_date];
                        l_SumTests += l_L2inTWAPix[l_date];
                    }
                }

                l_L2outPWAIt.Set(l_L2outPWAPix);
                l_L2outTWAIt.Set(l_L2outTWAPix);

                // Compute the ratio between the number of times the pixel
                // has been identified as water and the number of tested pixel
                if (static_cast<double>(l_SumTests) > CONST_EPSILON)
                {
                    // Add the SRTM bonus to the ratio
                    const double l_Ratio = static_cast<double>(l_Sum) / static_cast<double>(l_SumTests) + l_BonusSRTM;
                    // To be qualified as water, a pixel must be qualified as possible
                    // water at least m_MinPercentPossibleWater percent of tested times
                    if (l_Ratio > m_MinPercentPossibleWater)
                    {
                        l_L2WASIt.Set(static_cast<OutputImagePixelType>(1));
                    }
                }
                else if (vnsIsZeroDoubleMacro(static_cast<double>(l_SumTests)))
                {
                    // Equal 0 is really working
                    if (l_BonusSRTM > m_MinPercentPossibleWater)
                    {
                        l_L2WASIt.Set(static_cast<OutputImagePixelType>(1));
                    }
                }

                ++l_IPPWAIt;
                ++l_IPTWAIt;
                ++l_SRTMWATIt;
                ++l_L2WASIt;
                ++l_L2outPWAIt;
                ++l_L2outTWAIt;

                if (m_InitMode == false)
                {
                    ++l_L2inPWAIt;
                    ++l_L2inTWAIt;
                }

            } // end loop pixel

        }

    // PrintSelf method
    template<class TInputMask, class TVectorMask, class TSRTMMask, class TOutputImage>
        void
        SRTMWaterMaskImproveImageFilter<TInputMask, TVectorMask, TSRTMMask, TOutputImage>::PrintSelf(std::ostream& os,
                itk::Indent indent) const
        {
            Superclass::PrintSelf(os, indent);
            os << "m_InitMode :                 " << m_InitMode << std::endl;
            os << "m_SRTMPercent :              " << m_SRTMPercent << std::endl;
            os << "m_WaterMaskNumber :          " << m_WaterMaskNumber << std::endl;
            os << "m_MinPercentPossibleWater :  " << m_MinPercentPossibleWater << std::endl;

        }

} // End namespace vns

#endif /* __vnsSRTMWaterMaskImproveImageFilter_txx */
