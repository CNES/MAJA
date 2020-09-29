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
# Algo import
import orchestrator.processor.l2_pipelines
from orchestrator.modules.maja_module import MajaModule
from orchestrator.common.interfaces.maja_athmospheric_lut_handler import AthmosphericLutHandler
from orchestrator.common.interfaces.maja_cams_files_handler import CAMSFileHandler
from orchestrator.cots.otb.otb_app_handler import OtbAppHandler
from orchestrator.processor.base_processor import BaseProcessor
from orchestrator.plugins.common.factory.maja_plugin_provider import MAJAPluginProvider
import orchestrator.processor.l2_processor_bands_setup as l2_processor_bands_setup
import orchestrator.processor.l2_processor_image_writer_setup as l2_processor_image_writer_setup
import orchestrator.processor.l2_processor_header_writer_setup as l2_processor_header_writer_setup
from orchestrator.common.dem.dem_base import DEMBase
from orchestrator.common.maja_exceptions import *
from orchestrator.common.constants import AOTEstimation
from orchestrator.common.interfaces.maja_lut_converter import LutConverter
from orchestrator.common.interfaces.maja_smac_converter import SmacConverter
import orchestrator.common.gipp_utils as gipp_utils
import orchestrator.common.file_utils as file_utils
import orchestrator.common.date_utils as date_utils
import orchestrator.common.xml_tools as xml_tools
from orchestrator.common.earth_explorer.gipp_l2_site_earth_explorer_xml_file_handler import \
    GippL2SITEEarthExplorerXMLFileHandler
from orchestrator.common.earth_explorer.gipp_l2_comm_earth_explorer_xml_file_handler import \
    GippL2COMMEarthExplorerXMLFileHandler
from orchestrator.common.earth_explorer.earth_explorer_xml_file_handler import EarthExplorerXMLFileHandler
from orchestrator.cots.otb.algorithms.otb_stats import stats
from orchestrator.launcher.app_handler import AppHandler
import os
import math
from orchestrator.common.logger.maja_logging import configure_logger
from orchestrator.plugins.common.factory.maja_l2_header_writer_provider import L2HeaderWriterProvider


LOGGER = configure_logger(__name__)


class L2Processor(BaseProcessor):

    def __init__(self, apphandler):
        self._name = "L2PROCESSOR"
        self.DataDEMMap = {}
        self._GIP_L2WATV_LookUpTableConverterMap = {}
        self._GIP_L2SMAC_ListOfCoeffsConverterMap = {}
        self._AthmosphericLutHandlerMap = {}
        self._CAMS_Files_HandlersMAP = {}
        self.plugin = None

        super(L2Processor, self).__init__(apphandler)

        # Get the thread value
        self._nbThreads = self._apphandler.get_nb_threads()
        os.environ["ITK_GLOBAL_DEFAULT_NUMBER_OF_THREADS"] = str(self._nbThreads)

        LOGGER.info("Using " + str(self._nbThreads) + " Threads")
        # Get the ram value
        if self._apphandler.get_user_conf().get_Computing().get_RAM() is not None:
            self._ram = self._apphandler.get_user_conf().get_Computing().get_RAM()
        else:
            self._ram = AppHandler.DEFAULT_RAM_PER_THREAD * self._nbThreads
        LOGGER.info("Using " + str(self._ram) + " MB of RAM")
        # Set ram to use in otb apps
        OtbAppHandler.set_ram_to_use(self._ram)
        os.environ["OTB_MAX_RAM_HINT"] = str(self._ram)
        os.environ["GDAL_CACHEMAX"] = str(max(64,self._ram/20))
        self._validate_schemas = self._apphandler.get_user_conf().get_Processing().get_CheckXMLFilesWithSchema()
        LOGGER.info("Validating xml : " + str(self._validate_schemas))

    def pre_processing(self):
        LOGGER.info("Starting Preprocessing")
        list_of_unique_sat = self.get_list_of_unique_sat_activated()
        # Init DEM
        for sat in list_of_unique_sat:
            LOGGER.debug("Searching DEM for " + sat)
            dem_filename = gipp_utils.get_gipp_filename_with_mission(self._apphandler.get_input_directory(), "AUX_REFDE2",
                                                                     sat)
            LOGGER.info("DEM found : " + dem_filename)
            dem = DEMBase()
            l_plugin = MAJAPluginProvider.create_with_unique_sat(sat, self._apphandler)
            dem_working = self._apphandler.get_directory_manager().get_temporary_directory("DTMRead_",do_always_remove=False)
            dem.initialize(dem_filename, dem_working, l_plugin.SnowMasking)
            self.DataDEMMap[sat] = dem

        # Init ozone files
        list_of_meteo_files = gipp_utils.get_list_of_gipp_filenames(self._apphandler.get_input_directory(), "EXO_METDTA")
        LOGGER.info("There is(are) " + str(len(list_of_unique_sat)) + " meteo files (EXO METDTA)")
        for met in list_of_meteo_files:
            met_dbl = os.path.splitext(met)[0] + ".DBL"
            met_dbl = os.path.splitext(met)[0] + ".DBL"
            gipp_utils.uncompress_dbl_product(met_dbl)
            # retrieve the mission in HDR file
            if self._validate_schemas:
                met_hdr = os.path.splitext(met)[0] + ".HDR"
                mission = EarthExplorerXMLFileHandler(met_hdr).get_string_value_of("Mission")
                plugin_base = MAJAPluginProvider.create_with_unique_sat(mission, self._apphandler)
                xml_tools.check_xml(met_hdr, os.path.join(self._apphandler.get_schemas_root_install_dir(),
                                                          plugin_base.MAJA_INSTALL_SCHEMAS_DIR))

        # *************************************************************************************************************
        # Get all EXO_CAMS, regardless the satellite
        # START to UNCOMPRESS the .DBL file and CHECK the .HDR file with the schema
        # *************************************************************************************************************
        l_ListOfCAMSDataFilenames = gipp_utils.get_list_of_gipp_filenames(
            self._apphandler.get_input_directory(), "EXO_CAMS", True)
        # The list contains .DBL extension filenames !
        l_NbpListOfCAMSDataFilenames = len(l_ListOfCAMSDataFilenames)
        LOGGER.debug("Numbers of cams EXO_CAMS found: " + str(l_NbpListOfCAMSDataFilenames))
        LOGGER.info(
            "In the input data directory, there is(are) " +
            str(l_NbpListOfCAMSDataFilenames) +
            " cams filename (EXO_CAMS).")
        if l_NbpListOfCAMSDataFilenames > 0:
            LOGGER.info("Starting uncompress the EXO_CAMS data...")

        # ---------------------------------------------------------------------------------------------
        # Loops on the cams list
        for cam in l_ListOfCAMSDataFilenames:
            l_cams_dbl = os.path.splitext(cam)[0] + ".DBL"
            # Uncompresses the product
            gipp_utils.uncompress_dbl_product(l_cams_dbl)
            # Checks the ozone xml filename with the dedicated schema
            if self._validate_schemas:
                l_cams_hdr = os.path.splitext(cam)[0] + ".HDR"
                mission = EarthExplorerXMLFileHandler(l_cams_hdr).get_string_value_of("Mission")
                plugin_base = MAJAPluginProvider.create_with_unique_sat(mission, self._apphandler)
                xml_tools.check_xml(l_cams_hdr, os.path.join(self._apphandler.get_schemas_root_install_dir(),
                                                             plugin_base.MAJA_INSTALL_SCHEMAS_DIR))

        """ ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** **
        ** ** *START READ AND PREPARE DATAS FOR SCIENTIFICS CALCULATION ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** **
        ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** **"""
        l_constant_model_lut_map = {}
        for satkey in self._listofenabledsat:
            l_sat = self.get_associated_sat(satkey)
            l_plugin = MAJAPluginProvider.create(self.get_associated_plugin(satkey), self._apphandler)
            l_plugin_schema_dir = os.path.join(self._apphandler.get_schemas_root_install_dir(),
                                               l_plugin.MAJA_INSTALL_SCHEMAS_DIR)
            LOGGER.info(
                "Starting reading data for sat " + l_sat + " with the plugin " + self.get_associated_plugin(satkey))

            # Read L2SMAC
            gip_smac_filename = gipp_utils.get_gipp_filename_with_mission(self._apphandler.get_input_directory(),
                                                                          "GIP_L2SMAC", l_sat)
            smac_converter = SmacConverter(self._apphandler.get_working_directory(), gip_smac_filename,
                                           validate_schema=self._validate_schemas, schema_path=l_plugin_schema_dir)
            self._GIP_L2SMAC_ListOfCoeffsConverterMap[l_sat] = smac_converter
            LOGGER.debug("SMAC filename : " + gip_smac_filename)

            # Reader L2COMM
            gip_l2comm_filename = gipp_utils.get_gipp_filename_with_mission(self._apphandler.get_input_directory(),
                                                                            "GIP_L2COMM", l_sat)
            #Translate xsl ?
            if self._apphandler.get_stylesheet() is not None:
                gip_l2comm_filename = file_utils.copy_file_to_directory(gip_l2comm_filename, self._apphandler.get_working_directory(),notestmode=True)
                xml_tools.translate_xsl(gip_l2comm_filename, self._apphandler.get_stylesheet())
            LOGGER.debug("L2COMM filename : " + gip_l2comm_filename)
            if self._validate_schemas:
               xml_tools.check_xml(gip_l2comm_filename, l_plugin_schema_dir)
            # Load the l2comm
            l_gippl2commhandler = GippL2COMMEarthExplorerXMLFileHandler(gip_l2comm_filename)

            # For now it is hard coded
            if l_plugin.WaterVapourDetermination:

                if not l_gippl2commhandler.get_value_b("GIP_L2COMM_UseDefaultConstantWaterAmount"):
                    # Reader L2WATV
                    gip_l2watv_filename = gipp_utils.get_gipp_filename_with_mission(
                        self._apphandler.get_input_directory(), "GIP_L2WATV", l_sat)
                    LOGGER.debug("L2WATV filename : " + gip_l2watv_filename)
                    l2watv_dbl = os.path.splitext(gip_l2watv_filename)[0] + ".DBL"
                    gipp_utils.uncompress_dbl_product(l2watv_dbl)
                    watv_converter = LutConverter(
                        self._apphandler.get_working_directory(),
                        gip_l2watv_filename,
                        validate_schema=self._validate_schemas,
                        schema_path=l_plugin_schema_dir)
                    self._GIP_L2WATV_LookUpTableConverterMap[l_sat] = watv_converter

            # Athmospheric luts reader
            self._AthmosphericLutHandlerMap[l_sat] = AthmosphericLutHandler()
            l_constant_model = l_gippl2commhandler.get_value("ConstantModel")
            self._AthmosphericLutHandlerMap[l_sat].add_lut_for_model(
                self._apphandler.get_input_directory(),
                self._apphandler.get_working_directory(),
                l_sat,
                l_constant_model,
                self._validate_schemas,
                l_plugin_schema_dir)

            # Init cams file list
            if l_gippl2commhandler.get_value_b("UseCamsData"):
                LOGGER.info("Cams data requested")
                l_limaot = l_gippl2commhandler.get_value_f("LimAOT")
                l_rhsampling = l_gippl2commhandler.get_value("RHSampling").split()
                l_models = l_gippl2commhandler.get_value("ListOfModels").split()
                l_time_window = l_gippl2commhandler.get_value_f("TimeWindowsCAMS")
                tmp_cams_dir = self._apphandler.get_directory_manager().get_temporary_directory("CAMSRead_",do_always_remove=False)
                self._CAMS_Files_HandlersMAP[l_sat] = CAMSFileHandler(
                    l_limaot, l_rhsampling, l_models, l_time_window, tmp_cams_dir)
                self._CAMS_Files_HandlersMAP[l_sat].initializelistofcamsfiledescription(l_ListOfCAMSDataFilenames)
                l_modelList = l_gippl2commhandler.get_value("ListOfModels").split()
                # Verify if cams file are available before anything
                if len(l_modelList) > 0 and len(l_ListOfCAMSDataFilenames) > 0:
                    # Init the cams data file handler
                    l_extinctionData = {}
                    for m in l_modelList:
                        LOGGER.debug("Extracting info for model : "+str(m))
                        l_extinctionData[m] = AthmosphericLutHandler.extract_cams_info(
                            self._apphandler.get_input_directory(), l_sat, m)
                        if l_extinctionData[m] is None:
                            raise MajaDataException("Input TOCR GIPP for model "+m+" doesn't have CAMS informations requested (Extinction_Coefs, RH_Tab, RH_dep ...)")
                    self._CAMS_Files_HandlersMAP[l_sat].set_extinction_map(l_extinctionData)
                    self._CAMS_Files_HandlersMAP[l_sat].set_rh_tab(list(l_extinctionData.values())[0]["rh_tab"])

        """ ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** **
        ** ** * END READ AND PREPARE DATAS FOR SCIENTIFICS CALCULATION ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** **
        ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** **"""

        LOGGER.info("Ending Preprocessing")

    def process_one_product(self, p_initmode, p_backwardmode, p_WriteL2ProductToL2Resolution,
                            p_write_temporary_l2_product, p_WriteHeaders, p_finalize_backward,
                            p_InputL1ImageInformationsProvider,
                            p_InputL1ImageFileReader, p_OzoneAmountValue, p_CamsStatus ,p_InputL2ImageFileReader,
                            p_OutputL2ImageFileWriter,
                            p_checking_conditional_clouds):

        # ===================================================================================================================
        # !!!!!!!!!! NOTE !!!!!!!!!
        # The parameter "p_WriteL2ProductToL2Resolution" is set in the class that calls ScientificSingleProductProcessing.
        # In this method, the L2 Product writing is managed by this parameter tacking into account the processing mode.
        #     - Init mode => this option is set tacking into account a parameter in the configuration file : EnableL2ResolutionProcessing = true or false (degraded mode in debug)
        #    - Nominal mode => this option is also set with this parameter : EnableL2ResolutionProcessing = true or false (degraded mode in debug)
        #    - Backward mode
        #        . Init => this option is forced to false
        #        . Nominal => This option is forced to false
        #        . Last product => This option is set with the "EnableL2ResolutionProcessing" parameter : true or false (degraded mode in debug) and p_finalize_backward is true
        # ===================================================================================================================

        # *************************************************************************************************************
        # ***** START SCIENTIFICS CALCULATION  ************************************************************************
        LOGGER.debug("-----------------------------------------------------------------------------------------------")
        LOGGER.debug("Starting Scientifics processing with...")
        LOGGER.debug("       *  INIT MODE                        = " + str(p_initmode))
        LOGGER.debug("       *  BACKWARD MODE                    = " + str(p_backwardmode))
        LOGGER.debug("       *  p_finalize_backward              = " + str(p_finalize_backward))
        LOGGER.debug("       *  p_WriteL2ProductToL2Resolution   = " + str(p_WriteL2ProductToL2Resolution))
        LOGGER.debug("       *  p_write_temporary_l2_product     = " + str(p_write_temporary_l2_product))
        LOGGER.debug("       *  p_WriteHeaders                   = " + str(p_WriteHeaders))
        LOGGER.debug("-----------------------------------------------------------------------------------------------")
        LOGGER.debug("-----------------------------------------------------------------------------------------------")
        LOGGER.debug(
            "L1 Product : Date " + date_utils.get_utc_from_datetime(p_InputL1ImageInformationsProvider.ProductDate))
        LOGGER.debug("L1 Product : Filename " + os.path.basename(p_InputL1ImageInformationsProvider.HeaderFilename))
        LOGGER.debug("-----------------------------------------------------------------------------------------------")

        #Store the module list to delete them afterwards
        l_module_list = []

        m_WriteL2ProductToL2Resolution = p_WriteL2ProductToL2Resolution
        if not p_WriteL2ProductToL2Resolution:
            LOGGER.info(
                "*************************************************************************************************")
            LOGGER.info("ATTENTION: L2 PROCESSING: The option to Write product to L2 resolution has been DISABLE !!!")
            LOGGER.info(
                "**************************************************************************************************")

        # ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** *
        # Define the Output L2 product directory where it will be generated
        # ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** *
        if p_write_temporary_l2_product:
            l_L2OutputDirectory = self._apphandler.get_directory_manager().get_temporary_directory("L2Products_",do_always_remove=False)
        else:
            l_L2OutputDirectory = self._apphandler.get_output_directory()

        """* Snowrate """
        l_SnowRate = 0
        """* Validity of the current LTC contribution """
        """* Usefull when cloud rate > threshold and finalizing  a backward mode(FA1407) """
        l_IgnoreCurrentLTC = False
        """* STO list of dates """
        """* Indicate if default AOT is used """
        l_IsDefaultAOT = False
        """* Indicate if the private part is copied to L2 output """
        l_CopyPrivateFromL2InputToL2Output = False
        """* global parameters directionary """
        l_global_params_dict = {}

        # ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** **
        # In NOMINAL and BACKWARD only
        # ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** **
        if p_initmode == False and p_InputL2ImageFileReader is None:
            raise MajaException("ScientificSingleProductProcessing: Internal error. For Nominal or Backward mode, the "
                                "L2 INPUT image file reader must be initialized !")

        # ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** **
        # Get a pointer of the current plugin activated
        l_CurrentPluginBase = p_InputL1ImageFileReader.get_plugin()

        # ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** **
        # Get band definitions
        l_BandDefinitons = l_CurrentPluginBase.BandsDefinitions

        # ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** **
        # Getreference of the current config admin camera activated
        l_CurrentConfigAdminCamera = l_CurrentPluginBase.ConfigAdminCamera

        # Get the satellite of the current L1 product(date D)
        l_SatelliteD = p_InputL1ImageInformationsProvider.Satellite
        l_UniqueSatelliteD = l_CurrentPluginBase.UniqueSatellite
        l_PluginName = l_CurrentPluginBase.PluginName
        LOGGER.debug("SatelliteD from the L1 product                 = " + l_SatelliteD +
                     "  (from p_InputL1ImageInformationsProvider.GetSatellite())")
        LOGGER.debug("l_UniqueSatelliteD deduces from the L1 product = " + l_UniqueSatelliteD)
        LOGGER.debug("Plugin Name                                    = " + l_PluginName)

        # Get the L2COMM
        l_GIPPL2COMMHandler = p_InputL1ImageFileReader.get_l2comm()
        """ ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** **  **
               Register the GIPP file "GIP_L2SITE" */
                ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** *"""
        l_GIP_L2SITE_Filename = gipp_utils.get_gipp_filename_with_mission(self._apphandler.get_input_directory(),
                                                                          "GIP_L2SITE",
                                                                          l_UniqueSatelliteD)
        LOGGER.info("The GIP_L2SITE file detected for the satellite '" +
                    l_UniqueSatelliteD + "' is <" + l_GIP_L2SITE_Filename + ">.")
        l_GIPPL2SITEHandler = GippL2SITEEarthExplorerXMLFileHandler(l_GIP_L2SITE_Filename)

        # ---------------------------------------------------------------------------------------------
        l_AOTMethodString = l_GIPPL2COMMHandler.get_value("AOTMethod")
        l_AOTMethod = AOTEstimation.get_aot_method(l_AOTMethodString)
        LOGGER.info("AOT Estimation - l2 processor : " + l_AOTMethodString + " method")

        # ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** **
        # START COMMON VARIABLES
        # ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** **
        # Get the maximum cloud percentage for which a L2 product is generated
        l_MaxCloudPercentage = float(l_GIPPL2COMMHandler.get_value("MaxCloudPercentage"))
        l_MaxNoDataPercentage = float(l_GIPPL2COMMHandler.get_value("MaxNoDataPercentage"))
        # Get the environment correction option
        l_EnvCorOption = xml_tools.as_bool(l_GIPPL2COMMHandler.get_value("EnvCorrOption"))
        # Get the cams data use option
        l_UseCamsData = l_GIPPL2COMMHandler.get_value_b("UseCamsData") and self._CAMS_Files_HandlersMAP[l_SatelliteD].valid
        # init cloud rate boolean(false if there are to many clouds)
        p_checking_conditional_clouds[0] = True
        # = == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == ==
        # Get the date in the string YYYYMMDD
        # = == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == ==
        # Get the reference date double
        l_JDay = date_utils.get_julianday_as_double(p_InputL1ImageInformationsProvider.ProductDate)
        LOGGER.debug("Current JDay : "+str(l_JDay))
        LOGGER.debug("Current Product Date : " )
        LOGGER.debug(p_InputL1ImageInformationsProvider.ProductDate)
        l_JDayRef = date_utils.get_julianday_as_double(
            date_utils.get_datetime_from_yyyymmdd(l_CurrentPluginBase.ReferenceDate))
        LOGGER.debug("Current JDayRef : " + str(l_JDayRef))
        l_JDay = int(math.floor(l_JDay - l_JDayRef))
        l_global_params_dict["JDay"] = l_JDay
        l_global_params_dict["JDayRef"] = l_JDayRef

        # Get STO date list since V4 - 1, disable IsBackWard because STO variable is local(no m_STOListOfStringDates.
        # So to read STO from the previsous product, read the STO header
        l_STOListOfStringDates = []
        if not p_initmode:
            # ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** **
            # Read the STO Headerfor the L2 Input image file
            # ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** *
            l_STOListOfStringDates = p_InputL2ImageFileReader.STOListOfStringDates

        nbL2inSTODates = len(l_STOListOfStringDates)
        maxSTODates = int(l_GIPPL2COMMHandler.get_value("NumberOfStackImages"))
        nbSTODates = nbL2inSTODates
        LOGGER.debug("Number of L2 in STO dates: " + str(nbL2inSTODates))
        l_STOListOfJDates = []
        # it is unlikely that the number of STO dates is greater than the maximum number of STO dates but...
        if nbL2inSTODates > maxSTODates:
            nbSTODates = maxSTODates
        # LAIG - FA - MAC - 1180 - CNES: limit the number of STO dates to the maximum number set in the GIP L2COMM
        for i in range(0, nbSTODates):
            STOJDate = date_utils.get_julianday_as_double(date_utils.get_datetime_from_yyyymmdd(l_STOListOfStringDates[i]))
            # LAIG - FA - MAC - 1180 - CNES
            #  The list of dates used for the correlation with the STO image is reversed compared to the
            # order of the band in the STO image
            # l_STOListOfJDates.insert(l_STOListOfJDates.begin(), STOJDate - l_JDayRef)
            l_STOListOfJDates.append(str(STOJDate - l_JDayRef))
            LOGGER.debug("L2inSTODate i : " + l_STOListOfStringDates[i] + " = " + str(l_STOListOfJDates[-1]))

        l_DateString = p_InputL1ImageInformationsProvider.ProductDate  # m_L1DataFilter.GetAcquisitionDate()
        # LAIG - FA - MAC - 1180 - CNES
        #  The list of STO dates should not be updated if the STO imagefile is not updated too in the"composite image"algorithm
        # It happens when an invalid L2 product(L2NOTV) is generated(too cloudy)
        #  This update was movedafter the"composite image" algorithm
        l_L2NoData = float(l_GIPPL2COMMHandler.get_value("NoData"))
        l_ReflectanceQuantificationValue = float(p_InputL1ImageInformationsProvider.ReflectanceQuantification)
        l_RealL2NoData = l_L2NoData * l_ReflectanceQuantificationValue
        l_global_params_dict["RealL2NoData"] = l_RealL2NoData

        # Store the value of the satellite, use to write headers and Product report files
        LOGGER.debug("Start getting information from the DataDEMHandler instance...")
        l_DEMDataProvider = self.DataDEMMap.get(l_UniqueSatelliteD)

        # Get the L2 resolutions in the final L2 Product
        l_ListOfL2Resolution = l_BandDefinitons.ListOfL2Resolution
        l_NumberOfL2Resolution = len(l_ListOfL2Resolution)

        # = == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == ==
        # Get the index of each band         used in L2 algorithms
        l2_processor_bands_setup.prepare_bands(l_BandDefinitons, l_BandDefinitons, l_GIPPL2COMMHandler, l_AOTMethod,
                                               l_CurrentConfigAdminCamera, p_initmode, p_InputL2ImageFileReader,
                                               l_global_params_dict)

        # = == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == ==


        # In Init mode and spectro temporal method, AOT is obtained  using the multi spectral method
        if p_initmode and l_AOTMethod == AOTEstimation.SPECTROTEMPORAL:
            l_AOTMethod = AOTEstimation.MULTISPECTRAL
            LOGGER.info("In the INIT mode, the 'SPECTROTEMPORAL' AOT method is forced to 'MULTISPECTRAL'.")


        # Check if all LookUpTable exist for the satellite of the product
        l_WaterVapourDetermination = l_CurrentPluginBase.WaterVapourDetermination
        l_GIP_L2COMM_UseDefaultConstantWaterAmount = True
        # ATTENTION: the XML pathfor GIP_L2COMM_UseDefaultConstantWaterAmount exist only if the WaterVapourDetermination
        if (l_WaterVapourDetermination):
            l_GIP_L2COMM_UseDefaultConstantWaterAmount = xml_tools.as_bool(
                l_GIPPL2COMMHandler.get_value("GIP_L2COMM_UseDefaultConstantWaterAmount"))
        LOGGER.debug("WaterVapour parameter: Determination: " + str(
            l_WaterVapourDetermination) + ", GIP_L2COMM_UseDefaultConstantWaterAmount: "
            + str(l_GIP_L2COMM_UseDefaultConstantWaterAmount))

        # = == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == ==
        #  Check the look up table
        if l_WaterVapourDetermination and l_GIP_L2COMM_UseDefaultConstantWaterAmount == False:
            LOGGER.debug(self._GIP_L2WATV_LookUpTableConverterMap)
            if l_SatelliteD not in self._GIP_L2WATV_LookUpTableConverterMap:
                raise MajaDataException(
                    "The input GIP_L2WATV file for the satellite '" + l_SatelliteD + "' is not present WATV Map.")

        if l_SatelliteD not in self._AthmosphericLutHandlerMap:
            raise MajaDataException(
                "The input Luts files for the satellite '" + l_SatelliteD + "' is not present in the Athmospheric Map.")

        # = == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == ==
        #  Common vars
        l_SnowBandAvailable = l_CurrentPluginBase.SnowMasking
        l_SwirBandAvailable = l_CurrentPluginBase.HasSwirBand
        l_CirrusBandAvailable = l_CurrentPluginBase.CirrusMasking
        l_StereoCloudAltitudeAvailable = l_CurrentPluginBase.CirrusFlag
        l_IsCirrusCorrected = False
        l_DFPMaskAvailable = l_CurrentPluginBase.DFPMasking
        l_StopLevel2Processing = False
        l_WriteL2Products = True
        # New in 4-1 : Re init the validity of the product
        self._productIsValid = True
        # Get the caching option
        caching = self._apphandler.get_user_conf().get_Computing().get_Caching().get_EnableCachingConvertReflectanceData()

        # *******************************************************
        #  Init the global input dict for processing
        l_global_params_dict["RealL2NoData"] = l_RealL2NoData
        l_global_params_dict["L2NoData"] = l_L2NoData
        l_global_params_dict["OzoneAmount"] = p_OzoneAmountValue
       # l_global_params_dict["CamsStatus"] = CAMSStatus.get_cams_status_string(p_CamsStatus)
        l_global_params_dict["InitMode"] = p_initmode
        l_global_params_dict["StoListOfDates"] = l_STOListOfJDates
        l_global_params_dict["STOListOfStringDates"] = l_STOListOfStringDates
        l_global_params_dict["SnowBandAvailable"] = l_SnowBandAvailable
        l_global_params_dict["SwirBandAvailable"] = l_SwirBandAvailable
        l_global_params_dict["DFPMaskAvailable"] = l_DFPMaskAvailable
        l_global_params_dict["CirrusBandAvailable"] = l_CirrusBandAvailable
        l_global_params_dict["CloudMaskingKnownCloudsAltitude"] = l_CurrentPluginBase.CloudMaskingKnownCloudsAltitude
        l_global_params_dict["SWIRBandAvailable"] = l_SwirBandAvailable
        l_global_params_dict["AOTMethod"] = l_AOTMethod
        l_global_params_dict["WriteL2ProductToL2Resolution"] = m_WriteL2ProductToL2Resolution
        l_global_params_dict["WriteHeaders"] = p_WriteHeaders
        l_global_params_dict["L2OutputDirectory"] = l_L2OutputDirectory
        l_global_params_dict["EnvCorOption"] = l_EnvCorOption
        l_global_params_dict["FinalizeBackWard"] = p_finalize_backward
        l_global_params_dict["BackWardMode"] = p_backwardmode

        l_global_params_dict["UseCamsData"] = l_UseCamsData
        l_global_params_dict["Caching"] = caching
        l_global_params_dict["Threads"] = self._nbThreads
        l_global_params_dict["RAM"] = self._ram
      #  l_global_params_dict["CamsStatus"] = self._cams_status
        global_input_dict = {"DEM": l_DEMDataProvider,
                             "Plugin": l_CurrentPluginBase,
                             "AppHandler": self._apphandler,
                             "L1Info": p_InputL1ImageInformationsProvider,
                             "L1Reader": p_InputL1ImageFileReader,
                             "L2Reader": p_InputL2ImageFileReader,
                             "L2COMM": l_GIPPL2COMMHandler,
                             "L2SITE": l_GIPPL2SITEHandler,
                             "L2WATV": self._GIP_L2WATV_LookUpTableConverterMap.get(l_SatelliteD),
                             "L2SMAC": self._GIP_L2SMAC_ListOfCoeffsConverterMap.get(l_SatelliteD),
                             "Params": l_global_params_dict
                             }
        if l_UseCamsData:
            global_input_dict["L2TOCR"] = self._AthmosphericLutHandlerMap.get(l_SatelliteD).get_synth_tocr()
            global_input_dict["L2DIFT"] = self._AthmosphericLutHandlerMap.get(l_SatelliteD).get_synth_dift()
            global_input_dict["L2DIRT"] = self._AthmosphericLutHandlerMap.get(l_SatelliteD).get_synth_dirt()
            global_input_dict["L2ALBD"] = self._AthmosphericLutHandlerMap.get(l_SatelliteD).get_synth_albd()
            global_input_dict["CAMS_AOT"] = self._CAMS_Files_HandlersMAP.get(l_SatelliteD).out_sum_of_aot
        else:
            l_constant_model = l_GIPPL2COMMHandler.get_value("ConstantModel")
            global_input_dict["L2TOCR"] = self._AthmosphericLutHandlerMap.get(
                l_SatelliteD).get_tocr_for_model(l_constant_model)
            global_input_dict["L2DIFT"] = self._AthmosphericLutHandlerMap.get(
                l_SatelliteD).get_dift_for_model(l_constant_model)
            global_input_dict["L2DIRT"] = self._AthmosphericLutHandlerMap.get(
                l_SatelliteD).get_dirt_for_model(l_constant_model)
            global_input_dict["L2ALBD"] = self._AthmosphericLutHandlerMap.get(
                l_SatelliteD).get_albd_for_model(l_constant_model)

        global_output_dict = {}
        # Create now the header writer to ensure it is available
        if self._apphandler.get_output_plugin() is not None:
            l_L2HeaderFileWriter = L2HeaderWriterProvider.create(self._apphandler.get_output_plugin(),
                                                                 self._apphandler)
        else:
            l_L2HeaderFileWriter = L2HeaderWriterProvider.create(MAJAPluginProvider.auto_tm(p_InputL1ImageInformationsProvider.PluginName),
                                                                 self._apphandler)
        if not l_L2HeaderFileWriter.create(p_InputL1ImageInformationsProvider.PluginName, self._apphandler):
            raise MajaDriverException("Plugin '"+self._apphandler.get_output_plugin()+"' can not write product from '"+p_InputL1ImageInformationsProvider.PluginName+"' products ")

        # *************************************************************************************************************
        #                                     CHECK THE VALIDITY OF L2_in PRODUCT IN NOMINAL MODE
        if p_initmode == False and p_backwardmode == False:
            LOGGER.info("Check the validity of L2 Product")
            validity_l2_nominal = MajaModule.create("ValidityL2Nominal")
            validity_l2_nominal.launch(global_input_dict, global_output_dict)
            self._productIsValid = global_output_dict["L2inValidity"]

        if self._productIsValid:
            # GET THE NUMBER OF NODATA PIXELS IN THE L1 EDGE MASK SUB #
            edg_stat_app = stats(p_InputL1ImageFileReader.get_value("IPEDGSubOutput"), exclude_value=1)
            l_L2NoDataNumber = edg_stat_app.getoutput().get("excludedcount")
            LOGGER.debug("Number of NoData pixels : "+str(l_L2NoDataNumber))
            global_output_dict["L2NoDataNumber"] = l_L2NoDataNumber

            # ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** *
            # START ALGORITHMS DTM PROCESSING
            #OK
            dtm_processing = MajaModule.create("DTMProcessing")
            dtm_processing.launch(global_input_dict, global_output_dict)
            l_module_list.append(dtm_processing)
            # ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** *
            # START ALGORITHMS ATHMOSPHERIC ABSORPTION
            # OK
            atmospheric_absorption = MajaModule.create("AthmosphericAbsorption")
            atmospheric_absorption.launch(global_input_dict, global_output_dict)
            l_module_list.append(atmospheric_absorption)
            # ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** *
            # START ALGORITHMS REDUCE LUT COMPUTATION
            reduce_lut_computation = MajaModule.create("ReduceLut")
            reduce_lut_computation.launch(global_input_dict, global_output_dict)
            l_module_list.append(reduce_lut_computation)
            # ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** *
            # START ALGORITHMS RAYLEIGH CORRECTION
            rayleigh_correction = MajaModule.create("RayleighCorrection")
            rayleigh_correction.launch(global_input_dict, global_output_dict)
            l_module_list.append(rayleigh_correction)
            # ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** *
            # START ALGORITHMS GEOMETRIC FLAGS
            geometric_flag_computation = MajaModule.create("GeometricFlag")
            geometric_flag_computation.launch(global_input_dict, global_output_dict)
            l_module_list.append(geometric_flag_computation)
            # ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** *
            # START CLOUDMask Undersampling
            cloud_mask_undersampling = MajaModule.create("CloudMaskUndersampling")
            cloud_mask_undersampling.launch(global_input_dict, global_output_dict)
            l_module_list.append(cloud_mask_undersampling)
            # ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** *
            # START Cirrus flag computation
            if l_StereoCloudAltitudeAvailable:
                cirrus_flag_computation = MajaModule.create("CirrusFlag")
                cirrus_flag_computation.launch(global_input_dict, global_output_dict)
                l_BoolCirrusFlag = global_output_dict["CirrusFlag"]
                l_module_list.append(cirrus_flag_computation)
            # ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** *
            # START Cirrus mask computation
            if l_CirrusBandAvailable:
                cirrus_mask_computation = MajaModule.create("CirrusMask")
                cirrus_mask_computation.launch(global_input_dict, global_output_dict)
                l_BoolCirrusFlag = global_output_dict["CirrusFlag"]
                l_module_list.append(cirrus_mask_computation)
            # ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** *
            # START Snow mask computation
            if l_SnowBandAvailable:
                snow_mask_computation = MajaModule.create("SnowMask")
                snow_mask_computation.launch(global_input_dict, global_output_dict)
                l_module_list.append(snow_mask_computation)
            # ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** *
            # START Cloud mask computation
            cloud_mask_computation = MajaModule.create("CloudMask")
            cloud_mask_computation.launch(global_input_dict, global_output_dict)
            l_module_list.append(cloud_mask_computation)
            # ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** *
            # START Cloud mask resampling
            cloud_mask_resampling = MajaModule.create("CloudMaskResampling")
            cloud_mask_resampling.launch(global_input_dict, global_output_dict)
            l_module_list.append(cloud_mask_resampling)
            # ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** *
            # START Cloud percentage computation
            validity_l2_cloud_percentage = MajaModule.create("ValidityL2Cloud")
            validity_l2_cloud_percentage.launch(global_input_dict, global_output_dict)
            l_module_list.append(validity_l2_cloud_percentage)
            l_CloudRate = global_output_dict["CloudRate"]
            LOGGER.info("Cloud Rate on the Product : " + str(l_CloudRate))
            # Check validity of the  product
            if l_CloudRate > l_MaxCloudPercentage:
                p_checking_conditional_clouds[0] = False
                if not p_backwardmode:
                    self._productIsValid = False
                    l_StopLevel2Processing = True
                    LOGGER.warning(
                        "The number of cloudy pixel is too high. The level2 algorithm processing is stopped!")
                else:
                    LOGGER.warning("This is the latest product ? -> " + str(p_finalize_backward) + ".")
                    if p_initmode:
                        self._productIsValid = False
                        if p_finalize_backward:
                            l_StopLevel2Processing = True
                            LOGGER.warning(
                                "The number of cloudy pixel is too high. The level2 algorithm processing is stopped!")
                        else:
                            LOGGER.warning(
                                "The number of cloudy pixel is too high. The product is considered as not valid !")
                    else:
                        l_IgnoreCurrentLTC = True
                        if p_finalize_backward:
                            LOGGER.warning(
                                "Last product in backward mode is cloudy. It will be considered as a valid product.")
            #Log system infos
            LOGGER.progress(self._apphandler.get_system_infos())
            if not l_StopLevel2Processing:
                if p_checking_conditional_clouds[0] or p_finalize_backward:
                    # ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** *
                    # START Water Masking computation
                    water_masking_computation = MajaModule.create("WaterMask")
                    water_masking_computation.launch(global_input_dict, global_output_dict)
                    l_module_list.append(water_masking_computation)
                    # == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == ==
                    # IF NOT BACKWARD MODE OR NOT INIT
                    if not p_backwardmode and not p_initmode:
                        # ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** *
                        # START Rain detection computation
                        rain_detection_computation = MajaModule.create("RainDetection")
                        rain_detection_computation.launch(global_input_dict, global_output_dict)
                        l_module_list.append(rain_detection_computation)
                    # ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** *
                    # START check snow percentage
                    if l_SnowBandAvailable:
                        snow_percentage_computation = MajaModule.create("SnowPercentage")
                        snow_percentage_computation.launch(global_input_dict, global_output_dict)
                        l_module_list.append(snow_percentage_computation)
                        l_SnowRate = global_output_dict["SnowRate"]
                    # == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == ==
                    # IF INIT MODE(AOT ESTIMATION) AND MULTITEMPORAL METHOD
                    #  OR IF the percentage of cloudy and snowy pixel is too high
                    # == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == ==
                    if (p_initmode and l_AOTMethod == AOTEstimation.MULTITEMPORAL) or (
                            (l_CloudRate + l_SnowRate) > l_MaxCloudPercentage):
                        LOGGER.info("Using default constant AOT :")
                        LOGGER.debug("InitMode: "+str(p_initmode))
                        LOGGER.debug("Cloud+snowpercent = "+str(l_CloudRate + l_SnowRate)+ " <> "+str(l_MaxCloudPercentage))
                        aot_estimation_constant = MajaModule.create("AOTEstimationConstant")
                        aot_estimation_constant.launch(global_input_dict, global_output_dict)
                        l_module_list.append(aot_estimation_constant)
                        l_IsDefaultAOT = True
                    else:
                        LOGGER.debug("Start AOT Estimation computation")
                        # ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** *
                        # START AOT Estimation Algorithm
                        aot_estimation_computation = MajaModule.create("AOTEstimation")
                        aot_estimation_computation.launch(global_input_dict, global_output_dict)
                        l_module_list.append(aot_estimation_computation)
                        l_IsDefaultAOT = False

                    # ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** *
                    # START CIRRUS Correction Algorithm
                    if p_WriteL2ProductToL2Resolution and l_CirrusBandAvailable and xml_tools.as_bool(
                            l_GIPPL2COMMHandler.get_value("CirrusCorrectionOption")):
                        cirrus_correction = MajaModule.create("CirrusCorrection")
                        cirrus_correction.launch(global_input_dict, global_output_dict)
                        l_module_list.append(cirrus_correction)
                    # ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** *
                    # START SCATTERING Correction Algorithm
                    scattering_correction = MajaModule.create("ScatteringCorrection")
                    scattering_correction.launch(global_input_dict, global_output_dict)
                    l_module_list.append(scattering_correction)
                    # ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** *
                    # START COMPOSITE Algorithm
                    composite_computation = MajaModule.create("Composite")
                    composite_computation.launch(global_input_dict, global_output_dict)
                    l_module_list.append(composite_computation)
                    # Log system infos
                    LOGGER.info(self._apphandler.get_system_infos())
                # =======================================================================
                # IF ENVIRONMENT CORRECTION OPTION IS TRUE AND
                # IF THE OPTION "WRITE L2 PRODUCT TO L2 RESOLUTION" IS TRUE
                # IF the percentage of cloudy pixel is not too high or
                #      if it is the last product of the backward mode
                # ========================================================================
                if l_EnvCorOption and m_WriteL2ProductToL2Resolution and \
                        ((p_checking_conditional_clouds[0]) or (p_finalize_backward)):
                    # ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** *
                    # START Environment correction algorithm
                    environment_correction = MajaModule.create("EnvironmentCorrection")
                    environment_correction.launch(global_input_dict, global_output_dict)
                    l_module_list.append(environment_correction)
                    # ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** *
                    # START Environment correction algorithm
                    slope_correction = MajaModule.create("SlopeCorrection")
                    slope_correction.launch(global_input_dict, global_output_dict)
                    l_module_list.append(slope_correction)
                    # Log system infos
                    LOGGER.info(self._apphandler.get_system_infos())
                if m_WriteL2ProductToL2Resolution:
                    # ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** *
                    # START Water vapor post processing algo
                    water_vapor_post_processing_correction = MajaModule.create("WaterVaporPostProcessing")
                    water_vapor_post_processing_correction.launch(global_input_dict, global_output_dict)
                    l_module_list.append(water_vapor_post_processing_correction)
                    # Log system infos
                    LOGGER.info(self._apphandler.get_system_infos())

                # *************************************************************************************************************
                # ***** START WRITING DATA   *********************************************
                # *************************************************************************************************************
                # Copy the private part of the L2 Product :
                #  --> only in backward mode
                #  --> if the cloud rate is too high (no L2 product generated)
                l_WriteL2Products = True
                if p_backwardmode and p_checking_conditional_clouds[0] == False:
                    # FA LAIG-FA-MAC-885-CNES : ce if etait dans le if ci-dessus. Copy
                    # uniquement si pas mode INIT (sinon segfault)
                    if p_initmode == False and p_InputL2ImageFileReader is not None:
                        LOGGER.info("Copy of the private part of the input L2 product")
                        l_CopyPrivateFromL2InputToL2Output = True
                    l_WriteL2Products = False
                    if p_finalize_backward:
                        l_WriteL2Products = True

                LOGGER.debug("Produced datas :")
                for k, v in global_output_dict.items():
                    LOGGER.debug(k + " : " + str(v))

                LOGGER.debug("global_output_dict %s", global_output_dict)
                l_global_params_dict["CheckingConditionalClouds"] = p_checking_conditional_clouds[0]

                # Setup image writer
                l2_processor_image_writer_setup.setup_l2_image_writer(self, p_OutputL2ImageFileWriter, global_input_dict,
                                                       global_output_dict,
                                                       l_WriteL2Products, l_CopyPrivateFromL2InputToL2Output,
                                                       l_IsDefaultAOT)
                l_NumberOfStackImages = len(l_STOListOfStringDates)
                # Log system infos
                LOGGER.info(self._apphandler.get_system_infos())

            # Fin l_StopLevelProcessing Faux
        # Fin l_StopLevelProcessing Faux
        # *********              S T A R T     W R I T E S    H E A D E R S    P R O C E S S I N G            *********
        p_OutputL2ImageFileWriter.set_l1_image_informations_provider(global_input_dict["L1Info"])
        p_OutputL2ImageFileWriter.set_output_directory(global_input_dict["Params"]["L2OutputDirectory"])
        p_OutputL2ImageFileWriter.initialize_product()

        LOGGER.debug("global_input_dict %s", global_input_dict)
        LOGGER.debug("Params %s", l_global_params_dict)


        # Compute NDT ratio to insure product validity (FA1395)
        # --------------------------------------------------
        if not l_StopLevel2Processing and (p_checking_conditional_clouds[0] or p_finalize_backward):
            LOGGER.debug("MaxNoDataPercentage  : " + str(l_MaxNoDataPercentage)+"%")
            validity_l2_nodata_percentage = MajaModule.create("ValidityL2NoData")
            validity_l2_nodata_percentage.launch(global_input_dict, global_output_dict)
            l_module_list.append(validity_l2_nodata_percentage)
            l_NoDataRate = global_output_dict["NoDataRate"]
            LOGGER.info("NoData Rate on the Product : " + str(l_NoDataRate))

            if l_NoDataRate > l_MaxNoDataPercentage:
                self._productIsValid = False
                p_checking_conditional_clouds = False
                # Stop the level2 processing
                LOGGER.warn("The number of NoData pixel in the output L2 composite product is too high.")

        l2_processor_header_writer_setup.setup_l2_header_writer(l_L2HeaderFileWriter, l_CurrentPluginBase, p_OutputL2ImageFileWriter, global_input_dict,
                                                global_output_dict, self._productIsValid, l_WriteL2Products,
                                                self._cams_status, l_IgnoreCurrentLTC)

        LOGGER.info("Starting Scientifics processing done.")
        LOGGER.debug("L1 Product : Date " + str(p_InputL1ImageInformationsProvider.ProductDate))
        LOGGER.debug("       *  p_finalize_backward              = " + str(p_finalize_backward))
        LOGGER.debug("       *  l_WriteL2Products                = " + str(l_WriteL2Products))
        LOGGER.debug("       *  p_WriteL2ProductToL2Resolution   = " + str(p_WriteL2ProductToL2Resolution))
        LOGGER.debug("       *  p_write_temporary_l2_product     = " + str(p_write_temporary_l2_product))
        LOGGER.debug("       *  p_WriteHeaders                   = " + str(p_WriteHeaders))
        LOGGER.debug("       *  l_DealingLTC                     = " + str(l_global_params_dict["DealingLTC"]))
        LOGGER.debug("       *  l_IgnoreCurrentLTC               = " + str(l_IgnoreCurrentLTC))
        LOGGER.debug("  ->  self._productIsValid                     = " + str(self._productIsValid))
        LOGGER.debug("  ->  p_checking_conditional_clouds        = " + str(p_checking_conditional_clouds[0]))
        LOGGER.debug("  ->  use_cams_datas                       = " + str(l_UseCamsData))
        # Log system infos
        LOGGER.progress(self._apphandler.get_system_infos())
        for mod in l_module_list:
            LOGGER.debug("Deleting module : "+mod.NAME)
            mod.cleanup()
            del(mod)

        #deleting temporary that can be deleted
        if self._apphandler.get_user_conf().get_Computing().get_EnableCleaningTemporaryDirectory():
            LOGGER.debug(self._apphandler.get_directory_manager())
            self._apphandler.get_directory_manager().clean()
            # Log system infos
            LOGGER.progress(self._apphandler.get_system_infos())

    def scientific_processing(self):
        raise NotImplementedError

    def post_processing(self):
        raise NotImplementedError

    @staticmethod
    def get_average_product_time(start_date_utc, stop_date_utc):
        l_start_jd = date_utils.get_julianday_as_double(date_utils.get_datetime_from_utc(start_date_utc))
        l_stop_jd = date_utils.get_julianday_as_double(date_utils.get_datetime_from_utc(stop_date_utc))
        return (l_start_jd + l_stop_jd) / 2.0
