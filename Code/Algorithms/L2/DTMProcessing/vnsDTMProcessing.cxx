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
 * VERSION : 5-0-0 : FA : LAIG-FA-MAC-143764-CS : 12 avril 2016 : Mise en forme du code (indentation, etc.) *
 * VERSION : 2-0-0 : DM : LAIG-DM-MAC-146-CNES : 21 octobre 2011 : Modification pour prise en compte             *
 *                                                            d'evolutions liees a MACCS                    *
 * VERSION : 1-0-0 : <TypeFT> : <NumFT> : 16 sept. 2010 : Creation                                                           
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id$
 *                                                                                                          *
 ************************************************************************************************************/
#include "vnsDTMProcessing.h"

namespace vns
{

    // Constructor

    DTMProcessing::DTMProcessing() :
            m_L2CoarseResolution(1), m_VIEHRef(0.), m_SOLH1(0.), m_SOLH2(0.), m_SOLHRef(0.), m_DistanceThreshold(0.)
    {

        // Parameters initialization

        // Filters initialization
        m_SOLHRefConstantFilter = AltConstantFilterType::New();
        m_DTMProcessingFilter = DTMProcessingFilterType::New();
        m_SolarFilter = SolarFilterType::New();

        m_IPVIEInput = GRIDImageType::New();
        m_IPSOL1Input = GRIDImageType::New();
        m_DTMInput = DTMImageType::New();
        m_IPEDGInput = InputMaskType::New();

        m_IPSHDOutput = OutputImageType::New();
        m_IPHIDOutput = OutputImageType::New();

    }

    // Destructor

    DTMProcessing::~DTMProcessing()
    {
    }

    //================================== GenerateData ==================================/
    // Main computation method.

    void
    DTMProcessing::UpdateWithCaching()
    {

        m_IPVIEInput->UpdateOutputInformation();
        m_IPSOL1Input->UpdateOutputInformation();
        m_IPEDGInput->UpdateOutputInformation();

        // ===========================
        // Check the input images size
        // ===========================
        vnsChecking4ImagesSizeMacro(m_IPVIEInput, m_IPSOL1Input, m_DTMInput, m_IPEDGInput)

        m_I3D.Load(this->GetDTMInput());

        // =======================================================
        // Generate a constant image with solar reference altitude
        // =======================================================
        m_SOLHRefConstantFilter->SetSize(m_IPSOL1Input->GetLargestPossibleRegion().GetSize());
        m_SOLHRefConstantFilter->SetSpacing(m_IPSOL1Input->GetSpacing());
        m_SOLHRefConstantFilter->SetOrigin(m_IPSOL1Input->GetOrigin());
        m_SOLHRefConstantFilter->SetConstantValue(static_cast<AltImagePixelType>(m_SOLHRef));

        // ===============================================================
        // Estimate the solar direction for each reference altitude
        // The reference altitude should be the higher altitude of the DTM
        // ===============================================================

        if (m_IPSOL2Input.IsNull() == true)
        {
            m_DTMProcessingFilter->SetIPSOLInput(m_IPSOL1Input);
            m_DTMProcessingFilter->SetSOLHRef(m_SOLH1);
        }
        else
        {
            m_IPSOL2Input->UpdateOutputInformation();

            m_SolarFilter->SetInput1(m_IPSOL1Input);
            m_SolarFilter->SetInput2(m_IPSOL2Input);
            m_SolarFilter->SetInput3(m_SOLHRefConstantFilter->GetOutput());

            m_SolarFilter->GetFunctor().SetH1(m_SOLH1);
            m_SolarFilter->GetFunctor().SetH2(m_SOLH2);
            m_SolarFilter->GetFunctor().SetSOLHRef(m_SOLHRef);

            m_DTMProcessingFilter->SetIPSOLInput(m_SolarFilter->GetOutput());
            m_DTMProcessingFilter->SetSOLHRef(m_SOLHRef);
        }

        // =====================================================
        // Generate the hidden faces and the cast shadows masks
        // =====================================================
        m_DTMProcessingFilter->SetDTMInput(m_DTMInput);
        m_DTMProcessingFilter->SetIPVIEInput(m_IPVIEInput);
        m_DTMProcessingFilter->SetIPEDGInput(m_IPEDGInput);

        m_DTMProcessingFilter->SetI3D(&m_I3D);

        m_DTMProcessingFilter->SetL2CoarseResolution(m_L2CoarseResolution);
        m_DTMProcessingFilter->SetVIEHRef(m_VIEHRef);

        m_DTMProcessingFilter->SetDistanceThreshold(m_DistanceThreshold);

        m_IPHIDOutput = m_DTMProcessingFilter->GetIPHIDOutput();
        m_IPSHDOutput = m_DTMProcessingFilter->GetIPSHDOutput();

    }

    // PrintSelf method

    void
    DTMProcessing::PrintSelf(std::ostream& os, itk::Indent indent) const
    {
        Superclass::PrintSelf(os, indent);

        os << "m_L2CoarseResolution:    " << m_L2CoarseResolution << std::endl;

        os << "m_VIEHRef:    " << m_VIEHRef << std::endl;

        os << "m_SOLH1:    " << m_SOLH1 << std::endl;
        os << "m_SOLH2:    " << m_SOLH2 << std::endl;
        os << "m_SOLHRef:    " << m_SOLHRef << std::endl;

        os << "m_DistanceThreshold:    " << m_DistanceThreshold << std::endl;
    }

} // End namespace vns
