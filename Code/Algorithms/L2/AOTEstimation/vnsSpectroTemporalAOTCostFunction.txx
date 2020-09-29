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
 * VERSION : 1.0.0 : FA : LAIG-FA-MAC-148399-CS : 27 octobre 2016 : Audit code - Correction qualite         *
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

#ifndef __vnsSpectroTemporalAOTCostFunction_txx
#define __vnsSpectroTemporalAOTCostFunction_txx

#include "vnsSpectroTemporalAOTCostFunction.h"

namespace vns
{

    template <class TInputImage, class TInputDTM, class TLUT>
        SpectroTemporalAOTCostFunction<TInputImage, TInputDTM,  TLUT>::SpectroTemporalAOTCostFunction() :
        m_MTIsValid(false), m_MSIsValid(false)
        {
            m_MSAOTCostFunction = MultiSpectralAOTCostFunctionType::New();
        }

    template <class TInputImage, class TInputDTM, class TLUT>
        typename SpectroTemporalAOTCostFunction<TInputImage, TInputDTM,  TLUT>::MeasureType
        SpectroTemporalAOTCostFunction<TInputImage, TInputDTM,  TLUT>::GetValue(
                const ParametersType & parameters) const
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
            this->EvaluateErrors(valueindex,lAOTcurrentD,lAOTcurrentDm1,this->m_Measure);

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
        SpectroTemporalAOTCostFunction<TInputImage, TInputDTM,  TLUT>::PrintSelf( std::ostream& os, itk::Indent indent ) const
        {
            Superclass::PrintSelf(os,indent);
        }



    template <class TInputImage, class TInputDTM, class TLUT>
        void
        SpectroTemporalAOTCostFunction<TInputImage, TInputDTM,  TLUT>::EvaluateErrors(unsigned int & pValueindex, double pAOTcurrentD, double pAOTcurrentDm1, MeasureType & pSTMeasure) const
        {

            if (m_MTIsValid == true)
            {
                // Call the multi temporal EvaluateErrors method that initializes the m_Measure vector
                // used for the least squares inversion
                Superclass::EvaluateErrors(pValueindex, pAOTcurrentD, pAOTcurrentDm1, pSTMeasure);
            }

            if (m_MSIsValid == true)
            {
                // Then, add the measures of the multi spectral method
                m_MSAOTCostFunction->EvaluateErrors(pValueindex, pAOTcurrentD, pSTMeasure);
            }

        }

    template <class TInputImage, class TInputDTM, class TLUT>
         void
         SpectroTemporalAOTCostFunction<TInputImage, TInputDTM,  TLUT>::DerivativeEvaluateErrors(unsigned int & pValueindex, double pAOTcurrentD, double pAOTcurrentDm1, DerivativeType & pSTDerivative) const
         {
             if(static_cast<LUTPointerType>(this->GetLUTD()).IsNull())
             {
                 vnsExceptionBusinessMacro("SpectroTemporalAOTCostFunction::LUT D is null.");
             }

             if(static_cast<LUTPointerType>(this->GetLUTDm1()).IsNull() )
             {
                 vnsExceptionBusinessMacro("SpectroTemporalAOTCostFunction::LUT Dm1 is null.");
             }

             if (m_MTIsValid == true)
            {
                // Call the multi temporal DerivativeEvaluateErrors method that initializes the m_Derivative vector
                // used for the least square s inversion
                this->DerivativeEvaluateErrors(pValueindex, pAOTcurrentD, pAOTcurrentDm1, pSTDerivative);
            }
            if (m_MSIsValid == true)
            {
                // Then, add the measures to the m_Derivative vector of the multi spectral method
                m_MSAOTCostFunction->DerivativeEvaluateErrors(pValueindex, pAOTcurrentD, pSTDerivative);
            }


         }

} //end of namespace

#endif /* __vnsSpectroTemporalAOTCostFunction_txx */
