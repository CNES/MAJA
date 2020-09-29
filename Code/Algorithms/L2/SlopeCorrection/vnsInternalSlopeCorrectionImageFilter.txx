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
 * VERSION : 1.0.0 : FA : LAIG-FA-MAC-1988-CNES : 18 octobre 2016 : Correction qualite (code mort)          *
 * VERSION : 4-0-0 : FA : LAIG-FA-MAC-120653-CS : 3 avril 2014 : Correction de règles de qualité			*
 * VERSION : 4-0-0 : DM : LAIG-DM-MAC-517-CNES : 27 jan. 2014 : Utilisation du filtre de calcul de l'angle  *
 *                                                              d'incidence solaire                         *
 * VERSION : 1-0-0 : <TypeFT> : <NumFT> : 20 août 2010 : Creation                                                           
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id$
 *                                                                                                          *
 ************************************************************************************************************/
#ifndef __vnsInternalSlopeCorrectionImageFilter_txx
#define __vnsInternalSlopeCorrectionImageFilter_txx

#include "vnsInternalSlopeCorrectionImageFilter.h"
#include "vnsMath.h"
#include "vnsMacro.h"
#include "itkTimeProbe.h"

namespace vns
{

//
//******************* Constructor *******************
//
    template<class TInputVectorImage, class TInputImage, class TInputIncidenceAngleImage, class TOutputVectorImage, class TOutputMask>
        InternalSlopeCorrectionImageFilter<TInputVectorImage, TInputImage, TInputIncidenceAngleImage, TOutputVectorImage, TOutputMask>::InternalSlopeCorrectionImageFilter()
        {
            this->SetNumberOfRequiredInputs(7);
            this->SetNumberOfIndexedInputs(7);
            this->SetNumberOfIndexedOutputs(3);

            this->SetNthOutput(0, OutputVectorImageType::New());
            this->SetNthOutput(1, MaskType::New()); // STL
            this->SetNthOutput(2, MaskType::New()); // TGS

            // Initialization
            m_ThetaS = 0;
            m_ThetaV.clear();
            m_PhiS = 0;
            m_PhiV.clear();
            m_MinCosI = 0;
            m_MinCosE = 0;
            m_MinCosRatio = 0;
            m_NoData = 0;

        }

//
//******************* Destructor *******************
//
    template<class TInputVectorImage, class TInputImage, class TInputIncidenceAngleImage, class TOutputVectorImage, class TOutputMask>
        InternalSlopeCorrectionImageFilter<TInputVectorImage, TInputImage, TInputIncidenceAngleImage, TOutputVectorImage, TOutputMask>::~InternalSlopeCorrectionImageFilter()
        {
        }

//
//******************* BeforeThreadedGenerateData function *******************
//
    template<class TInputVectorImage, class TInputImage, class TInputIncidenceAngleImage, class TOutputVectorImage, class TOutputMask>
        void
        InternalSlopeCorrectionImageFilter<TInputVectorImage, TInputImage, TInputIncidenceAngleImage, TOutputVectorImage, TOutputMask>::BeforeThreadedGenerateData()
        {

            // Get the input image pointers
            InputImageConstPointer lAPtr = this->GetA();
            InputImageConstPointer lSPtr = this->GetS();
            InputVectorImageConstPointer lTdirPtr = this->GetTdir();
            InputVectorImageConstPointer lTdifPtr = this->GetTdif();
            InputVectorImageConstPointer lL2SREPtr = this->GetL2SRE();
            InputVectorImageConstPointer lRhoEnvPtr = this->GetRhoEnv();
            InternalIncidenceAngleImageConstPointer lIndidenceAnglePtr = this->GetIncidenceAngle();

            // Get outputMasks
            MaskConstPointer lSTLPtr = this->GetL2STL();
            MaskConstPointer lTGSPtr = this->GetL2TGS();

            // Get output image L2FRE
            OutputVectorImagePointerType lL2FREPtr = this->GetL2FRE();

            if ((lAPtr.IsNull() == true) || (lSPtr.IsNull() == true) || (lTdirPtr.IsNull() == true) || (lTdifPtr.IsNull() == true)
                    || /*(lTPtr.IsNull() == true) || */(lL2SREPtr.IsNull() == true) || (lSTLPtr.IsNull() == true)
                    || (lTGSPtr.IsNull() == true) || (lL2FREPtr.IsNull() == true) || (lRhoEnvPtr.IsNull() == true)
                    || (lIndidenceAnglePtr.IsNull() == true))
            {
                vnsExceptionBusinessMacro("At least one input is missing.");
            }

            // Check the input sizes
            const SizeType lSize1 = lAPtr->GetLargestPossibleRegion().GetSize();
            const SizeType lSize2 = lSPtr->GetLargestPossibleRegion().GetSize();
            const SizeType lSize3 = lTdirPtr->GetLargestPossibleRegion().GetSize();
            const SizeType lSize4 = lTdifPtr->GetLargestPossibleRegion().GetSize();
            const SizeType lSize6 = lL2SREPtr->GetLargestPossibleRegion().GetSize();
            const SizeType lSize7 = lRhoEnvPtr->GetLargestPossibleRegion().GetSize();
            const SizeType lSize8 = lIndidenceAnglePtr->GetLargestPossibleRegion().GetSize();

            if (lSize1 != lSize2)
            {
                vnsExceptionBusinessMacro("Size 2  is not correct.");
            }
            if (lSize1 != lSize3)
            {
                vnsExceptionBusinessMacro("Size 3  is not correct.");
            }
            if (lSize1 != lSize4)
            {
                vnsExceptionBusinessMacro("Size 4  is not correct.");
            }
            if (lSize1 != lSize6)
            {
                vnsExceptionBusinessMacro("Size 6  is not correct.");
            }
            if (lSize1 != lSize7)
            {
                vnsExceptionBusinessMacro("Size 7  is not correct.");
            }
            if (lSize1 != lSize8)
            {
                vnsExceptionBusinessMacro("Size 8  is not correct.");
            }
        }

//
//******************* ThreadedGenerateData function *******************
//
    template<class TInputVectorImage, class TInputImage, class TInputIncidenceAngleImage, class TOutputVectorImage, class TOutputMask>
        void
        InternalSlopeCorrectionImageFilter<TInputVectorImage, TInputImage, TInputIncidenceAngleImage, TOutputVectorImage, TOutputMask>::ThreadedGenerateData(
                const InputImageRegionType& outputRegionForThread, itk::ThreadIdType)
        {

            // Get the input image pointers
            InputImageConstPointer lAPtr = this->GetA();
            InputImageConstPointer lSPtr = this->GetS();
            InternalIncidenceAngleImageConstPointer lIncidenceAngleSPtr = this->GetIncidenceAngle();

            InputVectorImageConstPointer lTdirPtr = this->GetTdir();
            InputVectorImageConstPointer lTdifPtr = this->GetTdif();
            InputVectorImageConstPointer lL2SREPtr = this->GetL2SRE();
            InputVectorImageConstPointer lRhoEnvPtr = this->GetRhoEnv();

            // Get outputMasks
            MaskPointerType lSTLPtr = this->GetL2STL();
            MaskPointerType lTGSPtr = this->GetL2TGS();

            // Get output image L2FRE
            OutputVectorImagePointerType lL2FREPtr = this->GetL2FRE();

            // Define input Iterators
            ImageRegionConstIteratorType lAIt = ImageRegionConstIteratorType(lAPtr, outputRegionForThread);
            ImageRegionConstIteratorType lSIt = ImageRegionConstIteratorType(lSPtr, outputRegionForThread);
            InternalIncidenceAngleRegionConstIteratorType lIncidenceAngleIt = InternalIncidenceAngleRegionConstIteratorType(
                    lIncidenceAngleSPtr, outputRegionForThread);
            VectorImageRegionConstIteratorType lTdirIt = VectorImageRegionConstIteratorType(lTdirPtr, outputRegionForThread);
            VectorImageRegionConstIteratorType lTdifIt = VectorImageRegionConstIteratorType(lTdifPtr, outputRegionForThread);
            VectorImageRegionConstIteratorType lL2SREIt = VectorImageRegionConstIteratorType(lL2SREPtr, outputRegionForThread);
            VectorImageRegionConstIteratorType lRhoEnvIt = VectorImageRegionConstIteratorType(lRhoEnvPtr, outputRegionForThread);

            // Define output Iterators
            VectorImageRegionIteratorType lL2FREIt = VectorImageRegionIteratorType(lL2FREPtr, outputRegionForThread);
            MaskRegionIteratorType lL2STLIt = MaskRegionIteratorType(lSTLPtr, outputRegionForThread);
            MaskRegionIteratorType lL2TGSIt = MaskRegionIteratorType(lTGSPtr, outputRegionForThread);

            lAIt.GoToBegin();
            lSIt.GoToBegin();
            lIncidenceAngleIt.GoToBegin();
            lTdirIt.GoToBegin();
            lTdifIt.GoToBegin();
            lL2SREIt.GoToBegin();
            lRhoEnvIt.GoToBegin();
            lL2STLIt.GoToBegin();
            lL2TGSIt.GoToBegin();
            lL2FREIt.GoToBegin();

            // Previous declarations
            const unsigned int lNbOfBands = lL2SREPtr->GetNumberOfComponentsPerPixel();
            unsigned int i(0);
            //PixelType lNumerator;
            //PixelType lDenominator;
            PixelType lPixel;
            //lNumerator.SetSize(lNbOfBands);
            //lDenominator.SetSize(lNbOfBands);
            lPixel.SetSize(lNbOfBands);

            // Compute the ratio 1 / (cos(ThetaS) + cos(ThetaV)) for all the pixels
            // before processing the pixel loop
            const double lCosThetaS(std::cos(m_ThetaS));
            // Check if cos(ThetaS) is different from 0
            if (vnsEqualsDoubleMacro(lCosThetaS,0.0) == true)
            {
                vnsExceptionBusinessMacro("vnsInternalSlopeCorrectionImageFilter : divide by 0, cos( thetaS ) = 0 ");
            }

            if (m_ThetaV.size()!=lNbOfBands)
            {
                vnsExceptionBusinessMacro("The number of ThetaV values is not correct");
            }


            if (m_PhiV.size()!=lNbOfBands)
            {
                vnsExceptionBusinessMacro("The number of PhiV values is not correct");
            }



            const double lInvCosThetaS(1. / lCosThetaS);
            double lCosThetaV[lNbOfBands];
            double lSinThetaV[lNbOfBands];
            double lInvCosThetaSCosThetaV[lNbOfBands];
            for (i = 0; i < lNbOfBands; i++)
            {
                lCosThetaV[i] = std::cos(m_ThetaV[i]);
                lSinThetaV[i] = std::sin(m_ThetaV[i]);
                const double lDenom = lCosThetaS + lCosThetaV[i];
                if (vnsEqualsDoubleMacro(lDenom,0.0) == true)
                {
                    vnsExceptionBusinessMacro("vnsInternalSlopeCorrectionImageFilter : divide by 0, cos(thetaS) + cos(thetaV) = 0 ");
                }
                lInvCosThetaSCosThetaV[i] = 1. / lDenom;
            }

            // Pixel loop
            while (!lL2FREIt.IsAtEnd())
            {
                while (!lL2FREIt.IsAtEndOfLine())
                {
                    // Initialization
                    lL2TGSIt.Set(0);
                    lL2STLIt.Set(0);
                    const PixelType & lRsurfPix = lL2SREIt.Get();

                    // Check if the pixel is not a no_data pixel (edge)
                    if (vnsDifferentDoubleMacro(lRsurfPix[0], m_NoData))
                    {
                        const double lA = lAIt.Get();
                        const double lS = lSIt.Get();
                        const double lCosS = std::cos(lS);
                        const double lSinS = std::sin(lS);

                        // Correction of direct illumination of the visible ground fraction and of BRDF effect ground
                        // Kondratiev approximation for fraction of visible ground
                        const double lFVisibleGround = (1. - lCosS) * 0.5;
                        // Kondratiev approximation for fraction of visible sky
                        const double lFVisibleSky = (1. + lCosS) * 0.5;

                        double lIncidenceAngle = lIncidenceAngleIt.Get();

                        // Avoid over correction when sun is very close to local horizon
                        if (lIncidenceAngle < m_MinCosI)
                        {
                            lIncidenceAngle = m_MinCosI;
                            lL2STLIt.Set(1);
                        }

                        const double lIncidenceAngleDivByCosThetaS = lIncidenceAngle * lInvCosThetaS;
                        if (lIncidenceAngleDivByCosThetaS < m_MinCosRatio)
                        {
                            lL2TGSIt.Set(1);
                        }

                        // Initialization
                        lPixel.Fill(0.);
                        const PixelType & lTdirPix = lTdirIt.Get();
                        const PixelType & lTdifPix = lTdifIt.Get();
                        const PixelType & lRhoEnvPix = lRhoEnvIt.Get();

                        // Band loop
                        for (i = 0; i < lNbOfBands; i++)
                        {
                            const double lRsurf = lRsurfPix[i];
                            if (lRsurf >= 0.)
                            {
                                // Compute exitence angle
                                double lExitenceAngle = lCosThetaV[i] * lCosS;
                                lExitenceAngle = lExitenceAngle + lSinThetaV[i] * lSinS * std::cos(m_PhiV[i] - lA);

                                if (lExitenceAngle < m_MinCosE)
                                {
                                    lExitenceAngle = m_MinCosE;
                                }

                                // BRDF modeling
                                const double lFBRDF = (lIncidenceAngle + lExitenceAngle) * lInvCosThetaSCosThetaV[i];
                                const double lTPix = lTdirPix[i]+ lTdifPix[i];
                                const double lNumerator = lRsurf * lTPix * lFBRDF;

                                const double lDenominator = lTdirPix[i] * (lIncidenceAngleDivByCosThetaS) + lTdifPix[i] * lFVisibleSky
                                        + lTPix * lRhoEnvPix[i] * lFVisibleGround;

                                // The case where lDenominator[i]==0 is not handled : according to CESBIO, can't happen -> save time processing
                                // No: the case handle in the Slope TVA testing
                                // Flat surface reflectance value
                                if (vnsDifferentDoubleMacro(lDenominator,0.0) == true)
                                {
                                    lPixel[i] = lNumerator / lDenominator;
                                }
                            }
                        }
                        // Set the flat reflectance value in the output image (FRE)
                        lL2FREIt.Set(lPixel);
                    }
                    else
                    {
                        lL2FREIt.Set(lRsurfPix);
                    }
                    ++lAIt;
                    ++lSIt;
                    ++lIncidenceAngleIt;
                    ++lTdirIt;
                    ++lTdifIt;
                    ++lRhoEnvIt;
                    ++lL2SREIt;
                    ++lL2STLIt;
                    ++lL2TGSIt;
                    ++lL2FREIt;
                }
                lAIt.NextLine();
                lSIt.NextLine();
                lIncidenceAngleIt.NextLine();
                lTdirIt.NextLine();
                lTdifIt.NextLine();
                lRhoEnvIt.NextLine();
                lL2SREIt.NextLine();
                lL2STLIt.NextLine();
                lL2TGSIt.NextLine();
                lL2FREIt.NextLine();
            }

        }

//
//******************* PrintSelf method ****************
//
    template<class TInputVectorImage, class TInputImage, class TInputIncidenceAngleImage, class TOutputVectorImage, class TOutputMask>
        void
        InternalSlopeCorrectionImageFilter<TInputVectorImage, TInputImage, TInputIncidenceAngleImage, TOutputVectorImage, TOutputMask>::PrintSelf(
                std::ostream& os, itk::Indent indent) const
        {
            Superclass::PrintSelf(os, indent);
        }

//
//******************* L2FRE (output image) accessors  ****************
//
    template<class TInputVectorImage, class TInputImage, class TInputIncidenceAngleImage, class TOutputVectorImage, class TOutputMask>
        typename InternalSlopeCorrectionImageFilter<TInputVectorImage, TInputImage, TInputIncidenceAngleImage, TOutputVectorImage,
                TOutputMask>::OutputVectorImageType *
        InternalSlopeCorrectionImageFilter<TInputVectorImage, TInputImage, TInputIncidenceAngleImage, TOutputVectorImage, TOutputMask>::GetL2FRE(
                void)
        {
            return dynamic_cast<OutputVectorImageType *>(itk::ProcessObject::GetOutput(0));
        }

//
//******************* STL image accessors  ****************
//
    template<class TInputVectorImage, class TInputImage, class TInputIncidenceAngleImage, class TOutputVectorImage, class TOutputMask>
        typename InternalSlopeCorrectionImageFilter<TInputVectorImage, TInputImage, TInputIncidenceAngleImage, TOutputVectorImage,
                TOutputMask>::MaskType *
        InternalSlopeCorrectionImageFilter<TInputVectorImage, TInputImage, TInputIncidenceAngleImage, TOutputVectorImage, TOutputMask>::GetL2STL(
                void)
        {
            return dynamic_cast<MaskType *>(itk::ProcessObject::GetOutput(1));
        }

//
//******************* TGS image accessors  ****************
//
    template<class TInputVectorImage, class TInputImage, class TInputIncidenceAngleImage, class TOutputVectorImage, class TOutputMask>
        typename InternalSlopeCorrectionImageFilter<TInputVectorImage, TInputImage, TInputIncidenceAngleImage, TOutputVectorImage,
                TOutputMask>::MaskType *
        InternalSlopeCorrectionImageFilter<TInputVectorImage, TInputImage, TInputIncidenceAngleImage, TOutputVectorImage, TOutputMask>::GetL2TGS(
                void)
        {
            return dynamic_cast<MaskType *>(itk::ProcessObject::GetOutput(2));
        }

} // End namespace vns

#endif /* __vnsInternalSlopeCorrectionImageFilter_txx */
