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
 * VERSION : 4-0-0 : FA : LAIG-FA-MAC-120653-CS : 3 avril 2014 : Correction de règles de qualité			*
 * VERSION : 1-0-0 : <TypeFT> : <NumFT> : 23 avr. 2010 : Creation                                                           
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id$
 *                                                                                                          *
 ************************************************************************************************************/
#ifndef __vnsPressureFunctor_h
#define __vnsPressureFunctor_h

namespace vns
{
    /** \class  PressureFunctor
     * \brief This class is used to estimate the pressure as a function of the altitude.
     *
     * The formula is:
     *  \f$ 1013.25.\left ( 1-\frac{0.0065 * inPix}{288.15} \right ) ^{5.31} \f$
     *
     * where \f$ inPix \f$ is an elevation information value
     *
     * \author CS Systemes d'Information
     *
     * \ingroup L2
     *
     */
    namespace Functor
    {
        template<class TInputPixel, class TOutputPixel>
            class PressureFunctor
            {
            public:
                typedef PressureFunctor<TInputPixel, TOutputPixel> PressureFunctorType;
                typedef TInputPixel InputPixelType;
                typedef TOutputPixel OutputPixelType;

                PressureFunctor()
                {
                    m_PixCoef = 0.0065 / 288.15;
                }

                virtual
                ~PressureFunctor()
                {
                }

                inline OutputPixelType
                operator()(const InputPixelType & inPix) const
                {
                    double val = 1013.25 * vcl_pow((1 - m_PixCoef * static_cast<double> (inPix)), 5.31);

                    return static_cast<OutputPixelType> (val);
                }

            protected:
                /** Pixel coefficient to apply \f$ =\frac{0.0065}{288.15} \f$ */
                double m_PixCoef;

            };

    } // end namespace functor

} // End namespace vns

#endif /* __vnsPressureFunctor_h */
