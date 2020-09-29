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
/************************************************************************************************************ *                                                                                                          *
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
 * VERSION : 3.3.0 : DM : LAIG-DM-MAJA-3044-CNES : 15 mars 2019 : Integration modifications oliver hagolle  *
 * VERSION : 3.0.0 : DM : LAIG-DM-MAJA-2369-CNES : 28 aout 2017 : Introduction données des CAMS             *
 * VERSION : 2.0.0 : DM : LAIG-DM-MAJA-981-CNES : 2 mars 2017 : ReducedLutComputation extracted as new algo *
 * VERSION : 2.0.0 : <TypeFT> : <NumFT> : 2 mars. 2017 : Creation                                           *
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id$
 *                                                                                                          *
 ************************************************************************************************************/
#include "vnsReducedLutComputationFilter.h"

#include "vnsCaching.h"
#include "vnsMacro.h"
#include "vnsUtilities.h"

const unsigned int vns::ReducedLutComputationFilter::CR_LUT_INDEX = 0;

namespace vns
{

    // Constructor
    ReducedLutComputationFilter::ReducedLutComputationFilter() :
                                                        m_WideFieldSensor(false),m_DirectionalCorrection(false),m_SolarZenith(0), m_SolarAzimuth(0),  m_RefZenith(0),m_RefAzimuth(0),
                                                        m_UseCamsData(false)
    {
        // Input/Output initialization
        this->SetNumberOfIndexedInputs(1);
        this->SetNumberOfRequiredInputs(1);

        //Internal filter/data init
        m_MultiplyFilter = MultiplyByVectorFilterType::New();
        m_CRLutNotCorrected = ReducedLutType::New();
        m_CRLutCorrected = ReducedLutType::New();
        m_TOCRProportionApplyFilter = Lut3DProportionsFilterType::New();
        m_TDIRProportionApplyFilter = Lut3DProportionsFilterType::New();
        m_TDIFProportionApplyFilter = Lut3DProportionsFilterType::New();
        m_ALBProportionApplyFilter = Lut2DProportionsFilterType::New();
    }

    //================================== UpdateWithCaching ==========================================/
    // Main computation method.
    void
    ReducedLutComputationFilter::UpdateWithCaching()
    {
        //Point to interpolate
        VectorLookUpTableExtractorType::VectorExtractPointType vectorPoint;

        // Get the number of band
        const unsigned int numberOfBand(m_MeanViewingZenithAngles.size());

        vnsLogDebugMacro(
                " Mean solar angles [Z;A] => ["<<m_SolarZenith<<";"<<m_SolarAzimuth<<"]")
        const unsigned int nb_MeanViewingAngles = m_MeanViewingZenithAngles.size();
        vnsLogDebugMacro(" Mean viewing angles (one per band) => nb bands = ["<<nb_MeanViewingAngles<<"]")
        for (unsigned int bandNum = 0; bandNum < nb_MeanViewingAngles; bandNum++)
        {
            vnsLogDebugMacro(
                    "        [Z;A] => ["<<m_MeanViewingZenithAngles.at(bandNum)<<";"<<m_MeanViewingAzimuthAngles.at(bandNum)<<"] (p_InputL1ImageInformationsProvider->GetListOfViewingAngles Zenith/Azimuth PerBandAtL2CoarseResolution().at(bandNum).")
        }

        // Band Loop
        for (unsigned int bandNum = 0; bandNum < numberOfBand; bandNum++)
        {
            // Get the extract point values for each band because
            // viewing angles depend on spectral band
            VectorLookUpTableExtractorType::ExtractPointType point;
            point[0] = m_SolarZenith;
            point[1] = m_MeanViewingZenithAngles[bandNum];

            // Get the relative azimuth angle
            double deltaPhi = m_SolarAzimuth - m_MeanViewingAzimuthAngles[bandNum];
            if (deltaPhi < 0.)
            {
                deltaPhi = -deltaPhi;
            }
            if (deltaPhi > 180.)
            {
                deltaPhi = 360 - deltaPhi;
            }
            point[2] = deltaPhi;

            vectorPoint.push_back(point);
        }

        //No Cams version
        if (! m_UseCamsData)
        {
	  vnsLogInfoMacro("CAMS data not requested") //MODIF OH
        	(dynamic_cast<LutType *>(itk::ProcessObject::GetInput(0)))->UpdateOutputInformation();

            // Get pointers to the input and output images
            LutPointer l_LutInPtr = this->GetLUTInput();

            //Extract the lut for the point
            m_CRLutNotCorrected = VectorLookUpTableExtractorType::ExtractLUT(l_LutInPtr, vectorPoint);
            //If we have zoned angles
            if(m_WideFieldSensor) {
            	vnsLogDebugMacro("WideFieldSensor requested")
                // Generate the reduced TOCR Look Up Table
                // --------------------------------------------------
                ViewingAngleMapType::iterator iterZ = m_ViewingZenithAnglesMap.begin();
                ViewingAngleMapType::iterator iterA = m_ViewingAzimuthAnglesMap.begin();

                vnsLogDebugMacro(
                        "'HR Minilum':  SolarAngleUsefulImageCenter [Z;A] => ["<<m_SolarZenith<<";"<<m_SolarAzimuth<<"].")

                while ((iterZ != m_ViewingZenithAnglesMap.end()) && (iterA != m_ViewingAzimuthAnglesMap.end()))
                {
                    VectorLookUpTableExtractorType::VectorExtractPointType vectorPoint;
                    const unsigned int zone = (*iterZ).first;
                    vnsLogDebugMacro("            Zone detector: ["<<zone<<"].")
                    const unsigned int zone2 = (*iterA).first;
                    if (zone != zone2)
                    {
                        vnsExceptionAlgorithmsProcessingMacro("Internal ERROR: The Viewing Zenith angles Map per zone are incorrect !!")
                    }
                    const std::vector<double> & l_ViewingZenithAngle = (*iterZ).second;
                    const std::vector<double> & l_ViewingAzimuthAngle = (*iterA).second;
                    const unsigned int l_NbViewingZentithAngle = l_ViewingZenithAngle.size();
                    const unsigned int l_NbViewingAzimuthAngle = l_ViewingAzimuthAngle.size();
                    vnsLogDebugMacro(
                            "            NbViewingAngle [Z;A] => ["<<l_NbViewingZentithAngle<<";"<<l_NbViewingAzimuthAngle<<"].")
                    vnsLogDebugMacro("            TOCR_NumberOfBand ["<<numberOfBand<<"].")

                    // Band Loop
                    for (unsigned int bandNum = 0; bandNum < numberOfBand; bandNum++)
                    {
                        // Get the extract point values for each band because
                        // viewing angles depend on spectral band
                        VectorLookUpTableExtractorType::ExtractPointType point;
                        point[0] = m_SolarZenith;
                        point[1] = l_ViewingZenithAngle[bandNum];

                        // Get the relative azimuth angle
                        double deltaPhi = m_SolarAzimuth - l_ViewingAzimuthAngle[bandNum];
                        if (deltaPhi < 0.)
                        {
                            deltaPhi = -deltaPhi;
                        }
                        if (deltaPhi > 180.)
                        {
                            deltaPhi = 360 - deltaPhi;
                        }
                        point[2] = deltaPhi;

                        vnsLogDebugMacro(
                                " [band="<<bandNum<<"] : ViewingAngle [Z;A] => ["<<l_ViewingZenithAngle[bandNum]<<";"<<l_ViewingAzimuthAngle[bandNum]<<"] ; point ["<<point<<"].")

                        vectorPoint.push_back(point);

                    } // end band loop
                    vnsLogDebugMacro("Extracting lut for zone "<<zone)
                    m_LUTMapOutput[zone] = VectorLookUpTableExtractorType::ExtractLUT(l_LutInPtr, vectorPoint);

                    iterZ++;
                    iterA++;

                } // end zone loop
            }//End if wideModelSensor
        }
        else
            // interpolation of CAMS aerosol models
        {
	  vnsLogInfoMacro("CAMS data requested") //MODIF OH

            if( m_ModelList.size() == 0)
            {
                vnsExceptionAlgorithmsProcessingMacro("No model in the model list for lut computation")
            } else
            {
            	std::vector<double> l_propFactor;
                for (unsigned int i = 0; i < m_ModelList.size();i++)
                {
		    vnsLogDebugMacro("Next model")
                    const std::string& l_mod = m_ModelList[i];
                    vnsLogDebugMacro("Treating model : "<<l_mod);
                    //Get the proportion
                    if ( m_Proportions.find(l_mod) == m_Proportions.end())
                    {
                        vnsExceptionAlgorithmsProcessingMacro("No "<<l_mod<<" in input proportions")
                    }
                    const double l_prop = m_Proportions.at(l_mod);
                    l_propFactor.push_back(l_prop);
		    vnsLogInfoMacro("CAMS: percentage for model :"<< l_mod <<": "<< l_prop) // MODIF OH

                    //TOCR
                    //Extract the lut for the point
                    const std::string l_tocrKey = this->GetKeyForModel(m_TOCR, l_mod);
                    if ( m_TOCR.find(l_tocrKey) == m_TOCR.end())
                    {
                        vnsExceptionAlgorithmsProcessingMacro("No "<<l_mod<<" in input TOCR Lut map")
                    }
                    m_InternalModelReducedLuts.push_back(VectorLookUpTableExtractorType::ExtractLUT(m_TOCR.at(l_tocrKey), vectorPoint));
                    m_TOCRProportionApplyFilter->SetInput(i, m_InternalModelReducedLuts.back());
                    //If we have zoned angles
                    if(m_WideFieldSensor) {
                    	vnsLogDebugMacro("WideFieldSensor activated, nbzone : "<<  m_ViewingZenithAnglesMap.size())

                        // Generate the reduced TOCR Look Up Table
                        // --------------------------------------------------
                        ViewingAngleMapType::iterator iterZ = m_ViewingZenithAnglesMap.begin();
                        ViewingAngleMapType::iterator iterA = m_ViewingAzimuthAnglesMap.begin();

                        ReducedLutMapType l_wideLutMap;

                        vnsLogDebugMacro(
                                "'HR Minilum':  SolarAngleUsefulImageCenter [Z;A] => ["<<m_SolarZenith<<";"<<m_SolarAzimuth<<"].")

                        while ((iterZ != m_ViewingZenithAnglesMap.end()) && (iterA != m_ViewingAzimuthAnglesMap.end()))
                        {
                            VectorLookUpTableExtractorType::VectorExtractPointType vectorPointWide;
                            const unsigned int zone = (*iterZ).first;
                            vnsLogDebugMacro("            Zone detector: ["<<zone<<"].")
                            const unsigned int zone2 = (*iterA).first;
                            if (zone != zone2)
                            {
                                vnsExceptionAlgorithmsProcessingMacro("Internal ERROR: The Viewing Zenith angles Map per zone are incorrect !!")
                            }
                            const std::vector<double> & l_ViewingZenithAngle = (*iterZ).second;
                            const std::vector<double> & l_ViewingAzimuthAngle = (*iterA).second;
                            const unsigned int l_NbViewingZentithAngle = l_ViewingZenithAngle.size();
                            const unsigned int l_NbViewingAzimuthAngle = l_ViewingAzimuthAngle.size();
                            //Init filter if first model
                            if (i == 0)
                            {
                            	m_TOCRWideProportionApplyFilters[zone] = Lut3DProportionsFilterType::New();
                            }
                            vnsLogDebugMacro(
                                    "            NbViewingAngle [Z;A] => ["<<l_NbViewingZentithAngle<<";"<<l_NbViewingAzimuthAngle<<"].")
                            vnsLogDebugMacro("            TOCR_NumberOfBand ["<<numberOfBand<<"].")

                            // Band Loop
                            for (unsigned int bandNum = 0; bandNum < numberOfBand; bandNum++)
                            {
                                // Get the extract point values for each band because
                                // viewing angles depend on spectral band
                                VectorLookUpTableExtractorType::ExtractPointType point;
                                point[0] = m_SolarZenith;
                                point[1] = l_ViewingZenithAngle[bandNum];

                                // Get the relative azimuth angle
                                double deltaPhi = m_SolarAzimuth - l_ViewingAzimuthAngle[bandNum];
                                if (deltaPhi < 0.)
                                {
                                    deltaPhi = -deltaPhi;
                                }
                                if (deltaPhi > 180.)
                                {
                                    deltaPhi = 360 - deltaPhi;
                                }
                                point[2] = deltaPhi;

                                vnsLogDebugMacro(
                                        " [band="<<bandNum<<"] : ViewingAngle [Z;A] => ["<<l_ViewingZenithAngle[bandNum]<<";"<<l_ViewingAzimuthAngle[bandNum]<<"] ; point ["<<point<<"].")

                                vectorPointWide.push_back(point);

                            } // end band loop
                            vnsLogDebugMacro("Extracting lut for zone "<<zone)
                            l_wideLutMap[zone] = VectorLookUpTableExtractorType::ExtractLUT(m_TOCR.at(l_mod), vectorPointWide);
                            m_TOCRWideProportionApplyFilters[zone]->SetInput(i,l_wideLutMap[zone]);
                            iterZ++;
                            iterA++;

                        } // end zone loop
                        m_InternalModelLUTMaps.push_back(l_wideLutMap);
                    }//End if wideModelSensor


                    //TDIF
                    const std::string l_tdifKey = this->GetKeyForModel(m_TDIF, l_mod);
                    if ( m_TDIF.find(l_tdifKey) == m_TDIF.end())
                    {
                        vnsExceptionAlgorithmsProcessingMacro("No "<<l_mod<<" in input TDIF Lut map")
                    }
                    m_TDIFProportionApplyFilter->SetInput(i, m_TDIF.at(l_tdifKey));
                    //TDIR
                    const std::string l_tdirKey = this->GetKeyForModel(m_TDIR, l_mod);
                    if ( m_TDIR.find(l_tdirKey) == m_TDIR.end())
                    {
                        vnsExceptionAlgorithmsProcessingMacro("No "<<l_mod<<" in input TDIR Lut map")
                    }
                    m_TDIRProportionApplyFilter->SetInput(i, m_TDIR.at(l_tdirKey));
                    //ALB
                    const std::string l_albKey = this->GetKeyForModel(m_ALB, l_mod);
                    if ( m_ALB.find(l_albKey) == m_ALB.end())
                    {
                        vnsExceptionAlgorithmsProcessingMacro("No "<<l_mod<<" in input ALB Lut map")
                    }
                    m_ALBProportionApplyFilter->SetInput(i, m_ALB.at(l_albKey));
                }
                m_TOCRProportionApplyFilter->GetFunctor().SetFactors(l_propFactor);
                m_TDIFProportionApplyFilter->GetFunctor().SetFactors(l_propFactor);
                m_TDIRProportionApplyFilter->GetFunctor().SetFactors(l_propFactor);
                m_ALBProportionApplyFilter->GetFunctor().SetFactors(l_propFactor);
                std::map<unsigned int, Lut3DProportionsFilterPointerType>::iterator itWideProp = m_TOCRWideProportionApplyFilters.begin();
                for (; itWideProp != m_TOCRWideProportionApplyFilters.end();itWideProp++)
                {
                    itWideProp->second->GetFunctor().SetFactors(l_propFactor);
                    itWideProp->second->Update();
                    m_LUTMapOutput[itWideProp->first] = itWideProp->second->GetOutput();
                    m_LUTMapOutput[itWideProp->first]->copyParametersValue(m_InternalModelLUTMaps.front().begin()->second);
                }
                m_TOCRProportionApplyFilter->Update();
                m_TDIFProportionApplyFilter->Update();
                m_TDIRProportionApplyFilter->Update();
                m_ALBProportionApplyFilter->Update();
                m_TDIFOutput = m_TDIFProportionApplyFilter->GetOutput();
                m_TDIROutput = m_TDIRProportionApplyFilter->GetOutput();
                m_ALBOutput = m_ALBProportionApplyFilter->GetOutput();
                m_CRLutNotCorrected = m_TOCRProportionApplyFilter->GetOutput();
                //Reapply Lut parameters
                vnsLogDebugMacro("Reapplying parameters value on LUT")
                m_CRLutNotCorrected->copyParametersValue(m_InternalModelReducedLuts.front());
                m_TDIFOutput->copyParametersValue(m_TDIF.begin()->second);
                m_TDIROutput->copyParametersValue(m_TDIR.begin()->second);
                m_ALBOutput->copyParametersValue(m_ALB.begin()->second);
            }
        }


        //Is the directional correction enabled ?
        if(m_DirectionalCorrection){
        	vnsLogDebugMacro("Directional correction requested")
            //Verify consitency of input
            if (m_CoefsR.size() != m_MeanViewingZenithAngles.size()){
                vnsExceptionAlgorithmsProcessingMacro("Internal error: the number of CoefR '"<<m_CoefsR.size()<<
                        "' is different than the ViewingAnglesZenithPerBand'"<<m_MeanViewingZenithAngles.size()<<"'!")
            }
            if (m_CoefsV.size() != m_MeanViewingZenithAngles.size()){
                vnsExceptionAlgorithmsProcessingMacro("Internal error: the number of CoefV '"<<m_CoefsV.size()<<
                        "' is different than the ViewingAnglesZenithPerBand'"<<m_MeanViewingZenithAngles.size()<<"'!")
            }
            //Compute the directional correction coeffs
            CoefsVectorType coefs = DirectionalCorrectionComputeType::ComputeCorrectionCoeffs(m_SolarZenith,m_SolarAzimuth,
                    m_RefZenith,m_RefAzimuth,m_MeanViewingZenithAngles, m_MeanViewingAzimuthAngles, m_CoefsR,m_CoefsV);
            for (unsigned int bandNum = 0; bandNum < nb_MeanViewingAngles; bandNum++)
            {
                vnsLogDebugMacro("Direction Correction Coeff band "<<bandNum<<" = "<<std::setprecision(20)<<coefs.at(bandNum))
            }

            // Set the size of the output image ( because the Unary Functor calls GetOutputSize() )
            m_MultiplyFilter->GetFunctor().SetOutputSize(numberOfBand);
            m_MultiplyFilter->GetFunctor().SetCoefs(coefs);
            m_MultiplyFilter->SetInput(m_CRLutNotCorrected);
            //Update the multiply filter
            vnsLogDebugMacro("Updating Multiply")
            m_MultiplyFilter->Update();
            m_CRLutCorrected = m_MultiplyFilter->GetOutput();
            // Set kept parameters values */
            m_CRLutCorrected->copyParametersValue(m_CRLutNotCorrected);
        } else {
            //No Correct then same output
            m_CRLutCorrected = m_CRLutNotCorrected;
        }
        //Put in map for scattering
        m_LUTMapOutput[CR_LUT_INDEX] = m_CRLutCorrected;
    }

    const ReducedLutComputationFilter::ReducedLutPointer& ReducedLutComputationFilter::GetCRLut()
    {
        return m_CRLutCorrected;
    }

    const ReducedLutComputationFilter::ReducedLutPointer& ReducedLutComputationFilter::GetCRLutNotCorrected()
    {
        return m_CRLutNotCorrected;
    }


    template <class T>
    std::string ReducedLutComputationFilter::GetKeyForModel(const T& p_map, const std::string& p_model) const
    {
        std::string l_mod = p_model;
        std::transform(l_mod.begin(), l_mod.end(), l_mod.begin(), ::tolower);
        for (typename T::const_iterator it = p_map.begin(); it!= p_map.end(); it++)
        {
            std::string l_tmp = it->first;
            std::transform(l_tmp.begin(), l_tmp.end(), l_tmp.begin(), ::tolower);
            if (l_mod.find(l_tmp) != std::string::npos )
            {
                return it->first;
            }
        }
        return std::string("");
    }

    // PrintSelf method
    void
    ReducedLutComputationFilter::PrintSelf(std::ostream& os, itk::Indent indent) const
    {
        Superclass::PrintSelf(os, indent);
        os << indent << "SolarZenith: " << m_SolarZenith << std::endl;
        os << indent << "SolarAzimuth: " << m_SolarAzimuth << std::endl;
        os << indent << "RefZenith: " << m_RefZenith << std::endl;
        os << indent << "RefAzimuth: " << m_RefAzimuth << std::endl;

    }

} // End namespace vns

