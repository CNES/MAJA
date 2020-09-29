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
 * VERSION : 3.3.0 : FA : LAIG-FA-MAJA-3005-CNES : 15 mars 2019 : Erreur lecture CAMS                       *
 * VERSION : 3.2.0 : DM : LAIG-DM-MAJA-2737-CNES : 07 juin 2018 : CAMS utilises dans AOTEstimation          *
 * VERSION : 3.2.0 : DM : LAIG-DM-MAJA-2737-CNES : 07 juin 2018 : Creation                                  *
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id:
 *                                                                                                          *
 ************************************************************************************************************/
#ifndef __vnsReplaceValueWithImageFunctor_h
#define __vnsReplaceValueWithImageFunctor_h

namespace vns
{
    /** \class  ReplaceValueWithImageFunctor
     * \brief This class sets the input pixel value to other image value if the
     * first image pixel value is equal to the excluded value.
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
        template<class TInputPixel, class TOutputPixel>
            class ReplaceValueWithImageFunctor
            {
            public:
                typedef ReplaceValueWithImageFunctor<TInputPixel, TOutputPixel> ReplaceValueWithImageFunctorType;

                ReplaceValueWithImageFunctor()
                {
                }

                virtual
                ~ReplaceValueWithImageFunctor()
                {
                }

                typedef TInputPixel InputPixelType;
                typedef TOutputPixel OutputPixelType;

                /** Set the replaced value */
                void
                SetReplacedValue(InputPixelType value)
                {
                    m_ReplacedValue = value;
                }

                inline OutputPixelType
                operator()(InputPixelType inPix, InputPixelType otherPix) const
                {
                    OutputPixelType outPix;

                    outPix = static_cast<OutputPixelType>(inPix);

                    // Test if one of the pixel value of the three input mask is equal to 1
                    if (vnsEqualsDoubleMacro(inPix,m_ReplacedValue))
                    {
                        // Set output pixel value to default value
                        outPix = static_cast<OutputPixelType>(otherPix);
                    }
                    return outPix;
                }

            protected:
                /** Excluded value declaration */
                InputPixelType m_ReplacedValue;
            };

    } // End namespace functor

} // End namespace vns

#endif /* __vnsKeepValueWithMaskFunctor_h */
