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
 * VERSION : 2.1.0 : DM : LAIG-DM-MAJA-157814-CS : 17 mai 2017 : Refactoring plus de unary                  *
 * VERSION : 5-0-0 : FA : LAIG-FA-MAC-143764-CS : 12 avril 2016 : Mise en forme du code (indentation, etc.) *
 * VERSION : 1-0-0 : <TypeFT> : <NumFT> : 31 mai 2010 : Creation                                                           
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id$
 *                                                                                                          *
 ************************************************************************************************************/
#ifndef __vnsTernaryLogicalOrFunctor_h
#define __vnsTernaryLogicalOrFunctor_h

#include "itkImageToImageFilter.h"

namespace vns
{
    /** \class  TernaryLogicalOrFunctor
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
        template<class TInputPixel1, class TInputPixel2, class TInputPixel3, class TOutputPixel>
            class TernaryLogicalOrFunctor
            {
            public:
                typedef TernaryLogicalOrFunctor<TInputPixel1, TInputPixel2, TInputPixel3, TOutputPixel> TernaryLogicalOrFunctorType;

                TernaryLogicalOrFunctor()
                {
                }

                virtual
                ~TernaryLogicalOrFunctor()
                {
                }

                typedef TInputPixel1 InputPixelType1;
                typedef TInputPixel2 InputPixelType2;
                typedef TInputPixel3 InputPixelType3;
                typedef TOutputPixel OutputPixelType;

                inline OutputPixelType
                operator()(InputPixelType1 inPix1, InputPixelType2 inPix2, InputPixelType3 inPix3) const
                {
                    OutputPixelType outPix;

                    outPix = static_cast<OutputPixelType>(0);

                    // Test if one of the pixel value of the three input mask is equal to 1
                    if ((inPix1 != static_cast<InputPixelType1>(0)) || (inPix2 != static_cast<InputPixelType2>(0))
                            || (inPix3 != static_cast<InputPixelType3>(0)))
                    {
                        // Set output pixel value to 1
                        outPix = static_cast<OutputPixelType>(1);

                    }
                    return outPix;
                }

            };

    } // End namespace functor

} // End namespace vns

#endif /* __vnsTernaryLogicalOrFunctor_h */
