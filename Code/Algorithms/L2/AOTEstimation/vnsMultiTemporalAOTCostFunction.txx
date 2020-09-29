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
 * VERSION : 1.0.0 : FA : LAIG-FA-MAC-148399-CS : 26 octobre 2016 : Audit code - Correction qualite         *
 * VERSION : 2-0-0 : DM : LAIG-DM-MAC-146-CNES : 18 oct. 2011 : Méthode multi temporelle                         *
 *                                                                                                          *
 * VERSION : 1-0-0-3 : FT : 65407 : 23 sept. 2011 : Initialisation de la variable m_AOTmax.                   *
 * VERSION : 1-0-0 : <TypeFT> : <NumFT> : 26 juin 2010 : Creation                                                           
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id$
 *                                                                                                          *
 ************************************************************************************************************/

#ifndef __vnsMultiTemporalAOTCostFunction_txx
#define __vnsMultiTemporalAOTCostFunction_txx

#include "vnsMultiTemporalAOTCostFunction.h"

namespace vns
{

    template<class TInputImage, class TInputDTM, class TLUT>
        MultiTemporalAOTCostFunction<TInputImage, TInputDTM, TLUT>::MultiTemporalAOTCostFunction() :
                m_Weq2(1)
        {
            m_AOTbandsD.clear();
            m_AOTbandsReflectanceCompositeDm1.clear();
            m_AOTbandsLUTCompositeDm1.clear();
        }

    template<class TInputImage, class TInputDTM, class TLUT>
        typename MultiTemporalAOTCostFunction<TInputImage, TInputDTM, TLUT>::MeasureType
        MultiTemporalAOTCostFunction<TInputImage, TInputDTM, TLUT>::GetValue(const ParametersType & parameters) const
        {

            // Test that the current values stay within the LUT indexes
            double lAOTcurrentD = parameters[0];
            double lAOTcurrentDm1 = parameters[1];

            // Error bound computation
            const double lErrorBoundD = this->EvaluateErrorBound(this->m_AOTmin, this->m_AOTmax, lAOTcurrentD);
            const double lErrorBoundDm1 = this->EvaluateErrorBound(this->m_AOTmin, this->m_AOTmax, lAOTcurrentDm1);

            // Test that the current values stay within the LUT indexes
            lAOTcurrentD = this->LimitAOT(parameters[0]);
            lAOTcurrentDm1 = this->LimitAOT(parameters[1]);

            // Main cost computation
            unsigned int valueindex = 0;
            this->EvaluateErrors(valueindex, lAOTcurrentD, lAOTcurrentDm1, this->m_Measure);

            //Add CAMS to vector if used
            if(this->m_UseCamsData)
            {
                this->m_Measure[valueindex] = this->m_KPondCams * (lAOTcurrentD -  this->m_CAMSAot);
                valueindex++;
            }

            // Dark Error computation
            const double lDarkErrorD = this->EvaluateDarkError(lAOTcurrentD);

            this->m_Measure[valueindex] = lErrorBoundD + lErrorBoundDm1 + lDarkErrorD;

            return this->m_Measure;
        }

    template<class TInputImage, class TInputDTM, class TLUT>
        void
        MultiTemporalAOTCostFunction<TInputImage, TInputDTM, TLUT>::GetDerivative(const ParametersType & parameters,
                DerivativeType & derivative) const
        {
            // Test that the current values stay within the LUT indexes
            const double lAOTcurrentD = this->LimitAOT(parameters[0]);
            const double lAOTcurrentDm1 = this->LimitAOT(parameters[1]);

            // Main cost computation
            unsigned int valueindex = 0;
            this->DerivativeEvaluateErrors(valueindex, lAOTcurrentD, lAOTcurrentDm1, this->m_Derivative);

            //Add CAMS to vector if used
            if(this->m_UseCamsData)
            {
            	this->m_Derivative[0][valueindex] = 0;
            	this->m_Derivative[1][valueindex] = this->m_KPondCams;
            	valueindex++;
            }

            // Error bound computation
            const double lDerivativeErrorBoundD = this->DerivativeEvaluateErrorBound(this->m_AOTmin, this->m_AOTmax, lAOTcurrentD);
            const double lDerivativeErrorBoundDm1 = this->DerivativeEvaluateErrorBound(this->m_AOTmin, this->m_AOTmax, lAOTcurrentDm1);

            // Dark Error computation
            double lDerivativeDarkErrorD = this->DerivativeEvaluateDarkError(lAOTcurrentD);

            this->m_Derivative[0][valueindex] = lDerivativeErrorBoundDm1;
            this->m_Derivative[1][valueindex] = lDerivativeErrorBoundD + lDerivativeDarkErrorD;

            derivative = this->m_Derivative;

        }

    template<class TInputImage, class TInputDTM, class TLUT>
        void
        MultiTemporalAOTCostFunction<TInputImage, TInputDTM, TLUT>::PrintSelf(std::ostream& os, itk::Indent indent) const
        {
            Superclass::PrintSelf(os, indent);
        }

    template<class TInputImage, class TInputDTM, class TLUT>
        void
        MultiTemporalAOTCostFunction<TInputImage, TInputDTM, TLUT>::EvaluateErrors(unsigned int & pValueindex, double pAOTcurrentD,
                double pAOTcurrentDm1, MeasureType & pMTMeasure) const
        {

            if (this->m_LUTD.IsNull())
            {
                vnsExceptionBusinessMacro("MultiTemporalAOTCostFunction::LUT D is null.");
            }

            if (m_LUTDm1.IsNull())
            {
                vnsExceptionBusinessMacro("MultiTemporalAOTCostFunction::LUT Dm1 is null.");
            }

            const unsigned int lNbOfUsedBands = this->m_AOTbandsD.size();
            const unsigned int lPixelListSize = this->m_OffsetVect.size();
            unsigned int lUsedBandD(0);
            unsigned int lUsedBandReflectanceCompositeDm1(0);
            unsigned int lUsedBandLUTCompositeDm1(0);
            unsigned int b(0);
            unsigned int j(0);
            LUTPointType pointD;
            LUTPointType pointDm1;
            OffsetType lOffset;

            for (j = 0; j < lPixelListSize; j++)
            {

                lOffset = this->m_OffsetVect[j];
                const DTMPixelType DTMSubPix = this->m_DTMSub.GetPixel(lOffset);
                const PixelType & IPTOACSubPixel = this->m_IPTOACSub.GetPixel(lOffset);
                const PixelType & L2inRTAPixel = m_L2inRTA.GetPixel(lOffset);
                const PixelType & L2inRTCPixel = m_L2inRTC.GetPixel(lOffset);
                pointD[0] = static_cast<LUTPointValueType>(DTMSubPix);                 // altitude
                pointD[1] = static_cast<LUTPointValueType>(pAOTcurrentD);              // AOT

                pointDm1[0] = static_cast<LUTPointValueType>(DTMSubPix);               // altitude
                pointDm1[1] = static_cast<LUTPointValueType>(pAOTcurrentDm1);          // AOT

                for (b = 0; b < lNbOfUsedBands; b++)
                {
                    lUsedBandD = this->m_AOTbandsD[b];
                    lUsedBandReflectanceCompositeDm1 = this->m_AOTbandsReflectanceCompositeDm1[b];
                    lUsedBandLUTCompositeDm1 = this->m_AOTbandsLUTCompositeDm1[b];

                    pointD[2] = static_cast<LUTPointValueType>(IPTOACSubPixel[lUsedBandD]); // R_toa*_sub(D,b)
                    pointDm1[2] = static_cast<LUTPointValueType>(L2inRTAPixel[lUsedBandReflectanceCompositeDm1]); // L2in.R_toa*_sub(D-1,b)

                    // Reduced LUT at the date D (3dim) with 4 bands (LUTPixelType is a VariableLengthVector of size 4)
                    const LUTPixelType interpValueD = this->m_LUTD->InterpolateValue(pointD);

                    // Reduced LUT at the date LPD
                    const LUTPixelType interpValueDm1 = m_LUTDm1->InterpolateValue(pointDm1);

                    // Estimated Reflectance surface
                    const double RsurfEstD = static_cast<double>(interpValueD[lUsedBandD]);
                    const double RsurfEstDm1 = static_cast<double>(interpValueDm1[lUsedBandLUTCompositeDm1]);

                    const double cost1 = RsurfEstD - RsurfEstDm1;
                    const double cost2 = RsurfEstD - L2inRTCPixel[lUsedBandReflectanceCompositeDm1];

                    pMTMeasure[pValueindex] = this->m_Weq1 * cost1;
                    pValueindex++;
                    pMTMeasure[pValueindex] = m_Weq2 * cost2;
                    pValueindex++;

                } // end for used bands
            } // pixel list loop

        }

    template<class TInputImage, class TInputDTM, class TLUT>
        void
        MultiTemporalAOTCostFunction<TInputImage, TInputDTM, TLUT>::DerivativeEvaluateErrors(unsigned int & pValueindex,
                double pAOTcurrentD, double pAOTcurrentDm1, DerivativeType & pMTDerivative) const
        {
            if (this->m_LUTD.IsNull())
            {
                vnsExceptionBusinessMacro("MultiTemporalAOTCostFunction::LUT D is null.");
            }

            if (m_LUTDm1.IsNull())
            {
                vnsExceptionBusinessMacro("MultiTemporalAOTCostFunction::LUT Dm1 is null.");
            }

            const unsigned int lNbOfUsedBands = this->m_AOTbandsD.size();
            const unsigned int lPixelListSize = this->m_OffsetVect.size();
            const double lEpsilon(0.00001);
            const double l_RapEpsilon = 1. / (2. * lEpsilon);

            LUTPointType pointD;
            LUTPointType pointDm1;
            for (unsigned int b = 0; b < lNbOfUsedBands; b++)
            {
            	unsigned int lUsedBandD = this->m_AOTbandsD.at(b);
            	unsigned int lUsedBandReflectanceCompositeDm1 = this->m_AOTbandsReflectanceCompositeDm1.at(b);
            	unsigned int lUsedBandLUTCompositeDm1 = this->m_AOTbandsLUTCompositeDm1[b];
                for (unsigned int j = 0; j < lPixelListSize; j++)
                {

                	OffsetType lOffset = this->m_OffsetVect.at(j);

                    //** DERIVEES PARTIELLES PAR RAPPORT A TAU1 **//
                    //----dcout1/dtau1=0
                    //----dcout2/dtau1=[cout2(tau1+epsilon,tau2)-cout2(tau1-epsilon,tau2)]/2epsilon

                    const double lTauD = pAOTcurrentD;
                    const double lTauDm1_m = pAOTcurrentDm1 - lEpsilon;
                    const double lTauDm1_p = pAOTcurrentDm1 + lEpsilon;

                    pointD[0] = static_cast<LUTPointValueType>(this->m_DTMSub.GetPixel(lOffset));                 // altitude
                    pointD[1] = static_cast<LUTPointValueType>(lTauD);                                      // AOT
                    pointD[2] = static_cast<LUTPointValueType>((this->m_IPTOACSub.GetPixel(lOffset))[lUsedBandD]); // R_toa*_sub(D,b)

                    pointDm1[0] = static_cast<LUTPointValueType>(this->m_DTMSub.GetPixel(lOffset));                // altitude
                    pointDm1[2] = static_cast<LUTPointValueType>(m_L2inRTA.GetPixel(lOffset)[lUsedBandReflectanceCompositeDm1]); // L2in.R_toa*_sub(D-1,b)

                    // Reduced LUT at the date D
                    LUTPixelType interpValueD = this->m_LUTD->InterpolateValue(pointD);

                    // Reduced LUT at the date LPD (3dim) with 4 bands (LUTPixelType is a VariableLengthVector of size 4)
                    pointDm1[1] = static_cast<LUTPointValueType>(lTauDm1_m);                                  // AOT
                    LUTPixelType interpValueDm1_m = m_LUTDm1->InterpolateValue(pointDm1);

                    // Reduced LUT at the date LPD (3dim) with 4 bands (LUTPixelType is a VariableLengthVector of size 4)
                    pointDm1[1] = static_cast<LUTPointValueType>(lTauDm1_p);                                  // AOT
                    LUTPixelType interpValueDm1_p = m_LUTDm1->InterpolateValue(pointDm1);

                    // Estimated Reflectance surface
                    const double RsurfEstD = static_cast<double>(interpValueD[lUsedBandD]);
                    const double RsurfEstDm1_m = static_cast<double>(interpValueDm1_m[lUsedBandLUTCompositeDm1]);
                    const double RsurfEstDm1_p = static_cast<double>(interpValueDm1_p[lUsedBandLUTCompositeDm1]);

                    double cost1_m = (RsurfEstDm1_m - RsurfEstD);
                    double cost1_p = (RsurfEstDm1_p - RsurfEstD);

                    // Gradient
                    double dcost1 = (cost1_p - cost1_m) * l_RapEpsilon;

                    pMTDerivative[0][pValueindex] = 0;
                    pMTDerivative[0][pValueindex + 1] = this->m_Weq1 * dcost1;

                    //** DERIVEES PARTIELLES PAR RAPPORT A TAU2 **//
                    //----dcout1/dtau1=[cout1(tau1,tau2+epsilon)-cout1(tau1,tau2-epsilon)]/2epsilon
                    //----dcout2/dtau1=[cout2(tau1,tau2+epsilon)-cout2(tau1,tau2-epsilon)]/2epsilon

                    const double lTauDm1 = pAOTcurrentDm1;
                    const double lTauD_m = pAOTcurrentD - lEpsilon;
                    const double lTauD_p = pAOTcurrentD + lEpsilon;

                    // Interpolations
                    pointD[1] = static_cast<LUTPointValueType>(lTauD_m);                               // AOT
                    LUTPixelType interpValueD_m = this->m_LUTD->InterpolateValue(pointD);

                    pointD[1] = static_cast<LUTPointValueType>(lTauD_p);                               // AOT
                    LUTPixelType interpValueD_p = this->m_LUTD->InterpolateValue(pointD);

                    pointDm1[1] = static_cast<LUTPointValueType>(lTauDm1);                                  // AOT
                    LUTPixelType interpValueDm1 = m_LUTDm1->InterpolateValue(pointDm1);

                    // Estimated Reflectance surface
                    const double RsurfEstD_m = static_cast<double>(interpValueD_m[lUsedBandD]);
                    const double RsurfEstD_p = static_cast<double>(interpValueD_p[lUsedBandD]);
                    const double RsurfEstDm1 = static_cast<double>(interpValueDm1[lUsedBandLUTCompositeDm1]);

                    cost1_m = (RsurfEstDm1 - RsurfEstD_m);
                    cost1_p = (RsurfEstDm1 - RsurfEstD_p);
                    const double cost2_m = (RsurfEstD_m - m_L2inRTC.GetPixel(lOffset)[lUsedBandReflectanceCompositeDm1]);
                    const double cost2_p = (RsurfEstD_p - m_L2inRTC.GetPixel(lOffset)[lUsedBandReflectanceCompositeDm1]);

                    // Gradient
                    dcost1 = (cost1_p - cost1_m) * l_RapEpsilon;
                    const double dcost2 = (cost2_p - cost2_m) * l_RapEpsilon;

                    pMTDerivative[1][pValueindex] = m_Weq2 * dcost2;
                    pMTDerivative[1][pValueindex + 1] = this->m_Weq1 * dcost1;
                    // Increment the index
                    pValueindex = pValueindex + 2;
                }
            }
        }

} //end of namespace

#endif /* __vnsMultiTemporalAOTCostFunction_txx */
