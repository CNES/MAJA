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
 * VERSION : 2.1.0 : FA : LAIG-FA-MAJA-157185-CS : 9 mai 2017 : Correction de règles qualité                *
 * VERSION : 2.0.0 : DM : LAIG-DM-MAJA-981-CNES : 2 mars 2017 : ReducedLutComputation extracted as new algo *
 * VERSION : 2.0.0 : <TypeFT> : <NumFT> : 2 mars. 2017 : Creation                                           *                                                                                                          *
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id$
 *                                                                                                          *
 ************************************************************************************************************/
#ifndef __vnsDirectionalCorrectionCompute_h
#define __vnsDirectionalCorrectionCompute_h

#include "vnsMath.h"
#include <vector>
#include <algorithm>

namespace vns
{
    class DirectionalCorrectionCompute
    {
    private :
        struct AnglesCorr {

            AnglesCorr(const double theta_s, const double phi_s, const double theta_v, const double phi_v)
            {
                m_theta_s = theta_s *  otb::CONST_PI_180;
                m_theta_v = theta_v *  otb::CONST_PI_180;
                m_phi = (phi_s - phi_v) * otb::CONST_PI_180;
                m_xsi_0 =1.5 * otb::CONST_PI_180;
            }

            double m_theta_s;
            double m_theta_v;
            double m_phi;
            double m_xsi_0;

            //function delta
            inline double delta() const {
                return std::sqrt(std::tan(m_theta_s)*std::tan(m_theta_s) +
                        std::tan(m_theta_v)*std::tan(m_theta_v) -
                        2*std::tan(m_theta_s)*std::tan(m_theta_v)*std::cos(m_phi));
            }

            //Air Mass
            inline double masse() const {
                return (1/std::cos(m_theta_s)+1/std::cos(m_theta_v));
            }

            //Function xsi
            inline double cos_xsi() const {
                return ( std::cos(m_theta_s)*std::cos(m_theta_v) +
                        std::sin(m_theta_s)*std::sin(m_theta_v)*std::cos(m_phi) );
            }

            //sin xsi
            inline double sin_xsi() const {
                double x=this->cos_xsi();
                return std::sqrt(1 - x*x);
            }
            //xsi
            inline double xsi() const {
                return std::acos(this->cos_xsi());
            }

            //Function tan
            inline double cos_t() const {
                double trig=std::tan(m_theta_s)*std::tan(m_theta_v)*std::sin(m_phi);
                double d = this->delta();
                double coef=1; //Coef=1 looks good, but Bréon et Vermote use Coef=2
                double cos_t=std::min(std::max(coef/this->masse()*std::sqrt(d*d + trig*trig),-1.0),1.0);
                return cos_t;
            }

            inline double sin_t() const {
                const double x=this->cos_t();
                return std::sqrt(1 - x*x);
            }

            inline double t() const {
                return std::acos(this->cos_t());
            }

            //function FV Ross_Thick,  V stands for Volume
            double FV() const {
                const double FV=this->masse()/otb::CONST_PI*(this->t() - this->sin_t()*this->cos_t() - otb::CONST_PI) +
                        (1+this->cos_xsi())/2/std::cos(m_theta_s)/std::cos(m_theta_v);
                return FV;
            }

            //function FR  Li-Sparse, R stands for Roughness
            double FR() const {
                const double A=1/(std::cos(m_theta_s)+std::cos(m_theta_v));
                const double FR=4/3./otb::CONST_PI*A*((otb::CONST_PI/2- this->xsi())
                        *this->cos_xsi()+this->sin_xsi())*(1+1/(1+this->xsi()/m_xsi_0)) - 1./3;
                return FR;
            }

            double dir_mod(const double kV, const double kR) const {
                const double rho= 1.0 +kV*this->FV() + kR*this->FR();
                return rho;
            }
        };
    public :

        typedef std::vector<double> CoefsVectorType;

        DirectionalCorrectionCompute()
        {
            //Nothing ?
        }

        static CoefsVectorType ComputeCorrectionCoeffs(const double theta_s,const double phi_s,
                const double theta_v0, const double phi_v0, const CoefsVectorType& theta_v,
                const CoefsVectorType& phi_v,const CoefsVectorType& coefs_R,const CoefsVectorType& coefs_V)
        {
            CoefsVectorType corr_coeff(theta_v.size(), 0.0);
            for (unsigned int b = 0; b < theta_v.size();b++){
                // Image angles
                const AnglesCorr IA= AnglesCorr(theta_s,phi_s,theta_v[b],phi_v[b]);
                //Vertical angles
                const AnglesCorr VA= AnglesCorr(theta_s,phi_s,theta_v0,phi_v0);
                corr_coeff[b]=VA.dir_mod(coefs_V[b],coefs_R[b])/IA.dir_mod(coefs_V[b],coefs_R[b]);
            }

            return corr_coeff;
        }





    };//End of DirectionalCorrectionCompute



}//End of vns namespace


#endif
