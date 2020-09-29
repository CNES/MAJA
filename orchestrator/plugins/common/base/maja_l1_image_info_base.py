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
from orchestrator.common.maja_exceptions import MajaNotYetImplemented
from orchestrator.common.maja_common import Corner
import orchestrator.common.date_utils as date_utils
import re


class L1ImageInformationsBase(object):

    def __init__(self):
        self.ProductFileName = ""
        self.Site = ""
        self.FileClass = ""
        self.FileCategory = ""
        self.Satellite = ""
        self.SatelliteID = ""
        self.UniqueSatellite = ""
        self.Instrument = ""
        self.SpectralContent = ""
        self.PluginName = ""
        self.Prefix = ""
        self.LevelType = ""
        self.ProductDate = None
        self.ProductId = ""
        self.ProductVersion = "1.0"
        self.ProductDateStr = ""
        self.AcquisitionStart = ""
        self.GenerationDateStr = ""
        self.OrbitNumber = ""
        self.ProjectionRef = ""
        self.UTCValidityStart = ""
        self.UTCValidityStop = ""
        self.ReferenceSiteDefinitionId = ""
        self.HeaderFilename = ""
        self.SOLImageFileName = ""
        self.SOLHeaderFileName = ""
        self.VIEImageFileName = ""
        self.VIEHeaderFileName = ""
        self.L1NoData = -10000
        self.ReflectanceQuantification = 0.
        self.RealL1NoData = 0.
        self.ProjectionRef = ""
        self.ListOfViewingAnglesPerBandAtL2CoarseResolution = []
        self.ListOfViewingZenithAnglesPerBandAtL2CoarseResolution = []
        self.ListOfViewingAzimuthAnglesPerBandAtL2CoarseResolution = []
        self.ListOfViewingAnglesPerBandAtL2Resolution = []
        self.ViewingAngle = {}
        self.SolarAngle = {}
        self.SolarAngleGrid = {"Azimuth":[], "Zenith":[], "ColStep":"", "RowStep":""}
        self.ViewingAngleGrids = []
        self.CenterCorner = Corner()
        self.SpectralInfo = []
        self.AreaByResolution = []
        self.MuscateData = {}

    def initialize(self, product_filename):
        raise MajaNotYetImplemented("Initialize : Could not instanciate base class")

    def detect_l1_products(self, dir_name, product_list):
        raise MajaNotYetImplemented("DetectL1Products : Could not instanciate base class")

    def get_l2_identifier(self):
        l_identifier = self.SatelliteID
        if len(self.Instrument):
            l_identifier += '-' + self.Instrument
        if len(self.SpectralContent):
            specContent = self.SpectralContent.replace("XSTHPAN", "XS").replace("XSTH", "XS")
            l_identifier += '-' + specContent
        l_identifier += \
            "_"  + date_utils.get_date_yyyymmdd_from_tm(self.ProductDate) + \
            "-" + date_utils.get_date_hhmmss_from_tm(self.ProductDate) + \
            "-" + date_utils.get_date_millisecs_from_tm(self.ProductDate)
        l_identifier += "_L2A"
        l_Site = self.Site.strip('_')
        if re.search('^[0-9]{2}[A-Z]{3}$', l_Site) is not None:
            l_Site = 'T'+l_Site
        l_identifier += "_" + l_Site + "_C"
        return l_identifier

    def get_l2_product_id(self):
        return self.get_l2_identifier() + "_V" + self.ProductVersion.replace(".","-")
