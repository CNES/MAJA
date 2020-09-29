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


class MajaSentinel2Plugin(PluginBase):
    """
    classdocs
    """

    def __init__(self):
        super(MajaSentinel2Plugin, self).__init__()
        self.PluginName = "SENTINEL2"
        self.UniqueSatellite = "SENTINEL-2_"
        # -----------------------------------------------------------------------------
        # MACCS VENUS File_Version
        self.L2PRODUCT_FILE_VERSION = "0003"
        self.L3PRODUCT_FILE_VERSION = "-"
        self.ListOfSatellites.append("SENTINEL-2A")
        self.ListOfSatellites.append("SENTINEL-2B")
        self.ListOfL1Resolutions.append("R1")
        self.ListOfL1Resolutions.append("R2")
        self.ListOfL1Resolutions.append("R3")
        self.ListOfL2Resolutions.append("R1")
        self.ListOfL2Resolutions.append("R2")
        self.L1BandCharIdList = []
        # -----------------------------------------------------------------
        # Sorted list of band char id used in the  tile PDI_ID
        self.L1BandCharIdList.append("B01")
        self.L1BandCharIdList.append("B02")
        self.L1BandCharIdList.append("B03")
        self.L1BandCharIdList.append("B04")
        self.L1BandCharIdList.append("B05")
        self.L1BandCharIdList.append("B06")
        self.L1BandCharIdList.append("B07")
        self.L1BandCharIdList.append("B08")
        self.L1BandCharIdList.append("B8A")
        self.L1BandCharIdList.append("B09")
        self.L1BandCharIdList.append("B10")
        self.L1BandCharIdList.append("B11")
        self.L1BandCharIdList.append("B12")
        # General camera configuration
        self.WebSiteURL = ""
        self.ReferenceDate = "20000101"
        self.Creator_Version = "04.00.00"
        self.WideFieldSensor = True
        self.GRIDReferenceAltitudesSOL2GridAvailable = False
        self.WaterVapourDetermination = True
        self.CirrusFlag = False
        self.CirrusMasking = True
        self.SnowMasking = True
        self.HasSwirBand = True
        self.CirrusCorrectionReferenceResolution = "R2"
        self.DirectionalCorrection = True
        self.CloudMaskingKnownCloudsAltitude = False
        self.DFPMasking = False

        # Index of CLD
        self.CLDDataBandsSelected = []
        self.CLDDataBandsSelected.append(CLOUD_MASK_ALL)
        self.CLDDataBandsSelected.append(CLOUD_MASK_ALL_CLOUDS)
        self.CLDDataBandsSelected.append(CLOUD_MASK_SHADOWS)
        self.CLDDataBandsSelected.append(CLOUD_MASK_SHADVAR)
        self.CLDDataBandsSelected.append(CLOUD_MASK_REFL)
        self.CLDDataBandsSelected.append(CLOUD_MASK_REFL_VAR)
        self.CLDDataBandsSelected.append(CLOUD_MASK_EXTENSION)
        self.CLDDataBandsSelected.append(CLOUD_MASK_CIRRUS)
        self.NumbersOfBitsInCLDDataIOBand = len(self.CLDDataBandsSelected)

        # Fill BandsDefinition
        # ---------------------------------------------------------------------------------------------
        # Init the list of L2 resolution
        self.BandsDefinitions.ListOfL2Resolution.append("R1")
        self.BandsDefinitions.ListOfL2Resolution.append("R2")
        # ---------------------------------------------------------------------------------------------
        # Init the list of L1 resolution
        self.BandsDefinitions.ListOfL1Resolution.append("R1")
        self.BandsDefinitions.ListOfL1Resolution.append("R2")
        self.BandsDefinitions.ListOfL1Resolution.append("R3")

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
        self.BandsDefinitions.L2CoarseBandMap["B8A"] = 8
        self.BandsDefinitions.L2CoarseBandMap["B9"] = 9
        self.BandsDefinitions.L2CoarseBandMap["B10"] = 10
        self.BandsDefinitions.L2CoarseBandMap["B11"] = 11
        self.BandsDefinitions.L2CoarseBandMap["B12"] = 12

        # Init the list of L2 coarse resolution bands
        # For Sentinel2, the list of L2 coarse bands is equal to the list of L1 bands

        # ---------------------------------------------------------------------------------------------
        # ---------------------------------------------------------------------------------------------
        # Definition of the 3 resolutions
        l_R1ImageInfo = SingleResolutionImageInfo()
        l_R1ImageInfo.Resolution = 10

        # ---------------------------------------------------------------------------------------------
        l_R1ImageInfo.ListOfBandCode.append("B2")
        l_R1ImageInfo.ListOfBandCode.append("B3")
        l_R1ImageInfo.ListOfBandCode.append("B4")
        l_R1ImageInfo.ListOfBandCode.append("B8")
        self.BandsDefinitions.ImagesInformationMap[self.BandsDefinitions.ListOfL1Resolution[0]] = l_R1ImageInfo
        # ---------------------------------------------------------------------------------------------
        l_R2ImageInfo = SingleResolutionImageInfo()
        l_R2ImageInfo.Resolution = 20

        # ---------------------------------------------------------------------------------------------
        l_R2ImageInfo.ListOfBandCode.append("B5")
        l_R2ImageInfo.ListOfBandCode.append("B6")
        l_R2ImageInfo.ListOfBandCode.append("B7")
        l_R2ImageInfo.ListOfBandCode.append("B8A")
        l_R2ImageInfo.ListOfBandCode.append("B11")
        l_R2ImageInfo.ListOfBandCode.append("B12")
        self.BandsDefinitions.ImagesInformationMap[self.BandsDefinitions.ListOfL1Resolution[1]] = l_R2ImageInfo
        # ---------------------------------------------------------------------------------------------
        l_R3ImageInfo = SingleResolutionImageInfo()
        l_R3ImageInfo.Resolution = 60

        # ---------------------------------------------------------------------------------------------
        l_R3ImageInfo.ListOfBandCode.append("B1")
        l_R3ImageInfo.ListOfBandCode.append("B9")
        l_R3ImageInfo.ListOfBandCode.append("B10")
        # ---------------------------------------------------------------------------------------------
        self.BandsDefinitions.ImagesInformationMap[self.BandsDefinitions.ListOfL1Resolution[2]] = l_R3ImageInfo

        # ---------------------------------------------------------------------------------------------
        # L1
        # As for the list of indices, the list of L1 bands by resolution is equal to the list of L2 bands
        self.BandsDefinitions.L1BandMap = self.BandsDefinitions.L2CoarseBandMap
        # As for the list of indices, the list of L1 bands by resolution is equal to the list of L2 bands
        self.BandsDefinitions.L1ListOfBandsMap[self.BandsDefinitions.ListOfL1Resolution[0]] = \
            self.BandsDefinitions.ImagesInformationMap[self.BandsDefinitions.ListOfL1Resolution[0]].ListOfBandCode
        self.BandsDefinitions.L1ListOfBandsMap[self.BandsDefinitions.ListOfL1Resolution[1]] = \
            self.BandsDefinitions.ImagesInformationMap[self.BandsDefinitions.ListOfL1Resolution[1]].ListOfBandCode
        self.BandsDefinitions.L1ListOfBandsMap[self.BandsDefinitions.ListOfL1Resolution[2]] = \
            self.BandsDefinitions.ImagesInformationMap[self.BandsDefinitions.ListOfL1Resolution[2]].ListOfBandCode

        self.MAJA_INSTALL_SCHEMAS_DIR = "SENTINEL2"

        # Private Templates
        self.TEMPLATE_PDTANX_PRIVATE_HDR = "SENTINEL2/Templates/L2.product/PDTANX_PRIVATE.HDR"
        self.TEMPLATE_PDTANX_PRIVATE_LTC_HDR = "SENTINEL2/Templates/L2.product/PDTANX_PRIVATE_LTC.HDR"
        self.TEMPLATE_PDTANX_PRIVATE_NDT_HDR = "SENTINEL2/Templates/L2.product/PDTANX_PRIVATE_NDT.HDR"
        self.TEMPLATE_PDTANX_PRIVATE_WAM_HDR = "SENTINEL2/Templates/L2.product/PDTANX_PRIVATE_WAM.HDR"
        self.TEMPLATE_PDTANX_PRIVATE_CLD_HDR = "SENTINEL2/Templates/L2.product/PDTANX_PRIVATE_CLD.HDR"
        self.TEMPLATE_PDTANX_PRIVATE_CLA_HDR = "SENTINEL2/Templates/L2.product/PDTANX_PRIVATE_CLA.HDR"
        self.TEMPLATE_PDTANX_PRIVATE_PWD_HDR = "SENTINEL2/Templates/L2.product/PDTANX_PRIVATE_PWD.HDR"
        self.TEMPLATE_PDTANX_PRIVATE_RCR_HDR = "SENTINEL2/Templates/L2.product/PDTANX_PRIVATE_RCR.HDR"
        self.TEMPLATE_PDTANX_PRIVATE_RTA_HDR = "SENTINEL2/Templates/L2.product/PDTANX_PRIVATE_RTA.HDR"
        self.TEMPLATE_PDTANX_PRIVATE_RTC_HDR = "SENTINEL2/Templates/L2.product/PDTANX_PRIVATE_RTC.HDR"
        self.TEMPLATE_PDTANX_PRIVATE_STO_HDR = "SENTINEL2/Templates/L2.product/PDTANX_PRIVATE_STO.HDR"
        self.TEMPLATE_PDTANX_PRIVATE_PXD_HDR = "SENTINEL2/Templates/L2.product/PDTANX_PRIVATE_PXD.HDR"

        self.TEMPLATE_TEC_PRIVATE_EEF = "SENTINEL2/Templates/L2.product/TEC_PRIVATE.EEF"

        # Public Templates
        self.TEMPLATE_PDTIMG_HDR = "SENTINEL2/Templates/L2.product/PDTIMG.HDR"
        self.TEMPLATE_PDTANX_ATB_HDR = "SENTINEL2/Templates/L2.product/PDTANX_ATB.HDR"
        self.TEMPLATE_PDTANX_CLD_HDR = "SENTINEL2/Templates/L2.product/PDTANX_CLD.HDR"
        self.TEMPLATE_PDTANX_MSK_HDR = "SENTINEL2/Templates/L2.product/PDTANX_MSK.HDR"
        self.TEMPLATE_PDTANX_QLT_HDR = "SENTINEL2/Templates/L2.product/PDTANX_QLT.HDR"
        self.TEMPLATE_PDTQLK_HDR = "SENTINEL2/Templates/L2.product/PDTQLK.HDR"

        # GLOBAL
        self.TEMPLATE_GLOBAL_HDR = "SENTINEL2/Templates/L2.product/GLOBAL.HDR"

        # Only for sentinel 2
        self.MAJA_INSTALL_SCHEMAS_L1PRODUCT_FILE = "SENTINEL2/S2-PDGS-TAS-DI-PSD_Schema/S2_User_Product_Level-1C_Metadata.xsd"
        self.MAJA_INSTALL_SCHEMAS_L1TILE_FILE = "SENTINEL2/S2-PDGS-TAS-DI-PSD_Schema/S2_PDI_Level-1C_Tile_Metadata.xsd"

        # Command line parameters for gdal rasterize.
        self.GDAL_RASTERIZE_GML_MASK_ADDITIONAL_COMMAND_LINE_PARAMETERS = """ -ot Byte -init 0 -a_nodata 0 -burn 1 -l MaskFeature -at """
        self.GDAL_RASTERIZE_GML_DETECTOR_ADDITIONAL_COMMAND_LINE_PARAMETERS = """ -ot Byte -init 0 -a_nodata 0 -at """
        self.GDAL_RASTERIZE_GML_NODATA_ADDITIONAL_COMMAND_LINE_PARAMETERS = """ -ot Byte -init 0 -a_nodata 0 -burn 1 -at -sql "select fid, maskType, * from MaskFeature" -where 'maskType="QT_NODATA_PIXELS"' """

        # -----------------------------------------------------------------------------
        # MACCS VENUS File_Version
        self.L2PRODUCT_FILE_VERSION = "0001"
        self.L3PRODUCT_FILE_VERSION = "-"
        # Quicklook product (QCK)
        self.L1QCK_FILE_VERSION = "0001"
        self.L2QCK_FILE_VERSION = "0001"
        self.L3QCK_FILE_VERSION = "-"
