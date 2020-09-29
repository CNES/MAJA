# -*- coding: utf-8 -*-
#
# Copyright (C) 2020 Centre National d'Etudes Spatiales (CNES)
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#    http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#
#
"""
###################################################################################################

                        o     o
                        oo   oo   oo        o   oo        ,-.
                        o o o o  o  o       o  o  o       \_/
                        o  o  o o    o      o o    o     {|||D
                        o     o oooooo      o oooooo      / \
                        o     o o    o o    o o    o      `-^
                        o     o o    o  oooo  o    o

###################################################################################################

orchestrator.common.earth_explorer.gipp_l2_comm_earth_explorer_xml_file_handler -- shortdesc

orchestrator.common.earth_explorer.gipp_l2_comm_earth_explorer_xml_file_handler is a description

It defines classes_and_methods

###################################################################################################

:copyright: 2019 CNES. All rights reserved.
:license: license

###################################################################################################
"""

from orchestrator.common.maja_exceptions import MajaDriverException
import orchestrator.common.xml_tools as xml_tools

GIPP_COMM_HANDLER_XPATH = {
    "GIP_L2COMM_Mission": "/Earth_Explorer_File/Earth_Explorer_Header/Fixed_Header/Mission/text()",
    "FileName": "/Earth_Explorer_File/Earth_Explorer_Header/Fixed_Header/File_Name/text()",
    "FileClass": "/Earth_Explorer_File/Earth_Explorer_Header/Fixed_Header/File_Class/text()",
    "Creator": "/Earth_Explorer_File/Earth_Explorer_Header/Fixed_Header/Source/Creator/text()",
    "NoData": "/Earth_Explorer_File/Data_Block/Common_Parameters/No_Data/text()",
    "NoDataAsString": "/Earth_Explorer_File/Data_Block/Common_Parameters/No_Data/text()",
    "QuantificationValue": "/Earth_Explorer_File/Data_Block/Common_Parameters/TOA_Quantification_Value/text()",
    "MaxNoDataPercentage": "/Earth_Explorer_File/Data_Block/Common_Parameters/Max_No_Data_Percentage/text()",
    "MaxCloudPercentage": "/Earth_Explorer_File/Data_Block/Common_Parameters/Max_Cloud_Percentage/text()",
    "SlopeMinCosI": "/Earth_Explorer_File/Data_Block/Common_Parameters/Slope_Min_Cos_I/text()",
    "VAPNodataValueAsString": "/Earth_Explorer_File/Data_Block/Common_Parameters/VAP_Configuration_Values/" +
                              "VAP_No_Data_Value/text()",
    "VAPNodataValue": "/Earth_Explorer_File/Data_Block/Common_Parameters/VAP_Configuration_Values/VAP_No_Data_Value/" +
                      "text()",
    "VAPQuantificationValue": "/Earth_Explorer_File/Data_Block/Common_Parameters/VAP_Configuration_Values/" +
                              "VAP_Quantification_Value/text()",
    "AOTNodataValueAsString": "/Earth_Explorer_File/Data_Block/Common_Parameters/AOT_Configuration_Values/" +
                              "AOT_No_Data_Value/text()",
    "AOTNodataValue": "/Earth_Explorer_File/Data_Block/Common_Parameters/AOT_Configuration_Values/" +
                      "AOT_No_Data_Value/text()",
    "AOTQuantificationValue": "/Earth_Explorer_File/Data_Block/Common_Parameters/AOT_Configuration_Values/" +
                              "AOT_Quantification_Value/text()",
    "CalAdjustOption": "/Earth_Explorer_File/Data_Block/Common_Parameters/Cal_Adjust_Option/text()",
    "CalAdjustFactor": "/Earth_Explorer_File/Data_Block/Common_Parameters/Cal_Adjust_Factor/text()",
    "ConstantModel": "/Earth_Explorer_File/Data_Block/Common_Parameters/Constant_Model/text()",
    "BlueBandCode": "/Earth_Explorer_File/Data_Block/Bands_Definition/Thematic_Definition/Blue_Band_Code/text()",
    "GreenBandCode": "/Earth_Explorer_File/Data_Block/Bands_Definition/Thematic_Definition/Green_Band_Code/text()",
    "RedBandCode": "/Earth_Explorer_File/Data_Block/Bands_Definition/Thematic_Definition/Red_Band_Code/text()",
    "NIRBandCode": "/Earth_Explorer_File/Data_Block/Bands_Definition/Thematic_Definition/NIR_Band_Code/text()",
    "SWIR1BandCode": "/Earth_Explorer_File/Data_Block/Bands_Definition/Thematic_Definition/SWIR_Band_Code/text()",
    "Band2200Code": "/Earth_Explorer_File/Data_Block/Bands_Definition/Thematic_Definition/Band_2200_Code/text()",
    "QuicklookRedBandCode": "/Earth_Explorer_File/Data_Block/Bands_Definition/Thematic_Definition/" +
                            "Quicklook_Red_Band_Code/text()",
    "QuicklookGreenBandCode": "/Earth_Explorer_File/Data_Block/Bands_Definition/Thematic_Definition/" +
                              "Quicklook_Green_Band_Code/text()",
    "QuicklookBlueBandCode": "/Earth_Explorer_File/Data_Block/Bands_Definition/Thematic_Definition/" +
                             "Quicklook_Blue_Band_Code/text()",
    "QuicklookMinReflRedBand": "/Earth_Explorer_File/Data_Block/Quicklook_Refl_Variation/Min_Ref_Red_Band/text()",
    "QuicklookMaxReflRedBand": "/Earth_Explorer_File/Data_Block/Quicklook_Refl_Variation/Max_Ref_Red_Band/text()",
    "QuicklookMinReflGreenBand": "/Earth_Explorer_File/Data_Block/Quicklook_Refl_Variation/Min_Ref_Green_Band/text()",
    "QuicklookMaxReflGreenBand": "/Earth_Explorer_File/Data_Block/Quicklook_Refl_Variation/Max_Ref_Green_Band/text()",
    "QuicklookMinReflBlueBand": "/Earth_Explorer_File/Data_Block/Quicklook_Refl_Variation/Min_Ref_Blue_Band/text()",
    "QuicklookMaxReflBlueBand": "/Earth_Explorer_File/Data_Block/Quicklook_Refl_Variation/Max_Ref_Blue_Band/text()",
    "SaturationThreshold": "/Earth_Explorer_File/Data_Block/Subsampling/Saturation_Threshold/text()",
    "SaturationThresholdSub": "/Earth_Explorer_File/Data_Block/Subsampling/Saturation_Threshold_Sub/text()",
    "UseCamsData": "/Earth_Explorer_File/Data_Block/CAMS_Data_Preparation/Use_Cams_Data/text()",
    "ListOfModels": "/Earth_Explorer_File/Data_Block/CAMS_Data_Preparation/List_Of_Models/text()",
    "RHSampling": "/Earth_Explorer_File/Data_Block/CAMS_Data_Preparation/RH_Sampling/text()",
    "LimAOT": "/Earth_Explorer_File/Data_Block/CAMS_Data_Preparation/limAOT/text()",
    "TimeWindowsCAMS": "/Earth_Explorer_File/Data_Block/CAMS_Data_Preparation/Time_Windows_CAMS/text()",
    "DistanceThreshold": "/Earth_Explorer_File/Data_Block/DTM_Processing/Distance_Threshold/text()",
    "DTMViewingDirectionBandCode": "/Earth_Explorer_File/Data_Block/DTM_Processing/" +
                                   "DTM_Viewing_Direction_Band_Code/text()",
    "DirCorrOption": "/Earth_Explorer_File/Data_Block/Reduced_LUT_Computation/Directional_Correction_Option/text()",
    "CoefsR": "/Earth_Explorer_File/Data_Block/Reduced_LUT_Computation/Roughness_Coefficients/text()",
    "CoefsV": "/Earth_Explorer_File/Data_Block/Reduced_LUT_Computation/Volume_Coefficients/text()",
    "AzimuthRef": "/Earth_Explorer_File/Data_Block/Reduced_LUT_Computation/Azimuth_Reference/text()",
    "ZenithRef": "/Earth_Explorer_File/Data_Block/Reduced_LUT_Computation/Zenith_Reference/text()",
    "GIP_L2COMM_UseDefaultConstantWaterAmount": "/Earth_Explorer_File/Data_Block/Atmospheric_Absorption_Correction/" +
                                                "Use_Default_Constant_Water_Amount/text()",
    "WaterVaporBandCode": "/Earth_Explorer_File/Data_Block/Atmospheric_Absorption_Correction/" +
                          "Water_Vapor_Band_Code/text()",
    "WaterVaporReferenceBandCode": "/Earth_Explorer_File/Data_Block/Atmospheric_Absorption_Correction/" +
                                   "Water_Vapor_Reference_Band_Code/text()",
    "WaterVaporReflectanceThreshold": "/Earth_Explorer_File/Data_Block/Atmospheric_Absorption_Correction/" +
                                      "Water_Vapor_Reflectance_Threshold/text()",
    "WaterVaporReflectanceThresholdRef": "/Earth_Explorer_File/Data_Block/Atmospheric_Absorption_Correction/" +
                                         "Water_Vapor_Reflectance_Threshold_Ref/text()",
    "WaterVaporMaxThreshold": "/Earth_Explorer_File/Data_Block/Atmospheric_Absorption_Correction/" +
                              "Water_Vapor_Max_Threshold/text()",
    "WaterVaporInitWindowRadius": "/Earth_Explorer_File/Data_Block/Atmospheric_Absorption_Correction/" +
                                  "Water_Vapor_Init_Window_Radius/text()",
    "WaterVaporMaxWindowRadius": "/Earth_Explorer_File/Data_Block/Atmospheric_Absorption_Correction/" +
                                 "Water_Vapor_Max_Window_Radius/text()",
    "WaterVaporSEValidRadius": "/Earth_Explorer_File/Data_Block/Atmospheric_Absorption_Correction/" +
                               "Water_Vapor_SE_Valid_Radius/text()",
    "OzoneAmountDefaultValue": "/Earth_Explorer_File/Data_Block/Atmospheric_Absorption_Correction/" +
                               "Ozone_Amount_Default_Value/text()",
    "WaterAmountDefaultValue": "/Earth_Explorer_File/Data_Block/Atmospheric_Absorption_Correction/" +
                               "Water_Amount_Default_Value/text()",
    "RayleighCorrectionDefaultAOT": "/Earth_Explorer_File/Data_Block/Rayleigh_Correction/" +
                                    "Rayleigh_Correction_Default_AOT/text()",
    "SunglintThreshold": "/Earth_Explorer_File/Data_Block/Geometric_Flags/Sun_Glint_Threshold/text()",
    "HotspotThreshold": "/Earth_Explorer_File/Data_Block/Geometric_Flags/Hot_Spot_Threshold/text()",
    "AltitudeThreshold": "/Earth_Explorer_File/Data_Block/Cirrus_Flag/Altitude_Threshold/text()",
    "MinPercentCirrus": "/Earth_Explorer_File/Data_Block/Cirrus_Flag/Min_Percent_Cirrus/text()",
    "MinPercentCloud": "/Earth_Explorer_File/Data_Block/Cirrus_Flag/Min_Percent_Cloud/text()",
    "MinRstdCirrus": "/Earth_Explorer_File/Data_Block/Cirrus_Flag/Min_Rstd_Cirrus/text()",
    "MinPercentStdCirrus": "/Earth_Explorer_File/Data_Block/Cirrus_Flag/Min_Percent_Std_Cirrus/text()",
    "CirrusBandCode": "/Earth_Explorer_File/Data_Block/Cirrus_Mask/Cirrus_Band_Code/text()",
    "CirrusMaskThresholdOffset": "/Earth_Explorer_File/Data_Block/Cirrus_Mask/Cirrus_Mask_Threshold_Offset/text()",
    "CirrusMaskThresholdGain": "/Earth_Explorer_File/Data_Block/Cirrus_Mask/Cirrus_Mask_Threshold_Gain/text()",
    "MinPercentMaskCirrus": "/Earth_Explorer_File/Data_Block/Cirrus_Mask/Min_Percent_Mask_Cirrus/text()",
    "CirrusAltRef": "/Earth_Explorer_File/Data_Block/Cirrus_Mask/Cirrus_Alt_Ref/text()",
    "SESnowRadius": "/Earth_Explorer_File/Data_Block/Snow_Masking/SE_Snow_Radius/text()",
    "NDSIThreshold": "/Earth_Explorer_File/Data_Block/Snow_Masking/NDSI_Threshold/text()",
    "RedReflectanceThreshold": "/Earth_Explorer_File/Data_Block/Snow_Masking/Red_Reflectance_Threshold/text()",
    "SWIR1ReflectanceThreshold": "/Earth_Explorer_File/Data_Block/Snow_Masking/SWIR1_Reflectance_Threshold/text()",
    "UseWaterMask": "/Earth_Explorer_File/Data_Block/Snow_Masking/Use_Water_Mask/text()",
    "CLDViewingDirectionBandCode": "/Earth_Explorer_File/Data_Block/Cloud_Masking/" +
                                   "CLD_Viewing_Direction_Band_Code/text()",
    "RefinementOption": "/Earth_Explorer_File/Data_Block/Cloud_Masking/Refinement_Option/text()",
    "DiscardedPixelsRatioMin": "/Earth_Explorer_File/Data_Block/Cloud_Masking/Discarded_Pixels_Ratio_Min/text()",
    "CorrelBandCode": "/Earth_Explorer_File/Data_Block/Cloud_Masking/Correl_Band_Code/text()",
    "ShadowBandCode": "/Earth_Explorer_File/Data_Block/Cloud_Masking/Shadow_Band_Code/text()",
    "ReflectanceWaterBandCode": "/Earth_Explorer_File/Data_Block/Cloud_Masking/Reflectance_Water_Band_Code/text()",
    "CloudBlueReflectanceThreshold": "/Earth_Explorer_File/Data_Block/Cloud_Masking/" +
                                     "Cloud_Blue_Reflectance_Threshold/text()",
    "CloudRedReflectanceThreshold": "/Earth_Explorer_File/Data_Block/Cloud_Masking/" +
                                    "Cloud_Red_Reflectance_Threshold/text()",
    "NirRedRatioMaxThreshold": "/Earth_Explorer_File/Data_Block/Cloud_Masking/Nir_Red_Ratio_Max_Threshold/text()",
    "NirRedRatioMinThreshold": "/Earth_Explorer_File/Data_Block/Cloud_Masking/Nir_Red_Ratio_Min_Threshold/text()",
    "NirSwirRatioMinThreshold": "/Earth_Explorer_File/Data_Block/Cloud_Masking/Nir_Swir_Ratio_Min_Threshold/text()",
    "CorrelThreshold": "/Earth_Explorer_File/Data_Block/Cloud_Masking/Correl_Threshold/text()",
    "SEInputMasksRadius": "/Earth_Explorer_File/Data_Block/Cloud_Masking/SE_Input_Masks_Radius/text()",
    "WaterReflectanceThreshold": "/Earth_Explorer_File/Data_Block/Cloud_Masking/Water_Reflectance_Threshold/text()",
    "WaterSunglintReflectanceThreshold": "/Earth_Explorer_File/Data_Block/Cloud_Masking/" +
                                         "Water_Sunglint_Reflectance_Threshold/text()",
    "MinThresholdVarBlue": "/Earth_Explorer_File/Data_Block/Cloud_Masking/Min_Threshold_Var_Blue/text()",
    "MaxThresholdVarBlue": "/Earth_Explorer_File/Data_Block/Cloud_Masking/Max_Threshold_Var_Blue/text()",
    "DeltaThreshold": "/Earth_Explorer_File/Data_Block/Cloud_Masking/Delta_Threshold/text()",
    "CloudForgettingDuration": "/Earth_Explorer_File/Data_Block/Cloud_Masking/Cloud_Forgetting_Duration/text()",
    "DefaultAltitude": "/Earth_Explorer_File/Data_Block/Cloud_Masking/Default_Altitude/text()",
    "MaxCloudAltitude": "/Earth_Explorer_File/Data_Block/Cloud_Masking/Max_Cloud_Altitude/text()",
    "NumberOfStackImages": "/Earth_Explorer_File/Data_Block/Cloud_Masking/Number_Of_Stack_Images/text()",
    "KsigmaAltitude": "/Earth_Explorer_File/Data_Block/Cloud_Masking/Ksigma_Altitude/text()",
    "KNIRBlue": "/Earth_Explorer_File/Data_Block/Cloud_Masking/K_NIR_Blue/text()",
    "KRedBlue": "/Earth_Explorer_File/Data_Block/Cloud_Masking/K_Red_Blue/text()",
    "NCorrel": "/Earth_Explorer_File/Data_Block/Cloud_Masking/N_Correl/text()",
    "MinPixelCorrel": "/Earth_Explorer_File/Data_Block/Cloud_Masking/Min_Pixel_Correl/text()",
    "DeltaHMin": "/Earth_Explorer_File/Data_Block/Cloud_Masking/Delta_H_Min/text()",
    "DeltaHMax": "/Earth_Explorer_File/Data_Block/Cloud_Masking/Delta_H_Max/text()",
    "DeltaHStep": "/Earth_Explorer_File/Data_Block/Cloud_Masking/Delta_H_Step/text()",
    "AbsNbpixThreshold": "/Earth_Explorer_File/Data_Block/Cloud_Masking/Abs_Nbpix_Threshold/text()",
    "ThresholdDiffImage": "/Earth_Explorer_File/Data_Block/Cloud_Masking/Threshold_Diff_Image/text()",
    "ShadowCloudRatioMax": "/Earth_Explorer_File/Data_Block/Cloud_Masking/Shadow_Cloud_Ratio_Max/text()",
    "ShadowCloudRatioMin": "/Earth_Explorer_File/Data_Block/Cloud_Masking/Shadow_Cloud_Ratio_Min/text()",
    "ShadowCloudRatioPixNb": "/Earth_Explorer_File/Data_Block/Cloud_Masking/Shadow_Cloud_Ratio_Pix_Nb/text()",
    "ProjShadDarkeningMax": "/Earth_Explorer_File/Data_Block/Cloud_Masking/Proj_Shad_Darkening_Max/text()",
    "DarkeningMinPercentile": "/Earth_Explorer_File/Data_Block/Cloud_Masking/Darkening_Min_Percentile/text()",
    "CloudCoverMultFactor": "/Earth_Explorer_File/Data_Block/Cloud_Masking/Cloud_Cover_Mult_Factor/text()",
    "ShadVarDarkeningMax": "/Earth_Explorer_File/Data_Block/Cloud_Masking/Shad_Var_Darkening_Max/text()",
    "ShadVarPercentile": "/Earth_Explorer_File/Data_Block/Cloud_Masking/Shad_Var_Percentile/text()",
    "ShadVarCompositeLabelRatio": "/Earth_Explorer_File/Data_Block/Cloud_Masking/Shad_Var_Composite_Label_Ratio/text()",
    "SECloudRadius": "/Earth_Explorer_File/Data_Block/Cloud_Masking/SE_Cloud_Radius/text()",
    "WaterMaskNumber": "/Earth_Explorer_File/Data_Block/Water_Masking/Water_Mask_Number/text()",
    "WaterNDVIThreshold": "/Earth_Explorer_File/Data_Block/Water_Masking/Water_NDVI_Threshold/text()",
    "WaterRedReflectanceThreshold": "/Earth_Explorer_File/Data_Block/Water_Masking/" +
                                    "Water_Red_Reflectance_Threshold/text()",
    "WaterSlopeThreshold": "/Earth_Explorer_File/Data_Block/Water_Masking/Water_Slope_Threshold/text()",
    "WaterReflectanceVariationThreshold": "/Earth_Explorer_File/Data_Block/Water_Masking/" +
                                          "Water_Reflectance_Variation_Threshold/text()",
    "MaximumSunglintReflectance": "/Earth_Explorer_File/Data_Block/Water_Masking/Maximum_Sunglint_Reflectance/text()",
    "MinPercentPossibleWater": "/Earth_Explorer_File/Data_Block/Water_Masking/Min_Percent_Possible_Water/text()",
    "SRTMSigmaSmoothing": "/Earth_Explorer_File/Data_Block/Water_Masking/SRTM_Sigma_Smoothing/text()",
    "SRTMPercent": "/Earth_Explorer_File/Data_Block/Water_Masking/SRTM_Percent/text()",
    "WaterBandCode": "/Earth_Explorer_File/Data_Block/Rain_Flag/Water_Band_Code/text()",
    "ReflectanceDecreaseThreshold": "/Earth_Explorer_File/Data_Block/Rain_Flag/Reflectance_Decrease_Threshold/text()",
    "AOTMethod": "/Earth_Explorer_File/Data_Block/AOT_Estimation/AOT_Method/text()",
    "MSAOTBandCode": "/Earth_Explorer_File/Data_Block/AOT_Estimation/Multi_Spectral_Method/MS_AOT_Band_Code/text()",
    "MSAOTRedBandCode": "/Earth_Explorer_File/Data_Block/AOT_Estimation/Multi_Spectral_Method/" +
                        "MS_AOT_Red_Band_Code/text()",
    "NDVIAOTBand1Code": "/Earth_Explorer_File/Data_Block/AOT_Estimation/Multi_Spectral_Method/" +
                        "NDVI_AOT_Band1_Code/text()",
    "NDVIAOTBand2Code": "/Earth_Explorer_File/Data_Block/AOT_Estimation/Multi_Spectral_Method/" +
                        "NDVI_AOT_Band2_Code/text()",
    "NDVIThreshold": "/Earth_Explorer_File/Data_Block/AOT_Estimation/Multi_Spectral_Method/NDVI_Threshold/text()",
    "RedTOAThreshold": "/Earth_Explorer_File/Data_Block/AOT_Estimation/Multi_Spectral_Method/Red_TOA_Threshold/text()",
    "MSSlope": "/Earth_Explorer_File/Data_Block/AOT_Estimation/Multi_Spectral_Method/MS_Slope/text()",
    "MSYIntercept": "/Earth_Explorer_File/Data_Block/AOT_Estimation/Multi_Spectral_Method/MS_YIntercept/text()",
    "NDVISlope": "/Earth_Explorer_File/Data_Block/AOT_Estimation/Multi_Spectral_Method/NDVI_Slope/text()",
    "NDVIYIntercept": "/Earth_Explorer_File/Data_Block/AOT_Estimation/Multi_Spectral_Method/NDVI_YIntercept/text()",
    "WMS": "/Earth_Explorer_File/Data_Block/AOT_Estimation/Multi_Spectral_Method/W_MS/text()",
    "MTAOTBandCode": "/Earth_Explorer_File/Data_Block/AOT_Estimation/Multi_Temporal_Method/MT_AOT_Band_Code/text()",
    "VarBandCode": "/Earth_Explorer_File/Data_Block/AOT_Estimation/Multi_Temporal_Method/Var_Band_Code/text()",
    "RelativeVarMax": "/Earth_Explorer_File/Data_Block/AOT_Estimation/Multi_Temporal_Method/Relative_Var_Max/text()",
    "WMTequation2": "/Earth_Explorer_File/Data_Block/AOT_Estimation/Multi_Temporal_Method/W_MT_equation2/text()",
    "StdevBandCode": "/Earth_Explorer_File/Data_Block/AOT_Estimation/Multi_Temporal_Method/Stdev_Band_Code/text()",
    "StdevMin": "/Earth_Explorer_File/Data_Block/AOT_Estimation/Multi_Temporal_Method/Stdev_Min/text()",
    "StdNoise": "/Earth_Explorer_File/Data_Block/AOT_Estimation/Multi_Temporal_Method/Std_Noise/text()",
    "AOTinitialDm1": "/Earth_Explorer_File/Data_Block/AOT_Estimation/Multi_Temporal_Method/AOT_initial_Dm1/text()",
    "FirstAOT": "/Earth_Explorer_File/Data_Block/AOT_Estimation/Multi_Temporal_Method/First_AOT/text()",
    "SecondAOT": "/Earth_Explorer_File/Data_Block/AOT_Estimation/Multi_Temporal_Method/Second_AOT/text()",
    "TOAReflectanceMin": "/Earth_Explorer_File/Data_Block/AOT_Estimation/Multi_Temporal_Method/" +
                         "TOA_Reflectance_Min/text()",
    "TOAReflectanceMax": "/Earth_Explorer_File/Data_Block/AOT_Estimation/Multi_Temporal_Method/" +
                         "TOA_Reflectance_Max/text()",
    "TOAReflectanceStep": "/Earth_Explorer_File/Data_Block/AOT_Estimation/Multi_Temporal_Method/" +
                          "TOA_Reflectance_Step/text()",
    "MinDifferenceThreshold": "/Earth_Explorer_File/Data_Block/AOT_Estimation/Multi_Temporal_Method/" +
                              "Min_Difference_Thresholds_Calculation/text()",
    "ACoefficient": "/Earth_Explorer_File/Data_Block/AOT_Estimation/Spectro_Temporal_Method/A_Coefficient/text()",
    "BCoefficient": "/Earth_Explorer_File/Data_Block/AOT_Estimation/Spectro_Temporal_Method/B_Coefficient/text()",
    "MTWeightThreshold": "/Earth_Explorer_File/Data_Block/AOT_Estimation/Spectro_Temporal_Method/" +
                         "MT_Weight_Threshold/text()",
    "AOTMaxAge": "/Earth_Explorer_File/Data_Block/AOT_Estimation/AOT_Max_Age/text()",
    "DefaultAOT": "/Earth_Explorer_File/Data_Block/AOT_Estimation/Default_AOT/text()",
    "StepAOT": "/Earth_Explorer_File/Data_Block/AOT_Estimation/Step_AOT/text()",
    "SamplingIntervalMin": "/Earth_Explorer_File/Data_Block/AOT_Estimation/Sampling_Interval_Min/text()",
    "SamplingIntervalMax": "/Earth_Explorer_File/Data_Block/AOT_Estimation/Sampling_Interval_Max/text()",
    "NPixMin": "/Earth_Explorer_File/Data_Block/AOT_Estimation/N_Pix_Min/text()",
    "IncrementNDistance": "/Earth_Explorer_File/Data_Block/AOT_Estimation/Increment_N_Distance/text()",
    "DarkBandCode": "/Earth_Explorer_File/Data_Block/AOT_Estimation/Dark_Band_Code/text()",
    "SEValidAOTRadius": "/Earth_Explorer_File/Data_Block/AOT_Estimation/SE_Valid_AOT_Radius/text()",
    "SEMaskDilatationRadius": "/Earth_Explorer_File/Data_Block/AOT_Estimation/SE_Mask_Dilatation_Radius/text()",
    "HalfWindowSize": "/Earth_Explorer_File/Data_Block/AOT_Estimation/Half_Window_Size/text()",
    "Wdark": "/Earth_Explorer_File/Data_Block/AOT_Estimation/W_dark/text()",
    "Wbounds": "/Earth_Explorer_File/Data_Block/AOT_Estimation/W_bounds/text()",
    "AOTmin": "/Earth_Explorer_File/Data_Block/AOT_Estimation/AOT_min/text()",
    "FTolerance": "/Earth_Explorer_File/Data_Block/AOT_Estimation/F_Tolerance/text()",
    "GTolerance": "/Earth_Explorer_File/Data_Block/AOT_Estimation/G_Tolerance/text()",
    "XTolerance": "/Earth_Explorer_File/Data_Block/AOT_Estimation/X_Tolerance/text()",
    "EpsilonFunction": "/Earth_Explorer_File/Data_Block/AOT_Estimation/Epsilon_Function/text()",
    "MaxIterations": "/Earth_Explorer_File/Data_Block/AOT_Estimation/Max_Iterations/text()",
    "AOTinitialD": "/Earth_Explorer_File/Data_Block/AOT_Estimation/AOT_initial_D/text()",
    "InitWindowRadius": "/Earth_Explorer_File/Data_Block/AOT_Estimation/Init_Window_Radius/text()",
    "MaxWindowRadius": "/Earth_Explorer_File/Data_Block/AOT_Estimation/Max_Window_Radius/text()",
    "SigmaSmoothingAOT": "/Earth_Explorer_File/Data_Block/AOT_Estimation/Sigma_Smoothing_AOT/text()",
    "KPondsCams": "/Earth_Explorer_File/Data_Block/AOT_Estimation/AOT_KPondCAMS/text()",
    "HeighScale": "/Earth_Explorer_File/Data_Block/AOT_Estimation/AOT_HeightScale/text()",
    "CirrusCorrectionOption": "/Earth_Explorer_File/Data_Block/Cirrus_Correction/Cirrus_Correction_Option/text()",
    "CirrusCorrectionGammaBandCodes": "/Earth_Explorer_File/Data_Block/Cirrus_Correction/" +
                                      "Cirrus_Gamma_Band_Codes/text()",
    "CirrusCorrectionSWIRBandCodes": "/Earth_Explorer_File/Data_Block/Cirrus_Correction/" +
                                     "Cirrus_SWIR_Band_Codes/text()",
    "CirrusCorrectionMinPixInList": "/Earth_Explorer_File/Data_Block/Cirrus_Correction/" +
                                    "Cirrus_Min_Pix_In_List/text()",
    "CirrusCorrectionMaxDistance": "/Earth_Explorer_File/Data_Block/Cirrus_Correction/Cirrus_Max_Distance/text()",
    "CirrusCorrectionMinGammaValue": "/Earth_Explorer_File/Data_Block/Cirrus_Correction/Cirrus_Min_Gamma_Value/text()",
    "CirrusCorrectionMaxGammaValue": "/Earth_Explorer_File/Data_Block/Cirrus_Correction/Cirrus_Max_Gamma_Value/text()",
    "CirrusCorrectionSWIRGammaFactor": "/Earth_Explorer_File/Data_Block/Cirrus_Correction/" +
                                       "Cirrus_SWIR_Gamma_Factor/text()",
    "CirrusCorrectionThresholdFactor": "/Earth_Explorer_File/Data_Block/Cirrus_Correction/" +
                                       "Cirrus_Threshold_Factor/text()",
    "CirrusMaxReflectance": "/Earth_Explorer_File/Data_Block/Cirrus_Correction/" +
                                       "Cirrus_Max_Reflectance/text()",
    "MaxAOT": "/Earth_Explorer_File/Data_Block/Composite/Max_AOT/text()",
    "MinimumHighAOTStd": "/Earth_Explorer_File/Data_Block/Composite/Minimum_High_AOT_Std/text()",
    "MaxAge": "/Earth_Explorer_File/Data_Block/Composite/Max_Age/text()",
    "SEAOTRadius": "/Earth_Explorer_File/Data_Block/Composite/SE_AOT_Radius/text()",
    "CoefSigmaAOT": "/Earth_Explorer_File/Data_Block/Composite/Coef_Sigma_AOT/text()",
    "EnvCorrOption": "/Earth_Explorer_File/Data_Block/Environment_Correction/Env_Corr_Option/text()",
    "EnvCorrRadius": "/Earth_Explorer_File/Data_Block/Environment_Correction/Env_Corr_Radius/text()",
    "MinCosE": "/Earth_Explorer_File/Data_Block/Slope_Correction/Min_Cos_E/text()",
    "MinCosRatio": "/Earth_Explorer_File/Data_Block/Slope_Correction/Min_Cos_Ratio/text()"}


class GippL2COMMEarthExplorerXMLFileHandler(object):

    def __init__(self, gipp_filename):
        self.gipp_filename = gipp_filename
        self.root = xml_tools.get_root_xml(self.gipp_filename, deannotate=True)
        self.l2_comm_values = {}

        for key, value in list(GIPP_COMM_HANDLER_XPATH.items()):
            result = xml_tools.get_only_value(self.root, value, check=True)
            if result is not None:
                self.l2_comm_values[key] = result

    def get_value(self, key, check=False):
        if key in self.l2_comm_values:
            return self.l2_comm_values[key]
        else:
            if check:
                return None
            else:
                raise MajaDriverException("No " + key + " in L2COMM dictionnary")

    def get_value_f(self, key, check=False):
        return float(self.get_value(key, check))

    def get_value_i(self, key, check=False):
        return int(self.get_value(key, check))

    def get_value_b(self, key, check=False):
        return xml_tools.as_bool(self.get_value(key, check))


if __name__ == '__main__':
    pass
