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
 * VERSION : 2-0-0 : DM : LAIG-DM-MAC-146-CNES : 29 fevrier 2012 : Creation du functor pour MACCS                *
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id$
 *                                                                                                          *
 ************************************************************************************************************/
#ifndef __vnsOrVectorToScalarFunctor_h
#define __vnsOrVectorToScalarFunctor_h

namespace vns
{
    namespace Functor
    {
        /** \class OrVectorToScalarFunctor
         * \brief Implement the Or logical operator pixel-wise between each plan of a vector image.
         *
         */
        template<class TInputPixel, class TOutputPixel>
            class OrVectorToScalarFunctor
            {
            public:
                /** Constructor */
                OrVectorToScalarFunctor()
                {
                }

                /** Destructor */
                ~OrVectorToScalarFunctor()
                {
                }
                bool
                operator !=(const OrVectorToScalarFunctor&) const
                {
                    return false;
                }
                bool
                operator ==(const OrVectorToScalarFunctor&) const
                {
                    return true;
                }
                inline TOutputPixel
                operator ()(const TInputPixel& inPixel) const
                {
                    TOutputPixel outPixel;
                    const unsigned int l_NbInputBand = inPixel.Size();
                    outPixel = inPixel[0];
                    for (unsigned int i = 1; i < l_NbInputBand; i++)
                    {
                        outPixel = static_cast<TOutputPixel> (outPixel | inPixel[i]);
                    }
                    return outPixel;
                }
            };

     } // end namespace functor

 } // End namespace vns

 #endif /* __vnsOrVectorToScalarFunctor_h */
