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
 * VERSION : 1.0.0 : FA : LAIG-FA-MAC-148399-CS : 27 octobre 2016 : Audit code - Correction qualite         *
 * VERSION : 4-0-0 : FA : LAIG-FA-MAC-120653-CS : 7 mai 2014 : Correction de règles qualité ou commentaires *
 * VERSION : 2-0-0 : DM : LAIG-DM-MAC-146-CNES : 14 oct. 2011 : Création des différentes méthodes de calcul des AOT *
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id$
 *                                                                                                          *
 ************************************************************************************************************/
#ifndef __vnsAOTCostFunctionBase_txx
#define __vnsAOTCostFunctionBase_txx

#include "vnsAOTCostFunctionBase.h"

namespace vns
{

    template<class TInputImage, class TInputDTM, class TLUT>
        AOTCostFunctionBase<TInputImage, TInputDTM, TLUT>::AOTCostFunctionBase() :
                m_DTMsub(0), m_DarkestPixelAOT(0.), m_Weq1(0.), m_Wdark(0.), m_Wbounds(0.), m_AOTmin(0.), m_AOTmax(0.),
				m_UseCamsData(false), m_KPondCams(0.2), m_CAMSAot(0), m_SpaceDimension(2), m_RangeDimension(99)
        {
            m_RangeDimension = 99; // 99 = 49(MAX Window size) * 2 (two costs) * NbAOTBand + 1 (for the latest cost)

            m_Measure.SetSize(m_RangeDimension);
            m_Derivative.SetSize(m_SpaceDimension, m_RangeDimension);
            m_OffsetVect.clear();
            m_AOTmax = itk::NumericTraits<double>::min();
        }

    template<class TInputImage, class TInputDTM, class TLUT>
        unsigned int
        AOTCostFunctionBase<TInputImage, TInputDTM, TLUT>::GetNumberOfParameters(void) const
        {
            return m_SpaceDimension;
        }

    template<class TInputImage, class TInputDTM, class TLUT>
        unsigned int
        AOTCostFunctionBase<TInputImage, TInputDTM, TLUT>::GetNumberOfValues(void) const
        {
            return m_RangeDimension;
        }

    template<class TInputImage, class TInputDTM, class TLUT>
        void
        AOTCostFunctionBase<TInputImage, TInputDTM, TLUT>::PrintSelf(std::ostream& os, itk::Indent indent) const
        {
            Superclass::PrintSelf(os, indent);
        }

    template<class TInputImage, class TInputDTM, class TLUT>
        void
        AOTCostFunctionBase<TInputImage, TInputDTM, TLUT>::FindAOTmax(void)
        {
            if (m_LUTD.IsNotNull())
            {
                // Initialization
                m_AOTmax = m_AOTmin;
                LUTParameterValuesType lParamAOT;

                // Get the second parameter list : LUT_indexes.AOT
                lParamAOT = m_LUTD->GetParameterValues(1);
                const std::vector<double> lVal = lParamAOT.ParameterValues;
                const unsigned int lSize = lVal.size();

                // Loops on the pixel bands
                for (unsigned int i = 0; i < lSize; i++)
                {
                    if (m_AOTmax < lVal.at(i))
                    {
                        m_AOTmax = lVal.at(i);
                    }
                }
            }
        }

    template<class TInputImage, class TInputDTM, class TLUT>
        double
        AOTCostFunctionBase<TInputImage, TInputDTM, TLUT>::LimitAOT(const double & pAOT) const
        {
            double lAOT = pAOT;
            if (lAOT > m_AOTmax)
            {
                lAOT = m_AOTmax;
            }
            if (lAOT < m_AOTmin)
            {
                lAOT = m_AOTmin;
            }
            return lAOT;
        }

    template<class TInputImage, class TInputDTM, class TLUT>
        double
        AOTCostFunctionBase<TInputImage, TInputDTM, TLUT>::EvaluateErrorBound(double pAOTmin, double pAOTmax, double pAOTcurrentT) const
        {
            double lErrorBound = 0;
            double lDiff = 0;

            if (pAOTcurrentT < pAOTmin)
            {
                lDiff = pAOTmin - pAOTcurrentT;
                lErrorBound = m_Wbounds * (lDiff * lDiff);
            }
            if (pAOTcurrentT > pAOTmax)
            {
                lDiff = pAOTmax - pAOTcurrentT;
                lErrorBound = m_Wbounds * (lDiff * lDiff);
            }
            return lErrorBound;
        }

    template<class TInputImage, class TInputDTM, class TLUT>
        double
        AOTCostFunctionBase<TInputImage, TInputDTM, TLUT>::DerivativeEvaluateErrorBound(double pAOTmin, double pAOTmax,
                double pAOTcurrentT) const
        {
            double lDerivativeErrorBound(0.);
            double lDiff(0.);
            if (pAOTcurrentT < pAOTmin)
            {
                lDiff = pAOTcurrentT - pAOTmin;
                lDerivativeErrorBound = 2 * m_Wbounds * lDiff;
            }
            else if (pAOTcurrentT > pAOTmax)
            {
                lDiff = pAOTcurrentT - pAOTmax;
                lDerivativeErrorBound = 2 * m_Wbounds * lDiff;
            }
            else
            {
                lDerivativeErrorBound = 0.;
            }
            return lDerivativeErrorBound;
        }

    template<class TInputImage, class TInputDTM, class TLUT>
        double
        AOTCostFunctionBase<TInputImage, TInputDTM, TLUT>::EvaluateDarkError(double pAOTcurrentD) const
        {
            double lDarkError = 0.;
            if (pAOTcurrentD > m_DarkestPixelAOT)
            {
                const double lDiff = pAOTcurrentD - m_DarkestPixelAOT;
                lDarkError = m_Wdark * (lDiff * lDiff);
            }
            return lDarkError;
        }

    template<class TInputImage, class TInputDTM, class TLUT>
        double
        AOTCostFunctionBase<TInputImage, TInputDTM, TLUT>::DerivativeEvaluateDarkError(double pAOTcurrentD) const
        {
            double lDerivativeDarkError = 0.;

            if (pAOTcurrentD > m_DarkestPixelAOT)
            {
                const double lDiff = pAOTcurrentD - m_DarkestPixelAOT;
                lDerivativeDarkError = 2 * m_Wdark * (lDiff);
            }
            return lDerivativeDarkError;
        }

} //end of namespace

#endif /* __vnsAOTCostFunctionBase_txx */
