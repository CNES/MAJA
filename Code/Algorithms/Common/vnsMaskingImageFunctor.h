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
 * VERSION : 2.0.0 : DM : LAIG-DM-MAJA-153436-CS : 13 avril 2017 : Refactoring pour ameliorer la qualite    *
 * VERSION : 5-1-0 : FA : LAIG-FA-MAC-145739-CS : 27 juin 2016 : Audit code - Supp de la macro ITK_EXPORT   *
 * VERSION : 4-0-0 : DM : LAIG-DM-MAC-517-CNES : 28 janvier 2014 : Améliorations algorithmiques (Spec v2.1)	*
 * VERSION : 1-0-0 : <TypeFT> : <NumFT> : 7 sept. 2010 : Creation                                                           
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id$
 *                                                                                                          *
 ************************************************************************************************************/

#ifndef __vnsMaskingImageFilter_h
#define __vnsMaskingImageFilter_h

#include "vnsMath.h"

namespace vns
{
/** \class MaskingImageFilter
 *
 * \brief This class applies a mask to an input image.
 * If the mask pixel value is equal to a "Background" value, the output pixel value is set to the "Replace" value
 * else the output pixel value takes the input pixel value.
 *
 * The two input images are otb::image.
 *
 *
 */
namespace Functor
{
  template< class TInput, class TMask, class TOutput=TInput>
    class MaskingImageFunctor
    {
      public:
      
      MaskingImageFunctor() : m_BackgroundValue(0), m_ReplaceValue(0) {};
      virtual ~MaskingImageFunctor() {};
      
      void SetBackgroundValue(TMask val)
      {
        m_BackgroundValue = val;
      }
      
      void SetReplaceValue(TOutput val)
      {
        m_ReplaceValue = val;
      }
      
     
      inline TOutput operator() ( const TInput & A, const TMask & M ) const
      {
        TOutput outPix;
    
        if( std::abs( static_cast<double>(M) - static_cast<double>(m_BackgroundValue) ) < CONST_EPSILON )
          {
           outPix = m_ReplaceValue;
          }
        else
          {
            outPix = static_cast<TOutput>(A);
          }
        
        return outPix;
      }

      protected:
      TMask m_BackgroundValue;
      TOutput m_ReplaceValue;
    };
}// namespace Functor


} // end namespace vns


#endif

