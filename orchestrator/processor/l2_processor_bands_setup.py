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
"""
###################################################################################################
#
#                        o     o
#                        oo   oo   oo        o   oo        ,-.
#                        o o o o  o  o       o  o  o       \_/
#                        o  o  o o    o      o o    o     {|||)<
#                        o     o oooooo      o oooooo      / \
#                        o     o o    o o    o o    o      `-^
#                        o     o o    o  oooo  o    o
#
###################################################################################################

orchestrator.processor.base_processor -- shortdesc

orchestrator.processor.base_processor is the base of all processors

It defines method mandatory for a processor


###################################################################################################
"""

from orchestrator.common.maja_exceptions import *
from orchestrator.common.constants import AOTEstimation
from orchestrator.common.gipp_utils import *
LOGGER = configure_logger(__name__)


def prepare_bands(self, l_banddefinitons, l_gippl2commhandler, l_aotmethod, l_currentconfigadmincamera,
                  p_initmode, p_inputl2imagefilereader, l_global_params_dict):
    # = == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == ==
    # Get the index of each band         used in L2 algorithms
    # = == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == ==
    # Computes the list of the RTA, RTC and RCR  bands, use by the Date D  product, i.e from the GIP L2COMM
    #  Thebandsselectedare:
    # - BlueBand
    # - RedBand
    # - CorrelBand
    # - ShadowBand
    # - NIRBand
    # - WaterBand
    # - DarkBand
    # Si l_AOTMethod = MULTITEMPORAL or l_AOTMethod = SPECTROTEMPORAL, adds:
    # - VarBandCode
    # - MTAOTBandCode(listes)
    l_SetOfBandCodeForReflectancesComposite_DateD = set()
    l_SetOfBandCodeForLTCComposite_DateD = set()
    l_BlueBandCode_DateD = l_gippl2commhandler.get_value("BlueBandCode")
    l_BlueBandIndex_DateD = l_banddefinitons.get_band_id_in_l2_coarse(l_BlueBandCode_DateD)
    l_global_params_dict["BlueBandIndex_DateD"] = l_BlueBandIndex_DateD
    l_GreenBandCode_DateD = l_gippl2commhandler.get_value("GreenBandCode")
    l_GreenBandIndex_DateD = l_banddefinitons.get_band_id_in_l2_coarse(l_GreenBandCode_DateD)
    l_global_params_dict["GreenBandIndex_DateD"] = l_GreenBandIndex_DateD
    l_RedBandCode_DateD = l_gippl2commhandler.get_value("RedBandCode")
    l_RedBandIndex_DateD = l_banddefinitons.get_band_id_in_l2_coarse(l_RedBandCode_DateD)
    l_global_params_dict["RedBandIndex_DateD"] = l_RedBandIndex_DateD
    l_NIRBandCode_DateD = l_gippl2commhandler.get_value("NIRBandCode")
    l_NIRBandIndex_DateD = l_banddefinitons.get_band_id_in_l2_coarse(l_NIRBandCode_DateD)
    l_global_params_dict["NIRBandIndex_DateD"] = l_NIRBandIndex_DateD
    # Band used in Cloud Masking and Composite Image
    l_CorrelBandCode_DateD = l_gippl2commhandler.get_value("CorrelBandCode")
    l_CorrelBandIndex_DateD = l_banddefinitons.get_band_id_in_l2_coarse(l_CorrelBandCode_DateD)
    l_global_params_dict["CorrelBandIndex_DateD"] = l_CorrelBandIndex_DateD
    # Band used in Cloud Masking
    l_ShadowBandCode_DateD = l_gippl2commhandler.get_value("ShadowBandCode")
    l_ShadowBandIndex_DateD = l_banddefinitons.get_band_id_in_l2_coarse(l_ShadowBandCode_DateD)
    l_global_params_dict["ShadowBandIndex_DateD"] = l_ShadowBandIndex_DateD
    # Bands used in Rain Detection
    l_WaterBandCode_DateD = l_gippl2commhandler.get_value("WaterBandCode")
    l_WaterBandIndex_DateD = l_banddefinitons.get_band_id_in_l2_coarse(l_WaterBandCode_DateD)
    l_global_params_dict["WaterBandIndex_DateD"] = l_WaterBandIndex_DateD
    l_ReflectanceWaterBandCode_DateD = l_gippl2commhandler.get_value("ReflectanceWaterBandCode")
    l_ReflectanceWaterBandIndex_DateD = l_banddefinitons.get_band_id_in_l2_coarse(l_ReflectanceWaterBandCode_DateD)
    l_global_params_dict["ReflectanceWaterBandIndex_DateD"] = l_ReflectanceWaterBandIndex_DateD
    l_DarkBandCode_DateD = l_gippl2commhandler.get_value("DarkBandCode")
    l_DarkBandIndex_DateD = l_banddefinitons.get_band_id_in_l2_coarse(l_DarkBandCode_DateD)  # index starts at one
    l_global_params_dict["DarkBandIndex_DateD"] = l_DarkBandIndex_DateD
    LOGGER.debug("Band Index for 'l_BlueBandIndex_DateD': " + str(l_BlueBandIndex_DateD))
    LOGGER.debug("Band Index for 'l_GreenBandIndex_DateD': " + str(l_GreenBandIndex_DateD))
    LOGGER.debug("Band Index for 'l_RedBandIndex_DateD': " + str(l_RedBandIndex_DateD))
    LOGGER.debug("Band Index for 'l_NIRBandIndex_DateD': " + str(l_NIRBandIndex_DateD))
    LOGGER.debug("Band Index for 'l_CorrelBandIndex_DateD': " + str(l_CorrelBandIndex_DateD))
    LOGGER.debug("Band Index for 'l_ShadowBandIndex_DateD': " + str(l_ShadowBandIndex_DateD))
    LOGGER.debug("Band Index for 'l_WaterBandIndex_DateD': " + str(l_WaterBandIndex_DateD))
    LOGGER.debug("Band Index for 'l_DarkBandIndex_DateD': " + str(l_DarkBandIndex_DateD))
    # ---------------------------------------------------------------------------------------------
    # Set the list of band code for composite
    l_SetOfBandCodeForReflectancesComposite_DateD.add(l_BlueBandCode_DateD)
    l_SetOfBandCodeForReflectancesComposite_DateD.add(l_RedBandCode_DateD)
    l_SetOfBandCodeForReflectancesComposite_DateD.add(l_CorrelBandCode_DateD)
    l_SetOfBandCodeForReflectancesComposite_DateD.add(l_ShadowBandCode_DateD)
    l_SetOfBandCodeForReflectancesComposite_DateD.add(l_NIRBandCode_DateD)
    l_SetOfBandCodeForReflectancesComposite_DateD.add(l_WaterBandCode_DateD)
    l_SetOfBandCodeForReflectancesComposite_DateD.add(l_DarkBandCode_DateD)
    # ---------------------------------------------------------------------------------------------
    # Bands used in Composite for AOT
    # - VarBandCode
    # - MTAOTUsedBand
    l_VarBandCode_DateD = ""
    l_VarBandIndex_DateD = 9999
    l_AOTUsedBandCodesDateD = []
    l_MTAOTbandsDateD = []
    # If multi temporal
    if l_aotmethod == AOTEstimation.MULTITEMPORAL or l_aotmethod == AOTEstimation.SPECTROTEMPORAL:
        # VarBandCode
        l_VarBandCode_DateD = l_gippl2commhandler.get_value("VarBandCode")
        l_VarBandIndex_DateD = l_banddefinitons.get_band_id_in_l2_coarse(l_VarBandCode_DateD)
        l_global_params_dict["VarBandIndex_DateD"] = l_VarBandIndex_DateD
        LOGGER.debug("Band Index for 'l_VarBandIndex_DateD': " + str(l_VarBandIndex_DateD))
        # Addthis band in the list of bands for the composite
        l_SetOfBandCodeForReflectancesComposite_DateD.add(l_VarBandCode_DateD)
        # AOTBandCode list
        l_AOTUsedBandCodesDateD = l_gippl2commhandler.get_value("MTAOTBandCode").split()
        # Create the list of channel indexes
        for band in l_AOTUsedBandCodesDateD:
            # Add this band in the list of bands for the 'reflectance' composite
            l_SetOfBandCodeForReflectancesComposite_DateD.add(band)
            # Add this band in the list of bands for the 'LTC' composite
            l_SetOfBandCodeForLTCComposite_DateD.add(band)
            l_ind = l_banddefinitons.get_band_id_in_l2_coarse(band)
            l_MTAOTbandsDateD.append(str(l_ind))
            LOGGER.debug("Band Index for 'l_MTAOTbandsDateD[" + band + "] : " + str(l_ind))

    l_global_params_dict["MTAOTbandsDateD"] = l_MTAOTbandsDateD

    LOGGER.info("The parameters used to select the bands used for the 'reflectances' composite image are:")
    LOGGER.info("     From 'Bands_Definition/Thematic_Definition' node: ")
    LOGGER.info("     -  Blue_Band_Code        =  " + l_BlueBandCode_DateD)
    LOGGER.info("     -  Red_Band_Code         =  " + l_RedBandCode_DateD)
    LOGGER.info("     -  NIR_Band_Code         =  " + l_NIRBandCode_DateD)
    LOGGER.info("     From 'Cloud_Masking' node: ")
    LOGGER.info("     -  Correl_Band_Code      =  " + l_CorrelBandCode_DateD)
    LOGGER.info("     -  Shadow_Band_Code      =  " + l_ShadowBandCode_DateD)
    LOGGER.info("     From 'Rain_Flag' node: ")
    LOGGER.info("     -  Water_Band_Code       =  " + l_WaterBandCode_DateD)
    LOGGER.info("     From 'AOT_Estimation' node: ")
    LOGGER.info("     -  Dark_Band_Code        =  " + l_DarkBandCode_DateD)
    if l_aotmethod == AOTEstimation.MULTITEMPORAL or l_aotmethod == AOTEstimation.SPECTROTEMPORAL:
        LOGGER.info("     From 'AOT_Estimation/Multi_Temporal_Method' node: ")
        LOGGER.info("     -  Var_Band_Code      =  " + l_VarBandCode_DateD)
        for band in l_AOTUsedBandCodesDateD:
            LOGGER.info("     -  MT_AOT_Band_Code  = " + band)
        LOGGER.info("The parameters used to select the bands used for the 'LTC' composite image are:")
        LOGGER.info("     From 'AOT_Estimation/Multi_Temporal_Method' node: ")
        for band in l_AOTUsedBandCodesDateD:
            LOGGER.info("     -  MT_AOT_Band_Code  = " + band)

    # ---------------------------------------------------------------------------------------------
    # Computes the list of the "BandCode" of the  current product(sorted) - Date D
    l_ListOfBandCodeInL2CoarseSortedByIndex_DateD = l_banddefinitons.get_list_of_band_code_in_l2_coarse_sorted_by_index()
    l_NbListOfBandCodeInL2CoarseSortedByIndexDateD = len(l_ListOfBandCodeInL2CoarseSortedByIndex_DateD)
    for band in l_ListOfBandCodeInL2CoarseSortedByIndex_DateD:
        LOGGER.debug("ListOfBandCodeInL2CoarseSortedByIndex_DateD :" + band)

    # ---------------------------------------------------------------------------------------------
    # Get the list of the  "BandCode" reduc with the L2COMM useful band code for algorithms
    # This list is use to select  the band in the 'CompositeImage'   filter(MultiChannelExtract)
    # - Reflectance
    l_ListOfBandCodeForReflectancesComposite_DateD = []
    l_ListOfBandIndexForReflectancesComposite_DateD = []
    # ---------------------------------------------------------------------------------------------
    # Deduces the list of theoretical wavelength bands
    for band in l_ListOfBandCodeInL2CoarseSortedByIndex_DateD:
        LOGGER.debug("For the input band <" + band + ">, find if it exists in the composite map...")
        if band in l_SetOfBandCodeForReflectancesComposite_DateD:
            LOGGER.debug("Add the band <" + band + ">, to the BandCodeForReflectancesComposite_DateD")
            l_ListOfBandCodeForReflectancesComposite_DateD.append(band)
            # Add the index band in the list of index band
            i = l_banddefinitons.get_band_id_in_l2_coarse(band)
            l_ListOfBandIndexForReflectancesComposite_DateD.append(i)

    for band in l_ListOfBandIndexForReflectancesComposite_DateD:
        LOGGER.debug("ListOfBandIndexForReflectancesComposite_DateD : " + str(band))
    l_global_params_dict["ListOfBandIndexForReflectancesComposite_DateD"] = l_ListOfBandIndexForReflectancesComposite_DateD

    l_DealingLTC = True
    if l_aotmethod == AOTEstimation.MULTISPECTRAL:
        # Disable write LTC in the output product because only AOT multi - temporal use LTC
        LOGGER.warning(
            "Disable write LTC in the output product because the AOT method is multi-spectral (only AOT multi-temporal use LTC)")
        l_DealingLTC = False
    l_global_params_dict["DealingLTC"] = l_DealingLTC

    # ---------------------------------------------------------------------------------------------
    # - LTC
    l_ListOfBandCodeForLTCComposite_DateD = []
    l_ListOfBandIndexForLTCComposite_DateD = []
    # ---------------------------------------------------------------------------------------------
    # Deduces the list  of theoretical wavelength bands
    for band in l_ListOfBandCodeInL2CoarseSortedByIndex_DateD:
        if band in l_SetOfBandCodeForLTCComposite_DateD:
            l_ListOfBandCodeForLTCComposite_DateD.append(band)
            # Add the index band in the list of index band
            l_ListOfBandIndexForLTCComposite_DateD.append(l_banddefinitons.get_band_id_in_l2_coarse(band))

    for band in l_ListOfBandCodeForLTCComposite_DateD:
        LOGGER.debug("ListOfBandCodeForLTCComposite_DateD : " + str(band))
    for band in l_ListOfBandIndexForLTCComposite_DateD:
        LOGGER.debug("ListOfBandIndexForLTCComposite_DateD : " + str(band))
    l_global_params_dict["ListOfBandIndexForLTCComposite_DateD"] = l_ListOfBandIndexForLTCComposite_DateD

    # ---------------------------------------------------------------------------------------------
    # Get the number of composite bands
    l_NbListOfBandCodeForReflectancesComposite_DateD = len(l_ListOfBandCodeForReflectancesComposite_DateD)
    # Deduces the list of band theoretical wavelength
    l_ListOfBandTheoreticalWavelengthForReflectancesComposite_DateD = list()
    # Deduces the list of theoretical wavelength bands
    for band in l_ListOfBandCodeForReflectancesComposite_DateD:
        l_ListOfBandTheoreticalWavelengthForReflectancesComposite_DateD.append(
            l_currentconfigadmincamera.get_theoretical_wavelength_from_band_code_id(band))

    for band in l_ListOfBandTheoreticalWavelengthForReflectancesComposite_DateD:
        LOGGER.debug("ListOfBandTheoreticalWavelengthForReflectancesComposite_DateD : " + band)
    l_NbListOfBandTheoreticalWavelengthForReflectancesComposite_DateD = len(
        l_ListOfBandTheoreticalWavelengthForReflectancesComposite_DateD)
    if l_NbListOfBandTheoreticalWavelengthForReflectancesComposite_DateD != \
            l_NbListOfBandCodeForReflectancesComposite_DateD:
        raise MajaDataException(
            "Internal error while compare the size of list <l_NbListOfBandTheoreticalWavelength_DateD> (" +
            str(l_NbListOfBandTheoreticalWavelengthForReflectancesComposite_DateD) +
            ") and <l_NbListOfBandCodeForComposite_DateD> (" +
            str(l_NbListOfBandCodeForReflectancesComposite_DateD) +
            ")!!")
    # Store in the member class the list of BandTheoreticalWavelengthForReflectancesComposite
    l_global_params_dict["ListOfBandTheoreticalWavelengthForReflectancesComposite"] = \
        l_ListOfBandTheoreticalWavelengthForReflectancesComposite_DateD
    # ---------------------------------------------------------------------------------------------
    # Computes the 'Blue' Index in Composite Index the Date D
    l_CorrelTheoreticalWavelength_DateD = l_currentconfigadmincamera.get_theoretical_wavelength_from_band_code_id(
        l_CorrelBandCode_DateD)
    LOGGER.debug("Correl Band TheoreticalWavelength: " + l_CorrelTheoreticalWavelength_DateD)
    l_global_params_dict["CorrelTheoreticalWavelength_DateD"] = l_CorrelTheoreticalWavelength_DateD
    l_CorrelBandIndexInReflectancesComposite_DateD = l_currentconfigadmincamera.get_band_index_from_list_datedm1_corresponding_with_band_code_date_d(
        l_CorrelBandCode_DateD, l_ListOfBandTheoreticalWavelengthForReflectancesComposite_DateD)
    l_global_params_dict["CorrelBandIndexInReflectancesComposite_DateD"] = l_CorrelBandIndexInReflectancesComposite_DateD
    LOGGER.debug("Band Index for 'l_CorrelBandIndexInReflectancesComposite_DateD': " +
                 str(l_CorrelBandIndexInReflectancesComposite_DateD))
    l_DarkBandIndexInReflectancesComposite_DateD = l_currentconfigadmincamera.get_band_index_from_list_datedm1_corresponding_with_band_code_date_d(
        l_DarkBandCode_DateD, l_ListOfBandTheoreticalWavelengthForReflectancesComposite_DateD)
    l_global_params_dict[
        "DarkBandIndexInReflectancesComposite_DateD"] = l_DarkBandIndexInReflectancesComposite_DateD
    LOGGER.debug("Band Index for 'l_DarkBandIndexInReflectancesComposite_DateD': " +
                 str(l_DarkBandIndexInReflectancesComposite_DateD))
    # ---------------------------------------------------------------------------------------------
    # Get the number of composite  bands
    l_NbListOfBandCodeForLTCComposite_DateD = len(l_ListOfBandCodeForLTCComposite_DateD)
    # Deduces the list of     band     theoretical     wavelength
    l_ListOfBandTheoreticalWavelengthForLTCComposite_DateD = []
    # Deduces the list of theoretical     wavelength     bands
    for band in l_ListOfBandCodeForLTCComposite_DateD:
        l_ListOfBandTheoreticalWavelengthForLTCComposite_DateD.append(
            l_currentconfigadmincamera.get_theoretical_wavelength_from_band_code_id(band))
    for band in l_ListOfBandTheoreticalWavelengthForLTCComposite_DateD:
        LOGGER.debug("ListOfBandTheoreticalWavelengthForLTCComposite_DateD : " + band)
    l_NbListOfBandTheoreticalWavelengthForLTCComposite_DateD = len(
        l_ListOfBandTheoreticalWavelengthForLTCComposite_DateD)
    if (l_NbListOfBandTheoreticalWavelengthForLTCComposite_DateD != l_NbListOfBandCodeForLTCComposite_DateD):
        raise MajaDataException(
            "Internal error while compare the size of list <l_NbListOfBandTheoreticalWavelength_DateD> (" +
            str(l_NbListOfBandTheoreticalWavelengthForLTCComposite_DateD) +
            ") and <l_NbListOfBandCodeForComposite_DateD> (" +
            str(l_NbListOfBandCodeForLTCComposite_DateD) +
            ")!!")

    # Store in the member class the list of BandTheoreticalWavelengthForLTCComposite
    l_global_params_dict["ListOfBandTheoreticalWavelengthForLTCComposite"] = l_ListOfBandTheoreticalWavelengthForLTCComposite_DateD
    # ---------------------------------------------------------------------------------------------
    # Print to the log
    LOGGER.info(
        "The bands selected for the 'reflectances' composite image of the current product (date D) are (nb=" +
        str(l_NbListOfBandTheoreticalWavelengthForReflectancesComposite_DateD) + "):")
    for i in range(0, l_NbListOfBandTheoreticalWavelengthForReflectancesComposite_DateD):
        LOGGER.info("     band [" +
                    str(i) +
                    "] : BandCode = '" +
                    l_ListOfBandCodeForReflectancesComposite_DateD[i] +
                    "' and theoretical wavelength = '" +
                    str(l_ListOfBandTheoreticalWavelengthForReflectancesComposite_DateD[i]) +
                    "' (Index in L2Coarse = '" +
                    str(l_banddefinitons.get_band_id_in_l2_coarse(l_ListOfBandCodeForReflectancesComposite_DateD[i])) +
                    "')")

    # ---------------------------------------------------------------------------------------------
    # Print to the log
    LOGGER.info(
        "The bands selected for the 'LTC' composite image of the current product (date D) are (nb=" +
        str(l_NbListOfBandTheoreticalWavelengthForLTCComposite_DateD) + "):")
    for i in range(0, l_NbListOfBandTheoreticalWavelengthForLTCComposite_DateD):
        LOGGER.info("     band [" + str(i) + "] : BandCode = '" + l_ListOfBandCodeForLTCComposite_DateD[
            i] + "' and theoretical wavelength = '" +
            str(l_ListOfBandTheoreticalWavelengthForLTCComposite_DateD[i]) + "' (Index in L2Coarse = '" +
            str(l_ListOfBandIndexForLTCComposite_DateD[i]) + "')")

    # ---------------------------------------------------------------------------------------------
    # Initialize the Band index of the Date D - 1
    l_BlueBandIndex_DateDm1 = 9999
    l_RedBandIndex_DateDm1 = 9999
    l_NIRBandIndex_DateDm1 = 9999
    l_CorrelBandIndex_DateDm1 = 9999
    l_ShadowBandIndex_DateDm1 = 9999
    l_WaterBandIndex_DateDm1 = 9999
    l_DarkBandIndex_DateDm1 = 9999
    l_VarBandIndex_DateDm1 = 9999
    l_MTAOTbandsOfTheReflectancesComposite_DateDm1 = []
    l_MTAOTbandsOfTheLTCComposite_DateDm1 = []

    if not p_initmode:
        # ---------------------------------------------------------------------------------------------
        # Get the correspondence of the Band Code of the D product(L1) and the D - 1 product(L2) to read the composite bands(RTA, RTC, RCR and LTC)
        # 1.For the Band code D, get the Theoretical wavelength
        #  2. With the theoretical wavelength, get the band index for L2Coarse of the date D-1
        # ---------------------------------------------------------------------------------------------
        # Get the list of the Band (TheoreticalWavelength) in the L2 input 'reflectance' composite image
        l_ListOfBandTheoreticalWavelengthOfTheReflectancesComposite_DateDm1 = \
            p_inputl2imagefilereader.ListOfBandTheoreticalWavelengthOfTheReflectancesComposite
        for band in l_ListOfBandTheoreticalWavelengthOfTheReflectancesComposite_DateDm1:
            LOGGER.debug("l_ListOfBandTheoreticalWavelengthOfTheReflectancesComposite_DateDm1 : " + str(band))
        # ---------------------------------------------------------------------------------------------
        # Print to the log
        l_NbListOfBandTheoreticalWavelengthOfTheReflectancesComposite_DateDm1 = len(
            l_ListOfBandTheoreticalWavelengthOfTheReflectancesComposite_DateDm1)
        LOGGER.info(
            "The bands selected for the 'reflectances' composite image of the product (date D-1) [filename " +
            p_inputl2imagefilereader.GlobalProductFilename +
            "] are (nb=" +
            str(l_NbListOfBandTheoreticalWavelengthOfTheReflectancesComposite_DateDm1) +
            "):")
        for i in range(0, l_NbListOfBandTheoreticalWavelengthOfTheReflectancesComposite_DateDm1):
            LOGGER.info("     band [" + str(i) + "] : theoretical wavelength = '" +
                        str(l_ListOfBandTheoreticalWavelengthOfTheReflectancesComposite_DateDm1[i]) + "'")

        # ---------------------------------------------------------------------------------------------
        # Check the list of l_ListOfBandTheoreticalWavelength_DateD and
        # l_ListOfBandTheoreticalWavelengthOfTheReflectancesComposite_DateDm1
        # Must be identical
        if (l_NbListOfBandTheoreticalWavelengthForReflectancesComposite_DateD !=
                l_NbListOfBandTheoreticalWavelengthOfTheReflectancesComposite_DateDm1):
            raise MajaDriverException(
                "According to the input parameters (from GIPL2COMM), the list of bands included in the composite of the L2 input "
                "product (date D-1) are not compatible : the sizes are differents(Date D=" +
                str(l_NbListOfBandTheoreticalWavelengthForReflectancesComposite_DateD) +
                " and Date D-1=" +
                str(l_NbListOfBandTheoreticalWavelengthOfTheReflectancesComposite_DateDm1) +
                ")!")
        # ---------------------------------------------------------------------------------------------
        # Check elements
        for i in range(0, l_NbListOfBandTheoreticalWavelengthForReflectancesComposite_DateD):
            if int(l_ListOfBandTheoreticalWavelengthForReflectancesComposite_DateD[i]) != int(
                    l_ListOfBandTheoreticalWavelengthOfTheReflectancesComposite_DateDm1[i]):
                raise MajaDriverException("For the composite for the band index [" +
                                          str(i) +
                                          "], the theoretical wavelength of the Date D '" +
                                          str(l_ListOfBandTheoreticalWavelengthForReflectancesComposite_DateD[i]) +
                                          "' is not identical with the theoretical wavelength of the Date D-1 '" +
                                          str(l_ListOfBandTheoreticalWavelengthOfTheReflectancesComposite_DateDm1[i]) +
                                          "'")
        # ---------------------------------------------------------------------------------------------
        # Computes the 'Blue' Index in the Date Dm1
        l_BlueBandIndex_DateDm1 = l_currentconfigadmincamera.get_band_index_from_list_datedm1_corresponding_with_band_code_date_d(
            l_BlueBandCode_DateD, l_ListOfBandTheoreticalWavelengthOfTheReflectancesComposite_DateDm1)
        # Computes the 'Red' Index in the Date Dm1
        l_RedBandIndex_DateDm1 = l_currentconfigadmincamera.get_band_index_from_list_datedm1_corresponding_with_band_code_date_d(
            l_RedBandCode_DateD, l_ListOfBandTheoreticalWavelengthOfTheReflectancesComposite_DateDm1)
        # Computes the 'NIR' Index in the Date Dm1
        l_NIRBandIndex_DateDm1 = l_currentconfigadmincamera.get_band_index_from_list_datedm1_corresponding_with_band_code_date_d(
            l_NIRBandCode_DateD, l_ListOfBandTheoreticalWavelengthOfTheReflectancesComposite_DateDm1)
        # Computes the 'Correl' Index in the Date Dm1
        l_CorrelBandIndex_DateDm1 = l_currentconfigadmincamera.get_band_index_from_list_datedm1_corresponding_with_band_code_date_d(
            l_CorrelBandCode_DateD, l_ListOfBandTheoreticalWavelengthOfTheReflectancesComposite_DateDm1)
        LOGGER.debug("l_CorrelBandIndex_DateDm1 : " + str(l_CorrelBandIndex_DateDm1))
        LOGGER.debug("Check the STO band of the L2 input product. Its must be the Correl band  ?")
        # Check with the STO TheoreticalWavelength (must be the CorrelBand
        if p_inputl2imagefilereader.STOBandTheoreticalWavelengthIsPresent:
            l_STOBandTheoreticalWavelengthValue_DateDm1 = p_inputl2imagefilereader.STOBandTheoreticalWavelengthValue
            LOGGER.debug(
                "STO Date D-1 Info is present: STOBandTheoreticalWavelengthValue = " +
                str(l_STOBandTheoreticalWavelengthValue_DateDm1) +
                ". Check compatibility with current Date value : " +
                str(l_CorrelTheoreticalWavelength_DateD))
            if not (int(l_CorrelTheoreticalWavelength_DateD) == int(l_STOBandTheoreticalWavelengthValue_DateDm1)):
                raise MajaDataException(
                    "In the L2 input product, the theoretical wavelenght band of the STO product '" +
                    str(l_STOBandTheoreticalWavelengthValue_DateDm1) +
                    "' is different than the Correl band of the current product '" +
                    str(l_CorrelTheoreticalWavelength_DateD) +
                    " !")
        # ---------------------------------------------------------------------------------------------
        # Computes the 'Shadow' Index in the Date Dm1
        l_ShadowBandIndex_DateDm1 = l_currentconfigadmincamera.get_band_index_from_list_datedm1_corresponding_with_band_code_date_d(
            l_ShadowBandCode_DateD, l_ListOfBandTheoreticalWavelengthOfTheReflectancesComposite_DateDm1)
        # Computes the 'Water' Index in the Date Dm1
        l_WaterBandIndex_DateDm1 = l_currentconfigadmincamera.get_band_index_from_list_datedm1_corresponding_with_band_code_date_d(
            l_WaterBandCode_DateD, l_ListOfBandTheoreticalWavelengthOfTheReflectancesComposite_DateDm1)
        # Computes the 'Dark' Index in the Date Dm1
        l_DarkBandIndex_DateDm1 = l_currentconfigadmincamera.get_band_index_from_list_datedm1_corresponding_with_band_code_date_d(
            l_DarkBandCode_DateD, l_ListOfBandTheoreticalWavelengthOfTheReflectancesComposite_DateDm1)
        if l_aotmethod == AOTEstimation.MULTITEMPORAL or l_aotmethod == AOTEstimation.SPECTROTEMPORAL:
            # Get the list of the Band(TheoreticalWavelength) in the  L2 input 'reflectance' composite image
            if not p_inputl2imagefilereader.L2PrivateImageFilenamesProvider.get_dealing_ltc():
                raise MajaDriverException(
                    "The AOT method is multi-spectral (multi-temporal or spectro-temporal), the L2 input product <" +
                    p_inputl2imagefilereader.GlobalProductFilename +
                    "> should have the LTC data in the PRIVATE directory!")
            l_ListOfBandTheoreticalWavelengthOfTheLTCComposite_DateDm1 = p_inputl2imagefilereader.ListOfBandTheoreticalWavelengthOfTheLTCComposite
            l_NbListOfBandTheoreticalWavelengthOfTheLTCComposite_DateDm1 = len(
                l_ListOfBandTheoreticalWavelengthOfTheLTCComposite_DateDm1)
            LOGGER.info(
                "The bands selected for the 'LTC' composite image of the product (date D-1) are (nb=" +
                str(l_NbListOfBandTheoreticalWavelengthOfTheLTCComposite_DateDm1) + "):")
            for i in range(0, l_NbListOfBandTheoreticalWavelengthOfTheLTCComposite_DateDm1):
                LOGGER.info("     band [" + str(i) + "] : theoretical wavelength = '" +
                            str(l_ListOfBandTheoreticalWavelengthOfTheLTCComposite_DateDm1[i]) + "'")
            # Computes the 'Var' Index in the Date Dm1
            l_VarBandIndex_DateDm1 = l_currentconfigadmincamera.get_band_index_from_list_datedm1_corresponding_with_band_code_date_d(
                l_VarBandCode_DateD, l_ListOfBandTheoreticalWavelengthOfTheReflectancesComposite_DateDm1)
            # Computes the 'MTAOT' Index in the Date Dm1
            l_AOTUsedBandsDateDListSize = len(l_AOTUsedBandCodesDateD)
            # Create the list of channel indexes
            for i in range(0, l_AOTUsedBandsDateDListSize):
                # Use each code (B02) to get the channel index
                l_AOTUsedBandCodeDateD = l_AOTUsedBandCodesDateD[i]
                # Add the band code in the Date D - 1 for the 'reflectance' composite
                l_MTAOTbandsOfTheReflectancesComposite_DateDm1.append(
                    l_currentconfigadmincamera.get_band_index_from_list_datedm1_corresponding_with_band_code_date_d(
                        l_AOTUsedBandCodeDateD, l_ListOfBandTheoreticalWavelengthOfTheReflectancesComposite_DateDm1))
                # ---------------------------------------------------------------------------------------------
                # Add the band code in the Date D-1 for the 'LTC' composite
                l_MTAOTbandsOfTheLTCComposite_DateDm1.append(
                    l_currentconfigadmincamera.get_band_index_from_list_datedm1_corresponding_with_band_code_date_d(
                        l_AOTUsedBandCodeDateD, l_ListOfBandTheoreticalWavelengthOfTheLTCComposite_DateDm1))
            # ---------------------------------------------------------------------------------------------
            # Check size
            if len(l_MTAOTbandsDateD) != len(l_MTAOTbandsOfTheReflectancesComposite_DateDm1):
                raise MajaDataException("Internal error: problem of number of AOT bands values in the date D " + str(
                    len(l_MTAOTbandsDateD)) + " and date D-1 " + str(len(l_MTAOTbandsOfTheReflectancesComposite_DateDm1)))
        # Fin si  l_AOTMethod = MULTITEMPORAL ou SPECTROTEMPORAL
    # Fi si p_mode.IsInit() = Faux
    l_global_params_dict["BlueBandIndex_DateDm1"] = l_BlueBandIndex_DateDm1
    l_global_params_dict["RedBandIndex_DateDm1"] = l_RedBandIndex_DateDm1
    l_global_params_dict["NIRBandIndex_DateDm1"] = l_NIRBandIndex_DateDm1
    l_global_params_dict["CorrelBandIndex_DateDm1"] = l_CorrelBandIndex_DateDm1
    l_global_params_dict["ShadowBandIndex_DateDm1"] = l_ShadowBandIndex_DateDm1
    l_global_params_dict["WaterBandIndex_DateDm1"] = l_WaterBandIndex_DateDm1
    l_global_params_dict["DarkBandIndex_DateDm1"] = l_DarkBandIndex_DateDm1
    l_global_params_dict["VarBandIndex_DateDm1"] = l_VarBandIndex_DateDm1
    l_global_params_dict["MTAOTbandsOfTheReflectancesComposite_DateDm1"] = l_MTAOTbandsOfTheReflectancesComposite_DateDm1
    l_global_params_dict["MTAOTbandsOfTheLTCComposite_DateDm1"] = l_MTAOTbandsOfTheLTCComposite_DateDm1
