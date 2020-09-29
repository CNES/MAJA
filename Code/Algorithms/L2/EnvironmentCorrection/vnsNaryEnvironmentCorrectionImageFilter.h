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
 * VERSION : 5-1-0 : FA : LAIG-FA-MAC-145739-CS : 27 juin 2016 : Audit code - Supp de la macro ITK_EXPORT   *
 * VERSION : 1-0-0 : <TypeFT> : <NumFT> : 1 sept 2010 : Creation
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id$
 *                                                                                                          *
 ************************************************************************************************************/
#ifndef __vnsNaryEnvironmentCorrectionImageFilter_h
#define __vnsNaryEnvironmentCorrectionImageFilter_h
#include "vnsNaryFunctorImageFilter.h"
#include "vnsMath.h"

namespace vns
{
    /** \class  EnvironmentCorrectionFunctor
     * \brief This class computes the environment correction.
     *
     * For each pixel at L2_resolution. Input and output pixels are
     * variableLengthVector.
     *
     * \author CS Systemes d'Information
     *
     * \ingroup L2
     *
     */
    namespace Functor
    {
        template<class TInputPixel, class TOutputPixel>
            class EnvironmentCorrectionFunctor
            {
            public:
                typedef EnvironmentCorrectionFunctor<TInputPixel, TOutputPixel> EnvironmentCorrectionFunctorType;

                EnvironmentCorrectionFunctor(void)
                {
                    m_NoData = static_cast<InternalPixelType>(0.);
                }

                virtual
                ~EnvironmentCorrectionFunctor(void)
                {
                }

                typedef TInputPixel PixelType;
                typedef TOutputPixel OutputPixelType;
                typedef typename OutputPixelType::ValueType InternalPixelType;

                // NoData setter
                void
                SetNoData(InternalPixelType pNoData) //, const unsigned int p_nb_bands)
                {
                    m_NoData = pNoData;
//                    m_NumberOfBands = p_nb_bands;
//                    // Init the output value
//                    m_NoDataPixel.SetSize(m_NumberOfBands);
//                    m_NoDataPixel.Fill(m_NoData);
//                    m_CacheOutPixel = m_NoDataPixel;
                }

                inline OutputPixelType
                operator()(const std::vector<PixelType> & pVect)
                {
// //MACCS 4.3 - optimization
//                    const PixelType & pRhoSurf = pVect.at(0);
//                    const PixelType & pRhoEnv = pVect.at(1);
//                    const PixelType & pTdir = pVect.at(2);
//                    const PixelType & pTdif = pVect.at(3);
//                    const PixelType & pT = pVect.at(4);
//                    const PixelType & pSatm = pVect.at(5);
//                    const unsigned int lL2EDG = static_cast<unsigned int>(pVect.at(6)[0]);
                    const PixelType & pRhoSurf = pVect[0];
                    const PixelType & pRhoEnv = pVect[1];
                    const PixelType & pTdir = pVect[2];
                    const PixelType & pTdif = pVect[3];
                    const PixelType pT = pVect[2] + pVect[3];
                    const PixelType & pSatm = pVect[4];
                    const unsigned int lL2EDG = static_cast<unsigned int>(pVect[5][0]);

                    // Get the number of bands
                    // MACCS 4.3 - optimization
                    const unsigned int lNbOfBands = pRhoSurf.Size();
                    OutputPixelType outPix;
                    // Init the output value
                    outPix.SetSize(lNbOfBands);
                    outPix.Fill(m_NoData);
                    if ((lL2EDG == 0) && vnsDifferentDoubleMacro(pRhoSurf[0], m_NoData))
                    {
                        // Band Loop
                        for (unsigned int bd = 0; bd < lNbOfBands; bd++)
                        {

                            double lFraction = 0;
                            const double lDenom = 1 - pRhoSurf[bd] * pSatm[bd];
                            if (vcl_abs(lDenom) > CONST_EPSILON)
                            {
                                lFraction = (1 - pRhoEnv[bd] * pSatm[bd]) / lDenom;
                            }

                            // Compute the numerator
                            const double lNumerator = pRhoSurf[bd] * pT[bd] * lFraction - pRhoEnv[bd] * pTdif[bd];

                            // Computes the Rho surf cor env
                            double lRhoSurfCorEnv = lNumerator;
                            if (vcl_abs(pTdir[bd]) > CONST_EPSILON)
                            {
                                lRhoSurfCorEnv = lNumerator / pTdir[bd];
                            }
                            // Set the output pîxel value
                            outPix[bd] = static_cast<InternalPixelType>(lRhoSurfCorEnv);
                        }
                    }
                    return outPix;
                }

            private:

                InternalPixelType m_NoData;

                // MACCS 4.3 - optimization
//                OutputPixelType m_NoDataPixel;
//                OutputPixelType m_CacheOutPixel;
//                unsigned int m_NumberOfBands;

            };

    } // end namespace functor

    template<class TInputImage, class TOutputImage>
        class NaryEnvironmentCorrectionImageFilter : public NaryFunctorImageFilter<TInputImage, TOutputImage,
                Functor::EnvironmentCorrectionFunctor<typename TInputImage::PixelType, typename TInputImage::PixelType> >
        {
        public:
            /** Standard class typedefs. */
            typedef NaryEnvironmentCorrectionImageFilter Self;
            typedef NaryFunctorImageFilter<TInputImage, TOutputImage,
                    vns::Functor::EnvironmentCorrectionFunctor<typename TInputImage::PixelType, typename TInputImage::PixelType> > Superclass;
            typedef itk::SmartPointer<Self> Pointer;
            typedef itk::SmartPointer<const Self> ConstPointer;

            /** Method for creation through the object factory. */
            itkNewMacro(Self)

            /** Runtime information support. */
            itkTypeMacro(NaryEnvironmentCorrectionImageFilter,
                    NaryFunctorImageFilter)

        protected:
            NaryEnvironmentCorrectionImageFilter()
            {
            }
            virtual
            ~NaryEnvironmentCorrectionImageFilter()
            {
            }

        private:
            NaryEnvironmentCorrectionImageFilter(const Self&); //purposely not implemented
            void
            operator=(const Self&); //purposely not implemented

        };

} // End namespace vns

#endif /* __vnsNaryEnvironmentCorrectionImageFilter_h */
