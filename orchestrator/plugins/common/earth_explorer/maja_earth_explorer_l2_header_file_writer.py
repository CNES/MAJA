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
                        o  o  o o    o      o o    o     {|||D
                        o     o oooooo      o oooooo      / \
                        o     o o    o o    o o    o      `-^
                        o     o o    o  oooo  o    o

###################################################################################################

orchestrator.plugins.sentinel2.maja_sentinel2_angles -- shortdesc

orchestrator.plugins.sentinel2.maja_sentinel2_angles is a description

It defines classes_and_methods


###################################################################################################
"""
import orchestrator.common.file_utils as file_utils
from orchestrator.plugins.common.base.maja_l2_header_writer_base import L2HeaderWriterBase
from orchestrator.common.earth_explorer.earth_explorer_xml_file_handler import EarthExplorerXMLFileHandler
from orchestrator.common.maja_exceptions import *
from orchestrator.plugins.common.base.maja_l2_image_filenames_provider import L2ImageFilenamesProvider
from orchestrator.common.logger.maja_logging import configure_logger
import orchestrator.common.xml_tools as xml_tools
import os
LOGGER = configure_logger(__name__)


class EarthExplorerL2HeaderFileWriter(L2HeaderWriterBase):

    def __init__(self):
        super(EarthExplorerL2HeaderFileWriter, self).__init__()
        self.l2inputimagefilenamesprovider = None
        self.l2imagefilenamesprovider = None

    def preprocessing(self):
        # *********************************************************************************************************
        # Store the Global filename of the product. Here, it's a EarthExplorer L2 product organization
        self.l2imagefilenamesprovider = L2ImageFilenamesProvider()

        self.l2imagefilenamesprovider.initialize(self.l1imageinformationsproviderbase, self.plugin.ListOfL2Resolutions,
                                                 self.outputdirectory, False)  # Check existence
        # LOGGER.debug("EarthExplorerL2ImageFileWriter::GenerateData(): l_L2ImageFilenamesProvider pointer "
        # + self.l2imagefilenamesprovider)

        if self.plugin is None:
            raise MajaException("Internal error: the variable m_PluginBasePointer is NULL!")

        if self.l1imageinformationsproviderbase is None:
            raise MajaException("Internal error: the variable m_L1ImageInformationsProviderBase is NULL!")

        if not self.initmode:
            if self.inputl2imagefilereader is None:
                raise MajaException("Internal error: the variable m_InputL2ImageFileReader is NULL!")

        file_utils.create_directory(self.l2imagefilenamesprovider.get_public_directory())
        file_utils.create_directory(self.l2imagefilenamesprovider.get_private_filenames().get_private_directory())
        # ---------------------------------------------------------------------------------------------------
        # Write LTC Private Header
        # Note necessary for Backward mode
        # MACCS 4.7.2 - correction pour FA 1572
        if self.writeltc and self.writel2products and self.backwardmode:
            # ---------------------------------------------------------------------------------------------
            # Create the LTC directory if don't exist
            file_utils.create_directory(
                self.l2imagefilenamesprovider.get_private_filenames().get_ltc_image_dir_filename())
        # else:
        #    System::RemoveDirectory(m_L2ImageFilenamesProvider->GetPrivateFileNames().GetLTCImageDirFileName())
        #    itksys::SystemTools::RemoveFile(m_L2ImageFilenamesProvider->GetPrivateFileNames().GetLTCHeaderFileName().
        #    c_str())

        # ---------------------------------------------------------------------------------------------
        # Create the .DBL file (zero)
        file_utils.touch_file(self.l2imagefilenamesprovider.get_dbl_filename())

        # ---------------------------------------------------------------------------------------------
        # Call the PreProcessing Supperclass
        super(EarthExplorerL2HeaderFileWriter, self).pre_processing()

    def write_public_xml_handler(self, p_L2ImageFilenamesProvider, p_dem, p_Validity_Start, p_Validity_Stop, p_Mission,
                              p_root_template_directory, p_SchemaLocationDirectory, pCurrentDate):

        l_NumberOfResolutions = len(p_L2ImageFilenamesProvider.get_sre_headers())
        LOGGER.debug("L2ImageFileReader::Initialize Number of resolutions: " + str(l_NumberOfResolutions) + ".")

        # Write Public Headers

        output_filename = ""
        # ---------------------------------------
        # Get global information about the output L2 product
        # ---------------------------------------
        lCreator_Version = self.plugin.Creator_Version
        lFile_Version = self.plugin.L2PRODUCT_FILE_VERSION
        # Get Validity
        lReferenceProductHeaderId = p_L2ImageFilenamesProvider.get_reference_product_header_id()
        lReferenceProductInstance = p_L2ImageFilenamesProvider.get_reference_product_instance()
        LOGGER.debug("p_L2ImageFilenamesProvider->GetReferenceProductHeaderId() <" +
                     p_L2ImageFilenamesProvider.get_reference_product_header_id() + ">")
        LOGGER.debug("p_L2ImageFilenamesProvider->GetReferenceProductInstance() <" +
                     p_L2ImageFilenamesProvider.get_reference_product_instance() + ">")
        LOGGER.debug("Writing xml file <" + output_filename + ">...")
        # ---------------------------------------------------------------------------------------------------
        l_CoarseProductImageSizeX = p_dem.CoarseArea.size[0]
        l_CoarseProductImageSizeY = p_dem.CoarseArea.size[1]

        l_CLDNbBits = self.plugin.NumbersOfBitsInCLDDataIOBand
        l_BandsDefinitions = self.plugin.BandsDefinitions

        # *************************************************************************************************************
        # Write headers for all resolution
        # *************************************************************************************************************
        for resol in range(l_NumberOfResolutions):
            # Resolution information
            # ---------------------------------------
            l_ProductImageSizeX = p_dem.L2Areas[resol].size[0]
            l_ProductImageSizeY = p_dem.L2Areas[resol].size[1]
            l_ResollistOfBandCode = l_BandsDefinitions.get_list_of_l2_band_code(l_BandsDefinitions.ListOfL2Resolution[resol])

            # Write Public Headers
            if self.adjacencyeffectsandslopecorrection:
                # ---------------------------------------------------------------------------------------------------
                # Write FRE
                LOGGER.debug("Write the FRE header file ...")
                output_filename = p_L2ImageFilenamesProvider.get_fre_headers()[resol]

                current_header_filename = os.path.join(p_root_template_directory, self.plugin.TEMPLATE_PDTIMG_HDR)
                output_handler = EarthExplorerXMLFileHandler(current_header_filename)
                if not os.path.exists(current_header_filename):
                    raise MajaDataException(
                        "Internal error: the template file '" +
                        current_header_filename +
                        "' doesn't exist !!")
                LOGGER.debug("Update file ...")
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
                    l_ProductImageSizeX,
                    l_ProductImageSizeY,
                    lReferenceProductInstance,
                    self.productisvalid)
                output_handler.set_string_value("//Nodata_Value", self.nodatavalue)
                output_handler.set_list_of_bands(l_ResollistOfBandCode)  # , "//List_of_Bands", "Band")
                output_handler.set_string_value("//Bands", str(len(l_ResollistOfBandCode)))
                # Save File
                LOGGER.debug("SaveFile file ...")
                output_handler.save_to_file(output_filename)
                if self.checkxmlfileswithschema and self.productisvalid:
                    xml_tools.check_xml(output_filename, p_SchemaLocationDirectory)

            # ---------------------------------------------------------------------------------------------------
            # Write SRE
            LOGGER.debug("Write the SRE header file ...")
            output_filename = p_L2ImageFilenamesProvider.get_sre_headers()[resol]
            LOGGER.debug("p_L2ImageFilenamesProvider->GetReferenceProductHeaderId() <" +
                         p_L2ImageFilenamesProvider.get_reference_product_header_id() + ">")
            LOGGER.debug("p_L2ImageFilenamesProvider->GetReferenceProductInstance() <" +
                         p_L2ImageFilenamesProvider.get_reference_product_instance() + ">")
            LOGGER.debug("Writing xml file <" + output_filename + ">...")
            current_header_filename = os.path.join(p_root_template_directory, self.plugin.TEMPLATE_PDTIMG_HDR)
            if not os.path.exists(current_header_filename):
                raise MajaDataException(
                    "Internal error: the template file '" + current_header_filename + "' doesn't exist !!")
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
            output_handler.update_pdtanx(p_Mission, l_ProductImageSizeX, l_ProductImageSizeY, lReferenceProductInstance,
                                         self.productisvalid)
            output_handler.set_string_value("//Nodata_Value", self.nodatavalue)
            output_handler.set_list_of_bands(l_ResollistOfBandCode)  # , "//List_of_Bands", "Band")
            output_handler.set_string_value("//Bands", str(len(l_ResollistOfBandCode)))
            LOGGER.debug("SaveFile file ...")
            # Save File
            output_handler.save_to_file(output_filename)
            if self.checkxmlfileswithschema and self.productisvalid:
                xml_tools.check_xml(output_filename, p_SchemaLocationDirectory)
            # ---------------------------------------------------------------------------------------------------
            # Write ATB
            LOGGER.debug("Write the ATB header file ...")
            output_filename = p_L2ImageFilenamesProvider.get_atb_header_filenames()[resol]
            current_header_filename = os.path.join(p_root_template_directory, self.plugin.TEMPLATE_PDTANX_ATB_HDR)
            if not os.path.exists(current_header_filename):
                raise MajaDataException(
                    "Internal error: the template file '" + current_header_filename + "' doesn't exist !!")
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
            output_handler.update_pdtanx(p_Mission, l_ProductImageSizeX, l_ProductImageSizeY, lReferenceProductInstance,
                                         self.productisvalid)
            output_handler.set_string_value("//VAP_Nodata_Value", self.vapnodatavalue)
            output_handler.set_string_value("//VAP_Quantification_Value", self.vapquantificationvalue)
            output_handler.set_string_value("//AOT_Nodata_Value", self.aotnodatavalue)
            output_handler.set_string_value("//AOT_Quantification_Value", self.aotquantificationvalue)
            # Save File
            output_handler.save_to_file(output_filename)
            if self.checkxmlfileswithschema and self.productisvalid:
                xml_tools.check_xml(output_filename, p_SchemaLocationDirectory)
            # ---------------------------------------------------------------------------------------------------
            # Write CLD
            LOGGER.debug("Write the CLD header file ...")
            output_filename = p_L2ImageFilenamesProvider.get_cld_header_filenames()[resol]
            current_header_filename = os.path.join(p_root_template_directory, self.plugin.TEMPLATE_PDTANX_CLD_HDR)
            if not os.path.exists(current_header_filename):
                raise MajaDataException(
                    "Internal error: the template file '" + current_header_filename + "' doesn't exist !!")
            output_handler = EarthExplorerXMLFileHandler(current_header_filename)
            # ---------------------------------------------------------------------------------------------
            # Update bascis parameters
            output_handler.update(p_Validity_Start, p_Validity_Stop, lFile_Version, output_filename, self.creator,
                                  lCreator_Version,
                                  self.notes, self.system, self.outputfileclass, pCurrentDate,
                                  lReferenceProductHeaderId)
            output_handler.set_string_value("//Number_of_Significant_Bits", l_CLDNbBits)
            if l_CLDNbBits <= 8:
                output_handler.set_string_value("//Data_Type", "UNSIGNED_BYTE")
            elif l_CLDNbBits <= 16:
                output_handler.set_string_value("//Data_Type", "UNSIGNED_SHORT")
            else:
                raise MajaDataException("Internal error:  CLD band must be written with [1..16] bits !")
            # ---------------------------------------------------------------------------------------------
            # Update bascis ANX parameters
            output_handler.update_pdtanx(p_Mission, l_ProductImageSizeX, l_ProductImageSizeY, lReferenceProductInstance,
                                         self.productisvalid)
            # Save File
            output_handler.save_to_file(output_filename)
            if self.checkxmlfileswithschema and self.productisvalid:
                xml_tools.check_xml(output_filename, p_SchemaLocationDirectory)
            # ---------------------------------------------------------------------------------------------------
            # Write MSK
            LOGGER.debug("Write the MSK header file ...")
            output_filename = p_L2ImageFilenamesProvider.get_msk_header_filenames()[resol]
            current_header_filename = os.path.join(p_root_template_directory, self.plugin.TEMPLATE_PDTANX_MSK_HDR)
            if not os.path.exists(current_header_filename):
                raise MajaDataException(
                    "Internal error: the template file '" + current_header_filename + "' doesn't exist !!")
            output_handler = EarthExplorerXMLFileHandler(current_header_filename)
            # ---------------------------------------------------------------------------------------------
            # Update bascis parameters
            output_handler.update(p_Validity_Start, p_Validity_Stop, lFile_Version, output_filename, self.creator,
                                  lCreator_Version,
                                  self.notes, self.system, self.outputfileclass, pCurrentDate,
                                  lReferenceProductHeaderId)
            # ---------------------------------------------------------------------------------------------
            # Update bascis ANX parameters
            output_handler.update_pdtanx(p_Mission, l_ProductImageSizeX, l_ProductImageSizeY, lReferenceProductInstance,
                                         self.productisvalid)
            # Save File
            output_handler.save_to_file(output_filename)
            if self.checkxmlfileswithschema and self.productisvalid:
                xml_tools.check_xml(output_filename, p_SchemaLocationDirectory)
            # ---------------------------------------------------------------------------------------------------
            # Write QLT
            LOGGER.debug("Write the QLT header file ...")
            output_filename = p_L2ImageFilenamesProvider.get_qlt_header_filenames()[resol]
            current_header_filename = os.path.join(p_root_template_directory, self.plugin.TEMPLATE_PDTANX_QLT_HDR)
            if not os.path.exists(current_header_filename):
                raise MajaDataException(
                    "Internal error: the template file '" + current_header_filename + "' doesn't exist !!")
            output_handler = EarthExplorerXMLFileHandler(current_header_filename)
            # ---------------------------------------------------------------------------------------------
            # Update bascis parameters
            output_handler.update(p_Validity_Start, p_Validity_Stop, lFile_Version, output_filename, self.creator,
                                  lCreator_Version,
                                  self.notes, self.system, self.outputfileclass, pCurrentDate,
                                  lReferenceProductHeaderId)
            # ---------------------------------------------------------------------------------------------
            # Update bascis ANX parameters
            output_handler.update_pdtanx(p_Mission, l_ProductImageSizeX, l_ProductImageSizeY, lReferenceProductInstance,
                                         self.productisvalid)
            # Save File
            output_handler.save_to_file(output_filename)
            if self.checkxmlfileswithschema and self.productisvalid:
                xml_tools.check_xml(output_filename, p_SchemaLocationDirectory)
        # ---------------------------------------------------------------------------------------------------
        # Write QLK
        LOGGER.debug("Write the QLK header file ...")
        output_filename = p_L2ImageFilenamesProvider.get_qlk_header_filename()
        current_header_filename = os.path.join(p_root_template_directory, self.plugin.TEMPLATE_PDTQLK_HDR)
        if not os.path.exists(current_header_filename):
            raise MajaDataException(
                "Internal error: the template file '" + current_header_filename + "' doesn't exist !!")
        output_handler = EarthExplorerXMLFileHandler(current_header_filename)
        # ---------------------------------------------------------------------------------------------
        # Update bascis parameters
        output_handler.update(p_Validity_Start, p_Validity_Stop, lFile_Version, output_filename, self.creator,
                              lCreator_Version,
                              self.notes, self.system, self.outputfileclass, pCurrentDate,
                              lReferenceProductHeaderId)
        output_handler.set_string_value("//Mission", p_Mission)
        output_handler.set_string_value("//Lines", l_CoarseProductImageSizeY)
        output_handler.set_string_value("//Columns", l_CoarseProductImageSizeX)
        # Instance_Id node
        output_handler.set_string_value("//Reference_Product_Instance", lReferenceProductInstance)
        if self.productisvalid:
            output_handler.set_string_value("//Reference_Product_Semantic", "L2VALD")
        else:
            output_handler.set_string_value("//Reference_Product_Semantic", "L2NOTV")
        # Set the band informations
        listOfBands = list()
        listOfBands.append(self.quicklookredbandcode)
        listOfBands.append(self.quicklookgreenbandcode)
        listOfBands.append(self.quicklookbluebandcode)
        output_handler.set_string_value("//Bands", "3")
        RedBandId = 0
        GreenBandId = 0
        BlueBandId = 0
        ResolutionIndex = 0
        l_ResolutionForQLK = ""
        l_BandsDefinitions.get_l2_information_for_quicklook_band_code(
            self.quicklookredbandcode,
            self.quicklookgreenbandcode,
            self.quicklookbluebandcode
        )
        l_DL2L2CoarseRatio = float(p_dem.L2Areas[ResolutionIndex].size[0]) / float(p_dem.CoarseArea.size[0])
        l_L2L2CoarseRatio = int(l_DL2L2CoarseRatio + 0.5)

        output_handler.set_string_value("//Subsampling_Factor", l_L2L2CoarseRatio)
        output_handler.set_list_of_bands(listOfBands)  # , "//List_of_Bands", "Band")
        # Save File
        output_handler.save_to_file(output_filename)
        if self.checkxmlfileswithschema and self.productisvalid:
            xml_tools.check_xml(output_filename, p_SchemaLocationDirectory)
