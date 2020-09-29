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
 * $Id: vnsLowerConditionalThresholdImageFunctor.h abricier $
 *                                                                                                          *
 ************************************************************************************************************/
#ifndef __vnsLowerConditionalThresholdImageFunctor_h
#define __vnsLowerConditionalThresholdImageFunctor_h

#include "itkObject.h"
#include "vnsMath.h"

namespace vns
{
    /** \class  LowerConditionalThresholdImageFunctor
     * \brief This functor sets the output pixels to output value if the associated Input pixels
     * are strictly lower than the maximum threshold value and not no_data.
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
        template<class TInputPixel, class TOutputPixel>
            class LowerConditionalThresholdImageFunctor
            {
            public:
                typedef LowerConditionalThresholdImageFunctor<TInputPixel, TOutputPixel>
                        LowerConditionalThresholdImageFunctorType;

                LowerConditionalThresholdImageFunctor(void)
                {
                    m_MaxThresholdValue = 0;
                    m_NoData = 0;
                    m_OutsideValue = 0;
                    m_InsideValue = 1;
                }

                virtual
                ~LowerConditionalThresholdImageFunctor(void)
                {
                }

                typedef TInputPixel InputPixelType;
                typedef TOutputPixel OutputPixelType;

                /** Get the minimum threshold value */
                InputPixelType
                GetMaximumThresholdValue(void)
                {
                    return m_MaxThresholdValue;
                }
                /** Set the minimum threshold value */
                void
                SetMaximumThresholdValue(InputPixelType val)
                {
                    m_MaxThresholdValue = val;
                }
                /** Get the No Data */
                InputPixelType
                GetNoDataValue(void)
                {
                    return m_NoData;
                }
                /** Set the No Data */
                void
                SetNoDataValue(InputPixelType val)
                {
                    m_NoData = val;
                }
                /** Get the output value set to the output image */
                OutputPixelType
                GetOutsideValue(void)
                {
                    return m_OutsideValue;
                }
                /** Set the default value set to the output image */
                void
                SetOutsideValue(OutputPixelType val)
                {
                    m_OutsideValue = val;
                }

                OutputPixelType
                GetInsideValue(void)
                {
                    return m_InsideValue;
                }
                /** Set the default value set to the output image */
                void
                SetInsideValue(OutputPixelType val)
                {
                    m_InsideValue = val;
                }

                inline OutputPixelType
                operator()(const InputPixelType & inPix) const
                {
                    OutputPixelType outPix;

                    // Test if the pixel value of the mask is greater than the minimum threshold value
                    if (vnsStrictlyInfDoubleMacro(inPix, m_MaxThresholdValue, CONST_EPSILON) && (inPix != m_NoData))
                    {
                        // Set pixel to OutputValue for all bands
                        outPix = static_cast<OutputPixelType>(m_InsideValue);
                        vnsLogSuperDebugMacro("Darkening (" << inPix << ") < threshold (" << m_MaxThresholdValue << ")")
                    }
                    else
                    {
                        outPix = static_cast<OutputPixelType> (m_OutsideValue);
                    }

                    return outPix;
                }

            protected:
                /** Minimum threshold value declaration */
                InputPixelType m_MaxThresholdValue;
                InputPixelType m_NoData;

                /** Output value declaration */
                OutputPixelType m_OutsideValue;
                OutputPixelType m_InsideValue;
            };

    } // end namespace functor

} // End namespace vns

#endif /* __vnsLowerConditionalThresholdImageFunctor_h */
