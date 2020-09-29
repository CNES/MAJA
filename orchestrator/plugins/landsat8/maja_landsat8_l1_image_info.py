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
from orchestrator.plugins.common.base.maja_l1_image_info_base import L1ImageInformationsBase
from orchestrator.plugins.landsat8.maja_landsat8_plugin import MajaLandsat8Plugin
from orchestrator.common.logger.maja_logging import configure_logger
LOGGER = configure_logger(__name__)
import os
import orchestrator.common.file_utils as file_utils
from orchestrator.plugins.landsat8.maja_landsat8_ascii_file_handler import Landsat8ASCIIFileHandler
import orchestrator.common.date_utils as date_utils


class MajaLandsat8L1ImageInformations(L1ImageInformationsBase):
    def __init__(self):
        super(MajaLandsat8L1ImageInformations, self).__init__()
        self._plugin = MajaLandsat8Plugin()
        self.UniqueSatellite = self._plugin.UniqueSatellite
        self.HeaderHandler = None

    def detect_l1_products(self, input_directory, l_ListOfL1Products):
        LOGGER.info("Start Landsat8 L1 DetectL1Products in " + input_directory)

        l_ListOfSubDirectories = file_utils.maja_get_subdirectories(input_directory)
        # Note: in the list, there are only File, the directories have been excluded of the list
        l_NbFiles = len(l_ListOfSubDirectories)
        LOGGER.debug(
            "Landsat8L1InformationsProvider::DetectL1Products: nb sub directories: " + str(l_NbFiles))

        for dir_name in l_ListOfSubDirectories:
            LOGGER.debug("Parse the directory: " + dir_name)
            # Detect the L1C file
            l_ShortDirectoryFilename = os.path.basename(dir_name)
            LOGGER.debug("l_ShortDirectoryFilename: " + l_ShortDirectoryFilename)
            if "LC8" or "LC08" in l_ShortDirectoryFilename:
                l_ListOfFilenames = [os.path.join(dir_name, f) for f in os.listdir(dir_name)]
                l_NbSubFiles = len(l_ListOfFilenames)
                LOGGER.debug("Nb files in the sub directory: " + str(l_NbSubFiles))
                for filename in l_ListOfFilenames:
                    if MajaLandsat8L1ImageInformations.is_a_landsat8_l1product_by_checking_the_filename(filename):
                        if MajaLandsat8L1ImageInformations.is_a_landsat8_l1product_by_checking_the_satellite(filename, self._plugin):
                            LOGGER.debug("Add the Landsat8 L1 product "+filename+" in the list of products!")
                            l_ListOfL1Products.append(filename)


    def initialize(self, product_filename):
        LOGGER.info("Start Landsat8 L1 Initialize on product " + product_filename)

        if not MajaLandsat8L1ImageInformations.is_a_landsat8_l1product_by_checking_the_filename(product_filename):
            LOGGER.debug("The filename <" + product_filename + "> is not an Landsat8 L1 header file.")
            return False
        if not MajaLandsat8L1ImageInformations.is_a_landsat8_l1product_by_checking_the_satellite(product_filename, self._plugin):
            LOGGER.debug("The filename <" + product_filename + "> is not an Landsat8 L1 header file (by reading platform in the txt file).")
            return False

        lASCIIHandler = Landsat8ASCIIFileHandler(product_filename)

        self.Satellite = lASCIIHandler.get_platform()
        self.SatelliteID = self.Satellite.upper().replace("_","")
        self.PluginName = self._plugin.PluginName
        self.FileCategory = self._plugin.ShortFileType  # LSC
        self.LevelType = "L1VALD"
        self.Prefix = self._plugin.QCK_PREFIX_MAP[self.Satellite]
        self.FileClass = "TEST"
        self.Site = lASCIIHandler.get_nick_name()

        l_DatePDV = lASCIIHandler.get_date_pdv()
        self.ProductDate = date_utils.get_datetime_from_utc(l_DatePDV)
        self.ProductDateStr = self.ProductDate.strftime('%Y%m%d')
        LOGGER.debug("Product Date: " + self.ProductDateStr)
        self.ReferenceSiteDefinitionId = "UNKNOWN"
        self.ProductId = lASCIIHandler.get_product_id()
        LOGGER.debug("ProductID: "+self.ProductId)

        self.HeaderFilename = product_filename
        self.HeaderHandler = lASCIIHandler

        self.GenerationDateStr = lASCIIHandler.get_generation_date()
        self.AcquisitionStart = l_DatePDV[4:]

        self.OrbitNumber = lASCIIHandler.get_orbit_number()

        self.Instrument = lASCIIHandler.get_instrument()

        self.SpectralContent = "XSTHPAN"

        # VENUS specification
        # Store the VIE and SOL filenames (DATA and Headers) to copy in the L2 product
        self.SOLHeaderFileName = ""
        self.SOLImageFileName = ""
        self.VIEHeaderFileName = ""
        self.VIEImageFileName = ""

        #         Initialize the parameters necessary for the core of the algorithms of MACCS
        # Get longitude and latitude coordinates of the product
        ulc = lASCIIHandler.get_upper_left_corner()
        lrc = lASCIIHandler.get_lower_right_corner()
        # Estimation of the coordinate of the central point
        self.CenterCorner.longitude = ulc[0] + (lrc[0] - ulc[0]) / 2
        self.CenterCorner.latitude = ulc[1] + (lrc[1] - ulc[1]) / 2
        self.CenterCorner.column = 0
        self.CenterCorner.line = 0

        # Initialize the Validity Start/Stop
        self.UTCValidityStart = l_DatePDV  # Already formated in UTC
        self.UTCValidityStop = l_DatePDV  # Already formated in UTC

        # Get the list of bands in a L2 product
        l_BandsDefinitions = self._plugin.BandsDefinitions

        l_ListOfBandsL2Coarse = l_BandsDefinitions.get_list_of_band_id_in_l2_coarse()
        l_NbBandsL2Coarse = len(l_ListOfBandsL2Coarse)

        LOGGER.debug("l_BandsDefinitions->GetListOfBandCodeInL2Coarse -> l_NbBandsL2Coarse: " + str(l_NbBandsL2Coarse))
        LOGGER.debug("MuscateXmllHandler->GetListOfBands      -> l_NbBands                : " +
                     str(l_NbBandsL2Coarse))

        self.ListOfViewingAnglesPerBandAtL2Resolution = []
        self.ListOfViewingAnglesPerBandAtL2CoarseResolution = []
        self.ListOfViewingZenithAnglesPerBandAtL2CoarseResolution = []
        self.ListOfViewingAzimuthAnglesPerBandAtL2CoarseResolution = []

        lViewingAngles = lASCIIHandler.get_mean_view_angles()
        l_ViewAngleZenith = lViewingAngles[0]
        l_ViewAngleAzimuth = lViewingAngles[1]
        self.ViewingAngle = {
            "incidence_zenith_angle": str(l_ViewAngleZenith),
            "incidence_azimuth_angle": str(l_ViewAngleAzimuth)
            }

        # For each bands for EnviProduct
        for bd in l_ListOfBandsL2Coarse:
            angles = {"incidence_zenith_angle": str(l_ViewAngleZenith),
                      "incidence_azimuth_angle": str(l_ViewAngleAzimuth)
                      }
            self.ListOfViewingAnglesPerBandAtL2CoarseResolution.append(angles)
            self.ListOfViewingZenithAnglesPerBandAtL2CoarseResolution.append(str(angles["incidence_zenith_angle"]))
            self.ListOfViewingAzimuthAnglesPerBandAtL2CoarseResolution.append(
                str(angles["incidence_azimuth_angle"]))
        # Fill the L2 resolution angles
        l_nbRes = len(l_BandsDefinitions.ListOfL2Resolution)

        for r in range(0, l_nbRes):
            l_res = l_BandsDefinitions.ListOfL2Resolution[r]
            l_l2bandcodes = l_BandsDefinitions.get_list_of_l2_band_code(l_res)
            l_l2bandidx = [l_BandsDefinitions.get_band_id_in_l2_coarse(b) for b in l_l2bandcodes]
            for b in l_l2bandidx:
                self.ListOfViewingAnglesPerBandAtL2Resolution.append(
                    self.ListOfViewingAnglesPerBandAtL2CoarseResolution[b])

        # Solar Angles
        self.SolarAngle = {
            "sun_zenith_angle": lASCIIHandler.get_sol_zenith(),
            "sun_azimuth_angle": lASCIIHandler.get_sol_azitmuth()
        }

        # Spectral information
        self.SpectralInfo = []
        l_pathLuminanceMax = "RADIANCE_MAXIMUM_BAND_{}"
        l_pathLuminanceMin = "RADIANCE_MINIMUM_BAND_{}"
        l_pathQuantizeCalMax = "QUANTIZE_CAL_MAX_BAND_{}"
        l_pathQuantizeCalMin = "QUANTIZE_CAL_MIN_BAND_{}"
        l_pathRadianceAdd = "RADIANCE_ADD_BAND_{}"
        l_pathRadianceMult = "RADIANCE_MULT_BAND_{}"
        l_pathReflectanceAdd = "REFLECTANCE_ADD_BAND_{}"
        l_pathReflectanceMult = "REFLECTANCE_MULT_BAND_{}"
        for b, bidx in l_BandsDefinitions.L1BandMap.items():
            self.SpectralInfo.append({
                "Band":b,
                "LuminanceMax": lASCIIHandler._get_value(l_pathLuminanceMax.format(bidx+1)),
                "LuminanceMin": lASCIIHandler._get_value(l_pathLuminanceMin.format(bidx+1)),
                "QuantizeCalMax": lASCIIHandler._get_value(l_pathQuantizeCalMax.format(bidx+1)),
                "QuantizeCalMin": lASCIIHandler._get_value(l_pathQuantizeCalMin.format(bidx+1)),
                "RadianceAdd" : lASCIIHandler._get_value(l_pathRadianceAdd.format(bidx+1)),
                "RadianceMult" : lASCIIHandler._get_value(l_pathRadianceMult.format(bidx+1)),
                "ReflectanceAdd" : lASCIIHandler._get_value(l_pathReflectanceAdd.format(bidx+1)),
                "ReflectanceMult" : lASCIIHandler._get_value(l_pathReflectanceMult.format(bidx+1))
                })

        # Set the real value of the L1 NoData
        self.RealL1NoData = -10

        #Set the Reflectance
        self.ReflectanceQuantification = 0.02

        # # Get the projection ref
        # self.ProjectionRef = lASCIIHandler.get_toa_image_filename_from_band_code("B1")
        return True


    def is_a_landsat8_l1product_by_checking_the_filename(p_filename):
        #p_filename: ex: < ... > _MTL.txt
        l_ShortFilename = os.path.basename(p_filename)
        l_ShortDirectory = os.path.basename(os.path.dirname(p_filename))
        #Filename evolution
        #Before Landsat Collection 1: LC8...
        #After Landsat Collection 1: LC08...
        if "LC8" or "LC08" in l_ShortDirectory:
            if "_MTL" in l_ShortFilename and os.path.splitext(l_ShortFilename)[1] == ".txt":
                LOGGER.debug("Check filename OK: " + l_ShortFilename)
                return True
        return False


    def is_a_landsat8_l1product_by_checking_the_satellite(p_filename, plugin_base):
        lASCIIHandler = Landsat8ASCIIFileHandler(p_filename)
        lplatform = lASCIIHandler.get_platform()
        if plugin_base.is_valid_with_satellite(lplatform):
            LOGGER.debug(
                "The filename " +
                p_filename +
                " (platform=" +
                lplatform +
                ") is a Landsat8 L1 product compatible with the plugin '" +
                plugin_base.PluginName +
                "'!"
                )
            return True
        return False



