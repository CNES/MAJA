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
 * VERSION : 1.0.0 : FA : LAIG-FA-MAC-148399-CS : 12 octobre 2016 : Audit code - Correction qualite :       *
 *                                               Regle : 0-1-3 A project shall not contain unused variables *
 * VERSION : 5-0-0 : FA : LAIG-FA-MAC-143764-CS : 12 avril 2016 : Mise en forme du code (indentation, etc.) *
 * VERSION : 4-0-0 : DM : LAIG-DM-MAC-517-CNES : 3 mars 2014 : Améliorations algorithmiques                 *
 * 					BUG : suppression de la valeur absolue avant de retourner le coefficient de correlation *
 * VERSION : 1-0-0 : <TypeFT> : <NumFT> : 18 mai 2010 : Creation                                                           
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id$
 *                                                                                                          *
 ************************************************************************************************************/
#ifndef __vnsCrossCorrelationFunctor_h
#define __vnsCrossCorrelationFunctor_h

#include "vnsMacro.h"
#include "vnsMath.h"

namespace vns
{
    /** \class  CrossCorrelationFunctor
     * \brief This class computes the correlation between two vector images taking
     * into account an input mask for the first image.
     *
     * - cast the input 1 pixel value to \c double
     * - cast the input 2 pixel value to \c double
     * - compute the difference of the two pixel values
     * - compute the value of the cross-correlation
     * - cast the \c double value resulting to the pixel type of the output image
     * - store the casted value into the output image.
     *
     * \author CS Systemes d'Information
     *
     * \ingroup L2
     *
     */
    namespace Functor
    {
        template<class TInputPixel, class TMaskPixel, class TOutputPixel>
            class CrossCorrelationFunctor
            {
            public:
                CrossCorrelationFunctor() :
                        m_NoData(0), m_MinPixelCorrelation(0), m_CorrelBandA(0), m_CorrelBandB(0)
                {
                }

                virtual
                ~CrossCorrelationFunctor()
                {
                }

                /** Get no_data value */
                RealNoDataType
                GetNoData()
                {
                    return m_NoData;
                }
                /** Set no_data value */
                void
                SetNoData(RealNoDataType val)
                {
                    m_NoData = val;
                }

                /** Get the minimum number of pixels to take correlation into account */
                unsigned long
                GetMinPixelCorrelation()
                {
                    return m_MinPixelCorrelation;
                }
                /** Set the minimum number of pixels to take correlation into account */
                void
                SetMinPixelCorrelation(unsigned long val)
                {
                    m_MinPixelCorrelation = val;
                }

                /** Get the correlation band index of image A */
                unsigned int
                GetCorrelBandA()
                {
                    return m_CorrelBandA;
                }
                /** Set the correlation band index of image A */
                void
                SetCorrelBandA(unsigned int val)
                {
                    m_CorrelBandA = val;
                }

                /** Get the correlation band index of image B */
                unsigned int
                GetCorrelBandB()
                {
                    return m_CorrelBandB;
                }
                /** Set the correlation band index of image B */
                void
                SetCorrelBandB(unsigned int val)
                {
                    m_CorrelBandB = val;
                }

                // The mask of the second image as been removed because it is not always available
                // as for the stack of images.
                inline TOutputPixel
                operator()(const TInputPixel & itA, const TInputPixel & itB, const TMaskPixel & itM1)
                {

                    // Initialization
                    TOutputPixel valueA;
                    TOutputPixel valueB;

                    TOutputPixel sumA = itk::NumericTraits<TOutputPixel>::Zero;
                    TOutputPixel sumB = itk::NumericTraits<TOutputPixel>::Zero;

                    TOutputPixel sumSquaredA = itk::NumericTraits<TOutputPixel>::Zero;
                    TOutputPixel sumSquaredB = itk::NumericTraits<TOutputPixel>::Zero;

                    TOutputPixel meanA = itk::NumericTraits<TOutputPixel>::Zero;
                    TOutputPixel meanB = itk::NumericTraits<TOutputPixel>::Zero;

                    TOutputPixel varA = itk::NumericTraits<TOutputPixel>::Zero;
                    TOutputPixel varB = itk::NumericTraits<TOutputPixel>::Zero;

                    unsigned long count(0);
                    double invCount(0.);
                    const unsigned long l_SizeA(static_cast<unsigned long>(itA.Size()));

                    // For N_Correl = 3 => Size = 49
                    // Pixel list = [l-N_Correl:l+N_Correl]*[p-N_Correl:p+N_Correl]
                    // Iterators are neighborhood iterators
                    for (unsigned long pos = 0; pos < l_SizeA; pos++)
                    {
                        valueA = itA.GetPixel(pos)[m_CorrelBandA];
                        valueB = itB.GetPixel(pos)[m_CorrelBandB];

                        // Discard all No_Data pixels for date D and D-1
                        // Because the mask of the image B is not always available (for instance with stack images),
                        // the test is applied on the values of refelectance.
                        if ((itM1.GetPixel(pos) == 0) && (vnsDifferentDoubleMacro(valueB,m_NoData) == true))
                        {
                            // Preparation of statistical calculations computing the sum
                            // and the squared sum for the date D and D-1
                            sumA = sumA + static_cast<TOutputPixel>(valueA);
                            sumB = sumB + static_cast<TOutputPixel>(valueB);
                            sumSquaredA = sumSquaredA + static_cast<TOutputPixel>(valueA * valueA);
                            sumSquaredB = sumSquaredB + static_cast<TOutputPixel>(valueB * valueB);

                            count++;
                        }
                    }

                    // If all the pixels are equals to No_Data
                    if (count == 0)
                    {
                        invCount = 0.;
                    }
                    else
                    {
                        invCount = static_cast<double>(1. / count);
                    }

                    // Mean computation
                    meanA = static_cast<TOutputPixel>(sumA * invCount);
                    meanB = static_cast<TOutputPixel>(sumB * invCount);

                    // Standard deviation
                    varA = static_cast<TOutputPixel>(invCount * sumSquaredA - (invCount * sumA) * (invCount * sumA));
                    varB = static_cast<TOutputPixel>(invCount * sumSquaredB - (invCount * sumB) * (invCount * sumB));

                    TOutputPixel crossCorrel = itk::NumericTraits<TOutputPixel>::Zero;

                    // Check that the number of pixel used for the correlation is sufficient
                    if (count >= m_MinPixelCorrelation)
                    {
                        // If the image areas are not uniform
                        // MACCS 5.0 : DM982 : 14/04/2016 : Validation sur un pixel avec l'equipe CNES (Beatrice, Elise et Morgan) :
                        // TVA CloudMaskingImageFilterF2 : pixel l_Index[0] == 417) && (l_Index[1] == 207
                        // si CONST_EPSILON (1e-6), pas suffisant et difference avec la maquette.
                        // avec CONST_EPSILON_20 -> OK
                        if ((vcl_abs(varA) > static_cast<TOutputPixel> (CONST_EPSILON_20 /* CONST_EPSILON */))
                        && (vcl_abs(varB) > static_cast<TOutputPixel> (CONST_EPSILON_20 /* CONST_EPSILON */)))
                        {
                            for (unsigned long pos = 0; pos < l_SizeA; pos++)
                            {
                                valueB = itB.GetPixel(pos)[m_CorrelBandB];

                                // Discard all No_Data pixels for date D and D-1
                                if ((itM1.GetPixel(pos) == 0) &&
                                (vnsDifferentDoubleMacro(valueB,m_NoData) == true))
                                {

                                    // Compute the correlation coefficient :
                                    // c = (sum((Ai-<A>)(Bi-<B>))/N.sqrt(1/N²*sum(Ai-<A>)²*sum(Bi-<B>)²))
                                    crossCorrel = crossCorrel + (static_cast<TOutputPixel> (itA.GetPixel(pos)[m_CorrelBandA])
                                    - meanA) * (valueB - meanB) / (count * std::sqrt(static_cast<double> (varA * varB)));
                                }
                            }
                        }
                        else
                        {
                            // The two images don't correlate
                            crossCorrel = itk::NumericTraits<TOutputPixel>::Zero;
                        }
                        // The correlation coefficient ranges between -1 and 1
                        // 0 means that there is no linear relationship between the two values
                        //crossCorrel = vcl_abs(crossCorrel);
                    }
                    else
                    {
                        crossCorrel = static_cast<TOutputPixel> (m_NoData);
                    } // end if count > m_MinPixelCorrelation

                    return static_cast<TOutputPixel>(crossCorrel);
                }

            protected:
                /** No_Data definition */
                RealNoDataType m_NoData;
                /** Declaration of the minimum number of pixels to take correlation into account */
                unsigned long m_MinPixelCorrelation;
                /** Declaration of the correlation band index of image A */
                unsigned int m_CorrelBandA;
                /** Declaration of the correlation band index of image B */
                unsigned int m_CorrelBandB;
            };

    } // End namespace functor

} // End namespace vns

#endif /* __vnsCrossCorrelationFunctor_h */

