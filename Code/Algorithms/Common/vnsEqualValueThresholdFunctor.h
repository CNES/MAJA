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
 * HISTORIQUE                                                                                               *
 *                                                                                                          *
 * VERSION : 4-0-0 : FA : LAIG-FA-MAC-117040-CS : 13 mars 2014 : Modifications mineures                     *
 * VERSION : 2-0-0 : DM : LAIG-DM-MAC-146-CNES : 13 oct. 2011 : Creation du functor pour MACCS                   *
 *                                                                                                          *
 * MOD : VERSION : 1-0-0  : 18 nov. 2011 : Creation                                                           *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id$
 *                                                                                                          *
 ************************************************************************************************************/
#ifndef __vnsEqualValueThresholdFunctor_h
#define __vnsEqualValueThresholdFunctor_h

#include "vnsMacro.h"
#include "vnsMath.h"

namespace vns
{
    /** \class  EqualValueThresholdFunctor
     * \brief This class generates a mask thresholding the input image.
     *
     * It tests if the value of the input pixel is equal to a threshold value.
     * If it is true, this functor sets the value of the output pixel to m_EqualValue,
     * else it sets the value of the output pixel to m_OutsideValue.
     *
     * The pixel type is an otb::Image::PixelType.
     *
     * \author CS Systemes d'Information
     *
     * \ingroup L2
     *
     */
    namespace Functor
    {
        template<class TInputPixel, class TOutputPixel>
            class EqualValueThresholdFunctor
            {
            public:
                typedef EqualValueThresholdFunctor<TInputPixel, TOutputPixel> EqualValueThresholdFunctorType;

                EqualValueThresholdFunctor(void)
                {
                    m_ThresholdValue = 0;
                    m_EqualValue = 0;
                    m_OutsideValue = 1;
                    m_OutputSize = 1;
                }

                ~EqualValueThresholdFunctor(void)
                {
                }

                typedef TInputPixel InputPixelType;
                typedef TOutputPixel OutputPixelType;

                /** Get the threshold value */
                InputPixelType
                GetThresholdValue(void)
                {
                    return static_cast<InputPixelType>(m_ThresholdValue);
                }
                /** Set the threshold value */
                void
                SetThresholdValue(const InputPixelType &val)
                {
                    m_ThresholdValue = static_cast<double>(val);
                }

                /** Get the equal value set to the output mask*/
                OutputPixelType
                GetEqualValue(void)
                {
                    return m_EqualValue;
                }
                /** Set the equal value set to the output mask */
                void
                SetEqualValue(OutputPixelType val)
                {
                    m_EqualValue = val;
                }

                /** Get the outside value set to the output mask*/
                OutputPixelType
                GetOutsideValue()
                {
                    return m_OutsideValue;
                }
                /** Set the Outside value set to the output mask */
                void
                SetOutsideValue(OutputPixelType val)
                {
                    m_OutsideValue = val;
                }

                /** Get the output size of the output mask */
                unsigned int
                GetOutputSize(void)
                {
                    return m_OutputSize;
                }

                inline OutputPixelType
                operator()(InputPixelType inPix) const
                {
                    if (vnsEqualsDoubleMacro( m_ThresholdValue, static_cast<double>(inPix) ))
                    {
                        return m_EqualValue;
                    }

                    return m_OutsideValue;
                }

            protected:
                /** Threshold value declaration */
                double m_ThresholdValue;
                /** Mask equal value declaration */
                OutputPixelType m_EqualValue;
                /** Mask output value declaration */
                OutputPixelType m_OutsideValue;
                /** Output image dimension declaration
                 * Fixed at one. No set accessor. Keep this Get to be able to call it with an
                 * otbFunctorImageFilter and thus generate a VectorImage with only  band.
                 */
                unsigned int m_OutputSize;

            };

    } // end namespace functor

} // End namespace vns

#endif /* __vnsEqualThresholdFunctor_h */
