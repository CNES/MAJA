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
 * VERSION : 3.2.0 : DM : LAIG-DM-MAJA-2737-CNES : 07 juin 2018 : CAMS utilises dans AOTEstimation          *
 * VERSION : 1.0.0 : FA : LAIG-FA-MAC-148399-CS : 03 novembre 2016 : Audit code - Correction qualite        *
 * VERSION : 1.0.0 : FA : LAIG-FA-MAC-1990-CNES : 13 octobre 2016 : Correction qualite (TO DO)              *
 * VERSION : 5-0-0 : FA : LAIG-FA-MAC-143764-CS : 12 avril 2016 : Mise en forme du code (indentation, etc.) *
 * VERSION : 2-0-0 : DM : LAIG-DM-MAC-146-CNES : 13 octobre 2011 : Creation de la méthode multi spectrale        *
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id$
 *                                                                                                          *
 ************************************************************************************************************/

#ifndef __vnsMultiSpectralAOTCostFunction_txx
#define __vnsMultiSpectralAOTCostFunction_txx


#include "vnsMultiSpectralAOTCostFunction.h"

namespace vns
{

    template<class TInputImage, class TInputDTM, class TLUT>
        MultiSpectralAOTCostFunction<TInputImage, TInputDTM, TLUT>::MultiSpectralAOTCostFunction() :
                m_NDVIAOTBand1(0), m_NDVIAOTBand2(0), m_SlopeMS(0.), m_YInterceptMS(0.), m_SlopeNDVI(0.), m_YInterceptNDVI(0.)
        {
            this->m_RangeDimension = 49; // 49(MAX Window size) * 1 (one cost) + 1 (for the latest cost)

            this->m_Measure.SetSize(this->m_RangeDimension);
            this->m_Derivative.SetSize(this->m_SpaceDimension, this->m_RangeDimension);

            m_AOTbands.clear();
        }

    template<class TInputImage, class TInputDTM, class TLUT>
        typename MultiSpectralAOTCostFunction<TInputImage, TInputDTM, TLUT>::MeasureType
        MultiSpectralAOTCostFunction<TInputImage, TInputDTM, TLUT>::GetValue(const ParametersType & parameters) const
        {

            // Test that the current values stay within the LUT indexes
            double lAOTcurrentD = parameters[0];

            // Error bound computation
            const double lErrorBoundD = this->EvaluateErrorBound(this->m_AOTmin, this->m_AOTmax, lAOTcurrentD);

            // Test that the current values stay within the LUT indexes
            lAOTcurrentD = this->LimitAOT(parameters[0]);

            // Main cost computation
            unsigned int valueindex = 0;
            this->EvaluateErrors(valueindex, lAOTcurrentD, this->m_Measure);

            //Add CAMS to vector if used
            if(this->m_UseCamsData)
            {
            	this->m_Measure[valueindex] = this->m_KPondCams * (lAOTcurrentD -  this->m_CAMSAot);
            	valueindex++;
            }

            // Dark Error computation
            const double lDarkErrorD = this->EvaluateDarkError(lAOTcurrentD);

            this->m_Measure[valueindex] = lErrorBoundD + lDarkErrorD;

            return this->m_Measure;
        }

    template<class TInputImage, class TInputDTM, class TLUT>
        void
        MultiSpectralAOTCostFunction<TInputImage, TInputDTM, TLUT>::GetDerivative(const ParametersType & parameters,
                DerivativeType & derivative) const
        {
            // Test that the current values stay within the LUT indexes
            const double lAOTcurrentD = this->LimitAOT(parameters[0]);

            // Main cost computation
            unsigned int valueindex = 0;
            this->DerivativeEvaluateErrors(valueindex, lAOTcurrentD, this->m_Derivative);

            //Add CAMS to vector if used
            if(this->m_UseCamsData)
            {
            	this->m_Derivative[0][valueindex] = this->m_KPondCams;
            	valueindex++;
            }

            // Error bound computation
            double lDerivativeErrorBoundD = this->DerivativeEvaluateErrorBound(this->m_AOTmin, this->m_AOTmax, lAOTcurrentD);

            // Dark Error computation
            double lDerivativeDarkErrorD = this->DerivativeEvaluateDarkError(lAOTcurrentD);

            this->m_Derivative[0][valueindex] = lDerivativeErrorBoundD + lDerivativeDarkErrorD;

            derivative = this->m_Derivative;

        }

    template<class TInputImage, class TInputDTM, class TLUT>
        void
        MultiSpectralAOTCostFunction<TInputImage, TInputDTM, TLUT>::EvaluateErrors(unsigned int & pValueindex, double pAOTcurrentD,
                MeasureType & pMSMeasure) const
        {

            /** Blue band parameter (first band of the AOT bands vector) */
            const unsigned int lBlueBand = m_AOTbands[0];
            /** Red band parameter (second band of the AOT bands vector) */
            const unsigned int lRedBand = m_AOTbands[1];

            // Compute NDVI
            NDVIFunctorType lNDVIFunctor;

            if (this->m_LUTD.IsNull())
            {
                vnsExceptionBusinessMacro("MultiSpectralAOTCostFunction::LUT D is null.");
            }

            const unsigned int lPixelListSize = this->m_OffsetVect.size();

            LUTPointType pointD;

            for (unsigned int j = 0; j < lPixelListSize; j++)
            {
                const OffsetType & lOffset = this->m_OffsetVect[j];
                const DTMPixelType DTMSubPix = this->m_DTMSub.GetPixel(lOffset);
                const PixelType & IPTOACSubPixel = this->m_IPTOACSub.GetPixel(lOffset);

                pointD[0] = static_cast<LUTPointValueType>(DTMSubPix); // altitude
                pointD[1] = static_cast<LUTPointValueType>(pAOTcurrentD); // AOT

                pointD[2] = static_cast<LUTPointValueType>(IPTOACSubPixel[lBlueBand]); // R_toa*_sub(blue)

                // Reduced LUT at the date D (3dim) with n bands (LUTPixelType is a VariableLengthVector of size n)
                // spec : R_surf_est_blue
                const LUTPixelType blueInterpValue = this->m_LUTD->InterpolateValue(pointD);

                pointD[2] = static_cast<LUTPointValueType>(IPTOACSubPixel[lRedBand]); // R_toa*_sub(red)

                // spec : R_surf_est_red
                const LUTPixelType redInterpValue = this->m_LUTD->InterpolateValue(pointD);

                // Estimated surface reflectance
                const double RsurfEstBlue = static_cast<double>(blueInterpValue[lBlueBand]);
                // Simulated surface reflectance in blue
                const double RsurfSimBlue = static_cast<double>(m_SlopeMS * redInterpValue[lRedBand]) + m_YInterceptMS;

                // Compute NDVI indice to correct multi spectral weight
                itk::VariableLengthVector<double> v(2);
                v[0] = IPTOACSubPixel[m_NDVIAOTBand2];
                v[1] = IPTOACSubPixel[m_NDVIAOTBand1];
                lNDVIFunctor.SetBandIndex(CommonBandNames::RED, 1);
                lNDVIFunctor.SetBandIndex(CommonBandNames::NIR, 2);
                InternalPixelType lNDVIValue = lNDVIFunctor(v);

                if (lNDVIValue < static_cast<InternalPixelType>(0))
                {
                    lNDVIValue = static_cast<InternalPixelType>(0);
                }

                const double lWeqMS = this->m_Weq1 * (m_SlopeNDVI * lNDVIValue + m_YInterceptNDVI);

                // Cost function
                const double blueCost = RsurfEstBlue - RsurfSimBlue;

                pMSMeasure[pValueindex] = lWeqMS * blueCost;

                pValueindex++;

            } // pixel loop

        }

    template <class TInputImage, class TInputDTM, class TLUT>
        void
        MultiSpectralAOTCostFunction<TInputImage, TInputDTM, TLUT>::DerivativeEvaluateErrors(unsigned int & pValueindex,
                double pAOTcurrentD, DerivativeType & pMSDerivative) const
        {
            if (this->m_LUTD.IsNull())
            {
                vnsExceptionBusinessMacro("MultiSpectralAOTCostFunction::LUT D is null.");
            }
            /** Blue band parameter (first band of the AOT bands vector) */
            const unsigned int lBlueBand = m_AOTbands[0];
            /** Red band parameter (second band of the AOT bands vector) */
            const unsigned int lRedBand = m_AOTbands[1];

            const unsigned int lPixelListSize = this->m_OffsetVect.size();
            const double lEpsilon(0.00001);
            const double l_RapEpsilon = 1. / (2. * lEpsilon);

            LUTPointType pointD;

            //** COMPUTE COST GRADIENTS --> loop over selected pixels **//
            // PARTIAL DERIVATIVE (methode d'Accroissements finis) : df/taux=[f(taux+epsilon)-f(taux-epsilon)]/2epsilon

            for (unsigned int j = 0; j < lPixelListSize; j++)
            {
                OffsetType const& lOffset = this->m_OffsetVect.at(j);
                const LUTPointValueType blueIPTOACpix = static_cast<LUTPointValueType>((this->m_IPTOACSub.GetPixel(lOffset))[lBlueBand]); // R_toa*_sub(D,blue)
                const LUTPointValueType redIPTOACpix = static_cast<LUTPointValueType>((this->m_IPTOACSub.GetPixel(lOffset))[lRedBand]); // R_toa*_sub(D,blue)

                //** PARTIAL DERIVATIVE WITH REGARD TO AOTCurrent(D) **//
                //----dblue_cost/dtauD=[blue_cost(tauD+epsilon)-blue_cost(tauD-epsilon)]/2epsilon

                pointD[0] = static_cast<LUTPointValueType>(this->m_DTMSub.GetPixel(lOffset));                 // altitude

                // Compute blue_cost(tau-epsilon) et red_cost(tau-epsilon)
                double lTauD_m = pAOTcurrentD - lEpsilon;

                pointD[1] = static_cast<LUTPointValueType>(lTauD_m); // AOT

                // Interpolations
                pointD[2] = blueIPTOACpix;
                LUTPixelType blueInterpValue_m = this->m_LUTD->InterpolateValue(pointD);

                pointD[2] = redIPTOACpix;
                LUTPixelType redInterpValue_m = this->m_LUTD->InterpolateValue(pointD);

                // Estimated surface Reflectances
                const double RsurfEstBlue_m = static_cast<double>(blueInterpValue_m[lBlueBand]);
                const double RsurfSimBlue_m = static_cast<double>(m_SlopeMS * redInterpValue_m[lRedBand]) + m_YInterceptMS;

                const double blueCost_m = this->m_Weq1 * (RsurfEstBlue_m - RsurfSimBlue_m);

                // Compute blue_cost(tau+epsilon) and red_cost(tau+epsilon)
                double lTauD_p = pAOTcurrentD + lEpsilon;

                pointD[1] = static_cast<LUTPointValueType>(lTauD_p); // AOT

                // Interpolations
                pointD[2] = blueIPTOACpix;
                LUTPixelType blueInterpValue_p = this->m_LUTD->InterpolateValue(pointD);

                pointD[2] = redIPTOACpix;
                LUTPixelType redInterpValue_p = this->m_LUTD->InterpolateValue(pointD);

                // Estimated surface reflectances
                const double RsurfEstBlue_p = static_cast<double>(blueInterpValue_p[lBlueBand]);
                const double RsurfSimBlue_p = static_cast<double>(m_SlopeMS * redInterpValue_p[lRedBand]) + m_YInterceptMS;

                const double blueCost_p = this->m_Weq1 * (RsurfEstBlue_p - RsurfSimBlue_p);

                // Gradient
                double dblueCost = (blueCost_p - blueCost_m) * l_RapEpsilon;

                pMSDerivative[0][pValueindex] = dblueCost;
                // Increment the index
                pValueindex++;

            } //pixel loop

        }

    template <class TInputImage, class TInputDTM, class TLUT>
        void
        MultiSpectralAOTCostFunction<TInputImage, TInputDTM, TLUT>::PrintSelf(std::ostream& os, itk::Indent indent) const
        {
            Superclass::PrintSelf(os, indent);

            os << indent << "m_WeqMS: " << this->m_Weq1 << std::endl;
            os << indent << "m_SlopeMS: " << m_SlopeMS << std::endl;
            os << indent << "m_YInterceptMS: " << m_YInterceptMS << std::endl;
        }

} //end of namespace

#endif /* __vnsMultiSpectralAOTCostFunction_txx */
