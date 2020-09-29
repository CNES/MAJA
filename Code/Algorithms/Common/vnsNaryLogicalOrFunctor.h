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
 * VERSION : 3.0.0 : DM : LAIG-DM-MAJA-2298-CNES : 27 juillet 2017 : Prise en compte masque extension       *
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id$
 *                                                                                                          *
 ************************************************************************************************************/
#ifndef __vnsNaryLogicalOrFunctor_h
#define __vnsNaryLogicalOrFunctor_h

#include "itkImageToImageFilter.h"

namespace vns
{
    /** \class  NaryLogicalOrFunctor
     * \brief This class applies the logical OR between three images of mask type.
     *
     * If the pixel value of one of three input images is equal to one,
     * the output pixel value is set to one.
     *
     * The input images are otb::Image.
     *
     * \author CS Systemes d'Information
     *
     * \ingroup L2
     *
     */
    namespace Functor
    {
        template<class TInputPixel, class TOutputPixel>
            class NaryLogicalOrFunctor
            {
            public:
                typedef NaryLogicalOrFunctor<TInputPixel, TOutputPixel> NaryLogicalOrFunctorType;

                NaryLogicalOrFunctor()
                {
                }

                virtual
                ~NaryLogicalOrFunctor()
                {
                }

                typedef TInputPixel InputPixelType;
                typedef TOutputPixel OutputPixelType;

                inline OutputPixelType
                operator()(const std::vector< InputPixelType > & B) const
                {
                    OutputPixelType outPix;

                    outPix = static_cast<OutputPixelType>(0);

                    // Test if one of the pixel value of the three input mask is equal to 1
                    for ( unsigned int i = 0; i < B.size(); i++ )
                    {
                    	if (B[i] != static_cast<InputPixelType>(0))
                    	{
                    		// Set output pixel value to 1
                    		outPix = static_cast<OutputPixelType>(1);
                    		break;
                    	}
                    }
                    return outPix;
                }

            };

    } // End namespace functor

} // End namespace vns

#endif /* __vnsNaryLogicalOrFunctor_h */
