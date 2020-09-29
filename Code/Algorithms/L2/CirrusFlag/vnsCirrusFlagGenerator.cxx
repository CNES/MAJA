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
 * VERSION : 2.0.0 : DM : LAIG-DM-MAJA-153436-CS : 3 mars 2017 : Refactoring pour ameliorer la qualite      *
 * VERSION : 1.0.0 : FA : LAIG-FA-MAC-148399-CS : 07 novembre 2016 : Audit code - Correction qualite        *
 * VERSION : 1.0.0 : FA : LAIG-FA-MAC-1990-CNES : 13 octobre 2016 : Correction qualite (TO DO)              *
 * VERSION : 1.0.0 : FA : LAIG-FA-MAC-1988-CNES : 13 octobre 2016 : Correction qualite (code mort)          *
 * VERSION : 5-0-0 : FA : LAIG-FA-MAC-143764-CS : 12 avril 2016 : Mise en forme du code (indentation, etc.) *
 * VERSION : 4-0-0 : FA : LAIG-FA-MAC-120653-CS : 7 mai 2014 : Correction de règles qualité ou commentaires *
 * VERSION : 1-0-0-3 : DM : 331 : 04 oct. 2011 : Cirrus Flag, calcul fait à L2CoarseResolution                *
 * VERSION : 1-0-0-3 : FT : 65466 : 26 sept. 2011 : Test if the number of valid pixel is not null             *
 * VERSION : 1-0-0-3 : DM : 357 : 25 juil. 2011 : Add a test on cloud proportion                              *
 * VERSION : 1-0-0 : <TypeFT> : <NumFT> : 22 avr. 2010 : Creation                                                           
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id$
 *                                                                                                          *
 ************************************************************************************************************/
#include "vnsCirrusFlagGenerator.h"
#include "vnsMacro.h"
#include "vnsLoggers.h"
#include "vnsCaching.h"

namespace vns
{

    /** Constructor */

    CirrusFlagGenerator::CirrusFlagGenerator() :
            m_AltitudeThreshold(0), m_MinPercentCirrus(0), m_MinPercentCloud(0), m_MinRstdCirrus(0), m_MinPercentStdCirrus(0), m_L1NoData(
                    static_cast<RealNoDataType>(0)), m_CirrusProportion(0.), m_CloudProportion(0.), m_CloudRstd(0.), m_CloudStd(0.)

    {
        // Init the Inputs required
        this->ProcessObjectType::SetNumberOfIndexedInputs(2);
        this->ProcessObjectType::SetNumberOfRequiredInputs(2);

        // Initialize the pointer of the filters
        m_CountCirrusFilter = CountCirrusFilterType::New();
        m_ThresholdFilter = ThresholdFilterType::New();
        m_StatisticsFilter = StatisticsFilterType::New();

        // allocate the data object for the output which is
        // just a decorator around bool type
        BoolObjectType::Pointer output = static_cast<BoolObjectType*>(this->MakeOutput(0).GetPointer());
        this->itk::ProcessObject::SetNthOutput(0, output.GetPointer());

    }

    itk::DataObject::Pointer
    CirrusFlagGenerator::MakeOutput(unsigned int)
    {
        return static_cast<itk::DataObject*>(BoolObjectType::New().GetPointer());
    }

    //******************* Output cirrus flag accessors ****************/
    /** Get cirrus flag */

    CirrusFlagGenerator::BoolObjectType*
    CirrusFlagGenerator::GetCirrusFlagOutput()
    {
        return static_cast<BoolObjectType*>(this->itk::ProcessObject::GetOutput(0));
    }

    /** Get cirrus flag */

    const CirrusFlagGenerator::BoolObjectType*
    CirrusFlagGenerator::GetCirrusFlagOutput() const
    {
        return static_cast<const BoolObjectType*>(this->itk::ProcessObject::GetOutput(0));
    }

    /** Update Method calls GenerateData Method */

    void
    CirrusFlagGenerator::Update(void)
    {
        this->GenerateData();
    }

    //************************************** GenerateData ****************************************/
    /** Main computation method. */

    void
    CirrusFlagGenerator::GenerateData()
    {

        vnsLogDebugMacro("CirrusFlagGenerator::GenerateData() - Start")

        bool l_CirrusFlag = false;

        // Load input image of altitude
        InputImagePointer l_L1CLA = dynamic_cast<InputImageType *>(itk::ProcessObject::GetInput(0));

        InputMaskPointer l_EDGSub = dynamic_cast<InputMaskType *>(itk::ProcessObject::GetInput(1));

        // Count all cirrus clouds
        // Call the vnsCountCirrusPixelGenerator
        m_CountCirrusFilter->SetInput(l_L1CLA);
        m_CountCirrusFilter->SetAltitudeThreshold(m_AltitudeThreshold);
        m_CountCirrusFilter->SetL1NoData(m_L1NoData);
        m_CountCirrusFilter->SetEDGSub(l_EDGSub);
        vnsLaunchPersistentFilterCommand(CountCirrusFilterType, m_CountCirrusFilter, "CountCirrusFilter");

        // Get the results of the vnsCountCirrusPixelGenerator
        const unsigned long l_CirrusPixelNumber = m_CountCirrusFilter->GetCirrusPixelNumber();
        const unsigned long l_CloudPixelNumber = m_CountCirrusFilter->GetCloudPixelNumber();
        const unsigned long l_ValidPixelNumber = m_CountCirrusFilter->GetValidPixelNumber();

        // Display the number of cirrus pixels in debug mode
        vnsLogDebugMacro("l_CirrusPixelNumber: "<<l_CirrusPixelNumber);
        vnsLogDebugMacro("l_CloudPixelNumber: "<<l_CloudPixelNumber);
        vnsLogDebugMacro("l_ValidPixelNumber: "<<l_ValidPixelNumber);

        if (l_ValidPixelNumber == 0)
        {
            vnsLogWarningMacro("CirrusFlagGenerator::GenerateData(). The number of valid pixels is null")
        }
        else
        {

            // Threshold the cloud altitude (> 0) and generate a mask used to compute statistics
            // Statistics are computed if the pixel of the mask is equal to 0
            m_ThresholdFilter->SetInput(l_L1CLA);
            m_ThresholdFilter->GetFunctor().SetThresholdValue(0);
            m_ThresholdFilter->GetFunctor().SetInsideValue(1);
            m_ThresholdFilter->GetFunctor().SetOutputValue(0);

            // Cast the input image to a vector image
            // Compute image statistics with an input mask
            m_StatisticsFilter->SetInput(l_L1CLA);
            m_StatisticsFilter->SetEnableVariance(true);
            m_StatisticsFilter->SetMaskInput(m_ThresholdFilter->GetOutput());

//                m_StatisticsFilter->Update();
            vnsLaunchPersistentFilterCommand(StatisticsFilterType, m_StatisticsFilter, "StatisticsFilter");

            // Statistics used in debug mode
            vnsLogDebugMacro("CirrusFlagGenerator::GenerateData() Minimum: " <<m_StatisticsFilter->GetMinimumOutput()->Get());
            vnsLogDebugMacro("CirrusFlagGenerator::GenerateData() Maximum: "<< m_StatisticsFilter->GetMaximumOutput()->Get());
            vnsLogDebugMacro("CirrusFlagGenerator::GenerateData() Sum: " << m_StatisticsFilter->GetSum());
            vnsLogDebugMacro("CirrusFlagGenerator::GenerateData() Mean: " << m_StatisticsFilter->GetMean());
            vnsLogDebugMacro("CirrusFlagGenerator::GenerateData() Variance: " << m_StatisticsFilter->GetVariance());
            vnsLogDebugMacro("CirrusFlagGenerator::GenerateData() StandarDeviation: " << m_StatisticsFilter->GetStandardDeviation());

            // Proportion of cirrus within the image
            m_CirrusProportion = l_CirrusPixelNumber * 100. / l_ValidPixelNumber;
            m_CloudProportion = l_CloudPixelNumber * 100. / l_ValidPixelNumber;

            // Relative standard deviation for cirrus
            // Get the first element of the matrix while the input image of the statistics filter
            // is an otb::image and not an otb::VectorImage
            const double l_CloudMean = m_StatisticsFilter->GetMean(); //[0];
            m_CloudStd = m_StatisticsFilter->GetStandardDeviation();
            vnsLogDebugMacro("CirrusFlagGenerator l_CloudMean : "<<l_CloudMean);
            vnsLogDebugMacro("CirrusFlagGenerator m_CloudStd : "<<m_CloudStd);
            vnsLogDebugMacro("CirrusFlagGenerator m_StatisticsFilter->GetIsValid() : "<<m_StatisticsFilter->GetIsValid());

            // No pixel is valid. Returns null Statistics
            if (m_StatisticsFilter->GetIsValid() == false)
            {
                // Cirrus flag is set to false
                this->GetCirrusFlagOutput()->Set(l_CirrusFlag);
            }
            else
            {
                // Check if the mean value is not null before computing the "CloudRstd" value
                if(vnsIsZeroDoubleWithEpsilonMacro(l_CloudMean, CONST_EPSILON_20))
                {
                    vnsExceptionBusinessMacro("CirrusFlagGenerator failed. The cloud statistics of L1 CLA image is null")
                }
                m_CloudRstd = m_CloudStd / l_CloudMean;

                // Parameters used to raise the cirrus flag and displayed in debug mode
                vnsLogDebugMacro("m_CloudProportion: " << m_CloudProportion << " <> " << m_MinPercentCloud);
                vnsLogDebugMacro("m_CirrusProportion: " << m_CirrusProportion << " <> " << m_MinPercentCirrus);
                vnsLogDebugMacro("m_CloudRstd: " << m_CloudRstd << " <> " << m_MinRstdCirrus);
                vnsLogDebugMacro("m_CloudStd: " << m_CloudStd << " <> " << m_MinPercentStdCirrus);

                // If the proportion of cirrus within the image is too high
                // or if the standart deviation is high, the cirrus flag is raised
                if (m_CloudProportion > m_MinPercentCloud)
                {
                    if ((m_CirrusProportion > m_MinPercentCirrus) || (m_CloudRstd > m_MinRstdCirrus)
                            || (m_CloudStd > m_MinPercentStdCirrus))
                    {
                        l_CirrusFlag = true;
                    }
                }
            }

        }

        // Set the output flag
        this->GetCirrusFlagOutput()->Set(l_CirrusFlag);
    }

    /** PrintSelf method */

    void
    CirrusFlagGenerator::PrintSelf(std::ostream& os, itk::Indent indent) const
    {
        Superclass::PrintSelf(os, indent);

        os << indent << "AltitudeThreshold: " << this->GetAltitudeThreshold() << std::endl;
        os << indent << "MinPercentCirrus: " << this->GetMinPercentCirrus() << std::endl;
        os << indent << "MinRstdCirrus: " << this->GetMinRstdCirrus() << std::endl;
        os << indent << "MinPercentStdCirrus: " << this->GetMinPercentStdCirrus() << std::endl;

        os << indent << "Mean: " << m_StatisticsFilter->GetMean() << std::endl;
        os << indent << "Cirrus proportion: " << m_CirrusProportion << std::endl;
        os << indent << "Cloud Relative Std: " << m_CloudRstd << std::endl;
        os << indent << "Cloud Std: " << m_CloudStd << std::endl;

        os << indent << "CirrusFlag: " << this->GetCirrusFlag() << std::endl;

    }

    /** PrintResult method */

    std::string
    CirrusFlagGenerator::PrintResult()
    {

        std::ostringstream os;

        os << "AltitudeThreshold: " << this->GetAltitudeThreshold() << std::endl;
        os << "MinPercentCirrus: " << this->GetMinPercentCirrus() << std::endl;
        os << "MinRstdCirrus: " << this->GetMinRstdCirrus() << std::endl;
        os << "MinPercentStdCirrus: " << this->GetMinPercentStdCirrus() << std::endl;

        os << "Mean: " << m_StatisticsFilter->GetMean() << std::endl;
        os << "Cirrus proportion: " << m_CirrusProportion << std::endl;
        os << "Cloud Relative Std: " << m_CloudRstd << std::endl;
        os << "Cloud Std: " << m_CloudStd << std::endl;

        os << "CirrusFlag: " << this->GetCirrusFlag() << std::endl;

        return os.str();

    }

} // End namespace vns
