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
 * VERSION : 4-0-0 : DM : LAIG-DM-MAC-517-CNES : 27 jan. 2014 : Mise en commun du calcul de l'angle         *
 *                                                              d'incidence solaire                         *
 * VERSION : 1-0-0 : <TypeFT> : <NumFT> : 23 avr. 2010 : Creation                                                           
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id: vnsIncidenceAngleFunctor.h 3636 2011-12-13 17:44:04Z cruffel $
 *                                                                                                          *
 ************************************************************************************************************/
#ifndef __vnsIncidenceAngleFunctor_h
#define __vnsIncidenceAngleFunctor_h


#include "vnsMacro.h"
#include "vnsMath.h"


namespace vns
{
    /** \class  IncidenceAngleFunctor
     * \brief This class computes incidence angle from slope and aspect images.
     * Computed values are not clamped!
     *
     * \author CS Systemes d'Information
     *
     * \ingroup L2
     *
     */
    namespace Functor
    {
        template<class TInputAspectPixel, class TInputSlopePixel, class TOutputPixel>
            class IncidenceAngleFunctor
            {
            public:
                typedef TInputAspectPixel InputAspectPixelType;
                typedef TInputSlopePixel InputSlopePixelType;
                typedef TOutputPixel OutputPixelType;
                typedef IncidenceAngleFunctor<InputAspectPixelType, InputSlopePixelType, OutputPixelType> IncidenceAngleFunctorType;

                IncidenceAngleFunctor() : m_PhiS(0), m_CosThetaS(0), m_SinThetaS(0)
                {
                }

                virtual ~IncidenceAngleFunctor()
                {
                    // nothing to do
                }

                virtual void SetPhiS( double PhiSlope )
                {
                    m_PhiS = PhiSlope;
                }

                virtual void SetThetaS( double ThetaSlope )
                {
                    m_CosThetaS = std::cos( ThetaSlope );
                    m_SinThetaS = std::sin( ThetaSlope );
                }


                inline OutputPixelType
                operator()( TInputAspectPixel inAspect, TInputAspectPixel inSlope ) const
                {
                    const double dAspect = double(inAspect);
                    const double dSlope  = double(inSlope);
                    OutputPixelType lIncidenceAngle = m_CosThetaS * std::cos( dSlope );
                    lIncidenceAngle = lIncidenceAngle + m_SinThetaS * std::sin( dSlope ) * std::cos( m_PhiS - dAspect );
                    return lIncidenceAngle;
                }

            protected:
                double m_PhiS;
                double m_CosThetaS;
                double m_SinThetaS;

            };

    } // end namespace functor

} // End namespace vns

#endif /* __vnsIncidenceAngleFunctor_h */
