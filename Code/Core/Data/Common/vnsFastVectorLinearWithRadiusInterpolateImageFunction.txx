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
/***********************************************************************************************************
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
 * VERSION : 1.0.0 : FA : LAIG-FA-MAC-148399-CS : 27 octobre 2016 : Audit code - Correction qualite         *
 * VERSION : 1.0.0 : FA : LAIG-FA-MAC-1990-CNES : 13 octobre 2016 : Correction qualite (TO DO)              *
 * VERSION : 1-0-0 : <TypeFT> : <NumFT> : 10 avril 2010 : Creation
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id: vnsFastVectorLinearWithRadiusInterpolateImageFunction.txx 10478 2015-06-17 16:12:13Z tfeuvrie $
 *                                                                                                          *
 ************************************************************************************************************/
#ifndef __vnsFastVectorLinearWithRadiusInterpolateImageFunction_txx
#define __vnsFastVectorLinearWithRadiusInterpolateImageFunction_txx

#include "vnsFastVectorLinearWithRadiusInterpolateImageFunction.h"

#include "vnl/vnl_math.h"

namespace vns
{

    // Define the number of neighbors
    // Constructor
    template<class TInputImage, class TCoordRep>
        FastVectorLinearWithRadiusInterpolateImageFunction<TInputImage, TCoordRep>::FastVectorLinearWithRadiusInterpolateImageFunction()
        {

        }

    template<class TInputImage, class TCoordRep>
        void
        FastVectorLinearWithRadiusInterpolateImageFunction<TInputImage, TCoordRep>::SetRadius(unsigned int radius)
        {
            if (radius < 1)
            {
                vnsExceptionBusinessMacro("Radius must be strictly greater than 0")
            }
            else
            {
                m_Radius = radius;
                m_WinSize = 2 * m_Radius + 1;
                m_LinearCoef = this->EvaluateCoef(m_WinSize);
            }
        }

    template<class TInputImage, class TCoordRep>
        unsigned int
        FastVectorLinearWithRadiusInterpolateImageFunction<TInputImage, TCoordRep>::GetRadius() const
        {
            return m_Radius;
        }

    template<class TInputImage, class TCoordRep>
        typename FastVectorLinearWithRadiusInterpolateImageFunction<TInputImage, TCoordRep>::CoefContainerType
        FastVectorLinearWithRadiusInterpolateImageFunction<TInputImage, TCoordRep>::EvaluateCoef(const unsigned int & winsize)
        {
            // Init Linear coefficient container

            CoefContainerType Coef(winsize, 0.);
            // Compute Linear coefficients
            for (unsigned int i = 0; i <= m_Radius; i++)
            {
                const double l_coef = -1 / vcl_pow(static_cast<double> (m_Radius), 2) * vcl_abs(static_cast<double> (i)) + 1
                / static_cast<double> (m_Radius);

                Coef[m_Radius - i] = l_coef;
                Coef[m_Radius + i] = l_coef;

            }

            return Coef;
        }

    // PrintSelf
    template<class TInputImage, class TCoordRep>
        void
        FastVectorLinearWithRadiusInterpolateImageFunction<TInputImage, TCoordRep>::PrintSelf(std::ostream& os, itk::Indent indent) const
        {
            this->Superclass::PrintSelf(os, indent);
        }

    // Evaluate at image index position
    template<class TInputImage, class TCoordRep>
        typename FastVectorLinearWithRadiusInterpolateImageFunction<TInputImage, TCoordRep>::OutputType
        FastVectorLinearWithRadiusInterpolateImageFunction<TInputImage, TCoordRep>::EvaluateAtContinuousIndex(
                const ContinuousIndexType& index) const
        {
            typedef typename itk::NumericTraits<PixelType>::ScalarRealType ScalarRealType;

            const unsigned int componentNumber = this->GetInputImage()->GetNumberOfComponentsPerPixel();

            IndexType baseIndex;
            IndexType neighIndex;
            OutputType output;
            output.SetSize(componentNumber);
            std::vector<std::vector<ScalarRealType> > lineRes(this->m_WinSize,
                    std::vector<ScalarRealType>(componentNumber, itk::NumericTraits<ScalarRealType>::Zero));
            std::vector<ScalarRealType> value(componentNumber, itk::NumericTraits<ScalarRealType>::Zero);

            const unsigned int l_WinSize = this->m_WinSize;
            const CoefContainerType & l_LinearCoef = this->m_LinearCoef;

            //Compute base index = closet index
            for (unsigned int dim = 0; dim < ImageDimension; dim++)
            {
                baseIndex[dim] = itk::Math::Floor<IndexValueType>(index[dim] + 0.5) - this->m_Radius;
            }

            for (unsigned int i = 0; i < l_WinSize; ++i)
            {
                const double l_LinearCoef_I(l_LinearCoef[i]);
                neighIndex[0] = baseIndex[0] + i;
                if (neighIndex[0] > this->m_EndIndex[0])
                {
                    neighIndex[0] = this->m_EndIndex[0];
                }
                if (neighIndex[0] < this->m_StartIndex[0])
                {
                    neighIndex[0] = this->m_StartIndex[0];
                }
                for (unsigned int j = 0; j < l_WinSize; ++j)
                {
                    const double l_LinearCoef_J(l_LinearCoef[j]);

                    // get neighbor index
                    neighIndex[1] = baseIndex[1] + j;
                    if (neighIndex[1] > this->m_EndIndex[1])
                    {
                        neighIndex[1] = this->m_EndIndex[1];
                    }
                    if (neighIndex[1] < this->m_StartIndex[1])
                    {
                        neighIndex[1] = this->m_StartIndex[1];
                    }

                    const InputPixelType & pixel = this->GetInputImage()->GetPixel(neighIndex);
                    for (unsigned int k = 0; k < componentNumber; ++k)
                    {
                        lineRes[i][k] += static_cast<ScalarRealType>(pixel[k]) * l_LinearCoef_J;
                    }
                }
                for (unsigned int k = 0; k < componentNumber; ++k)
                {
                    value[k] += lineRes[i][k] * l_LinearCoef_I;
                }
            }
            for (unsigned int k = 0; k < componentNumber; ++k)
            {
                output[k] = value[k];
            }

            return (output);
        }

} // End namespace vns

#endif
