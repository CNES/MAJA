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

orchestrator.plugins.maja_dataset -- shortdesc

orchestrator.plugins.maja_dataset is a description

It defines classes_and_methods

###################################################################################################
"""

from orchestrator.plugins.common.base.bands_definition import BandsDefinitions
from orchestrator.common.conf.maja_camera_admin_config import ConfigAdminCamera
from orchestrator.common.constants import *
from orchestrator.common.maja_exceptions import MajaDataException
import orchestrator.common.conf.maja_xml_camera_user_config as camera_user_conf


class PluginBase(object):

    def __init__(self):
        self.WebSiteURL = ""
        self.ReferenceDate = "20000101"
        self.Creator_Version = "4.0.0"
        self.WideFieldSensor = False
        self.GRIDReferenceAltitudesSOL2GridAvailable = False
        self.WaterVapourDetermination = False
        self.CirrusFlag = False
        self.CirrusMasking = False
        self.SnowMasking = False
        self.HasSwirBand = False
        self.ShortFileType = ""
        self.CirrusCorrectionReferenceResolution = "R1"
        self.DirectionalCorrection = False
        self.CloudMaskingKnownCloudsAltitude = False
        self.DFPMasking = False
        self.UniqueSatellite = ""
        self.PluginName = ""
        self.ListOfSatellites = []
        self.ListOfL1Resolutions = []
        self.ListOfL2Resolutions = []
        self.BandsDefinitions = BandsDefinitions()
        self.CLDDataBandsSelected = []
        self.NumbersOfBitsInCLDDataIOBand = 0
        self.L2RejectedBands = []
        self.QCK_PREFIX_MAP = {}
        self.ConfigUserCamera = None
        self.ConfigAdminCamera = None

        # -----------------------------------------------------------------------------
        # MACCS VENUS File_Version
        self.L2PRODUCT_FILE_VERSION = "NOT INITIALIZED"
        self.L3PRODUCT_FILE_VERSION = "NOT INITIALIZED"

        self.CLDCoreAlgorithmsMapBand = {}
        self.CLDCoreAlgorithmsMapBand[CLOUD_MASK_ALL] = 0
        self.CLDCoreAlgorithmsMapBand[CLOUD_MASK_ALL_CLOUDS] = 1
        self.CLDCoreAlgorithmsMapBand[CLOUD_MASK_SHADOWS] = 2
        self.CLDCoreAlgorithmsMapBand[CLOUD_MASK_SHADVAR] = 3
        self.CLDCoreAlgorithmsMapBand[CLOUD_MASK_REFL] = 4
        self.CLDCoreAlgorithmsMapBand[CLOUD_MASK_REFL_VAR] = 5
        self.CLDCoreAlgorithmsMapBand[CLOUD_MASK_EXTENSION] = 6
        self.CLDCoreAlgorithmsMapBand[CLOUD_MASK_ALT] = 7
        self.CLDCoreAlgorithmsMapBand[CLOUD_MASK_CIRRUS] = 8

        # Schemas directories
        self.MAJA_INSTALL_SCHEMAS_DIR = ""
        self.MAJA_INSTALL_SCHEMAS_L2_DIR = ""
        self.MAJA_INSTALL_SCHEMAS_L3_DIR = ""

        # Private Templates for L2
        self.TEMPLATE_PDTANX_PRIVATE_HDR = ""
        self.TEMPLATE_PDTANX_PRIVATE_LTC_HDR = ""
        self.TEMPLATE_PDTANX_PRIVATE_NDT_HDR = ""
        self.TEMPLATE_PDTANX_PRIVATE_WAM_HDR = ""
        self.TEMPLATE_PDTANX_PRIVATE_CLD_HDR = ""
        self.TEMPLATE_PDTANX_PRIVATE_CLA_HDR = ""
        self.TEMPLATE_PDTANX_PRIVATE_PWD_HDR = ""
        self.TEMPLATE_PDTANX_PRIVATE_RCR_HDR = ""
        self.TEMPLATE_PDTANX_PRIVATE_RTA_HDR = ""
        self.TEMPLATE_PDTANX_PRIVATE_RTC_HDR = ""
        self.TEMPLATE_PDTANX_PRIVATE_STO_HDR = ""
        self.TEMPLATE_PDTANX_PRIVATE_PXD_HDR = ""

        self.TEMPLATE_TEC_PRIVATE_EEF = ""

        # Public Templates for L2
        self.TEMPLATE_PDTIMG_HDR = ""
        self.TEMPLATE_PDTANX_ATB_HDR = ""
        self.TEMPLATE_PDTANX_CLD_HDR = ""
        self.TEMPLATE_PDTANX_MSK_HDR = ""
        self.TEMPLATE_PDTANX_QLT_HDR = ""
        self.TEMPLATE_PDTQLK_HDR = ""
        self.TEMPLATE_PDTQKL_JPG = ""

        # GLOBAL
        self.TEMPLATE_GLOBAL_HDR = ""

        # Public Templates for L3
        self.TEMPLATE_L3_PDTIMG_HDR = ""
        self.TEMPLATE_L3_PDTANX_MSK_HDR = ""
        self.TEMPLATE_L3_PDTANX_QLT_HDR = ""
        self.TEMPLATE_L3_PDTQLK_HDR = ""
        # GLOBAL for L3
        self.TEMPLATE_L3_GLOBAL_HDR = ""

    def initialize(self, app_handler):
        self.ConfigUserCamera = camera_user_conf.parse(app_handler.get_user_conf_camera_filename(self.PluginName), True)
        self.ConfigAdminCamera = ConfigAdminCamera(app_handler.get_admin_conf_camera_filename(self.PluginName))

    def is_valid(self, plugin_name):
        return self.PluginName == plugin_name

    def get_cld_algoindex_from_bandname(self, band_name):
        if band_name in self.CLDCoreAlgorithmsMapBand:
            return self.CLDCoreAlgorithmsMapBand[band_name]

    def is_valid_with_satellite(self, p_sat):
        return p_sat in self.ListOfSatellites

    def is_valid_with_l2_satellite(self, p_sat):
        return self.is_valid_with_satellite(p_sat)

    def is_valid_with_unique_satellite(self, p_sat):
        return p_sat == self.UniqueSatellite
