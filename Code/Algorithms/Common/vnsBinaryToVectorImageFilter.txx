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
 * VERSION : 1-0-0 : <TypeFT> : <NumFT> : 11 mai 2010 : Creation                                                           
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id$
 *                                                                                                          *
 ************************************************************************************************************/
#ifndef __vnsBinaryToVectorImageFilter_txx
#define __vnsBinaryToVectorImageFilter_txx

#include "vnsBinaryToVectorImageFilter.h"

#include "itkImageRegionIterator.h"
#include "itkImageRegionConstIterator.h"
#include "itkNumericTraits.h"

namespace vns
{

    /** Constructor */
    template<class TInputImage, class TOutputImage>
        BinaryToVectorImageFilter<TInputImage, TOutputImage>::BinaryToVectorImageFilter() : m_NumberOfComponentsPerPixel(1)
        {
        }

    /** Destructor */
    template<class TInputImage, class TOutputImage>
        BinaryToVectorImageFilter<TInputImage, TOutputImage>::~BinaryToVectorImageFilter()
        {
        }

    template<class TInputImage, class TOutputImage>
        void
        BinaryToVectorImageFilter<TInputImage, TOutputImage>::GenerateOutputInformation()
        {
            // Call to the superclass implementation
            Superclass::GenerateOutputInformation();

            typename Superclass::InputImageConstPointer inputPtr = this->GetInput();
            typename Superclass::OutputImagePointer outputPtr = this->GetOutput();

            // initialize the number of channels of the output image
            outputPtr->SetNumberOfComponentsPerPixel(m_NumberOfComponentsPerPixel);
        }

    /** PrintSelf method */
    template<class TInputImage, class TOutputImage>
        void
        BinaryToVectorImageFilter<TInputImage, TOutputImage>::BeforeThreadedGenerateData()
        {
            if (m_NumberOfComponentsPerPixel > (sizeof(InputImagePixelType) * 8))
            {
                vnsExceptionBusinessMacro("The pixel type used to read the input image in not valid.\n"
                        "Its size must be greater than the number of component extracted from the pixel.")
            }
            for (unsigned int i = 0; i < m_NumberOfComponentsPerPixel; i++)
            {
                m_BitWeight.push_back(static_cast<unsigned int> (vcl_pow(2., static_cast<double> (i))));
            }
        }

    template<class TInputImage, class TOutputImage>
        void
        BinaryToVectorImageFilter<TInputImage, TOutputImage>::ThreadedGenerateData(
                const RegionType& outputRegionForThread, itk::ThreadIdType /*threadId*/)
        {
            // Grab the input and output
            typename Superclass::OutputImagePointer outputPtr = this->GetOutput();
            typename Superclass::InputImageConstPointer inputPtr = this->GetInput();

            // Define the iterators
            itk::ImageRegionConstIterator<InputImageType> inputIt(inputPtr, outputRegionForThread);
            itk::ImageRegionIterator<OutputImageType> outputIt(outputPtr, outputRegionForThread);


            // Iterator initialization
            inputIt.GoToBegin();
            outputIt.GoToBegin();


            OutputImagePixelType outputVectorValue;
            outputVectorValue.SetSize(m_NumberOfComponentsPerPixel);
            outputVectorValue.Fill(itk::NumericTraits<OutputImageInternalPixelType>::Zero);
            const unsigned int nbValue = outputVectorValue.GetSize();

            // Pixel loop
            while (inputIt.IsAtEnd() == false)
            {
                InputImagePixelType inputValue = inputIt.Get();
                // Band loop
                for (unsigned int j = 0 ; j < nbValue ; j++)
                {
                    // Set to one the bit to the associated band
                    outputVectorValue[j] = static_cast<OutputImageInternalPixelType> ((inputValue
                            & m_BitWeight[j]) >> j);

                }
                // Set the output pixel value
                outputIt.Set(static_cast<OutputImagePixelType> (outputVectorValue));

                ++inputIt;
                ++outputIt;
            }
        }

    /** PrintSelf method */
    template<class TInputImage, class TOutputImage>
        void
        BinaryToVectorImageFilter<TInputImage, TOutputImage>::PrintSelf(std::ostream& os, itk::Indent indent) const
        {
            Superclass::PrintSelf(os, indent);
        }

} // End namespace vns

#endif /* __vnsBinaryToVectorImageFilter_txx */
