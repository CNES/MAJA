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
 * VERSION : 4-4-1 : FA : LAIG-FA-MAC-127944-CS : 9 juillet 2015 : Correction warning et contrôle qualité   *
 * 														sur le code source C++   							*
 * VERSION : 4-0-0 : FA : LAIG-FA-MAC-120653-CS : 3 avril 2014 : Correction de règles de qualité			*
 * VERSION : 1-0-0 : <TypeFT> : <NumFT> : 26 août 2010 : Creation                                                           
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id$
 *                                                                                                          *
 ************************************************************************************************************/
#ifndef __vnsComputeTransmissionImageFilter_txx
#define __vnsComputeTransmissionImageFilter_txx

#include "vnsComputeTransmissionImageFilter.h"

namespace vns
{

//
//******************* Constructor *******************
//
    template<class TInputImage, class TInputMask, class TInputDTM, class TInputLUT, class TOutputVectorImage>
        ComputeTransmissionImageFilter<TInputImage, TInputMask, TInputDTM, TInputLUT, TOutputVectorImage>::ComputeTransmissionImageFilter()
        {

            this->SetNumberOfRequiredInputs(3);
            this->SetNumberOfIndexedInputs(3);
            this->SetNumberOfIndexedOutputs(2);

            this->SetNthOutput(0, OutputVectorImageType::New());
            this->SetNthOutput(1, OutputVectorImageType::New());

            // Resolution parameters declaration
            m_ThetaS = 0;
            m_NbOfBands = 0;

        }

//
//******************* Destructor *******************
//
    template<class TInputImage, class TInputMask, class TInputDTM, class TInputLUT, class TOutputVectorImage>
        ComputeTransmissionImageFilter<TInputImage, TInputMask, TInputDTM, TInputLUT, TOutputVectorImage>::~ComputeTransmissionImageFilter()
        {
        }

//
//******************* ThreadedGenerateData function *******************
//
    template<class TInputImage, class TInputMask, class TInputDTM, class TInputLUT, class TOutputVectorImage>
        void
        ComputeTransmissionImageFilter<TInputImage, TInputMask, TInputDTM, TInputLUT, TOutputVectorImage>::BeforeThreadedGenerateData()
        {
    		// Get input images
            InputImageConstPointerType lIPAOTSPtr = this->GetIPAOTSub();
            InputDTMConstPointerType lDTMsubPtr = this->GetDTMSub();
            OutputVectorImagePointerType lTdirPtr = this->GetTdir();
            OutputVectorImagePointerType lTdifPtr = this->GetTdif();

            // Check if all the input have been initialized
            if ((lIPAOTSPtr.IsNull() == true) || (lDTMsubPtr.IsNull() == true) || (m_LUT_Tdir.IsNull() == true)
                    || (m_LUT_Tdif.IsNull() == true))
            {
                itkExceptionMacro(<< "At least one input is missing.");
            }

            // Get the size of all the input images
            SizeType lSize1 = lIPAOTSPtr->GetLargestPossibleRegion().GetSize();
            SizeType lSize2 = lDTMsubPtr->GetLargestPossibleRegion().GetSize();
            SizeType lSize3 = lTdirPtr->GetLargestPossibleRegion().GetSize();
            SizeType lSize4 = lTdifPtr->GetLargestPossibleRegion().GetSize();

            // Check the input sizes
            if ((lSize1 != lSize2) || (lSize1 != lSize3) || (lSize1 != lSize4))
            {
                itkExceptionMacro(<< "At least one size is not correct.");
            }

        }

//
//******************* ThreadedGenerateData function *******************
//
    template<class TInputImage, class TInputMask, class TInputDTM, class TInputLUT, class TOutputVectorImage>
        void
        ComputeTransmissionImageFilter<TInputImage, TInputMask, TInputDTM, TInputLUT, TOutputVectorImage>::ThreadedGenerateData(
                const InputImageRegionType& outputRegionForThread, itk::ThreadIdType)
        {
            // Get the input images
            InputImageConstPointerType lIPAOTSubPtr = this->GetIPAOTSub();
            InputDTMConstPointerType lDTMsubPtr = this->GetDTMSub();
            InputMaskConstPointerType lIPEDGPtr = this->GetIPEDG();

            OutputVectorImagePointerType lTdirPtr = this->GetTdir();
            OutputVectorImagePointerType lTdifPtr = this->GetTdif();

            // Define input Iterators
            ImageRegionConstIteratorType lIPAOTSubIt = ImageRegionConstIteratorType(lIPAOTSubPtr, outputRegionForThread);
            DTMRegionConstIteratorType lDTMSubIt = DTMRegionConstIteratorType(lDTMsubPtr, outputRegionForThread);
            MaskRegionConstIteratorType lIPEDGIt = MaskRegionConstIteratorType(lIPEDGPtr, outputRegionForThread);

            // Define output Iterators
            VectorImageRegionIteratorType lTdirIt = VectorImageRegionIteratorType(lTdirPtr, outputRegionForThread);
            VectorImageRegionIteratorType lTdifIt = VectorImageRegionIteratorType(lTdifPtr, outputRegionForThread);

            lIPAOTSubIt.GoToBegin();
            lDTMSubIt.GoToBegin();
            lIPEDGIt.GoToBegin();
            lTdirIt.GoToBegin();
            lTdifIt.GoToBegin();

            LUTPointType point;

            OutputVectorImagePixelType lPixelTdir;
            OutputVectorImagePixelType lPixelTdif;

            // Initialize the number of bands of the output images
            lPixelTdir.SetSize(m_NbOfBands);
            lPixelTdif.SetSize(m_NbOfBands);

            // Compute the direct and diffuse images
            // by interpolating each pixel value in the associated LUT
            // Do the work
            while ((!lTdirIt.IsAtEnd()) || (!lTdifIt.IsAtEnd()) || (!lIPAOTSubIt.IsAtEnd()) || (!lDTMSubIt.IsAtEnd())
                    || (!lIPEDGIt.IsAtEnd()))
            {
                while ((!lTdirIt.IsAtEndOfLine()) || (!lTdifIt.IsAtEndOfLine()) || (!lIPAOTSubIt.IsAtEndOfLine())
                        || (!lDTMSubIt.IsAtEndOfLine()) || (!lIPEDGIt.IsAtEndOfLine()))
                {

                    // Init output pixels
                    lPixelTdir.Fill(0.);
                    lPixelTdif.Fill(0.);

                    // Except edges
                    if (lIPEDGIt.Get() == 0)
                    {
                        // ThetaS expected in degree
                        point[0] = static_cast<LUTPointValueType>(m_ThetaS);
                        point[1] = static_cast<LUTPointValueType>(lDTMSubIt.Get());
                        point[2] = static_cast<LUTPointValueType>(lIPAOTSubIt.Get());

                        // Interpolate in the LUT (3dim) with 4 bands (LUTPixelType is a VariableLengthVector of size 4)
                        lPixelTdir = m_LUT_Tdir->InterpolateValue(point);

                        // Interpolate in the LUT at the date LPD
                        lPixelTdif = m_LUT_Tdif->InterpolateValue(point);

                    }
                    // Set the interpolated value in the output image
                    lTdirIt.Set(lPixelTdir);
                    lTdifIt.Set(lPixelTdif);

                    ++lIPAOTSubIt;
                    ++lDTMSubIt;
                    ++lIPEDGIt;
                    ++lTdirIt;
                    ++lTdifIt;
                }

                lIPAOTSubIt.NextLine();
                lDTMSubIt.NextLine();
                lIPEDGIt.NextLine();
                lTdirIt.NextLine();
                lTdifIt.NextLine();

            } // end while

        }

//
//******************* PrintSelf method  ****************
//
    template<class TInputImage, class TInputMask, class TInputDTM, class TInputLUT, class TOutputVectorImage>
        void
        ComputeTransmissionImageFilter<TInputImage, TInputMask, TInputDTM, TInputLUT, TOutputVectorImage>::PrintSelf(std::ostream& os,
                itk::Indent indent) const
        {
            Superclass::PrintSelf(os, indent);
        }

//
//******************* Tdir image accessors  ****************
//
    template<class TInputImage, class TInputMask, class TInputDTM, class TInputLUT, class TOutputVectorImage>
        typename ComputeTransmissionImageFilter<TInputImage, TInputMask, TInputDTM, TInputLUT, TOutputVectorImage>::OutputVectorImageType *
        ComputeTransmissionImageFilter<TInputImage, TInputMask, TInputDTM, TInputLUT, TOutputVectorImage>::GetTdir(void)
        {
            return dynamic_cast<OutputVectorImageType *>(itk::ProcessObject::GetOutput(0));
        }

//
//******************* Tdif image accessors  ****************
//
    template<class TInputImage, class TInputMask, class TInputDTM, class TInputLUT, class TOutputVectorImage>
        typename ComputeTransmissionImageFilter<TInputImage, TInputMask, TInputDTM, TInputLUT, TOutputVectorImage>::OutputVectorImageType *
        ComputeTransmissionImageFilter<TInputImage, TInputMask, TInputDTM, TInputLUT, TOutputVectorImage>::GetTdif(void)
        {
            return dynamic_cast<OutputVectorImageType *>(itk::ProcessObject::GetOutput(1));
        }

} // End namespace vns

#endif /* __vnsComputeTransmissionImageFilter_txx */
