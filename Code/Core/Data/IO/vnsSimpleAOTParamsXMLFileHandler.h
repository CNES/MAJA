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
 *
 * VERSION : 3.3.0 : DM : LAIG-DM-MAJA-3044-CNES : 15 mars 2019 : Integration modifications oliver hagolle  *                                                                                                         *
 * VERSION : 3.2.0 : DM : LAIG-DM-MAJA-2737-CNES : 07 juin 2018 : CAMS utilises dans AOTEstimation          *
 * VERSION : 3.0.0 : DM : LAIG-DM-MAJA-2368-CNES : 27 juillet 2017 : Seuillage WaterPP sur bande B8/B9      *
 * VERSION : 3.0.0 : DM : LAIG-DM-MAJA-2299-CNES : 27 juillet 2017 : Seuillage WaterVapor                   *
 * VERSION : 3.0.0 : DM : LAIG-DM-MAJA-2369-CNES : 28 aout 2017 : Introduction données des CAMS             *
 * VERSION : 3.0.0 : DM : LAIG-DM-MAJA-2466-CNES : 18 sept 2017 : Ajout des coeffs de calibrations par bande*
 * VERSION : 2.0.0 : DM : LAIG-DM-MAJA-2149-CNES : 24 fevrier 2017 : Correction Cirrus - création           *
 * VERSION : 2.0.0 : DM : LAIG-DM-MAJA-2154-CNES : 13 avril 2017 : Ajout parametres Cirrus correction       *
 * VERSION : 2.0.0 : DM : LAIG-DM-MAJA-981-CNES : 13 avril 2017 : Ajout parametres Effet directionnels      *
 * VERSION : 2.0.0 : DM : LAIG-DM-MAJA-981-CNES : 2 mars 2017 : Directional Correction implementation       *
 * VERSION : 5-1-0 : DM : LAIG-DM-MAC-1666-CNES : 3 aout 2016 : Suppression parametre 'Min_Shadow_Variation'*
 * VERSION : 5-1-0 : FA : LAIG-FA-MAC-1752-CNES : 30 mai 2016 : MaJ GIPP L2COMM                             *
 * VERSION : 5-0-0 : DM : LAIG-DM-MAC-982-CNES : 12 avril 2016 : Nouveaux parametres algo                   *
 * VERSION : 4-4-0 : FA : LAIG-FA-MAC-133882-CS : 3 juin 2015 : Correction code pour la qualite             *
 * VERSION : 4-1-0 : DM : LAIG-DM-MAC-745-CNES : 2 septembre 2014 : Creation                                *
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id: vnsGippL2COMMEarthExplorerXMLFileHandler.h 8052 2014-05-12 12:24:42Z tfeuvrie $
 *                                                                                                          *
 ************************************************************************************************************/

#ifndef vns_SimpleAOTParamsXMLFileHandler_INCLUDED
#define vns_SimpleAOTParamsXMLFileHandler_INCLUDED

#include "vnsXMLFileHandler.h"

namespace vns
{

    class SimpleAOTParamsXMLFileHandler : public XMLFileHandler
    {
    public:
        typedef SimpleAOTParamsXMLFileHandler Self;

        typedef itk::SmartPointer<Self> Pointer;
        typedef itk::SmartPointer<const Self> ConstPointer;

        /** Run-time type information (and related methods). */
        itkTypeMacro( SimpleAOTParamsXMLFileHandler, XMLFileHandler )

        /** New macro for creation of through a Smart Pointer */
        itkNewMacro( Self )

        //Get Cams ponderation value
        double
        GetAOTKPondCAMS() const
        {
            return this->GetDoubleValue("/AOTEstimation/AOT_KPondCAMS");
        }
        //Get HeightScale value for exponential optical thickness decreasing
        double
        GetAOTHeightScale() const
        {
            return this->GetDoubleValue("/AOTEstimation/AOT_HeightScale");
        }
        //Multi spectral method
        ListOfUIntegers
        GetMSAOTBandCode() const
        {
            return this->GetUIntListValue("/AOTEstimation/Multi_Spectral_Method/MS_AOT_Band_Code");
        }
        unsigned int
        GetMSAOTRedBandCode() const
        {
            return this->GetUIntValue("/AOTEstimation/Multi_Spectral_Method/MS_AOT_Red_Band_Code");
        }
        unsigned int
        GetNDVIAOTBand1Code() const
        {
            return this->GetUIntValue("/AOTEstimation/Multi_Spectral_Method/NDVI_AOT_Band1_Code");
        }
        unsigned int
        GetNDVIAOTBand2Code() const
        {
            return this->GetUIntValue("/AOTEstimation/Multi_Spectral_Method/NDVI_AOT_Band2_Code");
        }
        double
        GetNDVIThreshold() const
        {
            return this->GetDoubleValue("/AOTEstimation/Multi_Spectral_Method/NDVI_Threshold");
        }
        double
        GetRedTOAThreshold() const
        {
            return this->GetDoubleValue("/AOTEstimation/Multi_Spectral_Method/Red_TOA_Threshold");
        }
        double
        GetMSSlope() const
        {
            return this->GetDoubleValue("/AOTEstimation/Multi_Spectral_Method/MS_Slope");
        }
        double
        GetMSYIntercept() const
        {
            return this->GetDoubleValue("/AOTEstimation/Multi_Spectral_Method/MS_YIntercept");
        }
        double
        GetNDVISlope() const
        {
            return this->GetDoubleValue("/AOTEstimation/Multi_Spectral_Method/NDVI_Slope");
        }
        double
        GetNDVIYIntercept() const
        {
            return this->GetDoubleValue("/AOTEstimation/Multi_Spectral_Method/NDVI_YIntercept");
        }
        double
        GetWMS() const
        {
            return this->GetDoubleValue("/AOTEstimation/Multi_Spectral_Method/W_MS");
        }
        //Multi temporal method
        ListOfUIntegers
        GetMTAOTBandCodeD() const
        {
            return this->GetUIntListValue("/AOTEstimation/Multi_Temporal_Method/MT_AOT_Band_Code_D");
        }
        ListOfUIntegers
		GetMTAOTBandReflectanceCompositeDm1() const
        {
        	return this->GetUIntListValue("/AOTEstimation/Multi_Temporal_Method/MT_AOT_Band_Reflectance_Composite_Dm1");
        }
        ListOfUIntegers
		GetMTAOTBandLUTCompositeDm1() const
        {
        	return this->GetUIntListValue("/AOTEstimation/Multi_Temporal_Method/MT_AOT_Band_LUT_Composite_Dm1");
        }
        unsigned int
        GetVarBandCodeD() const
        {
            return this->GetUIntValue("/AOTEstimation/Multi_Temporal_Method/Var_Band_Code_D");
        }
        unsigned int
		GetVarBandCodeDm1() const
        {
        	return this->GetUIntValue("/AOTEstimation/Multi_Temporal_Method/Var_Band_Code_Dm1");
        }
        double
        GetRelativeVarMax() const
        {
            return this->GetDoubleValue("/AOTEstimation/Multi_Temporal_Method/Relative_Var_Max");
        }
        double
        GetWMTequation2() const
        {
            return this->GetDoubleValue("/AOTEstimation/Multi_Temporal_Method/W_MT_equation2");
        }
        unsigned int
        GetStdevBandCode() const
        {
            return this->GetUIntValue("/AOTEstimation/Multi_Temporal_Method/Stdev_Band_Code");
        }
        double
        GetStdevMin() const
        {
            return this->GetDoubleValue("/AOTEstimation/Multi_Temporal_Method/Stdev_Min");
        }
        double
        GetStdNoise() const
        {
            return this->GetDoubleValue("/AOTEstimation/Multi_Temporal_Method/Std_Noise");
        }
        double
        GetAOTinitialDm1() const
        {
            return this->GetDoubleValue("/AOTEstimation/Multi_Temporal_Method/AOT_initial_Dm1");
        }
        double
        GetFirstAOT() const
        {
            return this->GetDoubleValue("/AOTEstimation/Multi_Temporal_Method/First_AOT");
        }
        double
        GetSecondAOT() const
        {
            return this->GetDoubleValue("/AOTEstimation/Multi_Temporal_Method/Second_AOT");
        }
        double
        GetTOAReflectanceMin() const
        {
            return this->GetDoubleValue("/AOTEstimation/Multi_Temporal_Method/TOA_Reflectance_Min");
        }
        double
        GetTOAReflectanceMax() const
        {
            return this->GetDoubleValue("/AOTEstimation/Multi_Temporal_Method/TOA_Reflectance_Max");
        }
        double
        GetTOAReflectanceStep() const
        {
            return this->GetDoubleValue("/AOTEstimation/Multi_Temporal_Method/TOA_Reflectance_Step");
        }
        double
        GetMinDifferenceThreshold() const
        {
            return this->GetDoubleValue(
                    "/AOTEstimation/Multi_Temporal_Method/Min_Difference_Thresholds_Calculation");
        }
        // Spectro temporal method
        double
        GetACoefficient() const
        {
            return this->GetDoubleValue("/AOTEstimation/Spectro_Temporal_Method/A_Coefficient");
        }
        double
        GetBCoefficient() const
        {
            return this->GetDoubleValue("/AOTEstimation/Spectro_Temporal_Method/B_Coefficient");
        }
        double
        GetMTWeightThreshold() const
        {
            return this->GetDoubleValue("/AOTEstimation/Spectro_Temporal_Method/MT_Weight_Threshold");
        }
        //All methods

        double
        GetAOTMaxAge() const
        {
            return this->GetDoubleValue("/AOTEstimation/AOT_Max_Age");
        }

        double
        GetDefaultAOT() const
        {
            return this->GetDoubleValue("/AOTEstimation/Default_AOT");
        }
        unsigned int
        GetStepAOT() const
        {
            return this->GetUIntValue("/AOTEstimation/Step_AOT");
        }
        unsigned int
        GetSamplingIntervalMin() const
        {
            return this->GetUIntValue("/AOTEstimation/Sampling_Interval_Min");
        }
        unsigned int
        GetSamplingIntervalMax() const
        {
            return this->GetUIntValue("/AOTEstimation/Sampling_Interval_Max");
        }
        unsigned int
        GetNPixMin() const
        {
            return this->GetUIntValue("/AOTEstimation/N_Pix_Min");
        }
        unsigned int
        GetIncrementNDistance() const
        {
            return this->GetUIntValue("/AOTEstimation/Increment_N_Distance");
        }
        unsigned int
        GetDarkBandCode() const
        {
            return this->GetUIntValue("/AOTEstimation/Dark_Band_Code");
        }
        unsigned int
        GetSEValidAOTRadius() const
        {
            return this->GetUIntValue("/AOTEstimation/SE_Valid_AOT_Radius");
        }
        unsigned int
        GetSEMaskDilatationRadius() const
        {
            return this->GetUIntValue("/AOTEstimation/SE_Mask_Dilatation_Radius");
        }
        unsigned int
        GetHalfWindowSize() const
        {
            return this->GetUIntValue("/AOTEstimation/Half_Window_Size");
        }
        double
        GetWdark() const
        {
            return this->GetDoubleValue("/AOTEstimation/W_dark");
        }
        double
        GetWbounds() const
        {
            return this->GetDoubleValue("/AOTEstimation/W_bounds");
        }
        double
        GetAOTmin() const
        {
            return this->GetDoubleValue("/AOTEstimation/AOT_min");
        }
        double
        GetFTolerance() const
        {
            return this->GetDoubleValue("/AOTEstimation/F_Tolerance");
        }
        double
        GetGTolerance() const
        {
            return this->GetDoubleValue("/AOTEstimation/G_Tolerance");
        }
        double
        GetXTolerance() const
        {
            return this->GetDoubleValue("/AOTEstimation/X_Tolerance");
        }
        double
        GetEpsilonFunction() const
        {
            return this->GetDoubleValue("/AOTEstimation/Epsilon_Function");
        }
        int
        GetMaxIterations() const
        {
            return this->GetIntValue("/AOTEstimation/Max_Iterations");
        }
        double
        GetAOTinitialD() const
        {
            return this->GetDoubleValue("/AOTEstimation/AOT_initial_D");
        }
        unsigned int
        GetInitWindowRadius() const
        {
            return this->GetUIntValue("/AOTEstimation/Init_Window_Radius");
        }
        unsigned int
        GetMaxWindowRadius() const
        {
            return this->GetUIntValue("/AOTEstimation/Max_Window_Radius");
        }
        double
        GetSigmaSmoothingAOT() const
        {
            return this->GetDoubleValue("/AOTEstimation/Sigma_Smoothing_AOT");
        }

    protected:
        /** Constructeur */
        SimpleAOTParamsXMLFileHandler(){}

        /** Destructeur */
        virtual
        ~SimpleAOTParamsXMLFileHandler(){}

    };

}

#endif
