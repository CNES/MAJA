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
 * VERSION : 2.1.0 : DM : LAIG-DM-MAJA-157814-CS : 18 mai 2017 : Refactoring, menage pour ameliorer qualite *
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id$
 *                                                                                                          *
 ************************************************************************************************************/
#ifndef __vnsGreaterConditionalThresholdImageFunctor_h
#define __vnsGreaterConditionalThresholdImageFunctor_h

#include "itkObject.h"

namespace vns
{
    /** \class  GreaterConditionalThresholdImageFunctor
     * \brief This functor sets the output pixels to output value if the associated mask pixels
     * are strictly greater than the minimum threshold value.
     *
     * Inputs are a vector image (otb::VectorImage) and the associated mask (otb::Image) and the
     * two parameters m_MinimumThresholdValue and m_OutputValue.
     * The output image is also a vector image.
     *
     * \author CS Systemes d'Information
     *
     * \sa Object
     *
     * \ingroup L2
     *
     */
    namespace Functor
    {
        template<class TInputPixel, class TMaskPixel, class TOutputPixel>
            class GreaterConditionalThresholdImageFunctor
            {
            public:
                typedef GreaterConditionalThresholdImageFunctor<TInputPixel, TMaskPixel, TOutputPixel>
                        GreaterConditionalThresholdImageFunctorType;

                GreaterConditionalThresholdImageFunctor(void)
                {
                    m_MinimumThresholdValue = 0;
                    m_OutputValue = 0;
                }

                virtual
                ~GreaterConditionalThresholdImageFunctor(void)
                {
                }

                typedef TInputPixel InputPixelType;
                typedef TMaskPixel MaskPixelType;
                typedef TOutputPixel OutputPixelType;
                typedef typename OutputPixelType::ValueType OutputInternalPixelType;

                /** Get the minimum threshold value of the mask */
                MaskPixelType
                GetMinimumThresholdValue(void)
                {
                    return m_MinimumThresholdValue;
                }
                /** Set the minimum threshold value of the mask */
                void
                SetMinimumThresholdValue(MaskPixelType val)
                {
                    m_MinimumThresholdValue = val;
                }
                /** Get the output value set to the output image */
                OutputInternalPixelType
                GetOutputValue(void)
                {
                    return m_OutputValue;
                }
                /** Set the default value set to the output image */
                void
                SetOutputValue(OutputInternalPixelType val)
                {
                    m_OutputValue = val;
                }

                inline OutputPixelType
                operator()(const InputPixelType & inPix, const MaskPixelType &maskPix) const
                {
                    OutputPixelType outPix;
                    const unsigned int l_size = inPix.Size();
                    // Set output pixel dimension
                    outPix.SetSize(l_size);

                    // Test if the pixel value of the mask is greater than the minimum threshold value
                    if (maskPix > m_MinimumThresholdValue)
                    {
                        // Set pixel to OutputValue for all bands
                        outPix.Fill(m_OutputValue);

                    }
                    else
                    {
                        // Band Loop
                        for (unsigned int i = 0; i < l_size; i++)
                        {
                            // Keep the input pixel value
                            outPix[i] = static_cast<OutputInternalPixelType> (inPix[i]);
                        }
                    }

                    return outPix;
                }

            protected:
                /** Minimum threshold value declaration */
                MaskPixelType m_MinimumThresholdValue;
                /** Output value declaration */
                OutputInternalPixelType m_OutputValue;

            };

    } // end namespace functor

} // End namespace vns

#endif /* __vnsGreaterConditionalThresholdImageFunctor_h */
