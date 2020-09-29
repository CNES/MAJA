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
 * VERSION : 2.1.0 : DM : LAIG-DM-MAJA-157814-CS : 18 mai 2017 : Refactoring, menage pour ameliorer qualite *
 * VERSION : 1.0.0 : FA : LAIG-FA-MAC-148399-CS : 03 novembre 2016 : Audit code - Correction qualite        *
 * VERSION : 5-1-0 : FA : LAIG-FA-MAC-145739-CS : 27 juin 2016 : Audit code - Supp de la macro ITK_EXPORT   *
 * VERSION : 4-0-0 : FA : LAIG-FA-MAC-120653-CS : 7 mai 2014 : Correction de règles qualité ou commentaires *
 * VERSION : 1-0-0 : <TypeFT> : <NumFT> : 1 sept. 2010 : Creation
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id$
 *                                                                                                          *
 ************************************************************************************************************/
#ifndef __vnsChangeValueWithMaskVectorImageFilter_h
#define __vnsChangeValueWithMaskVectorImageFilter_h

#include "itkBinaryFunctorImageFilter.h"
#include "itkConceptChecking.h"
#include "itkSimpleDataObjectDecorator.h"
#include "vnsMacro.h"

#include <map>

namespace vns
{

    /** \class ChangeValueWithMaskVectorImageFilter
     *
     * \brief Change pixel value in vector image. The new pixel value is set by OutputChangeValue.
     *
     *
     *
     */

    namespace Functor
    {

        template<class TInput, class TMask, class TOutput>
            class ChangeValueWithMaskVectorFunctor
            {
            public:
                typedef typename TOutput::ValueType ValueType;

                ChangeValueWithMaskVectorFunctor()
                {
                }

                virtual
                ~ChangeValueWithMaskVectorFunctor()
                {
                }


                TOutput
                GetChange(void)
                {
                    return m_OutputChangeValue;
                }

                void
                SetChange(const TOutput & value)
                {
                    m_OutputChangeValue = value;
                }

                TMask
                GetBackgroundMaskValue(void)
                {
                    return m_BackgroundMaskValue;
                }

                void
                SetBackgroundMaskValue(const TMask & value)
                {
                    m_BackgroundMaskValue = value;
                }

                inline TOutput
                operator()(const TInput & A, const TMask & M)
                {
                    // If the mask is Background, set the new output value
                    if (M != m_BackgroundMaskValue)
                    {
                        return m_OutputChangeValue;
                    }
                    else
                    {
                        const unsigned int lSize = A.Size();
                        // Initialize the size of the output pixel
                        TOutput out;
                        out.SetSize(lSize);
                        // Copy the input pixel to the output
                        for (unsigned int i = 0; i < lSize; i++)
                        {
                            out[i] = static_cast<ValueType>(A[i]);
                        }
                        return out;
                    }
                }

            private:
                TMask m_BackgroundMaskValue;
                TOutput m_OutputChangeValue;
            };
    }
} // end namespace vns

#endif

