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
 * VERSION : 5-1-0 : FA : LAIG-FA-MAC-145509-CS : 9 aout 2016 : Modification de la génération du Doxygen    *
 * VERSION : 5-0-0 : FA : LAIG-FA-MAC-143764-CS : 12 avril 2016 : Mise en forme du code (indentation, etc.) *
 * VERSION : 1-0-0 : <TypeFT> : <NumFT> : 21 juin 2010 : Creation                                                           
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id$
 *                                                                                                          *
 ************************************************************************************************************/
#ifndef __vnsSolarDirectionForCloudAltitudeFunctor_h
#define __vnsSolarDirectionForCloudAltitudeFunctor_h

#include "itkImageToImageFilter.h"

#include "vnsMath.h"
#include "vnsMacro.h"

namespace vns
{
    /** \class  SolarDirectionForCloudAltitudeFunctor
     * \brief This class interpolates the solar direction taking into account
     * the cloud altitude.
     *
     * Input images are :
     *  - the solar grid for the first reference altitude (otb::VectorImage)
     *  - the solar grid for the second reference altitude (otb::VectorImage)
     *  - the cloud (or reference) altitude (otb::Image) used for the solar grid interpolation
     *
     * Parameters are :
     *  - the first reference altitude H1
     *  - the second reference altitude H2
     *
     * The output image is the interpolate solar grid. The formula is :
     *
     * \f$ DS(Hn)=DS(H2).\frac{ Hn * (Hn-H1) }{ H2 * (H2-H1) } + DS(H1) .\frac{ Hn * (H2-Hn) }{ H1 * (H2-H1) } \f$
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
        template<class TInputPixel1, class TInputPixel2, class TCLAPixel, class TOutputPixel>
            class SolarDirectionForCloudAltitudeFunctor
            {
            public:
                typedef SolarDirectionForCloudAltitudeFunctor<TInputPixel1, TInputPixel2, TCLAPixel, TOutputPixel> SolarDirectionForCloudAltitudeFunctorType;

                SolarDirectionForCloudAltitudeFunctor(void) :
                        m_H1(0), m_H2(0), m_SOLHRef(0)
                {
                }

                virtual
                ~SolarDirectionForCloudAltitudeFunctor(void)
                {
                }

                typedef TInputPixel1 InputPixelType1;
                typedef TInputPixel2 InputPixelType2;
                typedef TCLAPixel CLAPixelType;
                typedef TOutputPixel OutputPixelType;
                typedef typename OutputPixelType::ValueType OutputInternalPixelType;

                /** Get the first reference altitude for solar direction value */
                CLAPixelType
                GetH1(void)
                {
                    return m_H1;
                }
                /** Set the first reference altitude for solar direction value */
                void
                SetH1(CLAPixelType val)
                {
                    m_H1 = val;
                }

                /** Get the second reference altitude for solar direction value */
                CLAPixelType
                GetH2(void)
                {
                    return m_H2;
                }
                /** Set the second reference altitude for solar direction value */
                void
                SetH2(CLAPixelType val)
                {
                    m_H2 = val;
                }
                /** Set the second reference altitude for solar direction value */
                void
                SetSOLHRef(CLAPixelType val)
                {
                    m_SOLHRef = val;
                }

                inline OutputPixelType
                operator()(const InputPixelType1 & inSOL1Pix, const InputPixelType2 & inSOL2Pix, const CLAPixelType & inCLAPix) const
                {
                    OutputPixelType outSOLPix;

                    // Set output pixel dimension
                    const unsigned int l_Size = inSOL1Pix.Size();
                    outSOLPix.SetSize(l_Size);

                    // Check the reference solar altitudes
                    if (vnsEqualsDoubleMacro((m_H2-m_H1),0) == true)
                    {
                        vnsStaticExceptionBusinessMacro("The difference between \n"
                                "the reference solar altitudes is null\n");
                    }

                    // If the second reference altitude is null
                    if (vnsEqualsDoubleMacro(m_H2,0) == true)
                    {
                        // Linear interpolation : outSOL = (Hn/SOLH).SOL
                        for (unsigned int bd = 0; bd < l_Size; bd++)
                        {
                            // Linear interpolation over the cloud altitude
                            outSOLPix[bd] = static_cast<OutputInternalPixelType>(inSOL1Pix[bd] * inCLAPix / m_H1);
                        }
                    } // end if m_H2 = 0
                    else
                    {
                        // If the first reference altitude is null
                        if (vnsEqualsDoubleMacro(m_H1,0) == true)
                        {
                            // Linear interpolation : outSOL = (Hn/SOLH).SOL
                            for (unsigned int bd = 0; bd < l_Size; bd++)
                            {
                                // Linear interpolation over the cloud altitude
                                outSOLPix[bd] = static_cast<OutputInternalPixelType>(inSOL2Pix[bd] * inCLAPix / m_H2);
                            }

                        } // end if m_H1 = 0
                        else
                        {
                            const double l_Denom1 = 1. / static_cast<double>(m_H1 * (m_H2 - m_H1));
                            const double l_Denom2 = 1. / static_cast<double>(m_H2 * (m_H2 - m_H1));

                            // Band Loop
                            for (unsigned int bd = 0; bd < l_Size; bd++)
                            {

                                // Linear interpolation over the cloud altitude
                                /* outSOLPix[bd] = static_cast<OutputInternalPixelType> (inSOL2Pix[bd]
                                 * l_Denom2 * inCLAPix * (inCLAPix - m_H1));
                                 outSOLPix[bd] = outSOLPix[bd]
                                 + static_cast<OutputInternalPixelType> (inSOL1Pix[bd]
                                 * l_Denom1 * inCLAPix * (m_H2 - inCLAPix));
                                 */
                                outSOLPix[bd] = static_cast<OutputInternalPixelType>(inSOL2Pix[bd] * l_Denom2 * m_SOLHRef
                                        * (inCLAPix - m_H1));
                                outSOLPix[bd] = outSOLPix[bd]
                                        + static_cast<OutputInternalPixelType>(inSOL1Pix[bd] * l_Denom1 * m_SOLHRef * (m_H2 - inCLAPix));
                            }

                        } // end if m_H1 et m_H2 <> 0

                    } // end if m_H2 <> 0

                    return outSOLPix;
                }

            protected:

                /** First reference altitude for solar direction declaration */
                CLAPixelType m_H1;
                /** Second reference altitude for solar direction declaration */
                CLAPixelType m_H2;
                /** Reference Solar direction declaration */
                CLAPixelType m_SOLHRef;

            };

        template<class TInputPixel, class TAltPixel, class TOutputPixel>
            class SolarDirectionLinearInterpolationForReferenceAltitudeFunctor
            {
            public:
                typedef SolarDirectionLinearInterpolationForReferenceAltitudeFunctor<TInputPixel, TAltPixel, TOutputPixel> SolarDirectionLinearInterpolationForReferenceAltitudeFunctorType;

                SolarDirectionLinearInterpolationForReferenceAltitudeFunctor(void) :
                        m_H(0), m_SOLHRef(0)
                {
                }

                virtual
                ~SolarDirectionLinearInterpolationForReferenceAltitudeFunctor(void)
                {
                }

                typedef TInputPixel InputPixelType;
                typedef TAltPixel AltPixelType;
                typedef TOutputPixel OutputPixelType;
                typedef typename OutputPixelType::ValueType OutputInternalPixelType;

                /** Get the first reference altitude for solar direction value */
                AltPixelType
                GetH(void)
                {
                    return m_H;
                }
                /** Set the first reference altitude for solar direction value */
                void
                SetH(AltPixelType val)
                {
                    m_H = val;
                }

                /** Set the second reference altitude for solar direction value */
                void
                SetSOLHRef(AltPixelType val)
                {
                    m_SOLHRef = val;
                }

                inline OutputPixelType
                operator()(const InputPixelType & inSOLPix, const AltPixelType & inAltPix) const
                {
                    OutputPixelType outSOLPix;

                    // Set output pixel dimension
                    const unsigned int l_Size = inSOLPix.Size();
                    outSOLPix.SetSize(l_Size);

                    // Check the reference solar altitudes
                    if (vnsEqualsDoubleMacro((m_H),0) == true)
                    {
                        vnsStaticExceptionBusinessMacro("the reference solar altitude is null\n");
                    }

                    // Linear interpolation : outSOL = (Hn/SOLH).SOL
                    for (unsigned int bd = 0; bd < l_Size; bd++)
                    {
                        // Linear interpolation over the cloud altitude
                        outSOLPix[bd] = static_cast<OutputInternalPixelType>(inSOLPix[bd] * inAltPix / m_H);
                    }
                    return outSOLPix;
                }

            protected:

                /** First reference altitude for solar direction declaration */
                AltPixelType m_H;
                /** Reference Solar direction declaration */
                AltPixelType m_SOLHRef;

            };

    } // end namespace functor

} // End namespace vns

#endif /* __vnsSolarDirectionForCloudAltitudeFunctor_h */
