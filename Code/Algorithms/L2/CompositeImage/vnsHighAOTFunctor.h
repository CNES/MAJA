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
 * VERSION : 1-0-0 : <TypeFT> : <NumFT> : 27 août 2010 : Creation                                                           
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id$
 *                                                                                                          *
 ************************************************************************************************************/
#ifndef __vnsHighAOTFunctor_txx
#define __vnsHighAOTFunctor_txx

namespace vns
{
    /** \class  HighAOTFunctor
      * \brief This class generates the mask of high aerosol optical thickness.
      *
      * It tests if the value of the input pixel is greater than a threshold
      * value (HighAOTThresholdValue).
      * If it is true, this functor sets the value of the output pixel to m_OutputValue,
      * else it sets the value of the output pixel to m_InsideValue.
      * The input and output image type is otb::Image.
      *
      * \author CS Systemes d'Information
      *
      * \ingroup L2
      *
      */
    namespace Functor
    {
        template<class TInputPixel, class TMaskPixel, class TOutputPixel>
            class HighAOTFunctor
            {
            public:
                typedef HighAOTFunctor<TInputPixel, TMaskPixel, TOutputPixel> HighAOTFunctorType;

                HighAOTFunctor(void) :
                    m_HighAOTThresholdValue(0.), m_InsideValue(0), m_OutputValue(0), m_OutputSize(1)
                {
                }

                virtual
                ~HighAOTFunctor(void)
                {
                }

                typedef TInputPixel InputPixelType;
                typedef TMaskPixel  MaskPixelType;
                typedef TOutputPixel OutputPixelType;

                /** Get AOT threshold value */
                InputPixelType
                GetHighAOTThresholdValue(void)
                {
                    return m_HighAOTThresholdValue;
                }
                /** Set AOT threshold value */
                void
                SetHighAOTThresholdValue(InputPixelType val)
                {
                    m_HighAOTThresholdValue = val;
                }

                /** Get the inside value set to the output mask*/
                OutputPixelType
                GetInsideValue(void)
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
                GetOutputValue(void)
                {
                    return m_OutputValue;
                }
                /** Set the output value set to the output mask */
                void
                SetOutputValue(OutputPixelType val)
                {
                    m_OutputValue = val;
                }

                /** Get the output size of the output mask*/
                unsigned int
                GetOutputSize(void)
                {
                    return m_OutputSize;
                }
                /** Set the output size of the output mask */
                void
                SetOutputSize(unsigned int val)
                {
                    m_OutputSize = val;
                }

                inline OutputPixelType
                operator()(const InputPixelType &inPix, const MaskPixelType &maskPix) const
                {
                    OutputPixelType outPix;

                    // Set the outside value to the pixel of the output mask
                    outPix = m_OutputValue;

                    if ( maskPix == 0 )
                    {
                        if ( inPix > m_HighAOTThresholdValue )
                        {
                            outPix = m_InsideValue;
                        }
                    }

                    return outPix;
                }

            protected:
                /** High AOT Threshold value */
                InputPixelType m_HighAOTThresholdValue;
                /** Mask inside value declaration */
                OutputPixelType m_InsideValue;
                /** Mask output value declaration */
                OutputPixelType m_OutputValue;
                /** Output image dimension declaration */
                unsigned int m_OutputSize;

            };

    } // end namespace functor

} // End namespace vns


#endif /* __vnsHighAOTFunctor_txx */
