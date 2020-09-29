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
 * VERSION : 2.1.0 : DM : LAIG-DM-MAJA-157814-CS : 17 mai 2017 : Refactoring plus de unary                   *
 * VERSION : 5-1-0 : FA : LAIG-FA-MAC-145739-CS : 27 juin 2016 : Audit code - Supp de la macro ITK_EXPORT   *
 * VERSION : 4-0-0 : DM : LAIG-DM-MAC-517-CNES : 20 janvier 2014 : Améliorations algorithmiques (Spec v2.1)	*
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id: vnsImageUpdatingWithMaskImageFilter.h 3545 2011-11-29 14:21:48Z cruffel $
 *                                                                                                          *
 ************************************************************************************************************/

#ifndef __vnsImageUpdatingWithMaskFunctor_h
#define __vnsImageUpdatingWithMaskFunctor_h

#include "itkTernaryFunctorImageFilter.h"

#include "vnsMath.h"

namespace vns
{
/** \class ImageUpdatingWithMaskImageFilter
 *
 * \brief Update an input image 1 with another one for the pixel raised to 1 in the input mask.
 * The input images are otb::image.
 *
 *
 */
namespace Functor
{
  template< class TInputPixel, class TMaskPixel, class TOutputPixel=TInputPixel>
    class ImageUpdatingWithMaskImageFunctor
    {
      public:
      
	  ImageUpdatingWithMaskImageFunctor() {};
      virtual ~ImageUpdatingWithMaskImageFunctor() {};
      
      inline TOutputPixel operator() ( const TInputPixel & inPix, const TMaskPixel & maskPix, const TInputPixel & corrPix ) const
      {
        TOutputPixel outPix(inPix);
    
        // if InPix != RefPix => update inPix in outPix
        if( maskPix > 0)
        {
           outPix = corrPix;
        }
        return outPix;
      }

    };
}// namespace Functor

} // end namespace vns


#endif

