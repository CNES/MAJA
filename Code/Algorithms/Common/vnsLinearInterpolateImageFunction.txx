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
 * VERSION : 1.0.0 : FA : LAIG-FA-MAC-148399-CS : 03 novembre 2016 : Audit code - Correction qualite        *
 * VERSION : 1.0.0 : FA : LAIG-FA-MAC-1990-CNES : 13 octobre 2016 : Correction qualite (TO DO)              *
 * VERSION : 5-1-0 : FA : LAIG-FA-MAC-145739-CS : 27 juillet 2016 : Audit code - cppcheck performances      *
 * VERSION : 5-1-0 : FA : LAIG-FA-MAC-144674-CS : 26 mai 2016 : Modifications cosmetique et menage, etc...  *
 * VERSION : 5-1-0 : FA : LAIG-FA-MAC-144719-CS : 17 mai 2016 : Correction warning et contrôle qualité      *
 * VERSION : 5-0-0 : FA : LAIG-FA-MAC-143764-CS : 12 avril 2016 : Mise en forme du code (indentation, etc.) *
 * VERSION : 4-4-1 : FA : LAIG-FA-MAC-127944-CS : 9 juillet 2015 : Correction warning et contrôle qualité   *
 * 														sur le code source C++   							*
 * VERSION : 4-4-0 : FA : LAIG-FA-MAC-1256-CNES : 1 juillet 2015 : Amelioration des performances            *
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id$
 *                                                                                                          *
 ************************************************************************************************************/
#ifndef __vnsLinearInterpolateImageFunction_txx
#define __vnsLinearInterpolateImageFunction_txx

#include "vnsLinearInterpolateImageFunction.h"

#include "itkNumericTraits.h"
#include "vnl/vnl_vector.h"
#include "vnsMacro.h"
#include "itkImageScanlineConstIterator.h"
#include "vnsLoggers.h"

namespace vns
{

    template<class TInputImage, class TCoordRep>
        void
        LinearInterpolateImageFunctionBase<TInputImage, TCoordRep>::PrintSelf(std::ostream& os, itk::Indent indent) const
        {
            Superclass::PrintSelf(os, indent);
            os << indent << "Radius: " << m_Radius << std::endl;
        }

    template<class TInputImage, class TCoordRep>
        void
        LinearInterpolateImageFunctionBase<TInputImage, TCoordRep>::SetRadius(unsigned int radius)
        {
            if (radius < 1)
            {
                vnsExceptionBusinessMacro("Radius must be strictly greater than 0")
            }
            // Initialize the linear coefficients of the interpolation
            else
            {
                m_Radius = radius;
                m_WinSize = 2 * m_Radius + 1;
                m_LinearCoef = this->EvaluateCoef(m_WinSize);
            }
        }

    template<class TInputImage, class TCoordRep>
        unsigned int
        LinearInterpolateImageFunctionBase<TInputImage, TCoordRep>::GetRadius() const
        {
            return m_Radius;
        }

    template<class TInputImage, class TCoordRep>
        typename LinearInterpolateImageFunctionBase<TInputImage, TCoordRep>::CoefContainerType
        LinearInterpolateImageFunctionBase<TInputImage, TCoordRep>::EvaluateCoef(const unsigned int & winsize)
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

    template<class TInputImage, class TCoordRep>
        void
        LinearInterpolateImageFunction<TInputImage, TCoordRep>::PrintSelf(std::ostream& os, itk::Indent indent) const
        {
            Superclass::PrintSelf(os, indent);
        }

    template<class TInputImage, class TCoordRep>
        typename LinearInterpolateImageFunction<TInputImage, TCoordRep>::OutputType
        LinearInterpolateImageFunction<TInputImage, TCoordRep>::EvaluateAtContinuousIndex(const ContinuousIndexType & index) const
        {

            IndexType baseIndex;
            IndexType neighIndex;

            std::vector<RealType> lineRes(this->m_WinSize, 0.);

            RealType value = itk::NumericTraits<RealType>::Zero;

            // Compute base index = closet index
            for (unsigned int dim = 0; dim < ImageDimension; dim++)
            {
                baseIndex[dim] = itk::Math::Floor<IndexValueType>(index[dim] + 0.5);
            }

            // Loop over the pixels of the window
            for (unsigned int i = 0; i < this->m_WinSize; ++i)
            {
                for (unsigned int j = 0; j < this->m_WinSize; ++j)
                {
                    // get neighbor index
                    neighIndex[0] = baseIndex[0] + i - this->m_Radius;
                    neighIndex[1] = baseIndex[1] + j - this->m_Radius;

                    // Set the boundary indices
                    if (neighIndex[0] > this->m_EndIndex[0])
                    {
                        neighIndex[0] = this->m_EndIndex[0];
                    }
                    if (neighIndex[0] < this->m_StartIndex[0])
                    {
                        neighIndex[0] = this->m_StartIndex[0];
                    }
                    if (neighIndex[1] > this->m_EndIndex[1])
                    {
                        neighIndex[1] = this->m_EndIndex[1];
                    }
                    if (neighIndex[1] < this->m_StartIndex[1])
                    {
                        neighIndex[1] = this->m_StartIndex[1];
                    }
                    // Apply the coefficients to the pixel value
                    lineRes[i] += static_cast<RealType>(this->GetInputImage()->GetPixel(neighIndex)) * this->m_LinearCoef[j];
                }
                value += lineRes[i] * this->m_LinearCoef[i];
            }

            return (static_cast<OutputType>(value));
        }

    template<typename TPixel, unsigned int VImageDimension, class TCoordRep>
        void
        LinearInterpolateImageFunction<otb::VectorImage<TPixel, VImageDimension>, TCoordRep>::PrintSelf(std::ostream& os,
                itk::Indent indent) const
        {
            Superclass::PrintSelf(os, indent);
        }

    template<typename TPixel, unsigned int VImageDimension, class TCoordRep>
        typename LinearInterpolateImageFunction<otb::VectorImage<TPixel, VImageDimension>, TCoordRep>::OutputType
        LinearInterpolateImageFunction<otb::VectorImage<TPixel, VImageDimension>, TCoordRep>::EvaluateAtContinuousIndex(
                const ContinuousIndexType & index) const
        {
            typedef typename itk::NumericTraits<InputPixelType>::ScalarRealType ScalarRealType;

            const unsigned int componentNumber = this->GetInputImage()->GetNumberOfComponentsPerPixel();

            IndexType baseIndex;
            IndexType neighIndex;
            OutputType output;
            output.SetSize(componentNumber);

            std::vector<std::vector<ScalarRealType> > lineRes(this->m_WinSize,
                    std::vector<ScalarRealType>(componentNumber, itk::NumericTraits<ScalarRealType>::Zero));
            std::vector<ScalarRealType> value(componentNumber, itk::NumericTraits<ScalarRealType>::Zero);

            //Compute base index = closet index
            for (unsigned int dim = 0; dim < ImageDimension; dim++)
            {
                baseIndex[dim] = itk::Math::Floor<IndexValueType>(index[dim] + 0.5);
            }

            // Loop over the pixels of the window
            for (unsigned int i = 0; i < this->m_WinSize; ++i)
            {
                const double l_LinearCoef_I(this->m_LinearCoef[i]);
                for (unsigned int j = 0; j < this->m_WinSize; ++j)
                {
                    const double l_LinearCoef_J(this->m_LinearCoef[j]);

                    // get neighbor index
                    neighIndex[0] = baseIndex[0] + i - this->m_Radius;
                    neighIndex[1] = baseIndex[1] + j - this->m_Radius;

                    // Set the boundary indices
                    if (neighIndex[0] > this->m_EndIndex[0])
                    {
                        neighIndex[0] = this->m_EndIndex[0];
                    }
                    if (neighIndex[0] < this->m_StartIndex[0])
                    {
                        neighIndex[0] = this->m_StartIndex[0];
                    }
                    if (neighIndex[1] > this->m_EndIndex[1])
                    {
                        neighIndex[1] = this->m_EndIndex[1];
                    }
                    if (neighIndex[1] < this->m_StartIndex[1])
                    {
                        neighIndex[1] = this->m_StartIndex[1];
                    }
                    // Apply the coefficients to the pixel value per band (vector image)
                    const InputPixelType & pixel = this->GetInputImage()->GetPixel(neighIndex);
                    for (unsigned int k = 0; k < componentNumber; ++k)
                    {
                        lineRes[i][k] += static_cast<ScalarRealType>(pixel.GetElement(k)) * l_LinearCoef_J;
                    }
                }
                for (unsigned int k = 0; k < componentNumber; ++k)
                {
                    value[k] += lineRes[i][k] * l_LinearCoef_I;
                }
            }

            for (unsigned int k = 0; k < componentNumber; ++k)
            {
                output.SetElement(k, value[k]);
            }

            return (output);
        }

} //namespace otb

#endif
