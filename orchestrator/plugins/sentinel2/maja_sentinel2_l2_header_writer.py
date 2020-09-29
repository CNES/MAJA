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
#                        o  o  o o    o      o o    o     |||)<
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
from orchestrator.common.maja_exceptions import *
from orchestrator.plugins.common.earth_explorer.maja_earth_explorer_l2_header_file_writer import EarthExplorerL2HeaderFileWriter
from orchestrator.common.earth_explorer.earth_explorer_xml_file_handler import EarthExplorerXMLFileHandler
from orchestrator.plugins.sentinel2.maja_sentinel2_l1_granule_xml_reader import MajaSentinel2L1GranuleXmlReader
from orchestrator.plugins.sentinel2.maja_sentinel2_l1_main_xml_reader import MajaSentinel2L1MainXmlReader
import orchestrator.common.file_utils as file_utils
import orchestrator.common.maja_utils as maja_utils
import orchestrator.common.xml_tools as xml_tools
from orchestrator.common.earth_explorer.earth_explorer_utilities import EarthExplorerUtilities
from orchestrator.plugins.sentinel2.maja_sentinel2_plugin import MajaSentinel2Plugin
from orchestrator.common.logger.maja_logging import configure_logger
import os
import datetime
import statistics
LOGGER = configure_logger(__name__)


class MajaSentinel2L2HeaderWriter(EarthExplorerL2HeaderFileWriter):

    def __init__(self):
        super(MajaSentinel2L2HeaderWriter, self).__init__()
        self.plugin = MajaSentinel2Plugin()

    def create(self, plugin_name, app_handler):
        if plugin_name == "SENTINEL2":
            self.apphandler = app_handler
            return True
        else:
            return False

    def write(self):
        # 4.3
        self.preprocessing()
        if self.plugin is None:
            raise MajaException("Internal error: the variable m_PluginBasePointer is NULL!")

        # Replace L2VALD by L2NOTV
        # Note: do in the L2Proefssor
        lCurrentDate = datetime.datetime.now().strftime(EarthExplorerXMLFileHandler.XML_FORMAT_DATE)

        l_FileType = "SSC_L2VALD"

        if not self.productisvalid:
            # TODO
            #self.l2imagefilenamesprovider.InvalidateTheFilenames()
            l_FileType = "SSC_L2NOTV"

        file_utils.create_directory(self.l2imagefilenamesprovider.get_public_directory())
        file_utils.create_directory(self.l2imagefilenamesprovider.get_private_filenames().get_private_directory())

        # ---------------------------------------------------------------------------------------------------
        # Write LTC Private Header
        # Note necessary for Backward mode
        # ---------------------------------------------------------------------------------------------------
        # Write LTC Private Header
        # Note necessary for Backward mode
        if self.writeltc:
            # ---------------------------------------------------------------------------------------------
            # Create the LTC directory if don't exist
            file_utils.create_directory(
                self.l2imagefilenamesprovider.get_private_filenames().get_ltc_image_dir_filename())
        # TODO else:
        #    System::RemoveDirectory(self.l2imagefilenamesprovider.GetPrivateFileNames().GetLTCImageDirFileName())
        #    itksys::SystemTools::RemoveFile(self.l2imagefilenamesprovider.GetPrivateFileNames().GetLTCHeaderFileName().c_str())

        # Create the .DBL file (zero)
        file_utils.touch_file(self.l2imagefilenamesprovider.get_dbl_filename())

        LOGGER.debug("Start Sentinel2L2HeaderFileWriterProvider::Write() ...")

        # Get the "share/config" directory
        root_template_directory = self.apphandler.get_share_config_directory()
        LOGGER.debug("Root template install directory '" + root_template_directory + "'")

        root_shemas_directory = self.apphandler.get_schemas_root_install_dir()
        LOGGER.debug("Root shemas install directory '" + root_shemas_directory + "'")
        lSchemaLocationDirectory = root_shemas_directory + self.plugin.MAJA_INSTALL_SCHEMAS_DIR
        LOGGER.debug("Root SENTIENL2 install directory '" + lSchemaLocationDirectory + "'")
        # Read the main header
        l1_input_handler = MajaSentinel2L1MainXmlReader(self.l1imageinformationsproviderbase.ProductFileName,tile_id=self.apphandler.get_tile_id())
        # Get the xml tile header filename
        tileFilename = self.l1imageinformationsproviderbase.xmlTileFilename

        LOGGER.info("Tile xml filename: " + tileFilename)
        l1_input_tile_handler = MajaSentinel2L1GranuleXmlReader(tileFilename)

        # Write Headers ?
        # MACCS 4.7.2 - correction pour FA 1572
        if self.writel2products or self.backwardmode:
            if self.enablewriteearthexplorerheaders:
                # ---------------------------------------------------------------------------------------------
                # Writes the PUBLIC headers files
                if self.writel2products:
                    self.write_public_xml_handler(self.l2imagefilenamesprovider, self.dem,
                                               self.l1imageinformationsproviderbase.UTCValidityStart,
                                               self.l1imageinformationsproviderbase.UTCValidityStop,
                                               self.l1imageinformationsproviderbase.Satellite, root_template_directory,
                                               lSchemaLocationDirectory, lCurrentDate)
                # ---------------------------------------------------------------------------------------------
                # Writes the PRIVATE others headers
                self.write_private_others_xmlhandler(
                    self.l2imagefilenamesprovider.get_private_filenames(),
                    self.l2imagefilenamesprovider.get_reference_product_header_id(),
                    self.l2imagefilenamesprovider.get_reference_product_instance(),
                    self.dem,
                    self.l1imageinformationsproviderbase.UTCValidityStart,
                    self.l1imageinformationsproviderbase.UTCValidityStop,
                    self.l1imageinformationsproviderbase.Satellite,
                    root_template_directory,
                    lSchemaLocationDirectory,
                    lCurrentDate)

                # ---------------------------------------------------------------------------------------------
                # Writes the PRIVATE TEC headers
                self.write_private_tec_xmlhandler(
                    self.l2imagefilenamesprovider.get_private_filenames().get_hdr_private_filename(),
                    self.l2imagefilenamesprovider.get_reference_product_header_id(),
                    self.l1imageinformationsproviderbase.UTCValidityStart,
                    self.l1imageinformationsproviderbase.UTCValidityStop,
                    self.l1imageinformationsproviderbase.Satellite,
                    self.l1imageinformationsproviderbase.Site,
                    self.l1imageinformationsproviderbase.ProductDateStr,
                    root_template_directory,
                    lSchemaLocationDirectory,
                    lCurrentDate)

            # ---------------------------------------------------------------------------------------------
            # Writes the PRIVATE STO header
            self.write_private_sto_xmlhandler(
                self.l2imagefilenamesprovider.get_private_filenames().get_sto_header_filename(),
                self.l2imagefilenamesprovider.get_reference_product_header_id(),
                self.l2imagefilenamesprovider.get_reference_product_instance(),
                self.dem,
                self.l1imageinformationsproviderbase.UTCValidityStart,
                self.l1imageinformationsproviderbase.UTCValidityStop,
                self.l1imageinformationsproviderbase.Satellite,
                root_template_directory,
                lSchemaLocationDirectory,
                lCurrentDate)
            # ---------------------------------------------------------------------------------------------
            # Writes the PRIVATE reflectances headers (RCR, RTA, RTC)
            self.write_private_reflectances_xmlhandler(self.l2imagefilenamesprovider.get_private_filenames(),
                                                    self.l2imagefilenamesprovider.get_reference_product_header_id(),
                                                    self.l2imagefilenamesprovider.get_reference_product_instance(),
                                                    self.dem, self.l1imageinformationsproviderbase.UTCValidityStart,
                                                    self.l1imageinformationsproviderbase.UTCValidityStop,
                                                    self.l1imageinformationsproviderbase.Satellite,
                                                    root_template_directory, lSchemaLocationDirectory, lCurrentDate)

            # ---------------------------------------------------------------------------------------------------
            # Write LTC Private Header (always, because rode in the Backward mode)
            # ---------------------------------------------------------------------------------------------------
            # Write LTC Private Header
            # Note necessary for Backward mode
            if self.writeltc:
                # ---------------------------------------------------------------------------------------------------
                # Get solar Angles
                #l_solarAngles = self.l1imageinformationsproviderbase.SolarAngle
                l_solarAngles = [float(item) for item in list(self.l1imageinformationsproviderbase.SolarAngle.values())]
                # ---------------------------------------------------------------------------------------------------
                # Get viewing Angles
                l_viewingAngles = []
                l_MeanViewingAzimuthalAngle = self.l1imageinformationsproviderbase.ListOfViewingAzimuthAnglesPerBandAtL2CoarseResolution
                l_MeanViewingZenithalAngles = self.l1imageinformationsproviderbase.ListOfViewingZenithAnglesPerBandAtL2CoarseResolution
                l_viewingAngles.append(statistics.mean([float(a) for a in l_MeanViewingZenithalAngles]))
                l_viewingAngles.append(statistics.mean([float(a) for a in l_MeanViewingAzimuthalAngle]))
                # ---------------------------------------------------------------------------------------------------
                # Write LTC Handler
                self.write_private_ltc_xmlhandler(
                    self.inputl2imagefilereader,
                    self.l2imagefilenamesprovider.get_reference_product_header_id(),
                    self.l2imagefilenamesprovider.get_reference_product_instance(),
                    os.path.join(
                        root_template_directory,
                        self.plugin.TEMPLATE_PDTANX_PRIVATE_LTC_HDR),
                    self.l2imagefilenamesprovider.get_private_filenames(),
                    lSchemaLocationDirectory,
                    lCurrentDate,
                    self.l1imageinformationsproviderbase.UTCValidityStart,
                    self.l1imageinformationsproviderbase.UTCValidityStop,
                    l_solarAngles,
                    l_viewingAngles,
                    False)

            # LAIG-DM-MAC-975-CNES
            # Copy the detector footprint (DETFOO gml files) from the L1C product to
            # the L2 product, in the sub directory "MSK_DETFOO"
            l_ZoneMaskFileNames = l1_input_tile_handler.ListOfDetFootPrintHeaderFileName
            l_FullPathDetfoo_subdir = os.path.join(self.l2imagefilenamesprovider.get_public_directory(), "MSK_DETFOO")
            file_utils.create_directory(l_FullPathDetfoo_subdir)
            for f in l_ZoneMaskFileNames:
                file_utils.copy_file_to_directory(f, l_FullPathDetfoo_subdir)

        self.write_global_xmlhandler(
            os.path.join(
                root_template_directory,
                self.plugin.TEMPLATE_GLOBAL_HDR),
            l1_input_handler,
            l1_input_tile_handler,
            self.l2imagefilenamesprovider.get_hdr_filename(),
            l_FileType,
            lSchemaLocationDirectory)

    def write_global_xmlhandler(
            self,
            root_sc_xml_templates,
            l1_input_xmlhandler,
            l1_input_tile_xmlhandler,
            output_filename,
            p_FileType,
            schemaLocationDirectory):
        input_dir = self.l2imagefilenamesprovider.m_PublicDirectory
        # Get global information about the output L2 product
        # ---------------------------------------
        lCurrentDate = datetime.datetime.now().strftime(EarthExplorerXMLFileHandler.XML_FORMAT_DATE)
        lCreator_Version = "3.2.2"
        lFile_Version = self.plugin.L2PRODUCT_FILE_VERSION
        # ---------------------------------------------------------------------------------------------------
        LOGGER.debug("Write the GLOBAL header file ...")
        current_header_filename = root_sc_xml_templates
        if not os.path.exists(current_header_filename):
            raise MajaDataException(
                "Internal error: the template file '" +
                current_header_filename +
                "' doesn't exist !!")
        # ---------------------------------------------------------------------------------------------
        # Load the template file used to writes the new header file
        output_handler = EarthExplorerXMLFileHandler(current_header_filename)
        # ---------------------------------------
        # Set the main header information
        # ---------------------------------------
        EE_File_Name = os.path.basename(output_filename)
        LOGGER.debug("output_filename: " + output_filename)
        output_handler.set_string_value("//File_Version", lFile_Version)
        output_handler.set_string_value("//File_Name", os.path.splitext(EE_File_Name)[0])
        output_handler.set_string_value("//Mission", l1_input_xmlhandler.get_mission())
        output_handler.set_string_value("//Validity_Start", l1_input_tile_xmlhandler.get_validity_start_date())
        output_handler.set_string_value("//Validity_Stop", l1_input_tile_xmlhandler.get_validity_stop_date())
        output_handler.set_string_value("//Creator", self.creator)
        output_handler.set_string_value("//Creator_Version", lCreator_Version)
        output_handler.set_string_value("//Notes", self.notes)
        output_handler.set_string_value("//System", self.system)
        output_handler.set_string_value("//File_Class", self.outputfileclass)
        output_handler.set_string_value("//File_Type", p_FileType)
        output_handler.set_string_value("//Creation_Date", lCurrentDate)
        # Nick_Name
        # Format site under 8 characters
        output_handler.set_string_value(
            "//Nick_Name",
            maja_utils.get_formated_site(
                self.l1imageinformationsproviderbase.Site))

        # Acquisition_Date
        output_handler.set_string_value("//Acquisition_Date", l1_input_tile_xmlhandler.get_date_formated_yyyymmdd())

        if self.initmode:
            output_handler.set_string_value("//Processing_Flags_And_Modes/Processing_Mode", "INIT_MODE")
        elif self.backwardmode:
            output_handler.set_string_value("//Processing_Flags_And_Modes/Processing_Mode", "BACKWARD_MODE")
        else:
            output_handler.set_string_value("//Processing_Flags_And_Modes/Processing_Mode", "STANDARD_MODE")
        output_handler.set_string_value(
            "//Adjacency_Effects_And_Slope_Correction",
            self.adjacencyeffectsandslopecorrection)
        output_handler.set_string_value("//Processing_Flags_And_Modes/Cirrus_Correction", self.cirruscorrection)

        # Acquisition_Date_Time
        output_handler.set_string_value("//Acquisition_Date_Time", l1_input_tile_xmlhandler.get_date_formated_utc())
        output_handler.set_string_value(
            "//Acquisition_Orbit_Number",
            l1_input_xmlhandler.get_string_value_of("OrbitNumber"))

        output_handler.set_string_value("//Web_Site_URL", self.plugin.WebSiteURL)

        output_handler.set_string_value("//Processing_Information/Date_Time", lCurrentDate)

        LOGGER.debug("Before WriteListofGIPPFiles")

        # ---------------------------------------
        # List_of_GIPP_Files
        # ---------------------------------------
        listOfGipps = EarthExplorerUtilities.convert_gipp_filenames_to_file_types(self.listofgippsfilenames)
        output_handler.write_list_of_gipp_files(listOfGipps)
        # ---------------------------------------
        # List_of_Quality_Indexes
        # ---------------------------------------
        listOfQualityInd = []
        qI = EarthExplorerXMLFileHandler.QualityIndex()
        qI.Code = EarthExplorerXMLFileHandler.CONST_RAIN_DETECTED_NV
        qI.Value = str(self.rainflag)
        listOfQualityInd.append(qI)
        qI = EarthExplorerXMLFileHandler.QualityIndex()
        qI.Code = EarthExplorerXMLFileHandler.CONST_HOTSPOT_DETECTED_NV
        qI.Value = str(self.hotspotflag)
        listOfQualityInd.append(qI)
        qI = EarthExplorerXMLFileHandler.QualityIndex()
        qI.Code = EarthExplorerXMLFileHandler.CONST_SUNGLINT_DETECTED_NV
        qI.Value = str(self.sunglintflag)
        listOfQualityInd.append(qI)
        # Since 4-1-0, writes the Cirrus flag if GetCirrusFlag() OR
        # GetCirrusMasking() activated. See #126349 (  FD
        # https:#thor.si.c-s.fr/gf/project/maccs/tracker/?action=TrackerItemEdit&tracker_item_id=126349
        # )
        if self.plugin.CirrusFlag or self.plugin.CirrusMasking:
            qI = EarthExplorerXMLFileHandler.QualityIndex()
            qI.Code = EarthExplorerXMLFileHandler.CONST_CIRRUS_DETECTED_NV
            qI.Value = str(self.cirrusflag)
            listOfQualityInd.append(qI)

        output_handler.write_list_of_quality_indexes(listOfQualityInd)

        # ---------------------------------------
        # Set information in the Geometry block for each resolution
        # ---------------------------------------
        l_UL_R1 = l1_input_tile_xmlhandler.get_geoposition_upperleftcorner(10)
        l_UL_R2 = l1_input_tile_xmlhandler.get_geoposition_upperleftcorner(20)

        output_handler.set_string_value(
            "//Image_Information/List_of_Resolutions/Resolution[@r='10']/Geoposition/ULX", str(l_UL_R1.x))
        output_handler.set_string_value(
            "//Image_Information/List_of_Resolutions/Resolution[@r='10']/Geoposition/ULY", str(l_UL_R1.y))
        output_handler.set_string_value(
            "//Image_Information/List_of_Resolutions/Resolution[@r='20']/Geoposition/ULX", str(l_UL_R2.x))
        output_handler.set_string_value(
            "//Image_Information/List_of_Resolutions/Resolution[@r='20']/Geoposition/ULY", str(l_UL_R2.y))

        l_DIM_R1 = l1_input_tile_xmlhandler.get_geoposition_dims(10)
        l_DIM_R2 = l1_input_tile_xmlhandler.get_geoposition_dims(20)

        output_handler.set_string_value(
            "//Image_Information/List_of_Resolutions/Resolution[@r='10']/Geoposition/XDIM", str(l_DIM_R1.x))
        output_handler.set_string_value(
            "//Image_Information/List_of_Resolutions/Resolution[@r='10']/Geoposition/YDIM", str(l_DIM_R1.y))
        output_handler.set_string_value(
            "//Image_Information/List_of_Resolutions/Resolution[@r='20']/Geoposition/XDIM", str(l_DIM_R2.x))
        output_handler.set_string_value(
            "//Image_Information/List_of_Resolutions/Resolution[@r='20']/Geoposition/YDIM", str(l_DIM_R2.y))

        l_ProductImageSizeX = self.dem.L2Areas[0].size[0]
        l_ProductImageSizeY = self.dem.L2Areas[0].size[1]
        output_handler.set_string_value(
            "//Image_Information/List_of_Resolutions/Resolution[@r='10']/Size/Lines",
            str(l_ProductImageSizeY))
        output_handler.set_string_value(
            "//Image_Information/List_of_Resolutions/Resolution[@r='10']/Size/Columns",
            str(l_ProductImageSizeX))

        l_ProductImageSizeX = self.dem.L2Areas[1].size[0]
        l_ProductImageSizeY = self.dem.L2Areas[1].size[1]
        output_handler.set_string_value(
            "//Image_Information/List_of_Resolutions/Resolution[@r='20']/Size/Lines",
            str(l_ProductImageSizeY))
        output_handler.set_string_value(
            "//Image_Information/List_of_Resolutions/Resolution[@r='20']/Size/Columns",
            str(l_ProductImageSizeX))

        # ---------------------------------------
        # Set solar and viewing angles
        # ---------------------------------------
        # LAIG-FA-MAC-135662-CS : values set in the l1_input_tile_handler and not in the l1_input_handler
        # Solar_Angles
        xml_tools.replace_node(
            l1_input_tile_xmlhandler.root,
            "//Tile_Angles/Mean_Sun_Angle",
            output_handler.root,
            "//Mean_Sun_Angle")
        xml_tools.replace_node(
            l1_input_tile_xmlhandler.root,
            "//Mean_Viewing_Incidence_Angle_List",
            output_handler.root,
            "//Mean_Viewing_Incidence_Angle_List")

        xml_tools.copies_to_child(l1_input_tile_xmlhandler.root, "//Tile_Angles/Sun_Angles_Grid/*",
                                  output_handler.root, "//Solar_Angles")
        xml_tools.copies_to_child(l1_input_tile_xmlhandler.root, "//Viewing_Incidence_Angles_Grids",
                                  output_handler.root, "//List_of_Viewing_Angles")

        output_handler.set_string_value(
            "//Reflectance_Quantification_Value",
            f"{self.reflectancequantificationvalue:.9f}")

        # ---------------------------------------
        # Set Cloud_Percentage
        # ---------------------------------------
        output_handler.set_string_value("//Cloud_Percentage", self.cloudrate)

        # ---------------------------------------
        # Set Snow_Percentage
        # ---------------------------------------
        output_handler.set_string_value("//Snow_Percentage", self.snowrate)

        # ---------------------------------------
        # Set DEM statistics
        # ---------------------------------------
        output_handler.set_string_value("//Used_DEM/DEM_Reference", self.dem.ALT_LogicalName)
        output_handler.set_string_value("//Used_DEM/Statistics/Min", f"{self.dem.ALT_Min:.9f}")
        output_handler.set_string_value("//Used_DEM/Statistics/Max", f"{self.dem.ALT_Max:.9f}")
        output_handler.set_string_value("//Used_DEM/Statistics/Average", f"{self.dem.ALT_Mean:.9f}")
        output_handler.set_string_value("//Used_DEM/Statistics/Standard_Deviation", f"{self.dem.ALT_Stdv:.9f}")

        output_handler.set_string_value("//Nodata_Value", self.nodatavalue)

        # , "//List_of_Used_Product_Ids", "Used_Product_Id");
        output_handler.set_list_of_used_products_ids(self.listofusedproductsids)

        # ---------------------------------------
        # Resolution information
        # ---------------------------------------
        l_NbResolution = len(self.l2imagefilenamesprovider.get_sre_regular_filenames())
        for resol in range(l_NbResolution):
            # ---------------------------------------
            # Product_Organization block per resolution
            # ---------------------------------------
            # Image_File
            regular_sre = self.l2imagefilenamesprovider.get_sre_regular_filenames()[resol]
            output_handler.set_string_value("//Product_Organization/Image_File/Nature", regular_sre.Nature)
            output_handler.set_string_value("//Product_Organization/Image_File/Logical_Name", regular_sre.LogicalName)
            output_handler.set_string_value("//Product_Organization/Image_File/File_Location", regular_sre.FileLocation)
            if self.adjacencyeffectsandslopecorrection:
                regular_fre = self.l2imagefilenamesprovider.get_fre_regular_filenames()[resol]
                xml_tools.copy_to_sibling_inplace(
                    output_handler.root,
                    "//Product_Organization/Image_File",
                    "//Product_Organization/Image_File")
                output_handler.set_string_value("//Product_Organization/Image_File/Nature", regular_fre.Nature)
                output_handler.set_string_value(
                    "//Product_Organization/Image_File/Logical_Name",
                    regular_fre.LogicalName)
                output_handler.set_string_value(
                    "//Product_Organization/Image_File/File_Location",
                    regular_fre.FileLocation)

        # Quicklook_File
        regular_qlk = self.l2imagefilenamesprovider.get_qlk_regular_file()[0]
        output_handler.set_string_value("//Product_Organization/Quicklook_File/Nature", regular_qlk.Nature)
        output_handler.set_string_value("//Product_Organization/Quicklook_File/Logical_Name", regular_qlk.LogicalName)
        output_handler.set_string_value("//Product_Organization/Quicklook_File/File_Location", regular_qlk.FileLocation)
        # Annex
        output_handler.write_list_of_annex_files(self.l2imagefilenamesprovider.get_list_of_annex_file())
        # Private_Data_File
        regular_hdr = self.l2imagefilenamesprovider.get_private_filenames().get_hdr_regular_file()
        output_handler.set_string_value("//Product_Organization/Private_Data_File/Nature", regular_hdr.Nature)
        output_handler.set_string_value(
            "//Product_Organization/Private_Data_File/Logical_Name",
            regular_hdr.LogicalName)
        output_handler.set_string_value(
            "//Product_Organization/Private_Data_File/File_Location",
            regular_hdr.FileLocation)

        # Reference_SystemCode
        output_handler.set_string_value("//Coordinate_Reference_System/Code",
                                        l1_input_tile_xmlhandler.get_string_value_of("ReferenceSystemCode"))
        output_handler.set_string_value("//Coordinate_Reference_System/Short_Description",
                                        l1_input_tile_xmlhandler.get_string_value_of("ReferenceSystemShortDescription"))
        # Save to file
        output_handler.save_to_file(output_filename)
        # Check the GLOBAL header even if  not valid (NOTV)
        if self.checkxmlfileswithschema:
            xml_tools.check_xml(output_filename, schemaLocationDirectory)
