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
 * VERSION : 4-4-0 : FA : LAIG-FA-MAC-133882-CS : 3 juin 2015 : Correction code pour la qualite             *
 * VERSION : 3-2-0 : FA : LAIG-FA-MAC-701-CNES : 21 novembre 2013 : Correction de l'information géographique*
 * 									dans les produits. Ajout de l'origine, du spacing et de la projection   *
 * VERSION : 1-0-0 : <TypeFT> : <NumFT> : 6 avr. 2010 : Creation                                                           
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id$
 *                                                                                                          *
 ************************************************************************************************************/
#ifndef __vnsBinaryConcatenationVectorImageFilter_txx
#define __vnsBinaryConcatenationVectorImageFilter_txx

#include "vnsMacro.h"
#include "vnsBinaryConcatenationVectorImageFilter.h"

#include "itkImageRegionIterator.h"
#include "itkImageRegionConstIterator.h"
#include "itkNumericTraits.h"

#include "itkMetaDataObject.h"
#include "otbMetaDataKey.h"
#include "vnsUtilities.h"

namespace vns
{

    /** Constructor */
    template<class TInputImage, class TOutputImage>
        BinaryConcatenationVectorImageFilter<TInputImage, TOutputImage>::BinaryConcatenationVectorImageFilter()
        {
        }

    /** Destructor */
    template<class TInputImage, class TOutputImage>
        BinaryConcatenationVectorImageFilter<TInputImage, TOutputImage>::~BinaryConcatenationVectorImageFilter()
        {
        }

    template<class TInputImage, class TOutputImage>
        void
        BinaryConcatenationVectorImageFilter<TInputImage, TOutputImage>::BeforeThreadedGenerateData()
        {
            // Get the number of bits per pixel
            unsigned int numberOfBits = sizeof(OutputInternalPixelType) * 8;

            // Get the number of bands
            typename Superclass::InputImageConstPointer inputPtr = this->GetInput();

            const unsigned int numberOfComponent = inputPtr->GetNumberOfComponentsPerPixel();

            // one bit per band is needed to concatenate the input masks
            if (numberOfComponent > numberOfBits)
            {
                vnsExceptionBusinessMacro("BinaryConcatenationVectorImageFilter::BeforeThreadedGenerateData() failed.\n"
                        "The number of bands of the input image must be less than or equal \n"
                        "to the number of bits of the output image ! ");
            }

        }

    template<class TInputImage, class TOutputImage>
        void
        BinaryConcatenationVectorImageFilter<TInputImage, TOutputImage>::ThreadedGenerateData(const RegionType& outputRegionForThread,
                itk::ThreadIdType /*threadId*/)
        {
            // Definition of the value of the pixel in a mask
            InputInternalPixelType maxPixelValue = static_cast<InputInternalPixelType>(1);

            // Grab the input
            typename Superclass::OutputImagePointer outputPtr = this->GetOutput();
            typename Superclass::InputImageConstPointer inputPtr = this->GetInput();

            // Define the iterators
            itk::ImageRegionConstIterator<InputImageType> inputIt(inputPtr, outputRegionForThread);
            itk::ImageRegionIterator<OutputImageType> outputIt(outputPtr, outputRegionForThread);


            // Iterator initialization
            inputIt.GoToBegin();
            outputIt.GoToBegin();

            // MACCS 4.3 - optimization
            // Pre computes Pow indexes
            const InputImagePixelType inputVectorValue = inputIt.Get();
            const unsigned int nbValue = inputVectorValue.GetSize();
            Utilities::ListOfDoubles l_TabPowIndexes;
            for (unsigned int i = 0; i < nbValue; i++)
            {
                l_TabPowIndexes.push_back( vcl_pow(2., static_cast<double> (i)));
            }

            // Pixel loop
            while (inputIt.IsAtEnd() == false)
            {
                const InputImagePixelType inputVectorValue = inputIt.Get();
                OutputInternalPixelType outputValue = itk::NumericTraits<OutputInternalPixelType>::Zero;

                // Band loop
                for (unsigned int j = 0; j < nbValue; j++)
                {
                    // The pixel values greater than or equal to one are reduced to one
                    // and the pixel values lower than one are set to zero.
                    if (inputVectorValue[j] >= maxPixelValue)
                    {
                        outputValue = outputValue + static_cast<OutputInternalPixelType>(l_TabPowIndexes[j]);
                    }
                }

                // Set the output pixel value
                outputIt.Set(static_cast<OutputImagePixelType>(outputValue));

                ++inputIt;
                ++outputIt;
            }
        }

    template<class TInputImage, class TOutputImage>
        void
        BinaryConcatenationVectorImageFilter<TInputImage, TOutputImage>::GenerateOutputInformation()
        {
            // Call the superclass' implementation of this method
            Superclass::GenerateOutputInformation();

            // Get pointers to the input and output
            InputImagePointer inputPtr = const_cast<TInputImage *>(this->GetInput());

            if ((inputPtr.IsNull() == true))
            {
                return;
            }
            SizeType l_InputImageSize = inputPtr->GetLargestPossibleRegion().GetSize();
            // We need to compute the output spacing, the output image size, and the
            // output image start index
            const typename TInputImage::SpacingType& inputSpacing = inputPtr->GetSpacing();
            const typename TInputImage::IndexType& inputStartIndex = inputPtr->GetLargestPossibleRegion().GetIndex();
            const typename TInputImage::PointType& inputOrigin = inputPtr->GetOrigin();

            OutputImagePointer outputPtr = this->GetOutput();

            outputPtr->SetSpacing(inputSpacing);
            outputPtr->SetOrigin(inputOrigin);

            typename TOutputImage::RegionType outputLargestPossibleRegion;
            outputLargestPossibleRegion.SetSize(l_InputImageSize);
            outputLargestPossibleRegion.SetIndex(inputStartIndex);

            outputPtr->SetLargestPossibleRegion(outputLargestPossibleRegion);
        }

    /** PrintSelf method */
    template<class TInputImage, class TOutputImage>
        void
        BinaryConcatenationVectorImageFilter<TInputImage, TOutputImage>::PrintSelf(std::ostream& os, itk::Indent indent) const
        {
            Superclass::PrintSelf(os, indent);
        }

} // End namespace vns

#endif /* __vnsBinaryConcatenationVectorImageFilter_txx */
