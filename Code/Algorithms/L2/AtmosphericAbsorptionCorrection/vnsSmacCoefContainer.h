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
 * VERSION : 2.0.0 : FA : LAIG-FA-MAJA-152790-CS : 21 fevrier 2017 : Correction qualite taux de commentaire *
 * VERSION : 1.0.0 : FA : LAIG-FA-MAC-148399-CS : 03 novembre 2016 : Audit code - Correction qualite        *
 * VERSION : 1.0.0 : FA : LAIG-FA-MAC-1990-CNES : 13 octobre 2016 : Correction qualite (TO DO)              *
 * VERSION : 1-0-0 : <TypeFT> : <NumFT> : 23 avr. 2010 : Creation                                                           
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id$
 *                                                                                                          *
 ************************************************************************************************************/
#ifndef __vnsSmacCoefContainer_h
#define __vnsSmacCoefContainer_h

#include "itkFixedArray.h"
#include "itkIndent.h"
#include <vector>

namespace vns
{
    /** \class  SmacCoefContainer
     * \brief Contains the smac inverse coefficient : 49 elements.
     *
     *
     * \author CS Systemes d'Information
     *
     * \ingroup L2
     *
     */

    class SmacCoefContainer : public itk::FixedArray<double, 49>
    {
    public:
        typedef itk::FixedArray<double, 49> Superclass;
        typedef Superclass::ValueType ValueType;

        SmacCoefContainer()
        {
            //const double lol = 0;
            this->Fill(0.);
        }

        SmacCoefContainer(std::vector<double> vect)
        {
            this->SetCoefficients(vect);
        }
        virtual
        ~SmacCoefContainer()
        {
        }

        /** Setters*/
        void
        SetAH2O(ValueType val)
        {
            this->SetElement(0, val);
        }
        void
        SetNH2O(ValueType val)
        {
            this->SetElement(1, val);
        }
        void
        SetAO3(ValueType val)
        {
            this->SetElement(2, val);
        }
        void
        SetNO3(ValueType val)
        {
            this->SetElement(3, val);
        }
        void
        SetAO2(ValueType val)
        {
            this->SetElement(4, val);
        }
        void
        SetNO2(ValueType val)
        {
            this->SetElement(5, val);
        }
        void
        SetPO2(ValueType val)
        {
            this->SetElement(6, val);
        }
        void
        SetACO2(ValueType val)
        {
            this->SetElement(7, val);
        }
        void
        SetNCO2(ValueType val)
        {
            this->SetElement(8, val);
        }
        void
        SetPCO2(ValueType val)
        {
            this->SetElement(9, val);
        }
        void
        SetACH4(ValueType val)
        {
            this->SetElement(10, val);
        }
        void
        SetNCH4(ValueType val)
        {
            this->SetElement(11, val);
        }
        void
        SetPCH4(ValueType val)
        {
            this->SetElement(12, val);
        }
        void
        SetANO2(ValueType val)
        {
            this->SetElement(13, val);
        }
        void
        SetNNO2(ValueType val)
        {
            this->SetElement(14, val);
        }
        void
        SetPNO2(ValueType val)
        {
            this->SetElement(15, val);
        }
        void
        SetACO(ValueType val)
        {
            this->SetElement(16, val);
        }
        void
        SetNCO(ValueType val)
        {
            this->SetElement(17, val);
        }
        void
        SetPCO(ValueType val)
        {
            this->SetElement(18, val);
        }
        void
        SetA0S(ValueType val)
        {
            this->SetElement(19, val);
        }
        void
        SetA1S(ValueType val)
        {
            this->SetElement(20, val);
        }
        void
        SetA2S(ValueType val)
        {
            this->SetElement(21, val);
        }
        void
        SetA3S(ValueType val)
        {
            this->SetElement(22, val);
        }
        void
        SetA0T(ValueType val)
        {
            this->SetElement(23, val);
        }
        void
        SetA1T(ValueType val)
        {
            this->SetElement(24, val);
        }
        void
        SetA2T(ValueType val)
        {
            this->SetElement(25, val);
        }
        void
        SetA3T(ValueType val)
        {
            this->SetElement(26, val);
        }
        void
        SetTauR(ValueType val)
        {
            this->SetElement(27, val);
        }
        void
        SetSR(ValueType val)
        {
            this->SetElement(28, val);
        }
        void
        SetA0TauP(ValueType val)
        {
            this->SetElement(29, val);
        }
        void
        SetA1TauP(ValueType val)
        {
            this->SetElement(30, val);
        }
        void
        SetWO(ValueType val)
        {
            this->SetElement(31, val);
        }
        void
        SetGC(ValueType val)
        {
            this->SetElement(32, val);
        }
        void
        SetA0P(ValueType val)
        {
            this->SetElement(33, val);
        }
        void
        SetA1P(ValueType val)
        {
            this->SetElement(34, val);
        }
        void
        SetA2P(ValueType val)
        {
            this->SetElement(35, val);
        }
        void
        SetA3P(ValueType val)
        {
            this->SetElement(36, val);
        }
        void
        SetA4P(ValueType val)
        {
            this->SetElement(37, val);
        }
        void
        SetResT1(ValueType val)
        {
            this->SetElement(38, val);
        }
        void
        SetResT2(ValueType val)
        {
            this->SetElement(39, val);
        }
        void
        SetResT3(ValueType val)
        {
            this->SetElement(40, val);
        }
        void
        SetResT4(ValueType val)
        {
            this->SetElement(41, val);
        }
        void
        SetResR1(ValueType val)
        {
            this->SetElement(42, val);
        }
        void
        SetResR2(ValueType val)
        {
            this->SetElement(43, val);
        }
        void
        SetResR3(ValueType val)
        {
            this->SetElement(44, val);
        }
        void
        SetResa1(ValueType val)
        {
            this->SetElement(45, val);
        }
        void
        SetResa2(ValueType val)
        {
            this->SetElement(46, val);
        }
        void
        SetResa3(ValueType val)
        {
            this->SetElement(47, val);
        }
        void
        SetResa4(ValueType val)
        {
            this->SetElement(48, val);
        }

        /** Getters*/
        ValueType
        GetAH2O() const
        {
            return this->GetElement(0);
        }
        ValueType
        GetNH2O() const
        {
            return this->GetElement(1);
        }
        ValueType
        GetAO3() const
        {
            return this->GetElement(2);
        }
        ValueType
        GetNO3() const
        {
            return this->GetElement(3);
        }
        ValueType
        GetAO2() const
        {
            return this->GetElement(4);
        }
        ValueType
        GetNO2() const
        {
            return this->GetElement(5);
        }
        ValueType
        GetPO2() const
        {
            return this->GetElement(6);
        }
        ValueType
        GetACO2() const
        {
            return this->GetElement(7);
        }
        ValueType
        GetNCO2() const
        {
            return this->GetElement(8);
        }
        ValueType
        GetPCO2() const
        {
            return this->GetElement(9);
        }
        ValueType
        GetACH4() const
        {
            return this->GetElement(10);
        }
        ValueType
        GetNCH4() const
        {
            return this->GetElement(11);
        }
        ValueType
        GetPCH4() const
        {
            return this->GetElement(12);
        }
        ValueType
        GetANO2() const
        {
            return this->GetElement(13);
        }
        ValueType
        GetNNO2() const
        {
            return this->GetElement(14);
        }
        ValueType
        GetPNO2() const
        {
            return this->GetElement(15);
        }
        ValueType
        GetACO() const
        {
            return this->GetElement(16);
        }
        ValueType
        GetNCO() const
        {
            return this->GetElement(17);
        }
        ValueType
        GetPCO() const
        {
            return this->GetElement(18);
        }
        ValueType
        GetA0S() const
        {
            return this->GetElement(19);
        }
        ValueType
        GetA1S() const
        {
            return this->GetElement(20);
        }
        ValueType
        GetA2S() const
        {
            return this->GetElement(21);
        }
        ValueType
        GetA3S() const
        {
            return this->GetElement(22);
        }
        ValueType
        GetA0T() const
        {
            return this->GetElement(23);
        }
        ValueType
        GetA1T() const
        {
            return this->GetElement(24);
        }
        ValueType
        GetA2T() const
        {
            return this->GetElement(25);
        }
        ValueType
        GetA3T() const
        {
            return this->GetElement(26);
        }
        ValueType
        GetTauR() const
        {
            return this->GetElement(27);
        }
        ValueType
        GetSR() const
        {
            return this->GetElement(28);
        }
        ValueType
        GetA0TauP() const
        {
            return this->GetElement(29);
        }
        ValueType
        GetA1TauP() const
        {
            return this->GetElement(30);
        }
        ValueType
        GetWO() const
        {
            return this->GetElement(31);
        }
        ValueType
        GetGC() const
        {
            return this->GetElement(32);
        }
        ValueType
        GetA0P() const
        {
            return this->GetElement(33);
        }
        ValueType
        GetA1P() const
        {
            return this->GetElement(34);
        }
        ValueType
        GetA2P() const
        {
            return this->GetElement(35);
        }
        ValueType
        GetA3P() const
        {
            return this->GetElement(36);
        }
        ValueType
        GetA4P() const
        {
            return this->GetElement(37);
        }
        ValueType
        GetResT1() const
        {
            return this->GetElement(38);
        }
        ValueType
        GetResT2() const
        {
            return this->GetElement(39);
        }
        ValueType
        GetResT3() const
        {
            return this->GetElement(40);
        }
        ValueType
        GetResT4() const
        {
            return this->GetElement(41);
        }
        ValueType
        GetResR1() const
        {
            return this->GetElement(42);
        }
        ValueType
        GetResR2() const
        {
            return this->GetElement(43);
        }
        ValueType
        GetResR3() const
        {
            return this->GetElement(44);
        }
        ValueType
        GetResa1() const
        {
            return this->GetElement(45);
        }
        ValueType
        GetResa2() const
        {
            return this->GetElement(46);
        }
        ValueType
        GetResa3() const
        {
            return this->GetElement(47);
        }
        ValueType
        GetResa4() const
        {
            return this->GetElement(48);
        }

        /** Fill the coefficient using a vector */
        void
        SetCoefficients(const std::vector<ValueType> &vectVal)
        {
            const unsigned int nbValues(vectVal.size());
            for (unsigned int i = 0; i < nbValues; i++)
            {
                this->SetElement(i, vectVal[i]);
            }
        }
        static const unsigned int m_NumbersOfSmacCoefficients;

        virtual void
        Print(std::ostream &os) const
        {
            // Display alls parameters
            os << "Coefficients list:" << std::endl;
            // Display H2o values
            os << "   ah2o  : " << this->GetAH2O() << std::endl;
            os << "   nh2o  : " << this->GetNH2O() << std::endl;
            // Display o3 values
            os << "   ao3   : " << this->GetAO3() << std::endl;
            os << "   no3   : " << this->GetNO3() << std::endl;
            // Display o2 values
            os << "   ao2   : " << this->GetAO2() << std::endl;
            os << "   no2   : " << this->GetNO2() << std::endl;
            os << "   po2   : " << this->GetPO2() << std::endl;
            // Display co2 values
            os << "   aco2  : " << this->GetACO2() << std::endl;
            os << "   nco2  : " << this->GetNCO2() << std::endl;
            os << "   pco2  : " << this->GetPCO2() << std::endl;
            // Display h4 values
            os << "   ach4  : " << this->GetACH4() << std::endl;
            os << "   nch4  : " << this->GetNCH4() << std::endl;
            os << "   pch4  : " << this->GetPCH4() << std::endl;
            // Display no2 values
            os << "   ano2  : " << this->GetANO2() << std::endl;
            os << "   nno2  : " << this->GetNNO2() << std::endl;
            os << "   pno2  : " << this->GetPNO2() << std::endl;
            // Display co values
            os << "   aco   : " << this->GetACO() << std::endl;
            os << "   nco   : " << this->GetNCO() << std::endl;
            os << "   pco   : " << this->GetPCO() << std::endl;
            // Display aoters values
            os << "   aos   : " << this->GetA0S() << std::endl;
            // Display a...s values
            os << "   a1s   : " << this->GetA1S() << std::endl;
            os << "   a2s   : " << this->GetA2S() << std::endl;
            os << "   a3s   : " << this->GetA3S() << std::endl;
            os << "   aot   : " << this->GetA0T() << std::endl;
            // Display altitude values
            os << "   a1t   : " << this->GetA1T() << std::endl;
            os << "   A2t   : " << this->GetA2T() << std::endl;
            os << "   a3t   : " << this->GetA3T() << std::endl;
            // Display taur values
            os << "   taur  : " << this->GetTauR() << std::endl;
            os << "   sr    : " << this->GetSR() << std::endl;
            // Display taup values
            os << "   a0taup: " << this->GetA0TauP() << std::endl;
            os << "   a1taup: " << this->GetA1TauP() << std::endl;
            os << "   w0    : " << this->GetWO() << std::endl;
            os << "   gc    : " << this->GetGC() << std::endl;
            // Display a..p values
            os << "   a0p   : " << this->GetA0P() << std::endl;
            os << "   a1p   : " << this->GetA1P() << std::endl;
            os << "   a2p   : " << this->GetA2P() << std::endl;
            os << "   a3p   : " << this->GetA3P() << std::endl;
            os << "   a4p   : " << this->GetA4P() << std::endl;
            // Display rest... values
            os << "   rest1 : " << this->GetResT1() << std::endl;
            os << "   rest2 : " << this->GetResT2() << std::endl;
            os << "   rest3 : " << this->GetResT3() << std::endl;
            os << "   rest4 : " << this->GetResT4() << std::endl;
            // Display resr... values
            os << "   resr1 : " << this->GetResR1() << std::endl;
            os << "   resr2 : " << this->GetResR2() << std::endl;
            os << "   resr3 : " << this->GetResR3() << std::endl;
            // Display resa... values
            os << "   resa1 : " << this->GetResa1() << std::endl;
            os << "   resa2 : " << this->GetResa2() << std::endl;
            os << "   resa3 : " << this->GetResa3() << std::endl;
            os << "   resa4 : " << this->GetResa4() << std::endl;

        }
    }; // End of class

} // End namespace vns

#endif /* __vnsSmacCoefContainer_h */
