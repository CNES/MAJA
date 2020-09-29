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
                        o  o  o o    o      o o    o     |||D
                        o     o oooooo      o oooooo      / \
                        o     o o    o o    o o    o      `-^
                        o     o o    o  oooo  o    o

###################################################################################################

orchestrator.common.earth_explorer.gipp_l2_comm_earth_explorer_xml_file_handler -- shortdesc

orchestrator.common.earth_explorer.gipp_l2_comm_earth_explorer_xml_file_handler is a description

It defines classes_and_methods

###################################################################################################
"""

import orchestrator.common.xml_tools as xml_tools
import os
from orchestrator.common.logger.maja_logging import configure_logger
from orchestrator.plugins.common.base.maja_l1_image_info_base import L1ImageInformationsBase
from orchestrator.common.maja_exceptions import MajaExceptionPluginBase
from orchestrator.plugins.venus.maja_venus_plugin import MajaVenusPlugin
from orchestrator.common.earth_explorer.earth_explorer_utilities import EarthExplorerUtilities
from orchestrator.plugins.venus.maja_venus_l1_header_image_earth_explorer import VenusL1HeaderImageEarthExplorerXMLFileHandler
from orchestrator.plugins.venus.maja_venus_l1_image_filenames import VenusL1ImageFilenames
from orchestrator.common.maja_common import Area
import orchestrator.common.date_utils as date_utils

LOGGER = configure_logger(__name__)


class MajaVenusL1ImageInformations(L1ImageInformationsBase):
    def __init__(self):
        super(MajaVenusL1ImageInformations, self).__init__()
        self._plugin = MajaVenusPlugin()
        self.UniqueSatellite = self._plugin.UniqueSatellite
        self.HeaderHandler = None
        self.FilenamesProvider = VenusL1ImageFilenames()


    def detect_l1_products(self, input_directory, l_ListOfL1Products):
        LOGGER.info("Start Venus L1 DetectL1Products in " + input_directory)
        l_ListOfFilenames = [os.path.join(input_directory, f) for f in os.listdir(input_directory) if os.path.isfile(os.path.join(input_directory, f))]
        l_NbFiles = len(l_ListOfFilenames)
        LOGGER.debug("Nb files in the sub directory: " + str(l_NbFiles))

        for filename in l_ListOfFilenames:
            LOGGER.debug("Test if the filename '" + filename + "' is a Venus L1 product (VSC_L1VALD and .DBL extension conditions) ..." )
            # Detect the VSC_L1VALD and .DBL file
            if "VSC_L1VALD" in filename:
                if os.path.splitext(filename)[1] == ".DBL":
                    l_ListOfL1Products.append(os.path.join(input_directory,filename))
                elif os.path.splitext(filename)[1] == ".HDR":
                    l_DBLFilename= os.path.splitext(filename)[0] + ".DBL"
                    if not os.path.exists(l_DBLFilename):
                        l_ListOfL1Products.append(l_DBLFilename)
        LOGGER.debug(l_ListOfL1Products)

        # FA LEVEE SUR TMA VENUS - V 4.1 - Corrige dans patch 4.1.1 et reportee dans 4.2
        # If no L1VALD product, try to detect the L1NOTV products
        if len(l_ListOfL1Products)==0:
            for filename in l_ListOfFilenames:
                LOGGER.debug(
                    "Test if the filename '" + filename + "' is a Venus L1 product (VSC_L1NOTV and .DBL extension conditions) ...")
                # Detect the VSC_L1VALD and .DBL file
                if "VSC_L1NOTV" in filename:
                    if os.path.splitext(filename)[1] == ".DBL":
                        l_ListOfL1Products.append(os.path.join(input_directory, filename))
                    elif os.path.splitext(filename)[1] == ".HDR":
                        l_DBLFilename = os.path.splitext(filename)[0] + ".DBL"
                        if not os.path.exists(l_DBLFilename):
                            l_ListOfL1Products.append(l_DBLFilename)


    def initialize(self, product_filename, validate=False, schema_path=None):
        LOGGER.info("Start Venus L1 Initialize on product " + product_filename)
        l_hdrfilename = os.path.splitext(product_filename)[0] + ".HDR"
        l_CanLoad = xml_tools.can_load_file(l_hdrfilename)
        if not l_CanLoad:
            return False

        rootNode = xml_tools.get_root_xml(l_hdrfilename, deannotate=True)
        self.Satellite = xml_tools.get_xml_string_value(rootNode, "//Mission")
        if not self._plugin.is_valid_with_satellite(self.Satellite):
            LOGGER.debug("The L1 product '" + product_filename + "' with satellite '" + self.Satellite + "' is not a VENUS product !")
        self.SatelliteID = self.Satellite.upper()
        self.PluginName = self._plugin.PluginName
        self.Prefix = "VE"
        l_File_Type = xml_tools.get_xml_string_value(rootNode, "//File_Type")
        filenamekey = l_File_Type.split("_")
        self.FileCategory = filenamekey[0]
        self.LevelType = filenamekey[1]
        self.FileClass = xml_tools.get_xml_string_value(rootNode, "//File_Class")
        self.Site = xml_tools.get_xml_string_value(rootNode, "//Instance_Id/Nick_Name")
        self.ReferenceSiteDefinitionId = xml_tools.get_xml_string_value(rootNode, "//Reference_SiteDefinition_Id")
        l_AcquisitionDateTime = xml_tools.get_xml_string_value(rootNode, "//Product_Information/Acquisition_Date_Time")
        self.ProductDate = date_utils.get_datetime_from_utc(l_AcquisitionDateTime)
        self.ProductDateStr = self.ProductDate.strftime('%Y%m%d')
        LOGGER.debug("Product Date: " + self.ProductDateStr)

        self.ProductId = xml_tools.get_xml_string_value(rootNode, "//Fixed_Header/File_Name")


        genDate = xml_tools.get_xml_string_value(rootNode, "//Processing_Information/Date_Time")
        genDate = genDate[4:]
        if genDate[-1] != 'Z':
            genDate = genDate + 'Z'
        self.GenerationDateStr = genDate
        self.AcquisitionStart = l_AcquisitionDateTime[4:]

        self.OrbitNumber = xml_tools.get_xml_string_value(rootNode, "//Product_Information/Acquisition_Orbit_Number")
        self.SpectralContent = "XS"

        self.FilenamesProvider.initialize(l_hdrfilename, validate=validate, schema_path=schema_path)
        self.HeaderFilename = self.FilenamesProvider.m_hdrfilename
        self.SOLImageFileName = self.FilenamesProvider.m_SOLImageFileName
        self.SOLHeaderFileName = self.FilenamesProvider.m_SOLHeaderFileName
        self.VIEImageFileName = self.FilenamesProvider.m_VIEImageFileName
        self.VIEHeaderFileName = self.FilenamesProvider.m_VIEHeaderFileName

        self.HeaderHandler = VenusL1HeaderImageEarthExplorerXMLFileHandler(l_hdrfilename)
        # Estimation of the coordinate of the central point
        self.CenterCorner.longitude = self.HeaderHandler.get_useful_image_geo_coverage_center_corner_long()
        self.CenterCorner.latitude = self.HeaderHandler.get_useful_image_geo_coverage_center_corner_lat()
        self.CenterCorner.column = self.HeaderHandler.get_useful_image_geo_coverage_center_corner_column()
        self.CenterCorner.line = self.HeaderHandler.get_useful_image_geo_coverage_center_corner_line()

        # Initialize the Validity Start/Stop
        self.UTCValidityStart = l_AcquisitionDateTime
        self.UTCValidityStop = l_AcquisitionDateTime
        # Initialize the Viewing angles for each detectors (Zenith and Azimuth)
        self.ListOfViewingZenithAnglesPerBandAtL2CoarseResolution = []
        self.ListOfViewingAzimuthAnglesPerBandAtL2CoarseResolution = []

        l_meanViewingZenith = 0.0
        l_meanViewingAzimuth = 0.0
        l_count = 0.0
        l_BandsDefinitions = self._plugin.BandsDefinitions
        for det in l_BandsDefinitions.DetectorMap:
            l_Zenith = self.HeaderHandler.get_useful_image_center_view_angle_zenith(det)
            l_Azimuth = self.HeaderHandler.get_useful_image_center_view_angle_azimuth(det)
            l_meanViewingZenith += l_Zenith
            l_meanViewingAzimuth += l_Azimuth
            l_count += 1.0
            self.ListOfViewingZenithAnglesPerBandAtL2CoarseResolution.append(str(l_Zenith))
            self.ListOfViewingAzimuthAnglesPerBandAtL2CoarseResolution.append(str(l_Azimuth))
            self.ListOfViewingAnglesPerBandAtL2CoarseResolution.append(
              { "incidence_zenith_angle": str(l_Zenith),
                "incidence_azimuth_angle": str(l_Azimuth)
              })

        self.ViewingAngle = {
            "incidence_zenith_angle": str(l_meanViewingZenith / l_count),
            "incidence_azimuth_angle": str(l_meanViewingAzimuth / l_count)
            }

        # Fill the L2 resolution angles
        self.ListOfViewingAnglesPerBandAtL2Resolution = self.ListOfViewingAnglesPerBandAtL2CoarseResolution
        # Solar Angles
        self.SolarAngle = {
            "sun_zenith_angle": self.HeaderHandler.get_useful_image_image_center_solar_angle_zenith(),
            "sun_azimuth_angle": self.HeaderHandler.get_useful_image_image_center_solar_angle_azimuth()
        }

        # Detect pixel size and product size
        originX = xml_tools.get_xml_float_value(rootNode, "//Geo_Referencing_Information/Product_Coverage/Cartographic/Upper_Left_Corner/X")
        originY = xml_tools.get_xml_float_value(rootNode, "//Geo_Referencing_Information/Product_Coverage/Cartographic/Upper_Left_Corner/Y")
        pixSizeX = xml_tools.get_xml_float_value(rootNode, "//Product_Sampling/By_Column")
        pixSizeY = xml_tools.get_xml_float_value(rootNode, "//Product_Sampling/By_Line")
        nbCol = xml_tools.get_xml_int_value(rootNode, "//Image_Information/Size/Columns")
        nbRow = xml_tools.get_xml_int_value(rootNode, "//Image_Information/Size/Lines")
        gridColStep = (nbCol - 1.0) * pixSizeX
        gridRowStep = (nbRow - 1.0) * pixSizeY

        gridColStepStr = f"{gridColStep:.1f}"
        gridRowStepStr = f"{gridRowStep:.1f}"
        
        # Solar angle grid
        ula = xml_tools.get_xml_string_value(rootNode, "//Solar_Angles/Product/Upper_Left_Corner/Azimuth")
        ulz = xml_tools.get_xml_string_value(rootNode, "//Solar_Angles/Product/Upper_Left_Corner/Zenith")
        ura = xml_tools.get_xml_string_value(rootNode, "//Solar_Angles/Product/Upper_Right_Corner/Azimuth")
        urz = xml_tools.get_xml_string_value(rootNode, "//Solar_Angles/Product/Upper_Right_Corner/Zenith")
        lla = xml_tools.get_xml_string_value(rootNode, "//Solar_Angles/Product/Lower_Left_Corner/Azimuth")
        llz = xml_tools.get_xml_string_value(rootNode, "//Solar_Angles/Product/Lower_Left_Corner/Zenith")
        lra = xml_tools.get_xml_string_value(rootNode, "//Solar_Angles/Product/Lower_Right_Corner/Azimuth")
        lrz = xml_tools.get_xml_string_value(rootNode, "//Solar_Angles/Product/Lower_Right_Corner/Zenith")
        self.SolarAngleGrid["StepUnit"] = "m"
        self.SolarAngleGrid["ColStep"] = gridColStepStr
        self.SolarAngleGrid["RowStep"] = gridRowStepStr
        self.SolarAngleGrid["Azimuth"] = [ula+' '+ura, lla+' '+lra]
        self.SolarAngleGrid["Zenith"] = [ulz+' '+urz, llz+' '+lrz]
        
        # Viewing angle grids
        detectors = [1,2,3,4]
        self.ViewingAngleGrids = []
        l_pathView = "//List_of_Viewing_Angles/Viewing_Angles[@sn='{}']/Product/{}"
        for det in detectors:
            ula = xml_tools.get_xml_string_value(rootNode, l_pathView.format(det,"Upper_Left_Corner/Azimuth"))
            ulz = xml_tools.get_xml_string_value(rootNode, l_pathView.format(det,"Upper_Left_Corner/Zenith"))
            ura = xml_tools.get_xml_string_value(rootNode, l_pathView.format(det,"Upper_Right_Corner/Azimuth"))
            urz = xml_tools.get_xml_string_value(rootNode, l_pathView.format(det,"Upper_Right_Corner/Zenith"))
            lla = xml_tools.get_xml_string_value(rootNode, l_pathView.format(det,"Lower_Left_Corner/Azimuth"))
            llz = xml_tools.get_xml_string_value(rootNode, l_pathView.format(det,"Lower_Left_Corner/Zenith"))
            lra = xml_tools.get_xml_string_value(rootNode, l_pathView.format(det,"Lower_Right_Corner/Azimuth"))
            lrz = xml_tools.get_xml_string_value(rootNode, l_pathView.format(det,"Lower_Right_Corner/Zenith"))
            self.ViewingAngleGrids.append({
                "StepUnit":"m",
                "ColStep":gridColStepStr,
                "RowStep":gridRowStepStr,
                "Detector":str(det),
                "Azimuth":[ula+' '+ura, lla+' '+lra],
                "Zenith":[ulz+' '+urz, llz+' '+lrz]
                })

        # Set Area by resolution
        curArea = Area()
        curArea.origin = (
            f"{originX:.1f}",
            f"{originY:.1f}")
        curArea.spacing = (
            f"{pixSizeX:.1f}",
            f"{-pixSizeY:.1f}")
        curArea.size = (
            str(nbCol),
            str(nbRow))
        self.AreaByResolution = [curArea]

        # Gather spectral information
        l_resol = l_BandsDefinitions.ListOfL1Resolution[0]
        l_pathAk = "//List_of_Aks/Ak[@sk='{}']"
        l_pathPolarCoef = "//List_of_Polarization_Coefficients/Polarization_Coefficient[@sk='{}']"
        l_pathWavelenghCentral = "//List_of_Band_Central_Wavelength/Band_Central_Wavelength[@sk='{}']"
        self.SpectralInfo = []
        for b, bidx in l_BandsDefinitions.L1BandMap.items():
            bcode = l_BandsDefinitions.L1ListOfBandsMap[l_resol][bidx]
            self.SpectralInfo.append({
                "Band":b.replace("B0", "B"),
                "Ak":xml_tools.get_xml_string_value(rootNode,l_pathAk.format(bcode)) ,
                "PolarizationCoefficient":xml_tools.get_xml_string_value(rootNode,l_pathPolarCoef.format(bcode)),
                "WavelengthCentral": xml_tools.get_xml_string_value(rootNode,l_pathWavelenghCentral.format(bcode))
                })

        # 4.2: New
        # Set the L1 no data value
        self.L1NoData = self.HeaderHandler.get_no_data_value_as_int()
        # Set the reflectance quantification value
        self.ReflectanceQuantification = self.HeaderHandler.get_reflectance_quantification_value()
        # Computes the real value of the L1 NoData
        self.RealL1NoData = self.L1NoData * self.ReflectanceQuantification

        return True















































