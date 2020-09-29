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
from orchestrator.common.logger.maja_logging import configure_logger
from orchestrator.common.muscate.muscate_xml_file_handler import MuscateXMLFileHandler
import orchestrator.common.xml_tools as xml_tools
import orchestrator.common.date_utils as date_utils
import orchestrator.common.file_utils as file_utils
from orchestrator.common.maja_common import Area
import os,statistics
LOGGER = configure_logger(__name__)


class MajaMuscateL1ImageInformations(L1ImageInformationsBase):

    def __init__(self):
        super(MajaMuscateL1ImageInformations, self).__init__()
        self.HeaderHandler = None
        self.TileHandler = None
        self.xmlTileFilename = ""

    @staticmethod
    def muscate_detect_l1_products(input_directory, l_ListOfL1Products, p_PluginBase):
        l_ListOfSubDirectories = file_utils.maja_get_subdirectories(input_directory)
        # Note: in the list, there are only File, the directories have been excluded of the list
        l_NbFiles = len(l_ListOfSubDirectories)
        LOGGER.debug("MuscateL1ImageInformationsProvider::MuscateDetectL1Products: nb sub directories: " + str(l_NbFiles))
        for dir_name in l_ListOfSubDirectories:
            LOGGER.debug("Parse the directory: " + dir_name)
            # Detect the L1C file
            l_ShortDirectoryFilename = os.path.basename(dir_name)
            LOGGER.debug("l_ShortDirectoryFilename: " + l_ShortDirectoryFilename)
            if "_L1C_" in l_ShortDirectoryFilename:
                l_ListOfFilenames = [os.path.join(dir_name, f) for f in os.listdir(dir_name)]
                l_NbSubFiles = len(l_ListOfFilenames)
                LOGGER.debug("Nb files in the sub directory: " + str(l_NbSubFiles))
                for filename in l_ListOfFilenames:
                    if MajaMuscateL1ImageInformations.is_a_muscate_by_checking_the_filename(filename):
                        LOGGER.debug("Is a muscate filename : " + filename)
                        if MajaMuscateL1ImageInformations.is_a_muscate_by_checking_the_satellite(
                                filename, p_PluginBase):
                            LOGGER.debug("Is a muscate satelite : " + filename)
                            LOGGER.debug("Add the 'muscate' L1 product " + filename + " in the list of products!")
                            l_ListOfL1Products.append(filename)

    @staticmethod
    def is_a_muscate_by_checking_the_filename(p_filename):
        # p_filename: ex: /../../../LANDSAT5-TM-XSTH_20100118-103000-000_L1C_EU93066200A00B_C_V1-0/LANDSAT5-TM-XSTH_20100118-103000-000_L1C_EU93066200A00B_C_V1-0_MTD_ALL.xml
        # Get the filename without the path
        l_ShortFilename = os.path.basename(p_filename)
        LOGGER.debug(
            "MuscateL1ImageInformationsProvider::IsAMuscateL1ProductByCheckingTheFilename l_ShortFilename: " +
            l_ShortFilename)
        l_ShortDirectory = os.path.basename(os.path.dirname(p_filename))
        LOGGER.debug(
            "MuscateL1ImageInformationsProvider::IsAMuscateL1ProductByCheckingTheFilename l_ShortDirectory: " +
            l_ShortDirectory)
        # Detect the L1C files : LANDSAT5-TM-XSTH_20100118-103000-000_L1C_EU93066200A00B_C_V1-0_MTD_ALL.xml
        if "_L1C_" in l_ShortDirectory:
            # Look for the MTD_ALL keys and the xml extension
            if "_L1C_" in l_ShortFilename and "_MTD_ALL" in l_ShortFilename and os.path.splitext(l_ShortFilename)[
                    1] == ".xml":
                return True
        return False

    @staticmethod
    def is_a_muscate_by_checking_the_satellite(p_filename, plugin_base):
        LOGGER.debug("File detected with the filenaming <" + p_filename +
                     ">. Check with the field 'Platform' in the xml file...")
        lXMLHandler = MuscateXMLFileHandler(p_filename)
        LOGGER.debug(
            "Is the filename " +
            p_filename +
            " a 'muscate' L1 product compatible with the plugin '" +
            plugin_base.PluginName +
            "' ??")
        if plugin_base.is_valid_with_satellite(lXMLHandler.get_string_value_of("Platform")):
            LOGGER.debug(
                "The filename " +
                p_filename +
                " is a 'muscate' L1 product compatible with the plugin '" +
                plugin_base.PluginName +
                "'!")
            return True
        return False

    def muscate_initialize(self, product_filename, plugin_base, validate=False, schema_path=None):
        # Initialize the Image filename provider
        if not MajaMuscateL1ImageInformations.is_a_muscate_by_checking_the_filename(product_filename):
            LOGGER.debug("The filename <" + product_filename + "> is not an 'muscate' L1 header file.")
            return False
        if not MajaMuscateL1ImageInformations.is_a_muscate_by_checking_the_satellite(product_filename, plugin_base):
            LOGGER.debug(
                "The filename <" +
                product_filename +
                "> is not an 'muscate' L1 header file (by reading platform in the xml file).")
            return False
        # Init XML handler
        lHandler = MuscateXMLFileHandler(product_filename, validate=validate, schema_path=schema_path)
        # Store the satellite
        self.Satellite = lHandler.get_string_value_of("Platform")
        self.SatelliteID = self.Satellite.upper().replace("-","")
        # Store the plugin name
        self.PluginName = plugin_base.PluginName
        self.ProductFileName = product_filename

        self.FileCategory = plugin_base.ShortFileType  # LSC
        self.LevelType = "L1VALD"
        self.Prefix = self.Satellite
        self.FileClass = "TEST"  # LANDSAT5-TM-XSTH...
        self.Site = lHandler.get_string_value_of("ZoneGeo")
        self.ProductDateStr = lHandler.get_acquisition_date_formated_yyyymmdd()        # YYYYMMDD
        # LANDSAT5-TM-XSTH_20100118-103000-000_L1C_EU93066200A00B_C_V1-0
        self.ProductId = lHandler.get_string_value_of("ProductId")
        self.ProductVersion = lHandler.get_string_value_of("ProductVersion")
        l_DatePDV = lHandler.get_date_pdv_formated_utc()      # UTC=2010-01-18T12:00:00

        self.ProductDate = date_utils.get_datetime_from_utc(l_DatePDV)

        self.GenerationDateStr = lHandler.get_string_value_of("ProductionDate")

        self.AcquisitionStart = lHandler.get_string_value_of("AcquisitionDate")

        self.OrbitNumber = lHandler.get_string_value_of("OrbitNumber")

        self.ReferenceSiteDefinitionId = "UNKNOWN"

        self.HeaderFilename = product_filename
        self.HeaderHandler = lHandler

        if xml_tools.get_only_value(lHandler.root, "//Product_Characteristics/INSTRUMENT",check=True) is not None:
            self.Instrument = lHandler.get_string_value_of("Instrument")

        if xml_tools.get_only_value(lHandler.root, "//Product_Characteristics/SPECTRAL_CONTENT",check=True) is not None:
            self.SpectralContent = lHandler.get_string_value_of("SpectralContent").replace("+","")

        # VENUS specification
        # Store the VIE and SOL filenames (DATA and Headers) to copy in the L2 product
        self.SOLHeaderFileName = ""
        self.SOLImageFileName = ""
        self.VIEHeaderFileName = ""
        self.VIEImageFileName = ""
        #         Initialize the parameters necessary for the core of the algorithms of MACCS
        # Get longitude and latitude coordinates of the product
        ulc = lHandler.get_upper_left_corner()
        lrc = lHandler.get_lower_right_corner()
        # Estimation of the coordinate of the central point
        center = lHandler.get_center()

        #l_Corner.Longitude = ulc[0] + (lrc[0] - ulc[0]) / 2.
        #l_Corner.Latitude = ulc[1] + (lrc[1] - ulc[1]) / 2.
        self.CenterCorner.longitude = center[0]
        self.CenterCorner.latitude = center[1]
        self.CenterCorner.column = 0
        self.CenterCorner.line = 0
        # Initialize the Validity Start/Stop
        l_UTCValidity = date_utils.get_utc_from_datetime(self.ProductDate)
        if l_UTCValidity.endswith('.000Z'):
            l_UTCValidity = l_UTCValidity[:-5]
        self.UTCValidityStart = l_UTCValidity
        self.UTCValidityStop = l_UTCValidity

        # Get the list of bands in a L2 product
        l_BandsDefinitions = plugin_base.BandsDefinitions

        l_ListOfBandsL2Coarse = l_BandsDefinitions.get_list_of_band_id_in_l2_coarse()
        l_NbBandsL2Coarse = len(l_ListOfBandsL2Coarse)

        LOGGER.debug("l_BandsDefinitions->GetListOfBandCodeInL2Coarse -> l_NbBandsL2Coarse: " + str(l_NbBandsL2Coarse))
        LOGGER.debug("MuscateXmllHandler->GetListOfBands      -> l_NbBands                : " +
                     str(len(lHandler.get_list_of_bands())))

        self.ListOfViewingAnglesPerBandAtL2Resolution = []
        self.ListOfViewingAnglesPerBandAtL2CoarseResolution = []
        self.ListOfViewingZenithAnglesPerBandAtL2CoarseResolution = []
        self.ListOfViewingAzimuthAnglesPerBandAtL2CoarseResolution = []

        # Initialize the Viewing angles for each detectors (Zenith and Azimuth)
        # Read the constants values from the Header Envi file name
        # The angles must be in degree
        if plugin_base.WideFieldSensor:
            # Initialize the Viewing angles for each detectors (Zenith and Azimuth)
            l_MeanViewingZenithalAngles = lHandler.get_mean_viewing_zenithal_angles()
            l_MeanViewingAzimuthalAngles = lHandler.get_mean_viewing_azimuthal_angles()
            self.ViewingAngle = {
                "incidence_zenith_angle": str(statistics.mean(l_MeanViewingZenithalAngles)),
                "incidence_azimuth_angle": str(statistics.mean(l_MeanViewingAzimuthalAngles))
                }

            # For each bands for EnviProduct
            for bd in range(0, len(l_ListOfBandsL2Coarse)):
                angles = {"incidence_zenith_angle": str(l_MeanViewingZenithalAngles[bd]),
                          "incidence_azimuth_angle": str(l_MeanViewingAzimuthalAngles[bd])
                          }
                self.ListOfViewingAnglesPerBandAtL2CoarseResolution.append(angles)
                self.ListOfViewingZenithAnglesPerBandAtL2CoarseResolution.append(str(angles["incidence_zenith_angle"]))
                self.ListOfViewingAzimuthAnglesPerBandAtL2CoarseResolution.append(
                    str(angles["incidence_azimuth_angle"]))
        else:
            lViewingAngles = lHandler.get_mean_viewing_angles()
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
        lSolarAngles = lHandler.get_mean_solar_angles()
        self.SolarAngle = {
            "sun_zenith_angle": lSolarAngles[0],
            "sun_azimuth_angle": lSolarAngles[1]
        }

        if xml_tools.get_only_value(lHandler.root, "//Angles_Grids_List/Sun_Angles_Grids", check=True) is not None:
            self.SolarAngleGrid["StepUnit"] = xml_tools.get_attribute(lHandler.root, "//Angles_Grids_List/Sun_Angles_Grids/Zenith", "step_unit")
            self.SolarAngleGrid["ColStep"] = lHandler.get_string_value_of("SunAngleColStep")
            self.SolarAngleGrid["RowStep"] = lHandler.get_string_value_of("SunAngleRowStep")
            self.SolarAngleGrid["Azimuth"] = lHandler.get_sun_azimuthal_angles()
            self.SolarAngleGrid["Zenith"] = lHandler.get_sun_zenithal_angles()

        # Viewing angle grids
        self.ViewingAngleGrids = []
        if len(xml_tools.get_all_values(lHandler.root,
            "//Angles_Grids_List/Viewing_Incidence_Angles_Grids_List/Band_Viewing_Incidence_Angles_Grids_List")):
            if lHandler.has_per_band_angles():
                for bn, bandid in l_BandsDefinitions.L2CoarseBandMap.items():
                    zonelist = lHandler.get_list_of_zones(bn)
                    zenith_values = lHandler.get_viewing_zenithal_angles(bn)
                    azimuth_values = lHandler.get_viewing_azimuthal_angles(bn)
                    LOGGER.debug("Viewing Angle grid for band " + str(bn))
                    for d, det in enumerate(zonelist):
                        self.ViewingAngleGrids.append({
                            "StepUnit":lHandler.get_viewing_grid_step_unit(bn,det),
                            "ColStep":lHandler.get_viewing_grid_col_step(bn,det),
                            "RowStep":lHandler.get_viewing_grid_row_step(bn,det),
                            "Band":str(bandid),
                            "Detector":det.lstrip('0'),
                            "Azimuth":azimuth_values[d],
                            "Zenith":zenith_values[d]
                            })
            else:
                zonelist = lHandler.get_list_of_zones("dummy")
                for det in zonelist:
                    LOGGER.debug("Viewing Angle grid for det " + det)
                    self.ViewingAngleGrids.append({
                        "StepUnit":lHandler.get_viewing_grid_step_unit(bandid=None,det=det),
                        "ColStep":lHandler.get_viewing_grid_col_step(bandid=None,det=det),
                        "RowStep":lHandler.get_viewing_grid_row_step(bandid=None,det=det),
                        "Detector":det.lstrip('0'),
                        "Azimuth":lHandler.get_viewing_azimuthal_angles(det)[0],
                        "Zenith":lHandler.get_viewing_zenithal_angles(det)[0]
                        })

        # Area by resolution
        self.AreaByResolution = []
        l_grpSuffixes = l_BandsDefinitions.ListOfL2Resolution
        if len(l_grpSuffixes) == 1:
            l_grpSuffixes = ["XS"]
        for res in l_grpSuffixes:
            l_path_group_geo = "//Group_Geopositioning_List/Group_Geopositioning[@group_id='{}']/{}"
            curArea = Area()
            curArea.origin = (
                xml_tools.get_xml_string_value(lHandler.root, l_path_group_geo.format(res,"ULX")),
                xml_tools.get_xml_string_value(lHandler.root, l_path_group_geo.format(res,"ULY")))
            curArea.spacing = (
                xml_tools.get_xml_string_value(lHandler.root, l_path_group_geo.format(res,"XDIM")),
                xml_tools.get_xml_string_value(lHandler.root, l_path_group_geo.format(res,"YDIM")))
            curArea.size = (
                xml_tools.get_xml_string_value(lHandler.root, l_path_group_geo.format(res,"NCOLS")),
                xml_tools.get_xml_string_value(lHandler.root, l_path_group_geo.format(res,"NROWS")))
            self.AreaByResolution.append(curArea)
        # Spectral information
        self.SpectralInfo = []
        l_pathBase = "//Spectral_Band_Informations_List/Spectral_Band_Informations[@band_id='{}']"
        l_pathNativeCoeff = "/Calibration_Coefficients_Lists/Native_Coefficients_List/COEFFICIENT[@name='{}']"
        l_pathRadiance = "/SOLAR_IRRADIANCE"
        l_pathWavelength = "/Wavelength/{}"
        l_pathResponse = "/Spectral_Response/{}"
        l_pathsSpecInfo = {
            'PhysicalGain': l_pathNativeCoeff.format("PhysicalGain"),
            'LuminanceMax' : l_pathNativeCoeff.format("LuminanceMax"),
            'LuminanceMin' : l_pathNativeCoeff.format("LuminanceMin"),
            'QuantizeCalMax' : l_pathNativeCoeff.format("QuantizeCalMax"),
            'QuantizeCalMin' : l_pathNativeCoeff.format("QuantizeCalMin"),
            'RadianceAdd' : l_pathNativeCoeff.format("RadianceAdd"),
            'RadianceMult' : l_pathNativeCoeff.format("RadianceMult"),
            'ReflectanceAdd' : l_pathNativeCoeff.format("ReflectanceAdd"),
            'ReflectanceMult' : l_pathNativeCoeff.format("ReflectanceMult"),
            'SolarIrradiance' : l_pathRadiance,
            'WavelengthMin' : l_pathWavelength.format("MIN"),
            'WavelengthMax' : l_pathWavelength.format("MAX"),
            'WavelengthCentral' : l_pathWavelength.format("CENTRAL"),
            'ResponseStep' : l_pathResponse.format("STEP"),
            'ResponseValues' : l_pathResponse.format("VALUES")
            }
        for b, bidx in l_BandsDefinitions.L1BandMap.items():
            specInfo = {"Band":b}
            realBase = l_pathBase.format(b)
            for measure, pathMeasure in l_pathsSpecInfo.items():
                res = xml_tools.get_xml_string_value(lHandler.root,realBase+pathMeasure,check=True)
                if len(res):
                    specInfo[measure] = res
                    if measure == 'PhysicalGain':
                        specInfo[measure] = float(res)
            self.SpectralInfo.append(specInfo)

        # -------------------------------------------------------------------------
        # 4.2: New
        # Set the L1 no data value
        self.L1NoData = int(lHandler.get_string_value_of("L1NoData"))
        # Set the reflectance quantification value
        self.ReflectanceQuantification = 1. / float(lHandler.get_string_value_of("QuantificationValue"))
        # Computes the real value of the L1 NoData
        self.RealL1NoData = float(self.L1NoData) * self.ReflectanceQuantification

        # Save metadata related to Muscate format
        self.MuscateData["Node_MetadataFormat"] = xml_tools.extract_nodes(lHandler.root, "//Metadata_Identification/METADATA_FORMAT")
        l_NodeOriginalDataDiffuser = xml_tools.extract_nodes(lHandler.root, "//ORIGINAL_DATA_DIFFUSER")
        if l_NodeOriginalDataDiffuser is not None:
            self.MuscateData["Node_OriginalDataDiffuser"] = l_NodeOriginalDataDiffuser
        self.MuscateData["Node_Geoposition_Informations"] = xml_tools.extract_nodes(lHandler.root, "//Geoposition_Informations")
        self.MuscateData["Node_Geometric_Informations"] = xml_tools.extract_nodes(lHandler.root, "//Geometric_Informations")
        #~ self.MuscateData["Identifier"] = lIdent
        self.MuscateData["Authority"] = lHandler.get_string_value_of("Authority")
        self.MuscateData["Producer"] = lHandler.get_string_value_of("Producer")
        self.MuscateData["Project"] = lHandler.get_string_value_of("Project")
        self.MuscateData["ZoneGeo"] = lHandler.get_string_value_of("ZoneGeo")
        #~ self.MuscateData["Platform"] = self.Satellite

        self.MuscateData["AcquisitionDate"] = lHandler.get_string_value_of("AcquisitionDate")
        self.MuscateData["UTCAcquisitionRangeMean"] = lHandler.get_string_value_of("UTCAcquisitionRangeMean")
        self.MuscateData["UTCAcquisitionRangeDatePrecision"] = lHandler.get_string_value_of("UTCAcquisitionRangeDatePrecision")

        l_NodeSolarAnglesGrid = xml_tools.extract_nodes(lHandler.root, "//Data_List/Data[Data_Properties/NATURE='Solar_Angles_Grid']")
        if l_NodeSolarAnglesGrid is not None:
            self.MuscateData["Node_Solar_Angles_Grid"] = l_NodeSolarAnglesGrid

        l_NodeViewingAnglesGrid = xml_tools.extract_nodes(lHandler.root, "//Data_List/Data[Data_Properties/NATURE='Viewing_Angles_Grid']")
        if l_NodeViewingAnglesGrid is not None:
            self.MuscateData["Node_Viewing_Angles_Grid"] = l_NodeViewingAnglesGrid

        l_NodeUsefulImageInfoFile = xml_tools.extract_nodes(lHandler.root, "//Data_List/Data[Data_Properties/NATURE='Useful_Image_Informations_File']")
        if l_NodeUsefulImageInfoFile is not None:
            self.MuscateData["Node_Useful_Image_Informations_File"] = l_NodeUsefulImageInfoFile

        l_NodeUsefulImage = xml_tools.extract_nodes(lHandler.root, "//Mask_List/Mask[Mask_Properties/NATURE='Useful_Image']")
        if l_NodeUsefulImage is not None:
            self.MuscateData["Node_Useful_Image"] = l_NodeUsefulImage

        l_NodeDetFoo = xml_tools.get_only_value(lHandler.root,"//Mask_List/Mask/Mask_Properties/NATURE[.='Detector_Footprint']",check=True)
        if l_NodeDetFoo is not None:
            self.MuscateData["ZoneMaskFileNames"] = lHandler.get_map_list_of_detector_footprint_image_filenames()

        pix_node = xml_tools.get_only_value(
            lHandler.root,
            "//Mask_List/Mask/Mask_Properties/NATURE[.='Aberrant_Pixels']",
            check=True)
        if pix_node is not None:
            self.MuscateData["PIXImages"] = lHandler.get_list_of_pix_mask_filenames()
            self.MuscateData["PIXIndices"] = lHandler.get_list_of_pix_mask_indices()

        spectral_node = xml_tools.extract_nodes(lHandler.root,"//Radiometric_Informations/Spectral_Band_Informations_List")
        if spectral_node is not None:
            self.MuscateData["Node_Spectral_Band_Informations_List"] = spectral_node

        qualityGeo_node = xml_tools.extract_nodes(lHandler.root, "//Current_Product/Product_Quality_List[@level='Geo']")
        if qualityGeo_node is not None:
            self.MuscateData["Node_Product_Quality_List_Geo"] = qualityGeo_node

        qualityNatif_node = xml_tools.extract_nodes(lHandler.root, "//Current_Product/Product_Quality_List[@level='Natif']")
        if qualityNatif_node is not None:
            self.MuscateData["Node_Product_Quality_List_Natif"] = qualityNatif_node

        processingJob_node = xml_tools.extract_nodes(lHandler.root, "//Production_Informations/Processing_Jobs_List")
        if processingJob_node is not None:
            self.MuscateData["Node_Processing_Jobs_List"] = processingJob_node

        return True
