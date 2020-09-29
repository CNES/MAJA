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
 * HISTORIQUE                                                                                               *
 *                                                                                                          *
 * VERSION : 1.0.0 : FA : LAIG-FA-MAC-148399-CS : 03 novembre 2016 : Audit code - Correction qualite        *
 * VERSION : 5-0-0 : FA : LAIG-FA-MAC-143764-CS : 12 avril 2016 : Mise en forme du code (indentation, etc.) *
 * VERSION : 4-0-0 : DM : LAIG-DM-MAC-517-CNES : 13 mars 2014 : Améliorations algorithmiques                *
 * VERSION : 2-0-0 : DM : LAIG-DM-MAC-146-CNES : 19 octobre 2011 : Modification pour prise en compte             *
 *                                                            d'evolutions liees a MACCS                    *
 * VERSION : 1-0-0  : 19 oct. 2011 : Creation
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id$
 *                                                                                                          *
 ************************************************************************************************************/
#include "vnsSnowMaskingImageFilter.h"
#include "vnsCaching.h"

namespace vns
{

    /** Constructor */

    SnowMaskingImageFilter::SnowMaskingImageFilter() :
            m_GreenBand(0), m_SWIRBand(0), m_RedBand(0), m_SERadius(0), m_NDSIThreshold(0.), m_RedThreshold(0.), m_SWIRThreshold(0.), m_ThetaS(
                    0.), m_PhiS(0.), m_MinCosI(0.), m_UseWaterMask(true), m_IsInitMode(true)
    {
        this->SetNumberOfIndexedInputs(6);
        this->SetNumberOfRequiredInputs(5);

        m_IncidenceAngleFilter = IncidenceAngleFilterType::New();
        m_SnowMaskDeterminationImageFilter = SnowMaskDeterminationImageFilterType::New();
        m_BinaryClosingImageFilter = BinaryMorphologicalClosingImageFilterType::New();
        m_SnowMaskOutput = OutputMaskType::New();
        m_OrFilter = OrFilterType::New();
    }

    /** Destructor */

    SnowMaskingImageFilter::~SnowMaskingImageFilter()
    {
    }

    /** PrintSelf method */

    void
    SnowMaskingImageFilter::PrintSelf(std::ostream& os, itk::Indent indent) const
    {
        this->Superclass::PrintSelf(os, indent);
    }

    /** Threaded Generate data */

    void
    SnowMaskingImageFilter::UpdateWithCaching()
    {
        // Get the input image pointers
        InputSimpleImageConstPointer l_SlopePtr = this->GetSlopeInput();
        InputSimpleImageConstPointer l_AspectPtr = this->GetAspectInput();

        // Get the input image pointers
        InputImageConstPointer l_TOCRPtr = this->GetIPTOCRInput();
        InputMaskConstPointer l_EDGPtr = this->GetEDGSubInput();
        InputMaskConstPointer l_CirrusMaskPtr = this->GetIPCirrusMaskSubInput();
        InputMaskConstPointer l_WaterMaskPtr = this->GetL2inWASInput();

        // Initializes the inputs of the Incidence filter
        m_IncidenceAngleFilter->SetInput1(l_AspectPtr);
        m_IncidenceAngleFilter->SetInput2(l_SlopePtr);
        m_IncidenceAngleFilter->GetFunctor().SetPhiS(m_PhiS * otb::CONST_PI_180);
        m_IncidenceAngleFilter->GetFunctor().SetThetaS(m_ThetaS * otb::CONST_PI_180);

        // Set the Cirrus mask pointer
        if (l_CirrusMaskPtr.IsNull() == true)
        {
            m_SnowMaskDeterminationImageFilter->SetIPNotValidMaskSubInput(l_EDGPtr);
        }
        else
        {
            m_OrFilter->SetInput1(l_EDGPtr);
            m_OrFilter->SetInput2(l_CirrusMaskPtr);
            m_SnowMaskDeterminationImageFilter->SetIPNotValidMaskSubInput(m_OrFilter->GetOutput());
        }

        // Initializes the now mask determination filter
        m_SnowMaskDeterminationImageFilter->SetIPTOCRInput(l_TOCRPtr);
        m_SnowMaskDeterminationImageFilter->SetIncidenceAngleInput(m_IncidenceAngleFilter->GetOutput());
        m_SnowMaskDeterminationImageFilter->SetL2inWASInput(l_WaterMaskPtr);
        m_SnowMaskDeterminationImageFilter->SetMinCosI(m_MinCosI);
        // Set the bands values
        m_SnowMaskDeterminationImageFilter->SetGreenBand(m_GreenBand);
        m_SnowMaskDeterminationImageFilter->SetSWIRBand(m_SWIRBand);
        m_SnowMaskDeterminationImageFilter->SetRedBand(m_RedBand);
        // Set the threshold values
        m_SnowMaskDeterminationImageFilter->SetNDSIThreshold(m_NDSIThreshold);
        m_SnowMaskDeterminationImageFilter->SetRedThreshold(m_RedThreshold);
        m_SnowMaskDeterminationImageFilter->SetSWIRThreshold(m_SWIRThreshold);
        // Set the thetaS value
        m_SnowMaskDeterminationImageFilter->SetThetaS(m_ThetaS * otb::CONST_PI_180);
        m_SnowMaskDeterminationImageFilter->SetUseWaterMask(m_UseWaterMask && !m_IsInitMode);

        // Initialize the Kernel of the Binary closing filter
        KernelType l_Kernel;
        l_Kernel.SetRadius(m_SERadius);
        l_Kernel.CreateStructuringElement();
        m_BinaryClosingImageFilter->SetInput(m_SnowMaskDeterminationImageFilter->GetOutput());
        m_BinaryClosingImageFilter->SetKernel(l_Kernel);
        m_BinaryClosingImageFilter->SetForegroundValue(1);

        // Initialize the output
        m_SnowMaskOutput = m_BinaryClosingImageFilter->GetOutput();
    }

} // End namespace vns
