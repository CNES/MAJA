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
 * VERSION : 5-1-0 : FA : LAIG-FA-MAC-144719-CS : 17 mai 2016 : Correction warning et contrôle qualité      *
 * VERSION : 4-0-0 : FA : LAIG-FA-MAC-120653-CS : 3 avril 2014 : Correction de règles de qualité			*
 * VERSION : 1-0-0 : <TypeFT> : <NumFT> : 1 septembre 2010 : Creation
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id$
 *                                                                                                          *
 ************************************************************************************************************/
#ifndef __vnsInterpolateForEnvCorrectionImageFilter_txx
#define __vnsInterpolateForEnvCorrectionImageFilter_txx

#include "vnsInterpolateForEnvCorrectionImageFilter.h"

namespace vns
{

//
//******************* Constructor *******************
//
    template<class TInputImage, class TInputMask, class TInputDTM, class TLUT3D, class TLUT2D, class TOutputVectorImage>
        InterpolateForEnvCorrectionImageFilter<TInputImage, TInputMask, TInputDTM, TLUT3D, TLUT2D, TOutputVectorImage>::InterpolateForEnvCorrectionImageFilter()
        {

            this->SetNumberOfRequiredInputs(3);
            this->SetNumberOfIndexedInputs(3);
            this->SetNumberOfIndexedOutputs(3);

            this->SetNthOutput(0, OutputVectorImageType::New());
            this->SetNthOutput(1, OutputVectorImageType::New());
            this->SetNthOutput(2, OutputVectorImageType::New());

            // Initialization
            m_ThetaV.clear();
            m_NbOfBands = 0;

        }

//
//******************* Destructor *******************
//
    template<class TInputImage, class TInputMask, class TInputDTM, class TLUT3D, class TLUT2D, class TOutputVectorImage>
        InterpolateForEnvCorrectionImageFilter<TInputImage, TInputMask, TInputDTM, TLUT3D, TLUT2D, TOutputVectorImage>::~InterpolateForEnvCorrectionImageFilter()
        {
        }

//
//******************* GenerateOutputInformation function *******************
//
    template<class TInputImage, class TInputMask, class TInputDTM, class TLUT3D, class TLUT2D, class TOutputVectorImage>
        void
        InterpolateForEnvCorrectionImageFilter<TInputImage, TInputMask, TInputDTM, TLUT3D, TLUT2D, TOutputVectorImage>::GenerateOutputInformation()
        {

            Superclass::GenerateOutputInformation();
            OutputVectorImagePointerType lTdirPtr = this->GetTdir();
            OutputVectorImagePointerType lTdifPtr = this->GetTdif();
            OutputVectorImagePointerType lSatmPtr = this->GetSatm();

            lTdirPtr->SetNumberOfComponentsPerPixel(m_NbOfBands);
            lTdifPtr->SetNumberOfComponentsPerPixel(m_NbOfBands);
            lSatmPtr->SetNumberOfComponentsPerPixel(m_NbOfBands);
        }

//
//******************* ThreadedGenerateData function *******************
//
    template<class TInputImage, class TInputMask, class TInputDTM, class TLUT3D, class TLUT2D, class TOutputVectorImage>
        void
        InterpolateForEnvCorrectionImageFilter<TInputImage, TInputMask, TInputDTM, TLUT3D, TLUT2D, TOutputVectorImage>::BeforeThreadedGenerateData()
        {

            InputImageConstPointerType lIPAOTSubPtr = this->GetIPAOTSub();
            InputDTMConstPointerType lDTMsubPtr = this->GetDTMSub();
            OutputVectorImagePointerType lTdirPtr = this->GetTdir();
            OutputVectorImagePointerType lTdifPtr = this->GetTdif();
            OutputVectorImagePointerType lSatmPtr = this->GetSatm();

            if ((lIPAOTSubPtr.IsNull() == true) || (lDTMsubPtr.IsNull() == true) || (m_LUT_Tdir.IsNull() == true)
                    || (m_LUT_Tdif.IsNull() == true))
            {
                itkExceptionMacro(<< "At least one input is missing.");
            }

            // Check the input sizes
            SizeType lSize1 = lIPAOTSubPtr->GetLargestPossibleRegion().GetSize();
            SizeType lSize2 = lDTMsubPtr->GetLargestPossibleRegion().GetSize();
            SizeType lSize3 = lTdirPtr->GetLargestPossibleRegion().GetSize();
            SizeType lSize4 = lTdifPtr->GetLargestPossibleRegion().GetSize();
            SizeType lSize5 = lSatmPtr->GetLargestPossibleRegion().GetSize();

            if ((lSize1 != lSize2) || (lSize1 != lSize3) || (lSize1 != lSize4) || (lSize1 != lSize5))
            {
                itkExceptionMacro(<< "At least one size is not correct.");
            }

        }

//
//******************* ThreadedGenerateData function *******************
//
    template<class TInputImage, class TInputMask, class TInputDTM, class TLUT3D, class TLUT2D, class TOutputVectorImage>
        void
        InterpolateForEnvCorrectionImageFilter<TInputImage, TInputMask, TInputDTM, TLUT3D, TLUT2D, TOutputVectorImage>::ThreadedGenerateData(
                const InputImageRegionType& outputRegionForThread, itk::ThreadIdType)
        {

            // Get the input images
            InputImageConstPointerType lIPAOTSubPtr = this->GetIPAOTSub();
            InputDTMConstPointerType lDTMsubPtr = this->GetDTMSub();
            InputMaskConstPointerType lIPEDGPtr = this->GetIPEDG();

            OutputVectorImagePointerType lTdirPtr = this->GetTdir();
            OutputVectorImagePointerType lTdifPtr = this->GetTdif();
            OutputVectorImagePointerType lSatmPtr = this->GetSatm();

            // Define input Iterators
            ImageRegionConstIteratorType lIPAOTSubIt = ImageRegionConstIteratorType(lIPAOTSubPtr, outputRegionForThread);
            DTMRegionConstIteratorType lDTMSubIt = DTMRegionConstIteratorType(lDTMsubPtr, outputRegionForThread);
            MaskRegionConstIteratorType lIPEDGIt = MaskRegionConstIteratorType(lIPEDGPtr, outputRegionForThread);

            // Define output Iterators
            VectorImageRegionIteratorType lTdirIt = VectorImageRegionIteratorType(lTdirPtr, outputRegionForThread);
            VectorImageRegionIteratorType lTdifIt = VectorImageRegionIteratorType(lTdifPtr, outputRegionForThread);
            VectorImageRegionIteratorType lSatmIt = VectorImageRegionIteratorType(lSatmPtr, outputRegionForThread);

            lIPAOTSubIt.GoToBegin();
            lDTMSubIt.GoToBegin();
            lIPEDGIt.GoToBegin();
            lTdirIt.GoToBegin();
            lTdifIt.GoToBegin();
            lSatmIt.GoToBegin();

            LUT3DPointType point;
            LUT2DPointType point2;

            OutputVectorImagePixelType lPixelTdir;
            OutputVectorImagePixelType lPixelTdif;
            OutputVectorImagePixelType lPixelSatm;

            // Initialize the number of bands of the output images
            lPixelTdir.SetSize(m_NbOfBands);
            lPixelTdif.SetSize(m_NbOfBands);
            lPixelSatm.SetSize(m_NbOfBands);

            // Compute the direct, diffuse, environment reflectance and atmospheric albedo images
            // by interpolating each pixel value in the associated LUT

            if(m_ThetaV.size()!=m_NbOfBands)
            {
            	vnsExceptionDataMacro("Incorrect number of Viewing angles given");
            }

            // Do the work
            while ((!lTdirIt.IsAtEnd()) || (!lTdifIt.IsAtEnd()) || (!lIPAOTSubIt.IsAtEnd()) || (!lDTMSubIt.IsAtEnd())
                    || (!lSatmIt.IsAtEnd()))
            {
                // Init output pixels
                lPixelTdir.Fill(0.);
                lPixelTdif.Fill(0.);
                lPixelSatm.Fill(0.);

                // Except edges
                if (lIPEDGIt.Get() == 0)
                {

                    point[1] = static_cast<LUT3DPointValueType>(lDTMSubIt.Get());
                    point[2] = static_cast<LUT3DPointValueType>(lIPAOTSubIt.Get());

                    point2[0] = static_cast<LUT2DPointValueType>(point[1]);
                    point2[1] = static_cast<LUT2DPointValueType>(point[2]);

                    // Compute PixelTdif and PixelTdir band per band
                    for (unsigned int b = 0; b < m_NbOfBands; b++)
                    {
                        // the value of the zenithal viewing angle depends of the band for sensor as Venus or S2
                        point[0] = static_cast<LUT3DPointValueType>(m_ThetaV.at(b));

                        // interpolate in the LUT of the direct transmission (3dim) with 4 bands (LUTPixelType is a VariableLengthVector of size 4)
                        lPixelTdir[b] = m_LUT_Tdir->InterpolateValue(point)[b];

                        // interpolate in the LUT of the diffuse transmission
                        lPixelTdif[b] = m_LUT_Tdif->InterpolateValue(point)[b];

                    }

                    // Atmorspheric albedo LUT 2 dim
                    lPixelSatm = m_LUT_Satm->InterpolateValue(point2);
                }

                // Set the interpolated value in the output image
                lTdirIt.Set(lPixelTdir);
                lTdifIt.Set(lPixelTdif);
                lSatmIt.Set(lPixelSatm);

                ++lIPAOTSubIt;
                ++lDTMSubIt;
                ++lIPEDGIt;
                ++lTdirIt;
                ++lTdifIt;
                ++lSatmIt;

            } // end while

        }

//
//******************* PrintSelf method  ****************
//
    template<class TInputImage, class TInputMask, class TInputDTM, class TLUT3D, class TLUT2D, class TOutputVectorImage>
        void
        InterpolateForEnvCorrectionImageFilter<TInputImage, TInputMask, TInputDTM, TLUT3D, TLUT2D, TOutputVectorImage>::PrintSelf(
                std::ostream& os, itk::Indent indent) const
        {
            Superclass::PrintSelf(os, indent);
        }

//
//******************* Tdir image accessors  ****************
//
    template<class TInputImage, class TInputMask, class TInputDTM, class TLUT3D, class TLUT2D, class TOutputVectorImage>
        typename InterpolateForEnvCorrectionImageFilter<TInputImage, TInputMask, TInputDTM, TLUT3D, TLUT2D, TOutputVectorImage>::OutputVectorImageType *
        InterpolateForEnvCorrectionImageFilter<TInputImage, TInputMask, TInputDTM, TLUT3D, TLUT2D, TOutputVectorImage>::GetTdir(void)
        {
            return dynamic_cast<OutputVectorImageType *>(itk::ProcessObject::GetOutput(0));
        }

//
//******************* Tdif image accessors  ****************
//
    template<class TInputImage, class TInputMask, class TInputDTM, class TLUT3D, class TLUT2D, class TOutputVectorImage>
        typename InterpolateForEnvCorrectionImageFilter<TInputImage, TInputMask, TInputDTM, TLUT3D, TLUT2D, TOutputVectorImage>::OutputVectorImageType *
        InterpolateForEnvCorrectionImageFilter<TInputImage, TInputMask, TInputDTM, TLUT3D, TLUT2D, TOutputVectorImage>::GetTdif(void)
        {
            return dynamic_cast<OutputVectorImageType *>(itk::ProcessObject::GetOutput(1));
        }

//
//******************* Satm image accessors  ****************
//
    template<class TInputImage, class TInputMask, class TInputDTM, class TLUT3D, class TLUT2D, class TOutputVectorImage>
        typename InterpolateForEnvCorrectionImageFilter<TInputImage, TInputMask, TInputDTM, TLUT3D, TLUT2D, TOutputVectorImage>::OutputVectorImageType *
        InterpolateForEnvCorrectionImageFilter<TInputImage, TInputMask, TInputDTM, TLUT3D, TLUT2D, TOutputVectorImage>::GetSatm(void)
        {
            return dynamic_cast<OutputVectorImageType *>(itk::ProcessObject::GetOutput(2));
        }

} // End namespace vns

#endif /* __vnsInterpolateForEnvCorrectionImageFilter_txx */
