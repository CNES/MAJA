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

                        o     o
                        oo   oo   oo        o   oo        ,-.
                        o o o o  o  o       o  o  o       \_/
                        o  o  o o    o      o o    o     {|||)<
                        o     o oooooo      o oooooo      / \
                        o     o o    o o    o o    o      `-^
                        o     o o    o  oooo  o    o

###################################################################################################

orchestrator.plugins.sentinel2.maja_sentinel2_l1_plugin -- shortdesc

orchestrator.plugins.sentinel2.maja_sentinel2_l1_plugin is a description

It defines classes_and_methods

###################################################################################################
"""

from orchestrator.plugins.common.base.maja_plugin_base import PluginBase
from orchestrator.plugins.common.base.bands_definition import SingleResolutionImageInfo
from orchestrator.common.constants import *


class MajaVenusMuscatePlugin(PluginBase):
    """
    classdocs
    """

    def __init__(self):
        super(MajaVenusMuscatePlugin, self).__init__()
        self.PluginName = "VENUS_MUSCATE"
        self.UniqueSatellite = "VENUS"
        self.ShortFileType = "VSC"
        self.ListOfSatellites.append("VENUS")
        self.QCK_PREFIX_MAP["VENUS"] = "VE"
        # Resolution/bands fill
        self.ListOfL1Resolutions.append("R1")
        self.ListOfL2Resolutions.append("R1")
        self.L1BandCharIdList = []
        # -----------------------------------------------------------------
        # Sorted list of band char id used in the  tile PDI_ID
        self.L1BandCharIdList.append("B1")
        self.L1BandCharIdList.append("B2")
        self.L1BandCharIdList.append("B3")
        self.L1BandCharIdList.append("B4")
        self.L1BandCharIdList.append("B5")
        self.L1BandCharIdList.append("B6")
        self.L1BandCharIdList.append("B7")
        self.L1BandCharIdList.append("B8")
        self.L1BandCharIdList.append("B9")
        self.L1BandCharIdList.append("B10")
        self.L1BandCharIdList.append("B11")
        self.L1BandCharIdList.append("B12")
        # General camera configuration
        self.WebSiteURL = ""
        self.ReferenceDate = "20000101"
        self.Creator_Version = "04.00.00"
        self.WideFieldSensor = False
        self.GRIDReferenceAltitudesSOL2GridAvailable = True
        self.WaterVapourDetermination = True
        self.CirrusFlag = True
        self.CirrusMasking = False
        self.SnowMasking = True
        self.HasSwirBand = False
        self.CirrusCorrectionReferenceResolution = "R1"
        self.DirectionalCorrection = False
        self.CloudMaskingKnownCloudsAltitude = True
        self.DFPMasking = False

        # Index of CLD
        # --------------------------------------------------------------------------------------------------------------------
        # FORMAT DISK MUSCATE
        #                    Bit 1 - Cloud_Mask.all : summary Logical or of All cloud and shadow masks
        #                    Bit 2 - Cloud_Mask.all clouds: Logical or of All cloud masks
        #                    Bit 3 - Cloud_Mask.refl : reflectance threshold
        #                    Bit 4 - Cloud_Mask.refl_var : reflectance variation threshold
        #                    Bit 5 - Cloud_Mask.extension : extension of the cloud mask
        #                    Bit 6 - Cloud_Mask.shadows : shadows mask from clouds within image
        #                    Bit 7 - Cloud_Mask.shadvar: shadows mask from clouds outside image bit 1 : Cloud_Mask.all
        #                    Bit 8 - Cloud_Mask.alt : stereoscopic mask  =>  VENUS et vide pour les autres
        #                    Bit 8 - Cirrus => L8 et S2
        #                    Bit 8 - Vide pour L5/L7, spot et F2
        # --------------------------------------------------------------------------------------------------------------------
        # 8 bands in the CLD product ( 7 base + (Cirrus/ALT))
        # Index from the GetCLDImage output of the CloudMasking algorithms
        self.CLDDataBandsSelected = []
        self.CLDDataBandsSelected.append(CLOUD_MASK_ALL)
        self.CLDDataBandsSelected.append(CLOUD_MASK_ALL_CLOUDS)
        self.CLDDataBandsSelected.append(CLOUD_MASK_REFL)
        self.CLDDataBandsSelected.append(CLOUD_MASK_REFL_VAR)
        self.CLDDataBandsSelected.append(CLOUD_MASK_EXTENSION)
        self.CLDDataBandsSelected.append(CLOUD_MASK_SHADOWS)
        self.CLDDataBandsSelected.append(CLOUD_MASK_SHADVAR)
        self.CLDDataBandsSelected.append(CLOUD_MASK_ALT)

        # Fill BandsDefinition
        # ---------------------------------------------------------------------------------------------
        # Init the list of L2 resolution
        self.BandsDefinitions.ListOfL2Resolution.append("R1")
        # ---------------------------------------------------------------------------------------------
        # Init the list of L1 resolution
        self.BandsDefinitions.ListOfL1Resolution.append("R1")
        # ---------------------------------------------------------------------------------------------
        # List of indices for the L1 bands
        self.BandsDefinitions.L2CoarseBandMap["B1"] = 0
        self.BandsDefinitions.L2CoarseBandMap["B2"] = 1
        self.BandsDefinitions.L2CoarseBandMap["B3"] = 2
        self.BandsDefinitions.L2CoarseBandMap["B4"] = 3
        self.BandsDefinitions.L2CoarseBandMap["B5"] = 4
        self.BandsDefinitions.L2CoarseBandMap["B6"] = 5
        self.BandsDefinitions.L2CoarseBandMap["B7"] = 6
        self.BandsDefinitions.L2CoarseBandMap["B8"] = 7
        self.BandsDefinitions.L2CoarseBandMap["B9"] = 8
        self.BandsDefinitions.L2CoarseBandMap["B10"] = 9
        self.BandsDefinitions.L2CoarseBandMap["B11"] = 10
        self.BandsDefinitions.L2CoarseBandMap["B12"] = 11

        # Init the list of L2 coarse resolution bands
        # For VENUS, the list of L2 coarse bands is equal to the list of L1 bands

        # ---------------------------------------------------------------------------------------------
        # ---------------------------------------------------------------------------------------------
        # Definition of the 3 resolutions
        l_R1ImageInfo = SingleResolutionImageInfo()
        l_R1ImageInfo.Resolution = 5

        # ---------------------------------------------------------------------------------------------
        l_R1ImageInfo.ListOfBandCode.append("B1")
        l_R1ImageInfo.ListOfBandCode.append("B2")
        l_R1ImageInfo.ListOfBandCode.append("B3")
        l_R1ImageInfo.ListOfBandCode.append("B4")
        l_R1ImageInfo.ListOfBandCode.append("B5")
        l_R1ImageInfo.ListOfBandCode.append("B6")
        l_R1ImageInfo.ListOfBandCode.append("B7")
        l_R1ImageInfo.ListOfBandCode.append("B8")
        l_R1ImageInfo.ListOfBandCode.append("B9")
        l_R1ImageInfo.ListOfBandCode.append("B10")
        l_R1ImageInfo.ListOfBandCode.append("B11")
        l_R1ImageInfo.ListOfBandCode.append("B12")
        self.BandsDefinitions.ImagesInformationMap[self.BandsDefinitions.ListOfL1Resolution[0]] = l_R1ImageInfo

        # ---------------------------------------------------------------------------------------------
        # L1
        # As for the list of indices, the list of L1 bands by resolution is equal to the list of L2 bands
        self.BandsDefinitions.L1BandMap = self.BandsDefinitions.L2CoarseBandMap
        # As for the list of indices, the list of L1 bands by resolution is equal to the list of L2 bands
        self.BandsDefinitions.L1ListOfBandsMap[self.BandsDefinitions.ListOfL1Resolution[0]] = \
            self.BandsDefinitions.ImagesInformationMap[self.BandsDefinitions.ListOfL1Resolution[0]].ListOfBandCode

        # detector mapping (specific for Venus)
        self.BandsDefinitions.DetectorMap = [1,1,4,4,1,4,3,3,3,2,2,2]

        self.MAJA_INSTALL_SCHEMAS_DIR = "VENUS_MUSCATE"

        # Private Templates
        self.TEMPLATE_PDTANX_PRIVATE_HDR = "VENUS_MUSCATE/Templates/L2.product/JPI_ALL.xml"

        self.TEMPLATE_PDTANX_PRIVATE_LTC_HDR = "VENUS_MUSCATE/Templates/L2.product/PDTANX_PRIVATE_LTC.HDR"
        self.TEMPLATE_PDTANX_PRIVATE_NDT_HDR = "VENUS_MUSCATE/Templates/L2.product/PDTANX_PRIVATE_NDT.HDR"
        self.TEMPLATE_PDTANX_PRIVATE_WAM_HDR = "VENUS_MUSCATE/Templates/L2.product/PDTANX_PRIVATE_WAM.HDR"
        self.TEMPLATE_PDTANX_PRIVATE_CLD_HDR = "VENUS_MUSCATE/Templates/L2.product/PDTANX_PRIVATE_CLD.HDR"
        self.TEMPLATE_PDTANX_PRIVATE_CLA_HDR = "VENUS_MUSCATE/Templates/L2.product/PDTANX_PRIVATE_CLA.HDR"
        self.TEMPLATE_PDTANX_PRIVATE_PWD_HDR = "VENUS_MUSCATE/Templates/L2.product/PDTANX_PRIVATE_PWD.HDR"
        self.TEMPLATE_PDTANX_PRIVATE_RCR_HDR = "VENUS_MUSCATE/Templates/L2.product/PDTANX_PRIVATE_RCR.HDR"
        self.TEMPLATE_PDTANX_PRIVATE_RTA_HDR = "VENUS_MUSCATE/Templates/L2.product/PDTANX_PRIVATE_RTA.HDR"
        self.TEMPLATE_PDTANX_PRIVATE_RTC_HDR = "VENUS_MUSCATE/Templates/L2.product/PDTANX_PRIVATE_RTC.HDR"
        self.TEMPLATE_PDTANX_PRIVATE_STO_HDR = "VENUS_MUSCATE/Templates/L2.product/PDTANX_PRIVATE_STO.HDR"
        self.TEMPLATE_PDTANX_PRIVATE_PXD_HDR = "VENUS_MUSCATE/Templates/L2.product/PDTANX_PRIVATE_PXD.HDR"

        self.TEMPLATE_TEC_PRIVATE_EEF = "VENUS_MUSCATE/Templates/L2.product/TEC_PRIVATE.EEF"

        # Public Templates
        self.TEMPLATE_PDTQKL_JPG = "VENUS_MUSCATE/Templates/L2.product/DEFAULT_QKL_ALL.jpg"

        # GLOBAL
        self.TEMPLATE_GLOBAL_HDR = "VENUS_MUSCATE/Templates/L2.product/MTD_ALL.xml"
        self.MAJA_INSTALL_SCHEMAS_L2_DIR = "VENUS_MUSCATE/PSC-SL-411-0032-CG_Schemas"

        # -----------------------------------------------------------------------------
        # MACCS VENUS File_Version
        self.L2PRODUCT_FILE_VERSION = "0003"
        self.L3PRODUCT_FILE_VERSION = "0002"
        # Quicklook product (QCK)
        self.L1QCK_FILE_VERSION = "0002"
        self.L2QCK_FILE_VERSION = "0002"
        self.L3QCK_FILE_VERSION = "0002"
