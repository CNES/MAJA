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
 * VERSION : 5-1-0 : FA : LAIG-FA-MAC-145739-CS : 13 juillet 2016 : Audit code - initialisation membre      *
 * VERSION : 1-0-0 : <TypeFT> : <NumFT> : 23 avr. 2010 : Creation                                                           
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id$
 *                                                                                                          *
 ************************************************************************************************************/
#ifndef __vnsMultiplyByVectorFunctor_h
#define __vnsMultiplyByVectorFunctor_h

namespace vns
{
    /** \class  MultiplyByVectorFunctor
     * \brief This class multiplies a vector image by a std::vector with a value by spectral band.
     *
     * \author CS Systemes d'Information
     *
     * \ingroup L2
     *
     */
    namespace Functor
    {
        template<class TInputPixel, class TOutputPixel>
            class MultiplyByVectorFunctor
            {
            public:
                typedef MultiplyByVectorFunctor<TInputPixel, TOutputPixel>
                        MultiplyByVectorFunctorType;


                MultiplyByVectorFunctor() : m_OutputSize(0)
                {
                }

                virtual ~MultiplyByVectorFunctor()
                {
                }


                typedef TInputPixel InputPixelType;
                typedef TOutputPixel OutputPixelType;
                typedef std::vector<double> CoefsVectorType;

                /** Get the coefficients by spectral band */
                CoefsVectorType
                GetCoefs()
                {
                    return m_Coefs;
                }
                /** Set the coefficients by spectral band*/
                void
                SetCoefs(CoefsVectorType & pCoefs)
                {
                    m_Coefs = pCoefs;
                }

                /** Get the output size of the output mask*/
                unsigned int
                GetOutputSize(void)
                {
                    return m_OutputSize;
                }
                /** Set the output size of the output mask */
                void
                SetOutputSize(unsigned int val)
                {
                    m_OutputSize = val;
                }

                size_t OutputSize(const std::array<size_t, 1>&) const
                {
                  return m_OutputSize;
                }

                inline OutputPixelType
                operator()(InputPixelType inPix) const
                {
                    OutputPixelType outPix;
                    const unsigned int l_size = inPix.Size();

                    // Set output pixel dimension
                    outPix.SetSize(l_size);

                    // Band Loop
                    for (unsigned int bd = 0; bd < l_size; bd++)
                    {
                        outPix[bd] = inPix[bd] * m_Coefs.at(bd);
                    }
                    return outPix;
                }

            protected:
                /** Vector of Coefficients declaration */
                CoefsVectorType m_Coefs;

                /** Output image dimension declaration */
                unsigned int m_OutputSize;

            };

    } // end namespace functor

} // End namespace vns

#endif /* __vnsMultiplyByVectorFunctor_h */
