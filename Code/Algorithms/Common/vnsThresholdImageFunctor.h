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
 * VERSION : 1-0-0 : <TypeFT> : <NumFT> : 23 avr. 2010 : Creation                                                           
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id$
 *                                                                                                          *
 ************************************************************************************************************/
#ifndef __vnsThresholdImageFunctor_h
#define __vnsThresholdImageFunctor_h

namespace vns
{
    /** \class  ThresholdImageFunctor
     * \brief This class applies a threshold to an image and generate the associated mask.
     *
     * It tests if the value of the input pixel is greater than a threshold value.
     * If it is true, this functor sets the value of the output pixel to m_OutputValue,
     * else it sets the value of the output pixel to m_InsideValue.
     * The input image and the output mask are otb::Image.
     *
     * \author CS Systemes d'Information
     *
     * \ingroup L2
     *
     */
    namespace Functor
    {
        template<class TInputPixel, class TOutputPixel>
            class ThresholdImageFunctor
            {
            public:
                typedef ThresholdImageFunctor<TInputPixel, TOutputPixel>
                        ThresholdImageFunctorType;

                ThresholdImageFunctor()
                {
                    m_ThresholdValue = static_cast<InputPixelType>(0);
                    m_InsideValue = static_cast<OutputPixelType>(0);
                    m_OutputValue = static_cast<OutputPixelType>(1);
                }


                virtual ~ThresholdImageFunctor()
                {
                }


                typedef TInputPixel InputPixelType;
                typedef TOutputPixel OutputPixelType;

                /** Get the threshold value */
                InputPixelType
                GetThresholdValue()
                {
                    return m_ThresholdValue;
                }
                /** Set the threshold value */
                void
                SetThresholdValue(InputPixelType val)
                {
                    m_ThresholdValue = val;
                }

                /** Get the inside value set to the output mask*/
                OutputPixelType
                GetInsideValue()
                {
                    return m_InsideValue;
                }
                /** Set the inside value set to the output mask */
                void
                SetInsideValue(OutputPixelType val)
                {
                    m_InsideValue = val;
                }

                /** Get the output value set to the output mask*/
                OutputPixelType
                GetOutputValue()
                {
                    return m_OutputValue;
                }
                /** Set the output value set to the output mask */
                void
                SetOutputValue(OutputPixelType val)
                {
                    m_OutputValue = val;
                }

                inline OutputPixelType
                operator()(InputPixelType inPix) const
                {
                    OutputPixelType outPix;

                    // Set the inside value to the pixel of the output mask
                    outPix = m_InsideValue;

                    if (inPix > m_ThresholdValue)
                    {
                        outPix = m_OutputValue;
                    }

                    return outPix;
                }

            protected:
                /** Threshold value declaration */
                InputPixelType m_ThresholdValue;
                /** Mask inside value declaration */
                OutputPixelType m_InsideValue;
                /** Mask output value declaration */
                OutputPixelType m_OutputValue;
                /** Output image dimension declaration */

            };

    } // end namespace functor

} // End namespace vns

#endif /* __vnsThresholdImageFunctor_h */
