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
 * VERSION : 3.2.2 : FA : LAIG-FA-MAJA-2956-CNES : 13 decembre 2018 : Gestion masques DEFTOO mal formatés   *
 * VERSION : 2.1.0 : DM : LAIG-DM-MAJA-157814-CS : 17 mai 2017 : Refactoring plus de unary                   *
 * VERSION : 1.0.0 : FA : LAIG-FA-MAC-148399-CS : 03 novembre 2016 : Audit code - Correction qualite        *
 * VERSION : 5-1-0 : FA : LAIG-FA-MAC-145739-CS : 27 juin 2016 : Audit code - Supp de la macro ITK_EXPORT   *
 * VERSION : 4-0-0 : FA : LAIG-FA-MAC-120653-CS : 7 mai 2014 : Correction de règles qualité ou commentaires *
 * VERSION : 1-0-0 : <TypeFT> : <NumFT> : 24 sept. 2010 : Creation                                                           
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id$
 *                                                                                                          *
 ************************************************************************************************************/
#ifndef __vnsChangeValueFunctor_h
#define __vnsChangeValueFunctor_h

#include "itkSimpleDataObjectDecorator.h"
#include "vnsMath.h"

namespace vns
{

    /** \class ChangeValueFunctor
     *
     * \brief Change pixel value in vector image.
     *
     *
     *
     */

    namespace Functor
    {

        template<class TInput, class TOutput>
            class ChangeValue
            {
            public:
                //typedef typename TOutput::ValueType ValueType;

                ChangeValue()
                {
                }

                virtual
                ~ChangeValue()
                {
                }


                typedef std::vector<TInput> InputValueVectorType;
                typedef std::vector<TOutput> OutputValueVectorType;
                typedef typename std::vector<TInput>::iterator InputValueVectorIteratorType;
                typedef typename std::vector<TOutput>::iterator OutputValueVectorIteratorType;


                bool
                operator!=(const ChangeValue & other) const
                {
                    if (m_ChangeValue != other.m_ChangeValue)
                    {
                        return true;
                    }
                    return false;
                }
                bool
                operator==(const ChangeValue & other) const
                {
                    return !(*this != other);
                }
                OutputValueVectorType
                GetChangeValueVector()
                {
                    return m_ChangeValue;
                }
                InputValueVectorType
                GetInputValueVector()
                {
                    return m_InputValue;
                }
                void
                SetChange(const TInput & original, const TOutput & result)
                {
                    InputValueVectorType  current = this->GetInputValueVector();
                    bool hasChange=false;
                    for(unsigned int i=0; i<current.size();i++)
                    {
                        if ( current[i] == original )
                        {
                            hasChange=true;
                        }
                    }
                    if( hasChange==false)
                    {
                    	 m_InputValue.push_back(original);
                    	 m_ChangeValue.push_back(result);
                    }
                }

                void
                SetChangeVector(const InputValueVectorType & InputValue, const OutputValueVectorType & ChangeValue)
                {
                    if(InputValue.size()==ChangeValue.size())
                    {
                        m_InputValue = InputValue;
                        m_ChangeValue = ChangeValue;
                    }
                }

                void
                ClearChange()
                {
                    m_InputValue.clear();
                    m_ChangeValue.clear();
                }

                inline TOutput
                operator()(const TInput & A)
                {
                    TOutput out(A);
                    InputValueVectorIteratorType inputIt;
                    OutputValueVectorIteratorType changeIt;

                    // Loops on the Input and Change values
                    for(inputIt=m_InputValue.begin() , changeIt=m_ChangeValue.begin();
                            inputIt != m_InputValue.end() && changeIt != m_ChangeValue.end();
                            ++inputIt, ++changeIt)
                    {
                        //if A is at NoData, the pixel is changed.
                        if(vnsIsNoDataMacro(A,*inputIt)==true)
                        {
                            out = *changeIt;
                        }
                    }
                    return out;
                }

            private:
                InputValueVectorType m_InputValue;
                OutputValueVectorType m_ChangeValue;
            };
    }
} // end namespace vns

#endif
