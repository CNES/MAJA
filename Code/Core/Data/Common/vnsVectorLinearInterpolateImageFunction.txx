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
/***********************************************************************************************************
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
 * VERSION : 5-1-0 : FA : LAIG-FA-MAC-144674-CS : 26 mai 2016 : Modifications cosmetique et menage, etc...  *
 * VERSION : 1-0-0 : <TypeFT> : <NumFT> : 10 avril 2010 : Creation
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id$
 *                                                                                                          *
 ************************************************************************************************************/
#ifndef __vnsVectorLinearInterpolateImageFunction_txx
#define __vnsVectorLinearInterpolateImageFunction_txx

#include "vnsVectorLinearInterpolateImageFunction.h"

#include "vnl/vnl_math.h"

namespace vns
{

    // Define the number of neighbors
    template<class TInputImage, class TCoordRep>
        const unsigned long VectorLinearInterpolateImageFunction<TInputImage, TCoordRep>::m_Neighbors = 1
                << TInputImage::ImageDimension;

    // Constructor
    template<class TInputImage, class TCoordRep>
        VectorLinearInterpolateImageFunction<TInputImage, TCoordRep>::VectorLinearInterpolateImageFunction()
        {

        }

    // PrintSelf
    template<class TInputImage, class TCoordRep>
        void
        VectorLinearInterpolateImageFunction<TInputImage, TCoordRep>::PrintSelf(std::ostream& os,
                itk::Indent indent) const
        {
            this->Superclass::PrintSelf(os, indent);
        }

    // Evaluate at image index position
    template<class TInputImage, class TCoordRep>
        typename VectorLinearInterpolateImageFunction<TInputImage, TCoordRep>::OutputType
        VectorLinearInterpolateImageFunction<TInputImage, TCoordRep>::EvaluateAtContinuousIndex(
                const ContinuousIndexType& index) const
        {
            unsigned int dim; // index over dimension


            // Compute base index = closet index below point
            // Compute distance from point to base index
            signed long baseIndex[ImageDimension];
            double distance[ImageDimension];
            long tIndex;

            for (dim = 0; dim < ImageDimension; dim++)
            {
                // The following "if" block is equivalent to the following line without
                // having to call floor.
                if (index[dim] >= 0.0)
                {
                    baseIndex[dim] = static_cast<long> (index[dim]);
                }
                else
                {
                    tIndex = static_cast<long> (index[dim]);
                    if (static_cast<double> (tIndex) != index[dim])
                    {
                        tIndex--;
                    }
                    baseIndex[dim] = tIndex;
                }
                distance[dim] = index[dim] - static_cast<double> (baseIndex[dim]);
            }

            // Interpolated value is the weighted sum of each of the surrounding
            // neighbors. The weight for each neighbor is the fraction overlap
            // of the neighbor pixel with respect to a pixel centered on point.

            const unsigned int size = this->GetInputImage()->GetNumberOfComponentsPerPixel();
            OutputType output;
            output.SetSize(size);
            output.Fill(0.0);

            typedef typename itk::NumericTraits<PixelType>::ScalarRealType ScalarRealType;
            ScalarRealType totalOverlap = itk::NumericTraits<ScalarRealType>::Zero;

            IndexType neighIndex;
            for (unsigned int counter = 0; counter < m_Neighbors; counter++)
            {
            	double overlap(1.); // fraction overlap
            	unsigned int upper = counter; // each bit indicates upper/lower neighbour
                // get neighbor index and overlap fraction
                for (dim = 0; dim < ImageDimension; dim++)
                {

                    if (upper & 1)
                    {
                        neighIndex[dim] = baseIndex[dim] + 1;
                        // Take care of the case where the pixel is just
                        // in the outer upper boundary of the image grid.
                        // MACCS 4.2 ITK_USE_CENTERED_PIXEL_COORDINATES_CONSISTENTLY
                        // Take care of the case where the pixel is just
                        // in the outer upper boundary of the image grid.
                        if (neighIndex[dim] > this->m_EndIndex[dim])
                        {
                            neighIndex[dim] = this->m_EndIndex[dim];
                        }
                        overlap = overlap * distance[dim];
                    }
                    else
                    {
                        neighIndex[dim] = baseIndex[dim];
                        // Take care of the case where the pixel is just
                        // in the outer lower boundary of the image grid.
                        // MACCS 4.2 ITK_USE_CENTERED_PIXEL_COORDINATES_CONSISTENTLY
                        // Take care of the case where the pixel is just
                        // in the outer lower boundary of the image grid.
                        if (neighIndex[dim] < this->m_StartIndex[dim])
                        {
                            neighIndex[dim] = this->m_StartIndex[dim];
                        }
                        overlap = overlap * (1.0 - distance[dim]);
                    }

                    upper >>= 1;

                }

                // Get neighbor value only if overlap is not zero
                if (overlap)
                {
                    const PixelType & input = this->GetInputImage()->GetPixel(neighIndex);
                    for (unsigned int k = 0; k < size; k++)
                    {
                        output[k] = output[k] + (overlap * static_cast<RealType> (input[k]));
                    }
                    totalOverlap = totalOverlap + overlap;
                }

                if (totalOverlap == 1.0)
                {
                    // Finished
                    break;
                }

            }

            return (output);
        }

} // End namespace vns

#endif
