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
 * VERSION : 3.2.1 : FA : LAIG-FA-MAJA-2861-CNES : 27 sept 2018 : Erreur dans la selection du netcdf        *
 * VERSION : 3.2.1 : FA : LAIG-FA-MAJA-2866-CNES : 26 sept 2018 : Error in log for files                    *
 * VERSION : 3.2.0 : DM : LAIG-DM-MAJA-2737-CNES : 07 juin 2018 : CAMS utilises dans AOTEstimation          *
 * VERSION : 3.0.0 : DM : LAIG-DM-MAJA-2369-CNES : 28 aout 2017 : Introduction données des CAMS             *
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id$                                                                                                     *
 *                                                                                                          *
 ************************************************************************************************************/
#include "vnsCAMSFileHandler.h"
#include "vnsLoggers.h"
#include "vnsMacro.h"
#include "vnsMath.h"
#include "vnsDate.h"
#include "itksys/SystemTools.hxx"
#include <itkImageRegionConstIterator.h>
#include <itkImageRegionIterator.h>
#include "vnsUtilities.h"
#include <iostream>
#include <string>
#include <algorithm>

namespace vns
{
    const unsigned int CAMSFileHandler::NETCDF_MAX_DATASET_NUMBER = 100;


    /** Constructor */
    CAMSFileHandler::CAMSFileHandler(): m_beforeFileFound(false),m_afterFileFound(false),m_IsValid(false), m_ImageTimeAquisitionJD(0),m_altLimit(0),
            m_indLimit(0), m_beforeSamplingRH(0),m_beforeAvgRH(0),m_afterSamplingRH(0),m_afterAvgRH(0),m_LimAOD(0),m_OutSamplingRH(0)
    {

    }

    /** Destructor */
    CAMSFileHandler::~CAMSFileHandler()
    {
    }

    void CAMSFileHandler::cleanup()
    {
        m_IsValid = false;
        m_ImageTimeAquisitionJD = 0;

        //Internal algo data
        m_AOTml.clear(); //AOT of the model level
        m_altLimit = 0; //limit altitude
        m_indLimit = 0; //Index of the limit altitude
        m_beforeAOTs.clear(); //AOT for before file
        m_beforeProps.clear(); //Model proportions for before file
        m_beforeSamplingRH = 0; //RH sampling for the before file
        m_beforeAvgRH = 0; //Average RH for the before file
        m_afterAOTs.clear(); //AOT for before file
        m_afterProps.clear(); //Model proportions for before file
        m_afterSamplingRH = 0; //RH sampling for the before file
        m_afterAvgRH = 0; //Average RH for the before file

        //Output
        m_OutProportions.clear();
        m_OutSamplingRH = 0; //RH sampling for the before file


    }
    void CAMSFileHandler::verifyData()
    {
        // Aerosol Models
        if (m_ListOfModels.size() ==0)
        {
            vnsExceptionDataMacro("ListOfModels are not filled")
        }
        // Extinction coefficients
        if (m_ExtinctionMap.size() == 0)
        {
            vnsExceptionDataMacro("ExtinctionMap is not filled")
        }
        // CAMS age limit
        if (vnsIsZeroDoubleMacro(m_LimAOD) == true)
        {
            vnsExceptionDataMacro("LimAOD is not filled")
        }
        // RH TAB
        if (m_RHTab.size() == 0)
        {
            vnsExceptionDataMacro("RHTab are not filled")
        }
        // RH Sampling
        if ( m_RHSampling.size() == 0)
        {
            vnsExceptionDataMacro("RHSampling are not filled")
        }
        // No file given
        if ( !m_beforeFileFound and !m_afterFileFound)
        {
            vnsExceptionDataMacro("No file given to treat")
        }
    }


    void CAMSFileHandler::ExtractCAMSData(const CornerType & p_CenterCorner,const double pImageProductAquisitionTimeJD)
    {
        //Do some cleanup
        this->cleanup();

        // Get the acquisition date in julian day
        m_ImageTimeAquisitionJD = pImageProductAquisitionTimeJD;

        //No need to continue if no valid file found
        if (!m_afterFileFound && !m_beforeFileFound)
        {
            vnsLogInfoMacro("No CAM file found for JD date "<<Date::GetDateUTCFromTm(Date::GetTmFromJulianDay(m_ImageTimeAquisitionJD)));
            return;
        }

        this->verifyData();
        // ---------------------------------------------------------------------------------------------
        if (m_afterFileFound )
        {
            vnsLogInfoMacro("Treating after CAMS file : "<<m_afterFile.AOT_Filename);
            this->TreatOneFile(m_afterFile, p_CenterCorner, AFTER);
        }

        if (m_beforeFileFound )
        {
            vnsLogInfoMacro("Treating before CAMS file : "<<m_beforeFile.AOT_Filename);
            this->TreatOneFile(m_beforeFile, p_CenterCorner, BEFORE);
        }

        //Consolidate data
        //only after
        if (m_afterFileFound && !m_beforeFileFound)
        {
            m_OutProportions = m_afterProps;
            m_OutSamplingRH = m_afterSamplingRH;
            m_IsValid = true;
            m_AOTImage = m_AOTAfterImage;
        }
        //only before
        if (!m_afterFileFound && m_beforeFileFound)
        {
            m_OutProportions = m_beforeProps;
            m_OutSamplingRH = m_beforeSamplingRH;
            m_IsValid = true;
            m_AOTImage = m_AOTBeforeImage;
        }
        //Both found need temporal interpolation
        if (m_afterFileFound && m_beforeFileFound)
        {
            TemporalInterpolation();
            m_IsValid = true;
        }

    }

    void CAMSFileHandler::TreatOneFile(CAMSFileType const& p_CamsFile, const CornerType & p_CenterCorner, const Mode& p_mode)
    {
        const std::string l_AOTFile = p_CamsFile.AOT_Filename;
        const std::string l_MRFile = p_CamsFile.MR_Filename;
        const std::string l_RHFile = p_CamsFile.RH_Filename;
        const ListOfDoubles l_ModelLevels = p_CamsFile.ModelLevels;
        const unsigned int l_NbNonInterpolable = p_CamsFile.NbNoInterpolable;

        if (l_AOTFile.empty() || l_MRFile.empty() || l_RHFile.empty())
        {
            vnsLogInfoMacro("l_AOTFile: " << l_AOTFile);
            vnsLogInfoMacro("l_MRFile: " << l_MRFile);
            vnsLogInfoMacro("l_RHFile: " << l_RHFile);
            vnsExceptionAlgorithmsProcessingMacro("Missing one CAMS file")
        }

        //Get RH data
        vnsLogDebugMacro("Opening: "<< l_RHFile );
        VectorPixel l_RHValue = GetCAMSValueAndDescription(l_RHFile, p_CenterCorner).second;
        vnsLogDebugValueMacro(l_RHValue);
        //Get RH levels
        ListOfDoubles l_RHLevels = GetCAMSLevels(l_RHFile);
        /** Verify integrity of data */
        if (l_RHLevels.size() != l_RHValue.GetSize())
        {
            vnsExceptionDataMacro("Mismatch between the number of RH values and the metadata")
        }
        std::stringstream l_ss;
        l_ss << "l_RHLevels:[";
        for (unsigned int i =0; i < l_RHLevels.size(); i++)
        {
            l_ss << l_RHLevels.at(i)<<" , ";
        }
        l_ss <<" ].";
        vnsLogDebugMacro(l_ss.str());
        //Get AOT Data
        AOTMapType l_AOTValueMap;
        l_ss.str("");
        bool l_isValid = true;
        unsigned int l_idx=0;
        while (l_isValid)
        {
            l_ss << l_AOTFile << "?&sdataidx="<<l_idx;
            if ( isValidDataset(l_ss.str()))
            {
                vnsLogDebugMacro("Opening: "<< l_ss.str() );
                const std::pair<std::string,VectorPixel> l_value = GetCAMSValueAndDescription(l_ss.str(), p_CenterCorner);
                if (!l_value.first.empty())
                {
                    const std::string l_curmod = GetModelFromDescription(l_value.first);
                    //The model is not in the list of treated
                    if(!l_curmod.empty())
                    {
                    l_AOTValueMap[l_curmod] = l_value.second;
                    }
                    if (l_AOTValueMap.size()==m_ListOfModels.size())
                    {
                        l_isValid = false;
                        break;
                    }
                }
                l_ss.str("");
                ++l_idx;
                if (l_idx > NETCDF_MAX_DATASET_NUMBER)
                {
                    vnsExceptionDataMacro("Netcdf "<<l_AOTFile<<" is malformed, stopping processing");
                }

            } else {
                l_isValid = false;
                break;
            }
        }
        //Verify Integrity
        if  (l_AOTValueMap.size()!=m_ListOfModels.size())
        {
            vnsExceptionDataMacro("AOT file does not contains all needed models");
        }

        //Get the sum of aot image
        ComputeSumOfAOT(l_AOTFile, p_mode);

        //Get MR Data
        MRMapType l_MRValueMap;
        l_isValid = true;
        l_idx=0;
        l_ss.str("");
        while (l_isValid)
        {
            l_ss << l_MRFile << "?&sdataidx="<<l_idx;
            if ( isValidDataset(l_ss.str()))
            {
                vnsLogDebugMacro("Opening: "<< l_ss.str() );
                std::pair<std::string,VectorPixel> l_value = GetCAMSValueAndDescription(l_ss.str(), p_CenterCorner);
                if(!l_value.first.empty())
                {
                    //Verify size
                    if (l_ModelLevels.size() != l_value.second.GetSize())
                    {
                        vnsExceptionDataMacro("Mismatch between the number of model levels and the data: Models "<<l_ModelLevels.size()<<" ,value: "<<l_value.second.GetSize())
                    }
                    l_MRValueMap[l_value.first] = l_value.second;
                    vnsLogDebugMacro(l_value.first<<" : "<<l_value.second);
                }
                l_ss.str("");
                ++l_idx;
                if (l_idx > NETCDF_MAX_DATASET_NUMBER)
                {
                    vnsExceptionDataMacro("Netcdf "<<l_MRFile<<" is malformed, stopping processing");
                }
            } else {
                l_isValid = false;
                break;
            }
        }

        //Get the average RH
        double l_averageRH = ComputeAverageRH(l_RHValue, l_MRValueMap, l_RHLevels, l_ModelLevels, l_NbNonInterpolable);
        vnsLogDebugValueMacro(l_averageRH);
        //Get the corresponding RH sampling
        double l_samplingRH = GetRHSampling(l_averageRH);
        vnsLogDebugValueMacro(l_samplingRH);
        //Compute proportions
        ProportionMapType l_props = ComputeModelsProportions(l_AOTValueMap);
        ProportionMapType::const_iterator it = l_props.begin();
        for (;it != l_props.end();++it)
        {
            vnsLogDebugMacro("Proportion for "<<it->first<<" = "<<it->second);
        }

        //Affect to corresponding
        switch (p_mode)
        {
        case BEFORE:
            m_beforeAOTs = l_AOTValueMap; //AOT for before file
            m_beforeProps = l_props; //Model proportions for before file
            m_beforeSamplingRH = l_samplingRH; //RH sampling for the before file
            m_beforeAvgRH = l_averageRH; //Average RH for the before file
            break;
        case AFTER:
            m_afterAOTs = l_AOTValueMap; //AOT for after file
            m_afterProps = l_props; //Model proportions for after file
            m_afterSamplingRH = l_samplingRH; //RH sampling for the after file
            m_afterAvgRH = l_averageRH; //Average RH for the after file
            break;
        default:
            break;
        }
    }

    /* Get the closest RH to avgRH */
    double
    CAMSFileHandler::GetRHSampling(const double p_avgRH)
    {
        /* Find the RH sampling value which is the closest to the average RH */
        double l_samplingRH = m_RHSampling[0];

        ListOfUIntegers::const_iterator itUp = std::lower_bound(m_RHSampling.begin(), m_RHSampling.end(), p_avgRH);
        if( itUp ==  m_RHSampling.end())
        {
            l_samplingRH = m_RHSampling.back();
        } else if (itUp ==  m_RHSampling.begin())
        {
            l_samplingRH = m_RHSampling.front();
        } else
        {
            const double l_upperBound = *itUp;
            const double l_lowerBound = *(--itUp);
            if ((p_avgRH - l_lowerBound) > (l_upperBound - p_avgRH))
            {
                l_samplingRH = l_upperBound;
            }
            else
            {
                l_samplingRH = l_lowerBound;
            }
        }
        return l_samplingRH;
    }

    /** Compute models proportions */
    CAMSFileHandler::ProportionMapType CAMSFileHandler::ComputeModelsProportions(const AOTMapType& p_AOT)
    {
        /* Compute the proportion of each aerosol model */
        VectorPixel l_sumOfAOT = p_AOT.begin()->second;
        l_sumOfAOT.Fill(0);
        //Sum up all the AOT for all the models
        for(AOTMapType::const_iterator it = p_AOT.begin(); it != p_AOT.end(); ++it)
        {
            l_sumOfAOT += it->second;
        }
        /* compute proportions for each model */
        ProportionMapType l_modelProps;
        for(AOTMapType::const_iterator it = p_AOT.begin(); it != p_AOT.end(); ++it)
        {
            VectorPixel l_aot = it->second;
            double l_props;
            l_props = l_aot[0] / l_sumOfAOT[0];
            l_modelProps[it->first] = l_props;
        }
        //Return proportions
        return l_modelProps;
    }


    /*Compute the average RH */
    double CAMSFileHandler::ComputeAverageRH(const VectorPixel& p_RH, const MRMapType& p_MR,
    		const ListOfDoubles& p_pressureLevels, const ListOfDoubles& p_ModelLevels ,
			const unsigned int p_NbNonInterpolable)
    {
        /**
         * Verify preconditions
         */
        if( p_RH.GetSize() != p_pressureLevels.size())
        {
            vnsExceptionDataMacro("The number of pressure Levels in cams doesn't match the number of RH");
        }

        /*Interpolate RH values from pressure levels to model levels, to match the mixing ratios
                Interpolation function*/
        Utilities::ListOfDoubles l_RHml;
        for (unsigned int i = 0; i < p_NbNonInterpolable;++i)
        {
            l_RHml.push_back(p_RH[0]);
        }
        for (unsigned int i = p_NbNonInterpolable; i < p_ModelLevels.size()-p_NbNonInterpolable;++i)
        {
            l_RHml.push_back(Interpolate1D(p_pressureLevels,p_RH, p_RH.GetSize(),p_ModelLevels[i]));
        }
        for (unsigned int i = p_ModelLevels.size()-p_NbNonInterpolable; i < p_ModelLevels.size();++i)
        {
            l_RHml.push_back(p_RH[p_RH.GetSize()-1]);
        }

        std::ostringstream l_ss;
        l_ss <<"Rhml[";
        for (unsigned int i = 0; i < p_ModelLevels.size();++i)
        {
            l_ss<<l_RHml.at(i)<<" , ";
        }
        l_ss <<"]";
        vnsLogDebugMacro(l_ss.str())

        /**Compute the altitude limit up to which the average RH will be computed,
            and the AOT of each model level (computed from the mixing ratios)*/
        this->ComputeAltitudeLimit(p_MR, l_RHml, p_ModelLevels);
        vnsLogDebugValueMacro(m_indLimit);
        vnsLogDebugValueMacro(m_altLimit);

        /* Compute the average RH, weighted by the AOT of each model level */
        double average = 0;
        double l_sumAOTAlt = 0;
        for (unsigned int i =  m_AOTml.size()-1; i>=m_indLimit ;i--)
        {
            l_sumAOTAlt += m_AOTml[i];
        }
        //Loop on model levels
        for (unsigned int i =  m_AOTml.size()-1; i>=m_indLimit ;i--)
        {
            average += l_RHml[i]*(m_AOTml[i]/l_sumAOTAlt);
        }

        return average;
    }

    /* Compute the total AOT of the column, from the mixing ratios, and save the AOT of each level*/
    void CAMSFileHandler::ComputeAltitudeLimit(const MRMapType& p_MR, const ListOfDoubles& p_RHModels,
    		const ListOfDoubles& p_ModelLevels)
    {
        /**
         * Verify preconditions
         */
        if( p_RHModels.size() != p_ModelLevels.size())
        {
            vnsExceptionDataMacro("The number of model Levels from parameter doesn't match the number of "
                    <<"model levels in data, ModelLevels: "<<p_ModelLevels.size()<<", "<<"DataLevels: "<<p_RHModels.size());
        }

        double l_AOTTot = 0;
        m_AOTml.clear();
        m_AOTml.resize(p_RHModels.size());

        //Loop on model levels
        for (unsigned int i =  p_RHModels.size()-1; i>0 ;i--)
        {
            const double l_RHLevel = p_RHModels[i];
            //Loop on models
            for (ListOfStrings::const_iterator a = m_ListOfModels.begin(); a != m_ListOfModels.end(); a++)
            {
                double l_AOTModel = 0;
                //Loop on mixing ratio (size or hydrophilic/hydrophobic)
                for (ExtinctionMapType::const_iterator m = m_ExtinctionMap.begin(); m != m_ExtinctionMap.end(); m++)
                {
                    const ExtinctionValue& l_ext = m->second;
                    const std::string& l_name = m->first;

                    //This coeff is related to this model
                    if (GetModelFromDescription(l_name).compare(*a) == 0)
                    {
                        double alpha = 0;
                        //Depends on RH, need interpolation
                        if (p_MR.find(l_ext.description) == p_MR.end())
                        {
                            vnsExceptionAlgorithmsProcessingMacro("Description : "<<l_ext.description<<" not found in the Mixing ratio map for model "<<l_name);
                        }

                        if (l_ext.isRhDep && (l_ext.values.size() > 1))
                        {
                            if (l_RHLevel <= m_RHTab.front())
                            {
                                alpha = l_ext.values.front();
                            }
                            else if (l_RHLevel >= m_RHTab.back())
                            {
                                alpha = l_ext.values.back();
                            }
                            else
                            {
                                alpha = Interpolate1D(m_RHTab, l_ext.values,
                                        l_ext.values.size(), l_RHLevel);
                            }
                        }
                        else //No RH dependent take first value
                        {
                            alpha = l_ext.values.front();
                        }
                        l_AOTModel += alpha*p_MR.at(l_ext.description)[i];
                    }
                }
                l_AOTModel *= (p_ModelLevels[i]-p_ModelLevels[i-1])*(1000.0*100.0/9.81);
                //Update sum
                m_AOTml[i] += l_AOTModel;
            }
            /*Update the total AOT of the column*/
            l_AOTTot += m_AOTml[i];
        }
        /*Search for the model level where limAOD *  AOT is reached*/
        double l_sumAOT = 0;
        m_indLimit = 0;
        //Loop on model levels search for values over lim
        for (int i =  p_RHModels.size()-1; i>=0 ;i--)
        {
            l_sumAOT += m_AOTml[i];
            if( l_sumAOT >= m_LimAOD*l_AOTTot)
            {
                m_indLimit = i;
                break;
            }
        }
        m_altLimit = p_ModelLevels[m_indLimit];
    }

    /** Temporal interpolation between cams */
    void CAMSFileHandler::TemporalInterpolation()
    {
        //Weight for each cams files
        double l_weigthAfter = (m_ImageTimeAquisitionJD - m_beforeFile.JulianDayAquisition)/
                (m_afterFile.JulianDayAquisition- m_beforeFile.JulianDayAquisition);
        double l_weigthBefore = 1 - l_weigthAfter;
        vnsLogDebugMacro("DateBefore : "<<std::setprecision(20)<<m_beforeFile.JulianDayAquisition);
        vnsLogDebugMacro("DateAfter : "<<std::setprecision(20)<<m_afterFile.JulianDayAquisition);
        vnsLogDebugMacro("DateProduct : "<<std::setprecision(20)<<m_ImageTimeAquisitionJD);
        vnsLogDebugMacro("WeightAfter : "<<std::setprecision(20)<<l_weigthAfter);
        vnsLogDebugMacro("WeightBefore : "<<std::setprecision(20)<<l_weigthBefore);
        //avg RH for the date
        double l_avgRh = m_beforeAvgRH*l_weigthBefore + m_afterAvgRH*l_weigthAfter;
        vnsLogDebugMacro("AverageRHCombined: "<<l_avgRh);
        //Find closest sampling
        m_OutSamplingRH = GetRHSampling(l_avgRh);
        /* Compute the proportion of each aerosol model */
        VectorPixel l_beforeSumOfAOT = m_beforeAOTs.begin()->second;
        l_beforeSumOfAOT.Fill(0);
        VectorPixel l_afterSumOfAOT = m_afterAOTs.begin()->second;
        l_afterSumOfAOT.Fill(0);
        //Sum up all the AOT for all the models
        for(AOTMapType::const_iterator it = m_beforeAOTs.begin(); it != m_beforeAOTs.end(); ++it)
        {
            l_beforeSumOfAOT += it->second;
        }
        for(AOTMapType::const_iterator it = m_afterAOTs.begin(); it != m_afterAOTs.end(); ++it)
        {
            l_afterSumOfAOT += it->second;
        }
        const VectorPixel l_weightedSumOfAOT = l_beforeSumOfAOT*l_weigthBefore+l_afterSumOfAOT*l_weigthAfter;
        /* compute proportions for each model */
        AOTMapType::const_iterator itBefore = m_beforeAOTs.begin();
        for(; itBefore != m_beforeAOTs.end() ; ++itBefore)
        {
            const VectorPixel& l_AOTBefore = itBefore->second;
            AOTMapType::const_iterator itAfter = m_afterAOTs.find(itBefore->first);
            if (itAfter == m_afterAOTs.end())
            {
                vnsExceptionDataMacro(itBefore->first<<" not found in the after CAMS proportions")
            }
            const VectorPixel& l_AOTAfter = itAfter->second;
            double l_props;
            l_props = (l_AOTBefore[0]*l_weigthBefore + l_AOTAfter[0]*l_weigthAfter)/l_weightedSumOfAOT.GetElement(0);
            m_OutProportions[itAfter->first] = l_props;
        }
        //Compute sum of AOT
        itk::ImageRegionIterator<AOTImageType> l_afterIt(m_AOTAfterImage, m_AOTBeforeImage->GetLargestPossibleRegion());
        itk::ImageRegionIterator<AOTImageType> l_beforIt(m_AOTBeforeImage, m_AOTAfterImage->GetLargestPossibleRegion());
        while(!l_afterIt.IsAtEnd() && !l_beforIt.IsAtEnd())
        {
            l_afterIt.Set(l_afterIt.Get()*l_weigthAfter+l_beforIt.Get()*l_weigthBefore);
            ++l_afterIt;
            ++l_beforIt;
        }
        m_AOTImage = m_AOTAfterImage;

    }

    /* linear interpolation same as python interp1D */
    template<typename T, typename P>
    double CAMSFileHandler::Interpolate1D(const T& p_indexes, const P& p_Values, const unsigned int p_ValuesSize, const double l_interIndex)
    {
        if( p_indexes.size() != p_ValuesSize)
        {
            return 0;
        }

        //Index to search is below the index of values
        if(p_indexes[0]>l_interIndex)
        {
            return p_indexes[0];
        }
        //Index to search is higher than index of values
        if(p_indexes[p_indexes.size()-1]<l_interIndex)
        {
            return p_indexes[p_indexes.size()-1];
        }

        double l_prevIdx = 0;
        double l_nextIdx = 0;
        for (unsigned int i = 0; i < p_indexes.size();++i)
        {
            if (p_indexes[i] <= l_interIndex)
            {
                l_prevIdx = i;
                l_nextIdx = i+1;
            }
        }
        return p_Values[l_prevIdx] + (l_interIndex - p_indexes[l_prevIdx])*(p_Values[l_nextIdx] - p_Values[l_prevIdx])/( p_indexes[l_nextIdx] - p_indexes[l_prevIdx]);
    }

    /** Read the cams and interpolate pixel at lat/long */
    std::pair<std::string,CAMSFileHandler::VectorPixel> CAMSFileHandler::GetCAMSValueAndDescription(const std::string & filename, const CornerType & p_CenterCorner)
    {
        //Read the file
        ReaderType::Pointer reader = ReaderType::New();
        reader->SetFileName(filename);
        reader->Update();

        //Interpolate
        const ImageType* image = reader->GetOutput();
        ContinuousIndexType l_contindex;
        PointType l_point;
        // Compute Lat,Long coordinate of the central point of the image
        // Read the UsefulImageGeoCoverageCenterCorner data from the HEADER_IMAGE_INFORMATIONS registered file
        l_point[0] = p_CenterCorner.Longitude;
        l_point[1] = p_CenterCorner.Latitude;
        image->TransformPhysicalPointToContinuousIndex(l_point, l_contindex);
        InterpolatorType::Pointer interp = InterpolatorType::New();
        interp->SetInputImage(reader->GetOutput());
        VectorPixel l_outPix = static_cast<VectorPixel> (interp->EvaluateAtContinuousIndex(l_contindex));

        //Search description
        std::string l_desc("");
        ListOfStrings l_longnames = GetMetadataValueAsListOfString(reader->GetOutput(),"long_name");
        // Search description containing one of the models requested
        for (ListOfStrings::const_iterator it = l_longnames.begin(); it != l_longnames.end();it++)
        {
            if (!GetModelFromDescription(*it).empty())
            {
                l_desc = *it;
                if (l_desc.find("=") != std::string::npos)
                {
                    l_desc = l_desc.substr(l_desc.find("=")+1);
                }
                break;
            }
        }

        //Apply factor and offeset to data
        const double factor = GetMetadataValueAsDouble(reader->GetOutput(), "factor");
        const double offset = GetMetadataValueAsDouble(reader->GetOutput(), "offset");

        for (unsigned int i = 0; i < l_outPix.GetNumberOfElements();i++)
        {
            l_outPix[i] = l_outPix[i]*factor + offset;
        }

        return std::pair<std::string, VectorPixel>(l_desc,l_outPix);
    }

    /**
     * Get the sum of AOT for the models
     */
    void CAMSFileHandler::ComputeSumOfAOT(const std::string & l_AOTFile, const Mode& p_mode)
    {
        //Get AOT Data
        std::ostringstream l_ss;
        l_ss.str("");
        bool l_isValid = true;
        bool l_isFirst = true;
        unsigned int l_idx=0;
        while (l_isValid)
        {
            l_ss << l_AOTFile << "?&sdataidx="<<l_idx;
            if ( isValidDataset(l_ss.str()))
            {
                vnsLogDebugMacro("Opening: "<< l_ss.str() );
                //Read the file
                ReaderType::Pointer reader = ReaderType::New();
                reader->SetFileName(l_ss.str());
                reader->Update();
                //Interpolate
                const ImageType* image = reader->GetOutput();
                //Search description
                std::string l_desc("");
                ListOfStrings l_longnames = GetMetadataValueAsListOfString(reader->GetOutput(),"long_name");
                // Search description containing one of the models requested
                for (ListOfStrings::const_iterator it = l_longnames.begin(); it != l_longnames.end();it++)
                {
                    if (!GetModelFromDescription(*it).empty())
                    {
                        l_desc = *it;
                        if (l_desc.find("=") != std::string::npos)
                        {
                            l_desc = l_desc.substr(l_desc.find("=")+1);
                        }
                        break;
                    }
                }

                if (!l_desc.empty())
                {
                    vnsLogDebugMacro("Using model "<<GetModelFromDescription(l_desc)<<" for the sum of AOT")
                    //Apply factor and offeset to data
                    const double factor = GetMetadataValueAsDouble(reader->GetOutput(), "factor");
                    const double offset = GetMetadataValueAsDouble(reader->GetOutput(), "offset");
                    //Allocate internal image
                    if (l_isFirst)
                    {
						switch (p_mode)
						{
						case AFTER:
							m_AOTAfterImage = AOTImageType::New();
							m_AOTAfterImage->SetRegions(image->GetLargestPossibleRegion());
							m_AOTAfterImage->SetOrigin(image->GetOrigin());
							m_AOTAfterImage->SetSignedSpacing(image->GetSignedSpacing());
							m_AOTAfterImage->Allocate();
							m_AOTAfterImage->FillBuffer(0.0);
							break;
						case BEFORE:
							m_AOTBeforeImage = AOTImageType::New();
							m_AOTBeforeImage->SetRegions(image->GetLargestPossibleRegion());
							m_AOTBeforeImage->SetOrigin(image->GetOrigin());
							m_AOTBeforeImage->SetSignedSpacing(image->GetSignedSpacing());
							m_AOTBeforeImage->Allocate();
							m_AOTBeforeImage->FillBuffer(0.0);
							break;
						}
						l_isFirst = false;
                    }

                    itk::ImageRegionConstIterator<ImageType> l_inIt(image, image->GetLargestPossibleRegion());
                    l_inIt.GoToBegin();
                    itk::ImageRegionIterator<AOTImageType> l_outIt;
                    switch (p_mode)
                    {
                    case AFTER:
                        l_outIt = itk::ImageRegionIterator<AOTImageType>(m_AOTAfterImage, image->GetLargestPossibleRegion());
                        break;
                    case BEFORE:
                        l_outIt = itk::ImageRegionIterator<AOTImageType>(m_AOTBeforeImage, image->GetLargestPossibleRegion());
                        break;
                    }
                    l_outIt.GoToBegin();
                    while(!l_inIt.IsAtEnd() && !l_outIt.IsAtEnd())
                    {
                        l_outIt.Set(l_outIt.Get() + (factor*l_inIt.Get()[0]+ offset));
                        ++l_outIt;
                        ++l_inIt;
                    }
                }

                l_ss.str("");
                ++l_idx;
                if (l_idx > NETCDF_MAX_DATASET_NUMBER)
                {
                    vnsExceptionDataMacro("Netcdf "<<l_AOTFile<<" is malformed, stopping processing");
                }
            } else {
                l_isValid = false;
                break;
            }
        }
    }


    /** Get the levels of the data in the CAMS */
    CAMSFileHandler::ListOfDoubles CAMSFileHandler::GetCAMSLevels(const std::string & filename)
    {
        //Read the file
        ReaderType::Pointer reader = ReaderType::New();
        reader->SetFileName(filename);
        reader->Update();

        return GetMetadataValueAsDoubleVector(reader->GetOutput(), "level_VALUES");
    }

    std::string CAMSFileHandler::GetModelFromDescription(const std::string& p_desc)
    {
        std::string l_mod("");
        std::string l_norms = p_desc;
        std::transform(l_norms.begin(), l_norms.end(), l_norms.begin(), ::tolower);
        l_norms.erase(std::remove_if(l_norms.begin(), l_norms.end(), ::isspace), l_norms.end());
        for (ListOfStrings::const_iterator mo = m_ListOfModels.begin(); mo != m_ListOfModels.end();mo++)
        {
            std::string l_tmp = *mo;
            std::transform(l_tmp.begin(), l_tmp.end(), l_tmp.begin(), ::tolower);
            if (l_norms.find(l_tmp) != std::string::npos )
            {
                l_mod = *mo;
                return l_mod;
            }
        }
        return l_mod;
    }

    double CAMSFileHandler::GetMetadataValueAsDouble(const ImageType* image, std::string const& name)
    {
        double l_ret = 0;
        std::string value = GetMetadataValueAsString(image, name);
        if (value.find("=") != std::string::npos)
        {
            value = value.substr(value.find("=")+1);
            l_ret = Utilities::AsDouble(value);
        }
        return l_ret;
    }

    CAMSFileHandler::ListOfDoubles CAMSFileHandler::GetMetadataValueAsDoubleVector(const ImageType* image, std::string const& name)
    {
        CAMSFileHandler::ListOfDoubles l_ret;
        std::string value = GetMetadataValueAsString(image, name);
        if (value.find("={") != std::string::npos)
        {
            value = value.substr(value.find("={")+2);
            value = value.substr(0, value.size()-1);
            std::istringstream iss(value);
            double sub;
            char c;
            while(iss>>sub)
            {
                l_ret.push_back(sub);
                //Remove coma
                iss>>c;
            };
        }
        return l_ret;
    }


    std::string CAMSFileHandler::GetMetadataValueAsString(const ImageType* image, std::string const& name)
    {
        std::string l_ret("");
        ListOfStrings l_vec = GetMetadataValueAsListOfString(image,name);
        if (l_vec.size() != 0)
        {
            l_ret = l_vec.front();
        }
        return l_ret;
    }

    CAMSFileHandler::ListOfStrings CAMSFileHandler::GetMetadataValueAsListOfString(const ImageType* image, std::string const& name)
    {
        ListOfStrings l_ret;
        itk::MetaDataDictionary meta = image->GetMetaDataDictionary();
        itk::MetaDataDictionary::ConstIterator it = meta.Begin();
        for (;it != meta.End();++it)
        {
            std::string value;
            itk::ExposeMetaData<std::string>(meta, it->first, value);
            itk::ExposeMetaData<std::string>(meta, it->first, value);
            if (value.find(name) != std::string::npos )
            {
                l_ret.push_back(value);
            }
        }
        return l_ret;
    }


    bool CAMSFileHandler::isValidDataset(const std::string& filename)
    {
        bool l_ret = true;
        //Read the file
        ReaderType::Pointer reader = ReaderType::New();
        reader->SetFileName(filename);
        try {
            reader->Update();
        } catch (itk::ExceptionObject& e) {
            l_ret = false;
        }
        return l_ret;
    }




    /** PrintSelf method */
    void
    CAMSFileHandler::PrintSelf(std::ostream& os, itk::Indent indent) const
    {
        this->Superclass::PrintSelf(os, indent);
    }

} // End namespace vns
