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
 * VERSION : 4-0-0 : FA : LAIG-FA-MAC-120653-CS : 3 avril 2014 : Correction de règles de qualité			*
 * VERSION : 1-0-0 : <TypeFT> : <NumFT> : 1 septembre 2010 : Creation
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id$
 *                                                                                                          *
 ************************************************************************************************************/
#ifndef __vnsInterpolateForEnvCorrection_txx
#define __vnsInterpolateForEnvCorrection_txx

#include "vnsInterpolateForEnvCorrection.h"

namespace vns
{

//
//******************* Constructor *******************
//
    template<class TInputImage, class TInputMask, class TInputDTM, class TLUT3D, class TLUT2D, class TOutputVectorImage>
        InterpolateForEnvCorrection<TInputImage, TInputMask, TInputDTM, TLUT3D, TLUT2D, TOutputVectorImage>::InterpolateForEnvCorrection()
        {

            this->SetNumberOfRequiredInputs(3);
            this->SetNumberOfIndexedInputs(3);
            this->SetNumberOfIndexedOutputs(4);

            this->SetNthOutput(0, OutputVectorImageType::New());
            this->SetNthOutput(1, OutputVectorImageType::New());
            this->SetNthOutput(2, OutputVectorImageType::New());
            this->SetNthOutput(3, OutputVectorImageType::New());

            // Resolution parameters declaration
            m_ThetaV = 0;
            m_NbOfBands = 0;

        }

//
//******************* Destructor *******************
//
    template<class TInputImage, class TInputMask, class TInputDTM, class TLUT3D, class TLUT2D, class TOutputVectorImage>
        InterpolateForEnvCorrection<TInputImage, TInputMask, TInputDTM, TLUT3D, TLUT2D, TOutputVectorImage>::~InterpolateForEnvCorrection()
        {
        }

//
//******************* ThreadedGenerateData function *******************
//
    template<class TInputImage, class TInputMask, class TInputDTM, class TLUT3D, class TLUT2D, class TOutputVectorImage>
        void
        InterpolateForEnvCorrection<TInputImage, TInputMask, TInputDTM, TLUT3D, TLUT2D, TOutputVectorImage>::BeforeThreadedGenerateData()
        {

            // Init the inputs pointers
            InputImageConstPointerType lIPAOTSPtr = this->GetIPAOTS();
            InputDTMConstPointerType lDTMsubPtr = this->GetDTMSub();
            OutputVectorImagePointerType lTdirPtr = this->GetTdir();
            OutputVectorImagePointerType lTdifPtr = this->GetTdif();
            OutputVectorImagePointerType lTPtr = this->GetT();
            OutputVectorImagePointerType lSatmPtr = this->GetSatm();

            // Check validity
            if ((lIPAOTSPtr.IsNull() == true) || (lDTMsubPtr.IsNull() == true) || (m_LUT_Tdir.IsNull() == true)
                    || (m_LUT_Tdif.IsNull() == true))
            {
                itkExceptionMacro(<< "At least one input is missing.");
            }

            // Get the input sizes
            SizeType lSize1 = lIPAOTSPtr->GetLargestPossibleRegion().GetSize();
            SizeType lSize2 = lDTMsubPtr->GetLargestPossibleRegion().GetSize();
            SizeType lSize3 = lTdirPtr->GetLargestPossibleRegion().GetSize();
            SizeType lSize4 = lTdifPtr->GetLargestPossibleRegion().GetSize();
            SizeType lSize5 = lTPtr->GetLargestPossibleRegion().GetSize();
            SizeType lSize6 = lSatmPtr->GetLargestPossibleRegion().GetSize();

            // Check the size
            if ((lSize1 != lSize2) || (lSize1 != lSize3) || (lSize1 != lSize4) || (lSize1 != lSize5) || (lSize1 != lSize6))
            {
                itkExceptionMacro(<< "At least one size is not correct.");
            }

        }

//
//******************* ThreadedGenerateData function *******************
//
    template<class TInputImage, class TInputMask, class TInputDTM, class TLUT3D, class TLUT2D, class TOutputVectorImage>
        void
        InterpolateForEnvCorrection<TInputImage, TInputMask, TInputDTM, TLUT3D, TLUT2D, TOutputVectorImage>::ThreadedGenerateData(
                const InputImageRegionType& outputRegionForThread, itk::ThreadIdType)
        {

    		// Get inputs
            InputImageConstPointerType lIPAOTSPtr = this->GetIPAOTS();
            InputDTMConstPointerType lDTMsubPtr = this->GetDTMSub();
            InputMaskConstPointerType lIPEDGPtr = this->GetIPEDG();

            OutputVectorImagePointerType lTdirPtr = this->GetTdir();
            OutputVectorImagePointerType lTdifPtr = this->GetTdif();
            OutputVectorImagePointerType lTPtr = this->GetT();

            OutputVectorImagePointerType lSatmPtr = this->GetSatm();

            // Define input Iterators
            ImageRegionConstIteratorType lIPAOTSIt = ImageRegionConstIteratorType(lIPAOTSPtr, outputRegionForThread);
            DTMRegionConstIteratorType lDTMSubIt = DTMRegionConstIteratorType(lDTMsubPtr, outputRegionForThread);
            MaskRegionConstIteratorType lIPEDGIt = MaskRegionConstIteratorType(lIPEDGPtr, outputRegionForThread);

            // Define output Iterators
            VectorImageRegionIteratorType lTdirIt = VectorImageRegionIteratorType(lTdirPtr, outputRegionForThread);
            VectorImageRegionIteratorType lTdifIt = VectorImageRegionIteratorType(lTdifPtr, outputRegionForThread);
            VectorImageRegionIteratorType lTIt = VectorImageRegionIteratorType(lTPtr, outputRegionForThread);
            VectorImageRegionIteratorType lSatmIt = VectorImageRegionIteratorType(lSatmPtr, outputRegionForThread);

            lIPAOTSIt.GoToBegin();
            lDTMSubIt.GoToBegin();
            lIPEDGIt.GoToBegin();
            lTdirIt.GoToBegin();
            lTdifIt.GoToBegin();
            lTIt.GoToBegin();
            lSatmIt.GoToBegin();

            LUT3DPointType point;
            LUT2DPointType point2;

            OutputVectorImagePixelType lPixelTdir;
            OutputVectorImagePixelType lPixelTdif;
            OutputVectorImagePixelType lPixelT;
            OutputVectorImagePixelType lPixelSatm;

            // Initialize the number of bands of the output images
            lPixelTdir.SetSize(m_NbOfBands);
            lPixelTdif.SetSize(m_NbOfBands);
            lPixelT.SetSize(m_NbOfBands);
            lPixelSatm.SetSize(m_NbOfBands);

            // Compute the direct, diffuse, environment reflectance and atmospheric albedo images
            // by interpolating each pixel value in the associated LUT
            // Do the work
            while ((!lTdirIt.IsAtEnd()) || (!lTdifIt.IsAtEnd()) || (!lTIt.IsAtEnd()) || (!lTIt.IsAtEnd()) || (!lIPAOTSIt.IsAtEnd())
                    || (!lDTMSubIt.IsAtEnd()) || (!lSatmIt.IsAtEnd()))
            {
                // Init output pixels
                lPixelTdir.Fill(0.);
                lPixelTdif.Fill(0.);
                lPixelT.Fill(0.);
                lPixelSatm.Fill(0.);

                // Except edges
                if (lIPEDGIt.Get() == 0)
                {
                    point[0] = static_cast<LUT3DPointValueType>(m_ThetaV);
                    point[1] = static_cast<LUT3DPointValueType>(lDTMSubIt.Get());
                    point[2] = static_cast<LUT3DPointValueType>(lIPAOTSIt.Get());

                    point2[0] = static_cast<LUT2DPointValueType>(point[1]);
                    point2[1] = static_cast<LUT2DPointValueType>(point[2]);

                    // interpolate in the LUT of the direct transmission (3dim) with 4 bands (LUTPixelType is a VariableLengthVector of size 4)
                    lPixelTdir = m_LUT_Tdir->InterpolateValue(point);

                    // interpolate in the LUT of the diffuse transmission
                    lPixelTdif = m_LUT_Tdif->InterpolateValue(point);

                    // Get the global transmission
                    lPixelT = lPixelTdir + lPixelTdif;

                    // Atmorspheric albedo LUT 2 dim
                    lPixelSatm = m_LUT_Satm->InterpolateValue(point2);
                }

                // Set the interpolated value in the output image
                lTdirIt.Set(lPixelTdir);
                lTdifIt.Set(lPixelTdif);
                lTIt.Set(lPixelT);
                lSatmIt.Set(lPixelSatm);

                ++lIPAOTSIt;
                ++lDTMSubIt;
                ++lIPEDGIt;
                ++lTdirIt;
                ++lTdifIt;
                ++lTIt;
                ++lSatmIt;

            } // end while

        }

//
//******************* PrintSelf method  ****************
//
    template<class TInputImage, class TInputMask, class TInputDTM, class TLUT3D, class TLUT2D, class TOutputVectorImage>
        void
        InterpolateForEnvCorrection<TInputImage, TInputMask, TInputDTM, TLUT3D, TLUT2D, TOutputVectorImage>::PrintSelf(std::ostream& os,
                itk::Indent indent) const
        {
            Superclass::PrintSelf(os, indent);
        }

} // End namespace vns

#endif /* __vnsInterpolateForEnvCorrection_txx */
