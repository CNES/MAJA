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
 * VERSION : 3.3.0 : DM : LAIG-DM-MAJA-3044-CNES : 15 mars 2019 : Integration modifications oliver hagolle  *
 * VERSION : 2.1.0 : FA : LAIG-FA-MAJA-157185-CS : 9 mai 2017 : Correction de règles qualité                *
 * VERSION : 2.0.0 : DM : LAIG-DM-MAJA-2149-CNES : 24 fevrier 2017 : Correction Cirrus - création           *
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id$
 *                                                                                                          *
 ************************************************************************************************************/

#ifndef __vnsCirrusCorrectionApplyImageFilter_txx
#define __vnsCirrusCorrectionApplyImageFilter_txx

#include "vnsCirrusCorrectionApplyImageFilter.h"
#include "itkImageRegionConstIterator.h"
#include "itkImageRegionIterator.h"
#include "vnsMacro.h"
#include "vnsMath.h"

namespace vns
{

    /**
     * Constructor
     */
    template <class TInputImageTOA, class TInputImageRefl,
    class TInputImageMask, class TInputImageDist,
    class TOutputImage>
    CirrusCorrectionApplyImageFilter<TInputImageTOA, TInputImageRefl, TInputImageMask, TInputImageDist, TOutputImage>
    ::CirrusCorrectionApplyImageFilter() : m_Gamma(0), m_SWIRGammaFactor(0), m_ForegroundMaskValue(0), m_MaxReflectance(0.07)
     {
        this->InPlaceOff();
        // Set the number of inputs
        this->SetNumberOfIndexedInputs(5);
        this->SetNumberOfRequiredInputs(5);
     }

    /**
     * BeforeThreadedGenerateData function. Validate inputs
     */
    template <class TInputImageTOA, class TInputImageRefl,
    class TInputImageMask, class TInputImageDist,
    class TOutputImage>
    void
    CirrusCorrectionApplyImageFilter<TInputImageTOA, TInputImageRefl, TInputImageMask, TInputImageDist, TOutputImage>
    ::BeforeThreadedGenerateData()
     {
        InputTOAImagePointer l_inputTOAPtr = this->GetTOAInput();
        InputReflImagePointer l_inputReflCenterPtr = this->GetReflCenterInput();
        InputReflImagePointer l_inputReflEdgePtr = this->GetReflEdgeInput();
        InputMaskImagePointer l_inputMaskPtr = this->GetCirrusMaskInput();
        InputDistImagePointer l_inputDistPtr = this->GetCirrusDistInput();

        if (l_inputTOAPtr.IsNull() || l_inputReflCenterPtr.IsNull() || l_inputReflEdgePtr.IsNull()
                || l_inputMaskPtr.IsNull() || l_inputDistPtr.IsNull())
        {
            itkExceptionMacro(<< "At least one input is missing."
                    << " InputToa is " << l_inputTOAPtr.GetPointer() << ", "
                    << " InputReflCenter is " << l_inputReflEdgePtr.GetPointer() << ", "
                    << " InputReflEdge is " << l_inputReflCenterPtr.GetPointer() << ", "
                    << " InputDist is " << l_inputDistPtr.GetPointer() << ", "
                    << " InputMask is " << l_inputMaskPtr.GetPointer());

        }
     }

    /**
     * ThreadedGenerateData function. Performs the pixel-wise addition
     */
    template <class TInputImageTOA, class TInputImageRefl,
    class TInputImageMask, class TInputImageDist,
    class TOutputImage>
    void
    CirrusCorrectionApplyImageFilter<TInputImageTOA, TInputImageRefl, TInputImageMask, TInputImageDist, TOutputImage>
    ::ThreadedGenerateData(const OutputImageRegionType& outputRegionForThread, itk::ThreadIdType /*threadId*/)
     {
        // We use dynamic_cast since inputs are stored as DataObjects.  The
        // ImageToImageFilter::GetInput(int) always returns a pointer to a
        // TInputImage1 so it cannot be used for the second or third input.
        InputTOAImagePointer l_inputTOAPtr = this->GetTOAInput();
        InputReflImagePointer l_inputReflCenterPtr = this->GetReflCenterInput();
        InputReflImagePointer l_inputReflEdgePtr = this->GetReflEdgeInput();
        InputMaskImagePointer l_inputMaskPtr = this->GetCirrusMaskInput();
        InputDistImagePointer l_inputDistPtr = this->GetCirrusDistInput();
        OutputImagePointer l_outputTOAPtr = this->GetOutput(0);

        itk::ImageRegionConstIterator<TInputImageTOA> inputTOAIt(l_inputTOAPtr, outputRegionForThread);
        itk::ImageRegionConstIterator<TInputImageRefl> inputReflCenterIt(l_inputReflCenterPtr, outputRegionForThread);
        itk::ImageRegionConstIterator<TInputImageRefl> inputReflEdgeIt(l_inputReflEdgePtr, outputRegionForThread);
        itk::ImageRegionConstIterator<TInputImageDist> inputDistIt(l_inputDistPtr, outputRegionForThread);
        itk::ImageRegionConstIterator<TInputImageMask> inputMaskIt(l_inputMaskPtr, outputRegionForThread);
        itk::ImageRegionIterator<TOutputImage> outputTOAIt(l_outputTOAPtr, outputRegionForThread);

        inputTOAIt.GoToBegin();
        inputReflCenterIt.GoToBegin();
        inputReflEdgeIt.GoToBegin();
        inputMaskIt.GoToBegin();
        inputDistIt.GoToBegin();
        outputTOAIt.GoToBegin();

        //Compute gamm final based on band code list
        std::vector<double> l_GammaFinal;
        for (Utilities::ListOfStrings::const_iterator it = m_ListOfBandCode.begin();it != m_ListOfBandCode.end();++it)
        {
            //If the band is found in SWIR list
            if (std::find(m_ListOfSWIRBandCode.begin(),m_ListOfSWIRBandCode.end(), *it) != m_ListOfSWIRBandCode.end())
            {
                l_GammaFinal.push_back(m_Gamma*m_SWIRGammaFactor);
            } else
            {
                l_GammaFinal.push_back(m_Gamma);
            }
        }
        //Loop on input pixels
        while (!inputTOAIt.IsAtEnd())
        {
            if(vnsEqualsDoubleMacro(inputMaskIt.Get(),1.0))
            {
                OutputImagePixelType outPix;
                const InputTOAImagePixelType& inPix = inputTOAIt.Get();
                const unsigned int l_size = inPix.Size();

                // Set output pixel dimension
                outPix.SetSize(l_size);

		// limit cirrus reflectance to a maximum value
		//const double maxReflCirrus=0.07;


		double reflCenter = inputReflCenterIt.Get();
		double reflEdge = inputReflEdgeIt.Get();


		if (reflCenter > m_MaxReflectance)
		   reflCenter = m_MaxReflectance;
		if (reflEdge > m_MaxReflectance)
		  reflEdge = m_MaxReflectance;

                // Band Loop
                for (unsigned int bd = 0; bd < l_size; bd++)
                {
                    const double l_RToaCenter = static_cast<double>(inPix[bd] - (reflCenter / l_GammaFinal[bd]));
                    const double l_RToaEdge = static_cast<double>(inPix[bd] - (reflEdge / l_GammaFinal[bd]));

                    outPix[bd] = static_cast<OutputImageInternalPixelType>((1.0 - inputDistIt.Get())*l_RToaEdge + inputDistIt.Get()*l_RToaCenter);
                    outPix[bd] = vnsStrictlyInfDoubleMacro(outPix[bd],0.0,CONST_EPSILON)?0.0:outPix[bd];

                }
                outputTOAIt.Set(outPix);
            } else
            {
                outputTOAIt.Set(inputTOAIt.Get());
            }
            ++inputTOAIt;
            ++inputReflCenterIt;
            ++inputReflEdgeIt;
            ++inputMaskIt;
            ++inputDistIt;
            ++outputTOAIt;

        }
     }
} // end namespace otb

#endif

