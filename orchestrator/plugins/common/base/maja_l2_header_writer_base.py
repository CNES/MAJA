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
from orchestrator.common.maja_exceptions import MajaNotYetImplemented
from orchestrator.common.logger.maja_logging import configure_logger
from orchestrator.common.maja_exceptions import *
import orchestrator.common.file_utils as file_utils
import orchestrator.common.maja_utils as maja_utils
import orchestrator.common.xml_tools as xml_tools
from orchestrator.common.earth_explorer.earth_explorer_xml_file_handler import EarthExplorerXMLFileHandler
from orchestrator.plugins.common.base.maja_l_2_3_header_writer_base import L23HeaderWriterBase
import os

LOGGER = configure_logger(__name__)


class L2HeaderWriterBase(L23HeaderWriterBase):

    def __init__(self):
        super(L2HeaderWriterBase, self).__init__()
        # Set/Get the cirrus flag
        self.cirrusflag = False
        # Set/Get the cloud rate flag
        self.cloudrate = 0
        # Set/Get the snow rate flag
        self.snowrate = 0
        # Set/Get the hot spot flag
        self.hotspotflag = False
        # Set/Get the rain flag
        self.rainflag = False
        # Set/Get the sunglint flag
        self.sunglintflag = False
        # Set the ProcessingMode
        self.initmode = False
        self.backwardmode = False
        # Boolean that indicates if the product has to be write
        self.writel2products = False
        # Boolean that indicates if the public part of the L2 product has to be write
        self.writepublicproduct = False
        self.enablewriteearthexplorerheaders = False
        # The number of stack images
        self.numberofstackimages = 0
        self.enablel2product = False
        # File class value using to write the L3 or L3 output products
        self.vapnodatavalue = ""
        self.vapquantificationvalue = 0.0
        self.aotnodatavalue = ""
        self.aotquantificationvalue = 0
        self.stolistofstringdates = []
        self.listofbandtheoreticalwavelengthforreflectancescomposite = []
        self.listofbandtheoreticalwavelengthforltccomposite = []
        self.correlbandtheoreticalwavelengthforstocomposite = 0
        self.listofusedproductsids = []
        # DEM Informations
        self.dem = None
        # Plugin
        self.plugin = None
        # Name of the L2 product (initialized adn come from the L2ImageFileWriter)
        self.outputl2productfilename = ""
        # This is the output directory where the product is written
        self.outputdirectory = ""
        self.outputl2privateimagefilenamesprovider = None
        # Set write LTC product boolean
        self.writeltc = False
        self.ignorecurrentltc = False
        self.l1imageinformationsproviderbase = None
        self.l1_plugin = None
        self.inputl2imagefilereader = None
        #CAMS status
        self.camsstatus = None

    def create(self, plugin_name, app_handler):
        raise MajaNotYetImplemented("Could not instanciate base class")

    def write(self):
        raise MajaNotYetImplemented("Could not instanciate base class")

    def pre_processing(self):
        # LAIG-FA-MAC-2010-CNES : ajout test sur WriteLTC et Produit valid
        if self.initmode == False and self.writeltc and self.productisvalid:
            # Copy all files from the L2 Input directory to the output L2 product
            LOGGER.debug(
                "Copy the LTC lookuptable from the L2 Input (" +
                self.inputl2imagefilereader.L2PrivateImageFilenamesProvider.get_ltc_image_dir_filename() +
                ") product to the output L2 product (" +
                self.outputl2privateimagefilenamesprovider.get_ltc_image_dir_filename() +
                ")...")
            file_utils.copy_tree(
                self.inputl2imagefilereader.L2PrivateImageFilenamesProvider.get_ltc_image_dir_filename(),
                self.outputl2privateimagefilenamesprovider.get_ltc_image_dir_filename())

    def write_private_reflectances_xmlhandler(
            self,
            p_L2PrivateImageFilenamesProvider,
            p_ReferenceProductHeaderId,
            p_ReferenceProductInstance,
            p_dem,
            p_Validity_Start,
            p_Validity_Stop,
            p_Mission,
            p_root_template_directory,
            p_SchemaLocationDirectory,
            pCurrentDate,
            p_EnableCheckXMLFilesWithSchema=True):
        # ---------------------------------------------------------------------------------------------------
        l_CoarseProductImageSizeX = p_dem.CoarseArea.size[0]
        l_CoarseProductImageSizeY = p_dem.CoarseArea.size[1]
        resol = 0
        l_DL2L2CoarseRatio = float(p_dem.L2Areas[resol].size[0]) / float(p_dem.CoarseArea.size[0])
        l_L2L2CoarseRatio = int(l_DL2L2CoarseRatio + 0.5)
        LOGGER.debug("Real L2L2CoarseRatio: " + str(l_DL2L2CoarseRatio) + " -> integer: " + str(l_L2L2CoarseRatio))
        # ---------------------------------------------------------------------------------------------------
        # Date PDV in UTC format
        # Get Validity
        lCreator_Version = self.plugin.Creator_Version
        lFile_Version = self.plugin.L2PRODUCT_FILE_VERSION

        lReferenceProductHeaderId = str(p_ReferenceProductHeaderId)
        lReferenceProductInstance = str(p_ReferenceProductInstance)

        # ---------------------------------------------------------------------------------------------------
        # Write RCR
        LOGGER.debug("Write the PRIVATE RCR header file ...")
        output_filename = p_L2PrivateImageFilenamesProvider.get_rcr_header_filename()
        current_header_filename = os.path.join(p_root_template_directory, self.plugin.TEMPLATE_PDTANX_PRIVATE_RCR_HDR)
        if not os.path.exists(current_header_filename):
            raise MajaExceptionPluginBase(
                "Internal error: the template file '" +
                current_header_filename +
                "' doesn't exist !!")
        # ---------------------------------------------------------------------------------------------------
        # Load the file
        output_handler = EarthExplorerXMLFileHandler(current_header_filename)
        # ---------------------------------------------------------------------------------------------
        # Update bascis parameters
        output_handler.update(
            p_Validity_Start,
            p_Validity_Stop,
            lFile_Version,
            output_filename,
            self.creator,
            lCreator_Version,
            self.notes,
            self.system,
            self.outputfileclass,
            pCurrentDate,
            lReferenceProductHeaderId)
        # Attribut for theoretical wavelength
        l_attr = {}
        l_attr["unit"] = "nanometer"
        # ---------------------------------------------------------------------------------------------
        # Update bascis ANX parameters
        output_handler.update_pdtanx(
            p_Mission,
            l_CoarseProductImageSizeX,
            l_CoarseProductImageSizeY,
            lReferenceProductInstance,
            self.productisvalid)
        output_handler.set_string_value("//Annex_Code", "RCR")
        output_handler.set_string_value("//Nodata_Value", self.nodatavalue)
        output_handler.set_string_value("//Subsampling_Factor/By_Line", l_L2L2CoarseRatio)
        output_handler.set_string_value("//Subsampling_Factor/By_Column", l_L2L2CoarseRatio)
#        output_handler->SetUIntegersListValueSortedByAttribut(m_ListOfBandTheoreticalWavelengthForReflectancesComposite,
#                "//List_of_Band_Theoretical_Wavelength", "Band_Theoretical_Wavelength", "count", "sn", l_attr)
        xml_tools.set_string_list_value_sorted_by_attribute(
            output_handler.root,
            self.listofbandtheoreticalwavelengthforreflectancescomposite,
            "//List_of_Band_Theoretical_Wavelength",
            "Band_Theoretical_Wavelength",
            "count",
            "sn",
            1,
            l_attr)
        output_handler.set_string_value("//Bands",
                                        str(len(self.listofbandtheoreticalwavelengthforreflectancescomposite)))
        output_handler.save_to_file(output_filename)
        if self.checkxmlfileswithschema and self.productisvalid and p_EnableCheckXMLFilesWithSchema:
            xml_tools.check_xml(output_filename, p_SchemaLocationDirectory)
        # ---------------------------------------------------------------------------------------------------
        # Write RTA
        LOGGER.debug("Write the PRIVATE RTA header file ...")
        output_filename = p_L2PrivateImageFilenamesProvider.get_rta_header_filename()
        current_header_filename = os.path.join(p_root_template_directory, self.plugin.TEMPLATE_PDTANX_PRIVATE_RCR_HDR)
        if not os.path.exists(current_header_filename):
            raise MajaExceptionPluginBase(
                "Internal error: the template file '" +
                current_header_filename +
                "' doesn't exist !!")
        # ---------------------------------------------------------------------------------------------------
        # Load the file
        output_handler = EarthExplorerXMLFileHandler(current_header_filename)
        # ---------------------------------------------------------------------------------------------
        # Update bascis parameters
        output_handler.update(
            p_Validity_Start,
            p_Validity_Stop,
            lFile_Version,
            output_filename,
            self.creator,
            lCreator_Version,
            self.notes,
            self.system,
            self.outputfileclass,
            pCurrentDate,
            lReferenceProductHeaderId)
        # ---------------------------------------------------------------------------------------------
        # Update bascis ANX parameters
        output_handler.update_pdtanx(
            p_Mission,
            l_CoarseProductImageSizeX,
            l_CoarseProductImageSizeY,
            lReferenceProductInstance,
            self.productisvalid)
        # Set other values
        output_handler.set_string_value("//Annex_Code", "RTA")
        output_handler.set_string_value("//Nodata_Value", self.nodatavalue)
        output_handler.set_string_value("//Subsampling_Factor/By_Line", l_L2L2CoarseRatio)
        output_handler.set_string_value("//Subsampling_Factor/By_Column", l_L2L2CoarseRatio)
        xml_tools.set_string_list_value_sorted_by_attribute(
            output_handler.root,
            self.listofbandtheoreticalwavelengthforreflectancescomposite,
            "//List_of_Band_Theoretical_Wavelength",
            "Band_Theoretical_Wavelength",
            "count",
            "sn",
            1,
            l_attr)
        output_handler.set_string_value("//Bands", len(self.listofbandtheoreticalwavelengthforreflectancescomposite))
        # Save file
        output_handler.save_to_file(output_filename)
        if self.checkxmlfileswithschema and self.productisvalid and p_EnableCheckXMLFilesWithSchema:
            xml_tools.check_xml(output_filename, p_SchemaLocationDirectory)
        # ---------------------------------------------------------------------------------------------------
        # Write RTC
        LOGGER.debug("Write the PRIVATE RTA header file ...")
        output_filename = p_L2PrivateImageFilenamesProvider.get_rtc_header_filename()
        current_header_filename = os.path.join(p_root_template_directory, self.plugin.TEMPLATE_PDTANX_PRIVATE_RCR_HDR)
        if not os.path.exists(current_header_filename):
            raise MajaExceptionPluginBase(
                "Internal error: the template file '" +
                current_header_filename +
                "' doesn't exist !!")
        # ---------------------------------------------------------------------------------------------------
        # Load the file
        output_handler = EarthExplorerXMLFileHandler(current_header_filename)
        # ---------------------------------------------------------------------------------------------
        # Update bascis parameters
        output_handler.update(p_Validity_Start, p_Validity_Stop, lFile_Version, output_filename, self.creator,
                              lCreator_Version,
                              self.notes, self.system, self.outputfileclass, pCurrentDate,
                              lReferenceProductHeaderId)
        # ---------------------------------------------------------------------------------------------
        # Update bascis ANX parameters
        output_handler.update_pdtanx(p_Mission, l_CoarseProductImageSizeX, l_CoarseProductImageSizeY,
                                     lReferenceProductInstance,
                                     self.productisvalid)

        output_handler.set_string_value("//Annex_Code", "RTC")
        output_handler.set_string_value("//Nodata_Value", self.nodatavalue)
        output_handler.set_string_value("//Subsampling_Factor/By_Line", l_L2L2CoarseRatio)
        output_handler.set_string_value("//Subsampling_Factor/By_Column", l_L2L2CoarseRatio)
        xml_tools.set_string_list_value_sorted_by_attribute(
            output_handler.root,
            self.listofbandtheoreticalwavelengthforreflectancescomposite,
            "//List_of_Band_Theoretical_Wavelength",
            "Band_Theoretical_Wavelength",
            "count",
            "sn",
            1,
            l_attr)
        output_handler.set_string_value("//Bands", len(self.listofbandtheoreticalwavelengthforreflectancescomposite))
        # Save file
        output_handler.save_to_file(output_filename)
        if self.checkxmlfileswithschema and self.productisvalid and p_EnableCheckXMLFilesWithSchema:
            xml_tools.check_xml(output_filename, p_SchemaLocationDirectory)

    def write_private_others_xmlhandler(self, p_L2PrivateImageFilenamesProvider, p_ReferenceProductHeaderId,
                                     p_ReferenceProductInstance, p_dem,
                                     p_Validity_Start, p_Validity_Stop, p_Mission,
                                     p_root_template_directory, p_SchemaLocationDirectory, pCurrentDate,
                                     p_EnableCheckXMLFilesWithSchema=True):
        LOGGER.debug("L2HeaderWriterBase __dict__ %s", self.__dict__)
        l_CoarseProductImageSizeX = p_dem.CoarseArea.size[0]
        l_CoarseProductImageSizeY = p_dem.CoarseArea.size[1]
        resol = 0
        l_DL2L2CoarseRatio = float(p_dem.L2Areas[resol].size[0]) / float(p_dem.CoarseArea.size[0])
        l_L2L2CoarseRatio = int(l_DL2L2CoarseRatio + 0.5)
        LOGGER.debug("Real L2L2CoarseRatio: " + str(l_DL2L2CoarseRatio) + " -> integer: " + str(l_L2L2CoarseRatio))
        # ---------------------------------------------------------------------------------------------------
        # Date PDV in UTC format
        # Get Validity
        lCreator_Version = self.plugin.Creator_Version
        lFile_Version = self.plugin.L2PRODUCT_FILE_VERSION
        lReferenceProductHeaderId = p_ReferenceProductHeaderId
        lReferenceProductInstance = p_ReferenceProductInstance
        # ---------------------------------------------------------------------------------------------------
        # Write CLD
        output_filename = p_L2PrivateImageFilenamesProvider.get_cld_header_filename()
        LOGGER.debug("Write the PRIVATE CLD header file " + output_filename + " ...")
        current_header_filename = os.path.join(p_root_template_directory, self.plugin.TEMPLATE_PDTANX_PRIVATE_CLD_HDR)
        if not os.path.exists(current_header_filename):
            raise MajaExceptionPluginBase(
                "Internal error: the template file '" +
                current_header_filename +
                "' doesn't exist !!")

        LOGGER.debug("LoadFile '" + current_header_filename + "...")
        # ---------------------------------------------------------------------------------------------------
        # Load the file
        output_handler = EarthExplorerXMLFileHandler(current_header_filename)
        # ---------------------------------------------------------------------------------------------
        # Update bascis parameters
        output_handler.update(p_Validity_Start, p_Validity_Stop, lFile_Version, output_filename, self.creator,
                              lCreator_Version,
                              self.notes, self.system, self.outputfileclass, pCurrentDate,
                              lReferenceProductHeaderId)
        # ---------------------------------------------------------------------------------------------
        # Update bascis ANX parameters
        output_handler.update_pdtanx(p_Mission, l_CoarseProductImageSizeX, l_CoarseProductImageSizeY,
                                     lReferenceProductInstance,
                                     self.productisvalid)
        # Update other parameter
        output_handler.set_string_value("//Subsampling_Factor/By_Line", l_L2L2CoarseRatio)
        output_handler.set_string_value("//Subsampling_Factor/By_Column", l_L2L2CoarseRatio)
        l_CLDNbBits = self.plugin.NumbersOfBitsInCLDDataIOBand
        output_handler.set_string_value("//Number_of_Significant_Bits", l_CLDNbBits)
        if l_CLDNbBits <= 8:
            output_handler.set_string_value("//Data_Type", "UNSIGNED_BYTE")
        elif l_CLDNbBits <= 16:
            output_handler.set_string_value("//Data_Type", "UNSIGNED_SHORT")
        else:
            raise MajaExceptionPluginBase("Internal error:  CLD band must be written with [1..16] bits !")
        # save file
        output_handler.save_to_file(output_filename)
        if self.checkxmlfileswithschema and self.productisvalid and p_EnableCheckXMLFilesWithSchema:
            xml_tools.check_xml(output_filename, p_SchemaLocationDirectory)
        # ---------------------------------------------------------------------------------------------------
        # Write NDT
        LOGGER.debug("Write the PRIVATE NDT header file ...")
        output_filename = p_L2PrivateImageFilenamesProvider.get_ndt_header_filename()
        current_header_filename = os.path.join(p_root_template_directory, self.plugin.TEMPLATE_PDTANX_PRIVATE_NDT_HDR)
        if not os.path.exists(current_header_filename):
            raise MajaExceptionPluginBase(
                "Internal error: the template file '" +
                current_header_filename +
                "' doesn't exist !!")
        # ---------------------------------------------------------------------------------------------------
        # Load the file
        output_handler = EarthExplorerXMLFileHandler(current_header_filename)
        # ---------------------------------------------------------------------------------------------
        # Update bascis parameters
        output_handler.update(p_Validity_Start, p_Validity_Stop, lFile_Version, output_filename, self.creator,
                              lCreator_Version,
                              self.notes, self.system, self.outputfileclass, pCurrentDate,
                              lReferenceProductHeaderId)
        # ---------------------------------------------------------------------------------------------
        # Update bascis ANX parameters
        output_handler.update_pdtanx(p_Mission, l_CoarseProductImageSizeX, l_CoarseProductImageSizeY,
                                     lReferenceProductInstance,
                                     self.productisvalid)
        # Update other parameter
        output_handler.set_string_value("//Subsampling_Factor/By_Line", l_L2L2CoarseRatio)
        output_handler.set_string_value("//Subsampling_Factor/By_Column", l_L2L2CoarseRatio)
        output_handler.save_to_file(output_filename)
        if self.checkxmlfileswithschema and self.productisvalid and p_EnableCheckXMLFilesWithSchema:
            xml_tools.check_xml(output_filename, p_SchemaLocationDirectory)
        # ---------------------------------------------------------------------------------------------------
        # Write WAM
        LOGGER.debug("Write the PRIVATE WAM header file ...")
        output_filename = p_L2PrivateImageFilenamesProvider.get_wam_header_filename()
        current_header_filename = os.path.join(p_root_template_directory, self.plugin.TEMPLATE_PDTANX_PRIVATE_WAM_HDR)
        if not os.path.exists(current_header_filename):
            raise MajaExceptionPluginBase(
                "Internal error: the template file '" +
                current_header_filename +
                "' doesn't exist !!")
        # -----------------------------------------------------
        # Load the file
        output_handler = EarthExplorerXMLFileHandler(current_header_filename)
        # ---------------------------------------------------------------------------------------------
        # Update bascis parameters
        output_handler.update(p_Validity_Start, p_Validity_Stop, lFile_Version, output_filename, self.creator,
                              lCreator_Version,
                              self.notes, self.system, self.outputfileclass, pCurrentDate,
                              lReferenceProductHeaderId)
        # ---------------------------------------------------------------------------------------------
        # Update bascis ANX parameters
        output_handler.update_pdtanx(p_Mission, l_CoarseProductImageSizeX, l_CoarseProductImageSizeY,
                                     lReferenceProductInstance,
                                     self.productisvalid)
        # Update other parameter
        output_handler.set_string_value("//Subsampling_Factor/By_Line", l_L2L2CoarseRatio)
        output_handler.set_string_value("//Subsampling_Factor/By_Column", l_L2L2CoarseRatio)
        output_handler.save_to_file(output_filename)
        if self.checkxmlfileswithschema and self.productisvalid and p_EnableCheckXMLFilesWithSchema:
            xml_tools.check_xml(output_filename, p_SchemaLocationDirectory)
        # ---------------------------------------------------------------------------------------------------
        # Write CLA
        # LAIG-DM-MAC-1691-CNES : Write CLA only is available (only for VENUS - stereo)
        if os.path.exists(p_L2PrivateImageFilenamesProvider.get_cla_image_filename()):
            LOGGER.debug("Write the PRIVATE CLA header file ...")
            output_filename = p_L2PrivateImageFilenamesProvider.get_cla_header_filename()

            current_header_filename = os.path.join(
                p_root_template_directory,
                self.plugin.TEMPLATE_PDTANX_PRIVATE_CLA_HDR)
            if not os.path.exists(current_header_filename):
                raise MajaExceptionPluginBase(
                    "Internal error: the template file '" + current_header_filename + "' doesn't exist !!")
            # ---------------------------------------------------------------------------------------------------
            # Load the file
            output_handler = EarthExplorerXMLFileHandler(current_header_filename)
            # ---------------------------------------------------------------------------------------------
            # Update bascis parameters
            output_handler.update(p_Validity_Start, p_Validity_Stop, lFile_Version, output_filename, self.creator,
                                  lCreator_Version,
                                  self.notes, self.system, self.outputfileclass, pCurrentDate,
                                  lReferenceProductHeaderId)
            # ---------------------------------------------------------------------------------------------
            # Update bascis ANX parameters
            output_handler.update_pdtanx(p_Mission, l_CoarseProductImageSizeX, l_CoarseProductImageSizeY,
                                         lReferenceProductInstance,
                                         self.productisvalid)
            # Update other parameter
            output_handler.set_string_value("//Nodata_Value", self.nodatavalue)
            output_handler.set_string_value("//Subsampling_Factor/By_Line", l_L2L2CoarseRatio)
            output_handler.set_string_value("//Subsampling_Factor/By_Column", l_L2L2CoarseRatio)
            output_handler.save_to_file(output_filename)
            if self.checkxmlfileswithschema and self.productisvalid and p_EnableCheckXMLFilesWithSchema:
                xml_tools.check_xml(output_filename, p_SchemaLocationDirectory)
        # ---------------------------------------------------------------------------------------------------
        # Write PXD
        LOGGER.debug("Write the PRIVATE PXD header file ...")
        output_filename = p_L2PrivateImageFilenamesProvider.get_pxd_header_filename()
        current_header_filename = os.path.join(p_root_template_directory, self.plugin.TEMPLATE_PDTANX_PRIVATE_PXD_HDR)
        if not os.path.exists(current_header_filename):
            raise MajaExceptionPluginBase(
                "Internal error: the template file '" + current_header_filename + "' doesn't exist !!")
        # ---------------------------------------------------------------------------------------------------
        # Load the file
        output_handler = EarthExplorerXMLFileHandler(current_header_filename)
        # ---------------------------------------------------------------------------------------------
        # Update bascis parameters
        output_handler.update(p_Validity_Start, p_Validity_Stop, lFile_Version, output_filename, self.creator,
                              lCreator_Version,
                              self.notes, self.system, self.outputfileclass, pCurrentDate,
                              lReferenceProductHeaderId)
        # ---------------------------------------------------------------------------------------------
        # Update bascis ANX parameters
        output_handler.update_pdtanx(p_Mission, l_CoarseProductImageSizeX, l_CoarseProductImageSizeY,
                                     lReferenceProductInstance,
                                     self.productisvalid)
        # Update other parameter
        output_handler.set_string_value("//Nodata_Value", self.nodatavalue)
        output_handler.set_string_value("//Subsampling_Factor/By_Line", l_L2L2CoarseRatio)
        output_handler.set_string_value("//Subsampling_Factor/By_Column", l_L2L2CoarseRatio)
        # Save to file
        output_handler.save_to_file(output_filename)
        if self.checkxmlfileswithschema and self.productisvalid and p_EnableCheckXMLFilesWithSchema:
            xml_tools.check_xml(output_filename, p_SchemaLocationDirectory)

    def write_private_sto_xmlhandler(self, sto_header_filename, p_ReferenceProductHeaderId,
                                  p_ReferenceProductInstance, p_dem, p_Validity_Start,
                                  p_Validity_Stop, p_Mission, p_root_template_directory,
                                  p_SchemaLocationDirectory, pCurrentDate,
                                  p_EnableCheckXMLFilesWithSchema=True):
        l_CoarseProductImageSizeX = p_dem.CoarseArea.size[0]
        l_CoarseProductImageSizeY = p_dem.CoarseArea.size[1]

        resol = 0
        l_DL2L2CoarseRatio = float(p_dem.L2Areas[resol].size[0]) / float(p_dem.CoarseArea.size[0])
        l_L2L2CoarseRatio = int(l_DL2L2CoarseRatio + 0.5)

        # ---------------------------------------------------------------------------------------------------
        # Date PDV in UTC format
        # Get Validity
        lCreator_Version = self.plugin.Creator_Version
        lFile_Version = self.plugin.L2PRODUCT_FILE_VERSION
        lReferenceProductHeaderId = p_ReferenceProductHeaderId
        lReferenceProductInstance = p_ReferenceProductInstance
        # ---------------------------------------------------------------------------------------------------
        # Write STO
        LOGGER.debug("Write the PRIVATE STO header file ...")
        output_filename = sto_header_filename  # p_L2PrivateImageFilenamesProvider.GetSTOHeaderFileName()
        current_header_filename = os.path.join(p_root_template_directory, self.plugin.TEMPLATE_PDTANX_PRIVATE_STO_HDR)
        if not os.path.exists(current_header_filename):
            raise MajaExceptionPluginBase(
                "Internal error: the template file '" +
                current_header_filename +
                "' doesn't exist !!")
        # ---------------------------------------------------------------------------------------------------
        if not os.path.exists(current_header_filename):
            raise MajaExceptionPluginBase(
                "Internal error: the template file '" + current_header_filename + "' doesn't exist !!")
        # ---------------------------------------------------------------------------------------------------
        # Load the file
        output_handler = EarthExplorerXMLFileHandler(current_header_filename)
        # ---------------------------------------------------------------------------------------------
        # Update bascis parameters
        output_handler.update(p_Validity_Start, p_Validity_Stop, lFile_Version, output_filename, self.creator,
                              lCreator_Version,
                              self.notes, self.system, self.outputfileclass, pCurrentDate,
                              lReferenceProductHeaderId)
        # ---------------------------------------------------------------------------------------------
        # Update bascis ANX parameters
        output_handler.update_pdtanx(p_Mission, l_CoarseProductImageSizeX, l_CoarseProductImageSizeY,
                                     lReferenceProductInstance,
                                     self.productisvalid)
        # Update other parameter
        output_handler.set_string_value("//Nodata_Value", self.nodatavalue)
        output_handler.set_string_value("//Subsampling_Factor/By_Line", l_L2L2CoarseRatio)
        output_handler.set_string_value("//Subsampling_Factor/By_Column", l_L2L2CoarseRatio)

        output_handler.set_string_value("//Size/Bands", self.numberofstackimages)
        output_handler.set_list_of_bands(self.stolistofstringdates)  # , "//List_of_Bands", "Band")
        # The Band_Theoretical_Wavelength could be not exist (ex: VENUS)
        # Check the existance of the Node in the template xml file, before write the value
        if xml_tools.get_only_value(output_handler.root, "//Band_Theoretical_Wavelength", check=True) is not None:
            output_handler.set_string_value(
                "//Band_Theoretical_Wavelength",
                self.correlbandtheoreticalwavelengthforstocomposite)
        # Save to file
        output_handler.save_to_file(output_filename)
        if self.checkxmlfileswithschema and self.productisvalid and p_EnableCheckXMLFilesWithSchema:
            xml_tools.check_xml(output_filename, p_SchemaLocationDirectory)

    def write_private_tec_xmlhandler(self, tec_header_filename, p_ReferenceProductHeaderId,
                                  p_Validity_Start, p_Validity_Stop, p_Mission,
                                  p_NickName, p_AcquisitionDate, p_root_template_directory,
                                  p_SchemaLocationDirectory, pCurrentDate,
                                  p_EnableCheckXMLFilesWithSchema=True):
        # ---------------------------------------------------------------------------------------------------
        # Date PDV in UTC format
        # Get Validity
        lCreator_Version = self.plugin.Creator_Version
        lFile_Version = self.plugin.L2PRODUCT_FILE_VERSION
        lReferenceProductHeaderId = p_ReferenceProductHeaderId
        l_TECFileType = "TEC_L2VALD"
        # ---------------------------------------------------------------------------------------------------
        if not self.productisvalid:
            l_TECFileType = "TEC_L2NOTV"
        # ---------------------------------------------------------------------------------------------------
        # Write TEC Private EEF
        LOGGER.debug("Write the PRIVATE TEC header file ...")
        output_filename = tec_header_filename  # p_L2ImageFilenamesProvider->GetPrivateFileNames().GetHDRPrivateFileName()
        current_header_filename = os.path.join(p_root_template_directory, self.plugin.TEMPLATE_TEC_PRIVATE_EEF)
        if not os.path.exists(current_header_filename):
            raise MajaExceptionPluginBase(
                "Internal error: the template file '" +
                current_header_filename +
                "' doesn't exist !!")
        # ---------------------------------------------------------------------------------------------
        # Load the template file used to writes the new header file
        # Load the file
        output_handler = EarthExplorerXMLFileHandler(current_header_filename)
        # ---------------------------------------------------------------------------------------------
        # Update bascis parameters
        output_handler.update(p_Validity_Start, p_Validity_Stop, lFile_Version, output_filename, self.creator,
                              lCreator_Version,
                              self.notes, self.system, self.outputfileclass, pCurrentDate,
                              lReferenceProductHeaderId)
        # Update other parameters
        output_handler.set_string_value("//Mission", p_Mission)
        # File_Type
        output_handler.set_string_value("//File_Type", l_TECFileType)
        # Nick_Name
        output_handler.set_string_value("//Nick_Name", p_NickName)
        # Acquisition_Date
        output_handler.set_string_value("//Acquisition_Date", p_AcquisitionDate)
        # Save to file
        output_handler.save_to_file(output_filename)
        LOGGER.debug(p_EnableCheckXMLFilesWithSchema)
        if self.checkxmlfileswithschema and self.productisvalid and p_EnableCheckXMLFilesWithSchema:
            xml_tools.check_xml(output_filename, p_SchemaLocationDirectory)

    def write_private_ltc_xmlhandler(self, p_InputL2ImageFileReader, p_ReferenceProductHeaderId,
                                  p_ReferenceProductInstance, root_sc_xml_templates,
                                  p_OutputL2PrivateImageFilenamesProvider, schemaLocationDirectory,
                                  pCurrentDate, pValidity_Start, pValidity_Stop,
                                  p_MeanSolarAngles, p_MeanViewingAngles,
                                  p_EnableCheckXMLFilesWithSchema=True):
        output_filename = p_OutputL2PrivateImageFilenamesProvider.get_ltc_header_filename()
        l_FileDefinition = self.plugin.ShortFileType + "_LUTANX"
        # ---------------------------------------
        # Get global information about the output L2 product
        # ---------------------------------------
        LOGGER.debug("Write the PRIVATE LTC header file ...")
        lReferenceProductHeaderId = p_ReferenceProductHeaderId
        lReferenceProductInstance = p_ReferenceProductInstance
        lFile_Version = self.plugin.L2PRODUCT_FILE_VERSION
        # Get Validity
        lValidity_Start = pValidity_Start
        lValidity_Stop = pValidity_Stop

        current_header_filename = root_sc_xml_templates
        if not os.path.exists(current_header_filename):
            raise MajaExceptionPluginBase(
                "Internal error: the template file '" +
                current_header_filename +
                "' doesn't exist !!")

        # Load the file
        output_handler = EarthExplorerXMLFileHandler(current_header_filename)
        # ---------------------------------------------------------------------------------------------
        # Update bascis parameters
        output_handler.update(pValidity_Start, pValidity_Stop, lFile_Version, output_filename, self.creator,
                              self.plugin.Creator_Version,
                              self.notes, self.system, self.outputfileclass, pCurrentDate,
                              lReferenceProductHeaderId)
        # ---------------------------------------
        # Detect the good GIPP TOCR filename to read data
        # ---------------------------------------
        lNbGippFilenames = len(self.listofgippsfilenames)
        l_found = False
        l_GIPTOCRFilename = ""
        # For each GIPP filename of the list
        for i in range(lNbGippFilenames):
            # Look for the header file of the GIP_L2TOCR
            l_FileName = self.listofgippsfilenames[i]
            l_FileName = os.path.splitext(l_FileName)[0] + ".HDR"
            # ---------------------------------------------------------------------------------------------------
            # Load the file
            l_CanLoad = xml_tools.can_load_file(l_FileName)
            if l_CanLoad:
                # Read the header file of the GIP_L2TOCR
                lHolder = EarthExplorerXMLFileHandler(l_FileName)
                l_Satellite = lHolder.get_string_value("//Mission")
                l_File_Type = lHolder.get_string_value("//File_Type")
                # Check the validity of the found GIPP : camera and type
                if l_Satellite in self.l1imageinformationsproviderbase.Satellite and "GIP_L2TOCR" in l_File_Type:
                    l_GIPTOCRFilename = l_FileName
                    l_found = True

        # The GIP_L2TOCR does not exist
        if not l_found:
            raise MajaExceptionPluginBase(
                "Impossible to detect the TOCR GIPP filename for the current satellite <" +
                self.l1imageinformationsproviderbase.Satellite +
                "> !")

        # ****************************************************************************************
        # Write LUT_Indexes information
        lTOCRHolder = EarthExplorerXMLFileHandler(l_GIPTOCRFilename)
        # ---------------------------------------------------------------------------------------------------
        # Get the information of the input LUT to copy them in the LTC header file
        output_handler.set_string_value("//LUT_Indexes/Altitude_Indexes",
                                        lTOCRHolder.get_string_value("//LUT_Indexes/Altitude_Indexes"))
        output_handler.set_string_value("//LUT_Indexes/AOT_Indexes",
                                        lTOCRHolder.get_string_value("//LUT_Indexes/AOT_Indexes"))
        output_handler.set_string_value("//LUT_Indexes/TOA_Reflectance_Indexes",
                                        lTOCRHolder.get_string_value("//LUT_Indexes/TOA_Reflectance_Indexes"))
        output_handler.set_string_value("//Mission", self.l1imageinformationsproviderbase.Satellite)
        output_handler.set_string_value("//Reference_Product_Instance", lReferenceProductInstance)
        l_attr = dict()
        l_attr["unit"] = "nanometer"
        xml_tools.set_string_list_value_sorted_by_attribute(output_handler.root,
                                                            self.listofbandtheoreticalwavelengthforltccomposite,
                                                            "//List_of_Band_Theoretical_Wavelength",
                                                            "Band_Theoretical_Wavelength", "count", "sn", 1, l_attr)

        #                    output_handler.save_to_fileAndCheck(output_filename, lSchemaLocationDirectory)
        # ---------------------------------------
        # Check the product validity of the output L2 product
        # ---------------------------------------
        if self.productisvalid:
            output_handler.set_string_value("//Reference_Product_Semantic", "L2VALD")
        else:
            output_handler.set_string_value("//Reference_Product_Semantic", "L2NOTV")
        # Initialization used to write the binary data of the LTC file
        listOfLTC = []
        listOfpft = []
        LOGGER.debug("p_OutputL2PrivateImageFilenamesProvider.GetLTCHeaderFileName(): " +
                     p_OutputL2PrivateImageFilenamesProvider.get_ltc_header_filename())

# 4.3         l_DestinationDir = Utilities::SetExtension(output_filename, ".DBL.DIR")
        l_DestinationDir = p_OutputL2PrivateImageFilenamesProvider.get_ltc_image_dir_filename()
        LOGGER.debug("l_DestinationDir: " + l_DestinationDir)
        l_DestinationRelativeDir = os.path.basename(l_DestinationDir)
        LOGGER.debug("l_DestinationRelativeDir: " + l_DestinationRelativeDir)
        # ****************************************************************************************
        # Reading Angles Solar and Viewing from LTC of L2 previous product
        if not self.initmode:
            if p_InputL2ImageFileReader is None:
                raise MajaExceptionPluginBase("Internal error: the variable m_InputL2ImageFileReader is NULL!")
            # p_L2PrivateImageFilenamesProvider.GetLTCHeaderFileName()
            # #m_L2InputImageFilenamesProvider->GetPrivateFileNames().GetLTCHeaderFileName()
            L2_LTCHeaderFilename = p_InputL2ImageFileReader.L2PrivateImageFilenamesProvider.get_ltc_header_filename()
            LOGGER.debug("Nominal mode: open the L2 input LTC HeaderFileName: " + L2_LTCHeaderFilename)
            lHolder = EarthExplorerXMLFileHandler(L2_LTCHeaderFilename)
            lHolder.get_l2_product_ltc_information(l_DestinationRelativeDir, listOfLTC, listOfpft)
            # Copy all files from the L2 Input directory to the output L2 product
            file_utils.copy_tree(
                p_InputL2ImageFileReader.L2PrivateImageFilenamesProvider.get_ltc_image_dir_filename(),
                l_DestinationDir)
        # ****************************************************************************************
        # Reading Solar and Viewing angles from L1 product
        # Push in the list
        LOGGER.debug("m_IgnoreCurrentLTC: " + str(self.ignorecurrentltc))
        if not self.ignorecurrentltc:
            ltc = EarthExplorerXMLFileHandler.LTCType()
            solarAngles = p_MeanSolarAngles
                #[p_MeanSolarAngles['sun_zenith_angle'], p_MeanSolarAngles['sun_azimuth_angle']]
            LOGGER.debug(solarAngles)
            LOGGER.debug(EarthExplorerXMLFileHandler.EE_SCHEMA_DECIMAL_9_FORMAT)
            ltc.solarangle.azimuth = maja_utils.format_to_string(
                float(solarAngles[1]), EarthExplorerXMLFileHandler.EE_SCHEMA_DECIMAL_9_FORMAT)
            ltc.solarangle.zenith = maja_utils.format_to_string(
                float(solarAngles[0]), EarthExplorerXMLFileHandler.EE_SCHEMA_DECIMAL_9_FORMAT)
            viewingAngles = p_MeanViewingAngles
            ltc.viewingangle.azimuth = maja_utils.format_to_string(
                viewingAngles[1], EarthExplorerXMLFileHandler.EE_SCHEMA_DECIMAL_9_FORMAT)
            ltc.viewingangle.zenith = maja_utils.format_to_string(
                viewingAngles[0], EarthExplorerXMLFileHandler.EE_SCHEMA_DECIMAL_9_FORMAT)
            ltc.Date = lValidity_Start
            # Add the current LTC header
            pft = EarthExplorerXMLFileHandler.PackagedDBLFileType()
            pft.relativefilepath = os.path.join(
                l_DestinationRelativeDir, os.path.basename(
                    p_OutputL2PrivateImageFilenamesProvider.get_ltc_image_filename()))
            pft.filedefinition = l_FileDefinition
            listOfpft.append(pft)
            listOfLTC.append(ltc)
        # Write to the output handler
        output_handler.write_private_ltc_header(listOfLTC)
        output_handler.write_private_ltc_header_list_of_dbl(listOfpft)
        # ****************************************************************************************
        # Save the file
        output_handler.save_to_file(output_filename)
        if self.checkxmlfileswithschema and self.productisvalid and p_EnableCheckXMLFilesWithSchema:
            xml_tools.check_xml(output_filename, schemaLocationDirectory)
        # ****************************************************************************************
        # Post processing : to be sur that there are only mha listed in the listOfpft present in the directory, remove others files
        # LAIG-FA-MAC-1630-CNES
        # ---------------------------------------------------------------------------------------
        # Get the Listed filename
        l_LTCProductMHAFilename = list()
        for itList in listOfpft:
            # Get file but onlyt the mha (without l_DestinationRelativeDir)
            # Ex :
            # <Relative_File_Path>VE_TEST_VSC_LUTANX_L2VALD_CAMARGUE_20120101_LTC.DBL.DIR/VE_TEST_VSC_LUTANX_L2VALD_CAMARGUE_20120103_LTC.DBL.mha</Relative_File_Path>
            l_LTCProductMHAFilename.append(os.path.basename(itList.relativefilepath))
        # ---------------------------------------------------------------------------------------
        # Get the Listed filename in the directory
        l_LTCProductMHAFilenameInDisc = [os.path.basename(f) for f in os.listdir(
            l_DestinationDir) if os.path.isfile(os.path.join(l_DestinationDir, f))]

        # Remove the files on disc that are not listed as used
        for f in l_LTCProductMHAFilenameInDisc:
            if f not in l_LTCProductMHAFilename:
                file_utils.remove_file(os.path.join(l_DestinationDir, f))
