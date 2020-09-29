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
 * VERSION : 1.0.0 : FA : LAIG-FA-MAC-1988-CNES : 21 octobre 2016 : Correction qualite (code mort)          *
 * VERSION : 4-0-0 : FA : LAIG-FA-MAC-117040-CS : 31 mars 2014 : Modifications mineures                     *
 * VERSION : 3-2-0 : FA : LAIG-FA-MAC-701-CNES : 21 novembre 2013 : Correction de l'information géographique*
 * 									dans les produits en sortie.											*
 * VERSION : 1-0-0 : <TypeFT> : <NumFT> : 31 mai 2010 : Creation                                                           
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id: vnsComputeFinalResultEnvironmentCorrectionImageFilter.txx 10093 2015-05-22 08:46:55Z tfeuvrie $
 *                                                                                                          *
 ************************************************************************************************************/
#ifndef __vnsComputeFinalResultEnvironmentCorrectionImageFilter_txx
#define __vnsComputeFinalResultEnvironmentCorrectionImageFilter_txx

#include "vnsComputeFinalResultEnvironmentCorrectionImageFilter.h"

#include "vnsMacro.h"
#include "itkImageScanlineIterator.h"
#include "itkImageScanlineConstIterator.h"
#include "itkImageRegionConstIterator.h"
#include "itkImageRegionIterator.h"


namespace vns
{

    template<class TTOAInputImage, class TOutputImage, class TLUT>
        void
        ComputeFinalResultEnvironmentCorrectionImageFilter<TTOAInputImage, TOutputImage, TLUT>::ThreadedGenerateData(
                const OutputImageRegionType& outputRegionForThread, itk::ThreadIdType /*threadId*/)
        {
            // Get input
            InputImageConstPointerType TocPtr = this->GetTocImage();
            InputImageConstPointerType RhoEnvPtr = this->GetRhoEnvImage();
            InputImageConstPointerType TdirPtr = this->GetTdirImage();
            InputImageConstPointerType TdifPtr = this->GetTdifImage();
            InputImageConstPointerType SatmPtr = this->GetSatmImage();
            InputMaskConstPointerType EdgPtr = this->GetEdgImage();
            // Get the output pointer
            OutputImageType * outputPtr = this->GetOutput();

            // local iterators

            itk::ImageRegionConstIterator<InputImageType> TocIt(TocPtr, outputRegionForThread);
            itk::ImageRegionConstIterator<InputImageType> RhoEnvIt(RhoEnvPtr, outputRegionForThread);
            itk::ImageRegionConstIterator<InputImageType> TdirIt(TdirPtr, outputRegionForThread);
            itk::ImageRegionConstIterator<InputImageType> TdifIt(TdifPtr, outputRegionForThread);
            itk::ImageRegionConstIterator<InputImageType> SatmIt(SatmPtr, outputRegionForThread);
            itk::ImageRegionConstIterator<InputMaskType> EdgIt(EdgPtr, outputRegionForThread);

            itk::ImageRegionIterator<OutputImageType> outIt(outputPtr, outputRegionForThread);

            // Init iterators to begin
            TocIt.GoToBegin();
            RhoEnvIt.GoToBegin();
            TdirIt.GoToBegin();
            TdifIt.GoToBegin();
            SatmIt.GoToBegin();
            EdgIt.GoToBegin();

            outIt.GoToBegin();

            const InputPixelType & ltocPix = TocIt.Get();
            // Get the number of bands
            const unsigned int lNbOfBands = ltocPix.Size();
            OutputPixelType outPix;
            // Init the output value
            outPix.SetSize(lNbOfBands);
            outPix.Fill(m_NoData);

            // Loop image
            while (outIt.IsAtEnd() == false)
            {
                    const InputPixelType & pRhoSurf = TocIt.Get();
                    const InputMaskPixelType lL2EDG = EdgIt.Get();
                    if ((lL2EDG == 0) && vnsDifferentDoubleMacro(pRhoSurf[0], m_NoData))
                    {
                        const InputPixelType & pRhoEnv = RhoEnvIt.Get();
                        const InputPixelType & pTdir = TdirIt.Get();
                        const InputPixelType & pTdif = TdifIt.Get();
                        const InputPixelType & pSatm = SatmIt.Get();

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
                            const double lNumerator = pRhoSurf[bd] * (pTdir[bd] + pTdif[bd]) * lFraction - pRhoEnv[bd] * pTdif[bd];

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
                    else
                    {
                        outPix.Fill(m_NoData);
                    }
                    outIt.Set(outPix);

                    ++TocIt;
                    ++RhoEnvIt;
                    ++TdirIt;
                    ++TdifIt;
                    ++SatmIt;
                    ++EdgIt;
                    ++outIt;
            }
        }

} // end namespace vns

#endif
