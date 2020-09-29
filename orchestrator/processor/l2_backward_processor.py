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

orchestrator.processor.base_processor -- shortdesc

orchestrator.processor.base_processor is the base of all processors

It defines method mandatory for a processor

###################################################################################################
"""
import orchestrator.plugins.common.factory.product_utils as product_utils
from orchestrator.processor.l2_processor import L2Processor
from orchestrator.common.logger.maja_logging import configure_logger
from orchestrator.common.interfaces.maja_ozone_extract import get_ozone_amount
from orchestrator.plugins.common.factory.maja_l1_image_reader_provider import L1ImageReaderProvider
from orchestrator.plugins.common.factory.maja_plugin_provider import MAJAPluginProvider
from orchestrator.plugins.common.factory.maja_l2_image_writer_provider import L2ImageWriterProvider
from orchestrator.plugins.common.factory.maja_l2_image_reader_provider import L2ImageReaderProvider
import orchestrator.common.constants as constants
from orchestrator.common.earth_explorer.gipp_l2_comm_earth_explorer_xml_file_handler import GippL2COMMEarthExplorerXMLFileHandler
from orchestrator.common.earth_explorer.earth_explorer_xml_file_handler import EarthExplorerXMLFileHandler
import orchestrator.common.gipp_utils as gipp_utils
import orchestrator.common.date_utils as date_utils
from orchestrator.common.maja_exceptions import *
from orchestrator.common.xml_tools import translate_xsl
import orchestrator.common.file_utils as file_utils
import os
LOGGER = configure_logger(__name__)
from orchestrator.common.constants import CAMSStatus


class L2BackwardProcessor(L2Processor):

    class OzoneParameterValue:
        def __init__(self):
            self.Filename = ""
            self.JulianDayDateStart = 0
            self.JulianDayDateStop = 0

    def __init__(self, apphandler):
        self._name = "L2_BACKWARD"
        self._listofl1product = []
        self._listofozoneparametervalue = []
        super(L2BackwardProcessor, self).__init__(apphandler)

    def pre_processing(self):
        LOGGER.info("Starting Preprocessing")
        self._listofl1product = product_utils.get_list_of_input_l1_image_product_sorted(self._apphandler.get_input_directory(),
                                                                                        tile_id=self._apphandler.get_tile_id())
        self.init_list_of_sat()

        LOGGER.info("There are " + str(len(self._listofl1product)) + " L1 images Products")
        for p in self._listofl1product:
            LOGGER.info("- " + p.ProductFileName)
            self.activate_this_sat(p.Satellite, p.PluginName, p.UniqueSatellite)

        # Call superclass
        super(L2BackwardProcessor, self).pre_processing()

        # If there are METDTA filename, init the parameters values
        ListOfOzone = gipp_utils.get_list_of_gipp_filenames(
            self._apphandler.get_input_directory(), "EXO_METDTA", sort=True)
        LOGGER.debug("Numbers of ozone EXO_METDTA found: " + str(len(ListOfOzone)))
        # Set the member variable of list of Ozone parameter value
        self._listofozoneparametervalue = L2BackwardProcessor.initialize_list_of_ozone_parameter_value(ListOfOzone)

        LOGGER.info("Ending Preprocessing")

    def scientific_processing(self):
        LOGGER.info("Starting ScientifiqProcessing")
        # Scientific processing mode. Set to INIT, NOMINAL or BACKWARD value
        l_NumberOfL1Product = len(self._listofl1product)
        LOGGER.debug(self._listofl1product)
        # Check if more than one valid input is in the list
        if l_NumberOfL1Product == 1:
            raise MajaDataException("L2BackwardProcessing need more than one input L1 image product.")
        # Process the last product in the list with INIT MODE
        # Initialize the mode
        l_InitMode = True
        l_BackwardMode = True
        l_IndexOfL1Product = l_NumberOfL1Product
        l_finalize_backward = False

        l_EnableL2ResolutionProcessing = self._apphandler.get_user_conf().get_Computing().get_EnableL2ResolutionProcessing()
        if not l_EnableL2ResolutionProcessing:
            LOGGER.info("*************************************************************************************************")
            LOGGER.info("ATTENTION: L2BACKWARD PROCESSING: The option to Write product to L2 resolution has been DISABLE !!!")
            LOGGER.info("**************************************************************************************************")

        # ----------------------------------------------------------------
        # Initialize the flag to validate the clouds probably
        # ----------------------------------------------------------------
        l_checking_conditional_clouds = [False]
        l_conditional_loop = False
        l_write_earthexplorer_headers = False
        l_write_temporary_l2_product = True
        l_write_l2_product_to_l2resolution = False

        l_InputL2ImageFilename = ""
        while not l_conditional_loop:
            l_InputL1ProductInfo = self._listofl1product[l_IndexOfL1Product - 1]
            l_Sat = l_InputL1ProductInfo.Satellite
            # If last L1 product, write headers and write in the output directory
            # ----------------------------------------------------------------
            if l_IndexOfL1Product == 1:
                l_write_earthexplorer_headers = True
                l_write_l2_product_to_l2resolution = l_EnableL2ResolutionProcessing
                l_write_temporary_l2_product = False
                l_finalize_backward = True

                LOGGER.warn(
                    "L2Backward processing: the input L1 product valid for the Init mode is the last product. The Standard mode is disable !")

            # *************************************************************************************************************
            # Init the current PluginBase pointer for the satellite in the MAIN global DataApplicationHandler Instance
            # *************************************************************************************************************
            self.plugin = MAJAPluginProvider.create(l_InputL1ProductInfo.PluginName, self._apphandler)

            """ ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** **  **
                    Get and registers the L2COMM GIPP file  L2COMM
                    parameters have been read in the L1ImaegFileReader !!
                     ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** *"""
            l_GIP_L2COMM_Filename = gipp_utils.get_gipp_filename_with_mission(self._apphandler.get_input_directory(),
                                                                              "GIP_L2COMM", l_Sat)
            if self._apphandler._stylesheet is not None:
                LOGGER.debug("Overloading L2COMM parameters...")
                l_GIP_L2COMM_Filename = file_utils.copy_file_to_directory(l_GIP_L2COMM_Filename,
                                                                          self._apphandler.get_working_directory(), notestmode=True)
                translate_xsl(l_GIP_L2COMM_Filename, self._apphandler._stylesheet)

            LOGGER.info(
                "The GIP_L2COMM file detected for the satellite '" + l_Sat + "' is <" + l_GIP_L2COMM_Filename + ">.")
            l_GIPPL2COMMHandler = GippL2COMMEarthExplorerXMLFileHandler(l_GIP_L2COMM_Filename)
            """ ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** **  **
            Register the GIPP file "GIP_L2SITE" */
             ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** *"""
            l_GIP_L2SITE_Filename = gipp_utils.get_gipp_filename_with_mission(self._apphandler.get_input_directory(),
                                                                              "GIP_L2SITE",
                                                                              l_InputL1ProductInfo.UniqueSatellite)
            if self._apphandler._stylesheet is not None:
                LOGGER.debug("Overloading L2SITE parameters...")
                l_GIP_L2SITE_Filename = file_utils.copy_file_to_directory(l_GIP_L2SITE_Filename,
                                                                          self._apphandler.get_working_directory(), notestmode=True)
                translate_xsl(l_GIP_L2SITE_Filename, self._apphandler._stylesheet)


            LOGGER.info(
                "The GIP_L2SITE file detected for the satellite '" +
                l_InputL1ProductInfo.UniqueSatellite +
                "' is <" +
                l_GIP_L2SITE_Filename +
                ">.")

            # *************************************************************************************************************
            # Initialize the L1 Image file reader
            # *************************************************************************************************************
            l_UniqueSatelliteD = l_InputL1ProductInfo.UniqueSatellite
            l1_image_reader = L1ImageReaderProvider.create(
                l_InputL1ProductInfo.PluginName,
                l_InputL1ProductInfo,
                self._apphandler,
                l_GIPPL2COMMHandler,
                self.DataDEMMap.get(l_UniqueSatelliteD),
                constants.ReadL1Mode.READ_L1_MODE_FOR_ALGORITHMS)

            # *************************************************************************************************************
            # Initialize the L2 Image file writer
            # *************************************************************************************************************
            if self._apphandler.get_output_plugin() is not None:
                l2_image_file_writer = L2ImageWriterProvider.create(self._apphandler.get_output_plugin())
            else:
                l2_image_file_writer = L2ImageWriterProvider.create(
                    MAJAPluginProvider.auto_tm(l_InputL1ProductInfo.PluginName))
            if not l2_image_file_writer.can_write(l_InputL1ProductInfo.PluginName):
                raise MajaDriverException(
                    "Plugin '" + self._apphandler.get_output_plugin() + "' can not write product from '" + l_InputL1ProductInfo.PluginName + "' products ")

            # Log system infos
            LOGGER.info(self._apphandler.get_system_infos())
            # Compute the Single product
            # ----------------------------------------------------------------
            self.single_product_processing(
                l_InitMode,
                l_BackwardMode,
                l_finalize_backward,
                l_InputL1ProductInfo,
                l1_image_reader,
                l_write_earthexplorer_headers,
                l_write_temporary_l2_product,
                l_write_l2_product_to_l2resolution,
                None,
                l_checking_conditional_clouds,
                l2_image_file_writer,
                l_GIPPL2COMMHandler)
            LOGGER.debug("  ->  self._productIsValid                     = " + str(self._productIsValid))
            LOGGER.debug("  ->  p_checking_conditional_clouds        = " + str(l_checking_conditional_clouds[0]))
            if (l_checking_conditional_clouds[0] is True) and (self._productIsValid is True):
                l_conditional_loop = True
            elif l_IndexOfL1Product == 1:
                l_conditional_loop = True
            else:
                l_conditional_loop = False

            # Store the output file name for the following loop
            # ----------------------------------------------------------------
            l_InputL2ImageFilename = l2_image_file_writer.get_global_product_filename()

            # Decrement the input L1 index product
            l_IndexOfL1Product = l_IndexOfL1Product - 1

            # Log system infos
            LOGGER.info(self._apphandler.get_system_infos())

        # Check the Init mode
        # ----------------------------------------------------------------

        # LAIG-DM-MAC-143599-CS
        # LAIG-FA-MAC-1658-CNES
        if (l_checking_conditional_clouds[0] == False) or (self._productIsValid == False):
            LOGGER.warn(
                "The INIT mode in Backward processing fail: all L1 products are not valid (too cloudy or too cirrus)"
                " or the output L2 product generated contains too NoData values! "
                "See previous [W] messages for more details.")

        # *************************************************************************************************************
        # *************************************************************************************************************
        # Process the Nominal mode
        # *************************************************************************************************************
        # *************************************************************************************************************
        if l_IndexOfL1Product > 0:
            l_InitMode = False
            l_write_earthexplorer_headers = False
            l_write_l2_product_to_l2resolution = False
            l_write_temporary_l2_product = True

            while l_IndexOfL1Product > 1:
                l_InputL1ProductInfo = self._listofl1product[l_IndexOfL1Product - 1]
                l_Sat = l_InputL1ProductInfo.Satellite

                # *************************************************************************************************************
                # Init the current PluginBase pointer for the satellite in the MAIN global DataApplicationHandler Instance
                # *************************************************************************************************************
                plugin = MAJAPluginProvider.create(l_InputL1ProductInfo.PluginName, self._apphandler)

                """ ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** **  **
                        Get and registers the L2COMM GIPP file  L2COMM
                        parameters have been read in the L1ImaegFileReader !!
                         ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** *"""
                l_GIP_L2COMM_Filename = gipp_utils.get_gipp_filename_with_mission(self._apphandler.get_input_directory(),
                                                                                  "GIP_L2COMM", l_Sat)
                if self._apphandler._stylesheet is not None:
                    LOGGER.debug("Overloading L2COMM parameters...")
                    translate_xsl(l_GIP_L2COMM_Filename, self._apphandler._stylesheet)

                LOGGER.info("The GIP_L2COMM file detected for the satellite '" +
                            l_Sat + "' is <" + l_GIP_L2COMM_Filename + ">.")
                l_GIPPL2COMMHandler = GippL2COMMEarthExplorerXMLFileHandler(l_GIP_L2COMM_Filename)
                """ ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** **  **
                Register the GIPP file "GIP_L2SITE" */
                 ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** *"""
                l_GIP_L2SITE_Filename = gipp_utils.get_gipp_filename_with_mission(self._apphandler.get_input_directory(),
                                                                                  "GIP_L2SITE",
                                                                                  l_InputL1ProductInfo.UniqueSatellite)
                if self._apphandler._stylesheet is not None:
                    LOGGER.debug("Overloading L2COMM parameters...")
                    translate_xsl(l_GIP_L2SITE_Filename, self._apphandler._stylesheet)

                LOGGER.info(
                    "The GIP_L2SITE file detected for the satellite '" +
                    l_InputL1ProductInfo.UniqueSatellite +
                    "' is <" +
                    l_GIP_L2SITE_Filename +
                    ">.")

                # *************************************************************************************************************
                # Initialize the L1 Image file reader
                # *************************************************************************************************************
                l_UniqueSatelliteD = l_InputL1ProductInfo.UniqueSatellite
                l1_image_reader = L1ImageReaderProvider.create(l_InputL1ProductInfo.PluginName, l_InputL1ProductInfo,
                                                               self._apphandler, l_GIPPL2COMMHandler,
                                                               self.DataDEMMap.get(l_UniqueSatelliteD),
                                                               constants.ReadL1Mode.READ_L1_MODE_FOR_ALGORITHMS)

                # *************************************************************************************************************
                # Initialize the L2 Image file reader
                # *************************************************************************************************************
                LOGGER.debug(
                    "Starting L2NominalProcessor::ScientificProcessing(): get the L2 product filename in the input directory...")
                l2_image_reader = L2ImageReaderProvider.create(l_InputL2ImageFilename, self._apphandler, False)
                LOGGER.info("The L2 product found is <" + l2_image_reader.ProductFilename + ">.")

                # *************************************************************************************************************
                # Initialize the L2 Image file writer
                # *************************************************************************************************************
                if self._apphandler.get_output_plugin() is not None:
                    l2_image_file_writer = L2ImageWriterProvider.create(self._apphandler.get_output_plugin())
                else:
                    l2_image_file_writer = L2ImageWriterProvider.create(
                        MAJAPluginProvider.auto_tm(l_InputL1ProductInfo.PluginName))
                if not l2_image_file_writer.can_write(l_InputL1ProductInfo.PluginName):
                    raise MajaDriverException(
                        "Plugin '" + self._apphandler.get_output_plugin() + "' can not write product from '" + l_InputL1ProductInfo.PluginName + "' products ")
                # Log system infos
                LOGGER.info(self._apphandler.get_system_infos())
                # Compute the L2 temporary product
                # ----------------------------------------------------------------
                self.single_product_processing(
                    l_InitMode,
                    l_BackwardMode,
                    l_finalize_backward,
                    l_InputL1ProductInfo,
                    l1_image_reader,
                    l_write_earthexplorer_headers,
                    l_write_temporary_l2_product,
                    l_write_l2_product_to_l2resolution,
                    l2_image_reader,
                    l_checking_conditional_clouds,
                    l2_image_file_writer,
                    l_GIPPL2COMMHandler)

                # Store the output file name for the following loop
                # ----------------------------------------------------------------
                l_InputL2ImageFilename = l2_image_file_writer.get_global_product_filename()

                # Decrement Index of L1 image input
                # ----------------------------------------------------------------
                l_IndexOfL1Product = l_IndexOfL1Product - 1

                # Log system infos
                LOGGER.info(self._apphandler.get_system_infos())

            # Finalize the product
            # Compute the last L2 product
            # ----------------------------------------------------------------
            l_write_earthexplorer_headers = True
            l_write_l2_product_to_l2resolution = self._apphandler.get_user_conf().get_Computing().get_EnableL2ResolutionProcessing()
            l_write_temporary_l2_product = False
            l_finalize_backward = True
            l_InputL1ProductInfo = self._listofl1product[l_IndexOfL1Product - 1]
            l_Sat = l_InputL1ProductInfo.Satellite
            # *************************************************************************************************************
            # Init the current PluginBase pointer for the satellite in the MAIN global DataApplicationHandler Instance
            # *************************************************************************************************************
            plugin = MAJAPluginProvider.create(l_InputL1ProductInfo.PluginName, self._apphandler)

            """ ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** **  **
                    Get and registers the L2COMM GIPP file  L2COMM
                    parameters have been read in the L1ImaegFileReader !!
                     ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** *"""
            l_GIP_L2COMM_Filename = gipp_utils.get_gipp_filename_with_mission(self._apphandler.get_input_directory(),
                                                                              "GIP_L2COMM", l_Sat)
            if self._apphandler._stylesheet is not None:
                LOGGER.debug("Overloading L2COMM parameters...")
                translate_xsl(l_GIP_L2COMM_Filename, self._apphandler._stylesheet)
            LOGGER.info(
                "The GIP_L2COMM file detected for the satellite '" + l_Sat + "' is <" + l_GIP_L2COMM_Filename + ">.")
            l_GIPPL2COMMHandler = GippL2COMMEarthExplorerXMLFileHandler(l_GIP_L2COMM_Filename)
            """ ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** **  **
            Register the GIPP file "GIP_L2SITE" */
             ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** *"""
            l_GIP_L2SITE_Filename = gipp_utils.get_gipp_filename_with_mission(self._apphandler.get_input_directory(),
                                                                              "GIP_L2SITE",
                                                                              l_InputL1ProductInfo.UniqueSatellite)
            if self._apphandler._stylesheet is not None:
                LOGGER.debug("Overloading L2COMM parameters...")
                translate_xsl(l_GIP_L2SITE_Filename, self._apphandler._stylesheet)
            LOGGER.info(
                "The GIP_L2SITE file detected for the satellite '" +
                l_InputL1ProductInfo.UniqueSatellite +
                "' is <" +
                l_GIP_L2SITE_Filename +
                ">.")

            # *************************************************************************************************************
            # Initialize the L1 Image file reader
            # *************************************************************************************************************
            l_UniqueSatelliteD = l_InputL1ProductInfo.UniqueSatellite
            l1_image_reader = L1ImageReaderProvider.create(l_InputL1ProductInfo.PluginName, l_InputL1ProductInfo,
                                                           self._apphandler, l_GIPPL2COMMHandler,
                                                           self.DataDEMMap.get(l_UniqueSatelliteD),
                                                           constants.ReadL1Mode.READ_L1_MODE_FOR_ALGORITHMS)

            # *************************************************************************************************************
            # Initialize the L2 Image file reader
            # *************************************************************************************************************
            LOGGER.debug(
                "Starting L2NominalProcessor::ScientificProcessing(): get the L2 product filename in the input directory...")
            l2_image_reader = L2ImageReaderProvider.create(l_InputL2ImageFilename, self._apphandler, False)
            LOGGER.info("The L2 product found is <" + l2_image_reader.ProductFilename + ">.")

            # *************************************************************************************************************
            # Initialize the L2 Image file writer
            # *************************************************************************************************************
            if self._apphandler.get_output_plugin() is not None:
                l2_image_file_writer = L2ImageWriterProvider.create(self._apphandler.get_output_plugin())
            else:
                l2_image_file_writer = L2ImageWriterProvider.create(
                    MAJAPluginProvider.auto_tm(l_InputL1ProductInfo.PluginName))
            if not l2_image_file_writer.can_write(l_InputL1ProductInfo.PluginName):
                raise MajaDriverException(
                    "Plugin '" + self._apphandler.get_output_plugin() + "' can not write product from '" + l_InputL1ProductInfo.PluginName + "' products ")

            # Log system infos
            LOGGER.info(self._apphandler.get_system_infos())
            self.single_product_processing(
                l_InitMode,
                l_BackwardMode,
                l_finalize_backward,
                l_InputL1ProductInfo,
                l1_image_reader,
                l_write_earthexplorer_headers,
                l_write_temporary_l2_product,
                l_write_l2_product_to_l2resolution,
                l2_image_reader,
                l_checking_conditional_clouds,
                l2_image_file_writer,
                l_GIPPL2COMMHandler)

        # end if it is not the last product

        else:
            LOGGER.debug("L1ImageInformationProvider initialized during backward but no associated valid L2 product. No more products.")

        LOGGER.info("L2BackwardProcessor::ScientificProcessing() done.")
        # Log system infos
        LOGGER.info(self._apphandler.get_system_infos())

    def single_product_processing(self, initmode, backwardmode, p_finalize_backward, p_L1ImageInformationsProvider,
                                p_InputL1ImageFileReader, p_write_earthexplorer_headers, p_write_temporary_l2_product,
                                p_write_l2_product_to_l2resolution, p_InputL2ImageFileReader,
                                checking_conditional_clouds, p_OutputL2ImageFileWriter, p_GIPPL2COMMHandler):

        l_Sat = p_L1ImageInformationsProvider.Satellite
        # ---------------------------------------------------------------------------------------------
        # Get the ozone value
        l_UseDefaultConstantOzoneAmount = self.plugin.ConfigUserCamera.get_Algorithms(). \
            get_Atmospheric_Absorption_Correction().get_Use_Default_Constant_Ozone_Amount()
        # l_UseDefaultConstantOzoneAmount = True
        l_OzoneAmountValue = 0.0
        if not l_UseDefaultConstantOzoneAmount:
            LOGGER.debug(
                "Find Ozone Value for the product date [" +
                p_L1ImageInformationsProvider.UTCValidityStart +
                "" +
                p_L1ImageInformationsProvider.UTCValidityStop +
                "]...")
            l_OzoneFileName = L2BackwardProcessor.get_valid_ozone_filename(
                self._listofozoneparametervalue, L2BackwardProcessor.get_average_product_time(
                    p_L1ImageInformationsProvider.UTCValidityStart, p_L1ImageInformationsProvider.UTCValidityStop))
            l_OzoneAmountValue = get_ozone_amount(l_OzoneFileName, p_L1ImageInformationsProvider.CenterCorner)
        else:
            l_OzoneAmountValue = float(p_GIPPL2COMMHandler.l2_comm_values["OzoneAmountDefaultValue"])
        LOGGER.info("The 'OzoneAmount' used is " + str(l_OzoneAmountValue))

        # Get the athmospheric/CAMS
        # ---------------------------------------------------------------------------------------------
        if p_GIPPL2COMMHandler.get_value_b("UseCamsData"):
            l_modelList = p_GIPPL2COMMHandler.get_value("ListOfModels").split()
            l_avgTime = date_utils.get_julianday_as_double(p_L1ImageInformationsProvider.ProductDate)
            LOGGER.info("Cams data requested")
            # Verify if cams file are available before anything
            if self._CAMS_Files_HandlersMAP[l_Sat].has_cams_data(l_avgTime):
                self._CAMS_Files_HandlersMAP[l_Sat].extract_cams_datas(
                    p_L1ImageInformationsProvider.CenterCorner.latitude,
                    p_L1ImageInformationsProvider.CenterCorner.longitude,
                    l_avgTime)
                if self._CAMS_Files_HandlersMAP[l_Sat].valid:
                    self._AthmosphericLutHandlerMap[l_Sat].set_proportions(
                        self._CAMS_Files_HandlersMAP[l_Sat].proportions)
                    LOGGER.info("CAMS found, sampling : " + str(self._CAMS_Files_HandlersMAP[l_Sat].out_rh_sampling))
                    for m in l_modelList:
                        l_use_rh = self._CAMS_Files_HandlersMAP[l_Sat].get_extinction_map()[m]["rh_dep"]
                        self._AthmosphericLutHandlerMap[l_Sat].add_lut_for_model(
                            self._apphandler.get_input_directory(),
                            self._apphandler.get_working_directory(),
                            l_Sat,
                            m,
                            self._validate_schemas,
                            os.path.join(
                                self._apphandler.get_schemas_root_install_dir(),
                                self.plugin.MAJA_INSTALL_SCHEMAS_DIR),
                            p_userh=l_use_rh,
                            p_rh=self._CAMS_Files_HandlersMAP[l_Sat].out_rh_sampling)
                    tmp_atmolut = self._apphandler.get_directory_manager().get_temporary_directory("AtmoLut_",do_always_remove=True)
                    self._AthmosphericLutHandlerMap[l_Sat].gen_synthetised_luts(tmp_atmolut)
                else:
                    LOGGER.warn("No valid cam data found for product, using constant model")
                self.update_cam_status(self._CAMS_Files_HandlersMAP[l_Sat].valid)
            else:
                LOGGER.warn("No valid cam data found for product date : "+ date_utils.get_date_yyyymmdd_from_tm(p_L1ImageInformationsProvider.ProductDate)+", using constant model")
                self.update_cam_status(False)

        # *************************************************************************************************************
        # Call the Scientifics single product processing
        # *************************************************************************************************************
        # V 4-1-0 THOMAS erreur dans l'ordre des parametres
        super(
            L2BackwardProcessor,
            self).process_one_product(
            initmode,
            backwardmode,
            p_write_l2_product_to_l2resolution,
            p_write_temporary_l2_product,
            p_write_earthexplorer_headers,
            p_finalize_backward,
            p_L1ImageInformationsProvider,
            p_InputL1ImageFileReader,
            l_OzoneAmountValue,
            self._cams_status,
            p_InputL2ImageFileReader,
            p_OutputL2ImageFileWriter,
            checking_conditional_clouds)

    def post_processing(self):
        LOGGER.info("Starting Postprocessing")

    # List ozone files
    @staticmethod
    def initialize_list_of_ozone_parameter_value(pListOfOzone):
        l_ListOfOzoneParameterValue = []
        # Loops on the ozone list
        for f in pListOfOzone:
            # Normaly, the prodcut is uncompress and XML file checked in the PreProcessing method
            l_HDRozoneFilename = os.path.splitext(f)[0] + ".HDR"
            l_OzoneParameterValueType = L2BackwardProcessor.OzoneParameterValue()
            # Load the EarthExplorer XML file
            l_Handler = EarthExplorerXMLFileHandler(l_HDRozoneFilename)
            # Converts these dates in Julian day
            l_OzoneParameterValueType.JulianDayDateStart = date_utils.get_julianday_as_double(
                date_utils.get_datetime_from_utc(l_Handler.get_string_value_of("ValidityStart")))
            l_OzoneParameterValueType.JulianDayDateStop = date_utils.get_julianday_as_double(
                date_utils.get_datetime_from_utc(l_Handler.get_string_value_of("ValidityStop")))
            l_OzoneParameterValueType.Filename = os.path.splitext(f)[0] + ".DBL"
            LOGGER.debug("Add Ozone Value for date [" + str(l_OzoneParameterValueType.JulianDayDateStart) + "" + str(
                l_OzoneParameterValueType.JulianDayDateStop) + "] -> file " + l_OzoneParameterValueType.Filename)
            l_ListOfOzoneParameterValue.append(l_OzoneParameterValueType)

        return l_ListOfOzoneParameterValue

    # Start the post-processing
    @staticmethod
    def get_valid_ozone_filename(p_ListOfOzoneParameterValue, p_averageProductTime):
        # the average date
        l_ImageProductValidityAverage_JD = p_averageProductTime
        # Loops on the ozone values
        for p in p_ListOfOzoneParameterValue:
            if ((l_ImageProductValidityAverage_JD >= p.JulianDayDateStart)
                    and (l_ImageProductValidityAverage_JD <= p.JulianDayDateStop)):
                # returns the ozone filename which is match with the average date
                return p.Filename
        # Throws an exception if the date is not found
        raise MajaException(
            "Impossible to find the validity period in list of input METDTA files provided for the product image !")

    def update_cam_status(self, p_stat):
        # updating CAMS flag regarding CAMS availability for current iteration
        # if current iteration has valid cams data
        if p_stat:
            if self._cams_status == CAMSStatus.ACTIVATED_PARTIALLYAVAILABLEDATA:
                # partially available remains partially available
                self._cams_status = CAMSStatus.ACTIVATED_PARTIALLYAVAILABLEDATA
            elif self._cams_status == CAMSStatus.ACTIVATED_NOAVAILABLEDATA:
                # not available becomes partially available
                self._cams_status = CAMSStatus.ACTIVATED_PARTIALLYAVAILABLEDATA
            else:
                # activated OK remains activated OK
                self._cams_status = CAMSStatus.ACTIVATED_OK
        else:
            if self._cams_status == CAMSStatus.ACTIVATED_PARTIALLYAVAILABLEDATA:
                # partially stays partially
                self._cams_status = CAMSStatus.ACTIVATED_PARTIALLYAVAILABLEDATA
            elif self._cams_status == CAMSStatus.ACTIVATED_OK:
                # activated OK becomes partially available
                self._cams_status = CAMSStatus.ACTIVATED_PARTIALLYAVAILABLEDATA
            else:
                # not available remains not available
                self._cams_status = CAMSStatus.ACTIVATED_NOAVAILABLEDATA
