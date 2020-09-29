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
 * VERSION : 5-1-0 : FA : LAIG-FA-MAC-144719-CS : 17 mai 2016 : Correction warning et contrôle qualité      *
 * VERSION : 4-4-1 : FA : LAIG-FA-MAC-127944-CS : 9 juillet 2015 : Correction warning et contrôle qualité   *
 * 														sur le code source C++   							*
 * VERSION : 1-0-0 : <TypeFT> : <NumFT> : 2 août 2010 : Creation
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id$
 *                                                                                                          *
 ************************************************************************************************************/
#ifndef __vnsConstantPadVectorImageFilter_txx
#define __vnsConstantPadVectorImageFilter_txx

#include "vnsConstantPadVectorImageFilter.h"
#include "itkImageRegionIterator.h"
#include "itkImageRegionConstIterator.h"
#include "itkObjectFactory.h"
#include "vnsMacro.h"

namespace vns
{

    /**
     *
     */
    template<class TInputImage, class TOutputImage>
        ConstantPadVectorImageFilter<TInputImage, TOutputImage>::ConstantPadVectorImageFilter()
        {
            m_ScalarConstant = itk::NumericTraits<OutputImageInternalPixelType>::Zero;
        }

    /**
     *
     */
    template<class TInputImage, class TOutputImage>
        void
        ConstantPadVectorImageFilter<TInputImage, TOutputImage>::PrintSelf(std::ostream& os, itk::Indent indent) const
        {
            Superclass::PrintSelf(os, indent);

            os << indent << "Constant: " << static_cast<typename itk::NumericTraits<OutputImagePixelType>::PrintType> (m_Constant)
                    << std::endl;
            os << std::endl;
        }

    /**
     * Given an n dimensional list of output region breakpoints in indices
     * and size (where the current region and maximum region for each dimension
     * is encoded in regIndices and regLimit), choose the next output region.
     */
    template<class TInputImage, class TOutputImage>
        int
        ConstantPadVectorImageFilter<TInputImage, TOutputImage>::GenerateNextRegion(long *regIndices, long *regLimit,
                OutputImageIndexType *indices, OutputImageSizeType *sizes, OutputImageRegionType& outputRegion)
        {
            unsigned int ctr;
            int done = 0;
            OutputImageIndexType nextIndex = outputRegion.GetIndex();
            OutputImageSizeType nextSize = outputRegion.GetSize();

            // loop over the image dimensions (2D)
            for (ctr = 0; (ctr < ImageDimension) && !done; ctr++)
            {
                regIndices[ctr]++;
                done = 1;
                // Test if the indices are below the limit
                if (regIndices[ctr] >= regLimit[ctr])
                {
                    regIndices[ctr] = 0;
                    done = 0;
                }
                // Store the indices
                nextIndex[ctr] = indices[regIndices[ctr]][ctr];
                nextSize[ctr] = sizes[regIndices[ctr]][ctr];
            }
            // Set in the indices of the output region
            outputRegion.SetIndex(nextIndex);
            outputRegion.SetSize(nextSize);

            // Test if an output region was found
            for (ctr = 0; ctr < ImageDimension; ctr++)
            {
                if (nextSize[ctr] == 0)
                {
                    return 0;
                }
            }

            return 1;
        }

    template<class TInputImage, class TOutputImage>
        void
        ConstantPadVectorImageFilter<TInputImage, TOutputImage>::BeforeThreadedGenerateData()
        {
            // Initialize the Constant value
            m_Constant.SetSize(this->GetInput()->GetNumberOfComponentsPerPixel());
            m_Constant.Fill(m_ScalarConstant);
            //vnsLogSuperDebugMacro("m_Constant Size:" << m_Constant.Size() )
        }
    /**
     *
     */
    template<class TInputImage, class TOutputImage>
        void
        ConstantPadVectorImageFilter<TInputImage, TOutputImage> // support progress methods/callbacks

        ::ThreadedGenerateData(const OutputImageRegionType& outputRegionForThread, itk::ThreadIdType /*threadId*/)
        {
            unsigned int dimCtr, regCtr, ctr = 0;
            unsigned int numRegions = 1; // number of regions in our decomposed space.
            long sizeTemp; // We need to calculate negative sizes.  This allows us to do so.

            itkDebugMacro(<<"Actually executing");

            // Get the input and output pointers
            typename Superclass::InputImageConstPointer inputPtr = this->GetInput();
            typename Superclass::OutputImagePointer outputPtr = this->GetOutput();

            // Define a few indices that will be used to translate from an input pixel
            // to an output pixel
            OutputImageIndexType outputIndex = outputRegionForThread.GetIndex();
            InputImageIndexType inputIndex = inputPtr->GetLargestPossibleRegion().GetIndex();
            OutputImageSizeType outputSize = outputRegionForThread.GetSize();
            InputImageSizeType inputSize = inputPtr->GetLargestPossibleRegion().GetSize();

            OutputImageRegionType outputRegion;
            InputImageRegionType inputRegion;

            // For n dimensions, there are 3^n combinations of before, between, and
            // after on these regions.  We are keeping this flexible so that we
            // can handle other blockings imposed by the mirror and wrap algorithms.
            OutputImageIndexType indices[3];
            OutputImageSizeType sizes[3];
            long regIndices[ImageDimension];
            long regLimit[ImageDimension];

            for (dimCtr = 0; dimCtr < ImageDimension; dimCtr++)
            {
                regIndices[dimCtr] = 2;
                regLimit[dimCtr] = 3;
                numRegions = 3*numRegions;

                // Region 0 is between, which has a starting index equal to
                // the input region starting index, unless that would be
                // outside the bounds of the output image.
                if (inputIndex[dimCtr] > outputIndex[dimCtr])
                {
                    indices[0][dimCtr] = inputIndex[dimCtr];
                }
                else
                {
                    indices[0][dimCtr] = outputIndex[dimCtr];
                }
                // Region 1 is before, which is always the output starting index,
                // and Region 2 is after, which is either the end of the input
                // image, or the start of the output image.
                indices[1][dimCtr] = outputIndex[dimCtr];

                if ((inputIndex[dimCtr] + static_cast<long> (inputSize[dimCtr])) > outputIndex[dimCtr])
                {
                    indices[2][dimCtr] = inputIndex[dimCtr] + static_cast<long> (inputSize[dimCtr]);
                }
                else
                {
                    indices[2][dimCtr] = outputIndex[dimCtr];
                }
                // Size 0 is the area from index 0 to the end of the input or the
                // output, whichever comes first.
                if ((inputIndex[dimCtr] + static_cast<long> (inputSize[dimCtr])) < (outputIndex[dimCtr]
                        + static_cast<long> (outputSize[dimCtr])))
                {
                    sizeTemp = inputIndex[dimCtr] + static_cast<long> (inputSize[dimCtr]) - indices[0][dimCtr];
                }
                else
                {
                    sizeTemp = outputIndex[dimCtr] + static_cast<long> (outputSize[dimCtr]) - indices[0][dimCtr];
                }
                if (sizeTemp > 0)
                {
                    sizes[0][dimCtr] = sizeTemp;
                }
                else
                {
                    sizes[0][dimCtr] = 0;
                }
                // Size 1 is all the output that preceeds the input, and Size 2 is
                // all the output that succeeds the input.
                if ((outputIndex[dimCtr] + static_cast<long> (outputSize[dimCtr])) > indices[0][dimCtr])
                {
                    sizeTemp = indices[0][dimCtr] - outputIndex[dimCtr];
                }
                else
                {
                    sizeTemp = static_cast<long> (outputSize[dimCtr]);
                }
                if (sizeTemp > 0)
                {
                    sizes[1][dimCtr] = sizeTemp;
                }
                else
                {
                    sizes[1][dimCtr] = 0;
                }
                sizeTemp = outputIndex[dimCtr] + static_cast<long> (outputSize[dimCtr]) - indices[2][dimCtr];
                if (sizeTemp > 0)
                {
                    sizes[2][dimCtr] = sizeTemp;
                }
                else
                {
                    sizes[2][dimCtr] = 0;
                }

            }


            // Define/declare iterators that will walk the input and output regions
            // for this thread.
            outputRegion.SetSize(sizes[0]);
            outputRegion.SetIndex(indices[0]);
            inputRegion.SetSize(sizes[0]);
            inputRegion.SetIndex(indices[0]);

            typedef itk::ImageRegionIterator<TOutputImage> OutputIterator;
            typedef itk::ImageRegionConstIterator<TInputImage> InputIterator;

            // Walk the first region which is defined as the between for everyone.
            if (GenerateNextRegion(regIndices, regLimit, indices, sizes, outputRegion))
            {
                inputRegion.SetIndex(outputRegion.GetIndex());
                inputRegion.SetSize(outputRegion.GetSize());
                OutputIterator outIt = OutputIterator(outputPtr, outputRegion);
                InputIterator inIt = InputIterator(inputPtr, inputRegion);

                // walk the output region, and sample the input image
                for (ctr = 0; !outIt.IsAtEnd(); ++outIt, ++inIt, ctr++)
                {
                    // copy the input pixel to the output
                    outIt.Set(inIt.Get());
                }
            }

            // Now walk the remaining regions.
            for (regCtr = 1; regCtr < numRegions; regCtr++)
            {
                if (GenerateNextRegion(regIndices, regLimit, indices, sizes, outputRegion))
                {
                    OutputIterator outIt = OutputIterator(outputPtr, outputRegion);

                    // walk the output region, and sample the input image
                    for (; !outIt.IsAtEnd(); ++outIt, ctr++)
                    {
                        // copy the input pixel to the output
                        outIt.Set(m_Constant);
                    }
                }
            }
        }

} // end namespace vns

#endif
