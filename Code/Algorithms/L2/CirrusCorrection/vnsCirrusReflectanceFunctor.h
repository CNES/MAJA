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
 * VERSION : 2.0.0 : DM : LAIG-DM-MAJA-2149-CNES : 24 fevrier 2017 : Correction Cirrus - création           *
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id$
 *                                                                                                          *
 ************************************************************************************************************/
#ifndef __vnsCirrusReflectanceFunctor_h
#define __vnsCirrusReflectanceFunctor_h

#include "vnsMath.h"
#include "itkMacro.h"

namespace vns
{
    namespace Functor
    {
        /** \class CirrusReflectanceFunctor
         * \brief Implement the cirrusReflectance image computation from the cirrus image and DTM
         *
         */
        template<class TInputPixelDTM,class TInputPixelCirrus, class TOutputPixel>
        class CirrusReflectanceFunctor
        {
        public:
            /** Constructor */
            CirrusReflectanceFunctor() : m_CirrusMaskThresholdGain(0.0),m_CirrusMaskThresholdOffset(0),
            m_CirrusThresholdFactor(0)
        {
        }

            /** Destructor */
            virtual ~CirrusReflectanceFunctor()
            {
            }

            inline TOutputPixel
            operator ()(const TInputPixelDTM& inDTMPixel, const TInputPixelCirrus& inCirrusPixel) const
            {
                double val = static_cast<double>(inCirrusPixel) -
                        m_CirrusThresholdFactor*(m_CirrusMaskThresholdGain*static_cast<double>(inDTMPixel)+m_CirrusMaskThresholdOffset);
                if (vnsStrictlyInfDoubleMacro(val,0.0, CONST_EPSILON))
                {
                    val = 0.0;
                }
                return static_cast<TOutputPixel>(val);
            }

            void SetMaskThresholdGain(double val)
            {
                m_CirrusMaskThresholdGain = val;
            }
            itkGetMacro(CirrusMaskThresholdGain, double);
            void SetMaskThresholdOffset(double val)
            {
                m_CirrusMaskThresholdOffset = val;
            }
            itkGetMacro(CirrusMaskThresholdOffset, double);
            void SetCirrusThresholdFactor(double val)
            {
                m_CirrusThresholdFactor = val;
            }
            itkGetMacro(CirrusThresholdFactor, double);


        protected:
            double m_CirrusMaskThresholdGain;
            double m_CirrusMaskThresholdOffset;
            double m_CirrusThresholdFactor;
        };

    } // end namespace functor

} // End namespace vns

#endif /* __vnsCirrusReflectanceFunctor_h */
