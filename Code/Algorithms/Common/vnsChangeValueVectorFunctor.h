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
 * VERSION : 1.0.0 : FA : LAIG-FA-MAC-148399-CS : 03 novembre 2016 : Audit code - Correction qualite        *
 * VERSION : 1.0.0 : FA : LAIG-FA-MAC-1990-CNES : 13 octobre 2016 : Correction qualite (TO DO)              *
 * VERSION : 5-1-0 : FA : LAIG-FA-MAC-145739-CS : 27 juin 2016 : Audit code - Supp de la macro ITK_EXPORT   *
 * VERSION : 4-0-0 : FA : LAIG-FA-MAC-120653-CS : 7 mai 2014 : Correction de règles qualité ou commentaires *
 * VERSION : 1-0-0 : <TypeFT> : <NumFT> : 1 sept. 2010 : Creation
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id$
 *                                                                                                          *
 ************************************************************************************************************/
#ifndef __vnsChangeValueVectorFunctor_h
#define __vnsChangeValueVectorFunctor_h

#include "itkUnaryFunctorImageFilter.h"
#include "itkConceptChecking.h"
#include "itkSimpleDataObjectDecorator.h"
#include "vnsMath.h"

namespace vns
{

    /** \class ChangeValueVectorImageFilter
     *
     * \brief Change pixel value in vector image.
     *
     *
     *
     */

    namespace Functor
    {

        template<class TInput, class TOutput>
            class VectorChangeValue
            {
            public:
                typedef typename TOutput::ValueType ValueType;

                VectorChangeValue() :
                        m_NumberOfComponentsPerPixel(0)
                {
                }

                virtual
                ~VectorChangeValue()
                {
                }

                typedef std::vector<TInput> InputValueVectorType;
                typedef std::vector<TOutput> OutputValueVectorType;
                typedef typename std::vector<TInput>::iterator InputValueVectorIteratorType;
                typedef typename std::vector<TOutput>::iterator OutputValueVectorIteratorType;

                void
                SetNumberOfComponentsPerPixel(unsigned int nb)
                {
                    m_NumberOfComponentsPerPixel = nb;
                }
                unsigned int
                GetNumberOfComponentsPerPixel()
                {
                    return m_NumberOfComponentsPerPixel;
                }
                bool
                operator!=(const VectorChangeValue & other) const
                {
                    if (m_ChangeValue != other.m_ChangeValue)
                    {
                        return true;
                    }
                    return false;
                }
                bool
                operator==(const VectorChangeValue & other) const
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
                SetChangeVector(const InputValueVectorType & InputValue, OutputValueVectorType & ChangeValue)
                {
                    if (InputValue.size() == ChangeValue.size())
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
                    TOutput out;
                    // Set the size of the output pixel
                    out.SetSize(m_NumberOfComponentsPerPixel);
                    out = A;
                    InputValueVectorIteratorType inputIt;
                    OutputValueVectorIteratorType changeIt;

                    const unsigned int l_ASize(A.GetSize());
                    // Loops on the input and change value iterators
                    for (inputIt = m_InputValue.begin(), changeIt = m_ChangeValue.begin();
                            inputIt != m_InputValue.end() && changeIt != m_ChangeValue.end(); ++inputIt, ++changeIt)
                    {
                        unsigned int i = 0;
                        // Loops and checks if the pixel is a NoData value
                        while (i < l_ASize)
                        {
                            //if only one composant is at NoData, all the pixel is changed.
                            if (vnsIsNoDataMacro(A[i],(*inputIt)[i]) == true)
                            {
                                return *changeIt;
                            }
                            i++;
                        }
                    }
                    return out;
                }

            private:
                InputValueVectorType m_InputValue;
                OutputValueVectorType m_ChangeValue;
                unsigned int m_NumberOfComponentsPerPixel;
            };
    }
} // end namespace vns

#endif

