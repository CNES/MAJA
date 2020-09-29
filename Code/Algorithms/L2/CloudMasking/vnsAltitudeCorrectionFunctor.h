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
 * VERSION : 5-0-0 : FA : LAIG-FA-MAC-143764-CS : 12 avril 2016 : Mise en forme du code (indentation, etc.) *
 * VERSION : 4-0-0 : DM : LAIG-DM-MAC-517-CNES : 13 mars 2014 : Améliorations algorithmiques                *
 * VERSION : 1-0-0 : <TypeFT> : <NumFT> : 24 juin 2010 : Creation                                                           
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id$
 *                                                                                                          *
 ************************************************************************************************************/
#ifndef __vnsAltitudeCorrectionFunctor_h
#define __vnsAltitudeCorrectionFunctor_h

#include "itkImageToImageFilter.h"

namespace vns
{
    /** \class  AltitudeCorrectionFunctor
     * \brief This class corrects the cloud altitude input mask.
     *
     * A map contains an altitude value for each label of the input mask.
     * This functor sets to the pixel the sum of the input altitude value and the
     * altitude one associated to his label.
     *
     * \author CS Systemes d'Information
     *
     * \sa ImageToImageFilter
     *
     * \ingroup L2
     *
     */
    namespace Functor
    {
        template<class TInputPixel, class TLabelPixel, class TOutputPixel>
            class AltitudeCorrectionFunctor
            {
            public:
                typedef AltitudeCorrectionFunctor<TInputPixel, TLabelPixel, TOutputPixel> AltitudeCorrectionFunctorType;

                AltitudeCorrectionFunctor()
                {
                }

                virtual
                ~AltitudeCorrectionFunctor()
                {
                }

                typedef TInputPixel InputPixelType;
                typedef TLabelPixel LabelPixelType;
                typedef TOutputPixel OutputPixelType;

                typedef typename std::map<LabelPixelType, double> DoubleMapType;

                /** Get the map */
                DoubleMapType
                GetMap()
                {
                    return m_Map;
                }
                /** Set the map */
                void
                SetMap(DoubleMapType map)
                {
                    m_Map = map;
                }

                inline OutputPixelType
                operator()(InputPixelType inPix, LabelPixelType labelPix) const
                {
                    OutputPixelType outPix;

                    // Initialization of the output pixel with the input pixel value
                    // (cloud altitude not corrected)
                    outPix = static_cast<OutputPixelType>(inPix);

                    // Map iterator associated to the input label value
                    typename DoubleMapType::const_iterator it = m_Map.find(labelPix);

                    // If the label exists in the map
                    if (it != m_Map.end())
                    {
                        // Correction of the cloud altitude
                        outPix = static_cast<OutputPixelType>(inPix + (*it).second);
                    }
                    // If the input altitude is lower than the altitude correction
                    if (outPix < 0.)
                    {
                        outPix = static_cast<OutputPixelType>(0);
                    }

                    return outPix;
                }

            protected:
                /** Altitude correction map */
                DoubleMapType m_Map;

            };

    } // End namespace functor

} // End namespace vns

#endif /* __vnsAltitudeCorrectionFunctor_h */
