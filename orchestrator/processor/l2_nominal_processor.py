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


from orchestrator.processor.l2_processor import L2Processor
from orchestrator.common.maja_exceptions import MajaDriverException
from orchestrator.plugins.common.factory.maja_l1_image_reader_provider import L1ImageReaderProvider
from orchestrator.plugins.common.factory.maja_plugin_provider import MAJAPluginProvider
from orchestrator.common.maja_exceptions import MajaDriverException
from orchestrator.plugins.common.factory.maja_l2_image_writer_provider import L2ImageWriterProvider
import orchestrator.plugins.common.factory.product_utils as product_utils
from orchestrator.common.earth_explorer.gipp_l2_comm_earth_explorer_xml_file_handler import GippL2COMMEarthExplorerXMLFileHandler
import orchestrator.common.gipp_utils as gipp_utils
from orchestrator.common.interfaces.maja_ozone_extract import get_ozone_amount
import orchestrator.common.constants as constants
import orchestrator.common.date_utils as date_utils
import os
from orchestrator.common.logger.maja_logging import configure_logger
LOGGER = configure_logger(__name__)
from orchestrator.common.xml_tools import translate_xsl
from orchestrator.common.constants import CAMSStatus
import orchestrator.common.file_utils as file_utils

class L2NominalProcessor(L2Processor):

    def __init__(self, apphandler):
        self._name = "L2_NOMINAL"
        self._l1product = None
        super(L2NominalProcessor, self).__init__(apphandler)

    def pre_processing(self):
        LOGGER.info("Starting Preprocessing")
        self._l1product = product_utils.get_input_l1_image_product(self._apphandler.get_input_directory(),
                                                                   tile_id=self._apphandler.get_tile_id())
        self.init_list_of_sat()
        self.activate_this_sat(self._l1product.Satellite, self._l1product.PluginName, self._l1product.UniqueSatellite)

        # Instanciate plugin
        self.plugin = MAJAPluginProvider.create(self._l1product.PluginName, self._apphandler)
        super(L2NominalProcessor, self).pre_processing()
        LOGGER.info("Ending Preprocessing")

    def scientific_processing(self):
        LOGGER.info("Starting ScientifiqProcessing")
        l_InitMode = False
        l_BackwardMode = False
        l_Sat = self._l1product.Satellite

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
                                                                          self._l1product.UniqueSatellite)

        if self._apphandler._stylesheet is not None:
            LOGGER.debug("Overloading L2SITE parameters...")
            l_GIP_L2SITE_Filename = file_utils.copy_file_to_directory(l_GIP_L2SITE_Filename,
                                                                      self._apphandler.get_working_directory(), notestmode=True)
            translate_xsl(l_GIP_L2SITE_Filename, self._apphandler._stylesheet)


        LOGGER.info(
            "The GIP_L2SITE file detected for the satellite '" +
            self._l1product.UniqueSatellite +
            "' is <" +
            l_GIP_L2SITE_Filename +
            ">.")

        # Get the athmospheric/CAMS
        # ---------------------------------------------------------------------------------------------
        if l_GIPPL2COMMHandler.get_value_b("UseCamsData"):
            l_modelList = l_GIPPL2COMMHandler.get_value("ListOfModels").split()
            l_avgTime = date_utils.get_julianday_as_double(self._l1product.ProductDate)
            LOGGER.info("Cams data requested")
            # Verify if cams file are available before anything
            if self._CAMS_Files_HandlersMAP[l_Sat].has_cams_data(l_avgTime):
                self._CAMS_Files_HandlersMAP[l_Sat].extract_cams_datas(self._l1product.CenterCorner.latitude,
                                                                       self._l1product.CenterCorner.longitude,
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
                    tmp_atmolut = self._apphandler.get_directory_manager().get_temporary_directory("AtmoLut_", do_always_remove=True)
                    self._AthmosphericLutHandlerMap[l_Sat].gen_synthetised_luts(tmp_atmolut)
                    self._cams_status = CAMSStatus.ACTIVATED_OK
                else:
                    LOGGER.warn("No valid cam data found for product, using constant model")
                    self._cams_status = CAMSStatus.ACTIVATED_NOAVAILABLEDATA
            else:
                LOGGER.warn("No valid cam data found for product at time "+ date_utils.get_date_yyyymmdd_from_tm(self._l1product.ProductDate)+", using constant model")
                self._cams_status = CAMSStatus.ACTIVATED_NOAVAILABLEDATA

        # ---------------------------------------------------------------------------------------------
        # Initialize L1 image reader
        l_UniqueSatelliteD = self._l1product.UniqueSatellite
        l1_image_reader = L1ImageReaderProvider.create(
            self._l1product.PluginName,
            self._l1product,
            self._apphandler,
            l_GIPPL2COMMHandler,
            self.DataDEMMap.get(l_UniqueSatelliteD),
            constants.ReadL1Mode.READ_L1_MODE_FOR_ALGORITHMS)

        # ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** **
        # Initialize the L2 Image file reader
        # Only in Nominal and Backward mode
        # ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** **
        LOGGER.debug(
            "Starting L2NominalProcessor::ScientificProcessing(): get the L2 product filename in the input directory...")
        l2_image_reader = product_utils.get_input_l2_image_product(
            self._apphandler.get_input_directory(), self._apphandler, False)
        LOGGER.info("The L2 product found is <" + l2_image_reader.ProductFilename + ">.")

        # ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** **
        # Initialize the Output L2 Image product filename
        if self._apphandler.get_output_plugin() is not None:
            l2_image_file_writer = L2ImageWriterProvider.create(self._apphandler.get_output_plugin())
        else:
            l2_image_file_writer = L2ImageWriterProvider.create(MAJAPluginProvider.auto_tm(self._l1product.PluginName))
        if not l2_image_file_writer.can_write(self._l1product.PluginName):
            raise MajaDriverException(
                "Plugin '" + self._apphandler.get_output_plugin() + "' can not write product from '" + self._l1product.PluginName + "' products ")

        # ---------------------------------------------------------------------------------------------
        # Get the ozone value
        l_UseDefaultConstantOzoneAmount = self.plugin.ConfigUserCamera.get_Algorithms(). \
            get_Atmospheric_Absorption_Correction().get_Use_Default_Constant_Ozone_Amount()
        # l_UseDefaultConstantOzoneAmount = True
        l_OzoneAmountValue = 0.0
        if not l_UseDefaultConstantOzoneAmount:
            l_OzoneAmountValue = get_ozone_amount(
                gipp_utils.get_gipp_filename(self._apphandler.get_input_directory(), "EXO_METDTA"),
                self._l1product.CenterCorner)
        else:
            l_OzoneAmountValue = float(l_GIPPL2COMMHandler.l2_comm_values["OzoneAmountDefaultValue"])
        LOGGER.info("The 'OzoneAmount' used is " + str(l_OzoneAmountValue))

        # Log system infos
        LOGGER.info(self._apphandler.get_system_infos())

        # check conditional clouds ?
        checking_conditional_clouds = [True]
        LOGGER.info("Starting from L2InitProcessor, the main ScientificSingleProductProcessing() method...")
        l_enableL2resolution = self._apphandler.get_user_conf().get_Computing().get_EnableL2ResolutionProcessing()
        super(L2NominalProcessor, self).process_one_product(l_InitMode, l_BackwardMode, l_enableL2resolution,
                                                            False, True, True, self._l1product, l1_image_reader,
                                                            l_OzoneAmountValue, self._cams_status, l2_image_reader,
                                                            l2_image_file_writer, checking_conditional_clouds)
        LOGGER.info("Ending ScientifiqProcessing")
        # Log system infos
        LOGGER.info(self._apphandler.get_system_infos())

    def post_processing(self):
        LOGGER.info("Starting Postprocessing")
