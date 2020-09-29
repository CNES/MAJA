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
 * VERSION : 1.0.0 : FA : LAIG-FA-MAC-148399-CS : 07 novembre 2016 : Audit code - Correction qualite        *
 * VERSION : 5-0-0 : FA : LAIG-FA-MAC-143764-CS : 12 avril 2016 : Mise en forme du code (indentation, etc.) *
 * VERSION : 4-0-0 : FA : LAIG-FA-MAC-120653-CS : 7 mai 2014 : Correction de règles qualité ou commentaires *
 * VERSION : 2-0-0 : DM : LAIG-DM-MAC-146-CNES : 24 octobre 2011 : Modification pour prise en compte        *
 *                                                            d'evolutions liees a MACCS                    *
 * VERSION : 1-0-0 : <TypeFT> : <NumFT> : 19 avr. 2010 : Creation                                                           
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id$
 *                                                                                                          *
 ************************************************************************************************************/

#include "vnsRainFlagGenerator.h"
#include "vnsMacro.h"
#include "vnsLoggers.h"
#include "vnsCaching.h"

namespace vns
{

    /** Constructor */

        RainFlagGenerator::RainFlagGenerator() :
                m_ReflectanceDecreaseThreshold(0.), m_WaterBandTOCR(0), m_WaterBandRCR(0), m_L2inRCRMean(0.), m_IPTOCRMean(0.), m_DiffRefelectance(0.)
        {
            this->ProcessObjectType::SetNumberOfIndexedInputs(8);
            this->ProcessObjectType::SetNumberOfRequiredInputs(7);

            // Initialize the filters
            m_ChannelFilter = ChannelFilterType::New();
            m_ChannelFilter2 = ChannelFilterType::New();
            m_OrFilter1 = OrFilterType::New();
            m_OrFilter2 = OrFilterType::New();
            m_OrFilter3 = OrFilterType::New();
            m_OrFilter4 = OrFilterType::New();
            m_OrFilter5 = OrFilterType::New();
            m_StatisticsFilter = StatisticsFilterType::New();
            // allocate the data object for the output which is
            // just a decorator around bool type

            BoolObjectType::Pointer output = static_cast<BoolObjectType*>(this->MakeOutput(0).GetPointer());
            this->itk::ProcessObject::SetNthOutput(0, output.GetPointer());

        }


        itk::DataObject::Pointer
        RainFlagGenerator::MakeOutput(unsigned int)
        {
            return static_cast<itk::DataObject*>(BoolObjectType::New().GetPointer());
        }

    //******************* Output rain flag accessors ****************/

        RainFlagGenerator::BoolObjectType*
        RainFlagGenerator::GetRainFlagOutput()
        {
            return static_cast<BoolObjectType*>(this->itk::ProcessObject::GetOutput(0));
        }


        const RainFlagGenerator::BoolObjectType*
        RainFlagGenerator::GetRainFlagOutput() const
        {
            return static_cast<const BoolObjectType*>(this->itk::ProcessObject::GetOutput(0));
        }



    //*********************************** Update method *******************************************/
    /** Update Method calls GenerateData Method */

        void
        RainFlagGenerator::Update(void)
        {
            this->GenerateData();
        }

    //************************************** GenerateData ****************************************/
    /** Main computation method. */

        void
        RainFlagGenerator::GenerateData()
        {

            bool l_RainFlag = false;

            // Load inputs
            InputImageConstPointer l_IPTOCR = this->GetIPTOCRInput();
            InputImageConstPointer l_L2inRCR = this->GetL2inRCRInput();
            InputMaskConstPointer l_L2CLDSub = this->GetL2CLDSubInput();
            InputMaskConstPointer l_L2WAS = this->GetL2WASInput();
            InputMaskConstPointer l_IPEDG = this->GetIPEDGInput();
            InputMaskConstPointer l_L2inWAS = this->GetL2inWASInput();
            InputMaskConstPointer l_L2inNDT = this->GetL2inNDTInput();
            InputMaskConstPointer l_IPSnow = this->GetIPSNWSubInput();

            // ===========================
            // Check the input images size
            // ===========================
            vnsChecking7ImagesSizeMacro(l_IPTOCR, l_L2inRCR, l_L2CLDSub, l_L2WAS, l_IPEDG, l_L2inWAS, l_L2inNDT)

            // Extract the NIR band of the input images
            m_ChannelFilter->SetInput(l_IPTOCR);
            // In the MultiChannelExtractROI filter, the first band index is 1 and not 0
            m_ChannelFilter->SetChannel(m_WaterBandTOCR + 1);

            m_ChannelFilter2->SetInput(l_L2inRCR);
            // In the MultiChannelExtractROI filter, the first band index is 1 and not 0
            m_ChannelFilter2->SetChannel(m_WaterBandRCR + 1);

            // Generate a composite mask with the 5 input masks
            // If one of the 5 pixel values is equal to 1, the output pixel value is set to 1
            // cloud mask or water mask of the current image
            m_OrFilter1->SetInput1(l_L2CLDSub);
            m_OrFilter1->SetInput2(l_L2WAS);

            // edges mask (No_Data)
            m_OrFilter2->SetInput1(m_OrFilter1->GetOutput());
            m_OrFilter2->SetInput2(l_IPEDG);

            // water mask of the composite product
            m_OrFilter3->SetInput1(m_OrFilter2->GetOutput());
            m_OrFilter3->SetInput2(l_L2inWAS);

            // edges mask of the composite product
            m_OrFilter4->SetInput1(m_OrFilter3->GetOutput());
            m_OrFilter4->SetInput2(l_L2inNDT);

            if (l_IPSnow.IsNull() == false)
            {
                m_OrFilter5->SetInput1(m_OrFilter4->GetOutput());
                m_OrFilter5->SetInput2(l_IPSnow);
                m_StatisticsFilter->SetMaskInput(m_OrFilter5->GetOutput());
            }
            else
            {
                m_StatisticsFilter->SetMaskInput(m_OrFilter4->GetOutput());
            }

            // Compute statistics only with pixels that are not raised in the composite mask
            m_StatisticsFilter->SetInput(m_ChannelFilter->GetOutput());
//            m_StatisticsFilter->Update();
            vnsLaunchPersistentFilterCommand(StatisticsFilterType, m_StatisticsFilter, "Compute statistics for Rain flag detector in the composite mask");

            // Get the mean value of the surface reflectance corrected from absorption and rayleigh for the date D
            m_IPTOCRMean = m_StatisticsFilter->GetMean()[0];

            // Statistics used in debug mode
            vnsLogDebugMacro("RainFlagGenerator::GenerateData()\n" "Mean for the date D : " << m_IPTOCRMean);

            // Compute statistics only with pixels that are not raised in the no_data mask
            m_StatisticsFilter->SetInput(m_ChannelFilter2->GetOutput());

//            m_StatisticsFilter->Update();
            vnsLaunchPersistentFilterCommand(StatisticsFilterType, m_StatisticsFilter, "Compute statistics for Rain flag detector in the no_data mask");

            // Get the mean value of the surface reflectance corrected from absorption and rayleigh for the date D-1
            m_L2inRCRMean = m_StatisticsFilter->GetMean()[0];

            // Statistics used in debug mode
            vnsLogDebugMacro("RainFlagGenerator::GenerateData()\n" "Mean for the date D-1 : " << m_L2inRCRMean);

            // Set the difference of reflectance between the current image and the composite image
            m_DiffRefelectance = m_L2inRCRMean - m_IPTOCRMean;

            vnsLogDebugMacro("RainFlagGenerator::GenerateData()\n" "Difference between dates D and D-1 : " << m_DiffRefelectance);

            // Test if an important variation of reflectance is detected
            if (m_DiffRefelectance > m_ReflectanceDecreaseThreshold)
            {
                l_RainFlag = true;
            }

            // Set the output flag
            this->GetRainFlagOutput()->Set(l_RainFlag);

        }

    /** PrintSelf method */

        void
        RainFlagGenerator::PrintSelf(std::ostream& os, itk::Indent indent) const
        {
            Superclass::PrintSelf(os, indent);

            os << indent << "ReflectanceDecreaseThreshold: " << this->GetReflectanceDecreaseThreshold() << std::endl;
            os << indent << "WaterBand TOCR: " << this->GetWaterBandTOCR() + 1 << std::endl;
            os << indent << "WaterBand RCR: " << this->GetWaterBandRCR() + 1 << std::endl;
            os << indent << "RainFlag: " << this->GetRainFlag() << std::endl;

        }

    /** PrintResult method */

        std::string
        RainFlagGenerator::PrintResult()
        {

            std::ostringstream os;

            os << "Reflectance Decrease Threshold: " << this->GetReflectanceDecreaseThreshold() << std::endl;
            os << "Water Band TOCR: " << this->GetWaterBandTOCR() + 1 << std::endl;
            os << "Water Band RCR: " << this->GetWaterBandRCR() + 1 << std::endl;

            os << "Mean for date D: " << m_IPTOCRMean << std::endl;
            os << "Mean for date D-1: " << m_L2inRCRMean << std::endl;
            os << "Diffrence between dates D and D-1: " << m_DiffRefelectance << std::endl;

            os << "Rain Flag: " << this->GetRainFlag() << std::endl;

            return os.str();

        }

} // End namespace vns
