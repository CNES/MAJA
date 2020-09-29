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
orchestrator.launcher.app_handler -- shortdesc

orchestrator.launcher.app_handler is a description

It defines classes_and_methods

###################################################################################################
"""


import argparse
import os
import orchestrator.common.file_utils as file_utils
from orchestrator.common.logger.maja_logging import configure_logger
from orchestrator.common.xml_tools import translate_xsl
import orchestrator.common.conf.maja_xml_admin_config_system as admin_conf
import orchestrator.common.conf.maja_xml_user_config_system as user_conf
from orchestrator.plugins.common.factory.factory_base import FactoryBase
from orchestrator.common.maja_exceptions import MajaDataException
from orchestrator.common.maja_exceptions import MajaProcessingError
from orchestrator.common.directory_manager import DirectoryManager
from orchestrator.common import version
from orchestrator.common.system_utils import memory_used_by_process2,disk_space_used
import logging,time

LOGGER = configure_logger(__name__)


class AppHandler:

    MAJA_L2_INIT_CHAIN = "MAJA_L2_INIT_CHAIN"
    MAJA_L2_NOMINAL_CHAIN = "MAJA_L2_NOMINAL_CHAIN"
    MAJA_L2_BACKWARD_CHAIN = "MAJA_L2_BACKWARD_CHAIN"
    INVALID_PROCESSOR_NAME = "INVALID"

    L2INIT = "L2INIT"
    L2NOMINAL = "L2NOMINAL"
    L2BACKWARD = "L2BACKWARD"

    DEFAULT_RAM_PER_THREAD = 2048

    def __init__(self):
        self._outputDirectory = "."
        self._inputDirectory = "."
        self._workingDirectory = "."
        if os.getenv("MAJA_INSTALL_DIR") is None:
            raise MajaProcessingError("MAJA_INSTALL_DIR is not Set")
        self._majaRootInstallDir = os.getenv("MAJA_INSTALL_DIR") + os.path.sep
        self._userconf_directory = self._majaRootInstallDir + "etc/conf/user/"
        self._adminconf_directory = self._majaRootInstallDir + "etc/conf/admin/"
        self._schemasInstallDirectoryRoot = self._majaRootInstallDir + "share/schemas" + os.path.sep
        self._schemasInstallDirectoryOfMaja = self._schemasInstallDirectoryRoot + "Maja" + os.path.sep
        self._shareConfigInstallDirectoryRoot = self._majaRootInstallDir + "share/config" + os.path.sep

        self._logLevel = ""
        self._processorName = AppHandler.L2INIT
        self._nbThreads = 4
        self._max_disc = 0
        self._adminConfigSystemFileName = ""
        self._stylesheet = None
        self._userConf = None
        self._adminConf = None
        self._output_Plugin = None
        self._tile_id = None
        self._validate_schemas = False
        self._directory_manager = DirectoryManager(self._workingDirectory)
        self._start_time = time.time()

    def get_stylesheet(self):
        return self._stylesheet

    def get_output_directory(self):
        return self._outputDirectory

    def get_input_directory(self):
        return self._inputDirectory

    def get_working_directory(self):
        return self._workingDirectory

    def get_maja_root_install_dir(self):
        return self._majaRootInstallDir

    def get_directory_manager(self):
        return self._directory_manager

    def get_schemas_root_install_dir(self):
        return self._schemasInstallDirectoryRoot

    def get_schemas_maja_install_dir(self):
        return self._schemasInstallDirectoryOfMaja

    def get_share_config_directory(self):
        return self._shareConfigInstallDirectoryRoot

    def get_loglevel(self):
        return self._logLevel

    def get_processor_name(self):
        return self._processorName

    def get_nb_threads(self):
        return self._nbThreads

    def get_admin_config_system_filename(self):
        return self._adminConfigSystemFileName

    def get_user_config_system_filename(self):
        return self._userConfigSystemFileName

    def get_user_conf(self):
        return self._userConf

    def get_admin_conf(self):
        return self._adminConf

    def get_output_plugin(self):
        return self._output_Plugin

    def get_tile_id(self):
        return self._tile_id

    def get_validate_schemas(self):
        return self._validate_schemas

    def get_system_infos(self):
        total_time =time.gmtime(time.time() - self._start_time)
        cpu_load = os.getloadavg()
        cpu_nb = os.cpu_count()
        total_disc = disk_space_used(self._workingDirectory)
        self._max_disc = max(total_disc,self._max_disc)
        return "System infos (RAM;DISK;HOUR;MIN;SEC;LOAD) : " + str(int(memory_used_by_process2(os.getpid()))) + ";" + str(
            int(self._max_disc)) + ";" + str(int(total_time.tm_hour)) + ";" + str(
            int(total_time.tm_min)) + ";" + str(int(total_time.tm_sec)) + ";" + str(int(100.0 * cpu_load[0] / self._nbThreads))

    def initialize(self):
        maja_description = """ ./maja [options] \n\n
            MAJA Chains \n\n
            CNES All rights reserved. For more details, see Copyright.txt file.\n\n
            Description: \n
            ------------\n\n
            The L2 processor offers advanced atmospheric correction algorithms including\n
            water vapour and aerosol estimates based on multitemporal data analysis.\n
            It also provides cloud mask generation.\n
                - Cloud masking\n
                    * Cloud detection\n
                    * Shadow detection\n
               - Atmospheric correction\n
                   * Gaseous absorption correction\n
                   * Scattering correction\n
               - Environment and slope correction\n
                   * Environment effects\n
                   * Slope correction\n
               - Composite image update\n
            The data and GIPPs files mandatory for MAJA are:\n
                - For L2 processing:\n
                    * GIP_L2COMM\n
                    * GIP_L2DIRT\n
                    * GIP_L2DIFT\n
                    * GIP_L2SMAC\n
                    * GIP_L2WATV\n
                    * GIP_L2TOCR\n
                    * GIP_L2ALBD\n
                    * GIP_L2SITE\n
                    * EXO_METDTA\n
                    * AUX_REFDE2\n
            Processing description:\n
            -----------------------\n
            The maja launches the following processes:\n
                - launches the pre-processing treatment\n
                    * Uncompresses all data (DBL package files and BZ2 images files)\n
                    * Check input data with the schemas\n
                    * Deletes all tarballs (if option is enable in the Configuration file)\n
                    * Applies a specific stylesheet on GIPPs files\n
                - launches the scientific-processing treatment\n
                    * Reads image products\n
                    * Applies algorithms\n
                    * Formats EE and writes datas\n
                - launches the post-processing treatment\n
                    * Check output data with the schemas\n
                    * Compress BZ2 all .TIF images data files\n
                    * Generates the .DBL image product data (L2/L3)\n\n
             For more details, report to the SUM (Software User Manual, ref. LAIG-MU-MAC-010-CS)\n\n
             Author: CS Systemes d'Information  (France)\n\n
             User cases:\n
             -----------\n
             1. First user case:  Use only a JobOrder file to launch maja processing. \n
                                  -> use the '--jobOrder' option.\n
                                  Note: the possible values for the <Processor_Name> field are:\n
                                     *  m_MAJA_L2_INIT_CHAIN \n
                                     *  m_MAJA_L2_NOMINAL_CHAIN \n
                                     *  m_MAJA_L2_BACKWARD_CHAIN \n               
             2. Second user case: Use command line parameters to launch maja processing\n
                                  -> for example, use at least the '--mode' and '--input' options\n\n
                                   
                                   
                                   """
        parser = argparse.ArgumentParser(
            description=maja_description,  # main description for help
            epilog='Beta', formatter_class=argparse.RawTextHelpFormatter)  # displayed after help

        parser.add_argument("-j", "--jobOrder", help="Specify the JobOrder file (xml file)")

        parser.add_argument(
            "-l",
            "--loglevel",
            help="Log level use and set to the JobOrder generated.",
            choices=[
                'INFO',
                'PROGRESS',
                'WARNING',
                'DEBUG',
                'ERROR'],
            default='INFO')

        parser.add_argument(
            "-m",
            "--mode",
            help="Processing mode.",
            choices=[
                'L2INIT',
                'L2NOMINAL',
                'L2BACKWARD'
                ],
            default='L2INIT',
            required=True)
        parser.add_argument(
            "-t",
            "--enableTest",
            help="Enable/Disable the field value 'Test' set in the JobOrder generated.",
            action="store_true")
        parser.add_argument(
            "--stylesheet",
            help="XML Stylesheet filename, used to overloads parameters in the XML configuration files and GIPP files. "
                 "See the [MU] for an example of StyleSheet.")
        parser.add_argument(
            "-acs",
            "--adminconf",
            help="Administration Configuration directory (contains for example the MAJAAdminConfigSystem.xml)")
        inputDescr = """Input data directory: must be contain images, all GIPPs files, the DTM, etc.).
                        The directory must be contain only one L1 product for the 'L2INIT' mode, a list of L1 products 
                        for the 'L2BACKWARD' mode, one
                        L1 product and one L2 product for the 'L2NOMINAL' mode and a list of L2 products 
                        """
        parser.add_argument("-i", "--input",
                            help=inputDescr,
                            required=False)
        parser.add_argument(
            "-o",
            "--output",
            help="Output data directory (product directory). Default value: '.'",
            required=True)
        parser.add_argument(
            "-w",
            "--workingdir",
            help="Working directory (working 'temporary' directory). Default value: the '--output parameter value'")

        plugin_choices = "Available plugins : "
        creators = FactoryBase.get_registered("PluginBase")
        for crea in creators:
            plugin_choices = plugin_choices + crea().PluginName + " , "
        parser.add_argument(
            "-p",
            "--plugin",
            help="Output plugin to use to write L2 product. "+plugin_choices)
        parser.add_argument(
            "-ucs",
            "--conf",
            help="User Configuration directory (contains for example MAJAUserConfigSystem.xml)")

        parser.add_argument(
            "--NbThreads",
            type=int,
            help="UserConfigSystem overloads value for the parameter 'NbThreads'")
        parser.add_argument(
            "--CheckXMLFilesWithSchema",
            help="UserConfigSystem overloads value for the parameter 'CheckXMLFilesWithSchema'",
            action="store_true")
        parser.add_argument(
            "--CleanInputZipFiles",
            help="UserConfigSystem overloads value for the parameter 'CleanInputZipFiles'",
            action="store_true")
        parser.add_argument(
            "--CleanFiles",
            help="UserConfigSystem overloads value for the parameter 'CleanFiles'",
            action="store_true")
        parser.add_argument(
            "--ZipFiles",
            help="UserConfigSystem overloads value for the parameter 'ZipFiles'",
            action="store_true")
        parser.add_argument(
            "--EnableCleaningCachingDirectoryBeforeProcessing",
            help="UserConfigSystem overloads value for the parameter 'EnableCleaningCachingDirectoryBeforeProcessing'",
            action="store_true")
        parser.add_argument(
            "--EnableCleaningCachingDirectoryAfterProcessing",
            help="UserConfigSystem overloads value for the parameter 'EnableCleaningCachingDirectoryAfterProcessing'",
            action="store_true")
        parser.add_argument(
            "--EnableCleaningTemporaryDirectory",
            help="UserConfigSystem overloads value for the parameter 'EnableCleaningTemporaryDirectory'",
            action="store_true")
        parser.add_argument(
            "--TileId",
            help="Set the Tile id of the Sentinel2 L1 product (Only necessary for SENTINEL2 plug-in and only for "
                 "the L1C product with PSD version < PSD 14)",
            required=False)
        parser.add_argument(
            "--perfos-log",
            help="Enable performance measurement in log",
            action="store_true")
        parser.add_argument(
            "--perfos-report",
            help="Enable synthesize performance measurement in log and xml file",
            action="store_true")
        parser.add_argument(
            "-v",
            "--version",
            help="Display version information and exit",
            action="store_true"
        )

        args = parser.parse_args()

        if args.version :
            LOGGER.info("Maja Software Version: "+version.MAJA_VERSION)
            exit(0)

        # Set the log level
        self._logLevel = args.loglevel
        os.environ["MAJA_LOGGER_LEVEL"] = args.loglevel
        if args.loglevel == "INFO":
            LOGGER.setLevel(logging.INFO)
        elif args.loglevel == "PROGRESS":
            LOGGER.setLevel(logging.PROGRESS)
        elif args.loglevel == "WARNING":
            LOGGER.setLevel(logging.WARNING)
        elif args.loglevel == "DEBUG":
            LOGGER.setLevel(logging.DEBUG)
            os.environ["OTB_LOGGER_LEVEL"] = "DEBUG"
        elif args.loglevel == "ERROR":
            LOGGER.setLevel(logging.ERROR)

        LOGGER.info("Logger in %s mode ( %s )", args.loglevel, LOGGER.getEffectiveLevel())

        if args.output is not None:
            self._outputDirectory = args.output + os.path.sep
        else:
            raise MajaDataException("Output option -o/--output must be specified")
        LOGGER.info("Output Directory : %s ", self._outputDirectory)

        if not os.path.exists(self._outputDirectory):
            LOGGER.debug("OUTPUT_DIR must be created in %s", self._outputDirectory)
            file_utils.create_directory(self._outputDirectory)

        if args.input is not None:
            self._inputDirectory = args.input + os.path.sep
        else:
            raise MajaDataException("Input option -i/--input must be specified")
        LOGGER.info("Input Directory : %s ", self._inputDirectory)

        if args.workingdir is not None:
            self._workingDirectory = args.workingdir
        else:
            self._workingDirectory = os.path.join(self._outputDirectory, ".maja-working-directory")
        LOGGER.info("Working Directory : %s ", self._workingDirectory)
        self._directory_manager = DirectoryManager(self._workingDirectory)

        if not os.path.exists(self._workingDirectory):
            LOGGER.debug("WORKING_DIR must be created in %s", self._workingDirectory)
            file_utils.create_directory(self._workingDirectory)

        if args.plugin is not None:
            self._output_Plugin = args.plugin

        if args.TileId is not None:
            self._tile_id = args.TileId

        if os.getenv("MAJA_INSTALL_DIR") is None :
            raise MajaProcessingError("MAJA_INSTALL_DIR is not Set")

        if args.stylesheet is not None:
            self._stylesheet = args.stylesheet

        # User config
        if args.conf is not None:
            self._userconf_directory = args.conf
        self.luserConfigSystemFileName = os.path.join(self._userconf_directory, "MAJAUserConfigSystem.xml")
        if not os.path.exists(self.luserConfigSystemFileName):
            raise MajaDataException(
                "The User configuration system file <" +
                self.luserConfigSystemFileName +
                "> doesn't exist!")
        # Load the file
        LOGGER.info("Using "+self.luserConfigSystemFileName+" as userConfig file")
        self._userConfigSystemFileName = file_utils.copy_file_to_directory(self.luserConfigSystemFileName, self._workingDirectory,
                                                                notestmode=True)
        if self._stylesheet is not None:
            translate_xsl(self._userConfigSystemFileName, self._stylesheet)
        self._userConf = user_conf.parse(self._userConfigSystemFileName, True)
        # overload values
        if args.CheckXMLFilesWithSchema:
            self._validate_schemas = True
        else:
            self._validate_schemas = self._userConf.get_Processing().get_CheckXMLFilesWithSchema()
        if args.CleanFiles:
            self._userConf.get_Processing().get_PostProcessing().set_CleanFiles(True)
        if args.ZipFiles:
            self._userConf.get_Processing().get_PostProcessing().set_ZipFiles(True)
        if args.CleanInputZipFiles:
            self._userConf.get_Processing().get_PreProcessing().set_CleanInputZipFiles(True)
        if args.EnableCleaningTemporaryDirectory:
            self._userConf.get_Computing().set_EnableCleaningTemporaryDirectory(True)
        if args.NbThreads is not None:
            self._nbThreads = args.NbThreads
        else:
            self._nbThreads = self._userConf.get_Computing().get_NbThreads()

        # Admin config
        if args.adminconf is not None:
            self._adminconf_directory = args.adminconf
        luseradminsystemfilename = os.path.join(self._adminconf_directory, "MAJAAdminConfigSystem.xml")
        if not os.path.exists(luseradminsystemfilename):
            raise MajaDataException("The Admin configuration system file <" + luseradminsystemfilename + "> doesn't exist!")
        self._adminConfigSystemFileName = file_utils.copy_file_to_directory(
            luseradminsystemfilename, self._workingDirectory, notestmode=True)
        if self._stylesheet is not None:
            translate_xsl(self._adminConfigSystemFileName, self._stylesheet)
        # Load the file
        self._adminConf = admin_conf.parse(self._adminConfigSystemFileName, True)

        if args.mode is not None:
            lProcessorName = args.mode
        else:
            raise MajaDataException("Mode must be specified : -m/--mode")

        self._processorName = AppHandler.MAJA_L2_NOMINAL_CHAIN
        if lProcessorName == AppHandler.L2INIT:
            self._processorName = AppHandler.MAJA_L2_INIT_CHAIN
        elif lProcessorName == AppHandler.L2NOMINAL:
            self._processorName = AppHandler.MAJA_L2_NOMINAL_CHAIN
        elif lProcessorName == AppHandler.L2BACKWARD:
            self._processorName = AppHandler.MAJA_L2_BACKWARD_CHAIN
        else:
            raise MajaDataException("Unknown mode in parameters")

        LOGGER.info("Processor is %s", self._processorName)
        LOGGER.progress("Starting " + self._processorName)

        LOGGER.info("Number of theads %i", self._nbThreads)

    def get_admin_conf_camera_filename(self, plugin_name):
        conf_file = os.path.join(self._adminconf_directory, "MAJAAdminConfig_" + plugin_name + ".xml")
        if not os.path.exists(conf_file):
            raise MajaDataException("The admin configuration file <" + conf_file +
                                    ">  (for the plugin  <" + plugin_name + ">) doesn't exist!")
        adminConfigCameraFileName = file_utils.copy_file_to_directory(conf_file, self._workingDirectory, notestmode=True)
        if self._stylesheet is not None:
            translate_xsl(adminConfigCameraFileName, self._stylesheet)
        return adminConfigCameraFileName

    def get_user_conf_camera_filename(self, plugin_name):
        conf_file = self._userconf_directory + os.path.sep + "MAJAUserConfig_" + plugin_name + ".xml"
        if not os.path.exists(conf_file):
            raise MajaDataException("The admin configuration file <" + conf_file +
                                    ">  (for the plugin  <" + plugin_name + ">) doesn't exist!")
        userConfigCameraFileName = file_utils.copy_file_to_directory(conf_file, self._workingDirectory, notestmode=True)
        if self._stylesheet is not None:
            translate_xsl(userConfigCameraFileName, self._stylesheet)
        return userConfigCameraFileName
