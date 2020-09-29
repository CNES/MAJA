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
 * VERSION : 2.1.0 : DM : LAIG-DM-MAJA-157814-CS : 18 mai 2017 : Refactoring plus de unary                   *
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id$
 *                                                                                                          *
 ************************************************************************************************************/
#ifndef __vnsChangeValueWithMaskFunctor_h
#define __vnsChangeValueWithMaskFunctor_h

#include "vnsMath.h"

namespace vns
{
    /** \class  ChangeValueWithMaskFunctor
     * \brief This class applies the input mask to an image.
     *
     * It tests if the pixel value of the mask is equal to m_BackgroundValue.
     * If it is true, this functor sets the value of the output pixel to m_DefaultValue for all bands
     * else the value of the output pixel keeps the value of the input pixel.
     *
     * \author CS Systemes d'Information
     *
     * \ingroup L2
     *
     */
    namespace Functor
    {
        template<class TInputPixel, class TMaskPixel, class TOutputPixel>
            class ChangeValueWithMaskFunctor
            {
            public:
                typedef ChangeValueWithMaskFunctor<TInputPixel, TMaskPixel,
                        TOutputPixel> ChangeValueWithMaskFunctorType;

                ChangeValueWithMaskFunctor()
                {
                    m_BackgroundValue = 0;
                    m_DefaultValue = 0;
                }

                virtual ~ChangeValueWithMaskFunctor(){}

                typedef TInputPixel     InputPixelType;
                typedef TMaskPixel      MaskPixelType;
                typedef TOutputPixel    OutputPixelType;


                /** Get the background value of the mask */
                MaskPixelType
                GetBackgroundValue()
                {
                    return m_BackgroundValue;
                }
                /** Set the background value of the mask */
                void
                SetBackgroundValue(MaskPixelType val)
                {
                    m_BackgroundValue = val;
                }
                /** Get the default value set to the output image */
                OutputPixelType
                GetDefaultValue()
                {
                    return m_DefaultValue;
                }
                /** Set the default value set to the output image */
                void
                SetDefaultValue(OutputPixelType val)
                {
                    m_DefaultValue = val;
                }

                inline OutputPixelType
                operator()(InputPixelType inPix, MaskPixelType maskPix) const
                {
                    OutputPixelType outPix;

                    // Test if the pixel value of the mask is equal to BackgroundValue
                    if (maskPix == m_BackgroundValue)
                    {
                        // Set pixel to DefaultValue for all bands
                        outPix = static_cast<OutputPixelType> (m_DefaultValue);
                    }
                    else
                    {
                        // Keep the input pixel value
                        outPix = static_cast<OutputPixelType> (inPix);
                    }

                    return outPix;
                }

            protected:
                /** Mask background value declaration */
                MaskPixelType m_BackgroundValue;
                /** Default value declaration */
                OutputPixelType m_DefaultValue;

            };

    } // End namespace functor

} // End namespace vns

#endif /* __vnsChangeValueWithMaskFunctor_h */
