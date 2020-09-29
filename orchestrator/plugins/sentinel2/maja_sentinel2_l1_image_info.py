#
# Copyright (C) 2020 Centre National d'Etudes Spatiales (CNES)
#
#Licensed under the Apache License, Version 2.0 (the "License");
#you may not use this file except in compliance with the License.
#You may obtain a copy of the License at
#
#    http://www.apache.org/licenses/LICENSE-2.0
#
#Unless required by applicable law or agreed to in writing, software
#distributed under the License is distributed on an "AS IS" BASIS,
#WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
#See the License for the specific language governing permissions and
#limitations under the License.
#
# -*- coding: utf-8 -*-
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
from orchestrator.common.file_utils import maja_get_subdirectories
import orchestrator.common.xml_tools as xml_tools
from orchestrator.plugins.sentinel2.maja_sentinel2_l1_main_xml_reader import MajaSentinel2L1MainXmlReader
from orchestrator.plugins.sentinel2.maja_sentinel2_plugin import MajaSentinel2Plugin
from orchestrator.plugins.sentinel2.maja_sentinel2_l1_granule_xml_reader import MajaSentinel2L1GranuleXmlReader
from orchestrator.plugins.common.base.maja_l1_image_info_base import L1ImageInformationsBase
from orchestrator.common.logger.maja_logging import configure_logger
from orchestrator.cots.otb.otb_app_handler import OtbAppHandler
from orchestrator.common.maja_exceptions import MajaDataException
from orchestrator.common.maja_common import Area
import orchestrator.common.date_utils as date_utils
import orchestrator.common.maja_utils as maja_utils
import os,statistics
LOGGER = configure_logger(__name__)


class MajaSentinel2L1ImageInformations(L1ImageInformationsBase):

    def __init__(self):
        super(MajaSentinel2L1ImageInformations, self).__init__()
        self.HeaderHandler = None
        self.TileHandler = None
        self.xmlTileFilename = ""
        self.IS_TILE_DEPENDENT =True

    def detect_l1_products(self, input_directory, product_list, tile_id=None):
        LOGGER.info("Start Sentinel2 L1 DetectL1Products in " + input_directory)

        """ LAIG - FA - MAC - 1482 - CNES
         Example of the header filename of the S2 L1C product PSD < 14
         -> S2A_OPER_PRD_MSIL1C_PDMC_20150507T132817_R065_V20091211T165913_20091211T165959.SAFE / S2A_OPER_MTD_SAFL1C_PDMC_20150507T132817_R065_V20091211T165913_20091211T165959.xml
         Example of the header filename of the S2 L1C product PSD >= 14
          -> L1C_T36JTT_A006424_20160914T081456 / MTD_TL.xml
        Get the list of subdirectories of the input product"""

        listOfSubDirectories = maja_get_subdirectories(input_directory)

        # For each directory
        for dir in listOfSubDirectories:
            # Get this name: S2A_OPER_PRD_MSIL1C_PDMC_20150507T132817_R065_V20091211T165913_20091211T165959.SAFE
            l_ShortDirectoryFilename = os.path.basename(dir)
            LOGGER.info("Short dirfilename : " + l_ShortDirectoryFilename)
            # PSD < 14: Detect S2A or S2B, MSIL1C and the extension SAFE
            # PSD >= 14: Detect L1C_T
            if "_MSIL1C_" in l_ShortDirectoryFilename and (
                    "S2A_" in l_ShortDirectoryFilename or "S2B_"in l_ShortDirectoryFilename) and ".SAFE" in l_ShortDirectoryFilename:
                # Get the list of files in the current subdirectory
                listOfFilenames = [
                    os.path.join(
                        dir,
                        f) for f in os.listdir(dir) if os.path.isfile(
                        os.path.join(
                            dir,
                            f))]
                for fi in listOfFilenames:
                    l_ShortFilename = os.path.basename(fi)
                    LOGGER.info("Short filename : " + l_ShortFilename)
                    # Detect the file type of the xml header file only for user product SAFE formatted
                    # PSD < 14: _MTD_SAFL1C_ -- > Not supported
                    # PSD >= 14: MTD_TL
                    if (("MTD_MSIL1C" in l_ShortFilename) or ("_MTD_SAFL1C" in l_ShortFilename)) \
                            and os.path.splitext(l_ShortFilename)[1] == ".xml":
                        # Load the xml header file and get the satellite
                        l_CanLoad = xml_tools.can_load_file(fi)
                        if l_CanLoad:
                            try:
                                s2xml = MajaSentinel2L1MainXmlReader(fi,tile_id=tile_id)
                                if s2xml.satellite_name == "Sentinel-2A" or s2xml.satellite_name == "Sentinel-2B":
                                    product_list.append(fi)
                                    LOGGER.info("Append  " + fi)
                            except Exception as e:
                                LOGGER.debug("Not compatible with SENTINEL2")
                                LOGGER.debug(e)

    def initialize(self, product_filename, validate=False, schema_path=None, tile_id=None):
        self.ProductFileName = product_filename
        self.PluginName = "SENTINEL2"
        LOGGER.info("Start Sentinel2 L1 ImageInformationProvider " + product_filename + "...")
        # detect correctly formatted
        if (("_MTD_SAFL1C_" not in product_filename and "MTD_MSIL1C" not in product_filename) or
                os.path.splitext(product_filename)[1] != ".xml"):
            return False

        try:
            headerHandler = MajaSentinel2L1MainXmlReader(product_filename, validate, schema_path,tile_id=tile_id)
        except Exception as e:
            LOGGER.info(e)
            return False
        self.HeaderHandler = headerHandler
        self.Satellite = headerHandler.satellite_name.upper()
        self.SatelliteID = self.Satellite.upper().replace("-","")
        self.UniqueSatellite = "SENTINEL-2_"
        self.LevelType = "L1VALD"
        self.FileCategory = "SSC"

        granule_id_split = headerHandler.granule_id.split("_")
        self.FileClass = granule_id_split[1]
        self.HeaderFilename = headerHandler.main_xml_file
        self.Prefix = granule_id_split[0]
        self.L1NoData = headerHandler.get_no_data()
        self.ReflectanceQuantification = 1.0 / headerHandler.quantification_value
        self.RealL1NoData = self.L1NoData * self.ReflectanceQuantification
        self.xmlTileFilename = headerHandler.XmlTileFileName
        LOGGER.info("Tile xml filename : " + self.xmlTileFilename)
        tile_handler = MajaSentinel2L1GranuleXmlReader(self.xmlTileFilename)
        self.TileHandler = tile_handler
        LOGGER.debug("TileId: " + self.HeaderHandler.TileId)
        self.Site = maja_utils.get_formated_site(self.HeaderHandler.TileId)
        self.TileHandler.TileId = self.Site

        self.ProductDate = date_utils.get_datetime_from_utc("UTC=" + tile_handler.sensing_time)
        self.ProductDateStr = self.ProductDate.strftime('%Y%m%d')
        LOGGER.debug("Product Date: " + self.ProductDateStr)
        self.ProductId = headerHandler.get_string_value_of("ProductURI")
        LOGGER.debug("ProductID: "+self.ProductId)

        self.GenerationDateStr = headerHandler.get_string_value_of("GenerationTime")
        self.AcquisitionStart = headerHandler.get_string_value_of("ProductStartTime")

        self.OrbitNumber = headerHandler.get_string_value_of("OrbitNumber")

        l_datastrip_split = tile_handler.datastrip_id.split("_")
        l_start_date_str = l_datastrip_split[-3]
        l_stop_date_str = l_datastrip_split[-2][1:]
        l_start_date_time = date_utils.get_datetime_from_yyyymmddthhmmss(l_start_date_str)
        l_stop_date_time = date_utils.get_datetime_from_yyyymmddthhmmss(l_stop_date_str)
        self.UTCValidityStart = date_utils.get_utc_from_datetime(l_start_date_time)
        LOGGER.debug("UTCValidityStart : " + self.UTCValidityStart)
        self.UTCValidityStop = date_utils.get_utc_from_datetime(l_stop_date_time)
        LOGGER.debug("UTCValidityStop : " + self.UTCValidityStop)

        self.ReferenceSiteDefinitionId = "UNKNOWN"

        # Not available in S2
        self.SOLHeaderFileName = ""
        self.SOLImageFileName = ""
        self.VIEHeaderFileName = ""
        self.VIEImageFileName = ""
        self.ListOfViewingAnglesPerBandAtL2Resolution = []
        self.ListOfViewingAnglesPerBandAtL2CoarseResolution = []
        self.ListOfViewingZenithAnglesPerBandAtL2CoarseResolution = []
        self.ListOfViewingAzimuthAnglesPerBandAtL2CoarseResolution = []
        l_MeanViewingZenithalAngles = []
        l_MeanViewingAzimuthalAngles = []
        for f in range(0, len(tile_handler.angles.viewing_incidence_angle.incidence_angles_mean)):
            angles = tile_handler.angles.viewing_incidence_angle.incidence_angles_mean[str(f)]
            self.ListOfViewingAnglesPerBandAtL2CoarseResolution.append(angles)
            self.ListOfViewingZenithAnglesPerBandAtL2CoarseResolution.append(angles["incidence_zenith_angle"])
            self.ListOfViewingAzimuthAnglesPerBandAtL2CoarseResolution.append(angles["incidence_azimuth_angle"])
            l_MeanViewingZenithalAngles.append(float(angles["incidence_zenith_angle"]))
            l_MeanViewingAzimuthalAngles.append(float(angles["incidence_azimuth_angle"]))
        self.ViewingAngle = {
            "incidence_zenith_angle": str(statistics.mean(l_MeanViewingZenithalAngles)),
            "incidence_azimuth_angle": str(statistics.mean(l_MeanViewingAzimuthalAngles))
            }
        l_bandsdefinition = MajaSentinel2Plugin().BandsDefinitions
        l_nbRes = len(l_bandsdefinition.ListOfL2Resolution)
        for r in range(0, l_nbRes):
            l_res = l_bandsdefinition.ListOfL2Resolution[r]
            l_l2bandcodes = l_bandsdefinition.get_list_of_l2_band_code(l_res)
            l_l2bandidx = [l_bandsdefinition.get_band_id_in_l2_coarse(b) for b in l_l2bandcodes]
            for b in l_l2bandidx:
                self.ListOfViewingAnglesPerBandAtL2Resolution.append(
                    self.ListOfViewingAnglesPerBandAtL2CoarseResolution[b])

        self.ViewingAngleGrids = []
        for grid in tile_handler.angles.viewing_incidence_angle.list_of_viewing_angles_grid:
            self.ViewingAngleGrids.append({
                "StepUnit":grid.step_unit,
                "ColStep":grid.col_step,
                "RowStep":grid.row_step,
                "Band":grid.band_id,
                "Detector":grid.detector_id,
                "Azimuth":grid.azimuth_values,
                "Zenith":grid.zenith_values
                })
        self.SolarAngle = tile_handler.angles.sun_angles.sun_angles_mean

        self.SolarAngleGrid["StepUnit"] = tile_handler.angles.sun_angles.step_unit
        self.SolarAngleGrid["ColStep"] = tile_handler.angles.sun_angles.col_step
        self.SolarAngleGrid["RowStep"] = tile_handler.angles.sun_angles.row_step
        self.SolarAngleGrid["Azimuth"] = tile_handler.angles.sun_angles.azimuth_angle_list_string_values
        self.SolarAngleGrid["Zenith"] = tile_handler.angles.sun_angles.zenith_angle_list_string_values

        # Area by resolution
        self.AreaByResolution = []
        for res in l_bandsdefinition.ListOfL2Resolution:
            res_m = l_bandsdefinition.get_l1_resolution(res)
            l_ul = tile_handler.get_geoposition_upperleftcorner(res_m)
            l_spa = tile_handler.get_geoposition_dims(res_m)
            l_size = tile_handler.get_size(res_m)
            curArea = Area()
            curArea.origin = (
                str(l_ul.x),
                str(l_ul.y))
            curArea.spacing = (
                str(l_spa.x),
                str(l_spa.y))
            curArea.size = (
                str(l_size.x),
                str(l_size.y))
            self.AreaByResolution.append(curArea)
        
        # Spectral information
        self.SpectralInfo = []
        l_pathGain = "//Product_Image_Characteristics/PHYSICAL_GAINS[@bandId='{}']"
        l_pathRadiance = "//Solar_Irradiance_List/SOLAR_IRRADIANCE[@bandId='{}']"
        l_pathWavelength = "//Spectral_Information_List/Spectral_Information[@bandId='{}']/Wavelength/{}"
        l_pathResponse = "//Spectral_Information_List/Spectral_Information[@bandId='{}']/Spectral_Response/{}"
        for b, bidx in l_bandsdefinition.L1BandMap.items():
            self.SpectralInfo.append({
                "Band":b,
                "PhysicalGain":float(headerHandler.get_string_value(l_pathGain.format(bidx))),
                "SolarIrradiance":headerHandler.get_string_value(l_pathRadiance.format(bidx)),
                "WavelengthMin":headerHandler.get_string_value(l_pathWavelength.format(bidx, "MIN")),
                "WavelengthMax":headerHandler.get_string_value(l_pathWavelength.format(bidx, "MAX")),
                "WavelengthCentral":headerHandler.get_string_value(l_pathWavelength.format(bidx, "CENTRAL")),
                "ResponseStep":headerHandler.get_string_value(l_pathResponse.format(bidx, "STEP")),
                "ResponseValues":headerHandler.get_string_value(l_pathResponse.format(bidx, "VALUES"))
                })

        # TODO CenterCorner
        l_resolution = 10
        l_boundingbox = tile_handler.get_geoposition_boundingbox(l_resolution)
        LOGGER.info("Geoposition BoundingBox computed: xmin, ymin, xmax, ymax: " + str(l_boundingbox.xmin) + ", "
                    + str(l_boundingbox.ymin) + ", " + str(l_boundingbox.xmax) + ", " + str(l_boundingbox.ymax) + ".")

        # Transform in Lat / Long coordinates
        # -------------------------------------------------------------------------
        # CSCode = EPSG:32615
        cs_code = tile_handler.reference_system_code
        cs_code = maja_utils.split_string(cs_code, ':')[-1]

        # Estimation of the coordinate of the central point
        l_inputpoint_0 = l_boundingbox.xmin + (l_boundingbox.xmax - l_boundingbox.xmin) / 2.0
        l_inputpoint_1 = l_boundingbox.ymin + (l_boundingbox.ymax - l_boundingbox.ymin) / 2.0
        LOGGER.debug(
            "Geoposition BoundingBox central point computed: x, y: " + str(l_inputpoint_0) + ", " + str(l_inputpoint_1))

        LOGGER.debug("Start Conversion ToWKT with the EPSG '" + cs_code + "'...")
        param_conv = {"carto.x": l_inputpoint_0,
                      "carto.y": l_inputpoint_1,
                      "mapproj": "epsg",
                      "mapproj.epsg.code": int(cs_code)
                      }
        conv_app = OtbAppHandler("ConvertCartoToGeoPoint", param_conv)
        LOGGER.debug("Start Conversion ToWKT done.")

        self.CenterCorner.longitude = conv_app.getoutput()["long"]
        self.CenterCorner.latitude = conv_app.getoutput()["lat"]
        self.CenterCorner.column = 0
        self.CenterCorner.line = 0
        conv_app = None

        return True
