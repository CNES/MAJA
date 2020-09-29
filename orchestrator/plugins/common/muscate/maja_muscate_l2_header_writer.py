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
import orchestrator.common.file_utils as file_utils
import orchestrator.common.date_utils as date_utils
import orchestrator.common.xml_tools as xml_tools
from orchestrator.common.muscate.muscate_xml_file_handler import MuscateXMLFileHandler
from orchestrator.processor.base_processor import BaseProcessor
from orchestrator.plugins.sentinel2.maja_sentinel2_l1_main_xml_reader import MajaSentinel2L1MainXmlReader
from orchestrator.plugins.sentinel2.maja_sentinel2_l1_granule_xml_reader import MajaSentinel2L1GranuleXmlReader
from orchestrator.plugins.common.base.maja_l2_header_writer_base import L2HeaderWriterBase
from orchestrator.common.logger.maja_logging import configure_logger

from orchestrator.plugins.common.base.maja_l2_image_filenames_provider import L2ImageFilenamesProvider
from orchestrator.cots.otb.otb_app_handler import OtbAppHandler
from orchestrator.common.maja_common import PointXY
import os,datetime,statistics
from lxml import etree as ET
import math
import re

from orchestrator.common.constants import CAMSStatus
LOGGER = configure_logger(__name__)


class MajaMuscateL2HeaderWriter(L2HeaderWriterBase):

    def __init__(self):
        super(MajaMuscateL2HeaderWriter, self).__init__()
        self.l2imagefilenamesprovider = None

    def write(self):
        if self.plugin is None:
            raise MajaExceptionPluginBase("Internal error: the variable self.plugin is NULL!")
        if self.l1_plugin is None:
            self.l1_plugin = self.plugin
        if self.l1imageinformationsproviderbase is None:
            raise MajaExceptionPluginBase("Internal error: the variable m_L1ImageInformationsProviderBase is NULL!")
        # 4.3
        self.pre_processing()
        # ---------------------------------------------------------------------------------------------
        # Get the "share/config" directory
        root_template_directory = self.apphandler.get_share_config_directory()
        LOGGER.debug("Root template install directory '" + root_template_directory + "'")
        root_shemas_directory = self.apphandler.get_schemas_root_install_dir()
        LOGGER.debug("Root shemas install directory '" + root_shemas_directory + "'")
        lSchemaLocationDirectory = root_shemas_directory + self.plugin.MAJA_INSTALL_SCHEMAS_L2_DIR
        LOGGER.debug("Root install directory '" + lSchemaLocationDirectory + "'")
        # Reference of the L2 output product filename (coe from the L2IOmageFielWriter
        l_ReferenceOutputL2ProductFilename = self.outputl2productfilename
        LOGGER.debug("ReferenceOutputL2ProductFilename: '" + l_ReferenceOutputL2ProductFilename + "'")
        l_PublicDirectory = os.path.abspath(os.path.join(l_ReferenceOutputL2ProductFilename, os.pardir))
        LOGGER.debug("l_PublicDirectory: '" + l_PublicDirectory + "'")
        l_DATADirectory = l_PublicDirectory + "/DATA"
        LOGGER.debug("l_DATADirectory: '" + l_DATADirectory + "'")
        # ---------------------------------------------------------------------------------------------
        LOGGER.debug("Start MuscateL2HeaderFileWriterProvider::Write() ...")

        LOGGER.debug("XML Header called: ")
        LOGGER.debug(self.plugin)
        LOGGER.debug(self.l1imageinformationsproviderbase.HeaderFilename)
        LOGGER.debug(self.plugin.PluginName)
        # ---------------------------------------------------------------------------------------------
        lCurrentDate = datetime.datetime.now()
        l_Mission = self.l1imageinformationsproviderbase.SatelliteID
        LOGGER.debug("l_Mission: "+l_Mission)
        # Try to load the necessary information before
        # Load the L1 header product test if muscate or EarthExplorer
        if self.l1imageinformationsproviderbase.PluginName != self.plugin.PluginName:
            self.l2imagefilenamesprovider = L2ImageFilenamesProvider()
            self.l2imagefilenamesprovider.initialize(self.l1imageinformationsproviderbase,
                                                       self.plugin.ListOfL2Resolutions,
                                                self.outputdirectory, False)  # Check existence

        l_ReferenceProductHeaderId = ""
        l_ReferenceProductInstance = ""
        l_Validity_Start = ""
        l_Validity_Stop = ""
        l_Acquisition_Date = ""
        l_Nick_Name = ""
     
        # If the product is valid
        if self.productisvalid:
            # Write Headers ?
            if self.enablewriteearthexplorerheaders:
                # Writes the PRIVATE others headers
                self.write_private_others_xmlhandler(
                    self.outputl2privateimagefilenamesprovider,
                    l_ReferenceProductHeaderId,
                    l_ReferenceProductInstance,
                    self.dem,
                    l_Validity_Start,
                    l_Validity_Stop,
                    l_Mission,
                    root_template_directory,
                    lSchemaLocationDirectory,
                    lCurrentDate,
                    False)
                # Writes the PRIVATE others headers
                self.write_private_tec_xmlhandler(
                    self.outputl2privateimagefilenamesprovider.get_hdr_private_filename(),
                    l_ReferenceProductHeaderId,
                    l_Validity_Start,
                    l_Validity_Stop,
                    l_Mission,
                    l_Nick_Name,
                    l_Acquisition_Date,
                    root_template_directory,
                    lSchemaLocationDirectory,
                    lCurrentDate,
                    False)
            # Writes the PRIVATE STO header
            self.write_private_sto_xmlhandler(
                self.outputl2privateimagefilenamesprovider.get_sto_header_filename(),
                l_ReferenceProductHeaderId,
                l_ReferenceProductInstance,
                self.dem,
                l_Validity_Start,
                l_Validity_Stop,
                l_Mission,
                root_template_directory,
                lSchemaLocationDirectory,
                lCurrentDate,
                False)
            # ---------------------------------------------------------------------------------------------
            # Writes the PRIVATE reflectances headers (RCR, RTA, RTC)
            self.write_private_reflectances_xmlhandler(
                self.outputl2privateimagefilenamesprovider,
                l_ReferenceProductHeaderId,
                l_ReferenceProductInstance,
                self.dem,
                l_Validity_Start,
                l_Validity_Stop,
                l_Mission,
                root_template_directory,
                lSchemaLocationDirectory,
                lCurrentDate,
                False)
            # ---------------------------------------------------------------------------------------------------
            # Write LTC Private Header
            # Note necessary for Backward mode
            # If the LTC exist (AOT multi-temporal used
            LOGGER.debug("Write LTC: " + str(self.writeltc))
            if self.writeltc:
                # Get Solar and Viewing angles
                l_solarAngles =  [float(item) for item in list(self.l1imageinformationsproviderbase.SolarAngle.values())]
                l_viewingAngles = [float(item) for item in list(self.l1imageinformationsproviderbase.ViewingAngle.values())]
                # ---------------------------------------------------------------------------------------------------
                # Write LTC Handler
                self.write_private_ltc_xmlhandler(self.inputl2imagefilereader, l_ReferenceProductHeaderId,
                                                  l_ReferenceProductInstance,
                                                  os.path.join(root_template_directory,
                                                               self.plugin.TEMPLATE_PDTANX_PRIVATE_LTC_HDR),
                                                  self.outputl2privateimagefilenamesprovider,
                                                  lSchemaLocationDirectory, lCurrentDate,
                                                  self.l1imageinformationsproviderbase.UTCValidityStart,
                                                  self.l1imageinformationsproviderbase.UTCValidityStop,
                                                  l_solarAngles, l_viewingAngles, False)

        # Write the Job Processing Info
        l_JpiFilename = os.path.basename(l_ReferenceOutputL2ProductFilename)
        l_JpiFilename = l_JpiFilename.replace("MTD_ALL", "JPI_ALL")
        l_JpiFilename = os.path.join(l_DATADirectory, l_JpiFilename)
        LOGGER.debug("JobProcessingInformation Filename: '" + l_JpiFilename + "'")
        self.write_job_processing_information(
            root_template_directory +
            self.plugin.TEMPLATE_PDTANX_PRIVATE_HDR,
            l_JpiFilename,
            lSchemaLocationDirectory)
        # Start write the global header file
        self.write_global_xml_handler(root_template_directory + self.plugin.TEMPLATE_GLOBAL_HDR,
            l_ReferenceOutputL2ProductFilename, lSchemaLocationDirectory, l_JpiFilename, l_Mission)

    def write_global_xml_handler(self, root_sc_xml_templates, output_filename, schemaLocationDirectory,
                              pJpiFilename, p_Mission):
        # ---------------------------------------------------------------------------------------------
        l_CurrentPluginBase = self.plugin
        # Get global information about the output L2 product

        l_BandsDefinitions = l_CurrentPluginBase.BandsDefinitions
        l_ListOfL2Res = l_BandsDefinitions.ListOfL2Resolution
        l_NumberOfResolutions = len(l_ListOfL2Res)
        l_Muscate = self.l1imageinformationsproviderbase.MuscateData
        l_l1info = self.l1imageinformationsproviderbase


        # ---------------------------------------------------------------------------------------------------
        LOGGER.debug("Write the GLOBAL header file ...")
        current_header_filename = root_sc_xml_templates
        if not os.path.exists(current_header_filename):
            raise MajaExceptionPluginBase(
                "Internal error: the template file '" +
                current_header_filename +
                "' doesn't exist !!")
        # Load the file
        output_handler = MuscateXMLFileHandler(current_header_filename)
        LOGGER.debug("output_filename: " + output_filename)
        # Set the main header information
        l_RootBaseFilename = os.path.basename(output_filename).replace("_MTD_ALL.xml", "")
        # PRODUCT_ID : LANDSAT5-TM-XS_20100118-103000-000_L2A_EU93066200A00B_C_V1-0
        if "Node_MetadataFormat" in l_Muscate:
            output_handler.replace_node(
                l_Muscate["Node_MetadataFormat"],
                "//METADATA_FORMAT",
                "//Metadata_Identification/METADATA_FORMAT")
        output_handler.set_value_of("ProductId",l_RootBaseFilename)

        if "ZoneGeo" in l_Muscate:
            l_Site = l_Muscate["ZoneGeo"]
        else:
            l_Site = l_l1info.Site.strip('_')
            l_tileMatch = re.search('^[0-9]{2}[A-Z]{3}$',l_Site)
            if l_tileMatch is not None:
              l_Site = 'T'+l_Site

        # Node Dataset_Identification
        l_identifier = l_l1info.get_l2_identifier()
        output_handler.set_value_of("Identifier",l_identifier)
        if len(l_l1info.Instrument):
            output_handler.set_value_of("Instrument",l_l1info.Instrument)
        if len(l_l1info.SpectralContent):
            specContent = l_l1info.SpectralContent.replace("XSTHPAN", "XS").replace("XSTH", "XS")
            output_handler.set_value_of("SpectralContent",specContent)

        if "Authority" in l_Muscate:
            output_handler.set_value_of("Authority", l_Muscate["Authority"])
        else:
            output_handler.set_value_of("Authority", "THEIA")

        if "Producer" in l_Muscate:
            output_handler.set_value_of("Producer", l_Muscate["Producer"])
        else:
            output_handler.set_value_of("Producer", "MUSCATE")

        if "Project" in l_Muscate:
            output_handler.set_value_of("Project", l_Muscate["Project"])
        else:
            output_handler.set_value_of("Project", "SENTINEL2")

        output_handler.set_value_of("ZoneGeo", l_Site)

        # Detect the type of site
        l_siteType = "Site"
        if re.search("^[0-9]{3}-[0-9]{3}$",l_Site) is not None:
            l_siteType = "Path-Row"
        elif re.search("^T[0-9]{2}[A-Z]{3}$",l_Site) is not None:
            l_siteType = "Tile"
        xml_tools.set_attribute(
            output_handler.root, "//GEOGRAPHICAL_ZONE", "type", l_siteType)

        # if ORIGINAL_DATA_DIFFUSER is available
        if "Node_OriginalDataDiffuser" in l_Muscate:
            output_handler.replace_node(l_Muscate["Node_OriginalDataDiffuser"],
                "//ORIGINAL_DATA_DIFFUSER")

        # Node Product_Characteristics
        l_sensingTime = date_utils.get_utc_from_datetime(l_l1info.ProductDate)[4:] \
            + "." \
            + date_utils.get_date_millisecs_from_tm(l_l1info.ProductDate) \
            + "Z"
        if "AcquisitionDate" in l_Muscate:
            output_handler.set_value_of("AcquisitionDate", l_Muscate["AcquisitionDate"])
        else:
            output_handler.set_value_of("AcquisitionDate", l_sensingTime)

        output_handler.set_value_of("ProductionDate",l_l1info.GenerationDateStr)

        output_handler.set_value_of("ProductVersion", l_l1info.ProductVersion)

        output_handler.set_value_of("Platform", p_Mission)
        
        if "UTCAcquisitionRangeMean" in l_Muscate:
            output_handler.set_value_of("UTCAcquisitionRangeMean", l_Muscate["UTCAcquisitionRangeMean"])
        else:
            output_handler.set_value_of("UTCAcquisitionRangeMean", l_sensingTime)

        if "UTCAcquisitionRangeDatePrecision" in l_Muscate:
            output_handler.set_value_of("UTCAcquisitionRangeDatePrecision", l_Muscate["UTCAcquisitionRangeDatePrecision"])
        else:
            output_handler.set_value_of("UTCAcquisitionRangeDatePrecision", "0.001")

        output_handler.set_value_of("OrbitNumber",l_l1info.OrbitNumber)

        # Writes the filename
        # QUICKLOOK
        xml_tools.set_attribute(
            output_handler.root,
            "//Muscate_Product/QUICKLOOK",
            "bands_id",
            self.quicklookredbandcode +
            "," +
            self.quicklookgreenbandcode +
            "," +
            self.quicklookbluebandcode)

        output_handler.set_string_value("//Muscate_Product/QUICKLOOK", l_RootBaseFilename + "_QKL_ALL.jpg")

        #        If not FRE, remove this node
        #        Else rename and set the values
        #        Image_MACC_template_temporary_node_Flat_Reflectance
        # MACCS 4.7.2 - correction pour FA 1572
        # MACCS 4.7.4 : LAIG-FA-MAC-1623-CNES : write always PDV directory
        l_PublicDirectory = os.path.abspath(os.path.join(output_filename, os.pardir))
        LOGGER.debug("Output public directory : " + l_PublicDirectory)
        # Write Private_Directory only if present on the disk
        # MACCS 5.1.0 : LAIG-FA-MAC-1553-CNES : check the existence of the private directory.
        l_PostFix_PrivateDirectoryFilename = "DATA/" + l_RootBaseFilename + "_PVD_ALL"
        l_PrivateDirectoryFilename = l_PublicDirectory + "/" + l_PostFix_PrivateDirectoryFilename
        if os.path.exists(l_PrivateDirectoryFilename) and os.path.isdir(l_PrivateDirectoryFilename):
            output_handler.set_value_of("PrivateDirectoryFilename", l_PostFix_PrivateDirectoryFilename)
        else:
            # Remove the Xml node containing the Private directory
            LOGGER.debug("The private directory <" + l_PostFix_PrivateDirectoryFilename +
                         "> doesn't exist. Remove the dedicated XML node in the XML header filename.")
            output_handler.remove_node(output_handler.get_the_xpath_root_of_the_private_directory_filename())

        for field in ['Solar_Angles_Grid', 'Useful_Image_Informations_File', 'Viewing_Angles_Grid']:
          # . Test de la presence du champs
          nodeName = "Node_"+field
          if nodeName in l_Muscate:
              xml_tools.copies_to_child(l_Muscate[nodeName], "Data", output_handler.root, "//Data_List" )
              l_XPathRoot_In = "//Data_List/Data[Data_Properties/NATURE='"+field+"']/Data_File_List/DATA_FILE"
              xnodes_in = xml_tools.get_all_values(output_handler.root, l_XPathRoot_In)
              l_L2OutputDataDirectoryFilename = os.path.join(l_PublicDirectory, "DATA")
              file_utils.create_directory(l_L2OutputDataDirectoryFilename)
              # Get the path in the xml product filename
              lPath = os.path.dirname(l_l1info.ProductFileName)
              for node in xnodes_in:
                  l_FullPathFilename = os.path.join(lPath, node.text)
                  LOGGER.debug("l_FullPathFilename: " + l_FullPathFilename)
                  l_ShortFileName = os.path.basename(l_FullPathFilename)
                  LOGGER.debug("l_ShortFileName: " + l_ShortFileName)
                  l_OutputFilename = l_ShortFileName.replace("_L1C_", "_L2A_")
                  node.text = os.path.join("DATA", l_OutputFilename)
                  # Copy the dedicated file
                  file_utils.copy_file(l_FullPathFilename,
                                       os.path.join(l_L2OutputDataDirectoryFilename, l_OutputFilename))
          else:
              LOGGER.debug("No '"+field+"' data detected in the L1 product.")


        # Resolution information
        l_grpSuffixes = l_ListOfL2Res
        if len(l_grpSuffixes) == 1:
            l_grpSuffixes = ["XS"]
        # ---------------------------------------------------------------------------------------------
        # . Test de la presence du champs  'Useful_Image'
        if "Node_Useful_Image" in l_Muscate:
            LOGGER.debug("The L1 product have 'Useful_Image' files. They are copied in the L2 out product...")
            xml_tools.copies_to_child(l_Muscate["Node_Useful_Image"], "Mask", output_handler.root, "//Mask_List" )
            for l_grpSuffix in l_grpSuffixes:
                l_XPathRoot_In = "//Mask_List/Mask[Mask_Properties/NATURE='Useful_Image']/Mask_File_List/MASK_FILE[@group_id='"+l_grpSuffix+"']"
                xnode_in = xml_tools.get_only_value(output_handler.root, l_XPathRoot_In)
                l_L2OutputMaskDirectoryFilename = os.path.join(l_PublicDirectory, "MASKS")
                file_utils.create_directory(l_L2OutputMaskDirectoryFilename)
                # Get the path in the xml product filename
                lPath = os.path.dirname(l_l1info.ProductFileName)
                l_FullPathFilename = os.path.join(lPath, xnode_in.text)
                LOGGER.debug("l_FullPathFilename: " + l_FullPathFilename)
                l_OutputFilename =  l_RootBaseFilename + "_USI_" + l_grpSuffix + ".tif"
                xnode_in.text = os.path.join("MASKS", l_OutputFilename)
                # Copy/resampling done by MuscateL2ImageWriter
        else:
            LOGGER.debug("No 'Useful_Image' mask detected in the L1 product.")
        # Fin si manage USI


        if self.writel2products:
            # Resolution information
            for resol in range(l_NumberOfResolutions):
                l_grpSuffix = l_grpSuffixes[resol]
                l_StrResolution = l_BandsDefinitions.ListOfL2Resolution[resol]
                l_ListOfBand = l_BandsDefinitions.get_list_of_l2_band_code(l_StrResolution)
                l_NumberOfBands = len(l_ListOfBand)
                for i in range(l_NumberOfBands):
                    output_handler.set_string_value(
                        "//Image[Image_Properties/NATURE='Surface_Reflectance']/Image_File_List/IMAGE_FILE[@band_id='" +
                        l_ListOfBand[i] + "']", l_RootBaseFilename + "_SRE_" + l_ListOfBand[i] + ".tif")
                if self.adjacencyeffectsandslopecorrection:
                    for i in range(l_NumberOfBands):
                        output_handler.set_string_value(
                            "//Image_MACC_template_temporary_node_Flat_Reflectance[Image_Properties/NATURE='Flat_Reflectance']/Image_File_List/IMAGE_FILE[@band_id='" +
                            l_ListOfBand[i] +
                            "']",
                            l_RootBaseFilename +
                            "_FRE_" +
                            l_ListOfBand[i] +
                            ".tif")
                for i in range(l_NumberOfBands):
                    output_handler.set_string_value(
                        "//Image[Image_Properties/NATURE='Surface_Reflectance']/Image_File_List/IMAGE_FILE[@band_id='"
                        + l_ListOfBand[i] + "']", l_RootBaseFilename + "_SRE_" + l_ListOfBand[i] + ".tif")
                output_handler.set_string_value(
                    "//Image[Image_Properties/NATURE='Water_Vapor_Content']/Image_File_List/IMAGE_FILE[@group_id='"
                    + l_grpSuffix + "']", l_RootBaseFilename + "_ATB_" + l_grpSuffix + ".tif")
                output_handler.set_string_value(
                    "//Image[Image_Properties/NATURE='Aerosol_Optical_Thickness']/Image_File_List/IMAGE_FILE[@group_id='"
                    + l_grpSuffix + "']", l_RootBaseFilename + "_ATB_" + l_grpSuffix + ".tif")
                # ---------------------------------------------------------------------------------------------------
                # Write WVC_Interpolation
                # Only for driver where WaterVapourDetermination is activated
                output_handler.set_string_value(
                    "//Mask_List/Mask[Mask_Properties/NATURE='WVC_Interpolation']/Mask_File_List/MASK_FILE[@group_id='"
                    + l_grpSuffix + "']", "MASKS/" + l_RootBaseFilename + "_IAB_" + l_grpSuffix + ".tif")
                # ---------------------------------------------------------------------------------------------------
                # Write AOT_Interpolation
                output_handler.set_string_value(
                    "//Mask_List/Mask[Mask_Properties/NATURE='AOT_Interpolation']/Mask_File_List/MASK_FILE[@group_id='"
                    + l_grpSuffix + "']", "MASKS/" + l_RootBaseFilename + "_IAB_" + l_grpSuffix + ".tif")
                # ---------------------------------------------------------------------------------------------------
                # Write Detailed Cloud
                output_handler.set_string_value(
                    "//Mask_List/Mask[Mask_Properties/NATURE='Detailed_Cloud']/Mask_File_List/MASK_FILE[@group_id='"
                    + l_grpSuffix + "']", "MASKS/" + l_RootBaseFilename + "_CLM_" + l_grpSuffix + ".tif")
                # ---------------------------------------------------------------------------------------------------
                # Write Edge
                output_handler.set_string_value(
                    "//Mask_List/Mask[Mask_Properties/NATURE='Edge']/Mask_File_List/MASK_FILE[@group_id='"
                    + l_grpSuffix + "']", "MASKS/" + l_RootBaseFilename + "_EDG_" + l_grpSuffix + ".tif")
                # ---------------------------------------------------------------------------------------------------
                # Write Saturation
                for i in range(l_NumberOfBands):
                    output_handler.set_string_value(
                        "//Mask_List/Mask[Mask_Properties/NATURE='Saturation']/Mask_File_List/MASK_FILE[@band_id='"
                        + l_ListOfBand[i] + "']", "MASKS/" + l_RootBaseFilename + "_SAT_" + l_grpSuffix + ".tif")
                # ---------------------------------------------------------------------------------------------------
                # Write Water
                output_handler.set_string_value(
                    "//Mask_List/Mask[Mask_Properties/NATURE='Water']/Mask_File_List/MASK_FILE[@group_id='"
                    + l_grpSuffix + "']", "MASKS/" + l_RootBaseFilename + "_MG2_" + l_grpSuffix + ".tif")
                # ---------------------------------------------------------------------------------------------------
                # Write Cloud
                output_handler.set_string_value(
                    "//Mask_List/Mask[Mask_Properties/NATURE='Cloud']/Mask_File_List/MASK_FILE[@group_id='"
                    + l_grpSuffix + "']", "MASKS/" + l_RootBaseFilename + "_MG2_" + l_grpSuffix + ".tif")
                # ---------------------------------------------------------------------------------------------------
                # Write Snow
                if self.l1_plugin.SnowMasking:
                    output_handler.set_string_value(
                        "//Mask_List/Mask[Mask_Properties/NATURE='Snow']/Mask_File_List/MASK_FILE[@group_id='"
                        + l_grpSuffix + "']", "MASKS/" + l_RootBaseFilename + "_MG2_" + l_grpSuffix + ".tif")
                # ---------------------------------------------------------------------------------------------------
                # Write Cloud Shadow
                output_handler.set_string_value(
                    "//Mask_List/Mask[Mask_Properties/NATURE='Cloud_Shadow']/Mask_File_List/MASK_FILE[@group_id='"
                    + l_grpSuffix + "']", "MASKS/" + l_RootBaseFilename + "_MG2_" + l_grpSuffix + ".tif")
                # ---------------------------------------------------------------------------------------------------
                # Write Topography_Shadow
                output_handler.set_string_value(
                    "//Mask_List/Mask[Mask_Properties/NATURE='Topography_Shadow']/Mask_File_List/MASK_FILE[@group_id='"
                    + l_grpSuffix + "']", "MASKS/" + l_RootBaseFilename + "_MG2_" + l_grpSuffix + ".tif")
                # ---------------------------------------------------------------------------------------------------
                # Write Hidden_Surface
                output_handler.set_string_value(
                    "//Mask_List/Mask[Mask_Properties/NATURE='Hidden_Surface']/Mask_File_List/MASK_FILE[@group_id='"
                    + l_grpSuffix + "']", "MASKS/" + l_RootBaseFilename + "_MG2_" + l_grpSuffix + ".tif")
                # ---------------------------------------------------------------------------------------------------
                # Write Sun_Too_Low
                output_handler.set_string_value(
                    "//Mask_List/Mask[Mask_Properties/NATURE='Sun_Too_Low']/Mask_File_List/MASK_FILE[@group_id='"
                    + l_grpSuffix + "']", "MASKS/" + l_RootBaseFilename + "_MG2_" + l_grpSuffix + ".tif")
                # ---------------------------------------------------------------------------------------------------
                # Write Tangent_Sun
                output_handler.set_string_value(
                    "//Mask_List/Mask[Mask_Properties/NATURE='Tangent_Sun']/Mask_File_List/MASK_FILE[@group_id='"
                    + l_grpSuffix + "']", "MASKS/" + l_RootBaseFilename + "_MG2_" + l_grpSuffix + ".tif")
                # ---------------------------------------------------------------------------------------------------
                # Write DFP
                l_DFPMaskAvailable = self.l1_plugin.DFPMasking
                if l_DFPMaskAvailable:
                    for i in range(l_NumberOfBands):
                        output_handler.set_string_value(
                            "//Mask_List/Mask[Mask_Properties/NATURE='Defective_Pixel']/Mask_File_List/MASK_FILE[@band_id='"
                            + l_ListOfBand[i] + "']", "MASKS/" + l_RootBaseFilename + "_DFP_" + l_grpSuffix + ".tif")
                else:
                    output_handler.remove_node("//Mask_List/Mask[Mask_Properties/NATURE='Defective_Pixel']")
                # ---------------------------------------------------------------------------------------------
                # LAIG-DM-MAC-2003-CNES : recopie des fichiers detfoot print
                # Uniquement pour Sentinel2 Muscate
                # . Test de la presence du champs  'Detector_Footprint'
                if "ZoneMaskFileNames" in l_Muscate:
                    LOGGER.debug("The L1 product have 'Detector_Footprint' masks. There are copied in the L2 out product...")
                    # Write the Detfoo in the Xml file, in the MASK Detector_Footprint node !
                    l_XPathRootDetFoot = "//Mask_List/Mask[Mask_Properties/NATURE='Detector_Footprint']/Mask_File_List"
                    xnode = xml_tools.get_only_value(output_handler.root, l_XPathRootDetFoot)
                    main_node = xnode
                    l_L2OutputMaskDirectoryFilename = os.path.join(l_PublicDirectory, "MASKS")
                    file_utils.create_directory(l_L2OutputMaskDirectoryFilename)
                    for i in range(l_NumberOfBands):
                        l1BandId = l_BandsDefinitions.get_band_id_in_l1(l_ListOfBand[i])
                        l_MapBandDetFnames = l_Muscate["ZoneMaskFileNames"][l1BandId]
                        for det, filename in l_MapBandDetFnames.items():
                            l_FullPathFilename = filename
                            LOGGER.debug("l_FullPathFilename: " + l_FullPathFilename)
                            l_ShortFileName = os.path.basename(l_FullPathFilename)
                            LOGGER.debug("l_ShortFileName: " + l_ShortFileName)
                            # Write inthe XML file
                            # attributes: band_id="B1" detector_id="01"
                            elt = ET.Element("MASK_FILE")
                            elt.set("band_id", l_ListOfBand[i])
                            elt.set("detector_id", det)
                            oss = str(i + 1)
                            elt.set("bit_number", oss)
                            elt.text = os.path.join("MASKS", l_ShortFileName.replace("_L1C_", "_L2A_"))
                            main_node.append(elt)
                            # Copy the dedicated file
                            l_OutputFilename = l_ShortFileName.replace("_L1C_", "_L2A_")
                            file_utils.copy_file(
                                l_FullPathFilename, os.path.join(
                                    l_L2OutputMaskDirectoryFilename, l_OutputFilename))
                else:
                    LOGGER.debug("No 'Detector_Footprint' masks detected in the L1 product.")
                    #Remove the node
                    xml_tools.remove_node(output_handler.root, "//Mask_List/Mask[Mask_Properties/NATURE='Detector_Footprint']")

                # Fin si manage Detfoo

                # ---------------------------------------------------------------------------------------------
                # . Test de la presence du champs  'Aberrant_Pixels'
                if ("PIXImages" in l_Muscate) and ("PIXIndices" in l_Muscate):
                    LOGGER.debug("The L1 product have 'Aberrant_Pixels' masks. There are copied in the L2 out product...")
                    # Write the PIX in the Xml file, in the MASK Aberrant_Pixels node !
                    l_XPathRootPIX = "//Mask_List/Mask[Mask_Properties/NATURE='Aberrant_Pixels']/Mask_File_List"
                    xnode = xml_tools.get_only_value(output_handler.root, l_XPathRootPIX)
                    main_node = xnode
                    l_L2OutputMaskDirectoryFilename = os.path.join(l_PublicDirectory, "MASKS")
                    file_utils.create_directory(l_L2OutputMaskDirectoryFilename)
                    for i in range(l_NumberOfBands):
                        l1BandId = l_BandsDefinitions.get_band_id_in_l1(l_ListOfBand[i])
                        l_FullPathFilename = l_Muscate["PIXImages"][l1BandId]
                        LOGGER.debug("l_FullPathFilename: " + l_FullPathFilename)
                        l_ShortFileName = os.path.basename(l_FullPathFilename)
                        LOGGER.debug("l_ShortFileName: " + l_ShortFileName)
                        # Write inthe XML file
                        # attributes: band_id="B1" "bit_number"
                        elt = ET.Element("MASK_FILE")
                        elt.set("band_id", l_ListOfBand[i])
                        oss = l_Muscate["PIXIndices"][l1BandId]
                        elt.set("bit_number", str(oss))
                        l_OutputFilename = l_ShortFileName.replace("_L1C_", "_L2A_")
                        elt.text = os.path.join("MASKS", l_OutputFilename)
                        main_node.append(elt)
                else:
                    LOGGER.debug("No 'Aberrant_Pixels' masks detected in the L1 product.")
                    # remove node if present
                    xml_tools.remove_node(output_handler.root,"//Mask_List/Mask[Mask_Properties/NATURE='Aberrant_Pixels']")

                # Fin si manage PIX
        else:
            # Generating default 1000x1000px black quicklook
            root_template_directory = self.apphandler.get_share_config_directory()
            LOGGER.debug("QCK template '" + root_template_directory + self.plugin.TEMPLATE_PDTQKL_JPG + "'")
            LOGGER.debug("QCK dst fname '" + l_PublicDirectory + "/" + l_RootBaseFilename + "_QKL_ALL.jpg'")
            file_utils.copy_file(root_template_directory + self.plugin.TEMPLATE_PDTQKL_JPG,
                                 l_PublicDirectory + "/" + l_RootBaseFilename + "_QKL_ALL.jpg")
            # Resolution information
            for resol in range(l_NumberOfResolutions):
                l_grpSuffix = l_grpSuffixes[resol]
                l_StrResolution = l_BandsDefinitions.ListOfL2Resolution[resol]
                l_ListOfBand = l_BandsDefinitions.get_list_of_l2_band_code(l_StrResolution)
                l_NumberOfBands = len(l_ListOfBand)
                for i in range(l_NumberOfBands):
                    output_handler.set_string_value(
                        "//Image[Image_Properties/NATURE='Surface_Reflectance']/Image_File_List/IMAGE_FILE[@band_id='" +
                        l_ListOfBand[i] + "']", "")
                if self.adjacencyeffectsandslopecorrection:
                    for i in range(l_NumberOfBands):
                        output_handler.set_string_value(
                            "//Image_MACC_template_temporary_node_Flat_Reflectance[Image_Properties/NATURE='Flat_Reflectance']/Image_File_List/IMAGE_FILE[@band_id='" +
                            l_ListOfBand[i] +
                            "']",
                            "")
                for i in range(l_NumberOfBands):
                    output_handler.set_string_value(
                        "//Image[Image_Properties/NATURE='Surface_Reflectance']/Image_File_List/IMAGE_FILE[@band_id='" +
                        l_ListOfBand[i] + "']", "")

                output_handler.set_string_value(
                    "//Image[Image_Properties/NATURE='Water_Vapor_Content']/Image_File_List/IMAGE_FILE[@group_id='" +
                    l_grpSuffix + "']", "")
                output_handler.set_string_value(
                    "//Image[Image_Properties/NATURE='Aerosol_Optical_Thickness']/Image_File_List/" +
                    "IMAGE_FILE[@group_id='" + l_grpSuffix + "']", "")
                # Write Cloud
                l_Nature = "Cloud"
                # Get the parent node
                l_CLDDataBandsSelected = self.plugin.CLDDataBandsSelected
                l_NbCLDDataBandsSelected = len(l_CLDDataBandsSelected)
                xnode = xml_tools.get_only_value(
                    output_handler.root,
                    "//Mask_List/Mask[Mask_Properties/NATURE='" + l_Nature + "']/Mask_File_List")
                main_node = xnode
                for i in l_CLDDataBandsSelected:
                    l_Group = l_grpSuffix
                    elt = ET.Element("MASK_FILE")
                    elt.set("group_id", l_Group)
                    elt.text = ""
                    main_node.append(elt)

                # Write Water
                output_handler.set_string_value(
                    "//Mask_List/Mask[Mask_Properties/NATURE='Water']/Mask_File_List/MASK_FILE[@group_id='" + l_grpSuffix + "']", "")
                # ---------------------------------------------------------------------------------------------------
                # Write Hidden_Surface
                output_handler.set_string_value(
                    "//Mask_List/Mask[Mask_Properties/NATURE='Hidden_Surface']/Mask_File_List/MASK_FILE[@group_id='" + l_grpSuffix + "']", "")
                # ---------------------------------------------------------------------------------------------------
                # Write Topography_Shadow
                output_handler.set_string_value(
                    "//Mask_List/Mask[Mask_Properties/NATURE='Topography_Shadow']/Mask_File_List/MASK_FILE[@group_id='" +
                    l_grpSuffix +
                    "']",
                    "")
                # ---------------------------------------------------------------------------------------------------
                # Write Sun_Too_Low
                output_handler.set_string_value(
                    "//Mask_List/Mask[Mask_Properties/NATURE='Sun_Too_Low']/Mask_File_List/MASK_FILE[@group_id='" + l_grpSuffix + "']", "")
                # ---------------------------------------------------------------------------------------------------
                # Write Tangent_Sun
                output_handler.set_string_value(
                    "//Mask_List/Mask[Mask_Properties/NATURE='Tangent_Sun']/Mask_File_List/MASK_FILE[@group_id='" + l_grpSuffix + "']", "")
                # ---------------------------------------------------------------------------------------------------
                # Write Snow
                if self.l1_plugin.SnowMasking:
                    output_handler.set_string_value(
                            "//Mask_List/Mask[Mask_Properties/NATURE='Snow']/Mask_File_List/MASK_FILE[@group_id='" + l_grpSuffix + "']", "")
                #---------------------------------------------------------------------------------------------------

                # Write Saturation
                l_Nature = "Saturation"
                for b in l_ListOfBand:
                    output_handler.set_string_value(
                        "//Mask_List/Mask[Mask_Properties/NATURE='" +
                        l_Nature +
                        "']/Mask_File_List/MASK_FILE[@band_id='" +
                        b +
                        "']",
                        "")
                # ---------------------------------------------------------------------------------------------------
                # Write Edge
                output_handler.set_string_value(
                    "//Mask_List/Mask[Mask_Properties/NATURE='Edge']/Mask_File_List/MASK_FILE[@group_id='" + l_grpSuffix + "']", "")
                # ---------------------------------------------------------------------------------------------------
                # Write AOT_Interpolation
                output_handler.set_string_value(
                    "//Mask_List/Mask[Mask_Properties/NATURE='AOT_Interpolation']/Mask_File_List/MASK_FILE[@group_id='" +
                    l_grpSuffix +
                    "']",
                    "")
                # ---------------------------------------------------------------------------------------------------
                # Write WVC_Interpolation
                # Only for driver where WaterVapourDetermination is activated
                if self.l1_plugin.WaterVapourDetermination:
                    output_handler.set_string_value(
                        "//Mask_List/Mask[Mask_Properties/NATURE='WVC_Interpolation']/Mask_File_List/MASK_FILE[@group_id='" +
                        l_grpSuffix +
                        "']",
                        "")
                # ---------------------------------------------------------------------------------------------
                if "ZoneMaskFileNames" in l_Muscate:
                    LOGGER.debug("The L1 product have 'Detector_Footprint' masks. There are copied in the L2 out product...")
                    # Write the Detfoo in the Xml file, in the MASK Detector_Footprint node !
                    l_XPathRootDetFoot = "//Mask_List/Mask[Mask_Properties/NATURE='Detector_Footprint']/Mask_File_List"
                    xnode = xml_tools.get_only_value(output_handler.root, l_XPathRootDetFoot)
                    main_node = xnode

                    for bd in range(l_NumberOfBands):                        

                        l1BandId = l_BandsDefinitions.get_band_id_in_l1(l_ListOfBand[bd])
                        l_MapBandDetFnames = l_Muscate["ZoneMaskFileNames"][l1BandId]
                        for it in list(l_MapBandDetFnames.values()):
                            # Write inthe XML file
                            elt = ET.Element("MASK_FILE")
                            elt.set("band_id", l_ListOfBand[bd])
                            elt.set("detector_id", it)
                            elt.text = ""
                            main_node.append(elt)


                # Fin si manage Detfoo

        if self.adjacencyeffectsandslopecorrection:
            LOGGER.debug("Renamed child Product_Organisation/Muscate_Product/Image_List/Image_MACC_template_temporary_node_Flat_Reflectance by Image")
            xml_tools.rename_node(
                output_handler.root,
                "//Product_Organisation/Muscate_Product/Image_List/Image_MACC_template_temporary_node_Flat_Reflectance",
                "Image")
        else:
            LOGGER.debug("Removed Product_Organisation/Muscate_Product/Image_List/Image_MACC_template_temporary_node_Flat_Reflectance child")
            xml_tools.remove_node(output_handler.root, "//Product_Organisation/Muscate_Product/Image_List/" +
                                   "Image_MACC_template_temporary_node_Flat_Reflectance")

        if not self.writel2products:
            # DM2706
            xml_tools.remove_child(output_handler.root, "//Product_Organisation/Muscate_Product/Image_List")
            xml_tools.remove_child(output_handler.root, "//Product_Organisation/Muscate_Product/Mask_List")

        # Write Private_Directory
        output_handler.set_value_of("JobProcessingInformationsFileFilename", "DATA/" + os.path.basename(pJpiFilename))

        #                <Image>
        #             <Image_Properties>
        #                 <NATURE>Aerosol_Optical_Thickness</NATURE>
        #                 <FORMAT>image/tiff</FORMAT>
        #                 <ENCODING>byte</ENCODING>
        #                 <ENDIANNESS>LittleEndian</ENDIANNESS>
        #                 <COMPRESSION>None</COMPRESSION>
        #             </Image_Properties>
        #             <Image_File_List>
        #                 <IMAGE_FILE group_id="ALL">NOT_INITIALIZED LANDSAT5-TM-XS_20100118-103000-000_L2A_EU93066200A00B_C_V1-0_AOT_ALL.tif</IMAGE_FILE>
        #             </Image_File_List>
        #         </Image>
        #

        #        nodata
        #
        #        p_l1_input_handler
        #        <Radiometric_Informations>
        #               <Special_Values_List>
        #                   <SPECIAL_VALUE name="nodata">NOT_INITIALIZED -10000</SPECIAL_VALUE>
        #                   <SPECIAL_VALUE name="VAP_Nodata_Value">NOT_INITIALIZED 0</SPECIAL_VALUE>
        #                   <SPECIAL_VALUE name="VAP_Quantification_Value">NOT_INITIALIZED 0.05</SPECIAL_VALUE>
        #                   <SPECIAL_VALUE name="AOT_Nodata_Value">NOT_INITIALIZED 0</SPECIAL_VALUE>
        #                   <SPECIAL_VALUE name="AOT_Quantification_Value">NOT_INITIALIZED 0.005</SPECIAL_VALUE>

        #        output_handler.Set(p_l1_input_handler.Get())
        # No cross writing, copy directly the datas from the input file
        #~ if "Node_Geoposition_Informations" in l_Muscate:
            #~ output_handler.replace_node(l_Muscate["Node_Geoposition_Informations"], "//Geoposition_Informations")

        # ----------------------------------------------------------------------
        # Compute Geoposition_Informations
        l_Areas = self.dem.L2Areas
        proj_full = self.dem.ProjCode
        proj_table = proj_full
        proj_code = proj_full
        if proj_full.find(":") != -1 :
            proj_table = proj_full[:proj_full.find(":")]
            proj_code = proj_full[proj_full.find(":")+1:]
    
        output_handler.set_string_value("//Geoposition_Informations/Coordinate_Reference_System/GEO_TABLES", proj_table)
        output_handler.set_string_value(
                "//Geoposition_Informations/Coordinate_Reference_System/Horizontal_Coordinate_System/HORIZONTAL_CS_TYPE", self.dem.ProjType)
        proj_name = self.dem.ProjRef.split('"')[1]
        if "WGS84" in proj_name:
            proj_name = "WGS 84" + proj_name[proj_name.find("WGS84")+5:]
        
        output_handler.set_string_value(
                "//Geoposition_Informations/Coordinate_Reference_System/Horizontal_Coordinate_System/HORIZONTAL_CS_NAME", proj_name)
        output_handler.set_string_value(
                "//Geoposition_Informations/Coordinate_Reference_System/Horizontal_Coordinate_System/HORIZONTAL_CS_CODE", proj_code)
        output_handler.set_string_value(
                "//Geoposition_Informations/Raster_CS/RASTER_CS_TYPE", "CELL")
        output_handler.set_string_value(
                "//Geoposition_Informations/Raster_CS/PIXEL_ORIGIN", "0")
        output_handler.set_string_value(
                "//Geoposition_Informations/Metadata_CS/METADATA_CS_TYPE", "CELL")
        output_handler.set_string_value(
                "//Geoposition_Informations/Metadata_CS/PIXEL_ORIGIN", "0")
        
        l_isCarto = (self.dem.ProjType == "PROJECTED")

        # Corners
        l_spacing = PointXY(l_Areas[0].spacing[0], l_Areas[0].spacing[1])
        l_origin = PointXY(l_Areas[0].origin[0], l_Areas[0].origin[1])
        l_ul = l_origin - 0.5 * l_spacing
        l_vecX = PointXY(l_Areas[0].size[0] * l_Areas[0].spacing[0], 0.0)
        l_vecY = PointXY(0.0, l_Areas[0].size[1] * l_Areas[0].spacing[1])
        l_corners = {}
        l_corners["upperLeft"] = l_ul
        l_corners["upperRight"] = l_ul + l_vecX
        l_corners["lowerRight"] = l_ul + l_vecX + l_vecY
        l_corners["lowerLeft"] = l_ul + l_vecY
        l_corners["center"] = l_ul + 0.5 * ( l_vecX + l_vecY)

        l_geopositionPath = "//Geoposition_Informations/Geopositioning/Global_Geopositioning/Point[@name='{}']/{}"
        if l_isCarto:
            LOGGER.debug("Start Conversion ToWKT with the EPSG '" + proj_code + "'...")
            param_conv = {"carto.x": 0.0,
                          "carto.y": 0.0,
                          "mapproj": "epsg",
                          "mapproj.epsg.code": int(proj_code)
                          }
            for corner, point in l_corners.items():
                # Compute lon lat coordinates of the corner
                param_conv["carto.x"] = point.x
                param_conv["carto.y"] = point.y
                conv_app = OtbAppHandler("ConvertCartoToGeoPoint", param_conv)
                l_lon = conv_app.getoutput().get("long")
                l_lat = conv_app.getoutput().get("lat")
                # Set corner
                output_handler.set_string_value(
                    l_geopositionPath.format(corner,"LAT"),
                    f"{l_lat:.12f}")
                output_handler.set_string_value(
                    l_geopositionPath.format(corner,"LON"),
                    f"{l_lon:.12f}")
                output_handler.set_string_value(
                    l_geopositionPath.format(corner,"X"),
                    f"{point.x:.1f}")
                output_handler.set_string_value(
                    l_geopositionPath.format(corner,"Y"),
                    f"{point.y:.1f}")
        else:
            for corner, point in l_corners.items():
                # Set corner
                output_handler.set_string_value(
                    l_geopositionPath.format(corner,"LAT"),
                    f"{point.y:.12f}")
                output_handler.set_string_value(
                    l_geopositionPath.format(corner,"LON"),
                    f"{point.x:.12f}")
                output_handler.remove_node(
                    l_geopositionPath.format(corner,"X"))
                output_handler.remove_node(
                    l_geopositionPath.format(corner,"Y"))

        # Set the XS geopositioning
        l_groupGeopositioningPath = "//Geoposition_Informations/Geopositioning/Group_Geopositioning_List/Group_Geopositioning[@group_id='{}']/{}"
        for idx, res in enumerate(l_grpSuffixes):
            l_spacing = PointXY(l_Areas[idx].spacing[0], l_Areas[idx].spacing[1])
            l_ul = PointXY(l_Areas[idx].origin[0], l_Areas[idx].origin[1]) - 0.5 * l_spacing
            if l_isCarto:
                l_str_ulx = f"{l_ul.x:.1f}"
                l_str_uly = f"{l_ul.y:.1f}"
                l_str_xdim = f"{l_spacing.x:.0f}"
                l_str_ydim = f"{l_spacing.y:.0f}"
            else:
                l_str_ulx = f"{l_ul.x:.12f}"
                l_str_uly = f"{l_ul.y:.12f}"
                l_str_xdim = f"{l_spacing.x:.12f}"
                l_str_ydim = f"{l_spacing.y:.12f}"
            output_handler.set_string_value(
                l_groupGeopositioningPath.format(res, "ULX"),
                l_str_ulx)
            output_handler.set_string_value(
                l_groupGeopositioningPath.format(res, "ULY"),
                l_str_uly)
            output_handler.set_string_value(
                l_groupGeopositioningPath.format(res, "XDIM"),
                l_str_xdim)
            output_handler.set_string_value(
                l_groupGeopositioningPath.format(res, "YDIM"),
                l_str_ydim)
            output_handler.set_string_value(
                l_groupGeopositioningPath.format(res, "NROWS"),
                l_Areas[idx].size[1])
            output_handler.set_string_value(
                l_groupGeopositioningPath.format(res, "NCOLS"),
                l_Areas[idx].size[0])
        
        #----------------------------------------------------------------------
        if "Node_Geometric_Informations" in l_Muscate:
            output_handler.replace_node(l_Muscate["Node_Geometric_Informations"], "//Geometric_Informations")
        else:
            #Fill geometric info node
            l_msza = float(l_l1info.SolarAngle["sun_zenith_angle"])
            l_msaa = float(l_l1info.SolarAngle["sun_azimuth_angle"])
            l_msza = f"{l_msza:.12f}"
            l_msaa = f"{l_msaa:.12f}"
            output_handler.set_string_value(
                    "//Geometric_Informations/Mean_Value_List/Sun_Angles/ZENITH_ANGLE", l_msza)
            output_handler.set_string_value(
                    "//Geometric_Informations/Mean_Value_List/Sun_Angles/AZIMUTH_ANGLE", l_msaa)

            l_mvza = float(l_l1info.ViewingAngle["incidence_zenith_angle"])
            l_mvaa = float(l_l1info.ViewingAngle["incidence_azimuth_angle"])
            l_mvza = f"{l_mvza:.12f}"
            l_mvaa = f"{l_mvaa:.12f}"
            output_handler.set_string_value(
                    "//Geometric_Informations/Mean_Value_List/Incidence_Angles/ZENITH_ANGLE", l_mvza)
            output_handler.set_string_value(
                    "//Geometric_Informations/Mean_Value_List/Incidence_Angles/AZIMUTH_ANGLE", l_mvaa)
        
            #Fill Mean viewing angle list
            #Get one mean viewing angle node to copy it
            l_InvBandMap = {v: k for k, v in l_BandsDefinitions.L2CoarseBandMap.items()}
            l_MeanViewingAngles = l_l1info.ListOfViewingAnglesPerBandAtL2CoarseResolution
            l_PathMeanViewingAngles = "//Geometric_Informations/Mean_Value_List/Mean_Viewing_Incidence_Angle_List/Mean_Viewing_Incidence_Angle[@{}='{}']/{}"
            for bname, b in l_BandsDefinitions.L2CoarseBandMap.items():
                l_mvza = float(l_MeanViewingAngles[b]["incidence_zenith_angle"])
                l_mvaa = float(l_MeanViewingAngles[b]["incidence_azimuth_angle"])
        
                output_handler.set_string_value(
                    l_PathMeanViewingAngles.format("band_id" ,bname, "ZENITH_ANGLE"),
                    f"{l_mvza:.12f}")
                output_handler.set_string_value(
                    l_PathMeanViewingAngles.format("band_id" ,bname, "AZIMUTH_ANGLE"),
                    f"{l_mvaa:.12f}")

                # also set per-detector viewing angles (if present)
                if len(l_BandsDefinitions.DetectorMap):
                    det = str(l_BandsDefinitions.DetectorMap[b])
                    if len(det) < 2:
                        det = "0"+det
                    output_handler.set_string_value(
                        l_PathMeanViewingAngles.format("detector_id", det, "ZENITH_ANGLE"),
                        f"{l_mvza:.12f}")
                    output_handler.set_string_value(
                        l_PathMeanViewingAngles.format("detector_id", det, "AZIMUTH_ANGLE"),
                        f"{l_mvaa:.12f}")

            #Fill Sun angle grids
            if len(l_l1info.SolarAngleGrid["Azimuth"]):
                l_PathSunAnglesGrid = "//Geometric_Informations/Angles_Grids_List/Sun_Angles_Grids/"
                output_handler.set_string_value(l_PathSunAnglesGrid+"Azimuth/COL_STEP", l_l1info.SolarAngleGrid["ColStep"])
                output_handler.set_string_value(l_PathSunAnglesGrid+"Azimuth/ROW_STEP", l_l1info.SolarAngleGrid["RowStep"])
                output_handler.set_string_value(l_PathSunAnglesGrid+"Zenith/COL_STEP", l_l1info.SolarAngleGrid["ColStep"])
                output_handler.set_string_value(l_PathSunAnglesGrid+"Zenith/ROW_STEP", l_l1info.SolarAngleGrid["RowStep"])
                xml_tools.set_attribute(output_handler.root,l_PathSunAnglesGrid+"Azimuth", "step_unit", l_l1info.SolarAngleGrid["StepUnit"])
                xml_tools.set_attribute(output_handler.root,l_PathSunAnglesGrid+"Zenith", "step_unit", l_l1info.SolarAngleGrid["StepUnit"])
                
                output_handler.set_string_value(l_PathSunAnglesGrid+"Azimuth/Values_List", "")
                output_handler.set_string_value(l_PathSunAnglesGrid+"Zenith/Values_List", "")
    
                for line in l_l1info.SolarAngleGrid["Azimuth"]:
                    xml_tools.add_child(output_handler.root, l_PathSunAnglesGrid+"Azimuth/Values_List", "VALUES", line)
                for line in l_l1info.SolarAngleGrid["Zenith"]:
                    xml_tools.add_child(output_handler.root, l_PathSunAnglesGrid+"Zenith/Values_List", "VALUES", line)

            #Fill Viewing incidence angle grids
            l_templateBaseNode = xml_tools.get_only_value(output_handler.root,"//Viewing_Incidence_Angles_Grids_List", check=True)
            for grid in l_l1info.ViewingAngleGrids:
                if l_templateBaseNode is None:
                    raise MajaExceptionPluginBase("Missing node 'Viewing_Incidence_Angles_Grids_List' in dom")
                if "Band" in grid:
                    l_idxBand = int(grid["Band"])
                    l_bname = l_InvBandMap[l_idxBand]
                    if not l_idxBand == l_BandsDefinitions.L2CoarseBandMap[l_bname]:
                        raise MajaExceptionPluginBase("Unexpected band order! {} is not at index {}".format(l_bname, l_idxBand))
                    l_band_selector = "[@band_id='{}']".format(l_bname)
                else:
                    l_band_selector = ""
                    l_bname = ""
                l_strDet = grid["Detector"]
                l_str2Det = l_strDet
                if int(l_strDet)<10:
                    l_str2Det = "0"+l_strDet
                # get the node for band 'l_bname' (or create one)
                l_templateBandNode = xml_tools.get_only_value(l_templateBaseNode, "Band_Viewing_Incidence_Angles_Grids_List"+l_band_selector, check=True)
                if l_templateBandNode is None:
                    l_templateBandNode = ET.Element("Band_Viewing_Incidence_Angles_Grids_List")
                    if len(l_bname):
                        l_templateBandNode.set("band_id",l_bname)
                    l_templateBaseNode.append(l_templateBandNode)
                # get the node for band 'l_bname' and detector 'l_str2Det' (or create one)
                l_templateDetNode = xml_tools.get_only_value(l_templateBandNode, "Viewing_Incidence_Angles_Grids[@detector_id='{}']".format(l_str2Det), check=True)
                if l_templateDetNode is None:
                    l_templateDetNode = ET.Element("Viewing_Incidence_Angles_Grids")
                    l_templateDetNode.set("detector_id",l_str2Det)
                    l_templateBandNode.append(l_templateDetNode)
                #Fill Zenith
                l_templateZenithNode = ET.Element("Zenith")
                l_templateZenithNode.set("step_unit", grid["StepUnit"])
                l_templateZenithNode.set("values_unit", "deg")
                l_templateZenithColNode = ET.Element("COL_STEP")
                l_templateZenithColNode.text = grid["ColStep"]
                l_templateZenithRowNode = ET.Element("ROW_STEP")
                l_templateZenithRowNode.text = grid["RowStep"]
                l_ZenithNode = ET.Element("Values_List")
                for line in grid["Zenith"]:
                    xml_tools.add_child(l_ZenithNode, ".", "VALUES", line)
                l_templateZenithNode.append(l_templateZenithColNode)
                l_templateZenithNode.append(l_templateZenithRowNode)
                l_templateZenithNode.append(l_ZenithNode)
                l_templateDetNode.append(l_templateZenithNode)
                #Fill Azimuth
                l_templateAzimuthNode = ET.Element("Azimuth")
                l_templateAzimuthNode.set("step_unit", grid["StepUnit"])
                l_templateAzimuthNode.set("values_unit", "deg")
                l_templateAzimuthColNode = ET.Element("COL_STEP")
                l_templateAzimuthColNode.text = grid["ColStep"]
                l_templateAzimuthRowNode = ET.Element("ROW_STEP")
                l_templateAzimuthRowNode.text = grid["RowStep"]
                l_AzimuthNode = ET.Element("Values_List")
                for line in grid["Azimuth"]:
                    xml_tools.add_child(l_AzimuthNode, ".", "VALUES", line)
                l_templateAzimuthNode.append(l_templateAzimuthColNode)
                l_templateAzimuthNode.append(l_templateAzimuthRowNode)
                l_templateAzimuthNode.append(l_AzimuthNode)
                l_templateDetNode.append(l_templateAzimuthNode)

        # Replace Radiometric_Informations and patch dedicated values
        # (REFLECTANCE_QUANTIFICATION_VALUE, Special_Values_List)

        # ATENTION self.GetReflectanceQuantificationValue() is 0.001 (not 1000)
        # The Quantification valude node is 1000

        output_handler.set_value_of("QuantificationValue", int(1. / float(self.reflectancequantificationvalue)))
        output_handler.set_value_of("AerosolOpticalThicknessQuantificationValue", int(1. / float(self.aotquantificationvalue)))
        output_handler.set_value_of("WaterVaporContentQuantificationValue", int(1. / float(self.vapquantificationvalue)))
        output_handler.set_string_value("//Radiometric_Informations/Special_Values_List/SPECIAL_VALUE[@name='nodata']", self.nodatavalue)
        output_handler.set_string_value("//Radiometric_Informations/Special_Values_List/SPECIAL_VALUE[@name='water_vapor_content_nodata']",
                self.vapnodatavalue)

        output_handler.set_string_value(
            "//Radiometric_Informations/Special_Values_List/SPECIAL_VALUE[@name='water_vapor_content_nodata']",
            self.vapnodatavalue)
        output_handler.set_string_value(
            "//Radiometric_Informations/Special_Values_List/SPECIAL_VALUE[@name='aerosol_optical_thickness_nodata']",
            self.aotnodatavalue)

        #Spectral band informations
        if "Node_Spectral_Band_Informations_List" in l_Muscate:
            # Copy this node Spectral_Band_Informations_List
            output_handler.replace_node(
                l_Muscate["Node_Spectral_Band_Informations_List"],
                "Spectral_Band_Informations_List",
                "//Radiometric_Informations/Spectral_Band_Informations_List")
        else:
            l_l1bands = list(l_BandsDefinitions.L2CoarseBandMap.keys())
            for spec in l_l1info.SpectralInfo:
                l_b = spec["Band"]
                if l_b in l_l1bands:
                    l_l1bands.remove(l_b)
                else:
                    raise MajaExceptionPluginBase("Unexpected band id: "+str(l_b))
                l_baseXPath = "//Spectral_Band_Informations_List/Spectral_Band_Informations[@band_id='{}']".format(l_b)
                # Native coefficients
                for measure in ['PhysicalGain', 'LuminanceMax', 'LuminanceMin',
                                'QuantizeCalMax', 'QuantizeCalMin',
                                'RadianceAdd', 'RadianceMult',
                                'ReflectanceAdd', 'ReflectanceMult',
                                'Ak', 'PolarizationCoefficient']:
                    if measure in spec:
                        if isinstance(spec[measure], float):
                            value = f"{spec[measure]:.12f}"
                        else:
                            value = spec[measure]
                        output_handler.set_string_value(l_baseXPath+
                            "/Calibration_Coefficients_Lists/Native_Coefficients_List/COEFFICIENT[@name='{}']".format(measure),
                            value)
                # Solar irradiance
                if "SolarIrradiance" in spec:
                    output_handler.set_string_value(l_baseXPath+"/SOLAR_IRRADIANCE",spec["SolarIrradiance"])
                # Spatial resolution
                output_handler.set_string_value(l_baseXPath + "/SPATIAL_RESOLUTION",
                    l_BandsDefinitions.get_l1_resolution(l_BandsDefinitions.get_l1_resolution_for_band_code(l_b)))
                # Wavelength section
                if ("WavelengthMin" in spec) or ("WavelengthMax" in spec) or ("WavelengthCentral" in spec):
                    node_wavelength = xml_tools.get_only_value(output_handler.root, l_baseXPath + "/Wavelength")
                    if "WavelengthMin" in spec:
                        node_min = ET.Element("MIN")
                        node_min.text = spec["WavelengthMin"]
                        node_min.set("unit","nm")
                        node_wavelength.append(node_min)
                    if "WavelengthMax" in spec:
                        node_max = ET.Element("MAX")
                        node_max.text = spec["WavelengthMax"]
                        node_max.set("unit","nm")
                        node_wavelength.append(node_max)
                    if "WavelengthCentral" in spec:
                        node_cen = ET.Element("CENTRAL")
                        node_cen.text = spec["WavelengthCentral"]
                        node_cen.set("unit","nm")
                        node_wavelength.append(node_cen)
                # Spectral response section
                if ("ResponseStep" in spec) and ("ResponseValues" in spec):
                    node_response = xml_tools.get_only_value(output_handler.root, l_baseXPath + "/Spectral_Response")
                    node_step = ET.Element("STEP")
                    node_step.text = spec["ResponseStep"]
                    node_step.set("unit","nm")
                    node_response.append(node_step)
                    node_values = ET.Element("VALUES")
                    node_values.text = spec["ResponseValues"]
                    node_response.append(node_values)
            if len(l_l1bands):
                raise MajaExceptionPluginBase("Missing spectral information for bands: "+str(l_l1bands))

        #Update the SPATIAL_RESOLUTION nodes
        for b in l_BandsDefinitions.L2CoarseBandMap.keys():
            b_res = l_BandsDefinitions.get_l1_resolution_for_band_code(b)
            if not b_res in l_ListOfL2Res:
                continue
            res_idx = l_ListOfL2Res.index(b_res)
            l_SPATPath = "//Spectral_Band_Informations_List/Spectral_Band_Informations[@band_id='{}']/SPATIAL_RESOLUTION".format(b)
            node_spatial = xml_tools.get_only_value(output_handler.root, l_SPATPath)
            if l_isCarto:
                l_spatial_resolution_in = math.fabs(l_Areas[res_idx].spacing[0])
            else:
                l_ProductImageSizeY = float(l_Areas[res_idx].size[1])
                l_L1SizeY = float(l_l1info.AreaByResolution[res_idx].size[1])
                l_L1L2Ratio = l_ProductImageSizeY / l_L1SizeY
                LOGGER.debug("lIntL1L2Ratio: " + str(l_L1L2Ratio) + " computed with " + str(l_ProductImageSizeY) + " / " + str(l_L1SizeY))
                if l_L1L2Ratio == 0 :
                    raise MajaExceptionPluginMuscate("Internal error: the L1L2 ratio is null !!")
                l_spatial_resolution_in = int( int(node_spatial.text) / l_L1L2Ratio + 0.5)
            node_spatial.text = '%d' % (l_spatial_resolution_in)
        
        # Removing information from L1 relative to bands not available in L2 product
        output_handler.remove_node("//Group_Geopositioning_List/Group_Geopositioning[@group_id='PAN']")
        output_handler.remove_node("//Group_Geopositioning_List/Group_Geopositioning[@group_id='TH']")
        output_handler.remove_node("//Group_Geopositioning_List/Group_Geopositioning[@group_id='R3']")

        # Get the parent node
        l_L2RejectedBands = self.plugin.L2RejectedBands
        l_NbL2RejectedBands = len(l_L2RejectedBands)
        # Remove the not L2 bands
        for bd in l_L2RejectedBands:
            output_handler.remove_node(
                "//Spectral_Band_Informations_List/Spectral_Band_Informations[@band_id='" + bd + "']")
            output_handler.remove_node(
                "//Mean_Viewing_Incidence_Angle_List/Mean_Viewing_Incidence_Angle[@band_id='" + bd + "']")
            output_handler.remove_node(
                "//Viewing_Incidence_Angles_Grids_List/Band_Viewing_Incidence_Angles_Grids_List[@band_id='" + bd + "']")

        # Node Quality_Informations
        #   - Source_Product
        output_handler.set_string_value(
            "//Current_Product/Product_Quality_List/Product_Quality/Source_Product/PRODUCT_ID",
            output_handler.get_string_value_of("ProductId"))
        output_handler.set_string_value(
            "//Current_Product/Product_Quality_List/Product_Quality/Source_Product/ACQUISITION_DATE",
            output_handler.get_string_value_of("AcquisitionDate"))
        output_handler.set_string_value(
            "//Current_Product/Product_Quality_List/Product_Quality/Source_Product/PRODUCTION_DATE",
            output_handler.get_string_value_of("ProductionDate"))
        #   - Global_Index_List
        output_handler.set_string_value(
            "//Current_Product/Product_Quality_List[@level='N2']/Product_Quality/Global_Index_List/QUALITY_INDEX[@name='CirrusDetected']",
            self.cirrusflag, check_if_present=True)
        output_handler.set_string_value(
            "//Current_Product/Product_Quality_List[@level='N2']/Product_Quality/Global_Index_List/QUALITY_INDEX[@name='CloudPercent']",
            int(self.cloudrate))
        if self.l1_plugin.SnowMasking:
            output_handler.set_string_value(
                "//Current_Product/Product_Quality_List[@level='N2']/Product_Quality/Global_Index_List/QUALITY_INDEX[@name='SnowPercent']",
                int(self.snowrate))
        output_handler.set_string_value(
            "//Current_Product/Product_Quality_List[@level='N2']/Product_Quality/Global_Index_List/QUALITY_INDEX[@name='RainDetected']",
            self.rainflag)
        output_handler.set_string_value(
            "//Current_Product/Product_Quality_List[@level='N2']/Product_Quality/Global_Index_List/QUALITY_INDEX[@name='HotSpotDetected']",
            self.hotspotflag)
        output_handler.set_string_value(
            "//Current_Product/Product_Quality_List[@level='N2']/Product_Quality/Global_Index_List/QUALITY_INDEX[@name='SunGlintDetected']",
            self.sunglintflag)

        # Copy node <Product_Quality_List level="Geo">
        if "Node_Product_Quality_List_Geo" in l_Muscate:
            output_handler.replace_node(l_Muscate["Node_Product_Quality_List_Geo"],
                "Product_Quality_List",
                "//Current_Product/Product_Quality_List[@level='Geo']",
                check_if_present=True)
        else:
            output_handler.remove_node("//Current_Product/Product_Quality_List[@level='Geo']")

        # Copy node <Product_Quality_List level="Natif">
        if "Node_Product_Quality_List_Natif" in l_Muscate:
            output_handler.replace_node(l_Muscate["Node_Product_Quality_List_Natif"],
                "Product_Quality_List",
                "//Current_Product/Product_Quality_List[@level='Natif']",
                check_if_present=True)
        else:
            output_handler.remove_node("//Current_Product/Product_Quality_List[@level='Natif']")

        # Copy this node Processing_Jobs_List
        if "Node_Processing_Jobs_List" in l_Muscate:
            output_handler.replace_node(l_Muscate["Node_Processing_Jobs_List"],
                "Processing_Jobs_List",
                "//Production_Informations/Processing_Jobs_List",
                check_if_present=True)
        else:
            l_baseJobXpath = "//Production_Informations/Processing_Jobs_List/Job"
            l_workplan = "generique_l2_"+date_utils.get_date_yyyymmdd_from_tm(l_l1info.ProductDate)+"_01"
            output_handler.set_string_value(l_baseJobXpath + "/WORKPLAN_ID", l_workplan)
            output_handler.set_string_value(l_baseJobXpath + "/PRODUCER_NAME", "MUSCATE")
            output_handler.set_string_value(l_baseJobXpath + "/START_PRODUCTION_DATE", l_l1info.GenerationDateStr)
            output_handler.set_string_value(l_baseJobXpath + "/END_PRODUCTION_DATE", l_l1info.GenerationDateStr)
            output_handler.set_string_value(l_baseJobXpath + "/Products_List/Inputs_List/PRODUCT/PRODUCT_ID", l_l1info.ProductId)
            output_handler.set_string_value(l_baseJobXpath + "/Products_List/Inputs_List/PRODUCT/ACQUISITION_DATE",l_l1info.AcquisitionStart)
            output_handler.set_string_value(l_baseJobXpath + "/Products_List/Inputs_List/PRODUCT/PRODUCTION_DATE", l_l1info.GenerationDateStr)
            output_handler.set_string_value(l_baseJobXpath + "/Products_List/Outputs_List/PRODUCT/PRODUCT_ID", output_handler.get_string_value_of("ProductId"))
            output_handler.set_string_value(l_baseJobXpath + "/Products_List/Outputs_List/PRODUCT/ACQUISITION_DATE", l_l1info.AcquisitionStart)
            output_handler.set_string_value(l_baseJobXpath + "/Products_List/Outputs_List/PRODUCT/PRODUCTION_DATE",
                output_handler.get_string_value_of("AcquisitionDate"))

        output_handler.save_to_file(output_filename)
        # Check the GLOBAL header even if  not valid (NOTV)
        if self.checkxmlfileswithschema:
            xml_tools.check_xml(output_filename, schemaLocationDirectory)

    def write_job_processing_information(self, root_sc_xml_templates, output_filename, schemaLocationDirectory):
        # ---------------------------------------------------------------------------------------------
        l_TemplateJpiFilename = root_sc_xml_templates
        # ---------------------------------------------------------------------------------------------
        if not os.path.exists(l_TemplateJpiFilename):
            raise MajaExceptionPluginBase(
                "Internal error: the template file for the JPI filename '" +
                l_TemplateJpiFilename +
                "' doesn't exist !!")
        # ---------------------------------------------------------------------------------------------
        l_JpiXMLFileHandler = MuscateXMLFileHandler(l_TemplateJpiFilename)
        # ---------------------------------------------------------------------------------------------
        l_JpiXMLFileHandler.set_string_value("//Min", int(self.dem.ALT_Min))
        l_JpiXMLFileHandler.set_string_value("//Max", int(self.dem.ALT_Max))
        # ---------------------------------------------------------------------------------------------
        l_JpiXMLFileHandler.set_string_value("//Average", f"{self.dem.ALT_Mean:.2f}")
        l_JpiXMLFileHandler.set_string_value("//Standard_Deviation", f"{self.dem.ALT_Stdv:.2f}")
        # Write the cirrus correction flag if plugin has it
        if self.l1_plugin.CirrusMasking:
            l_JpiXMLFileHandler.set_value_of("ProcessingFlagsAndModesCirrusCorrection", self.cirruscorrection)
        # ---------------------------------------------------------------------------------------------
        l_JpiXMLFileHandler.set_value_of("ProcessingFlagsAndModesProcessingAdjacencyEffectsAndSlopeCorrection",
                                         self.adjacencyeffectsandslopecorrection)
        # ---------------------------------------------------------------------------------------------

        l_JpiXMLFileHandler.set_value_of("ProcessingFlagsAndModesCAMSProcessing", CAMSStatus.get_cams_status_string(self.camsstatus))

        if self.initmode:
            l_JpiXMLFileHandler.set_value_of("ProcessingFlagsAndModesProcessingMode", "INIT_MODE")
        elif self.backwardmode:
            l_JpiXMLFileHandler.set_value_of("ProcessingFlagsAndModesProcessingMode", "BACKWARD_MODE")
        else:
            l_JpiXMLFileHandler.set_value_of("ProcessingFlagsAndModesProcessingMode", "STANDARD_MODE")



        # ---------------------------------------------------------------------------------------------------
        l_JpiXMLFileHandler.set_validity_flag(self.productisvalid)
        # ---------------------------------------------------------------------------------------------
        l_JpiXMLFileHandler.save_to_file(output_filename)
        # ---------------------------------------------------------------------------------------------
        # Check the GLOBAL header even if  not valid (NOTV)
        # ---------------------------------------------------------------------------------------------
        if self.checkxmlfileswithschema:
            xml_tools.check_xml(output_filename, schemaLocationDirectory)
