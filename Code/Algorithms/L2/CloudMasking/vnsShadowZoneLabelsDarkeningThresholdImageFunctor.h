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
 * VERSION : 5-0-0 : DM : LAIG-DM-MAC-982-CNES : 5 avr. 2016 : Creation                                       *
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id:
 *                                                                                                          *
 ************************************************************************************************************/
#ifndef __vnsShadowZoneLabelsDarkeningThresholdImageFunctor_h
#define __vnsShadowZoneLabelsDarkeningThresholdImageFunctor_h

#include "vnsMacro.h"
#include "vnsMath.h"

namespace vns
{
    /** \class  ReflectanceThresholdImageFunctor
     * \brief This class applies a threshold to a reflectance image and generate the associated mask.
     *
     * The threshold value is computed for each pixel
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
        template<class TInputLabelPixel, class TInputDarkeningPixel, class TOutputPixel>
            class ShadowZoneLabelsDarkeningThresholdImageFunctor
            {
            public:
                typedef TInputLabelPixel InputLabelPixelType;
                typedef TInputDarkeningPixel InputDarkeningPixelType;
                typedef TOutputPixel OutputPixelType;

                ShadowZoneLabelsDarkeningThresholdImageFunctor()
                {
                    m_LabelPixelValue = static_cast<InputLabelPixelType>(0);
                    m_DarkeningThresholPixelValue = static_cast<InputDarkeningPixelType>(0);
                    m_OutsidePixelValue = static_cast<OutputPixelType>(0);
                }

                virtual
                ~ShadowZoneLabelsDarkeningThresholdImageFunctor()
                {
                    // nothing to do
                }

                virtual void
                Modified() const
                {
                    // nothing to do
                }

                /** Declare Label pixel test value */
                vnsMemberAndSetAndGetConstReferenceMacro( LabelPixelValue, InputLabelPixelType )

                /** Declare the darkening threshold value */
                vnsMemberAndSetAndGetConstReferenceMacro( DarkeningThresholPixelValue, InputDarkeningPixelType )

                /** Outpise pixel cvalue if test threshold false */
                vnsMemberAndSetAndGetConstReferenceMacro( OutsidePixelValue, OutputPixelType )

                inline OutputPixelType
                operator()(InputLabelPixelType inLabelPix, InputDarkeningPixelType inDarkeningPix) const
                {
                    if ((inLabelPix == m_LabelPixelValue) && vnsStrictlyInfDoubleMacro(inDarkeningPix,m_DarkeningThresholPixelValue,CONST_EPSILON))
                    {
                        return static_cast<OutputPixelType>(inLabelPix);
                    }
                    // Else return the Input pixel (no change)
                    return m_OutsidePixelValue;
                }
            };

    } // end namespace functor

} // end namespace vns

#endif
