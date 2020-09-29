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
 * VERSION : 2.0.0 : DM : LAIG-DM-MAJA-153436-CS : 13 avril 2017 : Refactoring de classe doublons           *
 * VERSION : 5-0-0 : DM : LAIG-DM-MAC-982-CNES : 5 avr. 2016 : Creation                                       *
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id:
 *                                                                                                          *
 ************************************************************************************************************/
#ifndef __vnsKeepValueWithMaskFunctor_h
#define __vnsKeepValueWithMaskFunctor_h

namespace vns
{
    /** \class  KeepValueWithMaskFunctor
     * \brief This class sets the input pixel value to default value if the
     * mask pixel value is equal to the excluded value.
     *
     * The input and output pixel types are scalar (otb::Image).
     *
     * \author CS Systemes d'Information
     *
     * \ingroup L2
     *
     */
    namespace Functor
    {
        template<class TInputPixel, class TMaskPixel, class TOutputPixel>
            class KeepValueWithMaskFunctor
            {
            public:
                typedef KeepValueWithMaskFunctor<TInputPixel, TMaskPixel, TOutputPixel> KeepValueWithMaskFunctorType;

                KeepValueWithMaskFunctor()
                {
                }

                virtual
                ~KeepValueWithMaskFunctor()
                {
                }

                typedef TInputPixel InputPixelType;
                typedef TMaskPixel MaskPixelType;
                typedef TOutputPixel OutputPixelType;

                /** Set the excluded value */
                void
                SetKeptValue(MaskPixelType value)
                {
                    m_KeptValue = value;
                }

                /** Set the default value */
                void
                SetDefaultValue(InputPixelType value)
                {
                    m_DefaultValue = value;
                }

                inline OutputPixelType
                operator()(InputPixelType inPix, MaskPixelType maskPix) const
                {
                    OutputPixelType outPix;

                    outPix = static_cast<OutputPixelType>(inPix);

                    // Test if one of the pixel value of the three input mask is equal to 1
                    if (maskPix != m_KeptValue)
                    {
                        // Set output pixel value to default value
                        outPix = static_cast<OutputPixelType>(m_DefaultValue);

                    }
                    return outPix;
                }

            protected:
                /** Excluded value declaration */
                MaskPixelType m_KeptValue;
                /** Default value declaration */
                InputPixelType m_DefaultValue;

            };

    } // End namespace functor

} // End namespace vns

#endif /* __vnsKeepValueWithMaskFunctor_h */
