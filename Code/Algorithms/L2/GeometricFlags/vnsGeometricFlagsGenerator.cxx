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
 * VERSION : 2.0.0 : FA : LAIG-FA-MAJA-152790-CS : 23 fevrier 2017 : Correction qualite taux de commentaire *
 * VERSION : 1.0.0 : FA : LAIG-FA-MAC-148399-CS : 27 octobre 2016 : Audit code - Correction qualite         *
 * VERSION : 4-0-0 : FA : LAIG-FA-MAC-120653-CS : 7 mai 2014 : Correction de règles qualité ou commentaires *
 * VERSION : 1-0-0 : <TypeFT> : <NumFT> : 19 juil. 2010 : Creation                                                           
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id$
 *                                                                                                          *
 ************************************************************************************************************/
#include "vnsGeometricFlagsGenerator.h"
#include "vnsLoggers.h"

namespace vns
{

    // Constructor
    GeometricFlagsGenerator::GeometricFlagsGenerator() :
            m_ThetaS(0.), m_PhiS(0.), m_SunglintThreshold(0.), m_HotSpotThreshold(0.)
    {
        m_ThetaV.clear();
        m_PhiV.clear();
    }

    // Destructor
    GeometricFlagsGenerator::~GeometricFlagsGenerator()
    {
    }

    // Get sunglint distance
    GeometricFlagsGenerator::DoubleVectorType
    GeometricFlagsGenerator::GetSunglintDistance()
    {
        DoubleVectorType l_SunglintDistance;

        // Compute the ThetaS in radian
        const double l_ThetaSRad = m_ThetaS * otb::CONST_PI_180;
        // Compute the PhiS in radian
        const double l_PhiSRad = m_PhiS * otb::CONST_PI_180;

        // One value of viewing angles per band
        const unsigned int l_Size = m_ThetaV.size();
        for (unsigned int i = 0; i < l_Size; i++)
        {
            // Compute the ThetaV in radian
            const double l_ThetaVRad = m_ThetaV.at(i) * otb::CONST_PI_180;
            // Compute the PhiV in radian
            const double l_PhiVRad = m_PhiV.at(i) * otb::CONST_PI_180;
            l_SunglintDistance.push_back(
            std::acos(
            std::sin(l_ThetaSRad) * std::sin(l_ThetaVRad) * std::cos(l_PhiSRad - l_PhiVRad + otb::CONST_PI)
            + std::cos(l_ThetaSRad) * std::cos(l_ThetaVRad)) * otb::CONST_180_PI);

            //vnsLogDebugMacro("l_SunglintDistance[" << i << "] = "<< l_SunglintDistance.at(i));
        }

        // return the Sunglint distance
        return l_SunglintDistance;

    }

    // Get Hot-spot distance
    GeometricFlagsGenerator::DoubleVectorType
    GeometricFlagsGenerator::GetHotSpotDistance()
    {
        DoubleVectorType l_HotSpotDistance;

        // Compute the ThetaS in radian
        const double l_ThetaSRad = m_ThetaS * otb::CONST_PI_180;
        // Compute the PhiS in radian
        const double l_PhiSRad = m_PhiS * otb::CONST_PI_180;

        // One value of viewing angles per band
        const unsigned int l_Size = m_ThetaV.size();
        for (unsigned int i = 0; i < l_Size; i++)
        {
            // Compute the ThetaV in radian
        	const double l_ThetaVRad = m_ThetaV.at(i) * otb::CONST_PI_180;
            // Compute the PhiV in radian
        	const double l_PhiVRad = m_PhiV.at(i) * otb::CONST_PI_180;
            l_HotSpotDistance.push_back(
            std::acos(
            std::sin(l_ThetaSRad) * std::sin(l_ThetaVRad) * std::cos(l_PhiSRad - l_PhiVRad)
            + std::cos(l_ThetaSRad) * std::cos(l_ThetaVRad)) * otb::CONST_180_PI);
        }

        // return the Hotspot distance
        return l_HotSpotDistance;

    }

    // Sunglint flag
    bool
    GeometricFlagsGenerator::IsSunglint()
    {
        const DoubleVectorType l_SunglintDistance = GetSunglintDistance();

        // One value of sunglint distance per band
        const unsigned int l_Size = l_SunglintDistance.size();
        for (unsigned int i = 0; i < l_Size; i++)
        {
            // If one distance is greater than the threshold, the flag is raised
            if (l_SunglintDistance.at(i) < m_SunglintThreshold)
            {
                return true;
            }
        }
        return false;

    }

    // HotSpot flag
    bool
    GeometricFlagsGenerator::IsHotSpot()
    {
        const DoubleVectorType l_HotSpotDistance = GetHotSpotDistance();

        // One value of hotspot distance per band
        const unsigned int l_Size = l_HotSpotDistance.size();
        for (unsigned int i = 0; i < l_Size; i++)
        {
            // If one distance is greater than the threshold, the flag is raised
            if (l_HotSpotDistance.at(i) < m_HotSpotThreshold)
            {
                return true;
            }
        }
        return false;

    }

    // PrintSelf method
    void
    GeometricFlagsGenerator::PrintSelf(std::ostream& os, itk::Indent indent) const
    {
        Superclass::PrintSelf(os, indent);

        // Print the SunglintThreshold
        os << "m_SunglintThreshold: " << m_SunglintThreshold << std::endl;
        // Print the HotSpotThreshold
        os << "m_HotSpotThreshold:  " << m_HotSpotThreshold << std::endl;

        // Print the Theta S value
        os << "m_ThetaS:  " << m_ThetaS << std::endl;
        // Print the Phi S value
        os << "m_PhiS:  " << m_PhiS << std::endl;

        // Print the Theta V values
        os << "m_ThetaV:  " << std::endl;
        unsigned int l_Size = this->GetThetaV().size();
        for (unsigned int i = 0; i < l_Size; i++)
        {
            // Print the ThetaV values
            os << this->GetThetaV().at(i) << std::endl;
        }

        // Print the Phi V values
        os << "m_PhiV:  " << std::endl;
        l_Size = this->GetPhiV().size();
        for (unsigned int i = 0; i < l_Size; i++)
        {
            // Print the PhiV values
            os << this->GetPhiV().at(i) << std::endl;
        }
    }

    /** PrintResult method */
    std::string
    GeometricFlagsGenerator::PrintResult()
    {

        std::ostringstream os;
        DoubleVectorType l_HotSpotDistance = GetHotSpotDistance();
        DoubleVectorType l_SunglintDistance = GetSunglintDistance();

        // Print the Sunglint Threshold values
        os << "Sunglint Threshold: " << m_SunglintThreshold << std::endl;
        // Print the HotSpot Threshold values
        os << "HotSpot Threshold:  " << m_HotSpotThreshold << std::endl;

        // Print the Theta S value
        os << "ThetaS:  " << m_ThetaS << std::endl;
        // Print the Phi S value
        os << "PhiS:  " << m_PhiS << std::endl;

        // Print the Theta V values
        os << "ThetaV:  " << std::endl;
        unsigned int l_Size = this->GetThetaV().size();
        for (unsigned int i = 0; i < l_Size; i++)
        {
            os << this->GetThetaV().at(i) << std::endl;
        }

        // Print the Phi V values
        os << "PhiV:  " << std::endl;
        l_Size = this->GetPhiV().size();
        for (unsigned int i = 0; i < l_Size; i++)
        {
            os << this->GetPhiV().at(i) << std::endl;
        }

        // Print the Sunglint Distance values
        os << "Sunglint Distance:  " << std::endl;
        l_Size = l_SunglintDistance.size();
        for (unsigned int i = 0; i < l_Size; i++)
        {
            os << l_SunglintDistance.at(i) << std::endl;
        }

        // Print the HotSpot Distance values
        os << "HotSpot Distance:  " << std::endl;
        l_Size = l_HotSpotDistance.size();
        for (unsigned int i = 0; i < l_Size; i++)
        {
            os << l_HotSpotDistance.at(i) << std::endl;
        }

        return os.str();

    }

} // End namespace vns
