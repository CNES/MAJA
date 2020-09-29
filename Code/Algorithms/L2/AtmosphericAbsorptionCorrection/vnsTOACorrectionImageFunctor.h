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
 * VERSION : 1.0.0 : FA : LAIG-FA-MAC-1988-CNES : 21 octobre 2016 : Correction qualite (code mort)          *
 * VERSION : 1.0.0 : FA : LAIG-FA-MAC-1990-CNES : 13 octobre 2016 : Correction qualite (TO DO)              *
 * VERSION : 4-0-0 : FA : LAIG-FA-MAC-120653-CS : 3 avril 2014 : Correction de règles de qualité			*
 * VERSION : 1-0-0 : <TypeFT> : <NumFT> : 23 avr. 2010 : Creation                                                           
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id$
 *                                                                                                          *
 ************************************************************************************************************/
#ifndef __vnsTOACorrectionImageFunctor_h
#define __vnsTOACorrectionImageFunctor_h

#include "vnsSmacCoefContainer.h"
#include "itkFixedArray.h"
#include "otbMath.h"
#include "vnsMacro.h"
#include "vnsMath.h"
#include <cmath>

namespace vns
{
    /** \class  TOACorrectionImageFunctor
     * \brief This class implements the smac_inverse_tg function.
     * For more information, please refers to the SMAC library.
     *
     *
     * \author CS Systemes d'Information
     *
     * \ingroup L2
     *
     */
    namespace Functor
    {
        template<class TTOAInputPixel, class TWaterInputPixel, class TPressureInputPixel, class TOutputPixel>
            class TOACorrectionImageFunctor
            {
            public:
                typedef TOACorrectionImageFunctor<TTOAInputPixel, TWaterInputPixel, TPressureInputPixel, TOutputPixel> TOACorrectionImageFunctorType;

                typedef TTOAInputPixel TOAPixelType;
                typedef TWaterInputPixel WaterInputPixel;
                typedef TPressureInputPixel PressureInputPixel;
                typedef TOutputPixel OutputPixelType;
                typedef typename TOAPixelType::ValueType TOAInternalPixelType;
                typedef typename OutputPixelType::ValueType OutputInternalPixelType;
                typedef std::vector<SmacCoefContainer> SmacCoefContainerListType;
                typedef std::vector<double> DoubleVectorType;

                TOACorrectionImageFunctor(void) :
                        m_OzoneAmount(1.), m_ThetaS(0.), m_ThetaV(1, 0.), m_InvCosThetaS(0.), m_InvCosThetaV(1, 0.), m_AirMass(1, 0.), m_L2NoData(
                                0.)
                {
                    m_SmacCoefsList.clear();
                    m_CoefPressureAmount = 1. / 1013.25;
                }

                virtual
                ~TOACorrectionImageFunctor()
                {
                }

                /** Set the vector of SMAC coefficient containers. */
                void
                SetSmacCoefsList(const SmacCoefContainerListType & smacList)
                {
                    m_SmacCoefsList = smacList;
                }

                /**  Add one SmacCoefContainer to the list. */
                void
                AddSmacCoef(const SmacCoefContainer & smac)
                {
                    m_SmacCoefsList.push_back(smac);
                }

                /** Get the vector of SMAC coefficient containers. */
                SmacCoefContainerListType
                GetSmacCoefsList() const
                {
                    return m_SmacCoefsList;
                }

                /** Get the vector of SMAC coefficient containers. */
                SmacCoefContainerListType
                GetSmacCoefsList()
                {
                    return m_SmacCoefsList;
                }

                /** Set the ozone amount. */
                void
                SetOzoneAmount(const double & ozAmount)
                {
                    m_OzoneAmount = ozAmount;
                }

                /** Get the ozone amount. */
                double
                GetOzoneAmount() const
                {
                    return m_OzoneAmount;
                }

                /**  Set Solar zenith angle. */
                void
                SetThetaS(const double & thetaS)
                {
                    m_ThetaS = thetaS;
                    m_InvCosThetaS = 1. / std::cos(m_ThetaS * otb::CONST_PI_180);
                    const unsigned int size = m_InvCosThetaV.size();
                    if (m_AirMass.size() != size)
                    {
                        vnsStaticExceptionBusinessMacro(
                                "Internal error: the size of the AirMass vector is "<<m_AirMass.size()<<" and we want "<<size<<" values (m_InvCosThetaV vector)")
                    }
                    for (unsigned int i = 0; i < size; i++)
                    {
                        m_AirMass[i] = m_InvCosThetaS + m_InvCosThetaV[i];
                    }
                }
                /**  Get solar zenith angle. */
                double
                GetThetaS() const
                {
                    return m_ThetaS;
                }
                /**  Get solar zenith angle cosine value . */
                double
                GetInvCosThetaS() const
                {
                    return m_InvCosThetaS;
                }

                /**  Set view zenith angle. */
                void
                SetThetaV(const DoubleVectorType & thetaV)
                {
                    m_ThetaV = thetaV;
                    const unsigned int size = m_ThetaV.size();
                    m_InvCosThetaV.clear();
                    m_AirMass.clear();
                    for (unsigned int i = 0; i < size; i++)
                    {
                        double val = (1. / std::cos(m_ThetaV[i] * otb::CONST_PI_180));
                    m_InvCosThetaV.push_back(val);
                    m_AirMass.push_back(m_InvCosThetaS + val);
                }
            }
            /**  Get view zenith angle. */
            DoubleVectorType
            GetThetaV() const
            {
                return m_ThetaV;
            }

            /**  Get view zenith angle cosine value . */
            DoubleVectorType
            GetInvCosThetaV() const
            {
                return m_InvCosThetaV;
            }

            /**  Get view zenith angle cosine value . */
            DoubleVectorType
            GetAirMass() const
            {
                return m_AirMass;
            }

            /** L2 No_Data accessor */
            void
            SetL2NoData(const RealNoDataType & val)
            {
                m_L2NoData = val;
            }

            void
            operator()(const TOAPixelType & toaPix, const WaterInputPixel & watPix, const PressureInputPixel & pressPix,const unsigned int &nbBands, OutputPixelType & outPix) const
            {
                                // Pressure amount
                                const double l_pressure(static_cast<double>(pressPix) * m_CoefPressureAmount);
                                // Water amount
                                const double l_water(static_cast<double>(watPix));
                                // For each spectral band
                                for (unsigned int i = 0; i < nbBands; i++)
                                {
                                    const SmacCoefContainer & smacCoef = m_SmacCoefsList[i];
                                    const double lAirMass = m_AirMass[i];

                                    // Initialize the content of the different gaseous molecules as a function of the pressure
                                    const double l_uo2 = vcl_pow(l_pressure, smacCoef.GetPO2());
                                    const double l_uco2 = vcl_pow(l_pressure, smacCoef.GetPCO2());
                                    const double l_uch4 = vcl_pow(l_pressure, smacCoef.GetPCH4());
                                    const double l_uno2 = vcl_pow(l_pressure, smacCoef.GetPNO2());
                                    const double l_uco = vcl_pow(l_pressure, smacCoef.GetPCO());

                                    // Compute the total gaseous transmission as the product of the transmission of each molecule
                                    const double l_tg =  vcl_exp (
                                            smacCoef.GetAO3() * vcl_pow((m_OzoneAmount * lAirMass), smacCoef.GetNO3()) +
                                            smacCoef.GetAH2O() * vcl_pow((l_water * lAirMass), smacCoef.GetNH2O()) +
                                            smacCoef.GetAO2() * vcl_pow((l_uo2 * lAirMass), smacCoef.GetNO2()) +
                                            smacCoef.GetACO2() * vcl_pow((l_uco2 * lAirMass), smacCoef.GetNCO2()) +
                                            smacCoef.GetACH4() * vcl_pow((l_uch4 * lAirMass), smacCoef.GetNCH4()) +
                                            smacCoef.GetANO2() * vcl_pow((l_uno2 * lAirMass), smacCoef.GetNNO2()) +
                                            smacCoef.GetACO() * vcl_pow((l_uco * lAirMass), smacCoef.GetNCO()) );

                                    // Surface reflectance
                                    // Since t_g is the result of product comming from exponentional, it can't be negative.
                                    // Here a very small value of t_g comes from a problem in the SMAC coef.
                                    // This is a result of a conversation with M. Huc.
                                    if (vnsEqualsDoubleMacro(l_tg, 0.0) == true)
                                    {
                                        outPix[i] = static_cast<OutputInternalPixelType>(m_L2NoData);
                                    }
                                    else
                                    {
                                        // The reflectance after correction of the absorbing atmospheric molecules is
                                        const double l_surfRef = static_cast<double>(toaPix[i]) / l_tg;

                                        // Set the NaN values to NoData
                                        if (std::isnan(l_surfRef) == false)
                                        {
                                            outPix[i] = static_cast<OutputInternalPixelType>(l_surfRef);
                                        }
                                        else
                                        {
                                            outPix[i] = m_L2NoData;
                                        }
                                    }
                                }
                            }

                        protected:
                            /** Ozone Amount */
                            double m_OzoneAmount;
                            /**  Solar zenith angle in degreee. */
                            double m_ThetaS;
                            /**  View zenith angle in degree. */
                            DoubleVectorType m_ThetaV;
                            /**  Solar zenith angle inverse cosinus value. */
                            double m_InvCosThetaS;
                            /**  View zenith angle inverse cosinus value. */
                            DoubleVectorType m_InvCosThetaV;
                            /** Air mass. */
                            DoubleVectorType m_AirMass;
                            /** Vector of SMAC coefficient containers. */
                            SmacCoefContainerListType m_SmacCoefsList;
                            /** L2 No_Data value */
                            RealNoDataType m_L2NoData;

                            double m_CoefPressureAmount;

                        };

                    }
        } // End namespace vns

#endif /* __vnsTOACorrectionImageFunctor_h */
